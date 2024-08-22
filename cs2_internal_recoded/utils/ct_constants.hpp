#pragma once

namespace utils {
    template<typename T, T value>
    struct constant_holder {
        static constexpr T val = value;
    };

    template<typename T, T value>
    constexpr T get_constant() {
        return constant_holder<T, value>::val;
    }
}