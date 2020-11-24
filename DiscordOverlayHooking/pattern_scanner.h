#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

class PatternScanning
{
public:
	PatternScanning() = default;
	~PatternScanning() = default;

	PatternScanning(wchar_t* moduleName, wchar_t* processName)
		: _moduleName(moduleName),
		_processName(processName)
	{}

	uintptr_t PatternScan(BYTE* pattern, char* mask);
	uintptr_t FindPattern(BYTE* moduleContent, unsigned int moduleSize, BYTE* pattern, char* mask) const;
	HMODULE fnGetModuleHandle(const int processId);

private:
	wchar_t* _moduleName;
	wchar_t* _processName;

};