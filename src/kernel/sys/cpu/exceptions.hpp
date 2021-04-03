#pragma once

#include <cstdint>
#include <cstddef>

#include "../../debug.hpp"

extern "C" void exc_div0();
extern "C" void exc_debug();
extern "C" void exc_nmi();
extern "C" void exc_break();
extern "C" void exc_overflow();
extern "C" void exc_bounds();
extern "C" void exc_inv_opcode();
extern "C" void exc_no_device();
extern "C" void exc_double_fault();
extern "C" void exc_inv_tss();
extern "C" void exc_no_segment();
extern "C" void exc_ss_fault();
extern "C" void exc_gpf();
extern "C" void exc_page_fault();
extern "C" void exc_x87_fp();
extern "C" void exc_align_check();
extern "C" void exc_machine_check();
extern "C" void exc_simd_fp();
extern "C" void exc_virt();
extern "C" void exc_security();

namespace sys::cpu
{
	struct Registers
	{
		size_t r15;
		size_t r14;
		size_t r13;
		size_t r12;
		size_t r11;
		size_t r10;
		size_t r9;
		size_t r8;
		size_t rbp;
		size_t rdi;
		size_t rsi;
		size_t rdx;
		size_t rcx;
		size_t rbx;
		size_t rax;
		size_t rip;
		size_t cs;
		size_t flags;
		size_t rsp;
		size_t ss;

	};
};

extern "C" void exception_handler(int exception, sys::cpu::Registers *reg, size_t error_code)
{
	// FIXME: we really, really need printf..
	debug::write_string("fatal exception: ");
	debug::write_uint(exception, debug::base_hex);
	debug::write_string(" (");
	debug::write_uint(error_code, debug::base_hex);
	debug::write_string(")\n");

	debug::write_string("RIP=");
	debug::write_uint(reg->rip, debug::base_hex);
	debug::write_string("\n");
}