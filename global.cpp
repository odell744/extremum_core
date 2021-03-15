#include "global.hpp"

namespace extremum
{
	namespace global
	{
		global_variables::global_variables()
		{
			g_global_variables = this;
		}
		global_variables::~global_variables
		{
			g_global_variables = nullptr;
		}
	}
