﻿// SS_Dll.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "SS_Dll.h"
#include "SS_Calibration.h"
#include "SS_Caputre.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CSS_DllApp

BEGIN_MESSAGE_MAP(CSS_DllApp, CWinApp)
END_MESSAGE_MAP()


// CSS_DllApp 생성

CSS_DllApp::CSS_DllApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CSS_DllApp 개체입니다.

CSS_DllApp theApp;
// CSS_DllApp 초기화

BOOL CSS_DllApp::InitInstance()
{
	CWinApp::InitInstance();

	SSstring TestModelName(L"OD1012");
	pSSLogger = new CLogger<CIntraProcessLock>(LogLevel::Info, TestModelName);
	SS_LOG((*pSSLogger), LogLevel::Info, _T("ProgramStart"));

	ssFrameSave = new SS_FrameSaver;

	nRet = WSAStartup(0x0101, &wsaData);
	if (nRet)
	{
		//		fprintf(stderr,_T("\nWSAStartup() error : %d\n"), nRet);
		WSACleanup();
		FreeLibrary((HINSTANCE)hndlIcmp);
		return -1;
	}

	if (0x0101 != wsaData.wVersion)
	{
		//		fprintf(stderr,_T("\nThis computer cannot support winsock 1.1\n"));
		WSACleanup();
		FreeLibrary((HINSTANCE)hndlIcmp);
		return -1;
	}
	//log class create
	return true;
}

void CSS_DllApp::CreateCalibration()
{
	m_pD_Calibration = new SS_Calibration();
}

int CSS_DllApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete ssFrameSave;
	delete pSSLogger;
	delete m_pD_Calibration;
	ssDeInitialize();
	return CWinApp::ExitInstance();
}

int bCloseMutex = 0;
UINT CSS_DllApp::CloseThread(LPVOID pParam)
{
	ACQ_Close();
	return 0;
}
int CSS_DllApp::mMutex(int & pIMutex, int pCnt)
{
	int Cnt = 0;
	while (pIMutex) {
		Sleep(20);
		Cnt += 20;
		if (Cnt >= pCnt)
		{
			pIMutex = 0;
		}
	}
	return 0;
}