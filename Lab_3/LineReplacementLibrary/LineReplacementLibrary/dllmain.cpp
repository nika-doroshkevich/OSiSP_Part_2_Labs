// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "LineReplacementLibrary.h"

BOOL APIENTRY DllMain( HMODULE hModule,             
                       DWORD  ul_reason_for_call,   
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        MessageBoxA(NULL, "Dll was injected", "Inject", MB_OK);
        ReplaceString(GetCurrentProcessId(), "Old String", "New String");
        break;
    }
    case DLL_THREAD_ATTACH: 
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

