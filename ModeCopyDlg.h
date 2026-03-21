#pragma once
#include "UserSetting.h"

class CSerialComDlg;

// CModeCopyDlg 대화 상자

class CModeCopyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModeCopyDlg)
	
public:
	CModeCopyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CModeCopyDlg();
	CSerialComDlg* pMain;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ModeCopyDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReqcopy();
	CComboBox m_ctrlSourceMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeSourceheader();
	afx_msg void OnBnClickedSelectall();
	BYTE m_cTargetHead{ 'z' };
	BYTE m_cSourceHead{ 'z' };
	afx_msg void OnEnChangeTargetheader();
	afx_msg void OnBnClickedAllpresetcheck();
};
