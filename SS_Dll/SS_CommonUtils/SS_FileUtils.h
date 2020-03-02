#pragma once
#include "../stdafx.h"
#include "SS_StringDefines.h"


#define INVALID_PATH 0



const SSstring ExtractFileName(const SSstring sPath);
const SSstring ExtractFilePath(const SSstring sPath);
const SSstring ExtractPureFileName(const SSstring sPath);
const SSstring GetModuleDirectory();

const bool FileExists(const SSstring sFilename);
const bool FileExistsA(const string sFilename);
const bool DirectoryExists(const SSstring sDirName);
const bool FileDelete(const SSstring sFilename);
const bool MakeDirectory(const SSstring sDirName);
const bool MakeFile(const SSstring sFilePath);

void SaveRawFrame(CString filePath, bool sign, void *buffer, UINT length);

