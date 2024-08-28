#include <iostream>
#include <Windows.h>

#include "cheat.hpp"

namespace cheat {
	bool cs2_internal::setup() {
		// TODO

		using namespace log_system;

		log_class& logger = log_class::get_instance();
		logger.setup();

		logger << logger.set_level(log_level::LOG_INFO) << "logger inited\n"
			<< logger.set_level(log_level::LOG_WARNING) << "warning\n"
			<< logger.set_level(log_level::LOG_ERROR) << "error\n"
			<< logger.set_level(log_level::LOG_NONE) << "none\n"
			<< logger.set_color(log_color_flags::LOG_COLOR_FORE_BLUE) << "blue\n"
			<< logger.set_color(log_color_flags::LOG_COLOR_FORE_GREEN) << "green\n";

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
