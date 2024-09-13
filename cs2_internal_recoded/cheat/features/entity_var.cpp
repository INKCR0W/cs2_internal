#include "entity_var.hpp"

// used: offsets
#include "../../offset/offsets.hpp"
// used: client_dll_addr
#include "../../memory/memory.hpp"


namespace features {
	void update_entitys()
	{
		using namespace memory;
		using namespace cs2_dumper;

		vars::local_player_controller = mem.read_memory<entity::CCSPlayerController*>(client_dll_addr + offsets::client_dll::dwLocalPlayerController);
		if (vars::local_player_controller == nullptr) {
			return;
		}

		vars::entity_list_address = mem.read_memory<uintptr_t>(client_dll_addr + offsets::client_dll::dwEntityList);
		if (vars::entity_list_address == NULL)
			return;

		vars::local_player_base_pawn = vars::local_player_controller->get_base_pawn(vars::entity_list_address);
		if (vars::local_player_base_pawn == nullptr) {
			return;
		}

		auto obs = vars::local_player_base_pawn->m_pObserverServices();

		if (obs == nullptr) {
			vars::observer = vars::local_player_base_pawn;
		}
		else {
			vars::observer = obs->get_target_pawn(vars::entity_list_address);
		}

		vars::view_matrix = mem.read_memory<ViewMatrix_t>(client_dll_addr + offsets::client_dll::dwViewMatrix);

		using vars::player_list;

		player_list.clear();

		for (uint32_t i = 0; i < 64; ++i) {
			auto entity_controller = reinterpret_cast<entity::CCSPlayerController*>(entity::get_from_handle(vars::entity_list_address, i));
			if (!entity_controller)
				continue;


			//if (entity_controller == vars::local_player_controller) {
			//	continue;
			//}

			vars::player_list.emplace_back(entity_controller);
		}
	}
}