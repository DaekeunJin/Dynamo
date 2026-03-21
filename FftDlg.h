#pragma once
#include "UserSetting.h"

class CSerialComDlg;
// CFftDlg 대화 상자

class CFftDlg : public CDialogEx {
	DECLARE_DYNAMIC(CFftDlg)

public:
	CFftDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFftDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DftDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSerialComDlg* pMain;
	int m_iStartIndex;
	int m_iEndIndex;
	int m_iN_FFT;
	virtual BOOL OnInitDialog();
	CString m_strItemName;
	BOOL m_bIsMath;
	BOOL m_iDataChannel;
	CString m_strDataPeriod;
	afx_msg void ChangeControl();
	double m_fDataPeriod;
	afx_msg void OnBnClickedReqdraw();
	double m_FFT[N_MAX_FFT];
	int m_iScaleModeX;
	CString m_strMessage;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	int m_iDrawLimit[2][2];
	void CalculateFFT();
	afx_msg void OnPaint();
	double m_fFreqLimit[2];
	double m_fValueLimit[2];
	CPoint Cal_DrawaPosition(double x, double y);
	void DrawFFT();	
	BOOL ReqInvalidate;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strValuetLimit[3];
	BOOL m_bIsCalculated;	
	CEdit m_ctrlMaxY;
	CEdit m_ctrlMidY;
	CEdit m_ctrlMinY;
	BOOL m_bShowMark;
	BOOL m_bShowLine;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int m_iSelectIndex;
	BOOL m_bReqDraw;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
