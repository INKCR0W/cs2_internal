#include "memory.hpp"


// used: optional
#include <optional>
// used: vector
#include <vector>
// used: __readfsdword
#include <intrin.h>
// used: transform
#include <algorithm>
// used: towlower
#include <cwctype>
// used: string_compare wstring2string
#include "../utils/crt_string.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"
// used: client_dll
#include "../game/game_modules.hpp"
// used: logsystem
#include "../log/log.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"


namespace memory {
	bool memory_class::setup() {
		current_process = reinterpret_cast<HANDLE>(-1);

		client_dll_addr = reinterpret_cast<std::uintptr_t>(get_module_base_handle(modules::client_dll));
		if (client_dll_addr == 0) {
			using namespace log_system;
			logger << set_level(log_level_flags::LOG_ERROR) << xorstr_("Failed to find client.dll, wait until the game is fully loaded before injecting.") << set_level() << endl;
			return false;
		}

		return true;
	}

	const _LDR_DATA_TABLE_ENTRY* memory_class::find_module(const char* module_name) {
		const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

		std::wstring module_name_w = crt::crt.string2wstring(module_name);
		std::wstring base_dll_name = {};

		std::ranges::transform(module_name_w, module_name_w.begin(), ::tolower);

		if (module_name_w == L"")
			return {};

		for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			base_dll_name = pEntry->BaseDllName.Buffer;
			std::ranges::transform(base_dll_name, base_dll_name.begin(), ::tolower);
			if (pEntry->FullDllName.Buffer != nullptr && crt::crt.string_compare(base_dll_name.c_str(), module_name_w.c_str()) == 0)
			{
				return pEntry;
			}
		}

		return nullptr;
	}

	void* memory_class::get_module_base_handle(const char* module_name) {
		const _LDR_DATA_TABLE_ENTRY* module_entry = find_module(module_name);

		if (module_entry == nullptr)
			return nullptr;

		return module_entry->DllBase;
	}

	const uint64_t memory_class::get_module_base_size(const char* module_name) {
		const _LDR_DATA_TABLE_ENTRY* module_entry = find_module(module_name);

		if (module_entry == nullptr)
			return 0;

		return module_entry->SizeOfImage;
	}

	const uintptr_t memory_class::find_pattern(const char* module_name, const char* pattern)
	{
		uintptr_t module_base = reinterpret_cast<uintptr_t>(get_module_base_handle(module_name));
		const uint64_t module_size = get_module_base_size(module_name);

		std::vector<pattern_token_t> tokens;
		std::optional<uint8_t> cur{};
		for (int i{}; i < ::crt::crt.str_length(pattern); ++i)
		{
			if (pattern[i] == ' ')
				continue;

			if (pattern[i] != '?')
			{
				if (cur)
				{
					tokens.emplace_back(pattern_token_t{ static_cast<uint8_t>(*cur * 16 + hex_to_char(pattern[i])), false });
					cur = {};
					continue;
				}

				cur = hex_to_char(pattern[i]);
			}
			else
			{
				cur = {};
				tokens.emplace_back(pattern_token_t{ 0, true });
			}
		}

		const auto compare = [&](uintptr_t addr)
			{
				for (auto& [b, w] : tokens)
				{
					if (w)
					{
						addr++;
						continue;
					}

					if (b != *reinterpret_cast<uint8_t*>(addr++))
						return false;
				}

				return true;
			};

		for (uintptr_t i{ module_base }; i < static_cast<uintptr_t>(module_base + module_size); ++i)
			if (compare(i))
				return i;

		return 0;
	}

	void* memory_class::heap_alloc(const std::size_t nSize) {
		const HANDLE hHeap = windows_api::winapi.fn_GetProcessHeap();
		return windows_api::winapi.fn_HeapAlloc(hHeap, 0UL, nSize);
	}

	void memory_class::heap_free(void* memory) {
		if (memory != nullptr)
		{
			const HANDLE hHeap = windows_api::winapi.fn_GetProcessHeap();
			windows_api::winapi.fn_HeapFree(hHeap, 0UL, memory);
		}
	}


	__forceinline uint8_t memory_class::hex_to_char(const char in)
	{
		return static_cast<char>(isdigit(in) ? (in - '0') : (in - 'A' + 10));
	}



	void* memory_class::get_export_address(const void* hModuleBase, const char* szProcedureName)
	{
		const auto pBaseAddress = static_cast<const std::uint8_t*>(hModuleBase);

		const auto pIDH = static_cast<const IMAGE_DOS_HEADER*>(hModuleBase);
		if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
			return nullptr;

		const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS64*>(pBaseAddress + pIDH->e_lfanew);
		if (pINH->Signature != IMAGE_NT_SIGNATURE)
			return nullptr;

		const IMAGE_OPTIONAL_HEADER64* pIOH = &pINH->OptionalHeader;
		const std::uintptr_t nExportDirectorySize = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
		const std::uintptr_t uExportDirectoryAddress = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

		if (nExportDirectorySize == 0U || uExportDirectoryAddress == 0U)
		{
			return nullptr;
		}

		const auto pIED = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(pBaseAddress + uExportDirectoryAddress);
		const auto pNamesRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfNames);
		const auto pNameOrdinalsRVA = reinterpret_cast<const std::uint16_t*>(pBaseAddress + pIED->AddressOfNameOrdinals);
		const auto pFunctionsRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfFunctions);

		// Perform binary search to find the export by name
		std::size_t nRight = pIED->NumberOfNames, nLeft = 0U;
		while (nRight != nLeft)
		{
			// Avoid INT_MAX/2 overflow
			const std::size_t uMiddle = nLeft + ((nRight - nLeft) >> 1U);
			const int iResult = crt::crt.string_compare(szProcedureName, reinterpret_cast<const char*>(pBaseAddress + pNamesRVA[uMiddle]));

			if (iResult == 0)
			{
				const std::uint32_t uFunctionRVA = pFunctionsRVA[pNameOrdinalsRVA[uMiddle]];

				// Check if it's a forwarded export
				if (uFunctionRVA >= uExportDirectoryAddress && uFunctionRVA - uExportDirectoryAddress < nExportDirectorySize)
				{
					// Forwarded exports are not supported
					break;
				}

				return const_cast<std::uint8_t*>(pBaseAddress) + uFunctionRVA;
			}

			if (iResult > 0)
				nLeft = uMiddle + 1;
			else
				nRight = uMiddle;
		}

		// Export not found
		return nullptr;
	}

	std::uint8_t* memory_class::resolve_relative_address(std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
	{
		std::uint32_t nRVA = *reinterpret_cast<std::uint32_t*>(nAddressBytes + nRVAOffset);
		std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;

		return reinterpret_cast<std::uint8_t*>(nRVA + nRIP);
	}

}