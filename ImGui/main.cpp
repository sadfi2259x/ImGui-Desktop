//========================================== ImGui Application ==========================================//
//
//  Dear ImGui: standalone example application for DirectX 9 - Borderless
//  If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
//  Read online: https://github.com/ocornut/imgui/tree/master/docs
// 
//=======================================================================================================//

#include "main.hpp"

bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(gui::g_pd3dDevice, filename, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

// Main code
int main(int, char**)
{
    // Get monitor resolution
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);

    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Window", NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, L"Dear ImGui DirectX9 - Borderless", WS_POPUP, (desktop.right / 2) - (gui::size.x / 2), (desktop.bottom / 2) - (gui::size.y / 2), gui::size.x, gui::size.y, NULL, NULL, wc.hInstance, NULL);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return TRUE;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; 

    io.IniFilename = NULL; // Remove the imgui.ini

    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 6;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ItemSpacing = ImVec2(5.0f, 5.0f);
    style.ScrollbarSize = 13.0;
    style.ScrollbarRounding = 1;
    style.GrabMinSize = 8.0;
    style.GrabRounding = 3;
    style.WindowPadding = ImVec2(5.0f, 5.0f);
    style.FramePadding = ImVec2(2.5f, 3.5f);
    style.ButtonTextAlign = ImVec2(0.02f, 0.4f);
    style.WindowBorderSize = 1;
    style.FrameBorderSize = 1;

    style.Colors[ImGuiCol_WindowBg]     = ImVec4(0.06f, 0.06f, 0.06f, 0.75f);
    style.Colors[ImGuiCol_ChildBg]      = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(gui::g_pd3dDevice);

    io.Fonts->AddFontDefault();
    
    /*
    static const ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_16_MD, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_MD, 14.0f, &icons_config, icons_ranges);

    int my_image_width = 0;
    int my_image_height = 0;
    PDIRECT3DTEXTURE9 my_texture = NULL;
    bool ret = LoadTextureFromFile("texture/bg.png", &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
    */

    // Main loop
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
        ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize(gui::size);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(((windows_title_height - ImGui::CalcTextSize(GUI_TITLE).y) / 2) , ((windows_title_height - ImGui::CalcTextSize(GUI_TITLE).y) / 2)));
		ImGui::Begin(GUI_TITLE, &loaderOpen, main_window_flags);
		ImGui::PopStyleVar();
        ImVec2 pos = {
            ImGui::GetCursorScreenPos().x - ImGui::GetStyle().WindowPadding.x
            ,
            ImGui::GetCursorScreenPos().y - ImGui::GetStyle().WindowPadding.y
        };
        //ImGui::GetBackgroundDrawList()->AddImageRounded((void*)my_texture, ImVec2(pos.x, pos.y - windows_title_height), ImVec2(pos.x + gui::size.x, pos.y + gui::size.y - windows_title_height), ImVec2(0, 0), ImVec2(1, 1), ImU32(IM_COL32(225, 225, 225, 225)), ImGui::GetStyle().WindowRounding+2);
        {
			ImGui::Text("dear imgui (V%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
		ImGui::End();

        //ImGui::ShowDemoWindow();
        // Rendering
        ImGui::EndFrame();
        gui::g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        gui::g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        gui::g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

        gui::g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
        if (gui::g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            gui::g_pd3dDevice->EndScene();
        }
        HRESULT result = gui::g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && gui::g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((gui::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&gui::g_d3dpp, sizeof(gui::g_d3dpp));
    gui::g_d3dpp.Windowed = TRUE;
    gui::g_d3dpp.BackBufferCount = 1;
    gui::g_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    gui::g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; //D3DFMT_X8R8G8B8 , D3DFMT_A8R8G8B8
    gui::g_d3dpp.hDeviceWindow = hWnd;
    gui::g_d3dpp.EnableAutoDepthStencil = TRUE;
    gui::g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    gui::g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // D3DPRESENT_INTERVAL_ONE

    if (gui::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &gui::g_d3dpp, &gui::g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (gui::g_pd3dDevice) { gui::g_pd3dDevice->Release(); gui::g_pd3dDevice = NULL; }
    if (gui::g_pD3D) { gui::g_pD3D->Release(); gui::g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = gui::g_pd3dDevice->Reset(&gui::g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(NULL);
    ImGui_ImplDX9_CreateDeviceObjects();
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		gui::guiPosition = MAKEPOINTS(lParam);
		return 0L;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(lParam);
			auto rect = ::RECT { };
			
			GetWindowRect(hWnd, &rect);
			
			rect.left += points.x - gui::guiPosition.x;
			rect.top += points.y - gui::guiPosition.y;
			
			if (!block_moving && gui::guiPosition.x >= 0 &&
                gui::guiPosition.x >= 0 && gui::guiPosition.x <= (gui::size.x - (windows_title_height + (ImGui::GetStyle().WindowBorderSize * 2)) ) &&
                gui::guiPosition.y >= 0 && gui::guiPosition.y <= windows_title_height)
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