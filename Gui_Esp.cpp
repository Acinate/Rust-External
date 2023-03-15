#include "Gui.h"

// D2D
ID2D1Factory* pFactory;
ID2D1HwndRenderTarget* pRenderTarget;
ID2D1SolidColorBrush* ColorBrush;

IDWriteFactory1* pDWriteFactory;
IDWriteTextFormat* TextFormat;
IDWriteTextFormat* BigTextFormat;

D2D1_COLOR_F clear_color_D2D = { 0.0f, 0.0f, 0.0f, 0.0f };
D2D1_COLOR_F color_brush = { 0.0f, 0.0f, 0.0f, 1.0f };

// ESP var
WNDCLASS wcEsp = {};

//Forward declaration of the WndProc function
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool GuiEngine::Esp::init_window_Esp(const char* windowName, const char* className)
{
	strcpy_s(gameName, windowName);
	gameHWND = FindWindow(0, _T("Rust"));
	if (gameHWND == 0)
	{
		MessageBox(0, L"[GuiEngine ESP] Game not found", L"ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);

	wcEsp.cbClsExtra = 0;
	wcEsp.cbWndExtra = 0;
	wcEsp.hCursor = LoadCursor(0, IDC_ARROW);
	wcEsp.hIcon = LoadIcon(0, IDI_WINLOGO);
	wcEsp.lpszMenuName = 0;
	wcEsp.style = 0;
	wcEsp.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wcEsp.lpfnWndProc = GuiEngine::WndProc;
	wcEsp.hInstance = hInstance;
	wcEsp.lpszClassName = L"E5P Window";
	
	RegisterClass(&wcEsp);

	RECT rc;
	GetClientRect(gameHWND, &rc);
	cheatEspHWND = CreateWindowEx
	(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
		L"E5P Window", 
		L"Hack", 
		WS_POPUP,
		0, 
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL, 
		NULL, 
		hInstance,
		NULL
	);


	SetLayeredWindowAttributes(cheatEspHWND, RGB(0, 0, 0), 255, /*ULW_COLORKEY | */LWA_ALPHA);

	MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(cheatEspHWND, &Margin);

	if (!GuiEngine::Esp::init_render())
	{
		MessageBox(0, L"[GuiEngine ESP] init_render error", L"ERROR", MB_OK | MB_ICONERROR);
		cleanup_canvas();
		UnregisterClass(wcEsp.lpszClassName, wcEsp.hInstance);
		return 0;
	}

	ShowWindow(cheatEspHWND, SW_SHOWDEFAULT);
	UpdateWindow(cheatEspHWND);

	return 1;
}

bool GuiEngine::Esp::init_render()
{
	D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), (IUnknown**)&pDWriteFactory);
	pDWriteFactory->CreateTextFormat(L"Arial Unicode MS", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 12.f, L"", &TextFormat);
	pDWriteFactory->CreateTextFormat(L"Arial Unicode MS", NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 20.f, L"", &BigTextFormat);
	
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))
	{
		MessageBox(0, L"D2D1CreateFactory", L"ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}

	GuiEngine::Esp::create_canvas();

	return 1;
}

void GuiEngine::Esp::cleanup_canvas()
{
	if (pRenderTarget) 
	{
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
}

void GuiEngine::Esp::create_canvas()
{
	RECT rc;
	GetClientRect(cheatEspHWND, &rc);

	pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		D2D1::HwndRenderTargetProperties(cheatEspHWND, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&pRenderTarget
	);
	if (!pRenderTarget)
		MessageBox(0, L"GuiEngine create_canvas ] !pRenderTarget", L"ERROR", MB_OK | MB_ICONERROR);
	
	pRenderTarget->SetDpi(96, 96);
	pRenderTarget->CreateSolidColorBrush(color_brush, &ColorBrush);
}

void GuiEngine::Esp::begin_draw_esp()
{
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(clear_color_D2D);
}

void GuiEngine::Esp::clear_window()
{
	GuiEngine::Esp::begin_draw_esp();
	pRenderTarget->Clear(clear_color_D2D);
	GuiEngine::Esp::end_draw_esp();
}

void GuiEngine::Esp::end_draw_esp()
{
	HRESULT state = pRenderTarget->EndDraw();

	//if (state == D2DERR_RECREATE_TARGET)
	//{

	//}
}

void GuiEngine::Esp::shutdown()
{
	if (pRenderTarget) {
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
	if (pFactory) {
		pFactory->Release();
		pFactory = nullptr;
	}
	if (ColorBrush) {
		ColorBrush->Release();
		ColorBrush = nullptr;
	}
	if (pDWriteFactory) {
		pDWriteFactory->Release();
		pDWriteFactory = nullptr;
	}
	if (TextFormat) {
		TextFormat->Release();
		TextFormat = nullptr;
	}
	if (BigTextFormat) {
		BigTextFormat->Release();
		BigTextFormat = nullptr;
	}

	DestroyWindow(cheatEspHWND);
	UnregisterClass(wcEsp.lpszClassName, wcEsp.hInstance);
}

void GuiEngine::Esp::rect(float x, float y, float h, float w, const D2D1::ColorF& clr, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawRectangle(D2D1::RectF(x, y, x + h, y + w), ColorBrush, thick);
}

void GuiEngine::Esp::fill_rect(float x, float y, float w, float h, const D2D1::ColorF& clr)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->FillRectangle(D2D1::RectF(x, y, x + w, y + h), ColorBrush);
}

void GuiEngine::Esp::Line(const Vector2& start, const Vector2& end, const D2D1::ColorF& clr, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawLine({ start.x, start.y }, {end.x, end.y}, ColorBrush, thick);
}

void GuiEngine::Esp::Crosshair(const int& screenWidth, const int& screenHeight, const D2D1::ColorF& clr, const float& thick)
{
	ColorBrush->SetColor(clr);
	float xCenter = screenWidth / 2, yCenter = screenHeight / 2;
	pRenderTarget->DrawLine({ xCenter - 5, yCenter - 5 }, { xCenter + 5, yCenter + 5 }, ColorBrush, thick);
	pRenderTarget->DrawLine({ xCenter + 5, yCenter - 5 }, { xCenter - 5, yCenter + 5 }, ColorBrush, thick);
	ColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::BlueViolet));
	pRenderTarget->DrawRectangle(D2D1::RectF(xCenter, yCenter, xCenter, yCenter), ColorBrush, thick + 1.0f);
}

void GuiEngine::Esp::Circle(const Vector2& start, const D2D1::ColorF& clr, float rad, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawEllipse({ {start.x, start.y}, rad, rad }, ColorBrush, thick);
}

void GuiEngine::Esp::CenterString(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr, bool big)
{
	ColorBrush->SetColor(Clr);
	if (!big)pRenderTarget->DrawText(Str, wcslen(Str), TextFormat, { pos.x - wcslen(Str) * 5.8f / 2, pos.y - 7,FLT_MAX,FLT_MAX }, ColorBrush);
	else pRenderTarget->DrawText(Str, wcslen(Str), BigTextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, ColorBrush);
}

void GuiEngine::Esp::String(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr, bool big)
{
	ColorBrush->SetColor(Clr);
	if (!big)pRenderTarget->DrawText(Str, wcslen(Str), TextFormat, { pos.x , pos.y,FLT_MAX,FLT_MAX }, ColorBrush);
	else pRenderTarget->DrawText(Str, wcslen(Str), BigTextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, ColorBrush);
}

//WndProc function
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDc = BeginPaint(hWnd, &ps);

		FillRect(hDc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hWnd, &ps);

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}