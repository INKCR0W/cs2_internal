#include "memory.hpp"

// used: __readfsdword
#include <intrin.h>
// used: format
#include <format>
// used: transform
#include <algorithm>
// used: towlower
#include <cwctype>
// used: string_compare wstring2string
#include "../utils/crt_string.hpp"
// used: pe64
#include "../utils/pe64.hpp"

namespace memory {
	bool memory_class::setup() {
		current_process = reinterpret_cast<HANDLE>(-1);

		return true;
	}

	void* memory_class::get_module_base_handle(const char* module_name) {
		const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

		std::wstring module_name_w = ::crt::crt.string2wstring(module_name);
		std::wstring base_dll_name = {};

		std::ranges::transform(module_name_w, module_name_w.begin(), ::tolower);

		if (module_name_w == L"")
			return pPEB->ImageBaseAddress;

		void* pModuleBase = nullptr;
		for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			base_dll_name = pEntry->BaseDllName.Buffer;
			std::ranges::transform(base_dll_name, base_dll_name.begin(), ::tolower);
			if (pEntry->FullDllName.Buffer != nullptr && ::crt::crt.string_compare(base_dll_name.c_str(), module_name_w.c_str()) == 0)
			{
				pModuleBase = pEntry->DllBase;
				break;
			}
		}

		if (pModuleBase == nullptr)
			return nullptr;

		return pModuleBase;
	}
}