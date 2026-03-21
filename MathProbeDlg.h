#pragma once
#include "UserSetting.h"

class CSerialComDlg;
class CColorSelectDlg;
class CSerialComApp;
// CMathProbeDlg 대화 상자

class CMathProbeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMathProbeDlg)

public:
	CMathProbeDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMathProbeDlg();
	CSerialComDlg *pMain;
	CSerialComApp* pAppMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MathProbeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_ctrlData1[MAX_MATHCONTROL_SIZE];
	CComboBox m_ctrlData2[MAX_MATHCONTROL_SIZE];
	CComboBox m_ctrlOperator[MAX_MATHCONTROL_SIZE];
	CEdit m_ctrlDataSize[MAX_MATHCONTROL_SIZE];
	CString m_strDataSize[MAX_MATHCONTROL_SIZE];

	CString m_strMessage;

	void LoadMemoryData();
	void UpdateMemoryData();
	void UpdateControl();
	virtual BOOL OnInitDialog();
	void DrawLine();
	afx_msg void OnPaint();	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEnChangenDatasize();
	afx_msg void OnBnClickedReloadvalue();
	BOOL m_bUseMathProbe[MAX_MATHCONTROL_SIZE];
	afx_msg void OnBnClickedDefaultsetting();
	int  m_iDefaultSize[MAX_OPERATOR_COUNT];
	int m_IDC_PinName[MAX_MATHCONTROL_SIZE];
	int m_IDC_UsePinName[MAX_MATHCONTROL_SIZE];
    int m_IDC_ResInt[MAX_MATHCONTROL_SIZE];
	afx_msg void OnBnClickedDefaultbutton();
	afx_msg void OnBnClickedSelectcolormath0();
	afx_msg void OnBnClickedSelectcolormath1();
	afx_msg void OnBnClickedSelectcolormath2();
	afx_msg void OnBnClickedSelectcolormath3();
	afx_msg void OnBnClickedSelectcolormath4();
	afx_msg void OnBnClickedSelectcolormath5();
	afx_msg void OnBnClickedSelectcolormath6();
	afx_msg void OnBnClickedSelectcolormath7();
	afx_msg void OnBnClickedSelectcolormath8();
	afx_msg void OnBnClickedSelectcolormath9();
	afx_msg void OnBnClickedSelectcolormath10();
	afx_msg void OnBnClickedSelectcolormath11();
	afx_msg void OnBnClickedSelectcolormath12();
	afx_msg void OnBnClickedSelectcolormath13();
	afx_msg void OnBnClickedSelectcolormath14();
	afx_msg void OnBnClickedSelectcolormath15();
	void SelectColor(int iChannel);
    int m_iMathPage{ 0 };
    MathData_t m_MathData[MAXIMUM_MATH_SIZE];

	void CopyMathInfo(const MathData_t& From, MathData_t* To);
	void LoadControlData(void);

	BOOL m_bUseMathProbeName[MAX_MATHCONTROL_SIZE];
	CString m_strMathProbeName[MAX_MATHCONTROL_SIZE];

	void UpdateCurrentConfig(int iMathPage);
    afx_msg void ChangeMathConfigPage(int pre_page, int post_page);
    afx_msg void OnBnClickedPageup();
    afx_msg void OnBnClickedPagedn();
	afx_msg void OnBnClickedMathdefault();
};
