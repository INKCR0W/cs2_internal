#include "log.hpp"

// used: random_string
#include "../utils/random.hpp"
// used: get_woring_path
#include "../core/core.hpp"
// used: string
#include <string>
// used: exists create_directory
#include <filesystem>

namespace log_system {
	log_class::log_class() {
#ifdef _LOG_CONSOLE
		attach_console();
#elif _LOG_FILE
		open_file();
#endif
	}

	log_class::~log_class() {
#ifdef _LOG_CONSOLE
		detach_console();
#elif _LOG_FILE
		close_file();
#endif
	}

	const bool log_class::attach_console(const char* console_title = nullptr) {
		if (console_title == nullptr) {
			console_title = utils::random_string().c_str();
		}
		
		if (::AllocConsole() != true)
			return false;

		if (console_handle = ::CreateFileW(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr); console_handle == INVALID_HANDLE_VALUE)
			return false;

		if (::SetConsoleTitle(console_title) != TRUE)
			return false;

		return true;
	}

	void log_class::detach_console() const {
		::CloseHandle(console_handle);

		if (::FreeConsole() != TRUE)
			return;

		if (const HWND console_window = ::GetConsoleWindow(); console_window != nullptr)
			::PostMessageW(console_window, WM_CLOSE, 0U, 0L);
	}

	const bool log_class::open_file() {
		std::string log_path = core::get_working_path();
		log_path += "\\logs";

		if (std::filesystem::exists(log_path) == false)
		{
			if (std::filesystem::create_directory(log_path) == false)
			{
				return false;
			}
		}

		log_path += "\\himc.log";

		if (file_handle = ::CreateFile(log_path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); file_handle == INVALID_HANDLE_VALUE)
			return false;

		// insert UTF-8 BOM
		::WriteFile(file_handle, "\xEF\xBB\xBF", 3UL, nullptr, nullptr);

		return true;
	}

	void log_class::close_file() const {
		::CloseHandle(file_handle);
	}
}