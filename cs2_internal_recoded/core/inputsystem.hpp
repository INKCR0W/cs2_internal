#pragma once
// used: [win] winapi
#include <windows.h>

#include <cstdint>

#include "../core/common.hpp"

/*
 * INPUT SYSTEM
 * listen and handle key states
 */
namespace inputsystem
{
	using KeyState_t = std::uint8_t;

	enum EKeyState : KeyState_t
	{
		KEY_STATE_NONE,
		KEY_STATE_DOWN,
		KEY_STATE_UP,
		KEY_STATE_RELEASED
	};

	/* @section: values */
	// current window
	inline HWND hWindow = nullptr;
	// saved window messages handler
	inline WNDPROC old_WndProc = nullptr;

	bool setup();
	// restore window messages processor to original
	void destroy();

	/* @section: callbacks */
	// process input window message and save keys states in array
	// bool on_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}