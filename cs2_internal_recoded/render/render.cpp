#include "render.hpp"

// used: memory manager
#include "../memory/memory.hpp"
// used: font
#include "font.hpp"
// used: menu
#include "menu.hpp"
// used: input_system
#include "../core/interfaces.hpp"
// used: on_WndProc
#include "../core/inputsystem.hpp"
// used: hk_IsRelativeMouseMode
#include "../core/hooks.hpp"
// used: draw_skeleton
#include "../cheat/features/esp.hpp"

#include "../cheat/features/aimbot.hpp"
// used: menu_key skeleton_on
#include "../config/config.hpp"
// used: ImGui_ImplWin32_WndProcHandler
#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace render{
	draw_class::~draw_class() {
		if (!initialized)
			return;

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		initialized = false;
	}

	static void* __cdecl ImGuiAllocWrapper(const size_t nSize, [[maybe_unused]] void* pUserData = nullptr)
	{
		return memory::mem.heap_alloc(nSize);
	}

	static void __cdecl ImGuiFreeWrapper(void* pMemory, [[maybe_unused]] void* pUserData = nullptr) noexcept
	{
		memory::mem.heap_free(pMemory);
	}

	const bool draw_class::setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		if (initialized)
			return true;

		ImGui::SetAllocatorFunctions(ImGuiAllocWrapper, ImGuiFreeWrapper);

		ImGui::CreateContext();

		if (!ImGui_ImplWin32_Init(hWnd))
			return false;

		if (!ImGui_ImplDX11_Init(pDevice, pContext))
			return false;

		ImGuiIO& io = ImGui::GetIO();

		ImFontConfig Font_cfg;
		Font_cfg.FontDataOwnedByAtlas = false;
		font_a = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 18.0f, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());
		font_b = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, 24.0f, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());

		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		initialized = io.Fonts->Build();
		return initialized;
	}

	void draw_class::new_frame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void draw_class::render()
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

    void draw_class::help_marker(const char* Text, ImVec4 Color)
    {
        ImGui::TextColored(Color, "(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(Text);
        }
    }

	void draw_class::run()
	{
		if (initialized == false)
			return;

		new_frame();

		menu::menu.run();
		features::draw_skeleton();
		features::draw_spectator_list();

		render();
	}

	bool draw_class::on_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		inputsystem::on_WndProc(hWnd, uMsg, wParam, lParam);

		// switch menu state
		if (inputsystem::is_key_released(config::cfg.menu_key))
		{
			menu::menu.menu_opened = !menu::menu.menu_opened;

			 const auto oIsRelativeMouseMode = hook::hk_IsRelativeMouseMode.get_original();
			 oIsRelativeMouseMode(interfaces::input_system, menu::menu.menu_opened ? false : menu::menu.main_active);
		}
		return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) || menu::menu.menu_opened;
	}
}