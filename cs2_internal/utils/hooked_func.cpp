#include "hooked_func.hpp"

#include "../features/cheat.hpp"

namespace cheat {
	long __stdcall hooked_present(IDXGISwapChain* _this, UINT a, UINT b) {
		return cs2_internal::get_instance().hook_present(_this, a, b);
	}

	LRESULT __stdcall modified_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return cs2_internal::get_instance().modify_wndProc(hwnd, uMsg, wParam, lParam);
	}
}