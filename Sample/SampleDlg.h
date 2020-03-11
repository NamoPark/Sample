
// SampleDlg.h: 헤더 파일
//

#pragma once


template<typename T>
inline void DELETE_S(T& p)
{
	if (p != NULL) {
		delete p;
		p = NULL;
	}
}

template<typename T>
inline void DELETE_ARR_S(T& p)
{
	if (p != NULL) {
		delete[] p;
		p = NULL;
	}
}

#define TAB_SETTING				0
#define TAB_CALIBRATION			1
#define TAB_ACQUISITION			2

//
#define INIT_OFFSET_WIDTH				8
#define INIT_OFFSET_HEIGHT				240

#define MAIN_DLG_WIDTH					800
#define MAIN_DLG_HEIGHT					700

#define TAB_DLG_WIDTH					(MAIN_DLG_WIDTH - (INIT_OFFSET_WIDTH *2))
#define TAB_DLG_HEIGHT					(MAIN_DLG_HEIGHT - INIT_OFFSET_HEIGHT)

#define TAB_OFFSET_WIDTH				5
#define TAB_OFFSET_HEIGHT				30

#define TAB_ATTACH_DLG_WIDTH			TAB_DLG_WIDTH - TAB_OFFSET_WIDTH*2
#define TAB_ATTACH_DLG_HEIGHT			TAB_DLG_HEIGHT - TAB_OFFSET_HEIGHT

#define LOG_SUB_OFFSET_HEIGHT			10
#define LOG_SUB_WIDTH					TAB_DLG_WIDTH*0.7
#define LOG_SUB_HEIGHT					MAIN_DLG_HEIGHT - TAB_ATTACH_DLG_HEIGHT- LOG_SUB_OFFSET_HEIGHT
#define LOG_EDIT_OFFSET_WIDTH			5
#define LOG_EDIT_OFFSET_HEIGHT			10

#define CONNECTION_WIDTH				(TAB_DLG_WIDTH)*0.3
#define CONNECTION_HEIGHT				(LOG_SUB_HEIGHT)/2


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
	Tab_Setting* pTab_Setting = NULL;
	Tab_Calibration* pTab_Calibration = NULL;
	Tab_Acquisition* pTab_Acquisition = NULL;

	CStatic m_nConnectionSub;
	CStatic m_nCommonSub;
	CStatic m_nLogSub;
	CButton m_bCommonSetting;
	CButton m_bSAVE;
	CButton m_bSaveExit;
	CEdit m_nLogEdit;

	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedBtnCommonSetting();
	afx_msg void OnBnClickedButtonDlgSave();
	afx_msg void OnBnClickedButtonDlgSaveExit();
	afx_msg void OnClose();
};
