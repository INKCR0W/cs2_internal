#pragma once

#ifndef DEBUG_BREAK
#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__)
#define DEBUG_BREAK() __builtin_debugtrap()
#else
static_assert(false, "it is expected you to define CS_DEBUG_BREAK() into something that will break in a debugger!");
#define DEBUG_BREAK()
#endif
#endif

#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (DEBUG_BREAK(), 0))
#else
// disable assertion for release builds
#define ASSERT(EXPRESSION) static_cast<void>(0)
#endif
#endif
