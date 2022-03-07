#pragma once

#include "Winp.h"

void memoryAllocate(unsigned int size, BYTE* dst, BYTE* src);
void memoryPatch(unsigned int size, BYTE* dst);
uintptr_t AddyAddress(uintptr_t address, std::vector<unsigned int> offsets);
