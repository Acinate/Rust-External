#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <string>
#include <emmintrin.h>

#include "Memory.h"
#include "Offsets.h"
#include "Math.h"
#include "Variables.h"

enum PlayerFlags : int
{
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
};

enum  BoneList : int
{
	l_hip = 1,
	l_knee,
	l_foot,
	l_toe,
	l_ankle_scale,
	pelvis,
	penis,
	GenitalCensor,
	GenitalCensor_LOD0,
	Inner_LOD0,
	GenitalCensor_LOD1,
	GenitalCensor_LOD2,
	r_hip,
	r_knee,
	r_foot,
	r_toe,
	r_ankle_scale,
	spine1,
	spine1_scale,
	spine2,
	spine3,
	spine4,
	l_clavicle,
	l_upperarm,
	l_forearm,
	l_hand,
	l_index1,
	l_index2,
	l_index3,
	l_little1,
	l_little2,
	l_little3,
	l_middle1,
	l_middle2,
	l_middle3,
	l_prop,
	l_ring1,
	l_ring2,
	l_ring3,
	l_thumb1,
	l_thumb2,
	l_thumb3,
	IKtarget_righthand_min,
	IKtarget_righthand_max,
	l_ulna,
	neck,
	head,
	jaw,
	eyeTranform,
	l_eye,
	l_Eyelid,
	r_eye,
	r_Eyelid,
	r_clavicle,
	r_upperarm,
	r_forearm,
	r_hand,
	r_index1,
	r_index2,
	r_index3,
	r_little1,
	r_little2,
	r_little3,
	r_middle1,
	r_middle2,
	r_middle3,
	r_prop,
	r_ring1,
	r_ring2,
	r_ring3,
	r_thumb1,
	r_thumb2,
	r_thumb3,
	IKtarget_lefthand_min,
	IKtarget_lefthand_max,
	r_ulna,
	l_breast,
	r_breast,
	BoobCensor,
	BreastCensor_LOD0,
	BreastCensor_LOD1,
	BreastCensor_LOD2,
	collision,
	displacement
};

typedef struct wchar_str
{
	wchar_t str[100];
};

typedef struct _UncStr
{
	char stub[0x10];
	int len;
	wchar_t str[100];
} *pUncStr;

class Item
{
protected:
	DWORD64 this_old_ptr = 0;
	DWORD64 this_ptr = 0;
public:
	Item() :this_ptr(NULL) {};

	DWORD64 get_addr();
	bool set_addr(DWORD64 ptr);

	DWORD64 GetID();
	void no_recoil(float factor);
	void NoSpread();
	void fatbullet();

	void super_melee();
	void super_eoka();
};

class BasePlayer
{
protected:
	DWORD64 this_ptr = 0;
	Vector3 this_pos = NULL;
public:
	BasePlayer() :this_ptr(NULL) {};
	DWORD64 get_addr();
	void set_addr(DWORD64 ptr);
	void set_position(Vector3 pos);

	bool IsDead();
	bool HasFlags(int flag);
	bool IsVisible();
	float GetHealth();
	std::wstring GetName();

	Vector3 GetVelocity();
	DWORD64 GetTransform(int bone);
	Vector3 GetPosition();
	Vector3 GetPosition(DWORD64 pTransform);

	Vector3 GetBonePosition(BoneList BoneID);
	std::wstring GetActiveWeaponName();
};

class LocalPlayer :public BasePlayer
{
public:
	DWORD64 BaseEntityCamera = {0};
	DWORD64 ObjectClassCamera = {0};
	Matrix4x4 pViewMatrix = {0};

	Item myActiveWeapon;
	DWORD64 TodCycle = {0};
	LocalPlayer() :BasePlayer() {};


	bool update_view_matrix();
	
	bool WorldToScreen(Vector3 world, Vector2* screen);

	void AlwaysDay(float time);
	void NoSway(float var);
	void spider();
	void speedhack();
	void walkWater(bool enable);
	void waterBoost(float val);
	void wallWalk(bool enable);

	void setGravity(float val);
	bool isMenu();

	void set_active_weapon();
	void set_player_flags(int set, int remove);

	Vector2 GetBA();
	Vector2 GetRA();
	void SetRA(const Vector2& newAngle);
	void SetBA(const Vector2& newAngle);
};

extern LocalPlayer myLocalPlayer;