#pragma once

#include "../windows_api/win_api.hpp"

namespace memory {
	using namespace windows_api;
	class memory_class {
	public:
		bool setup();

		void* get_module_base_handle(const char* module_name);

		template <typename T>
		T read_memory(const std::uintptr_t addr);

		template <typename T>
		bool write_memory(const std::uintptr_t addr, const T& value);

		template <typename T>
		bool read_memory_size(const std::uintptr_t addr, const T* value, size_t size);

	private:
		HANDLE current_process = reinterpret_cast<HANDLE>(-1);
	};

	template <typename T>
	T memory_class::read_memory(const std::uintptr_t addr) {
		T value = {};
		winapi.fn_ReadProcessMemory(current_process, reinterpret_cast<LPCVOID>(addr), &value, sizeof(value), 0);
		return value;
	}

	template <typename T>
	bool memory_class::write_memory(const std::uintptr_t addr, const T& value) {
		return winapi.fn_WriteProcessMemory(current_process, reinterpret_cast<LPCVOID>(addr), &value, sizeof(value), 0);
	}

	template <typename T>
	bool memory_class::read_memory_size(const std::uintptr_t addr, const T* value, size_t size) {
		return winapi.fn_ReadProcessMemory(current_process, reinterpret_cast<LPCVOID>(addr), &value, size, 0);
	}

	inline memory_class mem;
}
