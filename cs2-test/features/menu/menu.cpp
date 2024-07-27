#include "../cheat.hpp"

#include "../../utils/locale.hpp"

namespace cheat {
	void cs2_internal::menu() {
		//if (ImGui::Begin("GUI TEST")) {
		//	ImGui::Text(ZH("hello InkCrow 我要玩原神"));
		//	ImGui::SliderFloat("FOV", &fov, 1.f, 500.f, "%.1f");
		//	if (ImGui::BeginTabBar("Render")) {
		//		//if (ImGui::BeginTabItem("ESP")) {
		//		//	ImGui::Text(ZH("hello InkCrow 我不要玩原神"));
		//		//	ImGui::EndTabItem();
		//		//}
		//		ImGui::Text(ZH("hello InkCrow 我不要玩原神"));
		//		ImGui::Checkbox("ESP", nullptr);
		//		ImGui::EndTabBar();
		//	}
		//	if (ImGui::BeginTabBar("2")) {
		//		//if (ImGui::BeginTabItem("ESP")) {
		//		//	ImGui::Text(ZH("hello InkCrow 我不要玩原神"));
		//		//	ImGui::EndTabItem();
		//		//}
		//		ImGui::Text(ZH("hello InkCrow 我不要玩原神"));
		//		ImGui::Checkbox("ESP", nullptr);
		//		ImGui::EndTabBar();
		//	}
		//	ImGui::End();
		//}

		ImGui::Begin("GUI TEST");

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

		//if (ImGui::Begin("GUI TEST", nullptr)) {
		//	ImGui::Text(ZH("hello InkCrow 我要玩原神"));

		//	if (ImGui::BeginTabBar("Render")) {
		//		if (ImGui::BeginTabItem("ESP")) {
		//		ImGui::SliderFloat("FOV", &fov, 1.f, 500.f, "%.1f");
		//		ImGui::Checkbox("ESP", nullptr);
		//		ImGui::EndTabItem();
		//		}

		//		ImGui::EndTabBar();
		//	}
		//ImGui::End();
		//}
	}
}