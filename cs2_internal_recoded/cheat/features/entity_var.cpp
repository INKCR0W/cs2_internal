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

		vars::view_matrix = mem.read_memory<ViewMatrix_t>(client_dll_addr + offsets::client_dll::dwViewMatrix);

		if (vars::entity_list_address == NULL)
			return;

		using vars::player_list;

		player_list.clear();

		for (uint32_t i = 0; i < 64; ++i) {
			auto entity_controller = reinterpret_cast<entity::CCSPlayerController*>(entity::get_from_handle(vars::entity_list_address, i));
			if (!entity_controller)
				continue;


			if (entity_controller == vars::local_player_controller) {
				continue;
			}

			vars::player_list.emplace_back(entity_controller);
		}
	}
}