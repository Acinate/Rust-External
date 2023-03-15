#include "threads.h"
#include "custom_elements.h"

extern HWND gameHWND, cheatMenuHWND, cheatEspHWND;

void update_cheat_windows_size(HWND GameHWND, int* Width, int* Height)
{
	RECT clientRect;
	GetClientRect(GameHWND, &clientRect);
	*Width = clientRect.right - clientRect.left;
	*Height = clientRect.bottom - clientRect.top;
}

void Menu_thread()
{
	while (!FindWindow(L"UnityWndClass", L"Rust")) Sleep(1000);
	if (!GuiEngine::Menu::init_window_Menu("Rust", "UnityWndClass"))
	{
		Vars::Config::panic = true;
		return;
	}

	c_gui gui;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && !Vars::Config::panic)
	{
		update_cheat_windows_size(gameHWND, &Vars::Config::ScreenWidth, &Vars::Config::ScreenHeight);
		update_cheat_windows_size_and_move(gameHWND, cheatMenuHWND);

		if (PeekMessage(&msg, cheatMenuHWND, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		// TODO: Remove
		bool isWait = 0;
		if (isWait)
		{

		}

		if (GetAsyncKeyState(VK_INSERT) & 0x8000)
		{
			Vars::Config::MenuActive = !Vars::Config::MenuActive;
			if (Vars::Config::MenuActive)
			{
				ShowWindow(cheatMenuHWND, SW_SHOWDEFAULT);
				SetForegroundWindow(cheatMenuHWND);
				SetActiveWindow(cheatMenuHWND);
			}
			else
			{
				ShowWindow(cheatMenuHWND, SW_HIDE);
				SetForegroundWindow(gameHWND);
				SetActiveWindow(gameHWND);
			}
			Sleep(400);
		}

		if (Vars::Config::MenuActive)
		{
			ShowWindow(cheatMenuHWND, SW_SHOWDEFAULT);
		}
		else
		{
			ShowWindow(cheatMenuHWND, SW_HIDE);
		}

		if (Vars::Config::MenuActive && Vars::Config::LocalPlayerIsValid && (GetForegroundWindow() == gameHWND) || (GetForegroundWindow() == cheatMenuHWND))
		{
			GuiEngine::Menu::begin_draw_menu();
			ImGui::Begin("Asddd", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

			static int MenuTab = 0;
			static int ESPTab = 0;

			ImGui::BeginGroup();
			{
				if (gui.tab("A1M", MenuTab == 0, ImVec2(136, 20)))		MenuTab = 0;
				ImGui::SameLine(0, 0);
				if (gui.tab("E5P", MenuTab == 1, ImVec2(136, 20)))		MenuTab = 1;
				ImGui::SameLine(0, 0);
				if (gui.tab("M15C", MenuTab == 2, ImVec2(136, 20)))		MenuTab = 2;
			}
			ImGui::EndGroup();

			ImGui::BeginGroup();
			{
				switch (MenuTab)
				{
				case 0:
					ImGui::Checkbox("Draw FOV", (bool*)&Vars::Aim::drawFov);
					if (Vars::Aim::drawFov) ImGui::SliderFloat("Fov", &Vars::Aim::fov, 10.f, 50.f, "%1.f", ImGuiSliderFlags_NoInput);

					ImGui::Checkbox("41M [UNSAF3]", (bool*)&Vars::Aim::aim);
					if (Vars::Aim::aim)
					{
						ImGui::Checkbox("Random bone hit [L3G1T 41M]", (bool*)&Vars::Aim::randomBone);
						ImGui::SliderFloat("Smooth", &Vars::Aim::smooth, 0.f, 2.f, "%0.05f", ImGuiSliderFlags_NoInput);
						ImGui::SetCursorPos({ ImGui::GetCursorPos().x + 5, ImGui::GetCursorPos().y - 23 });
						ImGui::Text("RAG3");
						ImGui::SetCursorPos({ ImGui::GetCursorPos().x + 239, ImGui::GetCursorPos().y - 20 });
						ImGui::Text("L3G1T");
						ImGui::Spacing();

						Vars::Esp::playerEsp = true;
					}

					ImGui::Text("R3CO1L");
					ImGui::SliderFloat(" ", &Vars::Aim::recoil, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_NoInput);

					ImGui::Checkbox("F4T BULL3T [UNSAF3] (CR4SH)", (bool*)&Vars::Aim::ModifyBullet);
					if (Vars::Aim::ModifyBullet)
					{
						ImGui::SliderFloat("Fatness", &Vars::Aim::fat, 0.1f, 1.5f, "%.1f", ImGuiSliderFlags_NoInput);
					}

					ImGui::Checkbox("N0 5PR3AD [UNSAF3] (CR4SH)", (bool*)&Vars::Aim::NoSpread);

					ImGui::Checkbox("N0 SW4Y", (bool*)&Vars::Aim::NoSway);
					break;
				case 1:
					ImGui::BeginGroup();
					{
						if (gui.tab("3NT1TY", ESPTab == 0, ImVec2(136, 20))) ESPTab = 0;
						ImGui::SameLine(0, 0);
						if (gui.tab("L00T", ESPTab == 1, ImVec2(136, 20))) ESPTab = 1;
						ImGui::SameLine(0, 0);
						if (gui.tab("E.T.C", ESPTab == 2, ImVec2(136, 20))) ESPTab = 2;
					}
					ImGui::EndGroup();

					switch (ESPTab)
					{
					case 0:
						ImGui::Checkbox("Player esp", (bool*)&Vars::Esp::playerEsp);
						if (Vars::Esp::playerEsp)
						{
							gui.move_item({ 10, 0 });
							ImGui::Checkbox("Draw boxes", (bool*)&Vars::Esp::drawBox);
							gui.move_item({ 10, 0 });
							ImGui::Checkbox("Draw health bar", (bool*)&Vars::Esp::drawHealthBar);
							gui.move_item({ 10, 0 });
							ImGui::Checkbox("Draw active weapon", (bool*)&Vars::Esp::drawActiveWeapons);
							gui.move_item({ 10, 0 });
							ImGui::Checkbox("Draw skeleton [LOW FPS]", (bool*)&Vars::Esp::drawSkeleton);
							gui.move_item({ 10, 0 });
							ImGui::Checkbox("Corpse", (bool*)&Vars::Esp::playerCorpse);
							gui.move_item({ 10, 0 });
							ImGui::Checkbox("Sleepers", (bool*)&Vars::Esp::playerSleepers);
						}
						else Vars::Esp::playerCorpse = Vars::Esp::playerSleepers = false;

						ImGui::Checkbox("Npc esp", (bool*)&Vars::Esp::npcEsp);
						break;
					case 1:
						ImGui::Checkbox("stone", (bool*)&Vars::Esp::stone);
						ImGui::Checkbox("sulphur", (bool*)&Vars::Esp::sulphur);
						ImGui::Checkbox("metal", (bool*)&Vars::Esp::metal);
						ImGui::Checkbox("hemp", (bool*)&Vars::Esp::hemp);
						ImGui::Checkbox("drop items", (bool*)&Vars::Esp::dropItems);
						ImGui::Checkbox("airdrop", (bool*)&Vars::Esp::airdrop);
						break;
					case 2:
						ImGui::Text("Traps");
						gui.move_item({ 10, 0 });
						ImGui::Checkbox("landAirTurret", (bool*)&Vars::Esp::landAirTurret);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("autoTurret", (bool*)&Vars::Esp::autoTurret);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("guntrap", (bool*)&Vars::Esp::guntrap);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("flameturret", (bool*)&Vars::Esp::flameturret);

						ImGui::Text("Buildings");
						gui.move_item({ 10,0 });
						ImGui::Checkbox("cupboard", (bool*)&Vars::Esp::cupboard);
						gui.move_item({ 10,0 });
						if (Vars::Esp::cupboard)
						{
							gui.move_item({ 10,0 });
							ImGui::Checkbox("Show autorized player", (bool*)&Vars::Esp::cupboardPlayer);
							gui.move_item({ 10,0 });
						}
						else Vars::Esp::cupboardPlayer = false;
						ImGui::Checkbox("large Box", (bool*)&Vars::Esp::largeBox);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("small Box", (bool*)&Vars::Esp::smallBox);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("stash", (bool*)&Vars::Esp::stash);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("sleeping bag", (bool*)&Vars::Esp::sleepingbag);

						ImGui::SetCursorPos({ 190, 57 });
						ImGui::BeginChild("Vehicle", ImVec2(200, 150));
						{
							ImGui::Text("Vehicle");
							gui.move_item({ 10,0 });
							ImGui::Checkbox("minicopter", (bool*)&Vars::Esp::minicopter);
							gui.move_item({ 10,0 });
							ImGui::Checkbox("begemot", (bool*)&Vars::Esp::cow);
							gui.move_item({ 10,0 });
							ImGui::Checkbox("boats", (bool*)&Vars::Esp::boat);
							gui.move_item({ 10,0 });
							ImGui::Checkbox("big boats", (bool*)&Vars::Esp::bigBoat);
						}
						ImGui::EndChild();
						break;
					}
					break;
				case 2:
					ImGui::SetCursorPos({ 10, 35 });
					ImGui::BeginChild("Vehicle", ImVec2(225, 200));
					{
						ImGui::Text("Movement");
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Walk Watter", (bool*)&Vars::Misc::walkWatter);	if (Vars::Misc::walkWatter) { Vars::Misc::HigthJump = false; }
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Spider", (bool*)&Vars::Misc::spider);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("HigthJump", (bool*)&Vars::Misc::HigthJump);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Sp33dhack", (bool*)&Vars::Misc::speedhack);
						if (Vars::Misc::speedhack)
						{
							gui.move_item({ 10,0 });
							ImGui::SliderFloat("speed+", &Vars::Misc::speedBonus, 0.f, 10.0f, "%0.005f", ImGuiSliderFlags_NoInput);
						}
						gui.move_item({ 10,0 });
						ImGui::Checkbox("WatterBoost", (bool*)&Vars::Misc::WatterBoost);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Wall Walk", (bool*)&Vars::Misc::WallWalk);
					}
					ImGui::EndChild();


					ImGui::SetCursorPos({ 225,35 });
					ImGui::BeginChild("Other", ImVec2(225, 200));
					{
						ImGui::Text("Other");
						gui.move_item({ 10,0 });
						ImGui::Checkbox("always day", (bool*)&Vars::Misc::alwaysDay);// Vars::Misc::alwaysDay = false;
						if (Vars::Misc::alwaysDay)
						{
							gui.move_item({ 10,0 });
							ImGui::SliderFloat("time", &Vars::Misc::alwaysDay_float, 0.f, 24.f, "%1.f", ImGuiSliderFlags_NoInput);
						}
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Super melee[MAY BE CRASH]", (bool*)&Vars::Misc::superMelee);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Super eoka[MAY BE CRASH]", (bool*)&Vars::Misc::superEoka);
						gui.move_item({ 10,0 });
						ImGui::Checkbox("ThirdPerson mode", (bool*)&Vars::Misc::ThirdPersonMode); //Vars::Misc::ThirdPersonMode = false;
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Admin mode", (bool*)&Vars::Misc::AdminMode);// Vars::Misc::AdminMode = false;
						gui.move_item({ 10,0 });
						ImGui::Checkbox("Crosshair", (bool*)&Vars::Misc::crosshair);
						gui.move_item({ 10,0 });
					}
					ImGui::EndChild();
					ImVec2 winSize = ImGui::GetWindowSize();
					ImGui::SetCursorPos({ winSize.x - 70,winSize.y - 30 });

					ImGui::Checkbox("Panic", (bool*)&Vars::Config::panic);

					break;
				}
			}
			ImGui::EndGroup();

			ImGui::End();
			GuiEngine::Menu::end_draw_menu();
		}
		else if (!Vars::Config::LocalPlayerIsValid)
		{
			if (Vars::Config::MenuActive)
			{
				ShowWindow(cheatMenuHWND, SW_HIDE);
				GuiEngine::Menu::clear_window();

			}
		}
		Sleep(1);
	}

	GuiEngine::Menu::shutdown();
}

void GOM_thread()
{
	while (!Vars::Config::panic)
	{
		if (Vars::Config::LocalPlayerIsValid && (!myLocalPlayer.BaseEntityCamera || !myLocalPlayer.TodCycle) && FindWindow(L"UnityWndClass", L"Rust"))
		{
			//Beep(500, 200);
			//Beep(500, 200);
			GameObjectManager_loop(Vars::Config::GameObjectManager);
		}
		Sleep(5'000);
	}
}

void BN_thread()
{
	while (!Vars::Config::panic)
	{
		if (!FindWindow(L"UnityWndClass", L"Rust"))
		{
			std::cout << "Could not find Rust" << std::endl;
			Vars::Config::LocalPlayerIsValid = false;
			Vars::Config::MenuActive = false;
			Vars::Config::panic = true;
			break;
		}
		BaseNetworkable_loop(Vars::Config::BaseNetworkable);
		Sleep(5000);
	}
}

void ESP_thread()
{
	while (!FindWindow(L"UnityWndClass", L"Rust")) Sleep(1000);
	if (!GuiEngine::Esp::init_window_Esp("Rust", "UnityWndClass"))
	{
		Vars::Config::panic = true;
		return;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && !Vars::Config::panic)
	{
		if (!FindWindow(L"UnityWndClass", L"Rust")) continue;

		update_cheat_windows_size_and_move(gameHWND, cheatEspHWND);

		if (PeekMessage(&msg, cheatEspHWND, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		if (GetForegroundWindow() == gameHWND || GetForegroundWindow() == cheatMenuHWND)
		{
			GuiEngine::Esp::begin_draw_esp();

			frame_rate(&Vars::Config::espFPS);

			GuiEngine::Esp::String({ 10, 10 }, (L"UP base:" + std::to_wstring(Vars::Config::GameObjectManager)).c_str());
			GuiEngine::Esp::String({ 10, 25 }, (L"GA base:" + std::to_wstring(Vars::Config::BaseNetworkable)).c_str());
			GuiEngine::Esp::String({ 10, 40 }, (L"FPS:" + std::to_wstring(Vars::Config::espFPS)).c_str());

			esp_drawner();

			GuiEngine::Esp::end_draw_esp();
		}
		else
		{
			GuiEngine::Esp::clear_window();
		}

		Sleep(1);
	}

	GuiEngine::Esp::shutdown();
}

void Aim_thread()
{
	while (!Vars::Config::panic && FindWindow(L"UnityWndClass", L"Rust"))
	{
		if (Vars::Config::LocalPlayerIsValid)
		{
			myLocalPlayer.set_active_weapon();

			if (myLocalPlayer.myActiveWeapon.get_addr())
			{
				if (Vars::Aim::aim) Aim(Vars::Aim::addr_BasePlayer_on_Aimming);

				//myLocalPlayer.SetBA(Vector2(0.0f, 0.0f));
				//myLocalPlayer.SetRA(Vector2(50.0f, 50.0f));

				DWORD64 Held = read(myLocalPlayer.myActiveWeapon.get_addr() + oHeldEntity, DWORD64);

				DWORD64 Magazine = NULL;
				if (Held && read(Held + oRecoil, DWORD64) && (Magazine = read(Held + 0x2a8, DWORD64)) && (read(Magazine + 0x1c, DWORD64) >= 2))
				{
					//cout << "firearms" << endl;
					if (Vars::Aim::ModifyBullet)myLocalPlayer.myActiveWeapon.fatbullet();
					if (Vars::Aim::NoSpread)myLocalPlayer.myActiveWeapon.NoSpread();

					static bool trigger_no_recoil = false;
					if (Vars::Aim::recoil != 1.0f)
					{
						myLocalPlayer.myActiveWeapon.no_recoil(Vars::Aim::recoil);
						trigger_no_recoil = true;
					}
					else if (trigger_no_recoil == true)
					{
						myLocalPlayer.myActiveWeapon.no_recoil(Vars::Aim::recoil);
						trigger_no_recoil = false;
					}
				}
				else
				{
					if (read(Held + 0x48, DWORD) == 2176761593)//eoka
					{
						if (Vars::Misc::superEoka)myLocalPlayer.myActiveWeapon.super_eoka();
					}
					else if (read(Held + 0x2A8, float) > 0.f)//BaseMelee
					{
						if (Vars::Misc::superMelee)myLocalPlayer.myActiveWeapon.super_melee();
					}
				}
			}
		}
		Sleep(1);
	}
}

void Hack_thread()
{
	while (!Vars::Config::panic)
	{
		Hacks();
		Sleep(1);
	}
}