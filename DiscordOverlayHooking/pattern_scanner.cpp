#include "pch.h"
#include "pattern_scanner.h"

/*
	Constructor overloading for initialization of class members
	In:
		hProcess - target process handle
		hModule - target module handle

	Out:
		None
*/

PatternScanning::PatternScanning(HANDLE hProcess, HMODULE hModule)
{
	if (hProcess != INVALID_HANDLE_VALUE)
		this->_hProcess = hProcess;

	if (hModule != INVALID_HANDLE_VALUE)
		this->_hModule = hModule;
}

/*
	This function manages the whole pattern scanning progress
	In:
		pattern - the scanned pattern
		mask - the scanned mask (for the pattern)
	Out:
		the desired address
*/

uintptr_t PatternScanning::PatternScan(const std::uint8_t* pattern, const std::string& mask) {
	MODULEINFO moduleInfo{};
	uintptr_t signatureIndex{};

	::GetModuleInformation(this->_hProcess, this->_hModule, &moduleInfo, sizeof(moduleInfo));
	const auto moduleContent = (std::uint8_t*)this->_hModule;

	if (moduleContent == nullptr)
		throw "[!] Failed to get the module content\n";

	signatureIndex = this->FindPattern(moduleContent, moduleInfo.SizeOfImage, pattern, mask);

	if (!signatureIndex)
		throw "[!] Failed to get the module content\n";

	return reinterpret_cast<uintptr_t>(&moduleContent[signatureIndex]);
}

/*
	This function execute the actual pattern scanning, its passing along the loop of the pattern and the module content and trying to match an equal bytes
	In:
		pattern for scanning, module content, module size, pattern mask
	Out:
		an index where the offset is located in the byte array of the module content
*/

UINT PatternScanning::FindPattern(const std::uint8_t* moduleContent, const size_t moduleSize, const std::uint8_t* pattern, const std::string& mask) {
	bool flag = true;

	for (unsigned i{}; i < moduleSize - mask.length(); i++) {
		flag = true;

		for (unsigned j{}; j < mask.length(); j++) {
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