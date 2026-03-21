// SettingDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "SerialComDlg.h"
#include "HeaderLogDlg.h"
#include "GraphLog.h"
#include "ConfirmDlg.h"
#include "CNavilog.h"
#include "XInputDlg.h"

// CSettingDlg 대화 상자

#define TIMER_CHANGE_DATASIZE	250
#define TIMER_CHANGE_X			211
#define TIMER_CHANGE_Y			212


IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Setting, pParent)
	, m_iTimeDisplayMode(0)
	, m_strModelName(_T(""))
	, m_strDescription(_T(""))
	, m_iSelectList(-1)
	, m_bFixGraphFrame(FALSE)
	, m_bUsePositionGraph(FALSE)
	, m_strPositonGraphHeader(_T(""))
	, m_iSelectPositionGraphX(0)
	, m_iSelectPositionGraphY(0)
	, m_iGridCountY(0)
	, m_iMarkSize(0)
	, m_strMinDataSize(_T("16"))
	, m_strMaxDataSize(_T("16"))
	, m_bGetSettingInLoading(FALSE)
	, m_iSaveMode(0)
	, m_iGridX_Type(0)
	, m_iGridX(0)
	, m_iColorItem(0)
{

}


CSettingDlg::~CSettingDlg()
{
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Command0, m_strCommand[0]);
	DDX_Text(pDX, IDC_Command1, m_strCommand[1]);
	DDX_Text(pDX, IDC_Command2, m_strCommand[2]);
	DDX_Text(pDX, IDC_Command3, m_strCommand[3]);
	DDX_Text(pDX, IDC_Command4, m_strCommand[4]);
	DDX_Text(pDX, IDC_Command5, m_strCommand[5]);
	DDX_Text(pDX, IDC_Command6, m_strCommand[6]);
	DDX_Text(pDX, IDC_Command7, m_strCommand[7]);
	DDX_Text(pDX, IDC_Command8, m_strCommand[8]);
	DDX_Text(pDX, IDC_Command9, m_strCommand[9]);
	DDX_Text(pDX, IDC_Command10, m_strCommand[10]);
	DDX_Text(pDX, IDC_Command11, m_strCommand[11]);
	DDX_Text(pDX, IDC_Command12, m_strCommand[12]);
	DDX_Text(pDX, IDC_Command13, m_strCommand[13]);
	DDX_Text(pDX, IDC_Command14, m_strCommand[14]);
	DDX_Text(pDX, IDC_Command15, m_strCommand[15]);
	DDX_Text(pDX, IDC_Command16, m_strCommand[16]);
	DDX_Text(pDX, IDC_Command17, m_strCommand[17]);
	DDX_Text(pDX, IDC_Command18, m_strCommand[18]);
	DDX_Text(pDX, IDC_Command19, m_strCommand[19]);
	DDX_Radio(pDX, IDC_TimeDisplayMode0, m_iTimeDisplayMode);
	DDX_Text(pDX, IDC_FontSizeSetting0, m_strFontSize[0]);
	DDX_Text(pDX, IDC_FontSizeSetting1, m_strFontSize[1]);
	DDX_Control(pDX, IDC_ModelList, m_ModelList);
	DDX_Control(pDX, IDC_CopyModel, m_cCopyModel);
	DDX_CBIndex(pDX, IDC_CopyModel, m_iCopyModel);
	DDX_Text(pDX, IDC_AddModelName, m_strModelName);
	DDX_Text(pDX, IDC_Description, m_strDescription);
	DDX_Check(pDX, IDC_UseFixedGraphFrame, m_bFixGraphFrame);
	DDX_Check(pDX, IDC_UsePositionGraph, m_bUsePositionGraph);
	DDX_Text(pDX, IDC_PositionDisplayHeader, m_strPositonGraphHeader);
	DDX_CBIndex(pDX, IDC_GridCount, m_iGridCountY);
	DDX_CBIndex(pDX, IDC_MarkSize2, m_iMarkSize);

	DDX_Text(pDX, IDC_DataMultiplier0, m_strCoff[0]);
	DDX_Text(pDX, IDC_DataMultiplier1, m_strCoff[1]);
	DDX_Text(pDX, IDC_DataMultiplier2, m_strCoff[2]);
	DDX_Text(pDX, IDC_DataMultiplier3, m_strCoff[3]);
	DDX_Text(pDX, IDC_DataMultiplier4, m_strCoff[4]);
	DDX_Text(pDX, IDC_DataMultiplier5, m_strCoff[5]);
	DDX_Text(pDX, IDC_DataMultiplier6, m_strCoff[6]);
	DDX_Text(pDX, IDC_DataMultiplier7, m_strCoff[7]);
	DDX_Text(pDX, IDC_DataMultiplier8, m_strCoff[8]);
	DDX_Text(pDX, IDC_DataMultiplier9, m_strCoff[9]);
	DDX_Text(pDX, IDC_DataMultiplier10, m_strCoff[10]);
	DDX_Text(pDX, IDC_DataMultiplier11, m_strCoff[11]);
	DDX_Text(pDX, IDC_DataMultiplier12, m_strCoff[12]);
	DDX_Text(pDX, IDC_DataMultiplier13, m_strCoff[13]);
	DDX_Text(pDX, IDC_DataMultiplier14, m_strCoff[14]);
	DDX_Text(pDX, IDC_DataMultiplier15, m_strCoff[15]);
	DDX_Text(pDX, IDC_MinDataSize, m_strMinDataSize);
	DDX_Text(pDX, IDC_MaxDataSize, m_strMaxDataSize);
	DDX_Radio(pDX, IDC_TextColorMode0, m_iTextColorMode);
	DDX_Radio(pDX, IDC_BKColorMode2, m_iBkColorMode);
	DDX_Text(pDX, IDC_TextColor0, m_strSelTextColor[0]);
	DDX_Text(pDX, IDC_TextColor1, m_strSelTextColor[1]);
	DDX_Text(pDX, IDC_TextColor2, m_strSelTextColor[2]);
	DDX_Text(pDX, IDC_BKColor3, m_strSelBkColor[0]);
	DDX_Text(pDX, IDC_BKColor4, m_strSelBkColor[1]);
	DDX_Text(pDX, IDC_BKColor5, m_strSelBkColor[2]);
	DDX_Check(pDX, IDC_GetSettingInLoading, m_bGetSettingInLoading);
	DDX_Radio(pDX, IDC_SaveMode0, m_iSaveMode);
	DDX_CBIndex(pDX, IDC_GridType, m_iGridX_Type);
	DDX_Text(pDX, IDC_nGridX, m_iGridX);
	DDX_CBIndex(pDX, IDC_ColorItem, m_iColorItem);
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(ID_SaveClose, &CSettingDlg::OnBnClickedSaveclose)
	ON_EN_CHANGE(IDC_Command0, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command1, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command2, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command3, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command4, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command5, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command6, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command7, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command8, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command9, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command10, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command11, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command12, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command13, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command14, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command15, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command16, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command17, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command18, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_Command19, &CSettingDlg::UpdateControl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_LoadDefaultColor, &CSettingDlg::OnBnClickedLoaddefaultcolor)
	ON_EN_CHANGE(IDC_FontSizeSetting0, &CSettingDlg::OnEnChangeFontsizesetting)
	ON_EN_CHANGE(IDC_FontSizeSetting1, &CSettingDlg::OnEnChangeFontsizesetting)
	ON_CBN_SELCHANGE(IDC_CopyModel, &CSettingDlg::OnCbnSelchangeCopymodel)
	ON_BN_CLICKED(IDC_AddModel, &CSettingDlg::OnBnClickedAddmodel)
	ON_BN_CLICKED(IDC_RemoveModel, &CSettingDlg::OnBnClickedRemovemodel)
	ON_NOTIFY(NM_CLICK, IDC_ModelList, &CSettingDlg::OnClickModellist)
	ON_EN_CHANGE(IDC_AddModelName, &CSettingDlg::OnEnChangeAddmodelname)
	ON_EN_CHANGE(IDC_Description, &CSettingDlg::OnEnChangeDescription)
	ON_BN_CLICKED(IDC_CopySetting, &CSettingDlg::OnBnClickedCopysetting)
	ON_BN_CLICKED(IDCANCEL, &CSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_UseFixedGraphFrame, &CSettingDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UsePositionGraph, &CSettingDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PositionDisplayHeader, &CSettingDlg::UpdateControl)
	ON_BN_CLICKED(IDC_ChangePenStyle0, &CSettingDlg::OnBnClickedChangepenstyle0)
	ON_BN_CLICKED(IDC_ChangePenStyle1, &CSettingDlg::OnBnClickedChangepenstyle1)
	ON_BN_CLICKED(IDC_ChangePenStyle2, &CSettingDlg::OnBnClickedChangepenstyle2)
	ON_BN_CLICKED(IDC_ChangePenStyle3, &CSettingDlg::OnBnClickedChangepenstyle3)
	ON_BN_CLICKED(IDC_ChangePenStyle4, &CSettingDlg::OnBnClickedChangepenstyle4)
	ON_BN_CLICKED(IDC_ChangePenStyle5, &CSettingDlg::OnBnClickedChangepenstyle5)
	ON_BN_CLICKED(IDC_ChangePenStyle6, &CSettingDlg::OnBnClickedChangepenstyle6)
	ON_BN_CLICKED(IDC_ChangePenStyle7, &CSettingDlg::OnBnClickedChangepenstyle7)
	ON_BN_CLICKED(IDC_ChangePenStyle8, &CSettingDlg::OnBnClickedChangepenstyle8)
	ON_BN_CLICKED(IDC_ChangePenStyle9, &CSettingDlg::OnBnClickedChangepenstyle9)
	ON_BN_CLICKED(IDC_ChangePenStyle10, &CSettingDlg::OnBnClickedChangepenstyle10)
	ON_BN_CLICKED(IDC_ChangePenStyle11, &CSettingDlg::OnBnClickedChangepenstyle11)
	ON_BN_CLICKED(IDC_ChangePenStyle12, &CSettingDlg::OnBnClickedChangepenstyle12)
	ON_BN_CLICKED(IDC_ChangePenStyle13, &CSettingDlg::OnBnClickedChangepenstyle13)
	ON_BN_CLICKED(IDC_ChangePenStyle14, &CSettingDlg::OnBnClickedChangepenstyle14)
	ON_BN_CLICKED(IDC_ChangePenStyle15, &CSettingDlg::OnBnClickedChangepenstyle15)
	ON_CBN_SELCHANGE(IDC_GridCount, &CSettingDlg::OnCbnSelchangeGridcount)
	ON_EN_CHANGE(IDC_MinDataSize, &CSettingDlg::OnEnChangeMindatasize)
	ON_BN_CLICKED(IDC_GetSettingInLoading, &CSettingDlg::OnBnClickedGetsettinginloading)
	ON_BN_CLICKED(IDC_ReqHeaderLog, &CSettingDlg::OnBnClickedReqheaderlog)
	ON_EN_CHANGE(IDC_MaxDataSize, &CSettingDlg::OnEnChangeMaxdatasize)
	ON_BN_CLICKED(IDC_ChangeSelectGraph0, &CSettingDlg::OnBnClickedChangeselectgraph0)
	ON_BN_CLICKED(IDC_ChangeSelectGraph1, &CSettingDlg::OnBnClickedChangeselectgraph1)
	ON_BN_CLICKED(IDC_SaveMode0, &CSettingDlg::ChangeDataSaveMode)
	ON_BN_CLICKED(IDC_SaveMode1, &CSettingDlg::ChangeDataSaveMode)
	ON_BN_CLICKED(IDC_SetPath, &CSettingDlg::OnBnClickedSetpath)
	ON_EN_CHANGE(IDC_nGridX, &CSettingDlg::OnEnChangengridx)
	ON_BN_CLICKED(IDC_ChangeSelectItemColor, &CSettingDlg::OnBnClickedChangeselectitemcolor)
	ON_BN_CLICKED(IDC_UseIntegerData, &CSettingDlg::OnBnClickedUseintegerdata)
	ON_BN_CLICKED(IDC_TextColorMode0, &CSettingDlg::OnBnClickedTextcolormode0)
	ON_BN_CLICKED(IDC_TextColorMode4, &CSettingDlg::OnBnClickedTextcolormode0)
	ON_BN_CLICKED(IDC_TextColorMode2, &CSettingDlg::OnBnClickedTextcolormode0)
	ON_BN_CLICKED(IDC_TextColorMode3, &CSettingDlg::OnBnClickedTextcolormode0)
	ON_BN_CLICKED(IDC_TextColorMode1, &CSettingDlg::OnBnClickedTextcolormode0)
	ON_BN_CLICKED(IDC_BKColorMode2, &CSettingDlg::OnBnClickedBkcolormode2)
	ON_BN_CLICKED(IDC_BKColorMode5, &CSettingDlg::OnBnClickedBkcolormode2)
	ON_BN_CLICKED(IDC_BKColorMode3, &CSettingDlg::OnBnClickedBkcolormode2)
	ON_BN_CLICKED(IDC_BKColorMode4, &CSettingDlg::OnBnClickedBkcolormode2)
	ON_BN_CLICKED(ID_SaveReg, &CSettingDlg::OnBnClickedSavereg)
	ON_BN_CLICKED(ID_InitReg, &CSettingDlg::OnBnClickedInitreg)
	ON_EN_CHANGE(IDC_MaxDataDigit, &CSettingDlg::OnEnChangeMaxdatadigit)
	ON_BN_CLICKED(IDC_ReqHeaderLog2, &CSettingDlg::OnBnClickedReqheaderlog2)
	ON_BN_CLICKED(IDC_NaviLogConvert, &CSettingDlg::OnBnClickedNavilogconvert)
	ON_BN_CLICKED(IDC_Use_Xinput, &CSettingDlg::OnBnClickedUseXinput)
	ON_BN_CLICKED(IDC_XInputSend, &CSettingDlg::OnBnClickedXinputsend)
    ON_BN_CLICKED(IDC_PageUp, &CSettingDlg::OnBnClickedPageup)
    ON_BN_CLICKED(IDC_PageDn, &CSettingDlg::OnBnClickedPagedn)
	ON_BN_CLICKED(IDC_SetDataPage, &CSettingDlg::OnBnClickedSetdatapage)
	ON_BN_CLICKED(IDC_SetMathPage2, &CSettingDlg::OnBnClickedSetmathpage)
	ON_BN_CLICKED(IDC_SetTestDataSize, &CSettingDlg::OnBnClickedSettestdatasize)
	ON_EN_CHANGE(IDC_TestDataSize, &CSettingDlg::OnEnChangeTestdatasize)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_PositionGraphSelX, &CSettingDlg::OnEnChangePositiongraphselx)
	ON_EN_CHANGE(IDC_PositionGraphSelY, &CSettingDlg::OnEnChangePositiongraphsely)
	ON_BN_CLICKED(ID_ConvertReg, &CSettingDlg::OnBnClickedConvertreg)
	ON_BN_CLICKED(ID_ConvertReg2, &CSettingDlg::OnBnClickedConvertreg2)
	ON_BN_CLICKED(IDC_DataSize0, &CSettingDlg::ChangeDataPage)
	ON_BN_CLICKED(IDC_DataSize1, &CSettingDlg::ChangeDataPage)
	ON_BN_CLICKED(IDC_DataSize2, &CSettingDlg::ChangeDataPage)
	ON_BN_CLICKED(IDC_DataSize3, &CSettingDlg::ChangeDataPage)
    ON_EN_CHANGE(IDC_ChangeHeader_RxCount, &CSettingDlg::OnEnChangeChangeheaderRxcount)
    ON_EN_CHANGE(IDC_MaxDataSize2, &CSettingDlg::OnEnChangeMaxdatasize2)
    ON_EN_CHANGE(IDC_MinDataSize2, &CSettingDlg::OnEnChangeMindatasize2)
		ON_EN_CHANGE(IDC_AutoSaveTimeout, &CSettingDlg::OnEnChangeAutosavetimeout)
		ON_EN_CHANGE(IDC_AutoSaveLogCount, &CSettingDlg::OnEnChangeAutosavelogcount)
        ON_EN_CHANGE(IDC_LogUpdateCount, &CSettingDlg::OnEnChangeLogupdatecount)
        END_MESSAGE_MAP()

// CSettingDlg 메시지 처리기

void CSettingDlg::OnBnClickedSaveclose() {
	UDT;

	int iPreDataPage = pMain->m_iDataPage;
	int iPreMathPage = pMain->m_iMathPage;
    int iHistoryCapacty = pMain->m_iHistoryCapacity;
	if (IsDlgButtonChecked(IDC_SetMathPage2)) {
		int idc[N_MAX_MATH_PAGE] = { IDC_MathSize0, IDC_MathSize1, IDC_MathSize2, IDC_MathSize3 };
		for (int i = 0; i < N_MAX_MATH_PAGE; i++) {
			if (IsDlgButtonChecked(idc[i])) {
				pMain->m_iMathPage = i + 1;
				pMain->m_iMaxMathSize = MAX_MATHCONTROL_SIZE * (i + 1);
				break;
			}
		}
	}
	if (IsDlgButtonChecked(IDC_SetDataPage)) {
		int idc[N_MAX_DATA_PAGE] = { IDC_DataSize0, IDC_DataSize1, IDC_DataSize2, IDC_DataSize3 };
		for (int i = 0; i < N_MAX_MATH_PAGE; i++) {
			if (IsDlgButtonChecked(idc[i])) {
				pMain->m_iDataPage = i + 1;
				pMain->m_iMaxDataSize = MAX_DATACONTROL_SIZE * (i + 1);
				break;
			}
		}
	}

    if (IsDlgButtonChecked(IDC_SetTestDataSize)) {
        pMain->m_iHistoryCapacity = GetDlgItemInt(IDC_TestDataSize);        
        if (pMain->m_iHistoryCapacity == 0) {
            pMain->m_iHistoryCapacity = DEFAULT_TESTDATA_SIZE;
        }
        pMain->m_iHistoryCapacity = max(MIN_TESTDATA_SIZE, min(pMain->m_iHistoryCapacity, MAX_TESTDATA_SIZE));
    }
	
	if (pMain->m_iDataPage != iPreDataPage || pMain->m_iMathPage != iPreMathPage || iHistoryCapacty != pMain->m_iHistoryCapacity) {
		if (pMain->m_iDataPage != iPreDataPage) {
			// MathProbe 정의도 다시로딩 필요함
			pMain->LoadMathProbeDefine(iPreDataPage);			
		}
		pMain->InitHistoryData();
	}	

	for (int i = 0; i < MAX_BT_COMMAND; i++) 
		pMain->m_BT_Command[i] = m_BT_Command[i];

	pMain->m_bUseIntData = IsDlgButtonChecked(IDC_UseIntegerData);
	for (int iData = 0; iData < MAXIMUM_DATA_SIZE; iData++) {
		for (int iColor = 0; iColor < 3; iColor++)
		{
			m_iPenColor[iData][iColor] = atoi(m_strPenColor[iData][iColor]);
			pMain->m_iPenColor[iData][iColor] = m_iPenColor[iData][iColor];
		}
		pMain->m_fCoff[iData] = atof(m_strCoff[iData]);
		pMain->m_iPenType[iData] = m_iPenType[iData];
		pMain->m_iPenThickness[iData] = max(1, min(10, atoi(m_strPenThickness[iData])));
		pMain->m_bUseDataMark[iData] = m_bUseDataMark[iData];
	}
	pMain->m_iTimeDisplayMode = m_iTimeDisplayMode;
	
	pMain->m_bFixGraphFrame = m_bFixGraphFrame;

	for (int i = 0; i < MAX_FONT_SIZE; i++) {
		pMain->m_iControlFontSize[i] = m_iFontSize[i];
	}

	pMain->m_bUsePositionGraph = m_bUsePositionGraph;
	pMain->m_strPositonGraphHeader.Format("%s", (LPCTSTR)m_strPositonGraphHeader);
	UpdateXY(0);
	UpdateXY(1);
	pMain->m_iSelectPositionGraphX = m_iSelectPositionGraphX;
	pMain->m_iSelectPositionGraphY = m_iSelectPositionGraphY;

	if (m_bUsePositionGraph && m_iSelectPositionGraphX >= 0 && m_iSelectPositionGraphX < MAX_DATACONTROL_SIZE + pMain->m_iMaxMathSize && m_iSelectPositionGraphY >= 0 && m_iSelectPositionGraphY < MAX_DATACONTROL_SIZE + pMain->m_iMaxMathSize) {
		pMain->m_bAvailablePositionGraph = TRUE;
	}
	else {
		pMain->m_bAvailablePositionGraph = FALSE;
	}

	pMain->m_iGridCountY = m_iGridCountY;

	pMain->LoadHeaderConfiguration();
	pMain->m_bRetrunSubProgram = TRUE;
	pMain->m_iMarkSize = m_iMarkSize;
	pMain->m_iMinPacketDataSize = GetDlgItemInt(IDC_MinDataSize2);
	pMain->m_iMaxPacketDataSize = GetDlgItemInt(IDC_MaxDataSize2);
	pMain->m_iMaxDataDigit = GetDlgItemInt(IDC_MaxDataDigit);

	pMain->m_iSelBkColorMode = m_iBkColorMode;
	pMain->m_iSelTextColorMode = m_iTextColorMode;
	for (int i = 0; i < 3; i++) {
		pMain->m_iSelTextColor[i] = atoi(m_strSelTextColor[i]);
		pMain->m_iSelBkColor[i] = atoi(m_strSelBkColor[i]);
	}

	pMain->m_bGetSettingInLoading = m_bGetSettingInLoading;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < N_COLOR_DRAW; j++) {
			pMain->m_iRGB_SelectControl_Select[i][j] = m_iRGB_SelectControl_Select[i][j];
			pMain->m_iRGB_SelectControl_Outline[i][j] = m_iRGB_SelectControl_Outline[i][j];
			pMain->m_iRGB_SelectControl_Blank[i][j] = m_iRGB_SelectControl_Blank[i][j];
		}
	}

	for (int j = 0; j < N_COLOR_DRAW; j++) {
		pMain->m_BrushSelectControl_Blank[j].CreateSolidBrush(RGB(m_iRGB_SelectControl_Blank[0][j], m_iRGB_SelectControl_Blank[1][j], m_iRGB_SelectControl_Blank[2][j]));
		pMain->m_BrushSelectControl_Select[j].CreateSolidBrush(RGB(m_iRGB_SelectControl_Select[0][j], m_iRGB_SelectControl_Select[1][j], m_iRGB_SelectControl_Select[2][j]));
		pMain->m_PenSelectControl_Blank[j].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Outline[0][j], m_iRGB_SelectControl_Outline[1][j], m_iRGB_SelectControl_Outline[2][j]));
		pMain->m_PenSelectControl_Select[j].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Select[0][j], m_iRGB_SelectControl_Select[1][j], m_iRGB_SelectControl_Select[2][j]));
	}

	if (IsDlgButtonChecked(IDC_ShowSizeAdjustCursor)) {
		pMain->m_bShowResizeCursor = TRUE;
	}
	else {
		pMain->m_bShowResizeCursor = FALSE;
	}

	if (m_iSaveMode == 0) {
		pMain->SetRegRoot_RegistryData("Common", "SaveMode", "0");
	}
	else {
		pMain->SetRegRoot_RegistryData("Common", "SaveMode", "1");
	}
	pMain->m_iSaveMode = m_iSaveMode;	
	pMain->m_iGridX_Type = m_iGridX_Type;
	pMain->m_iGridX = m_iGridX;

	for (int i = 0; i < N_ITEM_COLOR; i++) {
		if (m_bChangedColorItem[i]) {
			switch (i) {
			case 0:
			case 1:
				for (int j = 0; j < 3; j++) {
					pMain->m_iBackgroundColor[i][j] = m_iChangeColor[i][j];
				}
				pMain->m_BrushBackGround[i].CreateSolidBrush(RGB(m_iChangeColor[i][0], m_iChangeColor[i][1], m_iChangeColor[i][2]));
				break;
			case 2:
				for (int j = 0; j < 3; j++) {
					pMain->m_iTextColor[j] = m_iChangeColor[i][j];
				}
				break;
			}
		}
	}

	pMain->m_bShowProbeValue = IsDlgButtonChecked(IDC_ShowDataValue);
	pMain->m_bShowProbeDataID = IsDlgButtonChecked(IDC_ShowDataID);
	pMain->m_bShowTimelineN = IsDlgButtonChecked(IDC_ShowTimelineN);
	pMain->m_LoadSettingInfo_in_DataLoading = IsDlgButtonChecked(IDC_LoadSettingInfo_in_dataLoading);
	pMain->m_bUse_Xinput = IsDlgButtonChecked(IDC_Use_Xinput);
    
	pMain->m_bUseAutoHeader_rx = pMain->m_bUseAutoHeader_rx_reg = IsDlgButtonChecked(IDC_ChangeHeader_Rx);
	pMain->UpdateAutoHederRx();
    pMain->m_iAutoHeader_rx_count = GetDlgItemInt(IDC_ChangeHeader_RxCount);
    pMain->m_bUseAutoHeader_tx = IsDlgButtonChecked(IDC_ChangeHeader_Tx);
	
	CString str;
	GetDlgItemText(IDC_SavePath, str);
	if (str != "")
	pMain->SetRegRoot_RegistryData("Common", "SaveFolder", str);
	pMain->m_strSavePath[1].Format("%s", (LPCTSTR)str);

	if (IsDlgButtonChecked(IDC_WheelZoom0)) {
		pMain->m_bMouseWheelZoomDir = FALSE;
	}
	else {
		pMain->m_bMouseWheelZoomDir = TRUE;
	}

	if (IsDlgButtonChecked(IDC_WheelShift0)) {
		pMain->m_bMouseWheelShiftDir = FALSE;
	}
	else {
		pMain->m_bMouseWheelShiftDir = TRUE;
	}
	
	pMain->m_bUseAutosave1_LogCountLimit = IsDlgButtonChecked(IDC_UseAutoSaveCond0);
	pMain->m_bUseAutosave2_Time_LogCount = IsDlgButtonChecked(IDC_UseAutoSaveCond1);
	pMain->m_bAutosave_AddCapture = IsDlgButtonChecked(IDC_AutoTimeCapture);
	pMain->m_iAutosave2_cond_Time = max(0, GetDlgItemInt(IDC_AutoSaveTimeout)) * 1000;
	pMain->m_iAutosave2_cond_LogCount = max(0, GetDlgItemInt(IDC_AutoSaveLogCount));

	if (pMain->m_iAutosave2_cond_Time + pMain->m_iAutosave2_cond_LogCount == 0) {
		pMain->m_bUseAutosave2_Time_LogCount = false;
	}

    pMain->m_iLOG_Update_Period = GetDlgItemInt(IDC_LogUpdateCount);
	OnOK();
}

BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    // Not Implemented Function : Disable
    GetDlgItem(IDC_UseIntegerData)->EnableWindow(FALSE);
    
    // Get Information from Main
	m_iDataPage = pMain->m_iDataPage;
	SetDlgItemInt(IDC_DataSize, m_iDataPage* MAX_DATACONTROL_SIZE);
	m_iMathPage = pMain->m_iMathPage;
	SetDlgItemInt(IDC_MathSize, m_iMathPage* MAX_MATHCONTROL_SIZE);
	
    m_iHistoryCapacity = pMain->m_iHistoryCapacity;    
	SetDlgItemInt(IDC_TestDataSize_R, m_iHistoryCapacity);

	for (int i = 0; i < MAX_BT_COMMAND; i++)
	{
		m_BT_Command[i] = pMain->m_BT_Command[i];
		m_strCommand[i].Format("%c", m_BT_Command[i]);		
	}
	UDF;

	for (int iData = 0; iData < MAXIMUM_DATA_SIZE; iData++) {
		for (int iColor = 0; iColor < 3; iColor++)
		{
			m_strPenColor[iData][iColor].Format("%d", pMain->m_iPenColor[iData][iColor]);
			m_iPenColor[iData][iColor] = pMain->m_iPenColor[iData][iColor];
		}
		m_iPenType[iData] = pMain->m_iPenType[iData];
		m_iPenThickness[iData] = pMain->m_iPenThickness[iData];
		m_strPenThickness[iData].Format("%d", m_iPenThickness[iData]);
		m_bUseDataMark[iData] = pMain->m_bUseDataMark[iData];

		m_strCoff[iData].Format("%f", pMain->m_fCoff[iData]);
	}

	for (int i = 0; i < MAX_FONT_SIZE; i++) {
		m_iFontSize[i] = pMain->m_iControlFontSize[i];
		m_strFontSize[i].Format("%d", m_iFontSize[i]);
	}
    m_iMarkSize = pMain->m_iMarkSize;
    UDF;

	m_bFixGraphFrame = pMain->m_bFixGraphFrame;
	m_bUsePositionGraph = pMain->m_bUsePositionGraph;
	m_strPositonGraphHeader.Format("%s", (LPCTSTR)pMain->m_strPositonGraphHeader);
	m_iSelectPositionGraphX = max(-1, min(pMain->m_iSelectPositionGraphX, m_iDataPage * MAX_DATACONTROL_SIZE + m_iMathPage * MAX_MATHCONTROL_SIZE));
	CString str;
	if (m_iSelectPositionGraphX == -1) {
		str = "";
	}
	else if (m_iSelectPositionGraphX > m_iDataPage * MAX_DATACONTROL_SIZE){
		str.Format("M%d", m_iSelectPositionGraphX - m_iDataPage * MAX_DATACONTROL_SIZE + 1);
	}
	else {
		str.Format("D%d", m_iSelectPositionGraphX + 1);
	}
	SetDlgItemText(IDC_PositionGraphSelX, str);
	
	m_iSelectPositionGraphY = max(-1, min(pMain->m_iSelectPositionGraphY, m_iDataPage * MAX_DATACONTROL_SIZE + m_iMathPage * MAX_MATHCONTROL_SIZE));
	if (m_iSelectPositionGraphY == -1) {
		str = "";
	}
	else if (m_iSelectPositionGraphY > m_iDataPage * MAX_DATACONTROL_SIZE) {
		str.Format("M%d", m_iSelectPositionGraphY - m_iDataPage * MAX_DATACONTROL_SIZE + 1);
	}
	else {
		str.Format("D%d", m_iSelectPositionGraphY + 1);
	}
	SetDlgItemText(IDC_PositionGraphSelY, str);

	m_iGridCountY = pMain->m_iGridCountY;
	
	m_strMinDataSize.Format("%d", max(0, m_iDataPage * MAX_DATACONTROL_SIZE - pMain->m_iMinPacketDataSize));
	m_strMaxDataSize.Format("%d", max(0, pMain->m_iMaxPacketDataSize - m_iDataPage * MAX_DATACONTROL_SIZE));

	str.Format("( %d ~ %d )", pMain->m_iMinPacketDataSize, pMain->m_iMaxPacketDataSize);
	SetDlgItemText(IDC_DataSizeRange, str);
	
	SetDlgItemInt(IDC_MaxDataDigit, pMain->m_iMaxDataDigit);
	SetDlgItemInt(IDC_MinDataSize2, pMain->m_iMinPacketDataSize);
	SetDlgItemInt(IDC_MaxDataSize2, pMain->m_iMaxPacketDataSize);
	m_iBkColorMode = pMain->m_iSelBkColorMode;
	m_iTextColorMode = pMain->m_iSelTextColorMode;
	for (int i = 0; i < 3; i++) {
		m_strSelTextColor[i].Format("%d", pMain->m_iSelTextColor[i]);
		m_strSelBkColor[i].Format("%d", pMain->m_iSelBkColor[i]);
	}
	m_bGetSettingInLoading = pMain->m_bGetSettingInLoading;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < N_COLOR_DRAW; j++) {
			m_iRGB_SelectControl_Select[i][j] = pMain->m_iRGB_SelectControl_Select[i][j];
			m_iRGB_SelectControl_Outline[i][j] = pMain->m_iRGB_SelectControl_Outline[i][j];
			m_iRGB_SelectControl_Blank[i][j] = pMain->m_iRGB_SelectControl_Blank[i][j];
			m_iDefaultRGB_SelectControl_Select[i][j] = pMain->m_iDefaultRGB_SelectControl_Select[i][j];
			m_iDefaultRGB_SelectControl_Outline[i][j] = pMain->m_iDefaultRGB_SelectControl_Outline[i][j];
			m_iDefaultRGB_SelectControl_Blank[i][j] = pMain->m_iDefaultRGB_SelectControl_Blank[i][j];
		}
	}

	for (int j = 0; j < N_COLOR_DRAW; j++) {
		m_BrushSelectControl_Blank[j].CreateSolidBrush(RGB(m_iRGB_SelectControl_Blank[0][j], m_iRGB_SelectControl_Blank[1][j], m_iRGB_SelectControl_Blank[2][j]));
		m_BrushSelectControl_Select[j].CreateSolidBrush(RGB(m_iRGB_SelectControl_Select[0][j], m_iRGB_SelectControl_Select[1][j], m_iRGB_SelectControl_Select[2][j]));
		m_PenSelectControl_Blank[j].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Outline[0][j], m_iRGB_SelectControl_Outline[1][j], m_iRGB_SelectControl_Outline[2][j]));
		m_PenSelectControl_Select[j].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Select[0][j], m_iRGB_SelectControl_Select[1][j], m_iRGB_SelectControl_Select[2][j]));
	}
	
	CheckDlgButton(IDC_ShowSizeAdjustCursor, pMain->m_bShowResizeCursor);
	SetDlgItemText(IDC_SavePath, pMain->GetRegRoot_RegistryData("Common", "SaveFolder"));
	m_iSaveMode = atoi(pMain->GetRegRoot_RegistryData("Common", "SaveMode"));
	m_iGridX_Type = pMain->m_iGridX_Type;
	m_iGridX = pMain->m_iGridX;

	if (pMain->m_bMouseWheelZoomDir) {
		CheckDlgButton(IDC_WheelZoom0, FALSE);
		CheckDlgButton(IDC_WheelZoom1, TRUE);
	}
	else {
		CheckDlgButton(IDC_WheelZoom0, TRUE);
		CheckDlgButton(IDC_WheelZoom1, FALSE);
	}

	if (pMain->m_bMouseWheelShiftDir) {
		CheckDlgButton(IDC_WheelShift0, FALSE);
		CheckDlgButton(IDC_WheelShift1, TRUE);
	}
	else {
		CheckDlgButton(IDC_WheelShift0, TRUE);
		CheckDlgButton(IDC_WheelShift1, FALSE);
	}
	UDF;	
    ShowDataPage();
	ChangeDataSaveMode();	

	CheckDlgButton(IDC_UseIntegerData, pMain->m_bUseIntData);
	OnBnClickedUseintegerdata();
	AddColumn();
	OnBnClickedTextcolormode0();
	OnBnClickedBkcolormode2();
	CheckDlgButton(IDC_ShowDataValue, pMain->m_bShowProbeValue);
	CheckDlgButton(IDC_ShowDataID, pMain->m_bShowProbeDataID);
	CheckDlgButton(IDC_ShowTimelineN, pMain->m_bShowTimelineN);
	CheckDlgButton(IDC_LoadSettingInfo_in_dataLoading, pMain->m_LoadSettingInfo_in_DataLoading);
	CheckDlgButton(IDC_Use_Xinput, pMain->m_bUse_Xinput);
	OnBnClickedUseXinput();

    CheckDlgButton(IDC_ChangeHeader_Rx, pMain->m_bUseAutoHeader_rx_reg);
    SetDlgItemInt(IDC_ChangeHeader_RxCount, pMain->m_iAutoHeader_rx_count);

    CheckDlgButton(IDC_ChangeHeader_Tx, pMain->m_bUseAutoHeader_tx);

	CheckDlgButton(IDC_UseAutoSaveCond0, pMain->m_bUseAutosave1_LogCountLimit);
	CheckDlgButton(IDC_UseAutoSaveCond1, pMain->m_bUseAutosave2_Time_LogCount);
	CheckDlgButton(IDC_AutoTimeCapture, pMain->m_bAutosave_AddCapture);
	SetDlgItemInt(IDC_AutoSaveTimeout, pMain->m_iAutosave2_cond_Time / 1000);
	SetDlgItemInt(IDC_AutoSaveLogCount, pMain->m_iAutosave2_cond_LogCount);

	if (pMain->m_iAutosave2_cond_Time + pMain->m_iAutosave2_cond_LogCount == 0) {
		CheckDlgButton(IDC_UseAutoSaveCond1, false);
	}
    SetDlgItemInt(IDC_LogUpdateCount, pMain->m_iLOG_Update_Period);

	GetDlgItem(IDC_STATIC)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSettingDlg::UpdateControl(void)
{
	UDT;

	for (int i = 0; i < MAX_BT_COMMAND; i++)
	{
		// 유효성 검증	
		int iSize = m_strCommand[i].GetLength();
		//m_strButtonCommand[i] = " ";

		if (iSize < 256)
		{
			char myString[256];
			strcpy_s(myString, (LPCTSTR)m_strCommand[i]);

			BOOL bFind = FALSE;

			for (int j = 0; j < iSize; j++)
			{				
				if (myString[j] != ' ') {
					m_strCommand[i].Format("%c", myString[j]);
					m_BT_Command[i] = myString[j];
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				m_strCommand[i] = " ";
				m_BT_Command[i] = ' ';
			}
		}
	}

	int iSize = m_strPositonGraphHeader.GetLength();

	if (iSize < 256)
	{
		char myString[256];
		strcpy_s(myString, (LPCTSTR)m_strPositonGraphHeader);

		BOOL bFind = FALSE;

		for (int j = 0; j < iSize; j++)
		{
			if (myString[j] != ' ') {
				m_strPositonGraphHeader.Format("%c", myString[j]);
				m_cGraphPositionHeader = myString[j];
				bFind = TRUE;
				break;
			}
		}

		if (!bFind)
		{
			m_strPositonGraphHeader = " ";
			m_cGraphPositionHeader = ' ';
		}
	}

	UDF;
}

void CSettingDlg::OnBnClickedUpdatecolor()
{
	CClientDC dc(this);

	CPen Pen;
	LOGBRUSH LogBrush;
	LogBrush.lbHatch = 0;
	LogBrush.lbStyle = PS_SOLID;
	UDT;

	int x0 = 250;
	int l = 60;
	int y0 = 95;
	int dy = 52;
		
	if (m_bReqRedraw) {		
		Invalidate();
		m_bReqRedraw = FALSE;
	}

	for (int i= 0; i < MAX_DATACONTROL_SIZE; i++)
	{
        int iData = MAX_DATACONTROL_SIZE * m_iCurrentDataPage + i;
		for (int iColor = 0; iColor < 3; iColor++)
		{
			m_iPenColor[iData][iColor] = atoi(m_strPenColor[iData][iColor]);
		}

		int iThickness = max(1, min(10, atoi(m_strPenThickness[iData])));
		m_strPenThickness[iData].Format("%d", iThickness);
		LogBrush.lbColor = RGB(m_iPenColor[iData][0], m_iPenColor[iData][1], m_iPenColor[iData][2]);
		Pen.DeleteObject();
		Pen.CreatePen(PS_GEOMETRIC | m_iPenType[iData], iThickness, &LogBrush, 0, 0);
		dc.SelectObject(&Pen);	
		dc.MoveTo(x0, y0 + i * dy / 2);
		if (m_bUseDataMark[iData]) {
			int iSize = iThickness / 2 + 1;
			if (iSize * 2 < iThickness + 2) iSize++;

			dc.Rectangle(x0 + l / 2 - iSize, y0 + dy * i / 2 - iSize, x0 + l / 2 + iSize, y0 + dy * i / 2 + iSize);
		}
		dc.LineTo(x0 + l, y0 + i * dy / 2);
	}
	// Active
	x0 = 860; y0 = 319; dy = 75;
	int iLength = 25;	
	int gap = 3;
	dc.SelectObject(&(m_BrushSelectControl_Blank[1]));
	dc.SelectObject(&(m_PenSelectControl_Blank[1]));
	dc.Rectangle(x0, y0, x0 + iLength, y0 + iLength);
	dc.Rectangle(x0 + iLength + 5, y0, x0 + 2 * iLength + 5, y0 + iLength);

	dc.SelectObject(&(m_BrushSelectControl_Select[1]));
	dc.SelectObject(&(m_PenSelectControl_Select[1]));
	dc.Rectangle(x0 + iLength + 5 + gap, y0 + gap, x0 + 2 * iLength + 5 - gap, y0 + iLength - gap);

	y0 += dy;
	dc.SelectObject(&(m_BrushSelectControl_Blank[0]));
	dc.SelectObject(&(m_PenSelectControl_Blank[0]));
	dc.Rectangle(x0, y0, x0 + iLength, y0 + iLength);
	dc.Rectangle(x0 + iLength + 5, y0, x0 + 2 * iLength + 5, y0 + iLength);

	dc.SelectObject(&(m_BrushSelectControl_Select[0]));
	dc.SelectObject(&(m_PenSelectControl_Select[0]));
	dc.Rectangle(x0 + iLength + 5 + gap, y0 + gap, x0 + 2 * iLength + 5 - gap, y0 + iLength - gap);
}


void CSettingDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDblClk(nFlags, point);
	//CString str;
	//str.Format("%d %d", point.x, point.y);
	//SetDlgItemText(IDC_Description, str);
}


void CSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting.
	OnBnClickedUpdatecolor();
}


void CSettingDlg::OnBnClickedLoaddefaultcolor()
{
	for (int iData = 0; iData < MAXIMUM_DATA_SIZE; iData++) {
		for (int iColor = 0; iColor < 3; iColor++) {
			m_strPenColor[iData][iColor].Format("%d", pMain->m_iDefaultDataColor[iData][iColor]);
		}
		m_iPenType[iData] = 0;
		m_strPenThickness[iData] = "3";
		m_bUseDataMark[iData] = TRUE;
	}
	UDF;
	Invalidate();
	OnBnClickedUpdatecolor();
}

void CSettingDlg::OnEnChangeFontsizesetting()
{
	UDT;
	for (int i = 0; i < MAX_FONT_SIZE; i++) {
		m_iFontSize[i] = min(20, max(1, atoi(m_strFontSize[i])));
		m_strFontSize[i].Format("%d", m_iFontSize[i]);
	}
	UDF;
}


void CSettingDlg::AddColumn() {
	LV_COLUMN lvcolumn;
	char *listColumn[3] = { "No.", "Model Name", "Description"};
	int width[3] = { 1, 1, 1 };
	int i;

	for (i = 0; i < 3; i++)
	{
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.pszText = listColumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		m_ModelList.InsertColumn(i, &lvcolumn);
	}

	m_ModelList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (i = 0; i < m_ModelList.GetHeaderCtrl()->GetItemCount(); i++)
		VERIFY(m_ModelList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));

	InsertItem("default", "Default Model (Read Only)");

	CString strAddress, strItem, strValue, strName, strDescription;
	//Update Combobox
	strAddress.Format("Model");
	strItem.Format("nModel");
	int nModel = atoi(pMain->GetRegRoot_RegistryData(strAddress, strItem));

	for (int i = 1; i < nModel; i++) {
		strItem.Format("ModelName_%d", i);
		strName = pMain->GetRegRoot_RegistryData(strAddress, strItem);

		strItem.Format("ModelDescription_%d", i);
		strDescription = pMain->GetRegRoot_RegistryData(strAddress, strItem);
		InsertItem(strName, strDescription);
	}

	m_iCopyModel = 0;
	int nModelMain = pMain->m_cSelectModel.GetCount();
	for (int i = 1; i < nModelMain; i++) {
		CString str;
		pMain->m_cSelectModel.GetLBText(i, str);
		m_cCopyModel.AddString(str);
	}
	UDF;
}

void CSettingDlg::InsertItem(const CString& strModelName, const CString& strDescription)
{
	LV_ITEM lvitem;
	CString str, strAddress, strItem, strValue;
	int nCount = m_ModelList.GetItemCount();
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;


	lvitem.iItem = nCount;
	str.Format("%d", lvitem.iItem);
	lvitem.pszText = (LPSTR)(LPCSTR)str;
	m_ModelList.InsertItem(&lvitem);

	m_ModelList.SetItemText(nCount, 1, strModelName);
	m_ModelList.SetItemText(nCount, 2, strDescription);
	for (int i = 0; i < m_ModelList.GetHeaderCtrl()->GetItemCount(); i++)
		VERIFY(m_ModelList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));

	nCount++;
}

void CSettingDlg::OnClickModellist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_iSelectList = pNMItemActivate->iItem;

	*pResult = 0;
}

void CSettingDlg::OnBnClickedAddmodel()
{
	// ModelName 유효성 검증
	UDT;
	if (m_strModelName.GetLength() == 0) {
		MessageBox("Error(Add Model)\r\n No Model Name\r\n  → Input Model Name"); return;
	}

	if (m_strModelName.Find(" ") > -1) {
		MessageBox("Error(Add Model)\r\n Find spacer(' ')\r\n  → Remove spacer in Model Name"); return;
	}

	int iListCount = m_ModelList.GetItemCount();
	CString strModelName;
	strModelName.Format("%s", (LPCTSTR)m_strModelName);
	strModelName.MakeUpper();
	for (int i = 0; i < iListCount; i++) {
		CString strModel = m_ModelList.GetItemText(i, 1);
		strModel.MakeUpper();
		if (strModel == strModelName) {
			MessageBox("Error(Add Model)\r\n Same Name is already exist\r\n  → Change Model Name"); return;
		}
	}
	InsertItem(m_strModelName, m_strDescription);

	CString str;
	iListCount = m_ModelList.GetItemCount();
	str.Format("%d", iListCount);
	pMain->SetRegRoot_RegistryData("Model", "nModel", str);

	for (int i = 1; i < iListCount; i++) {
		str.Format("ModelName_%d", i);
		pMain->SetRegRoot_RegistryData("Model", str, m_ModelList.GetItemText(i, 1));
		str.Format("ModelDescription_%d", i);
		pMain->SetRegRoot_RegistryData("Model", str, m_ModelList.GetItemText(i, 2));
	}	   

	m_cCopyModel.AddString(m_strModelName);
	pMain->m_cSelectModel.AddString(m_strModelName);
	m_strModelName = "", m_strDescription = "";
	
	m_iSelectList = -1;
	pMain->LoadHeaderConfiguration();
	UDF;
}

void CSettingDlg::OnBnClickedRemovemodel()
{
	UDT;
	if (m_iSelectList == -1) { AfxMessageBox("선택된 항목이 없습니다(No item is selected)."); return; }
	if (m_iSelectList == 0) {
		MessageBox("Default Model은 지울수 없습니다(Default model cannot be deleted)."); return;
	}
	CString strSelectModel = m_ModelList.GetItemText(m_iSelectList, 1);
	CString str;
	str.Format("선택 항목을 지울까요 (Delete selected item)? : %s", (LPCTSTR)strSelectModel);

	if (AfxMessageBox(str, MB_YESNO) == IDNO) return;

	LV_ITEM lvitem;
	CString strValue, strItem, strAddress;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;

	m_ModelList.DeleteItem(m_iSelectList);
	UDF;

	// Total Cnt
	int iListCount = m_ModelList.GetItemCount();
	for (int i = m_iSelectList; i < iListCount; i++) {
		str.Format("%d", i);
		m_ModelList.SetItemText(i, 0, str);
	}

	str.Format("%d", iListCount);
	pMain->SetRegRoot_RegistryData("Model", "nModel", str);

	for (int i = 1; i < iListCount; i++) {
		str.Format("ModelName_%d", i);
		pMain->SetRegRoot_RegistryData("Model", str, m_ModelList.GetItemText(i, 1));
		str.Format("ModelDescription_%d", i);
		pMain->SetRegRoot_RegistryData("Model", str, m_ModelList.GetItemText(i, 2));
	}

	// Control수정
	// strSelectModel
	for (int i = 1; i < iListCount; i++) {
		m_cCopyModel.GetLBText(i, str);
		if (str == strSelectModel) {
			if (i == m_iCopyModel) {
				m_iCopyModel = 0;
				UDF;
				Wait(10);
			}
			m_cCopyModel.DeleteString(i);
		}
	}

	for (int i = 1; i < iListCount; i++) {
		pMain->m_cSelectModel.GetLBText(i, str);
		if (str == strSelectModel) {
			if (i == pMain->m_iSelectModel) {
				pMain->m_iSelectModel = 0;
				pMain->UDF;
				Wait(10);
			}
			pMain->m_cSelectModel.DeleteString(i);
		}
	}

	m_iSelectList = -1;	
}

void CSettingDlg::Wait(DWORD dwMillisecond) {
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
void CSettingDlg::OnCbnSelchangeCopymodel() {
	UDT;
}

void CSettingDlg::OnEnChangeAddmodelname() {
	UDT;
}

void CSettingDlg::OnEnChangeDescription() {
	UDT;
}


void CSettingDlg::OnBnClickedCopysetting()
{
	CString strSourceModel, strTargetModel, str;
	if (m_iSelectList == -1) {
		AfxMessageBox("선택된 항목이 없습니다 (No item selected).");
		return;
	}

	if (m_iSelectList == m_iCopyModel) {
		AfxMessageBox("현재 Model이 선택되어 복사를 취소합니다.\r\n(Copy canceled: the current model is selected.)");
		return;
	}

	m_cCopyModel.GetLBText(m_iCopyModel, strSourceModel);
	strTargetModel.Format("%s", (LPCTSTR)m_ModelList.GetItemText(m_iSelectList, 1));
	str.Format("설정을 복사할까요(Copy the settings)? : %s -> %s", (LPCTSTR)strSourceModel, (LPCTSTR)strTargetModel);
	CString strAddressTarget, strAddressSource, strAddressTarget0, strAddressSource0, strItem, strValue;
	
	if (strTargetModel == "default") {
		strAddressTarget0 = "Graph\\";
	}
	else {
		strAddressTarget0.Format("Model\\%s\\Graph\\", (LPCTSTR)strTargetModel);
	}

	if (strSourceModel == "default") {
		strAddressSource0 = "Graph\\";
	}
	else {
		strAddressSource0.Format("Model\\%s\\Graph\\", (LPCTSTR)strSourceModel);
	}

	if (AfxMessageBox(str, MB_YESNO) == IDYES) {
		for (BYTE i = 'a'; i <= 'z'; i++) {
			strAddressSource.Format("%s%c%d", (LPCTSTR)strAddressSource0, i, i);
			strAddressTarget.Format("%s%c%d", (LPCTSTR)strAddressTarget0, i, i);
			// Source Command 
			for (int j = 0; j < 16; j++) {
				strItem.Format("PinName%02d", j);
				strValue = pMain->GetRegRoot_RegistryData(strAddressSource, strItem);

				if (strValue != "")
					 pMain->SetRegRoot_RegistryData(strAddressTarget, strItem, strValue);
			}
		}

		for (BYTE i = 'A'; i <= 'Z'; i++) {
			strAddressSource.Format("%s%c%d", (LPCTSTR)strAddressSource0, i, i);
			strAddressTarget.Format("%s%c%d", (LPCTSTR)strAddressTarget0, i, i);
			// Source Command 
			for (int j = 0; j < 16; j++) {
				strItem.Format("PinName%02d", j);
				strValue = pMain->GetRegRoot_RegistryData(strAddressSource, strItem);

				if (strValue != "")
					pMain->SetRegRoot_RegistryData(strAddressTarget, strItem, strValue);
			}
		}
	}
	pMain->LoadHeaderConfiguration();
}

void CSettingDlg::OnBnClickedCancel()
{
	pMain->LoadHeaderConfiguration();
	CDialogEx::OnCancel();
}


void CSettingDlg::ReqUpdateColor() {
	m_bReqRedraw = TRUE;
	OnBnClickedUpdatecolor();
}

void CSettingDlg::OnBnClickedChangepenstyle0() { ChangePenstyle(0 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle1() { ChangePenstyle(1 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle2() { ChangePenstyle(2 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle3() { ChangePenstyle(3 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle4() { ChangePenstyle(4 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle5() { ChangePenstyle(5 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle6() { ChangePenstyle(6 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle7() { ChangePenstyle(7 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle8() { ChangePenstyle(8 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle9() { ChangePenstyle(9 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle10() { ChangePenstyle(10 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle11() { ChangePenstyle(11 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle12() { ChangePenstyle(12 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle13() { ChangePenstyle(13 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle14() { ChangePenstyle(14 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }
void CSettingDlg::OnBnClickedChangepenstyle15() { ChangePenstyle(15 + m_iCurrentDataPage * MAX_DATACONTROL_SIZE); }

void CSettingDlg::ChangePenstyle(int iChannel) {
	CColorSelectDlg dlg;
	dlg.pAppMain = pMain->pAppMain;
	if (pMain->pAppMain->m_PropertyChange.bIsBusy) {
		AfxMessageBox("Property수정 모드 활성중에는 사용 불가합니다. \r\n(Unavailable while Property Edit mode is active.)");
		return;
	}

	pMain->pAppMain->m_PropertyChange.bIsBusy = TRUE;
	pMain->pAppMain->m_PropertyChange.bIsChanged = FALSE;

	pMain->pAppMain->m_PropertyChange.Color[0][0] = atoi(m_strPenColor[iChannel][0]);
	pMain->pAppMain->m_PropertyChange.Color[1][0] = atoi(m_strPenColor[iChannel][1]);
	pMain->pAppMain->m_PropertyChange.Color[2][0] = atoi(m_strPenColor[iChannel][2]);
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][0] = pMain->m_iDefaultDataColor[iChannel][0];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][0] = pMain->m_iDefaultDataColor[iChannel][1];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][0] = pMain->m_iDefaultDataColor[iChannel][2];
	pMain->pAppMain->m_PropertyChange.iLineType = m_iPenType[iChannel];
	pMain->pAppMain->m_PropertyChange.iThickness = atoi(m_strPenThickness[iChannel]);
	pMain->pAppMain->m_PropertyChange.bUseMark = m_bUseDataMark[iChannel];
	dlg.DoModal();
	pMain->pAppMain->m_PropertyChange.bIsBusy = FALSE;
	if (pMain->pAppMain->m_PropertyChange.bIsChanged) {
		m_strPenColor[iChannel][0].Format("%d", pMain->pAppMain->m_PropertyChange.Color[0][0]);
		m_strPenColor[iChannel][1].Format("%d", pMain->pAppMain->m_PropertyChange.Color[1][0]);
		m_strPenColor[iChannel][2].Format("%d", pMain->pAppMain->m_PropertyChange.Color[2][0]);

		m_iPenType[iChannel] = pMain->pAppMain->m_PropertyChange.iLineType;
		m_iPenThickness[iChannel] = pMain->pAppMain->m_PropertyChange.iThickness;
		m_strPenThickness[iChannel].Format("%d", m_iPenThickness[iChannel]);
		m_bUseDataMark[iChannel] = pMain->pAppMain->m_PropertyChange.bUseMark;
		Invalidate();
		OnBnClickedUpdatecolor();
	}
}

void CSettingDlg::OnCbnSelchangeGridcount() {
	UDT;
}


void CSettingDlg::OnEnChangeMindatasize() {
	UDT;
	int iSize = max(0, min(MAX_DATACONTROL_SIZE * m_iDataPage - 5, atoi(m_strMinDataSize)));
	m_strMinDataSize.Format("%d", iSize);

	SetDlgItemInt(IDC_MinDataSize2, MAX_DATACONTROL_SIZE * m_iDataPage - iSize);
	CString str;
	str.Format("( %d ~ %d )", MAX_DATACONTROL_SIZE * m_iDataPage - iSize, GetDlgItemInt(IDC_MaxDataSize2));
	SetDlgItemText(IDC_DataSizeRange, str);
	UDF;
}


void CSettingDlg::OnBnClickedGetsettinginloading() {
	UDT;
}


void CSettingDlg::OnBnClickedReqheaderlog() {
	CHeaderLogDlg dlg;
	dlg.pMain = pMain;
	dlg.DoModal();
}


void CSettingDlg::OnEnChangeMaxdatasize()
{
	UDT;
	int iSize = max(0, min(80, atoi(m_strMaxDataSize)));
	m_strMaxDataSize.Format("%d", iSize);
	SetDlgItemInt(IDC_MaxDataSize2, MAX_DATACONTROL_SIZE * m_iDataPage + iSize);

	CString str;
	
	str.Format("( %d ~ %d )", GetDlgItemInt(IDC_MinDataSize2), MAX_DATACONTROL_SIZE * m_iDataPage + iSize);
	SetDlgItemText(IDC_DataSizeRange, str);

	UDF;
}


void CSettingDlg::OnBnClickedChangeselectgraph0() {
	int iActive = 1;
	CColorSelectDlg dlg;
	dlg.pAppMain = pMain->pAppMain;
	if (pMain->pAppMain->m_PropertyChange.bIsBusy) {		
        AfxMessageBox("Property수정 모드 활성중에는 사용 불가합니다. \r\n(Unavailable while Property Edit mode is active.)");
		return;
	}
	dlg.m_iMode = 1;
	pMain->pAppMain->m_PropertyChange.bIsBusy = TRUE;
	pMain->pAppMain->m_PropertyChange.bIsChanged = FALSE;
	
	pMain->pAppMain->m_PropertyChange.Color[0][0] = m_iRGB_SelectControl_Outline[0][iActive];
	pMain->pAppMain->m_PropertyChange.Color[1][0] = m_iRGB_SelectControl_Outline[1][iActive];
	pMain->pAppMain->m_PropertyChange.Color[2][0] = m_iRGB_SelectControl_Outline[2][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][0] = m_iDefaultRGB_SelectControl_Outline[0][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][0] = m_iDefaultRGB_SelectControl_Outline[1][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][0] = m_iDefaultRGB_SelectControl_Outline[2][iActive];

	pMain->pAppMain->m_PropertyChange.Color[0][1] = m_iRGB_SelectControl_Blank[0][iActive];
	pMain->pAppMain->m_PropertyChange.Color[1][1] = m_iRGB_SelectControl_Blank[1][iActive];
	pMain->pAppMain->m_PropertyChange.Color[2][1] = m_iRGB_SelectControl_Blank[2][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][1] = m_iDefaultRGB_SelectControl_Blank[0][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][1] = m_iDefaultRGB_SelectControl_Blank[1][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][1] = m_iDefaultRGB_SelectControl_Blank[2][iActive];

	pMain->pAppMain->m_PropertyChange.Color[0][2] = m_iRGB_SelectControl_Select[0][iActive];
	pMain->pAppMain->m_PropertyChange.Color[1][2] = m_iRGB_SelectControl_Select[1][iActive];
	pMain->pAppMain->m_PropertyChange.Color[2][2] = m_iRGB_SelectControl_Select[2][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][2] = m_iDefaultRGB_SelectControl_Select[0][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][2] = m_iDefaultRGB_SelectControl_Select[1][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][2] = m_iDefaultRGB_SelectControl_Select[2][iActive];

	dlg.DoModal();

	pMain->pAppMain->m_PropertyChange.bIsBusy = FALSE;

	if (pMain->pAppMain->m_PropertyChange.bIsChanged) {
		m_iRGB_SelectControl_Outline[0][iActive] = pMain->pAppMain->m_PropertyChange.Color[0][0];
		m_iRGB_SelectControl_Outline[1][iActive] = pMain->pAppMain->m_PropertyChange.Color[1][0];
		m_iRGB_SelectControl_Outline[2][iActive] = pMain->pAppMain->m_PropertyChange.Color[2][0];

		m_iRGB_SelectControl_Blank[0][iActive] = pMain->pAppMain->m_PropertyChange.Color[0][1];
		m_iRGB_SelectControl_Blank[1][iActive] = pMain->pAppMain->m_PropertyChange.Color[1][1];
		m_iRGB_SelectControl_Blank[2][iActive] = pMain->pAppMain->m_PropertyChange.Color[2][1];

		m_iRGB_SelectControl_Select[0][iActive] = pMain->pAppMain->m_PropertyChange.Color[0][2];
		m_iRGB_SelectControl_Select[1][iActive] = pMain->pAppMain->m_PropertyChange.Color[1][2];
		m_iRGB_SelectControl_Select[2][iActive] = pMain->pAppMain->m_PropertyChange.Color[2][2];

		m_BrushSelectControl_Blank[iActive].CreateSolidBrush(RGB(m_iRGB_SelectControl_Blank[0][iActive], m_iRGB_SelectControl_Blank[1][iActive], m_iRGB_SelectControl_Blank[2][iActive]));
		m_BrushSelectControl_Select[iActive].CreateSolidBrush(RGB(m_iRGB_SelectControl_Select[0][iActive], m_iRGB_SelectControl_Select[1][iActive], m_iRGB_SelectControl_Select[2][iActive]));
		m_PenSelectControl_Blank[iActive].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Outline[0][iActive], m_iRGB_SelectControl_Outline[1][iActive], m_iRGB_SelectControl_Outline[2][iActive]));
		m_PenSelectControl_Select[iActive].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Select[0][iActive], m_iRGB_SelectControl_Select[1][iActive], m_iRGB_SelectControl_Select[2][iActive]));

		Invalidate();
		OnBnClickedUpdatecolor();
	}
}


void CSettingDlg::OnBnClickedChangeselectgraph1() {
	int iActive = 0;
	CColorSelectDlg dlg;
	dlg.pAppMain = pMain->pAppMain;
	if (pMain->pAppMain->m_PropertyChange.bIsBusy) {
        AfxMessageBox("Property수정 모드 활성중에는 사용 불가합니다. \r\n(Unavailable while Property Edit mode is active.)");
		return;
	}
	dlg.m_iMode = 1;
	pMain->pAppMain->m_PropertyChange.bIsBusy = TRUE;
	pMain->pAppMain->m_PropertyChange.bIsChanged = FALSE;

	pMain->pAppMain->m_PropertyChange.Color[0][0] = m_iRGB_SelectControl_Outline[0][iActive];
	pMain->pAppMain->m_PropertyChange.Color[1][0] = m_iRGB_SelectControl_Outline[1][iActive];
	pMain->pAppMain->m_PropertyChange.Color[2][0] = m_iRGB_SelectControl_Outline[2][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][0] = m_iDefaultRGB_SelectControl_Outline[0][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][0] = m_iDefaultRGB_SelectControl_Outline[1][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][0] = m_iDefaultRGB_SelectControl_Outline[2][iActive];

	pMain->pAppMain->m_PropertyChange.Color[0][1] = m_iRGB_SelectControl_Blank[0][iActive];
	pMain->pAppMain->m_PropertyChange.Color[1][1] = m_iRGB_SelectControl_Blank[1][iActive];
	pMain->pAppMain->m_PropertyChange.Color[2][1] = m_iRGB_SelectControl_Blank[2][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][1] = m_iDefaultRGB_SelectControl_Blank[0][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][1] = m_iDefaultRGB_SelectControl_Blank[1][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][1] = m_iDefaultRGB_SelectControl_Blank[2][iActive];

	pMain->pAppMain->m_PropertyChange.Color[0][2] = m_iRGB_SelectControl_Select[0][iActive];
	pMain->pAppMain->m_PropertyChange.Color[1][2] = m_iRGB_SelectControl_Select[1][iActive];
	pMain->pAppMain->m_PropertyChange.Color[2][2] = m_iRGB_SelectControl_Select[2][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[0][2] = m_iDefaultRGB_SelectControl_Select[0][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[1][2] = m_iDefaultRGB_SelectControl_Select[1][iActive];
	pMain->pAppMain->m_PropertyChange.DefaultColor[2][2] = m_iDefaultRGB_SelectControl_Select[2][iActive];

	dlg.DoModal();

	pMain->pAppMain->m_PropertyChange.bIsBusy = FALSE;

	if (pMain->pAppMain->m_PropertyChange.bIsChanged) {
		m_iRGB_SelectControl_Outline[0][iActive] = pMain->pAppMain->m_PropertyChange.Color[0][0];
		m_iRGB_SelectControl_Outline[1][iActive] = pMain->pAppMain->m_PropertyChange.Color[1][0];
		m_iRGB_SelectControl_Outline[2][iActive] = pMain->pAppMain->m_PropertyChange.Color[2][0];

		m_iRGB_SelectControl_Blank[0][iActive] = pMain->pAppMain->m_PropertyChange.Color[0][1];
		m_iRGB_SelectControl_Blank[1][iActive] = pMain->pAppMain->m_PropertyChange.Color[1][1];
		m_iRGB_SelectControl_Blank[2][iActive] = pMain->pAppMain->m_PropertyChange.Color[2][1];

		m_iRGB_SelectControl_Select[0][iActive] = pMain->pAppMain->m_PropertyChange.Color[0][2];
		m_iRGB_SelectControl_Select[1][iActive] = pMain->pAppMain->m_PropertyChange.Color[1][2];
		m_iRGB_SelectControl_Select[2][iActive] = pMain->pAppMain->m_PropertyChange.Color[2][2];

		m_BrushSelectControl_Blank[iActive].CreateSolidBrush(RGB(m_iRGB_SelectControl_Blank[0][iActive], m_iRGB_SelectControl_Blank[1][iActive], m_iRGB_SelectControl_Blank[2][iActive]));
		m_BrushSelectControl_Select[iActive].CreateSolidBrush(RGB(m_iRGB_SelectControl_Select[0][iActive], m_iRGB_SelectControl_Select[1][iActive], m_iRGB_SelectControl_Select[2][iActive]));
		m_PenSelectControl_Blank[iActive].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Outline[0][iActive], m_iRGB_SelectControl_Outline[1][iActive], m_iRGB_SelectControl_Outline[2][iActive]));
		m_PenSelectControl_Select[iActive].CreatePen(PS_SOLID, 1, RGB(m_iRGB_SelectControl_Select[0][iActive], m_iRGB_SelectControl_Select[1][iActive], m_iRGB_SelectControl_Select[2][iActive]));

		Invalidate();
		OnBnClickedUpdatecolor();
	}
}


void CSettingDlg::ChangeDataSaveMode() {
	UDT;
	BOOL bEnablePathControl = IsDlgButtonChecked(IDC_SaveMode1);
	GetDlgItem(IDC_SavePath)->EnableWindow(bEnablePathControl);
	GetDlgItem(IDC_SetPath)->EnableWindow(bEnablePathControl);
}


void CSettingDlg::OnBnClickedSetpath() {
	char CurrentPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	CString strSavedDirectory;
	// 마지막 Access한 폴더으로 시작
	strSavedDirectory = pMain->GetRegRoot_RegistryData("Common", "SaveFolder");
	//dlg.m_ofn.lpstrInitialDir = strSavedDirectory;

	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	if (strSavedDirectory != "") {
		BrInfo.lParam = (LPARAM)(LPCTSTR)strSavedDirectory;
	}
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);				// 파일경로 읽어오기

	CString str;
	str.Format(_T("%s"), szBuffer);
	pMain->SetRegRoot_RegistryData("Common", "SaveFolder", str);
	pMain->m_strSavePath[1].Format("%s", (LPCTSTR)str);
	SetDlgItemText(IDC_SavePath, str);
	SetCurrentDirectory(CurrentPath);
}


void CSettingDlg::OnEnChangengridx() {
	UDT;
	int minVal = 0;
	int maxVal = 0;

	switch (m_iGridX_Type) {
	case 0: minVal = 1; maxVal = 1000; break;
	case 1: minVal = 1; maxVal = 100; break;
	case 2: minVal = 50; maxVal = 1000; break;
	}

	int val = GetDlgItemInt(IDC_nGridX);
	int val2 = min(maxVal, max(minVal, val));

	if (val != val2) SetDlgItemInt(IDC_nGridX, val2);
}

void CSettingDlg::OnBnClickedChangeselectitemcolor() {
	UDT;
	int iActive = 0;
	CColorSelectDlg dlg;
	dlg.pAppMain = pMain->pAppMain;
	if (pMain->pAppMain->m_PropertyChange.bIsBusy) {
        AfxMessageBox("Property수정 모드 활성중에는 사용 불가합니다. \r\n(Unavailable while Property Edit mode is active.)");
		return;
	}
	dlg.m_iMode = 2;

	switch (m_iColorItem) {
	case 0: // 0: data 분석 배경색     
	case 1: //data 수신시 배경색
		for (int i = 0; i < 3; i++) {
			pMain->pAppMain->m_PropertyChange.Color[i][0] = pMain->m_iBackgroundColor[m_iColorItem][i];
			pMain->pAppMain->m_PropertyChange.DefaultColor[i][0] = pMain->m_iDefault_BackgroundColor[m_iColorItem][i];
		}
		break;
	case 2: 
		for (int i = 0; i < 3; i++) {
			pMain->pAppMain->m_PropertyChange.Color[i][0] = pMain->m_iTextColor[i];
			pMain->pAppMain->m_PropertyChange.DefaultColor[i][0] = pMain->m_iDefault_TextColor[i];
		}
		break;
	}
			
	pMain->pAppMain->m_PropertyChange.bIsBusy = TRUE;
	pMain->pAppMain->m_PropertyChange.bIsChanged = FALSE;
	dlg.DoModal();

	pMain->pAppMain->m_PropertyChange.bIsBusy = FALSE;

	if (pMain->pAppMain->m_PropertyChange.bIsChanged) {
		m_bChangedColorItem[m_iColorItem] = TRUE;
		for (int i = 0; i < 3; i++) {
				m_iChangeColor[m_iColorItem][i] = pMain->pAppMain->m_PropertyChange.Color[i][0];
		}
		m_bChangedColorItem[m_iColorItem] = TRUE;
	}	
}


void CSettingDlg::OnBnClickedUseintegerdata() {
	int bActive = TRUE;
	if (IsDlgButtonChecked(IDC_UseIntegerData)) {
		bActive = FALSE;
	}
	int idc[MAX_DATACONTROL_SIZE] = { IDC_DataMultiplier0, IDC_DataMultiplier1, IDC_DataMultiplier2, IDC_DataMultiplier3, IDC_DataMultiplier4, IDC_DataMultiplier5, IDC_DataMultiplier6, IDC_DataMultiplier7, IDC_DataMultiplier8, IDC_DataMultiplier9, IDC_DataMultiplier10, IDC_DataMultiplier11, IDC_DataMultiplier12, IDC_DataMultiplier13, IDC_DataMultiplier14, IDC_DataMultiplier15, };
	for (int i = 0; i < MAX_DATACONTROL_SIZE; i++) {
		GetDlgItem(idc[i])->EnableWindow(bActive);
	}
}


void CSettingDlg::OnBnClickedTextcolormode0() {
	UDT;
	int idc[] = { IDC_BKColorMode2, IDC_BKColorMode3, IDC_BKColorMode4, IDC_BKColorMode5 };
	for (int i = 0; i < 4; i++) {
		GetDlgItem(idc[i])->EnableWindow(TRUE);
	}
	
	switch (m_iTextColorMode) {
	case 0: GetDlgItem(idc[0])->EnableWindow(FALSE); break;
	case 2: GetDlgItem(idc[1])->EnableWindow(FALSE); break;
	//case 4: GetDlgItem(idc[3])->EnableWindow(FALSE); break;
	}
}

void CSettingDlg::OnBnClickedBkcolormode2()
{
	UDT;
	int idc[] = { IDC_TextColorMode0, IDC_TextColorMode4, IDC_TextColorMode2, IDC_TextColorMode3, IDC_TextColorMode1 };
	for (int i = 0; i < 5; i++) {
		GetDlgItem(idc[i])->EnableWindow(TRUE);
	}

	switch (m_iBkColorMode) {
	case 0: GetDlgItem(idc[0])->EnableWindow(FALSE); break;
	case 2: GetDlgItem(idc[2])->EnableWindow(FALSE); break;
	//case 3: GetDlgItem(idc[1])->EnableWindow(FALSE); break;
	}
}


void CSettingDlg::OnBnClickedSavereg() {
	pMain->ExportRegFile();
}


void CSettingDlg::OnBnClickedInitreg() {
	if (AfxMessageBox("기존 설정 내용이 모두 삭제됩니다. 삭제후에는 정보 복원이 되지 않습니다. 계속 진행할까요?\r\n(All existing settings will be deleted. This action cannot be undone.Continue ?)", MB_YESNO) == IDYES) {
		CConfirmDlg dlg;
		dlg.pSetting = this;
		dlg.m_strJob.Format("설정 초기화를(Reset Settings) ");
		m_bConfirmInit = FALSE;
		dlg.DoModal();
		if (m_bConfirmInit) {
			AfxMessageBox("초기화를 시작합니다 (Initializing...)");
			pMain->ReqRegInit();
			AfxMessageBox("설정이 초기화 되었습니다(Settings reset completed).\r\n확인버튼을 누르면 프로그램이 종료되며 초기화 완료됩니다(Click OK to exit the program)!!");
			pMain->m_bReqCloseProgram = TRUE;
			OnBnClickedCancel();
		}
	}
	else {
		AfxMessageBox("설정 초기화를 취소하였습니다(Settings reset canceled.).");
	}
}


void CSettingDlg::OnEnChangeMaxdatadigit()
{
	int val = GetDlgItemInt(IDC_MaxDataDigit);
	CString str, str2;
	GetDlgItemText(IDC_MaxDataDigit, str);
	str2.Format("%d", max(6, min(20, val)));
	if (str != str2) {
		SetDlgItemText(IDC_MaxDataDigit, str);
	}
}


void CSettingDlg::OnBnClickedReqheaderlog2()
{
	CGraphLog dlg;
	dlg.pMain = pMain;
	dlg.DoModal();
}


void CSettingDlg::OnBnClickedNavilogconvert()
{
	CNavilog dlg;
	dlg.pMain = pMain;
	dlg.DoModal();
}

void CSettingDlg::OnBnClickedUseXinput()
{
	GetDlgItem(IDC_XInputSend)->EnableWindow(IsDlgButtonChecked(IDC_Use_Xinput));
}


void CSettingDlg::OnBnClickedXinputsend()
{
	CXInputDlg dlg;
    dlg.pMain = pMain;
	dlg.DoModal();
}

void CSettingDlg::ShowDataPage() {
    CString str;
	static int idc[MAX_DATACONTROL_SIZE] = { IDC_S_01, IDC_S_02, IDC_S_03, IDC_S_04, IDC_S_05, IDC_S_06, IDC_S_07, IDC_S_08, IDC_S_09, IDC_S_10, IDC_S_11, IDC_S_12, IDC_S_13, IDC_S_14, IDC_S_15, IDC_S_16 };
	for (int i = 0; i < MAX_DATACONTROL_SIZE; i++) {
		int iData = i + MAX_DATACONTROL_SIZE * m_iCurrentDataPage + 1;
		if (iData < 10) {
			str.Format("D%d", iData);
			SetDlgItemText(idc[i], str);
		}
		else {
			SetDlgItemInt(idc[i], iData);
		}
	}
    str.Format("%d / %d", m_iCurrentDataPage + 1, m_iDataPage);

	m_bReqRedraw = TRUE;
    SetDlgItemText(IDC_DataPage, str);
	OnBnClickedUpdatecolor();
}

void CSettingDlg::UpdateDataPage() {
	int prePage = m_iDataPage;
	if (IsDlgButtonChecked(IDC_SetDataPage)) {
		int idc[N_MAX_DATA_PAGE] = { IDC_DataSize0, IDC_DataSize1, IDC_DataSize2, IDC_DataSize3 };
		for (int i = 0; i < N_MAX_MATH_PAGE; i++) {
			if (IsDlgButtonChecked(idc[i])) {
				m_iDataPage = i + 1;
				break;
			}
		}
	}
	if (prePage != m_iDataPage) {
		int oldPage = m_iCurrentDataPage;
		m_iCurrentDataPage = max(0, min(m_iCurrentDataPage, m_iDataPage - 1));
	}
	ShowDataPage();
	OnEnChangeMindatasize();
	OnEnChangeMaxdatasize();
}

void CSettingDlg::OnBnClickedPageup()
{
	UpdateDataPage();
    if (++m_iCurrentDataPage >= m_iDataPage) {
		m_iCurrentDataPage--;
        return;
    }
	ShowDataPage();
}


void CSettingDlg::OnBnClickedPagedn()
{
	UpdateDataPage();
    if (m_iCurrentDataPage > 0) {
		m_iCurrentDataPage--;
		ShowDataPage();
    }
}


void CSettingDlg::OnBnClickedSetdatapage()
{
	int idc[] = { IDC_DataSize0, IDC_DataSize1, IDC_DataSize2, IDC_DataSize3 };
	BOOL bShow = IsDlgButtonChecked(IDC_SetDataPage);
	for (auto i : idc) {
		GetDlgItem(i)->ShowWindow(bShow);
		CheckDlgButton(i, FALSE);
	}
	CheckDlgButton(idc[pMain->m_iDataPage -1], TRUE);	
	GetDlgItem(IDC_DataSize)->ShowWindow(bShow == FALSE);	
	if (bShow == FALSE) {
		m_iDataPage = pMain->m_iDataPage;
	}
	UpdateDataPage();
}


void CSettingDlg::OnBnClickedSetmathpage()
{
	int idc[] = { IDC_MathSize0, IDC_MathSize1, IDC_MathSize2, IDC_MathSize3 };
	BOOL bShow = IsDlgButtonChecked(IDC_SetMathPage2);
	for (auto i : idc) {
		GetDlgItem(i)->ShowWindow(bShow);
		CheckDlgButton(i, FALSE);
	}
	CheckDlgButton(idc[pMain->m_iMathPage - 1], TRUE);	
	GetDlgItem(IDC_MathSize)->ShowWindow(bShow==FALSE);
	UpdateDataPage();
}


void CSettingDlg::OnBnClickedSettestdatasize()
{
	GetDlgItem(IDC_TestDataSize)->ShowWindow(IsDlgButtonChecked(IDC_SetTestDataSize));
    SetDlgItemInt(IDC_TestDataSize, m_iHistoryCapacity);
	GetDlgItem(IDC_TestDataSize_R)->ShowWindow(IsDlgButtonChecked(IDC_SetTestDataSize) == FALSE);
	SetDlgItemInt(IDC_TestDataSize_R, m_iHistoryCapacity);
}


void CSettingDlg::OnEnChangeTestdatasize()
{
	SetTimer(TIMER_CHANGE_DATASIZE, 1000, NULL);
}


void CSettingDlg::CheckTestDataSize() {
	int val;
	CString str, str1;
	GetDlgItemText(IDC_TestDataSize, str1);
	val = GetDlgItemInt(IDC_TestDataSize);
	if (val != 0) {
		val = min(MAX_TESTDATA_SIZE, max(val, MIN_TESTDATA_SIZE));
	}
	str.Format("%d", val);
	if (str != str1) {
		SetDlgItemText(IDC_TestDataSize, str);
	}
}


void CSettingDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
		case TIMER_CHANGE_DATASIZE:
			KillTimer(TIMER_CHANGE_DATASIZE);
			CheckTestDataSize();
			break;


		case TIMER_CHANGE_X:
			KillTimer(TIMER_CHANGE_X);
			UpdateXY(0);
			break;

		case TIMER_CHANGE_Y:
			KillTimer(TIMER_CHANGE_Y);
			UpdateXY(1);
			break;

		case TIMER_AUTOSAVE_TIMEOUT:
			KillTimer(TIMER_AUTOSAVE_TIMEOUT);
			UpdateAutosaveTimeout();
			break;

		case TIMER_AUTOSAVE_LOGCNT:
			KillTimer(TIMER_AUTOSAVE_LOGCNT);
			UpdateAutosaveLogCount();
			break;

		default:  break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CSettingDlg::OnEnChangePositiongraphselx()
{
	SetTimer(TIMER_CHANGE_X, 1000, NULL);
}

void CSettingDlg::OnEnChangePositiongraphsely()
{
	SetTimer(TIMER_CHANGE_Y, 1000, NULL);
}


int CSettingDlg::UpdatePositionItem(CString* pstr) {
	int n = pstr->GetLength();
	int type = -1;
	int item = 0;
	int mode = 0;
	
	for (int i = 0; i < n; i++) {
		char ch = pstr->GetAt(i);
		if (mode == 0) {
			switch (ch) {
			case 'd':
			case 'D': mode = 1; type = 0;
				break;
				
			case 'm':
			case 'M': mode = 1; type = 1;
				break;

			case ' ': break;
			default: mode = 1; 
			}
		}
		else {
			if (ch >= '0' && ch <= '9') {
				item = item * 10 + ch - '0';
			}
		}
	}
	
	CString str_new;
	if (type == -1) { pstr->Format("");  return -1; }
	
	if (type == 0) {
		if (item <1 || item > m_iDataPage * MAX_DATACONTROL_SIZE) {
			pstr->Format("");  
			return -1;
		}
		else {
			pstr->Format("D%d", item);
			return item - 1;
		}
	}
	else {
		if (item == 0 || item > m_iMathPage * MAX_MATHCONTROL_SIZE) {
			pstr->Format("");  
			return -1; 
		}
		else {
			pstr->Format("M%d", item);
			return m_iDataPage * MAX_DATACONTROL_SIZE + item - 1;
		}
	}
}


void CSettingDlg::UpdateXY(int dir) {
	int idc = IDC_PositionGraphSelX;
	if (dir == 1) idc = IDC_PositionGraphSelY;

	CString str1, str2;
	GetDlgItemText(idc, str1);
	GetDlgItemText(idc, str2);
	int val = UpdatePositionItem(&str1);

	if (dir == 1) m_iSelectPositionGraphY = val;
	else m_iSelectPositionGraphX = val;

	if (str1 != str2) {
		SetDlgItemText(idc, str1);
	}
}


void CSettingDlg::OnBnClickedConvertreg()
{
	if (AfxMessageBox("이전 설정(Ver,1)을 가져와서  Ver.2용으로 설정 파일을 저장할까요?\r\n(Import Ver. 1 settings and save as Ver. 2?)", MB_YESNO) == IDYES) {
		CConfirmDlg dlg;
		dlg.pSetting = this;
		dlg.m_strJob.Format("설정파일 생성을(Create Settings File) ");
		m_bConfirmInit = FALSE;
		dlg.DoModal();
		if (m_bConfirmInit) {			           
			pMain->ExportRegFile_after_Conversion_from_Old(0);
		}
	}
	else {
		AfxMessageBox("작업 취소하였습니다(Operation canceled).");
	}
}


void CSettingDlg::OnBnClickedConvertreg2()
{
	if (AfxMessageBox("Ver2.0 설정을 가져와서 Ver.1용으로 설정 파일을 저장할까요? \r\n(Import Ver. 2.0 settings and save as Ver. 1?)" , MB_YESNO) == IDYES) {
		CConfirmDlg dlg;
		dlg.pSetting = this;
		dlg.m_strJob.Format("설정파일 생성(Create Settings File)을 ");
		m_bConfirmInit = FALSE;
		dlg.DoModal();
		if (m_bConfirmInit) {
			pMain->ExportRegFile_after_Conversion_from_Old(1);
		}
	}
	else {
		AfxMessageBox("작업 취소하였습니다(Operation canceled).");
	}
}

void CSettingDlg::ChangeDataPage()
{
	UpdateDataPage();
}

void CSettingDlg::OnEnChangeChangeheaderRxcount()
{
    CString str1, str2;
    GetDlgItemText(IDC_ChangeHeader_RxCount, str1);
    str2.Format("%d", max(1, min(100, GetDlgItemInt(IDC_ChangeHeader_RxCount))));
    if (str1 != str2) {
        SetDlgItemText(IDC_ChangeHeader_RxCount, str2);
    }
}


void CSettingDlg::OnEnChangeMaxdatasize2()
{
    int getSize = GetDlgItemInt(IDC_MaxDataSize2);
    int iSize = min(100, max(MAX_DATACONTROL_SIZE * m_iDataPage, getSize));
    int getDelta = GetDlgItemInt(IDC_MaxDataSize);
    int iDelta = getSize - MAX_DATACONTROL_SIZE * m_iDataPage;

    if (iDelta != getDelta) {
        SetDlgItemInt(IDC_MaxDataSize, iDelta);
        m_strMaxDataSize.Format("%d", iDelta);
    }
    
    if (getSize != iSize) {
        SetDlgItemInt(IDC_MaxDataSize2, iSize);
    }
    
    CString str;
    str.Format("( %d ~ %d )", GetDlgItemInt(IDC_MinDataSize2), iSize);
    SetDlgItemText(IDC_DataSizeRange, str);
}


void CSettingDlg::OnEnChangeMindatasize2()
{
    int getSize = GetDlgItemInt(IDC_MinDataSize2);
    int iSize = max(5, min(MAX_DATACONTROL_SIZE * m_iDataPage, getSize));
    int getDelta = GetDlgItemInt(IDC_MinDataSize);
    int iDelta = MAX_DATACONTROL_SIZE * m_iDataPage - getSize;

    if (iDelta != getDelta) {
        SetDlgItemInt(IDC_MinDataSize, iDelta);
        m_strMinDataSize.Format("%d", iDelta);
    }

    if (getSize != iSize) {
        SetDlgItemInt(IDC_MinDataSize2, iSize);
    }

    CString str;
    str.Format("( %d ~ %d )", iSize, GetDlgItemInt(IDC_MaxDataSize2));
    SetDlgItemText(IDC_DataSizeRange, str);
}


void CSettingDlg::OnEnChangeAutosavetimeout() {	
	SetTimer(TIMER_AUTOSAVE_TIMEOUT, 1000, NULL);
}


void CSettingDlg::OnEnChangeAutosavelogcount() {
	SetTimer(TIMER_AUTOSAVE_LOGCNT, 1000, NULL);
}

void CSettingDlg::UpdateAutosaveTimeout() {
	int val = max(0, GetDlgItemInt(IDC_AutoSaveTimeout));
	CString str1, str2;
	GetDlgItemText(IDC_AutoSaveTimeout, str1);

	if (val > 0) {
		val = max(300, val);
	}
	str2.Format("%d", val);
	if (str1 != str2) {
		SetDlgItemText(IDC_AutoSaveTimeout, str2);
	}
}

void CSettingDlg::UpdateAutosaveLogCount() {
	int val = max(0, GetDlgItemInt(IDC_AutoSaveLogCount));
	CString str1, str2;
	GetDlgItemText(IDC_AutoSaveLogCount, str1);

	if (val > 0) {
		val = max(1000, val);
	}
	str2.Format("%d", val);
	if (str1 != str2) {
		SetDlgItemText(IDC_AutoSaveLogCount, str2);
	}
}

void CSettingDlg::OnEnChangeLogupdatecount(){
    CString s1, s2;
    int val = GetDlgItemInt(IDC_LogUpdateCount);
    GetDlgItemText(IDC_LogUpdateCount, s1);
    s2.Format("%d", max(MIN_LOG_UPDATE_PERIOD, min(MAX_LOG_UPDATE_PERIOD, val)));

    if (s1 != s2) {
        SetDlgItemText(IDC_LogUpdateCount, s2);
    }
}
