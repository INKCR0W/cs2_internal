#include <iostream>
#include <Windows.h>

#include "cheat.hpp"
#include "../utils/fnv1a.hpp"
#include "../third_party/xorstr.hpp"

#include <string>

namespace cheat {
	bool cs2_internal::setup() {
		// TODO

		using namespace log_system;

		logger.setup();

		logger << set_level(log_level::LOG_INFO) << "logger inited" << endl
			<< set_level(log_level::LOG_WARNING) << "warning" << endl
			<< set_level(log_level::LOG_ERROR) << "error" << endl
			<< set_level(log_level::LOG_NONE) << "none" << endl
			<< set_color(log_color_flags::LOG_COLOR_FORE_BLUE) << "blue" << endl
			<< set_color(log_color_flags::LOG_COLOR_FORE_GREEN) << "green" << endl;

		logger << set_color(log_color_flags::LOG_COLOR_FORE_WHITE) << end;

		logger << "fnv1a test start" << endl;
		using std::string;

		logger << "xor test start" << endl;
		logger << "xor:   " << xorstr_("This is a string that should be encrypted") << endl;


		return true;
	}

	bool cs2_internal::run() {
		// TODO
		Sleep(3000);

		return true;
	}

	cs2_internal& cs2_internal::get_instance() {
		static cs2_internal instance;
		return instance;
	}
}
