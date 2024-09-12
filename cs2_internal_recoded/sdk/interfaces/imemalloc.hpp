#pragma once
// used: mem::CallVFunc
#include "../../memory/memory.hpp"
#pragma warning(push)
#pragma warning(disable : 4191)

class IMemAlloc
{
public:
	void* alloc(std::size_t nSize)
	{
		return memory::mem.call_VFunc<void*, 1>(this, nSize);
	}

	void* re_alloc(const void* pMemory, std::size_t nSize)
	{
		return memory::mem.call_VFunc<void*, 2>(this, pMemory, nSize);
	}

	void free(const void* pMemory)
	{
		return memory::mem.call_VFunc<void, 3>(this, pMemory);
	}

	std::size_t get_size(const void* pMemory)
	{
		return memory::mem.call_VFunc<std::size_t, 21>(this, pMemory);
	}
};

#pragma warning(pop)