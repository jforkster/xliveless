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
	if (!logfile)
		return;
	SYSTEMTIME	t;
	GetLocalTime (&t);	
	fprintf (logfile, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	va_list	arg;	
	va_start (arg, message);
	vfprintf (logfile, message, arg);
 fflush (logfile);
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
// #1: XWSAStartup
extern "C"  int __stdcall XWSAStartup (WORD wVersionRequested, LPWSADATA lpWsaData) { 
	lpWsaData->wVersion = 2;
	trace ("XWSAStartup \n");
	return 0;
}

// #2: XWSACleanup
extern "C"  void __stdcall XWSACleanup () {	// XWSACleanup
	trace ("XWSACleanup\n");
}

// #3: XCreateSocket
extern "C"  SOCKET __stdcall XCreateSocket (int af, int type, int protocol) { 
	trace ("XCreateSocket (%d, %d, %d)\n", af, type, protocol);
	return INVALID_SOCKET;
}

// #4: XSockeClose
extern "C"  int __stdcall XSockeClose (SOCKET s) {	
	trace ("XSockeClose)\n");
	return 0;
}

// #5: XSocketShutdown
extern "C"  int __stdcall XSocketShutdown (SOCKET s, int how) {	
	trace ("XSocketShutdown\n");
	return 0;
}

// #6: XSocketIOCTLSocket
extern "C"  int __stdcall XSocketIOCTLSocket (SOCKET s, long cmd, long * argp) {
	trace ("XSocketIOCTLSocket\n");
	return 0;
}

// #7: XSocketSetSockOpt
extern "C"  int __stdcall XSocketSetSockOpt (SOCKET s, DWORD, DWORD, DWORD, DWORD) {
	trace ("XSocketSetSockOpt\n");
	return 0;
}

// #9: XSocketGetSockName
extern "C"  int __stdcall XSocketGetSockName (SOCKET s, sockaddr_in * name, int * namelen) {
	trace ("XSocketGetSockName\n");
	if (namelen && name && *namelen == sizeof (sockaddr_in)) 
		memset (name, 0, sizeof (sockaddr_in));
	return 0;
}

// #11: XSocketBind
extern "C"  SOCKET __stdcall XSocketBind (SOCKET s, sockaddr_in * addr, int * addrlen) {
	trace ("XSocketBind\n");
	return INVALID_SOCKET;
}

// #12: XSocketConnect
extern "C"  int __stdcall XSocketConnect (SOCKET s, sockaddr_in * addr, int * addrlen) {
	trace ("XSocketConnect\n");
	return 0;
}

// #13: XSocketListen
extern "C"  int __stdcall XSocketListen (SOCKET s, int backlog) { 
	trace ("XSocketListen\n");
	return 0;
}

// #14: XSocketAccept
extern "C"  SOCKET __stdcall XSocketAccept (SOCKET s, sockaddr_in * addr, int * addrlen) { 
	trace ("XSocketAccept\n");
	return INVALID_SOCKET;
}

// #15: XSocketSelect
extern "C"  int __stdcall XSocketSelect (int n, void *, void *, void *, void *) { 
	trace ("XSocketSelect\n");
	return 0;
}

// #18: XSocketRecv
extern "C"  int __stdcall XSocketRecv (SOCKET s, char * buf, int len, int flags) { 
	return 0;
}

// #20: XSocketRecvFrom
extern "C"  int __stdcall XSocketRecvFrom (SOCKET s, char * buf, int len, int flags, sockaddr_in * from, int fromlen) { 
	return 0;
}

// #22: XSocketSend
extern "C"  int __stdcall XSocketSend (SOCKET s, char * buf, int len, int flags) {
	return 0;
}

// #24: XSocketSendTo
extern "C"  int __stdcall XSocketSendTo (SOCKET s, char * buf, int len, int flags, sockaddr_in * to, int tolen) { 
	return 0;
}

// #26: XSocketInet_Addr
extern "C"  int __stdcall XSocketInet_Addr (char *) { 
	trace ("XSocketInet_Addr\n");
	return 0;
}

// #27: XWSAGetLastError
extern "C"  int __stdcall XWSAGetLastError () {
	return WSAENETDOWN; // 0 ?
}

// #38: XSocketNTOHS
extern "C"  WORD __stdcall XSocketNTOHS (WORD n) {	
	return ((n&0xFF00) >> 8)|((n&0xFF) << 8);
}

// #39: XSocketNTOHL
extern "C"  DWORD __stdcall XSocketNTOHL (DWORD n) { 
	return ((n&0xFF000000) >> 24)|((n & 0x00FF0000) >> 8)|((n&0x0000FF00) << 8)|((n & 0x000000FF) << 24);
}

// #51: XNetStartup
extern "C"  int __stdcall XNetStartup (void *) { // XNetStartup(XNetStartupParams *)
	trace ("XNetStartup\n");
	return 0;
}

// #52: XNetCleanup
extern "C"  int __stdcall XNetCleanup () { 
	trace ("xlive_52: XNetCleanup\n");
	return 0;
}

// #54: XNetCreateKey
extern "C" int __stdcall XNetCreateKey (void * pxnkid, void * pxnkey) { 
	trace ("XNetCreateKey\n");
	return 0;
}

// #55: XNetRegisterKey
extern "C" int __stdcall XNetRegisterKey (DWORD, DWORD) { 
	return 0;
}

// #56: XNetUnregisterKey
extern "C" int __stdcall XNetUnregisterKey (DWORD) { 
	return 0;
}

// #57: XNetXnAddrToInAddr
extern "C"  int __stdcall XNetXnAddrToInAddr (DWORD, DWORD, DWORD * p) { 
	*p = 0;
	return 0;
}

// #58: XNetServerToInAddr
extern "C"  DWORD __stdcall XNetServerToInAddr (DWORD, DWORD, DWORD) { 
	return 0;
}

// #60: XNetInAddrToXnAddr
extern "C" DWORD __stdcall XNetInAddrToXnAddr (DWORD, DWORD, DWORD) { 
	return 0;
} 

// #63: XNetUnregisterInAddr
extern "C"  int __stdcall XNetUnregisterInAddr (DWORD) {
	return 0;
}

// #65: XNetConnect
extern "C" int __stdcall XNetConnect (DWORD) { 
	return 0;
}

// #66: XNetGetConnectStatus
extern "C"  int __stdcall XNetGetConnectStatus (DWORD) { 
	trace ("XNetGetConnectStatus\n");
	return 0;	
}

// #69: XNetQosListen
extern "C"  DWORD __stdcall XNetQosListen (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	return 0; 
}

// #70: XNetQosLookup
extern "C"  DWORD __stdcall XNetQosLookup (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	return 0; 
}

// #71: XNetQosServiceLookup
extern "C" DWORD __stdcall XNetQosServiceLookup (DWORD, DWORD, DWORD) { 
	return 0;
}

// #72: XNetQosRelease
extern "C"  DWORD __stdcall XNetQosRelease (DWORD) {	
	return 0; 
}

// #73: XNetGetTitleXnAddr
extern "C"  DWORD __stdcall XNetGetTitleXnAddr (DWORD * pAddr) {
	// trace ("xlive_73: XNetGetTitleXnAddr\n");	// don't uncomment, unless you want to get very long trace log
	*pAddr = 0x0100007F;	// 127.0.0.1
	return 4; 
}

// #75: XNetGetEthernetLinkStatus
extern "C"  DWORD __stdcall XNetGetEthernetLinkStatus () { 
	// trace ("xlive_75 (XNetGetEthernetLinkStatus)\n");	// don't uncomment, unless you want to get very long trace log
	return 1; 
}

// #84: XNetSetSystemLinkPort
extern "C"  DWORD __stdcall XNetSetSystemLinkPort (DWORD) { 
	return 0; 
}

// #473: XCustomGetLastActionPress
extern "C" int __stdcall XCustomGetLastActionPress (DWORD, DWORD, DWORD) { 
	trace ("XCustomGetLastActionPress\n");
	return 0;
}

// #651: XNotifyGetNext
extern "C"  int __stdcall XNotifyGetNext (HANDLE hNotification, DWORD dwMsgFilter, DWORD * pdwId, void * pParam) {
//	trace ("XNotifyGetNext\n");	// too noisy
	return 0;
}

// #652: XNotifyPositionUI
extern "C" DWORD __stdcall XNotifyPositionUI (DWORD dwPosition) {
	return 0;
}

// #1082: XGetOverlappedExtendedError
extern "C"  DWORD __stdcall XGetOverlappedExtendedError (void *) { 
	trace ("XGetOverlappedExtendedError\n");
	return 0;
}

// #1083: XGetOverlappedResult
extern "C"  DWORD __stdcall XGetOverlappedResult (void *, DWORD * pResult, DWORD) { 
	if (pResult)
		*pResult = 0;	// 0 elements enumerated
	trace ("XGetOverlappedResult\n");
	return 0;
}

// #5000: XLiveInitialize
extern "C"  int __stdcall XLiveInitialize (DWORD) {	// XLiveInitialize(struct _XLIVE_INITIALIZE_INFO *)
	trace ("XLiveInitialize\n");
	return 0;
}

// #5001: XLiveInput
extern "C"  int __stdcall XLiveInput (DWORD * p) {
	// trace ("XLiveInput\n");
	p[5] = 0;
	return 1;	// -1 ?
}


// #5002: XLiveRender
extern "C"  int __stdcall XLiveRender () {
//	trace ("XLiveRender\n");
	return 0;
}

// #5003: XLiveUninitialize
extern "C"  int __stdcall XLiveUninitialize () { 
	trace ("XLiveUninitialize\n");
	return 0;
}

// #5005: XLiveOnCreateDevice
extern "C"  int __stdcall XLiveOnCreateDevice (DWORD, DWORD) {
	trace ("XLiveOnCreateDevice\n");
	return 0;
}

// #5007: XLiveOnResetDevice
extern "C"  int __stdcall XLiveOnResetDevice (DWORD) {
	trace ("XLiveOnResetDevice\n");
	return 0;
}

// #5008: XHVCreateEngine
extern "C"  int __stdcall XHVCreateEngine (DWORD, DWORD, DWORD) { 
	trace ("XHVCreateEngine\n");
	return -1;	// disable live voice
}

// #5022: XLiveGetUpdateInformation
extern "C"  int __stdcall XLiveGetUpdateInformation (DWORD) {
	trace ("XLiveGetUpdateInformation\n");
	return -1; // no update
}

// #5024: XLiveUpdateSystem
extern "C"  int __stdcall XLiveUpdateSystem (DWORD) {
	trace ("XLiveUpdateSystem\n");
	return -1; // no update
}

// #5030: XLivePreTranslateMessage
extern "C"  int __stdcall XLivePreTranslateMessage (DWORD) {
	return 0;
}

// #5031 XLiveSetDebugLevel
extern "C" int __stdcall XLiveSetDebugLevel (DWORD xdlLevel, DWORD * pxdlOldLevel) { 
	trace ("XLiveSetDebugLevel (%d)\n", xdlLevel);
	return 0;
}

// #5214: XShowPlayerReviewUI
extern "C"  int __stdcall XShowPlayerReviewUI (DWORD, DWORD, DWORD) {
	trace ("XShowPlayerReviewUI\n");
	return 0;
}

// #5215: XShowGuideUI
extern "C"  int __stdcall XShowGuideUI (DWORD) {
	trace ("XShowGuideUI\n");
	return 1;
}

// #5216: XShowKeyboardUI
extern "C" int __stdcall XShowKeyboardUI (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XShowKeyboardUI\n");
	return 0;
}

// #5251: XCloseHandle
extern "C"  int __stdcall XCloseHandle (DWORD) {
	trace ("XCloseHandle\n");
	return 0;
}

// #5252: XShowGamerCardUI
extern "C"  int __stdcall XShowGamerCardUI (DWORD, DWORD, DWORD) {
	trace ("XShowGamerCardUI\n");
	return 0;
}

// #5254: XCancelOverlapped
extern "C"  int __stdcall XCancelOverlapped (DWORD) {
	trace ("XCancelOverlapped\n");
	return 0;
}

// #5256: XEnumerate
extern "C"  int __stdcall XEnumerate (HANDLE hEnum, void * pvBuffer, DWORD cbBuffer, DWORD * pcItemsReturned, void * pOverlapped) { // XEnumerate
	trace ("XEnumerate\n");
	if (pcItemsReturned)
		*pcItemsReturned = 0;
	return 0;	// some error ? 
}

// #5260: XShowSigninUI
extern "C"  int __stdcall XShowSigninUI (DWORD, DWORD) { 
	trace ("XShowSigninUI\n");
	return 0;
}

// #5261: XUserGetXUID
extern "C"  int __stdcall XUserGetXUID (DWORD, DWORD * pXuid) { 
	pXuid[0] = pXuid[1] = 0x10001000; 
	return 0; // ???
}


// #5262: XUserGetSigninState
extern "C"  int __stdcall XUserGetSigninState (DWORD dwUserIndex) {
	trace ("xlive_5262: XUserGetSigninState (%d)\n", dwUserIndex);
	return 1; // eXUserSigninState_SignedInLocally
}

// #5263: XUserGetName
extern "C"  int __stdcall XUserGetName (DWORD dwUserId, char * pBuffer, DWORD dwBufLen) {
	trace ("xlive_5263: XUserGetName (%d, .. , %d)\n", dwUserId, dwBufLen);
	if (dwBufLen < 8)
		return 1;
	memcpy (pBuffer, "Player1", 8);
	return 0;
}

// #5264: XUserAreUsersFriends
extern "C"  int __stdcall XUserAreUsersFriends(DWORD, DWORD, DWORD, DWORD, DWORD) {
	trace ("XUserAreUsersFriends\n");
	return 0;
}

// #5265: XUserCheckPrivilege
extern "C"  int __stdcall XUserCheckPrivilege (DWORD user, DWORD priv, PBOOL b) {
	trace ("XUserCheckPrivilege (%d, %d, ..)\n", user, priv);
	*b = false;
	return 0;
}

struct XUSER_SIGNIN_INFO {
   DWORD		xuidL;
   DWORD		xuidH;
   DWORD                dwInfoFlags;
   DWORD		UserSigninState;
   DWORD                dwGuestNumber;
   DWORD                dwSponsorUserIndex;
   CHAR                 szUserName[16];
};


// #5267: XUserGetSigninInfo
extern "C"  int __stdcall XUserGetSigninInfo (DWORD dwUser, DWORD dwFlags, XUSER_SIGNIN_INFO * pInfo) {  
	trace ("XUserGetSigninInfo (%d, %d, ...)\n", dwUser, dwFlags);
	pInfo->xuidL = pInfo->xuidH = dwFlags != 1 ? (dwUser+1)*0x10001000 : 0; // some arbitrary id for offline user, INVALID_XUID for online user
	if (dwFlags != 1) {
		pInfo->dwInfoFlags = 1;
		pInfo->UserSigninState = 1; // eXUserSigninState_SignedInLocally
//		strcpy (pInfo->szUserName, "Player");
	}
	return 0;
}

// #5270: XNotifyCreateListener
extern "C"  HANDLE __stdcall XNotifyCreateListener (DWORD l, DWORD h) {
	trace ("xlive_5270: XNotifyCreateListener (0x%08x%08x)\n", h, l);
	return (HANDLE)1; // any non-zero value. (zero treated as fatal error)
}

// #5273: XUserReadGamerpictureByKey
extern "C" int __stdcall XUserReadGamerpictureByKey (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XUserReadGamerpictureByKey\n");
	return 0;
}

// #5275: XShowFriendsUI
extern "C" int __stdcall XShowFriendsUI (DWORD) {
	trace ("XShowFriendsUI\n");
	return 0;
}

// #5276: XUserSetProperty
extern "C"  int __stdcall XUserSetProperty (DWORD, DWORD, DWORD, DWORD) {
	trace ("XUserSetProperty\n");
	return 0;
}

// #5277: XUserSetContext
extern "C"  int __stdcall XUserSetContext (DWORD, DWORD, DWORD) {
	trace ("XUserSetContext\n");
	return 0;
}

// #5278: XUserWriteAchievements
extern "C"  DWORD __stdcall XUserWriteAchievements (DWORD, DWORD, DWORD) {
	trace ("XUserWriteAchievements\n");
	return 0;
}

// #5280: XUserCreateAchievementEnumerator
extern "C"  DWORD __stdcall XUserCreateAchievementEnumerator (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum) {
	trace ("XUserCreateAchievementEnumerator\n");
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

// #5281: XUserReadStats
extern "C"  DWORD __stdcall XUserReadStats (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD * pcbResults, DWORD * pResults, void *) { 
	trace ("XUserReadStats\n");	
	*pcbResults = 4;
	*pResults = 0;
	return 0;
}

// #5284: XUserCreateStatsEnumeratorByRank
extern "C"  DWORD __stdcall XUserCreateStatsEnumeratorByRank (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum) { 
	trace ("XUserCreateStatsEnumeratorByRank\n");
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

// #5286: XUserCreateStatsEnumeratorByXuid
extern "C"  DWORD __stdcall XUserCreateStatsEnumeratorByXuid (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum) { 
	trace ("XUserCreateStatsEnumeratorByXuid\n");
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

// #5292: XUserSetContextEx
extern "C"  int __stdcall XUserSetContextEx (DWORD, DWORD, DWORD, DWORD) {
	trace ("XUserSetContextEx\n");
	return 0;
}

// #5293: XUserSetPropertyEx
extern "C" int __stdcall XUserSetPropertyEx (DWORD dwUserIndex, DWORD dwPropertyId, DWORD cbValue, void * pvValue, void * pOverlapped) { 
	trace ("XUserSetPropertyEx (%d, 0x%x, ...)\n", dwUserIndex, dwPropertyId);
	return 0;
}

// #5297: XLiveInitializeEx
extern "C" int __stdcall XLiveInitializeEx (void * pXii, DWORD dwVersion) {
	trace ("XLiveInitializeEx\n");
	return 0;
}
	
// #5300: XSessionCreate
extern "C"  DWORD __stdcall XSessionCreate (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionCreate\n");
	return -1;
}

// #5303: XStringVerify
extern "C"  DWORD __stdcall XStringVerify (DWORD, DWORD, DWORD, DWORD, DWORD, WORD * pResult, DWORD) { // XStringVerify
	trace ("XStringVerify\n");
	*pResult = 0;
	return 0;
}

// #5305: XStorageUploadFromMemory
extern "C"  DWORD __stdcall XStorageUploadFromMemory (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XStorageUploadFromMemory\n");
	return 0;
}

// #5306: XStorageEnumerate
extern "C" int __stdcall XStorageEnumerate (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { // XStorageEnumerate
	trace ("XStorageEnumerate\n");
	return 0;
}

// #5310: XOnlineStartup
extern "C"  int __stdcall XOnlineStartup () { 
	trace ("XOnlineStartup\n");
	return 0; 
}

// #5311: XOnlineCleanup
extern "C"  int __stdcall XOnlineCleanup () {
	trace ("XOnlineCleanup\n");
	return 0;
}

// #5312: XFriendsCreateEnumerator
extern "C"  DWORD __stdcall XFriendsCreateEnumerator (DWORD, DWORD, DWORD, DWORD, HANDLE * phEnum) { 
	trace ("XFriendsCreateEnumerator\n");
	*phEnum = INVALID_HANDLE_VALUE;
	return 0; 
}

// #5314: XUserMuteListQuery
extern "C"  int __stdcall XUserMuteListQuery (DWORD, DWORD, DWORD, DWORD) { 
	trace ("XUserMuteListQuery\n");
	return 0; 
}

// #5315: XInviteGetAcceptedInfo
extern "C"  int __stdcall XInviteGetAcceptedInfo (DWORD, DWORD) { 
	trace ("XInviteGetAcceptedInfo\n");
	return 1; 
}

// #5316: XInviteSend
extern "C"  int __stdcall XInviteSend (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XInviteSend\n");
	return 0; 
}

// #5317: XSessionWriteStats
extern "C"  DWORD __stdcall XSessionWriteStats (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionWriteStats\n");
	return 0; 
}

// #5318
extern "C"  int __stdcall XSessionStart (DWORD, DWORD, DWORD) {
	trace ("XSessionStart\n");
	return 0;
}

// #5319: XSessionSearchEx
extern "C"  DWORD __stdcall XSessionSearchEx (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionSearchEx\n");
	return 0; 
}

// #5322: XSessionModify
extern "C"  DWORD __stdcall XSessionModify (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionModify\n");
	return 0; 
}

// #5323: XSessionMigrateHost
extern "C"  DWORD __stdcall XSessionMigrateHost (DWORD, DWORD, DWORD, DWORD) {  
	trace ("XSessionMigrateHost\n");
	return 0; 
}

// #5324: XOnlineGetNatType
extern "C"  int __stdcall XOnlineGetNatType () { 
	trace ("XOnlineGetNatType\n");
	return 0; 
}

// #5325: XSessionLeaveLocal
extern "C"  DWORD __stdcall XSessionLeaveLocal (DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionLeaveLocal\n");
	return 0; 
}

// #5326: XSessionJoinRemote
extern "C"  DWORD __stdcall XSessionJoinRemote (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionJoinRemote\n");
	return 0; 
}

// #5327: XSessionJoinLocal
extern "C"  DWORD __stdcall XSessionJoinLocal (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionJoinLocal\n");
	return 0; 
}

// #5328: XSessionGetDetails
extern "C"  DWORD __stdcall XSessionGetDetails (DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionGetDetails\n");
	return 0; 
}

// #5329: XSessionFlushStats
extern "C"  int __stdcall XSessionFlushStats (DWORD, DWORD) { 
	trace ("XSessionFlushStats\n");
	return 0; 
}

// #5330: XSessionDelete
extern "C"  DWORD __stdcall XSessionDelete (DWORD, DWORD) { 
	trace ("XSessionDelete\n");
	return 0; 
}

struct XUSER_READ_PROFILE_SETTINGS {
	DWORD	dwLength;
	BYTE *	pSettings;
};

// #5331: XUserReadProfileSettings
extern "C"  DWORD __stdcall XUserReadProfileSettings (DWORD dwTitleId, DWORD dwUserIndex, DWORD dwNumSettingIds, 
					DWORD * pdwSettingIds, DWORD * pcbResults, XUSER_READ_PROFILE_SETTINGS * pResults, DWORD pOverlapped) {
	trace ("XUserReadProfileSettings (%d, %d, %d, ..., %d, ...)\n", dwTitleId, dwUserIndex, dwNumSettingIds, *pcbResults);
	if (*pcbResults < 1036) {
		*pcbResults = 1036;	// TODO: make correct calculation by IDs.
		return ERROR_INSUFFICIENT_BUFFER;
	}
	memset (pResults, 0, *pcbResults);
	pResults->dwLength = *pcbResults-sizeof (XUSER_READ_PROFILE_SETTINGS);
	pResults->pSettings = (BYTE *)pResults+sizeof (XUSER_READ_PROFILE_SETTINGS);
	return 0;
}

// #5332: XSessionEnd
extern "C"  int __stdcall XSessionEnd (DWORD, DWORD) {	
	trace ("XSessionEnd\n");
	return 0;
}

// #5333: XSessionArbitrationRegister
extern "C"  DWORD __stdcall XSessionArbitrationRegister (DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionArbitrationRegister\n");
	return 0; 
}

// #5335: XTitleServerCreateEnumerator
extern "C"  DWORD __stdcall XTitleServerCreateEnumerator (DWORD, DWORD, DWORD, PHANDLE phEnum) {
	trace ("XTitleServerCreateEnumerator\n");
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

// #5336: XSessionLeaveRemote
extern "C"  DWORD __stdcall XSessionLeaveRemote (DWORD, DWORD, DWORD, DWORD) { 
	trace ("XSessionLeaveRemote\n");
	return 0; 
}

// #5337: XUserWriteProfileSettings
extern "C"  DWORD __stdcall XUserWriteProfileSettings (DWORD, DWORD, DWORD, DWORD) {
	trace ("XUserWriteProfileSettings\n");
	return 0;
}

// #5339: XUserReadProfileSettingsByXuid
extern "C" DWORD __stdcall XUserReadProfileSettingsByXuid (DWORD, DWORD, DWORD, DWORD,DWORD, DWORD,DWORD, DWORD,DWORD) {
	trace ("XUserReadProfileSettingsByXuid\n");
	return 0;
}

// #5343: XLiveCalculateSkill
extern "C" DWORD __stdcall XLiveCalculateSkill (DWORD, DWORD, DWORD, DWORD, DWORD) { 
	trace ("XLiveCalculateSkill\n");
	return 0;
}

// #5344: XStorageBuildServerPath
extern "C"  DWORD __stdcall XStorageBuildServerPath (DWORD dwUserIndex, DWORD StorageFacility, 
		void * pvStorageFacilityInfo, DWORD dwStorageFacilityInfoSize, 
		void * pwszItemName, void * pwszServerPath, DWORD * pdwServerPathLength) { 
	trace ("XStorageBuildServerPath\n");
	return 0; 
}

// #5345: XStorageDownloadToMemory
extern "C"  DWORD __stdcall XStorageDownloadToMemory (DWORD dwUserIndex, DWORD, DWORD, DWORD, DWORD, DWORD, void * ) { 
	trace ("XStorageDownloadToMemory\n");
	return 0; 
}

// #5349: XLiveProtectedVerifyFile
extern "C" DWORD __stdcall XLiveProtectedVerifyFile (HANDLE hContentAccess, VOID * pvReserved, PCWSTR pszFilePath) {
	trace ("XLiveProtectedVerifyFile\n");
	return 0;
}

// #5350: XLiveContentCreateAccessHandle
extern "C" DWORD __stdcall XLiveContentCreateAccessHandle (DWORD dwTitleId, void * pContentInfo, 
	DWORD dwLicenseInfoVersion, void * xebBuffer, DWORD dwOffset, HANDLE * phAccess, void * pOverlapped) {
	trace ("XLiveContentCreateAccessHandle\n");
	if (phAccess)
		*phAccess = INVALID_HANDLE_VALUE;
	return E_OUTOFMEMORY;	// TODO: fix it
}

// #5352: XLiveContentUninstall
extern "C" DWORD __stdcall XLiveContentUninstall (void * pContentInfo, void * pxuidFor, void * pInstallCallbackParams) {
	trace ("XLiveContentUninstall\n");
	return 0;
}

// #5355: XLiveContentGetPath
extern "C" DWORD __stdcall XLiveContentGetPath (DWORD dwUserIndex, void * pContentInfo, wchar_t * pszPath, DWORD * pcchPath) {
	if (pcchPath)
		*pcchPath = 0;
	if (pszPath)
		*pszPath = 0;
	return 0;
}

// #5360: XLiveContentCreateEnumerator
extern "C" DWORD __stdcall XLiveContentCreateEnumerator (DWORD, void *, DWORD *pchBuffer, HANDLE * phContent) {
	trace ("XLiveContentCreateEnumerator\n");
	if (phContent)
		*phContent = INVALID_HANDLE_VALUE;
	return 0;
}

// #5361: XLiveContentRetrieveOffersByDate
extern "C" DWORD __stdcall XLiveContentRetrieveOffersByDate (DWORD dwUserIndex, DWORD dwOffserInfoVersion, 
	SYSTEMTIME * pstStartDate, void * pOffserInfoArray, DWORD * pcOfferInfo, void * pOverlapped) {
	if (pcOfferInfo)
		*pcOfferInfo = 0;
	return 0;
} 

// #5365: XShowMarketplaceUI
extern "C" DWORD __stdcall XShowMarketplaceUI (DWORD dwUserIndex, DWORD dwEntryPoint, ULONGLONG dwOfferId, DWORD dwContentCategories) {
	return 1;
}

// === replacements ===
struct FakeProtectedBuffer {
	DWORD	dwMagick;	
	int		nSize;
	DWORD	__fill[2]; // To match buffer size in Rick's wrapper
	BYTE	bData[4];
};

// #5016: XLivePBufferAllocate
extern "C"  DWORD __stdcall XLivePBufferAllocate (int size, FakeProtectedBuffer ** pBuffer) {
//	trace ("xlive_5016: XLivePBufferAllocate (%d)\n", size);
	*pBuffer = (FakeProtectedBuffer *)malloc (size+16);
	if (!*pBuffer) {
		trace ("ERROR: XLivePBufferAllocate unable to allocate %d bytes\n", size);
		return E_OUTOFMEMORY;
	}

	(*pBuffer)->dwMagick = 0xDEADDEAD;	// some arbitrary number
	(*pBuffer)->nSize = size;
	return 0;
}

// #5017: XLivePBufferFree
extern "C"  DWORD __stdcall XLivePBufferFree (FakeProtectedBuffer * pBuffer) {
	// trace ("xlive_5017: XLivePBufferFree\n");
	if (pBuffer && pBuffer->dwMagick == 0xDEADDEAD)
		free (pBuffer);
	return 0;
}

// #5295: XLivePBufferSetByteArray
extern "C"  DWORD __stdcall XLivePBufferSetByteArray (FakeProtectedBuffer * pBuffer, int offset, BYTE * source, int size) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || !source || offset < 0 || offset+size > pBuffer->nSize)
		return 0;
	memcpy (pBuffer->bData+offset, source, size);
	return 0;
}

// #5294: XLivePBufferGetByteArray
extern "C"  DWORD __stdcall XLivePBufferGetByteArray (FakeProtectedBuffer * pBuffer, int offset, BYTE * destination, int size) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || !destination || offset < 0 || offset+size > pBuffer->nSize)
		return 0;
	memcpy (destination, pBuffer->bData+offset, size);
	return 0;
}

// #5019: XLivePBufferSetByte
extern "C"  DWORD __stdcall XLivePBufferSetByte (FakeProtectedBuffer * pBuffer, int offset, BYTE value) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || offset < 0 || offset > pBuffer->nSize)
		return 0;
	pBuffer->bData[offset] = value;
	return 0;
}

// #5018: XLivePBufferGetByte
extern "C"  DWORD __stdcall XLivePBufferGetByte (FakeProtectedBuffer * pBuffer, int offset, BYTE * value) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || !value || offset < 0 || offset > pBuffer->nSize)
		return 0;
	*value = pBuffer->bData[offset];
	return 0;
}

// #5020: XLivePBufferGetDWORD
extern "C" DWORD __stdcall XLivePBufferGetDWORD (FakeProtectedBuffer * pBuffer, DWORD dwOffset, DWORD * pdwValue) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || dwOffset < 0 || dwOffset > pBuffer->nSize-4 || !pdwValue)
		return 0;
	*pdwValue = *(DWORD *)(pBuffer->bData+dwOffset);
	return 0;
}

// #5021: XLivePBufferSetDWORD
extern "C" DWORD __stdcall XLivePBufferSetDWORD (FakeProtectedBuffer * pBuffer, DWORD dwOffset, DWORD dwValue ) {
	if (!pBuffer || pBuffer->dwMagick != 0xDEADDEAD || dwOffset < 0 || dwOffset > pBuffer->nSize-4)
		return 0;
	*(DWORD *)(pBuffer->bData+dwOffset) = dwValue;
	return 0;
}

// #5036: XLiveCreateProtectedDataContext
extern "C"  DWORD __stdcall XLiveCreateProtectedDataContext (DWORD * dwType, PHANDLE pHandle) {
	trace ("XLiveCreateProtectedDataContext\n");
	if (pHandle)
		*pHandle = (HANDLE)1;
	return 0;
}

// #5037: XLiveQueryProtectedDataInformation
extern "C"  DWORD __stdcall XLiveQueryProtectedDataInformation (HANDLE h, DWORD * p) {
	trace ("XLiveQueryProtectedDataInformation\n");
	return 0;
}

// #5038: XLiveCloseProtectedDataContext
extern "C"  DWORD __stdcall XLiveCloseProtectedDataContext (HANDLE h) {
	trace ("XLiveCloseProtectedDataContext\n");
	return 0;
}

// #5035: XLiveUnprotectData
extern "C"  DWORD __stdcall XLiveUnprotectData (BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE * ph) {
	trace ("XLiveUnprotectData (..., %d, ..., %d, %d)\n", dwInDataSize, *pDataSize, *(DWORD*)ph);
	if (!pDataSize || !ph)	// invalid parameter
		return E_FAIL;
	*ph = (HANDLE)1;
	if (dwInDataSize > 12)
		dwInDataSize = 12;	// RF:G workaround
	if (!pOutBuffer || *pDataSize < dwInDataSize) {
		*pDataSize = dwInDataSize;
		return ERROR_INSUFFICIENT_BUFFER;
	}
	*pDataSize = dwInDataSize;
	memcpy (pOutBuffer, pInBuffer, dwInDataSize);
	return 0;
}

// #5034: XLiveProtectData
extern "C"  DWORD __stdcall XLiveProtectData (BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h) {
	trace ("XLiveProtectData (..., %d, ..., %d, %d)\n", dwInDataSize, *pDataSize, (DWORD)h);
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
	else if (dwGameVersion == 0x00010004)
		pszPath = (char *)(0xFC4B00+dwLoadOffset);	// char pszPathPersonal[128]; // "%USERPROFILE%\Documents\Rockstar Games\GTA IV\"
	else if (dwGameVersion == 0x00010005)
		pszPath = (char *)(0x12898B0+dwLoadOffset);	// char pszPathPersonal[128]; // "%USERPROFILE%\Documents\Rockstar Games\GTA IV\"

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
//	*(BYTE  *)(0x472E50+dwLoadOffset) = 0xC3;	// RETN - certificates check
	*(BYTE  *)(0x472E50+dwLoadOffset) = 0xC2;	// RETN - certificates check
	*(BYTE  *)(0x472E51+dwLoadOffset) = 0x08;	// RETN - certificates check
	*(BYTE  *)(0x472E52+dwLoadOffset) = 0x00;	// RETN - certificates check
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

void patch104 () {
	dwGameVersion = 0x00010004;	// GTA IV 1.0.4 (patch 4)

	DWORD oldProtect;
	// enable write access to code and rdata
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x1000+0x400000), 0x8E4000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		trace ("ERROR: unable to unprotect code seg\n");
		return;
	}

	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x8e5000+0x400000), 0x1B1000, PAGE_READWRITE, &oldProtect)) 
		trace ("ERROR: unable to unprotect .rdata seg\n");

	// process patches
	*(WORD *)(0x60C1A5+dwLoadOffset) = 0x9090; 	// NOP; NOP - save file CRC32 check
	*(BYTE *)(0x7A4900+dwLoadOffset) = 0xC3;	// RETN - enable debugger in error menu (don't load WER.dll)
	*(BYTE *)(0x7A4A10+dwLoadOffset) = 0xC3;	// RETN - skip files.txt hash check

	*(BYTE  *)(0x471E5D+dwLoadOffset) = 0xE9;	// JMP
	*(DWORD *)(0x471E5E+dwLoadOffset) = 0x16;	// jmp target
//	*(BYTE  *)(0x472FF0+dwLoadOffset) = 0xC3;	// RETN - certificates check
	*(BYTE  *)(0x472FF0+dwLoadOffset) = 0xC2;	// RETN - certificates check
	*(BYTE  *)(0x472FF1+dwLoadOffset) = 0x08;	// RETN - certificates check
	*(BYTE  *)(0x472FF2+dwLoadOffset) = 0x00;	// RETN - certificates check

	*(BYTE  *)(0x471500+dwLoadOffset) = 0xC2;	// RETN - remove connect to the RGSC 
	*(BYTE  *)(0x471501+dwLoadOffset) = 0x04;	// RETN - remove connect to the RGSC 
	*(BYTE  *)(0x471502+dwLoadOffset) = 0x00;	// RETN - remove connect to the RGSC 
	memset ((BYTE *)(0x471F00+dwLoadOffset), 0x90, 0x1B);
	*(WORD  *)(0x471E8A+dwLoadOffset) = 0x9090;	// NOP; NOP - RGSC initialization check
        *(DWORD *)(0x471F20+dwLoadOffset) = 0x90909090;	// NOP*4- last RGSC init check
        *(WORD  *)(0x471F24+dwLoadOffset) = 0x9090;	// NOP*4- last RGSC init check 
	*(WORD *)(0x471E91+dwLoadOffset) = 0xC033;	// XOR eax, eax - RGSC initialization check
	*(WORD *)(0x471E98+dwLoadOffset) = 0xA390;	// NOP; MOV [], eax

	*(DWORD *)(0x4017F5+dwLoadOffset) = 1;		// disable sleep

	injectFunction (0x60BBD0, (DWORD)getSavefilePath); // replace getSavefilePath

        memset ((BYTE *)(0x452129+dwLoadOffset), 0x90, 0x1BF);	// EFC20

	*(DWORD *)(0xA0D9C0+dwLoadOffset) = 0x90C301B0;	// mov al, 1; retn
	*(DWORD *)(0xA0D9E0+dwLoadOffset) = 0x90C301B0;
	*(DWORD *)(0xA0D9F0+dwLoadOffset) = 0x90C301B0;
	*(DWORD *)(0xA0DA20+dwLoadOffset) = 0x90C301B0;

	trace ("Patching OK (1.0.4)\n");
}

void patch105 () {
	dwGameVersion = 0x00010005;	// GTA IV 1.0.0.4 (patch 5)

	DWORD oldProtect;
	// enable write access to code and rdata
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x1000+0x400000), 0x0915000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		trace ("ERROR: unable to unprotect code seg\n");
		return;
	}

	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x0916000+0x400000), 0x01AA000, PAGE_READWRITE, &oldProtect)) 
		trace ("ERROR: unable to unprotect .rdata seg\n");

	// process patches
	*(BYTE *)(0x7B82A0+dwLoadOffset) = 0xC3;	// RETN - enable debugger in error menu (don't load WER.dll)
	*(DWORD *)(0x401847+dwLoadOffset) = 1;		// disable sleep
	*(DWORD *)(0x527D90+dwLoadOffset) = 0x900008C2;	// RETN 8 - certificates check
	*(DWORD *)(0x5269B9+dwLoadOffset) = 0x06E9C033;	// xor eax, eax - address of the RGSC object
	*(DWORD *)(0x5269BD+dwLoadOffset) = 0x90000002;	// jmp 526BC6 (skip RGSC connect and EFC checks)		
	*(WORD *)(0x526BCB+dwLoadOffset) = 0xA390;	// NOP; MOV [g_rgsc], eax

	memset ((BYTE *)(0x526C30+dwLoadOffset), 0x90, 0x29);
        *(DWORD *)(0x526C5F+dwLoadOffset) = 0x90909090;	// NOP*4- last RGSC init check
        *(WORD  *)(0x526C63+dwLoadOffset) = 0x9090;	// NOP*2- last RGSC init check 

	// skip missing tests...
	*(WORD *)(0x474D0B+dwLoadOffset) = 0xC033;
	*(DWORD *)(0x474D0D+dwLoadOffset) = 0x90909090;
	memset ((BYTE *)(0x481225+dwLoadOffset), 0x90, 3);
	memset ((BYTE *)(0x48122B+dwLoadOffset), 0x90, 11);
	memset ((BYTE *)(0x4812F5+dwLoadOffset), 0x90, 3);
	memset ((BYTE *)(0x4812FB+dwLoadOffset), 0x90, 11);
	memset ((BYTE *)(0x481375+dwLoadOffset), 0x90, 11);
	memset ((BYTE *)(0x4813EF+dwLoadOffset), 0x90, 32);
	memset ((BYTE *)(0x4815E3+dwLoadOffset), 0x90, 25);
	memset ((BYTE *)(0x483BF5+dwLoadOffset), 0x90, 8);
	memset ((BYTE *)(0x483EA2+dwLoadOffset), 0x90, 8);
	memset ((BYTE *)(0x526E4F+dwLoadOffset), 0x90, 14);
	memset ((BYTE *)(0x526F77+dwLoadOffset), 0x90, 16);	
	*(BYTE *)(0x527930+dwLoadOffset) = 0xC3;
	memset ((BYTE *)(0x5280B0+dwLoadOffset), 0x90, 14);

	// savegames
	*(WORD *)(0x6C31A5+dwLoadOffset) = 0x9090; 	// NOP; NOP - save file CRC32 check
	injectFunction (0x6C2BD0, (DWORD)getSavefilePath); // replace getSavefilePath


	*(DWORD *)(0xB3E190+dwLoadOffset) = 0x90C301B0;	// mov al, 1; retn
	*(DWORD *)(0xB3E1B0+dwLoadOffset) = 0x90C301B0;
	*(DWORD *)(0xB3E1C0+dwLoadOffset) = 0x90C301B0;
	*(DWORD *)(0xB3E1F0+dwLoadOffset) = 0x90C301B0;
	
	trace ("Patching OK (1.0.0.4 - update 5)\n");
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

void patchRFG () {
	dwGameVersion = 0;	// RFG. should I make some constant ?
	DWORD	oldProtect;
	// enable write access to code and rdata
	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x1000+0x400000), 0x06CC000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		trace ("ERROR: unable to unprotect code seg\n");
		return;
	}

	if (!VirtualProtect ((LPVOID)(dwLoadOffset+0x6CD000+0x400000), 0xD2000, PAGE_READWRITE, &oldProtect)) 
		trace ("ERROR: unable to unprotect .rdata seg\n");

	// disable savegame check
	*(WORD*)(0x522A38+dwLoadOffset) = 0x9090;
	*(WORD*)(0x522A3E+dwLoadOffset) = 0x9090;
	*(WORD*)(0x522A44+dwLoadOffset) = 0x9090;
	trace ("Patching OK (RF:G)\n");
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
 	else if (signature == 0x110FF300)
		patch104 ();
	else if (signature == 0xf3385058)
		patch105 ();
	else if (signature == 0x108b1874)
		patchRFG ();
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
		trace ("Log started (xliveless 0.98)\n");
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
			EnterCriticalSection (&d_lock);
			fflush (logfile);
			fclose (logfile);
			logfile = NULL;
			LeaveCriticalSection (&d_lock);
			DeleteCriticalSection (&d_lock);
		}
#endif
		break;
	}
    return TRUE;
}

