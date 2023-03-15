#include "Loops.h"

void GameObjectManager_loop(DWORD64 ObjMgr)
{
	// UPDATE: Use Camera Manager (https://www.unknowncheats.me/forum/3387611-post8913.html)
	DWORD64 MainCamera = read(ObjMgr, DWORD64);
	DWORD64 Camera1 = read(ObjMgr + 0xB8, DWORD64);
	DWORD64 Camera2 = read(Camera1, DWORD64);
	DWORD64 Camera3 = read(Camera2 + 0x10, DWORD64);
	myLocalPlayer.BaseEntityCamera = Camera3;
}

//void GameObjectManager_loop(DWORD64 ObjMgr)
//{
//	for (DWORD64 BaseObject = read(ObjMgr + 0x8, DWORD64); BaseObject && BaseObject != read(ObjMgr, DWORD64); BaseObject = read(BaseObject + 0x8, DWORD64))
//	{
//		DWORD64 GameObject = read(BaseObject + 0x10, DWORD64);
//		WORD Tag = read(GameObject + 0x54, WORD);
//
//		DWORD64 ObjectClass = read(GameObject + 0x30, DWORD64);
//		DWORD64	Entity = read(ObjectClass + 0x18, DWORD64);
//
//
//		if (Tag == 6)
//		{
//			//std::cout <<6 << std::endl;
//			//cout << "entity" << endl;
//		}
//
//		//camera
//		else if (Tag == 5)
//		{
//			myLocalPlayer.ObjectClassCamera = ObjectClass;
//			myLocalPlayer.BaseEntityCamera = Entity;
//		}
//
//		//sky
//		else if (Tag == 20011)
//		{
//			DWORD64 Dome = read(Entity + 0x28, DWORD64);
//			myLocalPlayer.TodCycle = read(Dome + 0x38, DWORD64);
//
//		}
//	}
//}

static bool localPlayerIsExist = false;
static std::vector<objInf> draw_entities = {};
static std::mutex drawnerMtx;
void BaseNetworkable_loop(DWORD64 BaseNetworkable)
{
	// TODO: Replace with GameAssembly.dll + BaseEntity_TypeInfo (https://www.unknowncheats.me/forum/rust/164256-rust-reversal-structs-offsets-466.html) (https://www.unknowncheats.me/forum/3407562-post9015.html)
	// (https://www.unknowncheats.me/forum/3420720-post9090.html);
	DWORD64 EntityRealm = read(BaseNetworkable + 0xB8, DWORD64);
	DWORD64 ClientEntities = read(EntityRealm, DWORD64);
	DWORD64 ClientEntities_list = read(ClientEntities + 0x10, DWORD64);
	DWORD64 ClientEntities_values = read(ClientEntities_list + 0x28, DWORD64);

	if (!ClientEntities_values) return;
	int EntityCount = read(ClientEntities_values + 0x10, int);

	DWORD64 EntityBuffer = read(ClientEntities_values + 0x18, DWORD64);

	localPlayerIsExist = false;
	std::vector<objInf> BN_draw_entities = {};
	for (int i = 0; i < EntityCount; i++)
	{
		DWORD64 Entity = read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);
		if (Entity <= 100000) continue;

		DWORD64 Object = read(Entity + 0x10, DWORD64);
		if (Object <= 100000) continue;

		DWORD64 ObjectClass = read(Object + 0x30, DWORD64);
		if (ObjectClass <= 100000) continue;

		if (read(ObjectClass + 0x54, int16_t) == 6)
		{
			//std::cout << "Object:" << Object << std::endl;
		}

		DWORD64 addr_name = read(ObjectClass + 0x60, DWORD64);

		static char BNname[110] = { '\0' };
		kernelHandler.read_memory(addr_name, (uint64_t)BNname, sizeof(BNname));
		BNname[109] = { '\0' };

		objInf currentObj;
		currentObj.objAddr = Object;
		currentObj.prefabNumber = NULL;

		// local player
		if (strstr(BNname, "LocalPlayer"))
		{
			localPlayerIsExist = true;
			myLocalPlayer.set_addr(read(Object + 0x28, DWORD64));
		}

		// entities
		else if (Vars::Esp::playerEsp && strstr(BNname, "player.prefab") && !strstr(BNname, "prop") && !strstr(BNname, "corpse"))
		{
			currentObj.prefabNumber = ObjList::player;
		}
		else if (Vars::Esp::playerCorpse && strstr(BNname, "player_corpse.prefab"))
		{
			currentObj.prefabNumber = ObjList::corpse;
		} 
		else if (Vars::Esp::playerCorpse && strstr(BNname, "item_drop_backpack.prefab"))
		{
			currentObj.prefabNumber = ObjList::backpack;
		}
		else if (Vars::Esp::npcEsp && (strstr(BNname, "scientist") && !strstr(BNname, "corpse")))
		{
			currentObj.prefabNumber = ObjList::npc;
		}
		else if (Vars::Esp::heliEsp && (strstr(BNname, "patrolhelicoptermarker.prefab")))
		{
			currentObj.prefabNumber = ObjList::patrol_heli;
		}
		else if (Vars::Esp::bradleyEsp && (strstr(BNname, "bradleyapc.prefab")))
		{
			currentObj.prefabNumber = ObjList::bradley;
		}


		// loot
		// TODO: Scan loot every 10 seconds
		else if (Vars::Esp::wood && strstr(BNname, "wood-collectable.prefab"))
		{
			currentObj.prefabNumber = ObjList::wood;
		}
		else if (Vars::Esp::stone && strstr(BNname, "stone-ore.prefab"))
		{
			currentObj.prefabNumber = ObjList::stone;
		}
		else if (Vars::Esp::sulphur && strstr(BNname, "sulfur-ore.prefab"))
		{
			currentObj.prefabNumber = ObjList::sulfur;
		}
		else if (Vars::Esp::metal && strstr(BNname, "metal-ore.prefab"))
		{
			currentObj.prefabNumber = ObjList::metal;
		}
		else if (Vars::Esp::hemp && strstr(BNname, "hemp-collectable.prefab"))
		{
			currentObj.prefabNumber = ObjList::hemp;
		}
		else if (Vars::Esp::dropItems && strstr(BNname, "(world)"))
		{
			currentObj.prefabNumber = ObjList::world;
		}
		else if (Vars::Esp::mushroom && strstr(BNname, "mushroom-cluster"))
		{
			currentObj.prefabNumber = ObjList::mushroom;
		}
		else if (Vars::Esp::junkpile && strstr(BNname, "junkpile"))
		{
			currentObj.prefabNumber = ObjList::junkpile;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "crate_normal.prefab"))
		{
			currentObj.prefabNumber = ObjList::military_crate;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "crate_elite.prefab"))
		{
			currentObj.prefabNumber = ObjList::elite_crate;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "heli_crate.prefab"))
		{
			currentObj.prefabNumber = ObjList::heli_crate;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "codelockedhackablecrate.prefab"))
		{
			currentObj.prefabNumber = ObjList::locked_crate;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "bradley_crate.prefab"))
		{
			currentObj.prefabNumber = ObjList::bradley_crate;
		}
		else if (Vars::Esp::lootcrate && (strstr(BNname, "foodbox") || strstr(BNname, "crate_food")))
		{
			currentObj.prefabNumber = ObjList::foodbox;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "crate"))
		{
			//std::cout << "Object:" << BNname << std::endl;
			currentObj.prefabNumber = ObjList::lootcrate;
		}

		// etc
		else if (Vars::Esp::airdrop && strstr(BNname, "supply_drop.prefab"))
		{
			currentObj.prefabNumber = ObjList::supply;
		}
		else if (Vars::Esp::landAirTurret && strstr(BNname, "sam_site_turret_deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::sam_site_turret;
		}
		else if (Vars::Esp::autoTurret && strstr(BNname, "autoturret_deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::autoturret;
		}
		else if (Vars::Esp::guntrap && strstr(BNname, "guntrap.deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::guntrap;
		}
		else if (Vars::Esp::flameturret && strstr(BNname, "flameturret.deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::flameturret;
		}

		// building
		else if (Vars::Esp::cupboard && strstr(BNname, "cupboard.tool.deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::cupboard;
		}
		else if (Vars::Esp::largeBox && strstr(BNname, "box.wooden.large.prefab"))
		{
			currentObj.prefabNumber = ObjList::box_wooden_large;
		}
		else if (Vars::Esp::smallBox && strstr(BNname, "woodbox_deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::woodbox_deployed;
		}
		else if (Vars::Esp::stash && strstr(BNname, "small_stash_deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::small_stash;
		}
		else if (Vars::Esp::sleepingbag && strstr(BNname, "sleepingbag_leather_deployed.prefab"))
		{
			currentObj.prefabNumber = ObjList::sleepingbag;
		}

		// transports
		else if (Vars::Esp::minicopter && strstr(BNname, "minicopter.entity.prefab"))
		{
			currentObj.prefabNumber = ObjList::minicopter;
		}
		else if (Vars::Esp::cow && strstr(BNname, "scraptransporthelicopter.prefab"))
		{
			currentObj.prefabNumber = ObjList::scraptransporthelicopter;
		}
		else if (Vars::Esp::boat && strstr(BNname, "rowboat.prefab"))
		{
			currentObj.prefabNumber = ObjList::rowboat;
		}
		else if (Vars::Esp::bigBoat && strstr(BNname, "rhib.prefab"))
		{
			currentObj.prefabNumber = ObjList::rhib;
		}

		// xmas event
		else if (Vars::Esp::lootcrate && strstr(BNname, "presentdrop.prefab"))
		{
			currentObj.prefabNumber = ObjList::xmas_crate;
		}
		else if (Vars::Esp::lootcrate && strstr(BNname, "xmastunnellootbox.prefab"))
		{
			currentObj.prefabNumber = ObjList::xmas_crate;
		}
		//else if (Vars::Esp::npcEsp && (strstr(BNname, "xmasdwelling_c.prefab")))
		//{
		//	currentObj.prefabNumber = ObjList::npc;
		//}
		else if (Vars::Esp::npcEsp && (strstr(BNname, "/npc/gingerbread")))
		{
			currentObj.prefabNumber = ObjList::npc;
		}

		if (currentObj.prefabNumber != NULL) 
		{
			BN_draw_entities.push_back(currentObj);
		}



		else 
		{
			if (!strstr(BNname, "building") && !strstr(BNname, "deployable"))
			{
				std::cout << "Object:" << BNname << std::endl;
			}
		}

		drawnerMtx.lock();
		draw_entities.clear();
		draw_entities = BN_draw_entities;
		drawnerMtx.unlock();

		Vars::Config::LocalPlayerIsValid = localPlayerIsExist;
		if (!localPlayerIsExist)
		{
			myLocalPlayer.set_addr(NULL);
			myLocalPlayer.BaseEntityCamera = NULL;
			myLocalPlayer.TodCycle = NULL;
		}
	}
}

void esp_drawner()
{
	if (Vars::Config::LocalPlayerIsValid && myLocalPlayer.update_view_matrix()) 
	{
		GuiEngine::Esp::String({ 10, 55 }, (L"Entity draw count:" + std::to_wstring(draw_entities.size())).c_str());

		drawnerMtx.lock();

		for (int i = 0; i < draw_entities.size(); i++)
		{
			DWORD64 ObjectClass = read(draw_entities[i].objAddr + 0x30, DWORD64);
			if (ObjectClass <= 100000) continue;

			// entities
			if (Vars::Esp::playerEsp && (draw_entities[i].prefabNumber == (byte)ObjList::player))
			{
				static BasePlayer Player;
				Player.set_addr(read(draw_entities[i].objAddr + 0x28, DWORD64));
				if (!read(Player.get_addr() + oPlayerModel, DWORD64)) continue;
				if (Player.IsDead())continue;

				if (Vars::Aim::aim
					&& !Vars::Aim::addr_BasePlayer_on_Aimming
					&& Player.IsVisible()
					&& !Player.HasFlags(Sleeping)
					&& InFov(Player, head))
				{
					Vars::Aim::addr_BasePlayer_on_Aimming = Player.get_addr();
				}

				DrawEsp::player_esp(Player, myLocalPlayer, Player.GetName());
			}
			else if (Vars::Esp::playerCorpse && (draw_entities[i].prefabNumber == (byte)ObjList::corpse || draw_entities[i].prefabNumber == (byte)ObjList::backpack))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				if (!gameObject) continue;
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				if (!Trans) continue;
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos;
				if (myLocalPlayer.WorldToScreen(pos, &Pos))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos, (L"corpse [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
				}
			}
			else if (Vars::Esp::npcEsp && (draw_entities[i].prefabNumber == (byte)ObjList::npc))
			{
				static BasePlayer Player;
				Player.set_addr(read(draw_entities[i].objAddr + 0x28, DWORD64));
				if (!read(Player.get_addr() + oPlayerModel, DWORD64)) continue;
				if (Player.IsDead())continue;

				if (Vars::Aim::aim
					&& !Vars::Aim::addr_BasePlayer_on_Aimming
					&& Player.IsVisible()
					&& !Player.HasFlags(Sleeping)
					&& InFov(Player, head))
				{
					Vars::Aim::addr_BasePlayer_on_Aimming = Player.get_addr();
				}

				DrawEsp::player_esp(Player, myLocalPlayer, L"npc", true);
			}
			else if (Vars::Esp::heliEsp && (draw_entities[i].prefabNumber == (byte)ObjList::patrol_heli))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				if (!gameObject) continue;
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				if (!Trans) continue;
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos;
				if (myLocalPlayer.WorldToScreen(pos, &Pos))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos, (L"heli [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
				}
			}
			else if (Vars::Esp::bradleyEsp && (draw_entities[i].prefabNumber == (byte)ObjList::bradley))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				if (!gameObject) continue;
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				if (!Trans) continue;
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos;
				if (myLocalPlayer.WorldToScreen(pos, &Pos))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos, (L"bradley [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Red);
				}
			}

			// loot
			else if (Vars::Esp::stone && (draw_entities[i].prefabNumber == (byte)ObjList::stone))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"stone [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::White);
				}
			}
			else if (Vars::Esp::sulphur && (draw_entities[i].prefabNumber == (byte)ObjList::sulfur))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"sulfur [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::DarkGoldenrod);
				}
			}
			else if (Vars::Esp::metal && (draw_entities[i].prefabNumber == (byte)ObjList::metal))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"metal [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::SandyBrown);
				}
			}
			else if (Vars::Esp::hemp && (draw_entities[i].prefabNumber == (byte)ObjList::hemp))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"hemp [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Green);
				}
			}
			else if (Vars::Esp::mushroom && (draw_entities[i].prefabNumber == (byte)ObjList::mushroom))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"mushroom [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::LightGreen);
				}
			}
			else if (Vars::Esp::junkpile && (draw_entities[i].prefabNumber == (byte)ObjList::junkpile))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"junkpile [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Violet);
				}
			}
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::military_crate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"mil-crate [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Purple);
				}
			}
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::elite_crate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"elite-crate [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::OrangeRed);
				}
			}
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::heli_crate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"heli-crate [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::OrangeRed);
				}
			}
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::bradley_crate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"brad-crate [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::OrangeRed);
				}
			}
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::locked_crate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"locked-crate [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::OrangeRed);
				}
			}
			else if (Vars::Esp::foodbox && (draw_entities[i].prefabNumber == (byte)ObjList::foodbox))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"foodbox [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::LightGreen);
				}
			}
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::lootcrate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"lootcrate [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Beige);
				}
			}
			else if (Vars::Esp::dropItems && (draw_entities[i].prefabNumber == (byte)ObjList::world))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					std::wstring name;
					DWORD64 Entity = read(draw_entities[i].objAddr + 0x28, DWORD64);
					DWORD64 Item = read(Entity + 0x150, DWORD64);
					DWORD64 ItemDefinition = read(Item + 0x20, DWORD64);
					DWORD64 TranslatePhrase = read(ItemDefinition + 0x28, DWORD64);
					DWORD64 addr_name = read(TranslatePhrase + 0x18, DWORD64);
					_UncStr str = read(addr_name, _UncStr);
					for (int i = 0; i < str.len && i < 20; i++)
					{
						name += str.str[i];
					}

					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (name + L"[" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Violet);
				}
			}
			else if (Vars::Esp::airdrop && (draw_entities[i].prefabNumber == (byte)ObjList::supply))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"supply-drop [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Purple);
				}
			}
			else if (Vars::Esp::autoTurret && (draw_entities[i].prefabNumber == (byte)ObjList::autoturret))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"auto-turrent [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::guntrap && (draw_entities[i].prefabNumber == (byte)ObjList::guntrap))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"gun-trap [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::flameturret && (draw_entities[i].prefabNumber == (byte)ObjList::flameturret))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"flame-turret [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::cupboard && (draw_entities[i].prefabNumber == (byte)ObjList::cupboard))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					if (Vars::Esp::cupboardPlayer && DrawEsp::InFov(Pos1))
					{
						DWORD64 Entity = read(draw_entities[i].objAddr + 0x28, DWORD64);
						DWORD64 Authorized = read(Entity + 0x580, DWORD64);
						DWORD countPlayer = read(Authorized + 0x18, DWORD);
						if (countPlayer > 0)
						{
							DWORD64 items = read(Authorized + 0x10, DWORD64);

							float h = 10.0f, w = 10.0f;
							countPlayer > 0 ? h = countPlayer * 15 : h;

							for (int i = 0; i < countPlayer; i++)
							{
								DWORD64 item = read(items + 0x20 + (i * 0x8), DWORD64);
								DWORD64 string_struct = read(item + 0x18, DWORD64);
								_UncStr structure = read(string_struct, _UncStr);
								if (structure.len * 10 > w) w = structure.len * 10;
							}

							GuiEngine::Esp::fill_rect(Pos1.x - w / 2, Pos1.y - h, w, h, D2D1::ColorF(D2D1::ColorF::Black, 0.5f));
							for (int i = 0; i < countPlayer; i++)
							{
								DWORD64 item = read(items + 0x20 + (i * 0x8), DWORD64);
								DWORD64 string_struct = read(item + 0x18, DWORD64);
								_UncStr structure = read(string_struct, _UncStr);
								std::wstring name = {};
								for (int i = 0; i < structure.len && i < 20; i++)
								{
									name += structure.str[i];
								}

								GuiEngine::Esp::CenterString({ Pos1.x, Pos1.y - (15 + i * 15) + 5 }, name.c_str());
							}
						}
						else
						{
							GuiEngine::Esp::fill_rect(Pos1.x - 20, Pos1.y - 8, 45, 18, D2D1::ColorF(D2D1::ColorF::Black, 0.5f));
							GuiEngine::Esp::CenterString(Pos1, L"Empty", D2D1::ColorF::White);
						}
					}
					else
					{
						static short dist;
						dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
						GuiEngine::Esp::CenterString(Pos1, (L"cupboard [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
					}
				}
			}
			else if (Vars::Esp::largeBox && (draw_entities[i].prefabNumber == (byte)ObjList::box_wooden_large))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"large box [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::smallBox && (draw_entities[i].prefabNumber == (byte)ObjList::woodbox_deployed))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"small box [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::stash && (draw_entities[i].prefabNumber == (byte)ObjList::small_stash))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"small stash [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::sleepingbag && (draw_entities[i].prefabNumber == (byte)ObjList::sleepingbag))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"sleeping bag [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Blue);
				}
			}
			else if (Vars::Esp::minicopter && (draw_entities[i].prefabNumber == (byte)ObjList::minicopter))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"minicopter [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}
			else if (Vars::Esp::cow && (draw_entities[i].prefabNumber == (byte)ObjList::scraptransporthelicopter))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"scrappy [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}
			else if (Vars::Esp::boat && (draw_entities[i].prefabNumber == (byte)ObjList::rowboat))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"boat [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}
			else if (Vars::Esp::boat && (draw_entities[i].prefabNumber == (byte)ObjList::rhib))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"big boat [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::Aqua);
				}
			}

			// xmas event
			else if (Vars::Esp::lootcrate && (draw_entities[i].prefabNumber == (byte)ObjList::xmas_crate))
			{
				DWORD64 gameObject = read(ObjectClass + 0x30, DWORD64);
				DWORD64 Trans = read(gameObject + 0x8, DWORD64);
				DWORD64 Vec = read(Trans + 0x38, DWORD64);
				Vector3 pos = read(Vec + 0x90, Vector3);

				Vector2 Pos1;
				if (myLocalPlayer.WorldToScreen(pos, &Pos1))
				{
					static short dist;
					dist = Math::Calc3D_Dist(pos, myLocalPlayer.GetBonePosition(head));
					GuiEngine::Esp::CenterString(Pos1, (L"present [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF::OrangeRed);
				}
			}
		}

		drawnerMtx.unlock();

		if (Vars::Misc::crosshair)GuiEngine::Esp::Crosshair(Vars::Config::ScreenWidth, Vars::Config::ScreenHeight, { 255,255,255,0.7f }, 1.5f);
		if (Vars::Aim::drawFov)GuiEngine::Esp::Circle({ (float)Vars::Config::ScreenWidth / 2,(float)Vars::Config::ScreenHeight / 2 }, { 255,255,255,0.7f }, Vars::Aim::fov, 1.f);
	}
}

void Hacks()
{
	if (Vars::Config::LocalPlayerIsValid)
	{
		//if (Vars::Misc::spider) myLocalPlayer.spider();
		if (Vars::Misc::alwaysDay) myLocalPlayer.AlwaysDay(Vars::Misc::alwaysDay_float);
		//if (Vars::Misc::speedhack) myLocalPlayer.speedhack();

		if (Vars::Aim::NoSway)
		{
			myLocalPlayer.NoSway(0.99f);
			Vars::Aim::NoSway_trigger = true;
		}
		else if (Vars::Aim::NoSway_trigger)
		{
			myLocalPlayer.NoSway(0.0f);
			Vars::Aim::NoSway_trigger = false;
		}
	}
}