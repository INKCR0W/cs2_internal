#include <iostream>
#include <Windows.h>

#include "cheat.hpp"
#include "../utils/fnv1a.hpp"

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


		logger << "fnv1a test start" << endl;
		using std::string;

		string str = "This is a string that should be encrypted";

		logger << "hash_ct:\t" << utils::fnv1a.hash_ct(str.c_str()) << endl;
		logger << "hash:\t" << utils::fnv1a.hash(str.c_str()) << endl;



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
