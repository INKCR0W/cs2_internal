// hook manager
// @credits: https://github.com/maecry/asphyxia-cs2/blob/master/cstrike/utilities/detourhook.h

#pragma once
// used: [ext] minhook
// @credits: https://github.com/TsudaKageyu/minhook
#include "../extensions/minhook/include/MinHook.h"
#pragma comment(lib, "../extensions/minhook/lib/libMinHook.x64.lib")

template <typename T>
class hook_object
{
public:
	/// <summary>
    /// setup hook and replace function
    /// </summary>
	/// <returns>
	/// true if hook has been successfully created, false otherwise
	/// </returns>
	bool Create(void* pFunction, void* pDetour)
	{
		if (pFunction == nullptr || pDetour == nullptr)
			return false;

		pBaseFn = pFunction;
		pReplaceFn = pDetour;

		if (const MH_STATUS status = MH_CreateHook(pBaseFn, pReplaceFn, &pOriginalFn); status != MH_OK)
		{
			// @todo: debugging assert
			return false;
		}

		if (!Replace())
			return false;

		return true;
	}

	/// <summary>
    /// patch memory to jump to our function instead of original
    /// </summary>
	/// <returns>
	/// true if hook has been successfully applied, false otherwise
	/// </returns>
	bool Replace()
	{
		// check is hook has been created
		if (pBaseFn == nullptr)
			return false;

		// check that function isn't already hooked
		if (bIsHooked)
			return false;

		if (const MH_STATUS status = MH_EnableHook(pBaseFn); status != MH_OK)
		{
			// @todo: debugging assert
			return false;
		}

		// switch hook state
		bIsHooked = true;
		return true;
	}

	/// <summary>
    /// restore original function call and cleanup hook data
    /// </summary>
	/// <returns>
	/// true if hook has been successfully removed, false otherwise
	/// </returns>
	bool Remove()
	{
		// restore it at first
		if (!Restore())
			return false;

		if (const MH_STATUS status = MH_RemoveHook(pBaseFn); status != MH_OK)
		{
			// @todo: debugging assert
			return false;
		}

		return true;
	}

	/// <summary>
    /// restore patched memory to original function call
    /// </summary>
	/// <returns>
	/// true if hook has been successfully restored, false otherwise
	/// </returns>
	bool Restore()
	{
		// check that function is hooked
		if (!bIsHooked)
			return false;

		if (const MH_STATUS status = MH_DisableHook(pBaseFn); status != MH_OK)
		{
			// @todo: debugging assert
			return false;
		}

		// switch hook state
		bIsHooked = false;
		return true;
	}

	/// <returns>
	/// original, unwrapped function that would be called without the hook
	/// </returns>
	__forceinline T GetOriginal()
	{
		return reinterpret_cast<T>(pOriginalFn);
	}

	/// <returns>
	/// true if hook is applied at the time, false otherwise
	/// </returns>
	__forceinline bool IsHooked() const
	{
		return bIsHooked;
	}

private:
	// current hook state
	bool bIsHooked = false;
	// function base handle
	void* pBaseFn = nullptr;
	// function that being replace the original call
	void* pReplaceFn = nullptr;
	// original function
	void* pOriginalFn = nullptr;
};
