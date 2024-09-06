#include "inputsystem.hpp"

#include "../log/log.hpp"
#include "../utils/xorstr.hpp"
#include "../core/common.hpp"
#include "../core/hooks.hpp"

// used: windows api
#include "../windows_api/win_api.hpp"

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

	void destroy()
	{
		if (old_WndProc != nullptr)
		{
			windows_api::winapi.fn_SetWindowLongPtrW(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(old_WndProc));
			old_WndProc = nullptr;
		}
	}
}