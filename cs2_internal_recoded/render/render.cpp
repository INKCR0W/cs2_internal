#include "render.hpp"

// used: memory manager
#include "../memory/memory.hpp"
// used: font
#include "font.hpp"
#include "../log/log.hpp"

#include <ctime>

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

    void draw_class::Helpmarker(const char* Text, ImVec4 Color)
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

		try
		{
            ImGuiStyle& Style = ImGui::GetStyle();
            auto Color = Style.Colors;

            static bool WinPos = true;//用于初始化窗口位置
            int Screen_Width{ GetSystemMetrics(SM_CXSCREEN) };//获取显示器的宽
            int Screen_Heigth{ GetSystemMetrics(SM_CYSCREEN) };//获取显示器的高

            static bool CheckBox_1 = false, CheckBox_2 = true;
            static int InputInt = 0;
            static int Comb = 0;
            static float InputFloat = 0;
            static char InputString[80] = "";
            static std::string stdStr = "";

            static int Tab = 0;
            enum Tab
            {
                Panel,
                Button,
                Input,
                CheckBox
            };

            static int Color_ = 0;
            enum Color_
            {
                Red,
                Green,
                Blue,
                Orange
            };

            switch (Color_)
            {
            case Color_::Red:
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(192, 51, 74, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(212, 71, 94, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(172, 31, 54, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(192, 51, 74, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(192, 51, 74, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(172, 31, 54, 255);

                Color[ImGuiCol_Header] = ImColor(192, 51, 74, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(212, 71, 94, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(172, 31, 54, 255);
                break;
            case Color_::Green:
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(10, 105, 56, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(30, 125, 76, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(0, 95, 46, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(10, 105, 56, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(10, 105, 56, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(0, 95, 46, 255);

                Color[ImGuiCol_Header] = ImColor(10, 105, 56, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(30, 125, 76, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(0, 95, 46, 255);

                break;
            case Color_::Blue:
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(51, 120, 255, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(71, 140, 255, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(31, 100, 225, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(51, 120, 255, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(51, 120, 255, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(31, 100, 225, 255);

                Color[ImGuiCol_Header] = ImColor(51, 120, 255, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(71, 140, 255, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(31, 100, 225, 255);

                break;
            case Color_::Orange://233,87,33
                Style.ChildRounding = 8.0f;
                Style.FrameRounding = 5.0f;

                Color[ImGuiCol_Button] = ImColor(233, 87, 33, 255);
                Color[ImGuiCol_ButtonHovered] = ImColor(253, 107, 53, 255);
                Color[ImGuiCol_ButtonActive] = ImColor(213, 67, 13, 255);

                Color[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 150);
                Color[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42, 150);
                Color[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 150);

                Color[ImGuiCol_CheckMark] = ImColor(233, 87, 33, 255);

                Color[ImGuiCol_SliderGrab] = ImColor(233, 87, 33, 255);
                Color[ImGuiCol_SliderGrabActive] = ImColor(213, 67, 13, 255);

                Color[ImGuiCol_Header] = ImColor(233, 87, 33, 255);
                Color[ImGuiCol_HeaderHovered] = ImColor(253, 107, 53, 255);
                Color[ImGuiCol_HeaderActive] = ImColor(213, 67, 13, 255);

                break;
            }


            if (WinPos)//初始化窗口
            {
                ImGui::SetNextWindowPos({ float(Screen_Width - 600) / 2,float(Screen_Heigth - 400) / 2 });
                WinPos = false;//初始化完毕
            }


            ImGui::Begin("ImGui Tool", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);//开始绘制窗口
            ImGui::SetWindowSize({ 600.0f,400.0f });//设置窗口大小


            {
                ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 10.0f }, { ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 390.0f }, ImColor(100, 100, 100, 255));

                ImGui::SetCursorPos({ 430.0f,20.0f });
                ImGui::PushFont(font_b);
                ImGui::TextColored(Color[ImGuiCol_Button], "ImGui Tool \u9B08");
                ImGui::PopFont();



                ImGui::SetCursorPos({ 430.0f,65.0f });

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Panel ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                if (ImGui::Button("Panel", { 150.0f,40.0f }))
                {
                    Tab = Tab::Panel;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Button ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,115.0f });
                if (ImGui::Button("Button", { 150.0f,40.0f }))
                {
                    Tab = Tab::Button;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Input ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,165.0f });
                if (ImGui::Button("Input", { 150.0f,40.0f }))
                {
                    Tab = Tab::Input;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::CheckBox ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,215.0f });
                if (ImGui::Button("CheckBox", { 150.0f,40.0f }))
                {
                    Tab = Tab::CheckBox;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,265.0f });
                if (ImGui::Button("Exit", { 150.0f,40.0f }))
                {
                    exit(0);
                }
                ImGui::PopStyleColor();

                ImGui::SetCursorPos({ 430.0f,330.0f });
                ImGui::Text("主题颜色");
                ImGui::SameLine();
                ImGui::SetCursorPos({ 505.0f,328.0f });
                ImGui::SetNextItemWidth(80.0f);
                ImGui::Combo(" ", &Color_, "红色\0绿色\0蓝色\0橙色");

                time_t t = time(0);
                char tmp[32] = { NULL };
                strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M", localtime(&t));

                ImGui::SetCursorPos({ 430.0f,365.0f });
                ImGui::TextColored(Color[ImGuiCol_Button], "%s", tmp);
            }


            ImGui::SetCursorPos({ 10.0f,10.0f });
            ImGui::BeginChild("Fucking", { 400.0f,380.0f }, true);
            switch (Tab)
            {
            case Tab::Panel:
                ImGui::PushFont(font_b);
                ImGui::BulletText("Panel");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker("面板示例", Color[ImGuiCol_Button]);
                ImGui::Separator();
                ImGui::TextColored(Color[ImGuiCol_Button], "\t这种东西我没法和你解释\n因为我只是个小面板子");

                break;
            case Tab::Button:
                ImGui::PushFont(font_b);
                ImGui::BulletText("Button");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker("按钮示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::Button("我是个按钮子", { 120.0f,40.0f });
                ImGui::Button("按钮子", { 70.0f,35.0f });
                break;
            case Tab::Input:
                ImGui::PushFont(font_b);
                ImGui::BulletText("Input");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker("输入示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::InputInt("Int类型输入", &InputInt);
                ImGui::InputFloat("Float类型输入", &InputFloat);
                ImGui::InputText("String类型输入", &stdStr);
                ImGui::SliderInt("Int类型滑块", &InputInt, 0, 100);
                ImGui::SliderFloat("Float类型滑块", &InputFloat, 0.0F, 100.0F);
                ImGui::Combo("选择框", &Comb, "A11\0A22\0A33\0A44");
                break;
            case Tab::CheckBox:
                ImGui::PushFont(font_b);
                ImGui::BulletText("CheckBox");
                ImGui::PopFont();
                ImGui::SameLine();
                Helpmarker("复选框示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::Checkbox("复选框-1", &CheckBox_1);
                ImGui::Checkbox("复选框-2", &CheckBox_2);
                break;
            }
            ImGui::EndChild();


            ImGui::End();
		}
		catch (const std::exception& e)
		{
            using namespace log_system;
            logger << set_level(log_level_flags::LOG_ERROR) << e.what() << set_level() << endl;
		}

		render();
	}
}