#pragma once

#include <cstddef>
#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace crt {
	/// copy one buffer to another, alternative of 'memcpy()'
/// @remarks: copies @a'nCount' bytes from @a'pSource' to @a'pDestination'. if the source and destination regions overlap, the behavior is undefined
/// @returns: pointer to the @a'pDestination'
	__forceinline void* memory_copy(void* pDestination, const void* pSource, std::size_t nCount)
	{
#ifdef _MSC_VER
		// @test: clang always tries to detect 'memcpy' like instructions and replace them with CRT's function call
		if (const std::size_t nCountAlign = (nCount & 3U); nCountAlign == 0U)
		{
			auto pDestinationLong = static_cast<unsigned long*>(pDestination);
			auto pSourceLong = static_cast<const unsigned long*>(pSource);
			__movsd(pDestinationLong, pSourceLong, nCount >> 2U);
		}
		else if (nCountAlign == 2U)
		{
			auto pDestinationWord = static_cast<unsigned short*>(pDestination);
			auto pSourceWord = static_cast<const unsigned short*>(pSource);
			__movsw(pDestinationWord, pSourceWord, nCount >> 1U);
		}
		else
		{
			auto pDestinationByte = static_cast<unsigned char*>(pDestination);
			auto pSourceByte = static_cast<const unsigned char*>(pSource);
			__movsb(pDestinationByte, pSourceByte, nCount);
		}
#else
		auto pDestinationByte = static_cast<std::uint8_t*>(pDestination);
		auto pSourceByte = static_cast<const std::uint8_t*>(pSource);

		while (nCount--)
			*pDestinationByte++ = *pSourceByte++;
#endif

		return pDestination;
	}

	// indicate that an object may be "moved from", i.e. allowing the efficient transfer of resources to another object, alternative of 'std::move'
	template <class T>
	[[nodiscard]] __forceinline constexpr std::remove_reference_t<T>&& move(T&& argument) noexcept
	{
		return static_cast<std::remove_reference_t<T>&&>(argument);
	}

	/// swap value of @a'left' to @a'right' and @a'right' to @a'left', alternative of 'std::swap'
	template <typename T> requires (std::is_move_constructible_v<T>&& std::is_move_assignable_v<T>)
	__forceinline constexpr void swap(T& left, T& right) noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_assignable_v<T>)
	{
		T temporary = move(left);
		left = move(right);
		right = move(temporary);
	}
}
