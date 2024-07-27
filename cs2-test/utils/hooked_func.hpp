#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace cheat {
	long __stdcall hooked_present(IDXGISwapChain* _this, UINT a, UINT b);

	LRESULT __stdcall modified_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}