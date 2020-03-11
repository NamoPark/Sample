// Tab_Setting.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "SampleDlg.h"
#include "Tab_Setting.h"
#include "afxdialogex.h"


// Tab_Setting 대화 상자

IMPLEMENT_DYNAMIC(Tab_Setting, CDialogEx)

Tab_Setting::Tab_Setting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_SETTING, pParent)
{
}

Tab_Setting::~Tab_Setting()
{
	//nh ssFunction
	ssDestroyDetector(theApp.tDetectorHandle);
}

void Tab_Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL_LIST_CTRL, m_Model_ListCtrl);
}


BEGIN_MESSAGE_MAP(Tab_Setting, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Tab_Setting 메시지 처리기

BOOL Tab_Setting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	//nh 추후 수정, 선택 시 AppIni initialize

	pParent = (CSampleDlg *)AfxGetMainWnd();

	CString cstr_AppIniPath;
	TCHAR l_strBuf[WCHAR_MAX_LENGTH];
	GetModuleFileName(AfxGetInstanceHandle(), l_strBuf, WCHAR_MAX_LENGTH);
	cstr_AppIniPath = l_strBuf;
	cstr_AppIniPath = cstr_AppIniPath.Left(cstr_AppIniPath.GetLength() - 4); // Remove extension (.exe)
	cstr_AppIniPath = cstr_AppIniPath + _T(".ini");

	//nh ssFunction
	theApp.tDetectorHandle = ssCreateDetector(cstr_AppIniPath);
	if (theApp.tDetectorHandle == nullptr)
	{
		MessageBox(_T("Detector Create Fail"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Tab_Setting::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	pParent->pTab_Calibration = NULL;
	delete this;
	CDialogEx::PostNcDestroy();
}


void Tab_Setting::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnCancel();
}


void Tab_Setting::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}
