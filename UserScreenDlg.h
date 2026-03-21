#pragma once
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#define TIMER_CHECK_USERSCREEN 100
#define TIMER_CHECK_FRAME_RATE 200
#define TIMER_REPLAY 300

#define US_PX 10
#define US_PY 45


// CUserScreenDlg 대화 상자
class CSerialComDlg;

class CUserScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserScreenDlg)

	int m_iHeight;
	int m_iWidth;

public:
	CUserScreenDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserScreenDlg();
	CSerialComDlg* pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UserScreen };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTestbutton();
	virtual BOOL OnInitDialog();
	void DrawBackground();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CDC m_MemDC_US, m_MemDC_Img, m_MemDC_Edit;
	CBitmap m_Bitmap_US, m_Bitmap_Img, m_Bitmap_Edit;
	CBitmap *m_pOldBitmap_US, *m_pOldBitmap_Img, *m_pOldBitmap_Edit;

	void InitMemDC();
	void UpdateDraw();



	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CPoint m_StartPT;
	CRect m_rect{ -1, -1, -1, -1 };
	void OnCancel();
	afx_msg void OnBnClickedUpP0();
	afx_msg void OnBnClickedUpP1();
	afx_msg void OnBnClickedUpD1();
	afx_msg void OnBnClickedUpC0();
	afx_msg void OnBnClickedUpC1();
	afx_msg void OnBnClickedUpC2();
	afx_msg void OnBnClickedUpD0();
	afx_msg void OnBnClickedDnP0();
	afx_msg void OnBnClickedDnP1();
	afx_msg void OnBnClickedDnD0();
	afx_msg void OnBnClickedDnD1();
	afx_msg void OnBnClickedDnC0();
	afx_msg void OnBnClickedDnC1();
	afx_msg void OnBnClickedDnC2();
	afx_msg void OnEnChangeP0();
	afx_msg void OnEnChangeP1();
	afx_msg void OnEnChangeD0();
	afx_msg void OnEnChangeD1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL m_EditMode{ FALSE };
	BOOL m_ReplayMode{ FALSE };
	afx_msg void OnBnClickedHidewindow();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL IsGraphArea(CPoint pt);

	volatile BOOL m_bReqUpdateDrawData{ FALSE };
	volatile BOOL m_bBusyDrawData{ FALSE };
	BOOL m_bReqUpdateEditData{ FALSE };
	CDrawLogData m_DrawData[N_MAX_GRAPHLOG_ITEM];
	afx_msg void OnEnChangeColor0();
	afx_msg void OnEnChangeColor1();
	afx_msg void OnEnChangeColor2();
	afx_msg void OnBnClickedFill();
	afx_msg void OnBnClickedSendrange();
	afx_msg void OnBnClickedGetrange();
	afx_msg void OnBnClickedGetc1();
	afx_msg void OnBnClickedGetc2();
	BOOL m_bAdjustDisplayDataSize;
	afx_msg void OnBnClickedAutomode();
	int m_iLastDrawFrame{ 0 };
	int m_iIndex{ 0 };
	int m_iCurrentIndexTemp{ 0 };
	void CheckFrameRate();

	void DrawTimeLine();
	int m_iTimeLineP[2];
	int m_iDrawIndex{ 0 };

	CBrush m_brush_timeline;
	CBrush m_brush_current;
	CBrush m_brush_temp;
	CPen m_pen_timeline;
	void Cal_CurrentPos(int idx);
	int m_iCurrentPos;
	BOOL m_bReqChangeIndex{ FALSE };
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CComboBox m_ctrlMode;
	afx_msg void OnCbnSelchangeMode();
	afx_msg void OnEnChangeEventtime();
	afx_msg void OnEnChangeIndexperiod();
	void SetNextIndex();
	int m_iReqReplayIndex{ -1 };
	afx_msg void OnBnClickedreplaybutton0();
	afx_msg void OnBnClickedreplaybutton4();
	afx_msg void OnBnClickedreplaybutton1();
	BOOL m_bReplayOn{ FALSE };
	afx_msg void OnBnClickedreplaybutton2();
	int m_iReplaySpeed{ 1 };
	void UpdateReplayMessage();
	int m_iReplayStatus{ 0 };
	int m_iReplayDir{ 0 };
	afx_msg void OnBnClickedreplaybutton5();
	CComboBox m_ctrlSpeed;
	afx_msg void OnCbnSelchangeReplayspeed();
	CComboBox m_ctrlReplayStop;
};
