#pragma once

// used: vector
#include <vector>
// used: CBasePlayerController
#include "../../sdk/entity.hpp"

#include "../../sdk/datatypes/matrix.hpp"

namespace features {
	namespace vars {
		entity::CCSPlayerController* local_player_controller = nullptr;
		std::vector<entity::CCSPlayerController*> player_list = {};

		uintptr_t entity_list_address = {};

		ViewMatrix_t view_matrix = {};
	}

	void update_entitys();
}
