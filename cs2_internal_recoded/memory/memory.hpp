#pragma once

// used: cstdint
#include <cstdint>
// used: pe64
#include "../utils/pe64.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"

#define CS_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT

#define MEM_PAD(SIZE)										\
private:												\
	char CS_CONCATENATE(pad_0, __COUNTER__)[SIZE]; \
public:

namespace memory {
	using namespace windows_api;

	struct pattern_token_t
	{
		uint8_t byte;
		bool wc;
	};



	class memory_class {
	public:
		bool setup();

		void* get_module_base_handle(const char* module_name);
		const uint64_t get_module_base_size(const char* module_name);
		void* get_export_address(const void* hModuleBase, const char* szProcedureName);

		[[nodiscard]] std::uint8_t* resolve_relative_address(std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset);

		/// get pointer to function of virtual-function table
		/// @returns: pointer to virtual function
		template <typename T = void*>
		[[nodiscard]] __forceinline T get_VFunc(const void* thisptr, std::size_t nIndex)
		{
			return (*static_cast<T* const*>(thisptr))[nIndex];
		}

		const uintptr_t find_pattern(const char* module_name, const char* pattern);

		void* heap_alloc(const std::size_t nSize);
		void heap_free(void* pMemory);

		template <typename T>
		T read_memory(const std::uintptr_t addr);

		template <typename T>
		bool write_memory(const std::uintptr_t addr, const T& value);

		template <typename T>
		bool read_memory_size(const std::uintptr_t addr, const T* value, size_t size);

	private:
		HANDLE current_process = reinterpret_cast<HANDLE>(-1);

		const _LDR_DATA_TABLE_ENTRY* find_module(const char* module_name);
		__forceinline uint8_t hex_to_char(const char in);
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
