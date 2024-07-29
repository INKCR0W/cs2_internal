#include "../cheat.hpp"

namespace cheat {
	void cs2_internal::render() {
		update_entity();
		menu();

		if (esp_on)
			esp();
	}
}