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
// used: hash_ct
#include "../utils/fnv1a.hpp"

namespace windows_api{

	using hash_t = uint32_t;

	enum function_hash : hash_t {
		AllocConsole =				FNV1A("AllocConsole"),
		CreateFileW =				FNV1A("CreateFileW"),
		CreateFileA =				FNV1A("CreateFileA"),
		SetConsoleTitleW =			FNV1A("SetConsoleTitleW"),
		CloseHandle =				FNV1A("CloseHandle"),
		FreeConsole =				FNV1A("FreeConsole"),
		GetConsoleWindow =			FNV1A("GetConsoleWindow"),
		PostMessageW =				FNV1A("PostMessageW"),
		WriteFile =					FNV1A("WriteFile"),
		SetConsoleTextAttribute =	FNV1A("SetConsoleTextAttribute"),
		WriteConsoleA =				FNV1A("WriteConsoleA"),
		DisableThreadLibraryCalls =	FNV1A("DisableThreadLibraryCalls"),
		CreateThread =				FNV1A("CreateThread"),
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

		HMODULE kernel32_dll = nullptr;
		HMODULE user32_dll = nullptr;

		std::unordered_map<uint32_t, void*> func_ptr = {};
	};

    inline win_api winapi;
}
