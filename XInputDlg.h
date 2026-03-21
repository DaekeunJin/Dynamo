#pragma once
#include "UserSetting.h"
class CSerialComDlg;
// CXInputDlg 대화 상자

class CXInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CXInputDlg)

public:
	CXInputDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CXInputDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XInput };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CSerialComDlg* pMain{nullptr};
	int m_iEnableKeyMode;
	afx_msg void OnBnClickedUseenablekey();
	void ChangeEnableKey(BOOL bEnable);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApply();
	void Get_XInputSetting_from_Main();
    void Set_XInputSetting_from_Control();
    afx_msg void UpdateControl();
    afx_msg void OnBnClickedExit();
    afx_msg void OnBnClickedReload();
   
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    CDC m_MemDC, m_MemDC2;
    CBitmap m_Bitmap, m_Bitmap2;
    CBitmap *m_pOldBitmap { nullptr };
    CBitmap *m_pOldBitmap2 { nullptr };
    void Init_DC();
    void Show_XPad();
    void Update_XPad();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedUsepacketheader();
    afx_msg void OnEnChangeXinputheader();
    
    afx_msg void OnChangeSendtype();    
    void Circle(CClientDC* pDC, int x, int y, int r);

    int m_idc_use[8] { IDC_PressButton0, IDC_PressButton1, IDC_PressButton2, IDC_PressButton3, IDC_PressButton4, IDC_PressButton5, IDC_PressButton6, IDC_PressButton7 };
    int m_idc_ch_p[8] { IDC_SendChar0, IDC_SendChar1, IDC_SendChar2, IDC_SendChar3, IDC_SendChar4, IDC_SendChar5, IDC_SendChar6, IDC_SendChar7 };
    int m_idc_ch_n[4] { IDC_SendCharNeg0, IDC_SendCharNeg1, IDC_SendCharNeg2, IDC_SendCharNeg3 };  
    int m_idc_th[4] { IDC_SendCharTh0, IDC_SendCharTh1, IDC_SendCharTh2, IDC_SendCharTh3 };    
    int m_idc_SZ[8] = { IDC_SZ0, IDC_SZ1, IDC_SZ2, IDC_SZ3, IDC_SZ4, IDC_SZ5, IDC_SZ6, IDC_SZ7 };
    int m_idc_SZ_n[4] = { IDC_SZ8, IDC_SZ9, IDC_SZ10, IDC_SZ11 };
    int m_idc_static[4] = { IDC_STATIC1, IDC_STATIC2, IDC_STATIC3, IDC_STATIC4 };
    afx_msg void OnEnChangePacketsendperiod();
    BOOL m_bShowChecked{ FALSE };
};
