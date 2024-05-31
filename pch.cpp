#include "pch.h"

namespace bm
{
    constexpr uint32_t bm::Hash(uint32_t in)
	{
		constexpr uint32_t r[]{
			0xdf15236c, 0x16d16793, 0x3a697614, 0xe0fe08e4,
			0xa3a53275, 0xccc10ff9, 0xb92fae55, 0xecf491de,
			0x36e86773, 0x0ed24a6a, 0xd7153d80, 0x84adf386,
			0x17110e76, 0x6d411a6a, 0xcbd41fed, 0x4b1d6b30
		};
		uint32_t out{ in ^ r[in & 0xF] };
		out ^= std::rotl(in, 020) ^ r[(in >> 010) & 0xF];
		out ^= std::rotl(in, 010) ^ r[(in >> 020) & 0xF];
		out ^= std::rotr(in, 010) ^ r[(in >> 030) & 0xF];
		return out;
	}
}