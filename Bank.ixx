export module Bank;

namespace bm
{
	export class Bank final
	{
	public:
		static constexpr const size_t MAX_BALANCE = 500'000 * 16 + 100'000 * 40 + 50'000 * 20 + 20'000 * 20 + 10'000 * 20 + 5'000 * 20 + 1'000 * 20;
	};
}