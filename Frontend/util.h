#ifndef UTIL
#define UTIL

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "d3d9.lib")

//Headers
#include "../includes/imgui/imgui.h"
#include "../includes/imgui/imgui_impl_dx9.h"
#include "../includes/imgui/imgui_impl_win32.h"
#include "../includes/imgui/imgui_internal.h"
#include "../includes/imgui/imconfig.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "../includes/json/json.hpp"
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

template <typename ...Args>
void printd(Args && ...args) {
	(std::cout << ... << args);
}

using json = nlohmann::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

//Functions declarations
HANDLE startup(LPCSTR lpApplicationName, LPSTR lpArguments);
void suspend(HANDLE processHandle);
void resume(HANDLE processHandle);
DWORD GetProcId(const char* procName);
std::string GetEXEPath();

//Inline vars
inline std::vector<std::string> IDs;
inline HWND window;
inline std::vector<std::string> prices;
inline std::vector<std::string> ImportedLocker;
inline std::vector<std::string> LockerBackup;

inline char newsTitleE[33];
inline char newsBodyE[33];

inline char newsTitle[33];
inline char newsBody[33];

inline char tileURL[33];
inline char imageURL[33];

inline char DllName[33];

inline bool bUseRPC = true;

inline char name[33] = "NeoniteUser";
inline bool bHasBattlepass = false;
inline bool bIsCataba = false;
inline int seasonLevel = 100;
inline int vbucks = 100000;

inline bool bHasImportedLocker = false;

inline char ID[125];
inline char pricesDE[44];
inline char prices_[44];

inline bool bIsLockerOpen = false;
inline auto jsonObjects = json::array();

inline bool isDebug = false;
inline bool noInj = false;
inline bool isPaused = false;
inline std::string addDll;
inline const char* ip = "127.0.0.1";
inline unsigned short port = 5595;
inline ULONG pid = 0;

#endif // !UTIL
