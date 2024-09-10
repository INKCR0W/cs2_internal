const char* VERSION = "1.0.0";

#include <iostream>

// used: fn_DisableThreadLibraryCalls
#include "windows_api/win_api.hpp"
// used: cs2_internal
#include "cheat/cheat.hpp"
// used: xorstr_
#include "utils/xorstr.hpp"
// used: string_compare
#include "utils/crt_string.hpp"


#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

std::string get_version() {
    std::string result;
    HINTERNET hInternet = InternetOpenA("VersionCheck", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetOpenUrlA(hInternet, xorstr_("http://ver.crow.pub"), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect) {
            char buffer[1024];
            DWORD bytesRead;
            while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                result.append(buffer, bytesRead);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }

    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

bool __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	using namespace cheat;
    using namespace windows_api;

	// windows api initialization 
	if (!windows_api::winapi.setup()) {
		return false;
	}

    // version check
    std::string lastest_version = get_version();
    if (crt::crt.string_compare(lastest_version.c_str(), VERSION) != 0) {
        winapi.fn_MessageBoxA(NULL, (xorstr_("A new version: ") + lastest_version + xorstr_(" is available.") + VERSION).c_str(), xorstr_("ERROR"), MB_OK | MB_ICONMASK);
        windows_api::winapi.fn_ShellExecuteA(NULL, xorstr_("open"), xorstr_("https://www.crow.pub/down"), NULL, NULL, SW_SHOWNORMAL);
        exit(0);
    }

    // core functions
	windows_api::winapi.fn_DisableThreadLibraryCalls(hModule);

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

