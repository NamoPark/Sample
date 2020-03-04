#pragma once
#include "../stdafx.h"
#include "SS_StringDefines.h"


const SWstring ExtractFileName		(const SWstring sPath);
const SWstring ExtractFilePath		(const SWstring sPath);
const SWstring ExtractPureFileName	(const SWstring sPath);
const SWstring GetModuleDirectory	();

const bool FileExists				(const SWstring sFilename);
const bool FileExistsA				(const string sFilename);
const bool DirectoryExists			(const SWstring sDirName);
const bool FileDelete				(const SWstring sFilename);

const bool MakeAllDirectory			(const SWstring sDirName);
const bool MakeDirectory			(const SWstring sDirName);
const bool MakeFile					(const SWstring sFilePath);
const bool SaveToFile				(unsigned char *pData, int nSize, SWstring sFileName);


bool SaveRawFrame(CString filePath, bool sign, void *buffer, UINT length);

