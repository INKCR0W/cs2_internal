#pragma once


namespace cheat {
	class cs2_internal {
	public:
		cs2_internal();

		cs2_internal(const cs2_internal& copy) = delete;
		cs2_internal& operator=(const cs2_internal& other) = delete;

		// The setup function, run this before everything
		bool setup();
		// This one should run in a loop
		bool run();

	};
}