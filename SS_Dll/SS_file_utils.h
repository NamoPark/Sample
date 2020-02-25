#pragma once
#include "stdafx.h"
#include "SS_defines.h"


#define INVALID_PATH 0

extern TCHAR g_pSDK_Directory[MAX_PATH];


enum TSDPathDelimiter
{
	pdWin32,
	pdUNIX
};

const SSstring ExtractFileName(const SSstring sPath);
const SSstring ExtractFilePath(const SSstring sPath);
const SSstring EnsurePathDelimiter(const SSstring sDir, const TSDPathDelimiter pdDelim = TSDPathDelimiter::pdWin32);
const SSstring ExtractPureFileName(const SSstring sPath);
const SSstring GetModuleDirectory();
const SSstring GetSDKDirectory();

const bool FileExists(const SSstring sFilename);
const bool FileExistsA(const string sFilename);
const bool DirectoryExists(const SSstring sDirName);
const bool FileDelete(const SSstring sFilename);
const bool MakeDirectory(const SSstring sDirName);
const bool MakeFile(const SSstring sFilePath);

void SaveRawFrame(CString filePath, bool sign, void *buffer, UINT length);

