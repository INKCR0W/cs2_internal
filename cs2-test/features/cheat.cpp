#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <format>
#include <filesystem>
#include <thread>

#include "cheat.hpp"

#include "../utils/hooked_func.hpp"
#include "../third-party/minhook/include/MinHook.h"

// #pragma comment(lib, "minhook.x64.lib")
#pragma comment(lib, "d3d11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace cheat {
	using Present = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);

	LRESULT __stdcall cs2_internal::modify_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) { return true; }
		return CallWindowProc(cs2_internal::origin_wndproc, hwnd, uMsg, wParam, lParam);
	}

	long __stdcall cs2_internal::hook_present(IDXGISwapChain* _this, UINT a, UINT b) {

		if (!inited) {
			cs2_window = reinterpret_cast<HWND>(_this);
			_this->GetDevice(__uuidof(ID3D11Device), (void**)&d3d_device);
			d3d_device->GetImmediateContext(&d3d_context);

			DXGI_SWAP_CHAIN_DESC sd;
			_this->GetDesc(&sd);
			HWND _hwnd = sd.OutputWindow;

			screen_width = static_cast<float>(sd.BufferDesc.Width);
			screen_height = static_cast<float>(sd.BufferDesc.Height);

			ID3D11Texture2D* buf{};
			_this->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buf);
			if (!buf)
				return 1;
			d3d_device->CreateRenderTargetView(buf, nullptr, &d3d_view);
			buf->Release();

			origin_wndproc = (WNDPROC)SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)modified_wndProc);

			ImGui::SetCurrentContext(ImGui::CreateContext());

			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 16.f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			io.IniFilename = nullptr;
			io.LogFilename = nullptr;

			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

			ImGui_ImplWin32_Init(_hwnd);
			ImGui_ImplDX11_Init(d3d_device, d3d_context);
			inited = true;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		render();

		ImGui::EndFrame();
		ImGui::Render();
		d3d_context->OMSetRenderTargets(1, &d3d_view, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


		return ((Present)origin_present)(_this, a, b);
	}

	cs2_internal::cs2_internal() : inited(false), d3d_device(nullptr), swap_chain(nullptr), d3d_context(nullptr), d3d_view(nullptr),
		origin_present(nullptr), origin_wndproc(nullptr), present_addr(nullptr),
		base_handle(nullptr), cs2_window(nullptr), console_window(nullptr),
		file_path(""),
		screen_width(0.f), screen_height(0.f), fov(0.f), player_entity_list({}),
		client_dll_addr(NULL), is_in_match(false), local_player_controller(NULL), local_player_pawn(NULL),
		esp_on(false), unloading(false)
	{
		const char* appdata_path = nullptr;

		if (appdata_path = std::getenv("APPDATA"); appdata_path == nullptr) {
			dbg::dbg_print("Failed to get appdata path");
			MessageBox(NULL, "Failed to get appdata path", "ERROR", MB_OK | MB_ICONERROR);
			exit(1);
		}

		std::string path = appdata_path;
		path += "\\LfWare";
		file_path = path;

		if (std::filesystem::exists(path))
			dbg::dbg_print(std::format("Config folder connected: {}", path));
		else
		{
			if (std::filesystem::create_directory(path))
				dbg::dbg_print(std::format("Config folder created: {}", path));
			else
			{
				dbg::dbg_print("Failed to create the config directory");
				MessageBox(NULL, "Failed to create the config directory", "ERROR", MB_OK | MB_ICONERROR);
			}
		}
	}

	cs2_internal::~cs2_internal() {
		MH_DisableHook(NULL);
		MH_Uninitialize();
	}

	bool cs2_internal::init(HMODULE base) {
		base_handle = base;

		dbg::dbg_print("Initialization begins");

		dbg::dbg_print("Hook initialize begins");

		const unsigned level_count = 2;
		D3D_FEATURE_LEVEL levels[level_count] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = GetForegroundWindow();
		sd.SampleDesc.Count = 1;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		auto hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			levels,
			level_count,
			D3D11_SDK_VERSION,
			&sd,
			&swap_chain,
			&d3d_device,
			nullptr,
			nullptr);

		if (swap_chain) {
			dbg::dbg_print(std::format("Got swap_chain {}", reinterpret_cast<void*>(swap_chain)));
			auto vtable_ptr = reinterpret_cast<void***>(swap_chain);
			auto vtable = *vtable_ptr;
			present_addr = vtable[8];
			dbg::dbg_print(std::format("Got present_addr {}", present_addr));

			MH_Initialize();
			MH_CreateHook(present_addr, reinterpret_cast<void*>(hooked_present), &origin_present);

			d3d_device->Release();
			swap_chain->Release();
		}
		else {
			dbg::dbg_print(std::format("Bad swap_chain {}", reinterpret_cast<void*>(swap_chain)));
			MessageBox(NULL, std::format("Bad swap_chain {}", reinterpret_cast<void*>(swap_chain)).c_str(), "ERROR", MB_OK | MB_ICONERROR);
			return false;
		}

		dbg::dbg_print("Hook initialize done");

		dbg::dbg_print("Game information initialize begins");
		client_dll_addr = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
		dbg::dbg_print(std::format("Got client.dll {}", reinterpret_cast<void*>(client_dll_addr)));
		dbg::dbg_print("Game information initialize done");

		dbg::dbg_print("Initialization done");

		return true;
	}

	void cs2_internal::ren() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			if (unloading) {
				unload();
			}
		}
	}

	bool cs2_internal::run() {
		dbg::dbg_print("Running");

		if (!present_addr) {
			dbg::dbg_print(std::format("Wrong present_addr {}", present_addr));
			MessageBox(NULL, std::format("HOOK ERROR: {}", present_addr).c_str(), "ERROR", MB_OK | MB_ICONERROR);
			return false;
		}

		dbg::dbg_print(std::format("Enable hook {}", present_addr));
		MH_STATUS status = MH_EnableHook(present_addr);

		if (status != MH_OK) {
			dbg::dbg_print(std::format("Hook error {}", static_cast<int>(status)));
			MessageBox(NULL, std::format("HOOK ERROR: {}", static_cast<int>(status)).c_str(), "ERROR", MB_OK | MB_ICONERROR);
			return false;
		}

		std::thread unload_thread([this]() { this->ren(); });
		unload_thread.detach();

		dbg::dbg_print("Unload thread created");

		dbg::dbg_print("All done, everything was fine");

		return true;
	}

	void cs2_internal::unload() const {
		dbg::dbg_print("Unloading");

		// HWND _hwnd = FindWindow(NULL, "Counter-Strike 2");
		SetWindowLongPtr(cs2_window, GWLP_WNDPROC, (LONG_PTR)origin_wndproc);
		dbg::dbg_print("WndProc reseted");

		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
		dbg::dbg_print("Hooks disabled");

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();

		dbg::dbg_print("ImGui destroyed");

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		FreeConsole();

		if (console_window) {
			PostMessage(console_window, WM_CLOSE, 0, 0);
		}

		FreeLibraryAndExitThread(base_handle, 0);
	}


	cs2_internal& cs2_internal::get_instance() {
		static cs2_internal instance;
		return instance;
	}

	void cs2_internal::ensure_initialized() {
		get_instance();
	}

	void cs2_internal::set_path(const std::string& _path) {
		file_path = _path;
	}

	void cs2_internal::set_console(HWND handle) {
		console_window = handle;
	}

	const std::string& cs2_internal::get_path() {
		return file_path;
	}
}