#include "cheat.hpp"

#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>

#include "../utils/fnv1a.hpp"
#include "../utils/xorstr.hpp"
#include "../memory/memory.hpp"


namespace cheat {
	bool cs2_internal::setup() {
		using namespace log_system;
		using namespace utils;

		if (logger.setup()) {
			logger << set_level(log_level_flags::LOG_INFO) << xorstr_("The log system initialization is complete") << set_level() << endl;
		}
		else {
			return false;
		}

		logger << "kernal_32.dll: " << reinterpret_cast<uint64_t>(::memory::mem.get_module_base_handle("KERNEL32.DLL")) << endl;
		logger << "kernal_32.dll: " << reinterpret_cast<uint64_t>(GetModuleHandleA("kernel32.dll")) << endl;

		return true;
	}


	DWORD __stdcall cs2_internal::ren(LPVOID lpParameter)
	{
		cs2_internal* pThis = static_cast<cs2_internal*>(lpParameter);
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			log_system::logger << "running" << log_system::endl;
		}
		return 0;
	}

	bool cs2_internal::run() {
		// TODO
		Sleep(1000);

		DWORD thread_id = 0;

		if (CreateThread(NULL, 0, &cs2_internal::ren, this, 0, &thread_id) == NULL)
			return false;

		log_system::logger << "Thread created, thread id : " << thread_id << log_system::endl;

		return true;
	}


	cs2_internal& cs2_internal::get_instance() {
		static cs2_internal instance;
		return instance;
	}
}
