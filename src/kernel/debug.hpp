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
	using namespace stl::cstring;

	inline void write_string(const char *s)
	{
		auto len = strlen(s);

		for (size_t i = 0; i < len; i++) {
			sys::io::write8(0xE9, s[i]);
		}
	}
};