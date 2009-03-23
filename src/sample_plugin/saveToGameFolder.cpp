#include <windows.h>
#include "../xliveless.h"

void getSavefilePath (int __unused, char * pBuffer, char * pszSaveName) {
	char * pszPath = (char *)(0xF9FF08+dwLoadOffset);
	if (dwGameVersion == 0x00010001)	
		pszPath = (char *)(0xFA7778+dwLoadOffset);	// char pszProgramPath[128]; 
        else if (dwGameVersion == 0x00010003)
		pszPath = (char *)(0xFBF260+dwLoadOffset);
	strcpy_s (pBuffer, 256, pszPath);
	strcat_s (pBuffer, 256, "savegames");

	// check path and create directory if necessary
	DWORD attrs = GetFileAttributes (pBuffer);
	if (attrs == INVALID_FILE_ATTRIBUTES) 
		CreateDirectory (pBuffer, NULL);
	else if (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
		trace ("ERROR: unable to create directory '%s', file '%s' already exists\n", pBuffer);
		strcpy (pBuffer, pszSaveName);
		return;
	}
	if (pszSaveName) {
		strcat_s (pBuffer, 256, "\\");
		strcat_s (pBuffer, 256, pszSaveName);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (dwGameVersion == 0x00010001)
			injectFunction (0x608660, (DWORD)getSavefilePath);
		else if (dwGameVersion == 0x00010002)
			injectFunction (0x6080E0, (DWORD)getSavefilePath);
		else if (dwGameVersion == 0x00010003)
			injectFunction (0x60BAC0, (DWORD)getSavefilePath);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
