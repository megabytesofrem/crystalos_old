#pragma once

#include <cstdint>
#include <cstddef>

namespace sys::gdt
{
	struct [[gnu::packed]] GDTEntry {
		uint16_t limit;
		uint16_t base_low16;
		uint8_t  base_mid8;
		uint8_t  access;
		uint8_t  granularity;
		uint8_t  base_high8;
	};

	struct [[gnu::packed]] GDTPointer {
		uint16_t limit;
		uintptr_t addr;
	};

	void add_gdt_entry(size_t index, uint8_t access, uint8_t gran);
	void init_gdt();
};