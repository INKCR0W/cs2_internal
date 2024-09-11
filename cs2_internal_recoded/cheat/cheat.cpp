#include "cheat.hpp"

// used: xorstr_
#include "../utils/xorstr.hpp"
// used: mem
#include "../memory/memory.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"
// used: modules
#include "../game/game_modules.hpp"
// used: interfaces
#include "../core/interfaces.hpp"
// used: hooks
#include "../core/hooks.hpp"
// used: inputsystem
#include "../core/inputsystem.hpp"
// used: render
#include "../render/render.hpp"
// used: get_himc_version
#include "../core/core.hpp"

namespace cheat {
	cs2_internal::~cs2_internal() {
		inputsystem::destroy();
		hook::destroy();
	}

	bool cs2_internal::setup() {
		using namespace log_system;
		using namespace utils;

		if (!memory::mem.setup()) {
			return false;
		}

		if (logger.setup()) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("Log system initialization complete") << set_level() << endl;
		}
		else {
			return false;
		}

		if (interfaces::setup()) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("Interfaces initialization complete") << set_level() << endl;
		}
		else {
			return false;
		}

		if (hook::setup()) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("Hook initialization complete") << set_level() << endl;
		}
		else {
			return false;
		}

		if (inputsystem::setup()) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("Inputsystem initialization complete") << set_level() << endl;
		}
		else {
			return false;
		}

		if (render::draw.setup(inputsystem::hWindow, interfaces::device, interfaces::device_context)) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("Render initialization complete") << set_level() << endl;
		}
		else {
			return false;
		}

		logger << set_level(log_level_flags::LOG_INFO) << xorstr_("Initialization done, game version: ") << interfaces::engine->GetProductVersionString() << set_level() << endl;
		logger << set_level(log_level_flags::LOG_INFO) << xorstr_("HIMC version: ") << core::get_himc_version() << set_level() << endl;

		return true;
	}


	DWORD __stdcall cs2_internal::ren(LPVOID lpParameter)
	{
		//using log_system::logger;
		//using log_system::endl;

		//cs2_internal* pThis = static_cast<cs2_internal*>(lpParameter);

		//while (true) {
		//	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		//}

		return 0;
	}

	bool cs2_internal::run() {
		//if (windows_api::winapi.fn_CreateThread(NULL, 0, &cs2_internal::ren, this, 0, NULL) == NULL)
		//	return false;

		return true;
	}


	cs2_internal& cs2_internal::get_instance() {
		static cs2_internal instance;
		return instance;
	}
}
