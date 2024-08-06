#include "../cheat.hpp"

#include "../../utils/debug.hpp"
#include "../../utils/locale.hpp"

namespace cheat {
	void cs2_internal::menu() {

		try {
			ImGui::Begin("A", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::BeginTabBar("Render")) {
				if (ImGui::BeginTabItem("ESP")) {
					ImGui::Checkbox("ESP", &esp_on);
					if (esp_on) {
						ImGui::ColorPicker4("Color", reinterpret_cast<float*>(&esp_color));
						ImGui::Checkbox("Box", &box);
						ImGui::Checkbox("Skeleton", &skeleton);
						ImGui::Checkbox("Health", &health);
						ImGui::Checkbox("Name", &name);
						ImGui::Checkbox("Eye Ray", &eye_ray);
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Misc")) {
					if (ImGui::Button("Unload", { 50, 20 })) {
						unloading = true;
					}
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