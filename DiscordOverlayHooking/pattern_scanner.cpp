#include "pattern_scanner.h"

Pattern::Pattern(int processId, wchar_t* moduleName, 
				wchar_t* processName, char* pattern, char* mask)
	: _processId(processId),
	_moduleName(moduleName),
	_processName(processName),
	_pattern(pattern),
	_mask(mask)
{

}

uintptr_t Pattern::PatternScan() {

}

/*
	This function execute the actual pattern scanning, its passing along the loop of the pattern and the module content and trying to match an equal bytes
	In:
		pattern for scanning, module content, module size, pattern mask

	Out:
		an index where the offset is located in the byte array of the module content
*/

uintptr_t Pattern::FindPattern(BYTE* moduleContent, unsigned int moduleSize, char* pattern, char* mask) {
	unsigned int i, j = 0;
	int flag = TRUE;

	for (i = 0; i < moduleSize; i++) {
		flag = TRUE;

		for (j = 0; j < strlen(mask); j++) {
			if (mask[j] == '?') {
				continue;
			}
			if (pattern[j] != moduleContent[i + j]) {
				flag = FALSE;
				break;
			}
		}

		if (flag == TRUE) {
			return i;
		}
	}

	return;
}

/*
	This functuion returns process ID by the name
	In:
		process name
	Out:
		process ID
*/

uintptr_t Pattern::GetProcessIdByName(wchar_t* processName) {
	PROCESSENTRY32 processEntry{};
	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!snapShot)
		return;

	processEntry.dwSize = sizeof(processEntry);

	if (!Process32First(snapShot, &processEntry))
		CloseHandle(snapShot);
		return;

	do
	{
		if (!wcscmp(processEntry.szExeFile, processName))
		{
			CloseHandle(snapShot);
			return processEntry.th32ProcessID;
		}
	} while (Process32Next(snapShot, &processEntry));

	CloseHandle(snapShot);
	return;
}

/*
	This function returns module handle - module base address
	In:
		process ID (parent process), module name
	Out:
		module handle - module base address
*/

MODULEENTRY32 Pattern::fnGetModuleHandle(const int processId, const wchar_t* moduleName) {
	MODULEENTRY32 modEntry{};
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				if (wcscmp(modEntry.szModule, moduleName) == 0)
				{
					break;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}

	return modEntry;
}