#include "../cheat.hpp"

#include "../../utils/locale.hpp"

namespace cheat {
	void cs2_internal::menu() {
		ImGui::Begin("GUI TEST");
		ImGui::Text(ZH("hello InkCrow 我要玩原神"));
		ImGui::SliderFloat("FOV", &fov, 1.f, 500.f, "%.1f");
		ImGui::End();
	}
}