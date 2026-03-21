#pragma once
#include "UserSetting.h"
#include "FftDlg.h"

// PortSetting 대화 상자
#define ResultGraph_MinX 90
#define ResultGraph_MinY 400

class CSerialComDlg;


class CStatisticsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatisticsDlg)

public:
    CStatisticsDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStatisticsDlg();
	CSerialComDlg* pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StatisticsDlg};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strRmsStartIndex;
	CString m_strRmsEndIndex;

	BOOL OnBnClickedCalculation();
	CListCtrl m_ResultList;
	afx_msg void OnColumnclickResultlist(NMHDR *pNMHDR, LRESULT *pResult);
	void AddColumn();
	virtual BOOL OnInitDialog();
	BOOL OnEnChangeRmsStartindex();
	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strMessage;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_iHistoryIndex;
	int m_iStartIndex, m_iEndIndex;
	int m_iMaxValue, m_iMinValue;
	int m_iSelecIndexMode;
	afx_msg void OnBnClickedSelectindexmode();
	afx_msg void OnBnClickedLoaddefaultindex();
	afx_msg void OnBnClickedSavedata();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_iResultGraph_MaxX;
	int m_iResultGraph_MaxY;

	afx_msg void OnBnClickedDeterminindexmode();
	afx_msg void OnClose();
	BOOL m_bClearVariable;
	afx_msg void OnCancel();
	BOOL m_bSaveResult;
	BOOL m_bSaveRawData;
	void UpdateControlData();
	BOOL m_bMathResult;
	afx_msg void OnBnClickedSavemathresult();
	int m_iSelectList;
	afx_msg void OnClickResultlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDrawfft();
	CString m_strSpanTime;
	afx_msg void OnBnClickedSavedata2();
	int CopyTextToClipboard(const char *ap_string);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	long m_iSamplingTime;
	CString Dbl2Str(double fValue);
	afx_msg void OnBnClickedSavedata3();
};
