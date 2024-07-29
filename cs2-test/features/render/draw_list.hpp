#pragma once

#include "../../third-party/imgui/imgui.h"

namespace cheat {
	class command {
		ImVec2 start;
		ImVec2 end;
		ImU32 color;
	};
}