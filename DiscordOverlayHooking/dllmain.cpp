
#include "pch.h"
#include "pattern_scanner.h"

void HookEx() {
    const char* moduleName = "DiscordHook64.dll";
    const char* processName = "ida64.exe";

    const size_t moduleNameLen = strlen(moduleName) + 1;

    wchar_t* wModuleName = new wchar_t[moduleNameLen];
    mbstowcs(wModuleName, moduleName, moduleNameLen);


    const size_t processNameLen = strlen(processName) + 1;

    wchar_t* wProcessName = new wchar_t[processNameLen];
    mbstowcs(wProcessName, processName, processNameLen);

   PatternScanning newObj(wModuleName, wProcessName);
    uintptr_t addr = newObj.PatternScan((BYTE*)"\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x60", (char*)"xxxxxxxxxxxxxxx");

    ::MessageBoxA(NULL, "Hi", "hi", MB_OK);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HookEx();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

