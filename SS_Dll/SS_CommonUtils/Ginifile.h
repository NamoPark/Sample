/** *************************************************************** **
 **    Interface for the GIniFile class                             **
 **                                                     by S.B.     **
 ** *************************************************************** **/
#include "../stdafx.h"
#pragma once

#define cMaxStrLength 512
#define WR	0x5
#define RD	0xA

class GIniFile
{
public:
	TCHAR rFName[cMaxStrLength],
		rSName[cMaxStrLength];
	int  rLastResult;
	void  SetFileName(const TCHAR *AFileName);
	// sets name of ini file
	void  SetSectName(const TCHAR *ASectName);
	// sets name of section

	int    GetIniStr(const TCHAR *AKeyName, TCHAR *AResult, const TCHAR *ADeflt);
	// returns ini-value and its length
	int    SGetIniStr(const TCHAR *AKeyName, TCHAR *AResult, const TCHAR *ADeflt);
	// same as GetIniStr, but write ini-value if it does not exist yet
	int   SetIniStr(const TCHAR *AKeyName, const TCHAR *AStr);
	// writes ini-value
	int TouchIniStr(const TCHAR *AKeyName, TCHAR *AStr, unsigned int Method, const TCHAR *ADeflt);


	template<typename Tp>
	Tp GetIniInt(const TCHAR *AKeyName, Tp ADeflt);
	// returns ini-value
	template<typename Tp>
	Tp SGetIniInt(const TCHAR *AKeyName, Tp ADeflt);
	// same as GetIniInt, but write ini-value if it does not exist yet
	template<typename Tp>
	int SetIniInt(const TCHAR *AKeyName, Tp AVal);
	// writes ini-value
	// original
	/*template<typename Tp1, typename Tp3, typename Tp2>
	BOOL TouchIniInt(const TCHAR *AKeyName, Tp1* AVal, Tp3 Method, Tp2 ADeflt);*/
	template<typename Tp1, typename Tp3, typename Tp2>
	int TouchIniInt(const TCHAR *AKeyName, Tp1* AVal, Tp3 Method, Tp2 ADeflt);
	// W/R ini value


	double GetIniFloat(const TCHAR *AKeyName, double ADeflt);
	// returns ini-value
	double SGetIniFloat(const TCHAR *AKeyName, double ADeflt);
	// same as GetIniInt, but write ini-value if it does not exist yet
	int   SetIniFloat(const TCHAR *AKeyName, double AVal);
	// writes ini-value
	int TouchIniFloat(const TCHAR *AKeyName, double* AVal, unsigned int Method, double ADeflt);
};

#include "GIniFile_Template.hpp"