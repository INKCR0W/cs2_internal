#pragma once

#include "../utils/xorstr.hpp"

namespace modules {
	inline auto kernel32_dll = xorstr("kernel32.dll");
	inline auto user32_dll = xorstr("user32.dll");
	inline auto client_dll = xorstr("client.dll");
	inline auto engine2_dll = xorstr("engine2.dll");
	inline auto schemasystem_dll = xorstr("schemasystem.dll");
	inline auto inputsystem_dll = xorstr("inputsystem.dll");
	inline auto sdl3_dll = xorstr("SDL3.dll");
	inline auto tier0_dll = xorstr("tier0.dll");
	inline auto navsystem_dll = xorstr("navsystem.dll");
	inline auto rendersystem_dll = xorstr("rendersystemdx11.dll");
	inline auto localize_dll = xorstr("localize.dll");
	inline auto dbghelp_dll = xorstr("dbghelp.dll");
	inline auto gameoverlayrenderer_dll = xorstr("GameOverlayRenderer64.dll");
	inline auto particles_dll = xorstr("particles.dll");
	inline auto scenesystem_dll = xorstr("scenesystem.dll");
	inline auto material_system2_dll = xorstr("materialsystem2.dll");
	inline auto matchmaking_dll = xorstr("matchmaking.dll");
	inline auto resourcesystem_dll = xorstr("resourcesystem.dll");
	inline auto shell32_dll = xorstr("Shell32.dll");
	inline auto wininet_dll = xorstr("wininet.dll");
}
