#include "Gui.h"

//D3D
ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

//Imgui
ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

bool vsync = 1;
WNDCLASS wcMenu = {};


void imgui_init_style()
{

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGuiStyle* style = &ImGui::GetStyle();
	auto font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 15.0f);
	io.FontGlobalScale = 1.1;
	//ImGui::PushFont(font);


	style->Alpha = 1.f;
	style->WindowRounding = 0.7f;
	style->FrameRounding = 4.0f;

	style->WindowBorderSize = 1.8f;
	style->ChildBorderSize = 1.8f;

	style->ItemInnerSpacing = ImVec2(4, 4);
	style->ItemSpacing = ImVec2(8, 4);


	style->ScrollbarSize = 10.f;
	style->ScrollbarRounding = 5.f;

	style->WindowMinSize = ImVec2(425, 360);


	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImColor(100, 100, 100, 255);
	colors[ImGuiCol_ChildBg] = ImColor(100, 100, 100, 255);
	colors[ImGuiCol_PopupBg] = ImColor(100, 100, 100, 255);

	colors[ImGuiCol_Border] = ImColor(0, 0, 0, 255);
	colors[ImGuiCol_FrameBg] = ImColor(100, 149, 237, 255);
	//colors[ImGuiCol_FrameBgActive] = ImColor(139, 0, 139, 255);
	//colors[ImGuiCol_TitleBg] = ImColor(139, 0, 139, 255);
	colors[ImGuiCol_CheckMark] = ImColor(100, 100, 100, 255);


	//colors[ImGuiCol_FrameBgHovered] = ImColor(255, 0, 0, 255);

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls



	io.IniFilename = nullptr;
	io.LogFilename = nullptr;


	return;
}


bool GuiEngine::Menu::init_window_Menu(const char* windowsName, const char* className)
{
	//ImGui_ImplWin32_EnableDpiAwareness();

	strcpy_s(gameName, windowsName);
	gameHWND = FindWindow(0, _T("Rust"));
	if (gameHWND == NULL)
	{
		MessageBox(0, L"[GuiEngine MENU] Game not found", L"ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);

	//wcMenu = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, GuiEngine::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)),NULL, (LPCWSTR)randomStrGen(10), NULL };
	wcMenu.cbClsExtra = 0;
	wcMenu.cbWndExtra = 0;
	wcMenu.hCursor = LoadCursor(0, IDC_ARROW);
	wcMenu.hIcon = LoadIcon(0, IDI_WINLOGO);
	wcMenu.lpszMenuName = 0;
	wcMenu.style = 0;
	wcMenu.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wcMenu.lpfnWndProc = GuiEngine::WndProc;
	wcMenu.hInstance = hInstance;
	wcMenu.lpszClassName = L"M3NU Window";
	
	RegisterClass(&wcMenu);

	RECT rc;
	GetClientRect(gameHWND, &rc);
	//cheatMenuHWND = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, wcMenu.lpszClassName, wcMenu.lpszMenuName, WS_POPUP, 1, 1, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0);  // |  WS_EX_CLIENTEDGE| WS_EX_TRANSPARENT 
	cheatMenuHWND = CreateWindowEx
	(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		L"M3NU Window",
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

	SetLayeredWindowAttributes(cheatMenuHWND, RGB(0, 0, 0), 0, /*LWA_ALPHA | */LWA_COLORKEY);

	MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(cheatMenuHWND, &Margin);

	ShowWindow(cheatMenuHWND, SW_SHOWDEFAULT);
	UpdateWindow(cheatMenuHWND);

	if (!create_device_D3D(cheatMenuHWND))
	{
		MessageBox(0, L"[ GuiEngine Menu ] create_device_D3D error", L"ERROR", MB_OK | MB_ICONERROR);
		cleanup_device_D3D();
		UnregisterClass(wcMenu.lpszClassName, wcMenu.hInstance);
		return 0;

	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();

	imgui_init_style();

	ImGui_ImplWin32_Init(cheatMenuHWND);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	return 1;
}

bool GuiEngine::Menu::create_device_D3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT hrs;
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
	{
		return false;
	}

	GuiEngine::Menu::create_render_target();
	return true;
}
void GuiEngine::Menu::cleanup_device_D3D()
{
	cleanup_render_target();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void GuiEngine::Menu::create_render_target()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}
void GuiEngine::Menu::cleanup_render_target()
{
	if (g_mainRenderTargetView)
	{
		g_mainRenderTargetView->Release();
		g_mainRenderTargetView = NULL;
	}

}

void GuiEngine::Menu::begin_draw_menu()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GuiEngine::Menu::end_draw_menu()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present((UINT)vsync, 0); // Present with vsync
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
}
void GuiEngine::Menu::clear_window()
{
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
	g_pSwapChain->Present((UINT)vsync, 0); // Present with vsync
}

void GuiEngine::Menu::shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	GuiEngine::Menu::cleanup_device_D3D();
	GuiEngine::Menu::cleanup_render_target();
	DestroyWindow(cheatMenuHWND);
	UnregisterClass(wcMenu.lpszClassName, wcMenu.hInstance);

}
