#pragma once

// used: is_same_v
#include <type_traits>
// used: string wstring
#include <string>
// used: runtime_error
#include <stdexcept>
// used: ASSERT
#include "../core/common.hpp"

namespace crt {
	class crt_class {
	public:
		crt_class() = default;
		~crt_class() = default;

		template <typename any_char> requires (std::is_same_v<any_char, char> || std::is_same_v<any_char, wchar_t>)
			constexpr std::size_t str_length(const any_char* tszSource)
		{
			const any_char* tszSourceEnd = tszSource;

			while (*tszSourceEnd != any_char('\0'))
				++tszSourceEnd;

			return tszSourceEnd - tszSource;
		}

		//template <typename any_char> requires (std::is_same_v<any_char, char> || std::is_same_v<any_char, wchar_t>)
		//	int StringLengthMultiByte(const any_char* tszBegin, const any_char* tszEnd = nullptr)
		//{
		//	int nOctetCount = 0;

		//	// go through each character until terminating null up to end if given
		//	while (*tszBegin != any_char('\0') && (tszEnd == nullptr || tszBegin < tszEnd))
		//	{
		//		if (const std::uint32_t uChar = static_cast<std::uint32_t>(*tszBegin++); uChar < 0x80)
		//			nOctetCount += 1;
		//		else if (uChar < 0x800)
		//			nOctetCount += 2;
		//		else if (uChar < 0x10000)
		//			nOctetCount += 3;
		//		else if (uChar <= 0x7FFFFFFF)
		//			nOctetCount += 4;
		//	}

		//	return nOctetCount;
		//}

		//inline std::ptrdiff_t CharMultiByteFromUTF32(char* szOutBuffer, const std::size_t nOutBufferSize, const std::uint32_t uChar)
		//{
		//	// utf-8
		//	if (uChar < 0x80)
		//	{
		//		szOutBuffer[0] = static_cast<char>(uChar);
		//		return 1;
		//	}
		//	// utf-16
		//	if (uChar < 0x800 && nOutBufferSize >= 2U)
		//	{
		//		szOutBuffer[0] = static_cast<char>(0xC0 + (uChar >> 6U));
		//		szOutBuffer[1] = static_cast<char>(0x80 + (uChar & 0x3F));
		//		return 2;
		//	}
		//	// utf-16
		//	if (uChar < 0x10000 && nOutBufferSize >= 3U)
		//	{
		//		szOutBuffer[0] = static_cast<char>(0xE0 + (uChar >> 12U));
		//		szOutBuffer[1] = static_cast<char>(0x80 + ((uChar >> 6U) & 0x3F));
		//		szOutBuffer[2] = static_cast<char>(0x80 + (uChar & 0x3F));
		//		return 3;
		//	}
		//	// utf-32
		//	if (uChar <= 0x10FFFF && nOutBufferSize >= 4U)
		//	{
		//		szOutBuffer[0] = static_cast<char>(0xF0 + (uChar >> 18U));
		//		szOutBuffer[1] = static_cast<char>(0x80 + ((uChar >> 12U) & 0x3F));
		//		szOutBuffer[2] = static_cast<char>(0x80 + ((uChar >> 6U) & 0x3F));
		//		szOutBuffer[3] = static_cast<char>(0x80 + (uChar & 0x3F));
		//		return 4;
		//	}

		//	// invalid code point
		//	return 0;
		//}

		//__forceinline std::ptrdiff_t str_unicode2multibyte(char* szOutBuffer, const std::size_t nOutBufferLength, const wchar_t* wszBegin, const wchar_t* wszEnd = nullptr)
		//{
		//	char* pBufferBegin = szOutBuffer;
		//	const char* pBufferEnd = szOutBuffer + nOutBufferLength;

		//	while (pBufferBegin < pBufferEnd - 1 && (wszEnd == nullptr || wszBegin < wszEnd) && *wszBegin != L'\0')
		//		pBufferBegin += CharMultiByteFromUTF32(pBufferBegin, pBufferEnd - pBufferBegin - 1, *wszBegin++);

		//	*pBufferBegin = '\0';
		//	return pBufferBegin - szOutBuffer;
		//}

		std::wstring string2wstring(const std::string& str) {
			std::size_t len = std::mbstowcs(nullptr, str.c_str(), 0);
			if (len == static_cast<std::size_t>(-1)) {
				throw std::runtime_error("Invalid multibyte sequence");
			}

			std::wstring wstr(len, L'\0');
			std::mbstowcs(&wstr[0], str.c_str(), len);
			return wstr;
		}

		std::string wstring2string(const std::wstring& wstr) {
			std::size_t len = std::wcstombs(nullptr, wstr.c_str(), 0);
			if (len == static_cast<std::size_t>(-1)) {
				throw std::runtime_error("Invalid wide character sequence");
			}

			std::string str(len, '\0');
			std::wcstombs(&str[0], wstr.c_str(), len);
			return str;
		}

		// compare two strings, alternative of 'strcmp()', 'wcscmp()'
		// remarks: performs a signed/unsigned character comparison depending on the string type
		// returns: <0 - if @a'tszLeft' less than @a'tszRight', 0 - if @a'tszLeft' is identical to @a'tszRight', >0 - if @a'tszLeft' greater than @a'tszRight'
		template <typename C> requires (std::is_same_v<C, char> || std::is_same_v<C, wchar_t>)
		constexpr int string_compare(const C* tszLeft, const C* tszRight)
		{
			if (tszLeft == nullptr)
				return -1;

			if (tszRight == nullptr)
				return 1;

			using ComparisonType_t = std::conditional_t<std::is_same_v<C, char>, std::uint8_t, std::conditional_t<sizeof(wchar_t) == 2U, std::int16_t, std::int32_t>>;

			ComparisonType_t nLeft, nRight;
			do
			{
				nLeft = static_cast<ComparisonType_t>(*tszLeft++);
				nRight = static_cast<ComparisonType_t>(*tszRight++);

				if (nLeft == C('\0'))
					break;
			} while (nLeft == nRight);

			return nLeft - nRight;
		}

		template <typename C> requires (std::is_same_v<C, char> || std::is_same_v<C, wchar_t>)
			constexpr int string_compare_n(const C* tszLeft, const C* tszRight, std::size_t nCount)
		{
			using ComparisonType_t = std::conditional_t<std::is_same_v<C, char>, std::uint8_t, std::conditional_t<sizeof(wchar_t) == 2U, std::int16_t, std::int32_t>>;

			ComparisonType_t nLeft, nRight;
			while (nCount--)
			{
				nLeft = static_cast<ComparisonType_t>(*tszLeft++);
				nRight = static_cast<ComparisonType_t>(*tszRight++);

				if (nLeft != nRight)
					return nLeft - nRight;

				if (nLeft == C('\0'))
					break;
			}

			return 0;
		}


		[[nodiscard]] constexpr bool is_space(const std::uint8_t uChar)
		{
			return ((uChar >= '\t' && uChar <= '\r') || uChar == ' ');
		}

		[[nodiscard]] constexpr bool is_alpha(const std::uint8_t uChar)
		{
			return ((uChar >= 'A' && uChar <= 'Z') || (uChar >= 'a' && uChar <= 'z'));
		}

		[[nodiscard]] constexpr bool is_digit(const std::uint8_t uChar)
		{
			return (uChar >= '0' && uChar <= '9');
		}

		/// convert the string to an integer, @todo: no wide yet alternative of 'atoi()', '_wtoi()', '_atoi64()', '_wtoi64()', 'atol()', '_wtol()', 'atoll()', '_wtoll()', 'strtol()', 'wcstol()', '_strtoi64()', '_wcstoi64()', 'strtoul()', 'wcstoul()', 'strtoull()', 'wcstoull()'
		/// @param[in] iBase number of digits used to represent number. 0 to automatically determine number base in range [2 .. 16] or value in range [2 .. 36]
		/// @returns: integer converted from string
		template <typename T> requires (std::is_integral_v<T>)
			constexpr T string_to_integer(const char* szSourceBegin, char** pszSourceEnd = nullptr, int iBase = 0)
		{
			if (iBase < 0 || iBase == 1 || iBase > 36)
			{
				ASSERT(false); // given number base is out of range
				return 0;
			}

			const char* szSourceCurrent = szSourceBegin;

			// skip to first not whitespace
			while (is_space(*szSourceCurrent))
				++szSourceCurrent;

			// determine the sign and strip it
			const bool bIsPositive = ((*szSourceCurrent == '+' || *szSourceCurrent == '-') ? (*szSourceCurrent++ == '+') : true);
			constexpr bool bIsUnsigned = ((std::numeric_limits<T>::min)() == 0U);

			// user provided exact number base
			if (iBase > 0)
			{
				// strip 0x or 0X
				if (iBase == 16 && *szSourceCurrent == '0' && (szSourceCurrent[1] | ('a' ^ 'A')) == 'x')
					szSourceCurrent += 2;
			}
			// otherwise try to determine base automatically
			else if (*szSourceCurrent == '0')
			{
				if (((*szSourceCurrent++) | ('a' ^ 'A')) == 'x')
				{
					// a hexadecimal number is defined as "the prefix 0x or 0X followed by a sequence of the decimal digits and the letters a (or A) through f (or F) with values 10 through 15 respectively" (C standard 6.4.4.1)
					iBase = 16;
					++szSourceCurrent;
				}
				else
					// an octal number is defined as "the prefix 0 optionally followed by a sequence of the digits 0 through 7 only" (C standard 6.4.4.1) and so any number that starts with 0, including just 0, is an octal number
					iBase = 8;
			}
			else
				// a decimal number is defined as beginning "with a nonzero digit and consisting of a sequence of decimal digits" (C standard 6.4.4.1)
				iBase = 10;

			constexpr std::uint64_t ullNegativeMax = (bIsUnsigned ? (std::numeric_limits<T>::max)() : (static_cast<std::uint64_t>((std::numeric_limits<T>::max)()) + 1ULL));
			const std::uint64_t ullAbsoluteMax = (bIsPositive ? (std::numeric_limits<T>::max)() : ullNegativeMax);
			const std::uint64_t ullAbsoluteMaxOfBase = ullAbsoluteMax / iBase;

			bool bIsNumber = false;
			std::uint64_t ullResult = 0ULL;

			for (bool bIsDigit = false, bIsAlpha = false; ((bIsDigit = is_digit(*szSourceCurrent))) || ((bIsAlpha = is_alpha(*szSourceCurrent)));) // @note: looks slightly unsafe but have possibility to fast path, double parenthesis to suppress warnings
			{
				int iCurrentDigit = 0;

				if (bIsDigit)
					iCurrentDigit = *szSourceCurrent - '0';
				else if (bIsAlpha)
					iCurrentDigit = (*szSourceCurrent | ('a' ^ 'A')) - 'a' + 0xA;

				if (iCurrentDigit >= iBase)
					break;

				bIsNumber = true;
				++szSourceCurrent;

				// if the number has already hit the maximum value for the current type then the result cannot change, but we still need to advance source to the end of the number
				if (ullResult == ullAbsoluteMax)
				{
					ASSERT(false); // numeric overflow
					continue;
				}

				if (ullResult <= ullAbsoluteMaxOfBase)
					ullResult *= iBase;
				else
				{
					ASSERT(false); // numeric overflow
					ullResult = ullAbsoluteMax;
				}

				if (ullResult <= ullAbsoluteMax - iCurrentDigit)
					ullResult += iCurrentDigit;
				else
				{
					ASSERT(false); // numeric overflow
					ullResult = ullAbsoluteMax;
				}
			}

			if (pszSourceEnd != nullptr)
				*pszSourceEnd = const_cast<char*>(bIsNumber ? szSourceCurrent : szSourceBegin);

			// clamp on overflow
			if (ullResult == ullAbsoluteMax)
				return ((bIsPositive || bIsUnsigned) ? (std::numeric_limits<T>::max)() : (std::numeric_limits<T>::min)());

			return (bIsPositive ? static_cast<T>(ullResult) : -static_cast<T>(ullResult));
		}
	};

	inline crt_class crt;
}