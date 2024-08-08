#include "../cheat.hpp"

#include "../../offset/offset.hpp"

namespace cheat{
	using namespace cs2_dumper;

	uintptr_t cs2_internal::get_from_handle(uintptr_t entityList, uintptr_t handle) {
		uintptr_t listEntrySecond = read_memory<uintptr_t>(entityList + 0x8ULL * ((handle & 0x7FFF) >> 9) + 0x10);
		return listEntrySecond == 0
			? 0
			: read_memory<uintptr_t>(listEntrySecond + 0x78ULL * (handle & 0x1FF));
	}

	bool cs2_internal::update_entity() {
		const uintptr_t game_rules = read_memory<uintptr_t>(client_dll_addr + offsets::client_dll::dwGameRules);

		//if (is_in_match = read_memory<bool>(game_rules + schemas::client_dll::C_CSGameRules::m_bHasMatchStarted); is_in_match == false)
		//	return false;

		local_player_controller = read_memory<uintptr_t>(client_dll_addr + offsets::client_dll::dwLocalPlayerController);

		const auto entity_list_address = read_memory<uintptr_t>(client_dll_addr + offsets::client_dll::dwEntityList);

		if (entity_list_address == NULL)
			return false;

		local_player_pawn_handle = read_memory<uintptr_t>(local_player_controller + schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

		if (local_player_pawn = get_from_handle(entity_list_address, local_player_pawn_handle); local_player_pawn == NULL)
			return false;

		player_entity_list.clear();
		spectators_list.clear();

		for (uint32_t i = 0; i < 64; ++i) {
			const uintptr_t entity_address = read_memory<uintptr_t>(entity_list_address + (0x8ULL * (i & 0x7FFF) >> 9) + 0x10);
			if (!entity_address)
				continue;

			const uintptr_t entity_controller = read_memory<uintptr_t>(entity_address + 0x78ULL * (i & 0x1FF));
			if (!entity_controller)
				continue;

			if (entity_controller == local_player_controller) {
				continue;
			}

			//if (!read_memory<bool>(entity_controller + schemas::client_dll::CCSPlayerController::m_bPawnIsAlive))
			//	continue;

			const uint32_t C_CSPlayerPawn = read_memory<std::uint32_t>(entity_controller + schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
			const uintptr_t entity_pawn = get_from_handle(entity_list_address, C_CSPlayerPawn);

			//if (read_memory<int>(entity_pawn + schemas::client_dll::C_BaseEntity::m_iHealth) < 0)
			//	continue;

			player_entity_list.push_back({ entity_controller, entity_pawn, C_CSPlayerPawn });

			//if (spectator_list) {
			//	uint32_t spectatorPawn = read_memory<uint32_t>(entity_controller + schemas::client_dll::CBasePlayerController::m_hPawn);
			//	uintptr_t pawn = get_from_handle(entity_list_address, spectatorPawn);

			//	uintptr_t obs = read_memory<uintptr_t>(pawn + schemas::client_dll::C_BasePlayerPawn::m_pObserverServices);
			//	uint64_t oTarget = read_memory<uint64_t>(obs + schemas::client_dll::CPlayer_ObserverServices::m_hObserverTarget);
			//	uintptr_t target_pawn = get_from_handle(entity_list_address, oTarget);


			//	if (obs && (target_pawn == my_target)) {
			//		//std::string pawn_name;
			//		//uintptr_t name_address = read_memory<uintptr_t>(entity_controller + schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName);
			//		//if (name_address) {
			//		//	char buf[MAX_PATH] = {};
			//		//	read_memory_size(name_address, buf, MAX_PATH);
			//		//	pawn_name = std::string(buf);
			//		//}
			//		//else {
			//		//	pawn_name = "Unknown";
			//		//}
			//		spectators_list.push_back(entity_controller);
			//	}
			//}
		}

		return true;
	}
}