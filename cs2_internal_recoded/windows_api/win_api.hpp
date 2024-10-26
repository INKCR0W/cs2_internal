#pragma once

// used: LoadLibraryA
#include <Windows.h>

// used: string
#include <string>
// used: unordered_map
#include <unordered_map>
// used: function
#include <functional>
// used: runtime_error
#include <stdexcept>
// used: HINTERNET
#include <wininet.h>
// used: hash_ct
#include "../utils/fnv1a.hpp"

namespace windows_api{

	using hash_t = uint32_t;

	enum function_hash : hash_t {
		hAllocConsole =				FNV1A("AllocConsole"),
		hCreateFileW =				FNV1A("CreateFileW"),
		hCreateFileA =				FNV1A("CreateFileA"),
		hSetConsoleTitleW =			FNV1A("SetConsoleTitleW"),
		hCloseHandle =				FNV1A("CloseHandle"),
		hFreeConsole =				FNV1A("FreeConsole"),
		hGetConsoleWindow =			FNV1A("GetConsoleWindow"),
		hPostMessageW =				FNV1A("PostMessageW"),
		hWriteFile =					FNV1A("WriteFile"),
		hSetConsoleTextAttribute =	FNV1A("SetConsoleTextAttribute"),
		hWriteConsoleA =				FNV1A("WriteConsoleA"),
		hDisableThreadLibraryCalls =	FNV1A("DisableThreadLibraryCalls"),
		hCreateThread =				FNV1A("CreateThread"),
		hReadProcessMemory =			FNV1A("ReadProcessMemory"),
		hWriteProcessMemory =		FNV1A("WriteProcessMemory"),
		hCallWindowProcW =			FNV1A("CallWindowProcW"),
		hGetProcessHeap =			FNV1A("GetProcessHeap"),
		hHeapAlloc =					FNV1A("HeapAlloc"),
		hHeapFree =					FNV1A("HeapFree"),
		hSetWindowLongPtrW =			FNV1A("SetWindowLongPtrW"),
		hShellExecuteA =				FNV1A("ShellExecuteA"),
		hMessageBoxA =				FNV1A("MessageBoxA"),
		hInternetOpenA =				FNV1A("InternetOpenA"),
		hInternetOpenUrlA =			FNV1A("InternetOpenUrlA"),
		hInternetReadFile =			FNV1A("InternetReadFile"),
		hInternetCloseHandle =		FNV1A("InternetCloseHandle"),
	};


	class win_api {
	public:
		win_api() = default;
		~win_api() = default;

		const bool setup();

		BOOL fn_AllocConsole();
		HANDLE fn_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
		HANDLE fn_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
		BOOL fn_SetConsoleTitleW(LPCWSTR lpConsoleTitle);
		BOOL fn_CloseHandle(HANDLE hObject);
		BOOL fn_FreeConsole();
		HWND fn_GetConsoleWindow();
		BOOL fn_PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		BOOL fn_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
		BOOL fn_SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);
		BOOL fn_WriteConsoleA(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved);
		BOOL fn_DisableThreadLibraryCalls(HMODULE hModule);
		HANDLE fn_CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
		BOOL fn_ReadProcessMemory(HANDLE  hProcess, LPCVOID lpBaseAddress, LPVOID  lpBuffer, SIZE_T  nSize, SIZE_T* lpNumberOfBytesRead);
		BOOL fn_WriteProcessMemory(HANDLE  hProcess, LPVOID lpBaseAddress, LPCVOID  lpBuffer, SIZE_T  nSize, SIZE_T* lpNumberOfBytesRead);
		LRESULT fn_CallWindowProcW(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		HANDLE fn_GetProcessHeap();
		LPVOID fn_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
		BOOL fn_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
		LONG_PTR fn_SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
		HINSTANCE fn_ShellExecuteA(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
		int fn_MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
		HINTERNET fn_InternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);
		HINTERNET fn_InternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
		BOOL fn_InternetReadFile(void* hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
		BOOL fn_InternetCloseHandle(void* hInternet);
		 

	private:
		using FN_AllocConsole = BOOL(WINAPI*)();
		using FN_CreateFileW = HANDLE(WINAPI*)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
		using FN_CreateFileA = HANDLE(WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
		using FN_SetConsoleTitleW = BOOL(WINAPI*)(LPCWSTR);
		using FN_CloseHandle = BOOL(WINAPI*)(HANDLE);
		using FN_FreeConsole = BOOL(WINAPI*)();
		using FN_GetConsoleWindow = HWND(WINAPI*)();
		using FN_PostMessageW = BOOL(WINAPI*)(HWND, UINT, WPARAM, LPARAM);
		using FN_WriteFile = BOOL(WINAPI*)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
		using FN_SetConsoleTextAttribute = BOOL(WINAPI*)(HANDLE, WORD);
		using FN_WriteConsoleA = BOOL(WINAPI*)(HANDLE, const VOID*, DWORD, LPDWORD, LPVOID);
		using FN_DisableThreadLibraryCalls = BOOL(WINAPI*)(HMODULE);
		using FN_CreateThread = HANDLE(WINAPI*)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
		using FN_ReadProcessMemory = BOOL(WINAPI*)(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T*);
		using FN_WriteProcessMemory = BOOL(WINAPI*)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
		using FN_CallWindowProcW = LRESULT(WINAPI*) (WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		using FN_GetProcessHeap = HANDLE(WINAPI*) ();
		using FN_HeapAlloc = LPVOID(WINAPI*) (HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
		using FN_HeapFree = BOOL(WINAPI*) (HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
		using FN_SetWindowLongPtrW = LONG_PTR(WINAPI*)(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
		using FN_ShellExecuteA = HINSTANCE(WINAPI*)(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
		using FN_MessageBoxA = int(WINAPI*)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
		using FN_InternetOpenA = HINTERNET(WINAPI*)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);
		using FN_InternetOpenUrlA = HINTERNET(WINAPI*)(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
		using FN_InternetReadFile = BOOL(WINAPI*)(void* hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
		using FN_InternetCloseHandle = BOOL(WINAPI*)(void* hInternet);

		HMODULE kernel32_dll = nullptr;
		HMODULE user32_dll = nullptr;
		HMODULE shell32_dll = nullptr;
		HMODULE wininet_dll = nullptr;

		std::unordered_map<uint32_t, void*> func_ptr = {};
	};

    inline win_api winapi;
}

