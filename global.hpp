#ifndef EXTREMUM_GLOBALS
#define EXTREMUM_GLOBALS

#include "common.hpp"
#include "loop/loop.hpp"

namespace extremum
{
	namespace global
	{
		class global_variables;
		typedef global_variables* global_variables_ptr_t;
		class global_variables : public extremum::core::void_loop
		{
		public:
			global_variables();
			~global_variables();

			void tick();
		private:
			Ped m_local_ped{ 0 };
			Vehicle m_local_vehicle{ 0 };
			// booleans
			bool m_is_in_vehicle{ false };
		};
		
		global_variables_ptr_t	g_global_variables;
	}
}

#endif // !EXTREMUM_GLOBALS

