#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

// This class manages the pattern scanning part

class PatternScanning
{
public:
	PatternScanning(HANDLE hProcess, HMODULE hModule);

	uintptr_t PatternScan(const std::uint8_t* pattern, const std::string& mask);
	UINT FindPattern(const std::uint8_t* moduleContent, const size_t moduleSize, const std::uint8_t* pattern, const std::string& mask);

private:
	HANDLE _hProcess;
	HMODULE _hModule;

};