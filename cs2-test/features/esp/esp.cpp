#include "../cheat.hpp"
#include "../../offset/offset.hpp"

namespace cheat {
	void cs2_internal::esp() {
		using namespace cs2_dumper;
		view_matrix_t view_matrix = read_memory<view_matrix_t>(client_dll_addr + offsets::client_dll::dwViewMatrix);
		Vec3 previous, current, offset_current;
		Vec2 current_screen_pos, previous_screen_pos, offset_screen_pos;

		const uint8_t self_team = read_memory<uint8_t>(local_player_pawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);

		auto draw_list = ImGui::GetForegroundDrawList();
		const ImU32 current_color = ImGui::ColorConvertFloat4ToU32(esp_color);

		for (const entity& current_entity : player_entity_list) {
			if (read_memory<uint8_t>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_iTeamNum) == self_team)
				continue;

			const uintptr_t game_scene_node = read_memory<uintptr_t>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
			// const uintptr_t bone_array = read_memory<uintptr_t>(game_scene_node + schemas::client_dll::CSkeletonInstance::m_modelState + schemas::client_dll::CGameSceneNode::m_vecOrigin);
			const uintptr_t bone_array = read_memory<uintptr_t>(game_scene_node + 0x1F0);

			float left = 99999.f;
			float right = -1.f;
			float top = 99999.f;
			float bottom = -1.f;


			for (std::vector<int> current_group : bone_groups::all_groups) {
				previous = { 0.f, 0.f, 0.f };

				for (int currentBone : current_group) {
					current = read_memory<Vec3>(bone_array + currentBone * sizeof(BoneJointData));

					offset_current = { current.x, current.y, current.z - 5 };

					if (previous.x == 0 && previous.y == 0) {
						previous = current;

						current_screen_pos = world_to_screen(view_matrix, { current.x, current.y, current.z + 7 });
						if (current_screen_pos.x > 0.f) {
							left = left > current_screen_pos.x ? current_screen_pos.x : left;
							right = right < current_screen_pos.x ? current_screen_pos.x : right;
							top = top > current_screen_pos.y ? current_screen_pos.y : top;
							bottom = bottom < current_screen_pos.y ? current_screen_pos.y : bottom;
						}
						continue;
					}

					current_screen_pos = world_to_screen(view_matrix, current);
					previous_screen_pos = world_to_screen(view_matrix, previous);
					offset_screen_pos = world_to_screen(view_matrix, offset_current);

					if (current_screen_pos.x > 0.f) {
						left = left > current_screen_pos.x ? current_screen_pos.x : left;
						right = right < current_screen_pos.x ? current_screen_pos.x : right;
						top = top > current_screen_pos.y ? current_screen_pos.y : top;
						bottom = bottom < offset_screen_pos.y ? offset_screen_pos.y : bottom;
					}

					if (skeleton) {
						if (current_screen_pos.x > 0.f && previous_screen_pos.x > 0.f)
							draw_list->AddLine({ previous_screen_pos.x, previous_screen_pos.y }, { current_screen_pos.x, current_screen_pos.y }, current_color, 1.2f);
					}


					previous = current;
				}
			}

			left -= 1;
			right += 1;

			if (box && right > 5.f) {
				Vec2 head_pos = world_to_screen(view_matrix, read_memory<Vec3>(bone_array + bones::head * sizeof(BoneJointData)));
				Vec2 screen_pos = world_to_screen(view_matrix, read_memory<Vec3>(current_entity.pawn + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin));

				Vec2 size, pos;
				size.y = (screen_pos.y - head_pos.y) * 1.09f;
				size.x = size.y * 0.6f;

				pos = { screen_pos.x - size.x / 2.f, head_pos.y - size.y * 0.08f };

				draw_list->AddRect({ pos.x, pos.y }, { pos.x + size.x, pos.y + size.y }, current_color);
			}

			if (health) {
				const int current_health = read_memory<int>(current_entity.pawn + schemas::client_dll::C_BaseEntity::m_iHealth);

				float healthPercentage = static_cast<float>(current_health) / 100.f;

				uint8_t r = static_cast<uint8_t>((1.0f - healthPercentage) * 255);
				uint8_t g = static_cast<uint8_t>(healthPercentage * 255);

				draw_list->AddText({ right, top }, IM_COL32(r, g, 0, 255), std::to_string(current_health).c_str());
			}

			if (name) {
				const uintptr_t pawn_name_address = read_memory<uintptr_t>(current_entity.controller + schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName);
				float half_size = ImGui::CalcTextSize(reinterpret_cast<const char*>(pawn_name_address)).x / 2;
				draw_list->AddText({ left + half_size, bottom }, current_color, reinterpret_cast<const char*>(pawn_name_address));
			}

			if (eye_ray) {
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

				draw_list->AddLine({ start_point.x, start_point.y }, { end_point.x, end_point.y }, current_color, 1.2f);
			}
		}

	}
}