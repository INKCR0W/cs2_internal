#pragma once

#include <d3d11.h>


#include "../utils/debug.hpp"

#include "../third-party/imgui/imgui.h"
#include "../third-party/imgui/imgui_impl_win32.h"
#include "../third-party/imgui/imgui_impl_dx11.h"


namespace cheat {

	class cs2_internal {
	public:
		cs2_internal();
		~cs2_internal();

		void init();
		bool run();

	private:

		static long __stdcall hooked_present(IDXGISwapChain* _this, UINT a, UINT b);
		static LRESULT __stdcall modify_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static bool INITED;
		static bool HOOKED;

		static ID3D11Device* D3D_DEVICE;
		static IDXGISwapChain* SWAP_CHAIN;
		static ID3D11DeviceContext* D3D_CONTEXT;
		static ID3D11RenderTargetView* D3D_VIEW;
		static void* ORIGIN_PRESENT;
		static WNDPROC ORIGIN_WNDPROC;

		static void* PRESENT_ADDR;

		static float SCREEN_WIDTH;
		static float SCREEN_HEIGHT;

		static float fov;
	};

}