// SS_Dll.h : SS_Dll DLL의 주 헤더 파일
//

#pragma once
#include "SS_Logger/Logger.h"
#include "SS_CommonUtils/SS_FrameSaver.h"
#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

using namespace framework::Threading;
using namespace framework::Diagnostics;

#define DARK	0x01
#define BRIGHT	0x02
#define ACQIMG  0x03
#define DARKOFFSET	0x04

// CSS_DllApp
// 이 클래스 구현에 대해서는 SS_Dll.cpp를 참조하세요.
//

class CSS_DllApp : public CWinApp
{
public:
	CSS_DllApp();

// 재정의입니다.
public:
	HANDLE hndlIcmp;
	CLogger<CIntraProcessLock>* pSSLogger;

	DECLARE_MESSAGE_MAP()
	virtual BOOL InitInstance();
	virtual int ExitInstance();

};

extern CSS_DllApp theApp;
