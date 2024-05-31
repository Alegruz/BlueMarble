module;
#include "pch.h"
export module Player;

import City;
import Tile;

namespace bm
{
	export class Player final
	{
	public:
		explicit constexpr Player(const size_t index, const size_t balance) noexcept
			: mIndex(index)
			, mCurrentTileIndex(INVALID_SIZE)
			, mBalance(balance)
			, mRemainingNoMansLandTurn(0)
			, mMostValuableCityIndex(INVALID_SIZE)
			, mGoldenKeyIndices()
		{}

		inline constexpr size_t GetBalance() const noexcept { return mBalance; }
		inline constexpr size_t GetIndex() const noexcept { return mIndex; }
		inline constexpr size_t GetCurrentTileIndex() const noexcept { return mCurrentTileIndex; }

		inline constexpr void MoveTo(const size_t index) noexcept { assert(index < Tile::COUNT); mCurrentTileIndex = index; }
		inline constexpr size_t MoveBy(const size_t offset) noexcept { mCurrentTileIndex = (mCurrentTileIndex + offset) % Tile::COUNT; return mCurrentTileIndex; }

		inline constexpr bool IsAlive() const noexcept { return mCurrentTileIndex != INVALID_SIZE; }
		inline constexpr void GoBankrupt() noexcept { mCurrentTileIndex = INVALID_SIZE; }

		inline constexpr bool BuyCity(Tile& tile, const City& city) noexcept
		{
			tile.SetOwner(mIndex);
			const size_t cityIndex = tile.GetIndex();
			if (mBalance < City::CITY_LAND_ACQUIRE_FEES[cityIndex])
			{
				return false;
			}

			mBalance -= City::CITY_LAND_ACQUIRE_FEES[cityIndex];

			if (mMostValuableCityIndex == INVALID_SIZE)
			{
				mMostValuableCityIndex = cityIndex;
			}
			else
			{
				const size_t currentValue = City::GetCurrentValue(city, cityIndex);
				if (currentValue > mMostValuableCityIndex)
				{
					mMostValuableCityIndex = cityIndex;
				}
			}

			return true;
		}

		inline constexpr bool Withdraw(const size_t amount) noexcept
		{
			if (mBalance < amount)
			{
				return false;
			}

			mBalance -= amount;
			return true;
		}

		inline constexpr void Deposit(const size_t amount) noexcept { mBalance += amount; }

		inline constexpr size_t GetRemainingNoMansLandTurns() const noexcept { return mRemainingNoMansLandTurn; }
		inline constexpr void EnterNoMansLand() noexcept { mRemainingNoMansLandTurn = 3; }
		inline constexpr void ExitNoMansLand() noexcept { mRemainingNoMansLandTurn = 0; }
		inline constexpr void Update() noexcept { if (mRemainingNoMansLandTurn > 0) { --mRemainingNoMansLandTurn; } }

		inline constexpr void AddGoldenKey(const size_t index) noexcept { mGoldenKeyIndices.push_back(index); }

	private:
		size_t mIndex;
		size_t mCurrentTileIndex;
		size_t mBalance;
		size_t mRemainingNoMansLandTurn;
		size_t mMostValuableCityIndex;

		std::vector<size_t> mGoldenKeyIndices;
	};
}