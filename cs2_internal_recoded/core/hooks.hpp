#pragma once

// used: d3d api
#include <d3d11.h>
// used: hook system
#include "../hook/hook_manager.hpp"

#include "../memory/memory.hpp"

namespace hook {
	const bool setup();
	void destroy();

	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void* IsRelativeMouseMode(void* pThisptr, bool bActive);


	inline hook_object<decltype(&Present)> hk_Present = {};
	inline hook_object<decltype(&IsRelativeMouseMode)> hk_IsRelativeMouseMode = {};
}
