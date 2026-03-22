// SerialComDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialCom.h"
#include "SerialComDlg.h"
#include "CHistory.h"
#include "CRefDataDlg.h"
#include "Colorlistctrl.h"
#include "ModeCopyDlg.h"
#include "GamePad.h"
#include "XInputDlg.h"
#include "ReloadDataDlg.h"
#include "LoadCtrlDlg.h"

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include "Game.h"
#include <algorithm>
#include <string>
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strPort1name, strPort2name, strPort3name;

unsigned Counter = 0;
int count = 0;
HWND hCommWnd;

const CSerialComDlg::FuncLoad_T CSerialComDlg::m_LoadFunc[] = {
		&CSerialComDlg::Func_PS_REQ_LOCK,
		&CSerialComDlg::Func_PS_REQ_CUR_SET,
		&CSerialComDlg::Func_PS_GET_CUR_SET,
		&CSerialComDlg::Func_PS_REQ_VOLT_SET,
		&CSerialComDlg::Func_PS_GET_VOLT_SET,
		&CSerialComDlg::Func_PS_GET_CUR_OUT,
		&CSerialComDlg::Func_PS_GET_VOLT_OUT,
		&CSerialComDlg::Func_PS_REQ_ONOFF,
		&CSerialComDlg::Func_PS_REQ_SET_OVER_CUR,
		&CSerialComDlg::Func_PS_REQ_SET_OVER_VOLT,
		&CSerialComDlg::Func_PS_REQ_SET_OV_ENABLE,
		&CSerialComDlg::Func_PS_REQ_SET_OC_ENABLE,
		&CSerialComDlg::Func_PS_REQ_SAVESETTING,
		&CSerialComDlg::Func_PS_REQ_RESTORE,
		&CSerialComDlg::Func_PS_GET_INFO,
		&CSerialComDlg::Func_PS_REQ_SET_BEEP,
		&CSerialComDlg::Func_PS_REQ_SET_MODE,
		&CSerialComDlg::Func_PS_GET_STATUS,
};

CSerialComDlg::CSerialComDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialComDlg::IDD, pParent)
	, m_strPacketHeader(_T("w"))
	, m_cPacketHead('w')
	, m_iLogIndex(0)
	, m_strTemp(_T(""))
	, m_iProgramMode(DATA_ANALYZE_MODE)
	, m_bUpdateList(FALSE)
	, m_bReqGraphUpdate(FALSE)
	, m_bSaveFile(FALSE)
	, m_iDisplaySize(1000)
	, m_bDrawSelectData(FALSE)
	, m_iHistoryIndex(0)
	, m_iHistoryMove(0)
	, m_iTimeDisplaySize(10)
	, m_strIndexNumber(_T(""))
	, m_iPresetMode(0)
	, m_iFilterIndex(0)
	, m_bReqRedrawGraph(TRUE)
	, m_iLegendX(165)
 	, m_iLegendX_PositionGraph(250)
	, m_bAdjustLegendX(FALSE)
	, m_bIsAdjustMode(FALSE)
	, m_bSelectListBox(FALSE)
	, m_iHistorySelectPt(0)
	, m_iGraphCount(4)
	, m_iSelectGraphCount(3)
	, m_iTimeDisplayMode(0)
	, m_iGridX(50)
	, m_iSelectModel(0)
	, m_pStatistics(NULL)
	, m_bUseStatistics(FALSE)
	, m_iAdjustSelectMode(-1)
	, m_bFixGraphFrame(TRUE)
	, m_bDisplayPosition(FALSE)
	, m_fValueScale_PositionGraph(5.)
	, m_fMidValueX_PositionGraph(0)
	, m_fMidValueY_PositionGraph(0)
	, m_iDev(3)
	, m_iFrameSizeGraphPosition(800)
	, m_bRetrunSubProgram(FALSE)
	, m_bReqAutoScalePositionDraw(FALSE)
	, m_bUseAutoScaleGraphPosition(TRUE)
	, m_iRatioLogFilter(50)
	, m_iFilterViewMode(0)
	, m_iGridCountY(1)
	, m_iMarkSize(2)
	, m_bTimelineFullRangeView(FALSE)
	, m_bUseAutoSave(FALSE)
	, m_bUseAutoRange(FALSE)
	, m_bShowLabelValue(TRUE)
	, m_bGetSettingInLoading(TRUE)
	, m_iComPortCount(0)
	, m_iRangeLogFilterWidth(421)
	, m_iShowDataControl(MAX_DATACONTROL_SIZE)
	, m_iRangeDataControlVertical(322)
	, m_CursorMode(0)
	, m_bConnect_Xinput(FALSE)
{
	m_iStopBit = 0;
	m_iSerialPort = 1;
	m_iParity = 0;
	m_iDataBit = 3;
	m_iBaudRate = 7;  // 9 : 460800    // 7 : 115200  
    
	// Plot Pen초기화
	m_PenGrid.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	
	m_iDrawGraphRangeX[0] = MIN_RANGE_GRAPH_H;
	m_iPreDrawGraphRangeX = m_iDrawGraphRangeX[0];
	m_iDrawGraphRangeX[1] = 1680;
	m_iDrawGraphSpanX = max(1, m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0]);
	m_iDrawGraphMidX = (m_iDrawGraphRangeX[1] + m_iDrawGraphRangeX[0]) / 2;

	m_PenSelect.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_PenCursor.CreatePen(PS_SOLID, 1, RGB(200, 128, 200));
	m_PenCursor1.CreatePen(PS_SOLID, 2, RGB(255, 0, 255));

	m_PenAdjust.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
		m_iDrawGraphRangeY[i][0] = 65 + i * 224;
		m_iDrawGraphRangeY[i][1] = 260 + i * 224;

		m_strRangeMax[i].Format("1000");
		m_fRangeMax[i] = 1000;
		m_strRangeMin[i].Format("0");
		m_fRangeMin[i] = 0;

		m_strRangeMid[i].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMin[i] + m_fRangeMax[i]) / 2));

		for (int j = 0; j < m_iMaxDataSize; j++) {
			m_bDraw_GraphData[i][j] = FALSE;
			m_bSelectDraw_GraphData[i][j] = FALSE;
		}
		m_bDrawChannel[0] = TRUE;
		m_bDrawChannel[1] = TRUE;
		m_bDrawGraph[i] = TRUE;
	}

	for (int i = 0; i < 3; i++) {
		m_bTimelineControl[i] = FALSE;
	}

	for (int i = 0; i < m_iGraphCount; i++)
		m_bAutoRange_Graph[i] = FALSE;
	m_BrushNull.CreateStockObject(NULL_BRUSH);

	m_LogBrush.lbStyle = BS_SOLID;
	m_LogBrush.lbHatch = 0;

	m_BrushMark[0].CreateSolidBrush(RGB(0, 0, 230));
	m_BrushMark[1].CreateSolidBrush(RGB(0, 50, 230));

	m_BrushAdjust[0].CreateSolidBrush(RGB(168, 168, 168));
	m_BrushAdjust[1].CreateSolidBrush(RGB(200, 0, 200));

	m_Brush[0].CreateSolidBrush(COLOR_WHITE);
	m_Brush[1].CreateSolidBrush(COLOR_BLACK); 
	m_Brush[2].CreateSolidBrush(COLOR_YELLOW);
	m_Brush[3].CreateSolidBrush(COLOR_ORANGE);
	m_Brush[4].CreateSolidBrush(COLOR_GRAY);
	m_Brush[5].CreateSolidBrush(COLOR_BLUE);

	m_PenWhite.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_PenBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	m_PenMark[0].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_PenMark[1].CreatePen(PS_SOLID, 1, RGB(0, 64, 255));

	m_PenOrigin_PositionGraph.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_PenDash_PositionGraph.CreatePen(PS_DOT, 1, RGB(0, 0, 0));

	m_PenPosition[0].CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_PenPosition[1].CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	m_BrushPosition[0].CreateSolidBrush(RGB(100, 100, 100));
	m_BrushPosition[1].CreateSolidBrush(RGB(255, 255, 0));

	m_Panning_PositionGraph.isPressed = FALSE;
	m_Measure_PositionGraph.isPressed = FALSE;
	m_Resize_PositionGraph.isPressed = FALSE;
	m_AdjustLogFilter_V.isPressed = FALSE;
	m_AdjustLogFilter_H.isPressed = FALSE;
	m_AdjustDataWnd_V.isPressed = FALSE;

	m_FontLabel.CreateFontA(H_LABEL_TEXT, W_LABEL_TEXT, 0, 0, FW_NORMAL, FALSE, FALSE, 0, HANGUL_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "굴림체");
	m_FontDefault.CreateFontA(12, 0, 0, 0, 600, FALSE, FALSE, 0, HANGUL_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "ARIAL");
	m_FontBold.CreateFontA(13, 0, 0, 0, 600, FALSE, FALSE, 0, HANGUL_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "ARIAL");

	// Control ID
	int IDC_MathText[MAX_MATHCONTROL_SIZE] = { IDC_STATIC_1, IDC_STATIC_2, IDC_STATIC_3, IDC_STATIC_4, IDC_STATIC_5, IDC_STATIC_6, IDC_STATIC_7, IDC_STATIC_8, IDC_STATIC_9, IDC_STATIC_10, IDC_STATIC_11, IDC_STATIC_12, IDC_STATIC_13, IDC_STATIC_14, IDC_STATIC_15, IDC_STATIC_16, };
	
	int IDC_DataNumber[MAX_DATACONTROL_SIZE] = { IDC_STATIC_D1, IDC_STATIC_D2, IDC_STATIC_D3, IDC_STATIC_D4, IDC_STATIC_D5, IDC_STATIC_D6, IDC_STATIC_D7, IDC_STATIC_D8, IDC_STATIC_D9, IDC_STATIC_D10, IDC_STATIC_D11, IDC_STATIC_D12, IDC_STATIC_D13, IDC_STATIC_D14, IDC_STATIC_D15, IDC_STATIC_D16 };
	int IDC_PinName[MAX_DATACONTROL_SIZE] = { IDC_PinDescription0, IDC_PinDescription1, IDC_PinDescription2, IDC_PinDescription3, IDC_PinDescription4, IDC_PinDescription5, IDC_PinDescription6, IDC_PinDescription7, IDC_PinDescription8, IDC_PinDescription9, IDC_PinDescription10, IDC_PinDescription11, IDC_PinDescription12, IDC_PinDescription13, IDC_PinDescription14, IDC_PinDescription15 };
	int IDC_DataValue[MAX_DATACONTROL_SIZE] = { IDC_Name_DataPin0, IDC_Name_DataPin1, IDC_Name_DataPin2, IDC_Name_DataPin3, IDC_Name_DataPin4, IDC_Name_DataPin5, IDC_Name_DataPin6, IDC_Name_DataPin7, IDC_Name_DataPin8, IDC_Name_DataPin9, IDC_Name_DataPin10, IDC_Name_DataPin11, IDC_Name_DataPin12, IDC_Name_DataPin13, IDC_Name_DataPin14, IDC_Name_DataPin15 };

	int IDC_StaticMath[MAX_MATHCONTROL_SIZE] = { IDC_STATIC_1, IDC_STATIC_2, IDC_STATIC_3, IDC_STATIC_4, IDC_STATIC_5, IDC_STATIC_6, IDC_STATIC_7, IDC_STATIC_8, IDC_STATIC_9, IDC_STATIC_10, IDC_STATIC_11, IDC_STATIC_12, IDC_STATIC_13, IDC_STATIC_14, IDC_STATIC_15, IDC_STATIC_16, };
	int IDC_MathProbeName[MAX_MATHCONTROL_SIZE] = { IDC_MathProbeNameValue_M0, IDC_MathProbeNameValue_M1, IDC_MathProbeNameValue_M2, IDC_MathProbeNameValue_M3, IDC_MathProbeNameValue_M4, IDC_MathProbeNameValue_M5, IDC_MathProbeNameValue_M6, IDC_MathProbeNameValue_M7, IDC_MathProbeNameValue_M8, IDC_MathProbeNameValue_M9, IDC_MathProbeNameValue_M10, IDC_MathProbeNameValue_M11, IDC_MathProbeNameValue_M12, IDC_MathProbeNameValue_M13, IDC_MathProbeNameValue_M14, IDC_MathProbeNameValue_M15, };
	int IDC_MathProbeValue[MAX_MATHCONTROL_SIZE] = { IDC_MathProbeValue_M0, IDC_MathProbeValue_M1, IDC_MathProbeValue_M2, IDC_MathProbeValue_M3, IDC_MathProbeValue_M4, IDC_MathProbeValue_M5, IDC_MathProbeValue_M6, IDC_MathProbeValue_M7, IDC_MathProbeValue_M8, IDC_MathProbeValue_M9, IDC_MathProbeValue_M10, IDC_MathProbeValue_M11, IDC_MathProbeValue_M12, IDC_MathProbeValue_M13, IDC_MathProbeValue_M14, IDC_MathProbeValue_M15, };
	int IDC_DrawGraph[MAX_GRAPH_COUNT] = { IDC_DrawGraph0, IDC_DrawGraph1, IDC_DrawGraph2, IDC_DrawGraph3, IDC_DrawGraph4, IDC_DrawGraph5, IDC_DrawGraph6, IDC_DrawGraph7, };

	int iDefaultMathRed[MAX_MATHCONTROL_SIZE * 4] = { 64,  192, 0,   122, 0,   122, 0, 32,   122, 64, 164, 164, 32, 122,32, 60, 64,  192, 0,   122, 0,   122, 0, 32,   122, 64, 164, 164, 32, 122,32, 60,64,  192, 0,   122, 0,   122, 0, 32,   122, 64, 164, 164, 32, 122,32, 60, 64,  192, 0,   122, 0,   122, 0, 32,   122, 64, 164, 164, 32, 122,32, 60, };
	int iDefaultMathGreen[MAX_MATHCONTROL_SIZE * 4] = { 192, 0,   64,  0,  122, 64,  164, 122, 64,  0,  164,  0,  122, 32, 32, 12, 192, 0,   64,  0,  122, 64,  164, 122, 64,  0,  164,  0,  122, 32, 32, 12, 192, 0,   64,  0,  122, 64,  164, 122, 64,  0,  164,  0,  122, 32, 32, 12, 192, 0,   64,  0,  122, 64,  164, 122, 64,  0,  164,  0,  122, 32, 32, 12, };
	int iDefaultMathBlue[MAX_MATHCONTROL_SIZE * 4] = { 0,   64,  192, 122, 64, 0,   164,  32, 0,   122, 0, 164,  32, 32, 122, 224, 0,   64,  192, 122, 64, 0,   164,  32, 0,   122, 0, 164,  32, 32, 122, 224, 0,   64,  192, 122, 64, 0,   164,  32, 0,   122, 0, 164,  32, 32, 122, 224, 0,   64,  192, 122, 64, 0,   164,  32, 0,   122, 0, 164,  32, 32, 122, 224, };

	int iDefaultPenColor[MAX_DATACONTROL_SIZE * 4][3] ={
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
		{0, 0, 0}, {255, 0, 0},	{0, 255, 0}, {0, 0, 255}, {64, 64, 0}, {255, 0, 255}, {0, 255, 255}, {192, 192, 0}, {0, 128, 128},{128, 0, 128}, {128, 255, 0}, {0, 128, 255}, {255, 128, 0}, {128, 128, 128}, {128, 255, 128}, {128, 128, 255},
	};

	int IDC_MaxY_Graph[MAX_GRAPH_COUNT] = { IDC_MaxY_Graph1, IDC_MaxY_Graph2, IDC_MaxY_Graph3, IDC_MaxY_Graph4, IDC_MaxY_Graph5, IDC_MaxY_Graph6, IDC_MaxY_Graph7, IDC_MaxY_Graph8, };
	int IDC_MinY_Graph[MAX_GRAPH_COUNT] = { IDC_MinY_Graph1, IDC_MinY_Graph2, IDC_MinY_Graph3, IDC_MinY_Graph4, IDC_MinY_Graph5, IDC_MinY_Graph6, IDC_MinY_Graph7, IDC_MinY_Graph8, };
	int IDC_MidY_Graph[MAX_GRAPH_COUNT] = { IDC_MidY_Graph1, IDC_MidY_Graph2, IDC_MidY_Graph3, IDC_MidY_Graph4, IDC_MidY_Graph5, IDC_MidY_Graph6, IDC_MidY_Graph7, IDC_MidY_Graph8, };
	int IDC_AutoRange[MAX_GRAPH_COUNT] = { IDC_AutoRange0, IDC_AutoRange1, IDC_AutoRange2, IDC_AutoRange3, IDC_AutoRange4, IDC_AutoRange5, IDC_AutoRange6, IDC_AutoRange7, };

	for (int i = 0; i < MAXIMUM_MATH_SIZE; i++) {
		m_iDefaultMathColor[i][0] = iDefaultMathRed[i];
		m_iDefaultMathColor[i][1] = iDefaultMathGreen[i];
		m_iDefaultMathColor[i][2] = iDefaultMathBlue[i];
	}

	for (int i = 0; i < MAXIMUM_DATA_SIZE; i++) {
		m_iDefaultDataColor[i][0] = iDefaultPenColor[i][0];
		m_iDefaultDataColor[i][1] = iDefaultPenColor[i][1];
		m_iDefaultDataColor[i][2] = iDefaultPenColor[i][2];
	}
	for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
		m_IDC_DrawGraph[i] = IDC_DrawGraph[i];
	}

	for (int i = 0; i < MAX_DATACONTROL_SIZE; i++) {
		m_IDC_DataNumber[i] = IDC_DataNumber[i];
		m_IDC_PinName[i] = IDC_PinName[i];
		m_IDC_DataValue[i] = IDC_DataValue[i];
	}

	for (int i = 0; i < MAX_MATHCONTROL_SIZE; i++) {
		m_IDC_MathText[i] = IDC_MathText[i];
		m_IDC_StaticMath[i] = IDC_StaticMath[i];
		m_IDC_MathProbeName[i] = IDC_MathProbeName[i];
		m_IDC_MathProbeValue[i] = IDC_MathProbeValue[i];
	}

	for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
		m_IDC_MaxY_Graph[i] = IDC_MaxY_Graph[i];
		m_IDC_MinY_Graph[i] = IDC_MinY_Graph[i];
		m_IDC_MidY_Graph[i] = IDC_MidY_Graph[i];
		m_IDC_AutoRange[i] = IDC_AutoRange[i];
	}

	int IDC_UseFilter[MAX_FILTER] = { IDC_UseFilter0, IDC_UseFilter1, IDC_UseFilter2, IDC_UseFilter3, IDC_UseFilter4, IDC_UseFilter5, IDC_UseFilter6, IDC_UseFilter7, IDC_UseFilter8, IDC_UseFilter9, IDC_UseFilter10, IDC_UseFilter11, };
	int IDC_FilterName[MAX_FILTER] = { IDC_FilterName0, IDC_FilterName1, IDC_FilterName2, IDC_FilterName3, IDC_FilterName4, IDC_FilterName5, IDC_FilterName6, IDC_FilterName7, IDC_FilterName8, IDC_FilterName9, IDC_FilterName10, IDC_FilterName11, };
	int IDC_FilterValue[MAX_FILTER] = { IDC_FilterValue0, IDC_FilterValue1, IDC_FilterValue2, IDC_FilterValue3, IDC_FilterValue4, IDC_FilterValue5, IDC_FilterValue6, IDC_FilterValue7, IDC_FilterValue8, IDC_FilterValue9, IDC_FilterValue10, IDC_FilterValue11, };

	for (int i = 0; i < MAX_FILTER; i++) {
		m_IDC_UseFilter[i] = IDC_UseFilter[i];
		m_IDC_FilterName[i] = IDC_FilterName[i];
		m_IDC_FilterValue[i] = IDC_FilterValue[i];
	}

	ClearMeasure3P();

	m_iDefaultRGB_SelectControl_Select[0][0] = 50;
	m_iDefaultRGB_SelectControl_Select[1][0] = 50;
	m_iDefaultRGB_SelectControl_Select[2][0] = 200;
	m_iDefaultRGB_SelectControl_Select[0][1] = 50;
	m_iDefaultRGB_SelectControl_Select[1][1] = 50;
	m_iDefaultRGB_SelectControl_Select[2][1] = 250;

	m_iDefaultRGB_SelectControl_Outline[0][0] = 30;
	m_iDefaultRGB_SelectControl_Outline[1][0] = 30;
	m_iDefaultRGB_SelectControl_Outline[2][0] = 30;
	m_iDefaultRGB_SelectControl_Outline[0][1] = 0;
	m_iDefaultRGB_SelectControl_Outline[1][1] = 0;
	m_iDefaultRGB_SelectControl_Outline[2][1] = 0;

	m_iDefaultRGB_SelectControl_Blank[0][0] = 230;
	m_iDefaultRGB_SelectControl_Blank[1][0] = 230;
	m_iDefaultRGB_SelectControl_Blank[2][0] = 230;
	m_iDefaultRGB_SelectControl_Blank[0][1] = 255;
	m_iDefaultRGB_SelectControl_Blank[1][1] = 255;
	m_iDefaultRGB_SelectControl_Blank[2][1] = 255;

	LoadDefaultMathProbeSetting();

	mh_Cursor[0] = AfxGetApp()->LoadCursor(IDC_POINTER);
	mh_Cursor[1] = AfxGetApp()->LoadCursor(IDC_CURSOR2);
	mh_Cursor[2] = AfxGetApp()->LoadCursor(IDC_CURSOR3);
	mh_Cursor[3] = AfxGetApp()->LoadCursor(IDC_CURSOR4);

	char* str_default[N_CONVERT_REG][2] = {
		{"//",				"\\"},													// 1, 0
		{"/",				"\\"},													// 1, 0
		{"\\SerialMonitor",				"\\TestDataAnalyzer"},						// 0, 1
		{"\\SerialMonitor\\FilterView",	"\\SerialMonitor\\Filter\\FilterView"},		// 1, 1
		{"\\SerialMonitor\\Graph\\",	"\\SerialMonitor\\Header\\"},
		{"\\SerialMonitor\\Graph0",		"\\SerialMonitor\\Graph\\Graph0"},
		{"\\SerialMonitor\\Graph1",		"\\SerialMonitor\\Graph\\Graph1"},
		{"\\SerialMonitor\\Graph2",		"\\SerialMonitor\\Graph\\Graph2"},
		{"\\SerialMonitor\\Graph3",		"\\SerialMonitor\\Graph\\Graph3"},
		{"\\SerialMonitor\\Graph4",		"\\SerialMonitor\\Graph\\Graph4"},
		{"\\SerialMonitor\\Graph5",		"\\SerialMonitor\\Graph\\Graph5"},
		{"\\SerialMonitor\\Graph6",		"\\SerialMonitor\\Graph\\Graph6"},
		{"\\SerialMonitor\\Graph7",		"\\SerialMonitor\\Graph\\Graph7"},
		{"\\SerialMonitor\\MathProbe",	"\\SerialMonitor\\MathDefine\\MathProbe"},
		{"\\SerialMonitor\\GUI",		"\\SerialMonitor\\Graph"},					// 1, 0
		{"\\SerialMonitor",				"\\TestDataAnalyzer"},						// 1, 0
	};

	int use[N_CONVERT_REG][2] = {
		{1, 0}, {1, 0},
		{0, 1},
		{1, 1}, {1, 1},{1, 1},{1, 1},{1, 1},{1, 1},{1, 1},{1, 1},{1, 1},{1, 1},{1, 1},
		{1, 0}, {1, 0},
	};
	
	for (int i = 0; i < N_CONVERT_REG; i++) {
		m_ConverItem[i].str[0] = str_default[i][0];
		m_ConverItem[i].str[1] = str_default[i][1];
		m_ConverItem[i].use[0] = use[i][0];
		m_ConverItem[i].use[1] = use[i][1];
	}
}
	   
void CSerialComDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FilterMessage, m_ctrlFilterBox);
    DDX_Control(pDX, IDC_LogList, m_ctrlLogList);

    DDX_Control(pDX, IDC_SERIAL_PORT, m_cSerialPort);
    DDX_CBIndex(pDX, IDC_SERIAL_PORT, m_iSerialPort);
    DDX_Control(pDX, IDC_BAUD_RATE, m_cBaudRate);
    DDX_CBIndex(pDX, IDC_BAUD_RATE, m_iBaudRate);

	DDX_Control(pDX, IDC_SERIAL_PORT2, m_cSerialPort2);
	DDX_CBIndex(pDX, IDC_SERIAL_PORT2, m_iSerialPort2);
	DDX_Control(pDX, IDC_BAUD_RATE2, m_cBaudRate2);
	DDX_CBIndex(pDX, IDC_BAUD_RATE2, m_iBaudRate2);

	DDX_Control(pDX, IDC_SERIAL_PORT3, m_cSerialPort3);
	DDX_CBIndex(pDX, IDC_SERIAL_PORT3, m_iSerialPort3);
	DDX_Control(pDX, IDC_BAUD_RATE3, m_cBaudRate3);
	DDX_CBIndex(pDX, IDC_BAUD_RATE3, m_iBaudRate3);

    DDX_Control(pDX, IDC_Model, m_cSelectModel);
    DDX_CBIndex(pDX, IDC_Model, m_iSelectModel);
    DDX_Text(pDX, IDC_PacketHead, m_strPacketHeader);

    for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
        DDX_Text(pDX, m_IDC_MaxY_Graph[i], m_strRangeMax[i]);
        DDX_Text(pDX, m_IDC_MinY_Graph[i], m_strRangeMin[i]);
        DDX_Text(pDX, m_IDC_MidY_Graph[i], m_strRangeMid[i]);
        DDX_Check(pDX, m_IDC_AutoRange[i], m_bAutoRange_Graph[i]);
    }
    DDX_Text(pDX, IDC_IndexNumber, m_strIndexNumber);

    DDX_Text(pDX, IDC_DataSetting, m_strDataSetting);
    DDX_Text(pDX, IDC_DataSetting2, m_strDataSetting2);
    DDX_Control(pDX, IDC_ProgressFileReading, m_ctrlProgReading);    
    DDX_Check(pDX, IDC_Range, m_bUseAutoRange);    
    DDX_Control(pDX, IDC_Mode, m_ctrlMode);
    DDX_Check(pDX, IDC_ShowSelectValue, m_bShowLabelValue);
}

BEGIN_MESSAGE_MAP(CSerialComDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_COMM_READ1, OnCommunication) 
    ON_MESSAGE(WM_COMM_READ2, OnCommunication2)
	ON_MESSAGE(WM_COMM_READ3, OnCommunication3)
	ON_BN_CLICKED(IDC_PORT_OPEN, OnPortOpen1)
	ON_BN_CLICKED(IDC_PORT_CLOSE, OnPortClose1)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE, OnSelchangeBaudRate)
	ON_CBN_SELCHANGE(IDC_SERIAL_PORT, OnSelchangeSerialPort)
	ON_WM_TIMER()
	ON_CBN_SETFOCUS(IDC_SERIAL_PORT, &CSerialComDlg::OnSetfocusSerialPort)
	ON_CBN_SETFOCUS(IDC_SERIAL_PORT2, &CSerialComDlg::OnSetfocusSerialPort2)
	ON_CBN_SETFOCUS(IDC_SERIAL_PORT3, &CSerialComDlg::OnSetfocusSerialPort3)
	ON_EN_CHANGE(IDC_PacketHead, &CSerialComDlg::OnEnChangePackethead)
	ON_BN_CLICKED(IDC_Send, &CSerialComDlg::OnBnClickedSend)
	ON_WM_LBUTTONDBLCLK()
	ON_EN_CHANGE(IDC_MaxY_Graph1, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph1, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph2, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph2, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph3, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph3, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph4, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph4, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph5, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph5, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph6, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph6, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph7, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph7, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MaxY_Graph8, &CSerialComDlg::OnChangeRange)
	ON_EN_CHANGE(IDC_MinY_Graph8, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_DefaultButton, &CSerialComDlg::OnBnClickedDefaultbutton)
	ON_BN_CLICKED(IDC_SaveConfiguration, &CSerialComDlg::OnBnClickedSaveconfiguration)
	ON_BN_CLICKED(IDC_UserCommandSetting, &CSerialComDlg::OnBnClickedUsercommandsetting)

	ON_BN_CLICKED(IDC_UserCommand0, &CSerialComDlg::OnBnClickedUsercommand0)
	ON_BN_CLICKED(IDC_UserCommand1, &CSerialComDlg::OnBnClickedUsercommand1)
	ON_BN_CLICKED(IDC_UserCommand2, &CSerialComDlg::OnBnClickedUsercommand2)
	ON_BN_CLICKED(IDC_UserCommand3, &CSerialComDlg::OnBnClickedUsercommand3)
	ON_BN_CLICKED(IDC_UserCommand4, &CSerialComDlg::OnBnClickedUsercommand4)
	ON_BN_CLICKED(IDC_UserCommand5, &CSerialComDlg::OnBnClickedUsercommand5)
	ON_BN_CLICKED(IDC_UserCommand6, &CSerialComDlg::OnBnClickedUsercommand6)
	ON_BN_CLICKED(IDC_UserCommand7, &CSerialComDlg::OnBnClickedUsercommand7)
	ON_BN_CLICKED(IDC_UserCommand8, &CSerialComDlg::OnBnClickedUsercommand8)
	ON_BN_CLICKED(IDC_UserCommand9, &CSerialComDlg::OnBnClickedUsercommand9)
	
	ON_BN_CLICKED(IDC_UseFilter0, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter1, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter2, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter3, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter4, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter5, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter6, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter7, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter8, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter9, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter10, &CSerialComDlg::ChangeFilterUse)
	ON_BN_CLICKED(IDC_UseFilter11, &CSerialComDlg::ChangeFilterUse)

	ON_EN_CHANGE(IDC_FilterName0, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName1, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName2, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName3, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName4, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName5, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName6, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName7, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName8, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName9, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName10, &CSerialComDlg::ChangeFilterItem)
	ON_EN_CHANGE(IDC_FilterName11, &CSerialComDlg::ChangeFilterItem)

	ON_EN_CHANGE(IDC_DisplayDataSize, &CSerialComDlg::OnEnChangeDisplaydatasize)
	ON_BN_CLICKED(IDC_AutoRange0, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange1, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange2, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange3, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange4, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange5, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange6, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_AutoRange7, &CSerialComDlg::OnChangeRange)
	ON_BN_CLICKED(IDC_UserCommand10, &CSerialComDlg::OnBnClickedUsercommand10)
	ON_BN_CLICKED(IDC_UserCommand11, &CSerialComDlg::OnBnClickedUsercommand11)
	ON_BN_CLICKED(IDC_UserCommand12, &CSerialComDlg::OnBnClickedUsercommand12)
	ON_BN_CLICKED(IDC_UserCommand13, &CSerialComDlg::OnBnClickedUsercommand13)
	ON_BN_CLICKED(IDC_UserCommand14, &CSerialComDlg::OnBnClickedUsercommand14)
	ON_BN_CLICKED(IDC_UserCommand15, &CSerialComDlg::OnBnClickedUsercommand15)
	ON_BN_CLICKED(IDC_UserCommand16, &CSerialComDlg::OnBnClickedUsercommand16)
	ON_BN_CLICKED(IDC_UserCommand17, &CSerialComDlg::OnBnClickedUsercommand17)
	ON_BN_CLICKED(IDC_UserCommand18, &CSerialComDlg::OnBnClickedUsercommand18)
	ON_BN_CLICKED(IDC_UserCommand19, &CSerialComDlg::OnBnClickedUsercommand19)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_ClearWatch, &CSerialComDlg::OnBnClickedClearwatch)
	ON_BN_CLICKED(IDC_ReqResetData, &CSerialComDlg::OnBnClickedReqresetdata)
	ON_BN_CLICKED(IDC_IndexUp1, &CSerialComDlg::OnBnClickedIndexup1)
	ON_BN_CLICKED(IDC_IndexDn1, &CSerialComDlg::OnBnClickedIndexdn1)
	ON_BN_CLICKED(IDC_ReqSaveData, &CSerialComDlg::OnBnClickedReqsavedata)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_FilterMessage, &CSerialComDlg::OnLbnSelchangeFiltermessage)
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_IndexNumber, &CSerialComDlg::OnEnChangeIndexnumber)
	ON_BN_CLICKED(IDC_ClearFilterList, &CSerialComDlg::OnBnClickedClearfilterlist)
	ON_BN_CLICKED(IDC_LoadWatchList, &CSerialComDlg::OnBnClickedLoadwatchlist)
	ON_EN_CHANGE(IDC_PinDescription0, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription1, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription2, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription3, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription4, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription5, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription6, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription7, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription8, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription9, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription10, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription11, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription12, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription13, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription14, &CSerialComDlg::ChangeDataName)
	ON_EN_CHANGE(IDC_PinDescription15, &CSerialComDlg::ChangeDataName)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ShowHistory, &CSerialComDlg::OnBnClickedShowhistory)
	ON_CBN_SELCHANGE(IDC_Model, &CSerialComDlg::OnCbnSelchangeModel)
	ON_BN_CLICKED(IDC_LoadCalculation, &CSerialComDlg::OnBnClickedLoadcalculation)		
	ON_BN_CLICKED(IDC_DefineMathProbe, &CSerialComDlg::OnBnClickedDefinemathprobe)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_BN_CLICKED(IDC_SerialInfo, &CSerialComDlg::OnBnClickedSerialinfo)
	ON_BN_CLICKED(IDC_Game, &CSerialComDlg::OnBnClickedGame)
	ON_BN_CLICKED(IDC_ClearCursor, &CSerialComDlg::OnBnClickedClearCursor)
	ON_BN_CLICKED(IDC_ShowSelectValue, &CSerialComDlg::OnBnClickedShowselectvalue)
	ON_BN_CLICKED(IDC_AddCursor, &CSerialComDlg::OnBnClickedAddcursor)
	ON_BN_CLICKED(IDC_UseAutoSave, &CSerialComDlg::UpdateUseAutoSave)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ComPortList, &CSerialComDlg::OnBnClickedComportlist)
	ON_BN_CLICKED(IDC_Capture, &CSerialComDlg::OnBnClickedCapture)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_RefData, &CSerialComDlg::OnBnClickedRefdata)
	ON_BN_CLICKED(IDC_ReqcontrolDlg, &CSerialComDlg::OnBnClickedReqcontroldlg)
	ON_BN_CLICKED(IDC_Reload, &CSerialComDlg::OnBnClickedReload)
	ON_BN_CLICKED(IDC_FitToMaxData, &CSerialComDlg::OnBnClickedFittomaxdata)
	ON_BN_CLICKED(IDC_Display0, &CSerialComDlg::OnBnClickedDisplay0)
	ON_BN_CLICKED(IDC_Display1, &CSerialComDlg::OnBnClickedDisplay1)
	ON_BN_CLICKED(IDC_Display3, &CSerialComDlg::OnBnClickedDisplay3)
	ON_BN_CLICKED(IDC_Display2, &CSerialComDlg::OnBnClickedDisplay2)
	ON_CBN_SELCHANGE(IDC_Mode, &CSerialComDlg::OnCbnSelchangeMode)
	ON_BN_CLICKED(IDC_ModeCopy, &CSerialComDlg::OnBnClickedModecopy)
	ON_EN_CHANGE(IDC_MaxFitScreen, &CSerialComDlg::OnEnChangeMaxfitscreen)
	ON_BN_CLICKED(IDC_ShowGraphLog, &CSerialComDlg::OnBnClickedShowgraphlog)
	ON_BN_CLICKED(IDC_ShowTextLog, &CSerialComDlg::OnBnClickedShowtextlog)
	ON_BN_CLICKED(IDC_GraphUpdate, &CSerialComDlg::OnBnClickedGraphupdate)
	ON_BN_CLICKED(IDC_Test, &CSerialComDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_DrawGraph0, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph1, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph2, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph3, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph4, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph5, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph6, &CSerialComDlg::UpdateDrawGraphControl)
	ON_BN_CLICKED(IDC_DrawGraph7, &CSerialComDlg::UpdateDrawGraphControl)
    ON_BN_CLICKED(IDC_NoHeader, &CSerialComDlg::OnBnClickedNoheader)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LogList, &CSerialComDlg::OnLvnGetdispinfoLogList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LogList, &CSerialComDlg::OnItemChangedLogList)
    ON_NOTIFY(NM_CLICK, IDC_LogList, &CSerialComDlg::OnListCtrlClick)
	ON_CBN_SELCHANGE(IDC_SERIAL_PORT2, &CSerialComDlg::OnCbnSelchangeSerialPort2)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE2, &CSerialComDlg::OnCbnSelchangeBaudRate2)
	ON_BN_CLICKED(IDC_PORT_OPEN2, &CSerialComDlg::OnPortOpen2)
	ON_BN_CLICKED(IDC_PORT_CLOSE2, &CSerialComDlg::OnPortClose2)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_Display4, &CSerialComDlg::OnBnClickedDisplay4)
	ON_BN_CLICKED(IDC_PORT_OPEN3, &CSerialComDlg::OnPortOpen3)
	ON_BN_CLICKED(IDC_PORT_CLOSE3, &CSerialComDlg::OnPortClose3)
		ON_CBN_SELCHANGE(IDC_SERIAL_PORT3, &CSerialComDlg::OnCbnSelchangeSerialPort3)
		ON_CBN_SELCHANGE(IDC_BAUD_RATE3, &CSerialComDlg::OnCbnSelchangeBaudRate3)
		ON_BN_CLICKED(IDC_UseLoadCtrl, &CSerialComDlg::OnBnClickedUseloadctrl)
        ON_BN_CLICKED(IDC_AutoConnect, &CSerialComDlg::OnBnClickedAutoconnect)
		ON_BN_CLICKED(IDC_LoadControlSetting, &CSerialComDlg::OnBnClickedLoadcontrolsetting)
		ON_BN_CLICKED(IDC_VoltUp1, &CSerialComDlg::OnBnClickedVoltup1)
		ON_EN_CHANGE(IDC_PS_Volt, &CSerialComDlg::OnEnChangePsVolt)
		ON_BN_CLICKED(IDC_VoltUp2, &CSerialComDlg::OnBnClickedVoltup2)
		ON_BN_CLICKED(IDC_VoltUp3, &CSerialComDlg::OnBnClickedVoltup3)
		ON_BN_CLICKED(IDC_VoltUp4, &CSerialComDlg::OnBnClickedVoltup4)
		ON_BN_CLICKED(IDC_VoltDn1, &CSerialComDlg::OnBnClickedVoltdn1)
		ON_BN_CLICKED(IDC_VoltDn2, &CSerialComDlg::OnBnClickedVoltdn2)
		ON_BN_CLICKED(IDC_VoltDn3, &CSerialComDlg::OnBnClickedVoltdn3)
		ON_BN_CLICKED(IDC_VoltDn4, &CSerialComDlg::OnBnClickedVoltdn4)
		ON_BN_CLICKED(IDC_ReqNoLoad, &CSerialComDlg::OnBnClickedReqnoload)
		ON_BN_CLICKED(IDC_INC_Gain, &CSerialComDlg::OnBnClickedIncGain)
		ON_BN_CLICKED(IDC_DEC_Gain, &CSerialComDlg::OnBnClickedDecGain)
		ON_EN_CHANGE(IDC_PS_Rate, &CSerialComDlg::OnEnChangePsRate)
		ON_EN_CHANGE(IDC_LoadCtrl_TargetTorque, &CSerialComDlg::OnEnChangeLoadctrlTargettorque)
		ON_BN_CLICKED(IDC_TorqueUp1, &CSerialComDlg::OnBnClickedTorqueup1)
		ON_BN_CLICKED(IDC_TorqueUp2, &CSerialComDlg::OnBnClickedTorqueup2)
		ON_BN_CLICKED(IDC_TorqueUp3, &CSerialComDlg::OnBnClickedTorqueup3)
		ON_BN_CLICKED(IDC_TorqueDn1, &CSerialComDlg::OnBnClickedTorquedn1)
		ON_BN_CLICKED(IDC_TorqueDn2, &CSerialComDlg::OnBnClickedTorquedn2)
		ON_BN_CLICKED(IDC_TorqueDn3, &CSerialComDlg::OnBnClickedTorquedn3)
		ON_BN_CLICKED(IDC_TorqueDn4, &CSerialComDlg::OnBnClickedTorquedn4)
		ON_BN_CLICKED(IDC_VoltDn5, &CSerialComDlg::OnBnClickedVoltdn5)
		ON_EN_CHANGE(IDC_PS_Rate2, &CSerialComDlg::OnEnChangePsRate2)
		ON_BN_CLICKED(IDC_INC_Gain2, &CSerialComDlg::OnBnClickedIncGain2)
		ON_BN_CLICKED(IDC_DEC_Gain2, &CSerialComDlg::OnBnClickedDecGain2)
		ON_BN_CLICKED(IDC_SaveLoadSetting, &CSerialComDlg::OnBnClickedSaveloadsetting)
		ON_BN_CLICKED(IDC_INC_Gain5, &CSerialComDlg::OnBnClickedIncGain5)
		ON_BN_CLICKED(IDC_INC_Gain3, &CSerialComDlg::OnBnClickedIncGain3)
		ON_BN_CLICKED(IDC_DEC_Gain5, &CSerialComDlg::OnBnClickedDecGain5)
		ON_BN_CLICKED(IDC_DEC_Gain3, &CSerialComDlg::OnBnClickedDecGain3)
		ON_BN_CLICKED(IDC_INC_Gain6, &CSerialComDlg::OnBnClickedIncGain6)
		ON_BN_CLICKED(IDC_INC_Gain4, &CSerialComDlg::OnBnClickedIncGain4)
		ON_BN_CLICKED(IDC_DEC_Gain6, &CSerialComDlg::OnBnClickedDecGain6)
		ON_BN_CLICKED(IDC_DEC_Gain4, &CSerialComDlg::OnBnClickedDecGain4)
		ON_BN_CLICKED(IDC_HIDE_SIDEMENU, &CSerialComDlg::OnBnClickedHideSidemenu)
		ON_BN_CLICKED(IDC_ShowMathMenu, &CSerialComDlg::OnBnClickedShowmathmenu)
		ON_BN_CLICKED(IDC_HideMathMenu, &CSerialComDlg::OnBnClickedHidemathmenu)
		END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialComDlg message handlers
CFile Savefile;
CFileException ex;

BOOL CSerialComDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	hCommWnd = m_hWnd;	    
    GetDlgItem(IDC_LogList)->ShowWindow(TRUE);
	m_ctrlLogList.InsertColumn(0, _T("로그"), LVCFMT_LEFT, 400); // 단일 컬럼
	InitListCtrl();

	m_iGraphInfoMinX = (m_iDrawGraphRangeX[1] + GRAPH_GAP_X);  //1685
	m_iGraphInfoMaxX = (m_iGraphInfoMinX + 155);
	CheckDlgButton(IDC_GraphUpdate, m_bUseGraphUpdate);

	for (int i = 0; i < MAX_GRAPH_COUNT - 1; i++) { m_bAdjustLegendY[i] = FALSE; }
	
	OnSetfocusSerialPort();
	OnSetfocusSerialPort2();
	OnSetfocusSerialPort3();
	LoadComPortList();
	GetLastBaudRate();
	//GetLastBaudRate2();
	m_iBaudRate2 = 7;
	//GetLastBaudRate3();
	m_iBaudRate3 = 6;
	m_cBaudRate2.EnableWindow(FALSE);
	m_cBaudRate3.EnableWindow(FALSE);
	GetSavePath();
	GetSendConfig_XInput();
	InitHistoryData();	

	CClientDC dc(this);
	m_MemDC.CreateCompatibleDC(&dc); 
	m_Bitmap.CreateCompatibleBitmap(&dc, 5120, 3200);  //GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)
	m_pOldBitmap = m_MemDC.SelectObject(&m_Bitmap);
 	
	m_MemDC_Range.CreateCompatibleDC(&dc);
	m_Bitmap_Range.CreateCompatibleBitmap(&dc, 120, 120);
	m_pOldBitmap_Range = m_MemDC_Range.SelectObject(&m_Bitmap_Range);

	m_MemDC_Position.CreateCompatibleDC(&dc);
	m_Bitmap_Position.CreateCompatibleBitmap(&dc, 5120, 3200);  
	m_pOldBitmap_Position = m_MemDC_Position.SelectObject(&m_Bitmap_Position);

	m_MemDC_Position_Init.CreateCompatibleDC(&dc);
	m_Bitmap_Position_Init.CreateCompatibleBitmap(&dc, 5120, 3200);  
	m_pOldBitmap_Position_Init = m_MemDC_Position_Init.SelectObject(&m_Bitmap_Position_Init);
	
	m_MemDC_Timeline.CreateCompatibleDC(&dc);
	m_Bitmap_Timeline.CreateCompatibleBitmap(&dc, 5120, GRAPH_TIMELINE_Y1);  
	m_pOldBitmap_Timeline = m_MemDC_Timeline.SelectObject(&m_Bitmap_Timeline);

	m_MemDC_Select.CreateCompatibleDC(&dc);
	m_Bitmap_Select.CreateCompatibleBitmap(&dc, 380, 1200);  
	m_pOldBitmap_Select = m_MemDC_Select.SelectObject(&m_Bitmap_Select);

	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 50, 255));
	m_MemDC_Range.SelectObject(&brush);
	m_MemDC_Range.Rectangle(-10, -10, 130, 130);
		
	m_iGraphInfoMinX = (m_iDrawGraphRangeX[1] + 5); 
	m_iGraphInfoMaxX = (m_iGraphInfoMinX + 155);

	GetLastModel();
	LoadCommonConfiguration();
    LoadHeaderConfiguration();
    GetRegGrphHeight();

	OnBnClickedClearwatchlist();
	OnBnClickedClearmessage();

    GetDlgItem(IDC_FilterMessage)->ShowWindow(m_bShowFilterLogControl);
	if (m_bShowDataControl) {
		GetDlgItem(IDC_ShowMathMenu)->ShowWindow(m_bShowMathControl == FALSE);
		GetDlgItem(IDC_HideMathMenu)->ShowWindow(m_bShowMathControl == TRUE);
	}

	m_ctrlProgReading.SetBkColor(RGB(0xff, 0xff, 0xff));
	m_ctrlProgReading.SetBarColor(RGB(0x00, 0x10, 0x00));

	DragAcceptFiles(TRUE);
	ShowWindow(SW_SHOWMAXIMIZED);
	SetWindowText(TITLE);
	UDF;
	SetTimer(TIMER_GRAPH_UPDATE, 70, NULL);
	SetTimer(TIMER_LOAD_DEFALUT_DISPLAY_SIZE, 100, NULL);	  // Load Default Graph Size
	SetTimer(TIMER_GRAPHLOG_UPDATE, 20, NULL);
	SetTimer(TIMER_WHEEL_ACC_CHECK, 1000, NULL);
	SetTimer(TIMER_XINPUT_CHECK, 1000, NULL);
	SetTimer(TIMER_DLG_INIT,400, NULL);
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialComDlg::LoadDefaultGraphNumber() {
	CString strAddress, strItem, strValue;
	strAddress.Format("Graph");
	strItem.Format("Number_Graph");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);

	//if (strValue != "")  m_iSelectGraphCount = atoi(strValue) - 1; else  m_iSelectGraphCount = 3;
	m_iSelectGraphCount = MAX_GRAPH_COUNT - 1;
	m_iGraphCount = 0;
	ControlView();
}

void CSerialComDlg::OnPaint() {
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		DrawInit();
		ShowDataMathControl();
		m_bReqGraphUpdate = TRUE;
	}
}

HCURSOR CSerialComDlg::OnQueryDragIcon() {
	return (HCURSOR)m_hIcon;
}

CString CSerialComDlg::byNameComPort(const CString& str) {
	CString PortName;
	int iPort = String2Num(str);

	switch (iPort) {
	case 1:		PortName.Format("COM1"); 			break;
	case 2:		PortName.Format("COM2");			break;
	case 3:		PortName.Format("COM3"); 			break;
	case 4:		PortName.Format("COM4");			break;
	case 5:		PortName.Format("\\\\.\\COM5"); 	break;
	case 6:		PortName.Format("\\\\.\\COM6");		break;
	case 7:		PortName.Format("\\\\.\\COM7"); 	break;
	case 8:		PortName.Format("\\\\.\\COM8");		break;
	case 9:		PortName.Format("\\\\.\\COM9"); 	break;
	case 10:	PortName.Format("\\\\.\\COM10");	break;
	case 11:	PortName.Format("\\\\.\\COM11"); 	break;
	case 12:	PortName.Format("\\\\.\\COM12");	break;
	case 13:	PortName.Format("\\\\.\\COM13"); 	break;
	case 14:	PortName.Format("\\\\.\\COM14");	break;
	case 15:	PortName.Format("\\\\.\\COM15"); 	break;
	case 16:	PortName.Format("\\\\.\\COM16");	break;
	case 17:	PortName.Format("\\\\.\\COM17"); 	break;
	case 18:	PortName.Format("\\\\.\\COM18");	break;
	case 19:	PortName.Format("\\\\.\\COM19"); 	break;
	case 20:	PortName.Format("\\\\.\\COM20");	break;
	case 21:	PortName.Format("\\\\.\\COM21"); 	break;
	case 22:	PortName.Format("\\\\.\\COM22");	break;
	case 23:	PortName.Format("\\\\.\\COM23"); 	break;
	case 24:	PortName.Format("\\\\.\\COM24");	break;
	case 25:	PortName.Format("\\\\.\\COM25"); 	break;
	case 26:	PortName.Format("\\\\.\\COM26");	break;
	case 27:	PortName.Format("\\\\.\\COM27"); 	break;
	case 28:	PortName.Format("\\\\.\\COM28");	break;
	case 29:	PortName.Format("\\\\.\\COM29"); 	break;
	case 30:	PortName.Format("\\\\.\\COM30");	break;
	case 31:	PortName.Format("\\\\.\\COM31"); 	break;
	case 32:	PortName.Format("\\\\.\\COM32");	break;
	case 33:	PortName.Format("\\\\.\\COM33"); 	break;
	case 34:	PortName.Format("\\\\.\\COM34");	break;
	case 35:	PortName.Format("\\\\.\\COM35"); 	break;
	case 36:	PortName.Format("\\\\.\\COM36");	break;
	case 37:	PortName.Format("\\\\.\\COM37"); 	break;
	case 38:	PortName.Format("\\\\.\\COM38");	break;
	case 39:	PortName.Format("\\\\.\\COM39"); 	break;
	case 40:	PortName.Format("\\\\.\\COM40");	break;
	case 41:	PortName.Format("\\\\.\\COM41"); 	break;
	case 42:	PortName.Format("\\\\.\\COM42");	break;
	case 43:	PortName.Format("\\\\.\\COM43"); 	break;
	case 44:	PortName.Format("\\\\.\\COM44");	break;
	case 45:	PortName.Format("\\\\.\\COM45"); 	break;
	case 46:	PortName.Format("\\\\.\\COM46");	break;
	case 47:	PortName.Format("\\\\.\\COM47"); 	break;
	case 48:	PortName.Format("\\\\.\\COM48");	break;
	case 49:	PortName.Format("\\\\.\\COM49"); 	break;
	case 50:	PortName.Format("\\\\.\\COM50");	break;
	case 51:	PortName.Format("\\\\.\\COM51"); 	break;
	case 52:	PortName.Format("\\\\.\\COM52");	break;
	case 53:	PortName.Format("\\\\.\\COM53"); 	break;
	case 54:	PortName.Format("\\\\.\\COM54");	break;
	case 55:	PortName.Format("\\\\.\\COM55"); 	break;
	case 56:	PortName.Format("\\\\.\\COM56");	break;
	case 57:	PortName.Format("\\\\.\\COM57"); 	break;
	case 58:	PortName.Format("\\\\.\\COM58");	break;
	case 59:	PortName.Format("\\\\.\\COM59"); 	break;
	case 60:	PortName.Format("\\\\.\\COM60");	break;
	case 61:	PortName.Format("\\\\.\\COM61"); 	break;
	case 62:	PortName.Format("\\\\.\\COM62");	break;
	case 63:	PortName.Format("\\\\.\\COM63"); 	break;
	case 64:	PortName.Format("\\\\.\\COM64");	break;
	case 65:	PortName.Format("\\\\.\\COM65"); 	break;
	case 66:	PortName.Format("\\\\.\\COM66");	break;
	case 67:	PortName.Format("\\\\.\\COM67"); 	break;
	case 68:	PortName.Format("\\\\.\\COM68");	break;
	case 69:	PortName.Format("\\\\.\\COM69"); 	break;
	case 70:	PortName.Format("\\\\.\\COM70");	break;
	case 71:	PortName.Format("\\\\.\\COM71"); 	break;
	case 72:	PortName.Format("\\\\.\\COM72");	break;
	case 73:	PortName.Format("\\\\.\\COM73"); 	break;
	case 74:	PortName.Format("\\\\.\\COM74");	break;
	case 75:	PortName.Format("\\\\.\\COM75"); 	break;
	case 76:	PortName.Format("\\\\.\\COM76");	break;
	case 77:	PortName.Format("\\\\.\\COM77"); 	break;
	case 78:	PortName.Format("\\\\.\\COM78");	break;
	case 79:	PortName.Format("\\\\.\\COM79"); 	break;
	case 80:	PortName.Format("\\\\.\\COM80");	break;
	case 81:	PortName.Format("\\\\.\\COM81"); 	break;
	case 82:	PortName.Format("\\\\.\\COM82");	break;
	case 83:	PortName.Format("\\\\.\\COM83"); 	break;
	case 84:	PortName.Format("\\\\.\\COM84");	break;
	case 85:	PortName.Format("\\\\.\\COM85"); 	break;
	case 86:	PortName.Format("\\\\.\\COM86");	break;
	case 87:	PortName.Format("\\\\.\\COM87"); 	break;
	case 88:	PortName.Format("\\\\.\\COM88");	break;
	case 89:	PortName.Format("\\\\.\\COM89"); 	break;
	case 90:	PortName.Format("\\\\.\\COM90");	break;
	case 91:	PortName.Format("\\\\.\\COM91"); 	break;
	case 92:	PortName.Format("\\\\.\\COM92");	break;
	case 93:	PortName.Format("\\\\.\\COM93"); 	break;
	case 94:	PortName.Format("\\\\.\\COM94");	break;
	case 95:	PortName.Format("\\\\.\\COM95"); 	break;
	case 96:	PortName.Format("\\\\.\\COM96");	break;
	case 97:	PortName.Format("\\\\.\\COM97"); 	break;
	case 98:	PortName.Format("\\\\.\\COM98");	break;
	case 99:	PortName.Format("\\\\.\\COM99"); 	break;
	case 100:	PortName.Format("\\\\.\\COM100");	break;
	default:
		if (iPort != 0)
			PortName.Format("\\\\.\\COM%d", iPort);
		else
			PortName.Format("");
		break;
	}

	return PortName;
}

int CSerialComDlg::String2Num(const CString& str)
{
	char *buf = LPSTR(LPCTSTR(str));
	int iSize = str.GetLength();
	int iNum = 0;

	if (iSize < 10) {
		for (int i = 0; i < iSize; i++) {
			if (buf[i] >= '0' && buf[i] <= '9')
				iNum = iNum * 10 + buf[i] - '0';
		}
	}
	return iNum;
}

void CSerialComDlg::OnPortOpen1() {
	OnPortClose1();

	UpdateData(TRUE);
	CString PortName;

	if (m_ComuPort.m_bConnected == TRUE) {
		OnPortClose1();
		Wait(0);
        Wait(0);
        Wait(0);
	}

	m_iSerialPort = m_cSerialPort.GetCurSel();
	if (m_iSerialPort == -1) {
		PortName.Format("사용 가능한 Port가 없습니다. (No Available Port)");
		AfxMessageBox(PortName);
		return;
	}
	m_cSerialPort.GetLBText(m_iSerialPort, m_strSerial);
	m_strSerial.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial));
	if (m_ComuPort.OpenPort(byNameComPort(m_strSerial), byIndexBaud(m_iBaudRate), 8, 0, 0) == TRUE) {
		strPort1name = m_ComuPort.m_sPortName;
		CString strCom;
		strCom.Format("COM%d", String2Value(strPort1name));
		SetLastComPort(strCom);
		SetLastBaudRate(m_iBaudRate);
		m_bUpdateList = TRUE;
		m_iProgramMode = DATA_ACQUISITION_MODE;
		ProgramModeUpdate();
	}
}


void CSerialComDlg::OnPortOpen2(){
	OnPortClose2();

	UpdateData(TRUE);
	CString PortName2;

	if (m_ComuPort2.m_bConnected == TRUE) {
		OnPortClose2();
        Wait(0);
        Wait(0);
        Wait(0);
	}

	m_iSerialPort2 = m_cSerialPort2.GetCurSel();
	if (m_iSerialPort2 == -1) {
        PortName2.Format("사용 가능한 Port가 없습니다. (No Available Port)");
		AfxMessageBox(PortName2);
		return;
	}
	m_cSerialPort2.GetLBText(m_iSerialPort2, m_strSerial2);
	m_strSerial2.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial2));
	if (m_ComuPort2.OpenPort(byNameComPort(m_strSerial2), byIndexBaud(m_iBaudRate2), 8, 0, 0) == TRUE) {
		strPort2name = m_ComuPort2.m_sPortName;
		CString strCom;
		strCom.Format("COM%d", String2Value(strPort2name));
		SetLastComPort2(strCom);
		SetLastBaudRate2(m_iBaudRate2);
		m_bUpdateList = TRUE;
		m_iProgramMode = DATA_ACQUISITION_MODE;
		ProgramModeUpdate();
	}
}


void CSerialComDlg::OnPortOpen3() {
	OnPortClose3();

	UpdateData(TRUE);
	CString PortName3;

	if (m_ComuPort3.m_bConnected == TRUE) {
		OnPortClose3();
		Wait(0);
		Wait(0);
		Wait(0);
	}

	m_iSerialPort3 = m_cSerialPort3.GetCurSel();
	if (m_iSerialPort3 == -1) {
		PortName3.Format("사용 가능한 Port가 없습니다. (No Available Port)");
		AfxMessageBox(PortName3);
		return;
	}
	m_cSerialPort3.GetLBText(m_iSerialPort3, m_strSerial3);
	m_strSerial3.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial3));
	if (m_ComuPort3.OpenPort(byNameComPort(m_strSerial3), byIndexBaud(m_iBaudRate3), 8, 0, 0) == TRUE) {
		strPort3name = m_ComuPort3.m_sPortName;
		CString strCom;
		strCom.Format("COM%d", String2Value(strPort3name));
		SetLastComPort3(strCom);
		SetLastBaudRate3(m_iBaudRate3);
		m_bUpdateList = TRUE;
		m_iProgramMode = DATA_ACQUISITION_MODE;
		ProgramModeUpdate();
	}
}

void CSerialComDlg::OnSelchangeSerialPort() {
	UpdateData(TRUE);
	m_iSerialPort = m_cSerialPort.GetCurSel();
}

void CSerialComDlg::OnCbnSelchangeSerialPort2()
{
	UpdateData(TRUE);
	m_iSerialPort2 = m_cSerialPort2.GetCurSel();
}

void CSerialComDlg::OnCbnSelchangeSerialPort3()
{
	UpdateData(TRUE);
	m_iSerialPort3 = m_cSerialPort3.GetCurSel();
}


void CSerialComDlg::OnSelchangeBaudRate() {
	UpdateData(TRUE);
	m_iBaudRate = m_cBaudRate.GetCurSel();
	if (m_ComuPort.m_bConnected == TRUE) {
		m_ComuPort.ClosePort();
		OnPortOpen1();
	}
}

void CSerialComDlg::OnCbnSelchangeBaudRate2()
{
	UpdateData(TRUE);
	m_iBaudRate2 = m_cBaudRate2.GetCurSel();
	if (m_ComuPort2.m_bConnected == TRUE) {
		m_ComuPort2.ClosePort();
		OnPortOpen2();
	}
}

void CSerialComDlg::OnCbnSelchangeBaudRate3()
{
	UpdateData(TRUE);
	m_iBaudRate3 = m_cBaudRate3.GetCurSel();
	if (m_ComuPort3.m_bConnected == TRUE) {
		m_ComuPort3.ClosePort();
		OnPortOpen3();
	}
}

DWORD CSerialComDlg::byIndexBaud(int xBaud) {
	DWORD dwBaud = CBR_115200;
	switch (xBaud) {
		case 0:		dwBaud = CBR_4800;		break;
		case 1:		dwBaud = CBR_9600;		break;
		case 2:		dwBaud = CBR_14400;		break;
		case 3:		dwBaud = CBR_19200;		break;
		case 4:		dwBaud = CBR_38400;		break;
		case 5:		dwBaud = CBR_56000;		break;
		case 6:		dwBaud = CBR_57600;		break;
		case 7:		dwBaud = CBR_115200;	break;
		case 8:     dwBaud = 230400;	    break;
		case 9:		dwBaud = 460800;        break;
	}
	return dwBaud;
}

BYTE CSerialComDlg::byIndexData(int xData) {
	BYTE byData = 8;
	switch (xData) {
		case 0:	byData = 5;			break;
		case 1:	byData = 6;			break;
		case 2:	byData = 7;			break;
		case 3:	byData = 8;			break;
	}
	return byData;
}

BYTE CSerialComDlg::byIndexStop(int xStop) {
	BYTE byStop;
	if (xStop == 0) {
		byStop = ONESTOPBIT;
	}
	else {
		byStop = TWOSTOPBITS;
	}
	return byStop;
}

BYTE CSerialComDlg::byIndexParity(int xParity) {
	BYTE byParity = NOPARITY;
	switch (xParity) {
	case 0:	byParity = NOPARITY;	break;
	case 1:	byParity = ODDPARITY;	break;
	case 2:	byParity = EVENPARITY;	break;
	}

	return byParity;
}

BYTE CSerialComDlg::byCode2AsciiValue(char cData) {

	//이 함수는 char문자를 hex값으로 변경해 주는 함수 입니다.
	BYTE byAsciiValue;
	if (('0' <= cData) && (cData <= '9')) {
		byAsciiValue = cData - '0';
	}
	else if (('A' <= cData) && (cData <= 'F')) {
		byAsciiValue = (cData - 'A') + 10;
	}
	else if (('a' <= cData) && (cData <= 'f')) {
		byAsciiValue = (cData - 'a') + 10;
	}
	else {
		byAsciiValue = 0;
	}
	return byAsciiValue;
}

void CSerialComDlg::Wait(DWORD dwMillisecond) {
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	do {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (GetTickCount() - dwStart < dwMillisecond);
}

void CSerialComDlg::OnTimer(UINT_PTR nIDEvent) {
	CDialog::OnTimer(nIDEvent);	
	static HWND wndDlg;
	switch (nIDEvent) {
		case TIMER_GRAPH_UPDATE: 			
			CheckTimer();
            CheckLogUpdate();
			if (m_bUseGraphUpdate && m_bReqGraphUpdate) UpdateGraphData();
			break;

		case TIMER_LOAD_DEFALUT_DISPLAY_SIZE: 
			KillTimer(TIMER_LOAD_DEFALUT_DISPLAY_SIZE); 
			LoadDefaultGraphNumber(); 
			break;

		case TIMER_CHECK_STATISTICS_DLG:
			if (m_pStatistics == NULL || m_pStatistics->m_hWnd == NULL) {
				KillTimer(TIMER_CHECK_STATISTICS_DLG);
				m_bUseStatistics = FALSE;
				m_bReqGraphUpdate = TRUE;
			}
			break;

		case TIMER_AUTOCLOSE_MESSAGEBOX:
			KillTimer(TIMER_AUTOCLOSE_MESSAGEBOX);
			wndDlg = ::GetLastActivePopup(m_hWnd);
			if (wndDlg && wndDlg != m_hWnd)
			{
				char buffer[256] = { 0 };
				::GetClassName(wndDlg, buffer, 256);
				if (CString("#32770") == buffer) //메시지 박스는 분명히 다이얼로그이며 클래스명이 #32770
				{
					::EndDialog(wndDlg, IDOK);
				}
			}
			break;		

		case TIMER_REQ_CHANGE_DATASIZE:
			m_bForcedChangeDataSize = TRUE;
			OnEnChangeDisplaydatasize();
			break;

		case TIMER_GRAPHLOG_UPDATE:
			UpdateGraphLogData(0);
			UpdateTextLogData(0);
			break;

		case TIMER_WHEEL_ACC_CHECK:
			CheckWheelAcc();
			break;

		case TIMER_UPDATE_LOG:
			KillTimer(TIMER_UPDATE_LOG);
			UpdateLogMessage();
			break;

		case TIMER_XINPUT_CHECK:
			if (m_bUse_Xinput) {
				Get_XinputData();
			}
			break;

		case TIMER_DLG_INIT:
			KillTimer(TIMER_DLG_INIT);
			GetBackgroundColor();
			break;

		default:
			break;
	}
}


void CSerialComDlg::GetBackgroundColor() {
	CClientDC dc(this);
	m_bgColor = GetPixel(dc, 442, 57);
    if (m_bgColor == 0) {
        m_bgColor = RGB(255, 255, 255);
    }
	m_bFastDraw = TRUE;

	CBrush brush;
	brush.CreateSolidBrush(RGB(100, 100, 100));  // Not Connect
	m_MemDC_Select.SelectObject(&brush);
	m_MemDC_Select.Rectangle(0, 1000, 50, 1100);
	
	brush.CreateSolidBrush(RGB(200, 150, 150));
	m_MemDC_Select.SelectObject(&brush);
	m_MemDC_Select.Rectangle(50, 1000, 100, 1100);

	brush.CreateSolidBrush(RGB(250, 30, 30));
	m_MemDC_Select.SelectObject(&brush);
	m_MemDC_Select.Rectangle(100, 1000, 150, 1100);

	brush.CreateSolidBrush(RGB(30, 30, 250));
	m_MemDC_Select.SelectObject(&brush);
	m_MemDC_Select.Rectangle(150, 1000, 200, 1100);
}

long CSerialComDlg::OnCommunication(WPARAM wParam, LPARAM lParam) {
    //UpdateData(TRUE);//받는 데이터 타입을 알기 위해
    BYTE aByte; //데이터를 저장할 변수 

    int iSize = m_ComuPort.m_QueueRead.GetSize(); //포트로 들어온 데이터 갯수

    for (int i = 0; i < iSize; i++) {//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
        m_ComuPort.m_QueueRead.GetByte(&aByte);//큐에서 데이터 한개를 읽어옴
		aByte = 1;
    }
    return 1;
}

long CSerialComDlg::OnCommunication2(WPARAM wParam, LPARAM lParam) {
	//UpdateData(TRUE);//받는 데이터 타입을 알기 위해
	BYTE aByte; //데이터를 저장할 변수 
	static int iSeq = 0;
	static double val = 0.;
	static bool result = false;
	int iSize = m_ComuPort2.m_QueueRead.GetSize(); //포트로 들어온 데이터 갯수
	//SetCommState에 설정된 내용 때문에 거의 8개씩 들어옴

	if (m_req_Load_Packet_reset) {
		iSeq = 0;
		m_req_Load_Packet_reset = false;
	}

	if (m_LoadPacket.req_ack) {
		if (GetTickCount() - m_LoadPacket.tick_send > 100) {
			iSeq = 0;
		}
	}

	for (int i = 0; i < iSize; i++) {//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
		m_ComuPort2.m_QueueRead.GetByte(&aByte);//큐에서 데이터 한개를 읽어옴
		int iType = 0;
		if (aByte >= '0' && aByte <= '9') { iType = 1; }
		else if(aByte >= '.') { iType = 2; }
		
		if (m_LoadPacket.req_ack) {
			switch (m_LoadPacket.id) {

			case PS_GET_VOLT_SET:
				switch (iSeq++) { // 00.00V
				case 0:
					if (iType == 1) { result = true; }
					else { result = false; }
					val = 10. * (aByte - '0');	break;

				case 1: if (iType != 1) { result = false; }
						val += (aByte - '0');	break;

				case 2:
					if (iType != 2) { result = false; }	break;

				case 3: if (iType != 1) { result = false; }
						val += 0.1 * (aByte - '0');	break;

				case 4: if (iType != 1) { result = false; }
						val += 0.01 * (aByte - '0');
						iSeq = 0;
						if (result) {
							m_LoadPacket.volt[1] = val;
						}
						m_LoadPacket.req_ack = false;
						break;
				}
				break;

			case PS_GET_CUR_OUT:
				switch (iSeq++) { // 0.000A
				case 0:
					if (iType == 1) { result = true; }
					else { result = false; }
					val = aByte - '0';	break;
				case 1:
					if (iType != 2) { result = false; }	break;
				case 2: if (iType != 1) { result = false; }
						val += 0.1 * (aByte - '0');	break;
				case 3: if (iType != 1) { result = false; }
						val += 0.01 * (aByte - '0');	break;
				case 4: if (iType != 1) { result = false; }
						val += 0.001 * (aByte - '0');
						iSeq = 0;
						if (result) {
							m_LoadPacket.current[0] = val;
							CString str;
							str.Format("%5.3f", val);
							SetDlgItemText(IDC_PS_Current, str);
						}
						if (IsDlgButtonChecked(IDC_UseLoadCtrl)) {
							AutoControlVoltage();
						}
						m_LoadPacket.req_ack = false;
						break;
				}
				break;

			case PS_GET_CUR_SET:
				switch (iSeq++) { // 0.000A
				case 0: 
					if (iType == 1) { result = true; }	else { result = false;}
					val = aByte - '0';	break;
				case 1: 
					if (iType != 2) { result = false; }	break;
				case 2: if (iType != 1) { result = false; }	
					val += 0.1 * (aByte - '0');	break;
				case 3: if (iType != 1) { result = false; }	
					val += 0.01 * (aByte - '0');	break;
				case 4: if (iType != 1) { result = false; }
						val += 0.001 * (aByte - '0');	
						iSeq = 0;
						if (result) {
							m_LoadPacket.current[0] = val;
						}
						m_LoadPacket.req_ack = false;
						break;
				}
				break;

			case PS_GET_VOLT_OUT:
				switch (iSeq++) { // 00.00V
				case 0:
					if (iType == 1) { result = true; }
					else { result = false; }
					val = 10. * (aByte - '0');	break;

				case 1: if (iType != 1) { result = false; }
						val += (aByte - '0');	break;

				case 2:
					if (iType != 2) { result = false; }	break;

				case 3: if (iType != 1) { result = false; }
						val += 0.1 * (aByte - '0');	break;

				case 4: if (iType != 1) { result = false; }
						val += 0.01 * (aByte - '0');
						iSeq = 0;
						if (result) {
							m_LoadPacket.volt[0] = val;
						}
						m_LoadPacket.req_ack = false;
						break;
				}
				break;

			case PS_GET_INFO:
				// 왠만하면 사용하지 말자 
				// size불명확함
				break;

			case PS_GET_STATUS:
				m_LoadPacket.status = aByte;
				m_LoadPacket.req_ack = false;
				break;

			default: 
				iSeq = 0;
				break;
			}
		}		
	}
	return 1;
}

long CSerialComDlg::OnCommunication3(WPARAM wParam, LPARAM lParam) {
    //UpdateData(TRUE);//받는 데이터 타입을 알기 위해
    BYTE aByte; //데이터를 저장할 변수     
    static int iSeq = 0;    
	static int iSubSeq = 0;
	static bool bMinus = false;
	static bool bPeriod = false;
	static int iVal = 0;
	static int iSubDigit = 1;
	static double fTorque = 0., fPower = 0.;
	static int iRpm = 0;
	static TORQUE_DATA_T data;

    int iSize = m_ComuPort3.m_QueueRead.GetSize(); //포트로 들어온 데이터 갯수
	for (int i = 0; i < iSize; i++) {//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
		m_ComuPort3.m_QueueRead.GetByte(&aByte);//큐에서 데이터 한개를 읽어옴

		switch (iSeq) {
		case 0: // Header check
			switch (iSubSeq) {
			case 0:
				if (aByte == 'G') { iSubSeq = 1; }
				break;				

			case 1:
				if (aByte == 'S') {	iSubSeq = 2; }
				else { iSubSeq = 0; }
				break;

			case 2: // delimiter
				if (aByte == ',') { iSeq = 1; iSubSeq = 0; }
				else { iSubSeq = 0; }
				break;
			}
			break;
			
		case 1:  // Communication mode
			switch (iSubSeq) {
			case 0: iSubSeq++;	break; 
			case 1: iSubSeq++;	break;
			case 2: iSubSeq++;	break;
			case 3: iSubSeq++;	break;
			case 4: iSubSeq++;	break;
			case 5: iSubSeq++;	break;
			case 6: iSubSeq++;	break;
			case 7: iSubSeq++;	break;
			case 8: iSubSeq++;	break;
			case 9: // delimiter check
				if (aByte == ',') { iSeq = 2; iSubSeq = 0; }
				else { iSeq = 0; iSubSeq = 0; }
				break;
			}
			break;

		case 2: // Torque  [Nm]
			switch (iSubSeq) {
			case 0: // Sign
				if (aByte == '+') { bMinus = false; }
				else if (aByte == '-') { bMinus = true; }
				else { iSeq = 0; iSubSeq = 0; break; }
				iVal = 0;
				iSubDigit = 1;
				bPeriod = false;
				iSubSeq++; 
				break; 

			case 1:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;	
				break;

			case 2:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 3:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 4:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 5:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 6:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 7:
				if (aByte == ',') {
					if (bMinus) { fTorque = -iVal; }
					else { fTorque = iVal; }
					if (iSubDigit != 0) { fTorque /= iSubDigit; }
					iSeq = 3;  iSubSeq = 0;
					data.fTorque = fTorque;					
				}
				else { iSeq = 0; iSubSeq = 0; }
				break;
			}
			break;

		case 3: // Rotation Speed [rpm]
			switch (iSubSeq) {
			case 0: // Sign
				if (aByte != '+') { iSeq = 0; iSubSeq = 0; break; }	// '+' only				
				iVal = 0;								// No float point
				iSubSeq++; 
				break;

			case 1:
				if (aByte >= '0' && aByte <= '9') {	iVal = iVal * 10 + aByte - '0'; }
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;	
				break;

			case 2:
				if (aByte >= '0' && aByte <= '9') { iVal = iVal * 10 + aByte - '0'; }
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 3:
				if (aByte >= '0' && aByte <= '9') { iVal = iVal * 10 + aByte - '0'; }
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 4:
				if (aByte >= '0' && aByte <= '9') { iVal = iVal * 10 + aByte - '0'; }
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 5:
				if (aByte >= '0' && aByte <= '9') { iVal = iVal * 10 + aByte - '0'; }
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 6:
				if (aByte == '.') { data.iRpm = iVal; }
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;	
				
				break; 

			case 7:
				if (aByte != ',') { iSeq = 0; iSubSeq = 0; }
				else { iSeq = 4;  iSubSeq = 0; }
				break;
			}
			break;

		case 4: // Power [w]
			switch (iSubSeq) {
			case 0: // Sign
				if (aByte == '+') { bMinus = false; }
				else if (aByte == '-') { bMinus = true; }
				else { iSeq = 0; iSubSeq = 0; break; }
				iVal = 0;
				iSubDigit = 1;
				bPeriod = false;
				iSubSeq++; 
				break; 				

			case 1:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;	
				break; 
				

			case 2:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 3:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 4:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 5:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }
				iSubSeq++;
				break;

			case 6:
				if (aByte == '.') { bPeriod = true;	iSubDigit = 1; }
				else if (aByte >= '0' && aByte <= '9') {
					iVal = iVal * 10 + aByte - '0';
					if (bPeriod) { iSubDigit *= 10; }
				}
				else { iSeq = 0; iSubSeq = 0;	break; }

				if (bMinus) { fPower = -iVal; }
				else { fPower = iVal; }
				if (iSubDigit != 0) { fPower /= iSubDigit; }

				data.fPower = fPower;
				PostProcessTorqueData(&data);                
				iSeq = 0; iSubSeq = 0;
				break;
			}
			break;
		}
	}
	return 1;
}

void CSerialComDlg::PostProcessTorqueData(TORQUE_DATA_T *pData) {
	CString str;
	static int LoadCommID = PS_REQ_VOLT_SET;
	m_bGetTorqueData = TRUE;

	if (pData->fTorque > 1000. || pData->fTorque < -1000.) {
		CString str;
		str.Format("%f ", pData->fTorque );
	}



	str.Format("%s", AddComma(pData->iRpm));
	SetDlgItemText(IDC_LoadCtrl_Speed, str);

	str.Format("%.2f", pData->fTorque);
	SetDlgItemText(IDC_LoadCtrl_CurTorque, str);

	if (m_iProgramMode == DATA_ACQUISITION_MODE) {
		AddLogData(pData);
	}

	// Check Load Control
	if (m_ComuPort2.m_bConnected && m_LoadPacket.bIsSetting == false) {
		if (m_LoadPacket.req_ack == false || (m_LoadPacket.req_ack && m_LoadPacket.tick_send - GetTickCount() > 100))  {
			while (1) {
				if (++LoadCommID > PS_GET_CUR_OUT) {
					LoadCommID = PS_REQ_VOLT_SET;
				}

				if (m_bLoadControlSetting) { break; }
				else if (LoadCommID == PS_GET_CUR_OUT || LoadCommID == PS_REQ_VOLT_SET) {
					Send_LoadPacket((POWERSUPLY_CMD_e)LoadCommID);
					break;
				}				
			}
		}
	}
}

void CSerialComDlg::AddLogData(TORQUE_DATA_T *data) {
	CString strLog, strData;
	CString str;

	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	int cur_time_sec = (cur_time.wHour % 24) * 3600 + cur_time.wMinute * 60 + cur_time.wSecond;
	int cur_time_msec = cur_time.wMilliseconds;

	{
		UART_DATA current_data = m_dummy_UARTData;
		
		current_data.fDataValue[0] = data->iRpm;
		current_data.fDataValue[1] = m_fTargetTorque;
		current_data.fDataValue[2] = data->fTorque;		
		current_data.fDataValue[3] = data->fPower;
		current_data.fDataValue[4] = m_LoadPacket.volt[1];
		current_data.fDataValue[5] = m_LoadPacket.current[0];	
		current_data.iRcvTime = cur_time_sec;
		current_data.iRcvTime_ms = cur_time_msec;
		
		current_data.iHistoryLogIndex = m_iLogIndex + 1;
		current_data.iRecvChannel = 0;
		current_data.Header = m_cPacketHead;

		if (m_iHistoryIndex == 0) {
			m_HistoryData[0] = current_data;
		}
		else {
			m_HistoryData.emplace_back(current_data);
		}
		// Generate Math Data From Data
		GenerateMathDataFromRcvData();
		m_iHistoryIndex++;
	}

	strData.Format("M %s %s %.3f %.3f %.2f %.3f", AddComma(data->iRpm), m_strTargetTorque, data->fTorque, data->fPower, m_LoadPacket.volt[1], m_LoadPacket.current[0]);
	
	strLog.Format("%02d:%02d:%02d.%03d] %s", cur_time_sec / 3600, (cur_time_sec % 3600) / 60, cur_time_sec % 60, cur_time_msec, strData);
	BOOL bReqFocedSave = IsDlgButtonChecked(IDC_SaveRcvDataAll);

	if (m_bUpdateList || bReqFocedSave) {
		int iHistoryIndex = m_iHistoryIndex;
		iHistoryIndex = max(0, m_iHistoryIndex - 1);

		m_iLogIndex++;
		m_arrLogData.emplace_back(strLog);

		m_iLogHistoryIndexTable.emplace_back(iHistoryIndex);
		m_iLogFilterIndexTable.emplace_back(m_iFilterIndex);
		if (m_bUseAutosave2_Time_LogCount) { m_iAutosave_LogCount++; }
	}

	m_bReqLogSizeUpdate = TRUE;
	if (m_iProgramMode == DATA_ACQUISITION_MODE) {
		m_strIndexNumber.Format("%d", m_iHistoryIndex - 1);
		m_iCurrentIndex = m_iHistoryIndex - 1;
		if (m_bFitToScreen) {
			Req_Draw_ScreenFit();
		}
		m_bReqGraphUpdate = TRUE;	
	}
}


bool CSerialComDlg::CheckNumStr(CString *pStr) {
	bool update = false;
	double f1 = atof(*pStr);
	int n1 = pStr->GetLength();
	int nPeriod = 0;
	double f2 = 0.;
	double order;
	int nMinus = 0;
	int nPlus = 0;
	bool avail = false;
	for (int i = 0; i < n1; i++) {
		char ch = pStr->GetAt(i);

		if (ch == '.') { order = 1; nPeriod++; }
		else if (ch == '-') {
			if (avail) {
				update = true;
				break;
			}
			else 
				nMinus++; 
		}
		else if (ch == '+') { 
			if (avail) {
				update = true;
				break;
			}
			else
				nPlus++; 
		}
		else if (ch >= '0' && ch <= '9') {
			avail = true;
			if (nPeriod) {
				order /= 10.;
				if (nMinus) {
					f2 -= (order * (double)(ch - '0'));
				}
				else {
					f2 += (order * (double)(ch - '0'));
				}
			}
			else {
				f2 *= 10;
				if (nMinus) {
					f2 -= (double)(ch - '0');
				}
				else {
					f2 += (double)(ch - '0');
				}
			}
		}
		else {
			update = true;
			break;
		}
	}

	if (nPeriod >= 2) { update = true; }
	if (nMinus + nPlus >= 2) { update = true; }

	if (update || abs(f1 - f2) > 0.0000001) {
		pStr->Format("%s", (LPCTSTR)Double2Str(f2));		
		update = true;
	}
	return update;
}

long CSerialComDlg::Str2Num(const CString& str2) {
	CString str;
	str.Format("%s", (LPCTSTR)str2);
	long lSize = strlen(str);
	long lNum = 0;
	long i;
	bool bExistMinus = FALSE;
	BOOL bFindNum = FALSE;

	if (lSize != 0) {
		char *buf = LPSTR(LPCTSTR(str));

		// 숫자만 남겨놓기
		for (i = 0; i < lSize; i++) {
			if (buf[i] >= '0' && buf[i] <= '9') {
				lNum = lNum * 10 + buf[i] - '0';
				bFindNum = TRUE;
			}
			else if (buf[i] == '-') {
				bExistMinus = TRUE;
			}
			else if (bFindNum) {
				break;
			}
		}
	}
	if (bExistMinus)	lNum *= -1;

	return lNum;
}

void CSerialComDlg::OnSetfocusSerialPort() {
	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.
	HKEY hKey;

	//오픈할 레지스터 키에 대한 기본키 이름   
	//오픈할 레지스터 서브키 이름   
	//레지스터키에 대한 핸들   

	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[40], szName[200], szBT[200];

	DWORD index = 0, dwSize = 200, dwSize2 = 40, dwType = REG_SZ;
	CString strCurrentAvailPort;

	m_cSerialPort.ResetContent();
	CString strLastComPort = GetLastComPort();

	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));
	memset(szBT, 0x00, sizeof(szBT));

	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요   
	//szName - 항목값이 저장될 배열   
	//dwSize - 배열의 크기   
	int iLastComPortIndex = -1;
	CString str = "";
	
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)) {
		
		//szName-레지터스터 항목의 이름   
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열   
		//szData-항목값이 저장될 배열   
		//dwSize2-배열의 크기   

		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		str.Format("%s", szName);
		str.Format("(%s)", (LPCTSTR)(str.Right(str.GetLength() - 8)));		
		
		strCurrentAvailPort = CString(szData);
		if (strCurrentAvailPort == strLastComPort) {
			iLastComPortIndex = index;
		}		

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 200;
		dwSize2 = 40;
		strCurrentAvailPort += str;		
		m_cSerialPort.AddString(strCurrentAvailPort);
		index++;
	}
	RegCloseKey(hKey);

	m_iConnectedPortCount = 0;
	for (int i = 0; i < m_cSerialPort.GetCount(); i++) {
		CString strTemp;
		m_cSerialPort.GetLBText(i, strTemp);
		m_iConnectComPort[m_iConnectedPortCount++] = Str2Num(strTemp);
	}

	if (iLastComPortIndex == -1) {
		if (m_cSerialPort.GetCount() > 0) {
			m_iSerialPort = 0;
			m_cSerialPort.SetCurSel(m_iSerialPort);
			m_cSerialPort.GetLBText(m_iSerialPort, m_strSerial);
			m_strSerial.Format("%s", (LPCTSTR)(RemoveSerialInfo(m_strSerial)));
		}
	}
	else {
		BOOL bFind = FALSE;
		for (int i = 0; i < m_cSerialPort.GetCount(); i++) {
			CString strTemp;
			m_cSerialPort.GetLBText(i, strTemp);
			if (RemoveSerialInfo(strTemp) == strLastComPort) {
				m_iSerialPort = i;
				m_cSerialPort.SetCurSel(i);
				m_cSerialPort.GetLBText(i, m_strSerial);
				m_strSerial.Format("%s", (LPCTSTR)(RemoveSerialInfo(m_strSerial)));
				bFind = TRUE;
			}
		}

		if (!bFind) {
			m_iSerialPort = 0;
			m_cSerialPort.SetCurSel(0);
			m_cSerialPort.GetLBText(0, m_strSerial);
			m_strSerial.Format("%s", (LPCTSTR)(RemoveSerialInfo(m_strSerial)))                                                                                           ;
		}
	}
	UDF;
}

void CSerialComDlg::OnSetfocusSerialPort2() {
	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.
	HKEY hKey;

	//오픈할 레지스터 키에 대한 기본키 이름   
	//오픈할 레지스터 서브키 이름   
	//레지스터키에 대한 핸들   

	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[40], szName[200], szBT[200];

	DWORD index = 0, dwSize = 200, dwSize2 = 40, dwType = REG_SZ;
	CString strCurrentAvailPort;

	m_cSerialPort2.ResetContent();
	CString strLastComPort = GetLastComPort2();

	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));
	memset(szBT, 0x00, sizeof(szBT));

	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요   
	//szName - 항목값이 저장될 배열   
	//dwSize - 배열의 크기   
	int iLastComPortIndex = -1;
	CString str = "";

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)) {

		//szName-레지터스터 항목의 이름   
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열   
		//szData-항목값이 저장될 배열   
		//dwSize2-배열의 크기   

		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		str.Format("%s", szName);
		str.Format("(%s)", (LPCTSTR)str.Right(str.GetLength() - 8));

		strCurrentAvailPort = CString(szData);
		if (strCurrentAvailPort == strLastComPort) {
			iLastComPortIndex = index;
		}

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 200;
		dwSize2 = 40;
		strCurrentAvailPort += str;
		m_cSerialPort2.AddString(strCurrentAvailPort);
		index++;
	}
	RegCloseKey(hKey);

	m_iConnectedPortCount = 0;
	for (int i = 0; i < m_cSerialPort2.GetCount(); i++) {
		CString strTemp;
		m_cSerialPort2.GetLBText(i, strTemp);
		m_iConnectComPort[m_iConnectedPortCount++] = Str2Num(strTemp);
	}

	if (iLastComPortIndex == -1) {
		if (m_cSerialPort2.GetCount() > 0) {
			m_iSerialPort2 = 0;
			m_cSerialPort2.SetCurSel(m_iSerialPort2);
			m_cSerialPort2.GetLBText(m_iSerialPort, m_strSerial2);
			m_strSerial2.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial2));
		}
	}
	else {
		BOOL bFind = FALSE;
		for (int i = 0; i < m_cSerialPort2.GetCount(); i++) {
			CString strTemp;
			m_cSerialPort2.GetLBText(i, strTemp);
			if (RemoveSerialInfo(strTemp) == strLastComPort) {
				m_iSerialPort2 = i;
				m_cSerialPort2.SetCurSel(i);
				m_cSerialPort2.GetLBText(i, m_strSerial2);
				m_strSerial2.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial2));
				bFind = TRUE;
			}
		}

		if (!bFind) {
			m_iSerialPort2 = 0;
			m_cSerialPort2.SetCurSel(0);
			m_cSerialPort2.GetLBText(0, m_strSerial2);
			m_strSerial2.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial2));
		}
	}
	UDF;
}

void CSerialComDlg::OnSetfocusSerialPort3() {
	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.
	HKEY hKey;

	//오픈할 레지스터 키에 대한 기본키 이름   
	//오픈할 레지스터 서브키 이름   
	//레지스터키에 대한 핸들   

	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[40], szName[200], szBT[200];

	DWORD index = 0, dwSize = 200, dwSize2 = 40, dwType = REG_SZ;
	CString strCurrentAvailPort;

	m_cSerialPort3.ResetContent();
	CString strLastComPort = GetLastComPort3();

	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));
	memset(szBT, 0x00, sizeof(szBT));

	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요   
	//szName - 항목값이 저장될 배열   
	//dwSize - 배열의 크기   
	int iLastComPortIndex = -1;
	CString str = "";

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)) {

		//szName-레지터스터 항목의 이름   
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열   
		//szData-항목값이 저장될 배열   
		//dwSize2-배열의 크기   

		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		str.Format("%s", szName);
		str.Format("(%s)", (LPCTSTR)str.Right(str.GetLength() - 8));

		strCurrentAvailPort = CString(szData);
		if (strCurrentAvailPort == strLastComPort) {
			iLastComPortIndex = index;
		}

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 200;
		dwSize2 = 40;
		strCurrentAvailPort += str;
		m_cSerialPort3.AddString(strCurrentAvailPort);
		index++;
	}
	RegCloseKey(hKey);

	m_iConnectedPortCount = 0;
	for (int i = 0; i < m_cSerialPort3.GetCount(); i++) {
		CString strTemp;
		m_cSerialPort3.GetLBText(i, strTemp);
		m_iConnectComPort[m_iConnectedPortCount++] = Str2Num(strTemp);
	}

	if (iLastComPortIndex == -1) {
		if (m_cSerialPort3.GetCount() > 0) {
			m_iSerialPort3 = 0;
			m_cSerialPort3.SetCurSel(m_iSerialPort3);
			m_cSerialPort3.GetLBText(m_iSerialPort, m_strSerial3);
			m_strSerial3.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial3));
		}
	}
	else {
		BOOL bFind = FALSE;
		for (int i = 0; i < m_cSerialPort3.GetCount(); i++) {
			CString strTemp;
			m_cSerialPort3.GetLBText(i, strTemp);
			if (RemoveSerialInfo(strTemp) == strLastComPort) {
				m_iSerialPort3 = i;
				m_cSerialPort3.SetCurSel(i);
				m_cSerialPort3.GetLBText(i, m_strSerial3);
				m_strSerial3.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial3));
				bFind = TRUE;
			}
		}

		if (!bFind) {
			m_iSerialPort3 = 0;
			m_cSerialPort3.SetCurSel(0);
			m_cSerialPort3.GetLBText(0, m_strSerial3);
			m_strSerial3.Format("%s", (LPCTSTR)RemoveSerialInfo(m_strSerial3));
		}
	}
	UDF;
}

void CSerialComDlg::OnEnChangePackethead() {
	char cPacketHead = m_cPacketHead;
    CString str;
    GetDlgItemText(IDC_PacketHead, str);
    
    int iSize = str.GetLength();
    char ch = ' ';
    for (int i = 0; i < iSize; i++) {
        ch = str.GetAt(i);
        if (ch != ' ') {            
            break;
        }
    }

    bool bAvail_header = false;
    
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
        bAvail_header = true;
    }
    
    if (m_cPacketHead == ch || ch == ' ') { 
        if (iSize != 1) {
            SetDlgItemText(IDC_PacketHead, m_strPacketHeader);
        }
        return;   
    }
    
	if (( ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' ) {
		m_strPacketHeader.Format("%c", ch);
        m_cPacketHead = ch;
        SetDlgItemText(IDC_PacketHead, m_strPacketHeader);

        if (ch == '_') {
            // No Header 
            m_bAcceptNoHeader = TRUE;
        }
        else {
            m_bAcceptNoHeader = FALSE;
        }
		CheckDlgButton(IDC_NoHeader, m_bAcceptNoHeader);
	}
    else {
        // No Header Item 
        SetDlgItemText(IDC_PacketHead, m_strPacketHeader);
        return;
    }
    
    if ((m_cPacketHead >= 'a' &&m_cPacketHead <= 'z') || (m_cPacketHead >= 'A' && m_cPacketHead <= 'Z')) {
        m_bAcceptNoHeader = FALSE;
    }
    else {
        m_cPacketHead = '_';
        m_bAcceptNoHeader = TRUE;
    }
    CheckDlgButton(IDC_NoHeader, m_bAcceptNoHeader);

    m_strPacketHeader.Format("%c", m_cPacketHead);
    SetDlgItemText(IDC_PacketHead, m_strPacketHeader);
       
	if (cPacketHead != m_cPacketHead) {
		CString strAddress = "Command";
		CString strItem = "LastCommand";
		SetRegRoot_RegistryData(strAddress, strItem, m_strPacketHeader);

		strAddress.Format("Header\\%c%d", m_cPacketHead, m_cPacketHead);
		strItem = "LastMode";
		CString strLastMode = GetRegRoot_RegistryData(strAddress, strItem);
		if (m_iPresetMode != min(9, max(0, atoi(strLastMode)))) {
			m_iPresetMode = min(9, max(0, atoi(strLastMode)));
            m_ctrlMode.SetCurSel(m_iPresetMode);
		}
		LoadHeaderConfiguration();
		
		CheckDataBufferFull();
		CString strLog;			
		strLog.Format("#Header:%c", m_cPacketHead);

		++m_iLogIndex;
		AddLogData(strLog);
		m_iLogHistoryIndexTable.emplace_back(m_iHistoryIndex);
		m_iLogFilterIndexTable.emplace_back(m_iFilterIndex);
        ScrollToLastItem();

		LoadHeaderConfiguration();
		UpdateDrawGraphControl();
		OptimizeGraphCount();
		GetGraphLogData();
		GetTextLogData();
	}
}

void CSerialComDlg::LoadComPortList() {
	m_iComPortCount = max(0, min(MAX_COM_PORT_SIZE, atoi(GetRegRoot_RegistryData("ComPortList", "nSize"))));

	CString strItem;
	for (int i = 0; i < m_iComPortCount; i++) {
		strItem.Format("ComPort_%d", i);
		m_ComPortList[i].iComPort = atoi(GetRegRoot_RegistryData("ComPortList\\PortNumber", strItem));
		m_ComPortList[i].strPortID.Format("%s", (LPCTSTR)GetRegRoot_RegistryData("ComPortList\\PortID", strItem));
		m_ComPortList[i].strDescription.Format("%s", (LPCTSTR)GetRegRoot_RegistryData("ComPortList\\PortDescription", strItem));
	}
}

void CSerialComDlg::SaveComPortList() {
	CString strValue;
	strValue.Format("%d", m_iComPortCount);
	SetRegRoot_RegistryData("ComPortList", "nSize", strValue);

	CString strItem;
	for (int i = 0; i < m_iComPortCount; i++) {
		strItem.Format("ComPort_%d", i);
		strValue.Format("%d", m_ComPortList[i].iComPort);
		SetRegRoot_RegistryData("ComPortList\\PortNumber", strItem, strValue);
		SetRegRoot_RegistryData("ComPortList\\PortID", strItem, m_ComPortList[i].strPortID);
		SetRegRoot_RegistryData("ComPortList\\PortDescription", strItem, m_ComPortList[i].strDescription);
	}
}


void CSerialComDlg::UpdateControl() {
	UDT;
	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		if (m_fRangeMax[iGraph] - m_fRangeMin[iGraph] == 0) m_strRangeMax[iGraph].Format("%s", (LPCTSTR)Dbl2Str(++m_fRangeMax[iGraph]));
	}
	DrawInit();
	m_bReqGraphUpdate = TRUE;
}


void CSerialComDlg::ChangeFilterItem() {
	CWnd* pWnd = GetFocus();
	if (pWnd != nullptr) {
		int nCtrlID = pWnd->GetDlgCtrlID();
		for (int i = 0; i < MAX_FILTER; i++) {
			if (m_IDC_FilterName[i] == nCtrlID) {
				GetDlgItemText(m_IDC_FilterName[i], m_strFilterItem[i]);
				m_iFilterLength[i] = m_strFilterItem[i].GetLength();
				break;
			}
		}
	}
}

void CSerialComDlg::ChangeFilterUse() {
	CWnd* pWnd = GetFocus();
	if (pWnd != nullptr) {
		int nCtrlID = pWnd->GetDlgCtrlID();
		for (int i = 0; i < MAX_FILTER; i++) {
			if (m_IDC_UseFilter[i] == nCtrlID) {
				m_bUseFilterItem[i] = IsDlgButtonChecked(m_IDC_UseFilter[i]);
				break;
			}
		}
	}
}

void CSerialComDlg::OnBnClickedSend() {
    CString str;
    GetDlgItemText(IDC_SendChar, str);

    int iSize = str.GetLength();
    bool bFind = false;
    char ch = ' ';
    for (int i = 0; i < iSize; i++) {
        ch = str.GetAt(i);
        if (ch != ' ') {
            str.Format("%c", ch);
            SetDlgItemText(IDC_SendChar, str);
            break;
        }
    }
    
	// 유효성 검증	
	if (m_bUseAutoHeader_tx) {        
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
			CString strTemp;
			strTemp.Format("%c", ch);
			if (strTemp != m_strPacketHeader) {
				m_strPacketHeader.Format("%s", (LPCTSTR)strTemp);

				m_iPresetMode = 0;
                SetDlgItemText(IDC_PacketHead, m_strPacketHeader);
				ChangeHeader();
			}
		}
	}
	
	if (m_ComuPort.m_bConnected) m_ComuPort.WriteComm(&m_cPacketHead, 1);
	if (m_ComuPort2.m_bConnected) m_ComuPort2.WriteComm(&m_cPacketHead, 1);
	if (m_ComuPort.m_bConnected || m_ComuPort2.m_bConnected) {
		str.Format("Send: '%c' (0x%X) -> %s  %s", m_cPacketHead, m_cPacketHead, m_ComuPort.m_bConnected ? (LPCTSTR)m_ComuPort.m_sPortName : " ", m_ComuPort2.m_bConnected ? (LPCTSTR)m_ComuPort2.m_sPortName : " ");
		SetDlgItemText(IDC_Temp, str);
	}
}


CPoint CSerialComDlg::CalculateDrawPoint(int iGraph, int iIndex, double fValue) {
	double fMinY = m_fRangeMin[iGraph];
	double fMaxY = m_fRangeMax[iGraph];

	CPoint DrawPosition;

	DrawPosition.x = m_iDrawGraphRangeX[0] + (int64_t)m_iDrawGraphSpanX * iIndex / max(1, m_iDisplaySize);
	if ((fMaxY - fMinY) < 0.002) {
		fMaxY += 0.001;
		fMinY -= 0.001;
	}
	
	double fTemp1 = (fValue - fMinY) * (m_iDrawGraphRangeY[iGraph][1] - m_iDrawGraphRangeY[iGraph][0]);	
	DrawPosition.y = m_iDrawGraphRangeY[iGraph][1] - (int)(fTemp1 / (fMaxY - fMinY));
	
	DrawPosition.x = min(max(DrawPosition.x, m_iDrawGraphRangeX[0] + 3), m_iDrawGraphRangeX[1] - 3);
	DrawPosition.y = min(max(DrawPosition.y, m_iDrawGraphRangeY[iGraph][0] + 3), m_iDrawGraphRangeY[iGraph][1] - 3);

	return DrawPosition;
}


CPoint CSerialComDlg::CalculateDrawPointY(int iGraph, double fValue) {
	double fMinY = m_fRangeMin[iGraph];
	double fMaxY = m_fRangeMax[iGraph];
	CPoint DrawPosition;
	if (abs(fMaxY - fMinY) < 0.002) {
		fMaxY = max(fMaxY, fMinY) + 0.001;
		fMinY = min(fMaxY, fMinY) - 0.001;
	}
	
	double fTemp1 = (fValue - fMinY) * (m_iDrawGraphRangeY[iGraph][1] - m_iDrawGraphRangeY[iGraph][0]);
	DrawPosition.y = m_iDrawGraphRangeY[iGraph][1] - (int)((fTemp1) / (fMaxY - fMinY));
	DrawPosition.y = min(max(DrawPosition.y, m_iDrawGraphRangeY[iGraph][0] + 3), m_iDrawGraphRangeY[iGraph][1] - 3);

	return DrawPosition;
}


int CSerialComDlg::CalculateDrawPointX(int iIndex) {
	CPoint DrawPosition;

	DrawPosition.x = m_iDrawGraphRangeX[0] + (int64_t)m_iDrawGraphSpanX * iIndex / m_iDisplaySize;
	DrawPosition.x = min(max(DrawPosition.x, m_iDrawGraphRangeX[0] + 3), m_iDrawGraphRangeX[1] - 3);

	return DrawPosition.x;
}


int CSerialComDlg::CalculateIndex(int iPtx) {
	int Index;
	if (m_iDrawGraphRangeX[1] != m_iDrawGraphRangeX[0])
		Index = ((int64_t)(iPtx - m_iDrawGraphRangeX[0]) * m_iDisplaySize + m_iDrawGraphSpanX / 2)  / m_iDrawGraphSpanX;
	else
		Index = 0;
	if (Index < 0)
		Index = 0;
	else if (Index > m_iDisplaySize)
		Index = m_iDisplaySize;
	return Index;
}

void CSerialComDlg::UpdateGraphData(void) {
	static BOOL bIsBusy = FALSE;

	if (bIsBusy) return;
	if (m_pUserScreen != nullptr && m_pUserScreen->m_iReqReplayIndex != -1) return;
	
    static UINT32 preCheck = 0;
	if (GetTickCount() - preCheck < 5) {
		return;
	}

	bIsBusy = TRUE;
	m_bReqGraphUpdate = FALSE;
	Wait(0);
    preCheck = GetTickCount();

	// 대화상자가 비활성화 되었을때는 엔트키 반응하지 않도록 함
	HWND hForeground = ::GetForegroundWindow();
	if (hForeground == this->GetSafeHwnd()) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			if (m_bRelesedChangeMode && !m_bReqChangeMode) {
				m_bReqChangeMode = true;
				m_bRelesedChangeMode = false;
				OnBnClickedDefaultbutton();
				m_bRelesedChangeMode = true;
			}
		}
		else {
			m_bReqChangeMode = false;
		}
	}

	CPoint pt;
	CString str;
	int iReqSelectIndex = atoi(m_strIndexNumber);
	BOOL bPreserveUpdateGraph = FALSE;

	// Step 0. 그리기 준비 작업 
	// 시작 Index 	Draw 간격  종료 Index
	m_iHistorySelectIndex = m_iHistoryIndex - 1;
	int iStartIndex = m_iHistoryIndex - m_iDisplaySize;

	if (m_iProgramMode == DATA_ACQUISITION_MODE) {  // Data수집 모드인가
		m_bDrawSelectData = FALSE;
		m_iHistoryMove = 0;
		m_iHistorySelectIndex = max(0, m_iHistorySelectIndex - 1);
	}
	else if (m_bDrawSelectData) {  // 분석모드중 선택된 Index가 있는가
		m_iHistorySelectIndex = iReqSelectIndex;
		iStartIndex -= m_iHistoryMove;

		CPoint pt = CalculateDrawPoint(0, m_iHistorySelectIndex - iStartIndex, 0);
		m_iHistorySelectPt = pt.x;
	}
	else {
		m_iHistorySelectIndex = max(0, m_iHistorySelectIndex - 1);
		if (iReqSelectIndex != m_iHistorySelectIndex) {
			m_strIndexNumber.Format("%d", m_iHistorySelectIndex);
			m_iCurrentIndex = m_iHistorySelectIndex;
			m_iHistoryMove = 0;
		}
	}

	// Update Selected Data Value 	
	m_iHistorySelectIndex = max(0, min(m_iHistorySelectIndex, (int)m_HistoryData.size() - 1));
	for (int i = 0; i < m_iShowDataControl; i++) {
		int idx = i + m_iControlDataOffset;
		SetDlgItemText(m_IDC_DataValue[i], Double2Str(m_HistoryData[m_iHistorySelectIndex].fDataValue[idx]));
	}

	for (int i = 0; i < m_iMaxMathSize; i++) {
		if (m_MathData[i].bValid) {
            m_MathData[i].iValue = (int)(1000 * m_HistoryData[m_iHistorySelectIndex].fMathValue[i]) / 1000.;
		}
		else
            m_MathData[i].iValue = 0;
	}

	for (int i = 0; i < m_iShowMathControl; i++) {
		SetDlgItemText(m_IDC_MathProbeValue[i], Double2Str(m_MathData[i + m_iControlMathOffset].iValue));
	}
	
	if (m_iProgramMode == DATA_ACQUISITION_MODE)
		ShowSelectValueDrawInit(m_iHistoryIndex - 1);
	else
		ShowSelectValueDrawInit(m_iHistorySelectIndex);
	
	// Display Time
	CRect rect;
	str.Format("                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ");
	rect.SetRect(m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 5, m_iDrawGraphRangeX[1], m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 23);
	m_MemDC.DrawText(str, rect, DT_SINGLELINE | DT_LEFT);
	rect.SetRect(m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 15, m_iDrawGraphRangeX[1], m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 33);
	m_MemDC.DrawText(str, rect, DT_SINGLELINE | DT_LEFT);
	m_MemDC.SelectObject(&m_PenBlack);

	// Graph Clear
	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		DrawInit(iGraph);
	}
	Wait(0);
	// Time Line
	if (m_iHistoryIndex > 0) {
		int iTextHalfWidth = 0;  // width 8
		switch (m_iTimeDisplayMode) {
		case 0:
			iTextHalfWidth = 4 * 8;
			break;

		case 1:
			iTextHalfWidth = 4 * 9;
			break;

		case 2:
			iTextHalfWidth = 4 * 6;
			break;
		}

		if (m_bShowTimelineN) {
			if (m_iHistoryIndex < 10) {
				iTextHalfWidth += 4 * 3;
			}
			else {
				iTextHalfWidth += 4 * (int)log10(m_iHistoryIndex);
			}
		}

		int minWidth = iTextHalfWidth * 2 + 10;
		int nPt;
		switch (m_iGridX_Type) {
		case 1: nPt = m_iDrawGraphSpanX / m_iGridX; break;      // 등분
		case 2: nPt = m_iGridX; break;							// 절대 size
		default: nPt = max(minWidth, m_iDrawGraphSpanX * m_iGridX / m_iDisplaySize); break;  // 일정 Index마다
		}
		Wait(0);

		int index_delta = max(1, (int64_t)m_iDisplaySize * nPt / m_iDrawGraphSpanX);
		while (m_iDrawGraphSpanX * index_delta / max(1, m_iDisplaySize) < minWidth) {
			index_delta++;
		}

		int end = min(m_iHistoryIndex, m_iHistoryIndex - m_iHistoryMove);
		int start = end - m_iDisplaySize;

		m_MemDC.SetBkColor(RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]));
		m_MemDC.SetTextColor(RGB(m_iTextColor[0], m_iTextColor[1], m_iTextColor[2]));

		for (int index = (start + index_delta - 1) / index_delta * index_delta; index < end; index += index_delta) {
			if (index < 0) continue;
			pt = CalculateDrawPoint(0, index - start, 0);
			if (pt.x < m_iDrawGraphRangeX[0] + 3 || pt.x > m_iDrawGraphRangeX[1] - 3) {
				continue;
			}
			m_MemDC.SelectObject(m_PenGrid);
			for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
				m_MemDC.MoveTo(pt.x, m_iDrawGraphRangeY[iGraph][0] + 2);
				m_MemDC.LineTo(pt.x, m_iDrawGraphRangeY[iGraph][1] - 2);
			}
			m_MemDC.SelectObject(&m_PenBlack);

			if (pt.x < m_iDrawGraphRangeX[0] + minWidth / 2 || pt.x > m_iDrawGraphRangeX[1] - minWidth / 2) {
				continue;
			}
			int iTime = m_HistoryData[index].iRcvTime;

			switch (m_iTimeDisplayMode) {
			case 0:
				str.Format("%02d:%02d:%02d", iTime / 3600, (iTime / 60) % 60, iTime % 60);
				break;

			case 1:
				str.Format("%02d:%02d.%03d", (iTime / 60) % 60, iTime % 60, m_HistoryData[index].iRcvTime_ms);
				break;

			case 2:
				str.Format("%02d.%03d", iTime % 60, m_HistoryData[index].iRcvTime_ms);
				break;
			}

			if (m_bShowTimelineN) {
                CString strTemp = str;
				str.Format("%s(%d)", (LPCTSTR)strTemp, index);
			}

			m_MemDC.MoveTo(pt.x, m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 5);
			m_MemDC.LineTo(pt.x, m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 15);

			int iTextStart = pt.x - iTextHalfWidth;
			if (m_iDrawGraphRangeX[0] < iTextStart &&  m_iDrawGraphRangeX[1] > iTextStart + iTextHalfWidth) {
				m_MemDC.TextOutA(iTextStart, m_iDrawGraphRangeY[m_iGraphCount - 1][1] + 15, str);
			}
		}
	}
	Wait(0);
	// Graph 그리기
	double fMinValue[MAX_GRAPH_COUNT], fMaxValue[MAX_GRAPH_COUNT];

	BOOL bDrawMark = FALSE;
	BOOL bContinue = TRUE;
	int iMarkSize = m_iMarkSize;
	while (bContinue) {
		if (m_iDisplaySize && m_iDrawGraphSpanX / m_iDisplaySize > iMarkSize * 2 + 2) {
			bDrawMark = TRUE;
			bContinue = FALSE;
		}
		else {
			if (--iMarkSize <= 0) {
				bContinue = FALSE;
			}
		}
	}

	int iPeriodDisplayIndex = 1;
	if (m_iDisplaySize != 0 && m_iDrawGraphSpanX != 0) iPeriodDisplayIndex = max(1, m_iDisplaySize * 3 / m_iDrawGraphSpanX / 4 - 1);
	// index random : Reduction Moire
	MakeRandomIndexDataSet(iPeriodDisplayIndex);

	m_MemDC.SelectObject(&m_BrushNull);

	BOOL bReqGraphUpdate4ChangeMinMax = FALSE;
	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		BOOL bMinMaxCheck = FALSE;
		fMinValue[iGraph] = 65535;
		fMaxValue[iGraph] = -32767;

		// Range Check Flag setting
		if (m_bAutoRange_Graph[iGraph]) bMinMaxCheck = TRUE;

		// Draw Line
		BOOL bPreviousRobot, bCurrentRobot;
		CPoint PrePt[2];	
		BOOL bIsInitPrePt[2];
		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			bIsInitPrePt[0] = FALSE;
			bIsInitPrePt[1] = FALSE;

			if (m_bDraw_GraphData[iGraph][iData]) {
				if (iStartIndex < 0) {
					bPreviousRobot = m_HistoryData[0].iRecvChannel;
					bIsInitPrePt[bPreviousRobot] = TRUE;
					if (m_iHistoryIndex > 1) {
						PrePt[bPreviousRobot] = CalculateDrawPoint(iGraph, 1 - iStartIndex, m_HistoryData[0].fDataValue[iData]);
						m_MemDC.MoveTo(PrePt[bPreviousRobot]);
					}
				}
				else {
					bPreviousRobot = m_HistoryData[iStartIndex].iRecvChannel;
					bIsInitPrePt[bPreviousRobot] = TRUE;
					PrePt[bPreviousRobot] = CalculateDrawPoint(iGraph, 0, m_HistoryData[iStartIndex].fDataValue[iData]);
					m_MemDC.MoveTo(PrePt[bPreviousRobot]);
				};
				m_MemDC.SelectObject(&m_PenData[iData]);
				m_iCurrentRadomIntervalIndex = 0;

				for (int i = 0; i <= m_iDisplaySize; i += iPeriodDisplayIndex) {
					int index = iStartIndex + i + m_iRandInterval[m_iCurrentRadomIntervalIndex];
					if (++m_iCurrentRadomIntervalIndex >= N_RAND_INTERVAL)m_iCurrentRadomIntervalIndex = 0;
					if (index >= m_iHistoryIndex) index = m_iHistoryIndex - 1;
					if (index < 0) continue;

					bCurrentRobot = m_HistoryData[index].iRecvChannel;

					pt = CalculateDrawPoint(iGraph, i, m_HistoryData[index].fDataValue[iData]);

					if (m_bDrawChannel[m_HistoryData[index].iRecvChannel]) {
						if (!bIsInitPrePt[bCurrentRobot]) {
							bIsInitPrePt[bCurrentRobot] = TRUE;
							PrePt[bCurrentRobot] = pt;
						}

						if (bDrawMark && m_bUseDataMark[iData] && m_iProgramMode == DATA_ANALYZE_MODE) {
							m_MemDC.Rectangle(pt.x - iMarkSize, max(m_iDrawGraphRangeY[iGraph][0] + 1, pt.y - iMarkSize), pt.x + iMarkSize, min(m_iDrawGraphRangeY[iGraph][1] - 1, pt.y + iMarkSize + 1));
						}

						if (bPreviousRobot == bCurrentRobot) {
							m_MemDC.LineTo(pt);
						}
						else {
							m_MemDC.MoveTo(PrePt[bCurrentRobot]);
							m_MemDC.LineTo(pt);
							m_MemDC.Rectangle(pt.x - 1, pt.y - 1, pt.x + 1, pt.y + 2);
							bPreviousRobot = bCurrentRobot;
						}

						PrePt[bCurrentRobot] = pt;
					}

					if (bMinMaxCheck) {
						if (m_HistoryData[index].fDataValue[iData] > fMaxValue[iGraph]) {
							fMaxValue[iGraph] = m_HistoryData[index].fDataValue[iData];
						}
						if (m_HistoryData[index].fDataValue[iData] < fMinValue[iGraph]) {
							fMinValue[iGraph] = m_HistoryData[index].fDataValue[iData];
						}
					}
				}
			}
		}

		// Draw Math Points
		for (int iData = 0; iData < m_iMaxMathSize; iData++) {
			if (m_MathData[iData].bValid && m_MathData[iData].bDisplayGraph[iGraph]) {
				m_MemDC.SelectObject(&m_Pen_Math[iData]);
				BOOL bIsFirstData = TRUE;
				m_iCurrentRadomIntervalIndex = 0;

				for (int i = 0; i <= m_iDisplaySize; i += iPeriodDisplayIndex) {
					int index = iStartIndex + i + m_iRandInterval[m_iCurrentRadomIntervalIndex];
					if (++m_iCurrentRadomIntervalIndex >= N_RAND_INTERVAL)m_iCurrentRadomIntervalIndex = 0;
					if (index >= m_iHistoryIndex) index = m_iHistoryIndex - 1;
					if (index < 0) continue;

					pt = CalculateDrawPoint(iGraph, i, m_HistoryData[index].fMathValue[iData]);
					if (bIsFirstData) {
						m_MemDC.MoveTo(pt);
						bIsFirstData = FALSE;
					}
					if (bDrawMark && m_MathData[iData].bUseMark && m_iProgramMode == DATA_ANALYZE_MODE) {
						m_MemDC.Rectangle(pt.x - iMarkSize, max(m_iDrawGraphRangeY[iGraph][0] + 1, pt.y - iMarkSize), pt.x + iMarkSize, min(m_iDrawGraphRangeY[iGraph][1] - 1, pt.y + iMarkSize + 1));
					}
					m_MemDC.LineTo(pt);

					if (bMinMaxCheck) {
						if (m_HistoryData[index].fMathValue[iData] > fMaxValue[iGraph]) {
							fMaxValue[iGraph] = m_HistoryData[index].fMathValue[iData];
						}
						if (m_HistoryData[index].fMathValue[iData] < fMinValue[iGraph]) {
							fMinValue[iGraph] = m_HistoryData[index].fMathValue[iData];
						}
					}
				}
			}
		}

		if (bMinMaxCheck) {
			double fLimit = (fMaxValue[iGraph] - fMinValue[iGraph]) * 0.1;   // 10% Margin
			double fLimit2 = fLimit * 0.2;
			str.Format("Graph:%d   %f", iGraph, fLimit);
			if (m_fRangeMax[iGraph] <  fMaxValue[iGraph] + fLimit2 || m_fRangeMax[iGraph] > fLimit + fMaxValue[iGraph]) {
				m_fRangeMax[iGraph] = fMaxValue[iGraph] + fLimit;
				m_strRangeMax[iGraph].Format("%s", (LPCTSTR)Dbl2Str(m_fRangeMax[iGraph]));
				bReqGraphUpdate4ChangeMinMax = TRUE;
			}

			if (m_fRangeMin[iGraph] + fLimit < fMinValue[iGraph] || m_fRangeMin[iGraph] + fLimit2 > fMinValue[iGraph] ) {
				m_fRangeMin[iGraph] = fMinValue[iGraph] - fLimit;
				m_strRangeMin[iGraph].Format("%s", (LPCTSTR)Dbl2Str(m_fRangeMin[iGraph]));
				bReqGraphUpdate4ChangeMinMax = TRUE;
			}

			m_strRangeMid[iGraph].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMax[iGraph] + m_fRangeMin[iGraph]) / 2));
		}
	}

	if (m_bDrawSelectData) {
		m_MemDC.SelectObject(&m_PenSelect);

		for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
			m_MemDC.MoveTo(m_iHistorySelectPt, m_iDrawGraphRangeY[iGraph][0] + 2);
			m_MemDC.LineTo(m_iHistorySelectPt, m_iDrawGraphRangeY[iGraph][1] - 2);
		}
	}

	if (m_bDisplayPosition && m_bAvailablePositionGraph && m_iSelectPositionGraphX != -1 && m_iSelectPositionGraphY != -1) {
		// Draw on m_MemDC_Position
		// Copy MemDC_Position --> MemDC
		m_MemDC_Position.BitBlt(m_iPositionGraphRangeX[0] - m_iDev, m_iPositionGraphRangeY[0] - m_iDev, m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0] + 2 * m_iDev + 2, m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0] + 2 * m_iDev + 2, &m_MemDC_Position_Init, m_iPositionGraphRangeX[0] - m_iDev, m_iPositionGraphRangeY[0] - m_iDev, SRCCOPY);

		int iSize = 5;
		m_MemDC_Position.SelectObject(&m_PenPosition[0]);
		m_MemDC_Position.SelectObject(&m_BrushPosition[0]);
		CPoint pt;
		int iPeriodDisplayIndex_PositoinGraph = min(100, iPeriodDisplayIndex);
		double fMinX = 0, fMinY = 0, fMaxX = 0, fMaxY = 0;
		int n = 0;
		int x, y;
		int iSelectMathdataX = m_iSelectPositionGraphX - m_iMaxDataSize;
		int iSelectMathdataY = m_iSelectPositionGraphY - m_iMaxDataSize;
		
		// Position Graph mode
		for (int i = 0; i < m_iDisplaySize - 1; i += iPeriodDisplayIndex_PositoinGraph) {
			int index = iStartIndex + i;
			if (index >= m_iHistoryIndex) continue;
			if (index <= 0) continue;
			if (m_iSelectPositionGraphX < m_iMaxDataSize)
				x = (int)m_HistoryData[index].fDataValue[m_iSelectPositionGraphX];
			else
				x = (int)m_HistoryData[index].fMathValue[iSelectMathdataX];

			if (m_iSelectPositionGraphY < m_iMaxDataSize)
				y = (int)m_HistoryData[index].fDataValue[m_iSelectPositionGraphY];
			else
				y = (int)m_HistoryData[index].fMathValue[iSelectMathdataY];

			pt = CalculationDrawPosition_PositionGraph(x, y);
			if (pt.x > m_iPositionGraphRangeX[0] && pt.x < m_iPositionGraphRangeX[1] && pt.y > m_iPositionGraphRangeY[0] && pt.y < m_iPositionGraphRangeY[1]) {
				m_MemDC_Position.Ellipse(pt.x - iSize, pt.y - iSize, pt.x + iSize, pt.y + iSize);
			}

			if (n == 0) {
				fMinX = fMaxX = x;
				fMinY = fMaxY = y;
			}
			else {
				if (fMinX > x) fMinX = x;
				if (fMaxX < x) fMaxX = x;
				if (fMinY > y) fMinY = y;
				if (fMaxY < y) fMaxY = y;
			}
			n++;
		}

		iSize = 6;
		m_MemDC_Position.SelectObject(&m_PenPosition[1]);
		m_MemDC_Position.SelectObject(&m_BrushPosition[1]);
		// Draw Select Data
		int iSelectIndex = min(m_iHistoryIndex, max(atoi(m_strIndexNumber), 0));
		if (iSelectIndex >= iStartIndex && iSelectIndex <= iStartIndex + m_iDisplaySize && iSelectIndex > 0) {
			if (m_iSelectPositionGraphX < m_iMaxDataSize)
				x = (int)m_HistoryData[iSelectIndex].fDataValue[m_iSelectPositionGraphX];
			else
				x = (int)m_HistoryData[iSelectIndex].fMathValue[iSelectMathdataX];

			if (m_iSelectPositionGraphY < m_iMaxDataSize)
				y = (int)m_HistoryData[iSelectIndex].fDataValue[m_iSelectPositionGraphY];
			else
				y = (int)m_HistoryData[iSelectIndex].fMathValue[iSelectMathdataY];

			pt = CalculationDrawPosition_PositionGraph(x, y);
			if (pt.x > m_iPositionGraphRangeX[0] && pt.x < m_iPositionGraphRangeX[1] && pt.y > m_iPositionGraphRangeY[0] && pt.y < m_iPositionGraphRangeY[1]) {
				m_MemDC_Position.Ellipse(pt.x - iSize, pt.y - iSize, pt.x + iSize, pt.y + iSize);
			}
		}
		
		if (m_Measure3P.bIsMeasuring) {
			iSize = 3;
			m_MemDC_Position.SelectObject(&m_PenPosition[0]);
			m_MemDC_Position.SelectObject(&m_BrushAdjust[1]);
			switch (m_Measure3P.Index) {
			case 1:
				m_MemDC_Position.Ellipse(m_Measure3P.Pt[1].x - iSize, m_Measure3P.Pt[1].y - iSize, m_Measure3P.Pt[1].x + iSize, m_Measure3P.Pt[1].y + iSize);
				break;

			case 2:
				if (m_Measure3P.bIsClick) {
					double fDist = m_fValueScale_PositionGraph * sqrt((m_Measure3P.Pt[2].x - m_Measure3P.Pt[1].x)*(m_Measure3P.Pt[2].x - m_Measure3P.Pt[1].x) + (m_Measure3P.Pt[2].y - m_Measure3P.Pt[1].y)*(m_Measure3P.Pt[2].y - m_Measure3P.Pt[1].y));
					m_strTemp.Format("[Distance] Dist:%.0f  (X:%.0f   Y:%.0f)", fDist, m_fValueScale_PositionGraph* (m_Measure3P.Pt[2].x - m_Measure3P.Pt[1].x), m_fValueScale_PositionGraph * (m_Measure3P.Pt[2].y - m_Measure3P.Pt[1].y));
					SetDlgItemText(IDC_Temp, m_strTemp);

					m_MemDC_Position.MoveTo(m_Measure3P.Pt[1]);
					m_MemDC_Position.LineTo(m_Measure3P.Pt[2]);
					m_MemDC_Position.Ellipse(m_Measure3P.Pt[2].x - iSize, m_Measure3P.Pt[2].y - iSize, m_Measure3P.Pt[2].x + iSize, m_Measure3P.Pt[2].y + iSize);
				}
				m_MemDC_Position.Ellipse(m_Measure3P.Pt[1].x - iSize, m_Measure3P.Pt[1].y - iSize, m_Measure3P.Pt[1].x + iSize, m_Measure3P.Pt[1].y + iSize);
				break;
			case 3:
				if (!m_Measure3P.bIsClick) {
					m_MemDC_Position.MoveTo(m_Measure3P.Pt[1]);
					m_MemDC_Position.LineTo(m_Measure3P.Pt[2]);
					m_MemDC_Position.Ellipse(m_Measure3P.Pt[2].x - iSize, m_Measure3P.Pt[2].y - iSize, m_Measure3P.Pt[2].x + iSize, m_Measure3P.Pt[2].y + iSize);
					m_MemDC_Position.Ellipse(m_Measure3P.Pt[1].x - iSize, m_Measure3P.Pt[1].y - iSize, m_Measure3P.Pt[1].x + iSize, m_Measure3P.Pt[1].y + iSize);
					double fDist = m_fValueScale_PositionGraph * sqrt((m_Measure3P.Pt[2].x - m_Measure3P.Pt[1].x)*(m_Measure3P.Pt[2].x - m_Measure3P.Pt[1].x) + (m_Measure3P.Pt[2].y - m_Measure3P.Pt[1].y)*(m_Measure3P.Pt[2].y - m_Measure3P.Pt[1].y));
					m_strTemp.Format("[Distance] Dist:%.0f  (X:%.0f   Y:%.0f)", fDist, m_fValueScale_PositionGraph* (m_Measure3P.Pt[2].x - m_Measure3P.Pt[1].x), m_fValueScale_PositionGraph * (m_Measure3P.Pt[2].y - m_Measure3P.Pt[1].y));
					SetDlgItemText(IDC_Temp, m_strTemp);
				}
				else {
					double x1 = m_Measure3P.Pt[1].x;
					double x2 = m_Measure3P.Pt[2].x;
					double x3 = m_Measure3P.Pt[3].x;
					double y1 = m_Measure3P.Pt[1].y;
					double y2 = m_Measure3P.Pt[2].y;
					double y3 = m_Measure3P.Pt[3].y;

					double a1 = x1 - x2;
					double b1 = y1 - y2;
					double c1 = x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1;
					double a2 = x2 - x3;
					double b2 = y2 - y3;
					double c2 = x3 * x3 - x2 * x2 + y3 * y3 - y2 * y2;

					double m = b1 * a2 - b2 * a1;
					if (m != 0) {
						double b = (c1 * a2 - c2 * a1) / m;
						if (a1 != 0) {
							double a = (c1 - b1 * b) / a1;
							double c = -x1 * x1 - a * x1 - y1 * y1 - b * y1;

							int x0 = (int)(-a / 2);
							int y0 = (int)(-b / 2);
							int r = (int)(a* a / 4. + b * b / 4. - c);

							if (r > 0) {
								r = (int)sqrt(r);
								m_MemDC_Position.SelectObject(&m_PenMark[0]);
								m_MemDC_Position.MoveTo(x0 - r / 5, y0);
								m_MemDC_Position.LineTo(x0 + r / 5, y0);
								m_MemDC_Position.MoveTo(x0, y0 - r / 5);
								m_MemDC_Position.LineTo(x0, y0 + r / 5);

								m_MemDC_Position.SelectObject(&m_PenPosition[1]);
								m_pBrushNull = m_MemDC_Position.SelectObject(&m_BrushNull);
								m_MemDC_Position.Ellipse(x0 - r, y0 - r, x0 + r, y0 + r);
								m_MemDC_Position.SelectObject(m_pBrushNull);

								m_MemDC_Position.SelectObject(&m_PenPosition[0]);
								m_MemDC_Position.SelectObject(&m_BrushAdjust[1]);
								m_MemDC_Position.Ellipse(m_Measure3P.Pt[3].x - iSize, m_Measure3P.Pt[3].y - iSize, m_Measure3P.Pt[3].x + iSize, m_Measure3P.Pt[3].y + iSize);
								m_MemDC_Position.Ellipse(m_Measure3P.Pt[2].x - iSize, m_Measure3P.Pt[2].y - iSize, m_Measure3P.Pt[2].x + iSize, m_Measure3P.Pt[2].y + iSize);
								m_MemDC_Position.Ellipse(m_Measure3P.Pt[1].x - iSize, m_Measure3P.Pt[1].y - iSize, m_Measure3P.Pt[1].x + iSize, m_Measure3P.Pt[1].y + iSize);
								m_strTemp.Format("[Circle]  X:%d  Y:%d r:%.0f", m_fMinValueX_PositionGraph + (x0 - m_iPositionGraphRangeX[0]) * (m_fMaxValueX_PositionGraph - m_fMinValueX_PositionGraph) / (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]), m_fMinValueY_PositionGraph - (y0 - m_iPositionGraphRangeY[1]) * (m_fMaxValueY_PositionGraph - m_fMinValueY_PositionGraph) / (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]), m_fValueScale_PositionGraph * r);
								SetDlgItemText(IDC_Temp, m_strTemp);
							}
						}
					}
				}
				break;
			default: break;
			}

			CString str = "측정";
			m_MemDC_Position.SetTextColor(RGB(0, 0, 255));
			m_MemDC_Position.SetBkMode(TRANSPARENT);
			m_MemDC_Position.SelectObject(&m_PenData[1]);
			m_MemDC_Position.SelectObject(&m_BrushPosition[1]);
			m_MemDC_Position.Rectangle(m_iPositionGraphRangeX[1] - 35, m_iPositionGraphRangeY[1] - 25, m_iPositionGraphRangeX[1] - 2, m_iPositionGraphRangeY[1] - 2);
			m_MemDC_Position.TextOutA(m_iPositionGraphRangeX[1] - 34, m_iPositionGraphRangeY[1] - 20, str);
		}

		if (m_bUseAutoScaleGraphPosition) {
			CString str = "AUTO";
			m_MemDC_Position.SetTextColor(RGB(0, 0, 255));
			m_MemDC_Position.SetBkMode(TRANSPARENT);
			m_MemDC_Position.SelectObject(&m_PenData[1]);
			m_MemDC_Position.SelectObject(&m_BrushPosition[1]);
			m_MemDC_Position.Rectangle(m_iPositionGraphRangeX[1] - 35, m_iPositionGraphRangeY[1] - 50, m_iPositionGraphRangeX[1] - 2, m_iPositionGraphRangeY[1] - 28);
			m_MemDC_Position.TextOutA(m_iPositionGraphRangeX[1] - 35, m_iPositionGraphRangeY[1] - 46, str);
		}

		m_MemDC.BitBlt(m_iPositionGraphRangeX[0] - m_iDev, m_iPositionGraphRangeY[0] - m_iDev, m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0] + 2 * m_iDev + 2, m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0] + 2 * m_iDev + 2, &m_MemDC_Position, m_iPositionGraphRangeX[0] - m_iDev, m_iPositionGraphRangeY[0] - m_iDev, SRCCOPY);

		if (m_bReqAutoScalePositionDraw || m_bUseAutoScaleGraphPosition) {
			m_bReqAutoScalePositionDraw = FALSE;

			if (fMaxX == fMinX) {
				fMaxX++;
				fMinX--;
			}

			if (fMaxY == fMinY) {
				fMaxY++;
				fMinY--;
			}

			if (fMaxX > fMinX &&  fMaxY > fMinY) {
				if (m_iPositionGraphRangeX[1] != m_iPositionGraphRangeX[0] && m_iPositionGraphRangeY[1] != m_iPositionGraphRangeY[0]) {
					// Save Current Value;
					double fValueScale_PositionGraph;
					double fMidValueX_PositionGraph;
					double fMidValueY_PositionGraph;

					double fScaleX = ((double)fMaxX - fMinX) / (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]);
					double fScaleY = ((double)fMaxY - fMinY) / (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]);
					fValueScale_PositionGraph = max(fScaleX, fScaleY) * 1.2;
					fMidValueX_PositionGraph = (fMaxX + fMinX) / 2;
					fMidValueY_PositionGraph = (fMaxY + fMinY) / 2;

					if (fMidValueX_PositionGraph > 0 && fMidValueX_PositionGraph < 0) bPreserveUpdateGraph = TRUE;
					if (fMidValueX_PositionGraph < 0 && fMidValueX_PositionGraph > 0) bPreserveUpdateGraph = TRUE;
					if (fMidValueY_PositionGraph > 0 && fMidValueY_PositionGraph < 0) bPreserveUpdateGraph = TRUE;
					if (fMidValueY_PositionGraph < 0 && fMidValueY_PositionGraph > 0) bPreserveUpdateGraph = TRUE;
					if (fValueScale_PositionGraph * 0.95 > m_fValueScale_PositionGraph || fValueScale_PositionGraph * 1.1 < m_fValueScale_PositionGraph) bPreserveUpdateGraph = TRUE;
					if (0.95 * abs(fMidValueX_PositionGraph) > abs(m_fMidValueX_PositionGraph) || 1.1 * abs(fMidValueX_PositionGraph) < abs(m_fMidValueX_PositionGraph)) bPreserveUpdateGraph = TRUE;
					if (0.95 * abs(fMidValueY_PositionGraph) > abs(m_fMidValueY_PositionGraph) || 1.1 * abs(fMidValueY_PositionGraph) < abs(m_fMidValueY_PositionGraph)) bPreserveUpdateGraph = TRUE;
					if (bPreserveUpdateGraph) {
						m_fValueScale_PositionGraph = fValueScale_PositionGraph;
						m_fMidValueX_PositionGraph = fMidValueX_PositionGraph;
						m_fMidValueY_PositionGraph = fMidValueY_PositionGraph;
						CalculationScale();
					}
				}
			}
		}
	}
	Wait(0);
	DrawTimeline();
	m_MemDC.SetBkColor(RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]));
	// Select Data & Cursor Value 
	if (m_iProgramMode == DATA_ANALYZE_MODE && m_bShowLabelValue) {
		if (m_bDrawSelectData) DrawSelectDataLabel(m_iHistorySelectIndex, iStartIndex);
		if (m_Cursor.nCursor > 0) DrawCursorInfo();
		m_MemDC.SetTextColor(RGB(0, 0, 0));
		m_MemDC.SelectObject(&m_FontDefault);
	}
	Wait(0);
	// MemDC --> DC
	CClientDC dc(this);
	dc.BitBlt(m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[0][0], m_iWndSizeX - m_iDrawGraphRangeX[0], m_iWndSizeY - m_iDrawGraphRangeY[0][0], &m_MemDC, m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[0][0], SRCCOPY);

	// Panning 
	if (m_bDisplayPosition && m_Panning_PositionGraph.isPressed) {
		dc.SelectObject(&m_PenData[1]);
		dc.MoveTo(m_Panning_PositionGraph.P_Start.x, m_Panning_PositionGraph.P_Start.y);
		dc.LineTo(m_Panning_PositionGraph.P_Last.x, m_Panning_PositionGraph.P_Last.y);
	}

	// Measure 
	if (m_bDisplayPosition && m_Measure_PositionGraph.isPressed) {
		dc.SelectObject(&m_PenData[0]);
		dc.MoveTo(m_Measure_PositionGraph.P_Start.x, m_Measure_PositionGraph.P_Start.y);
		dc.LineTo(m_Measure_PositionGraph.P_Last.x, m_Measure_PositionGraph.P_Last.y);
	}

	// Resize
	if (m_bDisplayPosition && m_Resize_PositionGraph.isPressed) {
		dc.MoveTo(m_Resize_PositionGraph.P_Start);
		dc.LineTo(m_Resize_PositionGraph.P_Start.x, m_Resize_PositionGraph.P_Last.y);
		dc.LineTo(m_Resize_PositionGraph.P_Last);
		dc.LineTo(m_Resize_PositionGraph.P_Last.x, m_Resize_PositionGraph.P_Start.y);
		dc.LineTo(m_Resize_PositionGraph.P_Start);		// 
	}

	if (m_bIsAdjustMode) {
		int KeyMode = -1;
		int i = m_iAdjustLegendY;
		if (m_bAdjustLegendY[m_iAdjustLegendY]) {
			if (CheckShiftKeyDown()) {
				KeyMode = 1;
			}
			else if (CheckControlKeyDown()) {
				KeyMode = 2;
			}

			dc.SelectObject(&m_PenAdjust);
			if (KeyMode == -1 || KeyMode == 1) {
				dc.MoveTo(m_iDrawGraphRangeX[0] + 1, m_iDrawGraphRangeY[i][0] + 1);
				dc.LineTo(m_iDrawGraphRangeX[1] - 1, m_iDrawGraphRangeY[i][0] + 1);
				dc.LineTo(m_iDrawGraphRangeX[1] - 1, m_iDrawGraphRangeY[i][1] - 1);
				dc.LineTo(m_iDrawGraphRangeX[0] + 1, m_iDrawGraphRangeY[i][1] - 1);
				dc.LineTo(m_iDrawGraphRangeX[0] + 1, m_iDrawGraphRangeY[i][0] + 1);
			}
			if (KeyMode == -1 || KeyMode == 2) {
				dc.MoveTo(m_iDrawGraphRangeX[0] + 1, m_iDrawGraphRangeY[i + 1][0] + 1);
				dc.LineTo(m_iDrawGraphRangeX[1] - 1, m_iDrawGraphRangeY[i + 1][0] + 1);
				dc.LineTo(m_iDrawGraphRangeX[1] - 1, m_iDrawGraphRangeY[i + 1][1] - 1);
				dc.LineTo(m_iDrawGraphRangeX[0] + 1, m_iDrawGraphRangeY[i + 1][1] - 1);
				dc.LineTo(m_iDrawGraphRangeX[0] + 1, m_iDrawGraphRangeY[i + 1][0] + 1);
			}
		}
	}

	if (m_bUseStatistics) {
		BLENDFUNCTION bf;

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 20; //0:투명 ~ 255:불투명
		bf.AlphaFormat = 0;
		dc.AlphaBlend(m_iAdjustSelectPoint[0], m_iDrawGraphRangeY[0][0], m_iAdjustSelectPoint[1] - m_iAdjustSelectPoint[0], m_iDrawGraphRangeY[m_iSelectGraphCount][1] - m_iDrawGraphRangeY[0][0], &m_MemDC_Range, 0, 0, 100, 100, bf);

		if (m_iAdjustSelectMode == 0) {
			dc.SelectObject(&m_PenMark[0]);
			dc.SelectObject(&m_BrushMark[0]);
		}
		else {
			dc.SelectObject(&m_PenMark[1]);
			dc.SelectObject(&m_BrushMark[1]);
		}

		CPoint Pt[4];

		Pt[0].SetPoint(m_iAdjustSelectPoint[0], m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 3);
		Pt[1].SetPoint(m_iAdjustSelectPoint[0], m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 15);
		Pt[2].SetPoint(m_iAdjustSelectPoint[0] + 12, m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 15);
		dc.Polygon(Pt, 3);

		for (int i = 0; i <= m_iSelectGraphCount; i++) {
			dc.MoveTo(m_iAdjustSelectPoint[0], m_iDrawGraphRangeY[i][0]);
			dc.LineTo(m_iAdjustSelectPoint[0], m_iDrawGraphRangeY[i][1]);
			if (m_iAdjustSelectMode != 0) {
				dc.LineTo(max(m_iDrawGraphRangeX[0], m_iAdjustSelectPoint[0] - 1), m_iDrawGraphRangeY[i][1]);
				dc.LineTo(max(m_iDrawGraphRangeX[0], m_iAdjustSelectPoint[0] - 1), m_iDrawGraphRangeY[i][0]);
				dc.LineTo(min(m_iDrawGraphRangeX[1], m_iAdjustSelectPoint[0] + 1), m_iDrawGraphRangeY[i][0]);
				dc.LineTo(min(m_iDrawGraphRangeX[1], m_iAdjustSelectPoint[0] + 1), m_iDrawGraphRangeY[i][1]);
			}
		}

		if (m_iAdjustSelectMode == 1) {
			dc.SelectObject(&m_PenMark[0]);
			dc.SelectObject(&m_BrushMark[0]);
		}
		else {
			dc.SelectObject(&m_PenMark[1]);
			dc.SelectObject(&m_BrushMark[1]);
		}

		int d = m_iAdjustSelectPoint[1] - m_iDrawGraphRangeX[0];
		if (d >= 12) {
			Pt[0].SetPoint(m_iAdjustSelectPoint[1], m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 3);
			Pt[1].SetPoint(m_iAdjustSelectPoint[1], m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 15);
			Pt[2].SetPoint(m_iAdjustSelectPoint[1] - 12, m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 15);
			dc.Polygon(Pt, 3);
		}
		else {
			Pt[0].SetPoint(m_iAdjustSelectPoint[1], m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 3);
			Pt[1].SetPoint(m_iAdjustSelectPoint[1], m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 15);
			Pt[2].SetPoint(m_iAdjustSelectPoint[1] - d, m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 15);
			Pt[3].SetPoint(m_iAdjustSelectPoint[1] - d, m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 3 + d);
			dc.Polygon(Pt, 4);
		}

		for (int i = 0; i <= m_iSelectGraphCount; i++) {
			dc.MoveTo(m_iAdjustSelectPoint[1], m_iDrawGraphRangeY[i][0]);
			dc.LineTo(m_iAdjustSelectPoint[1], m_iDrawGraphRangeY[i][1]);
			if (m_iAdjustSelectMode != 1) {
				dc.LineTo(max(m_iDrawGraphRangeX[0], m_iAdjustSelectPoint[1] - 1), m_iDrawGraphRangeY[i][1]);
				dc.LineTo(max(m_iDrawGraphRangeX[0], m_iAdjustSelectPoint[1] - 1), m_iDrawGraphRangeY[i][0]);
				dc.LineTo(min(m_iDrawGraphRangeX[1], m_iAdjustSelectPoint[1] + 1), m_iDrawGraphRangeY[i][0]);
				dc.LineTo(min(m_iDrawGraphRangeX[1], m_iAdjustSelectPoint[1] + 1), m_iDrawGraphRangeY[i][1]);
			}
		}
	}

	if (!bPreserveUpdateGraph && (!bReqGraphUpdate4ChangeMinMax || m_bUseStatistics)) m_bReqGraphUpdate = FALSE;

	UDF;
	bIsBusy = FALSE;
}


void CSerialComDlg::SaveLog(CString AddMsg, int iCommand) {
	static CString strMsg = "";

	if (iCommand == 0) {

		CString str;
		SYSTEMTIME time;
		GetLocalTime(&time);

		str.Format("%d %d %d %d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, (LPCTSTR)AddMsg);
		strMsg += str;
		int iSize = strMsg.GetLength();

		if (iSize > 10000) {
			if (SaveFile.m_hFile == INVALID_HANDLE_VALUE) {
				CString strFileName;
				SYSTEMTIME time;
				GetLocalTime(&time);

				CString strPort;
				strPort.Format("%s", (LPCTSTR)strPort1name);
				strPort.Replace("\\", "");
				strPort.Replace(".", "");

				if (strPort1name == "")
					strFileName.Format("Data_%02d%02d%02d_%02d%02d%02d.csv", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
				else
					strFileName.Format("Data_%02d%02d%02d_%02d%02d%02d_%s.csv", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, (LPCTSTR)strPort);

				SaveFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);
				// Header 저장
				str.Format("#Header:%c\r\n", m_cPacketHead);
				SaveFile.Write(str, str.GetLength());
			}

			SaveFile.Write(strMsg, iSize);
			strMsg = "";
		}
	}
	else {
		// File Close
		if (SaveFile.m_hFile != INVALID_HANDLE_VALUE)
		{
			SaveFile.Write(strMsg, strMsg.GetLength());
			strMsg = "";

			SaveFile.Close();
		}
	}
}


void CSerialComDlg::OnChangeRange() {
	double fRangeMax[MAX_GRAPH_COUNT], fRangeMin[MAX_GRAPH_COUNT];

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		fRangeMax[iGraph]=  m_fRangeMax[iGraph];
		fRangeMin[iGraph] = m_fRangeMin[iGraph];
	}
	UDT;

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		double fPreMin = m_fRangeMin[iGraph];
		double fPreMax = m_fRangeMax[iGraph];

		m_fRangeMin[iGraph] = atof(m_strRangeMin[iGraph]);
		if (fPreMin != m_fRangeMin[iGraph]) {
			if (m_fRangeMin[iGraph] == m_fRangeMax[iGraph]) {
				m_fRangeMin[iGraph]--;
			}
			m_strRangeMid[iGraph].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMin[iGraph] + m_fRangeMax[iGraph]) / 2));
		}

		m_fRangeMax[iGraph] = atof(m_strRangeMax[iGraph]);
		if (fPreMax != m_fRangeMax[iGraph]) {
			if (m_fRangeMin[iGraph] == m_fRangeMax[iGraph]) {
				m_fRangeMax[iGraph]++;
			}
			m_strRangeMid[iGraph].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMin[iGraph] + m_fRangeMax[iGraph]) / 2.));
		}

		// m_RealGrpahInfo에 Update
		int ID = m_RealGrpahInfo[iGraph].iDraw2RealGraphID;
		m_RealGrpahInfo[ID].fRangeMax = m_fRangeMax[iGraph];
		m_RealGrpahInfo[ID].fRangeMin = m_fRangeMin[iGraph];
		if (fRangeMax[iGraph] != m_fRangeMax[iGraph] || fRangeMin[iGraph] != m_fRangeMin[iGraph]) m_bAutoRange_Graph[iGraph] = FALSE;
		m_RealGrpahInfo[ID].bAutoRange_Graph = m_bAutoRange_Graph[iGraph];
	}
	UDF;
	m_bReqGraphUpdate = TRUE;
}


void CSerialComDlg::OnBnClickedDefaultbutton() {
	m_iProgramMode = 1 - m_iProgramMode;
	m_bUpdateList = m_iProgramMode;
	ProgramModeUpdate();
    m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::ProgramModeUpdate(void) {
	if (m_iProgramMode == DATA_ACQUISITION_MODE) {
		((CButton*)GetDlgItem(IDC_DefaultButton))->SetWindowTextA("데이터 분석 모드");
		SetDlgItemInt(IDC_IndexNumber, max(0, m_iHistoryIndex - 1));
		GetDlgItem(IDC_IndexNumber)->EnableWindow(FALSE);
		m_bUseStatistics = FALSE;
		if (m_pStatistics == NULL || m_pStatistics->m_hWnd != NULL) {
			// m_pStatistics->CloseWindow();
			delete m_pStatistics;
			m_pStatistics = NULL;
		}
		m_bIsMoveMode = FALSE;
	}
	else {
		((CButton*)GetDlgItem(IDC_DefaultButton))->SetWindowTextA("그래프 표시 모드");
		GetDlgItem(IDC_IndexNumber)->EnableWindow(TRUE);
	}
	m_bReqGraphUpdate = TRUE;
}


void CSerialComDlg::SetReg_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData) {
	HKEY hkey = NULL;
	DWORD dwDisp;

	CString strRegDirectory;
	if (m_iSelectModel > 0) {
		CString strModel;
		m_cSelectModel.GetLBText(m_iSelectModel, strModel);
		strRegDirectory.Format("%sModel\\%s\\%s", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strModel, (LPCTSTR)strAddress);
	}
	else {
		strRegDirectory.Format("%s%s", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strAddress);
	}

	const unsigned char *str;
	unsigned char buff[1024];
	memcpy(buff, (unsigned char *)(LPCTSTR)stData, strlen(stData));
	str = buff;

	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	while (result != ERROR_SUCCESS) {
		// Key가 없다면
		::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
		result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	}

	::RegSetValueEx(hkey, strItem, 0, REG_SZ, str, strlen(stData));
	::RegCloseKey(hkey);
	::RegFlushKey(hkey);
}


CString CSerialComDlg::GetReg_RegistryData(const CString& strAddress, const CString& strItem) {
	HKEY hkey = NULL;
	DWORD dwDisp;

	CString strRegDirectory;

	if (m_iSelectModel > 0) {
		CString strModel;
		m_cSelectModel.GetLBText(m_iSelectModel, strModel);
		strRegDirectory.Format("%sModel\\%s\\%s", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strModel, (LPCTSTR)strAddress);
	}
	else {
		strRegDirectory.Format("%s%s", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strAddress);
	}

	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);

	if (result != ERROR_SUCCESS) {
		// Key가 없다면
		::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
		result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	}

	CString strValue;
	DWORD dwType = REG_SZ;
	DWORD dwCount = _MAX_PATH;

	::RegQueryValueEx(hkey, strItem, NULL, &dwType, (LPBYTE)strValue.GetBuffer(_MAX_PATH), &dwCount);
	::RegCloseKey(hkey);
	::RegFlushKey(hkey);

    if (strValue == "") {
        // Find mode
        CString str = "";
        int len = strAddress.GetLength();
        int pos = strAddress.Find("\\Mode_");
        str.Format("%s", (LPCTSTR)strAddress);
        if (pos != -1 && pos <= len - 7 && str.GetAt(pos+6) != '0') {
            str.SetAt(pos + 6, '0');
            strValue = GetReg_RegistryData(str, strItem);

            if (strValue == "") {
                str.SetAt(pos, 0);
                strValue = GetReg_RegistryData(str, strItem);
            }
        }
    }
	return strValue;
}


void CSerialComDlg::SetRegRoot_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData) {
	HKEY hkey = NULL;
	DWORD dwDisp;

	CString strRegDirectory;
	strRegDirectory.Format("%s%s", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strAddress);

	const unsigned char *str;
	unsigned char buff[1024];
	memcpy(buff, (unsigned char *)(LPCTSTR)stData, strlen(stData));
	str = buff;

	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	while (result != ERROR_SUCCESS) {
		// Key가 없다면
		::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
		result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	}

	::RegSetValueEx(hkey, strItem, 0, REG_SZ, str, strlen(stData));
	::RegCloseKey(hkey);
	::RegFlushKey(hkey);
}


CString CSerialComDlg::GetRegRoot_RegistryData(const CString& strAddress, const CString& strItem) {
	HKEY hkey = NULL;
	DWORD dwDisp;

	CString strRegDirectory;
	strRegDirectory.Format("%s%s", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strAddress);

	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);

	if (result != ERROR_SUCCESS) {
		// Key가 없다면
		::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
		result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	}

	CString str;
	DWORD dwType = REG_SZ;
	DWORD dwCount = _MAX_PATH;

	::RegQueryValueEx(hkey, strItem, NULL, &dwType, (LPBYTE)str.GetBuffer(_MAX_PATH), &dwCount);
	::RegCloseKey(hkey);
	::RegFlushKey(hkey);
	return str;
}

void CSerialComDlg::OnBnClickedSaveconfiguration() {
	if (AfxMessageBox("현재의 정보로 저장합니까(기존 정보 삭제됨)\r\nSave Current Setting(Remove Previous Setting) ", MB_YESNO) == IDYES) {	
		UDT;

		CString strAddress, strItem, strValue, strPresetMode;

		//  Data / Math page
		strValue.Format("%d", m_iDataPage);
		SetRegRoot_RegistryData("Common", "nDataPage", strValue);
		strValue.Format("%d", m_iMathPage);
		SetRegRoot_RegistryData("Common","nMathPage", strValue);	
        strValue.Format("%d", m_iHistoryCapacity);
        SetRegRoot_RegistryData("Common", "nHistoryCapacity", strValue);

		// Preset Mode
		if (m_iPresetMode == 0) {
			strPresetMode = "";
		}
		else {
			strPresetMode.Format("\\Mode_%d", m_iPresetMode);
		}
		
		for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
			if (m_strPacketHeader == " " && m_strPacketHeader == "")
				strAddress.Format("Graph\\Graph%d", iGraph);
			else
				strAddress.Format("Graph\\Graph%d\\%c%d%s", iGraph, m_cPacketHead, m_cPacketHead, (LPCTSTR)strPresetMode);

			for (int iData = 0; iData < m_iMaxDataSize; iData++) {
				strItem.Format("ViewData%02d", iData);

				if (m_bSelectDraw_GraphData[iGraph][iData])
					SetReg_RegistryData(strAddress, strItem, "1");
				else
					SetReg_RegistryData(strAddress, strItem, "0");
			}

			//m_bDrawGraph
			strItem.Format("DrawGraph");
			if (m_bDrawGraph[iGraph])
				SetReg_RegistryData(strAddress, strItem, "1");
			else
				SetReg_RegistryData(strAddress, strItem, "0");

			CheckDlgButton(m_IDC_DrawGraph[iGraph], m_bDrawGraph[iGraph]);
		}

		// Math Probe		
		for (int i = 0; i < MAXIMUM_MATH_SIZE; i++) {
			if (m_strPacketHeader == " " || m_strPacketHeader == "")
				strAddress.Format("MathDefine\\MathProbe%d\\default", i);
			else
				strAddress.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);

			strItem.Format("Data1");
			strValue.Format("%d", m_MathData[i].iDataID1);
			SetReg_RegistryData(strAddress, strItem, strValue);			

			strItem.Format("Data2");
			strValue.Format("%d", m_MathData[i].iDataID2);
			SetReg_RegistryData(strAddress, strItem, strValue);

			strItem.Format("Operator");
			strValue.Format("%d", m_MathData[i].iOperator);
			SetReg_RegistryData(strAddress, strItem, strValue);

			strItem.Format("PinName");
			strValue.Format("%s", (LPCTSTR)m_MathData[i].strName);
			strValue.Replace("[", "");
			strValue.Replace("]", "");

			SetReg_RegistryData(strAddress, strItem, strValue);

			strItem.Format("DataSize");
			strValue.Format("%d", m_MathData[i].nData);
			SetReg_RegistryData(strAddress, strItem, strValue);

			strItem.Format("ConstValue");
			strValue.Format("%f", m_MathData[i].fValue);
			SetReg_RegistryData(strAddress, strItem, strValue);

            strItem.Format("Result_Integer");
            strValue.Format("%d", m_MathData[i].bResInt);
            SetReg_RegistryData(strAddress, strItem, strValue);

			for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
				strItem.Format("DrawGraph%d", j);
				if (m_MathData[i].bDisplayGraph[j])					//
					SetReg_RegistryData(strAddress, strItem, "1");
				else
					SetReg_RegistryData(strAddress, strItem, "0");
			}

			strAddress.Format("MathDefine\\MathProbe%d", i);

			strItem.Format("Red");
			strValue.Format("%d", m_MathData[i].iColor[0]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);

			strItem.Format("Green");
			strValue.Format("%d", m_MathData[i].iColor[1]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);

			strItem.Format("Blue");
			strValue.Format("%d", m_MathData[i].iColor[2]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);

			strItem.Format("UseMark");
			strValue.Format("%d", m_MathData[i].bUseMark);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}

		// Display Time Mode
		strAddress.Format("Graph");
		strItem.Format("TimeDisplayMode");
		strValue.Format("%d", m_iTimeDisplayMode);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// No. of Graph
		// m_iGraphCount = m_iSelectGraphCount + 1;
		strAddress.Format("Graph");
		strItem.Format("Number_Graph");
		strValue.Format("%d", m_iGraphCount);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);
		
		// Pin Name
		if (m_strPacketHeader != " " &&  m_strPacketHeader != "") {
            strAddress.Format("Header\\%c%d", m_cPacketHead, m_cPacketHead);			
			for (int iData = 0; iData < m_iMaxDataSize; iData++) {
				strItem.Format("PinName%02d", iData);
				SetReg_RegistryData(strAddress, strItem, m_strPinName[iData]);
			}
		}

		// Min-Max Value
		for (int i = 0; i < m_iGraphCount; i++) {
			int ID = m_RealGrpahInfo[i].iDraw2RealGraphID;
			m_RealGrpahInfo[ID].fRangeMax = m_fRangeMax[i];
			m_RealGrpahInfo[ID].fRangeMin = m_fRangeMin[i];
			m_RealGrpahInfo[ID].bAutoRange_Graph = m_bAutoRange_Graph[i];
		}

		for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
			if (m_strPacketHeader == " " || m_strPacketHeader == "")
				strAddress.Format("Graph\\Graph%d", iGraph);
			else
				strAddress.Format("Graph\\Graph%d\\%c%d%s", iGraph, m_cPacketHead, m_cPacketHead, (LPCTSTR)strPresetMode);

			strItem.Format("MaxValue");
			strValue.Format("%s", (LPCTSTR)Dbl2Str(m_RealGrpahInfo[iGraph].fRangeMax));
			SetReg_RegistryData(strAddress, strItem, strValue);

			strItem.Format("MinValue");
			strValue.Format("%s", (LPCTSTR)Dbl2Str(m_RealGrpahInfo[iGraph].fRangeMin));
			SetReg_RegistryData(strAddress, strItem, strValue);

			strItem.Format("AutoRange");
			if (m_RealGrpahInfo[iGraph].bAutoRange_Graph)
				SetReg_RegistryData(strAddress, strItem, "1");
			else
				SetReg_RegistryData(strAddress, strItem, "0");
		}

		//Filter view
		for (int i = 0; i < MAX_FILTER; i++) {
			strAddress.Format("Filter\\FilterView%d", i);
			strItem.Format("UseFilter");
			if (m_bUseFilterItem[i])
				SetRegRoot_RegistryData(strAddress, strItem, "1");
			else
				SetRegRoot_RegistryData(strAddress, strItem, "0");

			strItem.Format("FilterName");
			SetRegRoot_RegistryData(strAddress, strItem, m_strFilterItem[i]);
		}

		// Serial Command
		strAddress.Format("Command\\Button");
		for (int i = 0; i < MAX_BT_COMMAND; i++) {
			strItem.Format("Command%d", i);
			strValue.Format("%d", m_BT_Command[i]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}

		// PositionGraph 
		strAddress = "Command\\PositionGraph";
		strItem = "Header";

		SetRegRoot_RegistryData(strAddress, strItem, m_strPositonGraphHeader);

		// Use Position Graph 
		strItem = "UsePositionGraph";
		if (m_bUsePositionGraph)
			SetRegRoot_RegistryData(strAddress, strItem, "1");
		else
			SetRegRoot_RegistryData(strAddress, strItem, "0");

		// Select data for X-dir in Position Graph 
		strItem = "SelectPositionGraphX";
		strValue.Format("%d", m_iSelectPositionGraphX);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Select data for Y-dir in Position Graph 
		strItem = "SelectPositionGraphY";
		strValue.Format("%d", m_iSelectPositionGraphY);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Position Graph SIze
		strItem = "FrameSize";
		strValue.Format("%d", max(20, min(m_iFrameSizeGraphPosition, m_iWndSizeX - 700)));
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Pen Color
		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			strAddress.Format("Graph\\PenColor\\PEN_%d", iData);
			for (int iColor = 0; iColor < 3; iColor++) {
				switch (iColor) {
				case 0: strItem.Format("Red"); break;
				case 1: strItem.Format("Green"); break;
				case 2: strItem.Format("Blue"); break;
				}

				strValue.Format("%d", m_iPenColor[iData][iColor]);
				SetRegRoot_RegistryData(strAddress, strItem, strValue);
			}
			strItem.Format("PenType");
			strValue.Format("%d", m_iPenType[iData]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);

			strItem.Format("Thickness");
			strValue.Format("%d", m_iPenThickness[iData]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);

			strItem.Format("UseMark");
			strValue.Format("%d", m_bUseDataMark[iData]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}

		for (int iActive = 0; iActive < 2; iActive++) {
			CString strMode = "";
			if (iActive == 0) strMode = "Inactive"; else strMode = "Active";
			for (int iColor = 0; iColor < 3; iColor++) {
				switch (iColor) {
				case 0: strItem.Format("Red"); break;
				case 1: strItem.Format("Green"); break;
				case 2: strItem.Format("Blue"); break;
				}
				strAddress.Format("Graph\\SelectControl\\%s\\Blank", (LPCTSTR)strMode);
				strValue.Format("%d", m_iRGB_SelectControl_Blank[iColor][iActive]);
				SetRegRoot_RegistryData(strAddress, strItem, strValue);

				strAddress.Format("Graph\\SelectControl\\%s\\Outline", (LPCTSTR)strMode);
				strValue.Format("%d", m_iRGB_SelectControl_Outline[iColor][iActive]);
				SetRegRoot_RegistryData(strAddress, strItem, strValue);

				strAddress.Format("Graph\\SelectControl\\%s\\Select", (LPCTSTR)strMode);
				strValue.Format("%d", m_iRGB_SelectControl_Select[iColor][iActive]);
				SetRegRoot_RegistryData(strAddress, strItem, strValue);
			}
		}

		// Display size & time stamp
		strAddress.Format("Graph");
		strItem.Format("DisplaySize");
        strValue.Format("%d", m_iDisplaySize);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Max Fit Screen size
		strAddress.Format("Graph");
		strItem.Format("MaxScreenFitSize");
		strValue.Format("%d", m_iMaxFitScreen);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Use Fit to Max Data
		strAddress.Format("Graph");
		strItem.Format("UseMaxScreenFit");
		strValue.Format("%d", IsDlgButtonChecked(IDC_FitToMaxData));
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Graph Frame
		strAddress.Format("Graph");
		strItem.Format("FixedGraphFrame");
		if (m_bFixGraphFrame)
			SetRegRoot_RegistryData(strAddress, strItem, "1");
		else
			SetRegRoot_RegistryData(strAddress, strItem, "0");

		// Legend Frame Size
		strAddress.Format("Graph");
		strItem = "LegendFrameSize";		
		strValue.Format("%d", m_iLegendX);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Legend Frame Size
		strAddress.Format("Graph");
		strItem = "LegendFrameSize_PositionGraph";
		strValue.Format("%d", m_iLegendX_PositionGraph);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Graph Mark Size
		strAddress.Format("Graph");
		strItem = "MarkSize";
		strValue.Format("%d", m_iMarkSize);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Log Filter Ratio
		strAddress.Format("Graph");
		strItem = "Log_Filter_Ratio";
		strValue.Format("%d", min(100, max(0, m_iRatioLogFilter)));
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Log Filter Width
		strAddress.Format("Graph");
		strItem = "Log_Filter_Width";
		strValue.Format("%d", m_iRangeLogFilterWidth);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Show Data Size
		strAddress.Format("Graph");
		strItem = "Show_Data_Size";
		strValue.Format("%d", m_iShowDataControl);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

        strItem = "Data_Offset";
        strValue.Format("%d", m_iControlDataOffset);
        SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Display No of Math Probe
        strAddress.Format("Graph");
        strItem = "Show_Math_Size";
        strValue.Format("%d", m_iShowMathControl);
        SetRegRoot_RegistryData(strAddress, strItem, strValue);

        strItem = "Math_Offset";
        strValue.Format("%d", m_iControlMathOffset);
        SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Display Filter item
		strAddress.Format("Graph");
		strItem = "FilterViewMode";
		if (m_iFilterViewMode == 0) {
			strValue = "1";
		}
		else {
			strValue = "0";
		}
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

        // Display Filtered Log control
        strAddress.Format("Graph");
        strItem = "FilterLogViewMode";
        if (m_bShowFilterLogControl) {
            strValue = "1";
        }
        else {
            strValue = "0";
        }
        SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Grid 
		strAddress.Format("Graph");
		strItem = "GridCountX";
		strValue.Format("%d", m_iGridX);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem = "GridX_Type";
		strValue.Format("%d", m_iGridX_Type);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem = "GridCountY";
		strValue.Format("%d", m_iGridCountY);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		// Control Font size
		strAddress.Format("Common\\Font");
		for (int i = 0; i < MAX_FONT_SIZE; i++) {
			strItem.Format("FontSize_%d", i);
			strValue.Format("%d", m_iControlFontSize[i]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
			ChangeControlFontSize(i, m_iControlFontSize[i]);
		}

		strAddress.Format("Graph\\DataMultiplier");
		for (int iData = 0; iData < MAX_DATACONTROL_SIZE; iData++) {
			strItem.Format("Coeff%02d", iData);
			strValue.Format("%f", m_fCoff[iData]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}

		strAddress.Format("Common\\Data");
		strValue.Format("%d", m_bUseIntData);
		SetRegRoot_RegistryData(strAddress, "DataType", strValue);

		// Select Data Label
		strAddress.Format("Graph\\SelectData\\");
		
		strItem.Format("TextColorMode");
		strValue.Format("%d", m_iSelTextColorMode);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem.Format("BkColorMode");
		strValue.Format("%d", m_iSelBkColorMode);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		for (int i = 0; i < 3; i++) {
			strItem.Format("TextColor_%d", i);
			strValue.Format("%d", m_iSelTextColor[i]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);

			strItem.Format("BkColor_%d", i);
			strValue.Format("%d", m_iSelBkColor[i]);
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}
		strValue.Format("%d", max(0, m_iMaxDataSize - m_iMinPacketDataSize));
		SetRegRoot_RegistryData("Common\\", "MinPacketDataSizeDelta", strValue);

		strValue.Format("%d", max(0, m_iMaxPacketDataSize - m_iMaxDataSize));
		SetRegRoot_RegistryData("Common\\", "MaxPacketDataSizeDelta", strValue);

		strValue.Format("%d", m_iMaxDataDigit);
		SetRegRoot_RegistryData("Common\\", "MaxDataDigit", strValue);
		
		strValue.Format("%d", m_bGetSettingInLoading);
		SetRegRoot_RegistryData("Common\\", "GetSettingInLoading", strValue);

		// Auto Header
		strValue.Format("%d", m_bUseAutoHeader_rx_reg);
		SetRegRoot_RegistryData("Common", "UseAutoHeader", strValue);	

        strValue.Format("%d", m_iAutoHeader_rx_count);
        SetRegRoot_RegistryData("Common", "UseAutoHeaderCount", strValue);

		strValue.Format("%d", m_bUseAutoHeader_tx);
		SetRegRoot_RegistryData("Common", "UseAutoHeader_Tx", strValue);

		strValue.Format("%d", m_bShowResizeCursor);
		SetRegRoot_RegistryData("Common", "ShowResizeCursor", strValue);

		// 정지중에도 data 수신
		strValue.Format("%d", IsDlgButtonChecked(IDC_SaveRcvDataAll));
		SetRegRoot_RegistryData("Common", "Save_All_Received_Data", strValue);
			
		// Use Auto Save Option
		strValue.Format("%d", IsDlgButtonChecked(IDC_UseAutoSave));
		SetRegRoot_RegistryData("Common", "UseAutoSave", strValue);

		// Load settings in Save File Loading
		strValue.Format("%d", m_LoadSettingInfo_in_DataLoading);
		SetRegRoot_RegistryData("Common", "LoadSetting_in_savedDataLoading", strValue);

		// Background color
		for (int iMode = 0; iMode < 2; iMode++) {
			strAddress.Format("Common\\Color\\Background\\Mode%d", iMode);
			for (int i = 0; i < 3; i++) {
				strItem.Format("RGB_color_%d", i);
				strValue.Format("%d", m_iBackgroundColor[iMode][i]);

				SetRegRoot_RegistryData(strAddress, strItem, strValue);
			}			
		}

		// Text Color
		strAddress.Format("Common\\Color\\Text");
		for (int i = 0; i < 3; i++) {
			strItem.Format("RGB_color_%d", i);
			strValue.Format("%d", m_iTextColor[i]);

			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}

		// Probe : Data Value
		strValue.Format("%d", m_bShowProbeValue);
		SetRegRoot_RegistryData("Common\\Graph", "Show_Probe_Value", strValue);

		// Probe : Data ID Show
		strValue.Format("%d", m_bShowProbeDataID);
		SetRegRoot_RegistryData("Common\\Graph", "Show_Probe_DataID", strValue);

		// Probe : Data ID Show
		strValue.Format("%d", m_bShowTimelineN);
		SetRegRoot_RegistryData("Common\\Graph", "Show_Timeline_N", strValue);
			   
		UpdateMathDefine();

		DrawInit();
		m_bReqGraphUpdate = TRUE;

		// xbox input
		strValue.Format("%d", m_bUse_Xinput);
		SetRegRoot_RegistryData("Common\\Xinput", "Use_Xinput", strValue);

		// Mouse Wheel Dir
		strValue.Format("%d", m_bMouseWheelZoomDir);
		SetRegRoot_RegistryData("Common", "MouseWheelZoomDir", strValue);

		strValue.Format("%d", m_bMouseWheelShiftDir);
		SetRegRoot_RegistryData("Common", "MouseWheelShiftDir", strValue);

		// Auto Save
		strValue.Format("%d", m_bUseAutosave1_LogCountLimit);
		SetRegRoot_RegistryData("Common", "UseAutoSave_LogCountLimit", strValue);

		strValue.Format("%d", m_bUseAutosave2_Time_LogCount);
		SetRegRoot_RegistryData("Common", "UseAutoSave_Time_LogCount", strValue);

		strValue.Format("%d", m_iAutosave2_cond_Time);
		SetRegRoot_RegistryData("Common", "AutoSave_Timeout", strValue);

		strValue.Format("%d", m_iAutosave2_cond_LogCount);
		SetRegRoot_RegistryData("Common", "AutoSave_LogCount", strValue);

		strValue.Format("%d", m_bAutosave_AddCapture);
		SetRegRoot_RegistryData("Common", "UseAutoSave_Add_Capture", strValue);

        strValue.Format("%d", m_iLOG_Update_Period);
        SetRegRoot_RegistryData("Common", "LogCountrol_UpdatePeriod", strValue);

        // Graph height 
        for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
            if (m_strPacketHeader == " " || m_strPacketHeader == "")
                strAddress.Format("Graph\\Graph%d", iGraph);
            else
                strAddress.Format("Graph\\Graph%d\\%c%d%s", iGraph, m_cPacketHead, m_cPacketHead, (LPCTSTR)strPresetMode);

            strItem.Format("GraphHeight");
            strValue.Format("%d", m_RealGrpahInfo[iGraph].iHeight);
            SetReg_RegistryData(strAddress, strItem, strValue);
        }
	}    
}


void CSerialComDlg::LoadHeaderConfiguration() {
	BOOL bPreDisplayPosition = m_bDisplayPosition;		

	// Header 저장
	HKEY hkey;
	CString strAddress, strSavedHeader, strPresetMode, strItem, strValue, strModel;

	if (m_iSelectModel == 0)
		strModel = "";
	else {
		m_cSelectModel.GetLBText(m_iSelectModel, strValue);
		strModel.Format("Model\\%s\\", (LPCTSTR)strValue);
	}

	strAddress.Format("%s%sGraph0\\%s%d", DEFAULT_REGISTRY_SETTING_PATH, (LPCTSTR)strModel, (LPCTSTR)m_strPacketHeader, m_cPacketHead);
	
	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strAddress, &hkey);

	if (result != ERROR_SUCCESS || m_strPacketHeader == "") {
		// Key가 없다면
		strSavedHeader = "";
	}
	else {
		strSavedHeader.Format("%s", (LPCTSTR)m_strPacketHeader);
	}

	// Preset Mode
	if (m_iPresetMode == 0) {
		strPresetMode = "";
	}
	else {
		strPresetMode.Format("\\Mode_%d", m_iPresetMode);
	}
	m_ctrlMode.SetCurSel(m_iPresetMode);

	for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
		if (m_strPacketHeader == " " && m_strPacketHeader == "")
			strAddress.Format("Graph\\Graph%d", iGraph);
		else
			strAddress.Format("Graph\\Graph%d\\%c%d%s", iGraph, m_cPacketHead, m_cPacketHead, (LPCTSTR)strPresetMode);

		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			strItem.Format("ViewData%02d", iData);
			strValue = GetReg_RegistryData(strAddress, strItem);

            if (strValue == "1") {
                CString str;
                str = "";
                m_bSelectDraw_GraphData[iGraph][iData] = TRUE;
            }
            else {
                m_bSelectDraw_GraphData[iGraph][iData] = FALSE;
            }
		}

		//m_bDrawGraph
		strItem.Format("DrawGraph");
		strValue = GetReg_RegistryData(strAddress, strItem);

		if (strValue == "0")
			m_bDrawGraph[iGraph] = FALSE;
		else
			m_bDrawGraph[iGraph] = TRUE;

		CheckDlgButton(m_IDC_DrawGraph[iGraph], m_bDrawGraph[iGraph]);
	}

	// Pin Name
	if (m_strPacketHeader != " " &&  m_strPacketHeader != "") {
		strAddress.Format("Header\\%c%d", m_cPacketHead, m_cPacketHead);
		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			strItem.Format("PinName%02d", iData);
			m_strPinName[iData].Format("%s", (LPCTSTR)GetReg_RegistryData(strAddress, strItem));
		}
	}
	else {
		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			m_strPinName[iData].Format("");
		}
	}
	LoadDynamoPinName();

	// Min-Max Valu, Auto Range
	for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
		if (m_strPacketHeader == " " && m_strPacketHeader == "")
			strAddress.Format("Graph\\Graph%d", iGraph);
		else
			strAddress.Format("Graph\\Graph%d\\%c%d%s", iGraph, m_cPacketHead, m_cPacketHead, (LPCTSTR)strPresetMode);

		strItem.Format("MaxValue");
		strValue = GetReg_RegistryData(strAddress, strItem);
		m_RealGrpahInfo[iGraph].fRangeMax = atof(strValue);

		strItem.Format("MinValue");
		strValue = GetReg_RegistryData(strAddress, strItem);
		m_RealGrpahInfo[iGraph].fRangeMin = atof(strValue);

		m_strRangeMid[iGraph].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMin[iGraph] + m_fRangeMax[iGraph]) / 2));

		strItem.Format("AutoRange");
		strValue = GetReg_RegistryData(strAddress, strItem);
		m_RealGrpahInfo[iGraph].bAutoRange_Graph = atoi(strValue);
	}

	// Math Probe		
	LoadMathProbeDefine(0);

	GetGraphLogData();
	GetTextLogData();

	// xbox input
	strValue = GetRegRoot_RegistryData("Common\\Xinput", "Use_Xinput");
	m_bUse_Xinput = atoi(strValue);
	UDF;

	if (m_bAvailablePositionGraph && m_strPacketHeader == m_strPositonGraphHeader)	m_bDisplayPosition = TRUE; else m_bDisplayPosition = FALSE;
	if (bPreDisplayPosition != m_bDisplayPosition) SetFrameSize();
	
	GetReg_LoadSetting();
	DrawInit();
}

void CSerialComDlg::LoadMathProbeDefine(int iPreDataPage) {
	CString strAddress, strItem, strValue;
		
	if (iPreDataPage > 0) {
		// modify setting 
		SetMathDefine_ChangeDataSize(iPreDataPage, m_iDataPage);
	}
    LoadDefaultMathProbeSetting();
			
	for (int i = 0; i < MAXIMUM_MATH_SIZE; i++) {
		if (m_strPacketHeader == " " || m_strPacketHeader == "")
			strAddress.Format("MathDefine\\MathProbe%d\\default", i);
		else
			strAddress.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);

		// Data 1
		strItem.Format("Data1");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].iDataID1 = atoi(strValue);

		strItem.Format("Data2");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].iDataID2 = atoi(strValue);

		strItem.Format("Operator");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].iOperator = atoi(strValue);

		strItem.Format("UseMathPinName");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].bUserName = atoi(strValue);
		else
            m_MathData[i].bUserName = FALSE;

		strItem.Format("PinName");
		strValue = GetReg_RegistryData(strAddress, strItem);
		strValue.Replace("]", "");
		strValue.Replace("[", "");
		if (strValue != "") {
            m_MathData[i].strName.Format("[%s]", (LPCTSTR)strValue);
			m_strMathProbeName[i].Format("[%s]", (LPCTSTR)strValue);
		}
		strItem.Format("ConstValue");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].fValue = atof(strValue);

        strItem.Format("Result_Integer");
        strValue = GetReg_RegistryData(strAddress, strItem);
        m_MathData[i].bResInt = atoi(strValue);

		strItem.Format("DataSize");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].nData = atoi(strValue);

		for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
			strItem.Format("DrawGraph%d", j);
			strValue = GetReg_RegistryData(strAddress, strItem);
			if (strValue == "1")
                m_MathData[i].bSelectGraph[j] = TRUE;				//
			else
                m_MathData[i].bSelectGraph[j] = FALSE;			//
			//m_MathData[i].bSelectGraph[j] = m_MathData[i].bDisplayGraph[j];
		}

		strAddress.Format("MathDefine\\MathProbe%d", i);

		strItem.Format("Red");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iColor[0] = atoi(strValue);
		else
            m_MathData[i].iColor[0] = m_iDefaultMathColor[i][0];
		
		strItem.Format("Green");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iColor[1] = atoi(strValue);
		else
            m_MathData[i].iColor[1] = m_iDefaultMathColor[i][1];

		strItem.Format("Blue");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iColor[2] = atoi(strValue);
		else
            m_MathData[i].iColor[2] = m_iDefaultMathColor[i][2];

		strItem.Format("PenThickness");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iPenThickness = atoi(strValue);
		else
            m_MathData[i].iPenThickness = 2;

		strItem.Format("PenStyle");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iPenStyle = atoi(strValue);
		else
            m_MathData[i].iPenStyle = PS_SOLID;

		strItem.Format("UseMark");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue == "")
            m_MathData[i].bUseMark = 1;
		else
            m_MathData[i].bUseMark = atoi(strValue);
	}		
	CheckMathProbeValid();

	ReCalculationMathData();	
}

void CSerialComDlg::LoadCommonConfiguration() {
	CString strAddress, strItem, strValue, strSavedHeader, strPresetMode;

	// Data / Math page	
	int iDataPage = m_iDataPage;
	int iMathPage = m_iMathPage;
    int iHistoryCapacity = m_iHistoryCapacity;
	
	strValue = GetRegRoot_RegistryData("Common", "nDataPage");
	if (strValue == "") strValue = "1";
	m_iDataPage = max(1, min(N_MAX_DATA_PAGE, atoi(strValue)));
	m_iMaxDataSize = min(N_SHOW_DATA_CONTROL, m_iDataPage * MAX_DATACONTROL_SIZE);
	
	strValue = GetRegRoot_RegistryData("Common", "nMathPage");
	if (strValue == "") strValue = "1";
	m_iMathPage = max(1, min(N_MAX_MATH_PAGE, atoi(strValue)));
	m_iMaxMathSize = m_iMathPage * MAX_MATHCONTROL_SIZE;

    strValue = GetRegRoot_RegistryData("Common", "nHistoryCapacity");
    if (strValue == "") strValue.Format("%d", DEFAULT_TESTDATA_SIZE);   
    m_iHistoryCapacity = max(MIN_TESTDATA_SIZE, min(MAX_TESTDATA_SIZE, atoi(strValue)));

	if (m_iDataPage != iDataPage || m_iMathPage != iMathPage || iHistoryCapacity != m_iHistoryCapacity) {
		if (m_iDataPage != iDataPage) {
			// MathProbe 정의도 다시로딩 필요함
			LoadMathProbeDefine(iDataPage);
		}
		InitHistoryData();
	}

	// Background color
	for (int iMode = 0; iMode < 2; iMode++) {
		strAddress.Format("Common\\Color\\Background\\Mode%d", iMode);
		for (int i = 0; i < 3; i++) {
			strItem.Format("RGB_color_%d", i);
			strValue = GetRegRoot_RegistryData(strAddress, strItem);
			if (strValue == "") {
				m_iBackgroundColor[iMode][i] = m_iDefault_BackgroundColor[iMode][i];
			}
			else {
				m_iBackgroundColor[iMode][i] = atoi(strValue);
			}
		}
		m_BrushBackGround[iMode].CreateSolidBrush(RGB(m_iBackgroundColor[iMode][0], m_iBackgroundColor[iMode][1], m_iBackgroundColor[iMode][2]));
	}

	// Text Color
	strAddress.Format("Common\\Color\\Text");
	for (int i = 0; i < 3; i++) {
		strItem.Format("RGB_color_%d", i);
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "") m_iTextColor[i] = m_iDefault_TextColor[i]; else m_iTextColor[i] = atoi(strValue);
	}

	// Probe : Data Value
	strValue = GetRegRoot_RegistryData("Common\\Graph", "Show_Probe_Value");
	if (strValue == "") strValue = "1";
	m_bShowProbeValue = atoi(strValue);

	// Probe : Data ID Show
	strValue = GetRegRoot_RegistryData("Common\\Graph", "Show_Probe_DataID");
	m_bShowProbeDataID = atoi(strValue);

	// Probe : Data ID Show
	strValue = GetRegRoot_RegistryData("Common\\Graph", "Show_Timeline_N");
	m_bShowTimelineN = atoi(strValue);

	// 마지막 Command Load
	strAddress = "Command";
	strItem = "LastCommand";

	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	m_strPacketHeader.Format("%s", (LPCTSTR)strValue);

	int iSize = m_strPacketHeader.GetLength();

	if (iSize < 20) {
		char myString[10];
		strcpy_s(myString, (LPCTSTR)m_strPacketHeader);
		m_strPacketHeader = "";
		m_cPacketHead = 0;
		for (int j = 0; j < iSize; j++) {
			if (myString[j] != ' ') {
				m_strPacketHeader.Format("%c", myString[j]);
				m_cPacketHead = myString[j];
				break;
			}
		}
	}
	if (m_cPacketHead >= '_') {
        m_bAcceptNoHeader = (m_cPacketHead == '_');
		CheckDlgButton(IDC_NoHeader, m_bAcceptNoHeader);
	}

	// PositionGraph 
	strAddress = "Command\\PositionGraph";
	strItem = "Header";

	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	m_strPositonGraphHeader.Format("%s", (LPCTSTR)strValue);

	// Use Position Graph 
	strItem = "UsePositionGraph";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	m_bUsePositionGraph = atoi(strValue);

	// Select data for X-dir in Position Graph 
	strItem = "SelectPositionGraphX";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") {
		m_iSelectPositionGraphX = -1;
	}
	else {
		m_iSelectPositionGraphX = atoi(strValue);
	}

	// Select data for X-dir in Position Graph 
	strItem = "SelectPositionGraphY";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") {
		m_iSelectPositionGraphY = -1;
	}
	else {
		m_iSelectPositionGraphY = atoi(strValue);
	}

	// Position Graph SIze
	strItem = "FrameSize";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iFrameSizeGraphPosition = atoi(strValue);

	// Position Graph 유효성 검증
	{
		if (m_bUsePositionGraph && m_iSelectPositionGraphX >= 0 && m_iSelectPositionGraphX < MAX_DATACONTROL_SIZE + m_iMaxMathSize && m_iSelectPositionGraphY >= 0 && m_iSelectPositionGraphY < MAX_DATACONTROL_SIZE + m_iMaxMathSize) {
			m_bAvailablePositionGraph = TRUE;
		}
		else {
			m_bAvailablePositionGraph = FALSE;
		}
	}
	//Filter view
	for (int i = 0; i < MAX_FILTER; i++)
	{
		strAddress.Format("Filter\\FilterView%d", i);
		strItem.Format("UseFilter");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "") {
			m_bUseFilterItem[i] = atoi(strValue);
		}
		else {
			m_bUseFilterItem[i] = FALSE;
		}

		CheckDlgButton(m_IDC_UseFilter[i], m_bUseFilterItem[i]);

		strItem.Format("FilterName");
		m_strFilterItem[i].Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, strItem));
		m_iFilterLength[i] = m_strFilterItem[i].GetLength();
		SetDlgItemText(m_IDC_FilterName[i], m_strFilterItem[i]);
	}

	// Serial Command
	strAddress.Format("Command\\Button");
	for (int i = 0; i < MAX_BT_COMMAND; i++) {
		BYTE Command[2]; Command[1] = 0;
		strItem.Format("Command%d", i);
		strValue.Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, strItem));
		if (strValue == "") {
			strValue = "32";
			SetRegRoot_RegistryData(strAddress, strItem, strValue);
		}
		m_BT_Command[i] = atoi(strValue);

		CString str;
		str.Format("%c", m_BT_Command[i]);

		switch (i) {
		case 0: SetDlgItemText(IDC_UserCommand0, str); break;
		case 1: SetDlgItemText(IDC_UserCommand1, str); break;
		case 2: SetDlgItemText(IDC_UserCommand2, str); break;
		case 3: SetDlgItemText(IDC_UserCommand3, str); break;
		case 4: SetDlgItemText(IDC_UserCommand4, str); break;
		case 5: SetDlgItemText(IDC_UserCommand5, str); break;
		case 6: SetDlgItemText(IDC_UserCommand6, str); break;
		case 7: SetDlgItemText(IDC_UserCommand7, str); break;
		case 8: SetDlgItemText(IDC_UserCommand8, str); break;
		case 9: SetDlgItemText(IDC_UserCommand9, str); break;
		case 10: SetDlgItemText(IDC_UserCommand10, str); break;
		case 11: SetDlgItemText(IDC_UserCommand11, str); break;
		case 12: SetDlgItemText(IDC_UserCommand12, str); break;
		case 13: SetDlgItemText(IDC_UserCommand13, str); break;
		case 14: SetDlgItemText(IDC_UserCommand14, str); break;
		case 15: SetDlgItemText(IDC_UserCommand15, str); break;
		case 16: SetDlgItemText(IDC_UserCommand16, str); break;
		case 17: SetDlgItemText(IDC_UserCommand17, str); break;
		case 18: SetDlgItemText(IDC_UserCommand18, str); break;
		case 19: SetDlgItemText(IDC_UserCommand19, str); break;
		}
	}

	for (int iData = 0; iData < MAXIMUM_DATA_SIZE; iData++) {
		strAddress.Format("Graph\\PenColor\\PEN_%d", iData);
		for (int iColor = 0; iColor < 3; iColor++) {
			switch (iColor) {
			case 0: strItem.Format("Red"); break;
			case 1: strItem.Format("Green"); break;
			case 2: strItem.Format("Blue"); break;
			}

			strValue = GetRegRoot_RegistryData(strAddress, strItem);
			if (strValue == "") {  // defalut color
				m_iPenColor[iData][iColor] = m_iDefalultPenColor[iData][iColor];
			}
			else {
				m_iPenColor[iData][iColor] = atoi(strValue);
			}
		}

		strItem.Format("PenType");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "")
			m_iPenType[iData] = 0;
		else
			m_iPenType[iData] = atoi(strValue);

		strItem.Format("Thickness");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "")
			m_iPenThickness[iData] = 3;
		else
			m_iPenThickness[iData] = max(1, min(10, atoi(strValue)));

		strItem.Format("UseMark");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "")
			m_bUseDataMark[iData] = 1;
		else
			m_bUseDataMark[iData] = atoi(strValue);

		m_LogBrush.lbColor = RGB(m_iPenColor[iData][0], m_iPenColor[iData][1], m_iPenColor[iData][2]);
		m_PenData[iData].CreatePen(PS_GEOMETRIC | m_iPenType[iData], m_iPenThickness[iData], &m_LogBrush, 0, 0);
	}

	for (int iActive = 0; iActive < 2; iActive++) {
		CString strMode = "";
		if (iActive == 0) strMode = "Inactive"; else strMode = "Active";
		for (int iColor = 0; iColor < 3; iColor++) {
			switch (iColor) {
			case 0: strItem.Format("Red"); break;
			case 1: strItem.Format("Green"); break;
			case 2: strItem.Format("Blue"); break;
			}

			strAddress.Format("Graph\\SelectControl\\%s\\Blank", (LPCTSTR)strMode);
			strValue = GetRegRoot_RegistryData(strAddress, strItem);
			if (strValue == "") {  // defalut color
				m_iRGB_SelectControl_Blank[iColor][iActive] = m_iDefaultRGB_SelectControl_Blank[iColor][iActive];
			}
			else {
				m_iRGB_SelectControl_Blank[iColor][iActive] = atoi(strValue);
			}

			strAddress.Format("Graph\\SelectControl\\%s\\Outline", (LPCTSTR)strMode);
			strValue = GetRegRoot_RegistryData(strAddress, strItem);
			if (strValue == "") {  // defalut color
				m_iRGB_SelectControl_Outline[iColor][iActive] = m_iDefaultRGB_SelectControl_Outline[iColor][iActive];
			}
			else {
				m_iRGB_SelectControl_Outline[iColor][iActive] = atoi(strValue);
			}

			strAddress.Format("Graph\\SelectControl\\%s\\Select", (LPCTSTR)strMode);
			strValue = GetRegRoot_RegistryData(strAddress, strItem);
			if (strValue == "") {  // defalut color
				m_iRGB_SelectControl_Select[iColor][iActive] = m_iDefaultRGB_SelectControl_Select[iColor][iActive];
			}
			else {
				m_iRGB_SelectControl_Select[iColor][iActive] = atoi(strValue);
			}
		}
		m_BrushSelectControl_Blank[iActive].CreateSolidBrush(RGB(m_iRGB_SelectControl_Blank[0][iActive], m_iRGB_SelectControl_Blank[1][iActive], m_iRGB_SelectControl_Blank[2][iActive]));
		m_BrushSelectControl_Select[iActive].CreateSolidBrush(RGB(m_iRGB_SelectControl_Select[0][iActive], m_iRGB_SelectControl_Select[1][iActive], m_iRGB_SelectControl_Select[2][iActive]));
		m_PenSelectControl_Blank[iActive].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Outline[0][iActive], m_iRGB_SelectControl_Outline[1][iActive], m_iRGB_SelectControl_Outline[2][iActive]));
		m_PenSelectControl_Select[iActive].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Select[0][iActive], m_iRGB_SelectControl_Select[1][iActive], m_iRGB_SelectControl_Select[2][iActive]));
	}

	// Display size & time stamp
	strAddress.Format("Graph");
	strItem.Format("DisplaySize");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") {
        m_iDisplaySize = 200;
	}
    else {
        m_iDisplaySize = atoi(strValue);
    }
	SetDisplayDataSize();
	m_iPreDisplaySize = m_iDisplaySize;

	// Max Fit Screen size
	strAddress.Format("Graph");
	strItem.Format("MaxScreenFitSize");
	strValue.Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, strItem));
	if (strValue == "") {
		strValue = "1000";
	}
	m_iMaxFitScreen = atoi(strValue);
	SetDlgItemInt(IDC_MaxFitScreen, m_iMaxFitScreen);

	// Use Fit to Max Data
	strAddress.Format("Graph");
	strItem.Format("UseMaxScreenFit");
	strValue.Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, strItem));
	m_bFitToScreen = atoi(strValue);
	CheckDlgButton(IDC_FitToMaxData, atoi(strValue));

	// Graph Frame
	strAddress.Format("Graph");
	strItem.Format("FixedGraphFrame");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (atoi(strValue))
		m_bFixGraphFrame = TRUE;
	else
		m_bFixGraphFrame = FALSE;

	// Legend Frame Size
	strAddress.Format("Graph");
	strItem = "LegendFrameSize";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iLegendX = atoi(strValue);

	strAddress.Format("Graph");
	strItem = "LegendFrameSize_PositionGraph";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iLegendX_PositionGraph = atoi(strValue);

	// Graph Mark Size
	strAddress.Format("Graph");
	strItem = "MarkSize";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iMarkSize = atoi(strValue);

	// Log Filter Ratio
	strAddress.Format("Graph");
	strItem = "Log_Filter_Ratio";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iRatioLogFilter = atoi(strValue); else m_iRatioLogFilter = 50;

	// Show Data Size
	strAddress.Format("Graph");
	strItem = "Show_Data_Size";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	m_iShowDataControl = N_SHOW_DATA_CONTROL;
#if 0
	if (strValue == "") {
		m_iShowDataControl = MAX_DATACONTROL_SIZE;
	}
	else {
		m_iShowDataControl = max(0, min(atoi(strValue), MAX_DATACONTROL_SIZE));
	}
#endif

	if (m_iShowDataControl > 0) {
		CheckDlgButton(IDC_Display0, TRUE);
	}
	else {
		CheckDlgButton(IDC_Display0, FALSE);
	}

    strItem = "Data_Offset";
    strValue = GetRegRoot_RegistryData(strAddress, strItem);
    m_iControlDataOffset = min(m_iMaxDataSize - m_iShowDataControl, max(0, atoi(strValue)));

 	// Log Filter Width
	strAddress.Format("Graph");
	strItem = "Log_Filter_Width";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") {
		m_iRangeLogFilterWidth = min(MAX_RANGE_LOGFILTER_H - 3, max(MIN_RANGE_LOGFILTER_H, atoi(strValue) - 2));
		m_iDrawGraphRangeX[0] = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H + MIN_RANGE_GRAPH_H;
		m_iPreDrawGraphRangeX = m_iDrawGraphRangeX[0];
	}

	// Display No of Math Probe	
    strAddress.Format("Graph");
    strItem = "Show_Math_Size";
    strValue = GetRegRoot_RegistryData(strAddress, strItem);
    
    if (strValue == "") {
        m_iShowMathControl = MAX_MATHCONTROL_SIZE;
    }
    else {
        m_iShowMathControl = min(max(0, atoi(strValue)), MAX_MATHCONTROL_SIZE);
    }

    if (m_iShowMathControl > 0) { 
        CheckDlgButton(IDC_Display2, TRUE); 
        m_bShowMathControl = TRUE;
    }
    else { 
        m_bShowMathControl = FALSE;
        CheckDlgButton(IDC_Display2, FALSE);
    }

    strItem = "Math_Offset";
    strValue = GetRegRoot_RegistryData(strAddress, strItem);
    m_iControlMathOffset = min(m_iMaxMathSize - m_iShowMathControl, max(0, atoi(strValue)));

	// Display Filter item
	strAddress.Format("Graph");
	strItem = "FilterViewMode";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
    
    {  strValue = "0";  }

	if (atoi(strValue)) {
        m_iFilterViewMode = 0;
		CheckDlgButton(IDC_Display1, TRUE);
	}
	else {
        m_iFilterViewMode = 1;
		CheckDlgButton(IDC_Display1, FALSE);
	}

    // Display Filtered Log item
    strAddress.Format("Graph");
    strItem = "FilterLogViewMode";
    strValue = GetRegRoot_RegistryData(strAddress, strItem);

	{  strValue = "0";  }

    if (atoi(strValue)) {
        m_bShowFilterLogControl = TRUE;
        CheckDlgButton(IDC_Display4, TRUE);
    }
    else {
        m_bShowFilterLogControl = FALSE;
        CheckDlgButton(IDC_Display4, FALSE);
    }

	// Grid 
	strAddress.Format("Graph");
	strItem = "GridCountX";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iGridX = atoi(strValue);

	strItem = "GridX_Type";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	m_iGridX_Type = atoi(strValue);

	strItem = "GridCountY";
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "") m_iGridCountY = atoi(strValue);

	// Display Time Mode
	strAddress.Format("Graph");
	strItem.Format("TimeDisplayMode");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue != "")	m_iTimeDisplayMode = atoi(strValue); else m_iTimeDisplayMode = 0;

	// Control Font size
	strAddress.Format("Common\\Font");
	for (int i = 0; i < MAX_FONT_SIZE; i++) {
		strItem.Format("FontSize_%d", i);
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "") m_iControlFontSize[i] = atoi(strValue); else m_iControlFontSize[i] = 9;
		ChangeControlFontSize(i, m_iControlFontSize[i]);
	}

	strAddress.Format("Graph\\DataMultiplier");
	for (int iData = 0; iData < m_iMaxDataSize; iData++) {
		strItem.Format("Coeff%02d", iData);
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "")
			m_fCoff[iData] = 1.;
		else
			m_fCoff[iData] = atof(strValue);
	}

	strAddress.Format("Common\\Data");
	strValue.Format("%d", m_bUseIntData);
	strValue = GetRegRoot_RegistryData(strAddress, "DataType");
	if (strValue) {
		m_bUseIntData = TRUE;
	}
	else {
		m_bUseIntData = atoi(strValue);
	}

	strValue = GetRegRoot_RegistryData("Common\\", "MinPacketDataSizeDelta");
	if (strValue == "")
		m_iMinPacketDataSize = m_iMaxDataSize / 2;
	else
		m_iMinPacketDataSize = min(m_iMaxDataSize, max(5,  m_iMaxDataSize - atoi(strValue)));

	strValue = GetRegRoot_RegistryData("Common\\", "MaxPacketDataSizeDelta");
	if (strValue == "")
		m_iMaxPacketDataSize = m_iMaxDataSize + 80;
	else
		m_iMaxPacketDataSize = min(max(m_iMaxDataSize, m_iMaxDataSize + atoi(strValue)), 80);

	strValue = GetRegRoot_RegistryData("Common\\", "MaxDataDigit");
	if (strValue != "") {
		m_iMaxDataDigit = atoi(strValue);
	}


	// Select Data Label
	strAddress.Format("Graph\\SelectData\\");

	strItem.Format("TextColorMode");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") strValue = "1";
	m_iSelTextColorMode = atoi(strValue);

	strItem.Format("BkColorMode");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") strValue = "0";
	m_iSelBkColorMode = atoi(strValue);;

	for (int i = 0; i < 3; i++) {
		strItem.Format("TextColor_%d", i);
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "") strValue = "255";
		m_iSelTextColor[i] = atoi(strValue);

		strItem.Format("BkColor_%d", i);
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue == "") strValue = "0";
		m_iSelBkColor[i] = atoi(strValue);
	}

	// Get Setting in File Loading 
	strValue = GetRegRoot_RegistryData("Common\\", "GetSettingInLoading");
	if (strValue == "") {
		m_bGetSettingInLoading = TRUE;
	}
	else {
		m_bGetSettingInLoading = atoi(strValue);
	}

	// Auto Header_Rx
    strValue = GetRegRoot_RegistryData("Common", "UseAutoHeader");
    if (strValue == "") strValue = "1";
	m_bUseAutoHeader_rx = m_bUseAutoHeader_rx_reg = atoi(strValue);

    strValue = GetRegRoot_RegistryData("Common", "UseAutoHeaderCount");
    if (strValue != "") {
        m_iAutoHeader_rx_count = atoi(strValue);
    }

	// Auto Header_Tx
	strValue = GetRegRoot_RegistryData("Common", "UseAutoHeader_Tx");
	if (strValue == "") { strValue = "1"; }
    m_bUseAutoHeader_tx = atoi(strValue);

	// Show Resize Cursor
	strValue = GetRegRoot_RegistryData("Common", "ShowResizeCursor");
	if (strValue == "") { strValue = "1"; }
	m_bShowResizeCursor = atoi(strValue);

	strValue = GetRegRoot_RegistryData("Common", "Save_All_Received_Data");
	CheckDlgButton(IDC_SaveRcvDataAll, atoi(strValue));

	// Use Auto Save Option
	strValue = GetRegRoot_RegistryData("Common", "UseAutoSave");
	m_bUseAutoSave = atoi(strValue);
	CheckDlgButton(IDC_UseAutoSave, m_bUseAutoSave);

	// Load settings in Save File Loading
	strValue = GetRegRoot_RegistryData("Common", "LoadSetting_in_savedDataLoading");
	if (strValue == ""){
		strValue = "1";
	}
	m_LoadSettingInfo_in_DataLoading = atoi(strValue);

	// Mouse Wheel Dir
	strValue = GetRegRoot_RegistryData("Common", "MouseWheelZoomDir");
	m_bMouseWheelZoomDir = atoi(strValue);

	strValue = GetRegRoot_RegistryData("Common", "MouseWheelShiftDir");
	m_bMouseWheelShiftDir = atoi(strValue);

	// Auto Save	
	strValue = GetRegRoot_RegistryData("Common", "UseAutoSave_LogCountLimit");
	if (strValue == "") { strValue = "1";  }
	m_bUseAutosave1_LogCountLimit = atoi(strValue);

	strValue = GetRegRoot_RegistryData("Common", "UseAutoSave_Time_LogCount");
	m_bUseAutosave2_Time_LogCount = atoi(strValue);
		
	strValue = GetRegRoot_RegistryData("Common", "AutoSave_Timeout");
	if (strValue == "") {
		strValue = "36000000";
	}
	m_iAutosave2_cond_Time = max(0, atoi(strValue));

	strValue = GetRegRoot_RegistryData("Common", "AutoSave_LogCount");
	m_iAutosave2_cond_LogCount = max(0, atoi(strValue));

	if (!(m_iAutosave2_cond_Time + m_iAutosave2_cond_LogCount)) {
		m_bUseAutosave2_Time_LogCount = false;
	}

	strValue = GetRegRoot_RegistryData("Common", "UseAutoSave_Add_Capture");
	m_bAutosave_AddCapture = atoi(strValue);

    strValue = GetRegRoot_RegistryData("Common", "LogCountrol_UpdatePeriod");
    if (strValue == "") { strValue = "5"; }
    m_iLOG_Update_Period = max(MIN_LOG_UPDATE_PERIOD, min(MAX_LOG_UPDATE_PERIOD, atoi(strValue)));

	UDF;
}


void CSerialComDlg::ChangeControlFontSize(int iItem, int iSize) {
	if (iItem >= MAX_FONT_SIZE) { AfxMessageBox("Invalid Control Font ID"); return; }
	int iControlID[MAX_FONT_SIZE] = { IDC_FilterMessage, IDC_LogList };
	m_FontControl[iItem].CreatePointFont(iSize*10, "굴림체", NULL);
	GetDlgItem(iControlID[iItem])->SetFont(&m_FontControl[iItem]);
}


double CSerialComDlg::GetValue(CString str) {
	double fValue = 0.;
	double fOrder = 1.;

	// 유효성 검증	
	int iSize = str.GetLength();
	if (iSize > 100) return -1;

	BOOL bFindNum = FALSE, bNegative = FALSE;
	BYTE Data;

	BOOL bFindPeriod = FALSE;
	for (int i = 0; i < iSize; i++) {
		Data = str.GetAt(i);
		if (Data == '-') bNegative = TRUE;
		if (Data >= '0' && Data <= '9') {
			bFindNum = TRUE;
			if (bFindPeriod) {
				fValue += fOrder * (Data - '0');
				fOrder *= 0.1;
			}
			else {
				fValue *= 10.;
				fValue += Data - '0';
			}
		}
		else if (Data == '.') {
			bFindPeriod = TRUE;
		}
		else {
			if (bFindNum) return fValue * (1 - 2 * bNegative);
		}
	}
	return fValue * (1. - 2. * bNegative);
}

void CSerialComDlg::OnBnClickedUsercommandsetting() {
	if (m_iProgramMode == DATA_ACQUISITION_MODE) { return; }
	CSettingDlg SettingDlg;
	SettingDlg.pMain = this;
	for (int i = 0; i < MAX_BT_COMMAND; i++) {
		SettingDlg.m_BT_Command[i] = m_BT_Command[i];
	}
	SettingDlg.m_iTimeDisplayMode = m_iTimeDisplayMode;
	m_bRetrunSubProgram = FALSE;
	m_bReqCloseProgram = FALSE;
	SettingDlg.DoModal();

	if (m_bReqCloseProgram) {
		OnOK();  // 설정 초기화후 종료
	}

	if (m_bRetrunSubProgram) {
		CString str;
		for (int i = 0; i < MAX_BT_COMMAND; i++) {
			str.Format("%c", m_BT_Command[i]);
			switch (i) {
			case 0: SetDlgItemText(IDC_UserCommand0, str); break;
			case 1: SetDlgItemText(IDC_UserCommand1, str); break;
			case 2: SetDlgItemText(IDC_UserCommand2, str); break;
			case 3: SetDlgItemText(IDC_UserCommand3, str); break;
			case 4: SetDlgItemText(IDC_UserCommand4, str); break;
			case 5: SetDlgItemText(IDC_UserCommand5, str); break;
			case 6: SetDlgItemText(IDC_UserCommand6, str); break;
			case 7: SetDlgItemText(IDC_UserCommand7, str); break;
			case 8: SetDlgItemText(IDC_UserCommand8, str); break;
			case 9: SetDlgItemText(IDC_UserCommand9, str); break;
			case 10: SetDlgItemText(IDC_UserCommand10, str); break;
			case 11: SetDlgItemText(IDC_UserCommand11, str); break;
			case 12: SetDlgItemText(IDC_UserCommand12, str); break;
			case 13: SetDlgItemText(IDC_UserCommand13, str); break;
			case 14: SetDlgItemText(IDC_UserCommand14, str); break;
			case 15: SetDlgItemText(IDC_UserCommand15, str); break;
			case 16: SetDlgItemText(IDC_UserCommand16, str); break;
			case 17: SetDlgItemText(IDC_UserCommand17, str); break;
			case 18: SetDlgItemText(IDC_UserCommand18, str); break;
			case 19: SetDlgItemText(IDC_UserCommand19, str); break;
			}
		}

		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			m_LogBrush.lbColor = RGB(m_iPenColor[iData][0], m_iPenColor[iData][1], m_iPenColor[iData][2]);
			m_PenData[iData].CreatePen(PS_GEOMETRIC | m_iPenType[iData], m_iPenThickness[iData], &m_LogBrush, 0, 0);
		}	

		OnBnClickedSaveconfiguration();
		CheckMathProbeSetting();
        CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
        InvalidateRect(&rect);
		DrawInit();
	}
	OptimizeGraphCount();
}

void CSerialComDlg::ClickedUsercommand(int index) {
    static int sendCount = 0;
	if (m_BT_Command[index] != 0 && m_BT_Command[index] != ' ') {
		if (m_bUseAutoHeader_tx) {
			if ((m_BT_Command[index] >= 'a' && m_BT_Command[index] <= 'z') || (m_BT_Command[index] >= 'A' && m_BT_Command[index] <= 'Z') || m_BT_Command[index] == '_') {
				CString strTemp;
				strTemp.Format("%c", m_BT_Command[index]);
				if (strTemp != m_strPacketHeader) {
					m_strPacketHeader.Format("%s", (LPCTSTR)strTemp);
					m_iPresetMode = 0;
                    SetDlgItemText(IDC_PacketHead, m_strPacketHeader);
                    m_ctrlMode.SetCurSel(m_iPresetMode);
					ChangeHeader();
				}
			}
		}

        if (m_ComuPort.m_bConnected) {
            m_ComuPort.WriteComm(&m_BT_Command[index], 1);
            m_strTemp.Format("Send ch: %c (cnt:%d)", m_BT_Command[index], ++sendCount);
            SetDlgItemText(IDC_Temp, m_strTemp);            
        }
	}
}

void CSerialComDlg::OnBnClickedUsercommand0() { ClickedUsercommand(0); }
void CSerialComDlg::OnBnClickedUsercommand1() { ClickedUsercommand(1); }
void CSerialComDlg::OnBnClickedUsercommand2() { ClickedUsercommand(2); }
void CSerialComDlg::OnBnClickedUsercommand3() { ClickedUsercommand(3); }
void CSerialComDlg::OnBnClickedUsercommand4() { ClickedUsercommand(4); }
void CSerialComDlg::OnBnClickedUsercommand5() { ClickedUsercommand(5); }
void CSerialComDlg::OnBnClickedUsercommand6() { ClickedUsercommand(6); }
void CSerialComDlg::OnBnClickedUsercommand7() { ClickedUsercommand(7); }
void CSerialComDlg::OnBnClickedUsercommand8() { ClickedUsercommand(8); }
void CSerialComDlg::OnBnClickedUsercommand9() { ClickedUsercommand(9); }
void CSerialComDlg::OnBnClickedUsercommand10() { ClickedUsercommand(10); }
void CSerialComDlg::OnBnClickedUsercommand11() { ClickedUsercommand(11); }
void CSerialComDlg::OnBnClickedUsercommand12() { ClickedUsercommand(12); }
void CSerialComDlg::OnBnClickedUsercommand13() { ClickedUsercommand(13); }
void CSerialComDlg::OnBnClickedUsercommand14() { ClickedUsercommand(14); }
void CSerialComDlg::OnBnClickedUsercommand15() { ClickedUsercommand(15); }
void CSerialComDlg::OnBnClickedUsercommand16() { ClickedUsercommand(16); }
void CSerialComDlg::OnBnClickedUsercommand17() { ClickedUsercommand(17); }
void CSerialComDlg::OnBnClickedUsercommand18() { ClickedUsercommand(18); }
void CSerialComDlg::OnBnClickedUsercommand19() { ClickedUsercommand(19); }

void CSerialComDlg::OnEnChangeIndexnumber() {
    GetDlgItemText(IDC_IndexNumber, m_strIndexNumber);
	int iIndex = atoi(m_strIndexNumber);
	iIndex = max(0, min(m_iHistoryIndex - 1, iIndex));
    CString str;
	str.Format("%d", iIndex);
    if (str != m_strIndexNumber) {
        m_strIndexNumber.Format("%d", iIndex);
        SetDlgItemText(IDC_IndexNumber, m_strIndexNumber);
    }
    
	m_iCurrentIndex = iIndex;
    
    int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;
    int iEndIndex = m_iHistoryIndex - m_iHistoryMove;
    if (iStartIndex >= m_iCurrentIndex || iEndIndex <= m_iCurrentIndex) {
        // 현재 View안에 없으면 뷰영역을 변경하여 화면 중앙에 배치  
        m_iHistoryMove = -(iIndex - (m_iHistoryIndex - m_iDisplaySize / 2));
    }
    int end = m_iHistoryIndex - m_iHistoryMove;
	

	if (end > m_iHistoryIndex) {
		m_iHistoryMove = 0;
		int start = m_iHistoryIndex - m_iDisplaySize;
		m_iHistorySelectPt = m_iDrawGraphRangeX[0] + (iIndex - start) * m_iDrawGraphSpanX / m_iDisplaySize;
	}
	else {
		m_iHistorySelectPt = m_iDrawGraphMidX - m_iDrawGraphSpanX / m_iDisplaySize;
	}
	m_bDrawSelectData = TRUE;
	m_bReqGraphUpdate = TRUE;
    if (m_iProgramMode == DATA_ACQUISITION_MODE) { HighlightLog(m_HistoryData[iIndex].iHistoryLogIndex); }
	
    static int preFilterIndex = -1;
    if (preFilterIndex != m_HistoryData[iIndex].iHistoryFilterIndex) {
        preFilterIndex = m_HistoryData[iIndex].iHistoryFilterIndex;
        m_ctrlFilterBox.SetCurSel(preFilterIndex);
    }
}

void CSerialComDlg::OnBnClickedClearmessage() {
	m_iLogIndex = 0;
	m_arrLogData.clear();
	m_vecPrevSelectedIndices.clear();
	AddLogData("");
}

BOOL CSerialComDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	CDialog::OnMouseHWheel(nFlags, zDelta, pt);	
	ScreenToClient(&pt);  // 마우스 위치를 상대위치로 변환
	m_iWheelCount++;

	// Area설정
	// iArea     1: Graph    2: Position   0: 그외
	int iArea = 0;
	if (pt.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (pt.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else
		iArea = 2;

	if (iArea == 1) {
		if (pt.y > GRAPH_RANGE_Y) {
			if (CheckControlKeyDown()) {
				// DisplayDataSize 변경
				UDT;
				int iPreHistoryNumber = atoi(m_strIndexNumber);

				int iData = m_iDisplaySize;

				int delta;
				if (CheckShiftKeyDown()) {
					delta = max(1, m_iDisplaySize * m_iWheelAcc  / 1200);
				}
				else {
					delta = max(1, m_iDisplaySize * m_iWheelAcc / 300);					
				}

				if ((zDelta > 0) == (m_bMouseWheelZoomDir == 0)) {
					iData += delta;
				}
				else {
					iData -= delta;
				}

				m_iDisplaySize = max(1, min(iData, m_iHistoryIndex));
				m_bManualAdjust = TRUE;
				SetDisplayDataSize();
				OnEnChangeDisplaydatasize();

				m_iHistoryMove = max(0, min(m_iHistoryMove, m_iHistoryIndex - m_iDisplaySize));

				if (m_iProgramMode != DATA_ACQUISITION_MODE && m_iHistorySelectIndex < m_iHistoryIndex - 1) {
					int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;
					int iEnd_index = iStartIndex + m_iDisplaySize;
					int idx = iStartIndex + (int)(0.5 + (double)m_iDisplaySize * (m_iHistorySelectPt - m_iDrawGraphRangeX[0]) / m_iDrawGraphSpanX);
					int delta = m_iHistorySelectIndex - idx;						
					m_iHistoryMove -= delta;
					if (iEnd_index > m_iHistoryIndex) {
						m_iHistoryMove -= iEnd_index - m_iHistoryIndex;
					}					
				}		
				m_iHistoryMove = max(0, min(m_iHistoryMove, m_iHistoryIndex - m_iDisplaySize));
			}
			else {
				// SelectPt, SelectIndex 변경
				if (m_iProgramMode == DATA_ANALYZE_MODE) {
					int iHistoryMove = m_iHistoryMove;
					int iShift;
					if (CheckShiftKeyDown()) {
						iShift = max(m_iDisplaySize * m_iWheelAcc / 2000, 1);
					}
					else {
						iShift = max(m_iDisplaySize * m_iWheelAcc / 200, 1);
					}

					if ((zDelta > 0) == (m_bMouseWheelShiftDir == 0)) {
						iShift = -iShift;
					}

					int end = m_iHistoryIndex - m_iHistoryMove;
					int iPreSelectIndex = m_iHistorySelectIndex;
					if (end + iShift > m_iHistoryIndex) {
						// 끝까지 Shift됨 --> 더 이상 shift안되도록 함
						// SelectIndex는 밀릴수 있으면 shift함
						m_iHistorySelectIndex = min(m_iHistorySelectIndex +iShift, m_iHistoryIndex);
						iShift = m_iHistoryIndex - end;
					}
					m_iHistoryMove = max(0, min(m_iHistoryIndex, m_iHistoryMove - iShift));

					BOOL bBlockedLimit = FALSE;
					int iBlockedIndex = 0;

					if (zDelta > 0 && m_iHistoryMove < 0) {
						iBlockedIndex = m_iHistoryMove;
						bBlockedLimit = TRUE;						
						m_iHistoryMove = 0;
					}

					if (zDelta < 0 && m_iHistoryMove > m_iHistoryIndex - m_iDisplaySize) {
						iBlockedIndex = m_iHistoryMove - (m_iHistoryIndex - m_iDisplaySize);
						bBlockedLimit = TRUE;
						m_iHistoryMove = max(0, m_iHistoryIndex - m_iDisplaySize);
					}

					if (iHistoryMove != m_iHistoryMove) {
						m_iHistorySelectIndex -= m_iHistoryMove - iHistoryMove;
						m_iHistorySelectIndex -= iBlockedIndex;
						m_iHistorySelectIndex = max(0, min(m_iHistorySelectIndex, m_iHistoryIndex));
						m_strIndexNumber.Format("%d", m_iHistorySelectIndex);
						m_iCurrentIndex = m_iHistorySelectIndex;
					}
					else if (bBlockedLimit) {
						m_iHistorySelectIndex -= iBlockedIndex;
						m_iHistorySelectIndex = max(0, min(m_iHistorySelectIndex, m_iHistoryIndex));
						m_strIndexNumber.Format("%d", m_iHistorySelectIndex);
						m_iCurrentIndex = m_iHistorySelectIndex;
					}
					else if(iPreSelectIndex != m_iHistorySelectIndex && iPreSelectIndex != m_iHistoryIndex) {
						m_strIndexNumber.Format("%d", m_iHistorySelectIndex);
						m_iCurrentIndex = m_iHistorySelectIndex;
					}
					m_bDrawSelectData = TRUE;

					//m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
					HighlightLog(m_HistoryData[m_iHistorySelectIndex].iHistoryLogIndex);
					m_ctrlFilterBox.SetCurSel(m_HistoryData[m_iHistorySelectIndex].iHistoryFilterIndex);

					if (m_bFitToScreen) {
						m_bFitToScreen = FALSE;
						CheckDlgButton(IDC_FitToMaxData, FALSE);
					}
				}
			}
			m_bReqGraphUpdate = TRUE;
		}
	}
	else if (iArea == 2) {
		if (CheckShiftKeyDown()) {
			if (zDelta > 0)
				m_fValueScale_PositionGraph *= 1.01;
			else
				m_fValueScale_PositionGraph *= 0.99;
		}
		else {
			if (zDelta > 0)
				m_fValueScale_PositionGraph *= 1.1;
			else
				m_fValueScale_PositionGraph *= 0.9;
		}
		m_fValueScale_PositionGraph = max(0.08, min(1000., m_fValueScale_PositionGraph));
		m_bUseAutoScaleGraphPosition = FALSE;
		CalculationScale();
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CSerialComDlg::OnLButtonDblClk(UINT nFlags, CPoint point) {
	CDialog::OnLButtonDblClk(nFlags, point);

	if (m_iProgramMode == DATA_ANALYZE_MODE) {
		if (point.x > m_iDrawGraphRangeX[0] && point.x < m_iDrawGraphRangeX[1]) {
			if (point.y > m_iDrawGraphRangeY[0][0] && point.y < m_iDrawGraphRangeY[m_iGraphCount - 1][1]) {
				m_iHistorySelectPt = point.x;
				m_bReqGraphUpdate = TRUE;
				m_bDrawSelectData = TRUE;
			}
		}

		// 분석 모드에서 영역 조절바를 더블 클릭시 그래프 수직 크기 동일 
		BOOL bReqAdjustSize = FALSE;
		if (m_iDrawGraphRangeX[0] <= point.x && m_iDrawGraphRangeX[1] >= point.x) {
			for (int i = 0; i < m_iGraphCount - 1; i++) {
				if (m_iDrawGraphRangeY[i][1] <= point.y && m_iDrawGraphRangeY[i + 1][0] >= point.y) {
					bReqAdjustSize = TRUE;
				}
			}
            for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
                m_RealGrpahInfo[i].iHeight = 1000;
            }
		}
		if (bReqAdjustSize) OnSize(0, m_iWndSizeX, m_iWndSizeY);

		if (point.y >= GRAPH_RANGE_Y && m_bUseAutoRange){ // && point.y <= GRAPH_START_Y) {
			if (point.x > m_iDrawGraphRangeX[0] && m_iHistoryIndex > 10) {
				// Current Index
				if (m_bTimelineFullRangeView) {
					// 이전 설정으로 되돌아가기
					m_bTimelineFullRangeView = FALSE;
					m_iDisplaySize = m_iPreDisplaySize;
					SetDisplayDataSize();

					if (m_bDrawSelectData) {
						int iCurrentSelectIndex = atoi(m_strIndexNumber);
						if (m_iHistoryMove < 0) m_iHistoryMove = 0;

						m_iHistoryMove = max(0, m_iHistoryIndex - atoi(m_strIndexNumber) - m_iPreDisplaySize / 2);
						ChangeIndexNumber(iCurrentSelectIndex);
					}
					else {
						m_iHistoryMove = 0;						
					}

					m_bReqGraphUpdate = TRUE;
				}
				else {
					// Full Range view			
					m_iPreDisplaySize = m_iDisplaySize;
					m_iDisplaySize = max(m_iHistoryIndex - 1, m_iDisplaySize);
					SetDisplayDataSize();
					m_iHistoryMove = 0;
					m_bReqGraphUpdate = TRUE;
					m_bTimelineFullRangeView = TRUE;					
				} 
			}
		}
	}
	else {
		m_strTemp.Format("Cursor Point] x:%d, y:%d", point.x, point.y); 
		SetDlgItemText(IDC_Temp, m_strTemp);
	}

	//Log Filter Data 창 크기 조절
	if (m_bShowDataControl && !m_bFixGraphFrame) {
		if (point.y >= m_iRangeLogFilterRatio[0] - 3 && point.y <= m_iRangeLogFilterRatio[1] + 3) {
			if (point.x >= 3 && point.x <= m_iRangeLogFilterWidth + 1) {
				m_iRatioLogFilter = 50;
				OnSize(0, m_iWndSizeX, m_iWndSizeY);
				m_AdjustLogFilter_V.isPressed = FALSE;
				m_bReqGraphUpdate = TRUE;
			}
		}

		if (((point.x > m_iDrawGraphRangeX[0] - 72 && point.x < m_iDrawGraphRangeX[0] - 64) || point.x < 5) && point.y > min(50, m_iRangeLogFilterVertical)) {
			m_iRangeLogFilterWidth = MIN_RANGE_LOGFILTER_H;
			m_iDrawGraphRangeX[0] = MIN_RANGE_GRAPH_H;
			m_iPreDrawGraphRangeX = m_iDrawGraphRangeX[0];

			OnSize(0, m_iWndSizeX, m_iWndSizeY);
            CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
            InvalidateRect(&rect);
			DrawInit();
			m_AdjustLogFilter_H.isPressed = FALSE;
			m_bReqGraphUpdate = TRUE;
			ControlView();
		}
		
		if (m_iRangeDataControlVertical - 3 <= point.y && point.y <= m_iRangeDataControlVertical + 5) {
			if (point.x <= m_iRangeLogFilterWidth) {
				m_AdjustDataWnd_V.isPressed = FALSE;
#if 0
				if (m_iShowDataControl < MAX_DATACONTROL_SIZE / 2) {
					m_iShowDataControl = MAX_DATACONTROL_SIZE;
				}
				else {
					m_iShowDataControl = 0;
				}
#endif
				m_iRangeDataControlVertical = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * m_iShowDataControl;
				ControlView();
			}
		}	        
	}

    if (m_bShowMathControl && !m_bFixGraphFrame) {
        if (point.y > m_iPos_MathEnd - 3 && point.y < m_iPos_MathEnd + 5 && point.x <= m_iRangeLogFilterWidth) {
            m_AdjustMathWnd_V.isPressed = FALSE;
            if (m_iShowMathControl < MAX_MATHCONTROL_SIZE / 2) {
                m_iShowMathControl = MAX_MATHCONTROL_SIZE;
            }
            else {
                m_iShowMathControl = 0;
            }
            m_iPos_MathEnd = m_iPos_MathStart + m_iShowMathControl * DRAW_CONTROL_DY + 33;
            ControlView();
        }
    }

	// Graph Select Check	
	if (m_bShowDataControl) {
		int iMoveControl_H = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H;
		int x_min = 270 + iMoveControl_H - 3;
		int x_max = 270 + (int)(MAX_GRAPH_COUNT * 19.7) + iMoveControl_H - 3 + 14;
		int y_min = DRAW_DATA_CONTROL_Y0 - 1;
		int y_max = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * m_iShowDataControl - 1 + 14;

		if (point.x > x_min && point.x < x_max &&  point.y > y_min && point.y < y_max) {
			int iData = -1, iGraph = -1;
			BOOL bConti = TRUE;
			for (int j = 0; j < m_iGraphCount && bConti; j++) {
				for (int i = 0; i < m_iShowDataControl; i++) {
					int x = 270 + (int)(j * 19.7) + iMoveControl_H - 3;
					int y = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * i - 1;
					if (point.x > x && point.x  < x + 14 && point.y > y && point.y < y + 14) {
						bConti = FALSE;
						iData = i, iGraph = j;
						break;
					}
				}
			}

			if (iData != -1) {
				if (m_bDrawGraph[iGraph]) {
					m_bSelectDraw_GraphData[iGraph][iData + m_iControlDataOffset] ^= 0x01;
					OptimizeGraphCount();
					ShowDataMathControl();
				}
			}
			else {
				// Math Select Check
				int y0;
				if (m_bShowMathControl) {
					y0 = 355;
				}
				else {
					y0 = 653;
				}
				int DeltaY = 0;
				DeltaY = DRAW_CONTROL_DY * (m_iShowDataControl - MAX_DATACONTROL_SIZE) + DRAW_CONTROL_START_Y;
				y0 += DeltaY;

				y_min = 360 + DeltaY - 1;
				y_max = 360 + DeltaY + DRAW_CONTROL_DY * (m_iShowMathControl - 1) - 1 + 14;

				if (point.y > y_min && point.y < y_max) {
					for (int i = 0; i < m_iShowMathControl; i++) {
						int y = 360 + DeltaY + DRAW_CONTROL_DY * i - 1;
						if (point.y > y && point.y < y + 14) {
							iData = i;
							break;
						}
					}
				}
				if (iData != -1) {
					if (m_bDrawGraph[iGraph]) {
						m_MathData[iData + m_iControlMathOffset].bSelectGraph[iGraph] ^= 0x01;
						m_bReqUpdateControlDraw = TRUE;
						OptimizeGraphCount();
						ShowDataMathControl();
					}
				}
			}
		}
	}

	// Timeline Control bar check
	if (point.x > m_iDrawGraphRangeX[0] && m_iProgramMode == DATA_ANALYZE_MODE) {
		if (point.y >= GRAPH_RANGE_Y && point.y <= GRAPH_START_Y && m_iHistoryIndex >= 2) {
			Req_Draw_ScreenFit();
		}
	}
}

void CSerialComDlg::OnBnClickedClearwatch() {
	if (AfxMessageBox("현재 저장된 Filter를 초기화 할까요?\r\n(Reset the currently saved filters?)", MB_YESNO) == IDYES) {
		for (int i = 0; i < MAX_FILTER; i++) {
			m_strFilterItem[i] = "";
			SetDlgItemText(m_IDC_FilterName[i], "");
			m_iFilterLength[i] = 0;
			SetDlgItemInt(m_IDC_FilterValue[i], 0);
			CheckDlgButton(m_IDC_UseFilter[i], FALSE);
		}
	}
}

void CSerialComDlg::ChangeHeader(void) {
	OnEnChangePackethead();
	// Load Setting
	LoadHeaderConfiguration();	
	UpdateDrawGraphControl();
	OptimizeGraphCount();
}

void CSerialComDlg::LoadDataFromFile(CString strFileName) {
	CFile DataFile;	

	if (DataFile.Open(strFileName, CFile::modeRead | CFile::shareDenyNone)) {	
        UINT buf_size = (UINT)DataFile.GetLength();
        BYTE* pBuffer = new BYTE[buf_size];
        DataFile.Read(pBuffer, buf_size);

		GetSaveLog(pBuffer, buf_size);
		DataFile.Close();
        delete[] pBuffer;
	}
	else {
		CString str;
		str.Format("Error in Loaing File");
		AfxMessageBox(str);
	}	
    m_bReqUpdateControlDraw = TRUE;
    ShowDataMathControl();
	strFileName.Replace("\\\\", "\\");
	m_strTemp.Format("LoadData: %d data(%s)", m_iHistoryIndex, (LPCTSTR)strFileName);
	SetDlgItemText(IDC_Temp, m_strTemp);
    Wait(0);
    Wait(0);
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::GetSaveLog(BYTE *pBuffer, UINT buf_size) {
	m_bReqGraphUpdate = FALSE;
	UDT;

	m_ctrlLogList.ShowWindow(FALSE);
	m_ctrlLogList.SetRedraw(FALSE);
	m_ctrlFilterBox.ShowWindow(FALSE);
	m_ctrlFilterBox.SetRedraw(FALSE);

	m_ctrlProgReading.ShowWindow(TRUE);
	int iDisplaySize = m_iDisplaySize;
	m_iDisplaySize = 10;
	SetDisplayDataSize();
	UDF;

	// 추가 Data 입력 보류
	m_iProgramMode = DATA_ANALYZE_MODE;
	m_bUpdateList = FALSE;
	ProgramModeUpdate();
	//m_iHistoryIndex = m_iHistoryCapacity - 1;

    int iSize = m_strPacketHeader.GetLength();
    char myString[10];

    strcpy_s(myString, (LPCTSTR)m_strPacketHeader);
    m_strPacketHeader = "";
    m_cPacketHead = 0;
    for (int j = 0; j < iSize; j++) {
        if (myString[j] != ' ') {
            m_strPacketHeader.Format("%c", myString[j]);
            m_cPacketHead = myString[j];
            m_bAcceptNoHeader = (m_cPacketHead == '_');
            CheckDlgButton(IDC_NoHeader, m_bAcceptNoHeader);
            break;
        }
    }
    
	CString strLog, str;
	int iSequence = -100;
	int iSubSequence = 0;
	int iDigit;
	char cRcvBuffer[300 + 100];
	int iRcvBufferIndex = 0;

	int iHour = 0, iMinute = 0, iSecond = 0, iMilliSecond = 0;

	int iCmdSequence = 0;
	int iSubCmd = 0;
	BOOL bIsCmdMode = FALSE;
	int iRecvChannel = 0;

	//int iCurrentIndex;
	double Buffer[100];
    int iBuffer[100];
	BOOL bPeriodic = FALSE;
	BOOL bMinus = FALSE;
	BOOL bAvailable = FALSE;
	int iOrder = 1;
	BOOL bHeadCheck = FALSE;

	InitHistoryData();

	m_iHistoryMove = 0;
	OnBnClickedClearwatchlist();
	OnBnClickedClearmessage();

	BOOL bIsHistoryDataCompleted = FALSE;

	UINT iDiplayPeriod = min(max(buf_size / 200, 30000), 400000);
	UINT iDisplayNext = iDiplayPeriod;

	char PinName[100];
	int iPinNameIndex;
	int iPinIndex;
	int iValue;
	int iSymbol;

	int iHeaderChangeCount = 0;
	char cCurrentHeader = ' ';
	char cLastHeader = ' ';
	BOOL bIsChangeHeader = FALSE;
	BOOL bReqHeaderUpdate = FALSE;
	BOOL bAddData = FALSE;
	UINT wait_cnt = 50000;

    int iCurrentIndex;
    BOOL bGetValue;
    char cPrePacketHead = 0;

	int capacity;
	int data_page;
	int math_page;
		
	SYSTEMTIME time;
	GetLocalTime(&time);
	UINT iSecStart = time.wHour * 3600 + time.wMinute * 60 + time.wSecond;
    UINT iSecCurrent, iSecElapsed, iSecRemain;

    int load_time[11] = {0,};
    int load_data[11] = {0,};
    int total_time[11] = { 0 };

	for (UINT cur_idx = 0; cur_idx < buf_size; cur_idx++) {
		if (cur_idx > wait_cnt) {
			Wait(0);
			wait_cnt = cur_idx + 50000;
		}
        if (cur_idx > iDisplayNext) {
            GetLocalTime(&time);
            iSecCurrent = time.wHour * 3600 + time.wMinute * 60 + time.wSecond;
            iSecElapsed = iSecCurrent - iSecStart;
            UINT ProgressRate = ((UINT64)cur_idx) * 10000 / buf_size;
            if (ProgressRate > 0) {
               iSecRemain = iSecElapsed * 10000 / ProgressRate - iSecElapsed;
            }
            ProgressRate /= 100;
            if (ProgressRate % 10 == 0 && ProgressRate < 100) {
                load_time[ProgressRate / 10] = iSecElapsed;
                load_data[ProgressRate / 10] = m_iHistoryIndex;
                total_time[ProgressRate / 10] = iSecRemain + iSecElapsed;
            }
            			
			if (m_bShowDataControl) {
				m_ctrlProgReading.SetBarColor(RGB(0x00, 0xA9 * ProgressRate / 100 + 0x30, 0x00));
				m_ctrlProgReading.SetPos(ProgressRate);
			}
			m_strTemp.Format("File Reading: %3d%%(%ld/%ld)  Ellapsed %02d:%02d    Remain %02d:%02d  nData : %d", (int)ProgressRate, cur_idx, buf_size, iSecElapsed / 60, iSecElapsed % 60, iSecRemain / 60, iSecRemain % 60, m_iHistoryIndex);
			SetDlgItemText(IDC_Temp, m_strTemp);
			Wait(0);
			iDisplayNext += iDiplayPeriod;			
		}
		BYTE aByte = pBuffer[cur_idx];

		if (bIsCmdMode) {
			switch (iCmdSequence) {
			case 0:
				if (aByte == 'H') { iCmdSequence = 1;  iSubCmd = 0; }
				else if (aByte == 'M') iCmdSequence = 2;
				else if (aByte == 'C') {
					if (m_LoadSettingInfo_in_DataLoading) {
						iCmdSequence = 4; iSubCmd = 0;
					}
					else {
						// Forced Stop
                        cur_idx = buf_size;
						break;
					}
				}
				else if (aByte == 'Z') {  // #N New Configuration
					if (m_LoadSettingInfo_in_DataLoading) {
						iCmdSequence = 5; iSubCmd = -10;
					}
				}
				else if (aByte == 'N') {  // #N New Configuration
					if (m_LoadSettingInfo_in_DataLoading) {
						iCmdSequence = 5; iSubCmd = -1;
					}
					else {
						// Forced Stop
                        cur_idx = buf_size;
						break;
					}
				}
				else { bIsCmdMode = FALSE; }
				break;

			case 1:
				switch (iSubCmd) {
				case 0: if (aByte == 'e') iSubCmd++;  else { bIsCmdMode = FALSE; } break;
				case 1: if (aByte == 'a') iSubCmd++;  else { bIsCmdMode = FALSE; } break;
				case 2: if (aByte == 'd') iSubCmd++;  else { bIsCmdMode = FALSE; } break;
				case 3: if (aByte == 'e') iSubCmd++;  else { bIsCmdMode = FALSE; } break;
				case 4: if (aByte == 'r') iSubCmd++;  else { bIsCmdMode = FALSE; } break;
				case 5: if (aByte == ':') iSubCmd++;  else { bIsCmdMode = FALSE; } break;
				case 6: 
                    if ((aByte >= 'A' && aByte <= 'Z') || (aByte >= 'a' && aByte <= 'z') ||  aByte == '_') {
					m_strPacketHeader.Format("%c", aByte);
					m_cPacketHead = aByte;
                    m_bAcceptNoHeader = (m_cPacketHead == '_');                    
                    CheckDlgButton(IDC_NoHeader, m_bAcceptNoHeader);

					CString strAddress = "Command";
					CString strItem = "LastCommand";                    

					SetRegRoot_RegistryData(strAddress, strItem, m_strPacketHeader);
					LoadHeaderConfiguration();
					if (m_bSaveFile) {
						CString str;
						str.Format("#Header:%c\r\n", m_cPacketHead);
						SaveFile.Write(str, str.GetLength());
					}
				}
						iCmdSequence = 0; bIsCmdMode = FALSE; break;
				default: iCmdSequence = 0; bIsCmdMode = FALSE; break;
				}
				break;

			case 2: if (aByte == 'C') iCmdSequence++; else bIsCmdMode = FALSE; break;

			case 3:
				if (aByte >= '1' && aByte <= '5') {
					m_iPresetMode = aByte - '1';
					UDF;
					ChangeHeader();
				}
				bIsCmdMode = FALSE;
				break;

			case 4: // Old Configuration 
				switch (iSubCmd) {
				case 0: // Get Graph View Info		
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
								m_bDrawGraph[iGraph] = TRUE;
							else
								m_bDrawGraph[iGraph] = FALSE;
						}
					}
					iSubCmd = 1;
					iPinNameIndex = 0;
					iPinIndex = 0;
					break;

				case 1:  // Get Pin Name
					if (iPinNameIndex < 90 && (aByte >= 32 && aByte <= 126)) {
						PinName[iPinNameIndex++] = aByte;
					}
					else if (aByte == 0) {
						PinName[iPinNameIndex] = 0;
						if (m_bGetSettingInLoading)	m_strPinName[iPinIndex].Format("%s", PinName);
						if (++iPinIndex < m_iMaxDataSize) {
							iPinNameIndex = 0;
						}
						else {
							iSubCmd = 2;
							iPinIndex = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 2:  // m_bSelectDraw_GraphData[IGraph][iData]
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
								m_bSelectDraw_GraphData[iGraph][iPinIndex] = TRUE;
							else
								m_bSelectDraw_GraphData[iGraph][iPinIndex] = FALSE;
						}
					}
					if (++iPinIndex >= m_iMaxDataSize) {
						iSubCmd = 3;
					}
					break;

				case 3:	// m_bAutoRange_Graph[iGraph]
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
								m_RealGrpahInfo[iGraph].bAutoRange_Graph = TRUE;
							else
								m_RealGrpahInfo[iGraph].bAutoRange_Graph = FALSE;
						}
					}
					iSubCmd = 4;
					iPinIndex = 0;
					iDigit = 0;
					break;

				case 4:  // m_strRangeMax[0]
					if (iDigit == 0) {
						iSymbol = 2 * aByte - 1;
						iDigit = 1;
					}
					else if (iDigit == 1) {
						iDigit = 2;
						iValue = aByte;
					}
					else if (iDigit == 2) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_RealGrpahInfo[iPinIndex].fRangeMax = iValue * iSymbol;
						if (++iPinIndex >= MAX_GRAPH_COUNT) {
							iSubCmd = 5;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					break;

				case 5: // m_strRangeMin[0]
					if (iDigit == 0) {
						iSymbol = 2 * aByte - 1;
						iDigit = 1;
					}
					else if (iDigit == 1) {
						iDigit = 2;
						iValue = aByte;
					}
					else if (iDigit == 2) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_RealGrpahInfo[iPinIndex].fRangeMin = iValue * iSymbol;
						if (++iPinIndex >= MAX_GRAPH_COUNT) {
							iSubCmd = 6;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					break;

				case 6: // m_iDisplaySize
					if (iDigit == 0) { iDigit++; iValue = aByte; }
					else if (iDigit >= 1 && iDigit <= 2) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 3) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) {
							iDisplaySize = iValue;
							iDisplaySize = max(1, min(iValue, m_iHistoryIndex));
						}
						iSubCmd = 7;
						iDigit = 0;
					}
					break;

				case 7: // m_strIndexNumber
					if (iDigit == 0) { iDigit++; iValue = aByte; }
					else if (iDigit >= 1 && iDigit <= 2) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 3) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) {
							m_strIndexNumber.Format("%d", iValue);
							m_iCurrentIndex = iValue;
						}
						iSubCmd = 8;
						iDigit = 0;
						iPinNameIndex = 0;
					}
					break;

				case 8:  // MathPinName
					if (iPinNameIndex < 90 && (aByte >= 32 && aByte <= 126)) {
						PinName[iPinNameIndex++] = aByte;
					}
					else if (aByte == 0) {
						PinName[iPinNameIndex] = 0;
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].strName.Format("%s", PinName);
						if (++iPinIndex < m_iMaxMathSize) {
							iPinNameIndex = 0;
						}
						else {
							iSubCmd = 9;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 9: // User Math PinName
					if (aByte == 1 || aByte == 0) {
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].bUserName = aByte;
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 10;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 10: // Use MathProbe
					if (aByte == 1 || aByte == 0) {
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].bValid = aByte;
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 11;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 11: // MathProbe Data1							
					if (m_bGetSettingInLoading) m_MathData[iPinIndex].iDataID1 = aByte;
					if (++iPinIndex >= m_iMaxMathSize) {
						iSubCmd = 12;
						iPinIndex = 0;
						iDigit = 0;
					}
					break;

				case 12: // MathProbe operator							
					if (m_bGetSettingInLoading) m_MathData[iPinIndex].iOperator = aByte;
					if (++iPinIndex >= m_iMaxMathSize) {
						iSubCmd = 13;
						iPinIndex = 0;
						iDigit = 0;
					}
					break;

				case 13: // MathProbe Data2							
					if (m_bGetSettingInLoading) m_MathData[iPinIndex].iDataID2 = aByte;
					if (++iPinIndex >= m_iMaxMathSize) {
						iSubCmd = 14;
						iPinIndex = 0;
						iDigit = 0;
					}
					break;

				case 14: // MathProbe nData
					if (iDigit == 0) { iDigit++; iValue = aByte; }
					else if (iDigit >= 1 && iDigit <= 2) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 3) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].nData = iValue;
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 15;
							iPinIndex = 0;
						}
					}
					break;

				case 15: // MathProbe fValue
					if (iDigit == 0) { iDigit++;  iSymbol = 2 * aByte - 1; }
					else if (iDigit == 1) { iDigit++; iValue = aByte; }
					else if (iDigit >= 2 && iDigit <= 3) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 4) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].fValue = ((double)iValue) / (10000 * iSymbol);
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 16;
							iPinIndex = 0;
						}
					}
					break;

				case 16: // MathPrboe Display Graph
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
                                m_MathData[iPinIndex].bSelectGraph[iGraph] = TRUE;
							else
                                m_MathData[iPinIndex].bSelectGraph[iGraph] = FALSE;
						}
					}
					if (++iPinIndex >= m_iMaxMathSize) {
						if (m_bGetSettingInLoading && buf_size - cur_idx < MAXIMUM_DATA_SIZE) {
							UDF;
							CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
							InvalidateRect(&rect);
							DrawInit();
							OptimizeGraphCount();
							OnChangeRange();
							OnEnChangeIndexnumber();
							bIsCmdMode = FALSE;
						}
						else {
							iSubCmd = 17;
							iPinIndex = 0;
							iDigit = 0;
							iValue = 0;
						}
					}
					break;

				case 17: // m_bUseIntData
					m_bUseIntData = aByte;
					iSubCmd = 18;
					break;

				case 18:
					if (iDigit == 0) { iDigit++;  iSymbol = 2 * aByte - 1; }
					else if (iDigit == 1) { iDigit++; iValue = aByte; }
					else if (iDigit <= 3) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 4) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading && m_bUseIntData == FALSE) {
							m_fCoff[iPinIndex] = ((double)iValue / 10000 * iSymbol);
						}

						if (++iPinIndex >= m_iMaxDataSize) {
							if (m_bGetSettingInLoading) {
								UDF;
								CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
								InvalidateRect(&rect);
								DrawInit();
								OptimizeGraphCount();
								OnChangeRange();
								OnEnChangeIndexnumber();
							}
							bIsCmdMode = FALSE;
						}
					}
					str = "";
					break;

				default:
					bIsCmdMode = FALSE; break;
				}
				break;

			case 5:  // NEW Configuration
				switch (iSubCmd) {
				case -10:  // dynamic 변경사항 반영  
					bIsCmdMode = FALSE;
					if (aByte == '`' && cur_idx + 10 < buf_size) {
                        cur_idx++;
						capacity = 0;
						for (int c = 0; c < 8; c++) {
							capacity = capacity * 10 + pBuffer[cur_idx++] - '0';
						}
						data_page = pBuffer[cur_idx++] - '0';
						math_page = pBuffer[cur_idx++] - '0';
						bool bReqInitHistoryData = false;
						// 유효성 check						
						if (data_page < 1 || data_page > 4) break;
						if (math_page < 1 || math_page > 4) break;

						if (capacity > m_iHistoryCapacity) { bReqInitHistoryData = true; }
						if (m_iDataPage != data_page) { bReqInitHistoryData = true; }
						if (m_iMathPage != math_page) { bReqInitHistoryData = true; }
						if (bReqInitHistoryData) {
							m_iDataPage = data_page;
							m_iMathPage = math_page;
							m_iMaxDataSize = MAX_DATACONTROL_SIZE * data_page;
							m_iMaxMathSize = MAX_MATHCONTROL_SIZE * math_page;
							m_iHistoryCapacity = capacity;
							InitHistoryData();
						}
					}
					break;

				case -1:
					if (aByte == '`') { iSubCmd = 0; }
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 0:
					// Get Graph View Info		
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
								m_bDrawGraph[iGraph] = TRUE;
							else
								m_bDrawGraph[iGraph] = FALSE;
						}
					}
					iSubCmd = 1;
					iPinNameIndex = 0;
					iPinIndex = 0;
					break;

				case 1:  // Get Pin Name
					if (iPinNameIndex < 90 && (aByte >= 32 && aByte <= 126)) {
						PinName[iPinNameIndex++] = aByte;
					}
					else if (aByte == 0) {
						PinName[iPinNameIndex] = 0;
						if (m_bGetSettingInLoading)	m_strPinName[iPinIndex].Format("%s", PinName);
						if (++iPinIndex < m_iMaxDataSize) {
							iPinNameIndex = 0;
						}
						else {
							iSubCmd = 2;
							iPinIndex = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 2:  // m_bSelectDraw_GraphData[IGraph][iData]
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
								m_bSelectDraw_GraphData[iGraph][iPinIndex] = TRUE;
							else
								m_bSelectDraw_GraphData[iGraph][iPinIndex] = FALSE;
						}
					}
					if (++iPinIndex >= m_iMaxDataSize) {
						iSubCmd = 3;
					}
					break;

				case 3:	// m_bAutoRange_Graph[iGraph]
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
								m_RealGrpahInfo[iGraph].bAutoRange_Graph = TRUE;
							else
								m_RealGrpahInfo[iGraph].bAutoRange_Graph = FALSE;
						}
					}
					iSubCmd = 4;
					iPinIndex = 0;
					iDigit = 0;
					break;

				case 4:  // m_strRangeMax[0]
					if (iDigit == 0) {
						iSymbol = 2 * aByte - 1;
						iDigit = 1;
					}
					else if (iDigit == 1) {
						iDigit = 2;
						iValue = aByte;
					}
					else if (iDigit == 2) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_RealGrpahInfo[iPinIndex].fRangeMax = iValue * iSymbol;
						if (++iPinIndex >= MAX_GRAPH_COUNT) {
							iSubCmd = 5;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					break;

				case 5: // m_strRangeMin[0]
					if (iDigit == 0) {
						iSymbol = 2 * aByte - 1;
						iDigit = 1;
					}
					else if (iDigit == 1) {
						iDigit = 2;
						iValue = aByte;
					}
					else if (iDigit == 2) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_RealGrpahInfo[iPinIndex].fRangeMin = iValue * iSymbol;
						if (++iPinIndex >= MAX_GRAPH_COUNT) {
							iSubCmd = 6;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					break;

				case 6: // m_iDisplaySize
					if (iDigit == 0) { iDigit++; iValue = aByte; }
					else if (iDigit >= 1 && iDigit <= 2) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 3) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) {
							iDisplaySize = max(1, min(iValue, m_iHistoryIndex));
						}
						iSubCmd = 7;
						iDigit = 0;
					}
					break;

				case 7: // m_strIndexNumber
					if (iDigit == 0) { iDigit++; iValue = aByte; }
					else if (iDigit >= 1 && iDigit <= 2) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 3) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) {
							m_strIndexNumber.Format("%d", iValue);
							m_iCurrentIndex = iValue;
						}
						iSubCmd = 8;
						iDigit = 0;
						iPinNameIndex = 0;
					}
					break;

				case 8:  // MathPinName
					if (iPinNameIndex < 90 && (aByte >= 32 && aByte <= 126)) {
						PinName[iPinNameIndex++] = aByte;
					}
					else if (aByte == 0) {
						PinName[iPinNameIndex] = 0;
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].strName.Format("%s", PinName);
						if (++iPinIndex < m_iMaxMathSize) {
							iPinNameIndex = 0;
						}
						else {
							iSubCmd = 9;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 9: // User Math PinName
					if (aByte == 1 || aByte == 0) {
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].bUserName = aByte;
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 10;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 10: // Use MathProbe
					if (aByte == 1 || aByte == 0) {
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].bValid = aByte;
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 11;
							iPinIndex = 0;
							iDigit = 0;
						}
					}
					else {
						bIsCmdMode = FALSE;
					}
					break;

				case 11: // MathProbe Data1							
					if (m_bGetSettingInLoading) m_MathData[iPinIndex].iDataID1 = aByte;
					if (++iPinIndex >= m_iMaxMathSize) {
						iSubCmd = 12;
						iPinIndex = 0;
						iDigit = 0;
					}
					break;

				case 12: // MathProbe operator							
					if (m_bGetSettingInLoading) m_MathData[iPinIndex].iOperator = aByte;
					if (++iPinIndex >= m_iMaxMathSize) {
						iSubCmd = 13;
						iPinIndex = 0;
						iDigit = 0;
					}
					break;

				case 13: // MathProbe Data2							
					if (m_bGetSettingInLoading) m_MathData[iPinIndex].iDataID2 = aByte;
					if (++iPinIndex >= m_iMaxMathSize) {
						iSubCmd = 14;
						iPinIndex = 0;
						iDigit = 0;
					}
					break;

				case 14: // MathProbe nData
					if (iDigit == 0) { iDigit++; iValue = aByte; }
					else if (iDigit >= 1 && iDigit <= 2) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 3) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].nData = iValue;
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 15;
							iPinIndex = 0;
						}
					}
					break;

				case 15: // MathProbe fValue
					if (iDigit == 0) { iDigit++;  iSymbol = 2 * aByte - 1; }
					else if (iDigit == 1) { iDigit++; iValue = aByte; }
					else if (iDigit >= 2 && iDigit <= 3) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 4) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading) m_MathData[iPinIndex].fValue = ((double)iValue) / (10000 * iSymbol);
						if (++iPinIndex >= m_iMaxMathSize) {
							iSubCmd = 16;
							iPinIndex = 0;
						}
					}
					break;

				case 16: // MathPrboe Display Graph
					if (m_bGetSettingInLoading) {
						for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
							if (aByte & (0x01 << iGraph))
                                m_MathData[iPinIndex].bSelectGraph[iGraph] = TRUE;
							else
                                m_MathData[iPinIndex].bSelectGraph[iGraph] = FALSE;
						}
					}
					if (++iPinIndex >= m_iMaxMathSize) {
						if (m_bGetSettingInLoading && buf_size - cur_idx < (UINT)m_iMaxDataSize) {
							UDF;
							CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
							InvalidateRect(&rect);
							DrawInit();
							OptimizeGraphCount();
							OnChangeRange();
							OnEnChangeIndexnumber();
							bIsCmdMode = FALSE;
						}
						else {
							iSubCmd = 17;
							iPinIndex = 0;
							iDigit = 0;
							iValue = 0;
						}
					}
					break;

				case 17: // m_bUseIntData
					m_bUseIntData = aByte;
					iSubCmd = 18;
					break;

				case 18:
					if (iDigit == 0) { iDigit++;  iSymbol = 2 * aByte - 1; }
					else if (iDigit == 1) { iDigit++; iValue = aByte; }
					else if (iDigit <= 3) { iDigit++; iValue = 256 * iValue + aByte; }
					else if (iDigit == 4) {
						iValue = 256 * iValue + aByte;
						iDigit = 0;
						if (m_bGetSettingInLoading && m_bUseIntData == FALSE) {
							m_fCoff[iPinIndex] = ((double)iValue / 10000 * iSymbol);
						}

						if (++iPinIndex >= m_iMaxDataSize) {
							if (m_bGetSettingInLoading) {
								UDF;
								CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
								InvalidateRect(&rect);
								CheckMathProbeValid();
								DrawInit();
								OptimizeGraphCount();
								OnChangeRange();
								OnEnChangeIndexnumber();
							}
							bIsCmdMode = FALSE;
						}
					}
					break;

				default:
					bIsCmdMode = FALSE; break;
				}
				break;
			default: bIsCmdMode = FALSE; break;
			}
		}
		else {
			if (aByte == '#') {
				bIsCmdMode = TRUE;
				iCmdSequence = 0;
			}
			else {
                if (aByte >= 32 && aByte <= 126) {
                    cRcvBuffer[iRcvBufferIndex++] = aByte;
                    if (iRcvBufferIndex > 1000) { iRcvBufferIndex = 0; }
                }

				switch (iSequence) {
					// Get Time 
				case -100:
					switch (iSubSequence) {
					case 0: // Hour
						if (aByte >= '0' && aByte <= '9') {
							iHour = 10 * (aByte - '0');
							if (cur_idx + 8 < buf_size) {
								aByte = pBuffer[cur_idx + 1]; if (aByte >= '0' && aByte <= '9') { iHour += aByte - '0'; }
								else { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 2]; if (aByte != ':') { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 3]; if (aByte >= '0' && aByte <= '9') { iMinute = 10 * (aByte - '0'); }
								else { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 4]; if (aByte >= '0' && aByte <= '9') { iMinute += aByte - '0'; }
								else { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 5]; if (aByte != ':') { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 6]; if (aByte >= '0' && aByte <= '9') { iSecond = 10 * (aByte - '0'); }
								else { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 7]; if (aByte >= '0' && aByte <= '9') { iSecond += aByte - '0'; }
								else { iSubSequence = 10;  break; }
								aByte = pBuffer[cur_idx + 8];
								if (aByte == '.') {
									aByte = pBuffer[cur_idx + 9]; if (aByte >= '0' && aByte <= '9') { iMilliSecond = 100 * (aByte - '0'); }
									else { iMilliSecond = 0;  iSubSequence = 20;  break; }
									aByte = pBuffer[cur_idx + 10]; if (aByte >= '0' && aByte <= '9') { iMilliSecond += 10 * (aByte - '0'); }
									else { iSubSequence = 20;  break; }
									aByte = pBuffer[cur_idx + 11]; if (aByte >= '0' && aByte <= '9') { iMilliSecond += aByte - '0'; iRecvChannel = 0; }
									else { iSubSequence = 20;  break; }
									memcpy(&(cRcvBuffer[iRcvBufferIndex]), &(pBuffer[cur_idx + 1]), 11);
									cur_idx += 11;
									iSubSequence = 30;
									break;
								}
								else {
									memcpy(&(cRcvBuffer[iRcvBufferIndex]), &(pBuffer[cur_idx + 1]), 7);
									cur_idx += 7;
									iSubSequence = 30;  break;
								}
							}
						}
						iSubSequence = 10;
						break;

					case 10:
						if (aByte == '\n') { iSubSequence = 0; break; }
						cur_idx++;
						while (cur_idx < buf_size) {
							aByte = pBuffer[cur_idx];
							if (iRcvBufferIndex < sizeof(cRcvBuffer)) {
                                if (aByte >= 32 && aByte <= 126) {
                                    cRcvBuffer[iRcvBufferIndex++] = aByte;
                                    if (iRcvBufferIndex > 1000) { iRcvBufferIndex = 0; }
                                }
								if (aByte == '\n') { iSubSequence = 0; break; }
								else if (aByte == 'b') { iRecvChannel  = 1;}
							}
							cur_idx++;
						}
						break;

					case 30:
						if (aByte == '\n') { iSubSequence = 0; break; }
						if (aByte == ']') {
							iSequence = 0;
							iSubSequence = 0;
							iRcvBufferIndex = 0;
							bHeadCheck = TRUE;
							break;
						}
						cur_idx++;
						do {
							if (cur_idx < buf_size) {
								aByte = pBuffer[cur_idx];
								if (iRcvBufferIndex < sizeof(cRcvBuffer)) {
                                    if (aByte >= 32 && aByte <= 126) {
                                        cRcvBuffer[iRcvBufferIndex++] = aByte;
                                        if (iRcvBufferIndex > 1000) { iRcvBufferIndex = 0; }
                                    }
								}
								if (aByte == '\n') { iSubSequence = 0; break; }
								if (aByte == ']') {
									iSequence = 0;
									iSubSequence = 0;
									iRcvBufferIndex = 0;
									bHeadCheck = FALSE;
									break;
								}								
							}
							cur_idx++;
						} while (cur_idx++ < buf_size);
						break;
					}
					break;

                case 0:
                    if (bHeadCheck) {
                        if (m_bAcceptNoHeader == FALSE) {
                            if ((aByte >= 'a' && aByte <= 'z') || (aByte >= 'A' && aByte <= 'Z')) {
                                if (aByte == m_cPacketHead) {
                                    iSequence = 1000;
                                    iCurrentIndex = -1;
                                    bPeriodic = FALSE;
                                    bMinus = FALSE;
                                    bGetValue = FALSE;
                                    iHeaderChangeCount = 0;
                                }
                                else if (m_bUseAutoHeader_rx) {
                                    if (cPrePacketHead == aByte) {
                                        if (++iHeaderChangeCount > 5) {
                                            str.Format("%c", cPrePacketHead);
                                            SetDlgItemText(IDC_PacketHead, str);
                                        }
                                    }
                                    else {
                                        iHeaderChangeCount = 1;
                                        cPrePacketHead = aByte;
                                    }
                                }
                                bHeadCheck = FALSE;
                            }
                        }
                        else if (aByte >= '0' && aByte <= '9') {
                            iCurrentIndex = -1;
                            bPeriodic = FALSE;
                            bMinus = FALSE;
                            bGetValue = TRUE;
                            iBuffer[++iCurrentIndex] = (aByte - '0');
                            iSequence = 1000;
                            break;
                        }
                        else if (aByte == '-') {
                            iCurrentIndex = -1;
                            bPeriodic = FALSE;
                            bMinus = TRUE;
                            bGetValue = FALSE;
                            iSequence = 1000;
                            break;
                        }
                        else if (aByte == '.') {
                            iCurrentIndex = -1;
                            bPeriodic = TRUE;
                            bMinus = FALSE;
                            bGetValue = FALSE;
                            iSequence = 1000;
                            iOrder = 1;
                            break;
                        }
                        else if (aByte == ' ' || aByte == '\t') {
                        }
                        else {
                            bHeadCheck = FALSE;
                        }
                    }
                    else if (aByte == '\n') { iSequence = -100; }
                    break;

                case 1000:
                {
                    BOOL bContiLoop = TRUE; // Stop GetByte 	
                    while (bContiLoop) {
                        if (aByte == '\r' || aByte == '\n') {
                            if (bGetValue) {
                                if (bMinus) {
                                    iBuffer[iCurrentIndex] = -iBuffer[iCurrentIndex];
                                }

                                if (bPeriodic) {
                                    Buffer[iCurrentIndex] = (double)iBuffer[iCurrentIndex] / iOrder;
                                }
                                else {
                                    Buffer[iCurrentIndex] = iBuffer[iCurrentIndex];
                                }
                            }
                            iSequence = 1100;
                            bContiLoop = FALSE;
                        }
                        else if (bGetValue) {
                            if (aByte >= '0' && aByte <= '9') {
                                if (bPeriodic) {
                                    iOrder *= 10;
                                }
                                if (bGetValue) {
                                    iBuffer[iCurrentIndex] *= 10;
                                    iBuffer[iCurrentIndex] += (aByte - '0');
                                }
                            }
                            else if (aByte == '.') {
                                bPeriodic = TRUE;
                                iOrder = 1;
                            }
                            else {
                                if (iCurrentIndex >= 0) {
                                    if (bMinus) {
                                        iBuffer[iCurrentIndex] = - iBuffer[iCurrentIndex];
                                    }

                                    if (bPeriodic) {
                                        Buffer[iCurrentIndex] = (double)iBuffer[iCurrentIndex] / iOrder;
                                    }
                                    else {
                                        Buffer[iCurrentIndex] = iBuffer[iCurrentIndex];
                                    }                                    
                                }

                                bPeriodic = FALSE;
                                if (aByte == '-') { bMinus = TRUE; }
                                else { bMinus = FALSE; }
                                bGetValue = FALSE;
                            }
                        }
                        else {
                            if (aByte >= '0' && aByte <= '9') {
                                if (++iCurrentIndex >= 100) {
                                    iSequence = 0;
                                    bHeadCheck = FALSE;
                                    bContiLoop = FALSE;
                                }
                                else {
                                    iBuffer[iCurrentIndex] = (aByte - '0');
                                    bGetValue = TRUE;
                                }
                            }
                            else if (aByte == '.') {
                                bPeriodic = TRUE;
                                iOrder = 1;
                            }
                            else if (aByte == '-') {
                                bMinus = TRUE;
                            }
                        }

                        if (bContiLoop) {
                            if (++cur_idx < buf_size) {
                                aByte = pBuffer[cur_idx];
                                if (aByte >= 32 && aByte <= 126) {
                                    cRcvBuffer[iRcvBufferIndex++] = aByte;
                                    if (iRcvBufferIndex > 1000) { iRcvBufferIndex = 0; }
                                }
                            }
                            else { bContiLoop = FALSE; }
                        }
                    }
                }
                if (aByte != '\n') { break; }

                case 1100:
                    if (aByte == '\n') {
                        iCurrentIndex++;
                        iSequence = -100;
                        if (iCurrentIndex < min(m_iMinPacketDataSize, MAX_DATACONTROL_SIZE)) {                            
                            break;
                        }

                        if (iCurrentIndex > max(m_iMaxPacketDataSize, m_iMaxDataSize)) {
                            break;
                        }

                        {
                            UART_DATA current_data = m_dummy_UARTData;
                            for (int i = max(iCurrentIndex, 0); i < m_iMaxDataSize; i++) {
                                current_data.fDataValue[i] = 0;
                                Buffer[i] = 0;
                            }

                            for (int i = 0; i < min(iCurrentIndex, m_iMaxDataSize); i++) {
                                current_data.fDataValue[i] = Buffer[i];
                            }
                            current_data.iRcvTime = iHour * 3600 + iMinute * 60 + iSecond;
                            current_data.iRcvTime_ms = iMilliSecond;
                            bIsHistoryDataCompleted = TRUE;

                            current_data.iHistoryLogIndex = m_iLogIndex + 1;
                            current_data.iHistoryFilterIndex = m_iFilterIndex;
                            current_data.iRecvChannel = iRecvChannel;
                            current_data.Header = m_cPacketHead;

                            if (m_iHistoryIndex == 0) {
                                m_HistoryData[0] = current_data;
                            }
                            else {
                                m_HistoryData.emplace_back(current_data);
                            }
                            m_iHistoryIndex++;
                        }
                        bAddData = TRUE;
                        bIsHistoryDataCompleted = TRUE;
                    }
                    break;

				default:
					iSequence = -100;
					break;
				}

				if (iRcvBufferIndex >= 300 || aByte == '\n') {
					cRcvBuffer[iRcvBufferIndex] = 0;

					str.Format("%s", cRcvBuffer);
					iRcvBufferIndex = 0;

					if (iMilliSecond > 0 || iSecond > 0 && iMinute > 0 || iHour > 0) {

						if (iRecvChannel)
							strLog.Format("%02d:%02d:%02d.%03d b] %s", iHour, iMinute, iSecond, iMilliSecond, (LPCTSTR)str);
						else
							strLog.Format("%02d:%02d:%02d.%03d] %s", iHour, iMinute, iSecond, iMilliSecond, (LPCTSTR)str);

						BOOL bFindFilter = FALSE;

						for (int i = 0; i < MAX_FILTER; i++) {
							if (m_bUseFilterItem[i] && m_iFilterLength[i] > 0) {
								int iFilterPosition = str.Find(m_strFilterItem[i]);
								if (iFilterPosition != -1) {
									CString strFilter;
									strFilter.Format("%s", (LPCTSTR)str.Right(str.GetLength() - iFilterPosition - m_iFilterLength[i]));

									double fValue = GetValue(strFilter);
									int iValue = (int)fValue;
									if (fValue == (double)iValue) {
										SetDlgItemInt(m_IDC_FilterValue[i], iValue);
									}
									else {
										CString strVal;
										strVal.Format("%f", fValue);
										SetDlgItemText(m_IDC_FilterValue[i], strVal);
									}
									bFindFilter = TRUE;
								}
							}
						}

						int iHistoryIndex = m_iHistoryIndex;
						if (bAddData) iHistoryIndex--;
						if (bFindFilter) {
							m_iFilterIndex++;
							CString strFilterLog;
							strFilterLog.Format("%s [%d]", (LPCTSTR)strLog, iHistoryIndex);
							m_ctrlFilterBox.InsertString(m_iFilterIndex, strFilterLog);
							m_iFilterHistoryIndexTable.emplace_back(iHistoryIndex);
							m_iFilterLogIndexTable.emplace_back(m_iLogIndex + 1);
							if (bIsHistoryDataCompleted) m_HistoryData[iHistoryIndex].iHistoryFilterIndex++;
						}
                        
						++m_iLogIndex;
						m_arrLogData.emplace_back(strLog);
						m_iLogHistoryIndexTable.emplace_back(iHistoryIndex);
						m_iLogFilterIndexTable.emplace_back(m_iFilterIndex);
						bAddData = FALSE;
					}

					bIsHistoryDataCompleted = FALSE;
				}
			}
		}
	}	

	if (bReqHeaderUpdate) {
		SetDlgItemText(IDC_PacketHead, m_strPacketHeader);
		OptimizeGraphCount();
	}

	m_iHistoryMove = 0;
	m_bReqGraphUpdate = TRUE;
	UDF;

	m_iDisplaySize = iDisplaySize;
	SetDisplayDataSize();

	m_ctrlProgReading.ShowWindow(FALSE);
	if (m_bShowDataControl) {
		m_ctrlLogList.ShowWindow(TRUE);
		m_ctrlLogList.SetRedraw(TRUE);	
		m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
		ScrollToLastItem();

		m_ctrlFilterBox.ShowWindow(TRUE);
		m_ctrlFilterBox.SetRedraw(TRUE);
	}

	// 새 항목에만 선택 + 포커스 설정
	m_ctrlLogList.SetFocus();
	m_ctrlLogList.SetItemState(m_iLogIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	// 화면에 보이도록 스크롤
	m_ctrlLogList.EnsureVisible(m_iLogIndex, FALSE);

	// 리스트 컨트롤에 포커스 주기
	m_ctrlLogList.SetFocus();
	m_ctrlFilterBox.SetCurSel(m_iFilterIndex);
	UDF;
	ReCalculationMathData();

	OptimizeGraphCount();
	m_bReqGraphUpdate = TRUE;

    load_time[10] = iSecElapsed;
    load_data[10] = m_iHistoryIndex;

    str = "Progress[%]  EllaspseTime  Est.TotalTime   nData     (delta time,  delta nData)";
    for (int i = 1; i < 11; i++) {
        CString strTemp = str;
        str.Format("%s\n     %3d      %4dsec    %5dsec    %7d  (%3ds  %4d)", (LPCTSTR)strTemp, i*10, load_time[i], total_time[i], load_data[i], load_time[i] - load_time[i-1], load_data[i] - load_data[i-1]);
    }
    // AfxMessageBox(str);
	m_iAutosave_EllapsedTime = 0;
	m_iAutosave_LogCount = 0;
}

void CSerialComDlg::OnBnClickedClearwatchlist() {
	m_ctrlFilterBox.ResetContent();
	m_iFilterIndex = 0;
	m_ctrlFilterBox.InsertString(m_iFilterIndex, " ");
	
	for (int i = 0; i < MAX_FILTER; i++) {
		SetDlgItemInt(m_IDC_FilterValue[i], 0);
	}
}

void CSerialComDlg::ReqResetData(BOOL bReqCheck) {
	if (bReqCheck) {
		if (AfxMessageBox("모든 Data를 지울까요?\r\n(Delete all data?)", MB_YESNO) != IDYES) {
			return;
		}
	}

	InitHistoryData();
	m_iHistoryMove = 0;
	OnBnClickedClearwatchlist();
	OnBnClickedClearmessage();
	m_bReqGraphUpdate = TRUE;
	SetWindowText(TITLE);
	m_iAutosave_EllapsedTime = 0;
	m_iAutosave_LogCount = 0;
}


void CSerialComDlg::OnBnClickedReqresetdata() {
	ReqResetData(TRUE);
}

void CSerialComDlg::OnBnClickedIndexup1() {
	m_bManualAdjust = TRUE;
    m_iDisplaySize *= 2;
	SetDisplayDataSize();
	m_bForcedChangeDataSize = TRUE;
	OnEnChangeDisplaydatasize();
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::OnBnClickedIndexdn1() {
	m_bManualAdjust = TRUE;
    m_iDisplaySize /= 2;
	SetDisplayDataSize();
	m_bForcedChangeDataSize = TRUE;
	OnEnChangeDisplaydatasize();
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::OnBnClickedPresetmode() {
	int iPreSet = m_iPresetMode + 1;
	UDT;
	if (iPreSet != m_iPresetMode + 1) {
		m_bReqGraphUpdate = TRUE;
		ChangeHeader();

		// Log추가
		CString str;
		SYSTEMTIME time;
		GetLocalTime(&time);
	
		str.Format("%02d:%02d:%02d.%03d] Change preset: %d → %d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, iPreSet, m_iPresetMode + 1);
        m_iLogIndex++;
        AddLogData(str);
		str.Format("#MC%d", m_iPresetMode + 1);
		m_iLogIndex++;
		AddLogData(str);

		if ((m_cPacketHead >= 'a' && m_cPacketHead <= 'z') || (m_cPacketHead >= 'A' && m_cPacketHead <= 'Z')) {
			CString strAddress, strItem, strLastMode;
			strAddress.Format("Header\\%c%d", m_cPacketHead, m_cPacketHead);
			strItem = "LastMode";
			strLastMode.Format("%d", m_iPresetMode);
			SetRegRoot_RegistryData(strAddress, strItem, strLastMode);
		}
	}
}

CString CSerialComDlg::GetLastComPort(void) {
	CString strAddress = "Common";
	CString strItem = "LastComPort";

	return GetRegRoot_RegistryData(strAddress, strItem);
}

CString CSerialComDlg::GetLastComPort2(void) {
	CString strAddress = "Common";
	CString strItem = "LastComPort2";

	return GetRegRoot_RegistryData(strAddress, strItem);
}

CString CSerialComDlg::GetLastComPort3(void) {
	CString strAddress = "Common";
	CString strItem = "LastComPort3";

	return GetRegRoot_RegistryData(strAddress, strItem);
}

void CSerialComDlg::SetLastComPort(const CString& strComPort) {
	if (strComPort == "")return;
	CString strAddress = "Common";
	CString strItem = "LastComPort";
	SetRegRoot_RegistryData(strAddress, strItem, strComPort);
}

void CSerialComDlg::SetLastComPort2(const CString& strComPort) {
	if (strComPort == "")return;
	CString strAddress = "Common";
	CString strItem = "LastComPort2";
	SetRegRoot_RegistryData(strAddress, strItem, strComPort);
}

void CSerialComDlg::SetLastComPort3(const CString& strComPort) {
	if (strComPort == "") return;
	CString strAddress = "Common";
	CString strItem = "LastComPort3";
	SetRegRoot_RegistryData(strAddress, strItem, strComPort);
}

void CSerialComDlg::SetLastBaudRate(int iBaud) {
	CString strAddress = "Common";
	CString strItem = "LastBaudRate";
	CString strValue;
	strValue.Format("%d", iBaud);
	SetRegRoot_RegistryData(strAddress, strItem, strValue);
}

void CSerialComDlg::GetLastBaudRate(void) {
	CString strAddress = "Common";
	CString strItem = "LastBaudRate";
	CString strValue;
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") return;
	m_iBaudRate = atoi(strValue);
	UDF;
}

void CSerialComDlg::SetLastBaudRate2(int iBaud) {
	CString strAddress = "Common";
	CString strItem = "LastBaudRate2";
	CString strValue;
	strValue.Format("%d", iBaud);
	SetRegRoot_RegistryData(strAddress, strItem, strValue);
}

void CSerialComDlg::GetLastBaudRate2(void) {
	CString strAddress = "Common";
	CString strItem = "LastBaudRate2";
	CString strValue;
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") return;
	m_iBaudRate2 = atoi(strValue);
	UDF;
}

void CSerialComDlg::SetLastBaudRate3(int iBaud) {
	CString strAddress = "Common";
	CString strItem = "LastBaudRate3";
	CString strValue;
	strValue.Format("%d", iBaud);
	SetRegRoot_RegistryData(strAddress, strItem, strValue);
}

void CSerialComDlg::GetLastBaudRate3(void) {
	CString strAddress = "Common";
	CString strItem = "LastBaudRate3";
	CString strValue;
	strValue = GetRegRoot_RegistryData(strAddress, strItem);
	if (strValue == "") return;
	m_iBaudRate3 = atoi(strValue);
	UDF;
}


void CSerialComDlg::ReqSaveData(BOOL bReqCheck) {
	ReqSaveData(bReqCheck, 0, m_arrLogData.size());
}

void CSerialComDlg::Open_File_with_FullPathname(CString& strFileName, CFile& SaveFile) {
	BOOL bMakeFile = FALSE;
	CString temp;
	temp.Format("%s", (LPCTSTR)strFileName);
	if (m_iSaveMode == 1 && m_strSavePath[1] != "") {
		temp.Format("%s\\%s", (LPCTSTR)m_strSavePath[1], (LPCTSTR)strFileName);
		bMakeFile = SaveFile.Open(temp, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);
	}
	if (bMakeFile == FALSE) {
		temp.Format("%s\\%s", (LPCTSTR)m_strSavePath[0], (LPCTSTR)strFileName);
		SaveFile.Open(temp, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);
	}
	strFileName.Format("%s", (LPCTSTR)temp);
}

void CSerialComDlg::ReqSaveData(BOOL bReqCheck, int iFrom,  int iTo) {
	if (bReqCheck) {
		if (AfxMessageBox("현재 Log를 저장할까요? \r\n(Save the current log?)", MB_YESNO) != IDYES) {
			return;
		}		
	}
	CFile SaveRcvData;
	CString str = "";
	CString strFileName;

	int iDisplaySize = m_iDisplaySize;
	m_iDisplaySize = 10;
	SetDisplayDataSize();

    auto Config = std::make_unique<BYTE[]>(30000);
	int index = 0;

	// Log저장
	SYSTEMTIME time;
	GetLocalTime(&time);

	for (int i = iFrom; i < iTo;  i++) {
		if (SaveRcvData.m_hFile == INVALID_HANDLE_VALUE) {
			CString strPort;
			strPort.Format("%s", (LPCTSTR)strPort1name);
			strPort.Replace("\\", "");
			strPort.Replace(".", "");

			strFileName.Format("Data_%02d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
            CString strTemp = strFileName;
            if (strPort1name != "")	strFileName.Format("%s_R1%s", (LPCTSTR)strTemp, (LPCTSTR)strPort);

			strPort.Format("%s", (LPCTSTR)strPort2name);
			strPort.Replace("\\", "");
			strPort.Replace(".", "");

			if (strPort2name != "")	strFileName.Format("%s_R2%s", (LPCTSTR)strFileName, (LPCTSTR)strPort);
			strFileName += ".CSV";

			Open_File_with_FullPathname(strFileName, SaveRcvData);
			
			// m_iHistoryCapcity, MAX_DATA_SZE, m_iMaxMathSize	
			{				
				index += sizeof(int);
				str.Format("#Z`%08d%d%d", m_iHistoryCapacity, m_iDataPage, m_iMathPage);
				SaveRcvData.Write(str, str.GetLength());				
				SaveRcvData.Write(Config.get(), index);
				index = 0;
			}			

			// Header 저장
			str.Format("#Header:%c\r\n", m_cPacketHead);
			SaveRcvData.Write(str, str.GetLength());
		}

        CString strTemp = str;
		str.Format("%s%s\r\n", (LPCTSTR)strTemp, (LPCTSTR)m_arrLogData[i]);
		str.Replace("  ", " ");
		if (str.GetLength() > 10000) {
			SaveRcvData.Write(str, str.GetLength());
			str = "";
		}

		if (i % 200 == 0) {
			Wait(0);
			iTo = m_arrLogData.size();
		}
	}
	// Configuration저장
	str.Format("#N`");
	SaveRcvData.Write(str, str.GetLength());

	// Graph View 
	int iValue = 0;
	for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
		if (m_bDrawGraph[iGraph]) iValue += (0x01 << iGraph);
	}
	Config[index++] = iValue;

	// Pin Name
	for (int iData = 0; iData < m_iMaxDataSize; iData++) {
		int iLen = m_strPinName[iData].GetLength();
		for (int iChar = 0; iChar < iLen; iChar++) {
			Config[index++] = m_strPinName[iData].GetAt(iChar);
		}
		Config[index++] = 0;
	}

	// m_bSelectDrawGraphData
	for (int iData = 0; iData < m_iMaxDataSize; iData++) {
		iValue = 0;
		for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
			if (m_bSelectDraw_GraphData[iGraph][iData]) iValue += (0x01 << iGraph);
		}
		Config[index++] = iValue;
	}

	// m_bAutoRange_Graph[]
	iValue = 0;
	for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
		if (m_RealGrpahInfo[iGraph].bAutoRange_Graph) iValue += (0x01 << iGraph);
	}
	Config[index++] = iValue;

	// m_strRangeMax[]
	for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
		iValue = (int)m_RealGrpahInfo[iGraph].fRangeMax;
		if (iValue < 0) {
			Config[index++] = 0;
			iValue = -iValue;
		}
		else {
			Config[index++] = 1;
		}
		Config[index++] = iValue >> 8;
		Config[index++] = iValue & 0xFF;
	}

	// m_strRangeMin[]
	for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
		iValue = (int)m_RealGrpahInfo[iGraph].fRangeMin;
		if (iValue < 0) {
			Config[index++] = 0;
			iValue = -iValue;
		}
		else {
			Config[index++] = 1;
		}
		Config[index++] = iValue >> 8;
		Config[index++] = iValue & 0xFF;
	}

	// m_iDisplaySize
	iValue = iDisplaySize >> 16;
	Config[index++] = iValue >> 8;
	Config[index++] = iValue && 0xFF;
	iValue = iDisplaySize & 0xFFFF;
	Config[index++] = iValue >> 8;
	Config[index++] = iValue & 0xFF;

	// m_strIndexNumber
	iValue = atoi(m_strIndexNumber) >> 16;
	Config[index++] = iValue >> 8;
	Config[index++] = iValue & 0xFF;
	iValue = atoi(m_strIndexNumber) & 0xFFFF;
	Config[index++] = iValue >> 8;
	Config[index++] = iValue & 0xFF;

	// MathPinName
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		int iLen = m_MathData[iData].strName.GetLength();
		for (int iChar = 0; iChar < iLen; iChar++) {
			Config[index++] = m_MathData[iData].strName.GetAt(iChar);
		}
		Config[index++] = 0;
	}

	// User Math PinName
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		if (m_MathData[iData].bUserName)
			Config[index++] = 1;
		else
			Config[index++] = 0;
	}

	// Use MathProbe
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		if (m_MathData[iData].bValid)
			Config[index++] = 1;
		else
			Config[index++] = 0;
	}

	// MathProbe Data1
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		Config[index++] = m_MathData[iData].iDataID1;
	}

	// MathProbe operator
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		Config[index++] = m_MathData[iData].iOperator;
	}

	// MathProbe Data2
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		Config[index++] = m_MathData[iData].iDataID2;
	}

	// MathProbe nData
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		iValue = m_MathData[iData].nData >> 16;
		Config[index++] = iValue >> 8;
		Config[index++] = iValue & 0xFF;
		iValue = m_MathData[iData].nData & 0xFFFF;
		Config[index++] = iValue >> 8;
		Config[index++] = iValue & 0xFF;
	}

	// MathProbe fValue
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		double fValue = m_MathData[iData].fValue;
		if (fValue > 0) {
			Config[index++] = 1;
		}
		else {
			Config[index++] = 0;
			fValue = -fValue;
		}
		fValue = fValue * 10000;
		iValue = ((long)fValue) / 65536;
		Config[index++] = iValue / 256;
		Config[index++] = iValue % 256;
		iValue = ((long)fValue) % 65536;
		Config[index++] = iValue / 256;
		Config[index++] = iValue % 256;
	}

	// MathPrboe Display Graph
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		iValue = 0;
		for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
			if (m_MathData[iData].bSelectGraph[iGraph]) iValue += (0x01 << iGraph);
		}
		Config[index++] = iValue;
	}	

	// Add 240824
	Config[index++] = m_bUseIntData;	
	for (int iData = 0; iData < m_iMaxDataSize; iData++) {
		double fValue = m_fCoff[iData];
		if (fValue > 0) {
			Config[index++] = 1;
		}
		else {
			Config[index++] = 0;
			fValue = -fValue;
		}
		fValue = fValue * 10000;
		iValue = ((long)fValue) / 65536;
		Config[index++] = iValue / 256;
		Config[index++] = iValue % 256;
		iValue = ((long)fValue) % 65536;
		Config[index++] = iValue / 256;
		Config[index++] = iValue % 256;
	}

	SaveRcvData.Write(Config.get(), index);

	if (SaveRcvData.m_hFile != INVALID_HANDLE_VALUE) {
		SaveRcvData.Write(str, str.GetLength());
		SaveRcvData.Close();
	}
	m_iDisplaySize = iDisplaySize;
	SetDisplayDataSize();

	m_strTemp.Format("%d ~ %d Log가 저장되었습니다.\r\n(Log saved successfully.)", iFrom, iTo);
	SetDlgItemText(IDC_Temp, m_strTemp);
	if (bReqCheck) {
		str.Format("%s\r\n%s", (LPCTSTR)strFileName, (LPCTSTR)m_strTemp);
		AfxMessageBox(str);
	}

	m_iAutosave_EllapsedTime = 0;
	m_iAutosave_LogCount = 0;
}

void CSerialComDlg::OnBnClickedReqsavedata() {
	ReqSaveData(TRUE);
}

void CSerialComDlg::OnBnClickedExportconfiguration() {
	// Export Current Setting to File

	CFile SaveConfigurationData;
	CString str = "";
	CString strAddress, strItem, strValue, strPresetMode;

	CString strFileName;
	strFileName.Format("Setting_%s(%d)_Mode%d.dat", (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);
	SaveConfigurationData.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);

	// Version
	str.Format("%s\r\n", CONFIGURATION_VERSION);	SaveConfigurationData.Write(str, str.GetLength());

	// Header
	str.Format("#%s #%03d #%1d\r\n", (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);
	SaveConfigurationData.Write(str, str.GetLength());

	// Preset Mode
	if (m_iPresetMode == 0)
		strPresetMode = "";
	else
		strPresetMode.Format("\\Mode_%d", m_iPresetMode);

	// Pin Name
	if (m_strPacketHeader != " " &&  m_strPacketHeader != "") {
		str.Format("%d\r\n", m_iMaxDataSize);
		SaveConfigurationData.Write(str, str.GetLength());

		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			str.Format("%d  %s\r\n", iData, (LPCTSTR)m_strPinName[iData]);
			SaveConfigurationData.Write(str, str.GetLength());
		}
	}
	else {
		str.Format("0 \r\n");
		SaveConfigurationData.Write(str, str.GetLength());
	}

	// Graph 설정
	// Use Data
	str.Format("%d\r\n", m_iGraphCount);
	SaveConfigurationData.Write(str, str.GetLength());
	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		str = "";
		for (int iData = 0; iData < m_iMaxDataSize; iData++) {
			if (m_bSelectDraw_GraphData[iGraph][iData])
				str += " 1";
			else
				str += " 0";
		}
		str += "\r\n";
		SaveConfigurationData.Write(str, str.GetLength());
	}

	// Min-Max Value
	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		str.Format("%s %s %d \r\n", (LPCTSTR)m_strRangeMax[iGraph], (LPCTSTR)m_strRangeMin[iGraph], m_bAutoRange_Graph[iGraph]);
		SaveConfigurationData.Write(str, str.GetLength());
	}

	//Filter view
	str.Format("%d\r\n", MAX_FILTER);
	SaveConfigurationData.Write(str, str.GetLength());
	for (int i = 0; i < MAX_FILTER; i++) {
		str.Format("%d  %s\r\n", m_bUseFilterItem[i], (LPCTSTR)m_strFilterItem[i]);
		SaveConfigurationData.Write(str, str.GetLength());
	}

	// Serial Command
	str.Format("%d\r\n", MAX_BT_COMMAND);
	SaveConfigurationData.Write(str, str.GetLength());
	for (int i = 0; i < MAX_BT_COMMAND; i++) {
		str.Format("%d ", m_BT_Command[i]);
		SaveConfigurationData.Write(str, str.GetLength());
	}
	str.Format("\r\n");
	SaveConfigurationData.Write(str, str.GetLength());

	// Pen Color
	str.Format("%d\r\n", m_iMaxDataSize);
	SaveConfigurationData.Write(str, str.GetLength());
	for (int iData = 0; iData < m_iMaxDataSize; iData++) {
		str.Format("%d %d %d\r\n", m_iPenColor[iData][0], m_iPenColor[iData][1], m_iPenColor[iData][2]);
		SaveConfigurationData.Write(str, str.GetLength());
	}

	// Display size & time stamp
	str.Format("%s %d\r\n", m_iDisplaySize);
	SaveConfigurationData.Write(str, str.GetLength());

	SaveConfigurationData.Close();
}


void CSerialComDlg::OnDropFiles(HDROP hDropInfo) {
	CDialog::OnDropFiles(hDropInfo);
	TCHAR szPathName[MAX_PATH];
	CString strFullPath;

	int nFile = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH);

	DragQueryFile(hDropInfo, 0, szPathName, MAX_PATH);
	DragFinish(hDropInfo);
	CString str;
	str.Format("%s - %s", TITLE, szPathName);
	SetWindowText(str);
	LoadDataFromFile(szPathName);	
}

void CSerialComDlg::OnLbnSelchangeFiltermessage() {
	if (m_iProgramMode == DATA_ACQUISITION_MODE) return;
	int iFilterIndex = m_ctrlFilterBox.GetCurSel();
	int iLogIndex = m_iFilterLogIndexTable[iFilterIndex];
	HighlightLog(iLogIndex);
	OnSelectLogList(iLogIndex);
	m_strTemp.Format("Selecting %dth Data in Filter Log", iFilterIndex);
	SetDlgItemText(IDC_Temp, m_strTemp);
}


void CSerialComDlg::OnLButtonDown(UINT nFlags, CPoint point) {

	::SetCapture(this->GetSafeHwnd());
	BOOL bIsAdustMode = FALSE;
	// Area설정
	// iArea     1: Graph    2: Position   0: 그외
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;

	BOOL bIsMeasure3P = FALSE;	
	if (m_Measure3P.bIsMeasuring) {
		if (iArea == 2) {
			m_Measure3P.bIsClick = TRUE;
			m_Measure3P.Pt[m_Measure3P.Index].SetPoint(point.x, point.y);
			bIsMeasure3P = TRUE;
		}
		else {
			ClearMeasure3P();
		}
		m_bReqGraphUpdate = TRUE;
	}

	if (point.x >= m_iPositionGraphRangeX[1] - 35 && point.x <= m_iPositionGraphRangeX[1] - 2) {
		if (point.y >= m_iPositionGraphRangeY[1] - 25 && point.y <= m_iPositionGraphRangeY[1] - 2) {
			bIsMeasure3P = TRUE;

			if (m_Measure3P.bIsMeasuring) {
				ClearMeasure3P();
			}
			else {
				m_Measure3P.bIsMeasuring = TRUE;
				m_Measure3P.Index = 0;
				m_Measure3P.bIsClick = TRUE;				
			}
			m_bReqGraphUpdate = TRUE;
		}

		if (point.y >= m_iPositionGraphRangeY[1] - 50 && point.y <= m_iPositionGraphRangeY[1] - 27) {
			m_bUseAutoScaleGraphPosition ^= 0x01;
			m_bReqGraphUpdate = TRUE;
		}
	}

	if (point.x >= m_iPositionGraphRangeX[1] - 35 && point.y >= m_iPositionGraphRangeY[1] - 25) {
		if (m_Panning_PositionGraph.isPressed) 	m_Panning_PositionGraph.isPressed = FALSE;
	}
	if (m_FrameAdjusting_PositionGraph.isPressed) m_FrameAdjusting_PositionGraph.isPressed = FALSE;

	if (iArea == 1) {
		if (m_iProgramMode == DATA_ANALYZE_MODE) {
			if (m_bUseStatistics) {
				// Graph 영역 내부 Check  m_iDrawGraphRangeX[0]
				BOOL bSelectRangeLine = FALSE;
				if (point.x >= m_iDrawGraphRangeX[0] && point.x <= m_iDrawGraphRangeX[1] && point.y >= m_iDrawGraphRangeY[m_iSelectGraphCount][1] && point.y <= m_iDrawGraphRangeY[m_iSelectGraphCount][1] + 20) {
					bSelectRangeLine = TRUE;
				}
				else {
					if (point.y <= m_iDrawGraphRangeY[m_iSelectGraphCount][1] && point.y >= m_iDrawGraphRangeY[0][0]) {
						if (point.x >= m_iAdjustSelectPoint[0] - 4 && point.x <= m_iAdjustSelectPoint[0] + 4) bSelectRangeLine = TRUE;
						if (point.x >= m_iAdjustSelectPoint[1] - 4 && point.x <= m_iAdjustSelectPoint[1] + 4) bSelectRangeLine = TRUE;
					}
				}
				
				if (m_iAdjustSelectMode == -1 && bSelectRangeLine) {
					if ((point.x >= m_iAdjustSelectPoint[0] - 10 && point.x <= m_iAdjustSelectPoint[0] + 10) || (point.x >= m_iAdjustSelectPoint[1] - 10 && point.x <= m_iAdjustSelectPoint[1] + 10)) {
						if (abs(m_iAdjustSelectPoint[0] - point.x) < abs(m_iAdjustSelectPoint[1] - point.x)) {
							m_iAdjustSelectMode = 0;
							m_iAdjustSelectPoint[0] = point.x;
						}
						else {
							m_iAdjustSelectMode = 1;
							m_iAdjustSelectPoint[1] = point.x;
						}
						m_bReqGraphUpdate = TRUE;
					}
				}
			}
			
			// 기존 Cursor를 선택하였는지 Check				
			BOOL bConti = TRUE;
			BOOL bFindCursor = FALSE;
			for (int i = 0; i < m_Cursor.nCursor && bConti; i++) {
				if (abs(point.x - m_Cursor.iPos[i]) <= 2) {
					bConti = FALSE;
					m_Cursor.iSelectCursor = i;
					bFindCursor = TRUE;
				}
			}

			// 기존 Cursor와 일치하지 않는다면 시작위치에서 추가항목인지 확인
			if (!bFindCursor) {
				if (abs(point.x - m_iDrawGraphRangeX[0]) <= 2) {
					if (m_Cursor.nCursor < MAX_CURSOR) {
						m_Cursor.iSelectCursor = m_Cursor.nCursor;
						m_Cursor.AddCursor(point.x + 2);
						m_bReqGraphUpdate = TRUE;
					}
				}
			}			

			if (m_iAdjustSelectMode == -1 && m_Cursor.iSelectCursor == -1) {
				if (point.x > m_iDrawGraphRangeX[0] && point.x < m_iDrawGraphRangeX[1]) {
					int iSelectIndex = max(0, min(m_iHistoryCapacity, m_iHistoryIndex + (int)((double)m_iDisplaySize * (m_iHistorySelectPt - m_iDrawGraphRangeX[0]) / m_iDrawGraphSpanX) - m_iHistoryMove - m_iDisplaySize));
					BOOL bIsGraphRange = FALSE;
					for (int i = 0; i < m_iGraphCount; i++) {
						if (point.y > m_iDrawGraphRangeY[i][0] && point.y < m_iDrawGraphRangeY[i][1]) {
							bIsGraphRange = TRUE;
						}
					}

					if (bIsGraphRange) {
						m_bIsMoveMode = TRUE; 
						m_PtMoveStart = point;

						if (CheckControlKeyDown() == FALSE || m_bDrawSelectData == FALSE) {
							m_iHistorySelectPt = point.x;
							int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;
							int idx = iStartIndex + (int)(0.5 + (double)m_iDisplaySize * (m_iHistorySelectPt - m_iDrawGraphRangeX[0]) / m_iDrawGraphSpanX);
							if (idx < 0) idx = 0;
							if (idx >= m_iHistoryIndex) idx = m_iHistoryIndex -1;
							
							m_strIndexNumber.Format("%d", idx);
							m_iCurrentIndex = idx;
							if (0 && m_HistoryData[idx].Header != m_cPacketHead) {
								m_strPacketHeader.Format("%c", m_HistoryData[idx].Header);
								UDF;
								OnEnChangePackethead();
							}
							m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
							m_strTemp = m_arrLogData[m_HistoryData[idx].iHistoryLogIndex];
							HighlightLog(m_HistoryData[idx].iHistoryLogIndex);
							m_ctrlFilterBox.SetCurSel(m_HistoryData[idx].iHistoryFilterIndex);
							UDF;
							m_bReqGraphUpdate = TRUE;
							m_bDrawSelectData = TRUE;
							SetTimer(TIMER_UPDATE_LOG, 50, NULL);
						}
					}
				}
			}
		}

		if (!m_bIsAdjustMode && !m_bUseStatistics) {
			if (point.x - m_iDrawGraphRangeX[1] <= 5 && point.x - m_iDrawGraphRangeX[1] >= -5 && point.y >= GRAPH_START_Y - 5 && point.y <= m_iDrawGraphRangeY[m_iGraphCount - 1][1] - 10)
			{
				m_bAdjustLegendX = TRUE;
				bIsAdustMode = TRUE;
			}

			if (m_iGraphInfoMaxX >= point.x && point.x >= m_iDrawGraphRangeX[0] && m_iGraphCount > 1) {
				for (int i = 0; i < m_iGraphCount - 1; i++) {
					if (point.y >= m_iDrawGraphRangeY[i][1] && point.y <= m_iDrawGraphRangeY[i + 1][0]) {
						m_bAdjustLegendY[i] = TRUE;
						m_iAdjustLegendY = i;
						bIsAdustMode = TRUE;
						break;
					}
				}
			}
		}

		if (bIsAdustMode && !m_bFixGraphFrame) {
			DrawInit();
			m_bIsAdjustMode = TRUE;
			m_bReqGraphUpdate = TRUE;
		}
	}
	else if (iArea == 2 && !bIsMeasure3P) {
		m_Panning_PositionGraph.isPressed = TRUE;
		m_Panning_PositionGraph.P_Start.x = m_Panning_PositionGraph.P_Last.x = point.x;
		m_Panning_PositionGraph.P_Start.y = m_Panning_PositionGraph.P_Last.y = point.y;
	}
	
	if (m_bDisplayPosition && !bIsMeasure3P) {
		if (point.x >= m_iPositionGraphRangeX[0] - 10 && point.x <= m_iPositionGraphRangeX[0] + 5) {
			if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1] ) {
				m_FrameAdjusting_PositionGraph.isPressed = TRUE;
				m_FrameAdjusting_PositionGraph.P_Start.x = m_FrameAdjusting_PositionGraph.P_Last.x = point.x;
				m_FrameAdjusting_PositionGraph.P_Start.y = m_FrameAdjusting_PositionGraph.P_Last.y = point.y;
			}
		}
	}

	if (m_bShowDataControl && !m_bFixGraphFrame) {
		//Log Filter 창 크기 조절 ( Vertical)
		if (point.y >= m_iRangeLogFilterRatio[0] - 3 && point.y <= m_iRangeLogFilterRatio[1] + 3) {
			if (point.x >= 3 && point.x <= m_iRangeLogFilterWidth) {
				if (m_AdjustLogFilter_H.isPressed == FALSE) {
					m_AdjustLogFilter_V.isPressed = TRUE;
					CClientDC dc(this);
					dc.SelectObject(&m_PenAdjust);
					dc.MoveTo(3, (m_iRangeLogFilterRatio[0] + m_iRangeLogFilterRatio[1]) / 2);
					dc.LineTo(m_iRangeLogFilterWidth, (m_iRangeLogFilterRatio[0] + m_iRangeLogFilterRatio[1]) / 2);
				}
			}
		}

		//Log Filter 창 크기 조절 ( Horizontal)
		if (0 && point.x > m_iDrawGraphRangeX[0] - 75&& point.x < m_iDrawGraphRangeX[0] - 62 && point.y > min(DRAW_CONTROL_START_Y, m_iRangeLogFilterVertical)) {
			if (m_AdjustLogFilter_V.isPressed == FALSE) {
				m_AdjustLogFilter_H.isPressed = TRUE;
				CClientDC dc(this);
				dc.SelectObject(&m_PenAdjust);
				dc.MoveTo(m_iRangeLogFilterWidth + 2, min(DRAW_CONTROL_START_Y, m_iRangeLogFilterVertical));
				dc.LineTo(m_iRangeLogFilterWidth + 2, m_iWndSizeY);
			}
		}
				
		if (m_iRangeDataControlVertical - 3 <= point.y && point.y <= m_iRangeDataControlVertical + 5) {
			if (0 && point.x <= m_iRangeLogFilterWidth) {
				m_AdjustDataWnd_V.isPressed = TRUE;
				CClientDC dc(this);
				dc.SelectObject(&m_PenAdjust);
				dc.MoveTo(2, m_iRangeDataControlVertical);
				dc.LineTo(m_iRangeLogFilterWidth, m_iRangeDataControlVertical);
			}
		}	
	}

    if (m_bShowMathControl && !m_bFixGraphFrame) {
        if (point.y > m_iPos_MathEnd - 3 && point.y < m_iPos_MathEnd + 5 && point.x <= m_iRangeLogFilterWidth) {
            m_AdjustMathWnd_V.isPressed = TRUE;
            CClientDC dc(this);
            dc.SelectObject(&m_PenAdjust);
            dc.MoveTo(2, m_iPos_MathEnd + 1);
            dc.LineTo(m_iRangeLogFilterWidth, m_iPos_MathEnd + 1);
        }
    }    

	// Timeline Control bar check
	if (point.x > m_iDrawGraphRangeX[0] && m_iProgramMode == DATA_ANALYZE_MODE) {
		if (point.y >= GRAPH_RANGE_Y && point.y <= GRAPH_START_Y) {
			BOOL bCheckReleaseFit = m_bFitToScreen;
			if (m_iTimeLineX[0] == m_iTimeLineX[1]) {
				if (point.x == m_iTimeLineX[0]) {
					m_bTimelineControl[0] = TRUE;
					m_bTimelineControl[1] = FALSE;
					m_bTimelineControl[2] = FALSE;
					m_iTimelineStartPoint = point.x;
					m_bManualAdjust = TRUE;
				}
				else if (point.x < m_iTimeLineX[0] && point.x > m_iTimeLineX[0] - 10) {
					m_bTimelineControl[0] = FALSE;
					m_bTimelineControl[1] = TRUE;
					m_bTimelineControl[2] = FALSE;
					m_iTimelineStartPoint = point.x;
					m_bManualAdjust = TRUE;
				}
				else if (point.x > m_iTimeLineX[1] && point.x < m_iTimeLineX[1] + 10) {
					m_bTimelineControl[0] = FALSE;
					m_bTimelineControl[1] = FALSE;
					m_bTimelineControl[2] = TRUE;
					m_bManualAdjust = TRUE;
					m_iTimelineStartPoint = point.x;
				}
			}
			else {
				if (point.x > m_iTimeLineX[0] && point.x < m_iTimeLineX[1]) {
					m_bTimelineControl[0] = TRUE;
					m_bTimelineControl[1] = FALSE;
					m_bTimelineControl[2] = FALSE;
					m_bManualAdjust = TRUE;
					m_iTimelineStartPoint = point.x;
				}
				else if (point.x <= m_iTimeLineX[0] && point.x > m_iTimeLineX[0] - 10) {
					m_bTimelineControl[0] = FALSE;
					m_bTimelineControl[1] = TRUE;
					m_bTimelineControl[2] = FALSE;
					m_bManualAdjust = TRUE;
					m_iTimelineStartPoint = point.x;
				}
				else if (point.x >= m_iTimeLineX[1] && point.x < m_iTimeLineX[1] + 10) {
					m_bTimelineControl[0] = FALSE;
					m_bTimelineControl[1] = FALSE;
					m_bTimelineControl[2] = TRUE;
					m_bManualAdjust = TRUE;
					m_iTimelineStartPoint = point.x;
				}
			}
			if (bCheckReleaseFit && m_bManualAdjust) {
				m_bFitToScreen = FALSE;
				CheckDlgButton(IDC_FitToMaxData, FALSE);
			}
		}
	}

	// Graph Use Select Check	
	if (m_bShowDataControl){
		int iMoveControl_H = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H;
		int x_min = 270 + iMoveControl_H - 3;
		int x_max = 270 + (int)(MAX_GRAPH_COUNT * 19.7) + iMoveControl_H - 3 + 14;
		int y_min = DRAW_DATA_CONTROL_Y0 - 1;
		int y_max = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * (m_iShowDataControl - 1) - 1 + 14;

		if (point.x > x_min && point.x < x_max) {
			int iGraph = -1; int iData = -1;

			for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
				int x = 270 + (int)(j * 19.7) + iMoveControl_H - 3;
				if (point.x > x && point.x < x + 14) {
					iGraph = j; break;
				}
			}

			if (iGraph > -1) {
				if (point.y > y_min && point.y < y_max) {
					for (int i = 0; i < m_iShowDataControl; i++) {
						int y = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * i - 1;
						if (point.y > y && point.y < y + 14) {
							iData = i;
							break;
						}
					}
				}
				if (iData != -1) {
					if (m_bDrawGraph[iGraph]) {
						m_bSelectDraw_GraphData[iGraph][iData + m_iControlDataOffset] ^= 0x01;
                        m_bReqUpdateControlDraw = TRUE;
						OptimizeGraphCount();
						ShowDataMathControl();
					}
				}
				else {
					// Math Select Check
					int y0;
					if (m_bShowMathControl) {
						y0 = 355; 
					}
					else {
						y0 = 653; 
					}
					int DeltaY = 0;
					DeltaY = DRAW_CONTROL_DY * (m_iShowDataControl - MAX_DATACONTROL_SIZE) + DRAW_CONTROL_START_Y;
					y0 += DeltaY;

					y_min = 360 + DeltaY - 1;
					y_max = 360 + DeltaY + DRAW_CONTROL_DY * (m_iShowMathControl - 1) - 1 + 14;

					if (point.y > y_min && point.y < y_max) {
						for (int i = 0; i < m_iShowMathControl; i++) {
							int y = 360 + DeltaY + DRAW_CONTROL_DY * i - 1;
							if (point.y > y && point.y < y + 14) {
								iData = i;
								break;
							}
						}
					}
					if (iData != -1) {
						if (m_bDrawGraph[iGraph]) {
                            m_MathData[iData + m_iControlMathOffset].bSelectGraph[iGraph] ^= 0x01;
                            m_bReqUpdateControlDraw = TRUE;
							OptimizeGraphCount();
							ShowDataMathControl();
						}
					}
				}
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CSerialComDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	::ReleaseCapture();

	// Area설정
	// iArea     1: Graph    2: Position   0: 그외	
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;
	BOOL bIsAdjustY = FALSE;

	m_bTimelineControl[0] = FALSE;
	m_bTimelineControl[1] = FALSE;
	m_bTimelineControl[2] = FALSE;

	if (m_Cursor.iSelectCursor != -1) {
		if (iArea == 1) {
			if (point.x < m_iDrawGraphRangeX[1])
				m_Cursor.iPos[m_Cursor.iSelectCursor] = point.x;
			else
				m_Cursor.RemoveCursor(m_Cursor.iSelectCursor);
		}
		else {
			m_Cursor.RemoveCursor(m_Cursor.iSelectCursor);
		}
		m_Cursor.iSelectCursor = -1;
		m_bReqGraphUpdate = TRUE;
	}

	if (m_bIsAdjustMode) {
		if (m_bAdjustLegendX) {
			if (point.x <= m_iDrawGraphRangeX[0] + 100) point.x = m_iDrawGraphRangeX[0] + 100;
			m_bAdjustLegendX = FALSE;
			int iLegendX = min(600, max(20, m_iGraphInfoMaxX - point.x + 1));
			int iDelta;
			if (m_bDisplayPosition)
				iDelta = iLegendX - m_iLegendX_PositionGraph;
			else 
				iDelta = iLegendX - m_iLegendX;

			m_iDrawGraphRangeX[1] -= iDelta;

			while (m_iDrawGraphSpanX <= 100 && iLegendX > 30) {
				m_iDrawGraphRangeX[1] += 10;
				iLegendX -= 10;
				m_iDrawGraphSpanX = m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0];
			}
			m_iDrawGraphSpanX = m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0];
			m_iDrawGraphMidX = (m_iDrawGraphRangeX[1] + m_iDrawGraphRangeX[0]) / 2;
			if (m_bDisplayPosition)
				m_iLegendX_PositionGraph = iLegendX;
			else 
				m_iLegendX = iLegendX;

			m_iGraphInfoMinX = (m_iDrawGraphRangeX[1] + GRAPH_GAP_X);
			m_iTimeDisplaySize = m_iDrawGraphSpanX / 80;
		}

		// 유효성 check
        if (m_bAdjustLegendY[m_iAdjustLegendY]) {
            int idx = m_iAdjustLegendY;
            if (CheckShiftKeyDown() || CheckControlKeyDown()) {
                int isLower = FALSE;
                if (CheckControlKeyDown()) {
                    idx++; isLower = TRUE;
                }

                int curSize[MAX_GRAPH_COUNT];
                for (int i = 0; i < m_iGraphCount; i++) {
                    curSize[i] = m_iDrawGraphRangeY[i][1] - m_iDrawGraphRangeY[i][0];
                }
                int preSize = curSize[idx];
                int reqSize;
                if (isLower) {
                    reqSize = max(30, m_iDrawGraphRangeY[idx][1] - point.y);
                }
                else {
                    reqSize = max(30, point.y - m_iDrawGraphRangeY[idx][0]);
                }

                if (preSize > reqSize) {
                    int iInc = (preSize - reqSize) / (m_iGraphCount - 1);
                    int remain = (preSize - reqSize) % (m_iGraphCount - 1);
                    curSize[idx] = reqSize;
                    for (int i = 0; i < m_iGraphCount; i++) {
                        if (i == idx) continue;
                        curSize[i] += iInc;
                        if (remain > 0) curSize[i]++;
                    }
                }
                else if (preSize < reqSize) {
                    int iDec = (reqSize - preSize) / (m_iGraphCount - 1);
                    int remain = (reqSize - preSize) % (m_iGraphCount - 1);
                    curSize[idx] = reqSize;
                    for (int i = 0; i < m_iGraphCount; i++) {
                        if (i == idx) continue;
                        curSize[i] -= iDec;
                        if (curSize[i] < 30) {
                            remain += 30 - curSize[i];
                            curSize[i] = 30;
                        }
                    }
                    // nCount개 Graph를 remain만큼 줄이자
                    while (remain > 0) {
                        for (int i = 0; i < m_iGraphCount; i++) {
                            if (curSize[i] > GRAPH_MIN_HEIGHT && remain > 0) {
                                curSize[i]--; remain--;
                            }
                        }
                    }
                }

                m_iDrawGraphRangeY[0][1] = m_iDrawGraphRangeY[0][0] + curSize[0];
                for (int i = 1; i < m_iGraphCount - 1; i++) {
                    m_iDrawGraphRangeY[i][0] = m_iDrawGraphRangeY[i - 1][1] + GRAPH_GAP_Y;
                    m_iDrawGraphRangeY[i][1] = m_iDrawGraphRangeY[i][0] + curSize[i];
                }
                m_iDrawGraphRangeY[m_iGraphCount - 1][0] = m_iDrawGraphRangeY[m_iGraphCount - 2][1] + GRAPH_GAP_Y;
            }
            else {
                if (point.y - m_iDrawGraphRangeY[idx][0] < GRAPH_MIN_HEIGHT)
                    point.y = m_iDrawGraphRangeY[idx][0] + GRAPH_MIN_HEIGHT;
                else if (m_iDrawGraphRangeY[idx + 1][1] - point.y <= GRAPH_MIN_HEIGHT)
                    point.y = m_iDrawGraphRangeY[idx + 1][1] - GRAPH_MIN_HEIGHT;
                m_iDrawGraphRangeY[idx][1] = point.y - GRAPH_GAP_Y / 2;
                m_iDrawGraphRangeY[idx + 1][0] = point.y + GRAPH_GAP_Y / 2;
            }

            // update height
            int adjustY = m_iDrawGraphRangeY[m_iGraphCount - 1][1] - m_iDrawGraphRangeY[0][0] - GRAPH_GAP_Y * (m_iGraphCount - 1) - GRAPH_MIN_HEIGHT * m_iGraphCount;
            if (adjustY > 0) {                
                int sum_height = 0;
                for (int i = 0; i < m_iGraphCount; i++) {
                    int idx = m_RealGrpahInfo[i].iDraw2RealGraphID;
                    sum_height += m_RealGrpahInfo[idx].iHeight;
                }

                if (sum_height > 0) {
                    double rate = (double)sum_height / adjustY;
                    for (int i = 0; i < m_iGraphCount; i++) {
                        int idx = m_RealGrpahInfo[i].iDraw2RealGraphID;
                        int dH = m_iDrawGraphRangeY[i][1] - m_iDrawGraphRangeY[i][0] - GRAPH_MIN_HEIGHT;
                        m_RealGrpahInfo[idx].iHeight = max(0, (int)(dH * rate + 0.5));
                    }
                }
            }

			m_bAdjustLegendY[m_iAdjustLegendY] = FALSE;
			bIsAdjustY = TRUE;
		}

		if (bIsAdjustY) {
            ShowGraphControlbyHeight();
            CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
            InvalidateRect(&rect);
		}
		DrawInit();

		m_bReqGraphUpdate = TRUE;
		m_bIsAdjustMode = FALSE;
	}

	if (m_iAdjustSelectMode != -1) {
		m_iAdjustSelectMode = -1;
		m_bReqGraphUpdate = TRUE;
	}

	if (m_Measure3P.bIsMeasuring && iArea == 2 && m_Measure3P.bIsClick) {
		m_Measure3P.Pt[m_Measure3P.Index].SetPoint(point.x, point.y);
		if (m_Measure3P.Index++ >= 3) {
			// Dispaly result		
			double x1 = m_Measure3P.Pt[1].x;
			double x2 = m_Measure3P.Pt[2].x;
			double x3 = m_Measure3P.Pt[3].x;
			double y1 = m_Measure3P.Pt[1].y;
			double y2 = m_Measure3P.Pt[2].y;
			double y3 = m_Measure3P.Pt[3].y;

			double a1 = x1 - x2;
			double b1 = y1 - y2;
			double c1 = x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1;
			double a2 = x2 - x3;
			double b2 = y2 - y3;
			double c2 = x3 * x3 - x2 * x2 + y3 * y3 - y2 * y2;

			double m = b1 * a2 - b2 * a1;
			if (m != 0) {
				double b = (c1 * a2 - c2 * a1) / m;
				if (a1 != 0) {
					double a = (c1 - b1 * b) / a1;
					double c = -x1 * x1 - a * x1 - y1 * y1 - b * y1;

					int x0 = (int)(-a / 2);
					int y0 = (int)(-b / 2);
					int r = (int)(a* a / 4. + b * b / 4. - c);

					if (r > 0) {
						r = (int)sqrt(r);
						CString str1, str2, str3, str;
						str1.Format("< Circle Measurement >\r\n\r\n");
						str2.Format("%sCenter Position\r\n X-direction: %d mm (%d pt)", (LPCTSTR)str1, m_fMinValueX_PositionGraph + (x0 - m_iPositionGraphRangeX[0]) * (m_fMaxValueX_PositionGraph- m_fMinValueX_PositionGraph) / (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]), x0);
						str3.Format("%s\r\n Y-direction: %d mm (%d pt)", (LPCTSTR)str2, m_fMinValueY_PositionGraph - (y0 - m_iPositionGraphRangeY[1]) * (m_fMaxValueY_PositionGraph - m_fMinValueY_PositionGraph) / (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]), y0);
						str.Format("%s\r\n Radius : %.0f mm (%d pt)", (LPCTSTR)str3, m_fValueScale_PositionGraph * r, r);
						AfxMessageBox(str);
					}
				}
			}
			ClearMeasure3P();
		}	
		UDF;
		m_bReqGraphUpdate = TRUE;
	}
	m_Measure3P.bIsClick = FALSE;

	if (m_Panning_PositionGraph.isPressed) {
		m_Panning_PositionGraph.isPressed = FALSE;
		if (iArea == 2) {
			int iDx = (int)(m_fValueScale_PositionGraph * (m_Panning_PositionGraph.P_Last.x - m_Panning_PositionGraph.P_Start.x));
			int iDy = (int)(m_fValueScale_PositionGraph * (m_Panning_PositionGraph.P_Last.y - m_Panning_PositionGraph.P_Start.y));
			
			m_fMidValueX_PositionGraph -= iDx;
			m_fMidValueY_PositionGraph += iDy;

			if (m_Panning_PositionGraph.P_Last != m_Panning_PositionGraph.P_Start ) m_bUseAutoScaleGraphPosition = FALSE;
			CalculationScale();
		}
	}

	if (m_FrameAdjusting_PositionGraph.isPressed) {
		m_FrameAdjusting_PositionGraph.isPressed = FALSE;
		if (m_bDisplayPosition) {
			m_iFrameSizeGraphPosition = m_iWndSizeX - point.x;
			SetFrameSize();
		}		
	}	
	if (m_AdjustLogFilter_V.isPressed) {
		m_AdjustLogFilter_V.isPressed = FALSE;
		SetLogFilterSize_V(point); 
	}

	if (m_AdjustDataWnd_V.isPressed) {
		m_AdjustDataWnd_V.isPressed = FALSE;
		SetDataWndSize_V(point);
	}

    if (m_AdjustMathWnd_V.isPressed) {
        m_AdjustMathWnd_V.isPressed = FALSE;
        SetMathWndSize_V(point);
    }    

	if (m_AdjustLogFilter_H.isPressed) {
		m_AdjustLogFilter_H.isPressed = FALSE;
		OnSize(0, m_iWndSizeX, m_iWndSizeY);
        CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
        InvalidateRect(&rect);
		DrawInit();
		m_bReqGraphUpdate = TRUE;
	}
	CDialog::OnLButtonUp(nFlags, point);
}


void CSerialComDlg::ClearMeasure3P(void) {
	m_Measure3P.Index = 0;
	m_Measure3P.bIsClick = FALSE;
	m_Measure3P.bIsMeasuring = FALSE;
	for (int i = 0; i < 4; i++)
		m_Measure3P.Pt[i].SetPoint(-1, -1);
}


void CSerialComDlg::CheckDataBufferFull(void) {
    if (m_iLogIndex < m_iHistoryCapacity  || m_bIsAutoSave) { return; }
    m_bIsAutoSave = TRUE;

    if (m_bUseAutoSave && m_bUseAutosave1_LogCountLimit) {
        // Save Data
        BOOL bUpdateList = m_bUpdateList;
        m_bUpdateList = FALSE;
        m_iProgramMode = DATA_ANALYZE_MODE;

        m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);

        int nItem = m_arrLogData.size();

        CFile SaveRcvData;
        CString str = "";

        for (int i = 0; i < nItem; i++) {
            if (SaveRcvData.m_hFile == INVALID_HANDLE_VALUE) {
                CString strFileName;
                SYSTEMTIME time;
                GetLocalTime(&time);
                CString strPort;
                strPort.Format("%s", (LPCTSTR)strPort1name);
                strPort.Replace("\\", "");
                strPort.Replace(".", "");

                strFileName.Format("AutoSave_%02d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
                if (strPort1name != "") {
                    CString strTemp = strFileName;
                    strFileName.Format("%s_R1%s", (LPCTSTR)strTemp, (LPCTSTR)strPort);
                }
                strPort.Format("%s", (LPCTSTR)strPort2name);
                strPort.Replace("\\", "");
                strPort.Replace(".", "");

                if (strPort2name != "") {
                    CString strTemp = strFileName;
                    strFileName.Format("%s_R2%s", (LPCTSTR)strTemp, (LPCTSTR)strPort);
                }
                strFileName += ".CSV";

                BOOL bMakeFile = FALSE;
                if (m_iSaveMode == 1) {
                    CString strTemp = strFileName;
                    strFileName.Format("%s\\%s", (LPCTSTR)m_strSavePath[1], (LPCTSTR)strTemp);
                    bMakeFile = SaveRcvData.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);
                }
                if (bMakeFile == FALSE) {
                    CString strTemp = strFileName;
                    strFileName.Format("%s\\%s", (LPCTSTR)m_strSavePath[0], (LPCTSTR)strTemp);
                    SaveRcvData.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);
                }

                // Header 저장
                str.Format("#Header:%c\r\n", m_cPacketHead);
                SaveRcvData.Write(str, str.GetLength());
            }
            CString strTemp = str;
            str.Format("%s%s\r\n", (LPCTSTR)strTemp, (LPCTSTR)m_arrLogData[i]);
            str.Replace("  ", " ");
            if (str.GetLength() > 10000) {
                SaveRcvData.Write(str, str.GetLength());
                str = "";
            }
            m_bUpdateList = bUpdateList;
            m_iProgramMode = bUpdateList;
        }

        if (SaveRcvData.m_hFile != INVALID_HANDLE_VALUE) {
            SaveRcvData.Write(str, str.GetLength());
            SaveRcvData.Close();
        }
    }

	InitHistoryData();
    m_bIsAutoSave = FALSE;

	m_iHistoryMove = 0;
	OnBnClickedClearwatchlist();
	OnBnClickedClearmessage();
}


void CSerialComDlg::OnBnClickedClearfilterlist() {
	OnBnClickedClearwatchlist();
}


void CSerialComDlg::OnBnClickedLoadwatchlist() {
	LoadCommonConfiguration();
}


int CSerialComDlg::String2Value(const CString& strValue) {
	char myString[20];
	UINT iValue = 0;
	BOOL bStart = FALSE;
	CString str;
	str.Format("%s", (LPCTSTR)strValue);
	int iSize = str.GetLength();
	if (iSize < 20) {
		strcpy_s(myString, (LPCTSTR)str);
		for (int i = 0; i < iSize; i++) {
			if (myString[i] >= '0' && myString[i] <= '9') {
				iValue *= 10;
				iValue += (myString[i] - '0');
			}
		}
	}
	return iValue;
}


int  CSerialComDlg::Hex2Value(const CString& strHex) {
	char myString[10];
	UINT iValue = 0;
	BOOL bStart = FALSE;
	CString str;
	str.Format("%s", (LPCTSTR)strHex);
	int iSize = str.GetLength();
	if (iSize < 10) {
		strcpy_s(myString, (LPCTSTR)str);
		for (int i = 0; i < iSize; i++) {
			if (myString[i] >= '0' && myString[i] <= '9') {
				iValue *= 16;
				iValue += (myString[i] - '0');
			}
			else if (myString[i] >= 'A' && myString[i] <= 'F') {
				iValue *= 16;
				iValue += (myString[i] - 'A' + 10);
			}
			else if (myString[i] >= 'a' && myString[i] <= 'f') {
				iValue *= 16;
				iValue += (myString[i] - 'a' + 10);
			}
			else
				return iValue;
		}
	}
	return iValue;
}

void CSerialComDlg::UpdateUseAutoSave() {
    m_bUseAutoSave = IsDlgButtonChecked(IDC_UseAutoSave);
}


int CSerialComDlg::CopyTextToClipboard(const char *ap_string) {
	// 저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기) 
	int string_length = strlen(ap_string) + 1;

	// 클립보드로 문자열을 복사하기 위하여 메모리를 할당한다. 
	// 클립보드에는 핸들을 넣는 형식이라서 HeapAlloc 함수 사용이 블가능하다. 
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_length);

	// 할당된 메모리에 문자열을 복사하기 위해서 사용 가능한 주소를 얻는다. 
	char *p_data = (char *)::GlobalLock(h_data);

	if (NULL != p_data)	{
		// 할당된 메모리 영역에 삽입할 문자열을 복사한다. 
		memcpy(p_data, ap_string, string_length);

		// 문자열을 복사하기 위해서 Lock 했던 메모리를 해제한다. 
		::GlobalUnlock(h_data);

		if (::OpenClipboard(m_hWnd)) {
			::EmptyClipboard(); // 클립보드를 연다.
			::SetClipboardData(CF_TEXT, h_data);  // 클립보드에 저장된 기존 문자열을 삭제한다. 

			// 클립보드로 문자열을 복사한다. 
			::CloseClipboard(); // 클립보드를 닫는다.
		}
	}
	return 0;
}


BOOL CSerialComDlg::PreTranslateMessage(MSG* pMsg) {
	static int ID = 0;
    static int iPreCursorDataV = -1;
	static int iPreCursorMathV = -1; 
	static int iPreCursorLogV = -1;
	static int iPreCursorLogH = -1;

	static int iPreAdjustCtrl = -1;
	static int iPreAdjustShift = -1;
	CString str;
	
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case 17:
			if (m_bAdjustLegendY[m_iAdjustLegendY]) {
				if (iPreAdjustCtrl != 1) {
					m_bReqGraphUpdate = TRUE;
				}
				iPreAdjustCtrl = 1;
			}
			break;

		case VK_RETURN:			
			if (m_bRelesedChangeMode && !m_bReqChangeMode) {
				m_bReqChangeMode = true;
				m_bRelesedChangeMode = false;
				OnBnClickedDefaultbutton();
				m_bRelesedChangeMode = true;
			}		
			return TRUE;
            break;


		case VK_SHIFT: 
			if (m_bAdjustLegendY[m_iAdjustLegendY]) {
				if (iPreAdjustShift != 1) {
					m_bReqGraphUpdate = TRUE;
				}
				iPreAdjustShift = 1;
			}
			break;

		case 'C':
			if (GetKeyState(VK_CONTROL) < 0) {
                if (pMsg->hwnd == m_ctrlLogList.GetSafeHwnd() && GetFocus()->GetSafeHwnd() == m_ctrlLogList.GetSafeHwnd()) {
					if (m_iProgramMode == 0) {
						CString strTemp = "";
						POSITION pos = m_ctrlLogList.GetFirstSelectedItemPosition();
						while (pos)
						{
							int nItem = m_ctrlLogList.GetNextSelectedItem(pos);
							CString strItemText = m_ctrlLogList.GetItemText(nItem, 0); // 첫 번째 컬럼 기준
							strTemp += strItemText + _T("\r\n");
						}

						if (!strTemp.IsEmpty())
						{
							CT2A ascii(strTemp, CP_UTF8);
							CopyTextToClipboard(ascii.m_psz);
						}
					}
				}
			}
			break;
		}
	}
	else if (pMsg->message == WM_KEYUP) {	
		switch (pMsg->wParam) {
		case 17: 
			if (m_bAdjustLegendY[m_iAdjustLegendY]) {
				if (iPreAdjustCtrl != 0) {
					m_bReqGraphUpdate = TRUE;
				}
				iPreAdjustCtrl = 0;
			}
			break;

		case VK_SHIFT:
			if (m_bAdjustLegendY[m_iAdjustLegendY]) {
				if (iPreAdjustShift != 0) {
					m_bReqGraphUpdate = TRUE;
				}
				iPreAdjustShift = 0;
			}
			break;

		case VK_RETURN:
			m_bReqChangeMode = false;
			break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE) {
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);
		SetCursorType(point);
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
			if (m_AdjustLogFilter_V.isPressed) {
				if (abs(iPreCursorLogV - point.y) > 5) {
					iPreCursorLogV = point.y;
					SetLogFilterSize_V(point);
				}
			}
			if (m_AdjustLogFilter_H.isPressed) {
				if (abs(iPreCursorLogH - point.x) > 5) {
					iPreCursorLogH = point.x;
					SetLogFilterSize_H(point);
				}
			}
			if (m_AdjustDataWnd_V.isPressed) {
				if (abs(iPreCursorDataV - point.y) > 5) {
					iPreCursorDataV = point.y;
					SetDataWndSize_V(point);
				}
			}
			if (m_AdjustMathWnd_V.isPressed) {
				if (abs(iPreCursorMathV - point.y) > 5) {
					iPreCursorMathV = point.y;
					SetMathWndSize_V(point);
				}
			}
		}

		// LButton 누른채 Drag 동작 
		if (m_bIsMoveMode) {
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
				if (GetKeyState(VK_CONTROL) < 0) {
					SetMoveMouse(point);
				}
				else {
					ChangeSelectPoint(point);
				}
			}
			else {
				m_bIsMoveMode = FALSE;
			}
			UpdateLogMessage();
		}
	}
	else if (pMsg->message == WM_LBUTTONUP) {
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		if (m_bIsMoveMode) {
			SetMoveMouse(point);
			m_bIsMoveMode = FALSE;
		}
		if (m_AdjustLogFilter_V.isPressed) {
			m_AdjustLogFilter_V.isPressed = FALSE;
			SetLogFilterSize_V(point);
		}
		if (m_AdjustLogFilter_H.isPressed) {
			m_AdjustLogFilter_H.isPressed = FALSE;
			SetLogFilterSize_H(point);
		}
		if (m_AdjustDataWnd_V.isPressed) {
			m_AdjustDataWnd_V.isPressed = FALSE;
			SetDataWndSize_V(point);
		}
        if (m_AdjustMathWnd_V.isPressed) {
            m_AdjustMathWnd_V.isPressed = FALSE;
            SetMathWndSize_V(point);
        }
		m_bTimelineControl[0] = m_bTimelineControl[1] = m_bTimelineControl[2] = FALSE;
	}
    else if (pMsg->message == WM_MOUSEWHEEL) {
        CPoint point;
        ::GetCursorPos(&point);
        ScreenToClient(&point);
        SetCursorType(point);

        int dir = 0;
        WPARAM param = (pMsg->wParam & 0xFFFF0000);
        if (param == 0x780000) {
            dir = -1;  // up
        }
        else if (param == 0xFF880000) {
            dir = +1;  // dn
        }

        if (dir != 0) {
            if (point.x > 2 && point.x < m_iRangeLogFilterWidth) {
				if (point.y > m_iPos_DataStart && point.y < m_iPos_DataEnd) {
					if (m_bShowDataControl) {
						if (m_iControlDataOffset == 0 && dir == -1) {
						}
						else if (m_iControlDataOffset + m_iShowDataControl == m_iMaxDataSize && dir == 1) {
						}
						else {							
							m_iControlDataOffset += dir;
							SetFocusOff();
							ShowDataMathControl();
						}
					}
#if 0
					else {
						m_iShowDataControl = 0;
					}			
#endif
                }
                else if (point.y > m_iPos_MathStart && point.y < m_iPos_MathEnd) {
                    if (m_bShowMathControl) {
                        if (m_iControlMathOffset == 0 && dir == -1) {
                        }
                        else if (m_iControlMathOffset + m_iShowMathControl == m_iMaxMathSize && dir == 1) {
                        }
                        else {
                            m_iControlMathOffset += dir;
                            CString str;
                            str.Format("%d", m_iControlMathOffset);
                            m_iControlMathOffset;
                            ShowDataMathControl();
                        }
                    }
                    else {
                        m_iShowMathControl = 0;
                    }
                }
            }
        }
    }
    else if (pMsg->message == WM_COMMAND) {
        // 마우스 클릭 좌표 (스크린 좌표)
        CPoint pt(pMsg->pt);

        // 다이얼로그 기준 좌표로 변환
        ScreenToClient(&pt);

        // 해당 좌표에 있는 컨트롤의 핸들 얻기
        CWnd* pWnd = ChildWindowFromPoint(pt);

        if (pWnd != nullptr && pWnd->GetDlgCtrlID() == IDC_DefaultButton) {
            OnBnClickedDefaultbutton();
            return TRUE;
        }
    }

	return CDialog::PreTranslateMessage(pMsg);
}


void CSerialComDlg::CheckDataOffset() {
    if (m_iControlDataOffset + m_iShowDataControl >= m_iMaxDataSize) {
        m_iControlDataOffset = m_iMaxDataSize - m_iShowDataControl;
    }
    if (m_iControlDataOffset < 0) {
        m_iControlDataOffset = 0;
    }

    if (m_iControlMathOffset + m_iShowMathControl >= m_iMaxMathSize) {
        m_iControlMathOffset = m_iMaxMathSize - m_iShowMathControl;
    }
    if (m_iControlMathOffset < 0) {
        m_iControlMathOffset = 0;
    }    
}

void CSerialComDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);

	m_strTemp.Format("%d, %d, %d", nChar, nRepCnt, nFlags);
	UDF;
}

void CSerialComDlg::SetFrameSize() {	
	int cx = m_iActuralWindowSize[0] = m_iWndSizeX;
	m_iActuralWindowSize[1] = m_iWndSizeY;
	static int pre_cx = cx;

	// m_iFrameSizeGraphPosition
	int iPositionGraph = 0;
	if (m_bDisplayPosition) {
		iPositionGraph = max(20, min(m_iFrameSizeGraphPosition, m_iWndSizeX - 700));
		m_iDrawGraphRangeX[1] = cx - m_iLegendX_PositionGraph - iPositionGraph;
	}
	else {
		m_iDrawGraphRangeX[1] = cx - m_iLegendX - iPositionGraph;
	}

	bool bChangeGraphSize = true;
	if (m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0] < CHANGE_FRAME_MIN_GRAPH_SIZE) {
		m_iDrawGraphRangeX[1] = m_iDrawGraphRangeX[0] + CHANGE_FRAME_MIN_GRAPH_SIZE;
	}
	else if (cx > pre_cx && cx - m_iDrawGraphRangeX[1] < CHANGE_FRAME_MIN_LEGEND_SIZE) {
		int maxAdjust = max(m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0] - CHANGE_FRAME_MIN_GRAPH_SIZE, 0);
		int maxExtend = CHANGE_FRAME_MIN_LEGEND_SIZE - (cx - m_iDrawGraphRangeX[1]);
		if (maxExtend <= maxAdjust) {
			m_iDrawGraphRangeX[1] -= maxExtend;
		}
		else {
			m_iDrawGraphRangeX[1] -= maxAdjust;
		}		
	}
	else {
		bool bChangeGraphSize = false;
	}

	if (bChangeGraphSize) {
		if (m_bDisplayPosition) {
			m_iLegendX_PositionGraph = max(cx - m_iDrawGraphRangeX[1] - iPositionGraph, 20);
		} 
		else {
			m_iLegendX = max(cx - m_iDrawGraphRangeX[1] - iPositionGraph, 20);
		}
	}

	m_iDrawGraphSpanX = max(1, m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0]);
	m_iDrawGraphMidX = (m_iDrawGraphRangeX[1] + m_iDrawGraphRangeX[0]) / 2;

	m_iGraphInfoMinX = (m_iDrawGraphRangeX[1] + GRAPH_GAP_X);
	m_iGraphInfoMaxX = (cx - 1 - iPositionGraph);
	m_iTimeDisplaySize = m_iDrawGraphSpanX / 80;
		
	// Graph영역 나누기
    // 가로 영역 결정
	if (m_bDisplayPosition) {
		int iDev = 3;
		m_iPositionGraphRangeX[0] = m_iGraphInfoMaxX + 3 + iDev;
		m_iPositionGraphRangeX[1] = cx - 2 - iDev;
		m_iPositionGraphRangeY[0] = m_iDrawGraphRangeY[0][0] + iDev;
		m_iPositionGraphRangeY[1] = m_iDrawGraphRangeY[m_iGraphCount-1][1] - iDev;
	}

	//m_iAdjustSelectPoint
	if (m_iDrawGraphRangeX[1] < m_iAdjustSelectPoint[0])m_iAdjustSelectPoint[0] = m_iDrawGraphRangeX[1];
	if (m_iDrawGraphRangeX[1] < m_iAdjustSelectPoint[1])m_iAdjustSelectPoint[1] = m_iDrawGraphRangeX[1];

    // 	세로 영역 결정
    SetGraphHeight();

	CalculationScale();
	pre_cx = cx;
}

void CSerialComDlg::SetGraphHeight() {
    // 총 변경 높이 구하자
    int adjust_height = m_iWndSizeY - GRAPH_BOTTOM_MARGIN - GRAPH_START_Y - GRAPH_GAP_Y * (m_iGraphCount - 1) - GRAPH_MIN_HEIGHT * m_iGraphCount;
    
    if (adjust_height > 0) {
        // 유효 그래프의 height 합을 구하자
        int sum_height = 0;  
        bool reset = false;
        for (int i = 0; i < m_iGraphCount; i++) {  
            int idx = m_RealGrpahInfo[i].iDraw2RealGraphID;
            sum_height += m_RealGrpahInfo[idx].iHeight;
            if (m_RealGrpahInfo[idx].iHeight < 0) reset = true;
        }
        if (sum_height == 0) {
            sum_height = 1000 * m_iGraphCount;
            for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
                int idx = m_RealGrpahInfo[i].iDraw2RealGraphID;
                m_RealGrpahInfo[m_RealGrpahInfo[idx].iDrawGraphID].iHeight = 1000;
            }
        }

        //  adjust_height 배분
        int remain = adjust_height;
        for (int i = 0; i < m_iGraphCount; i++){
            int idx = m_RealGrpahInfo[i].iDraw2RealGraphID;
            int dH = (m_RealGrpahInfo[idx].iHeight * adjust_height + sum_height / 2) / sum_height;
            if (dH > remain) {
                dH = remain;
            }
           
            if (i == 0) {
                m_iDrawGraphRangeY[i][0] = GRAPH_START_Y;
            }
            else {
                m_iDrawGraphRangeY[i][0] = m_iDrawGraphRangeY[i-1][1] + GRAPH_GAP_Y;
            }
            if (i == m_iGraphCount - 1) {   dH = remain;    }

            m_iDrawGraphRangeY[i][1] = m_iDrawGraphRangeY[i][0] + GRAPH_MIN_HEIGHT + dH;
            remain -= dH;
        }
    }
    else {
        // same height
        int height = (m_iWndSizeY - GRAPH_GAP_Y * (m_iGraphCount - 1) - GRAPH_BOTTOM_MARGIN - GRAPH_START_Y) / m_iGraphCount;
        for (int i = 0; i < m_iGraphCount; i++) {
            m_iDrawGraphRangeY[i][0] = GRAPH_START_Y + i * (height + GRAPH_GAP_Y);
            m_iDrawGraphRangeY[i][1] = GRAPH_START_Y + i * (height + GRAPH_GAP_Y) + height;
        }
    }
    
    {
        CString str;
        str.Format("Test %p", m_RealGrpahInfo[0]);
        str.Empty();
    }

}

void CSerialComDlg::OnSize(UINT nType, int cx, int cy) {
	CDialog::OnSize(nType, cx, cy);
	int iWndSizeX = m_iWndSizeX, iWndSizeY = m_iWndSizeY;
	m_iWndSizeX = cx, m_iWndSizeY = cy;
	SetFrameSize();
	UpdateControlPosition();

	int x0 = 2;
	int y0 = m_iPos_FilterEnd; 
	if (IsDlgButtonChecked(IDC_Display1) == TRUE) {
		y0 += DRAW_CONTROL_GAP_Y;
	}

	int dy1 = (cy - y0) * m_iRatioLogFilter / 100;  
	
	m_iRangeLogFilterVertical = y0;
    
    if (cy - y0 > 20) { 
        m_iRangeLogFilterRatio[0] = min(max(y0 + 1, y0 + dy1 - 2), cy - 1) - 2;
        m_iRangeLogFilterRatio[1] = min(max(y0 + 1, y0 + dy1 + 2), cy - 1);
        if (m_bShowFilterLogControl) {
            GetDlgItem(IDC_FilterMessage)->MoveWindow(x0, m_iRangeLogFilterVertical, m_iRangeLogFilterWidth, max(1, dy1 - 2), TRUE);
            GetDlgItem(IDC_LogList)->MoveWindow(x0, y0 + max(2, dy1 + 2), m_iRangeLogFilterWidth, max(1, cy - y0 - dy1 - 1), TRUE);
        }
        else {
            m_iRangeLogFilterRatio[0] = -100;
            m_iRangeLogFilterRatio[1] = -100;
            GetDlgItem(IDC_LogList)->MoveWindow(x0, m_iRangeLogFilterVertical, m_iRangeLogFilterWidth, m_iWndSizeY - m_iRangeLogFilterVertical + 1, TRUE);
        }
        m_ctrlProgReading.MoveWindow(x0, y0, m_iRangeLogFilterWidth, max(1, 2 * dy1 - 1), FALSE);        
    }  

    ShowGraphControlbyHeight();		
    CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
    InvalidateRect(&rect);
	DrawInit();
	UDF;
	m_bReqGraphUpdate = TRUE;
	GetDlgItem(IDC_HIDE_SIDEMENU)->MoveWindow(m_iDrawGraphRangeX[0] - 72, m_iDrawGraphRangeY[0][0] - 23, 25, 19, TRUE);

	if (iWndSizeX != cx || iWndSizeY != cy) {
		ControlView();
	}
}

void CSerialComDlg::OnSizing(UINT fwSide, LPRECT pRect) {
	CDialog::OnSizing(fwSide, pRect);
}

void CSerialComDlg::OnPortClose1() {
	if (m_ComuPort.m_bConnected == TRUE) {
		m_ComuPort.ClosePort();
		strPort1name.Empty();

		m_bUpdateList = FALSE;
		m_iProgramMode = DATA_ANALYZE_MODE;
		ProgramModeUpdate();
	}
}

void CSerialComDlg::OnPortClose2() {
	if (m_ComuPort2.m_bConnected == TRUE) {
		m_ComuPort2.ClosePort();
		strPort2name.Empty();

		m_bUpdateList = FALSE;
		m_iProgramMode = DATA_ANALYZE_MODE;
		ProgramModeUpdate();
	}
}

void CSerialComDlg::OnPortClose3() {
	if (m_ComuPort3.m_bConnected == TRUE) {
		m_ComuPort3.ClosePort();
		strPort3name.Empty();

		m_bUpdateList = FALSE;
		m_iProgramMode = DATA_ANALYZE_MODE;
		ProgramModeUpdate();
	}
}

void CSerialComDlg::DisplayControl() {
	if (m_bShowMathControl == FALSE) {
		m_strDataSetting2 = "Math Data : Graph only ";
	}
	else {
		m_strDataSetting2.Format("%s", (LPCTSTR)m_strDataSetting);
	}
}

void CSerialComDlg::OnCbnSelchangeGraphcount() {
	UDT;

	m_iSelectGraphCount = MAX_GRAPH_COUNT - 1;

	DisplayControl();
	OptimizeGraphCount();
}

void CSerialComDlg::OnEnChangeDisplaydatasize() {
	CString strPre;
	CString strLast;

	if (m_bFitToScreen) {
		if (GetDlgItem(IDC_DisplayDataSize) == GetFocus() || m_bManualAdjust == TRUE) {
			m_bFitToScreen = FALSE;
			CheckDlgButton(IDC_FitToMaxData, FALSE);
			m_bManualAdjust = FALSE;
		}
	}
	else {
		m_bManualAdjust = FALSE;
	}

	int iDataInput;
	GetDisplayDataSize(&iDataInput);
	int iData = max(MIN_DISPLAY_SIZE, min(iDataInput, m_iHistoryCapacity));

	if (m_bForcedChangeDataSize == FALSE && iData != m_iDisplaySize) {
		SetTimer(TIMER_REQ_CHANGE_DATASIZE, 1000, NULL);
		return;
	}
	else {
		KillTimer(TIMER_REQ_CHANGE_DATASIZE);
	}

	m_bForcedChangeDataSize = FALSE;

	int preDisplaySize = m_iDisplaySize;
	m_iDisplaySize = iData;
	   
	GetDlgItemText(IDC_DisplayDataSize, strPre);
	strLast = AddComma(m_iDisplaySize);
	
	if (strPre != strLast) {
		SetDlgItemText(IDC_DisplayDataSize, strLast);
	}

	if (preDisplaySize != m_iDisplaySize) {
		m_bForcedChangeDataSize = TRUE;
		m_bReqGraphUpdate = TRUE;

		if (m_iProgramMode == DATA_ACQUISITION_MODE) {
			m_iHistoryMove = 0;
			return;
		}

		int iSelectIndex = m_iHistoryIndex;;
		if (m_bDrawSelectData) {
			iSelectIndex = m_iHistorySelectIndex;
		}

		int end = iSelectIndex + m_iDisplaySize / 2;

		if (end > m_iHistoryIndex) {
			end = m_iHistoryIndex;
		}

		int start = end - m_iDisplaySize;
		m_iHistoryMove = m_iHistoryIndex - end;

		if (m_bDrawSelectData) {
			double dx = (m_iDrawGraphRangeX[1] - m_iDrawGraphRangeX[0]) / m_iDisplaySize;
			m_iHistorySelectPt = (int)((((double)m_iHistorySelectIndex - start) - 0.5) * dx) + m_iDrawGraphRangeX[0];
		}
		m_bTimelineFullRangeView = FALSE;
	}
}

void CSerialComDlg::ShowVersionHistory(void) {	
	CHistory history;	
	history.DoModal();
}

void CSerialComDlg::OnBnClickedShowhistory() {
	ShowVersionHistory();
}


void CSerialComDlg::GetLastModel() {
	CString strAddress, strItem, strValue;
	// Graph Count
	strAddress.Format("Graph");
	strItem.Format("Number_Graph");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);

	m_iSelectGraphCount = MAX_GRAPH_COUNT - 1;
	m_iGraphCount = MAX_GRAPH_COUNT;

	//Update Combobox
	strAddress.Format("Model");
	strItem.Format("nModel");
	int nModel = atoi(GetRegRoot_RegistryData(strAddress, strItem));
	m_cSelectModel.DeleteString(0);
	m_cSelectModel.AddString("default");
	for (int i = 1; i < nModel; i++) {
		strItem.Format("ModelName_%d", i);
		m_cSelectModel.AddString(GetRegRoot_RegistryData(strAddress, strItem));
	}
	
	strItem.Format("LastModel");
	strValue = GetRegRoot_RegistryData(strAddress, strItem);

	if (strValue != "") {
		int nSaveCount = m_cSelectModel.GetCount();
		for (int i = 1; i < nSaveCount; i++) {
			CString str;
			m_cSelectModel.GetLBText(i, str);
			if (strValue == str) {
				m_iSelectModel = i;
				m_cSelectModel.SetCurSel(i);
				return;
			}
		}
	}		
	UDF;
}

void CSerialComDlg::SetLastModel() {
	CString strAddress, strItem, strValue;
	strAddress.Format("Model");
	strItem.Format("LastModel");
	m_cSelectModel.GetLBText(m_iSelectModel, strValue);
	SetRegRoot_RegistryData(strAddress, strItem, strValue);
}

void CSerialComDlg::OnCbnSelchangeModel() {
	UDT;
	SetLastModel();
	ChangeHeader();	
}

void CSerialComDlg::OnBnClickedLoadcalculation() {

	if (m_iProgramMode != DATA_ACQUISITION_MODE) {
		if (m_iHistoryIndex > 0) {
			if (1) {
			//if (m_pStatistics == NULL || m_pStatistics->m_hWnd == NULL) {
				delete m_pStatistics;
				m_pStatistics = new CStatisticsDlg;
				m_pStatistics->pMain = this;
				m_pStatistics->m_bClearVariable = TRUE;
				m_iAdjustSelectPoint[0] = m_iDrawGraphRangeX[0];
				m_iAdjustSelectPoint[1] = m_iDrawGraphRangeX[1];

				m_pStatistics->Create(IDD_StatisticsDlg);
				m_pStatistics->ShowWindow(SW_SHOW);
				m_bUseStatistics = TRUE;
				SetTimer(TIMER_CHECK_STATISTICS_DLG, 500, NULL);
				m_bIsAdjustMode = FALSE;
				m_bReqGraphUpdate = TRUE;

			}
		}	
		else {
			AfxMessageBox("저장된 Data가 없습니다~!\r\n(No data saved)");
		}
	}
}

void CSerialComDlg::OnCancel() {
	if (AfxMessageBox("프로그램을 종료할까요 ? \r\n(Do you want to exit the program?)", MB_YESNO) == IDYES) {
		if (m_pUserScreen) {
			m_pUserScreen->DestroyWindow();
			delete m_pUserScreen;
			m_pUserScreen = nullptr;
		}

		if (m_pTextScreen) {
			m_pTextScreen->DestroyWindow();
			delete m_pTextScreen;
			m_pTextScreen = nullptr;
		}

		OnOK(); 
	}
}

void CSerialComDlg::OnBnClickedDefinemathprobe()
{
	static int iError = 0;
	if (m_iProgramMode != DATA_ANALYZE_MODE) {
		if (++iError < 5) {
			m_strTemp.Format("Math Probe정의는 분석모드에서만 가능합니다.\r\n(Math Probe definition is only available in Analysis mode.)");
			UDF;
		}
		else {
			AfxMessageBox("Math Probe정의는 분석모드에서만 가능합니다.\r\n(Math Probe definition is only available in Analysis mode.)");
			iError = 0;
		}
	}
	else {        
        auto MathProbe = std::make_unique<CMathProbeDlg>();
		MathProbe->pMain = this;
		MathProbe->pAppMain = pAppMain;
		MathProbe->DoModal();
		CheckMathProbeSetting();		
        LoadHeaderConfiguration();
		OptimizeGraphCount();
        CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
        InvalidateRect(&rect);
		DrawInit();
		m_bReqGraphUpdate = TRUE;
	}
	ShowDataMathControl();
}

void CSerialComDlg::LoadDefaultMathProbeSetting() {
	for (int i = 0; i < MAXIMUM_MATH_SIZE; i++) {
        m_MathData[i].bValid = FALSE;
        m_MathData[i].iDataID1 = -1;
        m_MathData[i].iDataID2 = -1;
        m_MathData[i].iOperator = -1;
		for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
            m_MathData[i].bDisplayGraph[j] = FALSE;
		}
        m_MathData[i].strName.Format("[ Not defined ]");
        m_MathData[i].iValue = 0;
        m_MathData[i].fValue = 0.;
        m_MathData[i].nData = 100;
        m_MathData[i].iColor[0] = m_iDefaultMathColor[i][0];
        m_MathData[i].iColor[1] = m_iDefaultMathColor[i][1];
        m_MathData[i].iColor[2] = m_iDefaultMathColor[i][2];
	}	
	
	InitMathData();
}


void CSerialComDlg::CheckMathProbeSetting() {
	CString str;
	CString str1;
	CString str2;
	// Math Probe		
	LoadDefaultMathProbeSetting();

	CString strAddress, strItem, strValue;
	for (int i = 0; i < m_iMaxMathSize; i++) {
		if (m_strPacketHeader == " " || m_strPacketHeader == "")
			strAddress.Format("MathDefine\\MathProbe%d\\default", i);
		else
			strAddress.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);

		// Data 1
		strItem.Format("Data1");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].iDataID1 = atoi(strValue);

		strItem.Format("Data2");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].iDataID2 = atoi(strValue);

		strItem.Format("Operator");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].iOperator = atoi(strValue);

		strItem.Format("UseMathPinName");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].bUserName = atoi(strValue);
		else
            m_MathData[i].bUserName = FALSE;

		strItem.Format("PinName");
		strValue = GetReg_RegistryData(strAddress, strItem);
		strValue.Replace("]", "");
		strValue.Replace("[", "");
		if (strValue != "") {
            m_MathData[i].strName.Format("[%s]", (LPCTSTR)strValue);
			m_strMathProbeName[i].Format("[%s]", (LPCTSTR)strValue);
		}
		strItem.Format("ConstValue");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].fValue = atof(strValue);

        strItem.Format("Result_Integer");
        strValue = GetReg_RegistryData(strAddress, strItem);
        m_MathData[i].bResInt = atoi(strValue);

		strItem.Format("DataSize");
		strValue = GetReg_RegistryData(strAddress, strItem);
		if (strValue != "")  m_MathData[i].nData = atoi(strValue);

		for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
			strItem.Format("DrawGraph%d", j);
			strValue = GetReg_RegistryData(strAddress, strItem);
			if (strValue == "1")
                m_MathData[i].bDisplayGraph[j] = TRUE;				//
			else if (strValue == "0")
                m_MathData[i].bDisplayGraph[j] = FALSE;			//
		}

		strAddress.Format("MathDefine\\MathProbe%d", i);

		strItem.Format("Red");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iColor[0] = atoi(strValue);
		else
            m_MathData[i].iColor[0] = m_iDefaultMathColor[i][0];


		strItem.Format("Green");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iColor[1] = atoi(strValue);
		else
            m_MathData[i].iColor[1] = m_iDefaultMathColor[i][1];

		strItem.Format("Blue");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iColor[2] = atoi(strValue);
		else
            m_MathData[i].iColor[2] = m_iDefaultMathColor[i][2];

		strItem.Format("PenThickness");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iPenThickness = atoi(strValue);
		else
            m_MathData[i].iPenThickness = 2;

		strItem.Format("PenStyle");
		strValue = GetRegRoot_RegistryData(strAddress, strItem);
		if (strValue != "")
            m_MathData[i].iPenStyle = atoi(strValue);
		else
            m_MathData[i].iPenStyle = PS_SOLID;
	}

	CheckMathProbeValid();
	UDF;		
	ReCalculationMathData();	
}

void CSerialComDlg::CheckMathProbeValid(void) {
	int check = 1;
	for (int iChannel = 0; iChannel < MAXIMUM_MATH_SIZE; iChannel++) {
		BOOL bValid = TRUE;
		BOOL bReqUDF = FALSE;

        m_MathData[iChannel].bValid = FALSE;
		CString str1 = "", str2 = "", str = "";

		// ID1 Check
		if (m_MathData[iChannel].iDataID1 < 0 || m_MathData[iChannel].iDataID1 > m_iMaxDataSize + m_iMaxMathSize) bValid = FALSE;

		if (m_MathData[iChannel].iDataID1 != m_iMaxDataSize + iChannel) {
			// Operation Check
			if (m_MathData[iChannel].iOperator < 0 || m_MathData[iChannel].iOperator >= MAX_OPERATOR_COUNT)	bValid = FALSE;

			if (bValid) {
				if (m_MathData[iChannel].iOperator < MAX_SIMPLE_OPERATOR) {
					// ID2 Check
					if (m_MathData[iChannel].iDataID2 < 0) {
						bValid = FALSE;
					}
					else {
						if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize)
							str1.Format("D%d", m_MathData[iChannel].iDataID1 + 1);
						else
							str1.Format("M%d", m_MathData[iChannel].iDataID1 + 1 - m_iMaxDataSize);

						if (m_MathData[iChannel].iDataID2 < m_iMaxDataSize)
							str2.Format("D%d", m_MathData[iChannel].iDataID2 + 1);
						else {
							if (m_MathData[iChannel].iDataID2 == m_iMaxDataSize + iChannel) {
								str2.Format("%s", (LPCTSTR)Double2Str(m_MathData[iChannel].fValue));
							}
							else {
								str2.Format("M%d", m_MathData[iChannel].iDataID2 + 1 - m_iMaxDataSize);
							}
						}

						if (!m_MathData[iChannel].bUserName || m_strMathProbeName[iChannel] == "") {
							switch (m_MathData[iChannel].iOperator) {
							case Operator_Add: str.Format("[%s + %s]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_Substract: str.Format("[%s - %s]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_Multiply: str.Format("[%s * %s]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_Divide: str.Format("[%s / %s]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_Exponential: str.Format("[%s ^ %s]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_EqualTo: str.Format("[if(%s==%s)]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_GreaterThan: str.Format("[if(%s > %s)]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_LessThan: str.Format("[if(%s < %s)]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							case Operator_Remainder: str.Format("[%s %% %s]", (LPCTSTR)str1, (LPCTSTR)str2); break;
							}
						}
						else {
							str.Format("[%s]", (LPCTSTR)m_MathData[iChannel].strName);
						}
					}
				}
				else {
					// nSize
					int nSize = max(1, min(MAX_MATH_DATA, m_MathData[iChannel].nData));
                    m_MathData[iChannel].nData = nSize;

					if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize)
						str1.Format("D%d", m_MathData[iChannel].iDataID1 + 1);
					else
						str1.Format("M%d", m_MathData[iChannel].iDataID1 + 1 - m_iMaxDataSize);

					switch (m_MathData[iChannel].iOperator) {
					case Operator_Average: str.Format("[Avr %s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_RMS: str.Format("[RMS %s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_Stdev: str.Format("[Std %s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_Change: str.Format("[Δ%s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_ABS: str.Format("[abs(%s)]", (LPCTSTR)str1); break;
					}
				}
			}
		}
		else {
			str2.Format("%s", (LPCTSTR)Double2Str(m_MathData[iChannel].fValue));
			if (!m_MathData[iChannel].bUserName || m_strMathProbeName[iChannel] == "") {
				str.Format("[Const. %s]", (LPCTSTR)str2);
			}
			else {
				str.Format("[%s]", (LPCTSTR)m_MathData[iChannel].strName);
			}
		}

		str.Replace("]", "");
		str.Replace("[", "");
		m_strMathProbeName[iChannel].Replace("]", "");
		m_strMathProbeName[iChannel].Replace("[", "");

		if (bValid) {
			m_MathData[iChannel].bValid = TRUE;
			if (m_MathData[iChannel].bUserName)
				m_strMathProbeName[iChannel].Format("[%s]", (LPCTSTR)m_MathData[iChannel].strName);
			else
				m_strMathProbeName[iChannel].Format("[%s]", (LPCTSTR)str);
		}
		else {
            m_MathData[iChannel].bUserName = FALSE;
			m_strMathProbeName[iChannel] = "";
            m_MathData[iChannel].bValid = FALSE;
			str.Format("[ Not defined ]");
			m_strMathProbeName[iChannel].Format("[%s]", (LPCTSTR)str);
		}

		m_MathData[iChannel].iValue = 0;
		m_LogBrush.lbColor = RGB(m_MathData[iChannel].iColor[0], m_MathData[iChannel].iColor[1], m_MathData[iChannel].iColor[2]);
		m_Pen_Math[iChannel].CreatePen(PS_GEOMETRIC | m_MathData[iChannel].iPenStyle, m_MathData[iChannel].iPenThickness, &m_LogBrush, 0, 0);
	}
}


void CSerialComDlg::InitMathData(void) {
	for (int iData = 0; iData < m_iMaxMathSize; iData++) {
		for (int j = 0; j < MAX_MATH_DATA; j++) {
			m_MathData[iData].fSaveData[j] = 0.;
		}		
		m_MathData[iData].fSum = 0.;
		m_MathData[iData].index = 0;
	}
}

void CSerialComDlg::ReCalculationMathData() {
    enum {
        DTYPE_DATA,
        DTYPE_MATH,
        DTYPE_CONST,
    };

	InitMathData();
	int iData1, iData2 , nSize;
	int index;	
    auto fSaveData = std::make_unique <double[]> (MAX_MATH_DATA);
	int Datatype1 = DTYPE_DATA;  // 0:Data  1:Math  2:const
	int Datatype2 = DTYPE_DATA;  // 0:Data  1:Math  2:const

	double fAverage, fSS, fSum, fValue;
	double fConstValue = 0; 
	double fComparedData1, fComparedData2;
	int iComparedData1, iComparedData2;

	for (int iChannel = 0; iChannel<m_iMaxMathSize; iChannel++) {
		if (m_MathData[iChannel].bValid) {
			if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize) {
				Datatype1 = DTYPE_DATA;
				iData1 = m_MathData[iChannel].iDataID1;
			}
			else if (m_MathData[iChannel].iDataID1 == m_iMaxDataSize + iChannel) {
				Datatype1 = DTYPE_CONST;
				fConstValue = m_MathData[iChannel].fValue;
			}
			else {
				Datatype1 = DTYPE_MATH;
				iData1 = m_MathData[iChannel].iDataID1 - m_iMaxDataSize;
				if (iData1 >= m_iMaxMathSize) {
					continue;
				}
			}

			if (m_MathData[iChannel].iDataID2 < m_iMaxDataSize) {
				Datatype2 = DTYPE_DATA;
				iData2 = m_MathData[iChannel].iDataID2;
			}
			else if (m_MathData[iChannel].iDataID2 == m_iMaxDataSize + iChannel) {
				Datatype2 = DTYPE_CONST;
				fConstValue = m_MathData[iChannel].fValue;
			}
			else {
				Datatype2 = DTYPE_MATH;
				iData2 = m_MathData[iChannel].iDataID2 - m_iMaxDataSize;
				if (iData2 >= m_iMaxMathSize) {
					continue;
				}
			}			
			nSize = max(0, min(m_MathData[iChannel].nData, MAX_MATH_DATA - 1));

			index = 0;
			fSum = 0;
			
			for (int i = 0; i < nSize; i++) {
				fSaveData[i] = 0;
			}

			if (Datatype1 == DTYPE_CONST){
				for (int i = 0; i < m_iHistoryIndex; i++) {
					m_HistoryData[i].fMathValue[iChannel] = fConstValue;
				}
			}
			else {
				switch (m_MathData[iChannel].iOperator) {

				case Operator_Add:  // Add
					if (Datatype1 == DTYPE_DATA) {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] + m_HistoryData[i].fDataValue[iData2];
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] + m_HistoryData[i].fMathValue[iData2];
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] + fConstValue;
							}
						}
					}
					else {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] + m_HistoryData[i].fDataValue[iData2];
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] + m_HistoryData[i].fMathValue[iData2];
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] + fConstValue;
							}
						}
					}
					break;

				case Operator_Substract: //Substract
					if (Datatype1 == DTYPE_DATA) {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] - m_HistoryData[i].fDataValue[iData2];
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] - m_HistoryData[i].fMathValue[iData2];
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] - fConstValue;
							}
						}
					}
					else {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] - m_HistoryData[i].fDataValue[iData2];
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] - m_HistoryData[i].fMathValue[iData2];
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] - fConstValue;
							}
						}
					}
					break;

				case Operator_Multiply: //Multiply
					if (Datatype1 == DTYPE_DATA) {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] * m_HistoryData[i].fDataValue[iData2];
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] * m_HistoryData[i].fMathValue[iData2];
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] * fConstValue;
							}
						}
					}
					else {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] * m_HistoryData[i].fDataValue[iData2];
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] * m_HistoryData[i].fMathValue[iData2];
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] * fConstValue;
							}
						}
					}
					break;

				case Operator_Divide: 
					if (Datatype1 == DTYPE_DATA) {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								if (m_HistoryData[i].fDataValue[iData2] != 0) {
									m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] / m_HistoryData[i].fDataValue[iData2];
								}
								else {
									m_HistoryData[i].fMathValue[iChannel] = 0;
								}
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								if (m_HistoryData[i].fMathValue[iData2] != 0) {
									m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] / m_HistoryData[i].fMathValue[iData2];
								}
								else {
									m_HistoryData[i].fMathValue[iChannel] = 0;
								}
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								if (fConstValue != 0) {
									m_HistoryData[i].fMathValue[iChannel] = (int)(m_HistoryData[i].fDataValue[iData1] / fConstValue);
								}
								else {
									m_HistoryData[i].fMathValue[iChannel] = 0;
								}
							}
						}
					}
					else {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								if (m_HistoryData[i].fDataValue[iData2] != 0) {
									m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] / m_HistoryData[i].fDataValue[iData2];
								}
								else {
									m_HistoryData[i].fMathValue[iChannel] = 0;
								}
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								if (m_HistoryData[i].fMathValue[iData2] != 0) {
									m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] / m_HistoryData[i].fMathValue[iData2];
								}
								else {
									m_HistoryData[i].fMathValue[iChannel] = 0;
								}
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								if (fConstValue != 0) {
									m_HistoryData[i].fMathValue[iChannel] = (int)(m_HistoryData[i].fMathValue[iData1] / fConstValue);
								}
								else {
									m_HistoryData[i].fMathValue[iChannel] = 0;
								}
							}
						}
					}
					break;

				case Operator_Exponential: //Exponential
					if (Datatype1 == DTYPE_DATA) {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = pow(m_HistoryData[i].fDataValue[iData1], m_HistoryData[i].fDataValue[iData2]);
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = pow(m_HistoryData[i].fDataValue[iData1], m_HistoryData[i].fMathValue[iData2]);
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = pow(m_HistoryData[i].fDataValue[iData1], fConstValue);
							}
						}
					}
					else {
						if (Datatype2 == DTYPE_DATA) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = pow(m_HistoryData[i].fMathValue[iData1], m_HistoryData[i].fDataValue[iData2]);
							}
						}
						else if (Datatype2 == DTYPE_MATH) {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = pow(m_HistoryData[i].fMathValue[iData1], m_HistoryData[i].fMathValue[iData2]);
							}
						}
						else {
							for (int i = 0; i < m_iHistoryIndex; i++) {
								m_HistoryData[i].fMathValue[iChannel] = pow(m_HistoryData[i].fMathValue[iData1], fConstValue);
							}
						}
					}
					break;

				case Operator_EqualTo: // Equal To
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (Datatype1 == DTYPE_DATA) {
							fComparedData1 = m_HistoryData[i].fDataValue[iData1];
						}
						else {
							fComparedData1 = m_HistoryData[i].fMathValue[iData1];
						}
						
						if (Datatype2 == DTYPE_DATA) {
							fComparedData2 = m_HistoryData[i].fDataValue[iData2];
						}
						else if (Datatype2 == DTYPE_MATH) {
							fComparedData2 = m_HistoryData[i].fMathValue[iData2];
						}
						else {
							fComparedData2 = fConstValue;
						}							

						if (fComparedData1 == fComparedData2)
							m_HistoryData[i].fMathValue[iChannel] = 1;
						else
							m_HistoryData[i].fMathValue[iChannel] = 0;
					}
					break;

				case Operator_GreaterThan: // Greater Than
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (Datatype1 == DTYPE_DATA) {
							fComparedData1 = m_HistoryData[i].fDataValue[iData1];
						}
						else {
							fComparedData1 = m_HistoryData[i].fMathValue[iData1];
						}

						if (Datatype2 == DTYPE_DATA) {
							fComparedData2 = m_HistoryData[i].fDataValue[iData2];
						}
						else if (Datatype2 == DTYPE_MATH) {
							fComparedData2 = m_HistoryData[i].fMathValue[iData2];
						}
						else {
							fComparedData2 = fConstValue;
						}

						if (fComparedData1 > fComparedData2)
							m_HistoryData[i].fMathValue[iChannel] = 1;
						else
							m_HistoryData[i].fMathValue[iChannel] = 0;
					}
					break;

				case Operator_LessThan: // Less Than
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (Datatype1 == DTYPE_DATA) {
							fComparedData1 = m_HistoryData[i].fDataValue[iData1];
						}
						else {
							fComparedData1 = m_HistoryData[i].fMathValue[iData1];
						}

						if (Datatype2 == DTYPE_DATA) {
							fComparedData2 = m_HistoryData[i].fDataValue[iData2];
						}
						else if (Datatype2 == DTYPE_MATH) {
							fComparedData2 = m_HistoryData[i].fMathValue[iData2];
						}
						else {
							fComparedData2 = fConstValue;
						}

						if (fComparedData1 < fComparedData2)
							m_HistoryData[i].fMathValue[iChannel] = 1;
						else
							m_HistoryData[i].fMathValue[iChannel] = 0;
					}
					break;

				case Operator_Remainder: // Remainder					
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (Datatype1 == DTYPE_DATA) {
							iComparedData1 = (int)m_HistoryData[i].fDataValue[iData1];
						}
						else {
							iComparedData1 = (int)m_HistoryData[i].fMathValue[iData1];
						}

						if (Datatype2 == DTYPE_DATA) {
							iComparedData2 = (int)m_HistoryData[i].fDataValue[iData2];
						}
						else if (Datatype2 == DTYPE_MATH) {
							iComparedData2 = (int)m_HistoryData[i].fMathValue[iData2];
						}
						else {
							iComparedData2 = (int)fConstValue;
						}

						if (iComparedData2 <= 0) {
							iComparedData2 = max(1, -iComparedData2);
						}

						if (iComparedData1 < 0) {
							int iMultiply = -iComparedData1 / iComparedData2;
							iComparedData1 += iMultiply * iComparedData2;

							if (iComparedData1 < 0) iComparedData1 += iComparedData2;
							if (iComparedData1 < 0) iComparedData1 += iComparedData2;
						}

						m_HistoryData[i].fMathValue[iChannel] = iComparedData1 % iComparedData2;
					}					
					break;


				case Operator_Average:  // Average		
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (++index >= nSize) index = 0;
						fSum -= fSaveData[index];
						if (Datatype1 == DTYPE_MATH) {
							fValue = m_HistoryData[i].fMathValue[iData1];
						}
						else if (Datatype1 == DTYPE_DATA) {
							fValue = m_HistoryData[i].fDataValue[iData1];
						}

						fSum += fValue;
						fSaveData[index] = fValue;
						m_HistoryData[i].fMathValue[iChannel] = fSum / nSize;
					}
					m_MathData[iChannel].index = index;
					m_MathData[iChannel].fSum = fSum;
					for (int i = 0; i < nSize; i++) m_MathData[iChannel].fSaveData[i] = fSaveData[i];
					break;

				case Operator_RMS:  // RMS				
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (++index >= nSize) index = 0;
						fSum -= fSaveData[index];
						if (Datatype1 == DTYPE_MATH) fValue = m_HistoryData[i].fMathValue[iData1]; else  fValue = (double)m_HistoryData[i].fDataValue[iData1];
						fValue = fValue * fValue;
						fSum += fValue;
						fSaveData[index] = fValue;

						m_HistoryData[i].fMathValue[iChannel] = sqrt(fSum / nSize);
					}
					m_MathData[iChannel].index = index;
					m_MathData[iChannel].fSum = fSum;
                    for (int i = 0; i < nSize; i++) { m_MathData[iChannel].fSaveData[i] = fSaveData[i]; }
					break;

				case Operator_Stdev: //Stdev				
					for (int i = 0; i < m_iHistoryIndex; i++) {
						if (++index >= nSize) index = 0;
						fSum -= fSaveData[index];
						if (Datatype1 == DTYPE_MATH) 
                            fValue = m_HistoryData[i].fMathValue[iData1]; 
                        else  
                            fValue = m_HistoryData[i].fDataValue[iData1];
						fSum += fValue;
						fSaveData[index] = fValue;
						fAverage = fSum / nSize;
						fSS = 0.;
						for (int j = 0; j < nSize; j++) {
							fSS += (fAverage - fSaveData[j])*(fAverage - fSaveData[j]);
						}
						m_HistoryData[i].fMathValue[iChannel] = sqrt(fSS / nSize);
					}
					m_MathData[iChannel].index = index;
					m_MathData[iChannel].fSum = fSum;
					for (int i = 0; i < nSize; i++) m_MathData[iChannel].fSaveData[i] = fSaveData[i];
					break;

				case Operator_Change: //Delta
					for (int i = 0; i < nSize; i++)
						m_HistoryData[i].fMathValue[iChannel] = 0;
					for (int i = nSize; i < m_iHistoryIndex; i++) {
						if (Datatype1 == DTYPE_MATH) {
							m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fMathValue[iData1] - m_HistoryData[i - nSize].fMathValue[iData1];
						}
						else {
							m_HistoryData[i].fMathValue[iChannel] = m_HistoryData[i].fDataValue[iData1] - m_HistoryData[i - nSize].fDataValue[iData1];
						}
					}
					break;

				case Operator_ABS: // abs
                    for (int i = 0; i < m_iHistoryIndex; i++) {
						if (Datatype1 == DTYPE_MATH)
							fComparedData1 = m_HistoryData[i].fMathValue[iData1];
						else
							fComparedData1 = m_HistoryData[i].fDataValue[iData1];

						if (fComparedData1 > 0)
							m_HistoryData[i].fMathValue[iChannel] = fComparedData1;
						else
							m_HistoryData[i].fMathValue[iChannel] = -fComparedData1;
					}
					break;
				}
                if (m_MathData[iChannel].bResInt) {
                    for (int i = 0; i < m_iHistoryIndex; i++) {
                        m_HistoryData[i].fMathValue[iChannel] = (int)m_HistoryData[i].fMathValue[iChannel];
                    }
                }
			}            
		}
	}
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::DrawInit(int iGraph) {
	m_MemDC.SelectObject(&m_PenBlack);

	// Range
	m_MemDC.SelectObject(&m_BrushBackGround[m_iProgramMode]);
	m_MemDC.Rectangle(m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[iGraph][0], m_iDrawGraphRangeX[1], m_iDrawGraphRangeY[iGraph][1]);

	// Grid
	m_MemDC.SelectObject(&m_PenGrid);
	for (int i = 1; i <= m_iGridCountY * 2 + 1; i++) {
		int iValue = m_iDrawGraphRangeY[iGraph][0] + (m_iDrawGraphRangeY[iGraph][1] - m_iDrawGraphRangeY[iGraph][0]) * i / (m_iGridCountY * 2 + 2);

		m_MemDC.MoveTo(m_iDrawGraphRangeX[0] + 3, iValue);
		m_MemDC.LineTo(m_iDrawGraphRangeX[1] - 3, iValue);
	}
}

void CSerialComDlg::DrawInit(void) {
	CClientDC dc(this);

	m_MemDC.SelectObject(&m_PenWhite);
	m_MemDC.SelectObject(&m_BrushBackGround[1]);	// BackGround Brush : light Green
	m_MemDC.Rectangle(m_iDrawGraphRangeX[0], GRAPH_START_Y - 7, m_iActuralWindowSize[0] + 3, m_iActuralWindowSize[1] + 3);

	m_MemDC.SelectObject(&m_PenBlack);
	m_MemDC.SelectObject(&m_BrushBackGround[0]);
	for (int i = 0; i < m_iGraphCount; i++) {
		m_MemDC.Rectangle(m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[i][0], m_iGraphInfoMaxX, m_iDrawGraphRangeY[i][1]);
	}
	m_MemDC.Rectangle(m_iGraphInfoMinX, m_iDrawGraphRangeY[0][0], m_iDrawGraphRangeX[1] - 1, m_iDrawGraphRangeY[m_iGraphCount - 1][1]);

	for (int i = 0; i < m_iGraphCount; i++) {
		m_MemDC.SelectObject(&m_BrushBackGround[m_iProgramMode]);
		m_MemDC.Rectangle(m_iDrawGraphRangeX[0], m_iDrawGraphRangeY[i][0], m_iDrawGraphRangeX[1], m_iDrawGraphRangeY[i][1]);
	}

	// Grid
	m_MemDC.SelectObject(&m_PenGrid);

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		for (int i = 1; i <= m_iGridCountY * 2 + 1; i++) {
			int iValue = m_iDrawGraphRangeY[iGraph][0] + (m_iDrawGraphRangeY[iGraph][1] - m_iDrawGraphRangeY[iGraph][0]) * i / (m_iGridCountY * 2 + 2);

			m_MemDC.MoveTo(m_iDrawGraphRangeX[0] + 3, iValue);
			m_MemDC.LineTo(m_iDrawGraphRangeX[1] - 3, iValue);
		}
	}

	int iMoveControl_H = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H;
	int iExtendLine = min(100, iMoveControl_H / 10);

	// Data Legend
	int x0 = 25 + iExtendLine / 5;
	int y0 = 38 + DRAW_CONTROL_START_Y;
	int l = 19 + iExtendLine * 3 / 5;
	int dy = DRAW_CONTROL_DY * 2;

	if (m_bFastDraw) {
		CBrush brush(m_bgColor);
		m_MemDC_Select.SelectObject(&brush);
		m_MemDC_Select.Rectangle(180, 0, 380, 800);

		int x1 = 185 + iExtendLine / 5;
		int y1 = 20;

		if (m_bShowDataControl) {
			for (int iData = 0; iData < m_iShowDataControl; iData++) {
				int idx = iData + m_iControlDataOffset;
				m_MemDC_Select.SelectObject(&m_PenData[idx]);

				if (m_bUseDataMark[idx]) {
					int iSize = m_iPenThickness[idx] / 2 + 1;
					if (iSize * 2 < m_iPenThickness[idx] + 2) iSize++;

					m_MemDC_Select.Rectangle(x1 + l / 2 - iSize, y1 + dy * iData / 2 - iSize + 1, x1 + l / 2 + iSize, y1 + dy * iData / 2 + iSize);
				}
				m_MemDC_Select.MoveTo(x1, y1 + dy * iData / 2);
				m_MemDC_Select.LineTo(x1 + l, y1 + dy * iData / 2);
			}
			dc.BitBlt(x0, y0 - DRAW_CONTROL_DY / 2, l + 1, DRAW_CONTROL_DY * m_iShowDataControl, &m_MemDC_Select, x1, y1 - DRAW_CONTROL_DY / 2, SRCCOPY);
		}

		// Math Legend	
		m_MemDC.SetBkColor(RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]));
		m_MemDC.SetTextColor(RGB(m_iTextColor[0], m_iTextColor[1], m_iTextColor[2]));
		if (m_bShowMathControl) {
			y0 = 365 - DRAW_CONTROL_DY * (MAX_DATACONTROL_SIZE - m_iShowDataControl) + DRAW_CONTROL_START_Y;
			y1 = 400;
			for (int iData = 0; iData < m_iShowMathControl; iData++) {
				int idx = iData + m_iControlMathOffset;
				m_MemDC_Select.SelectObject(&m_Pen_Math[idx]);
				if (m_MathData[idx].bUseMark) {
					int iSize = m_MathData[idx].iPenThickness / 2 + 1;
					if (iSize * 2 < m_MathData[idx].iPenThickness + 2) iSize++;

					m_MemDC_Select.Rectangle(x1 + l / 2 - iSize, y1 + dy * iData / 2 - iSize + 1, x1 + l / 2 + iSize, y1 + dy * iData / 2 + iSize);
				}
				m_MemDC_Select.MoveTo(x1, y1 + dy * iData / 2);
				m_MemDC_Select.LineTo(x1 + l, y1 + dy * iData / 2);
			}
			dc.BitBlt(x0, y0 - DRAW_CONTROL_DY / 2, l + 1, DRAW_CONTROL_DY * m_iShowMathControl, &m_MemDC_Select, x1, y1 - DRAW_CONTROL_DY / 2, SRCCOPY);
		}
	}
	else {
		if (m_bShowDataControl) {
			for (int iData = 0; iData < m_iShowDataControl; iData++) {
				int idx = iData + m_iControlDataOffset;
				dc.SelectObject(&m_PenData[idx]);

				if (m_bUseDataMark[idx]) {
					int iSize = m_iPenThickness[idx] / 2 + 1;
					if (iSize * 2 < m_iPenThickness[idx] + 2) iSize++;

					dc.Rectangle(x0 + l / 2 - iSize, y0 + dy * iData / 2 - iSize + 1, x0 + l / 2 + iSize, y0 + dy * iData / 2 + iSize);
				}
				dc.MoveTo(x0, y0 + dy * iData / 2);
				dc.LineTo(x0 + l, y0 + dy * iData / 2);
			}
		}

		// Math Legend	
		m_MemDC.SetBkColor(RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]));
		m_MemDC.SetTextColor(RGB(m_iTextColor[0], m_iTextColor[1], m_iTextColor[2]));
		if (m_bShowMathControl) {
			y0 = 365 - DRAW_CONTROL_DY * (MAX_DATACONTROL_SIZE - m_iShowDataControl) + DRAW_CONTROL_START_Y;
			for (int iData = 0; iData < m_iShowMathControl; iData++) {
				int idx = iData + m_iControlMathOffset;
				dc.SelectObject(&m_Pen_Math[idx]);
				if (m_MathData[idx].bUseMark) {
					int iSize = m_MathData[idx].iPenThickness / 2 + 1;
					if (iSize * 2 < m_MathData[idx].iPenThickness + 2) iSize++;

					dc.Rectangle(x0 + l / 2 - iSize, y0 + dy * iData / 2 - iSize + 1, x0 + l / 2 + iSize, y0 + dy * iData / 2 + iSize);
				}
				dc.MoveTo(x0, y0 + dy * iData / 2);
				dc.LineTo(x0 + l, y0 + dy * iData / 2);
			}
		}
	}
	
	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		BOOL bConti = TRUE;
		int nSelect = 0;
		for (int iData = 0; iData < m_iMaxDataSize && bConti; iData++) {
			if (m_bDraw_GraphData[iGraph][iData]) {
				m_MemDC.SelectObject(&m_PenData[iData]);
				m_MemDC.MoveTo(m_iGraphInfoMinX + 3, 9 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]);
				m_MemDC.LineTo(m_iGraphInfoMinX + 15, 9 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]);
				m_MemDC.TextOutA(m_iGraphInfoMinX + 25, 5 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0], m_strPinName[iData]);
				nSelect++;
				if (m_iDrawGraphRangeY[iGraph][1] < 25 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]) bConti = FALSE;
			}
		}

		// Math Probe
		for (int iData = 0; iData < m_iMaxMathSize && bConti; iData++) {
			if (m_MathData[iData].bValid && m_MathData[iData].bDisplayGraph[iGraph]) {
				m_MemDC.SelectObject(&m_Pen_Math[iData]);
				m_MemDC.MoveTo(m_iGraphInfoMinX + 3, 9 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]);
				m_MemDC.LineTo(m_iGraphInfoMinX + 15, 9 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]);
				m_MemDC.TextOutA(m_iGraphInfoMinX + 25, 5 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0], m_MathData[iData].strName);
				nSelect++;
				if (m_iDrawGraphRangeY[iGraph][1] < 25 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]) bConti = FALSE;
			}
		}

		if (m_bDisplayPosition) {
			// Range
			m_MemDC.SelectObject(&m_PenBlack);
			int iDev = m_iDev;
			m_MemDC.Rectangle(m_iPositionGraphRangeX[0] - iDev, m_iPositionGraphRangeY[0] - iDev, m_iPositionGraphRangeX[1] + iDev, m_iPositionGraphRangeY[1] + iDev);
			m_MemDC.Rectangle(m_iPositionGraphRangeX[0], m_iPositionGraphRangeY[0], m_iPositionGraphRangeX[1], m_iPositionGraphRangeY[1]);
				
			// Grid Line
			for (int i = 0; m_iStarGridValueX_PositionGraph + i * m_iGridSpan_PositionGraph + 3 < m_fMaxValueX_PositionGraph; i++) {
				int iValue = m_iStarGridValueX_PositionGraph + i * m_iGridSpan_PositionGraph;
				if (iValue == 0) m_MemDC.SelectObject(&m_PenOrigin_PositionGraph); else m_MemDC.SelectObject(&m_PenDash_PositionGraph);
				CPoint Pt = CalculationDrawPosition_PositionGraph(iValue, m_fMidValueY_PositionGraph);

				CString str;
				if (Pt.x - 40 > m_iPositionGraphRangeX[0]  && Pt.x + 40 < m_iPositionGraphRangeX[1]) {
					m_MemDC.MoveTo(Pt.x, m_iPositionGraphRangeY[0]);
					m_MemDC.LineTo(Pt.x, m_iPositionGraphRangeY[1]);
					if (iValue >= 0) {
						if (iValue >= 1000)
							str.Format(" %d,%03d ", iValue / 1000, iValue % 1000);
						else
							str.Format(" %d ", iValue);
					}
					else {
						if (iValue <= -1000)
							str.Format("-%d,%03d ", abs(iValue) / 1000, abs(iValue) % 1000);
						else
							str.Format(" %d ", iValue);
					}
					m_MemDC.TextOutA(Pt.x - 20, m_iPositionGraphRangeY[1] - 18, str);
				}
			}

			for (int i = 0; m_iStarGridValueY_PositionGraph + i * m_iGridSpan_PositionGraph + 3 < m_fMaxValueY_PositionGraph; i++) {
				int iValue = m_iStarGridValueY_PositionGraph + i * m_iGridSpan_PositionGraph;
				if (iValue == 0) m_MemDC.SelectObject(&m_PenOrigin_PositionGraph); else m_MemDC.SelectObject(&m_PenDash_PositionGraph);
				CPoint Pt = CalculationDrawPosition_PositionGraph(m_fMidValueX_PositionGraph, iValue);

				CString str;
				if (Pt.y - 40 > m_iPositionGraphRangeY[0] && Pt.y + 40 < m_iPositionGraphRangeY[1]) {
					m_MemDC.MoveTo(m_iPositionGraphRangeX[0], Pt.y);
					m_MemDC.LineTo(m_iPositionGraphRangeX[1], Pt.y);
					if (iValue >= 0) {
						if (iValue >= 1000)
							str.Format(" %d,%03d ", iValue / 1000, iValue % 1000);
						else
							str.Format(" %d ", iValue);
					}
					else {
						if (iValue <= -1000)
							str.Format("-%d,%03d ", abs(iValue) / 1000, abs(iValue) % 1000);
						else
							str.Format(" %d ", iValue);
					}					
					m_MemDC.TextOutA(m_iPositionGraphRangeX[0] + 5, Pt.y - 8 , str);

				}
			}		

			CString str = "측정";
			m_MemDC.SelectObject(&m_PenOrigin_PositionGraph);
			m_MemDC.SelectObject(&m_BrushMark);
			m_MemDC.Rectangle(m_iPositionGraphRangeX[1] - 35, m_iPositionGraphRangeY[1] - 25, m_iPositionGraphRangeX[1] - 2, m_iPositionGraphRangeY[1] - 2);
			m_MemDC.TextOutA(m_iPositionGraphRangeX[1] - 34, m_iPositionGraphRangeY[1] - 20, str);
		
			str = "OFF";
			m_MemDC.Rectangle(m_iPositionGraphRangeX[1] - 35, m_iPositionGraphRangeY[1] - 50, m_iPositionGraphRangeX[1] - 2, m_iPositionGraphRangeY[1] - 27);
			m_MemDC.TextOutA(m_iPositionGraphRangeX[1] - 32, m_iPositionGraphRangeY[1] - 45, str);

			m_MemDC_Position_Init.BitBlt(m_iPositionGraphRangeX[0]- iDev, m_iPositionGraphRangeY[0]- iDev, m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0] + 2 * iDev + 2, m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0] + 2 * iDev + 2, &m_MemDC, m_iPositionGraphRangeX[0]- iDev, m_iPositionGraphRangeY[0] - iDev, SRCCOPY);

		}
	}
}

CPoint CSerialComDlg::CalculationDrawPosition_PositionGraph(double ValueX, double ValueY) {
	CPoint Cal_Point;
	if (m_fMaxValueX_PositionGraph != m_fMinValueX_PositionGraph)
		Cal_Point.x = m_iPositionGraphRangeX[0] + (int)((ValueX - m_fMinValueX_PositionGraph) * (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]) / (m_fMaxValueX_PositionGraph - m_fMinValueX_PositionGraph));
	else
		Cal_Point.x = -1;

	if (m_fMaxValueY_PositionGraph != m_fMinValueY_PositionGraph)
		Cal_Point.y = m_iPositionGraphRangeY[1] - (int)((ValueY - m_fMinValueY_PositionGraph) * (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]) / (m_fMaxValueY_PositionGraph - m_fMinValueY_PositionGraph));
	else
		Cal_Point.y = -1;

	return Cal_Point;
}

void CSerialComDlg::CalculationScale(void) {
	int iCenterX = (m_iPositionGraphRangeX[1] + m_iPositionGraphRangeX[0]) / 2;
	int iCenterY = (m_iPositionGraphRangeY[1] + m_iPositionGraphRangeY[0]) / 2;

	m_fMinValueX_PositionGraph = m_fMidValueX_PositionGraph + (int)((m_iPositionGraphRangeX[0] - iCenterX) * m_fValueScale_PositionGraph);
	m_fMaxValueX_PositionGraph = m_fMidValueX_PositionGraph + (int)((m_iPositionGraphRangeX[1] - iCenterX) * m_fValueScale_PositionGraph);
	m_fMinValueY_PositionGraph = m_fMidValueY_PositionGraph + (int)((m_iPositionGraphRangeY[0] - iCenterY) * m_fValueScale_PositionGraph);
	m_fMaxValueY_PositionGraph = m_fMidValueY_PositionGraph + (int)((m_iPositionGraphRangeY[1] - iCenterY) * m_fValueScale_PositionGraph);

	int iValueSpan = (int)(max(min(m_fMaxValueX_PositionGraph - m_fMinValueX_PositionGraph, m_fMaxValueY_PositionGraph - m_fMinValueY_PositionGraph) / 2, 10));

	//Grid
	int SpanDigit = 0;
	if (iValueSpan > 0)
		SpanDigit = (int)log10(iValueSpan);
	else
		SpanDigit = 1;

	int SpanCoeff = (int)(iValueSpan / pow(10, SpanDigit));
	
	if (SpanCoeff >= 2 && SpanCoeff <= 4) m_iGridSpan_PositionGraph = (int)(10. * pow(10, SpanDigit - 1));
	else if (SpanCoeff >= 5 && SpanCoeff <= 7) m_iGridSpan_PositionGraph = (int)(2. * pow(10, SpanDigit));
	else if (SpanCoeff >= 8) m_iGridSpan_PositionGraph = (int)(5. * pow(10, SpanDigit));
	else m_iGridSpan_PositionGraph = (int)(5. * pow(10, SpanDigit - 1));
	
	// start Grid Value 
	m_iStarGridValueX_PositionGraph = (int)((m_fMinValueX_PositionGraph / m_iGridSpan_PositionGraph) * m_iGridSpan_PositionGraph);
	m_iStarGridValueY_PositionGraph = (int)((m_fMinValueY_PositionGraph / m_iGridSpan_PositionGraph) * m_iGridSpan_PositionGraph);

	DrawInit();
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;

	if (m_bDisplayPosition && iArea == 2) {
		m_Measure_PositionGraph.isPressed = TRUE;
		m_Measure_PositionGraph.P_Last.x = m_Measure_PositionGraph.P_Start.x = point.x;
		m_Measure_PositionGraph.P_Last.y = m_Measure_PositionGraph.P_Start.y = point.y;
	}
	else
		m_Measure_PositionGraph.isPressed = FALSE;		

	CDialog::OnRButtonDown(nFlags, point);
}


void CSerialComDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;

	if (m_bDisplayPosition && iArea == 2 && m_Measure_PositionGraph.isPressed) {
	
		m_Measure_PositionGraph.P_Last.x = point.x;
		m_Measure_PositionGraph.P_Last.y = point.y;

		CString str = "< Measurement Result>\r\n\r\n";

		BOOL bAvailableDistance = TRUE;
		int Sx = -1; 
		if ((m_iPositionGraphRangeX[1] != m_iPositionGraphRangeX[0]))
			Sx = (int)(m_fMinValueX_PositionGraph + (m_Measure_PositionGraph.P_Start.x - m_iPositionGraphRangeX[0]) * (m_fMaxValueX_PositionGraph - m_fMinValueX_PositionGraph) / (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]));
		else
			bAvailableDistance = FALSE;

		int Sy = -1;
		if ((m_iPositionGraphRangeY[1] != m_iPositionGraphRangeY[0]))
			Sy = (int)(m_fMaxValueY_PositionGraph - (m_Measure_PositionGraph.P_Start.y - m_iPositionGraphRangeY[0]) * (m_fMaxValueY_PositionGraph - m_fMinValueY_PositionGraph) / (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]));
		else
			bAvailableDistance = FALSE;

		int Lx = -1;
		if ((m_iPositionGraphRangeX[1] != m_iPositionGraphRangeX[0]))
			Lx = (int)(m_fMinValueX_PositionGraph + (m_Measure_PositionGraph.P_Last.x - m_iPositionGraphRangeX[0]) * (m_fMaxValueX_PositionGraph - m_fMinValueX_PositionGraph) / (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]));
		else
			bAvailableDistance = FALSE;

		int Ly = -1;
		if ((m_iPositionGraphRangeY[1] != m_iPositionGraphRangeY[0]))
			Ly = (int)(m_fMaxValueY_PositionGraph - (m_Measure_PositionGraph.P_Last.y - m_iPositionGraphRangeY[0]) * (m_fMaxValueY_PositionGraph - m_fMinValueY_PositionGraph) / (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]));
		else
			bAvailableDistance = FALSE;
		
        CString str1 = str;
		str.Format("%sStart Point : X:%d   Y:%d\r\n\r\n", (LPCTSTR)str1, Sx, Sy);
        CString str2 = str;
		str.Format("%s End  Point : X:%d   Y:%d\r\n\r\n", (LPCTSTR)str2, Lx, Ly);
		
		if (bAvailableDistance) {
			int iDistance = (int)sqrt((Sx - Lx)* (Sx - Lx) + (Sy - Ly)*(Sy - Ly));
            CString strTemp = str;
			str.Format("%sDistance : %d\r\n", (LPCTSTR)strTemp, iDistance);
		}
		AfxMessageBox(str);		
		m_bReqGraphUpdate = TRUE;
	}

	m_Measure_PositionGraph.isPressed = FALSE;

	CDialog::OnRButtonUp(nFlags, point);
}

void CSerialComDlg::UpdateMathDefine() {
	CString strAddress, strItem, strValue;

	// Math Probe		
	for (int i = 0; i < MAXIMUM_MATH_SIZE; i++) {
		if (m_strPacketHeader == " " || m_strPacketHeader == "")
			strAddress.Format("MathDefine\\MathProbe%d\\default", i);
		else
			strAddress.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);

		strItem.Format("Data1");
		strValue.Format("%d", m_MathData[i].iDataID1);
		SetReg_RegistryData(strAddress, strItem, strValue);

		strItem.Format("Data2");
		strValue.Format("%d", m_MathData[i].iDataID2);
		SetReg_RegistryData(strAddress, strItem, strValue);

		strItem.Format("Operator");
		strValue.Format("%d", m_MathData[i].iOperator);
		SetReg_RegistryData(strAddress, strItem, strValue);

		strItem.Format("DataSize");
		strValue.Format("%d", m_MathData[i].nData);
		SetReg_RegistryData(strAddress, strItem, strValue);

		strItem.Format("ConstValue");
		strValue.Format("%f", m_MathData[i].fValue);
		SetReg_RegistryData(strAddress, strItem, strValue);

        strItem.Format("Result_Integer");
        strValue.Format("%d", m_MathData[i].bResInt);
        SetReg_RegistryData(strAddress, strItem, strValue);

		strItem.Format("UseMathPinName");
		strValue.Format("%d", m_MathData[i].bUserName);
		SetReg_RegistryData(strAddress, strItem, strValue);

		strItem.Format("PinName");
		strValue.Format("%s", (LPCTSTR)m_MathData[i].strName);

		strValue.Replace("[", "");
		strValue.Replace("]", "");
		SetReg_RegistryData(strAddress, strItem, strValue);
		
		for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
			strItem.Format("DrawGraph%d", j);
			if (m_MathData[i].bSelectGraph[j])					
				SetReg_RegistryData(strAddress, strItem, "1");
			else
				SetReg_RegistryData(strAddress, strItem, "0");
		}

		strAddress.Format("MathDefine\\MathProbe%d", i);

		strItem.Format("Red");
		strValue.Format("%d", m_MathData[i].iColor[0]);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem.Format("Green");
		strValue.Format("%d", m_MathData[i].iColor[1]);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem.Format("Blue");
		strValue.Format("%d", m_MathData[i].iColor[2]);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem.Format("PenThickness");
		strValue.Format("%d", m_MathData[i].iPenThickness);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem.Format("PenStyle");
		strValue.Format("%d", m_MathData[i].iPenStyle);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);

		strItem.Format("UseMark");
		strValue.Format("%d", m_MathData[i].bUseMark);
		SetRegRoot_RegistryData(strAddress, strItem, strValue);
	}
}

void CSerialComDlg::OnMButtonDblClk(UINT nFlags, CPoint point) {
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;

	if (m_bDisplayPosition && iArea == 2) {
		m_Resize_PositionGraph.isPressed = FALSE;
		m_bReqAutoScalePositionDraw = TRUE;		
		m_bReqGraphUpdate = TRUE;
		m_bUseAutoScaleGraphPosition = TRUE;
	}

	CDialog::OnMButtonDblClk(nFlags, point);
}

void CSerialComDlg::OnMButtonDown(UINT nFlags, CPoint point)
{

	CDialog::OnMButtonDown(nFlags, point);
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;

	if (iArea == 2 && m_Resize_PositionGraph.isPressed == FALSE) {
		m_Resize_PositionGraph.isPressed = TRUE;
		m_Resize_PositionGraph.P_Start = point;
		m_Resize_PositionGraph.P_Last = point;
	}
	else {
		m_Resize_PositionGraph.isPressed = FALSE;
	}

	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else if (point.y >= m_iPositionGraphRangeY[0] && point.y <= m_iPositionGraphRangeY[1])
		iArea = 2;

	if (iArea == 2 && m_Resize_PositionGraph.isPressed) {
		int iResizePtMaxX = max(m_Resize_PositionGraph.P_Start.x, m_Resize_PositionGraph.P_Last.x);
		int iResizePtMinX = min(m_Resize_PositionGraph.P_Start.x, m_Resize_PositionGraph.P_Last.x);
		int iResizePtMaxY = max(m_Resize_PositionGraph.P_Start.y, m_Resize_PositionGraph.P_Last.y);
		int iResizePtMinY = min(m_Resize_PositionGraph.P_Start.y, m_Resize_PositionGraph.P_Last.y);
		int iResizePtCenterX = (iResizePtMaxX + iResizePtMinX) / 2;
		int iResizePtCenterY = (iResizePtMaxY + iResizePtMinY) / 2;

		if (iResizePtMaxX - iResizePtMinX >= 3 || iResizePtMaxY - iResizePtMinY >= 3) {
			int iWindowPtCenterX = (m_iPositionGraphRangeX[1] + m_iPositionGraphRangeX[0]) / 2;
			int iWindowPtCenterY = (m_iPositionGraphRangeY[1] + m_iPositionGraphRangeY[0]) / 2;

			int iValueMaxX = (int)(m_fMidValueX_PositionGraph + ((iResizePtMaxX - iWindowPtCenterX) * m_fValueScale_PositionGraph));
			int iValueMinX = (int)(m_fMidValueX_PositionGraph + ((iResizePtMinX - iWindowPtCenterX) * m_fValueScale_PositionGraph));
			int iValueMaxY = (int)(m_fMidValueY_PositionGraph - ((iResizePtMinY - iWindowPtCenterY) * m_fValueScale_PositionGraph));
			int iValueMinY = (int)(m_fMidValueY_PositionGraph - ((iResizePtMaxY - iWindowPtCenterY) * m_fValueScale_PositionGraph));

			double fScaleX = (iValueMaxX - iValueMinX) / (m_iPositionGraphRangeX[1] - m_iPositionGraphRangeX[0]);
			double fScaleY = (iValueMaxY - iValueMinY) / (m_iPositionGraphRangeY[1] - m_iPositionGraphRangeY[0]);
			
			m_Resize_PositionGraph.isPressed = FALSE;

			m_fValueScale_PositionGraph = max(0.05, max(fScaleX, fScaleY));
			m_fMidValueX_PositionGraph = (iValueMaxX + iValueMinX) / 2;
			m_fMidValueY_PositionGraph = (iValueMaxY + iValueMinY) / 2;
			m_bUseAutoScaleGraphPosition = FALSE;
			CalculationScale();
		}
		else {
			m_Resize_PositionGraph.isPressed = FALSE;
		}	

		m_bReqGraphUpdate = TRUE;
	}
	CDialog::OnMButtonUp(nFlags, point);
}


void CSerialComDlg::UpdateControlPosition() {
	if (IsDlgButtonChecked(IDC_Display3)) return;

	CClientDC dc(this);

	DRAW_CONTROL_START_Y;  // Control 최상위 위치   50
	DRAW_DATA_CONTROL_Y0;  // Data1 Start 위치      84    (DRAW_CONTROL_START_Y+34)

	m_iPos_DataStart = DRAW_CONTROL_START_Y;
	m_iPos_DataEnd = DRAW_CONTROL_DY * m_iShowDataControl + DRAW_DATA_CONTROL_Y0;

	m_iPos_MathStart = m_iPos_DataEnd + DRAW_CONTROL_GAP_Y;
	m_iPos_MathEnd = m_iPos_MathStart;

	if (IsDlgButtonChecked(IDC_Display2) == TRUE) {
		m_iPos_MathEnd = m_iPos_MathStart + m_iShowMathControl * DRAW_CONTROL_DY + 33;
		m_iPos_FilterStart = m_iPos_MathEnd + DRAW_CONTROL_GAP_Y;
	}
	else {
		m_iPos_FilterStart = m_iPos_DataEnd + DRAW_CONTROL_GAP_Y;
	}
	
	m_iPos_FilterEnd = m_iPos_FilterStart;
	if (IsDlgButtonChecked(IDC_Display1)) {
		m_iPos_FilterEnd = m_iPos_FilterStart + 4 * DRAW_CONTROL_DY + 37;
	}
	else {
		m_iPos_FilterEnd = m_iPos_FilterStart;
	}
}


void CSerialComDlg::ControlView() {	
	UpdateControlPosition();

	BOOL bMathDisplay;
	if (m_bShowMathControl == FALSE) {
        bMathDisplay = FALSE;
		GetDlgItem(IDC_DataSetting2)->ShowWindow(TRUE);
		GetDlgItem(IDC_MathPage0)->ShowWindow(FALSE);
	}
	else {
        bMathDisplay = TRUE;
		GetDlgItem(IDC_DataSetting2)->ShowWindow(TRUE);
		GetDlgItem(IDC_MathPage0)->ShowWindow(m_bShowDataControl);
	}    	
    
	for (int j = 0; j < m_iShowMathControl; j++) {
        GetDlgItem(m_IDC_MathText[j])->ShowWindow(bMathDisplay);
		GetDlgItem(m_IDC_MathProbeValue[j])->ShowWindow(bMathDisplay);
		GetDlgItem(m_IDC_MathProbeName[j])->ShowWindow(bMathDisplay);
	}
    
    for (int j = m_iShowMathControl; j < MAX_MATHCONTROL_SIZE; j++) {
        GetDlgItem(m_IDC_MathText[j])->ShowWindow(FALSE);
        GetDlgItem(m_IDC_MathProbeValue[j])->ShowWindow(FALSE);
        GetDlgItem(m_IDC_MathProbeName[j])->ShowWindow(FALSE);
    }

    if (m_bShowMathControl) {
        GetDlgItem(IDC_DefineMathProbe)->ShowWindow(TRUE);
        GetDlgItem(IDC_DataSetting2)->ShowWindow(TRUE);        
    }
    else {
        GetDlgItem(IDC_DefineMathProbe)->ShowWindow(FALSE);
        GetDlgItem(IDC_DataSetting2)->ShowWindow(FALSE);
    }

	GetDlgItem(IDC_HideMathMenu)->MoveWindow(5, m_iPos_MathStart + 4, 100, 23, TRUE);
	GetDlgItem(IDC_DefineMathProbe)->MoveWindow(108, m_iPos_MathStart + 4, 55, 23, TRUE);	
	GetDlgItem(IDC_MathPage0)->MoveWindow(169, m_iPos_MathStart + 9, 100, 20, TRUE);
	GetDlgItem(IDC_DataSetting2)->MoveWindow(270, m_iPos_MathStart + 7, 150, 20, TRUE);

	int iMoveControl_H = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H;	
	int iExtendValue = min(100, iMoveControl_H / 3);
	int iExtendLine = min(100, iMoveControl_H / 10);
	int iExtendName = iMoveControl_H - iExtendValue - iExtendLine;
	int iValueShift = iExtendLine + iExtendName;	

	// Data Controls Location & Size
	for (int i = 0; i < m_iShowDataControl; i++) {
		GetDlgItem(m_IDC_DataNumber[i])->MoveWindow(10, DRAW_CONTROL_START_Y + 33 + DRAW_CONTROL_DY * i, 14, 12, TRUE);
		GetDlgItem(m_IDC_PinName[i])->MoveWindow(48 + iExtendLine, DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * i - 3, 149 + iExtendName, 18, TRUE);
		GetDlgItem(m_IDC_DataValue[i])->MoveWindow(199 + iValueShift, DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * i - 2, 63 + iExtendValue, 14, TRUE);
	}	

	// Math Controls Location & Size
	for (int i = 0; i < m_iShowMathControl; i++) {
		GetDlgItem(m_IDC_StaticMath[i])->MoveWindow(10, m_iPos_MathStart + 33 + DRAW_CONTROL_DY * i, 14, 12, TRUE);
		GetDlgItem(m_IDC_MathProbeName[i])->MoveWindow(48 + iExtendLine, m_iPos_MathStart + 33 + DRAW_CONTROL_DY * i, 149 + iExtendName, 12, TRUE);
		GetDlgItem(m_IDC_MathProbeValue[i])->MoveWindow(199 + iValueShift, m_iPos_MathStart + 33 + DRAW_CONTROL_DY * i, 63 + iExtendValue, 12, TRUE);
	}	

	if (m_bShowDataControl) {
		GetDlgItem(IDC_STATIC_A1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_A1)->MoveWindow(203 + iMoveControl_H, DRAW_CONTROL_START_Y + 3, 63, 14, TRUE);
		GetDlgItem(IDC_DataSetting)->ShowWindow(TRUE);
		for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
			GetDlgItem(m_IDC_DrawGraph[i])->MoveWindow(270 + (int)(i * 19.7) + iMoveControl_H, DRAW_CONTROL_START_Y + 2, 14, 14, TRUE);
		}
	}
	else {
		GetDlgItem(IDC_STATIC_A1)->ShowWindow(FALSE);
		GetDlgItem(IDC_DataSetting)->ShowWindow(FALSE);
	}
	for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
		GetDlgItem(m_IDC_DrawGraph[i])->ShowWindow(m_bShowDataControl);
	}
    
    GetDlgItem(IDC_DataSetting)->MoveWindow(270 + iMoveControl_H, DRAW_CONTROL_START_Y + 19, 150, 14, TRUE);
    GetDlgItem(IDC_DataSetting2)->MoveWindow(270 + iMoveControl_H, m_iPos_MathStart + 11, 150, 14, TRUE);

	ShowFilterControl();

	OnCbnSelchangeGraphcount();
	ShowDataMathControl();
}

void CSerialComDlg::ShowFilterControl() {
	int cx = m_iWndSizeX, cy = m_iWndSizeY;
	int x0 = 2, y0;
	if (m_bShowMathControl == FALSE)
		y0 = 355;
	else
		y0 = 653;

	int DeltaY = 0;
	DeltaY = DRAW_CONTROL_DY * (m_iShowDataControl - MAX_DATACONTROL_SIZE) + DRAW_CONTROL_START_Y;

	y0 += DeltaY;	
	int iExtendWidth = (m_iRangeLogFilterWidth - 423) / 6;
	int iExtendSpace = (m_iRangeLogFilterWidth - 423) / 3;

	// Filter위치 이동
	BOOL bDisplay1;
	if (m_iFilterViewMode == 0) {
		bDisplay1 = TRUE;
		GetDlgItem(IDC_STATIC_Filter0)->MoveWindow(2, m_iPos_FilterStart, m_iRangeLogFilterWidth, m_iPos_FilterEnd - m_iPos_FilterStart, TRUE);
	}
	else {
		bDisplay1 = FALSE;				
	}

	GetDlgItem(IDC_ClearWatch)->ShowWindow(bDisplay1);
	GetDlgItem(IDC_LoadWatchList)->ShowWindow(bDisplay1);
	GetDlgItem(IDC_ClearFilterList)->ShowWindow(bDisplay1);
	GetDlgItem(IDC_STATIC_Filter0)->ShowWindow(bDisplay1);

	GetDlgItem(IDC_ClearWatch)->MoveWindow(6, m_iPos_FilterStart + 6, 45, 22, TRUE);
	GetDlgItem(IDC_LoadWatchList)->MoveWindow(53, m_iPos_FilterStart + 6, 60, 22, TRUE);
	GetDlgItem(IDC_ClearFilterList)->MoveWindow(91 + 2 * (iExtendSpace + 140) + iExtendWidth * 2, m_iPos_FilterStart + 6, 45, 22, TRUE);

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 4; y++) {
			GetDlgItem(m_IDC_UseFilter[y * 3 + x])->MoveWindow(9 + x * (iExtendSpace + 140), m_iPos_FilterStart + 19 * y + 34, 14, 14, TRUE);
			GetDlgItem(m_IDC_FilterName[y * 3 + x])->MoveWindow(25 + x * (iExtendSpace + 140), m_iPos_FilterStart + 19 * y + 32, 65 + iExtendWidth, 18, TRUE);
			GetDlgItem(m_IDC_FilterValue[y * 3 + x])->MoveWindow(91 + x * (iExtendSpace + 140) + iExtendWidth, m_iPos_FilterStart + 19 * y + 32, 45 + iExtendWidth, 18, TRUE);
		}
	}

	for (int i = 0; i < MAX_FILTER; i++) {
		GetDlgItem(m_IDC_UseFilter[i])->ShowWindow(bDisplay1);
		GetDlgItem(m_IDC_FilterName[i])->ShowWindow(bDisplay1);
		GetDlgItem(m_IDC_FilterValue[i])->ShowWindow(bDisplay1);
	}	
}


void CSerialComDlg::OnBnClickedSerialinfo() {
	OnSetfocusSerialPort();
	OnSetfocusSerialPort2();
	OnSetfocusSerialPort3();

	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[40], szName[200], szBT[200];
	DWORD index = 0, dwSize = 200, dwSize2 = 40, dwType = REG_SZ;

	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));
	memset(szBT, 0x00, sizeof(szBT));

	int iLastComPortIndex = -1;
	CString str = "", str1;
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)) {
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		str1.Format("%s", szName);
		str1.Format("%s", (LPCTSTR)str1.Right(str1.GetLength() - 8));
        CString strTemp = str;
		str.Format("%s%d. %s - %s\r\n", (LPCTSTR)strTemp, index + 1, szData, (LPCTSTR)str1);
		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 200;
		dwSize2 = 40;
		index++;
	}
	if (index == 0) {
		str = "이런~ 사용 가능한 디바이스가 없습니다.!\r\n(There are no available devices.)";
	}
	AfxMessageBox(str);
}

CString CSerialComDlg::RemoveSerialInfo(CString str) {
	CString strReturn;
	strReturn.Format("%s", (LPCTSTR)str);

	int iSize = strReturn.GetLength();
	if (iSize != 0) {
		int iPos = strReturn.Find('(', 0);
		if (iPos != -1) strReturn.Format("%s", (LPCTSTR)strReturn.Left(iPos));
	}
	return strReturn;
}

CString CSerialComDlg::Double2Str(double val) {
	BOOL bMinus = FALSE;
	CString str;
	if (val < -0.000005) {
		val -= 0.000005;
	}
	else if (val < 0.000005) {
		str = "0";
		return str;
	}
	else {
		val += 0.000005;
	}	
	
	str.Format("%.4f", val);
	int len = str.GetLength();
	for (int i = len - 1; i > 0; i--) {
		char ch = str.GetAt(i);
		if (ch == '0') {
			str.SetAt(i, 0);
		}
		else {
			if (ch == '.') {
				str.SetAt(i, 0);
			}
			break;
		}
	}
	return str;
}


CString CSerialComDlg::Float2Str(float val) {
    BOOL bMinus = FALSE;
    CString str;
    float v = abs(val);
    int n = 0;
    if (v >= 1.f) {
        while (v >= 1.f) {
            n++;
            v /= 10.;
        }
    }
    else if (v < 0.0000005f) {
        str = "0";
        return str;
    }
    else {
        while (v <= 1.f) {
            n--;
            v *= 10.;
        }
    }
    
    if (n >= 4) { str.Format("%.3f", val);  }
    else if (n == 3) { str.Format("%.4f", val); }
    else if (n == 2) { str.Format("%.5f", val); }
    else if (n == 1) { str.Format("%.6f", val); }
    else if (n == 0) { str.Format("%.7f", val); }
    else if (n == -1) { str.Format("%.8f", val); }
    else if (n == -2) { str.Format("%.9f", val); }
    else { str.Format("%.10f", val); }

    int len = str.GetLength();
    for (int i = len - 1; i > 0; i--) {
        char ch = str.GetAt(i);
        if (ch == '0') {
            str.SetAt(i, 0);
        }
        else {
            if (ch == '.') {
                str.SetAt(i, 0);
            }
            break;
        }
    }
    return str;
}

void CSerialComDlg::ShowSelectValueDrawInit(int iSelectIndex) {
	if (iSelectIndex >= m_iHistoryIndex) iSelectIndex = m_iHistoryIndex - 1;
	if (iSelectIndex < 0) iSelectIndex = 0;	

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		CString str;
		BOOL bConti = TRUE;
		int nSelect = 0;
		m_MemDC.SetBkColor(RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]));
		m_MemDC.SetTextColor(RGB(m_iTextColor[0], m_iTextColor[1], m_iTextColor[2]));
		for (int iData = 0; iData < m_iMaxDataSize && bConti; iData++) {
			if (m_bDraw_GraphData[iGraph][iData]) {
				// determine digit
				str.Format("%s  %s               ", (LPCTSTR)m_strPinName[iData], (LPCTSTR)Double2Str(m_HistoryData[iSelectIndex].fDataValue[iData]));
				m_MemDC.TextOutA(m_iGraphInfoMinX + 25, 5 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0], str);
				nSelect++;
				if (m_iDrawGraphRangeY[iGraph][1] < 25 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]) bConti = FALSE;				
			}
		}	

		// Math Probe
		for (int iData = 0; iData < m_iMaxMathSize && bConti; iData++) {
			if (m_MathData[iData].bValid && m_MathData[iData].bDisplayGraph[iGraph]) {
				// determine digit	
				str.Format("%s  %s               ", (LPCTSTR)m_MathData[iData].strName, (LPCTSTR)Double2Str(m_HistoryData[iSelectIndex].fMathValue[iData]));
				m_MemDC.TextOutA(m_iGraphInfoMinX + 25, 5 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0], str);
				nSelect++;
				if (m_iDrawGraphRangeY[iGraph][1] < 25 + nSelect * 20 + m_iDrawGraphRangeY[iGraph][0]) bConti = FALSE;				
			}			
		}
	}	

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		m_MemDC.SelectObject(&m_PenBlack);
		m_MemDC.MoveTo(m_iGraphInfoMaxX - 1, m_iDrawGraphRangeY[iGraph][0]);
		m_MemDC.LineTo(m_iGraphInfoMaxX - 1, m_iDrawGraphRangeY[iGraph][1]);
	}
}

void CSerialComDlg::OptimizeGraphCount() {
	int iGraphCount = 0;
	CString strDataSetting = "";
	strDataSetting = "";
	CString str;
	
	for (int i = 0; i < m_iMaxMathSize; i++) {
		for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
			m_MathData[i].bDisplayGraph[j] = m_MathData[i].bSelectGraph[j];
		}
	}

	for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
		BOOL bFindDraw = FALSE;
		if (m_bDrawGraph[i]) {
			for (int j = 0; j < m_iMaxDataSize; j++) {
				if (m_bSelectDraw_GraphData[i][j]) {
					bFindDraw = TRUE;
					break;
				}
			}
			if (!bFindDraw) {
				for (int j = 0; j < m_iMaxMathSize; j++) {
					if (m_MathData[j].bDisplayGraph[i] && m_MathData[j].bValid) {
						bFindDraw = TRUE;
						break;
					}
				}
			}
		}

		if (bFindDraw) {
			m_RealGrpahInfo[iGraphCount].iDraw2RealGraphID = i;
			m_RealGrpahInfo[i].iDrawGraphID = iGraphCount;

			for (int j = 0; j < m_iMaxDataSize; j++) {
				m_bDraw_GraphData[iGraphCount][j] = m_bSelectDraw_GraphData[i][j];
			}

			for (int j = 0; j < m_iMaxMathSize; j++) {
				m_MathData[j].bDisplayGraph[iGraphCount] = m_MathData[j].bDisplayGraph[i];
			}			

			m_fRangeMax[iGraphCount] = m_RealGrpahInfo[i].fRangeMax;
			m_fRangeMin[iGraphCount] = m_RealGrpahInfo[i].fRangeMin;
			m_bAutoRange_Graph[iGraphCount] = m_RealGrpahInfo[i].bAutoRange_Graph;

			m_strRangeMax[iGraphCount].Format("%s", (LPCTSTR)Dbl2Str(m_fRangeMax[iGraphCount]));
			m_strRangeMin[iGraphCount].Format("%s", (LPCTSTR)Dbl2Str(m_fRangeMin[iGraphCount]));
			m_strRangeMid[iGraphCount].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMax[iGraphCount] + m_fRangeMin[iGraphCount]) / 2));

			iGraphCount++;
            CString strTemp = strDataSetting;
			strDataSetting.Format("%s%d  ", (LPCTSTR)strTemp, iGraphCount);
		}
		else {
			strDataSetting += "   ";
		}
		str.Format("%d", i);
        if (i == 0 || i == 4) strDataSetting += " ";
	}

	m_strDataSetting.Format("%s", (LPCTSTR)strDataSetting);
	m_strDataSetting2.Format("%s", (LPCTSTR)strDataSetting);

	if (iGraphCount == 0) {
		for (int j = 0; j < m_iMaxDataSize; j++) m_bDraw_GraphData[iGraphCount][j] = FALSE;
		iGraphCount++;
	}
	m_iGraphCount = iGraphCount;
	UDF;	
	for (int i = max(1, m_iGraphCount); i < MAX_GRAPH_COUNT; i++) {
		GetDlgItem(m_IDC_MaxY_Graph[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_MidY_Graph[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_MinY_Graph[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_AutoRange[i])->ShowWindow(FALSE);
	}	

	OnSize(0, m_iWndSizeX, m_iWndSizeY);	// 변경된 Graph Count 반영

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		if (m_fRangeMax[iGraph] - m_fRangeMin[iGraph] == 0) m_strRangeMax[iGraph].Format("%s", (LPCTSTR)Dbl2Str(++m_fRangeMax[iGraph]));
	}
	DrawInit();
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::UpdateDrawGraphControl() {
	for (int i = 0; i < MAX_GRAPH_COUNT; i++) {
		m_bDrawGraph[i] = IsDlgButtonChecked(m_IDC_DrawGraph[i]);
	}
    GetRegGrphHeight();
	m_bReqUpdateControlDraw = TRUE;
	OptimizeGraphCount();
	ShowDataMathControl();
}

void CSerialComDlg::ShowGraphControlbyHeight() {
    for (int i = 0; i < m_iGraphCount; i++) {
        int spanY = m_iDrawGraphRangeY[i][1] - m_iDrawGraphRangeY[i][0];
        if (spanY < 18) {
            GetDlgItem(m_IDC_MidY_Graph[i])->ShowWindow(FALSE);
            GetDlgItem(m_IDC_MinY_Graph[i])->ShowWindow(FALSE);
            GetDlgItem(m_IDC_MaxY_Graph[i])->ShowWindow(FALSE);
            GetDlgItem(m_IDC_AutoRange[i])->ShowWindow(FALSE);
        }
        else if (spanY < 36) {
            GetDlgItem(m_IDC_MidY_Graph[i])->MoveWindow(m_iDrawGraphRangeX[0] - 60, (m_iDrawGraphRangeY[i][0] + m_iDrawGraphRangeY[i][1]) / 2 - 7, 48, 14, TRUE);
            GetDlgItem(m_IDC_MidY_Graph[i])->ShowWindow(TRUE);
            GetDlgItem(m_IDC_MinY_Graph[i])->ShowWindow(FALSE);
            GetDlgItem(m_IDC_MaxY_Graph[i])->ShowWindow(FALSE);
            GetDlgItem(m_IDC_AutoRange[i])->ShowWindow(FALSE);
        }
        else {            
            GetDlgItem(m_IDC_MinY_Graph[i])->MoveWindow(m_iDrawGraphRangeX[0] - 60, m_iDrawGraphRangeY[i][1] - 15, 58, 14, TRUE);
            GetDlgItem(m_IDC_MaxY_Graph[i])->MoveWindow(m_iDrawGraphRangeX[0] - 60, m_iDrawGraphRangeY[i][0], 58, 14, TRUE);            
            GetDlgItem(m_IDC_MinY_Graph[i])->ShowWindow(TRUE);
            GetDlgItem(m_IDC_MaxY_Graph[i])->ShowWindow(TRUE);

            if (spanY > 90) {
                GetDlgItem(m_IDC_AutoRange[i])->MoveWindow(m_iDrawGraphRangeX[0] - 64, (3 * m_iDrawGraphRangeY[i][0] + m_iDrawGraphRangeY[i][1]) / 4 - 5, 42, 14, TRUE);
                GetDlgItem(m_IDC_MidY_Graph[i])->MoveWindow(m_iDrawGraphRangeX[0] - 60, (m_iDrawGraphRangeY[i][0] + m_iDrawGraphRangeY[i][1]) / 2 - 7, 48, 14, TRUE);
                GetDlgItem(m_IDC_AutoRange[i])->ShowWindow(TRUE);
                GetDlgItem(m_IDC_MidY_Graph[i])->ShowWindow(TRUE);
            }
            else if (spanY > 48) {
                GetDlgItem(m_IDC_AutoRange[i])->MoveWindow(m_iDrawGraphRangeX[0] - 60, (m_iDrawGraphRangeY[i][0] + m_iDrawGraphRangeY[i][1]) / 2 - 7, 48, 14, TRUE);
                GetDlgItem(m_IDC_AutoRange[i])->ShowWindow(TRUE);
                GetDlgItem(m_IDC_MidY_Graph[i])->ShowWindow(FALSE);
            }
            else {
                GetDlgItem(m_IDC_AutoRange[i])->ShowWindow(FALSE);
                GetDlgItem(m_IDC_MidY_Graph[i])->ShowWindow(FALSE);
            }
        }
    }
}


void CSerialComDlg::ChangeDataName() {
    CWnd* pWnd = GetFocus();
    m_IDC_Focus = -1;
    if (pWnd) {
        m_IDC_Focus = pWnd->GetDlgCtrlID();
    }
    for (int i = 0; i < m_iShowDataControl; i++) {
        if (m_IDC_Focus == m_IDC_PinName[i]) {
            int idx = i + m_iControlDataOffset;
            GetDlgItemText(m_IDC_PinName[i], m_strPinName[idx]);
        }
    }    
}

void CSerialComDlg::DrawTimeline(void) {
	CClientDC dc(this);

	// Outline	
	dc.SelectObject(&m_BrushBackGround[1]);
	dc.Rectangle(m_iDrawGraphRangeX[0], GRAPH_TIMELINE_Y0, m_iGraphInfoMaxX, GRAPH_TIMELINE_Y1);

	// Timeline
	dc.SelectObject(&BRUSH_GRAY);
	dc.Rectangle(m_iDrawGraphRangeX[0] + 20, GRAPH_TIMELINE_Y0 + 4, m_iGraphInfoMaxX - 20, GRAPH_TIMELINE_Y1 - 4);
	if (m_iHistoryIndex < 10) { 
		m_iTimeLineX[0] = m_iTimeLineX[1] = 0;
		return; 
	}
	m_iTimeLineX[1] = CalTimelinePoint(m_iHistoryMove);   // Right
	m_iTimeLineX[0] = CalTimelinePoint(m_iHistoryMove + m_iDisplaySize);  // Left
	
	if (m_bTimelineControl[0]) {
		dc.SelectObject(&BRUSH_ORANGE);
	}
	else {
		dc.SelectObject(&BRUSH_YELLOW);
	}
	dc.Rectangle(m_iTimeLineX[0], GRAPH_TIMELINE_Y0 + 2, m_iTimeLineX[1], GRAPH_TIMELINE_Y1 - 2);

	if (m_bTimelineControl[1]) 
		dc.SelectObject(&BRUSH_BLUE);			
	else
		dc.SelectObject(&BRUSH_GRAY);
	dc.Rectangle(m_iTimeLineX[0] - 10, GRAPH_TIMELINE_Y0 + 2, m_iTimeLineX[0], GRAPH_TIMELINE_Y1 - 2);
	
	
	if (m_bTimelineControl[2]) 
		dc.SelectObject(&BRUSH_BLUE);
	else
		dc.SelectObject(&BRUSH_GRAY);
	dc.Rectangle(m_iTimeLineX[1] + 10, GRAPH_TIMELINE_Y0 + 2, m_iTimeLineX[1], GRAPH_TIMELINE_Y1 - 2);
	
}


int CSerialComDlg::CalTimelinePoint(int iIndex) {
	
	long long  iReturn;
	long long x2 = static_cast<long long>(m_iDrawGraphRangeX[0] + 20);  // for index = 0
	long long x1 = static_cast<long long>(m_iGraphInfoMaxX - 20);       // for index = m_iHistoryIndex
	if (m_iHistoryIndex > 0)
		iReturn = x1 + (x2 - x1)* iIndex / m_iHistoryIndex;
	else
		iReturn = (x1 + x2) / 2;
	if (iReturn < x2) iReturn = x2;
	if (iReturn > x1) iReturn = x1;
	return (int)iReturn;
}

int CSerialComDlg::CalIndexChangeIndexTimeline(int iShiftPoint) {
	int iReturnIndex;	
	iReturnIndex = m_iHistoryIndex * iShiftPoint / max(1, (m_iGraphInfoMaxX - m_iDrawGraphRangeX[0] - 40));
	return iReturnIndex;
}


void CSerialComDlg::OnMouseMove(UINT nFlags, CPoint point) {
	// Area설정
	// iArea     1: Graph    2: Position   0: 그외	

	int iArea = 0;
	if (point.x < m_iDrawGraphRangeX[0])
		iArea = 0;
	else if (point.x <= m_iGraphInfoMaxX)
		iArea = 1;
	else
		iArea = 2;

	BOOL bReqGraphUpdate = FALSE;
	int iPreSelectIndex = -1;
	int iSelectIndex = -1;

	if (m_Cursor.iSelectCursor != -1) {
		if (iArea == 1 && point.x < m_iDrawGraphRangeX[1] && point.x > m_iDrawGraphRangeX[0] + 1){
			m_Cursor.iPos[m_Cursor.iSelectCursor] = point.x;
			bReqGraphUpdate = TRUE;
		}
	}
	
	// Timeline Control bar check
	if (m_iHistoryIndex > 10) {
		if (m_bTimelineControl[0])
			bReqGraphUpdate = TRUE;
		else if (m_bTimelineControl[1])
			bReqGraphUpdate = TRUE;
		else if (m_bTimelineControl[2])
			bReqGraphUpdate = TRUE;

		if (bReqGraphUpdate && m_iProgramMode == DATA_ANALYZE_MODE) {
			int iShiftPoint = point.x - m_iTimelineStartPoint;
			int iMoveIndex = CalIndexChangeIndexTimeline(iShiftPoint);
			m_iTimelineStartPoint = point.x;			
			if (m_bDrawSelectData) {
				iPreSelectIndex = atoi(m_strIndexNumber);
			}

			if (m_bTimelineControl[0]) {  // Timeline Move				
				if (m_iHistoryMove - iMoveIndex < 0) iMoveIndex = m_iHistoryMove;
				if (m_iHistoryMove + m_iDisplaySize - iMoveIndex > m_iHistoryIndex) iMoveIndex = m_iHistoryMove + m_iDisplaySize - m_iHistoryIndex;					
				m_iHistoryMove = max(0, m_iHistoryMove - iMoveIndex);
			}
			else if (m_bTimelineControl[1]) {				
				m_iDisplaySize -= iMoveIndex;
				m_iDisplaySize = max(2, m_iDisplaySize);
				if (m_iTimeLineX[0] >= m_iTimeLineX[1]) {
					m_bTimelineControl[2] = TRUE;
					m_bTimelineControl[1] = FALSE;
				}
				SetDisplayDataSize();
				m_bTimelineFullRangeView = FALSE;
			}
			else if (m_bTimelineControl[2]) {
				m_iHistoryMove = max(0, m_iHistoryMove - iMoveIndex);
				m_iDisplaySize += iMoveIndex;				 
				m_iDisplaySize = max(2, m_iDisplaySize);
				SetDisplayDataSize();
				m_bTimelineFullRangeView = FALSE;

				if (m_iTimeLineX[0] >=  m_iTimeLineX[1]) {
					m_bTimelineControl[1] = TRUE;
					m_bTimelineControl[2] = FALSE;
				}
			}			
		}
		else {
			m_bTimelineControl[0] = FALSE;
			m_bTimelineControl[0] = FALSE;
			m_bTimelineControl[0] = FALSE;
		}
		
	}
	else {
		m_bTimelineControl[0] = FALSE;
		m_bTimelineControl[0] = FALSE;
		m_bTimelineControl[0] = FALSE;
	}

	if (iArea == 1) {
		if (m_bUseStatistics) {
			if (m_iAdjustSelectMode == 0) {
				if (nFlags & MK_LBUTTON) {
					int iTemp = m_iAdjustSelectPoint[0];
					m_iAdjustSelectPoint[0] = min(max(point.x, m_iDrawGraphRangeX[0]), m_iDrawGraphRangeX[1]);
					if (iTemp != m_iAdjustSelectPoint[0]) {
						if (m_iAdjustSelectPoint[0] > m_iAdjustSelectPoint[1]) {
							m_iAdjustSelectMode = 1;
							iTemp = m_iAdjustSelectPoint[1];
							m_iAdjustSelectPoint[1] = m_iAdjustSelectPoint[0];
							m_iAdjustSelectPoint[0] = iTemp;
						}
						m_bReqGraphUpdate = TRUE;
					}
					else if (nFlags != MK_LBUTTON) {
						CString str;
						str.Format("%d", nFlags);
						AfxMessageBox(str);
						m_iAdjustSelectMode = -1;
						m_bReqGraphUpdate = TRUE;
					}
				}
			}
			else if (m_iAdjustSelectMode == 1) {
				if (nFlags & MK_LBUTTON) {
					int iTemp = m_iAdjustSelectPoint[1];
					m_iAdjustSelectPoint[1] = min(max(point.x, m_iDrawGraphRangeX[0]), m_iDrawGraphRangeX[1]);
					if (iTemp != m_iAdjustSelectPoint[1]) {
						if (m_iAdjustSelectPoint[0] > m_iAdjustSelectPoint[1]) {
							m_iAdjustSelectMode = 0;
							iTemp = m_iAdjustSelectPoint[1];
							m_iAdjustSelectPoint[1] = m_iAdjustSelectPoint[0];
							m_iAdjustSelectPoint[0] = iTemp;
						}
						m_bReqGraphUpdate = TRUE;
					}
				}
				else if (nFlags != MK_LBUTTON) {
					m_iAdjustSelectMode = -1;
					m_bReqGraphUpdate = TRUE;
				}
			}
		}
		else if (m_bIsAdjustMode) {
			CClientDC dc(this);
			dc.SelectObject(&m_PenWhite);
			dc.SelectObject(&m_BrushBackGround[1]);
			CPoint P[3];

			if (m_bAdjustLegendX) {
				dc.Rectangle(m_iDrawGraphRangeX[0], GRAPH_START_Y - 7, m_iActuralWindowSize[0] + 1, GRAPH_START_Y - 1);

				if (point.x <= m_iDrawGraphRangeX[0] + 100) point.x = m_iDrawGraphRangeX[0] + 100;
				int iLegendX = min(600, max(20, m_iGraphInfoMaxX - point.x + 1));
				int iDelta;
				if (m_bDisplayPosition)
					iDelta = iLegendX - m_iLegendX_PositionGraph;
				else
					iDelta = iLegendX - m_iLegendX;

				int iNewPositionX = m_iDrawGraphRangeX[1] - iDelta;

				dc.SelectObject(&m_BrushAdjust[1]);

				P[0].SetPoint(point.x - GRAPH_GAP_X / 2, GRAPH_START_Y - 7);
				P[1].SetPoint(point.x + GRAPH_GAP_X * 3 / 2, GRAPH_START_Y - 7);
				P[2].SetPoint(point.x + GRAPH_GAP_X / 2, GRAPH_START_Y - 1);

				dc.Polygon(P, 3);
			}

			dc.SelectObject(&m_PenWhite);
			dc.SelectObject(&m_BrushBackGround[1]);
			dc.Rectangle(m_iGraphInfoMinX, m_iDrawGraphRangeY[0][0], m_iDrawGraphRangeX[1], m_iDrawGraphRangeY[m_iGraphCount - 1][1]);

			for (int i = 0; i < m_iGraphCount - 1; i++) {
				// 유효성 check
				for (int i = 0; i < m_iGraphCount - 1; i++) {
					if (m_bAdjustLegendY[i]) { // Y그래프 크기 조정	
						if (point.y - m_iDrawGraphRangeY[i][0] <= 30)
							point.y = m_iDrawGraphRangeY[i][0] + 30;
						else if (m_iDrawGraphRangeY[i + 1][1] - point.y <= 30)
							point.y = m_iDrawGraphRangeY[i + 1][1] - 30;			

						dc.SelectObject(&m_BrushAdjust[1]);
						P[0].SetPoint(m_iGraphInfoMinX - 1, point.y - GRAPH_GAP_Y);
						P[1].SetPoint(m_iGraphInfoMinX - GRAPH_GAP_X, point.y + GRAPH_GAP_Y / 2);
						P[2].SetPoint(m_iGraphInfoMinX - 1, point.y + GRAPH_GAP_Y * 2);
						dc.Polygon(P, 3);
					}
					else {
						dc.SelectObject(&m_BrushAdjust[0]);
						dc.Rectangle(m_iGraphInfoMinX, m_iDrawGraphRangeY[i][1] - 1, m_iDrawGraphRangeX[1] - 1, m_iDrawGraphRangeY[i + 1][0] + 1);
					}
				}
			}	
			dc.SelectObject(&m_PenBlack);
			dc.MoveTo(m_iDrawGraphRangeX[0], GRAPH_START_Y);
			dc.LineTo(m_iGraphInfoMaxX, GRAPH_START_Y);
			dc.MoveTo(m_iGraphInfoMinX + 0, m_iDrawGraphRangeY[m_iGraphCount - 1][1]);
			dc.LineTo(m_iGraphInfoMinX + 0, m_iDrawGraphRangeY[0][0]);
		}
	}
	if (iArea == 2) {
		if (m_Panning_PositionGraph.isPressed) {
			m_Panning_PositionGraph.P_Last.x = point.x;
			m_Panning_PositionGraph.P_Last.y = point.y;
			m_bReqGraphUpdate = TRUE;
		}

		if (m_Measure_PositionGraph.isPressed) {
			m_Measure_PositionGraph.P_Last.x = point.x;
			m_Measure_PositionGraph.P_Last.y = point.y;
			m_bReqGraphUpdate = TRUE;
		}

		if (m_Resize_PositionGraph.isPressed) {
			m_Resize_PositionGraph.P_Last.x = point.x;
			m_Resize_PositionGraph.P_Last.y = point.y;
			m_bReqGraphUpdate = TRUE;
		}


		if (m_Measure3P.bIsMeasuring && m_Measure3P.bIsClick) {
			if (nFlags == MK_LBUTTON) {
				m_Measure3P.Pt[m_Measure3P.Index].SetPoint(point.x, point.y);
				m_bReqGraphUpdate = TRUE;
			}
		}
	}
	else {
		m_Panning_PositionGraph.isPressed = FALSE;
		m_Measure_PositionGraph.isPressed = FALSE;
	}

	if (m_AdjustLogFilter_V.isPressed) {
		if (nFlags != MK_LBUTTON) {
			m_AdjustLogFilter_V.isPressed = FALSE;
			SetLogFilterSize_V(point);
		}
	}
	if (m_AdjustLogFilter_H.isPressed) {
		if (nFlags != MK_LBUTTON) {
			m_AdjustLogFilter_H.isPressed = FALSE;
			SetLogFilterSize_H(point);
		}
	}	
	if (m_AdjustDataWnd_V.isPressed) {
		if (nFlags != MK_LBUTTON) {
			m_AdjustDataWnd_V.isPressed = FALSE;
			SetDataWndSize_V(point);
		}
	}

	if (bReqGraphUpdate) {
		if (!m_bDrawSelectData) {
				iSelectIndex = m_iHistoryIndex - m_iHistoryMove - m_iDisplaySize / 2;
		}
		else {
			iSelectIndex = min(m_iHistoryIndex - m_iHistoryMove, iPreSelectIndex);
			iSelectIndex = max(iSelectIndex, m_iHistoryIndex - m_iHistoryMove - m_iDisplaySize);
		}
		if (m_Cursor.iSelectCursor == -1) {
			ChangeIndexNumber(iSelectIndex);
		}
		else {
			m_bReqGraphUpdate = TRUE;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CSerialComDlg::ChangeIndexNumber(int iIndex) {
	iIndex = max(0, min(m_iHistoryIndex, iIndex));
	m_strIndexNumber.Format("%d", iIndex);
	m_iCurrentIndex = iIndex;
	m_ctrlFilterBox.SetCurSel(m_HistoryData[iIndex].iHistoryFilterIndex);
	//m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
	HighlightLog(m_HistoryData[iIndex].iHistoryLogIndex);
	//UDF;
	m_iHistorySelectPt = m_iDrawGraphRangeX[0] + m_iDrawGraphSpanX * (iIndex + m_iHistoryMove + m_iDisplaySize - m_iHistoryIndex) / m_iDisplaySize;
	m_bDrawSelectData = TRUE;
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::OnBnClickedGame() {
	CGame game;
    WSADATA temp;
    WSAStartup(0x0202, &temp);
	game.pMain = this;
	game.DoModal();
	WSACleanup();
}

void CSerialComDlg::OnBnClickedRange() {
	UDT;
}


void CSerialComDlg::DrawSelectDataLabel(int iSelectIndex, int iStartIndex) {	
	if ((size_t)iSelectIndex >= m_HistoryData.size()) return;
	m_MemDC.SelectObject(&m_FontLabel);
	CString str, s;

	for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
		BOOL bConti = TRUE;		
		int nSelect = 0;
		m_iIndicator = 0;
		
		if (m_bShowProbeDataID || m_bShowProbeValue) {
			// Math Probe
			for (int iData = 0; iData < m_iMaxMathSize && bConti; iData++) {
				if (m_MathData[iData].bValid && m_MathData[iData].bDisplayGraph[iGraph] && m_iIndicator < N_MAX_INDICATOR) {
					// determine digit				
					double fValue = m_HistoryData[iSelectIndex].fMathValue[iData];
					str = "";
					if (m_bShowProbeValue) {
						str.Format("%s", (LPCTSTR)Double2Str(fValue));
					}
					if (m_bShowProbeDataID) {
						s.Format("M%d ", iData + 1);
						str = s + str;
					}
					m_Indicator[m_iIndicator].val = (fValue);					

					int iMaxX = m_iDrawGraphRangeX[1] - strlen(str) * W_LABEL_TEXT;
					int r, g, b;

					switch (m_iSelTextColorMode) {
					case 0:
					case 4:
						r = m_MathData[iData].iColor[0];
						g = m_MathData[iData].iColor[1];
						b = m_MathData[iData].iColor[2];
						break;
					case 1:
						r = m_iSelTextColor[0];
						g = m_iSelTextColor[1];
						b = m_iSelTextColor[2];
						break;
					case 2:
						r = m_iBackgroundColor[0][0];
						g = m_iBackgroundColor[0][1];
						b = m_iBackgroundColor[0][2];
						break;
					case 3:
						r = m_iTextColor[0];
						g = m_iTextColor[1];
						b = m_iTextColor[2];
						break;
					}

					if (m_iSelTextColorMode == 4)
						m_Indicator[m_iIndicator].text = InvColor(r, g, b);			//m_MemDC.SetTextColor(InvColor(r, g, b));
					else
						m_Indicator[m_iIndicator].text = RGB(r, g, b);

					switch (m_iSelBkColorMode) {
					case 0:
						m_Indicator[m_iIndicator].bk = RGB(m_MathData[iData].iColor[0], m_MathData[iData].iColor[1], m_MathData[iData].iColor[2]);
						break;
					case 1:
						m_Indicator[m_iIndicator].bk = RGB(m_iSelBkColor[0], m_iSelBkColor[1], m_iSelBkColor[2]);
						break;
					case 2:
						m_Indicator[m_iIndicator].bk = RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]);
						break;
					case 3:
						if (m_iSelTextColorMode != 4)
							m_Indicator[m_iIndicator].bk = InvColor(r, g, b);	//m_MemDC.SetBkColor(InvColor(r, g, b));
						else
							m_Indicator[m_iIndicator].bk = RGB(r, g, b);
						break;
					}

					// Position Check
					m_Indicator[m_iIndicator].pt = CalculateDrawPoint(iGraph, m_iHistorySelectIndex - iStartIndex, (float)m_HistoryData[iSelectIndex].fMathValue[iData]);

					m_Indicator[m_iIndicator].str.Format("%s", (LPCTSTR)str);
					m_iIndicator++;
				}
			}
		
			// Data Probe
			for (int iData = 0; iData < m_iMaxDataSize && bConti; iData++) {
				if (m_bDraw_GraphData[iGraph][iData] && m_iIndicator < N_MAX_INDICATOR) {
					// determine digit			
					str = "";
					double fValue = m_HistoryData[iSelectIndex].fDataValue[iData];
					if (m_bShowProbeValue) {
						str.Format("%s", (LPCTSTR)Double2Str(fValue));
					}

					if (m_bShowProbeDataID) {
						s.Format("D%d ", iData + 1);
						str = s + str;
					}
					m_Indicator[m_iIndicator].val = (fValue);
					int iMaxX = m_iDrawGraphRangeX[1] - strlen(str) * W_LABEL_TEXT;
					int r, g, b;
					switch (m_iSelTextColorMode) {
					case 0:
					case 4:
						r = m_iPenColor[iData][0];
						g = m_iPenColor[iData][1];
						b = m_iPenColor[iData][2];
						break;
					case 1:
						r = m_iSelTextColor[0];
						g = m_iSelTextColor[1];
						b = m_iSelTextColor[2];
						break;
					case 2:
						r = m_iBackgroundColor[0][0];
						g = m_iBackgroundColor[0][1];
						b = m_iBackgroundColor[0][2];
						break;
					case 3:
						r = m_iTextColor[0];
						g = m_iTextColor[1];
						b = m_iTextColor[2];
						break;
					}

					if (m_iSelTextColorMode == 4)
						m_Indicator[m_iIndicator].text = InvColor(r, g, b);			//m_MemDC.SetTextColor(InvColor(r, g, b));
					else
						m_Indicator[m_iIndicator].text = RGB(r, g, b);

					switch (m_iSelBkColorMode) {
					case 0:
						m_Indicator[m_iIndicator].bk = RGB(m_iPenColor[iData][0], m_iPenColor[iData][1], m_iPenColor[iData][2]);
						break;
					case 1:
						m_Indicator[m_iIndicator].bk = RGB(m_iSelBkColor[0], m_iSelBkColor[1], m_iSelBkColor[2]);
						break;
					case 2:
						m_Indicator[m_iIndicator].bk = RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]);
						break;
					case 3:
						if (m_iSelTextColorMode != 4)
							m_Indicator[m_iIndicator].bk = InvColor(r, g, b);	//m_MemDC.SetBkColor(InvColor(r, g, b));
						else
							m_Indicator[m_iIndicator].bk = RGB(r, g, b);
						break;
					}

					// Position Check
					m_Indicator[m_iIndicator].pt = CalculateDrawPoint(iGraph, m_iHistorySelectIndex - iStartIndex, (float)m_HistoryData[iSelectIndex].fDataValue[iData]);
					m_Indicator[m_iIndicator].str.Format("%s", (LPCTSTR)str);
					m_iIndicator++;			
				}			
			}
		}
		if (m_iIndicator > 0) {
			DrawIndicator(iGraph);
		}
	}
}

void CSerialComDlg::OnBnClickedShowselectvalue() {
	UDT;
	m_bReqGraphUpdate = TRUE;
}

void CSerialComDlg::OnBnClickedClearCursor() {
	m_Cursor.Clear();
	m_bDrawSelectData = FALSE;
	m_bReqGraphUpdate = TRUE;
}


void CSerialComDlg::DrawCursorInfo() {
	int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;

	m_MemDC.SelectObject(&m_PenCursor1);
	int nAvail = 0;
	int AvailIdx[2];
	for (int i = 0; i < m_Cursor.nCursor; i++) {
		if (m_Cursor.iPos[i] >= m_iDrawGraphRangeX[1]) continue;		
		int x = m_Cursor.iPos[i];
		for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
			m_MemDC.MoveTo(x, m_iDrawGraphRangeY[iGraph][0] + 2);
			m_MemDC.LineTo(x, m_iDrawGraphRangeY[iGraph][1] - 2);
		}
		if (nAvail < 2) {
			AvailIdx[nAvail] = i;
			if (++nAvail == 2) { m_MemDC.SelectObject(&m_PenCursor); }
		}		
	}
	m_MemDC.SelectObject(&m_FontLabel);
	CString str, s;

	int iHistoryIndex[MAX_CURSOR] = { 0, };
	BOOL bHistoryIndexValid[MAX_CURSOR] = { 0, };

	for (int iCursor = 0; iCursor < m_Cursor.nCursor; iCursor++) {
		if (m_Cursor.iPos[iCursor] >= m_iDrawGraphRangeX[1]) continue;
		int iSelectIndex = CalculateIndex(m_Cursor.iPos[iCursor]) + iStartIndex;
		iHistoryIndex[iCursor] = iSelectIndex;
		if (iSelectIndex < 0 || iSelectIndex > m_iHistoryIndex) continue;
		bHistoryIndexValid[iCursor] = TRUE;
		for (int iGraph = 0; iGraph < m_iGraphCount; iGraph++) {
			BOOL bConti = TRUE;
			int nSelect = 0;
			m_iIndicator = 0;

			if (m_bShowProbeValue || m_bShowProbeDataID) {
				// Math Probe
				for (int iData = 0; iData < m_iMaxMathSize && bConti; iData++) {
					if (m_MathData[iData].bValid && m_MathData[iData].bDisplayGraph[iGraph] && m_iIndicator < N_MAX_INDICATOR) {

						// determine digit				
						double fValue = m_HistoryData[iSelectIndex].fMathValue[iData];
						str = "";
						if (m_bShowProbeValue) {
							str.Format("%s", (LPCTSTR)Double2Str(fValue));
						}
						m_Indicator[m_iIndicator].val = fValue;
						if (m_bShowProbeDataID) {
							s.Format("M%d ", iData + 1);
							str = s + str;
						}

						int iMaxX = m_iDrawGraphRangeX[1] - strlen(str) * W_LABEL_TEXT;
						int r, g, b;
						switch (m_iSelTextColorMode) {
						case 0:
						case 4:
							r = m_MathData[iData].iColor[0];
							g = m_MathData[iData].iColor[1];
							b = m_MathData[iData].iColor[2];
							break;
						case 1:
							r = m_iSelTextColor[0];
							g = m_iSelTextColor[1];
							b = m_iSelTextColor[2];
							break;
						case 2:
							r = m_iBackgroundColor[0][0];
							g = m_iBackgroundColor[0][1];
							b = m_iBackgroundColor[0][2];
							break;
						case 3:
							r = m_iTextColor[0];
							g = m_iTextColor[1];
							b = m_iTextColor[2];
							break;
						}
						if (m_iSelTextColorMode == 4)
							m_Indicator[m_iIndicator].text = InvColor(r, g, b);
						else
							m_Indicator[m_iIndicator].text = RGB(r, g, b);

						switch (m_iSelBkColorMode) {
						case 0:
							m_Indicator[m_iIndicator].bk = RGB(m_MathData[iData].iColor[0], m_MathData[iData].iColor[1], m_MathData[iData].iColor[2]);
							break;
						case 1:
							m_Indicator[m_iIndicator].bk = RGB(m_iSelBkColor[0], m_iSelBkColor[1], m_iSelBkColor[2]);
							break;
						case 2:
							m_Indicator[m_iIndicator].bk = RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]);
							break;
						case 3:
							if (m_iSelTextColorMode != 4)
								m_Indicator[m_iIndicator].bk = InvColor(r, g, b);	//m_MemDC.SetBkColor(InvColor(r, g, b));
							else
								m_Indicator[m_iIndicator].bk = RGB(r, g, b);
							break;
						}

						// Position Check
						m_Indicator[m_iIndicator].pt = CalculateDrawPointY(iGraph, m_HistoryData[iSelectIndex].fMathValue[iData]);
						m_Indicator[m_iIndicator].pt.x = m_Cursor.iPos[iCursor];
						m_Indicator[m_iIndicator].str.Format("%s", (LPCTSTR)str);
						m_iIndicator++;
					}
				}

				for (int iData = 0; iData < m_iMaxDataSize && bConti; iData++) {
					if (m_bDraw_GraphData[iGraph][iData]) {
						// determine digit				
						double fValue = m_HistoryData[iSelectIndex].fDataValue[iData];
						str = "";
						if (m_bShowProbeValue) {
							str.Format("%s", (LPCTSTR)Double2Str(fValue));
						}

						m_Indicator[m_iIndicator].val = fValue;
						if (m_bShowProbeDataID) {
							s.Format("D%d ", iData + 1);
							str = s + str;
						}

						int iMaxX = m_iDrawGraphRangeX[1] - strlen(str) * W_LABEL_TEXT;
						int r, g, b;
						switch (m_iSelTextColorMode) {
						case 0:
						case 4:
							r = m_iPenColor[iData][0];
							g = m_iPenColor[iData][1];
							b = m_iPenColor[iData][2];
							break;
						case 1:
							r = m_iSelTextColor[0];
							g = m_iSelTextColor[1];
							b = m_iSelTextColor[2];
							break;
						case 2:
							r = m_iBackgroundColor[0][0];
							g = m_iBackgroundColor[0][1];
							b = m_iBackgroundColor[0][2];
							break;
						case 3:
							r = m_iTextColor[0];
							g = m_iTextColor[1];
							b = m_iTextColor[2];
							break;
						}
						if (m_iSelTextColorMode == 4)
							m_Indicator[m_iIndicator].text = InvColor(r, g, b);			//m_MemDC.SetTextColor(InvColor(r, g, b));
						else
							m_Indicator[m_iIndicator].text = RGB(r, g, b);

						switch (m_iSelBkColorMode) {
						case 0:
							m_Indicator[m_iIndicator].bk = RGB(m_iPenColor[iData][0], m_iPenColor[iData][1], m_iPenColor[iData][2]);
							break;
						case 1:
							m_Indicator[m_iIndicator].bk = RGB(m_iSelBkColor[0], m_iSelBkColor[1], m_iSelBkColor[2]);
							break;
						case 2:
							m_Indicator[m_iIndicator].bk = RGB(m_iBackgroundColor[0][0], m_iBackgroundColor[0][1], m_iBackgroundColor[0][2]);
							break;
						case 3:
							if (m_iSelTextColorMode != 4)
								m_Indicator[m_iIndicator].bk = InvColor(r, g, b);	//m_MemDC.SetBkColor(InvColor(r, g, b));
							else
								m_Indicator[m_iIndicator].bk = RGB(r, g, b);
							break;
						}

						// Position Check
						m_Indicator[m_iIndicator].pt = CalculateDrawPointY(iGraph, (float)m_HistoryData[iSelectIndex].fDataValue[iData]);
						m_Indicator[m_iIndicator].pt.x = m_Cursor.iPos[iCursor];
						m_Indicator[m_iIndicator].str.Format("%s", (LPCTSTR)str);
						m_iIndicator++;
					}
				}

				if (m_iIndicator > 0) {
					DrawIndicator(iGraph);
				}
			}
		}
	}
	
	if (nAvail >= 2) {
		str.Format("N: %d (%d~%d)", nAvail, AvailIdx[0], AvailIdx[1]);
		SetDlgItemText(IDC_Temp, str);

		if (bHistoryIndexValid[AvailIdx[0]] && bHistoryIndexValid[AvailIdx[1]]) {
			int idx0 = iHistoryIndex[AvailIdx[0]];
			int idx1 = iHistoryIndex[AvailIdx[1]];

			if (idx0 > idx1) {
				int t = idx1;
				idx1 = idx0;
				idx0 = t;
			}

			long Time = m_HistoryData[idx1].iRcvTime - m_HistoryData[idx0].iRcvTime;
			long Time_ms = m_HistoryData[idx1].iRcvTime_ms - m_HistoryData[idx0].iRcvTime_ms;

			if (Time_ms < 0) {
				Time--;
				Time_ms += 1000;
			}

			str.Format("%d.%03ds(%d)", Time, Time_ms, idx1 - idx0);
			m_MemDC.SetBkColor(RGB(255, 255, 255));
			m_MemDC.SetTextColor(RGB(0, 0, 0));

			int px0 = m_Cursor.iPos[AvailIdx[0]];
			int px1 = m_Cursor.iPos[AvailIdx[1]];
			if (px0 > px1) {
				int temp = px0;
				px0 = px1;
				px1 = temp;
			}
			
			int iTextPosX0 = (px0 + px1) / 2 - W_LABEL_TEXT * (strlen(str) / 2 + 1);
			int iTextPosX1 = (px0 + px1) / 2 + W_LABEL_TEXT * (strlen(str) / 2 + 1);

			if (iTextPosX0 < m_iDrawGraphRangeX[0] + 1)  iTextPosX0 = m_iDrawGraphRangeX[0] + 1;
			if (iTextPosX1 > m_iDrawGraphRangeX[1] - 1)  iTextPosX0 = m_iDrawGraphRangeX[1] - W_LABEL_TEXT * strlen(str) - 1;
			
			int posY = m_iDrawGraphRangeY[m_iGraphCount - 1][1] - 4 - H_LABEL_TEXT / 2;
			m_MemDC.SelectObject(&m_PenBlack);
			m_MemDC.MoveTo(px0, posY);
			m_MemDC.LineTo(px0 + 5, posY + 1);
			m_MemDC.LineTo(px0 + 5, posY - 1);
			m_MemDC.LineTo(px0, posY);

			m_MemDC.LineTo(px1, posY);
			m_MemDC.LineTo(px1 - 5, posY + 1);
			m_MemDC.LineTo(px1 - 5, posY - 1);
			m_MemDC.LineTo(px1, posY);

			m_MemDC.TextOutA(iTextPosX0, posY - H_LABEL_TEXT / 2, str);			
		}
	}
}


void CSerialComDlg::OnBnClickedAddcursor(){
	if (m_iProgramMode == DATA_ACQUISITION_MODE) return;

	if (m_Cursor.nCursor < MAX_CURSOR - 1) {
		// 이전 data와 같은 위치에 있다면 추가되지 않음
		BOOL bAddCursor = TRUE;
		int iPos = m_iDrawGraphMidX;

		for (int i = 0; i < m_Cursor.nCursor; i++) {
			if (iPos == m_Cursor.iPos[i]) bAddCursor = FALSE;
		}

		if (bAddCursor) {
			m_Cursor.AddCursor(iPos);
			m_bReqGraphUpdate = TRUE;
		}
	}
}

HBRUSH CSerialComDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_DataSetting){
		//글자색상설정		
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);		
	}
	else if (pWnd->GetDlgCtrlID() == IDC_DataSetting2) {
		//글자색상설정		
		pDC->SetTextColor(RGB(0, 0, 255));
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}

CString CSerialComDlg::Dbl2Str(double fValue) {
	return Double2Str(fValue);
}


void CSerialComDlg::OnBnClickedComportlist() {
	OnSetfocusSerialPort();
	OnSetfocusSerialPort2();
	OnSetfocusSerialPort3();
	CPortListDlg dlg;
	dlg.pMain = this;
	dlg.DoModal();
}


void CSerialComDlg::OnBnClickedCapture() {
	CWnd* pWndDesktop = GetDesktopWindow();
	CWindowDC ScrDC(pWndDesktop);
	CClientDC dc(this);

	CImage Image;
	CRect Rect;
	GetClientRect(&Rect);
	int cx = m_iWndSizeX;
	int cy = m_iWndSizeY;
	GetWindowRect(&Rect);

	Image.Create(cx, cy, ScrDC.GetDeviceCaps(BITSPIXEL));

	CDC* pDC = CDC::FromHandle(Image.GetDC());
	pDC->BitBlt(0, 0, cx, cy, &dc, 0, 0, SRCCOPY);

	Image.ReleaseDC();

	SYSTEMTIME time;
	GetLocalTime(&time);
	CString str;

	str.Format("Capture_%02d%02d%02d_%02d%02d%02d.jpg", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	if (m_iSaveMode == 1 && m_strSavePath[1] != "") {
        CString strTemp = str;
		str.Format("%s\\%s", (LPCTSTR)m_strSavePath[1], (LPCTSTR)strTemp);
	}
	if (!Image.Save(str, Gdiplus::ImageFormatJPEG)) {
		str.Format("Capture_%02d%02d%02d_%02d%02d%02d.jpg", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		Image.Save(str, Gdiplus::ImageFormatJPEG);
	}
	
}

void CSerialComDlg::LoadDefaultHeaderLog() {	
	//JIN Default값은 수정필요함 현재 주석 처리
	CString strAddress, strItem, strData, strHeader, str;
    BYTE Buffer[8000] = {"{b}{104}0 (1)(1)(0)(0)(255)(0)(0) { MainMCU State : Init }(0) {}1 (1)(1)(0)(0)(255)(1)(0) { MainMCU State : Normal }(0) {}2 (1)(1)(0)(0)(255)(2)(0) { MainMCU State : Warning zone3 }(0) {}3 (1)(1)(0)(0)(255)(3)(0) { MainMCU State : Warning zone2 }(0) {}4 (1)(1)(0)(0)(255)(4)(0) { MainMCU State : Warning zone1 }(0) {}5 (1)(1)(0)(0)(255)(5)(0) { MainMCU State : Protection zone }(0) {}6 (1)(1)(0)(0)(255)(6)(0) { MainMCU State : Dead }(0) {}7 (1)(1)(0)(0)(65280)(0)(0) { Normal Mode }(0) {}8 (1)(1)(0)(0)(65280)(256)(0) { Standby Mode }(0) {}9 (1)(1)(0)(0)(65280)(512)(0) { PowerSaving Mode }(0) {}10 (1)(1)(0)(0)(65280)(768)(0) { System off  Mode }(0) {}11 (1)(1)(0)(0)(65280)(1024)(0) { Power  Mode Max }(0) {}12 (1)(0)(0)(0)(0)(0)(0) {}(0) {}13 (1)(1)(0)(1)(1)(1)(0) { Dead: Magnetic }(0) {}14 (1)(1)(0)(1)(2)(2)(0) { Dead: LiDAR }(0) {}15 (1)(1)(0)(1)(4)(4)(0) { Dead: Navi }(0) {}16 (1)(1)(0)(1)(8)(8)(0) { Dead: STO2 }(0) {}17 (1)(1)(0)(1)(16)(16)(0) { Dead: Bumper }(0) {}18 (1)(1)(0)(1)(32)(32)(0) { Dead: Battery }(0) {}19 (1)(1)(0)(1)(64)(64)(0) { Dead: CAN }(0) {}20 (1)(1)(0)(1)(128)(128)(0) { Dead: Reset }(0) {}21 (1)(1)(0)(1)(256)(256)(0) { Dead: IMU }(0) {}22 (1)(1)(0)(1)(512)(512)(0) { Dead: Charger }(0) {}23 (1)(1)(0)(1)(1024)(1024)(0) { Dead: ExBrake }(0) {}24 (1)(1)(0)(1)(2048)(2048)(0) { Dead: STO }(0) {}25 (1)(1)(0)(1)(4096)(4096)(0) { Dead: Danger OBJ }(0) {}26 (1)(1)(0)(1)(8192)(8192)(0) { Dead: Danger Slope }(0) {}27 (1)(1)(0)(1)(16384)(16384)(0) { Dead: TOF }(0) {}28 (1)(0)(0)(0)(0)(0)(0) {}(0) {}29 (1)(1)(0)(2)(1)(1)(0) { L Wheel state : Servo On }(0) {}30 (1)(1)(0)(2)(2)(2)(0) { L Wheel state : Init }(0) {}31 (1)(1)(0)(2)(4)(4)(0) { L Wheel state : Force Stop }(0) {}32 (1)(1)(0)(2)(8)(8)(0) { L Wheel state : STO Stop }(0) {}33 (1)(1)(0)(2)(16)(16)(0) { L Wheel state : Feedback Error }(0) {}34 (1)(1)(0)(2)(32)(32)(0) { L Wheel state : Over Current }(0) {}35 (1)(1)(0)(2)(64)(64)(0) { L Wheel state : Abnormal Current }(0) {}36 (1)(1)(0)(2)(128)(128)(0) { L Wheel state : Abnormal Velocity }(0) {}37 (1)(1)(0)(2)(256)(256)(0) { L Wheel state : Motor Stuck }(0) {}38 (1)(1)(0)(2)(512)(512)(0) { L Wheel state : Encoder Diff Error }(0) {}39 (1)(1)(0)(2)(1024)(1024)(0) { L Wheel state : Emergency Switch }(0) {}40 (1)(1)(0)(2)(2048)(2048)(0) { L Wheel state : Over Temperature }(0) {}41 (1)(1)(0)(2)(4096)(4096)(0) { L Wheel state : Velocity Diff Error }(0) {}42 (1)(1)(0)(3)(1)(1)(0) { R Wheel state : Servo On }(0) {}43 (1)(1)(0)(3)(2)(2)(0) { R Wheel state : Init }(0) {}44 (1)(1)(0)(3)(4)(4)(0) { R Wheel state : Force Stop }(0) {}45 (1)(1)(0)(3)(8)(8)(0) { R Wheel state : STO Stop }(0) {}46 (1)(1)(0)(3)(16)(16)(0) { R Wheel state : Feedback Error }(0) {}47 (1)(1)(0)(3)(32)(32)(0) { R Wheel state : Over Current }(0) {}48 (1)(1)(0)(3)(64)(64)(0) { R Wheel state : Abnormal Current }(0) {}49 (1)(1)(0)(3)(128)(128)(0) { R Wheel state : Abnormal Velocity }(0) {}50 (1)(1)(0)(3)(256)(256)(0) { R Wheel state : Motor Stuck }(0) {}51 (1)(1)(0)(3)(512)(512)(0) { R Wheel state : Encoder Diff Error }(0) {}52 (1)(1)(0)(3)(1024)(1024)(0) { R Wheel state : Emergency Switch }(0) {}53 (1)(1)(0)(3)(2048)(2048)(0) { R Wheel state : Over Temperature }(0) {}54 (1)(1)(0)(3)(4096)(4096)(0) { R Wheel state : Velocity Diff Error }(0) {}55 (1)(0)(0)(0)(0)(0)(0) {}(0) {}56 (1)(1)(0)(4)(16384)(16384)(0) { Front LiDAR Protection }(0) {}57 (1)(1)(0)(4)(8192)(8192)(0) { Front LiDAR Warning 1 }(0) {}58 (1)(1)(0)(4)(4096)(4096)(0) { Front LiDAR Warning 2 }(0) {}59 (1)(1)(0)(4)(2048)(2048)(0) { Front LiDAR Warning 3 }(0) {}60 (1)(1)(0)(4)(1024)(1024)(0) { Back LiDAR Protection }(0) {}61 (1)(1)(0)(4)(512)(512)(0) { Back LiDAR Warning 1 }(0) {}62 (1)(1)(0)(4)(256)(256)(0) { Back LiDAR Warning 2 }(0) {}63 (1)(1)(0)(4)(128)(128)(0) { Back LiDAR Warning 3 }(0) {}64 (1)(1)(0)(4)(64)(64)(0) { Front TOF Protection }(0) {}65 (1)(1)(0)(4)(32)(32)(0) { Front TOF Warning 1 }(0) {}66 (1)(1)(0)(4)(16)(16)(0) { Front TOF Warning 2 }(0) {}67 (1)(1)(0)(4)(8)(8)(0) { Front TOF Warning 3 }(0) {}68 (1)(1)(0)(4)(4)(4)(0) { Back USS Protection }(0) {}69 (1)(1)(0)(4)(3)(1)(0) { Back USS Warning 1 }(0) {}70 (1)(1)(0)(4)(3)(2)(0) { Back USS Warning 2 }(0) {}71 (1)(1)(0)(4)(3)(3)(0) { Back USS Warning 3 }(0) {}72 (1)(0)(0)(0)(0)(0)(0) {}(0) {}73 (1)(1)(0)(5)(1)(1)(0) { Brake On State }(0) {}74 (1)(1)(0)(5)(2)(2)(0) { Emergency STO2 Active state }(0) {}75 (1)(1)(0)(5)(4)(4)(0) { LiDAR STO1 Active state }(0) {}76 (1)(1)(0)(5)(8)(8)(0) { Start Switch On state }(0) {}77 (1)(1)(0)(5)(16)(16)(0) { Bumper0 On }(0) {}78 (1)(1)(0)(5)(32)(32)(0) { Bumper1 On }(0) {}79 (1)(1)(0)(5)(64)(64)(0) { Docking0 On }(0) {}80 (1)(1)(0)(5)(128)(128)(0) { Docking1 On }(0) {}81 (1)(1)(0)(5)(256)(256)(0) { Brake Release On }(0) {}82 (1)(1)(0)(5)(512)(512)(0) { Fan0 Fail }(0) {}83 (1)(1)(0)(5)(1024)(1024)(0) { Fan1 Fail }(0) {}84 (1)(1)(0)(5)(2048)(2048)(0) { Magnetic N Pole Detect(L) }(0) {}85 (1)(1)(0)(5)(4096)(4096)(0) { Magnetic N Pole Detect(R) }(0) {}86 (1)(1)(0)(5)(8192)(8192)(0) { Magnetic S Pole Detect(L) }(0) {}87 (1)(1)(0)(5)(16384)(16384)(0) { Magnetic S Pole Detect(R) }(0) {}88 (1)(0)(0)(0)(0)(0)(0) {}(0) {}89 (1)(2)(0)(6)(0)(0)(0) { Left Wheel - Navi Target Velocity[mm / s] : }(0) {}90 (1)(2)(0)(7)(0)(0)(0) { Left Wheel - MCU Target Velocity[mm / s] : }(0) {}91 (1)(2)(0)(8)(0)(0)(0) { Left Wheel - Current Velocity[mm / s] : }(0) {}92 (1)(0)(0)(0)(0)(0)(0) {}(0) {}93 (1)(3)(0)(9)(255)(200)(0) { Motor Drv - Left Current[mA] : }(0) {}94 (1)(3)(0)(9)(65280)(200)(0) { Motor Drv - Right Current[mA] : }(0) {}95 (1)(0)(0)(0)(0)(0)(0) {}(0) {}96 (1)(2)(0)(10)(0)(0)(0) { Right Wheel - Navi Target Velocity[mm / s] : }(0) {}97 (1)(2)(0)(11)(0)(0)(0) { Right Wheel - MCU Target Velocity[mm / s] : }(0) {}98 (1)(2)(0)(12)(0)(0)(0) { Right Wheel - Current Velocity[mm / s] : }(0) {}99 (1)(0)(0)(0)(0)(0)(0) {}(0) {}100 (1)(2)(0)(13)(0)(0)(0) { IMU - Yaw Angle[0.1deg] : }(0) {}101 (1)(2)(0)(14)(0)(0)(0) { Battery Voltage[mV] : }(0) {}102 (1)(0)(0)(0)(0)(0)(0) {}(0) {}103 (1)(2)(0)(15)(0)(0)(0) { Lift Voltagex10 + BrakeStatus : }(0) {}"};	    

    str.Format("%s", Buffer);	m_iBufferIndex = 0, m_iBufferSize = str.GetLength();	BOOL bConti = TRUE;
	GetHeaderLogFromBuffer(Buffer, &bConti);

	m_iBufferIndex = 0, m_iBufferSize = str.GetLength();
	GetNextValue(Buffer, &strHeader);
	if (strHeader.GetLength() < 5) {
		char s[10];
		strcpy_s(s, (LPCTSTR)strHeader);
		for (int i = 0; i < strHeader.GetLength(); i++) {
			if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' || s[i] <= 'Z')) {
				str.Format("%c", s[i]);
				if (strData.Find(str) == -1) {
                    CString strTemp = strData;
					strData.Format("%s%s", (LPCTSTR)strTemp, (LPCTSTR)str);
					SetRegRoot_RegistryData(strAddress, strItem, strData);
				}				
				break;
			}
		}
	}
}


int CSerialComDlg::GetTextLogCount(char header) {
	CString strAddress, strItem, strValue;
	strAddress.Format("Log\\TextLog\\%c_0x%X", header, header);
	strItem.Format("nCount");
	strValue = GetReg_RegistryData(strAddress, strItem);
	int iTextLogCount = max(0, atoi(strValue));
	if (iTextLogCount > MAX_HEADER_LOG_COUNT) { iTextLogCount = 0; }

	return iTextLogCount;
}

void CSerialComDlg::GetHeaderLogFromRegistry(char header, int num, CTextLogSetting* data) {
	CString strAddress, strItem, strValue;
	
	strAddress.Format("Log\\TextLog\\%c_0x%X\\Item%d", header, header, num);
	strValue = GetRegRoot_RegistryData(strAddress, "Use");
	if (strValue == "Use" || atoi(strValue) == 1) {  data->use = 1; } 
	else {  data->use = 0;  }
	strValue = GetRegRoot_RegistryData(strAddress, "Condition"); data->item_type = atoi(strValue);
	strValue = GetRegRoot_RegistryData(strAddress, "DataType"); data->data_type = atoi(strValue);
	strValue.Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, "DataID"));
	if (strValue.GetAt(0) == 'D') {
		data->data_id = 0;		
		for (int i = 0; i < strValue.GetLength(); i++) {
			char ch = strValue.GetAt(i);
			if (ch >= '0' && ch <= '9') {
				data->data_id = data->data_id * 10 + ch - '0';
			}
		}			
	}
	else {
		data->data_id = atoi(strValue);
	}
	
	strValue = GetRegRoot_RegistryData(strAddress, "Masking");  data->mask_val = atoi(strValue);
	strValue = GetRegRoot_RegistryData(strAddress, "Value");
	switch  (data->item_type) {
	case eLogCondition_Greater:  
	case eLogCondition_GreaterEqual:
	case eLogCondition_Less:
	case eLogCondition_LessEqual:
		data->ref_fVal = atof(strValue);
		break;

	default :
		data->ref_iVal = atoi(strValue);
		break;
	}

	strValue = GetRegRoot_RegistryData(strAddress, "Show1");  data->show1 = atoi(strValue);
	data->strComment1.Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, "Comment1"));
	strValue = GetRegRoot_RegistryData(strAddress, "Show2");  data->show2 = atoi(strValue);
	data->strComment2.Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddress, "Comment2"));
    data->dp = atoi(GetRegRoot_RegistryData(strAddress, "Digit"));
    data->divide = atoi(GetRegRoot_RegistryData(strAddress, "Divide"));
}


void CSerialComDlg::GetHeaderLogFromBuffer(BYTE* pBuffer, BOOL* pConti) {
	char s[5];
	CString strValue, strAddress, strItem; 
	BOOL bConti = *pConti;

	if (bConti && GetNextValue(pBuffer, &strValue) > 0) {
		strcpy_s(s, (LPCTSTR)strValue);
	}

	// Get Item Count
	int iItemCount = 0;
	if (bConti && GetNextValue(pBuffer, &strValue) > 0)
	{
		strAddress.Format("Log\\TextLog\\%c_0x%X", s[0], s[0]);
		strItem = "nCount";
		SetRegRoot_RegistryData(strAddress, strItem, strValue);
		iItemCount = atoi(strValue);
	}
	else bConti = FALSE;

	for (int i = 0; i < iItemCount; i++)
	{
		strAddress.Format("Log\\TextLog\\%c_0x%X\\Item%d", s[0], s[0], i);

		// Use
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			if (atoi(strValue)) strValue = "Use"; else strValue = "Unused";
			SetRegRoot_RegistryData(strAddress, "Use", strValue);
		}
		else bConti = FALSE;

		// Condition
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			int iCondition = atoi(strValue);
			switch (iCondition)
			{
			case eLogCondition_Blank: strValue = "0"; break;
			case eLogCondition_MatchMask: strValue = "1"; break;
			case eLogCondition_ShowValue: strValue = "2";  break;
			case eLogCondition_MaskedValue: strValue = "3";  break;
			}
			SetRegRoot_RegistryData(strAddress, "Condition", strValue);
		}
		else bConti = FALSE;
        
        // DataType
        if (bConti && GetNextValue(pBuffer, &strValue) > 0)
        {
            SetRegRoot_RegistryData(strAddress, "DataType", strValue);
        }
        else bConti = FALSE;

        // DataID
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			SetRegRoot_RegistryData(strAddress, "DataID", strValue);
		}
		else bConti = FALSE;

		// Masking Value
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			SetRegRoot_RegistryData(strAddress, "Masking", strValue);
		}
		else bConti = FALSE;

		// Reference Value
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			SetRegRoot_RegistryData(strAddress, "Value", strValue);
		}
		else bConti = FALSE;

		// Show1
		if (bConti && GetNextValue(pBuffer, &strValue) > 0) {
			SetRegRoot_RegistryData(strAddress, "Show1", strValue);
		}
		else bConti = FALSE;

		// Comment1
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			SetRegRoot_RegistryData(strAddress, "Comment1", strValue);
		}
		else bConti = FALSE;

		// Show2
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			SetRegRoot_RegistryData(strAddress, "Show2", strValue);
		}
		else bConti = FALSE;

		// Comment2
		if (bConti && GetNextValue(pBuffer, &strValue) > 0)
		{
			SetRegRoot_RegistryData(strAddress, "Comment2", strValue);
		}
		else bConti = FALSE;

        // Digit
        if (bConti && GetNextValue(pBuffer, &strValue) > 0)
        {
            SetRegRoot_RegistryData(strAddress, "Digit", strValue);
        }
        else bConti = FALSE;

        // Divide
        if (bConti && GetNextValue(pBuffer, &strValue) > 0)
        {
            SetRegRoot_RegistryData(strAddress, "Divide", strValue);
        }
        else bConti = FALSE;
	}
	*pConti = bConti;
}


int CSerialComDlg::GetNextValue(BYTE* pBuffer, CString* pStrValue)
{
	BYTE aByte;
	int index = 0;
	int iDataType = -1;
	int iSeq = 0;
	int bConti = TRUE;
	char array[101];

	for (int i = m_iBufferIndex; i < m_iBufferSize && bConti; i++)
	{
		if (i >= m_iBufferSize)
		{
			iDataType = -3; break;
		}
		aByte = pBuffer[i];

		switch (iSeq)
		{
		case 0:
			if (aByte == '(')
			{
				iSeq = 100; iDataType = 1;
			}
			else if (aByte == '{')
			{
				iSeq = 200; iDataType = 2;
			}
			else if (aByte == '[')
			{
				iSeq = 300; iDataType = 3;
			}
			break;

		case 100:
			if (aByte == ')')
			{
				array[index] = 0; bConti = FALSE; m_iBufferIndex = i + 1;
			}
			else
			{
				array[index] = aByte;
				if (++index >= 100)
				{
					bConti = FALSE;  iDataType = -2;
				}
			}
			break;

		case 200:
			if (aByte == '}')
			{
				array[index] = 0; bConti = FALSE; m_iBufferIndex = i + 1;
			}
			else
			{
				array[index] = aByte;
				if (++index >= 100)
				{
					bConti = FALSE;  iDataType = -2;
				}
			}
			break;

		case 300:
			if (aByte == ']')
			{
				array[index] = 0; bConti = FALSE; m_iBufferIndex = i + 1;
			}
			else
			{
				array[index] = aByte;
				if (++index >= 100)
				{
					bConti = FALSE;  iDataType = -2;
				}
			}
			break;

		}
	}
	if (iDataType > 0)
	{
		array[index] = 0;
		pStrValue->Format("%s", array);
	}
	return iDataType;
}

void CSerialComDlg::SetLogFilterSize_H(CPoint point) {
	if (!m_bShowDataControl) return;

	static int iCount = 0; 
	if (m_AdjustLogFilter_H.isPressed) {
		if (iCount++ > 20) {
			CRect rect(m_iRangeLogFilterWidth - 50, min(50, m_iRangeLogFilterVertical), m_iRangeLogFilterWidth + 50, m_iWndSizeY);
			InvalidateRect(&rect);
			iCount = 0;
		}
	}
	else {
		iCount = 0;
	}

	int y0 = m_iRangeLogFilterVertical;

	if (point.x  < MIN_RANGE_LOGFILTER_H - 2 || point.x > MAX_RANGE_LOGFILTER_H) {
		m_AdjustLogFilter_H.isPressed = FALSE;
	}
	if (point.y  < min(50, m_iRangeLogFilterVertical)) {
		m_AdjustLogFilter_H.isPressed = FALSE;
	}
	m_iRangeLogFilterWidth = min(MAX_RANGE_LOGFILTER_H - 3, max(MIN_RANGE_LOGFILTER_H, point.x - 2));
	m_iDrawGraphRangeX[0] = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H + MIN_RANGE_GRAPH_H;
	m_iPreDrawGraphRangeX = m_iDrawGraphRangeX[0];
	
    if (m_bShowFilterLogControl) {
        GetDlgItem(IDC_FilterMessage)->MoveWindow(2, m_iRangeLogFilterVertical, m_iRangeLogFilterWidth, max(2, m_iRangeLogFilterRatio[0]) - m_iRangeLogFilterVertical - 1, TRUE);
        GetDlgItem(IDC_LogList)->MoveWindow(2, max(2, m_iRangeLogFilterRatio[1] + 1), m_iRangeLogFilterWidth, m_iWndSizeY - max(2, m_iRangeLogFilterRatio[1] + 1), TRUE);
    }
    else {
        GetDlgItem(IDC_LogList)->MoveWindow(2, m_iRangeLogFilterVertical, m_iRangeLogFilterWidth, m_iWndSizeY - m_iRangeLogFilterVertical + 1, TRUE);
    }
	OnSize(0, m_iWndSizeX, m_iWndSizeY);
	CRect rect(1, m_iPos_DataStart - 1, m_iDrawGraphRangeX[0], m_iWndSizeY + 1);
	InvalidateRect(&rect);
	DrawInit();

	if (!m_AdjustLogFilter_H.isPressed) {		
		ControlView();
	}
	else {
		if (m_bRefreshScreen) {
			m_bRefreshScreen = FALSE;
			ControlView();
		}
	}
	m_bReqGraphUpdate = TRUE;
}


void CSerialComDlg::SetLogFilterSize_V(CPoint point) {
	if (!m_bShowDataControl) return;
	int x0 = 2;
	int y0 = m_iRangeLogFilterVertical;

	if (point.x > 1 && point.x <= m_iRangeLogFilterWidth) {
		if (point.y > y0 + 30 && point.y < m_iWndSizeY - 30) {
			m_iRatioLogFilter = (point.y - y0) * 100 / (m_iWndSizeY - y0);
			m_iRangeLogFilterRatio[0] = min(max(y0 + 1, point.y - 1), m_iWndSizeY - 1);
			m_iRangeLogFilterRatio[1] = min(max(y0 + 1, point.y + 1), m_iWndSizeY - 1);
		}
		else if (point.y > y0 + 20 && point.y < m_iWndSizeY - 20) {
			m_AdjustLogFilter_V.isPressed = FALSE;
		}
	}
	else {
		m_AdjustLogFilter_V.isPressed = FALSE;
	}

    if (m_bShowFilterLogControl) {
        GetDlgItem(IDC_FilterMessage)->MoveWindow(2, m_iRangeLogFilterVertical, m_iRangeLogFilterWidth, max(2, m_iRangeLogFilterRatio[0]) - m_iRangeLogFilterVertical - 1, TRUE);
        GetDlgItem(IDC_LogList)->MoveWindow(2, max(2, m_iRangeLogFilterRatio[1] + 1), m_iRangeLogFilterWidth, m_iWndSizeY - max(2, m_iRangeLogFilterRatio[1] + 1), TRUE);
    }
    else {
        GetDlgItem(IDC_LogList)->MoveWindow(2, m_iRangeLogFilterVertical, m_iRangeLogFilterWidth, m_iWndSizeY - m_iRangeLogFilterVertical + 1, TRUE);
    }    
}

void CSerialComDlg::SetDataWndSize_V(CPoint point) {
	if (!m_bShowDataControl) return;
	return;

	int iPreSize = m_iShowDataControl;
	int iSize = (point.y - DRAW_DATA_CONTROL_Y0) / DRAW_CONTROL_DY;
	if (iSize < 0 || point.x > m_iRangeLogFilterWidth) {
		m_AdjustDataWnd_V.isPressed = FALSE;	
		m_CursorMode = -1;
		SetDefaultCursor();
	}
	else if (iSize > MAX_DATACONTROL_SIZE + 1) {
		m_AdjustDataWnd_V.isPressed = FALSE;
		if (m_iShowMathControl == 0) {
			m_bShowMathControl = TRUE;
			m_AdjustMathWnd_V.isPressed = TRUE;
			CheckDlgButton(IDC_Display2, TRUE);
			OnBnClickedDisplay2();
			SetMathWndSize_V(point);
		}
		else {
			m_CursorMode = -1;
			SetDefaultCursor();
		}
	}
	iSize = max(0, min(MAX_DATACONTROL_SIZE, iSize));
	
	if (iPreSize != iSize) {
		m_iShowDataControl = iSize;
		m_iRangeDataControlVertical = m_iShowDataControl * DRAW_CONTROL_DY + DRAW_DATA_CONTROL_Y0;
		if (iSize > 0) {
			CheckDlgButton(IDC_Display0, TRUE);
		}
		else {
			CheckDlgButton(IDC_Display0, FALSE);
		}
	}

	m_iShowDataControl = iSize;
	m_iRangeDataControlVertical = m_iShowDataControl * DRAW_CONTROL_DY + DRAW_DATA_CONTROL_Y0;

	if (m_AdjustDataWnd_V.isPressed == FALSE) {
		DrawInit();
	}	
	else {
		if (m_bRefreshScreen) {
			m_bRefreshScreen = FALSE;
		}
	}
	ControlView();
}

void CSerialComDlg::SetMathWndSize_V(CPoint point) {
    if (!m_bShowMathControl) return;

    int iPreSize = m_iShowMathControl;
    int iSize = (point.y - m_iPos_MathStart - 33) / DRAW_CONTROL_DY;
    if (iSize > MAX_MATHCONTROL_SIZE || point.x > m_iRangeLogFilterWidth) {
        m_AdjustMathWnd_V.isPressed = FALSE;        
		m_CursorMode = -1;
		SetDefaultCursor();
    }
	else if (iSize < -1) {
		m_AdjustMathWnd_V.isPressed = FALSE;
		if (m_iShowDataControl > 0) {
			m_AdjustDataWnd_V.isPressed = TRUE;
		}
		else {
			m_CursorMode = -1;
			SetDefaultCursor();
		}
	}
	else if (iSize <= 0) {
		iSize = 1;
	}

	//if (m_CursorMode == -1) {
	//	HCURSOR hDefaultCursor = ::LoadCursor(NULL, IDC_ARROW);
	//	::SetCursor(hDefaultCursor);
	//}

	iSize = max(0, min(MAX_MATHCONTROL_SIZE, iSize));

    if (iPreSize != iSize) {
        m_iShowMathControl = iSize;
        m_iPos_MathEnd = m_iPos_MathStart + m_iShowMathControl * DRAW_CONTROL_DY + 33;
        if (iSize > 0 && IsDlgButtonChecked(IDC_Display2) == FALSE) {
            CheckDlgButton(IDC_Display2, TRUE);
            OnBnClickedDisplay2();
        }
        else if (iSize == 0 && IsDlgButtonChecked(IDC_Display2)) {
            CheckDlgButton(IDC_Display2, FALSE);
            OnBnClickedDisplay2();
        }
        ControlView();
    }
        
    if (m_AdjustMathWnd_V.isPressed == FALSE) {
        DrawInit();
    }
    else {
        if (m_bRefreshScreen) {
            m_bRefreshScreen = FALSE;
        }
    }
}

void CSerialComDlg::SetDefaultCursor() {
	HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW); // 기본 화살표 커서
	SetCursor(hCursor);
}

void CSerialComDlg::ShowDataMathControl() {
    CString str, pre_str;
	static int preDataOffset = -1;
	static int preDataSize = -1;
	static int preMathOffset = -1;
	static int preMathSize = -1;

    if (m_bShowDataControl || m_bShowMathControl) {
        CheckDataOffset();
    }

	BOOL bChangeData = FALSE;
	BOOL bChangeMath = FALSE;
		
	if (preDataOffset != m_iControlDataOffset) {
		preDataOffset = m_iControlDataOffset;
		m_bReqUpdateControlDraw = TRUE;
		bChangeData = TRUE;
	}

	if (preDataSize != m_iShowDataControl) {
		preDataSize = m_iShowDataControl;
		m_bReqUpdateControlDraw = TRUE;
		bChangeData = TRUE;
	}
	
	if (preMathOffset != m_iControlMathOffset) {
		preMathOffset = m_iControlMathOffset;
		m_bReqUpdateControlDraw = TRUE;
		bChangeMath = TRUE;
	}
	
	if (preMathSize != m_iShowMathControl) {
		preMathSize = m_iShowMathControl;
		m_bReqUpdateControlDraw = TRUE;
		bChangeMath = TRUE;
	}	

	int iSize = m_iShowDataControl;
	DrawInit();
	BOOL bDataView;	
	if (m_bShowDataControl) bDataView = TRUE; else bDataView = FALSE;    
	
	if (m_iHistorySelectIndex >= m_iHistoryIndex) { 	m_iHistorySelectIndex = m_iHistoryIndex	- 1; }
	if (m_iHistorySelectIndex < 0) { m_iHistorySelectIndex = 0; }
	for (int i = 0; i < iSize; i++) {        
        int idx = i + m_iControlDataOffset;
        if (10 > idx + 1) {
            str.Format("D%d", idx+1);
        }
        else {
            str.Format("%d", idx+1);
        }
        SetDlgItemText(m_IDC_DataNumber[i], str);
        GetDlgItem(m_IDC_DataNumber[i])->ShowWindow(bDataView);

        str.Format("%s", (LPCTSTR)m_strPinName[idx]);  
        GetDlgItemText(m_IDC_PinName[i], pre_str);
        if (str != pre_str) {
            SetDlgItemText(m_IDC_PinName[i], str);
        }
		GetDlgItem(m_IDC_PinName[i])->ShowWindow(bDataView);

		SetDlgItemText(m_IDC_DataValue[i], Double2Str(m_HistoryData[m_iHistorySelectIndex].fDataValue[idx]));
		GetDlgItem(m_IDC_DataValue[i])->ShowWindow(bDataView);
	}

	for (int i = iSize; i < MAX_DATACONTROL_SIZE; i++) {
		GetDlgItem(m_IDC_DataNumber[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_PinName[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_DataValue[i])->ShowWindow(FALSE);
	}

	//Draw Blank Math Control
	for (int i = 0; i < m_iShowMathControl; i++) {
		int idx = i + m_iControlMathOffset;
		if (idx < 9) {
			str.Format("M%d", idx + 1);
			SetDlgItemText(m_IDC_MathText[i], str);
		}
		else {
			SetDlgItemInt(m_IDC_MathText[i], idx + 1);
		}
		SetDlgItemText(m_IDC_MathProbeName[i], m_MathData[idx].strName);
		SetDlgItemText(m_IDC_MathProbeValue[i], Double2Str(m_HistoryData[m_iHistorySelectIndex].fMathValue[idx]));
	}

	
	if (!bDataView) { iSize = 0; }
	m_iRangeDataControlVertical = DRAW_CONTROL_DY * iSize + DRAW_DATA_CONTROL_Y0;	

	// MathProbe Position
	int DeltaY = DRAW_CONTROL_DY * (m_iShowDataControl - MAX_DATACONTROL_SIZE) + DRAW_CONTROL_START_Y;

	int iMoveControl_H = m_iRangeLogFilterWidth - MIN_RANGE_LOGFILTER_H;
	CClientDC dc(this);
	// 수정 사항
	// dc.Rectangle() ==> BitBlt로 변경 하자  
	// m_bDrawGraph[j], m_bSelectDraw_GraphData, m_MathData[i + m_iControlMathOffset].bSelectGraph[j]

	// Type별 원형 
	// Data Select영역
	// Math Select영역

	// Blank 만들기

	if (m_bFastDraw) {
		if (m_bReqUpdateControlDraw) {
			m_bReqUpdateControlDraw = FALSE;

			CBrush brush(m_bgColor);
			m_MemDC_Select.SelectObject(&brush);
			m_MemDC_Select.Rectangle(0, 0, 180, 1000);

			for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
				m_MemDC_Select.SelectObject(&m_BrushSelectControl_Blank[m_bDrawGraph[j]]);
				m_MemDC_Select.SelectObject(&m_PenSelectControl_Blank[m_bDrawGraph[j]]);
				int x = 20 + (int)(j * 19.7);
				int y = 20;
				// unchecked 
				m_MemDC_Select.Rectangle(x, y, x + 14, y + 14);

				// checked
				y = 120;
				m_MemDC_Select.Rectangle(x, y, x + 14, y + 14);
			}

			for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
				m_MemDC_Select.SelectObject(&m_BrushSelectControl_Select[m_bDrawGraph[j]]);
				m_MemDC_Select.SelectObject(&m_PenSelectControl_Select[m_bDrawGraph[j]]);
				int x = 20 + (int)(j * 19.7) + 2;
				int y = 122;
				m_MemDC_Select.Rectangle(x, y, x + 10, y + 10);
			}

			if (m_bShowDataControl) {
				//Draw Blank Data Control
				for (int i = 0; i < m_iShowDataControl; i++) {
					m_MemDC_Select.BitBlt(20, 220 + DRAW_CONTROL_DY * i, (int)(8 * 19.7) + 15, 15, &m_MemDC_Select, 20, 20, SRCCOPY);
				}

				// Draw Select Data Control
				for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
					for (int i = 0; i < m_iShowDataControl; i++) {
						int idx = i + m_iControlDataOffset;
						if (m_bSelectDraw_GraphData[j][idx]) {
							int x = 20 + (int)(j * 19.7);
							int y = 220 + DRAW_CONTROL_DY * i;
							m_MemDC_Select.BitBlt(x, y, 15, 15, &m_MemDC_Select, x, 120, SRCCOPY);
						}
					}
				}
				dc.BitBlt(270 + iMoveControl_H - 3, DRAW_DATA_CONTROL_Y0 - 1, (int)((MAX_GRAPH_COUNT - 1) * 19.7) + 15, DRAW_CONTROL_DY * (m_iShowDataControl - 1) + 14, &m_MemDC_Select, 20, 220, SRCCOPY);
			}

			if (m_bShowMathControl) {
				//Draw Blank Math Control
				for (int i = 0; i < m_iShowMathControl; i++) {
					m_MemDC_Select.BitBlt(20, 620 + DRAW_CONTROL_DY * i, (int)(8 * 19.7) + 15, 15, &m_MemDC_Select, 20, 20, SRCCOPY);
				}

				// Draw Select Math Control
				for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
					for (int i = 0; i < m_iShowMathControl; i++) {
						if (m_MathData[i + m_iControlMathOffset].bSelectGraph[j]) {
							int x = 20 + (int)(j * 19.7);
							int y = 620 + DRAW_CONTROL_DY * i;
							m_MemDC_Select.BitBlt(x, y, 15, 15, &m_MemDC_Select, x, 120, SRCCOPY);
						}
					}
				}

				dc.BitBlt(270 + iMoveControl_H - 3, 360 + DeltaY - 1, (int)((MAX_GRAPH_COUNT - 1) * 19.7) + 15, DRAW_CONTROL_DY * (m_iShowMathControl - 1) + 15, &m_MemDC_Select, 20, 620, SRCCOPY);
			}			
		}
		else {
			if (m_bShowDataControl) {
				dc.BitBlt(270 + iMoveControl_H - 3, DRAW_DATA_CONTROL_Y0 - 1, (int)((MAX_GRAPH_COUNT - 1) * 19.7) + 15, DRAW_CONTROL_DY * (m_iShowDataControl - 1) + 14, &m_MemDC_Select, 20, 220, SRCCOPY);
			}
			if (m_bShowMathControl) {
				dc.BitBlt(270 + iMoveControl_H - 3, 360 + DeltaY - 1, (int)((MAX_GRAPH_COUNT - 1) * 19.7) + 15, DRAW_CONTROL_DY * (m_iShowMathControl - 1) + 15, &m_MemDC_Select, 20, 620, SRCCOPY);
			}
		}
	}
	else {
		for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
			dc.SelectObject(&m_BrushSelectControl_Blank[m_bDrawGraph[j]]);
			dc.SelectObject(&m_PenSelectControl_Blank[m_bDrawGraph[j]]);
			int x, y;
			for (int i = 0; i < m_iShowDataControl; i++) {
				x = 270 + (int)(j * 19.7) + iMoveControl_H - 3;
				y = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * i - 1;
				dc.Rectangle(x, y, x + 14, y + 14);
			}

			for (int i = 0; i < m_iShowMathControl; i++) {
				int idx = i + m_iControlMathOffset;
				if (idx < 9) {
					str.Format("M%d", idx + 1);
					SetDlgItemText(m_IDC_MathText[i], str);
				}
				else {
					SetDlgItemInt(m_IDC_MathText[i], idx + 1);
				}
				SetDlgItemText(m_IDC_MathProbeName[i], m_MathData[idx].strName);
				x = 270 + (int)(j * 19.7) + iMoveControl_H - 3;
				y = 360 + DeltaY + DRAW_CONTROL_DY * i - 1;
				dc.Rectangle(x, y, x + 14, y + 14);

				// offset변경시 값도 따라서 변경 
				SetDlgItemText(m_IDC_MathProbeValue[i], Double2Str(m_HistoryData[m_iHistorySelectIndex].fMathValue[idx]));
			}

			dc.SelectObject(&m_BrushSelectControl_Select[m_bDrawGraph[j]]);
			dc.SelectObject(&m_PenSelectControl_Select[m_bDrawGraph[j]]);
			for (int i = 0; i < m_iShowDataControl; i++) {
				int idx = i + m_iControlDataOffset;
				if (m_bSelectDraw_GraphData[j][idx]) {
					x = 270 + (int)(j * 19.7) + iMoveControl_H - 1;
					y = DRAW_DATA_CONTROL_Y0 + DRAW_CONTROL_DY * i + 1;
					dc.Rectangle(x, y, x + 10, y + 10);
				}
			}
			for (int i = 0; i < m_iShowMathControl; i++) {
				if (m_MathData[i + m_iControlMathOffset].bSelectGraph[j]) {
					x = 270 + (int)(j * 19.7) + iMoveControl_H - 1;
					y = 360 + DeltaY + DRAW_CONTROL_DY * i + 1;
					dc.Rectangle(x, y, x + 10, y + 10);
				}
			}
		}
	}

	if (m_bShowDataControl) {
		GetDlgItem(IDC_STATIC_view0)->MoveWindow(2, m_iPos_DataStart - 5, m_iRangeLogFilterWidth, m_iPos_DataEnd - m_iPos_DataStart + 5, TRUE);
	}
	GetDlgItem(IDC_STATIC_view0)->ShowWindow(m_bShowDataControl);

	if (m_bShowDataControl) {
		GetDlgItem(IDC_STATIC_Bracket16)->MoveWindow(2, m_iPos_MathStart, m_iRangeLogFilterWidth, m_iPos_MathEnd - m_iPos_MathStart, TRUE);
	}
	GetDlgItem(IDC_STATIC_Bracket16)->ShowWindow(m_bShowDataControl);
}


void CSerialComDlg::SetCursorType(CPoint point) {
	int CursorMode = -1;
	if (m_bShowResizeCursor) {
		int Hori = 0, Vert = 0, Hand = 0;
		// Graph Horizontal size Adjusting Bar Enable
		if (point.x - m_iDrawGraphRangeX[1] <= 5 && point.x - m_iDrawGraphRangeX[1] >= -5 && point.y >= GRAPH_START_Y - 5 && point.y <= m_iDrawGraphRangeY[m_iGraphCount - 1][1] - 10) {
			Hori = 1;
		}

		// Graph Vertical size Adjusting Bar Enable
		if (m_iGraphInfoMaxX >= point.x && point.x >= m_iDrawGraphRangeX[0]) {
			for (int i = 0; i < m_iGraphCount - 1; i++) {
				if (point.y >= m_iDrawGraphRangeY[i][1] && point.y <= m_iDrawGraphRangeY[i + 1][0] && m_iGraphCount > i) {
					Vert = 1;
					break;
				}
			}
		}

		if (m_bShowDataControl && !m_bFixGraphFrame) {
			//Log Filter 창 크기 조절 ( Vertical)
			if (point.y > m_iRangeLogFilterRatio[0] && point.y < m_iRangeLogFilterRatio[1]) {
				if (point.x >= 3 && point.x <= m_iRangeLogFilterWidth) {
					Vert = 1;
				}
			}

			//Log Filter 창 크기 조절 ( Horizontal)
			if (point.x > m_iDrawGraphRangeX[0] - 71 && point.x < m_iDrawGraphRangeX[0] - 62 && point.y > min(50, m_iRangeLogFilterVertical)) {
				//Hori = 1;
			}

			// Data 표시 항목 조절
			if (m_iRangeDataControlVertical - 3 <= point.y && point.y <= m_iRangeDataControlVertical + 5) {
				if (point.x <= m_iRangeLogFilterWidth) {
					//Hand = 1;
				}
			}

			if (m_iPos_MathEnd - 3 <= point.y && point.y <= m_iPos_MathEnd + 5) {
				if (point.x <= m_iRangeLogFilterWidth) {
					Hand = 1;
				}
			}
		}

		if (Hand) {
			CursorMode = 0;
		}
		else if (Vert && Hori) {
			CursorMode = 3;
		}
		else {
			if (Hori) {
				CursorMode = 1;
			}
			else if (Vert) {
				CursorMode = 2;
			}
		}
	}	
	
	m_CursorMode = CursorMode;	
}

BOOL CSerialComDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	if (m_CursorMode == -1) {
		return CDialog::OnSetCursor(pWnd, nHitTest, message);
	}
	else {
		::SetCursor(mh_Cursor[m_CursorMode]);
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CSerialComDlg::OnBnClickedRefdata() {	
	if (m_iProgramMode == 0) {
		if (m_iHistoryIndex == 0) {
			AfxMessageBox("저장된 Data가 없습니다.\r\n(No saved data available.)");
			return;
		}
		CRefDataDlg dlg;
		dlg.pMain = this;
		dlg.DoModal();
	}
}

void CSerialComDlg::GetSavePath() {
	char CurrentPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	// 마지막 Access한 폴더으로 시작
	m_strSavePath[0].Format("%s", CurrentPath);
	m_strSavePath[1].Format("%s", (LPCTSTR)GetRegRoot_RegistryData("Common", "SaveFolder"));

	m_iSaveMode = atoi(GetRegRoot_RegistryData("Common", "SaveMode"));
}

void CSerialComDlg::OnBnClickedReqcontroldlg() {
    if (m_pCmdControl == nullptr) {
        m_pCmdControl = new CControlDlg(this);

        m_pCmdControl->Create(IDD_ControlDlg, this);
        m_pCmdControl->pMain = this;
        m_pCmdControl->ShowWindow(SW_SHOW);
    }
    else {
        if (::IsWindow(m_pCmdControl->GetSafeHwnd()))
        {
            m_pCmdControl->ShowWindow(SW_SHOW);
            m_pCmdControl->SetForegroundWindow();
        }
        else
        {
            m_pCmdControl = nullptr;
        }
    }
}

void CSerialComDlg::SetMoveMouse(const CPoint& pt) {
	if (m_bIsMoveMode == FALSE) return;
	if (m_iHistorySelectPt == pt.x) return;
	if (pt.x < m_iDrawGraphRangeX[0] || pt.x >= m_iDrawGraphRangeX[1]) return;
	// m_iHistorySelectPt --> pt.x
	int iShift = (int)(0.5 + (double)m_iDisplaySize * (pt.x-  m_iHistorySelectPt) / m_iDrawGraphSpanX);
	if (iShift == 0) return;
	m_iHistorySelectPt = pt.x;
	m_PtMoveStart = pt;
	m_iHistoryMove += iShift;
	m_iCnt++;
	BOOL bBlockedLimit = FALSE;
	int iBlockedIndex = 0;

	if (iShift > 0 && m_iHistoryMove < 0) {
		iBlockedIndex = m_iHistoryMove;
		bBlockedLimit = TRUE;
		m_iHistoryMove = 0;
	}

	if (iShift < 0 && m_iHistoryMove > m_iHistoryIndex - m_iDisplaySize) {
		iBlockedIndex = m_iHistoryMove - (m_iHistoryIndex - m_iDisplaySize);
		bBlockedLimit = TRUE;
		m_iHistoryMove = m_iHistoryIndex - m_iDisplaySize;
	}
	m_bReqGraphUpdate = TRUE;
}


COLORREF CSerialComDlg::InvColor(int r, int g, int b) {
	if (r < 25 && g < 25 && b < 25) {
		return RGB(255, 255, 255);
	}

	if (abs(r - g) < 30 && abs(g - b) < 30 && abs(b - r) < 30) {
		if (r < 127) {
			return RGB(255, 255, 255);
		}
		else {
			return RGB(0, 0, 0);
		}
	}

	if (r > 230 && g > 230 && b == 230) {
		return RGB(0, 0, 0);
	}
	double rd = (double)r / 255.0;
	double gd = (double)g / 255.0;
	double bd = (double)b / 255.0;

	double max = fmax(rd, fmax(gd, bd));
	double min = fmin(rd, fmin(gd, bd));
	double delta = max - min;

	double h, s, v;

	v = max;

	if (max == 0) {	s = 0;}	else {	s = delta / max;	}

	if (delta == 0) { 
		h = 0;	
	} else {
		if (max == rd) {
			h = 60 * fmod(((gd - bd) / delta), 6);	
		} else if (max == gd) {	
			h = 60 * (((bd - rd) / delta) + 2);
		}
		else {
			h = 60 * (((rd - gd) / delta) + 4);
		}
	}

	if (h < 0) { h += 360; }
	h = fmod(h + 180, 360);

	double c = v * s;
	double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
	double m = v - c;
	
	if (h >= 0 && h < 60) {
		rd = c;
		gd = x;
		bd = 0;
	}
	else if (h >= 60 && h < 120) {
		rd = x;
		gd = c;
		bd = 0;
	}
	else if (h >= 120 && h < 180) {
		rd = 0;
		gd = c;
		bd = x;
	}
	else if (h >= 180 && h < 240) {
		rd = 0;
		gd = x;
		bd = c;
	}
	else if (h >= 240 && h < 300) {
		rd = x;
		gd = 0;
		bd = c;
	}
	else {
		rd = c;
		gd = 0;
		bd = x;
	}

	r = (int)((rd + m) * 255);
	g = (int)((gd + m) * 255);
	b = (int)((bd + m) * 255);

	return RGB(r, g, b);
}

void CSerialComDlg::DrawIndicator(int iGraph) {	
	ArrangeIndicator(iGraph);
	
	for (int i = 0; i < m_iIndicator; i++) {
		int x = min(m_Indicator[i].pt.x + 3, m_iDrawGraphRangeX[1] - (int)strlen(m_Indicator[i].str) * W_LABEL_TEXT);
		m_MemDC.SetTextColor(m_Indicator[i].text);
		m_MemDC.SetBkColor(m_Indicator[i].bk);
		m_MemDC.TextOutA(x, m_Indicator[i].pt.y, m_Indicator[i].str);
	}
}

void CSerialComDlg::ArrangeIndicator(int iGraph) {
	// sort 오름차순으로 정렬
	std::sort(m_Indicator, m_Indicator + m_iIndicator, [](const CIndicator& a, const CIndicator& b) {
		return a.val > b.val;
		//if (a.pt.y != b.pt.y) { return a.pt.y < b.pt.y; } else { return a.val < b.val; }
	});

	//Cal_margin 
	int sumMargin = 0;
	int h = H_LABEL_TEXT + 1;  // include gap 
	int y0, y1, pre_y0, pre_y1;
	int L0 = m_iDrawGraphRangeY[iGraph][0] + 2;
	int L1 = m_iDrawGraphRangeY[iGraph][1] - 2;
	int margin;
	for (int i = 0; i < m_iIndicator; i++) {
		y0 = m_Indicator[i].pt.y;
		y1 = y0 + h;
		if (i == 0) {
			if (y0 < L0) { 
				y0 = L0;  y1 = y0 + h;
				m_Indicator[i].pt.y = y0; 
			}
			else if (y1 > L1) {
				y0 = L1 - h;  y1 = L1;
				m_Indicator[i].pt.y = y0;
			}
			m_Indicator[0].sumMargin = m_Indicator[0].margin = y0 - L0;
		}
		else {
			margin = y0 - pre_y1;
			m_Indicator[i].sumMargin = m_Indicator[i-1].sumMargin + margin;
			m_Indicator[i].margin = margin;
			
			if (y1 > L1) {
				int m = y1 - L1;
				y0 = L1 - h;  y1 = L1;
				m_Indicator[i].pt.y = y0;
				m_Indicator[i].sumMargin -= m;
				m_Indicator[i].margin -= m;
			}
		}
		pre_y1 = y1;
	}

	pre_y0 = L1;
	for (int i = m_iIndicator - 1; i >= 0; i--) {
		if (m_Indicator[i].sumMargin < 0) {
			m_Indicator[i].pt.y -= m_Indicator[i].sumMargin;
		}
		if (m_Indicator[i].pt.y > pre_y0 - h) { m_Indicator[i].pt.y = pre_y0 - h; }
		if (m_Indicator[i].pt.y < L0) { m_Indicator[i].pt.y = L0; }

		pre_y0 = m_Indicator[i].pt.y;
	}
}

void CSerialComDlg::ChangeSelectPoint(CPoint point) {
	if (point.x >= m_iDrawGraphRangeX[0] && point.x <= m_iGraphInfoMaxX && m_iProgramMode == DATA_ANALYZE_MODE) {
		if (m_iAdjustSelectMode == -1 && m_Cursor.iSelectCursor == -1) {
			if (point.x > m_iDrawGraphRangeX[0] && point.x < m_iDrawGraphRangeX[1]) {
				int iSelectIndex = max(0, min(m_iHistoryCapacity - 1, m_iHistoryIndex + (int)((double)m_iDisplaySize * (m_iHistorySelectPt - m_iDrawGraphRangeX[0]) / m_iDrawGraphSpanX) - m_iHistoryMove - m_iDisplaySize));
				BOOL bIsMatch = FALSE;
				for (int i = 0; i < m_iGraphCount; i++) {
					if (point.y > m_iDrawGraphRangeY[i][0] && point.y < m_iDrawGraphRangeY[i][1]) {
						bIsMatch = TRUE;
					}
				}

				if (bIsMatch) {
					m_bIsMoveMode = TRUE;
					m_PtMoveStart = point;
					m_iHistorySelectPt = point.x;
					int iStartIndex = m_iHistoryIndex + 1 - m_iDisplaySize - m_iHistoryMove;
					int idx = iStartIndex + (int)(0.5 + (double)m_iDisplaySize * (m_iHistorySelectPt - m_iDrawGraphRangeX[0]) / m_iDrawGraphSpanX);
					if (idx < 0) idx = 0;
					if (idx > m_iHistoryIndex) idx = m_iHistoryIndex;
					m_strIndexNumber.Format("%d", idx);
					m_iCurrentIndex = idx;
					if (m_HistoryData[idx].Header != m_cPacketHead) {
						m_strPacketHeader.Format("%c", m_HistoryData[idx].Header);
						UDF;
						OnEnChangePackethead();
					}
					HighlightLog(m_HistoryData[idx].iHistoryLogIndex);
					m_strTemp = m_arrLogData[m_HistoryData[idx].iHistoryLogIndex];
					m_ctrlFilterBox.SetCurSel(m_HistoryData[idx].iHistoryFilterIndex);
					UDF;
					m_bReqGraphUpdate = TRUE;
					m_bDrawSelectData = TRUE;
				}
			}
		}
	}
}

BOOL CSerialComDlg::CheckControlKeyDown() {
	return (GetAsyncKeyState(VK_CONTROL) & 0x8000);
}

BOOL CSerialComDlg::CheckShiftKeyDown() {
	return (GetAsyncKeyState(VK_SHIFT) & 0x8000);
}

void CSerialComDlg::OnBnClickedReload() {
	// Forced Reload
	if (m_iLogIndex == 0) {
		AfxMessageBox("No Log Data.  Bye~!"); return;
	}

	CReloadDataDlg dlg;
	int result = -1;
	dlg.pMain = this;
	dlg.m_pResult = &result;
	dlg.DoModal();
	
	if (result != 1) return;
	// Program Mode 변경
	m_iProgramMode = DATA_ANALYZE_MODE;
	//OnPortClose1();
	//OnPortClose2();
	m_bUpdateList = m_iProgramMode;
	ProgramModeUpdate();

	// data  초기화	
	m_iHistoryIndex = 0;
	m_HistoryData.clear();

	OnBnClickedClearwatchlist();
	m_iFilterIndex = 0;
	m_iFilterHistoryIndexTable.clear();
	m_iFilterLogIndexTable.clear();
	m_iLogFilterIndexTable.clear();
	m_iLogHistoryIndexTable.clear();	
	m_iFilterHistoryIndexTable.emplace_back(0);	
	m_iFilterLogIndexTable.emplace_back(0);	
	m_iLogFilterIndexTable.emplace_back(0);
	m_iLogHistoryIndexTable.emplace_back(0);
	m_ctrlFilterBox.InsertString(m_iFilterIndex, " ");

	// Filter List Update
	BOOL arrUseFilter[MAX_FILTER];

	for (int i = 0; i < MAX_FILTER; i++) {
		if (m_bUseFilterItem[i] && m_iFilterLength[i] > 0) {
			arrUseFilter[i] = TRUE;
		}
		else {
			arrUseFilter[i] = FALSE;
		}
	}

	int nLog = m_arrLogData.size();
	for (int index = 1; index < nLog; index++) {
		Reload_Log(index, arrUseFilter);
	}

	UDF;
	ReCalculationMathData();
	OptimizeGraphCount();
	Req_Draw_ScreenFit();

	m_ctrlFilterBox.SetCurSel(m_iFilterIndex);
	m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
	HighlightLog(m_iLogIndex);
    Wait(0);
    Wait(0);
	m_bReqGraphUpdate = TRUE;
	CString str;
	str.Format("변환을 완료하였습니다 (Conversion completed)\r\nnData: %d", m_iHistoryIndex);
	AfxMessageBox(str);
}

BOOL  CSerialComDlg::Reload_Log(int index, BOOL* bUseFilter) {
	static char str[1000] = "";
	if (index > m_iLogIndex) return FALSE;
	strcpy_s(str, 1000, m_arrLogData[index]);
	int iLen = strlen(str);

	int seq = 0, sub = 0;
	int h = 0, m = 0, sec = 0, ms = 0;
	BOOL negative = FALSE;
	BOOL period = FALSE;
	double order = 1.;
	BOOL getValue = FALSE;
	double val = 0.;
	BYTE s;
	UART_DATA temp = m_dummy_UARTData;
	BOOL bGetHeader = FALSE;
	int iRecvChannel;
	if (m_cPacketHead == '_') {
		bGetHeader = TRUE;
	}
	  	 
	for (int i = 0; i < iLen; i++) {
		s = str[i];
		switch (seq) {
			// Get Data			
		case 2:			
			if (s >= '0' && s <= '9') {
				getValue = TRUE;
				if (period) {
					val += order * (s - '0');
					order *= 0.1;
				}
				else {
					val *= 10;
					val += (s - '0');
				}
				break;
			}

			if (s == '-') {
				negative = TRUE;
				break;
			}
			
			if (s == '.') {
				period = TRUE;
				getValue = TRUE;
				order = .1; 				
				break;
			}
			
			if (sub == 0 && m_cPacketHead == '_') {
				if (s >= 'a' && s <= 'z') { bGetHeader = FALSE; i = iLen;  break; }
				if (s >= 'A' && s <= 'Z') { bGetHeader = FALSE; i = iLen;  break; }
			}
				
			if (getValue && sub < m_iMaxDataSize) {               
				if (negative) val = -val;
				temp.fDataValue[sub] = val;
				sub++;
				getValue = FALSE;
				period = FALSE;
				negative = FALSE;
				val = 0.;
			}			
			break;

			// Get Time
		case 0:
			switch (sub) {			
			case 0: if (s >= '0' && s <= '9') { h = 10 * (s - '0'); sub++; } break;
			case 1: if (s >= '0' && s <= '9') { h += (s - '0'); sub++; } break;
			case 2: if (s >= '0' && s <= '9') { m = 10 * (s - '0'); sub++; } break;
			case 3: if (s >= '0' && s <= '9') { m += (s - '0'); sub++; } break;
			case 4: if (s >= '0' && s <= '9') { sec = 10 * (s - '0'); sub++; } break;
			case 5: if (s >= '0' && s <= '9') { sec += (s - '0'); sub++; } break;
			case 6: if (s >= '0' && s <= '9') { ms = 100 * (s - '0'); sub++; } break;
			case 7: if (s >= '0' && s <= '9') { ms += 10 * (s - '0'); sub++; } break;
			case 8: if (s >= '0' && s <= '9') {
				ms += (s - '0'); sub++; iRecvChannel = 0;
			} break;
			case 9: 
				if (s == ']') { 
					if (m_cPacketHead == '_') { seq = 2; } else { seq++; }					
					sub = 0;
				} 
				else if (s == 'b') { iRecvChannel = 1; }
				break;
			default: return FALSE;
			}
			break;

			// Header Check;
		case 1:
			if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z')) {
				if (m_cPacketHead == s) {
					seq = 2; sub = 0;
					negative = FALSE;
					period = FALSE;
					getValue = FALSE;
					val = 0.;
					bGetHeader = TRUE;
				}
				else {
					i = iLen;
				}
			}
			break;
		}
	}
	   	  
	if (getValue && sub < m_iMaxDataSize) {
		if (negative) val = -val;
		temp.fDataValue[sub] = val;
		sub++;
	}

	if (bGetHeader == TRUE) {
		if (sub < m_iMinPacketDataSize || sub > m_iMaxPacketDataSize) { bGetHeader = FALSE; }
	}

	
	if (bGetHeader) {
		sub = min(sub, m_iMaxDataSize);
		for (int i = sub; i < m_iMaxDataSize; i++)  temp.fDataValue[i] = 0;

		temp.iRcvTime = h * 3600 + m * 60 + sec;
		temp.iRcvTime_ms = ms;
		temp.iHistoryLogIndex = index;
		temp.iRecvChannel = iRecvChannel;
		temp.Header = m_cPacketHead;

		m_HistoryData.push_back(temp);
		m_iLogHistoryIndexTable.push_back(m_iHistoryIndex);
		m_iHistoryIndex++;
	}
	else {
		m_iLogHistoryIndexTable.push_back(m_iHistoryIndex);
	}
	
	//  Filter update
	{
		int nStart = m_arrLogData[index].Find("]");

		CString str;
		str.Format("%s", (LPCTSTR)m_arrLogData[index].Right(iLen - nStart));
		BOOL bIsFilterItem = FALSE;
		BOOL bIsHistoryItem = bGetHeader;
		CString strFilter;

		for (int i = 0; i < MAX_FILTER; i++) {
			if (bUseFilter[i]) {
				int iFiterPosition = str.Find(m_strFilterItem[i]);
				if (iFiterPosition != -1) {
					int iTotal = str.GetLength();
					strFilter.Format("%s", (LPCTSTR)str.Right(iTotal - iFiterPosition - m_iFilterLength[i]));
					double fValue = GetValue(strFilter);
					int iValue = (int)fValue;
					if (fValue == (double)iValue) {
						SetDlgItemInt(m_IDC_FilterValue[i], iValue);
					}
					else {
						CString strVal;
						strVal.Format("%f", fValue);
						SetDlgItemText(m_IDC_FilterValue[i], strVal);
					}
					bIsFilterItem = TRUE;
				}
			}
		}	

		if (bIsFilterItem) {
			m_iFilterIndex++;
			CString strFilterLog;
			strFilterLog.Format("%s [%d]", (LPCTSTR)m_arrLogData[index], m_iHistoryIndex);
			m_ctrlFilterBox.InsertString(m_iFilterIndex, strFilterLog);
			m_iFilterHistoryIndexTable.emplace_back(m_iHistoryIndex);
			m_iFilterLogIndexTable.emplace_back(index);
		}

		
		// Data정리
		if (bGetHeader) {
			m_HistoryData[m_iHistoryIndex - 1].iHistoryFilterIndex = m_iFilterIndex;
		}

		m_iLogFilterIndexTable[index] = m_iFilterIndex;
	}
	return TRUE;
}

void CSerialComDlg::ReqRegInit() {
	HKEY hKey;
	CString reg_root;
	reg_root.Format("%s", DEFAULT_REGISTRY_SETTING_PATH);

	// 루트 키를 엽니다.
	if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_root, 0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS) {
		return;
	}

	// 레지스트리 키를 삭제합니다.
	DeleteRegistryKey(HKEY_CURRENT_USER, reg_root);

	// 키를 닫습니다.
	RegCloseKey(hKey);
}

void CSerialComDlg::DeleteRegistryKey(HKEY hKey, CString reg_root)
{
	HKEY hSubKey;
	if (RegOpenKeyEx(hKey, reg_root, 0, KEY_READ | KEY_WRITE, &hSubKey) != ERROR_SUCCESS) {
		return;
	}

	// 하위 키 삭제
	while (true)
	{
		TCHAR szName[MAX_PATH];
		DWORD dwNameSize = MAX_PATH;
		FILETIME ft;

		LONG lResult = RegEnumKeyEx(hSubKey, 0, szName, &dwNameSize, NULL, NULL, NULL, &ft);
		if (lResult != ERROR_SUCCESS)
			break;

		CString szSubKeyPath;
		szSubKeyPath.Format(_T("%s\\%s"), (LPCTSTR)reg_root, szName);
		DeleteRegistryKey(hKey, szSubKeyPath);
	}

	// 값 삭제
	while (true)
	{
		TCHAR szValueName[MAX_PATH];
		DWORD dwValueNameSize = MAX_PATH;

		LONG lResult = RegEnumValue(hSubKey, 0, szValueName, &dwValueNameSize, NULL, NULL, NULL, NULL);
		if (lResult != ERROR_SUCCESS)
			break;

		RegDeleteValue(hSubKey, szValueName);
	}

	RegCloseKey(hSubKey);

	// 최종적으로 현재 키 삭제
	RegDeleteKey(hKey, reg_root);
}


void CSerialComDlg::ExportRegFile_after_Conversion_from_Old(int mode) {
	HKEY hKey;
	CString reg_root = "MY_TOOL\\SerialMonitor\\";
    if (mode == 1) {
        reg_root.Format("%s", DEFAULT_REGISTRY_SETTING_PATH);
    }

	while (reg_root.GetAt(reg_root.GetLength() - 1) == '\\') {
		reg_root.Delete(reg_root.GetLength() - 1);
	}
	CFile save_reg_file;

	// 루트 키를 엽니다.
	if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_root, 0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS) {
		AfxMessageBox("설정 레지스트리의 루트를 찾을 수 없습니다.\r\n(Unable to find the root of the settings registry.)");
		return;
	}

	CString strFileName;
	SYSTEMTIME time;
	GetLocalTime(&time);
	if (mode == 0)
		strFileName.Format("Reg_ver2_%02d%02d%02d_%02d%02d%02d_Conversion_from_ver1.reg", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	else
		strFileName.Format("Reg_ver1_%02d%02d%02d_%02d%02d%02d_Conversion_from_ver2.reg", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	Open_File_with_FullPathname(strFileName, save_reg_file);
	Save_reg_version(save_reg_file);

	ExportRegistryKey_conversion(HKEY_CURRENT_USER, reg_root, save_reg_file, mode);
	RegCloseKey(hKey);
	save_reg_file.Close();

	CString str;
	if (mode == 0)
		str.Format("%s 을 생성하였습니다. Ver.1의 설정을 추가하시려면 생성된 파일을 더블 클릭하세요\r\n(File created successfully. \r\nDouble - click the file to add Version 1 settings.)", (LPCTSTR)strFileName);
	else
        str.Format("%s 을 생성하였습니다. Ver.1의 설정을 추가하시려면 생성된 파일을 더블 클릭하세요\r\n(File created successfully. \r\nDouble - click the file to add Version 2 settings.)", (LPCTSTR)strFileName);
	AfxMessageBox(str);
}


void CSerialComDlg::ExportRegFile() {
	HKEY hKey;
	CString reg_root;
	reg_root.Format("%s", DEFAULT_REGISTRY_SETTING_PATH);
	while (reg_root.GetAt(reg_root.GetLength() - 1) == '\\') {		
		reg_root.Delete(reg_root.GetLength() - 1);
	}
	CFile save_reg_file;

	// 루트 키를 엽니다.
	if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_root, 0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS) {
		AfxMessageBox("설정 레지스트리의 루트를 찾을 수 없습니다.\r\n(Cannot find the settings registry root.)");
		return;
	}
	
	CString strFileName;
	SYSTEMTIME time;
	GetLocalTime(&time);
	strFileName.Format("Reg_Ver2_%02d%02d%02d_%02d%02d%02d.reg", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	Open_File_with_FullPathname(strFileName, save_reg_file);	
	Save_reg_version(save_reg_file);
	
	ExportRegistryKey(HKEY_CURRENT_USER, reg_root, save_reg_file);

	RegCloseKey(hKey);
	save_reg_file.Close();

	CString str;
	str.Format("%s 을 생성하였습니다.\r\n(File created successfully.)", (LPCTSTR)strFileName);
	AfxMessageBox(str);
}


void CSerialComDlg::Save_reg_version(CFile &outFile) {
	CString str;
	str.Format("Windows Registry Editor Version 5.00\r\n");
	outFile.Write(str, str.GetLength());
}

void CSerialComDlg::Conversion_Reg(CString& str, int mode) {
	for (int i = 0; i < N_CONVERT_REG; i++) {
		if (m_ConverItem[i].use[mode]) {
			if (mode == 0)
				str.Replace(m_ConverItem[i].str[0], m_ConverItem[i].str[1]);
			else 
				str.Replace(m_ConverItem[i].str[1], m_ConverItem[i].str[0]);
		}
	}
}

void CSerialComDlg::ExportRegistryKey_conversion(HKEY hKey, CString subkey, CFile &outFile, int mode) {
	HKEY hSubKey;
	DWORD dwIndex = 0;
	TCHAR szName[MAX_PATH];
	DWORD dwNameSize = MAX_PATH;
	DWORD dwType, dwDataSize;
	BYTE* pData = nullptr;

	// 열려 있는 키에서 하위 키를 엽니다.
	if (RegOpenKeyEx(hKey, subkey, 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) {
		return;
	}

	// 현재 키의 이름을 파일에 씁니다.
	CString str;
	str.Format("[HKEY_CURRENT_USER\\%s]\r\n", (LPCTSTR)subkey);
	Conversion_Reg(str, mode);
	outFile.Write(str, str.GetLength());

	// 현재 키의 모든 값을 파일에 씁니다.
	while (RegEnumValue(hSubKey, dwIndex, szName, &dwNameSize, NULL, &dwType, NULL, &dwDataSize) == ERROR_SUCCESS)
	{
		pData = new BYTE[dwDataSize + 1];
		if (RegQueryValueEx(hSubKey, szName, NULL, &dwType, pData, &dwDataSize) == ERROR_SUCCESS)
		{
			pData[dwDataSize] = '\0';
			str.Format("\"%s\"=", szName);
			outFile.Write(str, str.GetLength());
			switch (dwType)
			{
			case REG_SZ:
				str.Format("\"%s\"", pData);				
				outFile.Write(str, str.GetLength());
				break;
			case REG_DWORD:
				str.Format("%u", *(DWORD*)pData);
				outFile.Write(str, str.GetLength());
				break;
				// 다른 데이터 형식도 필요에 따라 처리할 수 있습니다.
			default:
				str.Format("Unsupported data type");
				outFile.Write(str, str.GetLength());
				break;
			}
			str.Format("\n");
			outFile.Write(str, str.GetLength());
		}
		delete[] pData;
		dwNameSize = MAX_PATH;
		dwIndex++;
	}

	// 현재 키의 모든 하위 키를 재귀적으로 파일에 씁니다.
	dwIndex = 0;
	dwNameSize = MAX_PATH;
	while (RegEnumKeyEx(hSubKey, dwIndex, szName, &dwNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		str.Format("%s\\%s", (LPCTSTR)subkey, szName);
		ExportRegistryKey_conversion(hKey, str, outFile, mode);
		dwNameSize = MAX_PATH;
		dwIndex++;
	}

	// 키를 닫습니다.
	RegCloseKey(hSubKey);
}

void CSerialComDlg::ExportRegistryKey(HKEY hKey, CString subkey, CFile &outFile) {
	HKEY hSubKey;
	DWORD dwIndex = 0;
	TCHAR szName[MAX_PATH];
	DWORD dwNameSize = MAX_PATH;
	DWORD dwType, dwDataSize;
	BYTE* pData = nullptr;

	// 열려 있는 키에서 하위 키를 엽니다.
	if (RegOpenKeyEx(hKey, subkey, 0, KEY_READ, &hSubKey) != ERROR_SUCCESS)	{
		return;
	}

	// 현재 키의 이름을 파일에 씁니다.
	CString str;
	str.Format("[HKEY_CURRENT_USER\\%s]\r\n", (LPCTSTR)subkey);
	outFile.Write(str, str.GetLength());

	// 현재 키의 모든 값을 파일에 씁니다.
	while (RegEnumValue(hSubKey, dwIndex, szName, &dwNameSize, NULL, &dwType, NULL, &dwDataSize) == ERROR_SUCCESS)
	{
		pData = new BYTE[dwDataSize+1];		
		if (RegQueryValueEx(hSubKey, szName, NULL, &dwType, pData, &dwDataSize) == ERROR_SUCCESS)
		{
			pData[dwDataSize] = '\0';
			str.Format("\"%s\"=", szName);
			outFile.Write(str, str.GetLength());
			switch (dwType)
			{
			case REG_SZ:
				str.Format("\"%s\"", pData);
				outFile.Write(str, str.GetLength());
				break;
			case REG_DWORD:
				str.Format("%u", *(DWORD*)pData);
				outFile.Write(str, str.GetLength());
				break;
				// 다른 데이터 형식도 필요에 따라 처리할 수 있습니다.
			default:
				str.Format("Unsupported data type");
				outFile.Write(str, str.GetLength());
				break;
			}
			str.Format("\n");
			outFile.Write(str, str.GetLength());
		}
		delete[] pData;
		dwNameSize = MAX_PATH;
		dwIndex++;
	}

	// 현재 키의 모든 하위 키를 재귀적으로 파일에 씁니다.
	dwIndex = 0;
	dwNameSize = MAX_PATH;
	while (RegEnumKeyEx(hSubKey, dwIndex, szName, &dwNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		str.Format("%s\\%s", (LPCTSTR)subkey, szName);
		ExportRegistryKey(hKey, str, outFile);
		dwNameSize = MAX_PATH;
		dwIndex++;
	}

	// 키를 닫습니다.
	RegCloseKey(hSubKey);
}

void CSerialComDlg::OnBnClickedFittomaxdata() {
	m_bFitToScreen = IsDlgButtonChecked(IDC_FitToMaxData);
	m_bManualAdjust = FALSE;
	if (m_bFitToScreen) {
		Req_Draw_ScreenFit();
	}
}

void CSerialComDlg::CopyDataProbeMode(BYTE cSourceHead) {
	CopyDataProbeMode(cSourceHead, m_cPacketHead);
}


void CSerialComDlg::CopyDataProbeMode(BYTE cSourceHead, BYTE cTargetHead)
{
	CString strSourceAddress, strTargetAddress, strItem;
	// Pin Name		
	strSourceAddress.Format("Header\\%c%d", cSourceHead, cSourceHead);
	strTargetAddress.Format("Header\\%c%d", cTargetHead, cTargetHead);
	BOOL bUpdateControl = FALSE;
	if (cTargetHead == m_cPacketHead) {
		bUpdateControl = TRUE;
	}

	for (int iData = 0; iData < m_iMaxDataSize; iData++) {
		strItem.Format("PinName%02d", iData);
		SetReg_RegistryData(strTargetAddress, strItem, GetReg_RegistryData(strSourceAddress, strItem));
		if (bUpdateControl) {
			m_strPinName[iData].Format("%s", (LPCTSTR)GetReg_RegistryData(strSourceAddress, strItem));
		}
	}
	UDF;
}

void CSerialComDlg::CopyDataGraphSetting(BYTE cSourceHead, int iSourceMode) {
	CopyDataGraphSetting(cSourceHead, iSourceMode, m_cPacketHead, m_iPresetMode, m_iPresetMode);
}


void CSerialComDlg::CopyDataGraphSetting(BYTE cSourceHead, int iSourceMode, BYTE cTargetHead, int iTargetMode0, int iTargetMode1) {
	CString strSRC_Addr, strTGT_Addr, strItem, strValue;
	CString strSRC_Mode, strTGT_Mode;

	if (iSourceMode == 0) {
		strSRC_Mode = "";
	}
	else {
		strSRC_Mode.Format("\\Mode_%d", iSourceMode);
	}

	for (int iMode = iTargetMode0; iMode <= iTargetMode1; iMode++) {
		if (iMode == 0) {
			strTGT_Mode = "";
		}
		else {
			strTGT_Mode.Format("\\Mode_%d", iMode);
		}

		BOOL bUpdateControl = FALSE;
		for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
			strSRC_Addr.Format("Graph\\Graph%d\\%c%d%s", iGraph, cSourceHead, cSourceHead, (LPCTSTR)strSRC_Mode);
			strTGT_Addr.Format("Graph\\Graph%d\\%c%d%s", iGraph, cTargetHead, cTargetHead, (LPCTSTR)strTGT_Mode);
			if (cTargetHead == m_cPacketHead && iMode == m_iPresetMode) {
				bUpdateControl = TRUE;
			}
			else {
				bUpdateControl = FALSE;
			}

			for (int iData = 0; iData < m_iMaxDataSize; iData++) {
				strItem.Format("ViewData%02d", iData);
				strValue = GetReg_RegistryData(strSRC_Addr, strItem);
				SetReg_RegistryData(strTGT_Addr, strItem, strValue);
				if (bUpdateControl) {
					if (strValue == "1") {
						m_bSelectDraw_GraphData[iGraph][iData] = TRUE;
					}
					else {
						m_bSelectDraw_GraphData[iGraph][iData] = FALSE;
					}
				}
			}

			strItem.Format("MaxValue");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				m_RealGrpahInfo[iGraph].fRangeMax = atof(strValue);
			}

			strItem.Format("MinValue");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				m_RealGrpahInfo[iGraph].fRangeMin = atof(strValue);
				m_strRangeMid[iGraph].Format("%s", (LPCTSTR)Dbl2Str((m_fRangeMin[iGraph] + m_fRangeMax[iGraph]) / 2));
			}

			strItem.Format("AutoRange");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				m_RealGrpahInfo[iGraph].bAutoRange_Graph = atoi(strValue);
			}
		}

		if (bUpdateControl) {
			UDF;
		}
	}
}


void CSerialComDlg::CopyMathProbeMode(BYTE cSourceHead, int iSourceMode) {
	CopyMathProbeMode(m_cPacketHead, iSourceMode, m_cPacketHead, m_iPresetMode, m_iPresetMode);
}

void CSerialComDlg::CopyMathProbeMode(BYTE cSourceHead, int iSourceMode, BYTE cTargetHead, int iTargetMode0, int iTargetMode1)
{
	CString strSRC_Addr, strTGT_Addr, strItem, strValue;
	CString strSRC_Mode, strTGT_Mode;


	for (int i = 0; i < m_iMaxMathSize; i++) {
		strSRC_Addr.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, cSourceHead, cSourceHead, iSourceMode);
		for (int iMode = iTargetMode0; iMode <= iTargetMode1; iMode++) {
			BOOL bUpdateControl = FALSE;
			if (cTargetHead == m_cPacketHead && iMode == m_iPresetMode) {
				bUpdateControl = TRUE;
			}

			strTGT_Addr.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, cTargetHead, cTargetHead, iMode);
			// Data 1
			strItem.Format("Data1");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				if (strValue != "")  m_MathData[i].iDataID1 = atoi(strValue);
			}

			strItem.Format("Data2");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				if (strValue != "")  m_MathData[i].iDataID2 = atoi(strValue);
			}

			strItem.Format("Operator");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				if (strValue != "")  m_MathData[i].iOperator = atoi(strValue);
			}

			strItem.Format("UseMathPinName");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				if (strValue != "")
                    m_MathData[i].bUserName = atoi(strValue);
				else
                    m_MathData[i].bUserName = FALSE;
			}

			strItem.Format("PinName");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				strValue.Replace("]", "");
				strValue.Replace("[", "");
				if (strValue != "") {
                    m_MathData[i].strName.Format("[%s]", (LPCTSTR)strValue);
					m_strMathProbeName[i].Format("[%s]", (LPCTSTR)strValue);
				}
			}

			strItem.Format("ConstValue");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				if (strValue != "")  m_MathData[i].fValue = atof(strValue);
			}

            strItem.Format("Result_Integer");
            strValue = GetReg_RegistryData(strSRC_Addr, strItem);
            SetReg_RegistryData(strTGT_Addr, strItem, strValue);
            if (bUpdateControl) {
                if (strValue != "")  m_MathData[i].bResInt = atoi(strValue);
            }

			strItem.Format("DataSize");
			strValue = GetReg_RegistryData(strSRC_Addr, strItem);
			SetReg_RegistryData(strTGT_Addr, strItem, strValue);
			if (bUpdateControl) {
				if (strValue != "")  m_MathData[i].nData = atoi(strValue);
			}
		}
	}
}


void CSerialComDlg::CopyMathGraphSetting(BYTE cSourceHead, int iSourceMode) {
	CopyMathGraphSetting(m_cPacketHead, iSourceMode, m_cPacketHead, m_iPresetMode, m_iPresetMode);
}

void CSerialComDlg::CopyMathGraphSetting(BYTE cSourceHead, int iSourceMode, BYTE cTargetHead, int iTargetMode0, int iTargetMode1) {
	CString strSRC_Addr, strTGT_Addr, strItem, strValue;
	CString strSRC_Mode, strTGT_Mode;

	for (int i = 0; i < m_iMaxMathSize; i++) {
		strSRC_Addr.Format("MathDefine\\MathProbe%d\\%c%d\\Mode_%d", i, cSourceHead, cSourceHead, iSourceMode);
		for (int iMode = iTargetMode0; iMode <= iTargetMode1; iMode++) {
			strTGT_Addr.Format("MathDefine\\MathProbe%d\\%c%d\\Mode_%d", i, cTargetHead, cTargetHead, iMode);

			BOOL bUpdateControl = FALSE;
			if (cTargetHead == m_cPacketHead && iMode == m_iPresetMode) {
				bUpdateControl = TRUE;
			}

			for (int j = 0; j < MAX_GRAPH_COUNT; j++) {
				strItem.Format("DrawGraph%d", j);
				strValue = GetReg_RegistryData(strSRC_Addr, strItem);
				SetReg_RegistryData(strTGT_Addr, strItem, strValue);
				if (bUpdateControl) {
					if (strValue == "1")
                        m_MathData[i].bSelectGraph[j] = TRUE;
					else
                        m_MathData[i].bSelectGraph[j] = FALSE;
				}
			}
		}
	}
	UDF;
}

void CSerialComDlg::OnBnClickedDisplay0()
{
	if (IsDlgButtonChecked(IDC_Display0)) {
		m_iShowDataControl = MAX_DATACONTROL_SIZE;
	}
	else {
		m_iShowDataControl = 0;
	}

	m_iRangeDataControlVertical = DRAW_CONTROL_DY * m_iShowDataControl + DRAW_DATA_CONTROL_Y0;
	ControlView();
}


void CSerialComDlg::OnBnClickedDisplay1()
{
	if (IsDlgButtonChecked(IDC_Display1) == TRUE) {
		m_iFilterViewMode = FALSE;
	}
	else {
		m_iFilterViewMode = TRUE;
	}
	ControlView();
}

void CSerialComDlg::OnBnClickedDisplay2()
{
	if (IsDlgButtonChecked(IDC_Display2)) {
		m_bShowMathControl = TRUE;
		m_iShowMathControl = MAX_MATHCONTROL_SIZE;
		GetDlgItem(IDC_ShowMathMenu)->ShowWindow(FALSE);
		GetDlgItem(IDC_HideMathMenu)->ShowWindow(TRUE);
	}
	else {
		m_bShowMathControl = FALSE;
		m_iShowMathControl = 0;
		GetDlgItem(IDC_ShowMathMenu)->ShowWindow(TRUE);
		GetDlgItem(IDC_HideMathMenu)->ShowWindow(FALSE);
	}
	ControlView();
}

void CSerialComDlg::OnBnClickedDisplay3()
{
	// ALL
	static BOOL bBusy = FALSE;
	static BOOL bPreMathViewMode = m_bShowMathControl;
	static int iPreFilterViewMode = m_iFilterViewMode;
	if (bBusy) return;
	bBusy = TRUE;    
    
	if (m_bShowDataControl == FALSE) {
		m_bShowDataControl = TRUE;
		GetDlgItem(IDC_HIDE_SIDEMENU)->SetWindowTextA("<<");
        m_bShowMathControl = bPreMathViewMode;
		m_iFilterViewMode = iPreFilterViewMode;        
		GetDlgItem(IDC_FilterMessage)->ShowWindow(m_bShowFilterLogControl);
		GetDlgItem(IDC_LogList)->ShowWindow(TRUE);
		m_ctrlLogList.SetRedraw(TRUE);
		GetDlgItem(IDC_DataSetting)->ShowWindow(TRUE);
		GetDlgItem(IDC_Model)->ShowWindow(FALSE);		
		GetDlgItem(IDC_ShowMathMenu)->ShowWindow(m_bShowMathControl==FALSE);
		GetDlgItem(IDC_HideMathMenu)->ShowWindow(m_bShowMathControl);
		m_iDrawGraphRangeX[0] = m_iPreDrawGraphRangeX;
	}
	else {
		m_bShowDataControl = FALSE;
		GetDlgItem(IDC_HIDE_SIDEMENU)->SetWindowTextA(">>");
		bPreMathViewMode = m_bShowMathControl;
		iPreFilterViewMode = m_iFilterViewMode;
        m_bShowMathControl = FALSE;
		m_iFilterViewMode = 1;
		GetDlgItem(IDC_FilterMessage)->ShowWindow(FALSE);
		GetDlgItem(IDC_LogList)->ShowWindow(FALSE);
		m_ctrlLogList.SetRedraw(FALSE);
		GetDlgItem(IDC_DataSetting)->ShowWindow(FALSE);
		GetDlgItem(IDC_Model)->ShowWindow(FALSE);
		GetDlgItem(IDC_ShowMathMenu)->ShowWindow(FALSE);
		GetDlgItem(IDC_HideMathMenu)->ShowWindow(FALSE);
		m_iDrawGraphRangeX[0] = 80;
	}
	GetDlgItem(IDC_Display2)->ShowWindow(m_bShowDataControl);
	OnSize(0, m_iWndSizeX, m_iWndSizeY);
	ControlView();

	BOOL bActive = TRUE;
	if (IsDlgButtonChecked(IDC_Display3)) {
		bActive = FALSE;
	}
	GetDlgItem(IDC_Display0)->EnableWindow(bActive);
	GetDlgItem(IDC_Display1)->EnableWindow(bActive);
	GetDlgItem(IDC_Display2)->EnableWindow(bActive);
	GetDlgItem(IDC_Display4)->EnableWindow(bActive);
	m_bReqGraphUpdate = TRUE;

	Wait(10);
	bBusy = FALSE;
}

void CSerialComDlg::OnBnClickedDisplay4() {
    // Filter Log 
    m_bShowFilterLogControl = IsDlgButtonChecked(IDC_Display4);
    GetDlgItem(IDC_FilterMessage)->ShowWindow(m_bShowFilterLogControl);
    ControlView();
}

void CSerialComDlg::OnCbnSelchangeMode()
{
	m_iPresetMode = m_ctrlMode.GetCurSel();
	m_bReqGraphUpdate = TRUE;
	ChangeHeader();

	// Log추가
	CString str;
	SYSTEMTIME time;
	GetLocalTime(&time);

	str.Format("%02d:%02d:%02d.%03d] Change Mode: %d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, m_iPresetMode + 1);
    m_iLogIndex++;
    AddLogData(str);
    m_iLogHistoryIndexTable.emplace_back(m_iHistoryIndex);
    m_iLogFilterIndexTable.emplace_back(m_iFilterIndex);

    str.Format("#MC%d", m_iPresetMode + 1);
	m_iLogIndex++;
	AddLogData(str);
    m_iLogHistoryIndexTable.emplace_back(m_iHistoryIndex);
    m_iLogFilterIndexTable.emplace_back(m_iFilterIndex);
	ScrollToLastItem();

	if ((m_cPacketHead >= 'a' && m_cPacketHead <= 'z') || (m_cPacketHead >= 'A' && m_cPacketHead <= 'Z')) {
		CString strAddress, strItem, strLastMode;
		strAddress.Format("Header\\%c%d", m_cPacketHead, m_cPacketHead);
		strItem = "LastMode";
		strLastMode.Format("%d", m_iPresetMode);
		SetRegRoot_RegistryData(strAddress, strItem, strLastMode);
	}
    m_bReqUpdateControlDraw = TRUE;
}


void CSerialComDlg::OnBnClickedModecopy() {
	CString str;
	GetDlgItemText(IDC_PacketHead, str);
	BYTE aByte = str.GetAt(0);
	str.Format("%c", aByte);
	if (str.GetLength() != 1 || m_strPacketHeader != str || m_cPacketHead != aByte) {
		AfxMessageBox("Header 설정후 다시 시도해주세요\r\n(Please set the header and try again.)");
		return;
	}

	if ((m_cPacketHead >= 'a' && m_cPacketHead <= 'z') || (m_cPacketHead >= 'A' && m_cPacketHead <= 'Z')) {
		CModeCopyDlg dlg;
		dlg.pMain = this;
		dlg.DoModal();
	}
	else {
        AfxMessageBox("Header 범위오류: Header 재성정이후 다시 시도해주세요/r/n(Header range error: retry after set header)");
	}
}

void CSerialComDlg::Req_Draw_ScreenFit() {
	m_iHistorySelectPt = m_iDrawGraphRangeX[1];
	m_iHistoryMove = 0;
	m_iDisplaySize = max(MIN_DISPLAY_SIZE, min(m_iMaxFitScreen, m_iHistoryIndex));
	SetDisplayDataSize();
}


void CSerialComDlg::OnEnChangeMaxfitscreen() {
	int iData = GetDlgItemInt(IDC_MaxFitScreen);
	if (iData != 0) {
		m_iMaxFitScreen = max(MIN_DISPLAY_SIZE, min(iData, m_iHistoryCapacity));
	}
	else {
		m_iMaxFitScreen = 0;
	}

	if (iData != m_iMaxFitScreen) {
		SetDlgItemInt(IDC_MaxFitScreen, m_iMaxFitScreen);
	}
}

void CSerialComDlg::SaveGraphLogData(BYTE cHeader) {

	CString strAddress, strItem, strValue;
	strAddress.Format("Log\\GraphLog\\%c%d", cHeader, cHeader);
	strItem.Format("nCount");
	strValue.Format("%d", m_iGraphLogCount);
	SetReg_RegistryData(strAddress, strItem, strValue);

	strItem.Format("Jpg_path");
	SetReg_RegistryData(strAddress, strItem, m_strGraphLog_Jpg);

	for (int i = 0; i < m_iGraphLogCount; i++) {
		strAddress.Format("Log\\GraphLog\\%c%d\\Item%d", cHeader, cHeader, i);
		strItem = "use";	strValue.Format("%d", m_GraphLog[i].use);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "item_type"; strValue.Format("%d", m_GraphLog[i].item_type);  SetReg_RegistryData(strAddress, strItem, strValue);		
		strItem = "data1_type";  strValue.Format("%d", m_GraphLog[i].data1_type);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "data1_val";  strValue.Format("%d", m_GraphLog[i].iData1_val);  SetReg_RegistryData(strAddress, strItem, strValue);
        strItem = "fdata1_val";  strValue.Format("%f", m_GraphLog[i].fData1_val);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "cond";  strValue.Format("%d", m_GraphLog[i].cond);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "data2_type";  strValue.Format("%d", m_GraphLog[i].data2_type);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "data2_val";  strValue.Format("%d", m_GraphLog[i].iData2_val);  SetReg_RegistryData(strAddress, strItem, strValue);
        strItem = "fdata2_val";  strValue.Format("%f", m_GraphLog[i].fData2_val);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "Digit";   strValue.Format("%d", m_GraphLog[i].digit);  SetReg_RegistryData(strAddress, strItem, strValue);
        strItem = "Div";   strValue.Format("%d", m_GraphLog[i].div);  SetReg_RegistryData(strAddress, strItem, strValue);
		for (int j = 0; j < 4; j++) {
			strItem.Format("range%d", j);
			strValue.Format("%d", m_GraphLog[i].range[j]);  SetReg_RegistryData(strAddress, strItem, strValue);
		}

		strItem = "color1";  strValue.Format("%d", m_GraphLog[i].color1);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "color2";  strValue.Format("%d", m_GraphLog[i].color2);  SetReg_RegistryData(strAddress, strItem, strValue);

		strItem = "MEMO";  SetReg_RegistryData(strAddress, strItem, m_GraphLogMemo[i]);
		strItem = "PreText";  SetReg_RegistryData(strAddress, strItem, m_GraphPreText[i]);		
		strItem = "PostText";  SetReg_RegistryData(strAddress, strItem, m_GraphPostText[i]);
		strItem = "UseMask"; strValue.Format("%d", m_GraphLog[i].use_mask);  SetReg_RegistryData(strAddress, strItem, strValue);
		strItem = "Mask_val"; strValue.Format("%d", m_GraphLog[i].mask_val);  SetReg_RegistryData(strAddress, strItem, strValue);
	}
}


void CSerialComDlg::GetGraphLogData() {
	CString strAddress, strItem, strValue;
	strAddress.Format("Log\\GraphLog\\%c%d", m_cPacketHead, m_cPacketHead);
	strItem.Format("nCount");
	strValue = GetReg_RegistryData(strAddress, strItem);
	m_iGraphLogCount = atoi(strValue);

	strItem.Format("Jpg_path");
	m_strGraphLog_Jpg = GetReg_RegistryData(strAddress, strItem);

	for (int i = 0; i < m_iGraphLogCount; i++) {
		strAddress.Format("Log\\GraphLog\\%c%d\\Item%d", m_cPacketHead, m_cPacketHead, i);

		strItem = "use";	strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].use = atoi(strValue); 
		strItem = "item_type"; strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].item_type = atoi(strValue);
		strItem = "data1_type";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].data1_type = atoi(strValue);
		strItem = "data1_val";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].iData1_val = atoi(strValue);
        strItem = "fdata1_val";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].fData1_val = atof(strValue);
		strItem = "cond";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].cond = atoi(strValue);
		strItem = "data2_type";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].data2_type = atoi(strValue);
		strItem = "data2_val";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].iData2_val = atoi(strValue);
        strItem = "fdata2_val";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].fData2_val = atof(strValue);
		strItem = "Digit";   strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].digit = atoi(strValue);
        strItem = "Div";   strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].div = atoi(strValue);
		for (int j = 0; j < 4; j++) {
			strItem.Format("range%d", j);
			strValue.Format("%d", m_GraphLog[i].range[j]); 
			strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].range[j] = atoi(strValue);
		}

		strItem = "color1";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].color1 = atoi(strValue);
		strItem = "color2";  strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].color2 = atoi(strValue);
		strItem = "MEMO"; m_GraphLogMemo[i] = GetReg_RegistryData(strAddress, strItem);
		strItem = "PreText";  m_GraphPreText[i] = GetReg_RegistryData(strAddress, strItem);
		strItem = "PostText";  m_GraphPostText[i] = GetReg_RegistryData(strAddress, strItem);

		strItem = "UseMask"; strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].use_mask = atoi(strValue);
		strItem = "Mask_val"; strValue = GetReg_RegistryData(strAddress, strItem); m_GraphLog[i].mask_val = atoi(strValue);
	}
}


void CSerialComDlg::UpdateGraphLogData(BOOL bInit) {
	static int pre_idx = -1;
	static BOOL bBusy = FALSE;
	if (m_pUserScreen == nullptr) return;
	if (bInit) { 
		pre_idx = -1;
	}
	if (m_bShowUS == FALSE) return;	
	if (m_pUserScreen->m_bReqUpdateDrawData) return;
	
	if (bBusy) {
		return;
	}
	Wait(0);
	if (bBusy) {
		return;
	}

	if (m_cGraphLogHeader != m_cPacketHead) {
		m_cGraphLogHeader = m_cPacketHead;
		// Load GraphHistorySetting Info
		GetGraphLogData();
		pre_idx = -1;

		if (m_iGraphLogCount > 0) {
			if (m_pUserScreen) {
				m_pUserScreen->DestroyWindow();
				delete m_pUserScreen;
				m_pUserScreen = nullptr;
			}
			OnBnClickedShowgraphlog();
			OnBnClickedShowgraphlog();
		}
		else {
			m_bShowUS = FALSE;
			CheckDlgButton(IDC_ShowGraphLog, FALSE);
			m_pUserScreen->m_bReqUpdateDrawData = TRUE;
			return;
		}
		m_pUserScreen->m_bReqUpdateDrawData = TRUE;
	}

	int idx;

	if (m_pUserScreen->m_iReqReplayIndex != -1) {
		idx = max(0, min(m_pUserScreen->m_iReqReplayIndex, m_iHistoryIndex - 1));
		SetDlgItemInt(IDC_IndexNumber, m_pUserScreen->m_iReqReplayIndex);
		m_pUserScreen->m_iReqReplayIndex = -1;
	}
	else if (m_pUserScreen->m_bReqChangeIndex) {
		idx = max(0, min(m_pUserScreen->m_iCurrentIndexTemp, m_iHistoryIndex - 1));
	}
	else {
		if (m_iCurrentIndex == pre_idx) {
			return;
		}
		pre_idx = m_iCurrentIndex;
		idx = max(0, min(m_iCurrentIndex, m_iHistoryIndex - 1));
	}
	bBusy = TRUE;

	CString str;		
	for (int i = 0; i < m_iGraphLogCount; i++) {
		if (m_GraphLog[i].use == FALSE) {
			m_pUserScreen->m_DrawData[i].use = FALSE;
			continue;
		}
		else {
			m_pUserScreen->m_DrawData[i].use = TRUE;
		}

		double v1, v2;

		// Data1
		switch (m_GraphLog[i].data1_type) {
		case 0: v1 = m_GraphLog[i].fData1_val; break;
		case 1: v1 = m_HistoryData[idx].fDataValue[m_GraphLog[i].iData1_val - 1];  break;
		case 2: v1 = m_HistoryData[idx].fMathValue[m_GraphLog[i].iData1_val - 1];  break;
		}

        switch (m_GraphLog[i].data2_type) {
        case 0: v2 = m_GraphLog[i].fData2_val; break;
        case 1: v2 = m_HistoryData[idx].fDataValue[m_GraphLog[i].iData2_val - 1];  break;
        case 2: v2 = m_HistoryData[idx].fMathValue[m_GraphLog[i].iData2_val - 1];  break;
        }
		if (m_GraphLog[i].use_mask) {
            int m1 = (int)v1;
            int m2 = (int)v2;
			int data1 = 0;
            int data2 = 0;

			int mask = m_GraphLog[i].mask_val;
			for (int i = 0; i < 16; i++) {
				if (mask & (0x01 << i)) {
					data1 <<= 1;
                    data2 <<= 1;
					if (m1 & (0x01 << i)) {	data1 += 1;	}
                    if (m2 & (0x01 << i)) { data2 += 1; }
				}
			}
			v1 = (double)data1;
            v2 = (double)data2;
		}

		int cond = m_GraphLog[i].cond;

		int item_type = m_GraphLog[i].item_type;
		m_pUserScreen->m_DrawData[i].color1 = m_GraphLog[i].color1;
		m_pUserScreen->m_DrawData[i].color2 = m_GraphLog[i].color2;

		m_pUserScreen->m_DrawData[i].range[0] = m_GraphLog[i].range[0];
		m_pUserScreen->m_DrawData[i].range[1] = m_GraphLog[i].range[1];
		m_pUserScreen->m_DrawData[i].range[2] = m_GraphLog[i].range[0] + m_GraphLog[i].range[2];
		m_pUserScreen->m_DrawData[i].range[3] = m_GraphLog[i].range[1] + m_GraphLog[i].range[3];

		m_pUserScreen->m_DrawData[i].type = item_type;
		if (item_type <= 2) {
			BOOL check = FALSE;
			switch (cond) {
			case 0: if (v1 > v2) check = TRUE; break;
			case 1: if (v1 >= v2) check = TRUE; break;
			case 2: if (v1 == v2) check = TRUE; break;
			case 3: if (v1 != v2) check = TRUE; break;
			case 4: if (v1 < v2) check = TRUE; break;
			case 5: if (v1 <= v2) check = TRUE; break;
			}

			if (check == FALSE) {
				if (item_type == 0) {
					m_pUserScreen->m_DrawData[i].color1 = m_GraphLog[i].color2;
				}
				else {
					m_pUserScreen->m_DrawData[i].use = FALSE;
					continue;
				}
			}
		}
        
		if (item_type > 0) {
			int t;
			switch (item_type) {
			case 1:
			case 3:
				str.Format("%s", (LPCTSTR)m_GraphPreText[i]); break;
			case 2: 
			case 4:
                switch (m_GraphLog[i].div) {
                case 0: break;
                case 1: v1 *= 0.1; break;
                case 2: v1 *= 0.01; break;
                case 3: v1 *= 0.001; break;
                case 4: v1 *= 0.0001; break;
                case 5: v1 *= 0.00001; break;
                }

                if (v1 >= 0) {
                    switch (m_GraphLog[i].digit) {
                    case 0: str.Format("%s%d%s", (LPCTSTR)m_GraphPreText[i], (int)(v1 + 0.5), (LPCTSTR)m_GraphPostText[i]); break;
                    case 1: str.Format("%s%.1f%s", (LPCTSTR)m_GraphPreText[i], v1 + 0.05, (LPCTSTR)m_GraphPostText[i]); break;
                    case 2: str.Format("%s%.2f%s", (LPCTSTR)m_GraphPreText[i], v1 + 0.005, (LPCTSTR)m_GraphPostText[i]); break;
                    case 3: str.Format("%s%.3f%s", (LPCTSTR)m_GraphPreText[i], v1 + 0.0005, (LPCTSTR)m_GraphPostText[i]); break;
                    case 4: str.Format("%s%.4f%s", (LPCTSTR)m_GraphPreText[i], v1 + 0.00005, (LPCTSTR)m_GraphPostText[i]); break;
                    case 5: str.Format("%s%.5f%s", (LPCTSTR)m_GraphPreText[i], v1 + 0.000005, (LPCTSTR)m_GraphPostText[i]); break;
                    }
                }
                else {
                    switch (m_GraphLog[i].digit) {
                    case 0: str.Format("%s%d%s", (LPCTSTR)m_GraphPreText[i], (int)(v1 - 0.5), (LPCTSTR)m_GraphPostText[i]); break;
                    case 1: str.Format("%s%.1f%s", (LPCTSTR)m_GraphPreText[i], v1 - 0.05, (LPCTSTR)m_GraphPostText[i]); break;
                    case 2: str.Format("%s%.2f%s", (LPCTSTR)m_GraphPreText[i], v1 - 0.005, (LPCTSTR)m_GraphPostText[i]); break;
                    case 3: str.Format("%s%.3f%s", (LPCTSTR)m_GraphPreText[i], v1 - 0.0005, (LPCTSTR)m_GraphPostText[i]); break;
                    case 4: str.Format("%s%.4f%s", (LPCTSTR)m_GraphPreText[i], v1 - 0.00005, (LPCTSTR)m_GraphPostText[i]); break;
                    case 5: str.Format("%s%.5f%s", (LPCTSTR)m_GraphPreText[i], v1 - 0.000005, (LPCTSTR)m_GraphPostText[i]); break;
                    }                    
                }
				break;
			case 5:
				switch (m_GraphLog[i].digit) {					
				case 0: // 현재 Data 수신 시간
					t = m_HistoryData[idx].iRcvTime * 1000 + m_HistoryData[idx].iRcvTime_ms;
					str.Format("%s%02d:%02d:%02d.%03d%s", (LPCTSTR)m_GraphPreText[i], t/3600000, (t / 60000) % 60, (t/1000) % 60, t%1000, (LPCTSTR)m_GraphPostText[i]); break;

				case 1: // 최초 수신 data로부터 경과시간
					t = m_HistoryData[idx].iRcvTime * 1000 + m_HistoryData[idx].iRcvTime_ms;
					t -= m_HistoryData[0].iRcvTime * 1000 + m_HistoryData[0].iRcvTime_ms;
					str.Format("%s%02d:%02d:%02d.%03d%s", (LPCTSTR)m_GraphPreText[i], t / 3600000, (t / 60000) % 60, (t / 1000) % 60, t % 1000, (LPCTSTR)m_GraphPostText[i]); break;

				case 2:  // 현재 Data Index
					str.Format("%s%d%s", (LPCTSTR)m_GraphPreText[i], idx, (LPCTSTR)m_GraphPostText[i]);
					break;

				default: str.Format("Not Provided Item");
				}
			}
			int n = str.GetLength();
			int posx = m_GraphLog[i].range[0] + m_GraphLog[i].range[2] / 2 - n * 38 / 10;
			int posy = m_GraphLog[i].range[1] + m_GraphLog[i].range[3] / 2 - 6;
			m_pUserScreen->m_DrawData[i].strText.Format("%s", (LPCTSTR)str);
			m_pUserScreen->m_DrawData[i].ptText.SetPoint(posx, posy);
		}
	}
	m_pUserScreen->m_iIndex = idx;
	m_pUserScreen->m_bReqUpdateDrawData = TRUE;
	bBusy = FALSE;
}

void CSerialComDlg::OnBnClickedShowgraphlog()
{
	if (IsDlgButtonChecked(IDC_ShowGraphLog)) {
		if (m_iGraphLogCount == 0) {
			m_bShowUS = FALSE;
			CheckDlgButton(IDC_ShowGraphLog, FALSE);
			AfxMessageBox("[E] Graph Log 설정이 없습니다(No Graph Log settings found.).");
		}
		else {
			if (m_pUserScreen == nullptr) {
				m_pUserScreen = new CUserScreenDlg(this);
				m_pUserScreen->pMain = this;

				if (!m_pUserScreen->Create(IDD_UserScreen, this)) {
					AfxMessageBox("Error in Create Window");
					delete m_pUserScreen;
					m_pUserScreen = nullptr;
					m_bShowUS = FALSE;
					CheckDlgButton(IDC_ShowGraphLog, FALSE);
					return;
				}
			}
			m_bShowUS = TRUE;
			m_pUserScreen->ShowWindow(SW_SHOW);
		}
	}
	else {
		m_bShowUS = FALSE;
		m_pUserScreen->ShowWindow(SW_HIDE);
	}
}


void CSerialComDlg::OnBnClickedShowtextlog()
{
	if (IsDlgButtonChecked(IDC_ShowTextLog)) {
		if (m_iTextLogCount == 0) {
			m_bShowTS = FALSE;
			CheckDlgButton(IDC_ShowTextLog, FALSE);
			AfxMessageBox("[E] Text Log 설정이 없습니다.(No Text Log settings found.)");
		}
		else {
			if (m_pTextScreen == nullptr) {
				m_pTextScreen = new CTextLogDlg(this);
				m_pTextScreen->pMain = this;

				if (!m_pTextScreen->Create(IDD_TextLog, this)) {
					AfxMessageBox("Error in Create Window");
					delete m_pTextScreen;
					m_pTextScreen = nullptr;
					m_bShowTS = FALSE;
					CheckDlgButton(IDC_ShowTextLog, FALSE);
					return;
				}
			}
			m_bShowTS = TRUE;
			m_pTextScreen->ShowWindow(SW_SHOW);
		}
	}
	else {
		m_bShowTS = FALSE;
		m_pTextScreen->ShowWindow(SW_HIDE);
	}
}


void CSerialComDlg::GetTextLogData() {
	m_iTextLogCount = GetTextLogCount(m_cPacketHead);

	for (int i = 0; i < m_iTextLogCount; i++) {
		GetHeaderLogFromRegistry(m_cPacketHead, i, &m_TextLog[i]);
	}
}




void CSerialComDlg::UpdateTextLogData(BOOL bInit) {
	static BYTE cHeader = ' ';
	static int idx = -1;
	if (bInit) {
		idx = -1;
	}
	if (m_bShowTS == FALSE) return;

	if (cHeader != m_cPacketHead) {
		cHeader = m_cPacketHead;
		GetTextLogData();
		idx = -1;
		if (m_iTextLogCount == 0) {
			m_bShowTS = FALSE;

			m_pTextScreen->ShowWindow(SW_HIDE);
			CheckDlgButton(IDC_ShowTextLog, FALSE);
			return;
		}		
	}

	// Calc Index 
	if (m_iCurrentIndex == idx) {
		return;
	}

	CString str{ "" };
	CString strLog{ "" };
    CString strVal{ "" };
	idx = m_iCurrentIndex;
	int iDataVal;
    double fDataVal;
	int output_val;
	int n_shift;

	str.Format("Header:%c    CurrentIndex:%d", m_cPacketHead, m_iCurrentIndex);
	m_pTextScreen->SetDlgItemText(IDC_InfoTextLog, str);

	bool res = false;

	for (int i = 0; i < m_iTextLogCount; i++) {
		if (m_TextLog[i].use == FALSE) {
			continue;
		}
		str = "";

		switch (m_TextLog[i].item_type) {
		case eLogCondition_Blank:
			if (strLog != ""  || m_TextLog[i].strComment1 != "") {
				str.Format("%s\r\n", (LPCTSTR)m_TextLog[i].strComment1);
			}
			break;


		case eLogCondition_MatchMask:
			if (m_TextLog[i].data_type == 0) {
				iDataVal = (int)m_HistoryData[idx].fDataValue[m_TextLog[i].data_id];                
			}
			else {
                iDataVal = (int)m_HistoryData[idx].fMathValue[m_TextLog[i].data_id];
			}
            iDataVal = (int)fDataVal;
			if ((iDataVal & m_TextLog[i].mask_val) == m_TextLog[i].ref_iVal) {
				if (m_TextLog[i].show1) {
					str.Format("%s\r\n", (LPCTSTR)m_TextLog[i].strComment1);
				}
			}
			else {
				if (m_TextLog[i].show2) {
					str.Format("%s\r\n", (LPCTSTR)m_TextLog[i].strComment2);
				}
			}
			break;

		case eLogCondition_ShowValue:
			if (m_TextLog[i].data_type == 0) {
				fDataVal = m_HistoryData[idx].fDataValue[m_TextLog[i].data_id];
			}
			else {
				fDataVal = m_HistoryData[idx].fMathValue[m_TextLog[i].data_id];
			}

            switch (m_TextLog[i].divide) {
            case 0: break;
            case 1: fDataVal *= 0.1; break;
            case 2: fDataVal *= 0.01; break;
            case 3: fDataVal *= 0.001; break;
            case 4: fDataVal *= 0.0001; break;
            case 5: fDataVal *= 0.00001; break;
            }

            switch (m_TextLog[i].dp) {
            case 0: strVal.Format("%.0f", fDataVal); break;
            case 1: strVal.Format("%.1f", fDataVal); break;
            case 2: strVal.Format("%.2f", fDataVal); break;
            case 3: strVal.Format("%.3f", fDataVal); break;
            }

			if (m_TextLog[i].show1) {
				str.Format("%d %s\r\n", strVal, (LPCTSTR)m_TextLog[i].strComment1);
			}
			else {
				str.Format("%s %s\r\n", (LPCTSTR)m_TextLog[i].strComment1, (LPCTSTR)strVal);
			}
			break;

		case eLogCondition_MaskedValue:
			if (m_TextLog[i].data_type == 0) {
				iDataVal = (int)m_HistoryData[idx].fDataValue[m_TextLog[i].data_id];
			}
			else {
				iDataVal = (int)m_HistoryData[idx].fMathValue[m_TextLog[i].data_id];
			}
			output_val = 0;
			n_shift = 0;

			for (int j = 0; j < 16; j++) {
				if (m_TextLog[i].mask_val & (0x01 << j)) {
					if (iDataVal & (0x01 << j)) {
						output_val += (0x01 << n_shift);
					}
					n_shift++;
				}
			}

			if (m_TextLog[i].show1) {
				str.Format("%d %s\r\n", output_val * m_TextLog[i].ref_iVal, (LPCTSTR)m_TextLog[i].strComment1);
			}
			else {
				str.Format("%s %d\r\n", (LPCTSTR)m_TextLog[i].strComment1, output_val * m_TextLog[i].ref_iVal);
			}
			break;

		default:
			if (m_TextLog[i].data_type == 0) {
				fDataVal = m_HistoryData[idx].fDataValue[m_TextLog[i].data_id];
			}
			else {
				fDataVal = m_HistoryData[idx].fMathValue[m_TextLog[i].data_id];
			}

            switch (m_TextLog[i].divide) {
            case 0: break;
            case 1: fDataVal *= 0.1; break;
            case 2: fDataVal *= 0.01; break;
            case 3: fDataVal *= 0.001; break;
            case 4: fDataVal *= 0.0001; break;
            case 5: fDataVal *= 0.00001; break;
            }            

			if (m_TextLog[i].item_type == eLogCondition_Greater) {
				if (fDataVal > m_TextLog[i].ref_fVal) res = true;
			}
			else if(m_TextLog[i].item_type == eLogCondition_GreaterEqual) {
				if (fDataVal >= m_TextLog[i].ref_fVal) res = true;
			}
			else if (m_TextLog[i].item_type == eLogCondition_Less) {
				if (fDataVal < m_TextLog[i].ref_fVal) res = true;
			}
			else if (m_TextLog[i].item_type == eLogCondition_LessEqual) {
				if (fDataVal <= m_TextLog[i].ref_fVal) res = true;
			}
			if (res) {
				if (m_TextLog[i].show2) {
                    switch (m_TextLog[i].dp) {
                    case 0: strVal.Format("%.0f", fDataVal); break;
                    case 1: strVal.Format("%.1f", fDataVal); break;
                    case 2: strVal.Format("%.2f", fDataVal); break;
                    case 3: strVal.Format("%.3f", fDataVal); break;
                    }
					str.Format("%s%s\r\n", (LPCTSTR)m_TextLog[i].strComment1, (LPCTSTR)strVal, (LPCTSTR)m_TextLog[i].strComment2);
				}
				else {
					str.Format("%s%s\r\n", (LPCTSTR)m_TextLog[i].strComment1, (LPCTSTR)m_TextLog[i].strComment2);
				}
			}
			break;
		}

        CString strTemp = strLog;
		strLog.Format("%s%s", (LPCTSTR)strTemp, (LPCTSTR)str);
	}

	m_pTextScreen->SetDlgItemText(IDC_LogData, strLog);
}

void CSerialComDlg::CheckWheelAcc() {
	if (m_iWheelCount > 1) {
		if (m_iWheelAcc < 50) {
			m_iWheelAcc += 3;
		}
	}
	else { 
		m_iWheelAcc = 10;
	}
	m_iWheelCount = 0;
}

void CSerialComDlg::OnBnClickedGraphupdate() {
	m_bUseGraphUpdate = IsDlgButtonChecked(IDC_GraphUpdate);
}

void CSerialComDlg::GenerateMathDataFromRcvData() {
	int iComparedData1, iComparedData2;
	double fAverage, fSS;
	double data1, data2;
	for (int iChannel = 0; iChannel < m_iMaxMathSize; iChannel++) {
		
		if (m_MathData[iChannel].bValid) {
			if (m_MathData[iChannel].iDataID1 == m_iMaxDataSize + iChannel) {  // 상수
				m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = m_MathData[iChannel].fValue;
			}
			else {
				if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize)
					data1 = m_HistoryData[m_iHistoryIndex].fDataValue[m_MathData[iChannel].iDataID1];
				else if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize + iChannel)
					data1 = m_HistoryData[m_iHistoryIndex].fMathValue[m_MathData[iChannel].iDataID1 - m_iMaxDataSize];
				else
					data1 = 0.;
				
				if (m_MathData[iChannel].iDataID2 < m_iMaxDataSize)
					data2= m_HistoryData[m_iHistoryIndex].fDataValue[m_MathData[iChannel].iDataID2];
				else if (m_MathData[iChannel].iDataID2 < m_iMaxDataSize + iChannel)
					data2 = m_HistoryData[m_iHistoryIndex].fMathValue[m_MathData[iChannel].iDataID2 - m_iMaxDataSize];
				else if (m_MathData[iChannel].iDataID2 == m_iMaxDataSize + iChannel)
					data2 = m_MathData[iChannel].fValue;
				else
					data2 = 0.;


				switch (m_MathData[iChannel].iOperator) {

				case Operator_Add:  // Add
					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = data1 + data2;
					break;

				case Operator_Substract:  // Substract
					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = data1 - data2;
					break;

				case Operator_Multiply:  // Multiply
					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = data1 * data2;
					break;

				case Operator_Divide:  // Divide
					if (data2 > 0.000001 || data2 < -0.000001) {
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = data1 / data2;
					}
					else {
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 0.;
					}
					break;

				case Operator_Exponential:  // Exponential
					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = pow(data1, data2);
					break;

				case Operator_EqualTo: // Equal To
					if (abs(data1 - data2) < 0.000001)
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 1;
					else
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 0;
					break;

				case Operator_GreaterThan: // Greater than
					if (data1 > data2)
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 1;
					else
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 0;
					break;

				case Operator_LessThan: // Less Than
					if (data1 < data2)
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 1;
					else
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 0;
					break;

				case Operator_Remainder: // Remainder
					iComparedData1 = (int)(data1 + 0.5);
					iComparedData2 = (int)(data2 + 0.5);

					if (iComparedData2 <= 0)iComparedData2 = max(1, -iComparedData2);

					if (iComparedData1 < 0) {
						int iMultiply = -iComparedData1 / iComparedData2;
						iComparedData1 += iMultiply * iComparedData2;

						if (iComparedData1 < 0) iComparedData1 += iComparedData2;
						if (iComparedData1 < 0) iComparedData1 += iComparedData2;
					}

					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = iComparedData1 % iComparedData2;
					break;

				case Operator_Average:  // Average					
					if (++(m_MathData[iChannel].index) >= m_MathData[iChannel].nData) m_MathData[iChannel].index = 0;
					m_MathData[iChannel].fSum -= m_MathData[iChannel].fSaveData[m_MathData[iChannel].index];
					m_MathData[iChannel].fSum += data1;
                    m_MathData[iChannel].fSaveData[m_MathData[iChannel].index] = data1;

					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = m_MathData[iChannel].fSum / m_MathData[iChannel].nData;
					break;

				case Operator_RMS:  // RMS
					data1 = data1 * data1;
					if (++(m_MathData[iChannel].index) >= m_MathData[iChannel].nData) m_MathData[iChannel].index = 0;
					m_MathData[iChannel].fSum -= m_MathData[iChannel].fSaveData[m_MathData[iChannel].index];
					m_MathData[iChannel].fSum += data1;
					m_MathData[iChannel].fSaveData[m_MathData[iChannel].index] = data1;

					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = (sqrt(m_MathData[iChannel].fSum / m_MathData[iChannel].nData));
					break;

				case Operator_Stdev:  // Stdev
					if (++(m_MathData[iChannel].index) >= m_MathData[iChannel].nData) m_MathData[iChannel].index = 0;
					m_MathData[iChannel].fSum -= m_MathData[iChannel].fSaveData[m_MathData[iChannel].index];
					m_MathData[iChannel].fSum += data1;
					m_MathData[iChannel].fSaveData[m_MathData[iChannel].index] = data1;

					fAverage = m_MathData[iChannel].fSum / m_MathData[iChannel].nData;
					fSS = 0.;
					for (int k = 0; k < m_MathData[iChannel].nData; k++) {
                        fSS += pow(fAverage - m_MathData[iChannel].fSaveData[k], 2);
					}
					m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = sqrt(fSS / m_MathData[iChannel].nData);

                    if (m_HistoryData[max(0, m_iHistoryIndex-1)].fMathValue[iChannel] < 0.0001) {
                        CString Error;
                        Error.Format("%f %f - %f", fSS, m_HistoryData[m_iHistoryIndex].fMathValue[iChannel], fAverage);
                        Error = "";
                    }
					break;

				case Operator_Change: //Delta
					if (m_MathData[iChannel].nData > m_iHistoryIndex)
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 0;
					else {
						if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize) {
							m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = m_HistoryData[m_iHistoryIndex].fDataValue[m_MathData[iChannel].iDataID1] - m_HistoryData[m_iHistoryIndex - m_MathData[iChannel].nData].fDataValue[m_MathData[iChannel].iDataID1];
						}
						else if (m_MathData[iChannel].iDataID1 < m_iMaxDataSize + iChannel) {
							m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = m_HistoryData[m_iHistoryIndex].fMathValue[m_MathData[iChannel].iDataID1 - m_iMaxDataSize] - m_HistoryData[m_iHistoryIndex - m_MathData[iChannel].nData].fMathValue[m_MathData[iChannel].iDataID1 - m_iMaxDataSize];
						}
						else {
							m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = 0;
						}
					}
					break;

				case Operator_ABS: // abs
					if (data1 > 0)
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = data1;
					else
						m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = -data1;
					break;
				}
                if (m_MathData[iChannel].bResInt) {
                    m_HistoryData[m_iHistoryIndex].fMathValue[iChannel] = (int)m_HistoryData[m_iHistoryIndex].fMathValue[iChannel];
                }
			}
			m_MathData[iChannel].iValue = m_HistoryData[m_iHistoryIndex].fMathValue[iChannel];
		}
	}
}

BOOL CSerialComDlg::Check_XinputData(XINPUT_GAMEPAD& Data, CString& str) {
	BOOL ret = FALSE;
	ret |= NoiseRemove_Xinput_Byte(Data.bLeftTrigger, str, "LT");
	ret |= NoiseRemove_Xinput_Byte(Data.bRightTrigger, str, "RT");

	ret |= NoiseRemove_Xinput_Short(Data.sThumbLX, str, "TLX");
	ret |= NoiseRemove_Xinput_Short(Data.sThumbLY, str, "TLY");
	ret |= NoiseRemove_Xinput_Short(Data.sThumbRX, str, "TRX");
	ret |= NoiseRemove_Xinput_Short(Data.sThumbRY, str, "TRY");

	if (Data.wButtons > 0) {
		ret = TRUE;
        CString strTemp = str;
		str.Format("%s/BTN-", (LPCTSTR)strTemp);

		if (Data.wButtons & XINPUT_GAMEPAD_START)
		{
            CString str1 = str;
			str.Format("%sS,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_A)
		{
            CString str1 = str;
			str.Format("%sA,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_B)
		{
            CString str1 = str;
			str.Format("%sB,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_X)
		{
            CString str1 = str;
			str.Format("%sX,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_Y)
		{
            CString str1 = str;
			str.Format("%sY,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
            CString str1 = str;
			str.Format("%sLB,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
            CString str1 = str;
			str.Format("%sRB,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_BACK)
		{
            CString str1 = str;
			str.Format("%sBK,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
            CString str1 = str;
			str.Format("%sU,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
            CString str1 = str;
			str.Format("%sD,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
            CString str1 = str;
			str.Format("%sL,", (LPCTSTR)str1);
		}
		if (Data.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
            CString str1 = str;
			str.Format("%sR,", (LPCTSTR)str1);
		}
	}
	return ret;
}


BYTE* CSerialComDlg::Make_XInput_UartData_Data(const XINPUT_GAMEPAD& pad, int* pSize) {       
	static BYTE arr[100]; 
	BOOL bActive = TRUE;
    int idx = 0;    

    // Enable Key
    if (m_XInputSetting.bUseEnableKey == TRUE) {
        bActive = FALSE;
        if (m_XInputSetting.iEnableKeyMode == 0) {
            if (((m_XInputSetting.bUseLB_EnableKey == FALSE) || (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) \
                && ((m_XInputSetting.bUseRB_EnableKey == FALSE) || (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))) {
                bActive = TRUE;
            }
        }
        else {
            if ((pad.bLeftTrigger >= m_XInputSetting.iLT_Threshold_EnableKey) && (pad.bRightTrigger >= m_XInputSetting.iRT_Threshold_EnableKey)) {
                bActive = TRUE;
            }
        }
    }
	m_bXInput_EnableKey = bActive;
    if (bActive == FALSE && m_XInputSetting.iSendType == 0) { 
		*pSize = 0;
		return arr; 
	}

	// Header
	int nHeader = 0;
	if (m_XInputSetting.bUseHeader) {
		nHeader = m_XInputSetting.nHeader;
		for (int i = 0; i < nHeader; i++) {
			arr[idx++] = m_XInputSetting.header[i];
		}
	}

    if (m_XInputSetting.iSendType == 0) {
        for (int i = 0; i < 8; i++) {
            if (m_XInputSetting.bUseSendData[i] == FALSE) continue;
            if (i < 4 && m_XInputSetting.iSendThreshold[i] == 0) continue;
            short val = 0;
            WORD mask = 0;

            switch (i) {
            case 0: val = pad.sThumbLX; break; //LX
            case 1: val = pad.sThumbLY; break; //LY
            case 2: val = pad.sThumbRX; break; //RX
            case 3: val = pad.sThumbRY; break; //RY
            case 4: mask = XINPUT_GAMEPAD_A; break;
            case 5: mask = XINPUT_GAMEPAD_B; break;
            case 6: mask = XINPUT_GAMEPAD_X; break;
            case 7: mask = XINPUT_GAMEPAD_Y; break;
            }

            if ((i < 4 && val >= m_XInputSetting.iSendThreshold[i]) || (mask & pad.wButtons)) {
                int sz = m_XInputSetting.strSendData[i].GetLength();
                for (int j = 0; j < sz; j++) {
                    char ch = (m_XInputSetting.strSendData[i]).GetAt(j);
                    arr[idx++] = ch;
                }
            }
            else if (i < 4 && val <= -m_XInputSetting.iSendThreshold[i]) {
                int sz = m_XInputSetting.strSendData_negative[i].GetLength();
                for (int j = 0; j < sz; j++) {
                    char ch = (m_XInputSetting.strSendData_negative[i]).GetAt(j);
                    arr[idx++] = ch;
                }
            }
        }
    }    
    else {
        // Send Packet
        if (m_XInputSetting.bUseSendData[0]) { if (bActive) *((short *)&arr[idx]) = pad.sThumbLX;  else *((short *)&arr[idx]) = 0;   idx += sizeof(short); }
        if (m_XInputSetting.bUseSendData[1]) { if (bActive) *((short *)&arr[idx]) = pad.sThumbLY;  else *((short *)&arr[idx]) = 0;  idx += sizeof(short); }
        if (m_XInputSetting.bUseSendData[2]) { if (bActive) *((short *)&arr[idx]) = pad.sThumbRX;  else *((short *)&arr[idx]) = 0;  idx += sizeof(short); }
        if (m_XInputSetting.bUseSendData[3]) { if (bActive) *((short *)&arr[idx]) = pad.sThumbRY;  else *((short *)&arr[idx]) = 0;  idx += sizeof(short); }

        if (m_XInputSetting.bUseSendButton) {
            if (bActive) *((WORD *)&arr[idx]) = pad.wButtons; else *((WORD *)&arr[idx]) = 0;
            idx += sizeof(WORD);
        }

		if (m_XInputSetting.bUseChecksum) {
			BYTE checksum = 0;
			for (int i = 0; i < idx; i++) {
				checksum += arr[i];
			}
			arr[idx++] = (BYTE)(0xFF - checksum);
			nHeader += 1;
		}
    }

	if (idx > nHeader) {
		*pSize = idx;
	}
	else {
		*pSize = 0;
	}
	return arr;
}

void CSerialComDlg::Send_UartData_Xinput(const XINPUT_GAMEPAD& pad) {
	int size = 0;
	BYTE *arr = Make_XInput_UartData_Data(pad, &size);

	if (size > 0 && m_ComuPort.m_bConnected == TRUE) {
		m_ComuPort.WriteComm(arr, size);
		if (m_XInputSetting.bShowUartSend) {
			CString str;
			str.Format("x_send:%dbtyte (%s)", size, XInput_SendData_hex(arr, size));
			SetDlgItemText(IDC_Temp, str);
		}
	}
	else if (m_XInputSetting.bShowUartSend) {
		SetDlgItemText(IDC_Temp, "");
	}
}

char*  CSerialComDlg::XInput_SendData_hex(BYTE *arr, int size) {
	static char buf[100];
	int idx = 0;

	for (int i = 0; i < size; i++) {
		int sz = snprintf(&(buf[idx]), 100 - idx, "%02X ", arr[i]);
		idx += sz;
	}
	buf[idx] = 0;
	return buf;
}

BOOL CSerialComDlg::NoiseRemove_Xinput_Byte(BYTE& Data, CString& str, const char *s) {
	// BYTE : 0~255
	BYTE th = 10;
	if (Data < th) {
		Data = 0;
		return FALSE;
	}
    CString str1 = str;
	str.Format("%s/%s:%d", (LPCTSTR)str1, s, Data);
	return TRUE;

}

BOOL CSerialComDlg::NoiseRemove_Xinput_Short(short& Data, CString& str, const char *s) {
	// short range : -32768 ~ 32767	
	short th = 300;  //  zero input ragne : -2000 ~ 2000
					 //  v_0 = acos(1 - th * 2 / 32768) * 32767 / M_PI	
	int max_val = 32767;
	int mag = max_val + 2 * th;
	int out;
	if (Data >= 0) {
		out = (int)((1 - cos(Data * M_PI / max_val)) / 2 * mag);
	}
	else {
		out = (int)((cos(Data * M_PI / max_val) - 1) / 2 * mag);
	}

	if (out > th) { out -= th; }
	else if (out < -th) { out += th; }
	else { out = 0; }

	if (out > max_val) {
		out = max_val;
	}
	if (out < -max_val) {
		out = -max_val;
	}

	Data = (short)out;
	if (Data == 0) return FALSE;
    CString str1 = s;
	str.Format("%s/%s:%d", (LPCTSTR)str1, s, Data);
	return TRUE;
}

void CSerialComDlg::GetSendConfig_XInput() {
	CString strAddr, strValue = "";
    // Type
    strAddr = "Common\\Xinput";
    strValue = GetRegRoot_RegistryData(strAddr, "SendType");
    m_XInputSetting.iSendType = atoi(strValue);

    // Checksum
    strValue = GetRegRoot_RegistryData(strAddr, "Use_Checksum");
    m_XInputSetting.bUseChecksum = atoi(strValue);

    // Header
    strAddr = "Common\\Xinput\\Header";
    strValue = GetRegRoot_RegistryData(strAddr, "Use_Header");
    m_XInputSetting.bUseHeader = atoi(strValue);

    strValue = GetRegRoot_RegistryData(strAddr, "Header");
    Update_XInput_Header(strValue, &m_XInputSetting);

	// EnableKey
	strAddr = "Common\\Xinput\\EnableKey";
	strValue = GetRegRoot_RegistryData(strAddr, "EnbaleKey");
    if (strValue == "") { strValue = "1"; }
    m_XInputSetting.bUseEnableKey = atoi(strValue);    

	strValue = GetRegRoot_RegistryData(strAddr, "EnbaleKeyMode");
    if (strValue == "") { strValue = "0"; }
    m_XInputSetting.iEnableKeyMode = atoi(strValue);

    strValue = GetRegRoot_RegistryData(strAddr, "UseLB_EnbaleKey");
    if (strValue == "") { strValue = "0"; }
    m_XInputSetting.bUseLB_EnableKey = atoi(strValue);
    strValue = GetRegRoot_RegistryData(strAddr, "UseRB_EnbaleKey");
    if (strValue == "") { strValue = "0"; }
    m_XInputSetting.bUseRB_EnableKey = atoi(strValue);

    strValue = GetRegRoot_RegistryData(strAddr, "LT_Threshold_EnbaleKey");
    if (strValue == "") { strValue = "0"; }
    m_XInputSetting.iLT_Threshold_EnableKey = atoi(strValue);
    strValue = GetRegRoot_RegistryData(strAddr, "RT_Threshold_EnbaleKey");
    if (strValue == "") { strValue = "0"; }
    m_XInputSetting.iRT_Threshold_EnableKey = atoi(strValue);

	// Send Char	
	for (int i = 0; i < 8; i++) {
		strAddr.Format("Common\\Xinput\\SendData\\item%d", i);
        strValue = GetRegRoot_RegistryData(strAddr, "UseSendData");
        if (strValue == "") { strValue = "0"; }
        m_XInputSetting.bUseSendData[i] = atoi(strValue);

        m_XInputSetting.strSendData[i].Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddr, "SendData"));
        
		if (i < 4) {
            m_XInputSetting.strSendData_negative[i].Format("%s", (LPCTSTR)GetRegRoot_RegistryData(strAddr, "SendData_Negative"));

            strValue = GetRegRoot_RegistryData(strAddr, "Threshold");
            if (strValue == "") { strValue = "0"; }
            m_XInputSetting.iSendThreshold[i] = atoi(strValue);
		}
	}

	// Add Button Info in Packet Data
	strAddr.Format("Common\\Xinput\\SendData");
	strValue = GetRegRoot_RegistryData(strAddr, "UseButtonData");
	m_XInputSetting.bUseSendButton = atoi(strValue);

    // Update 주기설정
    strAddr.Format("Common\\Xinput");
    strValue = GetRegRoot_RegistryData(strAddr, "UpdatePeriod");
    if (strValue == "") { strValue = "1000"; }
    m_XInputSetting.iUpdatePeriod = atoi(strValue);
    SetTimer(TIMER_XINPUT_CHECK, m_XInputSetting.iUpdatePeriod, NULL);

	// Show Uart Send Data in Staus window
	strAddr.Format("Common\\Xinput\\SendData");
	strValue = GetRegRoot_RegistryData(strAddr, "Show_UartSend");
	m_XInputSetting.bShowUartSend = atoi(strValue);
}

int CSerialComDlg::Update_XInput_Header(const CString& str, XINPUT_SETTING_T* m_XInputSetting)
{
    BYTE arr[20];
    int idx = 0;
        
    int i;
    CString strLocal;
    strLocal.Format("%s", (LPCTSTR)str);
    int sz = strLocal.GetLength();
    for (i = 0; i < sz; i++) {
        char ch = strLocal.GetAt(i);
        if (ch >= '0' && ch <= '9') {
            arr[idx++] = ch - '0';
        }
        else if (ch >= 'a' && ch <= 'f') {
            arr[idx++] = ch - 'a' + 10;
        }
        else if (ch >= 'A' && ch <= 'F') {
            arr[idx++] = ch - 'A' + 10;
        }

        if (idx >= 16) break;
    }

    int count = 0;
    i = 0;
    if (idx % 2 == 1) {
        m_XInputSetting->header[count++] = arr[i++];
    }
    while (i < idx) {
        m_XInputSetting->header[count++] = (arr[i] << 4) + arr[i+1];
        i += 2;
    }
    m_XInputSetting->nHeader = count;
    return count;
}

void CSerialComDlg::OnBnClickedTest()
{	
    
}


void CSerialComDlg::Get_XinputData()
{
    static int nCount_noinput = 0;
    CString str = "";
    BOOL bConnect = FALSE;
    
    XINPUT_STATE state;
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        //컨트롤러 입력 상태 초기화        
        ZeroMemory(&m_XInput_state, sizeof(state));

        //컨트롤러 입력 상태 받아오기
        DWORD dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS)
        {
            bConnect = TRUE; 
            memcpy(&m_XInput_state, &state, sizeof(state));
            break;
        }
    }

    if (bConnect) {
        m_bGetXInputData = TRUE;
        str = "";
        if (Check_XinputData(m_XInput_state.Gamepad, str) || m_XInputSetting.iSendType == 1) {

            nCount_noinput = 0;
            Send_UartData_Xinput(m_XInput_state.Gamepad);
            m_strXinput = str;
        }
        else {
            if (nCount_noinput++ == 5) {
                m_strXinput = "";
            }
        }
    }
    else {
        m_bGetXInputData = FALSE;
    }

    if (m_bConnect_Xinput != bConnect) {
        if (bConnect == TRUE) {
            if (m_XInputSetting.iSendType == 0) {
                SetTimer(TIMER_XINPUT_CHECK, 50, NULL);
                m_bConnect_Xinput = TRUE;
            }
            else {
                SetTimer(TIMER_XINPUT_CHECK, m_XInputSetting.iUpdatePeriod, NULL);
                m_bConnect_Xinput = TRUE;
            }
        }
        else {
            SetTimer(TIMER_XINPUT_CHECK, 1000, NULL);
            m_bConnect_Xinput = FALSE;
            m_strXinput = "";
        }
    }
}

/***********************************************************************************
@Function name : SetFocusOff
@Description   : focus된 control 해제시킴 (MouseWheel로 Data Offset변경시 
                 focus된 control의 Name 변경 오류 방지용으로 Dummy Control로 
				 focus를 변경함
@Argument      : 없음
@Call          : PreTranslate()에서 WM_MOUSEWHEEL로 DataOffset 변경시 호출
************************************************************************************/
void CSerialComDlg::SetFocusOff() {
    GetDlgItem(IDC_Test)->SetFocus();
}


bool CSerialComDlg::IsEnoughMemory(size_t requiredBytes) {
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	if (GlobalMemoryStatusEx(&status)) {
		return status.ullAvailVirtual > requiredBytes;
	}
	return false;
}


/***********************************************************************************
@Function name : InitHistoryData
@Description   : m_HistoryData를 초기화함
@Argument      : 없음
@Call          : Power Initial, Save File 로딩 및 유저 Data 초기화 요청시 호출
************************************************************************************/

void CSerialComDlg::InitHistoryData() {
	static int iHistoryCapacity = -1;
	static int iData = -1;
	static int iMath = -1;

	// m_dummy_UARTData 수정
	if (iData != m_iMaxDataSize || iMath != m_iMaxMathSize) {
        m_bReqGraphUpdate = FALSE;
		iData = m_iMaxDataSize;
		iMath = m_iMaxMathSize;
		m_dummy_UARTData.fDataValue = std::vector<double>(m_iMaxDataSize, 0.);
		m_dummy_UARTData.fMathValue = std::vector<double>(m_iMaxMathSize, 0.);

		for (auto& item : m_HistoryData) {
			item.fDataValue.resize(m_iMaxDataSize);
			item.fMathValue.resize(m_iMaxMathSize);
		}
	}

	// Math Probe변경시 정보로딩 필요함
	BOOL bReqAssign = FALSE;
	if (iHistoryCapacity < m_iHistoryCapacity) {
		iHistoryCapacity = m_iHistoryCapacity;
		bReqAssign = TRUE;
	}
	m_iHistoryIndex = 0;
	if (bReqAssign) {
        CString str;
		size_t reqiredBytes_data = (sizeof(UART_DATA) + sizeof(UINT) * 50);
		UINT64 total_Bytes = (UINT64)reqiredBytes_data * (m_iHistoryCapacity + 1000) * 3 / 2;
        
        str.Format("%d %d", total_Bytes >> 20, m_iHistoryCapacity);
		while (((total_Bytes >>10) > (3<<20)) || IsEnoughMemory(reqiredBytes_data * (m_iHistoryCapacity + 1000)) == false) {
			m_iHistoryCapacity = m_iHistoryCapacity * 8 / 10;
			if (m_iHistoryCapacity < MIN_TESTDATA_SIZE) {				
				SetDlgItemText(IDC_Temp, "Error in memory assign for buffer data.");
				return;
			}		
			total_Bytes = (UINT64)reqiredBytes_data * (m_iHistoryCapacity + 1000);
            str.Format("%d %d", total_Bytes >> 20, m_iHistoryCapacity);
		}

		m_HistoryData.reserve(m_iHistoryCapacity + 1000);
	}
	m_HistoryData.clear();
	m_HistoryData.push_back(m_dummy_UARTData);

	m_iFilterIndex = 0;
	if (bReqAssign) {
		m_iFilterHistoryIndexTable.reserve(m_iHistoryCapacity + 1000);
		m_iFilterLogIndexTable.reserve(m_iHistoryCapacity + 1000);
	}
	m_iFilterHistoryIndexTable.clear();
	m_iFilterLogIndexTable.clear();
	m_iFilterHistoryIndexTable.push_back(0);
	m_iFilterLogIndexTable.push_back(0);

	m_iLogIndex = 0;
	if (bReqAssign) {
		m_iLogHistoryIndexTable.reserve(m_iHistoryCapacity + 1000);
		m_iLogFilterIndexTable.reserve(m_iHistoryCapacity + 1000);
	}
	m_iLogHistoryIndexTable.clear();
	m_iLogFilterIndexTable.clear();
	m_iLogFilterIndexTable.emplace_back(0);
	m_iLogHistoryIndexTable.emplace_back(0);
	ShowDataMathControl();
}

/***********************************************************************************
@Function name : SetMathDefine_ChangeDataSize
@Description   : m_iDataPage의 갯수의 변경에 따른 Math Define변경
@Argument      : from  변경전 m_iDataPage,   to 변경후 m_iDataPage
@Call          : m_iDataPage변경  (Initial, Save Data Load, Setting) 변경시 적용함
************************************************************************************/
void CSerialComDlg::SetMathDefine_ChangeDataSize(int pre_data_page, int post_data_page) {
	if (pre_data_page == post_data_page) return;
	int pre = pre_data_page * MAX_DATACONTROL_SIZE;
	int post = post_data_page * MAX_DATACONTROL_SIZE;
	int change = post - pre;
	CString strAddress, strItem, strValue;

	for (int i = 0; i < MAXIMUM_MATH_SIZE; i++) {
		if (m_strPacketHeader == " " || m_strPacketHeader == "")
			strAddress.Format("MathDefine\\MathProbe%d\\default", i);
		else
			strAddress.Format("MathDefine\\MathProbe%d\\%s%d\\Mode_%d", i, (LPCTSTR)m_strPacketHeader, m_cPacketHead, m_iPresetMode);

		if (m_MathData[i].iDataID1 >= pre) {
			m_MathData[i].iDataID1 += change;
			m_MathData[i].iDataID1 = max(0, min(m_MathData[i].iDataID1, post + i));
			strItem.Format("Data1");
			strValue.Format("%d", m_MathData[i].iDataID1);
			SetReg_RegistryData(strAddress, strItem, strValue);
		}

		if (m_MathData[i].iDataID2 >= pre) {
			m_MathData[i].iDataID2 += change;
			m_MathData[i].iDataID2 = max(0, min(m_MathData[i].iDataID2, post + i));
			strItem.Format("Data2");
			strValue.Format("%d", m_MathData[i].iDataID2);
			SetReg_RegistryData(strAddress, strItem, strValue);
		}
	}
}

void CSerialComDlg::OnBnClickedNoheader()
{
	m_bAcceptNoHeader = IsDlgButtonChecked(IDC_NoHeader);
	CString str;
	GetDlgItemText(IDC_PacketHead, str);

	if (m_bAcceptNoHeader) {
		if (str != "_") {
			str = "_";
			SetDlgItemText(IDC_PacketHead, str);
            m_strPacketHeader.Format("%s", (LPCTSTR)str);
		}
	}
	else {
		if (str == "_") {
			CheckDlgButton(IDC_NoHeader, TRUE);
			m_bAcceptNoHeader = TRUE;
            
		}
	}
}

void CSerialComDlg::OnListCtrlClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nIndex = pNMItemActivate->iItem;    
    if (nIndex != -1 && nIndex < m_iLogIndex)
    {        
        m_iCurrentIndex = m_iLogHistoryIndexTable[nIndex];
        m_strIndexNumber.Format("%d", m_iCurrentIndex);

        // Graph 검토
        m_bDrawSelectData = TRUE;
        int iHistorySelectIndex = m_iCurrentIndex;

        if (m_HistoryData[iHistorySelectIndex].Header != m_cPacketHead) {
            m_strPacketHeader.Format("%c", m_HistoryData[iHistorySelectIndex].Header);
            UDF;
            OnEnChangePackethead();
        }

        int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;
        int iEndIndex = m_iHistoryIndex - m_iHistoryMove;

        if (iStartIndex > iHistorySelectIndex || iEndIndex < iHistorySelectIndex) {
            m_iHistoryMove = min(max(0, m_iHistoryIndex - iHistorySelectIndex - m_iDisplaySize / 2), m_iHistoryIndex);
        }
        UDF;
        OnEnChangeIndexnumber();
    }

    *pResult = 0;
}


void CSerialComDlg::OnItemChangedLogList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	return;
	static int preSel = -1;
	if (m_iProgramMode == 1) return;
	POSITION pos = m_ctrlLogList.GetFirstSelectedItemPosition();
	if (pos == nullptr) {
		*pResult = 0;
		return;
	}
	int iLogIndex = m_ctrlLogList.GetNextSelectedItem(pos);
	if (iLogIndex == preSel) { 
		*pResult = 0; 
		return; 
	}
	else {
		preSel = iLogIndex;
	}
	SetDlgItemInt(IDC_Temp, iLogIndex);

	m_strIndexNumber.Format("%d", m_iLogHistoryIndexTable[iLogIndex]);
	m_iCurrentIndex = m_iLogHistoryIndexTable[iLogIndex];

	// Graph 검토
	m_bDrawSelectData = TRUE;
	int iHistorySelectIndex = m_iLogHistoryIndexTable[iLogIndex];

	if (m_HistoryData[iHistorySelectIndex].Header != m_cPacketHead) {
		m_strPacketHeader.Format("%c", m_HistoryData[iHistorySelectIndex].Header);
		UDF;
		OnEnChangePackethead();
	}

	int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;
	int iEndIndex = m_iHistoryIndex - m_iHistoryMove;

	if (iStartIndex > iHistorySelectIndex || iEndIndex < iHistorySelectIndex) {
		m_iHistoryMove = min(max(0, m_iHistoryIndex - iHistorySelectIndex - m_iDisplaySize / 2), m_iHistoryIndex);
	}
	UDF;
	OnEnChangeIndexnumber();
	*pResult = 0;
}

void CSerialComDlg::OnSelectLogList(int iLogIndex) {
	if (iLogIndex == -1) { return; }
	m_strIndexNumber.Format("%d", m_iLogHistoryIndexTable[iLogIndex]);
	m_iCurrentIndex = m_iLogHistoryIndexTable[iLogIndex];

	// Graph 검토
	m_bDrawSelectData = TRUE;
	int iHistorySelectIndex = m_iLogHistoryIndexTable[iLogIndex];

	if (m_HistoryData[iHistorySelectIndex].Header != m_cPacketHead) {
		m_strPacketHeader.Format("%c", m_HistoryData[iHistorySelectIndex].Header);
		UDF;
		OnEnChangePackethead();
	}

	int iStartIndex = m_iHistoryIndex - m_iDisplaySize - m_iHistoryMove;
	int iEndIndex = m_iHistoryIndex - m_iHistoryMove;

	if (iStartIndex > iHistorySelectIndex || iEndIndex < iHistorySelectIndex) {
		m_iHistoryMove = min(max(0, m_iHistoryIndex - iHistorySelectIndex - m_iDisplaySize / 2), m_iHistoryIndex);
	}
	UDF;
	OnEnChangeIndexnumber();
}


void CSerialComDlg::InitListCtrl()
{
	// 컬럼 추가 (1개만 사용)
	m_ctrlLogList.InsertColumn(0, _T("Log"), LVCFMT_LEFT, 1800); // 너비는 적절히 조절

	// Owner Data 설정
	m_ctrlLogList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrlLogList.ModifyStyle(0, LVS_OWNERDATA | LVS_REPORT);
}


void CSerialComDlg::OnLvnGetdispinfoLogList(NMHDR* pNMHDR, LRESULT* pResult) {
    NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    int index = pDispInfo->item.iItem;
    if (index >= 0 && index < static_cast<int>(m_arrLogData.size())) {
        if ((pDispInfo->item.mask & LVIF_TEXT) && pDispInfo->item.pszText != nullptr && pDispInfo->item.cchTextMax > 0) {
            _tcsncpy_s( pDispInfo->item.pszText,pDispInfo->item.cchTextMax, m_arrLogData[index], _TRUNCATE);
        }
    }
    *pResult = 0;
}

void CSerialComDlg::AddLogData(const CString& strLog)
{
	m_arrLogData.emplace_back(strLog);
	m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
}


void CSerialComDlg::ScrollToLastItem()
{
	int nItemCount = static_cast<int>(m_arrLogData.size());
	if (nItemCount <= 0)
		return;

	int lastIndex = nItemCount - 1;

	// 🔹 이전 선택 항목들 해제
	for (int idx : m_vecPrevSelectedIndices)
	{
		if (idx >= 0 && idx < m_ctrlLogList.GetItemCount())
		{
			m_ctrlLogList.SetItemState(idx, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	// 🔹 현재 항목 선택 및 포커스
	m_ctrlLogList.SetFocus(); // 리스트 컨트롤에 포커스 설정
	m_ctrlLogList.SetItemState(lastIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctrlLogList.EnsureVisible(lastIndex, FALSE);

	// 🔹 현재 선택 항목 저장
	m_vecPrevSelectedIndices.clear();
	m_vecPrevSelectedIndices.emplace_back(lastIndex);
}

void CSerialComDlg::HighlightLog(int nIndex)
{
	int nItemCount = m_ctrlLogList.GetItemCount();
	if (nIndex < 0 || nIndex >= nItemCount)
		return;

	// 🔹 이전 선택 항목 해제
	for (int idx : m_vecPrevSelectedIndices)
	{
		if (idx >= 0 && idx < nItemCount)
		{
			m_ctrlLogList.SetItemState(idx, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	// 🔹 현재 항목 선택 및 포커스
	m_ctrlLogList.SetFocus();
	m_ctrlLogList.SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctrlLogList.EnsureVisible(nIndex, FALSE);

	// 🔹 현재 선택 항목 저장
	m_vecPrevSelectedIndices.clear();
	m_vecPrevSelectedIndices.emplace_back(nIndex);
}


CString CSerialComDlg::AddComma(int value) {
    CString result;
    bool isNegative = value < 0;

    // 절댓값으로 변환 후 문자열 생성
    int absValue = abs(value);
    result.Format("%d", absValue);

    // 뒤에서부터 3자리마다 쉼표 삽입
    int len = result.GetLength();
    int commaCount = (len - 1) / 3;

    for (int i = 1; i <= commaCount; ++i)
    {
        result.Insert(len - i * 3, ",");
    }

    // 음수일 경우 앞에 '-' 추가
    if (isNegative)
    {
        result.Insert(0, "-");
    }

    return result;
}


void CSerialComDlg::GetDisplayDataSize(void) {
	GetDisplayDataSize(&m_iDisplaySize);
}


void CSerialComDlg::GetDisplayDataSize(int* pval) {
	CString str;
	GetDlgItemText(IDC_DisplayDataSize, str);

	int sz = str.GetLength();
	int val = 0;
	for (int i = 0; i < sz; i++) {
		char ch = str.GetAt(i);
		if (ch >= '0' && ch <= '9') {
			val = val * 10 + ch - '0';
		}
	}
	*pval = val;
}

void CSerialComDlg::SetDisplayDataSize(void) {
	SetDisplayDataSize(m_iDisplaySize);
}


void CSerialComDlg::SetDisplayDataSize(int val) {
	SetDlgItemText(IDC_DisplayDataSize, AddComma(val));
}


void CSerialComDlg::MakeRandomIndexDataSet(int interval) {
	static int pre_interval = -1;
	if (pre_interval == interval) return;

	pre_interval = interval;
	if (interval == 1) {
		m_iRandInterval[N_RAND_INTERVAL];
		memset(m_iRandInterval, 0, N_RAND_INTERVAL*sizeof(int));
		return;
	}

	srand(0);
	for (int i = 0; i < N_RAND_INTERVAL; i++) {
		m_iRandInterval[i] = rand() % interval;
	}
}


void CSerialComDlg::CheckTimer() {
	// update Connect status
	static bool bBusy = FALSE;
	static DWORD pre_time = GetTickCount();
	static int connect_mode = 0;
	static DWORD AutosaveTimePre;
	static bool bUseAutosavePre = FALSE;

	if (bBusy) { return; }
	bBusy = true;
	
	DWORD current_time = GetTickCount();
	int port_status[3] = { m_ComuPort.m_bConnected, m_ComuPort2.m_bConnected,   m_ComuPort3.m_bConnected };
	for (int i = 0; i < 3; i++) {
		if (port_status[i]) {
			if (connect_mode) {
				if (m_iProgramMode == DATA_ANALYZE_MODE) {
					port_status[i] = 3;
				}
				else {
					port_status[i] = 2;
				}
			} else {
				port_status[i] = 0;
			}
		}
	}

	if (connect_mode == 0 && current_time - pre_time > 300) {
		pre_time = current_time;
		connect_mode = 1;
	}
	else if (connect_mode && current_time - pre_time > 800){
		pre_time = current_time;
		connect_mode = 0;
	}

	UpdateConnectStatus(port_status);
	
	// AutoSaveTimer
	if (m_bUseAutoSave && (m_ComuPort.m_bConnected || m_ComuPort2.m_bConnected) && m_bUseAutosave2_Time_LogCount) {
		if (!bUseAutosavePre) {
			AutosaveTimePre = current_time;
		}
		m_iAutosave_EllapsedTime += (current_time - AutosaveTimePre);

		if (m_iAutosave_EllapsedTime >= m_iAutosave2_cond_Time) {
			if (m_iAutosave_LogCount >= m_iAutosave2_cond_LogCount) {
				if (m_arrLogData.size() > 1) {
					m_iAutosave_EllapsedTime = 0;
					m_iAutosave_LogCount = 0;
					if (m_bAutosave_AddCapture) {  OnBnClickedCapture();  }
					ReqSaveData(FALSE);
					ReqResetData(FALSE);
				}
			}
		}
		AutosaveTimePre = current_time;
		bUseAutosavePre = m_bUseAutosave2_Time_LogCount;
	}
	else {
		bUseAutosavePre = FALSE;
	}
	bBusy = false;
}

void CSerialComDlg::UpdateConnectStatus(int* port) {
	CClientDC dc(this);   
#define __connect_x		11
#define __connect_y1	13
#define __connect_y2	45
#define __connect_y3	77
#define __connect_size	9

	dc.BitBlt(__connect_x, __connect_y1, __connect_size, __connect_size, &m_MemDC_Select, 50 * port[0] + 10, 1010, SRCCOPY);
	dc.BitBlt(__connect_x, __connect_y2, __connect_size, __connect_size, &m_MemDC_Select, 50 * port[1] + 10, 1010, SRCCOPY);
	dc.BitBlt(__connect_x, __connect_y3, __connect_size, __connect_size, &m_MemDC_Select, 50 * port[2] + 10, 1010, SRCCOPY);
}


void CSerialComDlg::UpdateLogMessage() {
	int iIndex = min(max(0, GetDlgItemInt(IDC_IndexNumber)), m_arrLogData.size()-1);
	m_strTemp = m_arrLogData[m_HistoryData[iIndex].iHistoryLogIndex];
	SetDlgItemText(IDC_Temp, m_strTemp);
}

void CSerialComDlg::UpdateAutoHederRx() {
	if (m_ComuPort.m_bConnected && m_ComuPort2.m_bConnected &&m_bUseAutoHeader_rx_reg) {
		m_bUseAutoHeader_rx = FALSE;
	}
	else {
		m_bUseAutoHeader_rx = m_bUseAutoHeader_rx_reg;
	}
}

void CSerialComDlg::GetRegGrphHeight(){
    // Graph height 
    CString strAddress;
    CString strValue;
    CString strPresetMode;

    // Preset Mode
    if (m_iPresetMode == 0) {
        strPresetMode = "";
    }
    else {
        strPresetMode.Format("\\Mode_%d", m_iPresetMode);
    }
    
    for (int iGraph = 0; iGraph < MAX_GRAPH_COUNT; iGraph++) {
        if (m_strPacketHeader == " " || m_strPacketHeader == "")
            strAddress.Format("Graph\\Graph%d", iGraph);
        else
            strAddress.Format("Graph\\Graph%d\\%c%d%s", iGraph, m_cPacketHead, m_cPacketHead, (LPCTSTR)strPresetMode);
        strValue = GetReg_RegistryData(strAddress, "GraphHeight");
        if (strValue == "")
            m_RealGrpahInfo[iGraph].iHeight = 1000;
        else 
            m_RealGrpahInfo[iGraph].iHeight = max(atoi(strValue), 0);        
    }
}

BOOL CSerialComDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{	
	TCHAR szTempPath[MAX_PATH];	
	::GetTempPath(MAX_PATH, szTempPath);

	CString strHelpPath;		
	strHelpPath.Format("%sTestDataAnalyzerTool_Help.chm", szTempPath);
    ::ShellExecute(NULL, _T("open"), strHelpPath, NULL, NULL, SW_SHOWNORMAL);

	return TRUE; // 부모 클래스의 OnHelpInfo를 호출하지 않고 TRUE를 반환하여 처리를 완료합니다.
}
     

void CSerialComDlg::OnBnClickedUseloadctrl()
{
	bool bActive = false;
	if (IsDlgButtonChecked(IDC_UseLoadCtrl)) {
		bActive = true;
		m_bResetIntGain = true;
	}
	
	//GetDlgItem(IDC_LoadCtrl_Torque)->EnableWindow(bActive);
}


void CSerialComDlg::OnBnClickedAutoconnect()
{
	m_bLoadControlSetting = true;
    OnPortClose1();
    OnPortClose2();
    OnPortClose3();

	Wait(50);

    OnSetfocusSerialPort();
    OnSetfocusSerialPort2();
    OnSetfocusSerialPort3();

    CString strPort[MAX_COM_PORT_SIZE+1];
    int iPortCount = 0;
    int iSize = m_cSerialPort.GetCount();
    bool bAvailable[MAX_COM_PORT_SIZE];
    int iConnectMode[3] = { -1, -1, -1,};
    // index 0 : Torquemeter
    // index 1 : Load Control
    // index 2 : MCU

    for (int i = 0; i < iSize; i++) {
        bAvailable[i] = true;
        m_cSerialPort.GetLBText(i, strPort[MAX_COM_PORT_SIZE]);
        strPort[i].Format("%s", (LPCTSTR)(RemoveSerialInfo(strPort[MAX_COM_PORT_SIZE])));
    }

    for (int mode = 0; mode < 3; mode++) {

        for (int i = 0; i < iSize; i++) {
            if (bAvailable[i]) {
                if (ReqConnetCheck(mode, i)) {
                    iConnectMode[mode] = i;
                    bAvailable[i] = false;
                    break;
                }
				Sleep(10);
            }
        }
    }

	if (iConnectMode[1] > -1) {
		// PS Init
		Init_PS_Setting();
	}

	{

		CString str = "";
		int nFail = 0;
		if (iConnectMode[0] == -1) {
			str += " MCU";			
			nFail++;
		}
		if (iConnectMode[1] == -1) {
			if (nFail > 0) { str += ","; }
			str += " Braker";
			nFail++;
		}

		if (iConnectMode[2] == -1) {
			if (nFail > 0) { str += ","; }
			str += " Torquemeter";
			nFail++;
		}

		if (nFail > 0) { 
			CString str2;
			str2.Format("Fail to Port Open (%d): ", nFail);
			AfxMessageBox(str2 + str); 			
		}

		if (iConnectMode[0] == -1) {
			m_iProgramMode = DATA_ACQUISITION_MODE;
		}
		else {
			m_iProgramMode = DATA_ANALYZE_MODE;
		}
		OnBnClickedDefaultbutton();
	}
}


bool CSerialComDlg::ReqConnetCheck(int iMode, int iPort) {
    switch (iMode) {    
	case 1: return CheckLoadPort(iPort);
	case 2: return CheckMcuPort(iPort);
    case 0: return CheckTorquePort(iPort);
    }
    return false;
}

bool CSerialComDlg::CheckTorquePort(int i, int iTimeout) {
    m_cSerialPort3.SetCurSel(i);
	Req_PortOpen(3);
    Wait(10);

    
    if (m_ComuPort3.m_bConnected == TRUE) {
        m_bGetTorqueData = false;
        // Timeout 2sec
        DWORD startTick = GetTickCount();
        int ellapsed = 0;
        while (m_bGetTorqueData == false && ellapsed <= iTimeout) {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            // CPU 점유율이 과도하게 높아지는 것을 방지하기 위해 짧은 대기 시간을 부여합니다.
            Sleep(1);
            ellapsed = GetTickCount()- startTick;
        }
        if (m_bGetTorqueData == false) { Req_PortClose(3); }
        return m_bGetTorqueData;
    }   
    return false;
}


bool CSerialComDlg::CheckLoadPort(int i, int iTimeout) {
	m_bLoadControlSetting = true;
	m_cSerialPort2.SetCurSel(i);
	Req_PortOpen(2);
	Wait(10);	

	if (m_ComuPort2.m_bConnected == TRUE) {
		m_bGetLoadData = false;			
		for (int count = 0; count < 2 && m_bGetLoadData == false; count++) {
			// Send Check Data
			DWORD startTick = GetTickCount();
			int ellapsed = 0;
			m_req_Load_Packet_reset = true;
			Wait(20);
			Send_LoadPacket(PS_GET_CUR_OUT);			

			// Waiting response
			while (m_bGetLoadData == false  && ellapsed <= iTimeout) {
				MSG msg;
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				// CPU 점유율이 과도하게 높아지는 것을 방지하기 위해 짧은 대기 시간을 부여합니다.
				Sleep(1);
				ellapsed = GetTickCount() - startTick;
				if (m_LoadPacket.req_ack == false) {
					m_bGetLoadData = true;
				}
			}
		}
		if (m_bGetLoadData == false) { Req_PortClose(2); }
		m_bLoadControlSetting = false;
		return m_bGetLoadData;
	}
	m_bLoadControlSetting = false;
	return false;
}

bool CSerialComDlg::CheckMcuPort(int i, int iTimeout) {
	// Need to be implementation

	// Req_PortClose(1)
	// Req_PortOpen(1)
	return false;
}

bool CSerialComDlg::Send_LoadPacket(POWERSUPLY_CMD_e id, double fValue) {
	if (id < PS_REQ_LOCK || id > PS_GET_STATUS) {
		m_strTemp.Format("Load command 호출에 오류가 있습니다.(id: %d)", id);
		SetDlgItemText(IDC_Temp, m_strTemp);
		return false;
	}
	
	bool result = (this->*m_LoadFunc[id])(fValue);
	if (result) {
		m_LoadPacket.id = id;
		m_ComuPort2.WriteComm((BYTE *)m_LoadPacket.send_buf, m_LoadPacket.send_size);
		m_LoadPacket.tick_send = GetTickCount();
	}
	else {
		m_LoadPacket.req_ack = false;
	}
	return result;
}

bool CSerialComDlg::Func_PS_REQ_LOCK(double fVal) {	
	int iVal = (int)(fVal + 0.1);
	if (iVal == 0 || iVal == 1) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "LOCK%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Lock 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_REQ_CUR_SET(double fVal) {	
	if (fVal < 0. || fVal >= 10.) {
		AfxMessageBox("Current 입력값 범위 오류");
		return false;
	} 	
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "ISET1:%05.3f", fVal);
	m_LoadPacket.req_ack = false;
	return true;
};

bool CSerialComDlg::Func_PS_GET_CUR_SET(double fVal) {
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "ISET1?");
	m_LoadPacket.req_ack = true;
	return true;
};

bool CSerialComDlg::Func_PS_REQ_VOLT_SET(double fVal) {
	if (fVal < 0. || fVal >= 100.) {
		AfxMessageBox("Voltage 입력값 범위 오류");
		return false;
	}

	fVal = m_iVoltTarget_x100 * 0.01f;
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "VSET1:%05.2f", fVal);
	m_LoadPacket.req_ack = false;
	return true;
};


bool CSerialComDlg::Func_PS_GET_VOLT_SET(double fVal) {
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "VSET1?");
	m_LoadPacket.req_ack = true;
	return true;
};

bool CSerialComDlg::Func_PS_GET_CUR_OUT(double fVal) {
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "IOUT1?");
	m_LoadPacket.req_ack = true;
	return true;
};

bool CSerialComDlg::Func_PS_GET_VOLT_OUT(double fVal) {
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "VOUT1?");
	m_LoadPacket.req_ack = true;
	return true;
};

bool CSerialComDlg::Func_PS_REQ_ONOFF(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal == 0 || iVal == 1) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "OUT%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("ON/OFF 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_REQ_SET_OVER_CUR(double fVal) {
	if (fVal < 0. || fVal >= 10.) {
		AfxMessageBox("Current 입력값 범위 오류");
		return false;
	}
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "OCPSTE1:%05.3f", fVal);
	m_LoadPacket.req_ack = false;
	return true;
};

bool CSerialComDlg::Func_PS_REQ_SET_OVER_VOLT(double fVal) {
	if (fVal < 0. || fVal >= 100.) {
		AfxMessageBox("Voltage 입력값 범위 오류");
		return false;
	}
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "OVPSTE1:%05.2f", fVal);
	m_LoadPacket.req_ack = false;
	return true;
};

bool CSerialComDlg::Func_PS_REQ_SET_OV_ENABLE(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal == 0 || iVal == 1) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "OVP%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Voltage Protection 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_REQ_SET_OC_ENABLE(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal == 0 || iVal == 1) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "OCP%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Current Protection 설정 오류");
		return false;
	}
	return true;
};


bool CSerialComDlg::Func_PS_REQ_SAVESETTING(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal >= 1 && iVal <= 5) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "SAV%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Save Page 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_REQ_RESTORE(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal >= 1 && iVal <= 5) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "RCL%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Restore Page 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_GET_INFO(double fVal) {
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "*IDN?");
	m_LoadPacket.req_ack = true;
	return true;
};

bool CSerialComDlg::Func_PS_REQ_SET_BEEP(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal == 0 || iVal == 1) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "BEEP%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Beep 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_REQ_SET_MODE(double fVal) {
	int iVal = (int)(fVal + 0.1);
	if (iVal >= 0 && iVal <= 2) {
		m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "TRACK%d", iVal);
		m_LoadPacket.req_ack = false;
	}
	else {
		AfxMessageBox("Operation Mode 설정 오류");
		return false;
	}
	return true;
};

bool CSerialComDlg::Func_PS_GET_STATUS(double fVal) {
	m_LoadPacket.send_size = snprintf(m_LoadPacket.send_buf, sizeof(m_LoadPacket.send_buf), "STATUS?");
	m_LoadPacket.req_ack = true;
	return true;
};


void CSerialComDlg::OnBnClickedLoadcontrolsetting()
{
	//CStringA str;
	//char buf[100];
	//GetDlgItemText(IDC_Temp, str);
	//int sz = str.GetLength();

	//for (int i = 0; i < sz; i++) {
	//	buf[i] = str.GetAt(i);
	//}
	
	CLoadCtrlDlg dlg;
	dlg.pMain = this;
	dlg.DoModal();
}

void CSerialComDlg::OnEnChangePsVolt()
{
	static int pre = 0;
	CString str, str2;
	GetDlgItemText(IDC_PS_Volt, str);
	int val = max(0, min(m_LoadSetting.overVoltageLevel, (int)(atof(str) * 100. + 0.001)));

	if (m_iVoltTarget_x100 != val) {
		m_iVoltTarget_x100 = val;
	} 

	if (m_iVoltTarget_x100 != pre) {
		pre = m_iVoltTarget_x100;
	}

	m_LoadPacket.volt[1] = 0.01 * m_iVoltTarget_x100;
	str2.Format("%05.2f", m_LoadPacket.volt[1] + 0.001);
	if (str != str2) {
		SetDlgItemText(IDC_PS_Volt, str2);
	}
}

void CSerialComDlg::OnBnClickedVoltup1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = 1000 + m_iVoltTarget_x100;

	if (pre != m_iVoltTarget_x100) {
		CString str;
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}


void CSerialComDlg::OnBnClickedVoltup2()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = 100 + m_iVoltTarget_x100;

	if (pre != m_iVoltTarget_x100) {
		CString str;
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}


void CSerialComDlg::OnBnClickedVoltup3()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = 10 + m_iVoltTarget_x100;

	if (pre != m_iVoltTarget_x100) {
		CString str;
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}


void CSerialComDlg::OnBnClickedVoltup4()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = 1 + m_iVoltTarget_x100;

	if (pre != m_iVoltTarget_x100) {
		CString str;
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}


void CSerialComDlg::OnBnClickedVoltdn1()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = max(0, m_iVoltTarget_x100 - 1000);

	CString str;
	GetDlgItemText(IDC_PS_Volt, str);
	if (pre != m_iVoltTarget_x100 || str == "") {
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}

void CSerialComDlg::OnBnClickedVoltdn2()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = max(0, m_iVoltTarget_x100 - 100);

	CString str;
	GetDlgItemText(IDC_PS_Volt, str);
	if (pre != m_iVoltTarget_x100 || str == "") {
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}

void CSerialComDlg::OnBnClickedVoltdn3()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = max(0, m_iVoltTarget_x100 - 10);

	CString str;
	GetDlgItemText(IDC_PS_Volt, str);
	if (pre != m_iVoltTarget_x100 || str == "") {
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}

void CSerialComDlg::OnBnClickedVoltdn4()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = max(0, m_iVoltTarget_x100 - 1);

	CString str;
	GetDlgItemText(IDC_PS_Volt, str);
	if (pre != m_iVoltTarget_x100 || str == "") {
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}

void CSerialComDlg::OnBnClickedVoltdn5()
{
	int pre = m_iVoltTarget_x100;
	m_iVoltTarget_x100 = 0;

	CString str;
	GetDlgItemText(IDC_PS_Volt, str);
	if (pre != m_iVoltTarget_x100 || str == "") {
		str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
		SetDlgItemText(IDC_PS_Volt, str);
	}
	CheckDlgButton(IDC_UseLoadCtrl, FALSE);
}


void CSerialComDlg::OnBnClickedReqnoload()
{
	Init_PS_Setting();
}

void CSerialComDlg::Init_PS_Setting() {
	m_bLoadControlSetting = true;
	Wait(50);

	GetReg_LoadSetting();

	// 전압 0
	m_iVoltTarget_x100 = 0;
	Send_LoadPacket(PS_REQ_VOLT_SET);
	Wait(30);

	// Beep설정
	if (m_LoadSetting.useBeep) {
		Send_LoadPacket(PS_REQ_SET_BEEP, 1.0f);
	}
	else {
		Send_LoadPacket(PS_REQ_SET_BEEP, 0.f);
	}
	Wait(30);

	Send_LoadPacket(PS_REQ_ONOFF, 1.f);
	Wait(30);	

	m_bLoadControlSetting = false;
}

void CSerialComDlg::AutoControlVoltage() {
	static int dir = 0;
	static double fInt = 0.f;	
	CString str;
	int speed = GetDlgItemInt(IDC_LoadCtrl_Speed);
	if ( m_LoadSetting.useMinSpeed && speed < m_LoadSetting.iMinSpeed_control) {
		m_iVoltTarget_x100 = m_iVoltTarget_x100 * 95 / 100;
	}
	else {
		int gain = max(1, GetDlgItemInt(IDC_PS_Rate));
		GetDlgItemText(IDC_LoadCtrl_CurTorque, str);
		double tor_cur = abs(atof(str));

		GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
		double tor_target = abs(atof(str));

		GetDlgItemText(IDC_PS_Rate2, str);
		double fI_gain = abs(atof(str));

		double delta = tor_target - tor_cur;
		if (delta > 0 && dir < 0) { dir = 1; fInt = 0; }
		else if (delta < 0 && dir > 0) { dir = -1; fInt = 0; }

		if (m_bResetIntGain) {
			fInt = 0.f;
			m_bResetIntGain = false;
		}
		else {
			fInt += delta;
		}

		m_iVoltTarget_x100 += (int)((tor_target - tor_cur) * gain + fInt * fI_gain);
	}
	str.Format("%05.2f", 0.01 * m_iVoltTarget_x100 + 0.001);
	SetDlgItemText(IDC_PS_Volt, str);
}

void CSerialComDlg::OnBnClickedIncGain()
{
	int iGain = GetDlgItemInt(IDC_PS_Rate);
	SetDlgItemInt(IDC_PS_Rate, iGain * 2);
}


void CSerialComDlg::OnBnClickedDecGain()
{
	int iGain = GetDlgItemInt(IDC_PS_Rate);
	SetDlgItemInt(IDC_PS_Rate, iGain / 2);
}


void CSerialComDlg::OnEnChangePsRate()
{
	CString str, str2;
	GetDlgItemText(IDC_PS_Rate, str);
	int Gain = min(10000, max(10, GetDlgItemInt(IDC_PS_Rate)));
	str2.Format("%d", Gain);

	if (str != str2) {
		SetDlgItemText(IDC_PS_Rate, str2);
	}
}


void CSerialComDlg::OnEnChangeLoadctrlTargettorque()
{
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, m_strTargetTorque);
	m_fTargetTorque = atof(m_strTargetTorque);
}


void CSerialComDlg::OnBnClickedTorqueup1()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = atof(str) + 0.1001f;
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}


void CSerialComDlg::OnBnClickedTorqueup2()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = atof(str) + 0.0101f;
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}


void CSerialComDlg::OnBnClickedTorqueup3()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = atof(str) + 0.0011f;
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}


void CSerialComDlg::OnBnClickedTorquedn1()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = max(0.f, atof(str) - 0.1f + 0.0001f);
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}


void CSerialComDlg::OnBnClickedTorquedn2()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = max(0.f, atof(str) - 0.01f + 0.0001f);
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}


void CSerialComDlg::OnBnClickedTorquedn3()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = max(0.f, atof(str) - 0.001f + 0.0001f);
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}


void CSerialComDlg::OnBnClickedTorquedn4()
{
	CString str;
	GetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
	double fTor = 0.f;
	str.Format("%.3f", fTor);
	SetDlgItemText(IDC_LoadCtrl_TargetTorque, str);
}




void CSerialComDlg::OnEnChangePsRate2()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = min(10.f, max(0.f, atof(str1)));
	str2.Format("%.5f", cur_gain + 0.000001f);
	if (str1 != str2) {
		SetDlgItemText(IDC_PS_Rate2, str2);
	}
}


void CSerialComDlg::OnBnClickedIncGain2()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = atof(str1);
	str2.Format("%.5f", cur_gain * 2.f + 0.000001f);
	SetDlgItemText(IDC_PS_Rate2, str2);
}


void CSerialComDlg::OnBnClickedDecGain2()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = atof(str1);
	str2.Format("%.5f", cur_gain * 0.5f + 0.000001f);
	if (str1 != str2) {
		SetDlgItemText(IDC_PS_Rate2, str2);
	}
}


void CSerialComDlg::GetReg_LoadSetting() {
	static bool init = true;

	CString strAddr = "LoadSetting";
	CString strItem, strValue;

	strValue = GetRegRoot_RegistryData(strAddr, "MinControlSpeed");
	if (strValue == "") { strValue = "10"; }
	m_LoadSetting.iMinSpeed_control = atoi(strValue);
	
	strValue = GetRegRoot_RegistryData(strAddr, "OCP_Level");
	if (strValue == "") { strValue = "9.999"; }
	m_LoadSetting.overCurrentLevel = atof(strValue);

	strValue = GetRegRoot_RegistryData(strAddr, "OVP_Level");
	if (strValue == "") { strValue = "1000"; }
	m_LoadSetting.overVoltageLevel = atoi(strValue);

	strValue = GetRegRoot_RegistryData(strAddr, "UseBeep");
	if (strValue == "") { strValue = "0"; }
	m_LoadSetting.useBeep = atoi(strValue);

	strValue = GetRegRoot_RegistryData(strAddr, "UseOCP");
	if (strValue == "") { strValue = "0"; }
	m_LoadSetting.useOverCurProtection = atoi(strValue);
	if (m_LoadSetting.useOverCurProtection == false) {
		m_LoadSetting.overCurrentLevel = 10.000;
	}

	strValue = GetRegRoot_RegistryData(strAddr, "UseOVP");
	if (strValue == "") { strValue = "0"; }
	m_LoadSetting.useOverVoltProtection = atoi(strValue);
	if (m_LoadSetting.useOverVoltProtection == false) {
		m_LoadSetting.overVoltageLevel = 3000;
	}

	strValue = GetRegRoot_RegistryData(strAddr, "UseMinSpeed");
	if (strValue == "") { strValue = "1"; }
	m_LoadSetting.useMinSpeed = atoi(strValue);



	if (init) {
		init = false;
		SetDlgItemInt(IDC_PS_Rate, m_LoadSetting.p_gain);

		strValue.Format("%.5f", m_LoadSetting.i_gain + 0.0000001f);
		SetDlgItemText(IDC_PS_Rate2, strValue);

		strValue = GetRegRoot_RegistryData(strAddr, "Target_Torque");
		SetDlgItemText(IDC_LoadCtrl_TargetTorque, strValue);

		strValue = GetRegRoot_RegistryData(strAddr, "I-Gain");
		if (strValue == "") { strValue = "0.3"; }
		m_LoadSetting.i_gain = atof(strValue);

		strValue = GetRegRoot_RegistryData(strAddr, "P-Gain");
		if (strValue == "") { strValue = "50"; }
		m_LoadSetting.p_gain = atoi(strValue);
	}
}


void CSerialComDlg::SetReg_LoadSetting() {
	CString strAddr = "LoadSetting";
	CString strItem, strValue;
		
	strValue.Format("%d", m_LoadSetting.iMinSpeed_control);
	SetRegRoot_RegistryData(strAddr, "MinControlSpeed", strValue);
	
	GetDlgItemText(IDC_PS_Rate2, strValue);
	SetRegRoot_RegistryData(strAddr, "I-Gain", strValue);
	
	GetDlgItemText(IDC_PS_Rate, strValue);
	SetRegRoot_RegistryData(strAddr, "P-Gain", strValue);

	strValue.Format("%f", m_LoadSetting.overCurrentLevel);
	SetRegRoot_RegistryData(strAddr, "OCP_Level", strValue);

	strValue.Format("%d", m_LoadSetting.overVoltageLevel);
	SetRegRoot_RegistryData(strAddr, "OVP_Level", strValue);

	strValue.Format("%d", m_LoadSetting.useBeep);
	SetRegRoot_RegistryData(strAddr, "UseBeep", strValue);

	strValue.Format("%d", m_LoadSetting.useOverCurProtection);
	SetRegRoot_RegistryData(strAddr, "UseOCP", strValue);

	strValue.Format("%d", m_LoadSetting.useOverVoltProtection);
	SetRegRoot_RegistryData(strAddr, "UseOVP", strValue);

	strValue.Format("%d", m_LoadSetting.useMinSpeed);
	SetRegRoot_RegistryData(strAddr, "UseMinSpeed", strValue);

	GetDlgItemText(IDC_LoadCtrl_TargetTorque, strValue);
	SetRegRoot_RegistryData(strAddr, "Target_Torque", strValue);
}

void CSerialComDlg::OnBnClickedSaveloadsetting()
{
	SetReg_LoadSetting();
}


void CSerialComDlg::LoadDynamoPinName()
{
	// Pin Name
	m_strPinName[0].Format("Motor Speed     [rpm]");
	m_strPinName[1].Format("Torque Target   [N m]");
	m_strPinName[2].Format("Torque Measured [rpm]");
	m_strPinName[3].Format("Power           [w]");
	m_strPinName[4].Format("Load Voltage    [V]");
	m_strPinName[5].Format("Load Current    [A]");
}



void CSerialComDlg::OnBnClickedIncGain5()
{
	int iGain = GetDlgItemInt(IDC_PS_Rate);
	SetDlgItemInt(IDC_PS_Rate, iGain * 11 / 10);
}


void CSerialComDlg::OnBnClickedIncGain3()
{
	int iGain = GetDlgItemInt(IDC_PS_Rate);
	SetDlgItemInt(IDC_PS_Rate, iGain * 3 / 2);
}


void CSerialComDlg::OnBnClickedDecGain5()
{
	int iGain = GetDlgItemInt(IDC_PS_Rate);
	SetDlgItemInt(IDC_PS_Rate, iGain * 10 / 11);
}


void CSerialComDlg::OnBnClickedDecGain3()
{
	int iGain = GetDlgItemInt(IDC_PS_Rate);
	SetDlgItemInt(IDC_PS_Rate, iGain * 2 / 3);
}


void CSerialComDlg::OnBnClickedIncGain6()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = atof(str1);
	str2.Format("%.5f", cur_gain * 1.1f + 0.000001f);
	SetDlgItemText(IDC_PS_Rate2, str2);
}


void CSerialComDlg::OnBnClickedIncGain4()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = atof(str1);
	str2.Format("%.5f", cur_gain * 1.5f + 0.000001f);
	SetDlgItemText(IDC_PS_Rate2, str2);
}


void CSerialComDlg::OnBnClickedDecGain6()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = atof(str1);
	str2.Format("%.5f", cur_gain /  1.1f + 0.000001f);
	SetDlgItemText(IDC_PS_Rate2, str2);
}


void CSerialComDlg::OnBnClickedDecGain4()
{
	CString str1, str2;
	GetDlgItemText(IDC_PS_Rate2, str1);
	double cur_gain = atof(str1);
	str2.Format("%.5f", cur_gain / 1.5f + 0.000001f);
	SetDlgItemText(IDC_PS_Rate2, str2);
}

void CSerialComDlg::CheckLogUpdate(bool bForced) {
	static DWORD preTick = GetTickCount();
	if (bForced || (m_bReqLogSizeUpdate && GetTickCount() - preTick > (DWORD)m_iLOG_Update_Period)) {
		m_bReqLogSizeUpdate = FALSE;
		preTick = GetTickCount();

		CWnd* pOldFocus = GetFocus();  // 현재 포커스 저장

		m_ctrlLogList.SetItemCountEx(static_cast<int>(m_arrLogData.size()), LVSICF_NOINVALIDATEALL);
		ScrollToLastItem();

		// 포커스 복원
		if (pOldFocus && ::IsWindow(pOldFocus->GetSafeHwnd()))
		{
			pOldFocus->SetFocus();
		}
	}
}

void CSerialComDlg::OnBnClickedHideSidemenu()
{
	OnBnClickedDisplay3();
}


void CSerialComDlg::OnBnClickedShowmathmenu()
{
	CheckDlgButton(IDC_Display2, IsDlgButtonChecked(IDC_Display2) == FALSE);
	OnBnClickedDisplay2();
}


void CSerialComDlg::OnBnClickedHidemathmenu()
{
	CheckDlgButton(IDC_Display2, IsDlgButtonChecked(IDC_Display2) == FALSE);
	OnBnClickedDisplay2();
}

void  CSerialComDlg::Req_PortOpen(int nPort) {
	CCommThread *pComPort;
	CString* pPortName;
	int * piSerialPort;
	CComboBox * pcSerialPort;
	CString * pstrSerial;
	int iBaudRate;

	switch (nPort) {
	case 1: pComPort = &m_ComuPort;  pPortName = &strPort1name; piSerialPort = &m_iSerialPort;  pcSerialPort = &m_cSerialPort;  pstrSerial = &m_strSerial; iBaudRate = m_iBaudRate;  break;
	case 2: pComPort = &m_ComuPort2;  pPortName = &strPort2name; piSerialPort = &m_iSerialPort2;  pcSerialPort = &m_cSerialPort2; pstrSerial = &m_strSerial2; iBaudRate = m_iBaudRate2; break;
	case 3: pComPort = &m_ComuPort3;  pPortName = &strPort3name; piSerialPort = &m_iSerialPort3;  pcSerialPort = &m_cSerialPort3; pstrSerial = &m_strSerial3; iBaudRate = m_iBaudRate3; break;
	default: AfxMessageBox("Port Close mode Error"); return;
	}
	Req_PortClose(nPort);

	if (m_ComuPort.m_bConnected == TRUE) {
		OnPortClose1();
		Wait(0);
		Wait(0);
		Wait(0);
	}

	*piSerialPort = pcSerialPort->GetCurSel();
	pcSerialPort->GetLBText(*piSerialPort, *pstrSerial);
	pstrSerial->Format("%s", (LPCTSTR)RemoveSerialInfo(*pstrSerial));		
	
	if (pComPort->OpenPort(byNameComPort(*pstrSerial), byIndexBaud(iBaudRate), 8, 0, 0) == TRUE) {
		pPortName->Format("%s", pComPort->m_sPortName);		
	}
}


void  CSerialComDlg::Req_PortClose(int nPort) {
	CCommThread *pComPort;
	CString* pPortName;

	switch (nPort) {
	case 1: pComPort = &m_ComuPort;  pPortName = &strPort1name; break;
	case 2: pComPort = &m_ComuPort2;  pPortName = &strPort2name; break;
	case 3: pComPort = &m_ComuPort3;  pPortName = &strPort3name; break;
	default: AfxMessageBox("Port Close mode Error"); return; 
	}
	
	if (pComPort->m_bConnected == TRUE) {
		pComPort->ClosePort();
		pPortName->Empty();;
		Wait(10);
	}
}