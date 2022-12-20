//========= Credits SADFI2259X =========//

#include "main.hpp"

/*
void RunHiddenCmdCommand(const char* command) // ex: RunHiddenCmdCommand("regedit /s file.reg");
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	//TCHAR cmd[] = command;

	BOOL res = CreateProcess(NULL,
		(LPSTR)command,
		NULL, NULL,
		FALSE, CREATE_NO_WINDOW,
		NULL, NULL,
		&si, &pi);
	if (!res)
	{
		// do error
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
*/

static int windows_title_height = 24;
static bool block_moving = false;

// forward declaration for wnd proc handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

VOID gui::CreateGuiApp(const char* windowName, const char* className)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	gui::wc = { sizeof(WNDCLASSEX), CS_CLASSDC, gui::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, className, NULL };
	::RegisterClassEx(&wc);
	hWnd = ::CreateWindow(wc.lpszClassName, windowName, WS_POPUP, ( desktop.right/2 ) - ( gui::size.x/2 ), ( desktop.bottom / 2 ) - ( gui::size.y / 2 ), gui::size.x, gui::size.y, NULL, NULL, wc.hInstance, NULL);
	
	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hWnd, &margins);

	InitializeD3D();

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
}

LRESULT WINAPI gui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	
	switch (msg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_KEYMENU) // Disable ALT application menu
			return 0L;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0L;
	case WM_LBUTTONDOWN:
		guiPosition = MAKEPOINTS(lParam);
		return 0L;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(lParam);
			auto rect = ::RECT { };
			
			GetWindowRect(hWnd, &rect);
			
			rect.left += points.x - guiPosition.x;
			rect.top += points.y - guiPosition.y;
			
			if (!block_moving && guiPosition.x >= 0 &&
				guiPosition.x <= rect.right &&
				guiPosition.y >= 0 && guiPosition.y <= windows_title_height)
				//guiPosition.y >= 0 && guiPosition.y <= gui::size.y)
			{
				SetWindowPos(
					hWnd,
					HWND_TOPMOST,
					rect.left, rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
			}
		}
		
		return 0L;
	}
	
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

bool gui::InitGui()
{
	return TRUE;
}

// TODO IMPORTANT: COPY THIS & USE DEFAULT IMGUI TO GET DECENT YES

// if (ImGui::IsItemActive()) { block_moving = true;} else { block_moving = false; } // >> use this code to stop moving the window when hovering on items like button

VOID gui::Main()
{
	SetupImGui();
	InitGui();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\trebucbd.ttf", 14);

	ImFont* title_size = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\trebucbd.ttf", 32);
	ImFont* content_font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\trebucbd.ttf", 14);

	bool loaderOpen = true;
	while (loaderOpen)
	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				loaderOpen = false;
		}

		if (!loaderOpen)
			break;

		// Start the Dear ImGui frame
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse ;

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize(gui::size);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(((windows_title_height - ImGui::CalcTextSize(GUI_TITLE).y) / 2) , ((windows_title_height - ImGui::CalcTextSize(GUI_TITLE).y) / 2)));
		ImGui::Begin(GUI_TITLE, &loaderOpen, main_window_flags);
		ImGui::PopStyleVar();
		{
			ImGui::Text("dear imgui (V%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		ImGui::EndFrame();

		device->SetRenderState(D3DRS_ZENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.f, NULL);
		if (device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			device->EndScene();
		}

		HRESULT result = device->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}

	ShutdownImGui();
}

///
///	DIRECT X
///

bool gui::CreateDeviceD3D(HWND hWnd)
{
	if ((gui::d3d9 = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return FALSE;

	// Create the D3D Device
	ZeroMemory(&gui::d3dpp, sizeof(gui::d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // vSync enabled

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device) < 0)
		return false;

	return true;
}

bool gui::InitializeD3D()
{
	if (!CreateDeviceD3D(hWnd))
	{
		// Cleanup
		CleanupDevice();

		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return TRUE;
	}
}

VOID gui::CleanupDevice()
{
	if (gui::device) { device->Release(); device = nullptr; }
	if (gui::d3d9) { d3d9->Release(); d3d9 = nullptr; }
}

VOID gui::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = device->Reset(&d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(NULL);

	ImGui_ImplDX9_CreateDeviceObjects();
}

///
/// IMGUI
/// 

VOID gui::SetupImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//ImFont* font1 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 32);

	io.IniFilename = NULL; // Remove the imgui.ini

	ImGui::StyleColorsDark(); // Apply the dark theme

	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding	= 3;
	style.WindowTitleAlign	= ImVec2(0.5f, 0.5f);
	style.ChildRounding		= 3;
	style.FrameRounding		= 3;
	style.ScrollbarSize		= 13.0;
	style.ScrollbarRounding = 1;
	style.GrabMinSize		= 8.0;
	style.GrabRounding		= 3;
	style.WindowPadding		= ImVec2(5.0f, 5.0f);
	style.ItemSpacing		= ImVec2(5.0f, 5.0f);
	style.FramePadding		= ImVec2(2.5f, 3.5f);
	style.ButtonTextAlign	= ImVec2(0.02f, 0.4f);
	style.WindowBorderSize	= 1;
	style.FrameBorderSize	= 1;

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.75f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(gui::device);
}

VOID gui::ShutdownImGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDevice();
	::DestroyWindow(hWnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

// ----------------------------------//
//     =====     MAIN     =====     //
// ---------------------------------//
int WINAPI WinMain(
	HINSTANCE hInst,
	HINSTANCE, /* unused */
	PSTR, /* unused */
	int) /* unused */
{
	gui::CreateGuiApp(WIN_TITLE, WIN_CLASS);
	gui::Main();

	return TRUE;
}