
#include "stdafx.h"
#include "SS_file_utils.h"
#include <strsafe.h>

TCHAR g_pSDK_Directory[MAX_PATH] = _T("");

const bool FileExists(const SSstring sFilename)
{
	DWORD dwAttrib = GetFileAttributes(sFilename.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}


const bool FileExistsA(const string sFilename)
{
	DWORD dwAttrib = GetFileAttributesA(sFilename.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

const bool DirectoryExists(const SSstring sDir)
{
	DWORD dwAttrib = GetFileAttributes(sDir.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

const bool FileDelete(const SSstring sFilename)
{
	return DeleteFile(sFilename.c_str()) != false;
}


const bool MakeDirectory(const SSstring sDir)
{
	CString csPath;
	csPath = sDir.c_str();
	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;
	while ((nEnd = csPath.Find(_T("\\"), nStart)) >= 0)
	{
		CString csToken = csPath.Mid(nStart, nEnd - nStart);
		CreateDirectory(csPrefix + csToken, NULL);

		csPrefix += csToken;
		csPrefix += _T("\\");
		nStart = nEnd + 1;
	}
	csToken = csPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);
	return false;
}

const bool MakeFile(const SSstring sFilePath)
{
	CFileStatus fstat;
	CFile fp_ini;
	CString csFilePath;
	csFilePath = sFilePath.c_str();

	if (CFile::GetStatus(csFilePath, fstat)) {
	}
	else {
		fp_ini.Open(csFilePath, CFile::modeCreate | CFile::modeReadWrite);
		fp_ini.Close();
	}
	return false;
}

const SSstring ExtractFileName(const SSstring sPath)
{
	SSstring::size_type idx = sPath.find_last_of(_T("\\:/"));

	if (idx == SSstring::npos)
		return sPath;

	return sPath.substr(idx + 1, sPath.length() - idx);
}

const SSstring ExtractFilePath(const SSstring sPath)
{
	SSstring::size_type idx = sPath.find_last_of(_T("\\:/"));

	if (idx == SSstring::npos)
		return _T("");

	return sPath.substr(0, idx + 1); // including /
}

const SSstring EnsurePathDelimiter(const SSstring sPath, const TSDPathDelimiter pdDelim)
{
	SSstring sPathInput = sPath;
	const SSstring::size_type len = sPath.length();

	if (len > 0)
	{
		if (sPath[len - 1] != '\\' && sPath[len - 1] != '/')
		{
			switch (pdDelim)
			{
			case TSDPathDelimiter::pdWin32:
				sPathInput.push_back('\\');
				return sPathInput;
			case TSDPathDelimiter::pdUNIX:
				sPathInput.push_back('/');
				return sPathInput;
			}
		}

		return sPathInput;
	}

	return sPath;
}

const SSstring ExtractPureFileName(const SSstring sPath)
{
	const SSstring sFileName = ExtractFileName(sPath);

	SSstring::size_type idx = sFileName.find_last_of(_T("."));
	if (idx != SSstring::npos)
		return sFileName.substr(0, idx);

	return sFileName;
}

const SSstring GetModuleDirectory()
{
	TCHAR pModulePath[MAX_PATH] = _T("");
	::GetModuleFileName(NULL, pModulePath, MAX_PATH);

	SSstring sModuleDir(pModulePath);

	sModuleDir = ExtractFilePath(sModuleDir);
	sModuleDir = EnsurePathDelimiter(sModuleDir);

	return sModuleDir;
}

const SSstring GetSDKDirectory()
{
	return SSstring(g_pSDK_Directory);
}


void SaveRawFrame(CString filePath, bool sign, void *buffer, UINT length)
{
	CFile fp;
	UINT16* usBuffer = (UINT16*)buffer;
	INT16 *sBuffer = (INT16*)buffer;
	// Save File
	if (!filePath.IsEmpty())
	{
		SSstring wsFilePath = ExtractFilePath((LPCTSTR)filePath);
		if (DirectoryExists(wsFilePath) == 0) 
		{
			MakeDirectory(wsFilePath);
		}

		fp.Open(filePath, CFile::modeCreate | CFile::modeWrite, nullptr);
		if (sign == TRUE)
		{
			fp.Write(sBuffer, length);
		}
		else // unsigned 
		{
			fp.Write(usBuffer, length);
		}
		fp.Close();
	}
}