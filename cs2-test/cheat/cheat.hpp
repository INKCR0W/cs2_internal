#pragma once

#include <d3d11.h>

namespace cheat {

	class cs2_internal {
	public:
		cs2_internal();
		~cs2_internal();

		bool run();

	private:
		void init();

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
	};

}