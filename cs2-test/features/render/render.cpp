#include "../cheat.hpp"

namespace cheat {
	void cs2_internal::render(IDXGISwapChain* _this) {
		update_entity();
		menu(_this);

		if (esp_on && local_player_pawn)
			esp();
	}
}