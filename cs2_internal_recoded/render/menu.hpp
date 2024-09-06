#pragma once


namespace menu {
	class menu_class {
	public:
		menu_class() = default;
		~menu_class() = default;

		void run();

		float screen_width = 0.f;
		float screen_height = 0.f;

		bool menu_opened = false;
		bool main_active = false;
	private:
	};


	inline menu_class menu;
}
