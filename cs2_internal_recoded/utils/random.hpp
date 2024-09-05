#pragma once

// used: string
#include <string>
// used: random_device mt19937 uniform_int_distribution
#include <random>
// used: generate
#include <algorithm>
// used: string2wstring
#include "../utils/crt_string.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"

namespace utils {
    /// <summary>
    /// Return a random string¡£
    /// </summary>
    /// <param name="length">
    /// The <c>length</c> of string
    /// </param>
    /// <param name="charset">
    /// Char set for string
    /// </param>
    /// <returns>The <c>random</c> string</returns>
    const std::string random_string(size_t length = 0, const std::string& charset = xorstr_("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()_+=-[]{};':,.<>/?")) noexcept
    {
        std::random_device rd;
        std::mt19937 generator(rd());

        if (length == 0) {
            std::uniform_int_distribution<std::size_t> length_dist(16, 32);
            length = length_dist(generator);
        }

        std::string result;
        result.resize(length);

        std::uniform_int_distribution<std::size_t> distribution(0, charset.size() - 1);
        std::generate(result.begin(), result.end(), [&]() { return charset[distribution(generator)]; });

        return result;
    }
    
    /// <summary>
    /// Return a random wchar string¡£
    /// </summary>
    /// <param name="length">
    /// The <c>length</c> of string
    /// </param>
    /// <param name="charset">
    /// Char set for string
    /// </param>
    /// <returns>The <c>random</c> wchar string</returns>
    const std::wstring random_wstring(size_t length = 0, const std::string& charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()_+=-[]{};':,.<>/?") noexcept
    {
        return crt::crt.string2wstring(random_string(length, charset));
    }
}