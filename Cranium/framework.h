#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

//disabled until we find a good xor lib
#define XOR(STR) STR

#pragma comment(lib, "minhook/minhook.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "advapi32.lib")
#include "minhook/MinHook.h"
