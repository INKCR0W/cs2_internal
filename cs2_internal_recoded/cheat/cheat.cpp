#include "cheat.hpp"

#include <iostream>
#include <Windows.h>
#include <string>

#include "../utils/fnv1a.hpp"
#include "../utils/xorstr.hpp"
#include "../windows_api/win_api.hpp"


namespace cheat {
	bool cs2_internal::setup() {
		using namespace windows_api;
		using namespace log_system;

		if (!winapi.setup()) {
			return false;
		}

		if (logger.setup()) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("The log system initialization is complete") << set_level() << endl;
		}
		else {
			return false;
		}

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
