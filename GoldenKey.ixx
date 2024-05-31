module;

#include "pch.h"

export module GoldenKey;

import Player;

namespace bm
{
	export class GoldenKey final
	{
	public:
		class Index final
		{
		private: static constexpr const size_t OFFSET = __LINE__ + 1;
		public: static constexpr const size_t SELL_MOST_VALUABLE_CITY_FOR_HALF_PRICE = __LINE__ - OFFSET;
			  static constexpr const size_t COUNT = __LINE__ - OFFSET;
		};

	public:
		static constexpr const size_t COUNT = 30;

	public:
		static const char* LogName(const size_t index) noexcept
		{
			assert(index < Index::COUNT);

			switch (index)
			{
			case Index::SELL_MOST_VALUABLE_CITY_FOR_HALF_PRICE:
				return "Sell Most Valuable City for Half Price";
				break;
			default:
				assert(false);
				break;
			}

			return nullptr;
		}

		static const char* LogDescription(const size_t index) noexcept
		{
			assert(index < Index::COUNT);

			switch (index)
			{
			case Index::SELL_MOST_VALUABLE_CITY_FOR_HALF_PRICE:
				return "Sell your most valuable city including the buildings by half its price.";
				break;
			default:
				assert(false);
				break;
			}

			return nullptr;
		}

		static const char* Process(const size_t index, Player& player) noexcept
		{
			(void)(player);
			assert(index < Index::COUNT);

			switch (index)
			{
			case Index::SELL_MOST_VALUABLE_CITY_FOR_HALF_PRICE:
				break;
			default:
				assert(false);
				break;
			}

			return nullptr;
		}

	private:
	};
}