#include <iostream>
#include <Windows.h>

#include "cheat.hpp"
#include "../utils/fnv1a.hpp"
#include "../third_party/xorstr.hpp"

#include <string>

namespace cheat {
	bool cs2_internal::setup() {
		// TODO

#pragma region testing_code

		using namespace log_system;

		logger.setup();

		jm::xor_string encrypted_str = xorstr("This is a string that should be save in variable");

		logger << "This is an unencrypted string" << endl;
		logger << xorstr("This is an encrypted string").get() << endl;
		logger << xorstr_("This is an encrypted string") << endl;

		logger << encrypted_str << endl;
#pragma endregion


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
