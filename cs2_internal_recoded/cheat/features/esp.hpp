#pragma once

#include "../../core/inputsystem.hpp"
#include "../../sdk/datatypes/matrix.hpp"
#include "../../sdk/datatypes/vector.hpp"

// used: screen_width screen_height
#include "../../render/menu.hpp"

namespace features {
	Vector2D_t world_to_screen(ViewMatrix_t matrix, Vector_t position);

	void draw_skeleton();
	void draw_spectator_list();
}
