#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

#include <dwmapi.h>
#include <dwrite_1.h>
#include <d3d11.h>
#include <d2d1.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <tchar.h>


#include "math.h"
#include "tools.h"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

extern HWND gameHWND, cheatMenuHWND, cheatEspHWND;
extern char gameName[256];


extern ID3D11Device* g_pd3dDevice;
extern IDXGISwapChain* g_pSwapChain;

extern ID2D1HwndRenderTarget* pRenderTarget;

namespace GuiEngine
{
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	namespace Menu
	{
		bool init_window_Menu(const char* windowsName, const char* className);

		bool create_device_D3D(HWND hWnd);
		void cleanup_device_D3D();

		void create_render_target();
		void cleanup_render_target();

		void begin_draw_menu();
		void end_draw_menu();
		void clear_window();

		void shutdown();
	}

	namespace Esp
	{
		bool init_window_Esp(const char* windowsName, const char* className);

		bool init_render();
		void create_canvas();
		void cleanup_canvas();

		void begin_draw_esp();
		void end_draw_esp();
		void clear_window();

		void shutdown();

		//primitives
		void rect(float x, float y, float h, float w, const D2D1::ColorF& Clr, float Thick = 1.5f);
		void fill_rect(float x, float y, float w, float h, const D2D1::ColorF& Clr);
		void Line(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick = 1.5f);
		void Circle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f);
		void CenterString(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false);
		void String(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false);
		void Crosshair(const int& screenWidth, const int& screenHeight, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), const float& thick = 2);
	}
};

#define update_cheat_windows_size_and_move(Game,Cheat)\
{\
	RECT winRect;\
	RECT clientRect;\
	GetWindowRect(Game, &winRect);\
	GetClientRect(Game, &clientRect);\
	int clientHeight, clientWidth, topBorder, sideBorder;\
	clientHeight = clientRect.bottom - clientRect.top;\
	clientWidth = clientRect.right - clientRect.left;\
	sideBorder = (winRect.right - winRect.left - clientWidth) / 2;\
	topBorder = (winRect.bottom - winRect.top - clientHeight) - sideBorder;\
	MoveWindow(Cheat, winRect.left + sideBorder, winRect.top + topBorder, clientWidth, clientHeight, true);\
}