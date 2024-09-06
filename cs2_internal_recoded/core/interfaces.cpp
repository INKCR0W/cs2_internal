#include "interfaces.hpp"

#include <d3d11.h>

#include "common.hpp"

// used: get_export_address
#include "../memory/memory.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"
// used: log system
#include "../log/log.hpp"
// used: crt
#include "../utils/crt_string.hpp"
// used: game modules
#include "../game/game_modules.hpp"
// used: hWindow
#include "inputsystem.hpp"
// used: screen_width screen_height
#include "../render/menu.hpp"

using InstantiateInterfaceFn_t = void* (*)();

class CInterfaceRegister
{
public:
	InstantiateInterfaceFn_t fnCreate;
	const char* szName;
	CInterfaceRegister* pNext;
};

static const CInterfaceRegister* get_register_list(const char* wszModuleName)
{
	using namespace log_system;

	void* hModule = memory::mem.get_module_base_handle(wszModuleName);
	if (hModule == nullptr)
		return nullptr;

	std::uint8_t* pCreateInterface = reinterpret_cast<std::uint8_t*>(memory::mem.get_export_address(hModule, xorstr_("CreateInterface")));

	if (pCreateInterface == nullptr)
	{
		logger << set_level(log_level_flags::LOG_ERROR) << xorstr_("failed to get \"CreateInterface\" address") << set_level() << endl;
		return nullptr;
	}

	return *reinterpret_cast<CInterfaceRegister**>(memory::mem.resolve_relative_address(pCreateInterface, 0x3, 0x7));
}


template <typename T = void*>
T* capture(const CInterfaceRegister* pModuleRegister, const char* szInterfaceName)
{
	using crt::crt;
	for (const CInterfaceRegister* pRegister = pModuleRegister; pRegister != nullptr; pRegister = pRegister->pNext)
	{
		if (const std::size_t nInterfaceNameLength = crt.str_length(szInterfaceName);
			// found needed interface
			crt.string_compare_n(szInterfaceName, pRegister->szName, nInterfaceNameLength) == 0 &&
			// and we've given full name with hardcoded digits
			(crt.str_length(pRegister->szName) == nInterfaceNameLength ||
				// or it contains digits after name
				crt.string_to_integer<int>(pRegister->szName + nInterfaceNameLength, nullptr, 10) > 0))
		{
			// capture our interface
			void* pInterface = pRegister->fnCreate();

			return static_cast<T*>(pInterface);
		}
	}

	return nullptr;
}

void interfaces::create_render_target() {
	using namespace log_system;

	// @note: i dont use this anywhere else so lambda is fine
	static const auto GetCorrectDXGIFormat = [](DXGI_FORMAT eCurrentFormat)
		{
			switch (eCurrentFormat)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			return eCurrentFormat;
		};

	DXGI_SWAP_CHAIN_DESC sd;
	swap_chain->pDXGISwapChain->GetDesc(&sd);

	inputsystem::hWindow = sd.OutputWindow;
	menu::menu.screen_width = static_cast<float>(sd.BufferDesc.Width);
	menu::menu.screen_height = static_cast<float>(sd.BufferDesc.Height);

	ID3D11Texture2D* pBackBuffer = nullptr;
	if (SUCCEEDED(swap_chain->pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
	{
		if (pBackBuffer)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc{};
			desc.Format = static_cast<DXGI_FORMAT>(GetCorrectDXGIFormat(sd.BufferDesc.Format));
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			if (FAILED(device->CreateRenderTargetView(pBackBuffer, &desc, &render_target_view)))
			{
				logger << set_level(log_level_flags::LOG_WARNING) << xorstr_("failed to create render target view with D3D11_RTV_DIMENSION_TEXTURE2D...") << set_level() << endl;;
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
				if (FAILED(device->CreateRenderTargetView(pBackBuffer, &desc, &render_target_view)))
				{
					logger << set_level(log_level_flags::LOG_WARNING) << xorstr_("Failed to create render target view with D3D11_RTV_DIMENSION_TEXTURE2D...") << set_level() << endl;;
					logger << set_level(log_level_flags::LOG_WARNING) << xorstr_("Retrying...") << set_level() << endl;
					if (FAILED(device->CreateRenderTargetView(pBackBuffer, NULL, &render_target_view)))
					{
						logger << set_level(log_level_flags::LOG_ERROR) << xorstr_("Failed to create render target view") << set_level() << endl;
						ASSERT(false);
					}
				}
			}
			pBackBuffer->Release();
			pBackBuffer = nullptr;
		}
	}
}

const bool interfaces::setup() {
	using namespace log_system;
	bool success = true;

	swap_chain = **reinterpret_cast<ISwapChainDx11***>(memory::mem.resolve_relative_address(reinterpret_cast<uint8_t*>(memory::mem.find_pattern(modules::rendersystem_dll, xorstr_("66 0F 7F 0D ? ? ? ? 66 0F 7F 05 ? ? ? ? 0F 1F 40"))), 0x4, 0x8));
	success &= (swap_chain != nullptr);

#ifdef _DEBUG
	logger << set_level(log_level_flags::LOG_INFO) << xorstr_("SwapChain found: ") << reinterpret_cast<uintptr_t>(&swap_chain) << set_level() << endl;
#endif

	if (swap_chain != nullptr)
	{
		if (swap_chain->pDXGISwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device) < 0)
		{
			logger << set_level(log_level_flags::LOG_ERROR) << "Failed to get device from swapchain" << set_level() << endl;
			return false;
		}
		else
			device->GetImmediateContext(&device_context);
	}
	success &= (device != nullptr && device_context != nullptr);

	const auto pInputSystemRegisterList = get_register_list(modules::inputsystem_dll);
	if (pInputSystemRegisterList == nullptr)
		return false;

	input_system = capture<IInputSystem>(pInputSystemRegisterList, INPUT_SYSTEM_VERSION);
	success = (input_system != nullptr);

#ifdef _DEBUG
	logger << set_level(log_level_flags::LOG_INFO) << xorstr_("InputSystem found: ") << reinterpret_cast<uintptr_t>(&input_system) << set_level() << endl;
#endif

	create_render_target();

	return success;
}