#pragma once


#include "../SS_Dll/SS_Interface.h"
#include "SampleDlg.h"
// Tab_Setting 대화 상자
#define LIST_OFFSET_WIDTH		50
#define LIST_OFFSET_HEIGHT		110 

class Tab_Setting : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_Setting)

public:
	Tab_Setting(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Tab_Setting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_SETTING };
#endif
private:
	CListCtrl m_Model_ListCtrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnClose();
	CSampleDlg* pParent;
};
