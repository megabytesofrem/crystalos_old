#include <stl/common>
#include <stl/cstring>
#include <stl/convert>

#include "sys/gdt.hpp"
#include "mm/frame_alloc.hpp"
#include "debug.hpp"

// Stivale 2
#include "stivale2.h"

static uint8_t stack[4096];
mm::FrameAllocator frame_alloc;

extern "C" void kmain(struct stivale2_struct *stivale2_struct);

struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = 0
	},
	.framebuffer_width  = 0,
	.framebuffer_height = 0,
	.framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header stivale_hdr = {
	.entry_point = (uint64_t)kmain,
	.stack = (uintptr_t)stack + sizeof(stack),
	.flags = 0,
	//.tags = (uintptr_t)&framebuffer_hdr_tag
};

inline void test_allocator()
{
	auto one = frame_alloc.alloc(1);
	auto two = frame_alloc.alloc(1);
	auto pog = frame_alloc.alloc(1);

	debug::write_string("\n-----------------------------------------\n");
	debug::write_uint((uintptr_t)one, debug::base_hex);
	debug::write_string("\n");
	debug::write_uint((uintptr_t)two, debug::base_hex);
	debug::write_string("\n");
	debug::write_uint((uintptr_t)pog, debug::base_hex);
	debug::write_string("\n-----------------------------------------\n");

	// Free after we are done
	frame_alloc.free(one, 1);
	frame_alloc.free(two, 1);
	frame_alloc.free(pog, 1);
}

extern "C" void kmain(struct stivale2_struct *stivale)
{
	debug::write_string("Initializing GDT..\n");
	sys::gdt::init_gdt();

	// Initialize page frame allocator
	debug::write_string("Initialize our page allocator\n");

	auto *mmtag = (stivale2_struct_tag_memmap*)stivale::get_tag(stivale, STIVALE2_STRUCT_TAG_MEMMAP_ID);
	frame_alloc.init_allocator(mmtag);

	// Test the allocator
	test_allocator();

	// Never ret from kmain
	for (;;) {
		asm ("hlt");
	}
}