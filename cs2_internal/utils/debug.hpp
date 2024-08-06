#pragma once

// #define _DEBUG_

#include <iostream>
#include <string>
#include <format>

namespace dbg {
	using std::string;
	using std::cerr;
	using std::cout;
	using std::endl;

	void dbg_print(const string& message);
}