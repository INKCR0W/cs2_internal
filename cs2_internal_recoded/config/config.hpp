#pragma once

// used: vector
#include <vector>

// #include <WinUser.h>

namespace config {

	struct key_bind {
		uint32_t name_hash = 0;
		unsigned int key = 0;
	};

	class config_class {
	public:
		config_class() = default;
		~config_class() = default;

		config_class(const config_class& other) = delete;
		config_class& operator=(const config_class& other) = delete;

#pragma region key_binds
		unsigned int menu_key = 0x2D;
#pragma endregion

#pragma region vars
		bool skeleton_on = false;

		bool spectator_list_on = false;

		bool rcs_on = false;
		float rcs_x = 2.40f;
		float rcs_y = 2.55f;

		bool silent_aim_on = false;
		bool draw_dilent_aim_fov = false;
		float silent_aim_fov = 30.f;

		bool bhop_on = false;
		bool strafe_on = false;
		float strafe_smooth = 80.0f;
		int strafe_mode = 0; // 0-wasd 1-viewangle
#pragma endregion

	private:

	};


	inline config_class cfg;
}
