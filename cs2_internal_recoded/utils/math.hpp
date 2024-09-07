#pragma once

// convert angle in degrees to radians
#define M_DEG2RAD(DEGREES) ((DEGREES) * (math::_PI / 180.f))
// convert angle in radians to degrees
#define M_RAD2DEG(RADIANS) ((RADIANS) * (180.f / math::_PI))
/// linearly interpolate the value between @a'X0' and @a'X1' by @a'FACTOR'
#define M_LERP(X0, X1, FACTOR) ((X0) + ((X1) - (X0)) * (FACTOR))

namespace math {
	// pi value
	inline constexpr float _PI = 3.141592654f;
	// double of pi
	inline constexpr float _2PI = 6.283185307f;
	// half of pi
	inline constexpr float _HPI = 1.570796327f;
	// quarter of pi
	inline constexpr float _QPI = 0.785398163f;
	// reciprocal of double of pi
	inline constexpr float _1DIV2PI = 0.159154943f;
	// golden ratio
	inline constexpr float _PHI = 1.618033988f;


	template <typename T>
	[[nodiscard]] __forceinline constexpr const T& clamp(const T& value, const T& minimal, const T& maximal) noexcept
	{
		return (value < minimal) ? minimal : (value > maximal) ? maximal : value;
	}
}
