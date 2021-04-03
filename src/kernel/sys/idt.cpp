#include "idt.hpp"
#include "cpu/exceptions.hpp"

static sys::idt::IDTEntry table[256];

void sys::idt::add_idt_entry(size_t index, void* handler, uint8_t ist)
{
	uintptr_t address = (uintptr_t)handler;

	table[index].offset_low = address;
	table[index].selector = 0x08;	// KERNEL_CODE_SELECTOR
	table[index].ist = ist;
	table[index].flags = 0x8e;
	table[index].offset_mid = (address >> 16);
	table[index].offset_high = (address >> 32);

	table[index].zero = 0x0;
}

void sys::idt::init_idt()
{
	add_idt_entry(0x00, (void*)&exc_div0, 0x0);
	add_idt_entry(0x01, (void*)&exc_debug, 0x0);
	add_idt_entry(0x02, (void*)&exc_nmi, 0x0);
	add_idt_entry(0x03, (void*)&exc_break, 0x0);
	add_idt_entry(0x04, (void*)&exc_overflow, 0x0);
	add_idt_entry(0x05, (void*)&exc_bounds, 0x0);
	add_idt_entry(0x06, (void*)&exc_inv_opcode, 0x0);
	add_idt_entry(0x07, (void*)&exc_no_device, 0x0);
	add_idt_entry(0x08, (void*)&exc_double_fault, 0x0);
	add_idt_entry(0x0a, (void*)&exc_inv_tss, 0x0);
	add_idt_entry(0x0b, (void*)&exc_no_segment, 0x0);
	add_idt_entry(0x0c, (void*)&exc_ss_fault, 0x0);
	add_idt_entry(0x0d, (void*)&exc_gpf, 0x0);
	add_idt_entry(0x0e, (void*)&exc_page_fault, 0x0);
	// add_idt_entry(0x10, &sys::cpu::exc_div0, 0x0);
	// add_idt_entry(0x11, &sys::cpu::exc_div0, 0x0);
	// add_idt_entry(0x12, &sys::cpu::exc_div0, 0x0);

	IDTPointer ptr = {
		.size = sizeof(table) - 1,
		.addr  = (uintptr_t)&table 
	};

	// Load the new IDT
	asm volatile("lidt %0" :: "m"(ptr) : "memory");
	asm volatile("sti");
}