#pragma once

// used: callvfunc
#include "../../memory/memory.hpp"

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// a network packet is being received
	FRAME_NET_UPDATE_START,
	// data has been received and we are going to start calling postdataupdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// data has been received and called postdataupdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// received all packets, we can now do interpolation, prediction, etc
	FRAME_NET_UPDATE_END,
	// start rendering the scene
	FRAME_RENDER_START,
	// finished rendering the scene
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class IEngineClient
{
public:
	int GetMaxClients()
	{
		return memory::mem.call_VFunc<int, 34U>(this);
	}

	bool IsInGame()
	{
		return memory::mem.call_VFunc<bool, 35U>(this);
	}

	bool IsConnected()
	{
		return memory::mem.call_VFunc<bool, 36U>(this);
	}

	// return CBaseHandle index
	int GetLocalPlayer()
	{
		int nIndex = -1;

		memory::mem.call_VFunc<void, 47U>(this, std::ref(nIndex), 0);

		return nIndex + 1;
	}

	[[nodiscard]] const char* GetLevelName()
	{
		return memory::mem.call_VFunc<const char*, 56U>(this);
	}

	[[nodiscard]] const char* GetLevelNameShort()
	{
		return memory::mem.call_VFunc<const char*, 57U>(this);
	}

	[[nodiscard]] const char* GetProductVersionString()
	{
		return memory::mem.call_VFunc<const char*, 82U>(this);
	}
};

