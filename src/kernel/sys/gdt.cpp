#include "gdt.hpp"

sys::gdt::GDTEntry table[3];

void sys::gdt::add_gdt_entry(size_t index, uint8_t access, uint8_t gran)
{
	// Limit and base are 0
	table[index].limit = 0;
	table[index].base_low16 = 0;
	table[index].base_mid8 = 0;
	table[index].base_high8 = 0;

	table[index].access = access;
	table[index].granularity = gran;
}

void sys::gdt::init_gdt()
{
	// Null descriptor
	add_gdt_entry(0, 0, 0);

	// Kernel code
	add_gdt_entry(1, 0b1001'1010, 0b0010'0000);

	// Kernel data
	add_gdt_entry(1, 0b1001'0010, 0b0000'0000);

	// Reload the GDT
	GDTPointer ptr = {
		.limit = sizeof(table) - 1,
		.addr  = (uintptr_t)&table 
	};

	asm volatile("lgdt %0" :: "m"(ptr) : "memory");
}