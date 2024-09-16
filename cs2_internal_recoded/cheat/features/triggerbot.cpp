#include "triggerbot.hpp"

#include "entity_var.hpp"

namespace features {
	void trigger(CUserCmd* cmd) {
		if (cmd == nullptr)
			return;

		if (vars::local_player_pawn->m_pClippingWeapon() == nullptr)
			return;

		if (vars::canhit && vars::local_player_pawn->m_vecAbsVelocity().Length() < 10.f) {
			if (cmd->csgoUserCmd.pBaseCmd->nClientTick >= vars::local_player_pawn->m_pClippingWeapon()->m_nNextPrimaryAttackTick()) {
				cmd->nButtons.nValue |= IN_ATTACK;
				cmd->csgoUserCmd.pBaseCmd->pInButtonState->SetBits(BASE_BITS_BUTTONPB);
			}
			else {
				cmd->nButtons.nValue &= ~IN_ATTACK;
				cmd->csgoUserCmd.pBaseCmd->pInButtonState->SetBits(BASE_BITS_BUTTONPB);
			}
		}
	}
}