#include <Windows.h>
#include <iostream>
#include <mutex>

#include "features/cheat.hpp"
#include "utils/debug.hpp"
#include "utils/random.hpp"

static unsigned long run(void* _) {
	if (!cheat::cs2_internal::get_instance().run()) {
		cheat::cs2_internal::get_instance().~cs2_internal();
		return 1;
	}

	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
#ifdef _DEBUG_
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

	SetConsoleTitle(utils::randomString(0ULL).c_str());

	// std::cerr.rdbuf(std::cout.rdbuf());

#endif

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		cheat::cs2_internal::get_instance().init();
		if (CreateThread(NULL, 0, &run, NULL, 0, NULL) == NULL) {
			dbg::dbg_print(std::format("创建线程失败 {}", GetLastError()));
		}
		break;
	default:
		break;
	}

	return true;
}
