#pragma once

// used: d3d api
#include <d3d11.h>
// used: hook system
#include "../hook/hook_manager.hpp"

#include "../memory/memory.hpp"

#include "../sdk/interfaces/csgoinput.hpp"

namespace hook {
	const bool setup();
	void destroy();

	inline void* return_address = nullptr;

	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool __fastcall CreateMove(CCSGOInput* pInput, int nSlot, CUserCmd* pCmd);
	void* IsRelativeMouseMode(void* pThisptr, bool bActive);


	inline hook_object<decltype(&Present)> hk_Present = {};
	inline hook_object<decltype(&CreateMove)> hk_CreateMove = {};
	inline hook_object<decltype(&IsRelativeMouseMode)> hk_IsRelativeMouseMode = {};
}
