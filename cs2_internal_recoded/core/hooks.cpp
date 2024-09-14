#include "hooks.hpp"

#include <format>
// used: interfaces
#include "interfaces.hpp"
// used: get_VFunc
#include "../memory/memory.hpp"
// used: vtable
#include "../game/vtable.hpp"
// used: log system
#include "../log/log.hpp"
// used: windows api
#include "../windows_api/win_api.hpp"
// used: old_WndProc
#include "../core/inputsystem.hpp"
// used: main_active
#include "../render/menu.hpp"
// used: draw.run
#include "../render/render.hpp"
// used: game modules
#include "../game/game_modules.hpp"
// used ISwapChainDx11
#include "../sdk/interfaces/iswapchaindx11.hpp"
// used: imgui
#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_dx11.h"
#include "../third_party/imgui/imgui_impl_win32.h"

// used: update_entitys
#include "../cheat/features/entity_var.hpp"
#include "../utils/return_address.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace hook {
	const bool setup() {
		using namespace log_system;

		if (MH_Initialize() != MH_OK)
		{
			log_system::logger << log_system::set_level(log_system::log_level_flags::LOG_ERROR) << xorstr_("Failed to initialize minhook") << log_system::set_level() << log_system::endl;
			return false;
		}

		if (!hk_Present.create(memory::mem.get_VFunc(interfaces::swap_chain->pDXGISwapChain, vtable::D3D::PRESENT), reinterpret_cast<void*>(&Present)))
			return false;

#ifdef _DEBUG
		logger << set_level(log_level_flags::LOG_INFO) << xorstr_("\"Present\" hook has been created: ") << reinterpret_cast<std::uintptr_t>(hk_Present.get_original()) << set_level() << endl;
#endif

		// @ida: #STR: cl: CreateMove clamped invalid attack history index %d in frame history to -1. Was %d, frame history size %d.\n
		if (!hk_CreateMove.create(memory::mem.get_VFunc(interfaces::input, vtable::CLIENT::CREATEMOVE), reinterpret_cast<void*>(&CreateMove)))
			return false;

#ifdef _DEBUG
		logger << set_level(log_level_flags::LOG_INFO) << xorstr_("\"CreateMove\" hook has been created: ") << reinterpret_cast<std::uintptr_t>(hk_CreateMove.get_original()) << set_level() << endl;
#endif

		if (!hk_IsRelativeMouseMode.create(memory::mem.get_VFunc(interfaces::input_system, vtable::INPUTSYSTEM::ISRELATIVEMOUSEMODE), reinterpret_cast<void*>(&IsRelativeMouseMode)))
			return false;

#ifdef _DEBUG
		logger << set_level(log_level_flags::LOG_INFO) << xorstr_("\"IsRelativeMouseMode\" hook has been created: ") << reinterpret_cast<std::uintptr_t>(hk_IsRelativeMouseMode.get_original()) << set_level() << endl;
#endif

		return true;
	}

	void destroy() {
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);

		MH_Uninitialize();
	}


	HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags)
	{
		const auto oPresent = hk_Present.get_original();

		if (interfaces::render_target_view == nullptr)
			interfaces::create_render_target();

		if (interfaces::render_target_view != nullptr) {
			interfaces::device_context->OMSetRenderTargets(1, &interfaces::render_target_view, nullptr);
		}

		features::update_entitys();

		render::draw.run();

		return oPresent(interfaces::swap_chain->pDXGISwapChain, uSyncInterval, uFlags);
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (render::draw.on_WndProc(hWnd, uMsg, wParam, lParam))
			return 1L;

		return windows_api::winapi.fn_CallWindowProcW(inputsystem::old_WndProc, hWnd, uMsg, wParam, lParam);
	}

	bool __fastcall CreateMove(void* pInput, int nSlot, bool bActive)
	{
		static void* return_address = reinterpret_cast<PVOID>(memory::mem.find_pattern(modules::client_dll, "FF 27"));

		const bool result = FakeReturnAddress(return_address, hk_CreateMove.get_original(), pInput, nSlot, bActive);

		log_system::logger << "ccc" << log_system::endl;
		// features::update_entitys();

		return result;
	}

	void* IsRelativeMouseMode(void* pThisptr, bool bActive)
	{
		const auto oIsRelativeMouseMode = hk_IsRelativeMouseMode.get_original();

		menu::menu.main_active = bActive;

		if (menu::menu.menu_opened)
			return oIsRelativeMouseMode(pThisptr, false);

		return oIsRelativeMouseMode(pThisptr, bActive);
	}
}