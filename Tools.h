#pragma once
#include <Windows.h>
#include <iostream>
#include <atomic>
#include <string>

#define frame_rate(target)\
{\
static DWORD fps;\
static DWORD total = 0;\
static DWORD frames = 0;\
static DWORD start = 0;\
frames++;\
total += GetTickCount() - start;\
if (total >= 500)\
{\
	*(short*)target = frames * 1000 / total; frames = 0; total = 0;\
}\
start = GetTickCount();\
}\

char* randomStrGen(int length);