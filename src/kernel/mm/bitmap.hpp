#pragma once

#include <cstddef>
#include <cstdint>

#include "frame_alloc.hpp"

inline void set_bit(uint8_t *bitmap, size_t bit)
{
	bitmap[bit / 8] |= (1 << (bit % 8));
}

inline void unset_bit(uint8_t *bitmap, size_t bit)
{
	bitmap[bit / 8] &= ~(1 << (bit % 8));
}

inline void flip_bit(uint8_t *bitmap, size_t bit)
{
	bitmap[bit / 8] ^= (1 << (bit % 8));
}

inline bool test_bit(uint8_t *bitmap, size_t bit)
{
	return !!(bitmap[bit / 8] & (1 << (bit % 8)));
}