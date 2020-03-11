#pragma once

// Tab_Calibration 대화 상자

#include "../SS_Dll/SS_Interface.h"
#include "SampleDlg.h"

#define WIDTH_OFFSET	5
#define HEIGHTOFFSET	5
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
	SS_AcqHandle AcqHandle;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGetdark();
	afx_msg void OnBnClickedGetbright();
	int InitTabDlg();
private:
	CRect TabDlgRect;
	CRect TabViwerRect;
	CPoint ViewerStartPoint;
	CSize ViewerSize;

	CStatic pCal_View;
public:
	CButton btn_GetDark;
	CButton btn_GetBright;
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnClose();
	CSampleDlg* pParent;
};
