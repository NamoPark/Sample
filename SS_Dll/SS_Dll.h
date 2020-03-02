// SS_Dll.h : SS_Dll DLL의 주 헤더 파일
//

#pragma once
#include "SS_Logger/Logger.h"
#include "SS_Param.h"
#include "GIniFile.h"
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


class SS_Calibration;
typedef struct _tDLIB_CBR tDLIB_CBRec;

// ----- Detector Model -----
enum class DetModel {
	//DM_Custom = 0,
	DM_SA130_Solution = 0,
	DM_SA132_Solution = 1,
};
typedef struct _tDM_List DM_List;

// CSS_DllApp
// 이 클래스 구현에 대해서는 SS_Dll.cpp를 참조하세요.
//

class CSS_DllApp : public CWinApp
{
public:
	CSS_DllApp();

// 재정의입니다.
public:
	WSADATA	wsaData;
	HANDLE hndlIcmp;
	int nRet;
	virtual BOOL InitInstance();
	CLogger<CIntraProcessLock>* pSSLogger;
	SS_App_Param* ssAppParam;
	SS_Calibration* m_pD_Calibration = nullptr;
	SS_FrameSaver* ssFrameSave;
	CWinThread *m_pThread;
	tDLIB_CBRec* m_DLIB_CBR;
	int mMutex(int &pIMutex, int pCnt);
	static UINT CloseThread(LPVOID pParam);
	void CreateCalibration();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

};

extern CSS_DllApp theApp;
