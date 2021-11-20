#pragma once

#include "../common.hpp"
#include "fwddec.hpp"
#include <cstdint>


namespace rage
{
	class fwEntity
	{
	public:
		char pad_0000[32]; //0x0000
		class CBaseModelInfo* m_model_info; //0x0020
		int8_t m_invisible; //0x0028
		char pad_0029[7]; //0x0029
		class CNavigation* m_navigation; //0x0030
		char pad_0038[1]; //0x0038
		int8_t m_entity_type; //0x0039
		char pad_003A[150]; //0x003A
		class netObject* m_net_object; //0x00D0
		char pad_00D8[176]; //0x00D8
		uint32_t m_damage_bits; //0x0188

		bool is_invincible() { return(m_damage_bits & (1 << 8)); }
		void enable_invincible() { m_damage_bits |= (1 << 8); }
		void disable_invincible() { m_damage_bits &= ~(1 << 8); }
	}; //Size: 0x018C
	//static_assert(sizeof(fwEntity) == 0x18C, "fwEntity is not properly sized size: %d", sizeof(fwEntity));


	class CVehicle : public fwEntity
	{
	public:
		char pad_018C[2716]; //0x018C
		//class CPed* m_driver; //0x0C28
		//class CPed* m_occupants[8]; //0x0C30
	}; //Size: 0x018C

	class CNavigation
	{
	public:
		char pad_0000[32]; //0x0000
		float m_heading; //0x0020
		float m_heading2; //0x0024
		char pad_0028[8]; //0x0028
		rage::vector3 m_rotation; //0x0030
		char pad_003C[20]; //0x003C
		rage::vector3 m_position; //0x0050
	}; //Size: 0x005C

	class CPed : public fwEntity
	{
	public:
		char pad_018C[2972]; //0x018C
		class CVehicle* m_last_vehicle; //0x0D28
		char pad_0D30[892]; //0x0D30
		int8_t m_decal_index; //0x10AC
		int8_t m_decal_info; //0x10AD
		char pad_10AE[26]; //0x10AE
		class CPlayerInfo* m_player_info; //0x10C8
	};

	class CObject
	{
	public:
		char pad_0x0000[0x8]; //0x0000
	}; //Size=0x0008
	class CObjectHandle
	{
	public:
		CObject* pCObject; //0x0000 
		__int32 iHandle; //0x0008 
		char pad_0x000C[0x4]; //0x000C
	}; //Size=0x0010
	class CObjectList
	{
	public:
		CObjectHandle ObjectList[2300]; //0x0000 
	}; //Size=0x8FC0
	class CObjectInterface
	{
	public:
		char pad_0x0000[0x158]; //0x0000
		CObjectList* pCObjectList; //0x0158 
		__int32 iMaxObjects; //0x0160 
		char pad_0x0164[0x4]; //0x0164
		__int32 iCurObjects; //0x0168 
		char pad_0x016C[0x5C]; //0x016C
		CObject* get_object(const int& index)
		{
			if (index < iMaxObjects)
				return this->pCObjectList->ObjectList[index].pCObject;
			return nullptr;
		}
	}; //Size=0x01C8

	class CReplayInterfacePed
	{
	private:
		class CPedList
		{
		private:
			struct Ped
			{
				CPed* ped;
				__int32 handle;
				char _pad0[0x4];
			};

		public:
			Ped peds[0x100];
		};

	public:
		char _pad0[0x100];            // 0000 - 0100
		CPedList* ped_list;            // 0100 - 0108
		int max_peds;                // 0108 - 010C
		char _pad1[0x4];            // 010C - 0110
		int number_of_peds;            // 0110 - 0114

		CPed* get_ped(const int& index)
		{
			if (index < max_peds)
				return ped_list->peds[index].ped;
			return nullptr;
		}

		int get_ped_handle(const int& index)
		{
			if (index < max_peds && ped_list->peds[index].ped != nullptr)
				return ped_list->peds[index].handle;
			return -1;
		}
	};
	class CCameraInterface
	{
	public:
		char pad_0x0000[0x2]; //0x0000

	}; //Size=0x0002
	class CVehicleInterface
	{
	public:
		char pad_0x0000[0x8]; //0x0000

	}; //Size=0x0008
	class CPickupInterface
	{
	public:
		char pad_0x0000[0x8]; //0x0000

	}; //Size=0x0008
	class CReplayInterface
	{
	public:
		void* N000006F5; //0x0000 
		CCameraInterface* pCCameraInterface; //0x0008 
		CVehicleInterface* pCVehicleInterface; //0x0010 
		CReplayInterfacePed* pCPedInterface; //0x0018 
		CPickupInterface* pCPickupInterface; //0x0020 
		CObjectInterface* pCObjectInterface; //0x0028 
	}; //Size=0x0030
}