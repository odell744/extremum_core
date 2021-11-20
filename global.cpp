#include "global.hpp"

#include "natives/natives.hpp"

namespace extremum
{
	namespace global
	{
		global_variables::global_variables()
			: base("global_variables", std::move([] {}))
		{
			g_global_variables = this;
		}
		global_variables::~global_variables()
		{
			g_global_variables = nullptr;
		}
		
		void global_variables::tick()
		try
		{
			m_local_ped = PLAYER::PLAYER_PED_ID();
		}
		catch (const std::exception& exception)
		{
			throw std::runtime_error("Error while updating global variables");
		}
	}
}