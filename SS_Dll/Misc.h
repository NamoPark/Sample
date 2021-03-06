#pragma once
typedef struct _tVaU_ImgDim tVaU_ImgDim;

ULONG VAU3_InetAddrTCHAR(const TCHAR *AAddr);

///// casual mutex //////
const int cVaU3_MaxMutexID = 2;
void VaU3_MutexCreate(int AMutexID);
BOOL VaU3_MutexGet(int AMutexID);
void VaU3_MutexRelease(int AMutexID);
void VaU3_MutexDelete(int AMutexID);
void VaU3_MutexLock(int AMutexID);
BOOL VaU3_MutexLocked(int AMutexID);
void VaU3_CloseThreadHandle(HANDLE *AThread, int ADelay);
void CreateDir(TCHAR* Path);
void SleepMsg(DWORD dwMilliseconds);
BOOL FileRW(BOOL Method, const TCHAR *AFName, int ASize, void *Buf, BOOL AMsgBox);
int CropFrame(UINT16* Src, UINT16* Dst, tVaU_ImgDim* pImgDim);