#pragma once

#include <d3d11.h>

// used: xorstr_
#include "../utils/xorstr.hpp"
//// used: IInputSystem
//#include "../sdk/interfaces/iinputsystem.hpp"
// used: IEngineClient
#include "../sdk/interfaces/iengineclient.hpp"
//// used: ISwapChainDx11
//#include "../sdk/interfaces/iswapchaindx11.hpp"
//// used: IMemAlloc
//#include "../sdk/interfaces/imemalloc.hpp"
//// used: IEngineCVar
//#include "../sdk/interfaces/ienginecvar.hpp"

#pragma region sdk_definitons
#define GAME_RESOURCE_SERVICE_CLIENT xorstr_("GameResourceServiceClientV00")
#define SOURCE2_CLIENT xorstr_("Source2Client00")
#define SCHEMA_SYSTEM xorstr_("SchemaSystem_00")
#define INPUT_SYSTEM_VERSION xorstr_("InputSystemVersion00")
#define SOURCE2_ENGINE_TO_CLIENT xorstr_("Source2EngineToClient00")
#define ENGINE_CVAR xorstr_("VEngineCvar00")
#define LOCALIZE xorstr_("Localize_00")
#define NETWORK_CLIENT_SERVICE xorstr_("NetworkClientService_00")
#define MATERIAL_SYSTEM2 xorstr_("VMaterialSystem2_00")
#define RESOURCE_SYSTEM xorstr_("ResourceSystem013")
#define RESOURCE_HANDLE_UTILS xorstr_("ResourceHandleUtils001")

// @source: master/game/shared/shareddefs.h
#define TICK_INTERVAL 0.015625F
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5F + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))
#define ROUND_TO_TICKS(TIME) (TICK_INTERVAL * TIME_TO_TICKS(TIME))
#define TICK_NEVER_THINK (-1)
#pragma endregion


class IMemAlloc;
class ISwapChainDx11;

class IInputSystem;
class IEngineClient;
class IEngineCVar;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace interfaces {
	const bool setup();

	void create_render_target();

	inline IMemAlloc* mem_alloc = nullptr;
	inline ISwapChainDx11* swap_chain = nullptr;

	inline ID3D11Device* device = nullptr;
	inline ID3D11DeviceContext* device_context = nullptr;
	inline ID3D11RenderTargetView* render_target_view = nullptr;

	inline IInputSystem* input_system = nullptr;
	inline IEngineClient* engine = nullptr;
	inline IEngineCVar* cvar = nullptr;
};

