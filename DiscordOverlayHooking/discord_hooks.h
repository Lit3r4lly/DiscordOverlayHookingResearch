#pragma once

#include <cstdint>
#include <Windows.h>

// Minhook functions callbacks

using wrapCreateHook = uint64_t(__fastcall*)(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal);
using wrapEnableHook = uint64_t(__fastcall*)(LPVOID pTarget);
using wrapEnableHookQueue = uint64_t(__stdcall*)();

class DiscordHook
{
public:
	DiscordHook() = default;
	~DiscordHook() = default;

	void CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal) const;
	void EnableHook(LPVOID pTarget) const;
	void EnableHookQueue() const;

private:
	HANDLE _hProcess;

	uintptr_t _fnOriginal;
	uintptr_t _fnHooked;
	uintptr_t _fnOriginalPresent;
};