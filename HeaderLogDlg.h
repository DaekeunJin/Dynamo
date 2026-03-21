#pragma once
#include "UserSetting.h"


// CHeaderLogDlg 대화 상자
class CSerialComDlg;

#define N_TEXTLOG_ITEM  12

class CHeaderLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHeaderLogDlg)

public:
	CHeaderLogDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHeaderLogDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HeaderLogDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	CSerialComDlg* pMain;
	afx_msg void OnEnChangeHeaderlogHeader();

	unsigned char m_cHeader;
	CString m_strHeader;
	int m_iTextLogCount;
	virtual BOOL OnInitDialog();
	void GetHeaderLog();
	void AddColumn();
	CListCtrl m_ctrlHeaderLog;
	afx_msg void OnClickItemlist(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iSelectItem;
	afx_msg void OnBnClickedAddheaderlogitem();	
	afx_msg void OnBnClickedDeleteheaderlogitem();

	CComboBox m_ctrlDataID;
	CComboBox m_ctrlCondition;
	afx_msg void OnSelchangeCondition();
	void ShowControl(int iCondition);
	afx_msg void OnEnChangeValuehex();
	afx_msg void OnBnClickedBin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL IsValid_Input(int iCondition, CString* strError);
	afx_msg void OnEnChangeMaskvalue();
	afx_msg void OnEnChangeValue();
	afx_msg void OnEnChangeValuehex2();
	afx_msg void OnBnClickedModifyheaderlogitem();

	void GetCurrentControl(int iSelectItem);
	BOOL IsAvailableDataID(CString *pstr, int* type, int* id);
	BOOL IsAvailableDataID(CString *pstr);
	void ShowBit(int iMode);
	afx_msg void OnBnClickedSaveheaderlog();
	void ShowHeaderLogData(int num, CTextLogSetting* data);
	BOOL m_bIsUpdated;
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedImport();
	
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedDeleteall();
	afx_msg void OnPaint();
	void DrawBitControl();

	BOOL m_bMask[16];
	BOOL m_bValue[16];
	afx_msg void OnBnClickedMoveup();
	afx_msg void OnBnClickedMovedn();
	static const char* const m_strCond[];	
    CComboBox m_ctrlDP;
    CComboBox m_ctrlDivide;
};
