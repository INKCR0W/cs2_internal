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

#include "../config/config.hpp"

#include "../windows_api/win_api.hpp"



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
                Visual,
                Aimbot,
                Misc,
                Shop,
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


            ImGui::Begin("HIMC", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);//开始绘制窗口
            ImGui::SetWindowSize({ 600.0f,400.0f });//设置窗口大小


            {
                ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 10.0f }, { ImGui::GetWindowPos().x + 420.0f,ImGui::GetWindowPos().y + 390.0f }, ImColor(100, 100, 100, 255));

                ImGui::SetCursorPos({ 430.0f,20.0f });
                ImGui::PushFont(draw.font_b);
                ImGui::TextColored(Color[ImGuiCol_Button], "HIMC TEST \u9B08");
                ImGui::PopFont();

                ImGui::SetCursorPos({ 430.0f,65.0f });

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Visual ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                if (ImGui::Button("绘制", { 150.0f,40.0f }))
                {
                    Tab = Tab::Visual;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Aimbot ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,115.0f });
                if (ImGui::Button("瞄准", { 150.0f,40.0f }))
                {
                    Tab = Tab::Aimbot;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Misc ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,165.0f });
                if (ImGui::Button("其它", { 150.0f,40.0f }))
                {
                    Tab = Tab::Misc;
                }
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, Tab == Tab::Shop ? Color[ImGuiCol_Button] : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::SetCursorPos({ 430.0f,215.0f });
                if (ImGui::Button("送地狱火", { 150.0f,40.0f }))
                {
                    Tab = Tab::Shop;
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
            case Tab::Visual:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Visual");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("绘制相关功能", Color[ImGuiCol_Button]);
                ImGui::Separator();
                ImGui::Checkbox("骨骼透视", &config::cfg.skeleton_on);
                ImGui::Checkbox("观战列表", &config::cfg.spectator_list_on);

                break;
            case Tab::Aimbot:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Aimbot");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("自瞄相关功能", Color[ImGuiCol_Button]);
                ImGui::Separator();
                ImGui::TextColored(Color[ImGuiCol_Button], "这里目前什么东西也没有");

                break;
            case Tab::Misc:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Misc");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("其它功能", Color[ImGuiCol_Button]);
                ImGui::Separator();
                ImGui::TextColored(Color[ImGuiCol_Button], "您猜怎么着，这里也啥都没有");

                break;
            case Tab::Shop:
                ImGui::PushFont(draw.font_b);
                ImGui::BulletText("Shop");
                ImGui::PopFont();
                ImGui::SameLine();
                draw.help_marker("扣1送地狱火", Color[ImGuiCol_Button]);
                ImGui::Separator();

                if (ImGui::Button("点我送地狱火", { 120.0f,40.0f })) {
                    windows_api::winapi.fn_ShellExecuteA(NULL, "open", "https://shop.crow.pub/", NULL, NULL, SW_SHOWNORMAL);
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