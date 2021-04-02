#include <stl/common>
#include <stl/cstring>
#include <stl/convert>

#include "sys/gdt.hpp"
#include "mm/frame_alloc.hpp"
#include "debug.hpp"

// Stivale 2
#include "stivale2.h"

static uint8_t stack[4096];
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

extern "C" void kmain(struct stivale2_struct *stivale)
{
	debug::write_string("Initializing GDT..\n");
	sys::gdt::init_gdt();

	// Initialize page frame allocator
	debug::write_string("Initialize our page allocator\n");
	mm::FrameAllocator alloc;

	stivale2_struct_tag_memmap *mmtag = (stivale2_struct_tag_memmap*)stivale::get_tag(stivale, STIVALE2_STRUCT_TAG_MEMMAP_ID);
	alloc.init_allocator(mmtag);

	auto shit = alloc.alloc(1);
	auto cum = alloc.alloc(1);
	auto fuck = alloc.alloc(1);
	debug::write_string("\n-----------------------------------------\n");
	debug::write_uint((uintptr_t)shit, debug::base_hex);
	debug::write_string("\n");
	debug::write_uint((uintptr_t)cum, debug::base_hex);
	debug::write_string("\n");
	debug::write_uint((uintptr_t)fuck, debug::base_hex);
	debug::write_string("\n-----------------------------------------\n");

	// Never ret from kmain
	for (;;) {
		asm ("hlt");
	}
}
