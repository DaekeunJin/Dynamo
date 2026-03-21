#pragma once


// CReloadDataDlg 대화 상자
class CSerialComDlg;

class CReloadDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReloadDataDlg)

public:
	CReloadDataDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CReloadDataDlg();
	CSerialComDlg* pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ReloadDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedProcess();
	virtual BOOL OnInitDialog();
	void UpdateDisplay();
	afx_msg void OnEnChangeNewheader();
	int* m_pResult;
};
