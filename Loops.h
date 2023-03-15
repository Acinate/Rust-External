#pragma once
#include <mutex>
#include "Memory.h"
#include "Variables.h"
#include "RustSDK.h"
#include "Gui.h"
#include "Draw.h"
#include "aim.h"

struct objInf {
	DWORD64 objAddr;
	byte prefabNumber;
};

enum ObjList : byte
{
	player = 1,
	corpse,
	backpack,
	npc,
	patrol_heli,
	bradley,
	wood,
	stone,
	sulfur,
	metal,
	hemp,
	mushroom,
	junkpile,
	military_crate,
	elite_crate,
	heli_crate,
	bradley_crate,
	locked_crate,
	lootcrate,
	foodbox,
	world,
	supply,
	sam_site_turret,
	autoturret,
	guntrap,
	flameturret,
	cupboard,
	box_wooden_large,
	woodbox_deployed,
	small_stash,
	sleepingbag,
	minicopter,
	scraptransporthelicopter,
	rowboat,
	rhib,
	xmas_crate
};

void BaseNetworkable_loop(DWORD64 BaseNetworkable);
void GameObjectManager_loop(DWORD64 BaseUnityPlayer);
void esp_drawner();
void Hacks();