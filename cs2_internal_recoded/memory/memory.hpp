#pragma once

// used: cstdint
#include <cstdint>
// used: pe64
#include "../utils/pe64.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"

#define CONCATENATE_IMPL(LEFT,RIGHT) LEFT##RIGHT
#define CONCATENATE(LEFT,RIGHT) CONCATENATE_IMPL(LEFT,RIGHT)

#define MEM_PAD(SIZE)										\
private:												\
	char CONCATENATE(pad_0,__COUNTER__)[SIZE]; \
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

		template <typename T = uintptr_t>
		T get_absolute_address(T pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0) {
			std::int32_t relativeOffset = *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pRelativeAddress) + nPreOffset);
			return pRelativeAddress + relativeOffset + sizeof(std::int32_t) + nPreOffset + nPostOffset;
		}

		/// get pointer to function of virtual-function table
		/// @returns: pointer to virtual function
		template <typename T = void*>
		[[nodiscard]] __forceinline T get_VFunc(const void* thisptr, std::size_t nIndex)
		{
			return (*static_cast<T* const*>(thisptr))[nIndex];
		}

		template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
		static __forceinline T call_VFunc(CBaseClass* thisptr, Args_t... argList)
		{
			using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
			return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
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
		return winapi.fn_WriteProcessMemory(current_process, reinterpret_cast<LPVOID>(addr), &value, sizeof(value), 0);
	}

	template <typename T>
	bool memory_class::read_memory_size(const std::uintptr_t addr, const T* value, size_t size) {
		return winapi.fn_ReadProcessMemory(current_process, reinterpret_cast<LPCVOID>(addr), &value, size, 0);
	}

	inline std::uintptr_t client_dll_addr = {};

	inline memory_class mem;
}
