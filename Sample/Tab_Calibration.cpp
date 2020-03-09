// Tab_Calibration.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "Tab_Calibration.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(Tab_Calibration, CDialogEx)
END_MESSAGE_MAP()


// Tab_Calibration 메시지 처리기
