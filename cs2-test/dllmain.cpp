#include <Windows.h>

#include "cheat/cheat.hpp"

static unsigned long run(void* _) {
	static cheat::cs2_internal cs2;
	
	if (!cs2.run())
		return 1;

	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, &run, NULL, 0, NULL);
	}

	return true;
}

