#include "mMem.h"

void memoryAllocate(unsigned int size, BYTE* dst, BYTE* src)
{
	DWORD protocol{};
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &protocol);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, protocol, &protocol);

}


void memoryPatch(unsigned int size, BYTE* dst)
{
	DWORD protocol{};
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &protocol);
	memset(dst, 0x90, size); // sets all bytes at dst to nop
	VirtualProtect(dst, size, protocol, &protocol);

}


uintptr_t AddyAddress(uintptr_t address, std::vector<unsigned int> offsets)
{
	uintptr_t theaddress{ address };
	for (unsigned int off : offsets)
	{
		address = *(uintptr_t*)address;
		address += off;
	}

	return theaddress;
}