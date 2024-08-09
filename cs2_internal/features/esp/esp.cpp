#include <limits>

#include "../cheat.hpp"
#include "../../utils/structs.hpp"
#include "../../offset/offset.hpp"

#undef max

namespace cheat {
	void cs2_internal::esp() {
		using namespace cs2_dumper;
		view_matrix_t view_matrix = read_memory<view_matrix_t>(client_dll_addr + offsets::client_dll::dwViewMatrix);
		Vec3 previous, current, offset_current;
		Vec2 current_screen_pos, previous_screen_pos, offset_screen_pos;

		const uint8_t self_team = read_memory<uint8_t>(local_player_pawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);

		auto draw_list = ImGui::GetForegroundDrawList();
		const ImU32 current_color = ImGui::ColorConvertFloat4ToU32(esp_color);

		const auto entity_list_address = read_memory<uintptr_t>(client_dll_addr + offsets::client_dll::dwEntityList);
		const uint32_t local_spectator_pawn_handle = read_memory<uint32_t>(local_player_controller + schemas::client_dll::CBasePlayerController::m_hPawn);
		const uintptr_t local_spectator_pawn = get_from_handle(entity_list_address, local_spectator_pawn_handle);
		const uintptr_t obs = read_memory<uintptr_t>(local_spectator_pawn + schemas::client_dll::C_BasePlayerPawn::m_pObserverServices);
		const uint64_t oTarget = read_memory<uint64_t>(obs + schemas::client_dll::CPlayer_ObserverServices::m_hObserverTarget);
		uintptr_t my_target = get_from_handle(entity_list_address, oTarget);

		if (!obs) {
			my_target = local_spectator_pawn;
		}

		float y_offset = 0;

		for (const entity& current_entity : player_entity_list) {
			if (current_entity.pawn == 0 || current_entity.controller == 0)
				continue;

			if (spectator_list) {
				uint32_t spectatorPawn = read_memory<uint32_t>(current_entity.controller + schemas::client_dll::CBasePlayerController::m_hPawn);
				uintptr_t pawn = get_from_handle(entity_list_address, spectatorPawn);

				uintptr_t obs = read_memory<uintptr_t>(pawn + schemas::client_dll::C_BasePlayerPawn::m_pObserverServices);
				uint64_t oTarget = read_memory<uint64_t>(obs + schemas::client_dll::CPlayer_ObserverServices::m_hObserverTarget);
				uintptr_t target_pawn = get_from_handle(entity_list_address, oTarget);

				//ImGui::Begin("Spectator List", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				if (obs && (target_pawn == my_target)) {
					const char* pawn_name_address = reinterpret_cast<const char*>(read_memory<uintptr_t>(current_entity.controller + schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName));
					const auto size = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFont()->FontSize, std::numeric_limits<float>::max(), 0.0f, pawn_name_address);
					draw_list->AddText({ screen_width - size.x, y_offset }, current_color, pawn_name_address);
					y_offset += size.y;
					//ImGui::Text(pawn_name_address);
				}
				//ImGui::End();
			}

			if (read_memory<uint8_t>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_iTeamNum) == self_team && !teammate_esp)
				continue;

			const bool is_alive = (read_memory<bool>(current_entity.controller + schemas::client_dll::CCSPlayerController::m_bPawnIsAlive) && (read_memory<int>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_iHealth) > 0));

			const uintptr_t game_scene_node = read_memory<uintptr_t>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
			// const uintptr_t bone_array = read_memory<uintptr_t>(game_scene_node + schemas::client_dll::CSkeletonInstance::m_modelState + schemas::client_dll::CGameSceneNode::m_vecOrigin);
			const uintptr_t bone_array = read_memory<uintptr_t>(game_scene_node + 0x1F0);


			for (std::vector<int> current_group : bone_groups::all_groups) {
				previous = { 0.f, 0.f, 0.f };

				for (int currentBone : current_group) {
					current = read_memory<Vec3>(bone_array + currentBone * sizeof(BoneJointData));

					if (previous.x == 0 && previous.y == 0) {
						previous = current;
						continue;
					}

					current_screen_pos = world_to_screen(view_matrix, current);
					previous_screen_pos = world_to_screen(view_matrix, previous);

					if (skeleton && is_alive) {
						if (current_screen_pos.x > 0.f && previous_screen_pos.x > 0.f)
							draw_list->AddLine({ previous_screen_pos.x, previous_screen_pos.y }, { current_screen_pos.x, current_screen_pos.y }, current_color, 1.2f);
					}


					previous = current;
				}
			}

			Vec2 head_pos = world_to_screen(view_matrix, read_memory<Vec3>(bone_array + bones::head * sizeof(BoneJointData)));

			if (head_pos.x < 0)
				continue;

			Vec2 origin_pos = world_to_screen(view_matrix, read_memory<Vec3>(current_entity.pawn + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin));

			Vec2 size, pos;
			size.y = (origin_pos.y - head_pos.y) * 1.09f;
			size.x = size.y * 0.6f;

			pos = { origin_pos.x - size.x / 2.f, head_pos.y - size.y * 0.08f };

			if (box && is_alive) {
				draw_list->AddRect({ pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, current_color);
			}

			if (name && is_alive) {
				const char* pawn_name_address = reinterpret_cast<const char*>(read_memory<uintptr_t>(current_entity.controller + schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName));
				const float half_size = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFont()->FontSize, std::numeric_limits<float>::max(), 0.0f, pawn_name_address).x / 2;
				
				draw_list->AddText({ (pos.x + size.x / 2) - half_size, pos.y + size.y }, current_color, pawn_name_address);
			}

			if (health && is_alive) {
				const int current_health = read_memory<int>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_iHealth);

				float healthPercentage = static_cast<float>(current_health) / 100.f;

				uint8_t r = static_cast<uint8_t>((1.0f - healthPercentage) * 255);
				uint8_t g = static_cast<uint8_t>(healthPercentage * 255);

				draw_list->AddText({ pos.x + size.x, pos.y }, IM_COL32(r, g, 0, 255), std::to_string(current_health).c_str());
			}

			if (eye_ray && is_alive) {
				Vec2 start_point, end_point;
				Vec3 temp;

				Vec3 head = read_memory<Vec3>(bone_array + bones::head * sizeof(BoneJointData));
				Vec2 view_angle = read_memory<Vec2>(current_entity.pawn + schemas::client_dll::C_CSPlayerPawnBase::m_angEyeAngles);

				start_point = world_to_screen(view_matrix, head);

				float LineLength = cos(view_angle.x * M_PI / 180) * 50.f;

				temp.x = head.x + cos(view_angle.y * M_PI / 180) * LineLength;
				temp.y = head.y + sin(view_angle.y * M_PI / 180) * LineLength;
				temp.z = head.z - sin(view_angle.x * M_PI / 180) * 50.f;

				end_point = world_to_screen(view_matrix, temp);

				if (start_point.x > 0 && end_point.x > 0)
					draw_list->AddLine({ start_point.x, start_point.y }, { end_point.x, end_point.y }, current_color, 1.2f);
			}
		}

		//if (spectator_list) {
		//	float y_offset = 0;
		//	for (const std::uintptr_t& spec : spectators_list) {
		//		const char* pawn_name_address = reinterpret_cast<const char*>(read_memory<uintptr_t>(spec + schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName));
		//		const auto size = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFont()->FontSize, std::numeric_limits<float>::max(), 0.0f, pawn_name_address);
		//		draw_list->AddText({ screen_width - size.x, y_offset }, current_color, pawn_name_address);
		//		y_offset += size.y;
		//	}
		//}

	}
}