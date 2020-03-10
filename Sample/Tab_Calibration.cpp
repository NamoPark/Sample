// Tab_Calibration.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "Tab_Calibration.h"
#include "afxdialogex.h"


#include "../SS_Dll/SS_Interface.h"


// Tab_Calibration 대화 상자

IMPLEMENT_DYNAMIC(Tab_Calibration, CDialogEx)

Tab_Calibration::Tab_Calibration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_CALIBRATION, pParent)
{

}

Tab_Calibration::~Tab_Calibration()
{
}

void Tab_Calibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAL_VIEW, pCal_View);
}


BEGIN_MESSAGE_MAP(Tab_Calibration, CDialogEx)
	ON_BN_CLICKED(IDC_GETDARK, &Tab_Calibration::OnBnClickedGetdark)
	ON_BN_CLICKED(IDC_GETBRIGHT, &Tab_Calibration::OnBnClickedGetbright)
END_MESSAGE_MAP()


// Tab_Calibration 메시지 처리기

BOOL Tab_Calibration::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void Tab_Calibration::OnBnClickedGetdark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ssCreateConnection(theApp.tDetectorHandle);
}


void Tab_Calibration::OnBnClickedGetbright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
