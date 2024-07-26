#include <Windows.h>
#include <iostream>
#include <mutex>

#include "features/cheat.hpp"
#include "utils/debug.hpp"
#include "utils/random.hpp"

cheat::cs2_internal cs2;

static unsigned long run(void* _) {
	if (!cs2.run()) {
		cs2.~cs2_internal();
		return 1;
	}

	std::cout << "Done" << std::endl;

	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

	SetConsoleTitle(utils::randomString(0ULL).c_str());

	// std::cerr.rdbuf(std::cout.rdbuf());

	std::cout << "HOOKING..." << std::endl;;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		cs2.init();
		if (CreateThread(NULL, 0, &run, NULL, 0, NULL) == NULL) {
			dbg::dbg_print(std::format("创建线程失败 {}", GetLastError()));
		}
		break;
	default:
		break;
	}

	return true;
}
