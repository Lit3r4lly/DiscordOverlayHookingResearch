#pragma once

#include <cstdint>
#include <Windows.h>
#include <dxgi.h>

// Minhook functions callbacks

using wrapCreateHook = uintptr_t(__fastcall*)(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal);
using wrapEnableHook = uintptr_t(__fastcall*)(LPVOID pTarget);
using wrapEnableHookQueue = uintptr_t(__stdcall*)();
using wrapPresent = HRESULT(__fastcall*)(IDXGISwapChain* dxSwapChain, UINT syncInterval, UINT flags);

HRESULT hookedPresentFunction(IDXGISwapChain* dxSwapChain, UINT syncInterval, UINT flags, uintptr_t presentAddr);

class DiscordHook
{
public:
	DiscordHook() = default;
	~DiscordHook() = default;

	static uintptr_t CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal, uintptr_t createHookAddr);
	static uintptr_t EnableHook(LPVOID pTarget, uintptr_t enableHookAddr);
	static uintptr_t EnableHookQueue(uintptr_t enableHookQueueAddr);

};