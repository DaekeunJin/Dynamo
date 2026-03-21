// SerialComDlg.h : header file
#include "SettingDlg.h"
#include "UserSetting.h"
#include "StatisticsDlg.h"
#include "MathProbeDlg.h"
#include "PortListDlg.h"
#include "UserScreenDlg.h"
#include "TextLogDlg.h"
#include "CControlDlg.h"
#include "Xinput.h"
#include <vector>

#if !defined(AFX_SERIALCOMDLG_H__4FB3E547_C3BA_4697_81B7_AB1991BEA61A__INCLUDED_)
#define AFX_SERIALCOMDLG_H__4FB3E547_C3BA_4697_81B7_AB1991BEA61A__INCLUDED_
#include "CommThread.h"	// Added by ClassView
#include "afxwin.h"
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// CSerialComDlg dialog
class CSerialComApp;
class CControlDlg;


class CSerialComDlg : public CDialog
{
	// Construction
public:
	CSerialComDlg(CWnd* pParent = NULL);	// standard constructor	
	CSerialComApp* pAppMain;
	HICON m_hIcon;

	// Dialog Data
		//{{AFX_DATA(CSerialComDlg)
	enum { IDD = IDD_SERIALCOM_DIALOG };

	CComboBox	m_cSerialPort, m_cSerialPort2, m_cSerialPort3;
	CComboBox	m_cBaudRate, m_cBaudRate2, m_cBaudRate3;
	CComboBox	m_cSelectModel;
	CString RemoveSerialInfo(CString str);

	//port1 - for micom
	int		m_iStopBit;
	int		m_iSerialPort, m_iSerialPort2, m_iSerialPort3;
	int		m_iParity;
	int		m_iDataBit;
	int		m_iBaudRate, m_iBaudRate2, m_iBaudRate3;
	int		m_iSelectModel;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialComDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CSerialComDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg long OnCommunication(WPARAM wParam, LPARAM lParam);
    afx_msg long OnCommunication2(WPARAM wParam, LPARAM lParam);
	afx_msg long OnCommunication3(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeBaudRate();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    BYTE byCode2AsciiValue(char cData);// 문자를 hex값으로 변경 0~F 까지는 그대로 그 외에 글자는 0으로 
    DWORD byIndexBaud(int xBaud);// 전송률 받기
    BYTE byIndexData(int xData);//데이터 비트 받기
    BYTE byIndexStop(int xStop);// 스톱비트 받기 
    BYTE byIndexParity(int xParity);// 펠리티 받기
    CString byNameComPort(const CString& str); // COM숫자로 포트이름 받기

public:
	afx_msg void Wait(DWORD dwMillisecond);
	afx_msg void OnPortOpen1();
	afx_msg void OnPortClose1();
	afx_msg void OnSelchangeSerialPort();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSetfocusSerialPort();
    afx_msg void OnSetfocusSerialPort2();
	afx_msg void OnSetfocusSerialPort3();

    CCommThread m_ComuPort, m_ComuPort2, m_ComuPort3;			//port1 for micom
    int String2Num(const CString& str);

	CString m_strSerial;
	CString m_strSerial2;
	CString m_strSerial3;

	void LoadComPortList();
	void SaveComPortList();
	CString m_strPacketHeader;
	BYTE m_cPacketHead = 0;
	afx_msg void OnEnChangePackethead();

	int m_iControlGraphCount;
	int	m_iSelectGraphCount;
	BOOL m_bDrawGraph[MAX_GRAPH_COUNT];
	BOOL m_bSelectDraw_GraphData[MAX_GRAPH_COUNT][MAXIMUM_DATA_SIZE];
	BOOL m_bDraw_GraphData[MAX_GRAPH_COUNT][MAXIMUM_DATA_SIZE];
	int m_IDC_DataNumber[MAX_DATACONTROL_SIZE];
	int m_IDC_PinName[MAX_DATACONTROL_SIZE];
	int m_IDC_DataValue[MAX_DATACONTROL_SIZE];

	int m_IDC_UseFilter[MAX_FILTER];
	int m_IDC_FilterName[MAX_FILTER];
	int m_IDC_FilterValue[MAX_FILTER];

	int m_IDC_MathText[MAX_MATHCONTROL_SIZE];
	int m_IDC_StaticMath[MAX_MATHCONTROL_SIZE];
	int m_IDC_MathProbeName[MAX_MATHCONTROL_SIZE];
	int m_IDC_MathProbeValue[MAX_MATHCONTROL_SIZE];
	int m_IDC_DrawGraph[MAX_GRAPH_COUNT];
	int m_IDC_MaxY_Graph[MAX_GRAPH_COUNT];
	int m_IDC_MinY_Graph[MAX_GRAPH_COUNT];
	int m_IDC_MidY_Graph[MAX_GRAPH_COUNT];
	int m_IDC_AutoRange[MAX_GRAPH_COUNT];

	int m_iDefaultMathColor[MAXIMUM_MATH_SIZE][3];		// Default Color
	int m_iDefaultDataColor[MAXIMUM_DATA_SIZE][3];	

	afx_msg void UpdateControl();
	afx_msg void ChangeFilterItem();
	afx_msg void ChangeFilterUse();     
	afx_msg void ChangeDataName();

	afx_msg void OnBnClickedSend();
	afx_msg void UpdateDrawGraphControl();
    void ShowGraphControlbyHeight();

	std::vector <UINT> m_iFilterHistoryIndexTable;
	std::vector <UINT> m_iLogHistoryIndexTable;
	std::vector <UINT> m_iFilterLogIndexTable;
	std::vector <UINT> m_iLogFilterIndexTable;

	int m_iHistoryIndex;
	int m_iFilterIndex;
	int m_iLogIndex;

	void UpdateGraphData(void);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	int m_iDrawGraphRangeX[2];
	int m_iDrawGraphSpanX;
	int m_iDrawGraphMidX;
	int m_iPreDrawGraphRangeX;
	int m_iDrawGraphRangeY[MAX_GRAPH_COUNT][2];

	void DrawInit(void);
	void DrawInit(int iGraph);

	typedef struct {						
        int iDrawGraphID;
		int iDraw2RealGraphID;
        int iHeight;
        double fRangeMax;
        double fRangeMin;
        BOOL bAutoRange_Graph;
	}GraphAdjust_T;

	GraphAdjust_T m_RealGrpahInfo[MAX_GRAPH_COUNT];

	CString m_strRangeMax[MAX_GRAPH_COUNT];
	CString m_strRangeMin[MAX_GRAPH_COUNT];
	double m_fRangeMax[MAX_GRAPH_COUNT];
	double m_fRangeMin[MAX_GRAPH_COUNT];
	CString m_strRangeMid[MAX_GRAPH_COUNT];
	BOOL m_bAutoRange_Graph[MAX_GRAPH_COUNT];

	CBrush m_BrushBackGround[2];
	CBrush m_BrushAdjust[2];
	CBrush m_Brush[20];
	CBrush m_BrushPosition[2];
	CBrush m_BrushSelectControl_Blank[2];
	CBrush m_BrushSelectControl_Select[2];
	CPen   m_PenSelectControl_Blank[2];
	CPen   m_PenSelectControl_Select[2];
	CPen   m_PenWhite;
	CPen   m_PenBlack;
	CPen   m_PenSelect;
	CPen   m_PenCursor, m_PenCursor1;
	CPen   m_PenGrid;
	CPen   m_PenData[MAXIMUM_DATA_SIZE];
	CPen   m_Pen_Math[MAXIMUM_MATH_SIZE];
	CPen   m_PenPosition[2];
	CPen   m_PenAdjust;

	int m_iRGB_SelectControl_Select[3][2];
	int m_iRGB_SelectControl_Outline[3][2];
	int m_iRGB_SelectControl_Blank[3][2];
	int m_iDefaultRGB_SelectControl_Select[3][2];
	int m_iDefaultRGB_SelectControl_Outline[3][2];
	int m_iDefaultRGB_SelectControl_Blank[3][2];

	int m_iPenType[MAXIMUM_DATA_SIZE];
	int m_iPenThickness[MAXIMUM_DATA_SIZE];
	int m_iPenColor[MAXIMUM_DATA_SIZE][3];
	BOOL m_bUseDataMark[MAXIMUM_DATA_SIZE];

	CPoint CalculateDrawPoint(int iGraph, int iIndex, double Value);
	int CalculateDrawPointX(int iIndex);
	CPoint CalculateDrawPointY(int iGraph, double Value);
	int CalculateIndex(int iPtx);

	afx_msg void OnChangeRange();
	int m_iProgramMode;	// 1: Data 수집 모드   // 0 : 분석 모드
	BOOL m_bUpdateList;
	BOOL m_bReqGraphUpdate;
	afx_msg void OnBnClickedDefaultbutton();
	bool m_bReqChangeMode{ false };
	bool m_bRelesedChangeMode{ true };

	BOOL m_bSaveFile;

	void SaveLog(CString AddMsg, int iCommand);
	CFile SaveFile;
	void SetReg_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData);
	CString GetReg_RegistryData(const CString& strAddress, const CString& strItem);

	void SetRegRoot_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData);
	CString GetRegRoot_RegistryData(const CString& strAddress, const CString& strItem);

	afx_msg void OnBnClickedSaveconfiguration();
	void LoadCommonConfiguration();
	void LoadHeaderConfiguration();
	void LoadDefaultGraphNumber();
	BOOL m_bUseFilterItem[MAX_FILTER];
	CString m_strFilterItem[MAX_FILTER];
	int m_iFilterLength[MAX_FILTER];

	double GetValue(CString str);
	afx_msg void OnBnClickedUsercommandsetting();

	BYTE m_BT_Command[MAX_BT_COMMAND];
	void CSerialComDlg::ClickedUsercommand(int index);
	afx_msg void OnBnClickedUsercommand0();
	afx_msg void OnBnClickedUsercommand1();
	afx_msg void OnBnClickedUsercommand2();
	afx_msg void OnBnClickedUsercommand3();
	afx_msg void OnBnClickedUsercommand4();
	afx_msg void OnBnClickedUsercommand5();
	afx_msg void OnBnClickedUsercommand6();
	afx_msg void OnBnClickedUsercommand7();
	afx_msg void OnBnClickedUsercommand8();
	afx_msg void OnBnClickedUsercommand9();
	afx_msg void OnBnClickedUsercommand10();
	afx_msg void OnBnClickedUsercommand11();
	afx_msg void OnBnClickedUsercommand12();
	afx_msg void OnBnClickedUsercommand13();
	afx_msg void OnBnClickedUsercommand14();
	afx_msg void OnBnClickedUsercommand15();
	afx_msg void OnBnClickedUsercommand16();
	afx_msg void OnBnClickedUsercommand17();
	afx_msg void OnBnClickedUsercommand18();
	afx_msg void OnBnClickedUsercommand19();

	int m_iDisplaySize;
	afx_msg void OnEnChangeDisplaydatasize();
	BOOL m_bForcedChangeDataSize{ FALSE };
	void OnBnClickedClearmessage();

	BOOL m_bDrawSelectData;
	int m_iHistoryMove;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	int m_iHistorySelectIndex;
	int m_iHistorySelectPt;
	afx_msg void OnBnClickedClearwatch();

	void ChangeHeader(void);
	int m_iTimeDisplaySize;
	void LoadDataFromFile(CString strFileName);
	void OnBnClickedClearwatchlist();
	void ReqResetData(BOOL bReqCheck);
	afx_msg void OnBnClickedReqresetdata();
	afx_msg void OnBnClickedIndexup1();
	CString m_strIndexNumber{ "0" };
	afx_msg void OnBnClickedIndexdn1();
	int m_iPresetMode;
	afx_msg void OnBnClickedPresetmode();
	CString m_strPinName[MAXIMUM_DATA_SIZE];
	CString m_strMathPinName[MAXIMUM_MATH_SIZE];
	afx_msg void OnBnClickedReqsavedata();
	void ReqSaveData(BOOL bReqCheck);
	void ReqSaveData(BOOL bReqCheck, int iFrom, int iTo);
	void OnBnClickedExportconfiguration();
	afx_msg void OnDropFiles(HDROP hDropInfo);	
	afx_msg void OnLbnSelchangeFiltermessage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ProgramModeUpdate(void);
	void CheckDataBufferFull(void);
	afx_msg void OnEnChangeIndexnumber();
	void ChangeIndexNumber(int iIndex);
	afx_msg void OnBnClickedClearfilterlist();
	afx_msg void OnBnClickedLoadwatchlist();
	int m_iCurrentIndex{ 0 };

	int  Hex2Value(const CString&);
	int String2Value(const CString& str);	
	
	afx_msg void UpdateUseAutoSave();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString GetLastComPort(void);
	CString GetLastComPort2(void);
	CString GetLastComPort3(void);

	void SetLastComPort(const CString& strComPort);
	void SetLastComPort2(const CString& strComPort);
	void SetLastComPort3(const CString& strComPort);

	void SetLastBaudRate(int iBaud);
	void GetLastBaudRate(void);
	void SetLastBaudRate2(int iBaud);
	void GetLastBaudRate2(void);
	void SetLastBaudRate3(int iBaud);
	void GetLastBaudRate3(void);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetFrameSize();
    void SetGraphHeight();

	int m_iGraphInfoMinX, m_iGraphInfoMaxX;
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	BOOL m_bReqRedrawGraph;
	int m_iLegendX;
	int m_iLegendX_PositionGraph;
	BOOL m_bIsAdjustMode;
	BOOL m_bAdjustLegendX, m_bAdjustLegendY[MAX_GRAPH_COUNT - 1];
	int m_iAdjustLegendY{ 0 };
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int m_iWndSizeX, m_iWndSizeY;

	BOOL m_bSelectListBox;
	int m_iSelectListBox;
	BOOL m_bDrawChannel[2];

	int m_iGraphCount;
	afx_msg void OnCbnSelchangeGraphcount();
	void DisplayControl(void);
	CString m_strDataSetting, m_strDataSetting2;
	void OptimizeGraphCount();

	int m_iTimeDisplayMode;  // 0  hh:mm:ss     /  1   mm:ss.ms     /  2   ss.ms 
	void ShowVersionHistory();
	afx_msg void OnBnClickedShowhistory();
	CProgressCtrl m_ctrlProgReading;

	CFont m_FontControl[MAX_FONT_SIZE];
	CFont m_FontLabel, m_FontDefault, m_FontBold;
	int m_iControlFontSize[MAX_FONT_SIZE];
	void ChangeControlFontSize(int iItem, int iSize);
	int m_iGridX;
	int m_iGridX_Type{ 0 };

	void GetLastModel(void);
	void SetLastModel(void);

	afx_msg void OnCbnSelchangeModel();

	// Double Buffer설정
	CDC m_MemDC, m_MemDC_Range, m_MemDC_Position, m_MemDC_Position_Init, m_MemDC_Timeline, m_MemDC_Select;
	CBitmap m_Bitmap, m_Bitmap_Range, m_Bitmap_Position, m_Bitmap_Position_Init, m_Bitmap_Timeline, m_Bitmap_Select;
	CBitmap *m_pOldBitmap, *m_pOldBitmap_Range, *m_pOldBitmap_Position, *m_pOldBitmap_Position_Init, *m_pOldBitmap_Timeline, *m_pOldBitmap_Select;

	afx_msg void OnBnClickedLoadcalculation();
    CStatisticsDlg* m_pStatistics;
	BOOL m_bUseStatistics;
	int m_iAdjustSelectMode;
	int m_iAdjustSelectPoint[2];

	CPen m_PenMark[2];
	CBrush m_BrushMark[2];
	afx_msg void OnCancel();

    MathData_t m_MathData[MAXIMUM_MATH_SIZE];
	afx_msg void OnBnClickedDefinemathprobe();
	void LoadDefaultMathProbeSetting();
	void CheckMathProbeSetting();
	void CheckMathProbeValid(void);

	void InitMathData(void);
	void ReCalculationMathData();

	BOOL m_bFixGraphFrame;

	BOOL m_bDisplayPosition;

	int m_iActuralWindowSize[2];
	int m_iPositionGraphRangeX[2];
	int m_iPositionGraphRangeY[2];
	CString m_strPositonGraphHeader;
	BOOL m_bUsePositionGraph;
	BOOL m_bAvailablePositionGraph;
	int m_iSelectPositionGraphX;
	int m_iSelectPositionGraphY;
	int m_iDev;

	// Position Graph관련
	void CalculationScale(void);
	int m_iStarGridValueX_PositionGraph, m_iStarGridValueY_PositionGraph;

	double m_fMinValueX_PositionGraph, m_fMaxValueX_PositionGraph;
	double m_fMinValueY_PositionGraph, m_fMaxValueY_PositionGraph;
	double m_fMidValueX_PositionGraph, m_fMidValueY_PositionGraph;
	double m_fValueScale_PositionGraph;
	int m_iFrameSizeGraphPosition;

	int m_iGridSpan_PositionGraph;

	struct MoveOrigin {
		CPoint P_Start;
		CPoint P_Last;
		BOOL isPressed;
	};

	MoveOrigin m_Panning_PositionGraph;
	MoveOrigin m_FrameAdjusting_PositionGraph;
	MoveOrigin m_Measure_PositionGraph;
	MoveOrigin m_Resize_PositionGraph;
	MoveOrigin m_AdjustLogFilter_V;
	MoveOrigin m_AdjustLogFilter_H;
	MoveOrigin m_AdjustDataWnd_V;
	MoveOrigin m_AdjustMathWnd_V;


	CPen m_PenOrigin_PositionGraph, m_PenDash_PositionGraph;
	CPoint CalculationDrawPosition_PositionGraph(double ValueX, double ValueY);
	BOOL m_bRetrunSubProgram;
	BOOL m_bReqAutoScalePositionDraw;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);

	struct Measure3Point {
		CPoint Pt[4];
		int Index;
		BOOL bIsMeasuring;
		BOOL bIsClick;
	};

	Measure3Point m_Measure3P;
	CBrush m_BrushNull, *m_pBrushNull;
	void ClearMeasure3P(void);
	void UpdateMathDefine();

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	BOOL m_bUseAutoScaleGraphPosition;
	CString m_strMathProbeName[MAXIMUM_MATH_SIZE];
	BOOL  m_bShowMathControl;
	int	m_iFilterViewMode;
    BOOL m_bShowFilterLogControl{FALSE};
	LOGBRUSH m_LogBrush;
	int m_iRatioLogFilter;
	int m_iRangeLogFilterRatio[2];
	int m_iRangeLogFilterWidth;
	int m_iRangeLogFilterVertical;
	void SetLogFilterSize_V(CPoint point);
	void SetLogFilterSize_H(CPoint point);
	void SetDataWndSize_V(CPoint point);
	void SetMathWndSize_V(CPoint point);
	void ControlView();

	afx_msg void OnBnClickedSerialinfo();
	int m_iGridCountY;
	void ShowSelectValueDrawInit(int iSelectIndex);
	int m_iMarkSize;
	int CopyTextToClipboard(const char *ap_string);
	int m_bUseIntData{ TRUE };
	double m_fCoff[MAXIMUM_DATA_SIZE];
	int m_iMinPacketDataSize;
	int m_iMaxPacketDataSize;
	void DrawTimeline(void);
	int CalTimelinePoint(int iIndex);
	int CalIndexChangeIndexTimeline(int iShiftPoint);
	BOOL m_bTimelineControl[3];  // 0: Move     1: Left     2:Right
	int m_iTimeLineX[2];
	int m_iTimelineStartPoint;
	BOOL m_bTimelineFullRangeView;
	BOOL m_iPreDisplaySize;
	BOOL m_bShowDataControl{ TRUE };
	BOOL m_bUseAutoSave;
	afx_msg void OnBnClickedGame();
	BOOL m_bUseAutoRange;
	afx_msg void OnBnClickedRange();

	CString m_strTemp;
	afx_msg void OnBnClickedClearCursor();
	void DrawSelectDataLabel(int iSelectIndex, int iStarIndex);
	BOOL m_bShowLabelValue;
	afx_msg void OnBnClickedShowselectvalue();

	int m_iSelTextColorMode;
	int m_iSelBkColorMode;
	int m_iSelTextColor[3];
	int m_iSelBkColor[3];

	void DrawCursorInfo();

	class CursorInfo {
	public:
		int nCursor, iSelectCursor, iPos[MAX_CURSOR];

		CursorInfo() { Clear(); }
		void Clear() { nCursor = 0; iSelectCursor = -1; }
		void AddCursor(int Pt) {
			if (nCursor < MAX_CURSOR - 1)
				iPos[nCursor++] = Pt;
		}
		void RemoveCursor(int iCursor) {
			if (iCursor < MAX_CURSOR) {
				for (int i = iCursor; i < MAX_CURSOR - 1; i++) 	iPos[i] = iPos[i + 1];
				nCursor = max(nCursor - 1, 0);
			}
		}
	};

	CursorInfo m_Cursor;
	afx_msg void OnBnClickedAddcursor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	BOOL m_bGetSettingInLoading;
	CString Dbl2Str(double fValue);

	BOOL m_bUseAutoHeader_rx;
	BOOL m_bUseAutoHeader_rx_reg;
    int m_iAutoHeader_rx_count{ 5 };
    BOOL m_bUseAutoHeader_tx;
	afx_msg void OnBnClickedComportlist();

	ComPortList_t m_ComPortList[MAX_COM_PORT_SIZE];
	int m_iComPortCount;

	int m_iConnectedPortCount;
	int m_iConnectComPort[MAX_COM_PORT_SIZE];
	afx_msg void OnBnClickedCapture();

	int GetTextLogCount(char header);
	void GetHeaderLogFromRegistry(char header, int num, CTextLogSetting* data);
	void LoadDefaultHeaderLog();
	void GetHeaderLogFromBuffer(BYTE* pBuffer, BOOL* pConti);
	int GetNextValue(BYTE* pBuffer, CString* pStrValue);
	int m_iBufferIndex, m_iBufferSize;
	void ShowDataMathControl(void);
	int m_iShowDataControl;
	int m_iShowMathControl{ 0 };
	int m_iRangeDataControlVertical;
	void ShowFilterControl();
	BOOL m_bRefreshScreen;

	HCURSOR mh_Cursor[4];
	int m_CursorMode;
	void SetCursorType(CPoint point);
	BOOL m_bShowResizeCursor;

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void GetSaveLog(BYTE *, UINT);
	afx_msg void OnBnClickedRefdata();
	RefData_t m_RefData;
	int m_iSaveMode;
	CString m_strSavePath[2];
	void GetSavePath();
	afx_msg void OnBnClickedReqcontroldlg();

	int m_iBackgroundColor[2][3];
	int m_iDefault_BackgroundColor[2][3]{ {255,255,255}, {240, 255, 240}, };
	int m_iTextColor[3];
	int m_iDefault_TextColor[3]{ 0, 0, 0 };
	CPoint m_PtMoveStart;
	BOOL m_bIsMoveMode{ FALSE };
	void SetMoveMouse(const CPoint& pt);
	int m_iCnt{ 0 };
	COLORREF InvColor(int r, int g, int b);

	CIndicator m_Indicator[N_MAX_INDICATOR];
	int m_iIndicator{ 0 };
	void ArrangeIndicator(int iGraph);
	void DrawIndicator(int iGraph);
	void ChangeSelectPoint(CPoint pt);

	BOOL m_bShowProbeDataID{ FALSE };
	BOOL m_bShowProbeValue{ FALSE };
	BOOL m_bShowTimelineN{ FALSE };

	BOOL CheckControlKeyDown();
	BOOL CheckShiftKeyDown();

	afx_msg void OnBnClickedReload();
	BOOL Reload_Log(int index, BOOL *arrUseFilter);
	void Req_Draw_ScreenFit();
	void ReqRegInit();
	void DeleteRegistryKey(HKEY hKey, CString str);
	BOOL m_bReqCloseProgram{ FALSE };

	void ExportRegFile();
	void ExportRegFile_after_Conversion_from_Old(int mode);
	void ExportRegistryKey_conversion(HKEY hKey, CString subkey, CFile &outFile, int mode = 0);
	void ExportRegistryKey(HKEY hKey, CString subkey, CFile &outFile);
	void Save_reg_version(CFile &outFile);
	
	class CConversion_Item {
	public:
		BOOL use[2];
		CString str[2];		
	};
	void Conversion_Reg(CString& str, int mode);
	CConversion_Item m_ConverItem[N_CONVERT_REG];

	afx_msg void CSerialComDlg::OnBnClickedFittomaxdata();
	BOOL m_bFitToScreen{ FALSE };
	BOOL m_bManualAdjust{ FALSE };
	int m_iMaxDataDigit{ 10 };

	void CopyMathProbeMode(BYTE cSourceHead, int iSourceMode);
	void CopyMathProbeMode(BYTE cSourceHead, int iSourceMode, BYTE cTargetHead, int iTargetMode0, int iTargetMode1);
	void CopyMathGraphSetting(BYTE cSourceHead, int iSourceMode);
	void CopyMathGraphSetting(BYTE cSourceHead, int iSourceMode, BYTE cTargetHead, int iTargetMode0, int iTargetMode1);

	void CopyDataProbeMode(BYTE cSourceHead);
	void CopyDataProbeMode(BYTE cSourceHead, BYTE cTargetHead);
	void CopyDataGraphSetting(BYTE cSourceHead, int iSourceMode);
	void CopyDataGraphSetting(BYTE cSourceHead, int iSourceMode, BYTE cTargetHead, int iTargetMode0, int iTargetMode1);

	afx_msg void OnBnClickedDisplay0();
	afx_msg void OnBnClickedDisplay1();
	afx_msg void OnBnClickedDisplay3();
	afx_msg void OnBnClickedDisplay2();
	afx_msg void OnCbnSelchangeMode();
	CComboBox m_ctrlMode;

	void UpdateControlPosition();
	int m_iPos_DataStart, m_iPos_DataEnd, m_iPos_MathStart, m_iPos_MathEnd, m_iPos_FilterStart, m_iPos_FilterEnd;
	afx_msg void OnBnClickedModecopy();

	int m_iDefalultPenColor[MAXIMUM_DATA_SIZE][3]{
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
	};
	afx_msg void OnEnChangeMaxfitscreen();
	int m_iMaxFitScreen;

    CControlDlg* m_pCmdControl{ nullptr }; 
    CUserScreenDlg* m_pUserScreen{ nullptr };
	CTextLogDlg* m_pTextScreen{ nullptr };    

	CGraphLogSetting m_GraphLog[N_MAX_GRAPHLOG_ITEM];
	CString m_GraphLogMemo[N_MAX_GRAPHLOG_ITEM]{ "" };
	CString m_GraphPreText[N_MAX_GRAPHLOG_ITEM]{ "" };
	CString m_GraphPostText[N_MAX_GRAPHLOG_ITEM]{ "" };
	int m_iGraphLogCount{ 0 };
	void SaveGraphLogData(BYTE cHeader);

	CTextLogSetting m_TextLog[MAX_HEADER_LOG_COUNT];
	int m_iTextLogCount{ 0 };

	void GetGraphLogData();
	void GetTextLogData();
	CString m_strGraphLog_Jpg;
	CGraphLogSetting m_DataConversion;

	BOOL m_bShowUS{ FALSE };
	BOOL m_bShowTS{ FALSE };
	void UpdateGraphLogData(BOOL init);
	void UpdateTextLogData(BOOL init);
	afx_msg void OnBnClickedShowgraphlog();
	afx_msg void OnBnClickedShowtextlog();
	BOOL m_LoadSettingInfo_in_DataLoading{ TRUE };
	BYTE m_cGraphLogHeader{ ' ' };
	int m_iWheelCount{ 0 };
	int m_iWheelAcc{ 10 };
	void CheckWheelAcc();
	afx_msg void OnBnClickedGraphupdate();
	BOOL m_bUseGraphUpdate{ TRUE };
	void GenerateMathDataFromRcvData();
	void Get_XinputData();
	CString m_strXinput;
	BYTE* Make_XInput_UartData_Data(const XINPUT_GAMEPAD&, int*);
	void Send_UartData_Xinput(const XINPUT_GAMEPAD&);
	char* XInput_SendData_hex(BYTE *, int);	
	BOOL Check_XinputData(XINPUT_GAMEPAD& Data, CString& str);	
	BOOL NoiseRemove_Xinput_Byte(BYTE& Data, CString& str, const char*);
	BOOL NoiseRemove_Xinput_Short(short& Data, CString& s, const char*);

	BOOL m_bConnect_Xinput;
	afx_msg void OnBnClickedTest();
	BOOL m_bUse_Xinput{ TRUE };
	XINPUT_SETTING_T m_XInputSetting;
	void GetSendConfig_XInput();
	BOOL m_bGetXInputData{ FALSE };
	XINPUT_STATE m_XInput_state;
	BOOL m_bXInput_EnableKey{ FALSE };
	int Update_XInput_Header(const CString& strValue, XINPUT_SETTING_T* m_XInputSetting);
	int m_iControlDataOffset{ 0 };
	int m_iControlMathOffset{ 0 };
	void CheckDataOffset();
	COLORREF m_bgColor;
	void GetBackgroundColor();
	BOOL m_bFastDraw{ FALSE };
	BOOL m_bReqUpdateControlDraw{ FALSE };
	void SetDefaultCursor();
	int m_IDC_Focus{ -1 };
	void SetFocusOff();   

	typedef struct {
		char Header{ ' ' };
		std::vector <double> fDataValue;
		std::vector <double> fMathValue;
		int iHistoryLogIndex{ 0 };
		int iHistoryFilterIndex{ 0 };
		UINT iRcvTime{ 0 };
		UINT iRcvTime_ms{ 0 };
		int iRecvChannel { 0 };
	} UART_DATA;
	std::vector <UART_DATA> m_HistoryData;
	UART_DATA m_dummy_UARTData;

	void InitHistoryData();
	bool IsEnoughMemory(size_t requiredBytes);

	int m_iDataPage{ 1 };
	int m_iMathPage{ 2 };	//default
    int m_iHistoryCapacity { DEFAULT_TESTDATA_SIZE };

	int m_iMaxDataSize{ MAX_DATACONTROL_SIZE * m_iDataPage };
	int m_iMaxMathSize{ MAX_MATHCONTROL_SIZE * m_iMathPage };
	void SetMathDefine_ChangeDataSize(int pre_data_page, int post_data_page);
	void Open_File_with_FullPathname(CString& strFileName, CFile& SaveRcvData);
	void LoadMathProbeDefine(int iPrePage);

    afx_msg void OnBnClickedNoheader();
    BOOL m_bAcceptNoHeader{ FALSE };
	CString Double2Str(double val);
    CString Float2Str(float val);
	bool CheckNumStr(CString *pStr);
	long Str2Num(const CString& str2);
    CListCtrl m_ctrlLogList;	
	void OnSelectLogList(int iLogIndex);

	CListBox m_ctrlFilterBox;

	afx_msg void OnItemChangedLogList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnListCtrlClick(NMHDR* pNMHDR, LRESULT* pResult);

	void InitListCtrl();
	afx_msg void OnLvnGetdispinfoLogList(NMHDR* pNMHDR, LRESULT* pResult);
	void AddLogData(const CString& strLog);
	void ScrollToLastItem();
	void HighlightLog(int nIndex);

	std::vector <CString> m_arrLogData;
	std::vector<int> m_vecPrevSelectedIndices; // 이전 선택된 항목 인덱스들

    BOOL m_bIsAutoSave{ FALSE };
    CString  AddComma(int value);
	
	void GetDisplayDataSize();
	void GetDisplayDataSize(int * val);
	void SetDisplayDataSize(int val);
	void SetDisplayDataSize(void);

	// Randomize Graph Display Data Index
	int m_iRandInterval[N_RAND_INTERVAL] = { 0, };
	int m_iCurrentRadomIntervalIndex{ 0 };
	void MakeRandomIndexDataSet(int interval);

	BOOL m_bMouseWheelZoomDir{ 0 };
	BOOL m_bMouseWheelShiftDir{ 0 };
	afx_msg void OnCbnSelchangeSerialPort2();
	afx_msg void OnCbnSelchangeBaudRate2();
	afx_msg void OnPortOpen2();
	afx_msg void OnPortClose2();
	void CheckTimer();
	void UpdateConnectStatus(int* port_status);

	// Autosave관련 추가 아이템 미구현 상태임
	bool m_bUseAutosave1_LogCountLimit;
	bool m_bUseAutosave2_Time_LogCount;
	DWORD m_iAutosave2_cond_Time;
	int m_iAutosave2_cond_LogCount;
	bool m_bAutosave_AddCapture;

	DWORD m_iAutosave_EllapsedTime{ 0 };
	int m_iAutosave_LogCount{ 0 };
	void UpdateLogMessage();
	void UpdateAutoHederRx();
    void GetRegGrphHeight();
	afx_msg  BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    BOOL m_bReqLogSizeUpdate{ FALSE };
    void CheckLogUpdate(bool bForced = false);
    int m_iLOG_Update_Period{ 500 };
	afx_msg void OnBnClickedDisplay4();
	afx_msg void OnPortOpen3();
	afx_msg void OnPortClose3();
	afx_msg void OnCbnSelchangeSerialPort3();
	afx_msg void OnCbnSelchangeBaudRate3();
	afx_msg void OnBnClickedUseloadctrl();
	void AddLogData(TORQUE_DATA_T* data);
	void PostProcessTorqueData(TORQUE_DATA_T* data);
    afx_msg void OnBnClickedAutoconnect();
    bool m_bGetTorqueData{ false };
	bool m_bGetLoadData{ false };
    bool ReqConnetCheck(int iMode, int iPort);
    bool CheckTorquePort(int i, int iTimeout = 100);
	bool CheckLoadPort(int i, int iTimeout = 100);
	bool CheckMcuPort(int i, int iTimeout = 100);
	void Init_PS_Setting ();
	LOAD_PACKET_T m_LoadPacket;
	bool Send_LoadPacket(POWERSUPLY_CMD_e id, double fVal = 0.f);

	typedef bool(CSerialComDlg::*FuncLoad_T)(double);

	bool Func_PS_REQ_LOCK(double);
	bool Func_PS_REQ_CUR_SET(double);
	bool Func_PS_GET_CUR_SET(double);
	bool Func_PS_REQ_VOLT_SET(double);
	bool Func_PS_GET_VOLT_SET(double);
	bool Func_PS_GET_CUR_OUT(double);
	bool Func_PS_GET_VOLT_OUT(double);
	bool Func_PS_REQ_ONOFF(double);
	bool Func_PS_REQ_SET_OVER_CUR(double);
	bool Func_PS_REQ_SET_OVER_VOLT(double);
	bool Func_PS_REQ_SET_OV_ENABLE(double);
	bool Func_PS_REQ_SET_OC_ENABLE(double);
	bool Func_PS_REQ_SAVESETTING(double);
	bool Func_PS_REQ_RESTORE(double);
	bool Func_PS_GET_INFO(double);
	bool Func_PS_REQ_SET_BEEP(double);
	bool Func_PS_REQ_SET_MODE(double);
	bool Func_PS_GET_STATUS(double);		

	static const FuncLoad_T m_LoadFunc[];
	bool m_req_Load_Packet_reset{ false };
	afx_msg void OnBnClickedLoadcontrolsetting();
	bool m_bLoadControlSetting { false };
	afx_msg void OnBnClickedVoltup1();
	afx_msg void OnEnChangePsVolt();
	afx_msg void OnBnClickedVoltup2();
	afx_msg void OnBnClickedVoltup3();
	afx_msg void OnBnClickedVoltup4();
	int m_iVoltTarget_x100{0};
	afx_msg void OnBnClickedVoltdn1();
	afx_msg void OnBnClickedVoltdn2();
	afx_msg void OnBnClickedVoltdn3();
	afx_msg void OnBnClickedVoltdn4();
	afx_msg void OnBnClickedReqnoload();

	void AutoControlVoltage();
	afx_msg void OnBnClickedIncGain();
	afx_msg void OnBnClickedDecGain();
	afx_msg void OnEnChangePsRate();
	afx_msg void OnEnChangeLoadctrlTargettorque();
	afx_msg void OnBnClickedTorqueup1();
	afx_msg void OnBnClickedTorqueup2();
	afx_msg void OnBnClickedTorqueup3();
	afx_msg void OnBnClickedTorquedn1();
	afx_msg void OnBnClickedTorquedn2();
	afx_msg void OnBnClickedTorquedn3();
	afx_msg void OnBnClickedTorquedn4();
	afx_msg void OnBnClickedVoltdn5();
	afx_msg void OnEnChangePsRate2();
	afx_msg void OnBnClickedIncGain2();
	afx_msg void OnBnClickedDecGain2();

	bool m_bResetIntGain{false};
	Load_Setting_t m_LoadSetting;

	void GetReg_LoadSetting();
	void SetReg_LoadSetting();
	afx_msg void OnBnClickedSaveloadsetting();
	void LoadDynamoPinName();
	double m_fTargetTorque;
	CString m_strTargetTorque;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMDLG_H__4FB3E547_C3BA_4697_81B7_AB1991BEA61A__INCLUDED_)
