#include <iostream>

// used: fn_DisableThreadLibraryCalls
#include "windows_api/win_api.hpp"

#include "cheat/cheat.hpp"

bool __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	using namespace cheat;
	using namespace windows_api;

	if (!winapi.setup()) {
		return false;
	}

	winapi.fn_DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		if (cs2_internal::get_instance().setup()) {
			cs2_internal::get_instance().run();
		}
		else {
			return false;
		}
    }

    return true;
}






