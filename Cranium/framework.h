#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <fstream>

//disabled until we find a good xor lib
#define XOR(STR) STR

#pragma comment(lib, "minhook/minhook.lib")
#include "minhook/MinHook.h"
