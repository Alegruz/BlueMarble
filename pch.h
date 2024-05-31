#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//#define DEBUG_CASES

#ifdef DEBUG_CASES
#define DEBUG_CASE0
#define DEBUG_CASE1
#define DEBUG_CASE2
#define DEBUG_CASE3
#endif

//#define BM_USE_SFML

namespace bm
{
	constexpr const size_t INVALID_SIZE = static_cast<size_t>(-1);
	constexpr const size_t DEFAULT_PLAYER_COUNT = 2;
	constexpr const size_t MAX_PLAYER_COUNT = 4;

	constexpr uint32_t Hash(uint32_t in);

	template <size_t N>
	constexpr uint32_t Hash(char const(&str)[N])
	{
		uint32_t h{};
		for (uint32_t i{}; i < N; ++i)
			h ^= uint32_t(str[i]) << (i % 4 * 8);
		return Hash(h);
	}

	template <size_t N>
	constexpr uint32_t constexpr_rand_impl(char const(&file)[N], uint32_t line, uint32_t column = 0x8dc97987)
	{
		return Hash(Hash(__TIME__) ^ Hash(file) ^ Hash(line) ^ Hash(column));
	}

#define RANDOM()	constexpr_rand_impl(__FILE__, __LINE__)
}