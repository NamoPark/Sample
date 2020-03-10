#include "../stdafx.h"
#include "Misc.h"
#include <WS2tcpip.h>

#define UNSIGNED_BYTE_0	85;
#define UNSIGNED_BYTE_1 83;
#define SIGNED_BYTE_0	83;
#define SIGNED_BYTE_1	73;

static char ix_Mutex[cVaU3_MaxMutexID + 1];

void SleepMsg(DWORD dwMilliseconds)
{
	DWORD dwStart = 0;
	MSG   msg = { 0, };

	dwStart = GetTickCount();
	while (GetTickCount() - dwStart < dwMilliseconds)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Lower CPU Usage  
		Sleep(10);
	}
}

ULONG VAU3_InetAddrTCHAR(const TCHAR *AAddr)
{
	char qAddr[40];
	ULONG Ret;
	memset(qAddr, sizeof(qAddr), 0);

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, AAddr, -1, qAddr, sizeof(qAddr), NULL, NULL);
#else
	_tcsncpy(qAddr, AAddr, sizeof(qAddr) - 1);
#endif

	//return inet_addr(qAddr);
	if (inet_pton(AF_INET, qAddr, &Ret) != 0)
		return Ret;
	else
		return -1;
}

void VaU3_MutexCreate(int AMutexID)
{
	ix_Mutex[AMutexID] = 0;
}

BOOL VaU3_MutexGet(int AMutexID)
{
	int qWaitCount = 0;
	while (ix_Mutex[AMutexID]) {
		Sleep(1);
		if (++qWaitCount > 10 && !VaU3_MutexLocked(AMutexID))
			return FALSE;
		if (ix_Mutex[AMutexID] == 2) {
			return 2;
		}
	};
	ix_Mutex[AMutexID] = 1;
	return TRUE;
}

void VaU3_MutexRelease(int AMutexID)
{
	ix_Mutex[AMutexID] = 0;
}

void VaU3_MutexDelete(int AMutexID)
{
	ix_Mutex[AMutexID] = 0;
}

void VaU3_MutexLock(int AMutexID)
{
	ix_Mutex[AMutexID] = 2;
}

BOOL VaU3_MutexLocked(int AMutexID)
{
	return ix_Mutex[AMutexID] == 2;
}

void VaU3_CloseThreadHandle(HANDLE *AThread, int ADelay)
{
	HANDLE qHandle = *AThread;
	if (qHandle) {
		*AThread = NULL;
		if (WaitForSingleObject(qHandle, ADelay) == WAIT_TIMEOUT)
			TerminateThread(qHandle, 255);
		CloseHandle(qHandle);
	};
}
