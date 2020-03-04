
#include "../stdafx.h"
#include "SS_FileUtils.h"
#include <strsafe.h>
#include "SS_Timer.h"

static SWstring ReportError(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code
	DWORD dw = GetLastError();

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf(
		(LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);

	SWstring sMessage = _T("");
	sMessage = (LPTSTR)lpDisplayBuf;

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	return sMessage;
}



const bool FileExists(const SWstring sFilename)
{
	DWORD dwAttrib = GetFileAttributes(sFilename.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}


const bool FileExistsA(const string sFilename)
{
	DWORD dwAttrib = GetFileAttributesA(sFilename.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

const bool DirectoryExists(const SWstring sDir)
{
	DWORD dwAttrib = GetFileAttributes(sDir.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

const bool FileDelete(const SWstring sFilename)
{
	return DeleteFile(sFilename.c_str()) != false;
}

const bool MakeAllDirectory(const SWstring sFilename)
{
	CString csPath;
	csPath = WTOC(sFilename);
	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;

	bool bResult = 1;
	while ((nEnd = csPath.Find(_T("\\"), nStart)) >= 0)
	{
		CString csToken = csPath.Mid(nStart, nEnd - nStart);
		bResult = MakeDirectory((LPCTSTR)(csPrefix + csToken));
		if (!bResult)
			return bResult;
		csPrefix += csToken;
		csPrefix += _T("\\");
		nStart = nEnd + 1;
	}
	csToken = csPath.Mid(nStart);

	bResult = MakeDirectory((LPCTSTR)(csPrefix + csToken));
	if (!bResult)
		return bResult;

	return bResult;
}

const bool MakeDirectory(const SWstring sDir)
{
	bool bResult = 1;
	if (!DirectoryExists(sDir))
	{
		bResult = CreateDirectory(WTOC(sDir), NULL);

		if (!bResult)
		{
			ReportError(_T("CreateDirectory"));
			return bResult;
		}
	}
	return bResult;
}

const bool MakeFile(const SWstring sFilePath)
{
	CFile fp_ini;
	bool bResult = fp_ini.Open(WTOC(sFilePath), CFile::modeCreate | CFile::modeReadWrite);
	if (!bResult)
	{
		ReportError(_T("OpenFile"));
		return bResult;
	}
	else
	{
		fp_ini.Close();
	}
	return bResult;
}

const SWstring ExtractFileName(const SWstring sPath)
{
	SWstring::size_type idx = sPath.find_last_of(_T("\\:/"));

	if (idx == SWstring::npos)
		return sPath;

	return sPath.substr(idx + 1, sPath.length() - idx);
}

const SWstring ExtractFilePath(const SWstring sPath)
{
	SWstring::size_type idx = sPath.find_last_of(_T("\\:/"));

	if (idx == SWstring::npos)
		return _T("");

	return sPath.substr(0, idx + 1); // including /
}


const SWstring ExtractPureFileName(const SWstring sPath)
{
	const SWstring sFileName = ExtractFileName(sPath);

	SWstring::size_type idx = sFileName.find_last_of(_T("."));
	if (idx != SWstring::npos)
		return sFileName.substr(0, idx);

	return sFileName;
}

const SWstring GetModuleDirectory()
{
	TCHAR pModulePath[MAX_PATH] = _T("");
	::GetModuleFileName(NULL, pModulePath, MAX_PATH);

	SWstring sModuleDir(pModulePath);

	sModuleDir = ExtractFilePath(sModuleDir);
	//sModuleDir = EnsurePathDelimiter(sModuleDir);

	return sModuleDir;
}

bool SaveRawFrame(CString filePath, bool sign, void *buffer, UINT length)
{
	UINT16* usBuffer = (UINT16*)buffer;
	INT16 *sBuffer = (INT16*)buffer;
	// Save File
	if (!filePath.IsEmpty())
	{
		SWstring wsFilePath = ExtractFilePath((LPCTSTR)filePath);
		if (!DirectoryExists(wsFilePath)) 
		{
			bool bValidPath = 0;
			bValidPath = MakeAllDirectory(wsFilePath);
			//nh : Directory creation failed
			if (!bValidPath)
				return bValidPath;
		}

		CFile fp;
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
	else
	{	
		//nh : Invalid CString
		return false;
	}
}

const bool SaveToFile(unsigned char * pData, int nSize, SWstring sFileName)
{
	SWstring wsFilePath = ExtractFilePath(sFileName);
	bool bOK = 1;
	if (!DirectoryExists(wsFilePath))
	{
		bOK = MakeAllDirectory(wsFilePath);
		if (!bOK)
			return bOK;
	}

	UINT16* usBuffer = (UINT16*)pData;
	FILE *fs = NULL;
	errno_t err = SS_fopen_s(&fs, sFileName.c_str(), fsWB);
	if (fs && err == 0)
	{
		fwrite(usBuffer, nSize, 1, fs);

		fflush(fs);

		fclose(fs);
		return bOK;
	}
	else
	{
		//LogDebug(_T("save memory to file error, %s"), sFileName.c_str());
	}

	return bOK;
}

