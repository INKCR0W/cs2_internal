#include "aimbot.hpp"

// used: local_player_controller
#include "entity_var.hpp"
// used: world_to_screen
#include "utils.hpp"
// used: C_UTL_VECTOR
#include "../../sdk/const.hpp"
// used: read_memory
#include "../../memory/memory.hpp"
// used: offsets
#include "../../offset/offset.hpp"
// used: rcs_on
#include "../../config/config.hpp"
#include "../../render/menu.hpp"

#include "../../sdk/datatypes/vector.hpp"
#include "../../sdk/datatypes/qangle.hpp"

#include "../../third_party/imgui/imgui.h"

#include "../../utils/math.hpp"

#include "../../core/interfaces.hpp"


inline QAngle_t CalculateAngleScalar(const Vector_t& src, const Vector_t& dst)
{
    Vector_t OppPos;

    OppPos = dst - src;
    float distance = static_cast<float>(sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2)));

    float yaw = 0;
    float pitch = 0;

    yaw = atan2(OppPos.y, OppPos.x) * 180 / math::_PI;
    pitch = -atan2(OppPos.z, distance) * 180 / math::_PI;

    if (yaw < -180)
        yaw += 360;
    else if (yaw > 180)
        yaw -= 360;

    if (pitch < -89)
        pitch = -89;
    else if (pitch > 89)
        pitch = 89;

    return { pitch, yaw, 0 };
}

namespace features {
	void rcs() {
		if (!config::cfg.rcs_on)
			return;


        if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
            return;

        if (!features::vars::local_player_controller->m_bPawnIsAlive() && features::vars::local_player_base_pawn->m_iHealth() <= 0)
            return;

		// static std::uintptr_t view_angle_addr = memory::client_dll_addr + cs2_dumper::offsets::client_dll::dwViewAngles;
		// QAngle_t current_view_angle = memory::mem.read_memory<QAngle_t>(view_angle_addr);
        QAngle_t current_view_angle = vars::input->GetViewAngles();

		C_UTL_VECTOR punch_punch_cache = vars::local_player_pawn->m_aimPunchCache();


        if (punch_punch_cache.count == 0 || punch_punch_cache.data == nullptr)
            return;

		QAngle_t* current_punch_angle = &punch_punch_cache.data[punch_punch_cache.count - 1];

		if (current_punch_angle == nullptr)
			return;

        static QAngle_t previous_punch_angle = { 0.0f, 0.0f, 0.0f };

        if (vars::local_player_pawn->m_iShotsFired() > 1)
        {
            QAngle_t RecoilCompensation;
            RecoilCompensation.x = (current_punch_angle->x - previous_punch_angle.x) * config::cfg.rcs_x;
            RecoilCompensation.y = (current_punch_angle->y - previous_punch_angle.y) * config::cfg.rcs_y;
            RecoilCompensation.z = 0.0f;

            QAngle_t TargetAngles = current_view_angle - RecoilCompensation;

            TargetAngles.Normalize();
            TargetAngles.Clamp();

			// memory::mem.write_memory<QAngle_t>(view_angle_addr, TargetAngles);
            vars::input->SetViewAngle(TargetAngles);

			previous_punch_angle = *current_punch_angle;
        }
        else
        {
			previous_punch_angle = { 0.0f, 0.0f, 0.0f };
        }
	}

    void silent_aim(CUserCmd* pCmd)
    {
        vars::canhit = false;

        if (!config::cfg.silent_aim_on)
            return;

        float ClosestDistance = FLT_MAX;
        entity::CCSPlayerController* ClosestEntity = nullptr;

        for (auto& current_player : features::vars::player_list) {
            if (current_player == vars::local_player_controller || !current_player->m_bPawnIsAlive())
                continue;

            if (current_player->m_iTeamNum() == features::vars::local_player_controller->m_iTeamNum())
                continue;

            if (!current_player->get_pawn(vars::entity_list_address)->eye_visible(features::vars::local_player_pawn))
                continue;

            auto screen_point = features::world_to_screen(vars::view_matrix, current_player->get_base_pawn(vars::entity_list_address)->m_pGameSceneNode()->get_skeleton_instance()->pBoneCache->GetOrigin(6));

            if (screen_point.x < 0.f || screen_point.y < 0.f)
                continue;

            float current_dis = screen_point.DistanceTo({ menu::menu.screen_width / 2, menu::menu.screen_height / 2 });
            
            if (current_dis > config::cfg.silent_aim_fov)
                continue;

            float DistanceToLocal = vars::local_player_base_pawn->m_vOldOrigin().DistTo(current_player->get_base_pawn(vars::entity_list_address)->m_vOldOrigin());

            if (DistanceToLocal < ClosestDistance)
            {
                ClosestDistance = DistanceToLocal;
                ClosestEntity = current_player;
            }
        }


        if (ClosestEntity != nullptr)
        {
            vars::canhit = true;

            QAngle_t TargetAngles = CalculateAngleScalar(vars::local_player_base_pawn->get_eye_position(), ClosestEntity->get_base_pawn(vars::entity_list_address)->m_pGameSceneNode()->get_skeleton_instance()->pBoneCache->GetOrigin(6));

            TargetAngles.Normalize();
            TargetAngles.Clamp();

            QAngle_t PunchAngle = vars::local_player_controller->get_pawn(vars::entity_list_address)->m_aimPunchAngle();
            TargetAngles.x -= PunchAngle.x * 2.0f;
            TargetAngles.y -= PunchAngle.y * 2.0f;

            pCmd->SetSubTickAngle(TargetAngles);
        }
    }
}