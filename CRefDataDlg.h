#pragma once
#include "SerialComDlg.h"

// CRefDataDlg 대화 상자

class CRefDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRefDataDlg)

public:
	CSerialComDlg* pMain;
	CRefDataDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRefDataDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RefData };
#endif

private:
	void AddColumn();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_RefList;

	virtual BOOL OnInitDialog();
	int m_iSourceMode;
	int m_iTargetMode;
	afx_msg void ChangeSourceMode();
	afx_msg void ChangeTargetMode();
	afx_msg void OnBnClickedReqsave();
	void SaveRefData(void);
	int m_iReferenceDataCount;
	int m_iSelectData;
	afx_msg void SelectRefDataItem(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iSource;
	int m_iTarget;
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedType1();
	afx_msg void OnBnClickedType0();
	afx_msg void OnEnChangeSaverange0();
	afx_msg void OnEnChangeSaverange1();
	void CheckMemory();
	afx_msg void OnBnClickedTargettype0();
	afx_msg void OnBnClickedTargettype1();
};
