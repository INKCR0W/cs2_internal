#pragma once

// used: LoadLibraryA
#include <Windows.h>

// used: string
#include <string>
// used: unordered_map
#include <unordered_map>
// used: function
#include <functional>
// used: runtime_error
#include <stdexcept>

namespace windows_api{
	class win_api {
	public:
		win_api() = default;
		~win_api() = default;

		const bool setup();

        template<typename... Args>
        auto operator()(const std::string& func_name, Args&&... args) -> decltype(auto) {
            auto it = func_ptr.find(func_name);
            if (it == func_ptr.end()) {
                throw std::runtime_error("Function not found: " + func_name);
            }

            using FuncType = std::function<decltype(std::invoke(reinterpret_cast<std::add_pointer_t<decltype(args)...>>(nullptr), std::forward<Args>(args)...))(Args...)>;

            FuncType func = reinterpret_cast<FuncType>(it->second);
            return std::invoke(func, std::forward<Args>(args)...);
        }

	private:
        bool inited = false;

		HMODULE kernel32_dll = nullptr;
		HMODULE user32_dll = nullptr;

        std::unordered_map<std::string, void*> func_ptr;
        std::unordered_map<std::string, HMODULE> func_info;
	};

    inline win_api winapi;
}

