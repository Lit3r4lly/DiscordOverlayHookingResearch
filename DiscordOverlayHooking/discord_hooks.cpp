#include "pch.h"
#include "discord_hooks.h"

uintptr_t DiscordHook::CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal, uintptr_t createHookAddr) {
	auto myCreareHook = (wrapCreateHook)createHookAddr;
	return myCreareHook(pTarget, pDetour, ppOriginal);
}

uintptr_t DiscordHook::EnableHook(LPVOID pTarget, uintptr_t enableHookAddr) {
	auto myEnableHook = (wrapEnableHook)enableHookAddr;
	return myEnableHook(pTarget);
}

uintptr_t DiscordHook::EnableHookQueue(uintptr_t enableHookQueueAddr) {
	auto myEnableHook = (wrapEnableHookQueue)enableHookQueueAddr;
	return myEnableHook();
}

HRESULT hookedPresentFunction(IDXGISwapChain* dxSwapChain, UINT syncInterval, UINT flags, uintptr_t presentAddr) {
	DebugBreak();
	MessageBoxA(nullptr, "Got hooked!", "Hi!", MB_OK);
	//std::cout << "Hooked" << '\n';
	auto originalPresent = (wrapPresent)presentAddr;

	return originalPresent(dxSwapChain, syncInterval, flags);
}