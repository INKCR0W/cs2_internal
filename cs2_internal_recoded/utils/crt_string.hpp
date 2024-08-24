#pragma once

// used: is_same_v
#include <type_traits>
// used: string wstring
#include <string>
// used: runtime_error
#include <stdexcept>

namespace crt {
	template <typename any_char> requires (std::is_same_v<any_char, char> || std::is_same_v<any_char, wchar_t>)
	constexpr std::size_t str_length(const any_char* tszSource)
	{
		const any_char* tszSourceEnd = tszSource;

		while (*tszSourceEnd != any_char('\0'))
			++tszSourceEnd;

		return tszSourceEnd - tszSource;
	}

	template <typename any_char> requires (std::is_same_v<any_char, char> || std::is_same_v<any_char, wchar_t>)
	int StringLengthMultiByte(const any_char* tszBegin, const any_char* tszEnd = nullptr)
	{
		int nOctetCount = 0;

		// go through each character until terminating null up to end if given
		while (*tszBegin != any_char('\0') && (tszEnd == nullptr || tszBegin < tszEnd))
		{
			if (const std::uint32_t uChar = static_cast<std::uint32_t>(*tszBegin++); uChar < 0x80)
				nOctetCount += 1;
			else if (uChar < 0x800)
				nOctetCount += 2;
			else if (uChar < 0x10000)
				nOctetCount += 3;
			else if (uChar <= 0x7FFFFFFF)
				nOctetCount += 4;
		}

		return nOctetCount;
	}

	inline std::ptrdiff_t CharMultiByteFromUTF32(char* szOutBuffer, const std::size_t nOutBufferSize, const std::uint32_t uChar)
	{
		// utf-8
		if (uChar < 0x80)
		{
			szOutBuffer[0] = static_cast<char>(uChar);
			return 1;
		}
		// utf-16
		if (uChar < 0x800 && nOutBufferSize >= 2U)
		{
			szOutBuffer[0] = static_cast<char>(0xC0 + (uChar >> 6U));
			szOutBuffer[1] = static_cast<char>(0x80 + (uChar & 0x3F));
			return 2;
		}
		// utf-16
		if (uChar < 0x10000 && nOutBufferSize >= 3U)
		{
			szOutBuffer[0] = static_cast<char>(0xE0 + (uChar >> 12U));
			szOutBuffer[1] = static_cast<char>(0x80 + ((uChar >> 6U) & 0x3F));
			szOutBuffer[2] = static_cast<char>(0x80 + (uChar & 0x3F));
			return 3;
		}
		// utf-32
		if (uChar <= 0x10FFFF && nOutBufferSize >= 4U)
		{
			szOutBuffer[0] = static_cast<char>(0xF0 + (uChar >> 18U));
			szOutBuffer[1] = static_cast<char>(0x80 + ((uChar >> 12U) & 0x3F));
			szOutBuffer[2] = static_cast<char>(0x80 + ((uChar >> 6U) & 0x3F));
			szOutBuffer[3] = static_cast<char>(0x80 + (uChar & 0x3F));
			return 4;
		}

		// invalid code point
		return 0;
	}

	__forceinline std::ptrdiff_t str_unicode2multibyte(char* szOutBuffer, const std::size_t nOutBufferLength, const wchar_t* wszBegin, const wchar_t* wszEnd = nullptr)
	{
		char* pBufferBegin = szOutBuffer;
		const char* pBufferEnd = szOutBuffer + nOutBufferLength;

		while (pBufferBegin < pBufferEnd - 1 && (wszEnd == nullptr || wszBegin < wszEnd) && *wszBegin != L'\0')
			pBufferBegin += CharMultiByteFromUTF32(pBufferBegin, pBufferEnd - pBufferBegin - 1, *wszBegin++);

		*pBufferBegin = '\0';
		return pBufferBegin - szOutBuffer;
	}

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
}