#include "common.hpp"
#include "features/features.hpp"
#include "script_mgr/fiber_pool.hpp"
#include "gui/gui.hpp"
#include "logger.hpp"
#include "hooking/hooking.hpp"
#include "pointers.hpp"
#include "renderer/renderer.hpp"
#include "script_mgr/script_mgr.hpp"
#include "translation/translate.hpp"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace extremum::core;
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);
		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD
			{
				auto m_logger_instance = std::make_unique<logger>();
				LOG_INFO("Logger System Initialized");
				auto m_translation_instance = std::make_unique<translation>();
				LOG_INFO("Translation System Initialized");
				try
				{
					auto m_pointers_instance = std::make_unique<pointers>();
					LOG_INFO("Pointers initialized.");

					if (*g_pointers->m_game_state != eGameState::Playing)
					{
						LOG_INFO("Waiting for the game to load.");
						do
						{
							std::this_thread::sleep_for(100ms);
						} while (*g_pointers->m_game_state != eGameState::Playing);

						LOG_INFO("The game has loaded.");
					}
					else
					{
						LOG_INFO("The game is already loaded.");
					}
					
					auto m_renderer_instance = std::make_unique<renderer>();
					LOG_INFO("Renderer initialized.");

					auto m_fiber_pool_instance = std::make_unique<fiber_pool>(10);
					LOG_INFO("Fiber pool initialized.");

					auto m_hooking_instance = std::make_unique<hooking>();
					LOG_INFO("Hooking initialized.");

					g_script_mgr.add_script(std::make_unique<script>(&features::script_func));
					g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));
					LOG_INFO("Scripts registered.");

					g_hooking->enable();
					LOG_INFO("Hooking enabled.");

					while (g_running)
					{
						if (GetAsyncKeyState(VK_F9) & 0x8000)
							g_running = false;

						g_hooking->ensure_dynamic_hooks();
						std::this_thread::sleep_for(10ms);
					}
					std::this_thread::sleep_for(1000ms);

					g_hooking->disable();
					LOG_INFO("Hooking disabled.");

					std::this_thread::sleep_for(1000ms);

					g_script_mgr.remove_all_scripts();
					LOG_INFO("Scripts unregistered.");

					m_hooking_instance.reset();
					LOG_INFO("Hooking uninitialized.");

					m_fiber_pool_instance.reset();
					LOG_INFO("Fiber pool uninitialized.");

					m_renderer_instance.reset();
					LOG_INFO("Renderer uninitialized.");

					m_pointers_instance.reset();
					LOG_INFO("Pointers uninitialized.");
				}
				catch (std::exception const& ex)
				{
					LOG_ERROR("{}", ex.what());
					MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
				}


				m_translation_instance.reset();
				LOG_INFO("Translation System Unitialized");

				LOG_INFO("Logger System Unitialized");
				m_logger_instance.reset();

				CloseHandle(g_main_thread);
				FreeLibraryAndExitThread(g_hmodule, 0);
			}, nullptr, 0, &g_main_thread_id);
	}
	return true;
}
