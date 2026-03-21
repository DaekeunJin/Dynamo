#pragma once
#include "UserSetting.h"


#define COLOR_MAP_START_X	5
#define COLOR_MAP_START_Y	20
#define COLOR_MAP_N_GRID	8
#define COLOR_PIXEL_SIZE	20

#define N_COLOR_DRAW		2
#define MAX_COLOR_ITEM			5

// CColorSelectDlg 대화 상자
class CSerialComApp;

class CColorSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorSelectDlg)

public:
	CColorSelectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColorSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ColorSelectDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSerialComApp* pAppMain;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void DrawColorMap();
	CString m_strMessage;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CDC m_MemDC_Colormap;
	CBitmap m_Bitmap_Colormap;
	CBitmap *m_pOldBitmap_Colormap;
	void DrawInit();

	int m_iLimitColorMap[2][2];
	int m_Color[N_COLOR_DRAW][3][MAX_COLOR_ITEM];
	void DrawSelectColor();
	BOOL m_bDrawSelectColor[N_COLOR_DRAW];
	afx_msg void OnBnClickedSelectcolor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strPreRGB[MAX_COLOR_ITEM];
	afx_msg void OnBnClickedCancel();
	void UpdateControl();

	CComboBox m_ctrlLineType;
	int m_iPenType;
	afx_msg void OnCbnSelchangePentype();
	BOOL m_bReqRedraw;
	int m_iPenThickness;
	BOOL m_bUseMark;
	afx_msg void OnBnClickedDrawmark();
	afx_msg void ChangeColorProperty();
	CString m_strColor[3][MAX_COLOR_ITEM];  // [3:Color][Item]
	afx_msg void OnBnClickedGetdefaultcolor();
	int m_iMode;
	void ShowControl();
	int m_iSelectItem;
	afx_msg void OnBnClickedGetdefaultcolor1();
	afx_msg void OnBnClickedGetdefaultcolor2();
	afx_msg void OnBnClickedSelectitem0();

	void DrawSubColorMap();
	int m_iLimitSubColorMap[2][2];
	int m_R, m_G, m_B;
	void CopySubColorMap();
	void DrawSubColorMapChecked(int y);
	void GetBkColor();
	COLORREF m_BK{ 0 };
	BOOL m_applyBK{ FALSE };
	BOOL m_bSubColorMode{ FALSE };
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRUp();
	afx_msg void OnBnClickedRUp2();
	afx_msg void OnBnClickedRUp3();
	afx_msg void OnBnClickedRDn();
	afx_msg void OnBnClickedRDn2();
	afx_msg void OnBnClickedRDn3();
	CComboBox m_ctrlInc;
};
