#include "RustSDK.h"
#include "Variables.h"
#include "Gui.h"

#pragma once
namespace DrawEsp
{
	bool InFov(Vector2& screenPos);
	void player_esp(BasePlayer player, LocalPlayer myPlayer, std::wstring name, bool isNPC = false);
}