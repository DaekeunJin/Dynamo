#pragma once

#include "UserSetting.h"

#define N_ITEM_COLOR 3
#define TIMER_AUTOSAVE_TIMEOUT	1000
#define TIMER_AUTOSAVE_LOGCNT	1001

// CSettingDlg 대화 상자
class CSerialComDlg;
class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSettingDlg();
	CSerialComDlg* pMain;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Setting };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveclose();
	virtual BOOL OnInitDialog();
	BYTE m_BT_Command[MAX_BT_COMMAND];
	CString m_strCommand[MAX_BT_COMMAND];
	void UpdateControl(void);

	int m_iPenColor[MAXIMUM_DATA_SIZE][3];
	BOOL m_bReqRedraw;
	CString m_strPenColor[MAXIMUM_DATA_SIZE][3];
	BOOL m_bUseDataMark[MAXIMUM_DATA_SIZE];
	afx_msg void OnBnClickedUpdatecolor();
	void ReqUpdateColor();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedLoaddefaultcolor();
	int m_iTimeDisplayMode;
	CString m_strFontSize[MAX_FONT_SIZE];
	int m_iFontSize[MAX_FONT_SIZE];
	afx_msg void OnEnChangeFontsizesetting();
	CListCtrl m_ModelList;
	void AddColumn(void);
	void InsertItem(const CString& strModelName, const CString& strDescription);
	afx_msg void OnCbnSelchangeCopymodel();

	int m_iCopyModel;
	CComboBox	m_cCopyModel;
	CString m_strModelName;
	afx_msg void OnBnClickedAddmodel();
	CString m_strDescription;
	afx_msg void OnBnClickedRemovemodel();
	afx_msg void OnClickModellist(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iSelectList;
	afx_msg void OnEnChangeAddmodelname();
	afx_msg void OnEnChangeDescription();
	afx_msg void OnBnClickedCopysetting();
	afx_msg void Wait(DWORD dwMillisecond);
	afx_msg void OnBnClickedCancel();
	BOOL m_bFixGraphFrame;
	BOOL m_bUsePositionGraph;
	CString m_strPositonGraphHeader;
	int m_iSelectPositionGraphX;
	int m_iSelectPositionGraphY;
	char m_cGraphPositionHeader;
	int m_iPenType[MAXIMUM_DATA_SIZE];
	CString m_strPenThickness[MAXIMUM_DATA_SIZE];
	int m_iPenThickness[MAXIMUM_DATA_SIZE];
	afx_msg void OnBnClickedChangepenstyle0();
	afx_msg void OnBnClickedChangepenstyle1();
	afx_msg void OnBnClickedChangepenstyle2();
	afx_msg void OnBnClickedChangepenstyle3();
	afx_msg void OnBnClickedChangepenstyle4();
	afx_msg void OnBnClickedChangepenstyle5();
	afx_msg void OnBnClickedChangepenstyle6();
	afx_msg void OnBnClickedChangepenstyle7();
	afx_msg void OnBnClickedChangepenstyle8();
	afx_msg void OnBnClickedChangepenstyle9();
	afx_msg void OnBnClickedChangepenstyle10();
	afx_msg void OnBnClickedChangepenstyle11();
	afx_msg void OnBnClickedChangepenstyle12();
	afx_msg void OnBnClickedChangepenstyle13();
	afx_msg void OnBnClickedChangepenstyle14();
	afx_msg void OnBnClickedChangepenstyle15();

	void ChangePenstyle(int iChannel);
	int m_iGridCountY;
	afx_msg void OnCbnSelchangeGridcount();
	int m_iMarkSize;

	CString m_strCoff[MAXIMUM_DATA_SIZE];
	CString m_strMinDataSize;
	CString m_strMaxDataSize;
	afx_msg void OnEnChangeMindatasize();
	int m_iTextColorMode;
	int m_iBkColorMode;
	CString m_strSelTextColor[3];
	CString m_strSelBkColor[3];
	BOOL m_bGetSettingInLoading;
	afx_msg void OnBnClickedGetsettinginloading();
	afx_msg void OnBnClickedReqheaderlog();
	afx_msg void OnEnChangeMaxdatasize();
	afx_msg void OnBnClickedChangeselectgraph0();
	afx_msg void OnBnClickedChangeselectgraph1();

	int m_iRGB_SelectControl_Select[3][2];
	int m_iRGB_SelectControl_Outline[3][2];
	int m_iRGB_SelectControl_Blank[3][2];
	int m_iDefaultRGB_SelectControl_Select[3][2];
	int m_iDefaultRGB_SelectControl_Outline[3][2];
	int m_iDefaultRGB_SelectControl_Blank[3][2];

	CBrush m_BrushSelectControl_Blank[2];
	CBrush m_BrushSelectControl_Select[2];
	CPen m_PenSelectControl_Blank[2];
	CPen m_PenSelectControl_Select[2];
	int m_iSaveMode;
	afx_msg void ChangeDataSaveMode();
	afx_msg void OnBnClickedSetpath();
	int m_iGridX_Type;
	int m_iGridX;
	afx_msg void OnEnChangengridx();
	int m_iColorItem;
	afx_msg void OnBnClickedChangeselectitemcolor();
	BOOL m_bChangedColorItem[N_ITEM_COLOR]{ FALSE, FALSE, FALSE };
	int m_iChangeColor[N_ITEM_COLOR][3];
	afx_msg void OnBnClickedUseintegerdata();
	afx_msg void OnBnClickedTextcolormode0();
	afx_msg void OnBnClickedBkcolormode2();
	afx_msg void OnBnClickedSavereg();
	afx_msg void OnBnClickedInitreg();
	BOOL m_bConfirmInit{ FALSE };
	afx_msg void OnEnChangeMaxdatadigit();
	afx_msg void OnBnClickedReqheaderlog2();
	afx_msg void OnBnClickedNavilogconvert();
	afx_msg void OnBnClickedUseXinput();
	afx_msg void OnBnClickedXinputsend();
    int m_iCurrentDataPage { 0 };
	
    void ShowDataPage();
    afx_msg void OnBnClickedPageup();
    afx_msg void OnBnClickedPagedn();
	afx_msg void OnBnClickedSetdatapage();
	afx_msg void OnBnClickedSetmathpage();
	afx_msg void OnBnClickedSettestdatasize();
	afx_msg void OnEnChangeTestdatasize();
	void CheckTestDataSize();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangePositiongraphselx();
	int UpdatePositionItem(CString* pstr);

	int m_iDataPage;
	int m_iMathPage;
    int m_iHistoryCapacity;
	void UpdateXY(int dir);

	afx_msg void OnEnChangePositiongraphsely();
	
	afx_msg void OnBnClickedConvertreg();
	afx_msg void OnBnClickedConvertreg2();
	void UpdateDataPage();
	afx_msg void ChangeDataPage();
    afx_msg void OnEnChangeChangeheaderRxcount();
    afx_msg void OnEnChangeMaxdatasize2();
    afx_msg void OnEnChangeMindatasize2();
	afx_msg void OnEnChangeAutosavetimeout();
	afx_msg void OnEnChangeAutosavelogcount();

	void UpdateAutosaveTimeout();
	void UpdateAutosaveLogCount();
    afx_msg void OnEnChangeLogupdatecount();
	afx_msg void OnEnChangeLogupdatecount2();
};
