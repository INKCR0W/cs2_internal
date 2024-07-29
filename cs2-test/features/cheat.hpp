#pragma once

#include <d3d11.h>


#include "../utils/debug.hpp"
#include "../utils/structs.hpp"

#include "../third-party/imgui/imgui.h"
#include "../third-party/imgui/imgui_impl_win32.h"
#include "../third-party/imgui/imgui_impl_dx11.h"


namespace cheat {

	class cs2_internal {
	public:
		cs2_internal();
		~cs2_internal();

		[[nodiscard]] bool init();
		bool run();

		static cs2_internal& get_instance();
		static void ensure_initialized();

	private:
		bool inited;

		bool esp;

		ID3D11Device* d3d_device;
		IDXGISwapChain* swap_chain;
		ID3D11DeviceContext* d3d_context;
		ID3D11RenderTargetView* d3d_view;
		void* origin_present;
		WNDPROC origin_wndproc;

		void* present_addr;

		float screen_width;
		float screen_height;

		HMODULE client_dll_addr;

		float fov;

		void render();
		void menu();

		friend long __stdcall hooked_present(IDXGISwapChain* _this, UINT a, UINT b);
		friend LRESULT __stdcall modified_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		long __stdcall hook_present(IDXGISwapChain* _this, UINT a, UINT b);
		LRESULT __stdcall modify_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		Vec2 world_to_screen(view_matrix_t matrix, Vec3 position) const;
	};

}