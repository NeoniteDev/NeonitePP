#ifndef UTIL
#define UTIL

//Headers
#include "json.hpp"
#include "Injection.h"
#include "termcolor.hpp"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <filesystem>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include <shlobj.h>
#include <fstream>

template<typename ...Args>
void printd(Args && ...args)
{
	(std::cout << ... << args);
}

using json = nlohmann::json;

//Functions declarations
HANDLE startup(LPCSTR lpApplicationName, LPSTR lpArguments);
void suspend(HANDLE processHandle);
void resume(HANDLE processHandle);
DWORD GetProcId(const char* procName);
std::string GetEXEPath();

//Inline vars
inline auto logo = R"(
888b    888                            d8b 888             .d8888b.                 
8888b   888                            Y8P 888            d88P  Y88b                
88888b  888                                888            888    888                
888Y88b 888  .d88b.   .d88b.  88888b.  888 888888 .d88b.  888         888     888   
888 Y88b888 d8P  Y8b d88""88b 888 "88b 888 888   d8P  Y8b 888       8888888 8888888 
888  Y88888 88888888 888  888 888  888 888 888   88888888 888    888  888     888   
888   Y8888 Y8b.     Y88..88P 888  888 888 Y88b. Y8b.     Y88b  d88P                
888    Y888  "Y8888   "Y88P"  888  888 888  "Y888 "Y8888   "Y8888P"                 
                                                                                    
                                                                                    
                                                                                    
)";
inline BOOL debug = false;
inline BOOL isPaused = false;
inline ULONG pid = 0;

#endif // !UTIL