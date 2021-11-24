#pragma once
#include "../common.hpp"
#include "imgui.h"
#include "imgui_internal.h"


namespace extremum::core
{
	// forward-declaration for classes
	class option_setting;
	class option;
	class submenu;
	class menu;
	// handling types
	enum handling_type {
		empty_handler,
		int32_handler,
		uint32_handler,
		int64_handler,
		double_handler,
		float_handler,
		bool_handler,
		action_handler,
	};
	class abstract_handler {
	public:
		~abstract_handler() noexcept = default;

		virtual handling_type get_type() { return handling_type::empty_handler;  };

		virtual const std::function<void()>& get_as_action() { throw std::runtime_error("cannot cast handler to action"); };
		virtual bool& get_as_bool() { throw std::runtime_error("cannot cast handler to bool"); };
		virtual float& get_as_float() { throw std::runtime_error("cannot cast handler to float"); };
		virtual double& get_as_double() { throw std::runtime_error("cannot cast handler to double"); };
		virtual std::int32_t& get_as_int32() { throw std::runtime_error("cannot cast handler to int32"); };
		virtual std::uint32_t& get_as_uint32() { throw std::runtime_error("cannot cast handler to uint32"); };
		virtual std::int64_t& get_as_int64() { throw std::runtime_error("cannot cast handler to int64"); };
	protected:
		explicit abstract_handler() = default;
		abstract_handler(abstract_handler const&) = default;
		abstract_handler& operator=(abstract_handler const&) = default;
		abstract_handler(abstract_handler&&) = default;
		abstract_handler& operator=(abstract_handler&&) = default;
	};
	template<class handler>
	class basic_handler : public abstract_handler {
		handling_type m_type{ empty_handler };
	public:
		~basic_handler() noexcept = default;
		handler& set_type(const handling_type& type) { m_type = type; return static_cast<handler&>(*this); };
		handling_type get_type() override { return m_type; };
	protected:
		explicit basic_handler() = default;
		basic_handler(basic_handler const&) = default;
		basic_handler& operator=(basic_handler const&) = default;
		basic_handler(basic_handler&&) = default;
		basic_handler& operator=(basic_handler&&) = default;
	};
	class action_handler_t : public basic_handler<action_handler_t>
	{
		using base = basic_handler<action_handler_t>;
		const std::function<void()>& m_function;
	public:
		~action_handler_t() noexcept = default;
		explicit action_handler_t() = default;
		action_handler_t(const std::function<void()>& func) : base(), m_function(func) { base::set_type(handling_type::action_handler); };
		const std::function<void()>& get_as_action() override { return m_function; };
	};
	class bool_handler_t : public basic_handler<bool_handler_t>
	{
		using base = basic_handler<bool_handler_t>;
		bool& m_boolean;
	public:
		~bool_handler_t() noexcept = default;
		explicit bool_handler_t() = default;
		bool_handler_t(bool& boolean_value) : base(), m_boolean(boolean_value) { base::set_type(handling_type::bool_handler); };
		bool& get_as_bool() override { return m_boolean; };
	};
	class int32_handler_t : public basic_handler<int32_handler_t>
	{
		using base = basic_handler<int32_handler_t>;
		std::int32_t& m_value;
	public:
		~int32_handler_t() noexcept = default;
		explicit int32_handler_t() = default;
		int32_handler_t(std::int32_t& value) : base(), m_value(value) { base::set_type(handling_type::int32_handler); };
		std::int32_t& get_as_int32() override { return m_value; };
	};
	class uint32_handler_t : public basic_handler<uint32_handler_t>
	{
		using base = basic_handler<uint32_handler_t>;
		std::uint32_t& m_value;
	public:
		uint32_handler_t(std::uint32_t& value) : base(), m_value(value) { base::set_type(handling_type::uint32_handler); };
		std::uint32_t& get_as_uint32() override { return m_value; };
	};
	class int64_handler_t : public basic_handler<int64_handler_t>
	{
		using base = basic_handler<int64_handler_t>;
		std::int64_t& m_value;
	public:
		~int64_handler_t() noexcept = default;
		explicit int64_handler_t() = default;
		int64_handler_t(std::int64_t& value) : base(), m_value(value) { base::set_type(handling_type::int64_handler); };
		std::int64_t& get_as_int64() override { return m_value; };
	};
	class float_handler_t : public basic_handler<float_handler_t>
	{
		using base = basic_handler<float_handler_t>;
		float& m_value;
	public:
		~float_handler_t() noexcept = default;
		explicit float_handler_t() = default;
		float_handler_t(float& value) : base(), m_value(value) { base::set_type(handling_type::float_handler); };
		float& get_as_float() override { return m_value; };
	};
	class double_handler_t : public basic_handler<double_handler_t>
	{
		using base = basic_handler<double_handler_t>;
		double& m_value;
	public:
		~double_handler_t() noexcept = default;
		explicit double_handler_t() = default;
		double_handler_t(double& value) : base(), m_value(value) { base::set_type(double_handler); };
		double& get_as_double() override { return m_value; };
	};

	enum current_window
	{
		submenus,
		options,
		settings
	};

	// main menu types
	class option_setting
	{
		using handler_t = std::unique_ptr<abstract_handler>;

		const std::uint32_t		m_id;
		const std::string		m_name;
		const std::string		m_description;

		handler_t				m_option_handler;
		option&					m_handler_option;

		bool					m_is_vip{ false };

		ImVec4					m_block_color{};
		ImVec4					m_text_color{};
	public:
		option_setting(const std::uint32_t& m_id, const std::string& m_name, const std::string& m_description, option& handler);

		template<typename Handler>
		option_setting&			add_handler(std::unique_ptr<Handler>&& handler);
		option_setting&			set_vip();
		option&					end_setting();

		bool					is_vip();

		handler_t&				get_handle() { return m_option_handler; };

		ImVec4&					get_block_color() { return m_block_color; };
		ImVec4&					get_text_color() { return m_text_color; };

		const std::uint32_t		id() { return m_id; };
		const std::string		name() { return m_name; };
		const std::string		description() { return m_description; };
	};

	class option
	{
		using handler_t = std::unique_ptr<abstract_handler>;
		using settings_t = std::vector<option_setting*>;

		const std::uint32_t		m_id;
		const std::string		m_name;
		const std::string		m_description;

		std::uint32_t			m_selected_setting{ 0 };

		handler_t				m_option_handler;
		submenu&				m_handler_submenu;
		settings_t				m_settings;

		bool					m_is_vip{ false };

		ImVec4					m_block_color{};
		ImVec4					m_text_color{};
	public:
		option(const std::uint32_t& m_id, const std::string& m_name, const std::string& m_description, submenu& handler);

		template<typename Handler>
		option&					add_handler(std::unique_ptr<Handler>&& handler);
		option_setting&			add_setting(const std::string& name, const std::string& description);
		option&					set_vip();
		submenu&				end_option();

		bool					is_vip();

		option_setting&			get_selected_setting() { return *m_settings[m_selected_setting]; };
		settings_t&				get_settings() { return m_settings; };
		handler_t&				get_handle() { return m_option_handler; };

		ImVec4&					get_block_color() { return m_block_color; };
		ImVec4&					get_text_color() { return m_text_color; };

		std::uint32_t			counter() { return m_settings.size(); };
		std::uint32_t			selected_id() { return m_selected_setting; };

		void					up()  { 
			m_selected_setting > 0 ? m_selected_setting-- : m_selected_setting = counter() - 1;
			LOG_DEBUG("option.up() [id: {} | result: [m_selected_setting: {} | m_setting_counter: {}]]", m_id, m_selected_setting, counter() - 1);
		};
		void					down() {
			m_selected_setting < counter() - 1 ? m_selected_setting++ : m_selected_setting = 0;
			LOG_DEBUG("option.down() [id: {} | result: [m_selected_setting: {} | m_setting_counter: {}]]", m_id, m_selected_setting, counter() - 1);
		};

		const std::uint32_t		id() { return m_id; };
		const std::string		name() { return m_name; };
		const std::string		description() { return m_description; };
	};
	class submenu
	{
		using options_t = std::vector<option*>;

		const std::uint32_t		m_id;
		const std::string		m_name;
		const std::string		m_description;

		std::uint32_t			m_selected_option{ 0 };

		menu&					m_handler_menu;
		options_t				m_options;

		bool					m_is_vip{ false };

		ImVec4					m_block_color{};
		ImVec4					m_text_color{};
	public:
		submenu(const std::uint32_t& id, const std::string& name, const std::string description, menu& handler);

		option&					add_option(const std::string& name, const std::string& description);
		submenu&				set_vip();
		menu&					end_submenu();

		bool					is_vip();

		option&					get_selected_option() { return *m_options[m_selected_option]; };
		options_t&				get_options() { return m_options; };

		ImVec4&					get_block_color() { return m_block_color; };
		ImVec4&					get_text_color() { return m_text_color; };

		std::uint32_t			counter() { return m_options.size(); };
		std::uint32_t			selected_id() { return m_selected_option; };

		void					up() { 
			m_selected_option > 0 ? m_selected_option-- : m_selected_option = counter() - 1;
			LOG_DEBUG("submenu.up() [id: {} | result: [m_selected_option: {} | m_options_counter: {}]]", m_id, m_selected_option, counter() - 1);
		};
		void					down() { 
			m_selected_option < counter() - 1 ? m_selected_option++ : m_selected_option = 0;
			LOG_DEBUG("submenu.down() [id: {} | result: [m_selected_option: {} | m_options_counter: {}]]", m_id, m_selected_option, counter() - 1);
		};

		const std::uint32_t		id() { return m_id; };
		const std::string		name() { return m_name; };
		const std::string		description() { return m_description; };
	};
	class menu
	{
		using submenus_t = std::vector<submenu*>;
		
		std::uint32_t			m_selected_menu{ 0 };

		submenus_t				m_submenus;

		current_window			m_current_window{ current_window::options };
	public:
		menu();
		submenu&				add_submenu(const std::string& name, const std::string& description);

		submenu&				get_selected_submenu() { return *m_submenus[m_selected_menu]; };
		submenus_t&				get_submenus() { return m_submenus; };

		current_window			get_current_window() { return m_current_window; };
		void					set_current_window(current_window state) { m_current_window = state; };

		std::uint32_t			counter() { return m_submenus.size(); };
		std::uint32_t			selected_id() { return m_selected_menu; };

		void					up() { 
			m_selected_menu > 0 ? m_selected_menu-- : m_selected_menu = counter() - 1;
			LOG_DEBUG("menu.up() [result: [m_selected_menu: {} | m_submenus_counter: {}]]", m_selected_menu, counter() - 1);
		};
		void					down() { 
			m_selected_menu < counter() - 1 ? m_selected_menu++ : m_selected_menu = 0;
			LOG_DEBUG("menu.down() [result: [m_selected_menu: {} | m_submenus_counter: {}]]", m_selected_menu, counter() - 1);
		};

	};
	// main drawing class

	

	class gui
	{
	private:
		using resourse_container_t = std::vector<ID3D11ShaderResourceView*>;

		int block_delimeter{ 16 };
		std::uint32_t max_submenus{ 10 }, max_options{ 16 }, max_settings{ 16 };
		std::uint32_t m_current_submenu{}, m_current_option{}, m_current_setting{};
		std::uint32_t m_submenu_count{}, m_option_count{}, m_setting_count{};
		bool					m_block_controlls{ false };
		bool					m_pressed{};
		bool					m_back{};
		bool					m_up_pressed{};
		bool					m_down_pressed{};
		bool					m_left_pressed{};
		bool					m_right_pressed{};
		menu					m_menu{};
		resourse_container_t	m_textures;

		void check_controlls();
		void reset_controlls();

		template<typename _Ty>
		void slide_to(_Ty& _value, _Ty _slide_value, int speed = 30);


		bool m_header_expanded{ true };

		ImVec2 m_settings_size{ 350.f, 500.f }, 
			   m_options_size{ 400.f, 500.f },
			   m_submenus_size{ 350.f, 500.f },
			   m_header_min_size{ 400.f, 50.f },
			   m_header_max_size{ 400.f, 100.f },
			   m_header_current_size{ m_header_max_size },
			   m_footer_current_size{ m_header_min_size };
		ImVec2 m_pos{ 100.f, 100.f },
			m_header_pos{},
			m_footer_pos{},
			m_settings_pos{},
			m_options_pos{ 0.f, 0.f },
			m_submenus_pos{};


		ImVec4 m_non_selected_color{ gui::color_from_hex(0x000000, .0f) };
		ImVec4 m_near_selected_color{ gui::color_from_hex(0x78909C, .3f) };
		ImVec4 m_selected_color{ gui::color_from_hex(0x90A4AE, .8f) };

		void calculate_sizes();
	public:
		bool m_startup_anim{ true };

		void init_textures();

		void dx_init();
		void dx_on_tick();

		void script_init();
		void script_on_tick();

		void background_overlay();

		static void script_func();
		
		// for design

		void draw_line(const ImVec2& p_min, const ImVec2& p_max, const ImVec4& color, float thickness = 1.f);
		void draw_rect(const ImVec2& pos, const ImVec2& size, const ImVec4& color);
		void draw_rect_rounded(const ImVec2& pos, const ImVec2& size, const ImVec4& color, float round = 0.f);
		void draw_text(const std::string_view& text, const ImVec2& pos, const ImVec2& size, const ImVec4& color = {1.f, 1.f, 1.f, 1.f});

		void draw_footer_window();
		void draw_header_window();
		void draw_submenus_window();
		void draw_options_window();
		void draw_settings_window();

		void calculate_color(std::uint32_t id, std::uint32_t current_id, ImVec4& color, const ImVec4& selected_color, const ImVec4& near_selected_color, const ImVec4& non_selected_color);

		void draw_submenu(submenu& submenu);
		void draw_option(option& option);
		void draw_setting(option_setting& setting);

		static ImVec4 color_from_hex(std::uint32_t value, float alpha = 1.f);
	public:
		bool m_opened{};
		bool m_vip{ false };
	};

	class random_color
	{
		ImVec4 color_vector{0.f, 0.f, 0.f, 1.f};
		void generate() { 
			color_vector.x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (255)))) / 255.f;
			color_vector.y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (255)))) / 255.f;
			color_vector.z = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (255)))) / 255.f;
		};
	public:
		random_color() { generate(); };
		ImVec4& get() { return color_vector; };
	};

	class random_color_timed
	{
		std::chrono::system_clock::time_point timer;
		ImVec4 color_vector{0.f, 0.f, 0.f, 1.f};
		void generate() {
			std::chrono::duration<double> timer_elapsed = std::chrono::system_clock::now() - timer;

			if (timer_elapsed.count() >= 1.000)
			{
				color_vector.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				color_vector.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				color_vector.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

				timer = std::chrono::system_clock::now();
			}
		};
	public:
		random_color_timed() {
			timer = std::chrono::system_clock::now();
			color_vector.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			color_vector.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			color_vector.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		};
		ImVec4& get() {
			generate();
			return color_vector; 
		};
	};

	inline gui g_gui;
}
