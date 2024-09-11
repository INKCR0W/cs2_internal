#include "entity.hpp"

#include "../core/interfaces.hpp"


namespace entity {
	CCSPlayerController* C_BasePlayerPawn::get_controller(std::uintptr_t entity_list) {
		return reinterpret_cast<CCSPlayerController*>(get_from_handle(entity_list, m_hController()));
	}

	C_BaseEntity* CPlayer_ObserverServices::get_target_pawn(std::uintptr_t entity_list) {
		return reinterpret_cast<C_BaseEntity*>(get_from_handle(entity_list, m_hObserverTarget()));
	}

	C_BasePlayerPawn* CCSPlayerController::get_base_pawn(std::uintptr_t entity_list) {
		return reinterpret_cast<C_BasePlayerPawn*>(get_from_handle(entity_list, m_hPawn()));
	}
}

