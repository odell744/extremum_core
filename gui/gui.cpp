#include "../common.hpp"
#include "../script_mgr/fiber_pool.hpp"
#include "../gta/player.hpp"
#include "../gta_util.hpp"
#include "gui.hpp"
#include "../logger.hpp"
#include "../memory/module.hpp"
#include "../memory/pattern.hpp"
#include "../natives/natives.hpp"
#include "../pointers.hpp"
#include "../renderer/renderer.hpp"
#include "../script/script.hpp"
#include "../pointers.hpp"
#include "../gta/joaat.hpp"

#include "../StackWalker/StackWalker.h"


// Menu Includes
#include "../menu/selfoptions.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

char buffer[0x24]{};

namespace extremum::core
{
	void gui::init_textures()
	{
		LOG_INFO("loading textures");

		static auto load_texture = [](std::filesystem::path texture_path) -> ID3D11ShaderResourceView* {
			ID3D11ShaderResourceView* my_texture = NULL;
			if (D3DX11CreateShaderResourceViewFromFile(g_renderer->m_d3d_device.Get(), texture_path.string().c_str(), NULL, NULL, &my_texture, NULL) != S_OK) {
				LOG_INFO("Failed to initialize texture {}\n", texture_path.string());
				return nullptr;
			}
			return my_texture;
		};

		static auto load_textures_from_path = [](std::filesystem::path dir_path, std::vector<ID3D11ShaderResourceView*>& container, std::string prefix, int fmt_int = 5, int offset = 0) {
			int counter_files{ 0 };
			for (const auto& file : std::filesystem::directory_iterator(dir_path))
				++counter_files;
			for (int i = 0 + offset; i <= counter_files; ++i) {
				std::filesystem::path current_ = fmt::format("{0}\\{1}{2:0>{3}}.png", dir_path.string().c_str(), prefix, i, fmt_int);
				if (std::filesystem::exists(current_)) {
					ID3D11ShaderResourceView* _current = load_texture(current_);
					if (_current != nullptr)
						container.push_back(_current);
				}
			}
		};
		static auto load_textures_map_from_path = [](std::filesystem::path dir_path, std::vector<std::string> name_container, std::map<std::string, ID3D11ShaderResourceView*>& container) {
			for (auto& name : name_container) {
				std::filesystem::path current_ = fmt::format("{0}\\{1}.png", dir_path.string(), name);
				if (std::filesystem::exists(current_)) {
					ID3D11ShaderResourceView* _current = load_texture(current_);
					if (_current != nullptr)
						container.insert(std::pair(name, _current));
				}
			}
		};

		std::filesystem::path standard = "C:\\EXFiles\\UIDesign\\UIDesign\\BGDesign\\Standard";
		std::filesystem::path exclusive = "C:\\EXFiles\\UIDesign\\UIDesign\\BGDesign\\Exclusive";
		std::filesystem::path vip = "C:\\EXFiles\\UIDesign\\UIDesign\\BGDesign\\Vip";
		std::filesystem::path toggle = "C:\\EXFiles\\UIDesign\\UIDesign\\Elements\\Toggle";
		std::filesystem::path icons = "C:\\EXFiles\\UIDesign\\UIDesign\\Elements\\Icons";
		std::filesystem::path menu_icons = "C:\\EXFiles\\UIDesign\\UIDesign\\Elements\\MenuIcons";


		load_textures_from_path(standard, m_header_standard, "Standard_", 5, 1);
		load_textures_from_path(exclusive, m_header_exclusive, "EXCLUSIVE_", 5);
		load_textures_from_path(vip, m_header_vip, "VIP_", 5);
		load_textures_from_path(toggle, m_toggle, "tumb_", 5);
		load_textures_map_from_path(menu_icons, m_menu_icons_names, m_menu_icons);

		m_header = &m_header_standard;

		LOG_INFO("textures loaded");
	}
	void gui::dx_init()
	{
		auto &style = ImGui::GetStyle();
		style.WindowPadding = { 0.f, 0.f };
		style.PopupRounding = 0.f;
		style.FramePadding = { 0.f, 0.f };
		style.ItemSpacing = { 0.f, 0.f };
		style.ItemInnerSpacing = { 6.f, 6.f };
		style.TouchExtraPadding = { 0.f, 0.f };
		style.IndentSpacing = 21.f;
		style.ScrollbarSize = 15.f;
		style.GrabMinSize = 8.f;
		style.WindowBorderSize = 0.f;
		style.ChildBorderSize = 0.f;
		style.PopupBorderSize = 1.f;
		style.FrameBorderSize = 0.f;
		style.TabBorderSize = 0.f;
		style.WindowRounding = 0.f;
		style.ChildRounding = 0.f;
		style.FrameRounding = 0.f;
		style.ScrollbarRounding = 0.f;
		style.GrabRounding = 0.f;
		style.TabRounding = 0.f;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.ButtonTextAlign = { 0.1f, 0.5f };
		style.DisplaySafeAreaPadding = { 3.f, 3.f };

		auto &colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.6f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.06f, 0.06f, 0.06f, 0.05f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.06f, 0.06f, 0.06f, 0.05f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.06f, 0.06f, 0.05f);
		colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(1.f, 1.f, 1.f, 1.f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
		colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
		colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
	void gui::dx_on_tick()
	{
		ImGuiWindow* m_header_window{};
		ImGuiWindow* m_option_window{};
		ImGuiWindow* m_scroller_window{};
		ImGuiWindow* m_footer_window{};

		static ImVec2 header_anim_size(400.f, 110.f);
		static ImVec2 header_size(400.f, 135.f);
		static ImVec2 menu_size(400.f, button_height * button_visible);
		static ImVec2 fotter_size(400.f, 50.f);
		ImVec2 header_pos;
#pragma region HEADER_WINDOW
		ImGui::SetNextWindowSize(header_size);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, ImVec4(0.06f, 0.06f, 0.06f, 0.9f));
		if (ImGui::Begin("header", nullptr, ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar))
		{
			m_header_window = ImGui::GetCurrentWindow();
			header_pos = m_header_window->Pos;

			static bool timer_reset = true;
			static std::chrono::system_clock::time_point timer_Start;
			static int idleFrame = 0;
			if (m_header != nullptr) {
				if (m_header->size() > 0) {
					m_header_window->DrawList->AddImage((ImTextureID)m_header->at(idleFrame), ImVec2(header_pos.x, header_pos.y), ImVec2(header_pos.x + header_anim_size.x, header_pos.y + header_anim_size.y));
					std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_Start;
					if (elapsed_seconds.count() >= 0.019)
					{
						idleFrame++;
						if (idleFrame == m_header->size() - 1)
							idleFrame = 0;
						timer_Start = std::chrono::system_clock::now();
					}
				}
			}
			std::string text_ = TRANSLATE(m_subtitles[m_current_menu]);
			auto text_size = ImGui::CalcTextSize(text_.c_str());
			ImGui::SetCursorPos({ header_size.x / 2 - text_size.x / 2, header_size.y - (header_size.y - header_anim_size.y) / 2 - text_size.y / 2});
			ImGui::Text(text_.c_str());
		}
		ImGui::PopStyleColor();
		ImGui::End();
#pragma endregion
#pragma region OPTION_WINDOW
		ImGui::SetNextWindowPos(ImVec2(header_pos.x, header_pos.y + header_size.y));
		ImGui::SetNextWindowSize(menu_size_visible);

		static bool test_bool{ false };
		static bool test_bool2{ false };
		static bool test_bool3{ true };
		static bool test_bool4{ true };
		static bool test_bool5{ true };
		if (ImGui::Begin("console", nullptr, ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar))
		{
			m_option_window = ImGui::GetCurrentWindow();

			static float slider_y{ 0.f };
			ImVec2 p_min_slider{ m_option_window->Pos.x, m_option_window->Pos.y + slider_y };
			ImVec2 p_max_slider{ m_option_window->Pos.x + m_option_window->Size.x, m_option_window->Pos.y + slider_y + button_height};
			slide_to(slider_y, (m_current_button - 1) * button_height - ImGui::GetScrollY(), 15);

			m_option_window->DrawList->AddRectFilled(p_min_slider, p_max_slider, IM_COL32(255, 255, 255, 255));

			switch (m_current_menu)
			{
			case extremum::core::mainmenu: {
				menu_option("selfOption", "mp_specitem_ped", extremum::core::self_options);
				menu_option("protMenu", "mp_specitem_safe", extremum::core::protection_menu);
				menu_option("onlinePlayers", "survival", extremum::core::online_players);
				menu_option("networkOptions", "team_deathmatch", extremum::core::network_options);
				menu_option("weaponOptions", "ammo_pickup", extremum::core::weapon_options);
				menu_option("teleportOptions", "shooting_range", extremum::core::teleport_options);
				menu_option("vehicleSpawnOptions", "mp_specitem_car", extremum::core::vehicle_spawn_options);
				menu_option("vehicleOptions", "vehicle_deathmatch", extremum::core::vehicle_options);
				menu_option("moneyOptions", "mp_specitem_cash", extremum::core::money_options);
				menu_option("recoveryOptions", "mp_alerttriangle", extremum::core::recovery_options);
				menu_option("spawnOptions", "mp_specitem_package", extremum::core::spawn_options);
				menu_option("worldOptions", "mp_specitem_remote", extremum::core::world_options);
				menu_option("extremumExclusive", "custom_mission", extremum::core::extremum_exclusive);
				menu_option("gameplayOptions", "mp_specitem_meth", extremum::core::gameplay_options);
				menu_option("menuSettings", "mp_specitem_keycard", extremum::core::menu_settings);
			} break;
			case extremum::core::self_options:									extremum::menu::selfoptions::submenu_selfoptions(); break;
			case extremum::core::personal_globals:								extremum::menu::selfoptions::submenu_personal_globals(); break;
			case extremum::core::wanted_level_options:							extremum::menu::selfoptions::submenu_wanted_level_options(); break;
			case extremum::core::noclip_options:								extremum::menu::selfoptions::submenu_noclip_options(); break;
			case extremum::core::freecamera_options:							extremum::menu::selfoptions::submenu_freecamera_options(); break;
			case extremum::core::wardrobe_options:								extremum::menu::selfoptions::submenu_wardrobe_options(); break;
			case extremum::core::ride_options:									extremum::menu::selfoptions::submenu_ride_options(); break;
			case extremum::core::vision_effects_options:						extremum::menu::selfoptions::submenu_vision_effects_options(); break;
			case extremum::core::models_changer:								extremum::menu::selfoptions::submenu_models_changer(); break;
			case extremum::core::animations:									extremum::menu::selfoptions::submenu_animations(); break;
			case extremum::core::scenarios:										extremum::menu::selfoptions::submenu_scenarios(); break;
			case extremum::core::walk_styles:									extremum::menu::selfoptions::submenu_walk_styles(); break;
			case extremum::core::particle_effects:								extremum::menu::selfoptions::submenu_particle_effects(); break;
			case extremum::core::cutscenes:										extremum::menu::selfoptions::submenu_cutscenes(); break;
			case extremum::core::auto_functions:								extremum::menu::selfoptions::submenu_auto_functions(); break;
			case extremum::core::local_name_changer:							extremum::menu::selfoptions::submenu_local_name_changer(); break;
			default:
				button("not available", "current menu will be\navailable in future");
				break;
			}

			float button_level = m_current_button * button_height - button_height;
			bool upper = m_option_window->Scroll.y > button_level;
			bool lower = m_option_window->Scroll.y + button_height * button_visible <= button_level;
			if (upper || lower)
				m_option_window->Scroll = { ImGui::GetScrollX(), m_current_button * button_height - button_height };
			if (ImGui::IsWindowHovered() || upper || lower)
				ImGui::SetNextWindowScroll({ImGui::GetScrollX(), ImGui::GetScrollY()});
		}
		ImGui::End();
#pragma endregion
#pragma region DRAWING_SCROLLER
		if (m_button_counter * button_height > menu_size_visible.y) {
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.06f, 0.06f, 0.06f, 0.05f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 1.f, 1.f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.41f, 0.41f, 0.41f, 0.87f));
			ImGui::SetNextWindowPos(ImVec2(header_pos.x - 45.f, header_pos.y + header_size.y));
			ImGui::SetNextWindowSize(ImVec2(0.001f, menu_size_visible.y));
			if (ImGui::Begin("scroller", nullptr, ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoTitleBar)) {
				m_scroller_window = ImGui::GetCurrentWindow();
				for (int i = 0; i < m_button_counter; ++i)
					ImGui::Button("", ImVec2(0.001f, button_height));

				if (ImGui::IsWindowHovered())
					m_option_window->Scroll = { ImGui::GetScrollX(), ImGui::GetScrollY() };
			}
			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}
#pragma endregion
#pragma region DRAWING_FOTTER
		ImGui::SetNextWindowPos(ImVec2(header_pos.x, header_pos.y + header_size.y + menu_size_visible.y));
		ImGui::SetNextWindowSize(fotter_size);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, ImVec4(0.06f, 0.06f, 0.06f, 0.9f));
		if (ImGui::Begin("footer", nullptr, ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar))
		{
			std::string counter = fmt::format("{}/{}", m_current_button, m_button_counter);
			std::string version = fmt::format("v{}", EXTREMUM_CHEAT_VER_TXT);
			std::string developer = fmt::format("{}", EXTREMUM_DEVELOPER);

			auto counter_size = ImGui::CalcTextSize(counter.c_str());
			auto version_size = ImGui::CalcTextSize(version.c_str());
			auto developer_size = ImGui::CalcTextSize(developer.c_str());

			auto start_pos_x = ImGui::GetCursorPosX();
			auto start_pos_y = ImGui::GetCursorPosY();

			/// DEVELOPER
			{
				ImGui::SetCursorPosX(start_pos_x + 10.f);
				ImGui::SetCursorPosY(start_pos_y + fotter_size.y / 2 - developer_size.y / 2);
				ImGui::Text(developer.c_str());
			}
			/// COUNTER
			{
				ImGui::SetCursorPosX(start_pos_x + fotter_size.x / 2 - counter_size.x / 2);
				ImGui::SetCursorPosY(start_pos_y + fotter_size.y / 2 - counter_size.y / 2);
				ImGui::Text(counter.c_str());
			}
			/// VERSION
			{
				ImGui::SetCursorPosX(start_pos_x + fotter_size.x - 10.f - version_size.x);
				ImGui::SetCursorPosY(start_pos_y + fotter_size.y / 2 - version_size.y / 2);
				ImGui::Text(version.c_str());
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
#pragma endregion
		m_pressed = m_back = m_left_pressed = m_right_pressed = m_up_pressed = m_down_pressed = false;
		if (m_button_counter > button_visible && m_opened)
			slide_to(menu_size_visible.y, menu_size.y);
		else if (m_opened)
			slide_to(menu_size_visible.y, button_height * m_button_counter);
		if (!m_block_controlls)
			check_controlls();
	}

	void gui::script_init()
	{
	}
	void gui::script_on_tick()
	{
		if (g_gui.m_opened)
		{
			CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(1);
		}
	}
	void gui::script_func()
	{
		g_gui.script_init();
		while (true)
		{
			g_gui.script_on_tick();
			script::get_current()->yield();
		}
	}

	void gui::background_overlay()
	{
		static float m_bg_alpha{ 0.f };
		static float m_font_alpha{ 0.f };
		static float m_font_scale{ 0.1f };

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, m_bg_alpha));

		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
		if (ImGui::Begin("##BackgroundOverlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs))
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (m_startup_anim) {
				ImGui::PushFont(g_renderer->m_font_startup);

				static bool line_inited{ false };
				static ImVec2 fl_p_min;
				static ImVec2 fl_p_max;
				static ImVec2 fl_p_max_end;
				static ImVec2 text_size;

				static std::chrono::system_clock::time_point timer_start;
				static bool timer_inited{ false };


				switch (m_startup_anim_pos)
				{
				case extremum::core::BG_IN: {
					if (m_bg_alpha != 0.5f)
						slide_to(m_bg_alpha, 0.5f);
					else
						m_startup_anim_pos = extremum::core::TEXT_IN;
				} break;
				case extremum::core::TEXT_IN: {
					if (m_font_alpha != 1.f, m_font_scale != 1.f) {
						slide_to(m_font_alpha, 1.f);
						slide_to(m_font_scale, 1.f);
					}
					else
						m_startup_anim_pos = extremum::core::UNDER_LINE_IN;
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, m_font_alpha));
					ImGui::SetWindowFontScale(m_font_scale);
					text_size = ImGui::CalcTextSize("EXTREMUM");
					ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2 - text_size.x / 2, io.DisplaySize.y / 2 - text_size.y / 2));
					ImGui::Text("EXTREMUM");
					ImGui::PopStyleColor();
				} break;
				case extremum::core::UNDER_LINE_IN: {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
					ImGui::SetWindowFontScale(1.f);
					text_size = ImGui::CalcTextSize("EXTREMUM");
					ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2 - text_size.x / 2, io.DisplaySize.y / 2 - text_size.y / 2));
					ImGui::Text("EXTREMUM");
					ImGui::PopStyleColor();
					if (!line_inited) {
						fl_p_min = fl_p_max = ImVec2(io.DisplaySize.x / 2 - text_size.x / 2, io.DisplaySize.y / 2 + text_size.y / 2 + 30.f);
						fl_p_max_end = ImVec2(io.DisplaySize.x / 2 + text_size.x / 2, io.DisplaySize.y / 2 + text_size.y / 2 + 30.f);
						line_inited = true;
					}
					window->DrawList->AddLine(fl_p_min, fl_p_max, IM_COL32(255, 255, 255, 255), 10.f);
					if (fl_p_max.x != fl_p_max_end.x)
						slide_to(fl_p_max.x, fl_p_max_end.x);
					else
						m_startup_anim_pos = extremum::core::UNDER_LINE_OUT;
				} break;
				case extremum::core::TEXT_WAIT: {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
					ImGui::SetWindowFontScale(1.f);
					text_size = ImGui::CalcTextSize("EXTREMUM");
					ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2 - text_size.x / 2, io.DisplaySize.y / 2 - text_size.y / 2));
					ImGui::Text("EXTREMUM");
					ImGui::PopStyleColor();
					window->DrawList->AddLine(fl_p_min, fl_p_max, IM_COL32(255, 255, 255, 255), 10.f);
					if (!timer_inited) {
						timer_start = std::chrono::system_clock::now();
						timer_inited = true;
					}
					std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_start;
					if (elapsed_seconds.count() >= 1.5f)
						m_startup_anim_pos = extremum::core::UNDER_LINE_OUT;
				} break;
				case extremum::core::UNDER_LINE_OUT: {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
					ImGui::SetWindowFontScale(1.f);
					text_size = ImGui::CalcTextSize("EXTREMUM");
					ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2 - text_size.x / 2, io.DisplaySize.y / 2 - text_size.y / 2));
					ImGui::Text("EXTREMUM");
					ImGui::PopStyleColor();
					window->DrawList->AddLine(fl_p_min, fl_p_max, IM_COL32(255, 255, 255, 255), 10.f);
					if (fl_p_min.x != fl_p_max_end.x)
						slide_to(fl_p_min.x, fl_p_max_end.x);
					else
						m_startup_anim_pos = extremum::core::TEXT_OUT;
				} break;
				case extremum::core::TEXT_OUT: {
					if (m_font_alpha != 0.f, m_font_scale != 0.1f) {
						slide_to(m_font_alpha, 0.f);
						slide_to(m_font_scale, 0.1f);
					}
					else
						m_startup_anim_pos = extremum::core::BG_OUT;
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, m_font_alpha));
					ImGui::SetWindowFontScale(m_font_scale);
					text_size = ImGui::CalcTextSize("EXTREMUM");
					ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2 - text_size.x / 2, io.DisplaySize.y / 2 - text_size.y / 2));
					ImGui::Text("EXTREMUM");
					ImGui::PopStyleColor();
				} break;
				case extremum::core::BG_OUT: {
					ImGui::SetWindowFontScale(1.f);
					if (m_bg_alpha != 0.f)
						slide_to(m_bg_alpha, 0.f);
					else
						m_startup_anim = false;
				} break;
				default:
					m_startup_anim = false;
					break;
				}
				ImGui::PopFont();
			}
			else {

			}
			ImGui::End();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	/// <summary>
	/// Drawing functions
	/// </summary>
	bool gui::button(std::string text, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		++m_button_counter;

		std::string text_ = TRANSLATE(text);
		if (vip) text_ = fmt::format("{} [VIP]", TRANSLATE(text));

		bool return_bool{ false };
		if (m_current_button == m_button_counter) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.f, 0.f, 1.f));
			if (ImGui::Button(text_.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), button_height)) || m_pressed) {
				if (!vip || vip && m_vip) {
					return_bool = true;
					g_fiber_pool->queue_job(action);
				}
			}
			ImGui::PopStyleColor();
		}
		else
			ImGui::Button(text_.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), button_height));

		if (ImGui::IsItemHovered()) {
			m_current_button = m_button_counter;
			m_hovered_id = ImGui::GetHoveredID();
			if (!vip || vip && m_vip) 
				on_hover();
		}
		return return_bool;
	}
	bool gui::button(std::string text, std::string description, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		bool ret = button(text, vip, action, on_hover);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(description.c_str());
		return ret;
	}

	bool gui::menu_option(std::string text, sub_menus next_level, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		ImGuiWindow* m_current = ImGui::GetCurrentWindow();
		bool ret = button(text, vip, action, on_hover);

		float height = button_height * 0.6;
		float width = height;
		float offset_y = (button_height - height) / 2;
		auto current_pos = m_current->Pos;
		if (m_menu_icons.find("arrow") != m_menu_icons.end())
			m_current->DrawList->AddImage(
				(ImTextureID)m_menu_icons["arrow"], 
				ImVec2(current_pos.x + 400.f - width * 1.5, current_pos.y + (ImGui::GetCursorPosY() - ImGui::GetScrollY()) - offset_y - height),
				ImVec2(current_pos.x + 400.f - width * 0.5, current_pos.y + (ImGui::GetCursorPosY() - ImGui::GetScrollY()) - offset_y));

		if (ret) {
			move_menu(next_level);
			if (m_subtitles.find(next_level) == m_subtitles.end())
				m_subtitles.insert(std::pair(next_level, text));
		}
		return ret;
	}
	bool gui::menu_option(std::string text, sub_menus next_level, std::string description, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		bool ret = menu_option(text, next_level, vip, action, on_hover);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(description.c_str());
		return ret;
	}
	bool gui::menu_option(std::string text, std::string icon, sub_menus next_level, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		bool ret{ false };
		if (m_menu_icons.find(icon) != m_menu_icons.end()) {
			ImGuiWindow* m_current = ImGui::GetCurrentWindow();
			ret = menu_option("", next_level, vip, action, on_hover);
			if (ret)
				if (m_subtitles.find(next_level) != m_subtitles.end())
					m_subtitles[next_level] = text;
			float height = button_height * 0.7;
			float width = height;
			float offset_y = (button_height - height) / 2;
			auto current_pos = m_current->Pos;

			m_current->DrawList->AddImage(
				(ImTextureID)m_menu_icons[icon],
				ImVec2(current_pos.x + width * 1.5, current_pos.y + (ImGui::GetCursorPosY() - ImGui::GetScrollY()) - offset_y - height),
				ImVec2(current_pos.x + width * 0.5, current_pos.y + (ImGui::GetCursorPosY() - ImGui::GetScrollY()) - offset_y));

			auto cursor_pos = ImGui::GetCursorPos();
			auto text_size = ImGui::CalcTextSize(text.c_str());
			ImGui::SetCursorPos({ width * 2, ImGui::GetCursorPosY() - button_height / 2 - text_size.y / 2});

			std::string text_ = TRANSLATE(text);
			if (vip) text_ = fmt::format("{} [VIP]", TRANSLATE(text));

			if (m_current_button == m_button_counter)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.01f, 0.01f, 0.01f, 1.00f));
			ImGui::Text(text_.c_str());
			if (m_current_button == m_button_counter)
				ImGui::PopStyleColor();
			ImGui::SetCursorPos(cursor_pos);
		} else
			ret = menu_option(text, next_level, vip, action, on_hover);
		return ret;
	}
	bool gui::menu_option(std::string text, std::string icon, sub_menus next_level, std::string description, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		bool ret = menu_option(text, icon, next_level, vip, action, on_hover);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(description.c_str());
		return ret;
	}
	
	bool gui::toggle(std::string text, bool &_boolean, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		ImGuiWindow* m_current = ImGui::GetCurrentWindow();
		static std::map<std::uint32_t, std::uint32_t> toggle_position;
		static std::map<std::uint32_t, std::chrono::system_clock::time_point> timers;
		bool ret = button(text, vip, action, on_hover);

		std::uint32_t hash = rage::joaat(text.c_str());

		if (toggle_position.find(hash) == toggle_position.end())
			toggle_position.insert(std::pair(hash, _boolean ? 0 : m_toggle.size() - 1));
		if (timers.find(hash) == timers.end())
			timers.insert(std::pair(hash, std::chrono::system_clock::now()));

		if ((_boolean && toggle_position[hash] > 0) || (!_boolean && toggle_position[hash] < m_toggle.size() - 1))
		{
			std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timers[hash];
			if (elapsed_seconds.count() >= 0.019)
			{
				if (_boolean && toggle_position[hash] > 0)
					--toggle_position[hash];
				else if (!_boolean && toggle_position[hash] < m_toggle.size() - 1)
					++toggle_position[hash];
				timers[hash] = std::chrono::system_clock::now();
			}
		}

		float height = button_height * 0.8;
		float width = height * 1.5;
		float offset_y = (button_height - height) / 2;

		auto current_pos = m_current->Pos;

		auto p_min = ImVec2(current_pos.x + 400.f - width * 1.5, current_pos.y + (ImGui::GetCursorPosY() - ImGui::GetScrollY()) - offset_y - height);
		auto p_max = ImVec2(current_pos.x + 400.f - width * 0.5, current_pos.y + (ImGui::GetCursorPosY() - ImGui::GetScrollY()) - offset_y);

		m_current->DrawList->AddImage((ImTextureID)m_toggle.at(toggle_position[hash]), p_min, p_max);
		
		if (ret)
			_boolean ^= 1;
		return ret;
	}
	bool gui::toggle(std::string text, bool& _boolean, std::string description, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		bool ret = toggle(text, vip, _boolean, action, on_hover);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(description.c_str());
		return ret;
	}

	template<typename NumberType>
	bool gui::number(std::string text, NumberType& _number, NumberRange<NumberType> range, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		std::string text_ = TRANSLATE(text);
		if (vip) text_ = fmt::format("{} [VIP]", TRANSLATE(text));

		++m_button_counter;
		// Get Variables
		ImGuiWindow* m_current = ImGui::GetCurrentWindow();
		auto& io = ImGui::GetIO();
		auto mouse_pos = ImGui::GetMousePos();
		auto text_size = ImGui::CalcTextSize(text.c_str());
		auto cursor_pos = ImGui::GetCursorPos();
		auto cursor_pos_end = ImVec2(cursor_pos.x, cursor_pos.y + button_height);
		
		auto p_min = ImVec2(m_current->Pos.x + cursor_pos.x, m_current->Pos.y + cursor_pos.y - ImGui::GetScrollY());
		auto p_max = ImVec2(m_current->Pos.x + m_current->Size.x, m_current->Pos.y + cursor_pos_end.y - ImGui::GetScrollY());

		if (mouse_pos.x > p_min.x && mouse_pos.x < p_max.x && mouse_pos.y > p_min.y && mouse_pos.y < p_max.y)
		{
			m_current_button = m_button_counter;
		}
		bool on_this = m_current_button == m_button_counter;
		if (on_this) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.f, 0.f, 1.f));
		}
		// Draw Text
		{
			ImGui::SetCursorPos({ button_height * 0.8f, cursor_pos.y + button_height / 2 - text_size.y / 2 });
			ImGui::Text(text_.c_str());
		}
		// Draw number slider
		{
			ImGui::SetNextItemWidth(menu_size_visible.x / 2 - button_height * 0.8f);
			ImGui::SetCursorPos({ menu_size_visible.x / 2, cursor_pos_end.y - button_height / 4 * 3 });
			if (std::is_integral<NumberType>::value)
				ImGui::DragInt(fmt::format("\t\t{}", text).c_str(), (int*)&_number, (int)range.step, (int)range.min, (int)range.max, "< %d >");
			if (std::is_floating_point<NumberType>::value)
				ImGui::DragFloat(fmt::format("\t\t{}", text).c_str(), (float*)&_number, (float)range.step, (float)range.min, (float)range.max, "< %.2f >");
			ImGui::SetCursorPos(cursor_pos_end);
		}
		if (on_this) {
			ImGui::PopStyleColor();
		}
		return (on_this && (m_pressed));
	}
	template<typename NumberType>
	bool gui::number(std::string text, NumberType& _number, NumberRange<NumberType> range, std::string description, bool vip, std::function<void()> action, std::function<void()> on_hover)
	{
		bool ret = number<NumberType>(text, _number, range, action, on_hover);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(description.c_str());
		return ret;
	}

	void gui::check_controlls()
	{
		static std::chrono::system_clock::time_point up_down_timer = std::chrono::system_clock::now();
		static std::chrono::system_clock::time_point left_right_timer = std::chrono::system_clock::now();
		static std::chrono::system_clock::time_point press_timer = std::chrono::system_clock::now();

		std::chrono::duration<double> up_down_elapsed = std::chrono::system_clock::now() - up_down_timer;
		std::chrono::duration<double> left_right_elapsed = std::chrono::system_clock::now() - left_right_timer;
		std::chrono::duration<double> press_elapsed = std::chrono::system_clock::now() - press_timer;

		if (up_down_elapsed.count() >= 0.100)
		{
			bool _up = iskeypressed(VK_NUMPAD8) || iskeypressed(VK_UP);
			bool _down = iskeypressed(VK_NUMPAD2) || iskeypressed(VK_DOWN);
			if (_up || _down) {
				if (_up)
					m_up_pressed = true;
				else if (_down)
					m_down_pressed = true;

				up_down_timer = std::chrono::system_clock::now();
			}
		}
		if (left_right_elapsed.count() >= 0.100)
		{
			bool _left = iskeypressed(VK_NUMPAD4) || iskeypressed(VK_LEFT);
			bool _right = iskeypressed(VK_NUMPAD6) || iskeypressed(VK_RIGHT);
			if (_left || _right) {
				if (_left)
					m_left_pressed = true;
				else if (_right)
					m_right_pressed = true;

				left_right_timer = std::chrono::system_clock::now();
			}
		}
		if (press_elapsed.count() >= 0.100)
		{
			bool _press = iskeypressed(VK_NUMPAD5) || iskeypressed(VK_RETURN);
			bool _back = iskeypressed(VK_NUMPAD0) || iskeypressed(VK_BACK);

			if (_press || _back) {
				if (_press)
					m_pressed = true;
				else if (_back)
					m_back = true;

				press_timer = std::chrono::system_clock::now();
			}
		}

		if (m_opened) {
			if (m_up_pressed) {
				if (m_current_button == 1) m_current_button = m_button_counter;
				else --m_current_button;
			}
			else if (m_down_pressed) {
				if (m_current_button == m_button_counter) m_current_button = 1;
				else ++m_current_button;
			}
			else if (m_back) {
				back_menu();
			}
		}
		m_button_counter = 0;
	}

	template<typename _Ty>
	void gui::slide_to(_Ty& x, _Ty y, int speed)
	{
		if (x < y && y - x > 0.01) {
			float z = y - x;
			x += 0.01 + z / speed;
		}
		else if (x > y && x - y > 0.01) {
			float z = x - y;
			x -= (0.01 + z / speed);
		}
		else x = y;
	}
	void gui::dx_on_disable()
	{
		slide_to(menu_size_visible.y, 0.f);
	}

	void gui::move_menu(sub_menus level)
	{
		m_menu_levels[m_menu_level] = m_current_menu;
		m_menu_option_level[m_menu_level] = m_current_button;
		++m_menu_level;
		m_current_menu = level; 
		m_current_button = 1;
	}
	void gui::back_menu()
	{
		if (m_menu_level > 0) {
			--m_menu_level;
			m_current_menu = m_menu_levels[m_menu_level];
			m_current_button = m_menu_option_level[m_menu_level];
		}
		else if (m_opened)
			m_opened = false;
	}
}
