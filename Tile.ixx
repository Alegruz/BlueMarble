module;
#include "pch.h"
export module Tile;

namespace bm
{
	export class Tile final
	{
	public:
		enum class eType : uint8_t
		{
			START,
			CITY,
			SPECIAL_CITY,
			GOLDEN_KEY,
			NO_MANS_LAND,
			SOCIAL_WELFARE_FUND_WITHDRAW,
			SPACE_TRAVEL,
			SOCIAL_WELFARE_FUND_DEPOSIT,
			COUNT,
		};

	public:
		static constexpr const size_t COUNT = 40;

	public:
		explicit constexpr Tile(const eType type) noexcept : Tile(type, INVALID_SIZE) {}
		explicit constexpr Tile(const eType type, const size_t index) noexcept : mType(type), mIndex(index), mOwnerPlayerIndex(INVALID_SIZE) {}

		inline constexpr bool IsOwned() const noexcept { return mOwnerPlayerIndex != INVALID_SIZE; }

		inline constexpr size_t GetOwnerPlayerIndex() const noexcept { return mOwnerPlayerIndex; }
		inline constexpr void SetOwner(const size_t playerIndex) noexcept { mOwnerPlayerIndex = playerIndex; }

		inline constexpr eType GetType() const noexcept { return mType; }
		inline constexpr size_t GetIndex() const noexcept { return mIndex; }

	public:
		eType   mType;
		size_t  mIndex;
		size_t	mOwnerPlayerIndex;
	};
}