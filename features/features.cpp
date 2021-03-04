#include "..\common.hpp"
#include "features.hpp"
#include "..\logger.hpp"
#include "..\natives\natives.hpp"
#include "..\script\script.hpp"

namespace extremum::core
{
	void features::run_tick()
	{
	}

	void features::script_func()
	{
		while (true)
		{
			run_tick();
			script::get_current()->yield();
		}
	}
}
