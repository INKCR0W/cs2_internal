#pragma once

#include "../../core/inputsystem.hpp"
#include "../../sdk/datatypes/matrix.hpp"
#include "../../sdk/datatypes/vector.hpp"
#include "../../sdk/interfaces/csgoinput.hpp"

namespace features {
	void rcs();
	void silent_aim(CUserCmd* pCmd);
}