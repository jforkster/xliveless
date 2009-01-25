// XLiveLess plugin API

extern "C" {
	// game version 
	__declspec(dllimport) DWORD dwGameVersion;
	// Offset to the "real" loding address 
	__declspec(dllimport) DWORD dwLoadOffset;

	// for Delphi plugins (same as two variables above)
	__declspec(dllimport) DWORD getGameVersion();
	__declspec(dllimport) DWORD getLoadOffset ();

	// Print message to the log 
	void trace(char * message, ...);

	// Replace game function at dwAddress with plugin function
	void injectFunction (DWORD dwAddress, DWORD pfnReplacement);
}