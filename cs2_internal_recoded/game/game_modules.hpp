#pragma once

#include "../utils/xorstr.hpp"

namespace modules {
	inline auto client_dll = xorstr(L"client.dll");
	inline auto engine2_dll = xorstr(L"engine2.dll");
	inline auto schemasystem_dll = xorstr(L"schemasystem.dll");
	inline auto inputsystem_dll = xorstr(L"inputsystem.dll");
	inline auto sdl3_dll = xorstr(L"SDL3.dll");
	inline auto tier0_dll = xorstr(L"tier0.dll");
	inline auto navsystem_dll = xorstr(L"navsystem.dll");
	inline auto rendersystem_dll = xorstr(L"rendersystemdx11.dll");
	inline auto localize_dll = xorstr(L"localize.dll");
	inline auto dbghelp_dll = xorstr(L"dbghelp.dll");
	inline auto gameoverlayrenderer_dll = xorstr(L"GameOverlayRenderer64.dll");
	inline auto particles_dll = xorstr(L"particles.dll");
	inline auto scenesystem_dll = xorstr(L"scenesystem.dll");
	inline auto material_system2_dll = xorstr(L"materialsystem2.dll");
	inline auto matchmaking_dll = xorstr(L"matchmaking.dll");
	inline auto resourcesystem_dll = xorstr(L"resourcesystem.dll");
}
