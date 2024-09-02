#pragma once

namespace memory {
	class memory_class {
	public:
		void* get_module_base_handle(const char* module_name);
	};

	inline memory_class mem;
}
