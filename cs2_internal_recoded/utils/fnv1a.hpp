#pragma once

// used: uint32_t
#include <cstdint>
// used: strlen
#include <cstring>
// used: get_constant
#include "ct_constants.hpp"

namespace utils
{
	class fnv1a_class {
	public:
		// use the default fnv1a_class()
		fnv1a_class();
		fnv1a_class(const uint32_t seed = 2166136261U, const uint32_t prime = 16777619U);

		fnv1a_class(const fnv1a_class& copy) = delete;
		fnv1a_class& operator=(const fnv1a_class& other) = delete;

		inline const uint32_t hash(const char* str);
		constexpr const uint32_t hash_ct(const char* str, const uint32_t value = fnv1a_seed) noexcept;
		const bool cmp(const char* str, const uint32_t key) noexcept;

	private:
		static constexpr uint32_t fnv1a_seed = 2166136261U;
		static constexpr uint32_t fnv1a_prime = 16777619U;
	};


	// The two strings here should change every time the project is updated
	// Of course using two random numbers directly is also feasible
	// But I think it's not enough RANDOM in source lol
	// nwm, this is just a random shit
	fnv1a_class::fnv1a_class() {
		// This should be a random string
		uint32_t fnv1a_seed = hash_ct("r7Xk3kvdSMyK4dCgBb31nHNDKR6ieqwO", 2166136261U);
		// This should be another random string
		uint32_t fnv1a_prime = hash_ct("jl3PCfiND4qPSiWjnRiG0oUxVYF2UnDJ", 2166136261U);
	}

	fnv1a_class::fnv1a_class(const uint32_t seed, const uint32_t prime) {
		uint32_t fnv1a_seed = seed;
		uint32_t fnv1a_prime = prime;
	}

	inline const uint32_t fnv1a_class::hash(const char* str) {
		if (!str)
			return 0U;

		const char* data = const_cast<char*>(str);
		uint32_t hash = fnv1a_seed;

		for (size_t i = 0U; i < strlen(str); ++i)
		{
			const uint8_t value = data[i];
			hash = hash ^ value;
			hash *= fnv1a_prime;
		}

		return hash;
	}

	constexpr const uint32_t fnv1a_class::hash_ct(const char* str, const uint32_t value = fnv1a_seed) noexcept {
		return !*str ? value : hash_ct(str + 1, static_cast<unsigned>(1ull * (value ^ static_cast<uint8_t>(*str)) * fnv1a_prime));
	}

	const bool fnv1a_class::cmp(const char* str, const uint32_t key) noexcept {
		return hash(str) == key;
	}


	static fnv1a_class fnv1a;
}
