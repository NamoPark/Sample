#pragma once


// Tab_Calibration 대화 상자

class Tab_Calibration : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_Calibration)

public:
	Tab_Calibration(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Tab_Calibration();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_CALIBRATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic pCal_View;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGetdark();
	afx_msg void OnBnClickedGetbright();
};
