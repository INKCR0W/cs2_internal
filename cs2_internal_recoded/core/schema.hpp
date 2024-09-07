#pragma once

#include <type_traits>


#define SCHEMA_ADD_OFFSET(TYPE, NAME, OFFSET)                                                                 \
	[[nodiscard]] __forceinline std::add_lvalue_reference_t<TYPE> NAME()                                          \
	{                                                                                                         \
		static const std::uint32_t uOffset = OFFSET;                                                          \
		return *reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
	}

#define SCHEMA_ADD_POFFSET(TYPE, NAME, OFFSET)                                                               \
	[[nodiscard]] __forceinline std::add_pointer_t<TYPE> NAME()                                                  \
	{                                                                                                        \
		const static std::uint32_t uOffset = OFFSET;                                                         \
		return reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
	}


#define SCHEMA_ADD_FIELD(TYPE, NAME, FIELD) SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, 0U)

#define SCHEMA_ADD_PFIELD(TYPE, NAME, FIELD) SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, 0U)
