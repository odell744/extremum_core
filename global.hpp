#ifndef EXTREMUM_GLOBALS
#define EXTREMUM_GLOBALS

#include "common.hpp"

namespace extremum
{
	namespace global
	{
		class GlobalVariables;
		typedef std::unique_ptr<GlobalVariables> GlobalVariablesPtr_t;
		

		// variables
		inline Ped m_local_ped{ 0 };
		inline Vehicle m_local_vehicle{ 0 };
		// booleans
		inline bool m_is_in_vehicle{ false };
	}
}

#endif // !EXTREMUM_GLOBALS

