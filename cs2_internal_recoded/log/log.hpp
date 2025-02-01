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
// used: string wstring
#include <string>

namespace log_system {
	enum log_level_flags : std::uint16_t
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

		struct end_t
		{
			bool line;
		};

#pragma endregion


		log_class() = default;
		~log_class();

		const bool setup();
		
		const bool attach_console(const wchar_t* console_title = nullptr);
		void detach_console() const;

		const bool open_file();
		void close_file() const;

		log_class& operator<<(const char* message);
		log_class& operator<<(const std::string message);

		log_class& operator<<(const wchar_t* wchar_message);
		log_class& operator<<(const std::wstring wchar_message);
		log_class& operator<<(const bool value);

		log_class& operator<<(const color_t color);
		log_class& operator<<(const mode_t level);
		
		log_class& operator<<(const end_t end);

		template <typename T> requires std::is_integral_v<T>
		log_class& operator<<(const T value)
		{
			write_message(std::to_string(value));
			return *this;
		}


	private:
		bool ended = true;

		HANDLE console_handle = INVALID_HANDLE_VALUE;
		HANDLE file_handle = INVALID_HANDLE_VALUE;

		uint16_t current_log_level = log_level_flags::LOG_INFO;
		uint16_t current_log_color = log_color_flags::LOG_COLOR_FORE_WHITE;

		void write_message(const std::string message);
	};

	const log_class::color_t set_color(uint16_t color = log_color_flags::LOG_COLOR_FORE_WHITE);
	const log_class::mode_t set_level(uint16_t level = log_level_flags::LOG_NONE);

	inline log_class::end_t end = { false };
	inline log_class::end_t endl = { true };
	inline log_class logger;
}



