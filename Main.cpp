#include "Threads.h"

// TODO: Always Day: https://www.unknowncheats.me/forum/3386277-post8902.html

int main() {
	std::cout << "Welcome to program" << std::endl;

	while (!kernelHandler.attach(L"RustClient.exe")) 
	{
		MessageBox(0, L"ERROR attach", L"ERROR", MB_OK | MB_ICONERROR);
		return 1;
	}

	while (Vars::Config::BaseNetworkable == NULL || Vars::Config::GameObjectManager == NULL)
	{
		Vars::Config::BaseNetworkable = read(kernelHandler.get_module_base(L"GameAssembly.dll") + oBaseNetworkable, DWORD64);
		Vars::Config::GameObjectManager = read(kernelHandler.get_module_base(L"GameAssembly.dll") + oGameObjectManager, DWORD64);

		Sleep(1);

		std::cout << "BaseNetworkable:" << std::hex << Vars::Config::BaseNetworkable << std::endl;
		std::cout << "GameObjectManager:" << std::hex << Vars::Config::GameObjectManager << std::endl;
	}

	std::thread Menu_thr(Menu_thread);
	Menu_thr.detach();
	std::thread BN_thr(BN_thread);
	BN_thr.detach();
	std::thread GOM_thr(GOM_thread);
	GOM_thr.detach();
	std::thread ESP_thr(ESP_thread);
	ESP_thr.detach();
	std::thread aim_thr(Aim_thread);
	aim_thr.detach();
	std::thread Hack_thr(Hack_thread);
	Hack_thr.detach();

	system("pause");
	return 0;
}