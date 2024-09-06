#include "log.hpp"


// used: string
#include <string>
// used: exists create_directory
#include <filesystem>
// used: tm
#include <ctime>
// used: format
#include <format>


// used: random_string
#include "../utils/random.hpp"
// used: get_woring_path
#include "../core/core.hpp"
// used: wstring2string
#include "../utils/crt_string.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"

namespace log_system {
	using windows_api::winapi;

	log_class::~log_class() {
#ifdef _LOG_CONSOLE
		detach_console();
#endif

#ifdef _LOG_FILE
		close_file();
#endif
	}

	const bool log_class::setup() {
#ifdef _LOG_CONSOLE
		return attach_console();
#endif

#ifdef _LOG_FILE
		return open_file();
#endif
	}


	const bool log_class::attach_console(const wchar_t* console_title) {
		const std::wstring random_title = utils::random_wstring();
		if (console_title == nullptr) {
			console_title = random_title.c_str();
		}

		if (winapi.fn_AllocConsole() != TRUE)
			return false;

		// if (::AllocConsole() != TRUE)
		// 	return false;

		if (console_handle = winapi.fn_CreateFileW(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr); console_handle == INVALID_HANDLE_VALUE)
			return false;

		if (winapi.fn_SetConsoleTitleW(console_title) != TRUE)
			return false;

		return true;
	}

	void log_class::detach_console() const {
		winapi.fn_CloseHandle(console_handle);

		if (winapi.fn_FreeConsole() != TRUE)
			return;

		if (const HWND console_window = winapi.fn_GetConsoleWindow(); console_window != nullptr)
			winapi.fn_PostMessageW(console_window, WM_CLOSE, 0U, 0L);
	}

	const bool log_class::open_file() {
		std::string log_path = core::get_working_path();

		if (std::filesystem::exists(log_path) == false)
		{
			if (std::filesystem::create_directory(log_path) == false)
			{
				return false;
			}
		}

		log_path += "\\logs";

		if (std::filesystem::exists(log_path) == false)
		{
			if (std::filesystem::create_directory(log_path) == false)
			{
				return false;
			}
		}

		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;
		ss << log_path << "\\log_" << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".txt";
		std::string new_log_file = ss.str();

		if (file_handle = winapi.fn_CreateFileA(new_log_file.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); file_handle == INVALID_HANDLE_VALUE)
			return false;

		std::vector<std::filesystem::path> logFiles;
		for (const auto& entry : std::filesystem::directory_iterator(log_path)) {
			if (entry.path().extension() == ".txt") {
				logFiles.push_back(entry.path());
			}
		}
		std::sort(logFiles.begin(), logFiles.end(), std::greater<std::filesystem::path>());

		while (logFiles.size() > 10) {
			std::filesystem::remove(logFiles.back());
			logFiles.pop_back();
		}

		winapi.fn_WriteFile(file_handle, "\xEF\xBB\xBF", 3UL, nullptr, nullptr);

		return true;
	}

	void log_class::close_file() const {
		winapi.fn_CloseHandle(file_handle);
	}

	void log_class::write_message(const std::string message) {
		const std::time_t time = std::time(nullptr);
		std::tm time_point;
		localtime_s(&time_point, &time);

		const char* level = nullptr;
		uint16_t level_color = LOG_COLOR_FORE_WHITE;


		// [YEAR-MONTH-DAY HOUR:MINUTE:SECOND]
		std::string curr_time;

		if (ended) {
			curr_time = std::format("[{}-{:02}-{:02} {:02}:{:02}:{:02}]", time_point.tm_year + 1900, time_point.tm_mon + 1, time_point.tm_mday, time_point.tm_hour, time_point.tm_min, time_point.tm_sec);

			switch (current_log_level)
			{
			case log_level_flags::LOG_INFO:
				level = " [INFO] ";
				level_color = LOG_COLOR_FORE_GREEN;
				break;
			case log_level_flags::LOG_WARNING:
				level = " [WARNING] ";
				level_color = LOG_COLOR_FORE_YELLOW;
				break;
			case log_level_flags::LOG_ERROR:
				level = " [ERROR] ";
				level_color = LOG_COLOR_FORE_RED;
				break;
			default:
				level = " ";
				level_color = current_log_color;
				break;
			}

			ended = false;
		}
		else {
			curr_time = "";

			level = "";
			level_color = current_log_color;
		}

		std::string result_string = curr_time + level + message;

#ifdef _LOG_CONSOLE
		winapi.fn_SetConsoleTextAttribute(console_handle, static_cast<WORD>(level_color));
		winapi.fn_WriteConsoleA(console_handle, result_string.c_str(), static_cast<DWORD>(result_string.length()), nullptr, nullptr);
#endif

#ifdef _LOG_FILE
		winapi.fn_WriteFile(file_handle, result_string.c_str(), static_cast<DWORD>(result_string.length()), nullptr, nullptr);
#endif
	}


	log_class& log_class::operator<<(const char* message) {
		write_message(message);
		return *this;
	}

	log_class& log_class::operator<<(const std::string message) {
		write_message(message);
		return *this;
	}

	log_class& log_class::operator<<(const wchar_t* wchar_message) {
		write_message(crt::crt.wstring2string(wchar_message));
		return *this;
	}

	log_class& log_class::operator<<(const std::wstring wchar_message) {
		write_message(crt::crt.wstring2string(wchar_message));
		return *this;
	}

	log_class& log_class::operator<<(const bool value) {
		const char* boolean = value ? "true" : "false";
		write_message(boolean);
		return *this;
	}

	log_class& log_class::operator<<(const color_t color) {
		current_log_color = color.clr;
#ifdef _LOG_CONSOLE
		winapi.fn_SetConsoleTextAttribute(console_handle, static_cast<WORD>(color.clr));
#endif
		return *this;
	}

	log_class& log_class::operator<<(const mode_t level) {
		current_log_level = level.mode;
		return *this;
	}

	log_class& log_class::operator<<(const end_t end) {
		if (end.line)
			write_message("\n");
		ended = true;

		return *this;
	}

	const log_class::color_t set_color(uint16_t color) {
		return { color };
	}

	const log_class::mode_t set_level(uint16_t level) {
		return { level };
	}
}