#pragma once
#pragma execution_character_set("utf-8")

#include <SDKDDKVer.h>
#include <Windows.h>

#include <d3d9.h>

#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>

#include <dxgi.h>
#include <d3d11.h>

#include <dxgi.h>
#include <d3d12.h>

#include "directxsdk/D3D11.h"
#include "directxsdk/D3DX11.h"
#include "directxsdk/D3DX11tex.h"
#pragma comment(lib, "C:/dev/vcpkg/packages/directxsdk_x64-windows-static/lib/d3d11.lib")
#pragma comment(lib, "C:/dev/vcpkg/packages/directxsdk_x64-windows-static/lib/d3dx11.lib")

#include <wrl/client.h>

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <chrono>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>
#include <new>

#include <sstream>
#include <string>
#include <string_view>

#include <algorithm>
#include <functional>
#include <utility>

#include <stack>
#include <vector>

#include <typeinfo>
#include <type_traits>

#include <exception>
#include <stdexcept>

#include <any>
#include <optional>
#include <variant>

#include "nlohmann/json.hpp"
#include "StackWalker/StackWalker.h"

#include "logger.hpp"
#include "classes/vector3/vector3_c.h"
#include "translation/translate.hpp"

using namespace std::chrono_literals;


#define EXTREMUM_CHEAT_VER 1.8
#define EXTREMUM_CHEAT_VER_TXT "1.8"
#define EXTREMUM_DEVELOPER "Jason"

#define iskeypressed(key)	GetAsyncKeyState(key) & 0x8000


namespace extremum {
	namespace core {
		template <typename T>
		using comptr = Microsoft::WRL::ComPtr<T>;

		inline HMODULE g_hmodule{};
		inline HANDLE g_main_thread{};
		inline DWORD g_main_thread_id{};
		inline std::atomic_bool g_running{ true };

		struct stackwalker : public StackWalker
		{
			using StackWalker::StackWalker;

			void OnOutput(LPCSTR szText) override
			{
				g_logger->raw(log_color::red, szText);
			}
		};

		inline stackwalker g_stackwalker;
	};
};