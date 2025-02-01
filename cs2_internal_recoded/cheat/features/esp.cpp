#include "esp.hpp"

// used: player_list
#include "entity_var.hpp"
// used: engineclient
#include "../../core/interfaces.hpp"
// used: entity structs
#include "../../sdk/entity.hpp"
// used: consts
#include "../../sdk/const.hpp"
// used: vars
#include "../../config/config.hpp"
// used: screen_width
#include "../../render/menu.hpp"
// used: imgui api
#include "../../third_party/imgui/imgui.h"
// used: world_to_screen
#include "utils.hpp"

#include "../../utils/math.hpp"

#include <string>

#undef max

namespace features {
	void draw_skeleton() {
		if (!config::cfg.skeleton_on)
			return;

		if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
			return;

		if (vars::local_player_controller == nullptr)
			return;


		for (auto& current_player : features::vars::player_list) {
			auto current_player_pawn = current_player->get_base_pawn(vars::entity_list_address);
			
			if (current_player_pawn == nullptr || current_player_pawn == vars::observer)
				 continue;

			bool is_teammate = current_player_pawn->m_iTeamNum() == vars::observer->m_iTeamNum();

			// casual
			//if (game_type == GAMETYPE_CLASSIC && game_mode == GAMEMODE_CLASSIC_CASUAL && is_teammate)
			//	continue;
			if (is_teammate)
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

	void draw_spectator_list() {
		if (!config::cfg.spectator_list_on)
			return;

		if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
			return;

		if (vars::local_player_base_pawn == nullptr)
			return;


		auto draw_list = ImGui::GetForegroundDrawList();

		float y_offset = 0;

		for (auto& current_player : features::vars::player_list) {
			auto current_player_pawn = current_player->get_base_pawn(vars::entity_list_address);

			if (current_player_pawn == nullptr)
				continue;

			auto current_obs = current_player_pawn->m_pObserverServices();
			if (current_obs == nullptr)
				continue;

			if (current_obs->get_target_pawn(vars::entity_list_address) == vars::observer) {
				const char* pawn_name_address = current_player->m_sSanitizedPlayerName();
				const auto size = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFont()->FontSize, std::numeric_limits<float>::max(), 0.0f, pawn_name_address);
				draw_list->AddText({ menu::menu.screen_width - size.x, y_offset }, IM_COL32(255, 255, 255, 255), pawn_name_address);
				y_offset += 20;
			}
		}
	}

	void draw_silent_aim_fov() {
		if (!config::cfg.silent_aim_on || !config::cfg.draw_dilent_aim_fov)
			return;

		if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
			return;


		ImGui::GetForegroundDrawList()->AddCircle({menu::menu.screen_width / 2.f, menu::menu.screen_height / 2.f}, config::cfg.silent_aim_fov, IM_COL32(255, 0, 0, 255), 0, 0.3f);
	}
}