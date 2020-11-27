#include "pch.h"
#include "pattern_scanner.h"

/*
	Constructor overloading for initialization of class members
	In:
		hProcess - target process handle
		hModule - target module handle
		pattern - scanned pattern (pattern scanning)
		mask - scanned mask (pattern scanning)

	Out:
		None
*/

PatternScanning::PatternScanning(HANDLE hProcess, HMODULE hModule)
	: _hProcess(hProcess), _hModule(hModule)
{}

/*
	This function manages the whole pattern scanning progress
	In:
		pattern - the scanned pattern
		mask - the scanned mask (for the pattern)
	Out:
		the desired address
*/

uintptr_t PatternScanning::PatternScan(BYTE* pattern, char* mask) {
	MODULEINFO moduleInfo{};
	BYTE* moduleContent{};
	uintptr_t signatureIndex{};
	uintptr_t signatureOffset{};

	::GetModuleInformation(this->_hProcess, this->_hModule, &moduleInfo, sizeof(moduleInfo));
	moduleContent = new BYTE[moduleInfo.SizeOfImage];

	if (moduleContent == NULL)
		exit(1);

	if (!::ReadProcessMemory(this->_hProcess, (void*)this->_hModule, (void*)moduleContent, (uintptr_t)moduleInfo.SizeOfImage, NULL)) {
		delete[] moduleContent;
		exit(1);
	}

	signatureIndex = this->FindPattern(moduleContent, moduleInfo.SizeOfImage, pattern, mask);

	if (!signatureIndex) {
		delete[] moduleContent;
		exit(1);
	}

	memcpy(&signatureOffset, &moduleContent[signatureIndex], sizeof(DWORD));
	delete[] moduleContent;

	return signatureOffset;
}

/*
	This function execute the actual pattern scanning, its passing along the loop of the pattern and the module content and trying to match an equal bytes
	In:
		pattern for scanning, module content, module size, pattern mask
	Out:
		an index where the offset is located in the byte array of the module content
*/

uintptr_t PatternScanning::FindPattern(BYTE* moduleContent, unsigned int moduleSize, BYTE* pattern, char* mask) {
	bool flag = true;

	for (unsigned i{}; i < moduleSize; i++) {
		flag = true;

		for (unsigned j{}; j < strlen(mask); j++) {
			if (mask[j] == '?')
				continue;

			if (pattern[j] != moduleContent[i + j]) {
				flag = false;
				break;
			}
		}

		if (flag == true)
			return i;
	}

	return NULL;
}