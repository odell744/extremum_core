#pragma once
#pragma pack(push, 1)

#include "extensible.hpp"
#include "vector.hpp"

namespace rage
{
	class CGameInvite
	{
	public:
		char pad_0008[0x128];	//0x008
		int16_t _f;				//0x128 + 0x008
	};
}
#pragma pack(pop)