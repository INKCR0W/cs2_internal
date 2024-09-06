#pragma once

#include <d3d11.h>

#include <string>

// used: imgui
#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_dx11.h"
#include "../third_party/imgui/imgui_impl_win32.h"
#include "../third_party/imgui/imgui_stdlib.h"


namespace render {
	class draw_class {
	public:
		draw_class() = default;
		~draw_class();

		draw_class(const draw_class& copy) = delete;
		draw_class& operator=(const draw_class& other) = delete;

		const bool setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		void run();

	private:
		void new_frame();
		void render();

		void Helpmarker(const char* Text, ImVec4 Color);

		ImFont* font_a;
		ImFont* font_b;

		bool initialized = false;

	};

	inline draw_class draw;
}
