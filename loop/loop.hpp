#pragma once
#ifndef EXTREMUM_CORE_LOOP
#define EXTREMUM_CORE_LOOP

#include "../common.hpp"

namespace extremum
{
	namespace core
	{
		class general_loop
		{
		public:
			virtual void tick() = 0;
		};

		template<class loop_type, typename function_return_type, typename ...arg_types>
		class base_loop : public general_loop
		{
		public:
			using function_type = std::function<function_return_type(arg_types...)>;

			base_loop(std::string&& name, std::function<function_return_type(arg_types...)>&& action);
			~base_loop() = default;
			
			void tick();

			std::string				get_name();
			function_return_type	do_action(arg_types&&... args);
		private:
			std::string				m_name;
			function_type			m_action;
		};

		class void_loop : base_loop<void_loop, void>
		{
		private:
			using base = base_loop<void_loop, void>;
		public:
			void_loop(std::string&& name, base::function_type&& action);
			~void_loop() = default;

			void tick();
		};

		class bool_loop : base_loop<bool_loop, bool>
		{
		private:
			using base = base_loop<bool_loop, bool>;
		public:
			bool_loop(std::string&& name, bool& enable_bool, base::function_type&& action);
			~bool_loop() = default;

			void tick();
		private:
			bool&					m_enable;
		};
	}
}

#endif // !EXTREMUM_CORE_LOOP
