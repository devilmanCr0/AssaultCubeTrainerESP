#pragma once
#include "Winp.h"
#include "mMem.h"

DWORD trampHook(void* toHook, void* ourFunc, int size);
bool hook(void* toHook, void* ourFunc, int size);


struct Hooking
{
	bool bStatus{ false };

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	BYTE* PtrToGateWay{ nullptr };

	int len{ 0 };

	BYTE originalByte[15]{ 0 };

	Hooking(BYTE* src, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len);
	Hooking(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len);

	void Enable();
	void Disable();
	void Toggle();
};