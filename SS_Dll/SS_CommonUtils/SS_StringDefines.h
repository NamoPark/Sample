#pragma once

/*
*  Copyright 2015 by Samsung Co.,Ltd., All rights reserved.
*
*  This software is the confidential and proprietary information
*  of Samsung, Inc. ("Confidential Information").  You
*  shall not disclose such Confidential Information and shall use
*  it only in accordance with the terms of the license agreement
*  you entered into with Samsung.
*/

/*!
* @file      sd_StringDefines.h
* @brief     common definitions
* @author    js.view.kim@samsung.com
* @date      2015.07.16 (heading comment adding)
*/
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;
//nh : wstring to CString
#define WTOC(X) X.c_str()
//nh : wstring to string
#define WTOS(X) CT2CA(X.c_str())
//nh : CString to wstring
#define CTOW(X) X.operator LPCWSTR()
//nh : CString to string
#define CTOS(X) CT2CA(X.operator LPCWSTR())
//nh : string to CString
#define STOC(X) CA2CT(X.c_str())
//nh : string to wstring
#define STOW(X) CA2CT(X.c_str())

#ifdef UNICODE
#define SWstring			wstring
#define SS_cout				wcout
#define sout				wstringstream
#define SSifstream			wifstream
#define SSistream			wistream
#define SWstringstream		wstringstream
#define SSostringstream		wostringstream
#define SWostream			wostream
#define SWstringstream		wstringstream
#else 
#define SWstring			string
#define SS_cout				cout
#define SSut				stringstream
#define SS_ifstream			ifstream
#define SS_istream			istream
#define SS_stringstream		stringstream
#define SS_ostringstream	ostringstream
#define SS_ostream			ostream
#define SS_stringstream     stringstream
#endif

#define fsRB				_T("rb")
#define fsWB				_T("wb")
#define fsAB				_T("ab")
#define fsRT				_T("rt")
#define fsWT				_T("wt")
#define fsAT				_T("at")

#define SS_fopen		_tfopen
#define SS_fopen_s		_tfopen_s
#define SS_printf		_tprintf
#define SS_sprintf		_stprintf
#define SS_sprintf_s	_stprintf_s
#define SS_fprintf		_ftprintf
#define SS_fputs		_fputts
#define SS_strcpy		_tcscpy
#define SS_strlen		_tcslen
#define SS_strcpy_s		_tcscpy_s
#define SS_strcat		_tcscat
#define SS_strftime		_tcsftime
#define SS_vsprintf		_vstprintf
#define SS_vsprintf_s	_vstprintf_s
#define SS_strtok		_tcstok 
#define SS_strtok_s		_tcstok_s

#define SS_strtol		_tcstol
#define SS_atof			_tstof 
#define SS_strstr		_tcsstr 
#define CAutoPtr		auto_ptr
#define not				!
#define M_PI			3.14159265358979323846
#define WCHAR_MAX_LENGTH 2048