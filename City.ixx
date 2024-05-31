module;
#include "pch.h"
export module City;

namespace bm
{
	export class City final
	{
	public:
		enum class eColor : uint8_t
		{
			RED = 0,
			GREEN,
			YELLOW,
			BLACK,
			COUNT,
		};

	public:
		class Index final
		{
		public:
		private:    static constexpr const size_t OFFSET = __LINE__ + 1;
		public: static constexpr const size_t TAIPEI = __LINE__ - OFFSET;
			  static constexpr const size_t BEIJING = __LINE__ - OFFSET;
			  static constexpr const size_t MANILA = __LINE__ - OFFSET;
			  static constexpr const size_t JEJU = __LINE__ - OFFSET;
			  static constexpr const size_t SINGAPORE = __LINE__ - OFFSET;
			  static constexpr const size_t CAIRO = __LINE__ - OFFSET;
			  static constexpr const size_t ISTANBUL = __LINE__ - OFFSET;
			  static constexpr const size_t ATHENS = __LINE__ - OFFSET;
			  static constexpr const size_t COPENHAGEN = __LINE__ - OFFSET;
			  static constexpr const size_t STOCKHOLM = __LINE__ - OFFSET;
			  static constexpr const size_t CONCORD = __LINE__ - OFFSET;
			  static constexpr const size_t BERN = __LINE__ - OFFSET;
			  static constexpr const size_t BERLIN = __LINE__ - OFFSET;
			  static constexpr const size_t OTTAWA = __LINE__ - OFFSET;
			  static constexpr const size_t BUENOS_AIRES = __LINE__ - OFFSET;
			  static constexpr const size_t SAO_PAULO = __LINE__ - OFFSET;
			  static constexpr const size_t SYDNEY = __LINE__ - OFFSET;
			  static constexpr const size_t BUSAN = __LINE__ - OFFSET;
			  static constexpr const size_t HAWAII = __LINE__ - OFFSET;
			  static constexpr const size_t LISBON = __LINE__ - OFFSET;
			  static constexpr const size_t QUEEN_ELIZABETH = __LINE__ - OFFSET;
			  static constexpr const size_t MADRID = __LINE__ - OFFSET;
			  static constexpr const size_t TOKYO = __LINE__ - OFFSET;
			  static constexpr const size_t COLUMBIA = __LINE__ - OFFSET;
			  static constexpr const size_t PARIS = __LINE__ - OFFSET;
			  static constexpr const size_t ROME = __LINE__ - OFFSET;
			  static constexpr const size_t LONDON = __LINE__ - OFFSET;
			  static constexpr const size_t NEW_YORK = __LINE__ - OFFSET;
			  static constexpr const size_t SEOUL = __LINE__ - OFFSET;
		};
		static constexpr const size_t COUNT = Index::SEOUL + 1;

		struct CityConstructionFee final
		{
			int32_t Hotel;
			int32_t Building;
			int32_t Villa;
		};

		struct CityUsageFee final
		{
			int32_t Land;
			int32_t Hotel = -1;
			int32_t Building = -1;
			int32_t Villa = -1;
			int32_t DoubleVilla = -1;
		};

		static constexpr const CityConstructionFee CITY_CONSTRUCTION_FEES[static_cast<size_t>(eColor::COUNT)] =
		{
			{.Hotel = 250'000, .Building = 150'000, .Villa = 50'000},
			{.Hotel = 500'000, .Building = 300'000, .Villa = 100'000},
			{.Hotel = 750'000, .Building = 450'000, .Villa = 150'000},
			{.Hotel = 1'000'000, .Building = 600'000, .Villa = 200'000},
		};

		static constexpr const int32_t CITY_LAND_ACQUIRE_FEES[COUNT] =
		{
				 50'000,	// taipei
				 80'000,	// beijing
				 80'000,	// manila
				200'000,	// jeju
				100'000,	// singapore
				100'000,	// cairo
				120'000,	// istanbul
				140'000,	// athens
				160'000,	// copenhagen
				160'000,	// stockholm
				200'000,	// concord
				180'000,	// bern
				180'000,	// berlin
				200'000,	// ottawa
				220'000,	// buenos aires
				240'000,	// sao paulo
				240'000,	// sydney
				500'000,	// busan
				260'000,	// hawaii
				260'000,	// lisbon
				300'000,	// queen elizabeth
				280'000,	// madrid
				300'000,	// tokyo
				450'000,	// columbia
				320'000,	// paris
				320'000,	// rome
				350'000,	// london
				350'000,	// new york
			  1'000'000,	// seoul
		};

		static constexpr const CityUsageFee CITY_USAGE_FEES[COUNT] =
		{
			{.Land = 2'000,	.Hotel = 250'000,	.Building = 90'000,	.Villa = 10'000,	.DoubleVilla = 30'000},	// taipei
			{.Land = 4'000,	.Hotel = 450'000,	.Building = 180'000,	.Villa = 20'000,	.DoubleVilla = 60'000},	// beijing
			{.Land = 4'000,	.Hotel = 450'000,	.Building = 180'000,	.Villa = 20'000,	.DoubleVilla = 60'000},	// manila
			{.Land = 300'000,			  						  					 								  },	// jeju
			{.Land = 6'000,	.Hotel = 550'000,	.Building = 270'000,	.Villa = 30'000,	.DoubleVilla = 90'000},	// singapore
			{.Land = 6'000,	.Hotel = 550'000,	.Building = 270'000,	.Villa = 30'000,	.DoubleVilla = 90'000},	// cairo
			{.Land = 8'000,	.Hotel = 600'000,	.Building = 300'000,	.Villa = 40'000,	.DoubleVilla = 100'000},	// istanbul
			{.Land = 10'000,	.Hotel = 750'000,	.Building = 450'000,	.Villa = 50'000,	.DoubleVilla = 150'000},	// athens
			{.Land = 12'000,	.Hotel = 900'000,	.Building = 500'000,	.Villa = 60'000,	.DoubleVilla = 180'000},	// copenhagen
			{.Land = 12'000,	.Hotel = 900'000,	.Building = 500'000,	.Villa = 60'000,	.DoubleVilla = 180'000},	// stockholm
			{.Land = 300'000,				  					  					 								  },	// concord
			{.Land = 14'000,	.Hotel = 950'000,	.Building = 550'000,	.Villa = 70'000,	.DoubleVilla = 200'000},	// bern
			{.Land = 14'000,	.Hotel = 950'000,	.Building = 550'000,	.Villa = 70'000,	.DoubleVilla = 200'000},	// berlin
			{.Land = 16'000,	.Hotel = 1'000'000,	.Building = 600'000,	.Villa = 80'000,	.DoubleVilla = 220'000},	// ottawa
			{.Land = 18'000,	.Hotel = 1'050'000,	.Building = 700'000,	.Villa = 90'000,	.DoubleVilla = 250'000},	// buenos aires
			{.Land = 20'000,	.Hotel = 1'100'000,	.Building = 750'000,	.Villa = 100'000,	.DoubleVilla = 300'000},	// sao paulo
			{.Land = 20'000,	.Hotel = 1'100'000,	.Building = 750'000,	.Villa = 100'000,	.DoubleVilla = 300'000},	// sydney
			{.Land = 600'000,				  					  					 								  },	// busan
			{.Land = 22'000,	.Hotel = 1'150'000,	.Building = 800'000,	.Villa = 110'000,	.DoubleVilla = 330'000},	// hawaii
			{.Land = 22'000,	.Hotel = 1'150'000,	.Building = 800'000,	.Villa = 110'000,	.DoubleVilla = 330'000},	// lisbon
			{.Land = 250'000,				  					  					 								  },	// queen elizabeth
			{.Land = 24'000,	.Hotel = 1'200'000,	.Building = 850'000,	.Villa = 120'000,	.DoubleVilla = 360'000},	// madrid
			{.Land = 26'000,	.Hotel = 1'270'000,	.Building = 900'000,	.Villa = 130'000,	.DoubleVilla = 390'000},	// tokyo
			{.Land = 300'000,				  					  					 								  },	// columbia
			{.Land = 28'000,	.Hotel = 1'400'000,	.Building = 1'000'000,	.Villa = 150'000,	.DoubleVilla = 450'000},	// paris
			{.Land = 28'000,	.Hotel = 1'400'000,	.Building = 1'000'000,	.Villa = 150'000,	.DoubleVilla = 450'000},	// rome
			{.Land = 30'000,	.Hotel = 1'500'000,	.Building = 1'100'000,	.Villa = 170'000,	.DoubleVilla = 500'000},	// london
			{.Land = 30'000,	.Hotel = 1'500'000,	.Building = 1'100'000,	.Villa = 170'000,	.DoubleVilla = 500'000},	// new york
			{.Land = 2'000'000,				  					  					 								  },	// seoul
		};

		static constexpr const size_t MAX_NUM_VILLAS = 2;

		enum class eOwnerChoice : uint8_t
		{
			DO_NOTHING = 0,
			CONSTRUCT_VILLA,
			CONSTRUCT_BUILDING,
			CONSTRUCT_HOTEL,
			COUNT,
		};

	public:
		inline static constexpr size_t GetCurrentValue(const City& city, const size_t cityIndex)
		{
			return City::CITY_LAND_ACQUIRE_FEES[cityIndex] + city.GetNumVillas() * City::CITY_CONSTRUCTION_FEES[cityIndex].Villa + city.HasBuilding() * City::CITY_CONSTRUCTION_FEES[cityIndex].Building + city.HasHotel() * City::CITY_CONSTRUCTION_FEES[cityIndex].Hotel;
		}

	public:
		explicit constexpr City(const std::string& name, const eColor color) noexcept
			: mName(name)
			, mColor(color)
			, mNumVillas(0)
			, mbHasBuilding(false)
			, mbHasHotel(false)
		{}
		~City() = default;

		inline constexpr const std::string& GetName() const noexcept { return mName; }
		inline constexpr eColor GetColor() const noexcept { return mColor; }

		inline constexpr size_t GetNumVillas() const noexcept { return mNumVillas; }
		inline constexpr bool HasBuilding() const noexcept { return mbHasBuilding; }
		inline constexpr bool HasHotel() const noexcept { return mbHasHotel; }

		inline constexpr void ConstructVilla() noexcept
		{
			assert(mNumVillas < MAX_NUM_VILLAS);
			++mNumVillas;
		}

		inline constexpr void ConstructBuilding() noexcept { assert(mbHasBuilding == false); mbHasBuilding = true; }
		inline constexpr void ConstructHotel() noexcept { assert(mbHasHotel == false); mbHasHotel = true; }

	private:
		std::string mName;
		eColor mColor;

		size_t mNumVillas;
		bool mbHasBuilding;
		bool mbHasHotel;
	};
}