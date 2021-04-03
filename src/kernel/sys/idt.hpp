#pragma once

#include <cstdint>
#include <cstddef>

namespace sys::idt
{
	struct [[gnu::packed]] IDTEntry
	{
		uint16_t offset_low;	// 0..15
		uint16_t selector;
		uint8_t ist; 			// 0..2 hold IST offset, rest zero
		uint8_t flags;
		uint16_t offset_mid;	// 16..31
		uint32_t offset_high;	// 32..63
		uint32_t zero;			// reserved, always zero
	};

	struct [[gnu::packed]] IDTPointer
	{
		uint16_t size;
		uintptr_t addr;
	};

	void add_idt_entry(size_t index, void* handler, uint8_t ist);
	void init_idt();
};