#include "stdafx.h"
#include "Misc.h"
#include <WS2tcpip.h>
#include "SS_Interface.h"

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

BOOL FileRW(BOOL Method, const TCHAR *AFName, int ASize, void *Buf, BOOL AMsgBox)
{
	HANDLE qF = CreateFile(AFName,
		Method ? GENERIC_WRITE : GENERIC_READ,
		Method ? 0 : FILE_SHARE_READ, NULL,
		Method ? CREATE_ALWAYS : OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD  qNumRW;
	if (qF == INVALID_HANDLE_VALUE) {
		qNumRW = 0;
		while (++qNumRW < 3 && GetLastError() == ERROR_SHARING_VIOLATION) {
			Sleep(200);
			qF = CreateFile(AFName,
				Method ? GENERIC_WRITE : GENERIC_READ,
				Method ? 0 : FILE_SHARE_READ, NULL,
				Method ? CREATE_ALWAYS : OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (qF != INVALID_HANDLE_VALUE)
				goto Label_Begin;
		};

		return FALSE;
	};

Label_Begin:
	if (!Method) {
		int  qFSize = GetFileSize(qF, NULL);
		if (ASize <= 0)
			ASize = qFSize;
		else
			if (ASize != qFSize) {
				CloseHandle(qF);
				return FALSE;
			};
	};
	if (Method) WriteFile(qF, Buf, ASize, &qNumRW, NULL);
	else ReadFile(qF, Buf, ASize, &qNumRW, NULL);
	if ((int)qNumRW != ASize) {
		CloseHandle(qF);
		return FALSE;
	};
	CloseHandle(qF);
	return TRUE;
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

void CreateDir(TCHAR* Path)
{
	TCHAR DirName[256];
	TCHAR* p = Path;
	TCHAR* q = DirName;

	while (*p) {
		if (('\\' == *p) || ('/' == *p)) {
			if (':' != *(p - 1))
				CreateDirectory(DirName, NULL);
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory(DirName, NULL);
}


int CropFrame(UINT16* Src, UINT16* Dst, tVaU_ImgDim* pImgDim)
{
	if ((pImgDim->rFrameHeight - pImgDim->rImgCutBottom - pImgDim->rImgCutTop) <= 0)
		return 0;
	if ((pImgDim->rFrameWidth - pImgDim->rImgCutLeft - pImgDim->rImgCutRight) <= 0)
		return 0;

	UINT16 *rowSrc, *rowDst;
	int widthCropped = pImgDim->rFrameWidth - pImgDim->rImgCutLeft - pImgDim->rImgCutRight;
	for (int y = pImgDim->rImgCutTop; y < (pImgDim->rFrameHeight - pImgDim->rImgCutBottom); y++) {
		rowSrc = (Src + (pImgDim->rFrameWidth * y) + pImgDim->rImgCutLeft);
		rowDst = (Dst + (widthCropped * (y - pImgDim->rImgCutTop)));
		memcpy_s(rowDst, sizeof(UINT16)*widthCropped, rowSrc, sizeof(UINT16)*widthCropped);
	}
	return 0;
}