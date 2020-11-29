#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

// This class manages the pattern scanning part

class PatternScanning
{
public:
	PatternScanning() = default;
	PatternScanning(HANDLE hProcess, HMODULE hModule);

	uintptr_t PatternScan(std::uint8_t* pattern, char* mask);
	uintptr_t FindPattern(std::uint8_t* moduleContent, uintptr_t moduleSize, std::uint8_t* pattern, char* mask);

private:
	HANDLE _hProcess;
	HMODULE _hModule;

};