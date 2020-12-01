#include "pch.h"
#include "pattern_scanner.h"
#include "discord_hooks.h"

void HookEx() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    uintptr_t createHookAddr{};
    uintptr_t enableHookAddr{};
    uintptr_t enableHookQueueAddr{};
    uintptr_t presentAddr{};
    const auto originalPresent = nullptr;

    const std::uint8_t* createHookPattern = (std::uint8_t*)"\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x60";
    const std::uint8_t* enableHookPattern = (std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x33\xF6\x8B\xFA";
    const std::uint8_t* enableHookQueuePattern = (std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x7C\x24\x00\x41\x57";
    const std::uint8_t* presentPattern = (std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xD9\x41\x8B\xF8";

    const std::string createHookMask = "xxxxxxxxxxxxxxx";
    const std::string enableHookMask = "xxxx?xxxx?xxxx?xxxxxxxxxxxxx";
    const std::string enableHookQueueMask = "xxxx?xxxx?xxxx?xx";
    const std::string presentMask = "xxxx?xxxx?xxxxxxxxxxx";

    const auto hProcess = ::GetCurrentProcess();
    const auto hModule = ::GetModuleHandle("DiscordHook64.dll");

    if (hModule == INVALID_HANDLE_VALUE) {
        std::cerr << "Invalid module handle!" << '\n';
        system("pause");

        fclose(stdout);
        FreeConsole();
        return;
    }

    if (hProcess == INVALID_HANDLE_VALUE) {
        std::cerr << "Invalid process handle!" << '\n';
        system("pause");

        fclose(stdout);
        FreeConsole();
        return;
    }

    PatternScanning newObj(hProcess, hModule);

    try {
        createHookAddr = newObj.PatternScan(createHookPattern, createHookMask);
        enableHookAddr = newObj.PatternScan(enableHookPattern, enableHookMask);
        enableHookQueueAddr = newObj.PatternScan(enableHookQueuePattern, enableHookQueueMask);
        presentAddr = newObj.PatternScan(presentPattern, presentMask);
    } catch (const std::string& message) {
        std::cerr << message << '\n';
        system("pause");

        fclose(stdout);
        FreeConsole();
        return;
    }

    std::cout << "[!] Starting to hook discord overlay..." << '\n';

    std::cout << "[?] Process handle - 0x" << (void*)hProcess << '\n';
    std::cout << "[?] DiscordHook64.dll handle - 0x" << (void*)hModule << '\n' << '\n';

    std::cout << "[-] Create Hook absolue address - 0x" << (void*)(createHookAddr) << '\n';
    std::cout << "[-] Enable Hook absolue address - 0x" << (void*)(enableHookAddr) << '\n';
    std::cout << "[-] Enable Hook Queue absolue address - 0x" << (void*)(enableHookQueueAddr) << '\n';
    std::cout << "[-] Present absolue address - 0x" << (void*)(presentAddr) << '\n';

    DiscordHook::CreateHook((LPVOID)presentAddr, (LPVOID)hookedPresentFunction, (LPVOID*)&originalPresent, createHookAddr);
    DiscordHook::EnableHook((LPVOID)presentAddr, enableHookAddr);
    DiscordHook::EnableHookQueue(enableHookQueueAddr);

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