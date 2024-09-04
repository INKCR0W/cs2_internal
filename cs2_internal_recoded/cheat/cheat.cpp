#include "cheat.hpp"

// used: this_thread::sleep_for
#include <thread>

// used: xorstr_
#include "../utils/xorstr.hpp"
// used: mem
#include "../memory/memory.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"
// used: modules
#include "../game/game_modules.hpp"


namespace cheat {
	bool cs2_internal::setup() {
		using namespace log_system;
		using namespace utils;

		if (!windows_api::winapi.setup()) {
			return false;
		}

		if (!memory::mem.setup()) {
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


	DWORD __stdcall cs2_internal::ren(LPVOID lpParameter)
	{
		cs2_internal* pThis = static_cast<cs2_internal*>(lpParameter);

		log_system::logger << "create_move (48 8B C4 4C 89 48 20 55) : " << memory::mem.find_pattern(modules::client_dll, "48 8B C4 4C 89 48 20 55") << log_system::endl;

		while (true) {
			log_system::logger << xorstr_("running") << log_system::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}

		return 0;
	}

	bool cs2_internal::run() {
		// TODO
		Sleep(1000);

		if (windows_api::winapi.fn_CreateThread(NULL, 0, &cs2_internal::ren, this, 0, NULL) == NULL)
			return false;

		return true;
	}


	cs2_internal& cs2_internal::get_instance() {
		static cs2_internal instance;
		return instance;
	}
}
