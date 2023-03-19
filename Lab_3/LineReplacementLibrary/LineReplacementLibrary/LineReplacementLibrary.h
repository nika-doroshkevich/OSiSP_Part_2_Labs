#pragma once
#include <Windows.h>
#include <vector>

extern "C" __declspec(dllexport) INT __cdecl ReplaceString(DWORD pid, const char* srcStr, const char* repStr);
