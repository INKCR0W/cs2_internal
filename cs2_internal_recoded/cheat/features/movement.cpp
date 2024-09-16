#include "movement.hpp"

#include <array>
#include "entity_var.hpp"

#include "../../sdk/const.hpp"
#include "../../sdk/interfaces/csgoinput.hpp"
#include "../../sdk/datatypes/usercmd.hpp"
#include "../../core/interfaces.hpp"
#include "../../utils/math.hpp"
#include "../../config/config.hpp"
#include <DirectXMath.h>

#undef max
#undef min

bool pressed_jump = false;

template <typename T>
__forceinline T clamp(const T& n, const T& lower, const T& upper)
{
	return std::max(lower, std::min(n, upper));
}


__forceinline float normalize_yaw(float angle)
{
	auto revolutions = angle / 360.f;
	if (angle > 180.f || angle < -180.f)
	{
		revolutions = round(abs(revolutions));
		if (angle < 0.f)
			angle = (angle + 360.f * revolutions);
		else
			angle = (angle - 360.f * revolutions);
		return angle;
	}
	return angle;
}


inline QAngle_t CalcAngles(Vector_t viewPos, Vector_t aimPos)
{
	QAngle_t angle = { 0, 0, 0 };

	Vector_t delta = aimPos - viewPos;

	angle.x = -asin(delta.z / delta.Length()) * (180.0f / 3.141592654f);
	angle.y = atan2(delta.y, delta.x) * (180.0f / 3.141592654f);

	return angle;
}

Vector_t ExtrapolatePosition(Vector_t pos, Vector_t vel) {
	float ticks = 1.f;
	const float intervalpertick = 1.f / 64.f;

	return (pos)+(vel * intervalpertick * static_cast<float>(ticks)) + vel;
}

namespace features {
	void bhop(CUserCmd* cmd) {
		if (!config::cfg.bhop_on)
			return;

		static std::array<bool, 150> onground_tick = {};
		static std::array<bool, 150> remove_tick = {};

		int current_command = interfaces::input->nSequenceNumber;

		pressed_jump = cmd->nButtons.nValue & in_jump;
		onground_tick[current_command % 150] = vars::local_player_pawn->m_fFlags() & FL_ONGROUND;
		remove_tick[current_command % 150] = false;

		if (!pressed_jump)
			return;

		constexpr move_type bad_mts[] = { move_type::MOVETYPE_OBSERVER, move_type::MOVETYPE_NOCLIP };

		const auto in_water = vars::local_player_pawn->m_flWaterLevel() >= 2;
		const auto bad_mt = std::ranges::find(bad_mts, vars::local_player_pawn->m_MoveType()) != std::end(bad_mts);
		if (bad_mt || in_water)
			return;

		if ((!onground_tick[current_command % 150] && !onground_tick[(current_command - 1) % 150]) || onground_tick[current_command % 150] && onground_tick[(current_command - 1) % 150] && !remove_tick[(current_command - 1) % 150])
		{
			cmd->nButtons.nValue &= ~in_jump;
			cmd->nButtons.nValueChanged &= ~in_jump;
			cmd->nButtons.nValueScroll &= ~in_jump;
			remove_tick[current_command % 150] = true;
		}
	}

	void strafe(CCSGOInput* input) {
		if (!config::cfg.strafe_on)
			return;

		static uint64_t last_pressed = 0;
		static uint64_t last_buttons = 0;
		static uint64_t m_last_button = 0;

		auto cmd = input->GetUserCmd();
		auto base = cmd->csgoUserCmd.pBaseCmd;

		const auto current_buttons = cmd->nButtons.nValue;
		auto yaw = normalize_yaw(base->pViewAngles->angValue.y);

		const auto check_button = [&](const uint64_t button)
			{
				if (current_buttons & button && (!(last_buttons & button) || button & IN_MOVELEFT && !(last_pressed & IN_MOVERIGHT) || button &
					IN_MOVERIGHT && !(last_pressed & IN_MOVELEFT) || button & IN_FORWARD && !(last_pressed & IN_BACK) ||
					button & IN_BACK && !(last_pressed & IN_FORWARD)))
				{
					if (button & IN_MOVELEFT)
						last_pressed &= ~IN_MOVERIGHT;
					else if (button & IN_MOVERIGHT)
						last_pressed &= ~IN_MOVELEFT;
					else if (button & IN_FORWARD)
						last_pressed &= ~IN_BACK;
					else if (button & IN_BACK)
						last_pressed &= ~IN_FORWARD;

					last_pressed |= button;

				}
				else if (!(current_buttons & button))
					last_pressed &= ~button;
			};

		check_button(IN_MOVELEFT);
		check_button(IN_MOVERIGHT);
		check_button(IN_FORWARD);
		check_button(IN_BACK);

		last_buttons = current_buttons;

		const auto velocity = vars::local_player_pawn->m_vecAbsVelocity();
		float smoothing = 70.f;
		bool wasdstrafe = true;

		if (vars::local_player_pawn->m_fFlags() & FL_ONGROUND)
			return;

		auto rotate_movement = [](CUserCmd& cmd, float target_yaw, CBaseUserCmdPB* bb)
			{
				CBaseUserCmdPB* m_base_cmd = bb;

				const float rot = M_DEG2RAD(m_base_cmd->pViewAngles->angValue.y - target_yaw);

				const float new_forward = std::cos(rot) * m_base_cmd->flForwardMove - std::sin(rot) * m_base_cmd->flSideMove;
				const float new_side = std::sin(rot) * m_base_cmd->flForwardMove + std::cos(rot) * m_base_cmd->flSideMove;

				cmd.nButtons.nValue &= (~IN_FORWARD | ~IN_BACK | ~IN_MOVELEFT | ~IN_MOVERIGHT);

				bb->pInButtonState->SetBits(EButtonStatePBBits::BUTTON_STATE_PB_BITS_BUTTONSTATE1);
				bb->pInButtonState->nValue &= (~IN_FORWARD | ~IN_BACK | ~IN_MOVELEFT | ~IN_MOVERIGHT);

				m_base_cmd->SetBits(32U);
				m_base_cmd->flForwardMove = clamp(new_forward, -1.f, 1.f);

				m_base_cmd->SetBits(64U);
				m_base_cmd->flSideMove = clamp(new_side * -1.f, -1.f, 1.f);

				if (m_base_cmd->flForwardMove > 0.f) {
					cmd.nButtons.nValue |= IN_FORWARD;
					bb->pInButtonState->SetBits(IN_FORWARD);
					m_last_button |= IN_FORWARD;
				}
				else if (m_base_cmd->flForwardMove < 0.f) {
					cmd.nButtons.nValue |= IN_BACK;
					bb->pInButtonState->SetBits(IN_BACK);
					m_last_button |= IN_BACK;
				}
				if (m_base_cmd->flSideMove > 0.f) {
					cmd.nButtons.nValue |= IN_MOVELEFT;
					bb->pInButtonState->SetBits(IN_MOVELEFT);
					m_last_button |= IN_MOVELEFT;
				}
				else if (m_base_cmd->flSideMove < 0.f) {
					cmd.nButtons.nValue |= IN_MOVERIGHT;
					bb->pInButtonState->SetBits(IN_MOVERIGHT);
					m_last_button |= IN_MOVERIGHT;
				}

			};

		auto m_base_cmd = base;
		if (!m_base_cmd)
			return;

		if (wasdstrafe)
		{
			auto offset = 0.f;
			if (last_pressed & IN_MOVELEFT)
				offset += 90.f;
			if (last_pressed & IN_MOVERIGHT)
				offset -= 90.f;
			if (last_pressed & IN_FORWARD)
				offset *= 0.5f;
			else if (last_pressed & IN_BACK)
				offset = -offset * 0.5f + 180.f;

			yaw += offset;

			m_base_cmd->SetBits(BASE_BITS_FORWARDMOVE);
			m_base_cmd->flForwardMove = 0.f;

			m_base_cmd->SetBits(BASE_BITS_LEFTMOVE);
			m_base_cmd->flSideMove = 0.f;

			rotate_movement(*cmd, normalize_yaw(yaw), base);

		}

		if (m_base_cmd->flSideMove != 0.0f || m_base_cmd->flForwardMove != 0.0f)
			return;

		auto velocity_angle = M_RAD2DEG(std::atan2f(velocity.y, velocity.x));
		if (velocity_angle < 0.0f)
			velocity_angle += 360.0f;

		if (velocity_angle < 0.0f)
			velocity_angle += 360.0f;

		velocity_angle -= floorf(velocity_angle / 360.0f + 0.5f) * 360.0f;

		const auto speed = velocity.Length2D();
		const auto ideal = clamp(M_RAD2DEG(std::atan2(15.f, speed)), 0.f, 45.f);

		const auto correct = (100.f - smoothing) * 0.02f * (ideal + ideal);

		m_base_cmd->SetBits(BASE_BITS_FORWARDMOVE);
		m_base_cmd->flForwardMove = 0.f;
		const auto velocity_delta = normalize_yaw(yaw - velocity_angle);

		if (fabsf(velocity_delta) > 170.f && speed > 80.f || velocity_delta > correct && speed > 80.f)
		{
			yaw = correct + velocity_angle;
			m_base_cmd->SetBits(BASE_BITS_LEFTMOVE);
			m_base_cmd->flSideMove = -1.f;
			rotate_movement(*cmd, normalize_yaw(yaw), base);
			return;
		}
		const bool side_switch = interfaces::input->nSequenceNumber % 2 == 0;

		if (-correct <= velocity_delta || speed <= 80.f)
		{
			if (side_switch)
			{
				yaw = yaw - ideal;
				m_base_cmd->SetBits(BASE_BITS_LEFTMOVE);
				m_base_cmd->flSideMove = -1.f;

			}
			else
			{
				yaw = ideal + yaw;
				m_base_cmd->SetBits(BASE_BITS_LEFTMOVE);
				m_base_cmd->flSideMove = 1.f;

			}
			rotate_movement(*cmd, normalize_yaw(yaw), base);
		}
		else
		{
			yaw = velocity_angle - correct;
			m_base_cmd->SetBits(BASE_BITS_LEFTMOVE);
			m_base_cmd->flSideMove = 1.f;

			rotate_movement(*cmd, normalize_yaw(yaw), base);
		}

		//if (!config::cfg.strafe_on)
		//	return;

		//static uint64_t last_pressed = 0;
		//static uint64_t last_buttons = 0;

		//auto& cmd = interfaces::input->arrCommands[interfaces::input->nSequenceNumber % 150];
		//auto pUserCmd = cmd.csgoUserCmd.pBaseCmd;
		//bool strafe_assist = false;
		//const auto current_buttons = cmd.nButtons.nValue;
		//auto yaw = normalize_yaw(pUserCmd->pViewAngles->angValue.y);

		//const auto check_button = [&](const uint64_t button)
		//	{
		//		if (current_buttons & button && (!(last_buttons & button) || button & IN_MOVELEFT && !(last_pressed & IN_MOVERIGHT) || button & IN_MOVERIGHT && !(last_pressed & IN_MOVELEFT) || button & IN_FORWARD && !(last_pressed & IN_BACK) || button & IN_BACK && !(last_pressed & IN_FORWARD)))
		//		{
		//			if (strafe_assist)
		//			{
		//				if (button & IN_MOVELEFT)
		//					last_pressed &= ~IN_MOVERIGHT;
		//				else if (button & IN_MOVERIGHT)
		//					last_pressed &= ~IN_MOVELEFT;
		//				else if (button & IN_FORWARD)
		//					last_pressed &= ~IN_BACK;
		//				else if (button & IN_BACK)
		//					last_pressed &= ~IN_FORWARD;
		//			}

		//			last_pressed |= button;
		//		}
		//		else if (!(current_buttons & button))
		//			last_pressed &= ~button;
		//	};

		//check_button(IN_MOVELEFT);
		//check_button(IN_MOVERIGHT);
		//check_button(IN_FORWARD);
		//check_button(IN_BACK);

		//last_buttons = current_buttons;

		//const auto velocity = vars::local_player_pawn->m_vecAbsVelocity();

		//bool wasdstrafe = false;
		//bool viewanglestrafe = true;

		//float smoothing = 0;

		///*const auto weapon = pLocalPawn->get_weapon_services_ptr()->get_h_active_weapon().get();
		//const auto js = weapon && (cfg.weapon_config.is_scout && cfg.weapon_config.cur.scout_jumpshot && pLocalPawn->get_vec_abs_velocity().length_2d() < 50.f);
		//const auto throwing_nade = weapon && weapon->is_grenade() && ticks_to_time(local_player->get_tickbase()) >= weapon->get_throw_time() && weapon->get_throw_time() != 0.f;

		//if (js)
		//	return;*/

		//if (vars::local_player_pawn->m_fFlags() & FL_ONGROUND)
		//	return;

		//auto rotate_movement = [](CUserCmd& cmd, float target_yaw)
		//	{
		//		auto pUserCmd = cmd.csgoUserCmd.pBaseCmd;

		//		const float rot = M_DEG2RAD(pUserCmd->pViewAngles->angValue.y - target_yaw);

		//		const float new_forward = std::cos(rot) * pUserCmd->flForwardMove - std::sin(rot) * pUserCmd->flSideMove;
		//		const float new_side = std::sin(rot) * pUserCmd->flForwardMove + std::cos(rot) * pUserCmd->flSideMove;

		//		cmd.nButtons.nValue &= ~(IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT);
		//		pUserCmd->flForwardMove = clamp(new_forward, -1.f, 1.f);
		//		pUserCmd->flSideMove = clamp(new_side * -1.f, -1.f, 1.f);

		//		if (pUserCmd->flForwardMove > 0.f)
		//			cmd.nButtons.nValue |= IN_FORWARD;
		//		else if (pUserCmd->flForwardMove < 0.f)
		//			cmd.nButtons.nValue |= IN_BACK;

		//		if (pUserCmd->flSideMove > 0.f)
		//			cmd.nButtons.nValue |= IN_MOVELEFT;
		//		else if (pUserCmd->flSideMove < 0.f)
		//			cmd.nButtons.nValue |= IN_MOVERIGHT;
		//	};

		//if (wasdstrafe)
		//{
		//	auto offset = 0.f;
		//	if (last_pressed & IN_MOVELEFT)
		//		offset += 90.f;
		//	if (last_pressed & IN_MOVERIGHT)
		//		offset -= 90.f;
		//	if (last_pressed & IN_FORWARD)
		//		offset *= 0.5f;
		//	else if (last_pressed & IN_BACK)
		//		offset = -offset * 0.5f + 180.f;

		//	yaw += offset;

		//	pUserCmd->flForwardMove = 0.f;
		//	pUserCmd->flSideMove = 0.f;

		//	rotate_movement(cmd, normalize_yaw(yaw));

		//	if (!viewanglestrafe && offset == 0.f)
		//		return;
		//}

		//if (pUserCmd->flSideMove != 0.0f || pUserCmd->flForwardMove != 0.0f)
		//	return;

		//auto velocity_angle = M_RAD2DEG(std::atan2f(velocity.y, velocity.x));
		//if (velocity_angle < 0.0f)
		//	velocity_angle += 360.0f;

		//if (velocity_angle < 0.0f)
		//	velocity_angle += 360.0f;

		//velocity_angle -= floorf(velocity_angle / 360.0f + 0.5f) * 360.0f;

		//const auto speed = velocity.Length2D();
		//const auto ideal = clamp(M_RAD2DEG(std::atan2(15.f, speed)), 0.f, 45.f);

		//const auto correct = (100.f - config::cfg.strafe_smooth) * 0.02f * (ideal + ideal);

		//pUserCmd->flForwardMove = 0.f;
		//const auto velocity_delta = normalize_yaw(yaw - velocity_angle);

		///*if (throwing_nade && fabsf(velocity_delta) <=20.f)
		//{
		//	auto &wish_angle = antiaim::wish_angles[globals::current_cmd->command_number % 150];
		//	wish_angle.y = math::normalize_yaw(yaw);
		//	globals::current_cmd->forwardmove = 450.f;

		//	antiaim::fix_movement(globals::current_cmd);
		//	return;
		//}*/

		//if (fabsf(velocity_delta) > 170.f && speed > 80.f || velocity_delta > correct && speed > 80.f)
		//{
		//	yaw = correct + velocity_angle;
		//	pUserCmd->flSideMove = -1.f;
		//	rotate_movement(cmd, normalize_yaw(yaw));
		//	return;
		//}
		//const bool side_switch = interfaces::input->nSequenceNumber % 2 == 0;

		//if (-correct <= velocity_delta || speed <= 80.f)
		//{
		//	if (side_switch)
		//	{
		//		yaw = yaw - ideal;
		//		pUserCmd->flSideMove = -1.f;
		//	}
		//	else
		//	{
		//		yaw = ideal + yaw;
		//		pUserCmd->flSideMove = 1.f;
		//	}
		//	rotate_movement(cmd, normalize_yaw(yaw));
		//}
		//else
		//{
		//	yaw = velocity_angle - correct;
		//	pUserCmd->flSideMove = 1.f;

		//	rotate_movement(cmd, normalize_yaw(yaw));
		//}
	}

	void autostop(CUserCmd* cmd) {
		CBaseUserCmdPB* pCmd = cmd->csgoUserCmd.pBaseCmd;

		if (vars::local_player_pawn->m_fFlags() & FL_ONGROUND && !(cmd->nButtons.nValue & IN_JUMP) && vars::local_player_pawn->m_vecAbsVelocity().Length() <= 5.f && vars::canhit) {
			pCmd->pInButtonState->SetBits(BASE_BITS_FORWARDMOVE);
			pCmd->pInButtonState->SetBits(BASE_BITS_LEFTMOVE);
			pCmd->flForwardMove = 0.f;
			pCmd->flSideMove = 0.f;
		}

		if (vars::local_player_pawn->m_fFlags() & FL_ONGROUND && !(cmd->nButtons.nValue & IN_JUMP) && vars::local_player_pawn->m_vecAbsVelocity().Length() > 5.f && vars::canhit) {

			pCmd->flForwardMove = 1.f;
			pCmd->flSideMove = 0.f;
			Vector_t* move = (Vector_t*)&pCmd->flForwardMove;
			Vector_t move_backup = *move;
			const QAngle_t& current_angles = pCmd->pViewAngles->angValue;

			QAngle_t angle = CalcAngles(vars::local_player_pawn->m_pGameSceneNode()->m_vecAbsOrigin(), ExtrapolatePosition(vars::local_player_pawn->m_pGameSceneNode()->m_vecAbsOrigin(), vars::local_player_pawn->m_vecAbsVelocity() * -5.f));

			const float delta = remainderf(angle.y - current_angles.y, 360.f);
			const float yaw = DirectX::XMConvertToRadians(delta);

			move->x = move_backup.x * std::cos(yaw) - move_backup.y * std::sin(yaw);
			move->y = move_backup.x * std::sin(yaw) + move_backup.y * std::cos(yaw);

			float mul = 1.f;

			if (std::fabsf(move->x) > 1.f)
				mul = 1.f / std::fabsf(move->x);
			else if (std::fabsf(move->y) > 1.f)
				mul = 1.f / std::fabsf(move->y);

			move->x *= mul;
			move->y *= mul;
			move->z = 0.f;

			pCmd->pInButtonState->SetBits(BASE_BITS_FORWARDMOVE);
			pCmd->pInButtonState->SetBits(BASE_BITS_LEFTMOVE);
			pCmd->flForwardMove = move->x * mul;
			pCmd->flSideMove = move->y * mul;
		}
	}
}