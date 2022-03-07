#include "hooking.h"

bool hook(void* toHook, void* ourFunc, int size)
{
	if (size < 5)
	{
		return false;
	}

	DWORD protocol{};
	VirtualProtect(toHook, size, PAGE_EXECUTE_READWRITE, &protocol);
	memset(toHook, 0x90, size);

	/*
	example of why we need to subtract 5 PAY ATTENTION TO TOHOOK
	toHook:
	crap
	jmp [ourFunc] -- I THINK WHATS CONFUSING YOU IS THIS PART BC YOU FORGOT TO
	REALIZE THAT WHEN YOU HOOK AT THE GAMES FUNCTION WE ARE NOPING AND THEN
	ADDING THE JMP STATEMENT AND BECAUSE THE JMP STATEMENT STARTS TO JUMP FROM THE ADDRESS
	OF THE END OF THE JMP WE NEED TO SUBTRACT THE LENGTH FROM THE START OF THE HOOK ADDRESS TO THE END OF
	THE JUMP VARIABLE - IMAGINE ADDING EXTRA CODE OTHER THAN THE JUMP, THEN WE'D HAVE TO SUBTRACT BY THE LENGTH OF THE
	CODE CHUNK WE ADDED AND THE START OF THE HOOK ADDRESS
	[
	   distance from the end of the jump statement TO the start of our func label
	   The relative jump uses the number of bytes
	   between the last byte occupied by the construct
	   and the byte before the destination operation
	   . For this reason, when you calculate HOW many
	   bytes the construct must jump, you subtract 5.
	   To exclude the bytes occupied by the construct
	   itself -- 
	]
	ourFunc:




	*/


	DWORD addressOffset{ (DWORD)ourFunc - (DWORD)toHook - 5 }; //subtract 5 because we need to jump to the start of jmp

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = addressOffset;

	VirtualProtect(toHook, size, protocol, &protocol);

	return true;
}

DWORD trampHook(void* toHook, void* ourFunc, int size)
{
	if (size < 5)
	{
		return 0;
	}
	//reserving a page of memory in the program we're inj3cting
	BYTE* gateway{ (BYTE*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE) };

	//copying the cotents of where we are hooking to the reserved page
	memcpy_s(gateway, size, toHook, size);

	*(BYTE*)(gateway + size) = 0xE9;

	*(DWORD*)((DWORD)gateway + size + 1) = (DWORD)toHook - (DWORD)gateway - 5;

	hook(toHook, ourFunc, size);


	return (DWORD)gateway;
}

//For our hooking class

Hooking::Hooking(BYTE* src, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len)
{
	this->src = src;
	this->dst = dst;
	this->len = len;
	this->PtrToGateWay = PtrToGateWay;
}
Hooking::Hooking(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len)
{
	HMODULE hModule = GetModuleHandleA(modName);

	//if this is an external method perhaps there is a way to inject internally? Would that make it "stealthier"?
	this->src = (BYTE*)GetProcAddress(hModule, exportName);
	this->dst = dst;
	this->len = len;
	this->PtrToGateWay = PtrToGateWay;
}

void Hooking::Enable()
{
	memcpy(originalByte, src, len);
	*(uintptr_t*)PtrToGateWay = (uintptr_t)trampHook(src, dst, len);

	bStatus = true;

}
void Hooking::Disable()
{
	memoryAllocate(len, src, originalByte);
	bStatus = false;

}
void Hooking::Toggle()
{
	if (!bStatus) Enable();
	else Disable();
}