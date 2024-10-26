#include "win_api.hpp"

// used: runtime_error
#include <stdexcept>
// used: format
#include <format>
// used: get_module_base_handle
#include "../memory/memory.hpp"
// used: xorstr_
#include "../utils/xorstr.hpp"
// used: modules
#include "../game/game_modules.hpp"

#include <string>

namespace windows_api{
	using memory::mem;

	const bool win_api::setup() {
		kernel32_dll = reinterpret_cast<HMODULE>(mem.get_module_base_handle(modules::kernel32_dll));
		user32_dll = reinterpret_cast<HMODULE>(mem.get_module_base_handle(modules::user32_dll));
        shell32_dll = reinterpret_cast<HMODULE>(mem.get_module_base_handle(modules::shell32_dll));
        wininet_dll = reinterpret_cast<HMODULE>(mem.get_module_base_handle(modules::wininet_dll));

		if (kernel32_dll == nullptr || user32_dll == nullptr || shell32_dll == nullptr || wininet_dll == nullptr)
			return false;
        
        return true;
	}

    BOOL win_api::fn_AllocConsole() {
        hash_t hash = function_hash::hAllocConsole;
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
        hash_t hash = function_hash::hCreateFileW;
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
        hash_t hash = function_hash::hCreateFileA;
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
        hash_t hash = function_hash::hSetConsoleTitleW;
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
        hash_t hash = function_hash::hCloseHandle;
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
        hash_t hash = function_hash::hFreeConsole;
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
        hash_t hash = function_hash::hGetConsoleWindow;
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
        hash_t hash = function_hash::hPostMessageW;
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
        hash_t hash = function_hash::hWriteFile;
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
        hash_t hash = function_hash::hSetConsoleTextAttribute;
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
        hash_t hash = function_hash::hWriteConsoleA;
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
        hash_t hash = function_hash::hDisableThreadLibraryCalls;
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
        hash_t hash = function_hash::hCreateThread;
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

    BOOL win_api::fn_ReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesRead) {
        hash_t hash = function_hash::hReadProcessMemory;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("ReadProcessMemory"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : ReadProcessMemory"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_ReadProcessMemory>(func_ptr[hash])(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
    }

    BOOL win_api::fn_WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten) {
        hash_t hash = function_hash::hWriteProcessMemory;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("WriteProcessMemory"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : WriteProcessMemory"));
                return false;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_WriteProcessMemory>(func_ptr[hash])(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
    }

    LRESULT win_api::fn_CallWindowProcW(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
        hash_t hash = function_hash::hCallWindowProcW;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(user32_dll, xorstr_("CallWindowProcW"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : CallWindowProcW"));
                return 0;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_CallWindowProcW>(func_ptr[hash])(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
    }

    HANDLE win_api::fn_GetProcessHeap() {
        hash_t hash = function_hash::hGetProcessHeap;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("GetProcessHeap"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : GetProcessHeap"));
                return NULL;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_GetProcessHeap>(func_ptr[hash])();
    }

    LPVOID win_api::fn_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) {
        hash_t hash = function_hash::hHeapAlloc;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("HeapAlloc"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : HeapAlloc"));
                return NULL;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_HeapAlloc>(func_ptr[hash])(hHeap, dwFlags, dwBytes);
    }

    BOOL win_api::fn_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) {
        hash_t hash = function_hash::hHeapFree;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(kernel32_dll, xorstr_("HeapFree"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : HeapFree"));
                return FALSE;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_HeapFree>(func_ptr[hash])(hHeap, dwFlags, lpMem);
    }


    LONG_PTR win_api::fn_SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong) {
        hash_t hash = function_hash::hSetWindowLongPtrW;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(user32_dll, xorstr_("SetWindowLongPtrW"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : SetWindowLongPtrW"));
                return 0;
            }
            func_ptr[hash] = funcPtr;
        }

        return reinterpret_cast<FN_SetWindowLongPtrW>(func_ptr[hash])(hWnd, nIndex, dwNewLong);
    }

    HINSTANCE win_api::fn_ShellExecuteA(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd)
    {
        hash_t hash = function_hash::hShellExecuteA;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(shell32_dll, xorstr_("ShellExecuteA"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : ShellExecuteA"));
                return 0;
            }
            func_ptr[hash] = funcPtr;
        }

        return reinterpret_cast<FN_ShellExecuteA>(func_ptr[hash])(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
    }

    int win_api::fn_MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
    {
        hash_t hash = function_hash::hMessageBoxA;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(user32_dll, xorstr_("MessageBoxA"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : MessageBoxA"));
                return 0;
            }
            func_ptr[hash] = funcPtr;
        }

        return reinterpret_cast<FN_MessageBoxA>(func_ptr[hash])(hWnd, lpText, lpCaption, uType);
    }

    HINTERNET win_api::fn_InternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags)
    {
        hash_t hash = function_hash::hInternetOpenA;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(wininet_dll, xorstr_("InternetOpenA"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : InternetOpenA"));
                return NULL;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_InternetOpenA>(func_ptr[hash])(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
    }

    HINTERNET win_api::fn_InternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
    {
        hash_t hash = function_hash::hInternetOpenUrlA;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(wininet_dll, xorstr_("InternetOpenUrlA"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : InternetOpenUrlA"));
                return NULL;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_InternetOpenUrlA>(func_ptr[hash])(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
    }

    BOOL win_api::fn_InternetReadFile(void* hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead)
    {
        hash_t hash = function_hash::hInternetReadFile;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(wininet_dll, xorstr_("InternetReadFile"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : InternetReadFile"));
                return FALSE;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_InternetReadFile>(func_ptr[hash])(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
    }

    BOOL win_api::fn_InternetCloseHandle(void* hInternet)
    {
        hash_t hash = function_hash::hInternetCloseHandle;
        if (func_ptr.find(hash) == func_ptr.end()) {
            void* funcPtr = GetProcAddress(wininet_dll, xorstr_("InternetCloseHandle"));
            if (!funcPtr) {
                throw std::runtime_error(xorstr_("Failed to get module handle : InternetCloseHandle"));
                return FALSE;
            }
            func_ptr[hash] = funcPtr;
        }
        return reinterpret_cast<FN_InternetCloseHandle>(func_ptr[hash])(hInternet);
    }
}