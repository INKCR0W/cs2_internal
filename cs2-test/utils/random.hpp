#pragma once

#include <string>

namespace utils {
	using std::string;

	const string randomString(size_t length, const std::string& charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()_+=-[]{};':,.<>/?");
}