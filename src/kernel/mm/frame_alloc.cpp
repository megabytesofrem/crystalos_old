#include "frame_alloc.hpp"

#include "../debug.hpp"
#include "memory.hpp"
#include "bitmap.hpp"

size_t div_roundup(size_t a, size_t b)
{
	return (a + (b - 1)) / b;
}

void mm::FrameAllocator::init_allocator(stivale2_struct_tag_memmap *tag)
{
	auto memory_map = tag->memmap;
	uint64_t nentries = tag->entries;

	// Find the first and last available addresses
	for (size_t i = 0; i < nentries; i++)
	{
		auto entry = memory_map[i];
		auto top = entry.base + entry.length;

		if (entry.type != STIVALE2_MMAP_USABLE)
			continue;

		// Update top to point to our new highest address in memory
		if (top > highest_addr)
			highest_addr = top;
	}

	bitmap_size = div_roundup(highest_addr, mm::PAGE_SIZE) / 8;

	for (size_t i = 0; i < nentries; i++)
	{
		auto entry = memory_map[i];
		if (entry.type != STIVALE2_MMAP_USABLE)
			continue;

		if (entry.length >= bitmap_size) 
		{
			// Place our bitmap here
			bitmap = (uint8_t*)(entry.base + mm::MEMORY_PHYS_OFFSET);
			stl::cstring::memset(bitmap, 0xFF, bitmap_size);

			memory_map[i].length -= bitmap_size;
			memory_map[i].base += bitmap_size;

			break;
		}
	}

	for (size_t i = 0; i < nentries; i++)
	{
		auto entry = memory_map[i];
		if (entry.type != STIVALE2_MMAP_USABLE)
			continue;

		// Loop through each page
		for (size_t j = 0; j < entry.length; j += mm::PAGE_SIZE)
		{
			unset_bit(bitmap, (entry.base + j) / mm::PAGE_SIZE);
		}
	}
}

void* mm::FrameAllocator::alloc(uint64_t count)
{
	for (size_t i = 0; i < highest_addr / mm::PAGE_SIZE; i++)
	{
		bool enough = check_range(i, count);
		if (!enough)
			continue;

		for (size_t j = 0; j < count; j++)
		{
			set_bit(bitmap, i + j);
		}

		// Return the address of the first page
		return (void*)(i * mm::PAGE_SIZE);
	}

	return 0;
}

void* mm::FrameAllocator::allocz(uint64_t count)
{
	void *addr = alloc(count);
	stl::cstring::memset(addr, 0, count * mm::PAGE_SIZE);

	return addr;
}

void mm::FrameAllocator::free(void* addr, uint64_t count)
{
	size_t start = (uintptr_t)addr / mm::PAGE_SIZE;
	for (size_t j = 0; j < count; j++)
	{
		unset_bit(bitmap, start + j);
	}
}

bool mm::FrameAllocator::check_range(size_t start, size_t count)
{
	auto page = start;
	while (count)
	{
		if (test_bit(bitmap, page) == 1)
			return false;

		page++;
		count--;
	}

	return true;
}