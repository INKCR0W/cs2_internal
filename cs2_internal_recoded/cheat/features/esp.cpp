#include "esp.hpp"

// used: player_list
#include "entity_var.hpp"
// used: engineclient
#include "../../core/interfaces.hpp"
// used: entity structs
#include "../../sdk/entity.hpp"

#include "../../sdk/const.hpp"

#include "../../config/config.hpp"

#include "../../third_party/imgui/imgui.h"

#include "../../log/log.hpp"

namespace features {
	Vector2D_t world_to_screen(ViewMatrix_t matrix, Vector_t position)
	{
		float View = 0.f;
		float SightX = menu::menu.screen_width / 2.f;
		float SightY = menu::menu.screen_height / 2.f;

		View = matrix[3][0] * position.x + matrix[3][1] * position.y + matrix[3][2] * position.z + matrix[3][3];

		if (View <= 0.01)
			return { -1.0f, -1.0f };

		float final_x = SightX + (matrix[0][0] * position.x + matrix[0][1] * position.y + matrix[0][2] * position.z + matrix[0][3]) / View * SightX;
		float final_y = SightY - (matrix[1][0] * position.x + matrix[1][1] * position.y + matrix[1][2] * position.z + matrix[1][3]) / View * SightY;

		return { final_x, final_y };
	}

	void draw_skeleton() {
		if (!config::cfg.skeleton_on)
			return;

		if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
			return;

		if (vars::local_player_controller == nullptr)
			return;

		for (auto current_player : features::vars::player_list) {
			 auto current_player_pawn = current_player->get_pawn(vars::entity_list_address);

			if (current_player_pawn->m_iTeamNum() == vars::local_player_controller->get_pawn(vars::entity_list_address)->m_iTeamNum())
				continue;

			if (current_player_pawn->m_iHealth() <= 0 || current_player->m_bPawnIsAlive() == false)
				continue;

			Vector_t previous = {};
			Vector_t current = {};

			Vector2D_t current_screen_pos = {};
			Vector2D_t previous_screen_pos = {};

			auto game_scene_node = current_player_pawn->m_pGameSceneNode();
			auto bones = game_scene_node->get_skeleton_instance()->pBoneCache;

			for (std::vector<int> current_group : bone_groups::all_groups) {
				previous = { 0.f, 0.f, 0.f };

				for (int currentBone : current_group) {
					current = bones->GetOrigin(currentBone);

					if (previous.x == 0 && previous.y == 0) {
						previous = current;
						continue;
					}

					current_screen_pos = world_to_screen(vars::view_matrix, current);
					previous_screen_pos = world_to_screen(vars::view_matrix, previous);

					if (current_screen_pos.x > 0.f && previous_screen_pos.x > 0.f)
						ImGui::GetForegroundDrawList()->AddLine({previous_screen_pos.x, previous_screen_pos.y}, {current_screen_pos.x, current_screen_pos.y}, IM_COL32(255, 0, 0, 255), 1.2f);
					previous = current;
				}
			}
		}
	}
}