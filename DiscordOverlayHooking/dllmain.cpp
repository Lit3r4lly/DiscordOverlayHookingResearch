#include "pch.h"
#include "pattern_scanner.h"
#include "discord_hooks.h"

void HookEx() {
    const auto hProcess = ::GetCurrentProcess();
    const auto hModule = ::GetModuleHandle("DiscordHook64.dll");
    PatternScanning newObj(hProcess, hModule);
    
    const auto createHookAddr = newObj.PatternScan((std::uint8_t*)"\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x60", (char*)"xxxxxxxxxxxxxxx");
    const auto enableHookAddr = newObj.PatternScan((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x33\xF6\x8B\xFA", (char*)"xxxx?xxxx?xxxx?xxxxxxxxxxxxx");
    const auto enableHookQueueAddr = newObj.PatternScan((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x7C\x24\x00\x41\x57", (char*)"xxxx?xxxx?xxxx?xx");
    const auto presentAddr = newObj.PatternScan((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xD9\x41\x8B\xF8", (char*)"xxxx?xxxx?xxxxxxxxxxx");
    const auto originalPresent = (wrapPresent)nullptr;

    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    std::cout << "[!] Starting to hook discord overlay..." << '\n';

    std::cout << "[?] Process handle - 0x" << (void*)hProcess << '\n';
    std::cout << "[?] DiscordHook64.dll handle - 0x" << (void*)hModule << '\n';

    std::cout << "[-] Create Hook absolue address - 0x" << (void*)(createHookAddr) << '\n';
    std::cout << "[-] Enable Hook absolue address - 0x" << (void*)(enableHookAddr)<< '\n';
    std::cout << "[-] Enable Hook Queue absolue address - 0x" << (void*)(enableHookQueueAddr) << '\n';
    std::cout << "[-] Present absolue address - 0x" << (void*)(presentAddr) << '\n';

    DiscordHook myDis;
    myDis.CreateHook((LPVOID)presentAddr, (LPVOID)hookedPresentFunction, (LPVOID*)&originalPresent, createHookAddr);
    myDis.EnableHook((LPVOID)presentAddr, enableHookAddr);
    myDis.EnableHookQueue(enableHookQueueAddr);

    std::cout << "\n[!] Finished, you can close the console right now." << '\n';
    system("PAUSE");

    fclose(stdout);
    FreeConsole();

    return;
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
