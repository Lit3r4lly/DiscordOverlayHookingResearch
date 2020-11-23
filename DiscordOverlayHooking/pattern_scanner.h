#pragma once

#include <Windows.h>
#include <TlHelp32.h>

class Pattern
{
public:
	Pattern() = default;
	~Pattern() = default;

	Pattern(int processId, wchar_t* moduleName, wchar_t* processName, char* pattern, char* mask);
	
	uintptr_t PatternScan();
	uintptr_t FindPattern(BYTE* moduleContent, unsigned int moduleSize, char* pattern, char* mask);
	uintptr_t GetProcessIdByName(wchar_t* processName);
	MODULEENTRY32 fnGetModuleHandle(const int processId, const wchar_t* moduleName);

private:
	uintptr_t _processId;
	wchar_t* _moduleName;
	wchar_t* _processName;
	char* _pattern;
	char* _mask;

};