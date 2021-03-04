#pragma once
#include "../common.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace extremum::core
{
	enum startup_anim
	{
		BG_IN,
		TEXT_IN,
		UNDER_LINE_IN,
		TEXT_WAIT,
		UNDER_LINE_OUT,
		TEXT_OUT,
		BG_OUT
	};
	/// <summary>
	/// List of availible menus
	/// </summary>
	enum sub_menus : uint32_t {
		mainmenu,

		self_options,
		protection_menu,
		online_players,
		network_options,
		weapon_options,
		teleport_options,
		vehicle_spawn_options,
		vehicle_options,
		money_options,
		recovery_options,
		spawn_options,
		world_options,
		extremum_exclusive,
		gameplay_options,
		menu_settings,

		/// <summary>
		/// self options
		/// </summary>
		personal_globals,
		wanted_level_options,
		noclip_options,
		freecamera_options,
		wardrobe_options,
		ride_options,
		vision_effects_options,
		models_changer,
		animations, 
		scenarios,
		walk_styles,
		particle_effects,
		cutscenes,
		auto_functions,
		local_name_changer
	};

	template<typename NumberType>
	struct NumberRange {
		NumberType min;
		NumberType max;
		NumberType step;
	};

	class gui
	{
	private:
		sub_menus		m_current_menu{ sub_menus::mainmenu };
		sub_menus		m_menu_levels[1000]{ sub_menus::mainmenu };
		std::uint32_t	m_menu_option_level[1000]{ 1 };

		std::uint32_t	m_menu_level{ 0 };
		std::uint32_t	button_visible{ 16 };
		float			button_height{ 35.f };
		std::uint32_t	m_current_button{ 1 };
		std::uint32_t	m_button_counter{ 0 };
		std::uint32_t	m_hovered_id{ 0 };

		///
		///	CONTROLS
		/// 

		bool			m_block_controlls{ false };
		bool			m_pressed{};
		bool			m_back{};
		bool			m_up_pressed{};
		bool			m_down_pressed{};
		bool			m_left_pressed{};
		bool			m_right_pressed{};

		void check_controlls();

		template<typename _Ty>
		void slide_to(_Ty& _value, _Ty _slide_value, int speed = 30);

		// design variables
		std::vector<ID3D11ShaderResourceView*> m_toggle;

		std::vector<ID3D11ShaderResourceView*> m_header_exclusive;
		std::vector<ID3D11ShaderResourceView*> m_header_vip;
		std::vector<ID3D11ShaderResourceView*> m_header_standard;

		std::vector<ID3D11ShaderResourceView*>* m_header{ nullptr };

		std::map<std::string, ID3D11ShaderResourceView*> m_icons;
		std::map<std::string, ID3D11ShaderResourceView*> m_menu_icons;

		std::vector<std::string> m_icons_names{};
		std::vector<std::string> m_menu_icons_names{ "arrow", "mp_specitem_ped", "mp_specitem_safe", "survival", "team_deathmatch", "ammo_pickup",
													 "shooting_range", "mp_specitem_car", "vehicle_deathmatch", "mp_specitem_cash", "mp_alerttriangle",
													 "mp_specitem_package", "mp_specitem_remote", "custom_mission", "mp_specitem_meth", "mp_specitem_keycard" };
		ImVec2 menu_size_visible{ 400.f, 0.f };

		//
		//	STARTUP ANIMATION
		//
		startup_anim m_startup_anim_pos{ startup_anim::BG_IN };

		std::map<core::sub_menus, std::string> m_subtitles{{ sub_menus::mainmenu, "mainMenu" }};
	public:
		bool m_startup_anim{ true };

		void move_menu(sub_menus sub_menu);
		void back_menu();

		void init_textures();

		void dx_init();
		void dx_on_tick();
		void dx_on_disable();

		void script_init();
		void script_on_tick();

		void background_overlay();

		static void script_func();
		
		// for design
		bool button(std::string text, bool vip = false,  std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		bool button(std::string text, std::string description, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
	
		bool menu_option(std::string text, sub_menus next_level, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		bool menu_option(std::string text, sub_menus next_level, std::string description, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		bool menu_option(std::string text, std::string icon, sub_menus next_level, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		bool menu_option(std::string text, std::string icon, sub_menus next_level, std::string description, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});

		bool toggle(std::string text, bool& _boolean, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		bool toggle(std::string text, bool& _boolean, std::string description, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		


		template<typename NumberType>
		bool number(std::string text, NumberType& _number, NumberRange<NumberType> range, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
		template<typename NumberType>
		bool number(std::string text, NumberType& _number, NumberRange<NumberType> range, std::string description, bool vip = false, std::function<void()> action = [] {}, std::function<void()> on_hover = [] {});
	public:
		bool m_opened{};
		bool m_vip{ false };
	};

	inline gui g_gui;
}
