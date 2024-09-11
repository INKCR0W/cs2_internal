#pragma once

// used: vector
#include <vector>
// used: CBasePlayerController
#include "../../sdk/entity.hpp"

#include "../../sdk/datatypes/matrix.hpp"

namespace features {
	namespace vars {
		inline entity::CCSPlayerController* local_player_controller = nullptr;
		inline entity::C_BasePlayerPawn* local_player_pawn = nullptr;
		inline std::vector<entity::CCSPlayerController*> player_list = {};

		inline uintptr_t entity_list_address = {};

		inline ViewMatrix_t view_matrix = {};
		inline entity::C_BaseEntity* observer = nullptr;
	}

	void update_entitys();
}
