#include "aimbot.hpp"

// used: local_player_controller
#include "entity_var.hpp"
// used: C_UTL_VECTOR
#include "../../sdk/const.hpp"
// used: read_memory
#include "../../memory/memory.hpp"
// used: offsets
#include "../../offset/offset.hpp"
// used: rcs_on
#include "../../config/config.hpp"

#include "../../sdk/datatypes/vector.hpp"
#include "../../sdk/datatypes/qangle.hpp"

#include "../../third_party/imgui/imgui.h"

namespace features {
	void rcs() {
		if (!config::cfg.rcs_on)
			return;

		auto local_pawn = vars::local_player_controller->get_pawn(vars::entity_list_address);

		static std::uintptr_t view_angle_addr = memory::client_dll_addr + cs2_dumper::offsets::client_dll::dwViewAngles;
		QAngle_t current_view_angle = memory::mem.read_memory<QAngle_t>(view_angle_addr);

		C_UTL_VECTOR punch_punch_cache = local_pawn->m_aimPunchCache();


        if (punch_punch_cache.count == 0 || punch_punch_cache.data == nullptr)
            return;

		QAngle_t* current_punch_angle = &punch_punch_cache.data[punch_punch_cache.count - 1];

		if (current_punch_angle == nullptr)
			return;

        static QAngle_t previous_punch_angle = { 0.0f, 0.0f, 0.0f };

        if (local_pawn->m_iShotsFired() > 1)
        {
            QAngle_t RecoilCompensation;
            RecoilCompensation.x = (current_punch_angle->x - previous_punch_angle.x) * config::cfg.rcs_x;
            RecoilCompensation.y = (current_punch_angle->y - previous_punch_angle.y) * config::cfg.rcs_y;
            RecoilCompensation.z = 0.0f;

            QAngle_t TargetAngles = current_view_angle - RecoilCompensation;

            TargetAngles.Normalize();
            TargetAngles.Clamp();

			memory::mem.write_memory<QAngle_t>(view_angle_addr, TargetAngles);

			previous_punch_angle = *current_punch_angle;
        }
        else
        {
			previous_punch_angle = { 0.0f, 0.0f, 0.0f };
        }
	}
}