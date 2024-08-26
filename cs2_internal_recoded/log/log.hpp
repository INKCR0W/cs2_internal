#pragma once

#ifdef _DEBUG
#define _LOG_CONSOLE
#else
#define _LOG_FILE
#endif

// used: windows api
#include <Windows.h>
// used: source_location
#include <source_location>

namespace log_system {
	enum log_level : std::uint8_t
	{
		LOG_NONE = 0,
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR
	};

	// @credits: https://github.com/maecry/asphyxia-cs2
	enum log_color_flags : std::uint16_t
	{
		LOG_COLOR_FORE_BLUE = FOREGROUND_BLUE,
		LOG_COLOR_FORE_GREEN = FOREGROUND_GREEN,
		LOG_COLOR_FORE_RED = FOREGROUND_RED,
		LOG_COLOR_FORE_INTENSITY = FOREGROUND_INTENSITY,
		LOG_COLOR_FORE_GRAY = FOREGROUND_INTENSITY,
		LOG_COLOR_FORE_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
		LOG_COLOR_FORE_MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED,
		LOG_COLOR_FORE_YELLOW = FOREGROUND_GREEN | FOREGROUND_RED,
		LOG_COLOR_FORE_BLACK = 0U,
		LOG_COLOR_FORE_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

		LOG_COLOR_BACK_BLUE = BACKGROUND_BLUE,
		LOG_COLOR_BACK_GREEN = BACKGROUND_GREEN,
		LOG_COLOR_BACK_RED = BACKGROUND_RED,
		LOG_COLOR_BACK_INTENSITY = BACKGROUND_INTENSITY,
		LOG_COLOR_BACK_GRAY = BACKGROUND_INTENSITY,
		LOG_COLOR_BACK_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN,
		LOG_COLOR_BACK_MAGENTA = BACKGROUND_BLUE | BACKGROUND_RED,
		LOG_COLOR_BACK_YELLOW = BACKGROUND_GREEN | BACKGROUND_RED,
		LOG_COLOR_BACK_BLACK = 0U,
		LOG_COLOR_BACK_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

		/* [internal] */
		LOG_COLOR_DEFAULT = LOG_COLOR_FORE_WHITE | LOG_COLOR_BACK_BLACK
	};

	class log_class {
	public:

#pragma region structs

		struct color_t
		{
			uint16_t clr;
		};

		struct mode_t
		{
			uint16_t mode;
		};

#pragma endregion


		log_class();
		~log_class();
		
		const bool attach_console(const wchar_t* console_title = nullptr);
		void detach_console() const;

		const bool open_file();
		void close_file() const;

		log_class& operator<<(const std::string message);
		log_class& operator<<(const std::wstring wchar_message);
		log_class& operator<<(const bool value);

		log_class& operator<<(const color_t color);

		log_class& operator<<(const mode_t level);

		const color_t set_color(uint16_t color);
		const mode_t set_level(uint16_t level);



	private:
		HANDLE console_handle = INVALID_HANDLE_VALUE;
		HANDLE file_handle = INVALID_HANDLE_VALUE;

		uint8_t current_log_level = log_level::LOG_INFO;

		void write_message(const std::string message);
	};
}