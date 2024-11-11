#include <Windows.h>
#include <iostream>
#include "options.h"
#include "MinHook.h"
#include "sharedMemory.h"

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

#include "hooks.h"

#include <GL/gl.h>
#pragma comment(lib, "OpenGL32.lib")

void cheatInitialize() {
    if (MH_Initialize() != MH_OK) {
        std::cout << "Failed to initialize MinHook.\n";
        return;
    }
    
    if (!createMappingData()) return;

     options::monitorX = (float)GetSystemMetrics(SM_CXSCREEN);
     options::monitorY = (float)GetSystemMetrics(SM_CYSCREEN);

    if (MH_CreateHook(&glDrawElements, Hooks::get_glDrawElements(), reinterpret_cast<void**>(&fn_glDrawElements)) != MH_OK) std::cout << "[-] Failed to create hook for glDrawElements.\n";
    if (MH_CreateHook(&glFlush, Hooks::get_glFlush(), reinterpret_cast<void**>(&fn_glFlush)) != MH_OK) std::cout << "[-] Failed to create hook for glFlush.\n";

    if (MH_EnableHook(&glDrawElements) != MH_OK) std::cout << "[-] Failed to enable hook glDrawElements\n";
    if (MH_EnableHook(&glFlush) != MH_OK) std::cout << "[-] Failed to enable hook glFlush\n";
}

void createConsole() {
    AllocConsole();

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
}

bool __stdcall DllMain(HMODULE hModule, DWORD ulReason, LPVOID lpReserved) {
    if (ulReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        //createConsole();  Alloc console if you want

        cheatInitialize();
    }
    else if (ulReason == DLL_PROCESS_DETACH) {
        MH_Uninitialize();
        closeMappingHandle();
    }

    return true;
}