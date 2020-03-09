#pragma once


// Tab_Acquisition 대화 상자

class Tab_Acquisition : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_Acquisition)

public:
	Tab_Acquisition(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Tab_Acquisition();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_ACQUISITION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
