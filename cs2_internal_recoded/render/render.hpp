#pragma once

#include <d3d11.h>

// used: imgui
#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_dx11.h"
#include "../third_party/imgui/imgui_impl_win32.h"


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

		ImFont* font_a;

		bool initialized = false;

	};

	inline draw_class draw;
}
