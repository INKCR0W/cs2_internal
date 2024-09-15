#pragma once

#include "../../sdk/datatypes/vector.hpp"
#include "../../sdk/datatypes/matrix.hpp"


namespace features {
	Vector2D_t world_to_screen(ViewMatrix_t matrix, Vector_t position);
}
