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


BEGIN_MESSAGE_MAP(Tab_Acquisition, CDialogEx)
END_MESSAGE_MAP()


// Tab_Acquisition 메시지 처리기
