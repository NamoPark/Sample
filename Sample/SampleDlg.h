
// SampleDlg.h: 헤더 파일
//

#pragma once


template<typename T>
inline void DELETE_S(T& p)
{
	delete p;
	p = NULL;
}

template<typename T>
inline void DELETE_ARR_S(T& p)
{
	delete[] p;
	p = NULL;
}

#define TAB_SETTING				0
#define TAB_CALIBRATION			1
#define TAB_ACQUISITION			2

#define MAIN_DLG_WIDTH			800
#define MAIN_DLG_HEIGHT			600

#define TAB_DLG_OFFSET_WIDTH	30	
#define TAB_DLG_OFFSET_HEIGHT	50

class Tab_Acquisition;
class Tab_Calibration;
class Tab_Setting;
// CSampleDlg 대화 상자
class CSampleDlg : public CDialogEx
{
// 생성입니다.
public:
	CSampleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:

	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CTabCtrl m_Tab;
	Tab_Setting* pTab_Setting;
	Tab_Calibration* pTab_Calibration;
	Tab_Acquisition* pTab_Acquisition;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};
