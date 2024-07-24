#include <Windows.h>
#include <iostream>

#include "cheat/cheat.hpp"

static unsigned long run(void* _) {
	static cheat::cs2_internal cs2;
	
	if (!cs2.run())
		return 1;

	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

	SetConsoleTitle(L"My Console");

	std::cout << "HOOKING...\n";
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, &run, NULL, 0, NULL);
		std::cout << "HOOKED\n";
	}

	return true;
}

