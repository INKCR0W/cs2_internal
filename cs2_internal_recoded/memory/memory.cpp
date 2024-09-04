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



namespace memory {
	bool memory_class::setup() {
		current_process = reinterpret_cast<HANDLE>(-1);

		return true;
	}

	const _LDR_DATA_TABLE_ENTRY* memory_class::find_module(const char* module_name) {
		const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

		std::wstring module_name_w = ::crt::crt.string2wstring(module_name);
		std::wstring base_dll_name = {};

		std::ranges::transform(module_name_w, module_name_w.begin(), ::tolower);

		if (module_name_w == L"")
			return {};

		for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			base_dll_name = pEntry->BaseDllName.Buffer;
			std::ranges::transform(base_dll_name, base_dll_name.begin(), ::tolower);
			if (pEntry->FullDllName.Buffer != nullptr && ::crt::crt.string_compare(base_dll_name.c_str(), module_name_w.c_str()) == 0)
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


	__forceinline uint8_t memory_class::hex_to_char(const char in)
	{
		return static_cast<char>(isdigit(in) ? (in - '0') : (in - 'A' + 10));
	}


}