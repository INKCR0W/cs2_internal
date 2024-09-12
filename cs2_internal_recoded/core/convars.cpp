#include "convars.hpp"

// used: convar interface
#include "interfaces.hpp"

#include "../sdk/interfaces/ienginecvar.hpp"

bool convar::setup()
{
	bool bSuccess = true;

	m_pitch = interfaces::cvar->find(xorstr_("m_pitch"));
	bSuccess &= m_pitch != nullptr;

	m_yaw = interfaces::cvar->find(xorstr_("m_yaw"));
	bSuccess &= m_yaw != nullptr;

	sensitivity = interfaces::cvar->find(xorstr_("sensitivity"));
	bSuccess &= sensitivity != nullptr;

	game_type = interfaces::cvar->find(xorstr_("game_type"));
	bSuccess &= game_type != nullptr;

	game_mode = interfaces::cvar->find(xorstr_("game_mode"));
	bSuccess &= game_mode != nullptr;

	mp_teammates_are_enemies = interfaces::cvar->find(xorstr_("mp_teammates_are_enemies"));
	bSuccess &= mp_teammates_are_enemies != nullptr;

	sv_autobunnyhopping = interfaces::cvar->find(xorstr_("sv_autobunnyhopping"));
	bSuccess &= sv_autobunnyhopping != nullptr;

	return bSuccess;
}
