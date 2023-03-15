#include "Variables.h"

namespace Vars
{
	namespace Config
	{
		DWORD64 GameObjectManager = NULL;
		DWORD64 BaseNetworkable = NULL;

		int ScreenWidth = 1920;
		int ScreenHeight = 1080;

		std::atomic<short> espFPS = 0;
		std::atomic<short> menuFPS = 0;

		bool LocalPlayerIsValid = false;
		bool panic = false;
		bool MenuActive = false;
	}

	namespace  Aim
	{
		bool aim = true;
		bool randomBone = false;
		DWORD64 addr_BasePlayer_on_Aimming = NULL;
		float fov = 30.0f;
		bool drawFov = true;
		float smooth = 0.0f;

		float recoil = 0.5f;
		bool NoSpread = false;
		bool NoSway = false;
		bool NoSway_trigger = false;

		bool ModifyBullet = false;
		float fat = 0.1f;
	}

	namespace  Esp
	{
		//player
		bool playerEsp = true;
		bool drawBox = true;
		bool drawHealthBar = true;
		bool drawSkeleton = true;
		bool drawActiveWeapons = true;

		// entities
		bool playerCorpse = true;
		bool playerSleepers = true;
		bool npcEsp = true;
		bool heliEsp = true;
		bool bradleyEsp = true;

		//loot
		bool wood = false; // TODO: BROKEN
		bool stone = false;
		bool sulphur = false;
		bool metal = false;
		bool hemp = false;
		bool dropItems = true;
		bool airdrop = true;
		bool mushroom = false;
		bool junkpile = false;
		bool lootcrate = true;
		bool foodbox = true;

		//etc
		bool landAirTurret = false;
		bool autoTurret = true;
		bool guntrap = true;
		bool flameturret = false;

		bool cupboard = false;
		bool cupboardPlayer = false;
		bool largeBox = false;
		bool smallBox = false;
		bool stash = false;
		bool sleepingbag = false;

		bool minicopter = true;
		bool cow = false;
		bool boat = true;
		bool bigBoat = true;

	}

	namespace  Misc
	{
		bool walkWatter = false;
		bool spider = false;
		bool HigthJump = false;
		bool speedhack = false;
		float speedBonus = 0.f;
		bool WatterBoost = false;
		bool WallWalk = false;

		bool ThirdPersonMode = false;
		bool AdminMode = false;
		bool superMelee = false;
		bool superEoka = false;
		bool alwaysDay = false;
		float alwaysDay_float = 10.0f;

		bool crosshair = false;

	}
}