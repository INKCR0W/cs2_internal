#include "../cheat.hpp"

#include "../../utils/debug.hpp"
#include "../../utils/locale.hpp"

namespace cheat {
	void cs2_internal::menu() {

		try {
			ImGui::Begin("GUI TEST");

			ImGui::Text(ZH("hello InkCrow 我不要玩原神"));

			if (ImGui::BeginTabBar("Render")) {
				if (ImGui::BeginTabItem("ESP")) {
					ImGui::Text("hello InkCrow 1");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Aimbot")) {
					ImGui::Text("hello InkCrow 2");
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Misc")) {
					ImGui::Text("hello InkCrow 3");
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
		catch (const std::exception& e) {
			dbg::dbg_print(std::format("Exception in ImGui drawing : {}", e.what()));
		}

	}
}