#include "Draw.h"

void Skeleton(BasePlayer* BasePlayer, D2D1::ColorF color)
{
	BoneList Bones[15] = {
		/*LF*/l_foot, l_knee, l_hip,
		/*RF*/r_foot, r_knee, r_hip,
		/*BD*/spine1, neck, head,
		/*LH*/l_upperarm, l_forearm, l_hand,
		/*RH*/r_upperarm, r_forearm, r_hand
	};
	Vector2 BonePos[15];

	// get bones screen pos
	for (int j = 0; j < 15; j++)
	{
		if (!myLocalPlayer.WorldToScreen(BasePlayer->GetBonePosition(Bones[j]), &BonePos[j]))
			return;
	}

	// draw main lines
	for (int j = 0; j < 15; j += 3)
	{
		GuiEngine::Esp::Line(Vector2{ BonePos[j].x, BonePos[j].y }, Vector2{ BonePos[j + 1].x, BonePos[j + 1].y }, color);
		GuiEngine::Esp::Line(Vector2{ BonePos[j + 1].x, BonePos[j + 1].y }, Vector2{ BonePos[j + 2].x, BonePos[j + 2].y }, color);
	}

	// draw add lines
	GuiEngine::Esp::Line(Vector2{ BonePos[2].x, BonePos[2].y }, Vector2{ BonePos[6].x, BonePos[6].y }, color);
	GuiEngine::Esp::Line(Vector2{ BonePos[5].x, BonePos[5].y }, Vector2{ BonePos[6].x, BonePos[6].y }, color);
	GuiEngine::Esp::Line(Vector2{ BonePos[9].x, BonePos[9].y }, Vector2{ BonePos[7].x, BonePos[7].y }, color);
	GuiEngine::Esp::Line(Vector2{ BonePos[12].x, BonePos[12].y }, Vector2{ BonePos[7].x, BonePos[7].y }, color);
}

bool DrawEsp::InFov(Vector2& screenPos)
{
	return Math::Calc2D_Dist(Vector2(Vars::Config::ScreenWidth / 2, Vars::Config::ScreenHeight / 2), screenPos) <= Vars::Aim::fov;
}

void DrawEsp::player_esp(BasePlayer player, LocalPlayer myPlayer, std::wstring name, bool isNPC)
{
	Vector2 Vec2topBoxPos;
	Vector3 Vec3topBoxPos = player.GetBonePosition(head); Vec3topBoxPos.y += 0.35f;

	if (myPlayer.WorldToScreen(Vec3topBoxPos, &Vec2topBoxPos))
	{
		static short dist;
		dist = Math::Calc3D_Dist(Vec3topBoxPos, myLocalPlayer.GetBonePosition(head));
		
		if (dist <= 0) return;

		if (!isNPC && player.HasFlags(Sleeping))
		{
			if (Vars::Esp::playerSleepers) GuiEngine::Esp::CenterString(Vec2topBoxPos, (name + L" [" + std::to_wstring(dist) + L"m]").c_str(), D2D1::ColorF(D2D1::ColorF::Gray));
			return;
		}

		GuiEngine::Esp::CenterString({ Vec2topBoxPos.x, Vec2topBoxPos.y - 18 }, (name + std::wstring(L" [" + std::to_wstring(dist) + L"m]")).c_str(), D2D1::ColorF(D2D1::ColorF::Orange));

		//if (isNPC) return;

		Vector2 Vec2playerFoot;
		//myLocalPlayer.WorldToScreen(player.GetBonePosition(l_foot), &Vec2playerFoot);
		myPlayer.WorldToScreen(player.GetBonePosition(l_foot), &Vec2playerFoot);
		static short boxHeight;
		static short boxWidth;
		boxHeight = Vec2playerFoot.y - Vec2topBoxPos.y;
		if (boxHeight <= 5) boxHeight = 5;
		boxWidth = boxHeight / 2;

		// Box
		if (Vars::Esp::drawBox)
		{
			GuiEngine::Esp::rect((Vec2topBoxPos.x) - (boxWidth + 2) / 2, (Vec2topBoxPos.y - 1), boxWidth + 2, boxHeight + 2, D2D1::ColorF(D2D1::ColorF::Black, 0.5f));
			if (player.IsVisible()) GuiEngine::Esp::rect(Vec2topBoxPos.x - boxWidth / 2, Vec2topBoxPos.y, boxWidth, boxHeight, { 0, 255, 0, 1 });
			else GuiEngine::Esp::rect(Vec2topBoxPos.x - boxWidth / 2, Vec2topBoxPos.y, boxWidth, boxHeight, { 255, 0, 0, 1});
		}

		if (Vars::Esp::drawHealthBar)
		{
			static float healthThick = 2;
			static float health_to_box_dist = 2;

			GuiEngine::Esp::Line(
				{
					(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
					Vec2topBoxPos.y + boxHeight + 1
				},
				{
					(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
					Vec2topBoxPos.y - 1
				},
				D2D1::ColorF(D2D1::ColorF::Black, 0.5f), 
				healthThick + 2
			);

			static float health; health = player.GetHealth();
			static float myGreen; myGreen = health / 100.0f;
			static float myRed; myRed = 1.0f - myGreen;

			GuiEngine::Esp::Line(
				{
					(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
					Vec2topBoxPos.y + boxHeight
				},
				{
					(Vec2topBoxPos.x - (boxWidth + 2) / 2) - (health_to_box_dist + healthThick),
					(Vec2topBoxPos.y + boxHeight) - boxHeight * ((health / 100.0f) <= 1.0f ? (health / 100.0f) : (1.0f))
				},
				{ myRed,myGreen,0,1 }, healthThick);
		}

		if (Vars::Esp::drawActiveWeapons)
		{
			GuiEngine::Esp::CenterString({ Vec2topBoxPos.x, Vec2topBoxPos.y + boxHeight + 6 }, player.GetActiveWeaponName().c_str());
		}
		if (Vars::Esp::drawSkeleton)
		{
			
			if (player.IsVisible()) Skeleton(&player, D2D1::ColorF::Green);
			else Skeleton(&player, D2D1::ColorF::Red);
		}
	}
}