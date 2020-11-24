#include "pch.h"
#include "pattern_scanner.h"

/*
	This function manages the whole pattern scanning progress
	In:
		pattern - the scanned pattern
		mask - the scanned mask (for the pattern)

	Out:
		the desired address
*/

uintptr_t PatternScanning::PatternScan(BYTE* pattern, char* mask) {
	const auto hProcess = ::GetCurrentProcess();
	const auto processId = ::GetProcessId(hProcess);
	const auto hModule = this->fnGetModuleHandle(processId);

	MODULEINFO moduleInfo{};
	BYTE* moduleContent{};
	uintptr_t signatureIndex{};
	uintptr_t signatureOffset{};

	::GetModuleInformation(hProcess, hModule, &moduleInfo, sizeof(moduleInfo));
	moduleContent = new BYTE[moduleInfo.SizeOfImage];

	if (moduleContent == NULL) {
		::CloseHandle(hProcess);
		exit(1);
	}

	if (!::ReadProcessMemory(hProcess, (void*)hModule, (void*)moduleContent, (size_t)moduleInfo.SizeOfImage, NULL)) {
		delete[] moduleContent;
		::CloseHandle(hProcess);

		exit(1);
	}

	signatureIndex = this->FindPattern(moduleContent, moduleInfo.SizeOfImage, pattern, mask);
	if (!signatureIndex) {
		delete[] moduleContent;
		::CloseHandle(hProcess);

		exit(1);
	}

	memcpy(&signatureOffset, &moduleContent[signatureIndex], sizeof(DWORD));

	delete[] moduleContent;
	::CloseHandle(hProcess);

	return signatureOffset;
}

/*
	This function execute the actual pattern scanning, its passing along the loop of the pattern and the module content and trying to match an equal bytes
	In:
		pattern for scanning, module content, module size, pattern mask

	Out:
		an index where the offset is located in the byte array of the module content
*/

uintptr_t PatternScanning::FindPattern(BYTE* moduleContent, unsigned int moduleSize, BYTE* pattern, char* mask) const {
	bool flag = true;

	for (unsigned i{}; i < moduleSize; i++) {
		flag = true;

		for (unsigned j{}; j < strlen(mask); j++) {
			if (mask[j] == '?') {
				continue;
			}
			if (pattern[j] != moduleContent[i + j]) {
				flag = false;
				break;
			}
		}

		if (flag == true) {
			return i;
		}
	}

	return NULL;
}


/*
	This function returns module handle - module base address
	In:
		process ID (parent process), module name
	Out:
		module handle - module base address
*/

HMODULE PatternScanning::fnGetModuleHandle(const int processId) {
	MODULEENTRY32 modEntry{};
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	wchar_t* wModuleName{};

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				wModuleName = (wchar_t*)modEntry.szModule;
				if (wcscmp(wModuleName, this->_moduleName) == 0)
				{
					break;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}

	return modEntry.hModule;
}