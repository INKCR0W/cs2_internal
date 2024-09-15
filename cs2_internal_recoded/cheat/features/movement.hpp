#pragma once

#include "../../sdk/datatypes/usercmd.hpp"
#include "../../sdk/interfaces/csgoinput.hpp"

namespace features {
	void bhop(CUserCmd* cmd);
	void strafe(CCSGOInput* input);
}
