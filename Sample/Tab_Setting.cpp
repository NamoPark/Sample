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
}

void Tab_Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL_LIST_CTRL, m_Model_ListCtrl);
}


BEGIN_MESSAGE_MAP(Tab_Setting, CDialogEx)
END_MESSAGE_MAP()


// Tab_Setting 메시지 처리기


BOOL Tab_Setting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Model_ListCtrl.SetWindowPos(NULL, 0, 20, MAIN_DLG_WIDTH/2, MAIN_DLG_HEIGHT/2, SWP_NOREPOSITION);
	m_Model_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
