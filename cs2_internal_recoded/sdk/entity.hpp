#pragma once

// used: stdint
#include <cstdint>
// used: SCHEMA_ADD_OFFSET
#include "../core/schema.hpp"
// used: macros
#include "entity_handle.hpp"
// used: offsets
#include "../offset/offset.hpp"
// used: call_VFunc
#include "../memory/memory.hpp"
// used: Matrix2x4_t
#include "../sdk/datatypes/matrix.hpp"



namespace entity {
	using namespace cs2_dumper;

	inline std::uintptr_t get_from_handle(std::uintptr_t entityList, std::uintptr_t handle) {
		std::uintptr_t listEntrySecond = memory::mem.read_memory<std::uintptr_t>(entityList + 0x8ULL * ((handle & 0x7FFF) >> 9) + 0x10);
		return listEntrySecond == 0 ? 0 : memory::mem.read_memory<std::uintptr_t>(listEntrySecond + 0x78ULL * (handle & 0x1FF));
	}

	using game_time_t = float;
	using game_tick_t = std::int32_t;

	class CEntityIdentity;

	class CEntityInstance {
	public:
		CEntityInstance() = delete;

		SCHEMA_ADD_OFFSET(CEntityIdentity*, m_pEntity, schemas::client_dll::CEntityInstance::m_pEntity)
	};

	class CEntityIdentity {
	public:
		CEntityIdentity() = delete;

		SCHEMA_ADD_OFFSET(std::uint32_t, get_index, 0x10)
		SCHEMA_ADD_OFFSET(const char*, get_designer_name, schemas::client_dll::CEntityIdentity::m_designerName)
		SCHEMA_ADD_OFFSET(std::uint32_t, get_flags, schemas::client_dll::CEntityIdentity::m_flags)


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

	class CSkeletonInstance;

	class CGameSceneNode {
	public:
		CGameSceneNode() = delete;

		CSkeletonInstance* get_skeleton_instance()
		{
			return memory::mem.call_VFunc<CSkeletonInstance*, 8U>(this);
		}
	};

	class CSkeletonInstance : public CGameSceneNode
	{
	public:
		MEM_PAD(0x1CC); //0x0000
		int nBoneCount; //0x01CC
		MEM_PAD(0x18); //0x01D0
		int nMask; //0x01E8
		MEM_PAD(0x4); //0x01EC
		Matrix2x4_t* pBoneCache; //0x01F0
	};

	class C_BaseEntity : public CEntityInstance {
	public:
		C_BaseEntity() = delete;

		SCHEMA_ADD_OFFSET(CGameSceneNode*, m_pGameSceneNode, schemas::client_dll::C_BaseEntity::m_pGameSceneNode)
		SCHEMA_ADD_OFFSET(std::int32_t, m_iHealth, schemas::client_dll::C_BaseEntity::m_iHealth)
		SCHEMA_ADD_OFFSET(std::int32_t, m_iMaxHealth, schemas::client_dll::C_BaseEntity::m_iMaxHealth)
		SCHEMA_ADD_OFFSET(std::uint8_t, m_lifeState, schemas::client_dll::C_BaseEntity::m_lifeState)
		SCHEMA_ADD_OFFSET(float, m_flSpeed, schemas::client_dll::C_BaseEntity::m_flSpeed)
		SCHEMA_ADD_OFFSET(std::uint8_t, m_iTeamNum, schemas::client_dll::C_BaseEntity::m_iTeamNum)

	};


	class C_BaseModelEntity : public C_BaseEntity {
	public:
		C_BaseModelEntity() = delete;
	};

	class CCSPlayerController;
	class CPlayer_ObserverServices;

	class C_BasePlayerPawn : public C_BaseModelEntity {
	public:
		C_BasePlayerPawn() = delete;

		SCHEMA_ADD_OFFSET(CPlayer_ObserverServices*, m_pObserverServices, schemas::client_dll::C_BasePlayerPawn::m_pObserverServices)
		SCHEMA_ADD_OFFSET(std::uintptr_t, m_hController, schemas::client_dll::C_BasePlayerPawn::m_hController)

		[[nodiscard]] Vector_t GetEyePosition()
		{
			Vector_t vecEyePosition = Vector_t(0.0f, 0.0f, 0.0f);
			memory::mem.call_VFunc<void, 166U>(this, &vecEyePosition);
			return vecEyePosition;
		}

		[[nodiscard]] CCSPlayerController* get_controller(std::uintptr_t entity_list);
	};


	class CBasePlayerController : public C_BaseModelEntity {
	public:
		CBasePlayerController() = delete;
		SCHEMA_ADD_OFFSET(std::uint64_t, m_steamID, schemas::client_dll::CBasePlayerController::m_steamID)
		SCHEMA_ADD_OFFSET(std::uintptr_t, m_hPawn, schemas::client_dll::CBasePlayerController::m_hPawn)
		SCHEMA_ADD_OFFSET(bool, m_bIsLocalPlayerController, schemas::client_dll::CBasePlayerController::m_bIsLocalPlayerController)

	};

	class CCSPlayerController : public CBasePlayerController
	{
	public:
		CCSPlayerController() = delete;

		[[nodiscard]] C_BasePlayerPawn* get_base_pawn(std::uintptr_t entity_list);

		SCHEMA_ADD_OFFSET(std::uint32_t, m_iPing, schemas::client_dll::CCSPlayerController::m_iPing)
		SCHEMA_ADD_OFFSET(const char*, m_sSanitizedPlayerName, schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName)
		SCHEMA_ADD_OFFSET(std::int32_t, m_iPawnHealth, schemas::client_dll::CCSPlayerController::m_iPawnHealth)
		SCHEMA_ADD_OFFSET(std::int32_t, m_iPawnArmor, schemas::client_dll::CCSPlayerController::m_iPawnArmor)
		SCHEMA_ADD_OFFSET(bool, m_bPawnHasDefuser, schemas::client_dll::CCSPlayerController::m_bPawnHasDefuser)
		SCHEMA_ADD_OFFSET(bool, m_bPawnHasHelmet, schemas::client_dll::CCSPlayerController::m_bPawnHasHelmet)
		SCHEMA_ADD_OFFSET(bool, m_bPawnIsAlive, schemas::client_dll::CCSPlayerController::m_bPawnIsAlive)
		SCHEMA_ADD_OFFSET(std::uintptr_t, m_hPlayerPawn, schemas::client_dll::CCSPlayerController::m_hPlayerPawn)

	};

	class CPlayer_ObserverServices {
	public:
		CPlayer_ObserverServices() = delete;

		[[nodiscard]] C_BaseEntity* get_target_pawn(std::uintptr_t entity_list);

		SCHEMA_ADD_OFFSET(std::uint8_t, m_iObserverMode, schemas::client_dll::CPlayer_ObserverServices::m_iObserverMode)
		SCHEMA_ADD_OFFSET(std::uintptr_t, m_hObserverTarget, schemas::client_dll::CPlayer_ObserverServices::m_hObserverTarget)
	};

}
