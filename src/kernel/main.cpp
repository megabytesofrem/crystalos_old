#include <stl/common>
#include <stl/cstring>
#include <stl/convert>

#include "sys/gdt.hpp"
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

extern "C" void kmain(struct stivale2_struct *stivale2_struct)
{
	debug::write_string("Initializing GDT..");
	sys::gdt::init_gdt();

	char shit[50];
	stl::convert::to_int(420, 10, shit, 50);

	debug::write_string(shit);

	// Never ret from kmain
	for (;;) {
		asm ("hlt");
	}
}
