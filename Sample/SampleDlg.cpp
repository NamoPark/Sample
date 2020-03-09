
// SampleDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "SampleDlg.h"
#include "afxdialogex.h"

#include "Tab_Acquisition.h"
#include "Tab_Calibration.h"
#include "Tab_Setting.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSampleDlg 대화 상자



CSampleDlg::CSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSampleDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()



// CSampleDlg 메시지 처리기

BOOL CSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(false);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, true);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, false);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//nh UI
	this->SetWindowPos(NULL, 0, 0, MAIN_DLG_WIDTH, MAIN_DLG_HEIGHT, SWP_NOREPOSITION);
	m_Tab.SetWindowPos(NULL, 0, 0, MAIN_DLG_WIDTH - TAB_DLG_OFFSET_WIDTH, MAIN_DLG_HEIGHT - TAB_DLG_OFFSET_HEIGHT, SWP_NOREPOSITION);
	m_Tab.InsertItem(0, _T("Setting"));
	m_Tab.InsertItem(1, _T("Calibration"));
	m_Tab.InsertItem(2, _T("Acquisition"));
	
	m_Tab.SetCurSel(0);

	CRect rect;
	int temp_height = 25;
	m_Tab.GetWindowRect(&rect);

	pTab_Setting = new Tab_Setting;
	pTab_Setting->Create(IDD_TAB_SETTING,&m_Tab);
	pTab_Setting->SetWindowPos(NULL, 5, 25, rect.Width()-10, rect.Height() - 30, SWP_NOREPOSITION);
	pTab_Setting->ShowWindow(SW_SHOW);

	pTab_Calibration = new Tab_Calibration;
	pTab_Calibration->Create(IDD_TAB_CALIBRATION, &m_Tab);
	pTab_Calibration->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOREPOSITION);
	pTab_Calibration->ShowWindow(SW_HIDE);

	pTab_Acquisition = new Tab_Acquisition;
	pTab_Acquisition->Create(IDD_TAB_ACQUISITION,&m_Tab);
	pTab_Acquisition->SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOREPOSITION);
	pTab_Acquisition->ShowWindow(SW_HIDE);

	//nh AppIni initialize
	CString cstr_AppIniPath;
	TCHAR l_strBuf[WCHAR_MAX_LENGTH];
	GetModuleFileName(AfxGetInstanceHandle(), l_strBuf, WCHAR_MAX_LENGTH);
	cstr_AppIniPath = l_strBuf;
	cstr_AppIniPath = cstr_AppIniPath.Left(cstr_AppIniPath.GetLength() - 4); // Remove extension (.exe)
	cstr_AppIniPath = cstr_AppIniPath + _T(".ini");
	temp_detector = ssCreateDetector(cstr_AppIniPath);
	if (temp_detector == nullptr) 
	{
		MessageBox(_T("Detector Create Fail"));
	}

	return true;  // 포커스를 컨트롤에 설정하지 않으면 true를 반환합니다.
}

void CSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSampleDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DELETE_S(pTab_Setting);
	DELETE_S(pTab_Calibration);
	DELETE_S(pTab_Acquisition);
	ssDestroyDetector(temp_detector);
}


void CSampleDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_Tab.GetCurSel();

	switch (sel) 
	{
		case TAB_SETTING:
			pTab_Setting->ShowWindow(SW_SHOW);
			pTab_Calibration->ShowWindow(SW_HIDE);
			pTab_Acquisition->ShowWindow(SW_HIDE);
			break;
		case TAB_CALIBRATION:
			pTab_Setting->ShowWindow(SW_HIDE);
			pTab_Calibration->ShowWindow(SW_SHOW);
			pTab_Acquisition->ShowWindow(SW_HIDE);
			break;
		case TAB_ACQUISITION:
			pTab_Setting->ShowWindow(SW_HIDE);
			pTab_Calibration->ShowWindow(SW_SHOW);
			pTab_Acquisition->ShowWindow(SW_HIDE);
			break;
	}
	*pResult = 0;
}
