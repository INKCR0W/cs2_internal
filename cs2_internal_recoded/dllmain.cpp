#include <Windows.h>
#include <iostream>

// used: randomString
//#include "utils/random.hpp"

#include "cheat/cheat.hpp"

bool __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
#ifdef _DEBUG
	//AllocConsole();

	//FILE* fDummy;
	//freopen_s(&fDummy, "CONOUT$", "w", stdout);
	//freopen_s(&fDummy, "CONOUT$", "w", stderr);
	//freopen_s(&fDummy, "CONIN$", "r", stdin);

	//SetConsoleTitle(utils::randomString(0ULL).c_str());

	// @todo: save the console window handle, so we can close it when the DLL is about to unload
	// cheat::cs2_internal::get_instance().set_console(GetConsoleWindow());
#endif


	DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

    }

    return TRUE;
}

