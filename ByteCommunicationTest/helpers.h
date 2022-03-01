#pragma once

#include <cinttypes>
#include <type_traits>
#include <random>

namespace test_helpers
{
	namespace RandomSeedHelper
	{
		template<std::uint8_t Value>
		using SeedType = std::integral_constant<std::uint8_t, Value>;

		using RANDOM = SeedType<0>;
		using SET = SeedType<1>;
	}

	template<typename T>
	class RandomSeed
	{
	public:
		template<typename R = T, typename = std::enable_if_t<std::is_same_v<R, RandomSeedHelper::RANDOM>>>
		RandomSeed() {
			std::random_device rd{};
			seed = rd();
		}

		template<typename R = T, typename = std::enable_if_t<std::is_same_v<R, RandomSeedHelper::SET>>>
		RandomSeed(std::uint64_t seed) {
			RandomSeed::seed = seed;
		}

		operator std::uint64_t() const noexcept {
			return seed;
		}

		constexpr bool isRandom() const noexcept {
			return std::is_same_v<T, RandomSeedHelper::RANDOM>;
		}

	private:

		inline static std::uint64_t seed;
	};
}