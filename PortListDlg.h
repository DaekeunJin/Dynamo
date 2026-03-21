#pragma once


// CPortListDlg 대화 상자
#include "UserSetting.h"
#include "SerialComDlg.h"

class CPortListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPortListDlg)

public:
	CPortListDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPortListDlg();
	CSerialComDlg * pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ComPortList };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlComList;
	virtual BOOL OnInitDialog();
	void Init_ComPortList();
	afx_msg void OnBnClickedAddport();
	afx_msg void OnEnChangeComno();
	afx_msg void OnClickPortlist(NMHDR *pNMHDR, LRESULT *pResult);

	int m_iSelectItem;
	afx_msg void OnBnClickedModifylist();
	afx_msg void OnBnClickedDeletelist();
	afx_msg void OnBnClickedSaveport();
	afx_msg void OnBnClickedConnectlist();
	afx_msg void OnDblclkPortlist(NMHDR *pNMHDR, LRESULT *pResult);
};
