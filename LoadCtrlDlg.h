#pragma once


// CLoadCtrlDlg 대화 상자
#include  "SerialComDlg.h"

//class CSerialComDlg;

class CLoadCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadCtrlDlg)

public:
	CLoadCtrlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLoadCtrlDlg();
	CSerialComDlg* pMain;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LoadControlDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSavesetting();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedUseprotectionOvervoltge();
};
