#include "../cheat.hpp"

namespace cheat {
	void cs2_internal::menu() {
		ImGui::Begin("GUI TEST");
		ImGui::Text("hello InkCrow");
		ImGui::SliderFloat("FOV", &fov, 1.f, 500.f, "%.1f");
		ImGui::End();
	}
}