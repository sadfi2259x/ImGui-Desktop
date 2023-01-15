#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#include "addons/imaddons/ImAddons.h"
#include "addons/imspinner.h"
#include "addons/blur/blur.hpp"

#include "font/IconsFontAwesome6.h"
#include "font/Inter_Regular.h"
#include "font/trebucbd.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")	

#include <windows.h>
#include <dwmapi.h>
#include <string> 
#include <stdio.h>
#include <tchar.h>

using namespace std;

#include "winver/WinVersion.h"
VersionInfo info;

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
	static ImVec2					size = { 512 , 430 };
}

static int windows_title_height = 40;
static int windows_sidebar_width = 40; //128
static bool block_moving = false;

#define GUI_TITLE "ImGui Menu - Designed by Rex.08"

bool test_bool1 = true;
bool test_bool2 = true;

/*
ImDrawFlags_RoundCornersTopLeft
ImDrawFlags_RoundCornersTopRight
ImDrawFlags_RoundCornersBottomRight
ImDrawFlags_RoundCornersBottomLeft
*/