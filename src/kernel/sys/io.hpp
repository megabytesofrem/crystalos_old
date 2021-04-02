#pragma once

#include <cstdint>
#include <cstddef>

#include <stl/cstring>

namespace sys::io
{
	inline void write8(uint16_t port, uint8_t value) {
		asm volatile("outb %b0, %w1" : : "a"(value), "d"(port));
	}

	inline void write16(uint16_t port, uint8_t value) {
		asm volatile("outw %b0, %w1" : : "a"(value), "d"(port));
	}

	inline void write32(uint16_t port, uint8_t value) {
		asm volatile("outl %b0, %w1" : : "a"(value), "d"(port));
	}

	inline uint8_t read8(uint16_t port) {
		uint8_t value;

		asm volatile("inb %w1, %b0" : "=a"(value) : "d"(port));
		return value;
	}

	inline uint16_t read16(uint16_t port) {
		uint16_t value;

		asm volatile("inw %w1, %b0" : "=a"(value) : "d"(port));
		return value;
	}

	inline uint32_t read32(uint16_t port) {
		uint32_t value;

		asm volatile("inl %w1, %b0" : "=a"(value) : "d"(port));
		return value;
	}
};