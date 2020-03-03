/** *************************************************************** **
 **    Implementation of the GIniFile class.                        **
 **                                                     by S.B.     **
 ** *************************************************************** **/

#include "stdafx.h"
#include "GIniFile.h"

 //////////////////////////////////////////////////////////////////////
 // Construction/Destruction
 //////////////////////////////////////////////////////////////////////

void GIniFile::SetFileName(const TCHAR *AFileName)
{
	_tcscpy_s(rFName, AFileName);
}

void GIniFile::SetSectName(const TCHAR *ASectName)
{
	_tcscpy_s(rSName, ASectName);
}

#define mc_GINIFILE_CheckDeflt()\
  const TCHAR *qDeflt;\
  if (ADeflt) qDeflt = ADeflt;\
         else qDeflt = AKeyName;

int GIniFile::GetIniStr(const TCHAR *AKeyName, TCHAR *AResult, const TCHAR *ADeflt)
{
	int  qOk = GetPrivateProfileString(rSName, AKeyName, _T(""), AResult, cMaxStrLength, rFName);
	
	if (qOk) {
		rLastResult = TRUE;
		if (AResult[0] == _T('\\') && AResult[1] <= _T(' ')) { // it allows to specify empty lines using back slash
			_tcscpy_s(AResult, cMaxStrLength, (AResult + 1));
			qOk--;
		};
	}
	else {
		mc_GINIFILE_CheckDeflt();
		_tcscpy_s(AResult, cMaxStrLength, qDeflt);
		rLastResult = FALSE;
		qOk = (int)_tcslen(ADeflt);
	};

	return qOk;
}

int GIniFile::SGetIniStr(const TCHAR *AKeyName, TCHAR *AResult, const TCHAR *ADeflt)
{
	int  qOk;
	//default valid Check ¾ø´Ù¸é qdeflt = Akeyname
	mc_GINIFILE_CheckDeflt();
	qOk = GetIniStr(AKeyName, AResult, qDeflt);
	if (!rLastResult)
		if (qDeflt && *qDeflt) 
			SetIniStr(AKeyName, qDeflt);
		else 
			SetIniStr(AKeyName, _T("\\"));
		return qOk;
}

int GIniFile::SetIniStr(const TCHAR *AKeyName, const TCHAR *AStr)
{
	return WritePrivateProfileString(rSName, AKeyName, AStr, rFName);
}

int GIniFile::TouchIniStr(const TCHAR *AKeyName, TCHAR *AStr, unsigned int Method, const TCHAR *ADeflt)
{
	switch (Method)
	{
	case WR:
	{
		SetIniStr(AKeyName, AStr);
		return true;
	}
	case RD:
	{
		SGetIniStr(AKeyName, AStr, ADeflt);
		return true;
	}
	default:
	{
		return false;
	}
	}
}

double GIniFile::GetIniFloat(const TCHAR *AKeyName, double ADeflt)
{
	TCHAR qS[cMaxStrLength], qSD[cMaxStrLength];
	_stprintf_s(qSD, _T("%.4f"), ADeflt);
	GetIniStr(AKeyName, qS, qSD);
	if (!rLastResult) return ADeflt;
	else return _tcstod(qS, NULL);
}

double GIniFile::SGetIniFloat(const TCHAR *AKeyName, double ADeflt)
{
	double qRes = GetIniFloat(AKeyName, ADeflt);
	if (!rLastResult)
		SetIniFloat(AKeyName, ADeflt);
	return qRes;
}

int GIniFile::SetIniFloat(const TCHAR *AKeyName, double AVal)
{
	TCHAR qS[cMaxStrLength];
	_stprintf_s(qS, _T("%f"), AVal);
	return SetIniStr(AKeyName, qS);
}

int GIniFile::TouchIniFloat(const TCHAR *AKeyName, double* AVal, unsigned int Method, double ADeflt)
{
	switch (Method)
	{
	case WR:
	{
		SetIniFloat(AKeyName, *AVal);
		return true;
	}
	case RD:
	{
		*AVal = SGetIniFloat(AKeyName, ADeflt);
		return true;
	}
	default:
	{
		return false;
	}
	}
}
