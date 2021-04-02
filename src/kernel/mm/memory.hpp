#pragma once

#include <cstddef>
#include <cstdint>

namespace mm
{
	constexpr inline uint64_t PAGE_SIZE = 0x1000;

	// Start of physical memory offset map
	constexpr inline uintptr_t MEMORY_PHYS_OFFSET = 0xffff800000000000;
};