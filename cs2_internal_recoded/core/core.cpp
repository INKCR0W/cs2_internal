#include "core.hpp"

// used: getenv
#include <cstdlib>
// used: exists create_directory
#include <filesystem>
// used: windows api
#include "../windows_api/win_api.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"

namespace core {
	std::string get_working_path()
	{
		if (const char* appdata_path = std::getenv("APPDATA"); appdata_path != nullptr) {
			std::string path = appdata_path;
			path += "\\himc";
			return path;
		}
		else {
			return "";
		}
	}
	std::string get_himc_version()
	{
		std::string result;
		void* hInternet = windows_api::winapi.fn_InternetOpenA("VersionCheck", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		if (hInternet) {
			void* hConnect = windows_api::winapi.fn_InternetOpenUrlA(hInternet, xorstr_("http://ver.crow.pub"), NULL, 0, INTERNET_FLAG_RELOAD, 0);
			if (hConnect) {
				char buffer[1024];
				DWORD bytesRead;
				while (windows_api::winapi.fn_InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
					result.append(buffer, bytesRead);
				}
				windows_api::winapi.fn_InternetCloseHandle(hConnect);
			}
			windows_api::winapi.fn_InternetCloseHandle(hInternet);
		}

		result.erase(result.find_last_not_of(" \n\r\t") + 1);
		return result;
	}
}