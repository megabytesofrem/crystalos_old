#pragma once

#include <cstdint>
#include <cstddef>

#include <stl/cstring>
#include "sys/io.hpp"

/**
 * Debugging functions to write to port 0xE9
 */
namespace debug
{
	// Predefined bases
	constexpr int base_binary = 2;
	constexpr int base_octal = 8;
	constexpr int base_hex = 16;

	using namespace stl::cstring;

	inline void write_string(const char *s)
	{
		auto len = strlen(s);

		for (size_t i = 0; i < len; i++) {
			sys::io::write8(0xE9, s[i]);
		}
	}

	template <typename T>
	inline void write_int(T num, int base)
	{
		char buffer[50];

		stl::convert::to_int(num, base, buffer, 50);
		write_string(buffer);
	}

	template <typename T>
	inline void write_uint(T num, int base)
	{
		char buffer[50];
		stl::convert::to_unsigned(num, base, buffer, 50);

		// Add a prefix if needed
		switch (base) {
			case 2:
				write_string("0b");
				break;
			case 8:
				write_string("0o");
				break;
			case 16:
				write_string("0x");
				break;
			default: break;
		}
		write_string(buffer);
	}
};