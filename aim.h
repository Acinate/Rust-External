#pragma once
#include <iostream>
#include <string>
#include "RustSDK.h"

bool InFov(class BasePlayer& BasePlayer_on_Aimming, enum BoneList bone);
void Aim(DWORD64& BasePlayer_on_Aimming);