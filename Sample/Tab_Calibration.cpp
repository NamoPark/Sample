﻿// Tab_Calibration.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "Tab_Calibration.h"
#include "afxdialogex.h"

// Tab_Calibration 대화 상자

IMPLEMENT_DYNAMIC(Tab_Calibration, CDialogEx)

struct TContextData
{
	SS_AcqHandle Handle;
	FILE *fsFullImage;
	int FrameWidth;
	int FrameHeight;
};


void __cdecl FullDataHandler(void *pContextData, void *pData, int nSize)
{
	TContextData *pFileData = (TContextData *)pContextData;

	FILE *fs = (FILE *)(pFileData->fsFullImage);
	int nDataSize = pFileData->FrameWidth * pFileData->FrameHeight * sizeof(short);

	if (fs && nSize == nDataSize)
	{
		fwrite(pData, nDataSize, 1, fs);
		fclose(fs);
	}
	else
	{
		//CERR << "invalid full-data size." << endl;
	}
}
void __cdecl EventHandler(void *pContextData, int nEventId)
{
	TContextData *pData = (TContextData *)pContextData;

	switch (nEventId)
	{

	default:
		break;
	}
}

void __cdecl ExceptionHandler(void *pContextData, int nExpId)
{
	switch (nExpId)
	{

	default:
		break;
	}
}


Tab_Calibration::Tab_Calibration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_CALIBRATION, pParent)
{
	AcqHandle = NULL;
}

Tab_Calibration::~Tab_Calibration()
{
	ssDestroyConnection(AcqHandle);
}

void Tab_Calibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAL_VIEW, pCal_View);
	DDX_Control(pDX, IDC_GETDARK, btn_GetDark);
	DDX_Control(pDX, IDC_GETBRIGHT, btn_GetBright);
}


BEGIN_MESSAGE_MAP(Tab_Calibration, CDialogEx)
	ON_BN_CLICKED(IDC_GETDARK, &Tab_Calibration::OnBnClickedGetdark)
	ON_BN_CLICKED(IDC_GETBRIGHT, &Tab_Calibration::OnBnClickedGetbright)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Tab_Calibration 메시지 처리기

BOOL Tab_Calibration::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	pParent = (CSampleDlg *)AfxGetMainWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void Tab_Calibration::OnBnClickedGetdark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 1.CreateConnection
	AcqHandle = ssCreateConnection(theApp.tDetectorHandle);
	if (AcqHandle == NULL) 
	{
		MessageBox(_T("Connection Fail"));
	}

	// 2.SetHandler
	int bOk = false;
	bOk = ssSetDataHandler(AcqHandle, FullDataHandler);
	if(!bOk)
		MessageBox(_T("SetDataHandler Fail"));
	bOk = ssSetEventHandler(AcqHandle, EventHandler);
	if (!bOk)
		MessageBox(_T("SetEventHandler Fail"));
	bOk = ssSetExceptionHandler(AcqHandle, ExceptionHandler);
	if (!bOk)
		MessageBox(_T("SetExceptionHandler Fail"));

	// 3.
	int bResult = ssCaptureStart(AcqHandle, SS_GetDark);
}


void Tab_Calibration::OnBnClickedGetbright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

int Tab_Calibration::InitTabDlg()
{
	this->GetWindowRect(&TabDlgRect);
	ViewerStartPoint.x = TabDlgRect.Width()*0.2;
	ViewerStartPoint.y = 25;
	ViewerSize.cx = TabDlgRect.Width()*0.6;
	ViewerSize.cy = ViewerSize.cx / 6 * 5;
	pCal_View.MoveWindow(ViewerStartPoint.x, ViewerStartPoint.y, ViewerSize.cx, ViewerSize.cy);
	pCal_View.GetWindowRect(&TabViwerRect);

	return 0;
}


void Tab_Calibration::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	pParent->pTab_Calibration = NULL;
	delete this;
	CDialogEx::PostNcDestroy();
}


void Tab_Calibration::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnCancel();
}


void Tab_Calibration::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}
