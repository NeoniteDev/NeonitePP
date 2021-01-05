#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include "xorstr.hpp"

//bye bye skid
#define XOR(STR) xorstr(STR).crypt_get()

extern "C" void Pain();


#pragma comment(lib, "minhook/minhook.lib")
#include "minhook/MinHook.h"
