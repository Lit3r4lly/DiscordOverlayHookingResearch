#include "pch.h"
#include "pattern_scanner.h"

void HookEx() {
    const auto hProcess = ::GetCurrentProcess();
    const auto hModule = ::GetModuleHandle("DiscordHook64.dll");

    BYTE fnCreateHookPattern[] = "\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x60";
    char fnCreateHookMask[] = "xxxxxxxxxxxxxxx";

    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    std::cout << "[!] Starting to hook discord overlay" << '\n';

    std::cout << "[?] Process Handle - 0x" << (void*)hProcess << '\n';
    std::cout << "[?] DiscordHook64.dll handle - 0x" << (void*)hModule << '\n';

    std::cout << "[?] Pattern - " << (BYTE*)fnCreateHookPattern << '\n';
    std::cout << "[?] Mask - " << fnCreateHookMask << '\n';

    PatternScanning newObj(hProcess, hModule);
    uintptr_t addr = newObj.PatternScan(fnCreateHookPattern, fnCreateHookMask);

    std::cout << "[-] CreateHook address - 0x" << (void*)addr << '\n';
    
    std::cout << "\n[!] Finished, you can close the console right now." << '\n';
    system("PAUSE");
    FreeConsole();
}

BOOL APIENTRY DllMain(HMODULE hModule,
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
