#pragma once

class CSerialComDlg;

// CTextLogDlg 대화 상자

class CTextLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextLogDlg)

public:
	CTextLogDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTextLogDlg();
	CSerialComDlg *pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TextLog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTest();
	void OnCancel();
	afx_msg void OnBnClickedHide();
};
