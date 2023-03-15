#include "RustSDK.h"

LocalPlayer myLocalPlayer;

#pragma region BasePlayer

void BasePlayer::set_addr(DWORD64 ptr)
{
	if (this_ptr == ptr) return;
	this_ptr = ptr;
}

void BasePlayer::set_position(Vector3 pos)
{
	this_pos = pos;
}

DWORD64 BasePlayer::get_addr()
{
	return this_ptr;
}

bool BasePlayer::HasFlags(int flag)
{
	if (!this_ptr) return 0;
	return (read(this_ptr + oPlayerFlags, int) & flag);
}

bool BasePlayer::IsDead()
{
	if (!this_ptr) return 0;
	return read(this_ptr + oLifeState, bool);
}

bool BasePlayer::IsVisible()
{
	if (!this_ptr) return 0;
	DWORD64 PlayerModel = read(this_ptr + oPlayerModel, DWORD64);
	return read(PlayerModel + oVisible, bool);
}

float BasePlayer::GetHealth()
{
	if (!this_ptr) return 0.f;
	return read(this_ptr + oHealth, float);
}

std::wstring BasePlayer::GetName()
{
	if (!this_ptr) return L"Null str";
	_UncStr structure = read(read(this_ptr + oDisplayName, DWORD64), _UncStr);
	std::wstring name = {};

	for (int i = 0; i < structure.len && i < 20; i++)
	{
		name += structure.str[i];
	}

	return name;
}

Vector3 BasePlayer::GetVelocity()
{
	if (!this_ptr) return { 0.f, 0.f, 0.f };
	DWORD64 PlayerModel = read(this_ptr + oPlayerModel, DWORD64);
	return read(PlayerModel + oNewVelocity, Vector3);
}

Vector3 BasePlayer::GetPosition()
{
	if (!this_ptr) return 0;
	DWORD64 ObjectClass = read(this_ptr + 0x30, DWORD64);
	DWORD64 GameObject = read(ObjectClass + 0x30, DWORD64);
	DWORD64 Trans = read(GameObject + 0x8, DWORD64);
	DWORD64 Vec = read(Trans + 0x38, DWORD64);
	return read(Vec + 0x90, Vector3);
}

DWORD64 BasePlayer::GetTransform(int bone)
{
	// https://www.unknowncheats.me/forum/3398634-post8972.html

	if (!this_ptr) return 0;
	//DWORD64 baseEntity = read(this_ptr + 0x28, DWORD64);
	DWORD64 model = read(this_ptr + 0x130, DWORD64);
	DWORD64 boneTransforms = read(model + 0x48, DWORD64);
	DWORD64 val = read(boneTransforms + (0x20 + (bone * 0x8)), DWORD64);
	return read(val + 0x10, DWORD64);
}

struct TransformAccessReadOnly
{
	ULONGLONG	pTransformData;
	int			index;
};

struct TransformData
{
	ULONGLONG pTransformArray;
	ULONGLONG pTransformIndices;
};

struct Matrix34
{
	Vector4 vec0;
	Vector4 vec1;
	Vector4 vec2;
};

Vector3 BasePlayer::GetPosition(DWORD64 transform)
{
	__m128 result;

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly = read(transform + 0x38, TransformAccessReadOnly);
	unsigned int index = read(transform + 0x40, DWORD64);
	TransformData transformData = read(pTransformAccessReadOnly.pTransformData + 0x18, TransformData);

	if (transformData.pTransformArray && transformData.pTransformIndices)
	{
		result = read(transformData.pTransformArray + (uint64_t)0x30 * index, __m128);
		int transformIndex = read(transformData.pTransformIndices + (uint64_t)0x4 * index, int);
		int pSafe = 0;
		while (transformIndex >= 0 && pSafe++ < 200)
		{
			Matrix34 matrix34 = read(transformData.pTransformArray + (uint64_t)0x30 * transformIndex, Matrix34);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

			result = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix34.vec0));

			transformIndex = read(transformData.pTransformIndices + (uint64_t)0x4 * transformIndex, int);
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

Vector3 BasePlayer::GetBonePosition(BoneList BoneID)
{
	if (!this_ptr) return { 0.f, 0.f, 0.f };
	return GetPosition(GetTransform(BoneID));
}

std::wstring BasePlayer::GetActiveWeaponName()
{
	if (!this_ptr)return L"Null str";

	int ActweUID = read(this_ptr + oActiveItem, int);
	if (ActweUID == NULL)
	{
		return L"empty";
	}


	DWORD64 Inventory = read(this_ptr + oPlayerInventory, DWORD64);
	DWORD64 Belt = read(Inventory + oItemContainer, DWORD64);
	DWORD64 ItemList = read(Belt + oListList, DWORD64);  // public List<Item> itemList;
	DWORD64 Items = read(ItemList + 0x10, DWORD64); //	public List<InventoryItem.Amount> items;
	for (int i = 0; i < 6; i++)
	{
		DWORD64 weapon = read(Items + 0x20 + (i * 0x8), DWORD64);//public class Item
		int UID = read(weapon + 0x28, int);

		if (UID == ActweUID)
		{
			DWORD64 ItemDefinition = read(weapon + 0x20, DWORD64);
			DWORD64 TranslatePhrase = read(ItemDefinition + 0x28, DWORD64);
			DWORD64 addr_name = read(TranslatePhrase + 0x18, DWORD64);
			_UncStr Str = read(addr_name, _UncStr);
			if (!Str.len) return L"dick";

			std::wstring name = {};
			for (int i = 0; i < Str.len && i < 20; i++)
			{
				name += Str.str[i];
			}
			return name;

			//std::cout << "weapon " << weapon << std::endl;
		}

	}
	return L"dick";
}

#pragma endregion

#pragma region LocalPlayer
bool LocalPlayer::update_view_matrix()
{
	if (BaseEntityCamera != NULL)
	{
		pViewMatrix = read(BaseEntityCamera + 0x2E4, Matrix4x4);
		return true;
	}
	else return false;
}

bool LocalPlayer::WorldToScreen(Vector3 world, Vector2* screen)
{
	Matrix4x4* matrix = &pViewMatrix;

	Vector3 translationVector = Vector3(matrix->_14, matrix->_24, matrix->_34);
	Vector3 up = Vector3(matrix->_12, matrix->_22, matrix->_32);
	Vector3 right = Vector3(matrix->_11, matrix->_21, matrix->_31);

	const float w = translationVector.dot(world) + matrix->_44;

	if (w < 0.5f)return false; //0.098f

	const float y = up.dot(world) + matrix->_42;
	const float x = right.dot(world) + matrix->_41;
	
	*screen =
	{
		((float)Vars::Config::ScreenWidth / 2) * (1.f + x / w),
		((float)Vars::Config::ScreenHeight / 2) * (1.f - y / w)
	};
	return true;
}

void LocalPlayer::AlwaysDay(float time)
{
	if (!this_ptr) return;
	if (TodCycle != NULL)
	{
		write(TodCycle + 0x10, time, float);
	}
}

void LocalPlayer::set_active_weapon()
{
	if (!this_ptr)return;

	int ActweUID = read(this_ptr + oActiveItem, int);
	if (ActweUID == NULL)
	{
		myActiveWeapon.set_addr(NULL);
		return;
	}


	DWORD64 Inventory = read(this_ptr + oPlayerInventory, DWORD64);
	if (!Inventory)return;

	DWORD64 Belt = read(Inventory + oItemContainer, DWORD64);
	DWORD64 ItemList = read(Belt + oListList, DWORD64);  // public List<Item> itemList;
	DWORD64 Items = read(ItemList + 0x10, DWORD64); //	public List<InventoryItem.Amount> items;
	for (int i = 0; i < 6; i++)
	{
		DWORD64 weapon = read(Items + 0x20 + (i * 0x8), DWORD64); //public class Item
		int UID = read(weapon + 0x28, int);

		if (ActweUID == UID)
		{
			if (weapon)myActiveWeapon.set_addr(weapon);
			else myActiveWeapon.set_addr(NULL);
			return;
		}

	}
	myActiveWeapon.set_addr(NULL);
}

void LocalPlayer::NoSway(float var)
{
	if (!this_ptr) return;
	{
		write(this_ptr + oClothingAccuracyBonus, var, float);
	}
}

Vector2  LocalPlayer::GetBA() {
	if (!this_ptr)return { 0.0f,0.0f };
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return { 0.0f,0.0f };

	return  read(Input + obodyAngles, Vector2);
}
Vector2  LocalPlayer::GetRA() {
	if (!this_ptr)return{ 0.0f,0.0f };
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return { 0.0f,0.0f };

	return read(Input + oRecoilAngles, Vector2);
}

void  LocalPlayer::SetBA(const Vector2& newAngle) {
	if (!this_ptr)return;
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return;

	write(Input + obodyAngles, newAngle, Vector2);
}

void  LocalPlayer::SetRA(const Vector2& newAngle)
{
	if (!this_ptr)return;
	DWORD64 Input = read(this_ptr + oPlayerInput, DWORD64);
	if (!Input)return;

	write(Input + oRecoilAngles, newAngle, Vector2);
}

#pragma endregion

#pragma region Item

bool Item::set_addr(DWORD64 ptr)
{
	if (this_ptr == ptr) return false;
	this_ptr = ptr;
}

DWORD64 Item::get_addr()
{
	return this_ptr;
}

DWORD64 Item::GetID()
{
	if (!this_ptr) return 0;

	DWORD64 Info = read(this_ptr + 0x20, DWORD64);
	if (!Info) return NULL;

	return read(Info + 0x18, DWORD64);
}

float Old_RecoilYawMax;
float Old_RecoilYawMin;
float Old_RecoilPitchMax;
float Old_RecoilPitchMin;
float Old_ADSScale;
float Old_MovementPenalty;

DWORD64 oldRecoilPtr = NULL;

void Item::no_recoil(float factor)
{
	if (!this_ptr) return;
	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);
	DWORD64 ActiveRecoilPtr = read(Held + oRecoil, DWORD64);

	if (oldRecoilPtr != ActiveRecoilPtr)
	{
		if (oldRecoilPtr != NULL)
		{

			write(oldRecoilPtr + oRecoilYawMax, Old_RecoilYawMax, float);
			write(oldRecoilPtr + oRecoilYawMin, Old_RecoilYawMin, float);

			write(oldRecoilPtr + oRecoilPitchMax, Old_RecoilPitchMax, float);
			write(oldRecoilPtr + oRecoilPitchMin, Old_RecoilPitchMin, float);

			write(oldRecoilPtr + oADSScale, Old_ADSScale, float);
			write(oldRecoilPtr + oMovementPenalty, Old_MovementPenalty, float);
		}

		if (ActiveRecoilPtr != NULL)
		{

			Old_RecoilYawMax = read(ActiveRecoilPtr + oRecoilYawMax, float);
			Old_RecoilYawMin = read(ActiveRecoilPtr + oRecoilYawMin, float);
			Old_RecoilPitchMax = read(ActiveRecoilPtr + oRecoilPitchMax, float);
			Old_RecoilPitchMin = read(ActiveRecoilPtr + oRecoilPitchMin, float);
			Old_ADSScale = read(ActiveRecoilPtr + oADSScale, float);
			Old_MovementPenalty = read(ActiveRecoilPtr + oMovementPenalty, float);

			write(ActiveRecoilPtr + oRecoilYawMax, factor * Old_RecoilYawMax, float);
			write(ActiveRecoilPtr + oRecoilYawMin, factor * Old_RecoilYawMin, float);

			write(ActiveRecoilPtr + oRecoilPitchMax, factor * Old_RecoilPitchMax, float);
			write(ActiveRecoilPtr + oRecoilPitchMin, factor * Old_RecoilPitchMin, float);

			write(ActiveRecoilPtr + oADSScale, factor * Old_ADSScale, float);
			write(ActiveRecoilPtr + oMovementPenalty, factor * Old_MovementPenalty, float);
		}
		oldRecoilPtr = ActiveRecoilPtr;
	}
	else if (factor == 1.0f)
	{
		write(ActiveRecoilPtr + oRecoilYawMax, Old_RecoilYawMax, float);
		write(ActiveRecoilPtr + oRecoilYawMin, Old_RecoilYawMin, float);

		write(ActiveRecoilPtr + oRecoilPitchMax, Old_RecoilPitchMax, float);
		write(ActiveRecoilPtr + oRecoilPitchMin, Old_RecoilPitchMin, float);

		write(ActiveRecoilPtr + oADSScale, Old_ADSScale, float);
		write(ActiveRecoilPtr + oMovementPenalty, Old_MovementPenalty, float);
	}
	else
	{
		write(ActiveRecoilPtr + oRecoilYawMax, factor * Old_RecoilYawMax, float);
		write(ActiveRecoilPtr + oRecoilYawMin, factor * Old_RecoilYawMin, float);

		write(ActiveRecoilPtr + oRecoilPitchMax, factor * Old_RecoilPitchMax, float);
		write(ActiveRecoilPtr + oRecoilPitchMin, factor * Old_RecoilPitchMin, float);

		write(ActiveRecoilPtr + oADSScale, factor * Old_ADSScale, float);
		write(ActiveRecoilPtr + oMovementPenalty, factor * Old_MovementPenalty, float);
	}
}

void Item::NoSpread()
{
	if (!this_ptr)return;
	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);
	if (!Held)return;

	write(Held + oStancePenalty, 0.f, float);
	write(Held + oAimconePenalty, 0.f, float);
	write(Held + oAimCone, 0.f, float);
	write(Held + oHipAimCone, 0.f, float);
	write(Held + oAimconePenaltyPerShot, 0.f, float);
}

void Item::fatbullet()
{
	if (!this_ptr)return;
	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);//BaseProjectile
	if (!Held)return;

	DWORD64 Projectile_list = read(Held + 0x338, DWORD64);

	DWORD64 Projectile_items = read(Projectile_list + 0x10, DWORD64);

	for (int i = 0; i < 24; i++)
	{
		DWORD64 Projectile_item = read(Projectile_items + 0x20 + (i * 0x8), DWORD64);

		if (Projectile_item)
		{
			float thick = read(Projectile_item + 0x2C, float);
			if (thick > 0.f && thick < 4.0f) //public float thickness;
			{
				write(Projectile_item + 0x2C, Vars::Aim::fat, float);

			}
		}
	}
}

void  Item::super_melee()
{
	if (!this_ptr)return;

	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);

	if (!Held)return;

	write(Held + omaxDistance, 3.0f, float);
	write(Held + oAttackRadius, 1.0f, float);

	write(Held + oBlockSprintOnAttack, false, bool);
	write(Held + oonlyThrowAsProjectile, false, bool);
	write(Held + oisAutomatic, true, bool);
	write(Held + othrowReady, true, bool);

}


void  Item::super_eoka()
{
	if (!this_ptr)return;

	DWORD64 Held = read(this_ptr + oHeldEntity, DWORD64);
	if (!Held)return;
	//std::cout << "Held:" << std::hex << Held << std::endl;
	write(Held + 0x360, 1.0f, float);
	write(Held + 0x370, true, bool);

}

#pragma endregion