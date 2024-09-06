#include "inputsystem.hpp"

#include "../log/log.hpp"
#include "../utils/xorstr.hpp"
#include "../core/common.hpp"
#include "../core/hooks.hpp"

// used: windows api
#include "../windows_api/win_api.hpp"

#include "../render/menu.hpp"

#include "../config/config.hpp"

namespace inputsystem {
	using namespace log_system;

	bool setup()
	{
		if (hWindow == nullptr)
		{
			logger << set_level(log_level_flags::LOG_ERROR) << xorstr_("Window not found") << set_level() << endl;
			ASSERT(false);
		}

		old_WndProc = reinterpret_cast<WNDPROC>(windows_api::winapi.fn_SetWindowLongPtrW(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hook::WndProc)));
		if (old_WndProc == nullptr)
			return false;

		return true;
	}

	bool on_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// prevent process when e.g. binding something in-menu
		if (wParam != config::cfg.menu_key && menu::menu.menu_opened)
			return false;

		// current active key
		int nKey = 0;
		// current active key state
		key_state_t state = KEY_STATE_NONE;

		switch (uMsg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (wParam < 256U)
			{
				nKey = static_cast<int>(wParam);
				state = KEY_STATE_DOWN;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (wParam < 256U)
			{
				nKey = static_cast<int>(wParam);
				state = KEY_STATE_UP;
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
			nKey = VK_LBUTTON;
			state = uMsg == WM_LBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			nKey = VK_RBUTTON;
			state = uMsg == WM_RBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
			break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
			nKey = VK_MBUTTON;
			state = uMsg == WM_MBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
			break;
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDBLCLK:
			nKey = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
			state = uMsg == WM_XBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
			break;
		default:
			return false;
		}

		// save key states
		if (state == KEY_STATE_UP && arr_key_state[nKey] == KEY_STATE_DOWN) // if swap states it will be pressed state
			arr_key_state[nKey] = KEY_STATE_RELEASED;
		else
			arr_key_state[nKey] = state;

		return true;
	}

	void destroy()
	{
		if (old_WndProc != nullptr)
		{
			windows_api::winapi.fn_SetWindowLongPtrW(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(old_WndProc));
			old_WndProc = nullptr;
		}
	}

}