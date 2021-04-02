#pragma once

#include <cstdint>
#include "../stivale2.h"

namespace mm::frame
{
	void init_allocator(stivale2_struct_tag_memmap *tag);
	void alloc_page(uint64_t count);
	void free(uintptr_t addr, uint64_t count);
};