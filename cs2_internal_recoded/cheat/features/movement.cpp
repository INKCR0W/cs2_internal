#include "movement.hpp"

#include <array>
#include "entity_var.hpp"

#include "../../sdk/const.hpp"
#include "../../sdk/interfaces/csgoinput.hpp"
#include "../../core/interfaces.hpp"
#include "../../utils/math.hpp"

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


namespace features {
	void bhop(CUserCmd* cmd) {
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
		static uint64_t last_pressed = 0;
		static uint64_t last_buttons = 0;

		auto cmd = input->GetUserCmd();

		const auto current_buttons = cmd->nButtons.nValue | cmd->nButtons.nValueChanged;
		auto yaw = normalize_yaw(vars::input->GetViewAngles().y);

		const auto check_button = [&](const uint64_t button)
			{
				if (current_buttons & button && (!(last_buttons & button) || button & in_moveleft && !(last_pressed & in_moveright) || button &
					in_moveright && !(last_pressed & in_moveleft) || button & in_forward && !(last_pressed & in_back) ||
					button &in_back && !(last_pressed & in_forward)))
				{
					// if (cfg.misc.autostrafe->test(cfg_t::autostrafe_easy_strafe))
					if (true)
					{
						if (button & in_moveleft)
							last_pressed &= ~in_moveright;
						else if (button &in_moveright)
							last_pressed &= ~in_moveleft;
						else if (button & in_forward)
							last_pressed &= ~in_back;
						else if (button &in_back)
							last_pressed &= ~in_forward;
					}

					last_pressed |= button;
				}
				else if (!(current_buttons & button))
					last_pressed &= ~button;
			};

		check_button(in_moveleft);
		check_button(in_moveright);
		check_button(in_forward);
		check_button(in_back);

		last_buttons = current_buttons;

		const auto velocity = vars::local_player_pawn->m_vecAbsVelocity();

		if (!vars::local_player_controller->m_bPawnIsAlive() || vars::local_player_controller->m_flWaterLevel() >= 2 || vars::local_player_controller->m_MoveType() ==
			MOVETYPE_NOCLIP || vars::local_player_controller->m_MoveType() == MOVETYPE_LADDER)
			return;

		if (!pressed_jump || current_buttons & in_speed)
			return;

		//if (!pressed_jump && !cfg.misc.autostrafe->test(cfg_t::autostrafe_jump_release) && !cfg.misc.edge_jump.get() || current_buttons & in_speed)
		//	return;

		//if (!cfg.misc.autostrafe->test(cfg_t::autostrafe_movement_keys) && !cfg.misc.autostrafe->test(cfg_t::autostrafe_viewangles))
		//	return;

		if (vars::local_player_pawn->m_fFlags() & FL_ONGROUND)
			return;

		// if (cfg.misc.autostrafe->test(cfg_t::autostrafe_movement_keys))
		if (true)
		{
			auto offset = 0.f;
			if (last_pressed & in_moveleft)
				offset += 90.f;
			if (last_pressed &in_moveright)
				offset -= 90.f;
			if (last_pressed & in_forward)
				offset *= 0.5f;
			else if (last_pressed &in_back)
				offset = -offset * 0.5f + 180.f;

			yaw += offset;

			//input->currentMoveData.flForwardMove = 0.f;
			//input->currentMoveData.flSideMove = 0.f;
			input->currentMoveData.flForwardMove = 0.f;
			input->currentMoveData.flSideMove = 0.f;

			//if (!cfg.misc.autostrafe->test(cfg_t::autostrafe_viewangles) && offset == 0.f)
			//	return;
			if (offset == 0.f)
				return;
		}

		if (input->currentMoveData.flSideMove != 0.0f || input->currentMoveData.flForwardMove != 0.0f)
			return;

		auto velocity_angle = M_RAD2DEG(std::atan2f(velocity.y, velocity.x));
		if (velocity_angle < 0.0f)
			velocity_angle += 360.0f;

		if (velocity_angle < 0.0f)
			velocity_angle += 360.0f;

		velocity_angle -= floorf(velocity_angle / 360.0f + 0.5f) * 360.0f;

		const auto speed = velocity.Length();
		const auto ideal = clamp(M_RAD2DEG(std::atan2(15.f, speed)), 0.f, 45.f);

		// const auto correct = (100.f - cfg.misc.autostrafe_smoothing.get()) * 0.02f * (ideal + ideal);
		const auto correct = (100.f - 20.f) * 0.02f * (ideal + ideal);

		input->currentMoveData.flForwardMove = 0.f;
		const auto velocity_delta = normalize_yaw(yaw - velocity_angle);


		auto rotate_movement = [input](CUserCmd* cmd, float target_yaw)
			{
				const float rot = M_RAD2DEG(input->GetViewAngles().y - target_yaw);

				const float new_forward = std::cos(rot) * input->currentMoveData.flForwardMove - std::sin(rot) * input->currentMoveData.flSideMove;
				const float new_side = std::sin(rot) * input->currentMoveData.flForwardMove + std::cos(rot) * input->currentMoveData.flSideMove;

				cmd->nButtons.nValue &= ~(in_back | in_forward | in_moveleft |in_moveright);
				cmd->nButtons.nValueChanged &= ~(in_back | in_forward | in_moveleft |in_moveright);

				input->currentMoveData.flForwardMove = clamp(new_forward, -1.f, 1.f);
				input->currentMoveData.flSideMove = clamp(new_side * -1.f, -1.f, 1.f);

				if (input->currentMoveData.flForwardMove > 0.f)
					cmd->nButtons.nValue |= in_forward;
				else if (input->currentMoveData.flForwardMove < 0.f)
					cmd->nButtons.nValue |= in_back;

				if (input->currentMoveData.flSideMove > 0.f)
					cmd->nButtons.nValue |= in_moveleft;
				else if (input->currentMoveData.flSideMove < 0.f)
					cmd->nButtons.nValue |= in_moveright;
			};

		if (fabsf(velocity_delta) > 170.f && speed > 80.f || velocity_delta > correct && speed > 80.f)
		{
			yaw = correct + velocity_angle;
			input->currentMoveData.flSideMove = -1.f;
			rotate_movement(cmd, normalize_yaw(yaw));
			return;
		}

		const bool side_switch = interfaces::input->nSequenceNumber % 2 == 0;

		if (-correct <= velocity_delta || speed <= 80.f)
		{
			if (side_switch)
			{
				yaw = yaw - ideal;
				input->currentMoveData.flSideMove = -1.f;

			}
			else
			{
				yaw = ideal + yaw;
				input->currentMoveData.flSideMove = 1.f;

			}
			rotate_movement(cmd, normalize_yaw(yaw));
		}
		else
		{
			yaw = velocity_angle - correct;
			input->currentMoveData.flSideMove = 1.f;

			rotate_movement(cmd, normalize_yaw(yaw));
		}
	}
}