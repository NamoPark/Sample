
// SampleDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Sample.h"
#include "SampleDlg.h"
#include "afxdialogex.h"

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
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Capture, &CSampleDlg::OnBnClickedCapture)
	ON_BN_CLICKED(IDC_Stop, &CSampleDlg::OnBnClickedStop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

#define STR(A) _T(#A)
#define TEST 2000

static void _stdcall CBP_Dark(tDLIB_CBRec *ACallBackRec)
{
	CSampleDlg *pParent = (CSampleDlg*)ACallBackRec->rUserParam;
	int CapturePercent = ACallBackRec->rCapturePercent;
	int AvrgEnable = ACallBackRec->RouteFunc;

	switch (ACallBackRec->rType) {
	case ETWSAErr: {
		break;
	}
	case ETErr: {
		switch (ACallBackRec->rEvent) {
		case EAbort:
		{
			break;
		}
		case ECapture:
		{
			break;
		}
		}
		break;
	}
	case ETTrace: {
		switch (ACallBackRec->rEvent) {
		case EAck_Start: {
			break;
		}
		case EClose:
		{
			break;
		}
		case ECapturePerc:
		{
			if (ACallBackRec->rCapturePercent >= 100)
			{
				//
				//ssCaptureStop();
				TRACE(_T("\nRecv Done\n"));
			}
			break;
		}
		}; // switch rEvent
		break;
	}
	}; // switch rType
}


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
	
	//nhPark AppIni initialize

	TCHAR l_strBuf[WCHAR_MAX_LENGTH];
	GetModuleFileName(AfxGetInstanceHandle(), l_strBuf, WCHAR_MAX_LENGTH);
	theApp.cstr_AppIniPath = l_strBuf;
	theApp.cstr_AppIniPath = theApp.cstr_AppIniPath.Left(theApp.cstr_AppIniPath.GetLength() - 4); // Remove extension (.exe)
	theApp.cstr_AppIniPath = theApp.cstr_AppIniPath + _T(".ini");
	bAppInitialize = ssInitialize(theApp.cstr_AppIniPath);

	if (!bAppInitialize) 
	{
		MessageBox(_T("App Initialize Fail"));
	}
	theApp.m_AppDlgParam = ssGetAppParam();


	if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
	{
		DWORD dwError = GetLastError();
		CString strMsg;
		strMsg.Format(_T("Failed to enter ABOVE_NORMAL_PRIORITY_CLASS mode (%d)"), dwError);
		AfxMessageBox(strMsg);
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



void CSampleDlg::OnBnClickedCapture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_AppDlgParam = ssGetAppParam();
	if (theApp.m_AppDlgParam == NULL)
	{
		MessageBox(_T("App Initialize Fail"));
		return;
	}

	memset(&m_AcqPar, 0, sizeof(tAcqPar));

	m_AcqPar.ExpTime = theApp.m_AppDlgParam->WndTime;
	m_AcqPar.ExpDelay = theApp.m_AppDlgParam->ExpDelay;
	m_AcqPar.PostDelay = theApp.m_AppDlgParam->PostDelay;
	m_AcqPar.PreDelay = theApp.m_AppDlgParam->PreDelay;
	m_AcqPar.RdyDelay = theApp.m_AppDlgParam->RdyDelay;
	m_AcqPar.bAverageDark = theApp.m_AppDlgParam->AverageDark;
	m_AcqPar.nAverageTotal = theApp.m_AppDlgParam->TotalDark;
	m_AcqPar.nAverageSkip = theApp.m_AppDlgParam->SkipDark;
	// shlee
	m_AcqPar.MultiFrame = 0;

	ssCaptureStart(CBP_Dark, this, &m_AcqPar, 1);
}


void CSampleDlg::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ssCaptureStop();
}



void CSampleDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
