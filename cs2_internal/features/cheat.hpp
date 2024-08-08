#pragma once

#include <d3d11.h>
#include <string>


#include "../utils/debug.hpp"
#include "../utils/structs.hpp"
#include "render/draw_list.hpp"

#include "../third-party/imgui/imgui.h"
#include "../third-party/imgui/imgui_impl_win32.h"
#include "../third-party/imgui/imgui_impl_dx11.h"


namespace cheat {
	constexpr float M_PI = 3.1415926535f;

	class cs2_internal {
	public:
		cs2_internal();
		~cs2_internal();

		[[nodiscard]] bool init(HMODULE base);
		bool run();
		void unload() const;

		static cs2_internal& get_instance();
		static void ensure_initialized();

		void set_path(const std::string& _path);
		void set_console(HWND handle);
		const std::string& get_path();

	private:
		HMODULE base_handle;
		HWND cs2_window;
		HWND console_window;

		std::string file_path;

		bool inited;
		bool unloading;

		ID3D11Device* d3d_device;
		IDXGISwapChain* swap_chain;
		ID3D11DeviceContext* d3d_context;
		ID3D11RenderTargetView* d3d_view;
		void* origin_present;
		WNDPROC origin_wndproc;

		void* present_addr;

		float screen_width;
		float screen_height;

		uintptr_t client_dll_addr;

		std::vector<entity> player_entity_list;

		bool is_in_match;

		uintptr_t local_player_pawn;
		uintptr_t local_player_controller;
		uintptr_t local_player_pawn_handle;

		float fov;

		ImVec4 esp_color;
		bool esp_on;
		bool box;
		bool skeleton;
		bool health;
		bool name;
		bool eye_ray;
		bool spectator_list;

		std::vector<command> command_list;
		std::vector<uintptr_t> spectators_list;

		void ren();
		void render();
		void menu();

		void esp();

		friend long __stdcall hooked_present(IDXGISwapChain* _this, UINT a, UINT b);
		friend LRESULT __stdcall modified_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		long __stdcall hook_present(IDXGISwapChain* _this, UINT a, UINT b);
		LRESULT __stdcall modify_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool update_entity();
		uintptr_t get_from_handle(uintptr_t entityList, uintptr_t handle);

		Vec2 world_to_screen(view_matrix_t matrix, Vec3 position) const;


		template <typename T>
		T read_memory(const std::uintptr_t addr);

		template <typename T>
		bool write_memory(const std::uintptr_t addr, const T& value);

		template <typename T>
		bool read_memory_size(const std::uintptr_t addr, const T* value, size_t size);
	};



	template <typename T>
	T cs2_internal::read_memory(const std::uintptr_t addr) {
		T value = {};
		ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(addr), &value, sizeof(value), 0);
		return value;
	}

	template <typename T>
	bool cs2_internal::write_memory(const std::uintptr_t addr, const T& value) {
		return WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(addr), &value, sizeof(value), 0);
	}

	template <typename T>
	bool cs2_internal::read_memory_size(const std::uintptr_t addr, const T* value, size_t size) {
		return ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(addr), &value, size, 0);
	}
}