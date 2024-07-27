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

		static cs2_internal& get_instance();

		long __stdcall hook_present(IDXGISwapChain* _this, UINT a, UINT b);
		LRESULT __stdcall modify_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:

		bool inited;
		bool hooked;

		ID3D11Device* d3d_device;
		IDXGISwapChain* swap_chain;
		ID3D11DeviceContext* d3d_context;
		ID3D11RenderTargetView* d3d_view;
		void* origin_present;
		WNDPROC origin_wndproc;

		void* present_addr;

		float screen_width;
		float screen_height;

		float fov;
	};

}