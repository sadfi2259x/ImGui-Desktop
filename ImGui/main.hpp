#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#include "addons/imaddons/ImAddons.h"
#include "addons/imspinner.h"
#include "addons/blur/blur.hpp"

#include "font/IconsFontAwesome6.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")	
#pragma comment(lib, "ntdll")

#include <windows.h>
#include <dwmapi.h>
#include <string> 
#include <stdio.h>
#include <tchar.h>

using namespace std;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Forward declare message handler from imgui_impl_win32.cpp

namespace gui
{

	// Data
	static LPDIRECT3D9              g_pD3D = NULL;
	static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
	static D3DPRESENT_PARAMETERS    g_d3dpp = {};

	static POINTS					guiPosition = { };
	static ImVec2					size = { 500 , 300 };
}

static int windows_title_height = 24;
static bool block_moving = false;

#define GUI_TITLE "ImGui Menu"

/*
ImDrawFlags_RoundCornersTopLeft
ImDrawFlags_RoundCornersTopRight
ImDrawFlags_RoundCornersBottomRight
ImDrawFlags_RoundCornersBottomLeft
*/

struct VersionInfo
{
	VersionInfo() : Major(0), Minor(0), BuildNum(0) {}
	unsigned int Major;
	unsigned int Minor;
	unsigned int BuildNum;
};
VersionInfo info;

class WinVersion
{
public:
	static bool GetVersion(VersionInfo& info);
	static bool IsBuildNumGreaterOrEqual(unsigned int buildNumber);
};

extern "C" NTSTATUS __stdcall RtlGetVersion(OSVERSIONINFOEXW * lpVersionInformation);

bool WinVersion::GetVersion(VersionInfo& info)
{
    OSVERSIONINFOEXW osv;
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    if (RtlGetVersion(&osv) == 0)
    {
        info.Major = osv.dwMajorVersion;
        info.Minor = osv.dwMinorVersion;
        info.BuildNum = osv.dwBuildNumber;
        if (osv.dwBuildNumber >= 22000)
            info.Major = 11;
        return true;
    }
    return false;
}

bool WinVersion::IsBuildNumGreaterOrEqual(unsigned int buildNumber)
{
    VersionInfo info;
    if (GetVersion(info))
    {
        return (buildNumber >= info.BuildNum);
    }
    return false;
}