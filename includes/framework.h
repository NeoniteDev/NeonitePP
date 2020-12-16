#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "d3d9.lib")

#include "../includes/imgui/imgui.h"
#include "../includes/imgui/imgui_impl_dx9.h"
#include "../includes/imgui/imgui_impl_win32.h"
#include "../includes/imgui/imgui_internal.h"
#include "../includes/imgui/imconfig.h"
#include "../includes/json/json.hpp"
#include "../includes/httplib.h"
#include "../includes/injection/Injection.h"
#include "../includes/termcolor/termcolor.hpp"
#include "../includes/DiscordSDK/discord_rpc.h"
#include "../includes/DiscordSDK/discord_register.h"

#include <chrono>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <filesystem>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include <shlobj.h>
#include <fstream>
#include <Windows.h>
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>