#include "random.hpp"

#include <string>
#include <random>
#include <algorithm>

namespace utils {
    const string randomString(size_t length, const std::string& charset)
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
}