//========================================== ImGui Application ==========================================//
//
//  Dear ImGui: standalone example application for DirectX 9 - Borderless
//  If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
//  Read online: https://github.com/ocornut/imgui/tree/master/docs
// 
//=======================================================================================================//

#include "main.hpp"

bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture)
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
    return true;
}

void RenderWindowBlur(HWND hwnd, bool boolean)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary(L"user32.dll");
    if (hm && boolean)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 0 }; // and even works 4,0,155,0 (Acrylic blur)
            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}

ImFont* AddCustomDefaultFont(const ImFontConfig* font_cfg_template)
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (!font_cfg_template)
    {
        font_cfg.OversampleH = font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
    }
    if (font_cfg.SizePixels <= 0.0f)
        font_cfg.SizePixels = 13.0f * 1.0f;
    if (font_cfg.Name[0] == '\0')
        ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "ProggyClean.ttf, %dpx", (int)font_cfg.SizePixels);
    font_cfg.EllipsisChar = (ImWchar)0x0085;
    font_cfg.GlyphOffset.y = 1.0f * IM_FLOOR(font_cfg.SizePixels / 13.0f);  // Add +1 offset per 13 units

    //const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
    const ImWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : io.Fonts->GetGlyphRangesDefault();
    //ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, trebucbd_compressed_size, font_cfg.SizePixels, &font_cfg, glyph_ranges);
    ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, trebucbd_compressed_size, 14, nullptr, glyph_ranges);
    return font;
}

int nav_num = 0;

// Main code
int main(int, char**)
{
    // Get monitor resolution
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);

    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"OrkGui Window", NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, L"OrkGui", WS_POPUP, (desktop.right / 2) - (gui::size.x / 2), (desktop.bottom / 2) - (gui::size.y / 2), gui::size.x, gui::size.y, NULL, NULL, wc.hInstance, NULL);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    RenderWindowBlur(hwnd, true);

    //SetWindowRgn(hwnd, CreateRoundRectRgn(0, 0, gui::size.x+1, gui::size.y+1, 6, 6), true);

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

    style.WindowRounding = 4;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ItemSpacing = ImVec2(5.0f, 5.0f);
    style.ScrollbarSize = 13.f;
    style.ScrollbarRounding = 1;
    style.GrabMinSize = 8.0;
    style.GrabRounding = 3;
    style.WindowPadding = ImVec2(5.0f, 5.0f);
    style.FramePadding = ImVec2(2.5f, 3.5f);
    style.ButtonTextAlign = ImVec2(0.5f, 0.6f);
    style.WindowBorderSize = 0;
    style.FrameBorderSize = 0;

    style.Colors[ImGuiCol_WindowBg]                 = ImVec4(0.06f, 0.06f, 0.06f, 0.75f);
    style.Colors[ImGuiCol_ChildBg]          	    = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    style.Colors[ImGuiCol_Text]                     = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]             = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]                  = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border]                   = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]                  = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered]           = ImVec4(0.45f, 0.45f, 0.45f, 0.85f);
    style.Colors[ImGuiCol_FrameBgActive]            = ImVec4(0.63f, 0.63f, 0.63f, 0.63f);
    style.Colors[ImGuiCol_TitleBg]                  = ImVec4(0.13f, 0.13f, 0.13f, 0.99f);
    style.Colors[ImGuiCol_TitleBgActive]            = ImVec4(0.13f, 0.13f, 0.13f, 0.99f);
    style.Colors[ImGuiCol_TitleBgCollapsed]         = ImVec4(0.05f, 0.05f, 0.05f, 0.79f);
    style.Colors[ImGuiCol_MenuBarBg]                = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]              = ImVec4(0.13f, 0.13f, 0.13f, 0.99f);
    style.Colors[ImGuiCol_ScrollbarGrab]            = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]     = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]      = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]                = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]               = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]         = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_Button]                   = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    style.Colors[ImGuiCol_ButtonHovered]            = ImVec4(0.34f, 0.34f, 0.35f, 0.89f);
    style.Colors[ImGuiCol_ButtonActive]             = ImVec4(0.21f, 0.21f, 0.21f, 0.81f);
    style.Colors[ImGuiCol_Header]                   = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    style.Colors[ImGuiCol_HeaderHovered]            = ImVec4(0.34f, 0.34f, 0.35f, 0.89f);
    style.Colors[ImGuiCol_HeaderActive]             = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    style.Colors[ImGuiCol_Separator]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]               = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]         = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_PlotLines]                = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]         = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]            = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]     = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]           = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(gui::g_pd3dDevice);

    AddCustomDefaultFont(nullptr);

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    ImFont* FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
    ImFont* FontAwesomeBig = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 19.f, nullptr, icons_ranges);

    ImFont* TitleFont = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, trebucbd_compressed_size, 30, nullptr, io.Fonts->GetGlyphRangesDefault());

    /*
    PDIRECT3DTEXTURE9 bg_texture = NULL;
    bool ret = LoadTextureFromFile("bg.png", &bg_texture);
    IM_ASSERT(ret);
    */

    // Main loop
    bool loaderOpen = true;
    if (WinVersion::GetVersion(info) && info.Major <= 6)
    {
        MessageBox(hwnd, L"your operating system is not supported", L"Error", MB_ICONERROR);
    }
    else while ( loaderOpen && (info.Major > 6) )
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
        ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize(gui::size);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(((windows_title_height - ImGui::CalcTextSize(GUI_TITLE).y) / 2) , ((windows_title_height - ImGui::CalcTextSize(GUI_TITLE).y) / 2)));
		ImGui::Begin(GUI_TITLE, &loaderOpen, main_window_flags);
		ImGui::PopStyleVar();
        {
            auto BackgroundDrawList = ImGui::GetBackgroundDrawList();
            auto ForegroundDrawList = ImGui::GetForegroundDrawList();
            auto WindowDrawList = ImGui::GetWindowDrawList();
            auto WindowSize = ImGui::GetWindowSize();
            auto WindowPos = ImGui::GetWindowPos();
            auto CursorPos = ImGui::GetCursorPos();
            //BackgroundDrawList->AddImageRounded((void*)bg_texture, ImVec2(WindowPos.x, WindowPos.y + windows_title_height), ImVec2(WindowPos.x + gui::size.x, WindowPos.y + gui::size.y), ImVec2(0, 0), ImVec2(1, 1), ImU32(IM_COL32(225, 225, 225, 225)), ImGui::GetStyle().WindowRounding*(4/3), ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight);
            //BackgroundDrawList->AddRectFilled(WindowPos, ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + windows_title_height), IM_COL32(13, 13, 12, 255), style.WindowRounding, ImDrawFlags_RoundCornersTop); // Title
            //BackgroundDrawList->AddRectFilled(ImVec2(WindowPos.x, WindowPos.y + windows_title_height), ImVec2(WindowPos.x + windows_sidebar_width, WindowPos.y + WindowSize.y), IM_COL32(13, 13, 12, 255), style.WindowRounding, ImDrawFlags_RoundCornersBottomLeft); // sidebar
            BackgroundDrawList->AddRectFilled(WindowPos, ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y), IM_COL32(13, 13, 12, 255/2), style.WindowRounding); // MainBg
            BackgroundDrawList->AddRectFilled(ImVec2(WindowPos.x + windows_sidebar_width, WindowPos.y + windows_title_height), ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y), IM_COL32(25, 24, 24, 255), style.WindowRounding, ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopLeft); // Main
            ImGui::SetCursorPos(ImVec2(WindowPos.x + style.ItemSpacing.x, WindowPos.y + style.ItemSpacing.y));
            {
                ImGui::PushFont(TitleFont);
                ImGui::Text(" " GUI_TITLE);
                ImGui::PopFont();
                ImGui::SameLine(WindowSize.x - (windows_title_height - style.WindowPadding.x));
                {
                    ImGui::PushFont(FontAwesomeBig);
                    if (ImGui::Button(ICON_FA_XMARK, ImVec2(windows_title_height - style.WindowPadding.x * 2, windows_title_height - style.WindowPadding.x * 2)))
                        loaderOpen = false;
                    ImGui::PopFont();
                }
            }
            ImGui::SetCursorPosY(windows_title_height + style.ItemSpacing.y);
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.f);
                ImGui::BeginChild("sidebar", ImVec2(windows_title_height - style.WindowPadding.x*2, 0), false, ImGuiWindowFlags_NoBackground);
                {
                    auto nav_btn_size = ImVec2(windows_title_height - style.WindowPadding.x * 2, windows_title_height - style.WindowPadding.x * 2);
                    ImAdd::NavigationRadio(ICON_FA_HOUSE, nav_btn_size, 0, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_USER, nav_btn_size, 1, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_EYE, nav_btn_size, 2, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_CROSSHAIRS, nav_btn_size, 3, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_CAR, nav_btn_size, 4, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_GAMEPAD, nav_btn_size, 5, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_SERVER, nav_btn_size, 6, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_DATABASE, nav_btn_size, 7, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_DOWNLOAD, nav_btn_size, 8, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_SHIELD_HALVED, nav_btn_size, 9, &nav_num);
                    ImAdd::NavigationRadio(ICON_FA_GEAR, nav_btn_size, 10, &nav_num);
                }
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }
            ImGui::SetCursorPos(ImVec2(WindowPos.x + windows_sidebar_width + style.WindowPadding.x, WindowPos.y + windows_title_height + style.WindowPadding.y));
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.f);
                ImGui::BeginChild("content", ImVec2(0, 0), false, ImGuiWindowFlags_NoBackground);
                {
                    if (nav_num == 0)
                    {
                        ImGui::Text("hello world !");
                        ImGui::Button("button", ImVec2(-0.1, 0));
                        ImAdd::ToggleButton("toggle1", &test_bool1, true);
                        ImAdd::ToggleButton("toggle2", &test_bool2, false);
                        ImAdd::TextURL("google link", ICON_FA_LINK "  Open in browser\n\"https:://www.google.com\"", L"https:://www.google.com", true);
                        ImAdd::TextURL("google link", ICON_FA_LINK "  Open in browser\n\"https:://www.google.com\"", L"https:://www.google.com", false);
                    }
                    else if (nav_num == 1)
                    {

                    }
                    else if (nav_num == 2)
                    {

                    }
                    else if (nav_num == 3)
                    {

                    }
                    else if (nav_num == 4)
                    {

                    }
                    else if (nav_num == 5)
                    {

                    }
                    else if (nav_num == 6)
                    {

                    }
                    else if (nav_num == 7)
                    {

                    }
                    else if (nav_num == 8)
                    {

                    }
                    else if (nav_num == 9)
                    {

                    }
                    else if (nav_num == 10)
                    {

                    }
                    else
                    {
                        ImGui::Text("404");
                    }

                }
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }
        }
		ImGui::End();
        //ImGui::ShowDemoWindow();
        // 
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