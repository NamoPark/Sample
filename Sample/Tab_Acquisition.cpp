// Tab_Acquisition.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "Tab_Acquisition.h"
#include "afxdialogex.h"


// Tab_Acquisition 대화 상자

IMPLEMENT_DYNAMIC(Tab_Acquisition, CDialogEx)

Tab_Acquisition::Tab_Acquisition(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_ACQUISITION, pParent)
{

}

Tab_Acquisition::~Tab_Acquisition()
{
}

void Tab_Acquisition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL Tab_Acquisition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	pParent = (CSampleDlg *)AfxGetMainWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
BEGIN_MESSAGE_MAP(Tab_Acquisition, CDialogEx)
//	ON_WM_NCDESTROY()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// Tab_Acquisition 메시지 처리기


//void Tab_Acquisition::OnNcDestroy()
//{
//	CDialogEx::OnNcDestroy();
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}


void Tab_Acquisition::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	pParent->pTab_Calibration = NULL;
	delete this;
	CDialogEx::PostNcDestroy();
}


void Tab_Acquisition::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnCancel();
}


void Tab_Acquisition::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}


