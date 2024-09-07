#pragma once

// used: stdint
#include <cstdint>

#include "../core/schema.hpp"

#include "entity_handle.hpp"

#include "../offset/offset.hpp"

using game_time_t = float;
using game_tick_t = std::int32_t;

class CEntityInstance;

class CEntityIdentity
{
public:
	CEntityIdentity() = delete;

	[[nodiscard]] __forceinline std::add_lvalue_reference_t<std::uint32_t> get_index() {
		static const std::uint32_t uOffset = 0x10;
		return *reinterpret_cast<std::add_pointer_t<std::uint32_t>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));
	};

	[[nodiscard]] __forceinline std::add_lvalue_reference_t<const char*> get_designer_name() {
		static const std::uint32_t uOffset = cs2_dumper::schemas::client_dll::CEntityIdentity::m_designerName;
		return *reinterpret_cast<std::add_pointer_t<const char*>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));
	};

	[[nodiscard]] __forceinline std::add_lvalue_reference_t<std::uint32_t> get_flags() {
		static const std::uint32_t uOffset = cs2_dumper::schemas::client_dll::CEntityIdentity::m_flags;
		return *reinterpret_cast<std::add_pointer_t<std::uint32_t>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));
	};

	[[nodiscard]] bool is_valid()
	{
		return get_index() != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int get_entry_index()
	{
		if (!is_valid())
			return ENT_ENTRY_MASK;

		return get_index() & ENT_ENTRY_MASK;
	}

	[[nodiscard]] int get_serial_number()
	{
		return get_index() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	void* pInstance; // 0x00
};
