#include "render.hpp"

// used: memory manager
#include "../memory/memory.hpp"
// used: font
#include "font.hpp"
#include "../log/log.hpp"

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

	void draw_class::run()
	{
		if (initialized == false)
			return;

		new_frame();

		auto draw_list = ImGui::GetForegroundDrawList();
		draw_list->AddText({ 500, 500 }, ImColor(255, 0, 0, 255), "TEST");

		try
		{
			ImGui::Begin("Crow", nullptr);

			ImGui::End();
		}
		catch (const std::exception&)
		{

		}
		// log_system::logger << "drawing" << log_system::endl;

		//{
		//	ImGui::Begin("A", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		//	if (ImGui::BeginTabBar("Render")) {
		//		if (ImGui::BeginTabItem("ESP")) {
		//			ImGui::Checkbox("ESP", nullptr);
		//			ImGui::EndTabItem();
		//		}

		//		if (ImGui::BeginTabItem("Misc")) {
		//			if (ImGui::Button("Unload", { 50, 20 })) {

		//			}
		//			ImGui::EndTabItem();
		//		}

		//		ImGui::EndTabBar();
		//	}

		//	ImGui::End();
		//}
		render();
	}
}