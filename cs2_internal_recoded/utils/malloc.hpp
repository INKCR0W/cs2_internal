#pragma once

// used: memory api
#include <malloc.h>

#pragma warning(push)
#pragma warning(disable: 6255) // '_alloca' indicates failure by raising a stack overflow exception. consider using '_malloca' instead
#define MEM_STACKALLOC(SIZE) _malloca(SIZE)
#pragma warning(pop)
#define MEM_STACKFREE(MEMORY) static_cast<void>(0)