#pragma once

#include <cstddef>
#include <cstdint>
#include <stl/cstring>
#include <stl/convert>

#include "../stivale2.h"

namespace mm
{
	struct FrameAllocator
	{
	public:
		void init_allocator(stivale2_struct_tag_memmap *tag);
		void* allocz(uint64_t count);
		void* alloc(uint64_t count);
		void free(void* addr, uint64_t count);
	private:
		bool check_range(size_t start, size_t count);
		// TODO: this really shouldn't be a property..
		size_t bitmap_size;
		uint8_t* bitmap;

		uint64_t highest_addr;
	};
};