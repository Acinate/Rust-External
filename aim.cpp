#include "aim.h"

bool InFov(class BasePlayer& target, enum BoneList bone)
{
	Vector2 ScreenPos;
	if (!myLocalPlayer.WorldToScreen(target.GetBonePosition(head), &ScreenPos)) return false;
	return Math::Calc2D_Dist(Vector2(Vars::Config::ScreenWidth / 2, Vars::Config::ScreenHeight / 2), ScreenPos) <= Vars::Aim::fov;
}

void Normalize(Vector2& angle)
{
	if (angle.x > 89) angle.x = 89;
	if (angle.x < -89) angle.x = -89;
	while (angle.y > 180) angle.y -= 360;
	while (angle.y < -180) angle.y += 360;
}

float GetBulletSpeed()
{
	switch (myLocalPlayer.myActiveWeapon.GetID())
	{
	case 1545779598:	//ak47
		return 375.f;
	case 2482412119:	//lr300
		return 375.f;
	case 3390104151:	//semi-rifle
		return 375.f;
	case 28201841:		//m39
		return 375.f * 1.16f;
	case 2225388408:	//m249
		return 375.f * 1.4f;
	case 1588298435:	//bolt
		return 375.f * 1.8f;
	case 3516600001:	//l96	
		return 375.f * 3.2f;
	case 1318558775:	//mp5a4
		return 300.f * 0.8f;
	case 1796682209:	//smg
		return 300.f * 0.8f;
	case 2536594571:	//thompson
		return 300.f;
	case 3442404277:	//m92
		return 300.f;
	case 818877484:		//semi-pistol
		return 300.f;
	case 1373971859:	//python
		return 300.f;
	case 649912614:		//revolver
		return 300.f;
	default:
		return 0.f;
	}
}

double CalcBulletDrop(double height, double depthPlayerTarget, float velocity, float gravity)
{
	double pitch = (atan2(height, depthPlayerTarget));
	double BulletVelocityXY = velocity * cos(pitch);
	double Time = depthPlayerTarget / BulletVelocityXY;
	double TotalVerticalDrop = (0.5f * gravity * Time * Time);
	return TotalVerticalDrop * 10;
}

Vector3 Prediction(const Vector3& my_Pos, BasePlayer& BasePlayer_on_Aiming, BoneList bone)
{
	Vector3 BonePos = BasePlayer_on_Aiming.GetBonePosition(bone);

	float Dist = Math::Calc3D_Dist(my_Pos, BonePos);

	float BulletSpeed = GetBulletSpeed();
	if (BulletSpeed <= 0) return BonePos;

	float BulletTime = Dist / BulletSpeed;
	Vector3 vel = BasePlayer_on_Aiming.GetVelocity();
	Vector3 Predict = vel * BulletTime;
	BonePos += Predict;

	float DegAngle = myLocalPlayer.GetBA().x;
	float coef = 1;
	if (DegAngle >= 10 && Dist <= 100)
	{
		coef = cos(DEG2RAD(DegAngle)) * 0.1f;
	}
	else if (DegAngle >= 0)
	{
		coef = cos(DEG2RAD(DegAngle)) * 0.9f;
	}
	BonePos.y += (6.4f * BulletTime * BulletTime) * coef;

	return BonePos;
}

void GoToTarget(BasePlayer& BasePlayer_on_Aiming, BoneList bone)
{
	Vector3 Local = myLocalPlayer.GetBonePosition(head);
	Vector3 PlayerPos = Prediction(Local, BasePlayer_on_Aiming, bone);
	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - myLocalPlayer.GetBA();

	Normalize(Offset);

	Offset.x *= 1.0f - (Vars::Aim::smooth * 0.3 + 0.4);
	Offset.y *= 1.0f - (Vars::Aim::smooth * 0.3 + 0.4);

	Vector2 AngleToAim1 = myLocalPlayer.GetBA() + Offset;
	Vector2 AngleToAim2 = myLocalPlayer.GetRA() + Offset;
	myLocalPlayer.SetBA(AngleToAim1);
	std::cout << "Object:" << AngleToAim1.x << ", " << AngleToAim1.y << std::endl;
	//myLocalPlayer.SetRA(AngleToAim2);
}

void Aim(DWORD64& BasePlayer_on_Aiming)
{
	static BasePlayer Player;
	Player.set_addr(BasePlayer_on_Aiming);

	if (BasePlayer_on_Aiming && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		if (Player.IsDead())
		{
			BasePlayer_on_Aiming = NULL;
		}
		else
		{
			static int boneArr[6] = { head, spine1, r_upperarm, l_breast, r_hand };
			static BoneList bone;
			static DWORD64 isBasePlayerChange = NULL;
			if (isBasePlayerChange != Player.get_addr())
			{
				if (Vars::Aim::randomBone) bone = BoneList(boneArr[int(rand() % 6)]);
				else bone = head;

				isBasePlayerChange = Player.get_addr();
			}

			static int start = 0;
			static int summ = 0;

			summ += clock() - start;
			start += clock();

			if (summ >= 20)
			{
				GoToTarget(Player, bone);
				summ = 0;
			}

			static int RCSstart = 0;
			static int RCSsumm = 0;

			RCSsumm += clock() - RCSstart;
			RCSstart = clock();

			if (RCSsumm >= (0.2 + Vars::Aim::smooth * 0.3) * 10)
			{
				//myLocalPlayer.SetRA({ 0.f, 0.f });
				RCSsumm = 0;
			}
		}
	}
	else
	{
		BasePlayer_on_Aiming = NULL;
	}
}