#pragma once
#include "SerialComDlg.h"


// CNavilog 대화 상자

class CNavilog : public CDialogEx
{
	DECLARE_DYNAMIC(CNavilog)

public:
	CNavilog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNavilog();
	CSerialComDlg* pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NaviLog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTest();

	
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSavephase();
	int m_iTimeMode;
};
