#pragma once
#include "..\common.hpp"
#include "imgui.h"

namespace extremum::core
{
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void on_present();

		void pre_reset();
		void post_reset();

		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	public:
		ImFont *m_font;
		ImFont *m_font_title;
		ImFont* m_subtitle_font;
		ImFont *m_monospace_font;

		comptr<ID3D11Device> m_d3d_device;
		comptr<ID3D11DeviceContext> m_d3d_device_context;
		comptr<IDXGISwapChain> m_dxgi_swapchain;
	};

	inline renderer *g_renderer{};
}
