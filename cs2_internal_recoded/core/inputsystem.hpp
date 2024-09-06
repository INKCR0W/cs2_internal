#pragma once
// used: [win] winapi
#include <windows.h>
// used: uint8_t
#include <cstdint>
// used: ASSERT
#include "../core/common.hpp"

/*
 * INPUT SYSTEM
 * listen and handle key states
 */
namespace inputsystem
{
	using key_state_t = std::uint8_t;

	enum key_state_flag : key_state_t
	{
		KEY_STATE_NONE,
		KEY_STATE_DOWN,
		KEY_STATE_UP,
		KEY_STATE_RELEASED
	};

	inline key_state_t arr_key_state[256] = {};

	// current window
	inline HWND hWindow = nullptr;
	// saved window messages handler
	inline WNDPROC old_WndProc = nullptr;

	bool setup();
	// restore window messages processor to original
	void destroy();

	bool on_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	[[nodiscard]] __forceinline bool is_key_released(const std::uint32_t uButtonCode)
	{
		if (arr_key_state[uButtonCode] == KEY_STATE_RELEASED)
		{
			arr_key_state[uButtonCode] = KEY_STATE_UP;
			return true;
		}

		return false;
	}
}