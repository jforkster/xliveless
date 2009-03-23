// XLiveLess - xlive.dll replacement with integrated ASI loader
// This file donated to the public domain.
#include <windows.h>
#include <psapi.h>
#include <stdlib.h>

#ifndef NO_TRACE
#include <stdio.h>
CRITICAL_SECTION d_lock;
static FILE * logfile;
extern "C" __declspec(dllexport) void trace (char * message, ...) {
	if (!logfile)
		return;
	EnterCriticalSection (&d_lock);
	SYSTEMTIME	t;
	GetLocalTime (&t);	
	fprintf (logfile, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	va_list	arg;	
	va_start (arg, message);
	vfprintf (logfile, message, arg);
	// fflush (logfile);
	va_end (arg); 
	LeaveCriticalSection (&d_lock);
}
#else
extern "C" __declspec(dllexport) void trace(char * message, ...) {}
#endif

extern "C" __declspec(dllexport) DWORD dwGameVersion = 0x00000000;	// 0 - unknown
extern "C" __declspec(dllexport) DWORD	dwLoadOffset = 0x400000;	

// Delphi don't support importing variables from DLL
extern "C" __declspec(dllexport) DWORD getGameVersion () { return dwGameVersion; }
extern "C" __declspec(dllexport) DWORD getLoadOffset () { return dwLoadOffset; }

extern "C" __declspec(dllexport) void injectFunction (DWORD dwAddress, DWORD pfnReplacement) {
	dwAddress += dwLoadOffset;
	BYTE * patch = (BYTE *)dwAddress;
	*patch = 0xE9;	// JMP
	*(DWORD *)(patch+1) = (pfnReplacement-(dwAddress+5));	
}


// === Start of xlive functions ===
extern "C"  int __stdcall xlive_1 (WORD wVersionRequested, LPWSADATA lpWsaData) { // XWSAStartup
	lpWsaData->wVersion = 2;
	trace ("xlive_1: XWSAStartup\n");
	return 0;
}

extern "C"  void __stdcall xlive_2 () {	// XWSACleanup
	trace ("xlive_2: XWSACleanup called\n");
}

extern "C"  SOCKET __stdcall xlive_3 (int af, int type, int protocol) { // XSocketCreate
	trace ("xlive_3: XCreateSocket (%d, %d, %d)\n", af, type, protocol);
	return INVALID_SOCKET;
}

extern "C"  int __stdcall xlive_4 (SOCKET s) {	// XSocketClose
	trace ("xlive_4: XSockeClose)\n");
	return 0;
}

extern "C"  int __stdcall xlive_5 (SOCKET s, int how) {	// XSocketShutdown
	trace ("xlive_5: XSocketShutdown\n");
	return 0;
}

extern "C"  int __stdcall xlive_6 (SOCKET s, long cmd, long * argp) { // XSocketIOCTLSocket
	trace ("xlive_6: XSocketIOCTLSocket\n");
	return 0;
}

extern "C"  int __stdcall xlive_7 (SOCKET s, DWORD, DWORD, DWORD, DWORD) {	// XSocketSetSockOpt	
	trace ("xlive_7: XSocketSetSockOpt\n");
	return 0;
}

extern "C"  int __stdcall xlive_9 (SOCKET s, sockaddr_in * name, int * namelen) { // XSocketGetSockName
	trace ("xlive_9: XSocketGetSockName\n");
	if (namelen && name && *namelen == sizeof (sockaddr_in)) 
		memset (name, 0, sizeof (sockaddr_in));
	return 0;
}

extern "C"  SOCKET __stdcall xlive_11 (SOCKET s, sockaddr_in * addr, int * addrlen) {// XSocketBind
	trace ("xlive_11: XSocketBind\n");
	return INVALID_SOCKET;
}

extern "C"  int __stdcall xlive_12 (SOCKET s, sockaddr_in * addr, int * addrlen) {// XSocketConnect
	trace ("xlive_12: XSocketConnect\n");
	return 0;
}

extern "C"  int __stdcall xlive_13 (SOCKET s, int backlog) { // XSocketListen
	trace ("xlive_13: XSocketListen\n");
	return 0;
}

extern "C"  SOCKET __stdcall xlive_14 (SOCKET s, sockaddr_in * addr, int * addrlen) { // XSocketAccept
	trace ("xlive_14: XSocketAccept\n");
	return INVALID_SOCKET;
}

extern "C"  int __stdcall xlive_15 (int n, void *, void *, void *, void *) { // XSocketSelect
	trace ("xlive_15: XSocketSelect\n");
	return 0;
}

extern "C"  int __stdcall xlive_18 (SOCKET s, char * buf, int len, int flags) { // XSocketRecv
	return 0;
}

extern "C"  int __stdcall xlive_20 (SOCKET s, char * buf, int len, int flags, sockaddr_in * from, int fromlen) { // XSocketRecvFrom
	return 0;
}

extern "C"  int __stdcall xlive_22 (SOCKET s, char * buf, int len, int flags) { // XSocketSend
	return 0;
}

extern "C"  int __stdcall xlive_24 (SOCKET s, char * buf, int len, int flags, sockaddr_in * to, int tolen) { // XSocketSendTo
	return 0;
}

extern "C"  int __stdcall xlive_26 (char *) { // XSocketInet_Addr
	trace ("xlive_26: XSocketInet_Addr\n");
	return 0;
}

extern "C"  int __stdcall xlive_27 () { // XWSAGetLastError
	return WSAENETDOWN; // 0 ?
}

extern "C"  WORD __stdcall xlive_38 (WORD n) {	// XSocketNTOHS
	return ((n&0xFF00) >> 8)|((n&0xFF) << 8);
}

extern "C"  DWORD __stdcall xlive_39 (DWORD n) { // XSocketNTOHL
	return ((n&0xFF000000) >> 24)|((n & 0x00FF0000) >> 8)|((n&0x0000FF00) << 8)|((n & 0x000000FF) << 24);
}

extern "C"  int __stdcall xlive_51 (void *) { // XNetStartup(XNetStartupParams *)
	trace ("xlive_51: XNetStartup\n");
	return 0;
}

extern "C"  int __stdcall xlive_52 () { // XNetCleanup
	trace ("xlive_52: XNetCleanup\n");
	return 0;
}

extern "C"  int __stdcall xlive_57 (DWORD, DWORD, DWORD * p) { // XNetXnAddrToInAddr
	*p = 0;
	return 0;
}

extern "C"  DWORD __stdcall xlive_58 (DWORD, DWORD, DWORD) { // XNetServerToInAddr
	return 0;
}

extern "C"  int __stdcall xlive_63 (DWORD) {	// XNetUnregisterInAddr
	return 0;
}

extern "C"  int __stdcall xlive_66 (DWORD) { // XNetGetConnectStatus
	trace ("xlive_66: XNetGetConnectStatus\n");
	return 0;	
}

extern "C"  DWORD __stdcall xlive_69 (DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_70 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_72 (DWORD) {	return 0; }

extern "C"  DWORD __stdcall xlive_73 (DWORD * pAddr) {	// XNetGetTitleXnAddr
	// trace ("xlive_73: XNetGetTitleXnAddr\n");	// don't uncomment, unless you want to get very long trace log
	*pAddr = 0x0100007F;	// 127.0.0.1
	return 4; 
}

extern "C"  DWORD __stdcall xlive_75 () { // XNetGetEthernetLinkStatus
	// trace ("xlive_75 (XNetGetEthernetLinkStatus)\n");	// don't uncomment, unless you want to get very long trace log
	return 1; 
}

extern "C"  DWORD __stdcall xlive_84 (DWORD) { return 0; }

extern "C"  int __stdcall xlive_651 (DWORD, DWORD, DWORD, DWORD) {	// XNotifyGetNext
	return 0;
}

extern "C"  DWORD __stdcall xlive_1082 (void *) { // XGetOverlappedExtendedError
	trace ("xlive_1082: XGetOverlappedExtendedError\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_1083 (void *, DWORD * pResult, DWORD) { // XGetOverlappedResult
	if (pResult)
		*pResult = 0;	// 0 elements enumerated
	trace ("xlive_1083: XGetOverlappedResult\n");
	return 0;
}

extern "C"  int __stdcall xlive_5000 (DWORD) {	// XLiveInitialize(struct _XLIVE_INITIALIZE_INFO *)
	trace ("xlive_5000: XLiveInitialize\n");
	return 0;
}

extern "C"  int __stdcall xlive_5001 (DWORD * p) {
	// trace ("xlive_5001: XLiveInput\n");
	p[5] = 0;
	return 1;	// -1 ?
}


extern "C"  int __stdcall xlive_5002 () {
//	trace ("xlive_5002: XLiveRender\n");
	return 0;
}

extern "C"  int __stdcall xlive_5003 () { 
	trace ("xlive_5003: XLiveUninitialize\n");
	return 0;
}

extern "C"  int __stdcall xlive_5005 (DWORD, DWORD) {
	trace ("xlive_5005: XLiveOnCreateDevice\n");
	return 0;
}

extern "C"  int __stdcall xlive_5007 (DWORD) {
	trace ("xlive_5007: XLiveOnResetDevice\n");
	return 0;
}

extern "C"  int __stdcall xlive_5008 (DWORD, DWORD, DWORD) { // XHVCreateEngine
	trace ("xlive_5008 (XHVCreateEngine)\n");
	return -1;	// disable live voice
}

extern "C"  int __stdcall xlive_5022 (DWORD) {
	trace ("xlive_5022: XLiveGetUpdateInformation\n");
	return -1; // no update
}

extern "C"  int __stdcall xlive_5024 (DWORD) {
	trace ("xlive_5024: XLiveUpdateSystem\n");
	return -1; // no update
}

extern "C"  int __stdcall xlive_5030 (DWORD) {	// XLivePreTranslateMessage
	return 0;
}


extern "C"  int __stdcall xlive_5214 (DWORD, DWORD, DWORD) {
	trace ("xlive_5214: XShowPlayerReviewUI\n");
	return 0;
}

extern "C"  int __stdcall xlive_5215 (DWORD) {
	trace ("xlive_5215: XShowGuideUI\n");
	return 1;
}

extern "C"  int __stdcall xlive_5251 (DWORD) {
	trace ("xlive_5251: XCloseHandle\n");
	return 0;
}

extern "C"  int __stdcall xlive_5252 (DWORD, DWORD, DWORD) {
	trace ("xlive_5252: XShowGamerCardUI\n");
	return 0;
}
extern "C"  int __stdcall xlive_5254 (DWORD) {
	trace ("xlive_5254: XCancelOverlapped\n");
	return 0;
}

extern "C"  int __stdcall xlive_5256 (HANDLE hEnum, void * pvBuffer, DWORD cbBuffer, DWORD *, void * pOverlapped) { // XEnumerate
	trace ("xlive_5256: XEnumerate\n");
	return 0;	// some error ? 
}

extern "C"  int __stdcall xlive_5260 (DWORD, DWORD) { // XShowSigninUI
	trace ("xlive_5260: XShowSigninUI\n");
	return 0;
}


extern "C"  int __stdcall xlive_5261 (DWORD, DWORD * pXuid) { // XUserGetXUID
	pXuid[0] = pXuid[1] = 0x10001000; 
	return 0; // ???
}


extern "C"  int __stdcall xlive_5262 (DWORD) {
	trace ("xlive_5262: XUserGetSigninState\n");
	return 1; // eXUserSigninState_SignedInLocally
}


extern "C"  int __stdcall xlive_5263 (DWORD dwUserId, char * pBuffer, DWORD dwBufLen) {
	trace ("xlive_5263: XUserGetName (%d, .. , %d)\n", dwUserId, dwBufLen);
	if (dwBufLen < 8)
		return 1;
	memcpy (pBuffer, "Player1", 8);
	return 0;
}

extern "C"  int __stdcall xlive_5264 (DWORD, DWORD, DWORD, DWORD, DWORD) {
	return 0;
}

extern "C"  int __stdcall xlive_5265 (DWORD user, DWORD priv, PBOOL b) {
	*b = false;
	return 0;
}

extern "C"  int __stdcall xlive_5267 (DWORD dwUser, DWORD type, DWORD * pInfo) { // XUserGetSigninInfo
	pInfo[0] = pInfo[1] = type == 2 ? (dwUser+1)*0x10001000 : 0; // some arbitrary id for offline user, INVALID_XUID for online user
	return 0;
}

extern "C"  HANDLE __stdcall xlive_5270 (DWORD l, DWORD h) {
	trace ("xlive_5270: XNotifyCreateListener (0x%08x%08x)\n", h, l);
	return (HANDLE)1; // any non-zero value. (zero treated as fatal error)
}

extern "C"  int __stdcall xlive_5276 (DWORD, DWORD, DWORD, DWORD) {
	return 0;
}

extern "C"  int __stdcall xlive_5277 (DWORD, DWORD, DWORD) {
	return 0;
}

extern "C"  DWORD __stdcall xlive_5278 (DWORD, DWORD, DWORD) {
	return 0;
}

extern "C"  DWORD __stdcall xlive_5280 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum) {
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5281 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD * pcbResults, DWORD * pResults, void *) { // XUserReadStats
	*pcbResults = 4;
	*pResults = 0;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5284 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum) { 
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5286 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum) { 
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

extern "C"  int __stdcall xlive_5292 (DWORD, DWORD, DWORD, DWORD) {
	return 0;
}

extern "C" int __stdcall xlive_5297 (void *, int) {
	trace ("xlive_5297: XLiveInitializeEx\n");
	return 0;
}
	

extern "C"  DWORD __stdcall xlive_5300 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("xlive_5300: XSessionCreate\n");
	return -1;
}

extern "C"  DWORD __stdcall xlive_5303 (DWORD, DWORD, DWORD, DWORD, DWORD, WORD * pResult, DWORD) { // XStringVerify
	*pResult = 0;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5305 (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	return 0;
}

extern "C"  int __stdcall xlive_5310 () { 
	trace ("xlive_5310: XOnlineStartup\n");
	return 0; 
}

extern "C"  int __stdcall xlive_5311 () {
	trace ("xlive_5310: XOnlineCleanup\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_5312 (DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  int __stdcall xlive_5314 (DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  int __stdcall xlive_5315 (DWORD, DWORD) { return 1; }
extern "C"  int __stdcall xlive_5316 (DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5317 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }

extern "C"  int __stdcall xlive_5318 (DWORD, DWORD, DWORD) { // XSessionStart
	trace ("xlive_5318: XSessionStart\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_5319 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5322 (DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5323 (DWORD, DWORD, DWORD, DWORD) {  return 0; }
extern "C"  int __stdcall xlive_5324 () { return 0; }
extern "C"  DWORD __stdcall xlive_5325 (DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5326 (DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5327 (DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5328 (DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  int __stdcall xlive_5329 (DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5330 (DWORD, DWORD) { return 0; }

struct XUSER_READ_PROFILE_SETTINGS {
	DWORD	dwLength;
	BYTE *	pSettings;
};

extern "C"  DWORD __stdcall xlive_5331 (DWORD dwTitleId, DWORD dwUserIndex, DWORD dwNumSettingIds, 
					DWORD * pdwSettingIds, DWORD * pcbResults, XUSER_READ_PROFILE_SETTINGS * pResults, DWORD pOverlapped) {
	trace ("xlive_5331: XUserReadProfileSettings (%d, %d, %d, ..., %d, ...)\n", dwTitleId, dwUserIndex, dwNumSettingIds, *pcbResults);
	if (*pcbResults < 1036) {
		*pcbResults = 1036;	// TODO: make correct calculation by IDs.
		return ERROR_INSUFFICIENT_BUFFER;
	}
	memset (pResults, 0, *pcbResults);
	pResults->dwLength = *pcbResults-sizeof (XUSER_READ_PROFILE_SETTINGS);
	pResults->pSettings = (BYTE *)pResults+sizeof (XUSER_READ_PROFILE_SETTINGS);
	return 0;
}

extern "C"  int __stdcall xlive_5332 (DWORD, DWORD) {	// XSessionEnd
	trace ("xlive_5332: XSessionEnd\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_5333 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5335 (DWORD, DWORD, DWORD, PHANDLE phEnum) {
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5336 (DWORD, DWORD, DWORD, DWORD) { return 0; }

extern "C"  DWORD __stdcall xlive_5337 (DWORD, DWORD, DWORD, DWORD) {
	trace ("xlive_5337: XUserWriteProfileSettings\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_5344 (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { return 0; }
extern "C"  DWORD __stdcall xlive_5345 (DWORD dwUserIndex, DWORD, DWORD, DWORD, DWORD, DWORD, void * ) { return 0; }

// === replacements ===
struct FakeProtectedBuffer {
	DWORD	dwMagick;	
	int		nSize;
	DWORD	__fill[2]; // To match buffer size in Rick's wrapper
	BYTE	bData[4];
};

extern "C"  DWORD __stdcall xlive_5016 (int size, FakeProtectedBuffer ** pBuffer) {
	// trace ("xlive_5016: XLivePBufferAllocate (%d)\n", size);
	*pBuffer = (FakeProtectedBuffer *)malloc (size+16);
	if (!*pBuffer) {
		trace ("ERROR: XLivePBufferAllocate unable to allocate %d bytes\n", size);
		return E_OUTOFMEMORY;
	}

	(*pBuffer)->dwMagick = 0xDEADDEAD;	// some arbitrary number
	(*pBuffer)->nSize = size;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5017 (FakeProtectedBuffer * pBuffer) {
	// trace ("xlive_5017: XLivePBufferFree\n");
	if (pBuffer && pBuffer->dwMagick == 0xDEADDEAD)
		free (pBuffer);
	return 0;
}

extern "C"  DWORD __stdcall xlive_5295 (FakeProtectedBuffer * pBuffer, int offset, BYTE * source, int size) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || !source || offset < 0 || offset+size > pBuffer->nSize)
		return 0;
	memcpy (pBuffer->bData+offset, source, size);
	return 0;
}

extern "C"  DWORD __stdcall xlive_5294 (FakeProtectedBuffer * pBuffer, int offset, BYTE * destination, int size) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || !destination || offset < 0 || offset+size > pBuffer->nSize)
		return 0;
	memcpy (destination, pBuffer->bData+offset, size);
	return 0;
}

extern "C"  DWORD __stdcall xlive_5019 (FakeProtectedBuffer * pBuffer, int offset, BYTE value) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || offset < 0 || offset > pBuffer->nSize)
		return 0;
	pBuffer->bData[offset] = value;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5018 (FakeProtectedBuffer * pBuffer, int offset, BYTE * value) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || !value || offset < 0 || offset > pBuffer->nSize)
		return 0;
	*value = pBuffer->bData[offset];
	return 0;
}

extern "C"  DWORD __stdcall xlive_5036 (DWORD * dwType, PHANDLE pHandle) {
	trace ("xlive_5036: XLiveCreateProtectedDataContext\n");
	if (pHandle)
		*pHandle = (HANDLE)1;
	return 0;
}

extern "C"  DWORD __stdcall xlive_5037 (HANDLE h, DWORD * p) {
	trace ("xlive_5037 (XLiveQueryProtectedDataInformation)\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_5038 (HANDLE h) {
	trace ("xlive_5038 (XLiveCloseProtectedDataContext)\n");
	return 0;
}

extern "C"  DWORD __stdcall xlive_5035 (BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h) {
	trace ("xlive_5035: XLiveUnprotectData (..., %d, ..., %d, %d)\n", dwInDataSize, *pDataSize, (DWORD)h);
	*pDataSize = dwInDataSize;
	if (*pDataSize >= dwInDataSize && pOutBuffer)
		memcpy (pOutBuffer, pInBuffer, dwInDataSize);
	return 0;
}

extern "C"  DWORD __stdcall xlive_5034 (BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h) {
	trace ("xlive_5035: XLiveProtectData (..., %d, ..., %d, %d)\n", dwInDataSize, *pDataSize, (DWORD)h);
	*pDataSize = dwInDataSize;
	if (*pDataSize >= dwInDataSize && pOutBuffer)
		memcpy (pOutBuffer, pInBuffer, dwInDataSize);
	return 0;
}
// === end of xlive functions ===

// change savefile path to "%USERPROFILE%\Documents\Rockstar Games\GTA IV\savegames\"
void getSavefilePath (int __unused, char * pBuffer, char * pszSaveName) {
	char * pszPath = (char *)(0xFA0308+dwLoadOffset);	// char pszPathPersonal[128]; // "%USERPROFILE%\Documents\Rockstar Games\GTA IV\"
	
	if (dwGameVersion == 0x00010001)
		pszPath = (char *)(0xFA7878+dwLoadOffset);	// char pszPathPersonal[128]; // "%USERPROFILE%\Documents\Rockstar Games\GTA IV\"
        else if (dwGameVersion == 0x00010003)       
		pszPath = (char *)(0xFBF860+dwLoadOffset);	// char pszPathPersonal[128]; // "%USERPROFILE%\Documents\Rockstar Games\GTA IV\"

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

// === miscellaneous patches ===

void patch101 () {
	dwGameVersion = 0x00010001;	// GTA IV 1.0.1 (patch 1)

	DWORD oldProtect;
	// enable write access to code and rdata
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x1000+0x400000), 0x8D8000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		trace ("ERROR: unable to unprotect code seg\n");
		return;
	}
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x8D9000+0x400000), 0x1A8000, PAGE_READWRITE, &oldProtect)) 
		trace ("ERROR: unable to unprotect .rdata seg\n");

	// process patches
	*(WORD *)(0x608C35+dwLoadOffset) = 0x9090; // NOP; NOP	- save file CRC32 check
	*(BYTE *)(0x7A14B0+dwLoadOffset) = 0xC3;	// RETN - enable debugger in error menu (don't load WER.dll)
	*(BYTE *)(0x470DB0+dwLoadOffset) = 0xC3;	// RETN - skip files.txt hash check
	*(WORD *)(0x4702FF+dwLoadOffset) = 0x9090;	// NOP; NOP - another files.txt hash check
	*(BYTE *)(0x7A2530+dwLoadOffset) = 0xC3;	// RETN - skip files.txt hash check

	// disable RGSC (no more this stupid window!)
	*(BYTE *)(0x46FB80+dwLoadOffset) = 0xC3;	// RETN - remove connect to the RGSC 
	*(WORD *)(0x47030A+dwLoadOffset) = 0x9090;	// NOP; NOP - RGSC initialization check
	*(WORD *)(0x47030F+dwLoadOffset) = 0x9090;	// NOP; NOP - RGSC initialization check
	*(BYTE *)(0x470317+dwLoadOffset) = 0x1D;	// mov dword_15B47DC, esi => mov ..., ebx - another RGSC related object 

	// replace getSavefilePath
	injectFunction (0x608660, (DWORD)getSavefilePath);

	trace ("Patching OK (1.0.1)\n");
}

void patch103 () {
	dwGameVersion = 0x00010003;	// GTA IV 1.0.3 (patch 3)


	DWORD oldProtect;
	// enable write access to code and rdata
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x1000+0x400000), 0x8E4000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		trace ("ERROR: unable to unprotect code seg\n");
		return;
	}

	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x8e5000+0x400000), 0x1B1000, PAGE_READWRITE, &oldProtect)) 
		trace ("ERROR: unable to unprotect .rdata seg\n");


	// process patches
	*(WORD *)(0x60C095+dwLoadOffset) = 0x9090; 	// NOP; NOP - save file CRC32 check
	*(BYTE *)(0x7A3A70+dwLoadOffset) = 0xC3;	// RETN - enable debugger in error menu (don't load WER.dll)
	*(BYTE *)(0x7A4750+dwLoadOffset) = 0xC3;	// RETN - skip files.txt hash check

	*(BYTE  *)(0x471CBD+dwLoadOffset) = 0xE9;	// JMP
	*(DWORD *)(0x471CBE+dwLoadOffset) = 0x16;	// jmp target
	*(BYTE  *)(0x472E50+dwLoadOffset) = 0xC3;	// RETN - certificates check
	*(BYTE  *)(0x471360+dwLoadOffset) = 0xC2;	// RETN - remove connect to the RGSC 
	*(BYTE  *)(0x471361+dwLoadOffset) = 0x04;	// RETN - remove connect to the RGSC 
	*(BYTE  *)(0x471362+dwLoadOffset) = 0x00;	// RETN - remove connect to the RGSC 
	memset ((BYTE *)(0x471D60+dwLoadOffset), 0x90, 0x1B);
	*(WORD  *)(0x471CEA+dwLoadOffset) = 0x9090;	// NOP; NOP - RGSC initialization check
        *(DWORD *)(0x471D80+dwLoadOffset) = 0x90909090;	// NOP*4- last RGSC init check
        *(WORD  *)(0x471D84+dwLoadOffset) = 0x9090;	// NOP*4- last RGSC init check
	*(WORD *)(0x471CF1+dwLoadOffset) = 0xC033;	// XOR eax, eax - RGSC initialization check
	*(WORD *)(0x471CF8+dwLoadOffset) = 0xA390;	// NOP; MOV [], eax

	*(DWORD *)(0x401815+dwLoadOffset) = 1;		// disable sleep

	injectFunction (0x60BAC0, (DWORD)getSavefilePath); // replace getSavefilePath

        memset ((BYTE *)(0x451E19+dwLoadOffset), 0x90, 0x1BF);	// EFC20
	*(DWORD *)(0xA0D1E0+dwLoadOffset) = 0x90C301B0;	// mov al, 1; retn
	*(DWORD *)(0xA0D200+dwLoadOffset) = 0x90C301B0;
	*(DWORD *)(0xA0D210+dwLoadOffset) = 0x90C301B0;
	*(DWORD *)(0xA0D240+dwLoadOffset) = 0x90C301B0;

// B001C3 => 0xC301B090

	trace ("Patching OK (1.0.3)\n");
}
void patch102 () {
	dwGameVersion = 0x00010002;	// GTA IV 1.0.2 (patch 2)

	DWORD oldProtect;
	// enable write access to code and rdata
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x1000+0x400000), 0x8D7000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		trace ("ERROR: unable to unprotect code seg\n");
		return;
	}
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x8D8000+0x400000), 0x19F000, PAGE_READWRITE, &oldProtect)) 
		trace ("ERROR: unable to unprotect .rdata seg\n");

	// process patches
	*(WORD *)(0x6086B5+dwLoadOffset) = 0x9090; 	// NOP; NOP - save file CRC32 check
	*(BYTE *)(0x79F710+dwLoadOffset) = 0xC3;	// RETN - enable debugger in error menu (don't load WER.dll)
	*(BYTE *)(0x7A0740+dwLoadOffset) = 0xC3;	// RETN - skip files.txt hash check

	*(BYTE *)(0x46FA7D+dwLoadOffset) = 0xE9;	// JMP
	*(DWORD *)(0x46FA7E+dwLoadOffset)= 0x16;	// jmp target
	*(BYTE *)(0x4701C0+dwLoadOffset) = 0xC3;	// RETN - certificates check
	*(BYTE *)(0x46F120+dwLoadOffset) = 0xC2;	// RETN - remove connect to the RGSC 
	*(BYTE *)(0x46F121+dwLoadOffset) = 0x04;	// RETN - remove connect to the RGSC 
	*(BYTE *)(0x46F122+dwLoadOffset) = 0x00;	// RETN - remove connect to the RGSC 
	memset ((BYTE *)(0x46FB16+dwLoadOffset), 0x90, 0x1B);
	*(WORD *)(0x46FAAA+dwLoadOffset) = 0x9090;	// NOP; NOP - RGSC initialization check
        *(WORD *)(0x46FB36+dwLoadOffset) = 0x9090;	// NOP; NOP - last RGSC init check
        *(DWORD *)(0x46FB38+dwLoadOffset) = 0x90909090;
	*(WORD *)(0x46FAB1+dwLoadOffset) = 0xC033;	// XOR eax, eax - RGSC initialization check
	*(WORD *)(0x46FAB8+dwLoadOffset) = 0xA390;	// NOP; MOV [], eax

	*(DWORD *)(0x401825+dwLoadOffset) = 1;		// disable sleep

	// replace getSavefilePath
	injectFunction (0x6080E0, (DWORD)getSavefilePath);

	trace ("Patching OK (1.0.2)\n");
}

void patchCode () {
	// get load address of the exe
	dwLoadOffset = (DWORD)GetModuleHandle (NULL);
	trace ("GetModuleHandle returns %08x\n", dwLoadOffset);
	dwLoadOffset -= 0x400000;	
	
	// version check
	DWORD signature = *(DWORD *)(0x608C34+dwLoadOffset);
	if (signature == 0x831F7518)
		patch101 ();
	else if (signature == 0xC483FFE4) 
		patch102 ();
	else if (signature == 0x280F0000)
		patch103 ();
	else 
		trace ("Unknown game version, skipping patches (signature = 0x%08x)\n", signature);
}

// Plugin Loader
void loadPlugins (char * pszMask) {	
	if (!pszMask)
		return;
	char * pszType = strrchr (pszMask, '.');
	DWORD typeMask = pszType ? *(DWORD *)pszType : 0x6c6c642e; // '.dll'

	WIN32_FIND_DATA fd;
	char pathName[MAX_PATH];	// module name buffer
	char * p = strrchr (pszMask, '\\');
	char * namePtr = pathName;
	if (p) {
		strcpy (pathName, pszMask);
		pathName[p-pszMask+1] = '\0';
		namePtr = pathName + (p-pszMask+1);
	}


	HANDLE asiFile = FindFirstFile (pszMask, &fd);
	if (asiFile == INVALID_HANDLE_VALUE)
		return;
	do {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			int pos = 0;
			while (fd.cFileName[pos])
				pos++;
			DWORD type = *(DWORD *)(fd.cFileName+pos-4);
			type |= 0x20202020;
			if (type == typeMask) {
				strcpy (namePtr, fd.cFileName);
				if (!LoadLibrary (pathName)) 
					trace ("Error loading library %d\n", GetLastError ());
				trace ("plugin loader: loaded '%s'\n", pathName);
			}

		}
	} while (FindNextFile (asiFile, &fd));
	FindClose (asiFile);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
#ifndef NO_TRACE
		logfile = fopen ("xlive_trace.log", "at");
		if (logfile)
			InitializeCriticalSection (&d_lock);
		trace ("Log started (xliveless 0.95)\n");
#endif
		patchCode ();
		loadPlugins ("*.asi");
		loadPlugins ("plugins\\*.asi");
		loadPlugins ("plugins\\*.dll");
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
#ifndef NO_TRACE
		if (logfile) {
			fclose (logfile);
			DeleteCriticalSection (&d_lock);
		}
#endif
		break;
	}
    return TRUE;
}

