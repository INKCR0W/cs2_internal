#include "win_api.hpp"

// used: runtime_error
#include <stdexcept>
// used: format
#include <format>
// used: get_module_base_handle
#include "../memory/memory.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"
// used: 
#include "../utils/fnv1a.hpp"

namespace windows_api{
	using memory::mem;

	const bool win_api::setup() {
		kernel32_dll = reinterpret_cast<HMODULE>(mem.get_module_base_handle(xorstr_("kernel32.dll")));
		user32_dll = reinterpret_cast<HMODULE>(mem.get_module_base_handle(xorstr_("user32.dll")));

		if (kernel32_dll == nullptr || user32_dll == nullptr)
			return false;
        
        //func_info = {
        //    {xorstr_("AllocConsole"), kernel32_dll},
        //    {xorstr_("CreateFileW"), kernel32_dll},
        //    {xorstr_("SetConsoleTitleW"), kernel32_dll},
        //    {xorstr_("CloseHandle"), kernel32_dll},
        //    {xorstr_("FreeConsole"), kernel32_dll},
        //    {xorstr_("GetConsoleWindow"), kernel32_dll},
        //    {xorstr_("PostMessageW"), user32_dll},
        //    {xorstr_("WriteFile"), kernel32_dll},
        //    {xorstr_("SetConsoleTextAttribute"), kernel32_dll},
        //    {xorstr_("WriteConsoleA"), kernel32_dll},
        //    {xorstr_("FreeLibrary"), kernel32_dll}
        //};

        //for (auto& [func_name, module_name] : func_info) {
        //    FARPROC funcPtr = GetProcAddress(module_name, func_name.c_str());

        //    if (!funcPtr) {
        //        throw std::runtime_error(xorstr_("Failed to get module handle : ") + func_name);
        //        return false;
        //    }

        //    func_ptr[func_name] = funcPtr;
        //}

        //inited = true;
        return true;
	}

    BOOL win_api::fn_AllocConsole() {
        uint32_t hash = function_hash::AllocConsole;

        if (func_ptr.find(hash) != func_ptr.end()) {
            return reinterpret_cast<FN_AllocConsole>(func_ptr[hash])();
        }
        else {
            MessageBox(NULL, std::string("kernal.dll: " + std::to_string(reinterpret_cast<uintptr_t>(kernel32_dll))).c_str(), "ERROR", MB_OK | MB_ICONERROR);
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("AllocConsole"));
            // void* funcPtr = GetProcAddress(kernel32_dll, "AllocConsole");

            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : AllocConsole"));
                return false;
            }

            func_ptr[hash] = funcPtr;

            return reinterpret_cast<FN_AllocConsole>(funcPtr)();
        }
    }
}