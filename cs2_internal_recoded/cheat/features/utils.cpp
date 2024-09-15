#include "utils.hpp"

#include "../../render/menu.hpp"

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
}