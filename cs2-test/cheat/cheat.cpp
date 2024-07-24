#include "cheat.hpp"

#include "../third-party/imgui/imgui.h"
#include "../third-party/imgui/imgui_impl_win32.h"
#include "../third-party/imgui/imgui_impl_dx11.h"

#include "../third-party/minhook/include/MinHook.h"

#pragma comment(lib, "minhook.x64.lib")
#pragma comment(lib, "d3d11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace cheat {
	bool cs2_internal::INITED = false;
	bool cs2_internal::HOOKED = false;

	ID3D11Device* cs2_internal::D3D_DEVICE = nullptr;
	IDXGISwapChain* cs2_internal::SWAP_CHAIN = nullptr;
	ID3D11DeviceContext* cs2_internal::D3D_CONTEXT = nullptr;
	ID3D11RenderTargetView* cs2_internal::D3D_VIEW = nullptr;
	void* cs2_internal::ORIGIN_PRESENT = nullptr;
	WNDPROC cs2_internal::ORIGIN_WNDPROC = nullptr;

	void* cs2_internal::PRESENT_ADDR = nullptr;

	float cs2_internal::SCREEN_WIDTH = 0.f;
	float cs2_internal::SCREEN_HEIGHT = 0.f;

	float cs2_internal::fov = 0.f;

	using Present = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);

	LRESULT __stdcall cs2_internal::modify_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) { return true; }
		return CallWindowProc(ORIGIN_WNDPROC, hwnd, uMsg, wParam, lParam);
		return 0;
	}

	long __stdcall cs2_internal::hooked_present(IDXGISwapChain* _this, UINT a, UINT b) {

		if (!INITED) {
			_this->GetDevice(__uuidof(ID3D11Device), (void**)&D3D_DEVICE);
			D3D_DEVICE->GetImmediateContext(&D3D_CONTEXT);

			DXGI_SWAP_CHAIN_DESC sd;
			_this->GetDesc(&sd);
			HWND _hwnd = sd.OutputWindow;

			SCREEN_WIDTH = static_cast<float>(sd.BufferDesc.Width);
			SCREEN_HEIGHT = static_cast<float>(sd.BufferDesc.Height);

			ID3D11Texture2D* buf{};
			_this->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buf);
			if (!buf)
				return 1;
			D3D_DEVICE->CreateRenderTargetView(buf, nullptr, &D3D_VIEW);
			buf->Release();

			ORIGIN_WNDPROC = (WNDPROC)SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)modify_wndProc);

			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 20.f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(_hwnd);
			ImGui_ImplDX11_Init(D3D_DEVICE, D3D_CONTEXT);
			INITED = true;
		}


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("GUI TEST");
		ImGui::Text("hello InkCrow");
		ImGui::Text(u8"Ô­ÉñÆô¶¯");
		ImGui::SliderFloat("FOV", &fov, 1.f, 500.f, "%.1f");
		ImGui::End();


		auto draw_list = ImGui::GetForegroundDrawList();

		draw_list->AddCircle({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, fov, IM_COL32(255, 0, 0, 255));
		draw_list->AddLine({ 0,0 }, { SCREEN_WIDTH, SCREEN_HEIGHT }, IM_COL32(255, 0, 0, 255));



		ImGui::EndFrame();
		ImGui::Render();
		D3D_CONTEXT->OMSetRenderTargets(1, &D3D_VIEW, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


		return ((Present)ORIGIN_PRESENT)(_this, a, b);
	}

	cs2_internal::cs2_internal() {
		init();
	}

	cs2_internal::~cs2_internal() {
		MH_DisableHook(NULL);
		MH_Uninitialize();
	}

	void cs2_internal::init() {
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
			&SWAP_CHAIN,
			&D3D_DEVICE,
			nullptr,
			nullptr);

		if (SWAP_CHAIN) {
			auto vtable_ptr = (void***)(SWAP_CHAIN);
			auto vtable = *vtable_ptr;
			PRESENT_ADDR = vtable[8];
			MH_Initialize();
			MH_CreateHook(PRESENT_ADDR, hooked_present, &ORIGIN_PRESENT);
			D3D_DEVICE->Release();
			SWAP_CHAIN->Release();
			HOOKED = true;
		}
	}

	bool cs2_internal::run() {
		if (!HOOKED)
			return false;

		MH_EnableHook(PRESENT_ADDR);

		return true;
	}

}