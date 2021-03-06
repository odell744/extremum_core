#include "loop.hpp"

namespace extremum {
	namespace core {
		template<class loop_type, typename function_return_type, typename ...arg_types>
		base_loop<loop_type, function_return_type, arg_types...>::base_loop(
			std::string&& name,
			std::function<function_return_type(arg_types...)>&& action)
			:
			m_name(std::move(name)),
			m_action(std::move(action))
		{
		}

		template<class loop_type, typename function_return_type, typename ...arg_types>
		void base_loop<loop_type, function_return_type, arg_types...>::tick()
		{};

		template<class loop_type, typename function_return_type, typename ...arg_types>
		std::string base_loop<loop_type, function_return_type, arg_types...>::get_name()
		{
			return m_name; 
		}
		template<class loop_type, typename function_return_type, typename ...arg_types>
		function_return_type base_loop<loop_type, function_return_type, arg_types...>::do_action(arg_types && ...args)
		{
			return m_action(std::forward<arg_types>()...);
		}
		bool_loop::bool_loop(std::string&& name, bool& enable_bool, base::function_type&& action)
			:
			base(std::move(name), std::move(action)),
			m_enable(enable_bool)
		{
		}
		void bool_loop::tick()
		{
			if (m_enable)
				base::do_action();
		}
	}
}