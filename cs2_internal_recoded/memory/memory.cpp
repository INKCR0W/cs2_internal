#include "memory.hpp"

// used: __readfsdword
#include <intrin.h>
// used: MessageBox
#include <Windows.h>
// used: format
#include <format>
// used: transform
#include <algorithm>
// used: string_compare wstring2string
#include "../utils/crt_string.hpp"
// used: pe64
#include "../utils/pe64.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"

#include "../utils/fnv1a.hpp"

#include "../log/log.hpp"


namespace memory {
	void* memory_class::get_module_base_handle(const char* module_name) {
		const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

		std::wstring module_name_w = ::crt::crt.string2wstring(module_name);

		if (module_name_w == L"")
			return pPEB->ImageBaseAddress;

		void* pModuleBase = nullptr;
		for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			// ::log_system::logger << pEntry->BaseDllName.Buffer << " " << module_name << ::log_system::endl;
			if (pEntry->FullDllName.Buffer != nullptr && ::crt::crt.string_compare(pEntry->BaseDllName.Buffer, module_name_w.c_str()) == 0)
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