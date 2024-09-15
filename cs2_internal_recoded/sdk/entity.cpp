#include "entity.hpp"

#include "../core/interfaces.hpp"
#include "../sdk/interfaces/tracing.hpp"


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

	C_CSPlayerPawn* CCSPlayerController::get_pawn(std::uintptr_t entity_list) {
		return reinterpret_cast<C_CSPlayerPawn*>(get_from_handle(entity_list, m_hPlayerPawn()));
	}

	bool C_CSPlayerPawn::eye_visible(C_CSPlayerPawn* other) {
		Trace_Filter_t Filter = {};
		interfaces::trace->Init(Filter, this, 0x1C3003, 4, 7);
		Game_Trace_t Trace = {};
		Ray_t Ray = {};
		Vector_t LocalEye = this->get_eye_position();
		Vector_t EnemyEye = other->get_eye_position();
		interfaces::trace->TraceShape(Ray, &LocalEye, &EnemyEye, Filter, Trace);
		return Trace.HitEntity && Trace.HitEntity->m_pEntity()->get_entry_index() == other->m_pEntity()->get_entry_index() || Trace.Fraction > 0.97f;
	}
}

