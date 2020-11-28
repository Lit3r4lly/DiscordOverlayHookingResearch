#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

class PatternScanning
{
public:
	PatternScanning() = default;
	PatternScanning(HANDLE hProcess, HMODULE hModule);

	uintptr_t PatternScan(BYTE* pattern, char* mask);
	unsigned FindPattern(std::uint8_t* moduleContent, unsigned int moduleSize, BYTE* pattern, char* mask);

private:
	HANDLE _hProcess;
	HMODULE _hModule;

};