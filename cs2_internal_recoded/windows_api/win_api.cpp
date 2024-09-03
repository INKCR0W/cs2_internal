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
        hash_t hash = function_hash::AllocConsole;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("AllocConsole"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : AllocConsole"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }

        return reinterpret_cast<FN_AllocConsole>(func_ptr[hash])();
    }

    HANDLE win_api::fn_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
        hash_t hash = function_hash::CreateFileW;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("CreateFileW"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : CreateFileW"));
                return HANDLE();
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_CreateFileW>(func_ptr[hash])(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    HANDLE win_api::fn_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
        hash_t hash = function_hash::CreateFileA;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("CreateFileA"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : CreateFileA"));
                return HANDLE();
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_CreateFileA>(func_ptr[hash])(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    BOOL win_api::fn_SetConsoleTitleW(LPCWSTR lpConsoleTitle) {
        hash_t hash = function_hash::SetConsoleTitleW;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("SetConsoleTitleW"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : SetConsoleTitleW"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_SetConsoleTitleW>(func_ptr[hash])(lpConsoleTitle);
    }

    BOOL win_api::fn_CloseHandle(HANDLE hObject) {
        hash_t hash = function_hash::CloseHandle;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("CloseHandle"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : CloseHandle"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_CloseHandle>(func_ptr[hash])(hObject);
    }

    BOOL win_api::fn_FreeConsole() {
        hash_t hash = function_hash::FreeConsole;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("FreeConsole"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : FreeConsole"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_FreeConsole>(func_ptr[hash])();
    }

    HWND win_api::fn_GetConsoleWindow() {
        hash_t hash = function_hash::GetConsoleWindow;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("GetConsoleWindow"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : GetConsoleWindow"));
                return HWND();
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_GetConsoleWindow>(func_ptr[hash])();
    }

    BOOL win_api::fn_PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
        hash_t hash = function_hash::PostMessageW;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(user32_dll, xorstr_("PostMessageW"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : PostMessageW"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_PostMessageW>(func_ptr[hash])(hWnd, Msg, wParam, lParam);
    }

    BOOL win_api::fn_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
        hash_t hash = function_hash::WriteFile;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("WriteFile"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : WriteFile"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_WriteFile>(func_ptr[hash])(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    }

    BOOL win_api::fn_SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes) {
        hash_t hash = function_hash::SetConsoleTextAttribute;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("SetConsoleTextAttribute"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : SetConsoleTextAttribute"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_SetConsoleTextAttribute>(func_ptr[hash])(hConsoleOutput, wAttributes);
    }

    BOOL win_api::fn_WriteConsoleA(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved) {
        hash_t hash = function_hash::WriteConsoleA;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("WriteConsoleA"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : WriteConsoleA"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_WriteConsoleA>(func_ptr[hash])(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);
    }

    BOOL win_api::fn_DisableThreadLibraryCalls(HMODULE hModule) {
        hash_t hash = function_hash::DisableThreadLibraryCalls;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("DisableThreadLibraryCalls"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : DisableThreadLibraryCalls"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_DisableThreadLibraryCalls>(func_ptr[hash])(hModule);
    }

    HANDLE win_api::fn_CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) {
        hash_t hash = function_hash::CreateThread;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("CreateThread"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : CreateThread"));
                return HANDLE();
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_CreateThread>(func_ptr[hash])(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
    }
}