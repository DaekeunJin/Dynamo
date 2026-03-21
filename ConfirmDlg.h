#pragma once
#include "SettingDlg.h"

// CConfirmDlg 대화 상자

class CConfirmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfirmDlg)
	int m_num;
public:
	CConfirmDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConfirmDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ConfimrDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeAnswer();
	CSettingDlg* pSetting;
	virtual BOOL OnInitDialog();
	CString m_strJob;
};
