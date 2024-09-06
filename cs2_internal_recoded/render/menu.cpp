#include "menu.hpp"

// used: imgui
#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_dx11.h"
#include "../third_party/imgui/imgui_impl_win32.h"
#include "../third_party/imgui/imgui_stdlib.h"

// used: fonts
#include "render.hpp"
// used: time
#include "ctime"
// used: log system
#include "../log/log.hpp"

namespace menu {
    using namespace render;

	void menu_class::run() {
        if (!menu_opened)
            return;
        try
        {
            ImGuiStyle& Style = ImGui::GetStyle();
            auto Color = Style.Colors;

            static bool WinPos = true;//用于初始化窗口位置

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
                ImGui::SetNextWindowPos({ float(screen_width - 600) / 2,float(screen_height - 400) / 2 });
                WinPos = false;//初始化完毕
            }


            ImGui::Begin("ImGui Tool", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);//开始绘制窗口
            ImGui::SetWindowSize({ 600.0f,400.0f });//设置窗口大小


            {
                ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 10.0f }, { ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 390.0f }, ImColor(100, 100, 100, 255));

                ImGui::SetCursorPos({ 430.0f,20.0f });
                ImGui::PushFont(draw.font_b);
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
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Panel");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("面板示例", Color[ImGuiCol_Button]);
                ImGui::Separator();
                ImGui::TextColored(Color[ImGuiCol_Button], "\t这种东西我没法和你解释\n因为我只是个小面板子");

                break;
            case Tab::Button:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Button");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("按钮示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::Button("我是个按钮子", { 120.0f,40.0f });
                ImGui::Button("按钮子", { 70.0f,35.0f });
                break;
            case Tab::Input:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Input");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("输入示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::InputInt("Int类型输入", &InputInt);
                ImGui::InputFloat("Float类型输入", &InputFloat);
                ImGui::InputText("String类型输入", &stdStr);
                ImGui::SliderInt("Int类型滑块", &InputInt, 0, 100);
                ImGui::SliderFloat("Float类型滑块", &InputFloat, 0.0F, 100.0F);
                ImGui::Combo("选择框", &Comb, "A11\0A22\0A33\0A44");
                break;
            case Tab::CheckBox:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("CheckBox");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("复选框示例", Color[ImGuiCol_Button]);
                ImGui::Separator();

                ImGui::Checkbox("复选框-1", &CheckBox_1);
                ImGui::Checkbox("复选框-2", &CheckBox_2);

                static bool selected[3] = { false, false, false };  // 用于存储每个选项的状态

                if (ImGui::BeginCombo("Select options", "Multiple choices")) {
                    ImGui::Selectable("Option 1", &selected[0], ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Option 2", &selected[1], ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Option 3", &selected[2], ImGuiSelectableFlags_DontClosePopups);
                    ImGui::EndCombo();
                }

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
	}
}