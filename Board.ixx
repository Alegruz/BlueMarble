module;
#include "pch.h"
export module Board;

import City;
import GoldenKey;
import Tile;

namespace bm
{
	export class Board final
	{
	public:
		explicit constexpr Board() noexcept
			: mTiles
			{
				Tile(Tile::eType::START),
				Tile(Tile::eType::CITY, City::Index::TAIPEI),
				Tile(Tile::eType::GOLDEN_KEY),
				Tile(Tile::eType::CITY, City::Index::BEIJING),
				Tile(Tile::eType::CITY, City::Index::MANILA),
				Tile(Tile::eType::CITY, City::Index::JEJU),
				Tile(Tile::eType::CITY, City::Index::SINGAPORE),
				Tile(Tile::eType::GOLDEN_KEY),
				Tile(Tile::eType::CITY, City::Index::CAIRO),
				Tile(Tile::eType::CITY, City::Index::ISTANBUL),
				Tile(Tile::eType::NO_MANS_LAND),
				Tile(Tile::eType::CITY, City::Index::ATHENS),
				Tile(Tile::eType::GOLDEN_KEY),
				Tile(Tile::eType::CITY, City::Index::COPENHAGEN),
				Tile(Tile::eType::CITY, City::Index::STOCKHOLM),
				Tile(Tile::eType::CITY, City::Index::CONCORD),
				Tile(Tile::eType::CITY, City::Index::BERN),
				Tile(Tile::eType::GOLDEN_KEY),
				Tile(Tile::eType::CITY, City::Index::BERLIN),
				Tile(Tile::eType::CITY, City::Index::OTTAWA),
				Tile(Tile::eType::SOCIAL_WELFARE_FUND_WITHDRAW),
				Tile(Tile::eType::CITY, City::Index::BUENOS_AIRES),
				Tile(Tile::eType::GOLDEN_KEY),
				Tile(Tile::eType::CITY, City::Index::SAO_PAULO),
				Tile(Tile::eType::CITY, City::Index::SYDNEY),
				Tile(Tile::eType::CITY, City::Index::BUSAN),
				Tile(Tile::eType::CITY, City::Index::HAWAII),
				Tile(Tile::eType::CITY, City::Index::LISBON),
				Tile(Tile::eType::CITY, City::Index::QUEEN_ELIZABETH),
				Tile(Tile::eType::CITY, City::Index::MADRID),
				Tile(Tile::eType::SPACE_TRAVEL),
				Tile(Tile::eType::CITY, City::Index::TOKYO),
				Tile(Tile::eType::CITY, City::Index::COLUMBIA),
				Tile(Tile::eType::CITY, City::Index::PARIS),
				Tile(Tile::eType::CITY, City::Index::ROME),
				Tile(Tile::eType::GOLDEN_KEY),
				Tile(Tile::eType::CITY, City::Index::LONDON),
				Tile(Tile::eType::CITY, City::Index::NEW_YORK),
				Tile(Tile::eType::SOCIAL_WELFARE_FUND_DEPOSIT),
				Tile(Tile::eType::CITY, City::Index::SEOUL),
			}
			, mGoldenKeys{}
			, mCities
			{
					City("Taipei",          City::eColor::RED),
					City("Beijing",         City::eColor::RED),
					City("Manila",          City::eColor::RED),
					City("Jeju",            City::eColor::RED),
					City("Singapore",       City::eColor::RED),
					City("Cairo",           City::eColor::RED),
					City("Istanbul",        City::eColor::RED),
					City("Athens",          City::eColor::GREEN),
					City("Copenhagen",      City::eColor::GREEN),
					City("Stockholm",       City::eColor::GREEN),
					City("Concord",         City::eColor::GREEN),
					City("Bern",            City::eColor::GREEN),
					City("Berlin",          City::eColor::GREEN),
					City("Ottawa",          City::eColor::GREEN),
					City("Buenos Aires",    City::eColor::YELLOW),
					City("Sao Paulo",       City::eColor::YELLOW),
					City("Sydney",          City::eColor::YELLOW),
					City("Busan",           City::eColor::YELLOW),
					City("Hawaii",          City::eColor::YELLOW),
					City("Lisbon",          City::eColor::YELLOW),
					City("Queen Elizabeth", City::eColor::YELLOW),
					City("Madrid",          City::eColor::YELLOW),
					City("Tokyo",           City::eColor::BLACK),
					City("Columbia",        City::eColor::BLACK),
					City("Paris",           City::eColor::BLACK),
					City("Rome",            City::eColor::BLACK),
					City("London",          City::eColor::BLACK),
					City("New York",        City::eColor::BLACK),
					City("Seoul",           City::eColor::BLACK),
			}
		{}

		inline constexpr Tile& GetTile(const size_t index) noexcept
		{
			assert(index < Tile::COUNT);
			return mTiles[index];
		}

		inline constexpr City& GetCity(const size_t index) noexcept
		{
			assert(index < City::COUNT);
			return mCities[index];
		}

		inline constexpr const City& GetCity(const size_t index) const noexcept
		{
			assert(index < City::COUNT);
			return mCities[index];
		}

	private:
		Tile mTiles[Tile::COUNT];
		GoldenKey   mGoldenKeys[GoldenKey::COUNT];
		City        mCities[City::COUNT];
	};
}