#include "pch.h"
#include "discord_hooks.h"

/*
	This method handles a wrapper for the hooked MH_CreateHook function
	In:
		pTarget, pDetour, ppOriginal - "https://github.com/TsudaKageyu/minhook/blob/master/include/MinHook.h"
		createHookAddr - the absolute address for the MH_CreateHook function
*/

uintptr_t DiscordHook::CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal, uintptr_t createHookAddr) {
	auto myCreareHook = (wrapCreateHook)createHookAddr;
	return myCreareHook(pTarget, pDetour, ppOriginal);
}

/*
	This method handles a wrapper for the hooked MH_EnableHook function
	In:
		pTarget - the target hooked present scene function
		enableHookAddr - the absoule address for the MH_EnableHook function
	
	Out:
		the return value from MH_EnableHook
*/

uintptr_t DiscordHook::EnableHook(LPVOID pTarget, uintptr_t enableHookAddr) {
	auto myEnableHook = (wrapEnableHook)enableHookAddr;
	return myEnableHook(pTarget);
}

/*
	This method handles a wrapper for the hooked MH_EnableHookQueue function
	In:
		enableHookQueueAddr - the absolute address for the MH_EnableHookQueue function

	Out:
		the return value from the MH_EnableHookQueue
*/

uintptr_t DiscordHook::EnableHookQueue(uintptr_t enableHookQueueAddr) {
	auto myEnableHook = (wrapEnableHookQueue)enableHookQueueAddr;
	return myEnableHook();
}

HRESULT hookedPresentFunction(IDXGISwapChain* dxSwapChain, UINT syncInterval, UINT flags, uintptr_t presentAddr) {
	MessageBoxA(nullptr, "Got hooked!", "Hi!", MB_OK);
	auto originalPresent = (wrapPresent)presentAddr;

	return originalPresent(dxSwapChain, syncInterval, flags);
}