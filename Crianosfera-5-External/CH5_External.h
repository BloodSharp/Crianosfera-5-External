#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <dwmapi.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <corecrt_math_defines.h>
#include <cmath>

#include "JSON/json.h"
#include "JSON/json-forwards.h"

#include "ValveSDK.h"
#include "ch5_undocumented.h"

#include "Lua/lua.hpp"
#include <LuaBridge/LuaBridge.h>
#include "ch5_lua_bindings.h"

#include "ch5_aimbot.h"
#include "ch5_interface.h"
#include "ch5_engine.h"
#include "ch5_client.h"
#include "ch5_settings.h"
#include "ch5_players.h"
#include "ch5_localplayer.h"
#include "ch5_offsets.h"
#include "ch5_utils.h"

#include "xorstr.h"

extern HWND g_HackWnd;