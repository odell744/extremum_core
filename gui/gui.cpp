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

#define START_MENU m_menu
#define END_MENU ;
#define ADD_SUBMENU(name, desc) .add_submenu(name, desc)
#define END_SUBMENU .end_submenu()
#define ADD_OPTION(name, desc) .add_option(name, desc)
#define END_OPTION .end_option()
#define ADD_HANDLER(handler) .add_handler(handler)
#define ADD_SETTING(name, desc) .add_setting(name, desc)
#define END_SETTING .end_setting()
#define ADD_ACTION_HANDLER(action) ADD_HANDLER(std::make_unique<action_handler_t>(action))
#define ADD_BOOL_HANDLER(var) ADD_HANDLER(std::make_unique<bool_handler_t>(var))
#define ADD_INT32_HANDLER(var) ADD_HANDLER(std::make_unique<int32_handler_t>(var))
#define ADD_UINT32_HANDLER(var) ADD_HANDLER(std::make_unique<uint32_handler_t>(var))
#define ADD_INT64_HANDLER(var) ADD_HANDLER(std::make_unique<int64_handler_t>(var))
#define ADD_FLOAT_HANDLER(var) ADD_HANDLER(std::make_unique<float_handler_t>(var))
#define ADD_DOUBLE_HANDLER(var) ADD_HANDLER(std::make_unique<double_handler_t>(var))


char buffer[0x24]{};

namespace extremum::core
{
	template<typename Handler>
	inline option_setting& option_setting::add_handler(std::unique_ptr<Handler>&& handler)
	{
		m_option_handler = std::move(handler);
		return *this;
	}
	template<typename Handler>
	inline option& option::add_handler(std::unique_ptr<Handler>&& handler)
	{
		m_option_handler = std::move(handler);
		return *this;
	}

	menu::menu() {};
	submenu::submenu(const std::uint32_t& id, const std::string& name, const std::string description, menu& handler)
		:
		m_id(id), m_name(name), m_description(description), m_handler_menu(handler)
	{
		LOG_DEBUG("submenu created [id: {}, name: {}, desc: {}]", m_id, m_name, m_description);
	};
	option::option(const std::uint32_t& id, const std::string& name, const std::string& description, submenu& handler)
		:
		m_id(id), m_name(name), m_description(description), m_handler_submenu(handler)
	{
		LOG_DEBUG("option created [id: {}, name: {}, desc: {}, handler_submenu_id: {}]", m_id, m_name, m_description, handler.id());
	};
	option_setting::option_setting(const std::uint32_t& id, const std::string& name, const std::string& description, option& handler)
		:
		m_id(id), m_name(name), m_description(description), m_handler_option(handler)
	{
		LOG_DEBUG("option_setting created [id: {}, name: {}, desc: {}, handler_option_id: {}]", m_id, m_name, m_description, handler.id());
	};

	bool option_setting::is_vip() { return m_is_vip; };
	bool option::is_vip() { return m_is_vip; };
	bool submenu::is_vip() { return m_is_vip; };

	option_setting& option_setting::set_vip() { m_is_vip = true; return *this; };
	option& option::set_vip() { m_is_vip = true; return *this; };
	submenu& submenu::set_vip() { m_is_vip = true; return *this; };

	option& option_setting::end_setting() { return m_handler_option; };
	submenu& option::end_option() { return m_handler_submenu; };
	menu& submenu::end_submenu() { return m_handler_menu; };

	option_setting& option::add_setting(const std::string& name, const std::string& description)
	{
		m_settings.push_back(new option_setting(m_settings.size(), name, description, *this));
		return *m_settings.back();
	}
	option& submenu::add_option(const std::string& name, const std::string& description)
	{
		m_options.push_back(new option(m_options.size(), name, description, *this));
		return *m_options.back();
	}
	
	submenu& menu::add_submenu(const std::string& name, const std::string& description)
	{
		m_submenus.push_back(new submenu(m_submenus.size(), name, description, *this));
		return *m_submenus.back();
	}

	

	void gui::init_textures()
	{
		static auto load_texture = [](std::filesystem::path texture_path) -> ID3D11ShaderResourceView* {
			ID3D11ShaderResourceView* my_texture = NULL;
			if (D3DX11CreateShaderResourceViewFromFileA(g_renderer->m_d3d_device.Get(), texture_path.string().c_str(), NULL, NULL, &my_texture, NULL) != S_OK) {
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
	}
	void gui::dx_init()
	{
		auto& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		auto& style = ImGui::GetStyle();
		{
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

			auto& colors = style.Colors;
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
		START_MENU
			ADD_SUBMENU("mainMenu", "mainMenuDesc")

				ADD_OPTION("versionText", "versionTextDsc")
					ADD_ACTION_HANDLER([] {})
				END_OPTION

				ADD_OPTION("authorText", "authorTextDsc")
					ADD_ACTION_HANDLER([] {})
				END_OPTION

			END_SUBMENU

			ADD_SUBMENU("playerOptions", "playerOptionsDsc")

				ADD_OPTION("option1", "dsc")
					ADD_ACTION_HANDLER([] {})

					ADD_SETTING("godModeSetting", "settingPlayer1Desc")
						ADD_ACTION_HANDLER([] {})
					END_SETTING
					ADD_SETTING("godModeSetting2", "settingPlayer1Desc")
						ADD_ACTION_HANDLER([] {})
					END_SETTING
				END_OPTION

				ADD_OPTION("option2", "dsc")
					ADD_ACTION_HANDLER([] {})
					ADD_SETTING("option2Setting", "settingPlayer1Desc")
						ADD_ACTION_HANDLER([] {})
					END_SETTING
				END_OPTION
				ADD_OPTION("option3", "dsc")
					ADD_ACTION_HANDLER([] {})
				END_OPTION
				ADD_OPTION("option4", "dsc")
					ADD_ACTION_HANDLER([] {})
				END_OPTION
				ADD_OPTION("option5", "dsc")
					ADD_ACTION_HANDLER([] {})
				END_OPTION
			END_SUBMENU
		END_MENU
	}
	void gui::dx_on_tick()
	try
	{
		static ImVec2 start_position{}, diff{}, draw_position{ m_pos };
		static bool window_moving{ false };
		check_controlls();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(draw_position);
		ImGui::SetNextWindowSize(ImVec2(m_options_size.x, 
								m_options_size.y + m_header_current_size.y + m_footer_current_size.y), ImGuiCond_Always);
		if (ImGui::Begin("##draw_window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoResize))
		{
			slide_to(draw_position.x, m_pos.x);
			slide_to(draw_position.y, m_pos.y);

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !window_moving) {
				auto mouse_pos = io.MousePos;
				
				if (mouse_pos.x >= draw_position.x && mouse_pos.x <= (draw_position.x + m_header_current_size.x)
					&& mouse_pos.y >= draw_position.y && mouse_pos.y <= (draw_position.y + m_header_current_size.y))
				{
					window_moving = true;
					start_position = draw_position;
					diff = ImVec2(mouse_pos.x - draw_position.x, mouse_pos.y - draw_position.y);
				}
			}
			else if (window_moving) {
				auto mouse_pos = io.MousePos;
				if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
					window_moving = false;
				}
				m_pos = ImVec2(mouse_pos.x - diff.x, mouse_pos.y - diff.y);
			}

			calculate_sizes();
			draw_header_window();
			draw_options_window();
			draw_submenus_window();
			draw_settings_window();
			draw_footer_window();
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		reset_controlls();
	}
	catch (const std::exception& exception) {
		LOG_ERROR("{}", exception.what());
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

	void gui::calculate_sizes()
	{
		m_options_pos = ImVec2{ m_header_pos.x, m_header_pos.y + m_header_current_size.y };
		m_submenus_pos.y = m_options_pos.y;
		m_settings_pos.y = m_options_pos.y;
		m_footer_pos.x = m_options_pos.x;
		m_footer_pos.y = m_options_pos.y + m_options_size.y;

		switch (m_menu.get_current_window())
		{
		case current_window::options:
		{
			if (m_option_count > 16 &&
				m_menu.get_selected_submenu().selected_id() > 15)
				m_header_expanded = false;
			else {
				m_header_expanded = true;
			}
			if (m_left_pressed) {
				m_menu.set_current_window(current_window::submenus);
			}
			else if (m_right_pressed) {
				auto settings = m_menu.get_selected_submenu().get_selected_option().get_settings();
				if (!settings.empty())
					m_menu.set_current_window(current_window::settings);
			}

			if (m_up_pressed) 
			{
				m_menu.get_selected_submenu().up();
			}
			else if (m_down_pressed)
			{
				m_menu.get_selected_submenu().down();
			}
			slide_to(m_submenus_pos.x, m_options_pos.x - m_submenus_size.x, 5);
			slide_to(m_settings_pos.x, m_options_pos.x + m_options_size.x, 5);
		} break;
		case current_window::submenus:
		{
			m_header_expanded = true;
			if (m_right_pressed) {
				m_menu.set_current_window(current_window::options);
			}

			if (m_up_pressed)
			{
				m_menu.up();
			}
			else if (m_down_pressed)
			{
				m_menu.down();
			}
			slide_to(m_submenus_pos.x, m_options_pos.x);
			slide_to(m_settings_pos.x, m_options_pos.x + m_options_size.x, 5);
		} break;
		case current_window::settings:
		{
			m_header_expanded = true;
			if (m_left_pressed) {
				m_menu.set_current_window(current_window::options);
			}

			if (m_up_pressed)
			{
				m_menu.get_selected_submenu().get_selected_option().up();
			}
			else if (m_down_pressed)
			{
				m_menu.get_selected_submenu().get_selected_option().down();
			}
			slide_to(m_submenus_pos.x, m_options_pos.x - m_submenus_size.x, 5);
			slide_to(m_settings_pos.x, m_options_pos.x + m_options_size.x - m_settings_size.x);
		} break;
		}

		if (m_header_expanded)
		{
			slide_to(m_header_current_size.x, m_header_max_size.x);
			slide_to(m_header_current_size.y, m_header_max_size.y);
		}
		else {
			slide_to(m_header_current_size.x, m_header_min_size.x);
			slide_to(m_header_current_size.y, m_header_min_size.y);
		}
	}

	void gui::draw_line(const ImVec2& p_min, const ImVec2& p_max, const ImVec4& color, float thickness)
	{
		auto window = ImGui::GetCurrentWindow();
		auto& window_pos = window->Pos;

		window->DrawList->AddLine(
			ImVec2(window_pos.x + p_min.x, window_pos.y + p_min.y),
			ImVec2(window_pos.x + p_max.x, window_pos.y + p_max.y),
			ImColor(color),
			thickness
		);
	}
	void gui::draw_rect(const ImVec2& pos, const ImVec2& size, const ImVec4& color)
	{
		auto window = ImGui::GetCurrentWindow();
		auto& window_pos = window->Pos;

		window->DrawList->AddRectFilled(
			ImVec2(window_pos.x + pos.x, window_pos.y + pos.y),
			ImVec2(window_pos.x + pos.x + size.x, window_pos.y + pos.y + size.y),
			ImColor(color),
			0.f,
			ImDrawCornerFlags_::ImDrawCornerFlags_None
		);
	}
	void gui::draw_rect_rounded(const ImVec2& pos, const ImVec2& size, const ImVec4& color, float round)
	{
		auto window = ImGui::GetCurrentWindow();
		auto& window_pos = window->Pos;

		window->DrawList->AddRectFilled(
			ImVec2(window_pos.x + pos.x, window_pos.y + pos.y),
			ImVec2(window_pos.x + pos.x + size.x, window_pos.y + pos.y + size.y),
			ImColor(color),
			round);
	}

	void gui::draw_text(const std::string_view& text, const ImVec2& pos, const ImVec2& size, const ImVec4& color)
	{
		auto window = ImGui::GetCurrentWindow();

		auto text_size = ImGui::CalcTextSize(text.data());
		ImGui::SetCursorPos({ pos.x + 10.f, pos.y + size.y / 2 - text_size.y / 2});
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text(text.data());
		ImGui::PopStyleColor();
	}

	ImVec4 gui::color_from_hex(std::uint32_t value, float alpha)
	{
		return ImVec4(((value & 0xFF0000) >> 16) / 255.f, ((value & 0x00FF00) >> 8) / 255.f, ((value & 0x0000FF) >> 0) / 255.f, alpha);
	}

	void gui::draw_footer_window()
	{
		draw_rect(m_footer_pos, m_footer_current_size, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));

		ImVec2 author_text_size{ m_footer_current_size.x, m_options_size.y / block_delimeter };
		ImVec2 author_text_pos{ m_footer_pos.x, m_footer_pos.y + m_footer_current_size.y - author_text_size.y };

		draw_text(fmt::format("<{}/{}>", m_current_option + 1, m_option_count), author_text_pos, author_text_size);
		draw_line(
			m_footer_pos,
			ImVec2(m_footer_pos.x + m_footer_current_size.x, m_footer_pos.y),
			m_selected_color,
			5.f);
	}
	void gui::draw_header_window()
	{
		static random_color_timed title_color{};
		draw_rect(m_header_pos, m_header_current_size, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));

		ImVec2 submenu_text_size{ m_header_current_size.x, m_options_size.y / block_delimeter };
		ImVec2 submenu_text_pos{ m_header_pos.x, m_header_pos.y + m_header_current_size.y - submenu_text_size.y - 12.5f };
		ImVec2 menu_text_size{ submenu_text_size.x, submenu_text_size.y * 2 };
		ImVec2 menu_text_pos{ submenu_text_pos.x, submenu_text_pos.y - submenu_text_size.y * 2};

		static ImVec4 menu_color{1.f, 1.f, 1.f, 1.f};
		auto& end_color = title_color.get();

		if (m_header_expanded)
		{
			slide_to(menu_color.x, end_color.x);
			slide_to(menu_color.y, end_color.y);
			slide_to(menu_color.z, end_color.z);
			slide_to(menu_color.w, 1.f);
		} else 
		{
			slide_to(menu_color.w, .0f);
		}
		ImGui::PushFont(g_renderer->m_font_title);
		draw_text("SPIRIT MENU", menu_text_pos, menu_text_size, menu_color);
		ImGui::PopFont();
		ImGui::PushFont(g_renderer->m_subtitle_font);
		draw_text(TRANSLATE(m_menu.get_selected_submenu().name()), submenu_text_pos, submenu_text_size, m_selected_color);
		ImGui::PopFont();
		draw_line(
			ImVec2(m_header_pos.x, m_header_pos.y + m_header_current_size.y - 2.5f),
			ImVec2(m_header_pos.x + m_header_current_size.x, m_header_pos.y + m_header_current_size.y - 2.5f),
			m_selected_color,
			5.f);
	}

	void gui::draw_submenus_window()
	{
		draw_rect(m_submenus_pos, m_submenus_size, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
		auto& submenus = m_menu.get_submenus();

		m_current_submenu = m_menu.selected_id();
		m_submenu_count = m_menu.counter();

		for (auto& submenu : submenus) {
			draw_submenu(*submenu);
		}
	}

	void gui::draw_options_window()
	{
		draw_rect(m_options_pos, m_options_size, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
		auto& options = m_menu.get_selected_submenu().get_options();

		m_current_option = m_menu.get_selected_submenu().selected_id();
		m_option_count = m_menu.get_selected_submenu().counter();

		for (auto& option : options) {
			draw_option(*option);
		}
	}

	void gui::draw_settings_window()
	{
		draw_rect(m_settings_pos, m_settings_size, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
		auto& settings = m_menu.get_selected_submenu().get_selected_option().get_settings();

		m_current_setting = m_menu.get_selected_submenu().get_selected_option().selected_id();
		m_setting_count = m_menu.get_selected_submenu().get_selected_option().counter();

		for (auto& setting : settings) {
			draw_setting(*setting);
		}
	}

	void gui::calculate_color(
		std::uint32_t id, std::uint32_t current_id,
		ImVec4& color, const ImVec4& selected_color, const ImVec4& near_selected_color, const ImVec4& non_selected_color)
	{
		if (id == current_id)
		{
			slide_to(color.x, selected_color.x);
			slide_to(color.y, selected_color.y);
			slide_to(color.z, selected_color.z);
			slide_to(color.w, selected_color.w);
		}
		else {
			slide_to(color.x, non_selected_color.x, 100);
			slide_to(color.y, non_selected_color.y, 100);
			slide_to(color.z, non_selected_color.z, 100);
			slide_to(color.w, non_selected_color.w, 100);
		}
	}

	void gui::draw_submenu(submenu& submenu_elem)
	{
		bool draw_{ false };
		ImVec2 pos_{}, size_{};

		if (submenu_elem.id() < max_submenus && m_submenu_count < max_submenus)
		{
			draw_ = true;
			pos_ = ImVec2(m_submenus_pos.x, m_submenus_pos.y + (m_submenus_size.y / block_delimeter) * submenu_elem.id());
		}
		else if (submenu_elem.id() > (m_current_submenu - max_submenus) && submenu_elem.id() <= m_current_submenu)
		{
			draw_ = true;
			pos_ = ImVec2(m_submenus_pos.x, m_submenus_pos.y + (m_submenus_size.y / block_delimeter) * (submenu_elem.id() - (m_current_submenu - max_submenus)));
		}
		if (draw_)
		{
			size_ = ImVec2(m_submenus_size.x, m_submenus_size.y / block_delimeter);
			calculate_color(submenu_elem.id(), m_current_submenu,
				submenu_elem.get_block_color(), m_selected_color, m_near_selected_color, m_non_selected_color);

			draw_rect(pos_, size_, submenu_elem.get_block_color());
			draw_text(TRANSLATE(submenu_elem.name()), pos_, size_);
		}
	}

	void gui::draw_option(option& option)
	{
		bool draw_{ false };
		ImVec2 pos_{}, size_{};

		if (option.id() < max_options && m_option_count < max_options)
		{
			draw_ = true;
			pos_ = ImVec2(m_options_pos.x, m_options_pos.y + (m_options_size.y / block_delimeter) * option.id());
		}
		else if (option.id() > (m_current_option - max_options) && option.id() <= m_current_option)
		{
			draw_ = true;
			pos_ = ImVec2(m_options_pos.x, m_options_pos.y + (m_options_size.y / block_delimeter) * (option.id() - (m_current_option - max_options)));
		}
		if (draw_)
		{
			size_ = ImVec2(m_options_size.x, m_options_size.y / block_delimeter);

			calculate_color(option.id(), m_current_option,
				option.get_block_color(), m_selected_color, m_near_selected_color, m_non_selected_color);

			draw_rect(pos_, size_, option.get_block_color());
			draw_text(TRANSLATE(option.name()), pos_, size_);
		}
	}

	void gui::draw_setting(option_setting& setting)
	{
		bool draw_{ false };
		ImVec2 pos_{}, size_{};


		if (setting.id() < max_settings && m_setting_count < max_settings)
		{
			draw_ = true;
			pos_ = ImVec2(m_settings_pos.x, m_settings_pos.y + (m_settings_size.y / block_delimeter) * setting.id());
		}
		else if (setting.id() > (m_current_setting - max_settings) && setting.id() <= m_current_setting)
		{
			draw_ = true;
			pos_ = ImVec2(m_settings_pos.x,
				m_settings_pos.y + m_settings_size.y / block_delimeter * (setting.id() - (m_current_setting - max_settings)));
		}
		if (draw_)
		{
			size_ = ImVec2(m_submenus_size.x, m_submenus_size.y / block_delimeter);

			calculate_color(setting.id(), m_current_setting,
				setting.get_block_color(), m_selected_color, m_near_selected_color, m_non_selected_color);

			draw_rect(pos_, size_, setting.get_block_color());
			draw_text(TRANSLATE(setting.name()), pos_, size_);
		}
	}

	void gui::background_overlay()
	{
		static float m_bg_alpha{ 0.f };
		static float m_font_alpha{ 0.f };
		static float m_font_scale{ 0.1f };

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
		if (ImGui::Begin("##background_overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs))
		{
			ImGui::End();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
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
				g_fiber_pool->queue_job([] { AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0); });
			}
		}
		if (left_right_elapsed.count() >= 0.500)
		{
			bool _left = iskeypressed(VK_NUMPAD4) || iskeypressed(VK_LEFT);
			bool _right = iskeypressed(VK_NUMPAD6) || iskeypressed(VK_RIGHT);
			if (_left || _right) {
				if (_left)
					m_left_pressed = true;
				else if (_right)
					m_right_pressed = true;

				left_right_timer = std::chrono::system_clock::now();
				g_fiber_pool->queue_job([] { AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0); });
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
				g_fiber_pool->queue_job([] { AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0); });
			}
		}
	}
	void gui::reset_controlls()
	{
		m_pressed = false;
		m_back = false;
		m_up_pressed = false;
		m_down_pressed = false;
		m_left_pressed = false;
		m_right_pressed = false;
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
}
