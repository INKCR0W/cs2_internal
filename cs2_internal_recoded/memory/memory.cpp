#include "memory.hpp"

// used: __readfsdword
#include <intrin.h>
// used: MessageBox
#include <Windows.h>
// used: format
#include <format>
// used: string_compare
#include "../utils/crt_string.hpp"
// used: pe64
#include "../utils/pe64.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"


namespace memory {
	void* memory_class::get_module_base_handle(const wchar_t* module_name) {
		const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

		if (module_name == nullptr)
			return pPEB->ImageBaseAddress;

		void* pModuleBase = nullptr;
		for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

			if (pEntry->FullDllName.Buffer != nullptr && crt::crt.string_compare(module_name, pEntry->BaseDllName.Buffer) == 0)
			{
				pModuleBase = pEntry->DllBase;
				break;
			}
		}

		//if (pModuleBase == nullptr)
			//throw std::runtime_error(std::format("Failed to get module handle : {}", module_name));

		return pModuleBase;
	}
}