#pragma once

// used string
#include <string>
// used getenv
#include <cstdlib>
// used exists create_directory
#include <filesystem>

// @todo: create working path
namespace core {
	std::string get_working_path() {
		if (const char* appdata_path = std::getenv("APPDATA"); appdata_path != nullptr) {
			std::string path = appdata_path;
			path += "\\LfWare";
			return path;
		}
		else {
			return "";
		}
	}
}