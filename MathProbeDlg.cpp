// MathProbeDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "MathProbeDlg.h"
#include "afxdialogex.h"
#include "SerialComDlg.h"


// CMathProbeDlg 대화 상자

IMPLEMENT_DYNAMIC(CMathProbeDlg, CDialogEx)

CMathProbeDlg::CMathProbeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MathProbeDlg, pParent)
	, m_strMessage(_T(""))
	, m_iMathPage(0)
{
	int iDefaultSize[MAX_OPERATOR_COUNT] = {0, 0, 1, 1, 0, 0, 0, 1, 100, 100, 100, 1};
	for (int i = 0; i < MAX_OPERATOR_COUNT; i++) {
		m_iDefaultSize[i] = iDefaultSize[i];
	}
	int IDC_PinName[MAX_MATHCONTROL_SIZE] = { IDC_PinName_M0, IDC_PinName_M1, IDC_PinName_M2, IDC_PinName_M3, IDC_PinName_M4, IDC_PinName_M5, IDC_PinName_M6, IDC_PinName_M7, IDC_PinName_M8, IDC_PinName_M9, IDC_PinName_M10, IDC_PinName_M11, IDC_PinName_M12, IDC_PinName_M13, IDC_PinName_M14, IDC_PinName_M15, };
	int IDC_UsePinName[MAX_MATHCONTROL_SIZE] = { IDC_UseMathPinName_M0, IDC_UseMathPinName_M1, IDC_UseMathPinName_M2, IDC_UseMathPinName_M3, IDC_UseMathPinName_M4, IDC_UseMathPinName_M5, IDC_UseMathPinName_M6, IDC_UseMathPinName_M7, IDC_UseMathPinName_M8, IDC_UseMathPinName_M9, IDC_UseMathPinName_M10, IDC_UseMathPinName_M11, IDC_UseMathPinName_M12, IDC_UseMathPinName_M13, IDC_UseMathPinName_M14, IDC_UseMathPinName_M15, };
    int IDC_ResInt[MAX_MATHCONTROL_SIZE] = { IDC_ResultInt0, IDC_ResultInt1, IDC_ResultInt2, IDC_ResultInt3, IDC_ResultInt4, IDC_ResultInt5, IDC_ResultInt6, IDC_ResultInt7, IDC_ResultInt8, IDC_ResultInt9, IDC_ResultInt10, IDC_ResultInt11, IDC_ResultInt12, IDC_ResultInt13, IDC_ResultInt14, IDC_ResultInt15, };
	for (int i = 0; i < MAX_MATHCONTROL_SIZE; i++) {
		m_IDC_PinName[i] = IDC_PinName[i];
		m_IDC_UsePinName[i] = IDC_UsePinName[i];
        m_IDC_ResInt[i] = IDC_ResInt[i];
    }
}

CMathProbeDlg::~CMathProbeDlg()
{
}

void CMathProbeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Data1_M0, m_ctrlData1[0]);
	DDX_Control(pDX, IDC_Data1_M1, m_ctrlData1[1]);
	DDX_Control(pDX, IDC_Data1_M2, m_ctrlData1[2]);
	DDX_Control(pDX, IDC_Data1_M3, m_ctrlData1[3]);
	DDX_Control(pDX, IDC_Data1_M4, m_ctrlData1[4]);
	DDX_Control(pDX, IDC_Data1_M5, m_ctrlData1[5]);
	DDX_Control(pDX, IDC_Data1_M6, m_ctrlData1[6]);
	DDX_Control(pDX, IDC_Data1_M7, m_ctrlData1[7]);
	DDX_Control(pDX, IDC_Data2_M0, m_ctrlData2[0]);
	DDX_Control(pDX, IDC_Data2_M1, m_ctrlData2[1]);
	DDX_Control(pDX, IDC_Data2_M2, m_ctrlData2[2]);
	DDX_Control(pDX, IDC_Data2_M3, m_ctrlData2[3]);
	DDX_Control(pDX, IDC_Data2_M4, m_ctrlData2[4]);
	DDX_Control(pDX, IDC_Data2_M5, m_ctrlData2[5]);
	DDX_Control(pDX, IDC_Data2_M6, m_ctrlData2[6]);
	DDX_Control(pDX, IDC_Data2_M7, m_ctrlData2[7]);
	DDX_Control(pDX, IDC_Operator_M0, m_ctrlOperator[0]);
	DDX_Control(pDX, IDC_Operator_M1, m_ctrlOperator[1]);
	DDX_Control(pDX, IDC_Operator_M2, m_ctrlOperator[2]);
	DDX_Control(pDX, IDC_Operator_M3, m_ctrlOperator[3]);
	DDX_Control(pDX, IDC_Operator_M4, m_ctrlOperator[4]);
	DDX_Control(pDX, IDC_Operator_M5, m_ctrlOperator[5]);
	DDX_Control(pDX, IDC_Operator_M6, m_ctrlOperator[6]);
	DDX_Control(pDX, IDC_Operator_M7, m_ctrlOperator[7]);
	DDX_Control(pDX, IDC_nDataSize_M0, m_ctrlDataSize[0]);
	DDX_Control(pDX, IDC_nDataSize_M1, m_ctrlDataSize[1]);
	DDX_Control(pDX, IDC_nDataSize_M2, m_ctrlDataSize[2]);
	DDX_Control(pDX, IDC_nDataSize_M3, m_ctrlDataSize[3]);
	DDX_Control(pDX, IDC_nDataSize_M4, m_ctrlDataSize[4]);
	DDX_Control(pDX, IDC_nDataSize_M5, m_ctrlDataSize[5]);
	DDX_Control(pDX, IDC_nDataSize_M6, m_ctrlDataSize[6]);
	DDX_Control(pDX, IDC_nDataSize_M7, m_ctrlDataSize[7]);
	DDX_Text(pDX, IDC_nDataSize_M0, m_strDataSize[0]);
	DDX_Text(pDX, IDC_nDataSize_M1, m_strDataSize[1]);
	DDX_Text(pDX, IDC_nDataSize_M2, m_strDataSize[2]);
	DDX_Text(pDX, IDC_nDataSize_M3, m_strDataSize[3]);
	DDX_Text(pDX, IDC_nDataSize_M4, m_strDataSize[4]);
	DDX_Text(pDX, IDC_nDataSize_M5, m_strDataSize[5]);
	DDX_Text(pDX, IDC_nDataSize_M6, m_strDataSize[6]);
	DDX_Text(pDX, IDC_nDataSize_M7, m_strDataSize[7]);
	DDX_Text(pDX, IDC_PinName_M0, m_strMathProbeName[0]);
	DDX_Text(pDX, IDC_PinName_M1, m_strMathProbeName[1]);
	DDX_Text(pDX, IDC_PinName_M2, m_strMathProbeName[2]);
	DDX_Text(pDX, IDC_PinName_M3, m_strMathProbeName[3]);
	DDX_Text(pDX, IDC_PinName_M4, m_strMathProbeName[4]);
	DDX_Text(pDX, IDC_PinName_M5, m_strMathProbeName[5]);
	DDX_Text(pDX, IDC_PinName_M6, m_strMathProbeName[6]);
	DDX_Text(pDX, IDC_PinName_M7, m_strMathProbeName[7]);
	DDX_Text(pDX, IDC_Message, m_strMessage);
	DDX_Check(pDX, IDC_UseMathProbe_M0, m_bUseMathProbe[0]);
	DDX_Check(pDX, IDC_UseMathProbe_M1, m_bUseMathProbe[1]);
	DDX_Check(pDX, IDC_UseMathProbe_M2, m_bUseMathProbe[2]);
	DDX_Check(pDX, IDC_UseMathProbe_M3, m_bUseMathProbe[3]);
	DDX_Check(pDX, IDC_UseMathProbe_M4, m_bUseMathProbe[4]);
	DDX_Check(pDX, IDC_UseMathProbe_M5, m_bUseMathProbe[5]);
	DDX_Check(pDX, IDC_UseMathProbe_M6, m_bUseMathProbe[6]);
	DDX_Check(pDX, IDC_UseMathProbe_M7, m_bUseMathProbe[7]);
	DDX_Check(pDX, IDC_UseMathPinName_M0, m_bUseMathProbeName[0]);
	DDX_Check(pDX, IDC_UseMathPinName_M1, m_bUseMathProbeName[1]);
	DDX_Check(pDX, IDC_UseMathPinName_M2, m_bUseMathProbeName[2]);
	DDX_Check(pDX, IDC_UseMathPinName_M3, m_bUseMathProbeName[3]);
	DDX_Check(pDX, IDC_UseMathPinName_M4, m_bUseMathProbeName[4]);
	DDX_Check(pDX, IDC_UseMathPinName_M5, m_bUseMathProbeName[5]);
	DDX_Check(pDX, IDC_UseMathPinName_M6, m_bUseMathProbeName[6]);
	DDX_Check(pDX, IDC_UseMathPinName_M7, m_bUseMathProbeName[7]);

	DDX_Control(pDX, IDC_Data1_M8, m_ctrlData1[8]);
	DDX_Control(pDX, IDC_Data2_M8, m_ctrlData2[8]);
	DDX_Control(pDX, IDC_Operator_M8, m_ctrlOperator[8]);
	DDX_Control(pDX, IDC_nDataSize_M8, m_ctrlDataSize[8]);
	DDX_Text(pDX, IDC_nDataSize_M8, m_strDataSize[8]);
	DDX_Text(pDX, IDC_PinName_M8, m_strMathProbeName[8]);
	DDX_Check(pDX, IDC_UseMathProbe_M8, m_bUseMathProbe[8]);
	DDX_Check(pDX, IDC_UseMathPinName_M8, m_bUseMathProbeName[8]);

	DDX_Control(pDX, IDC_Data1_M9, m_ctrlData1[9]);
	DDX_Control(pDX, IDC_Data2_M9, m_ctrlData2[9]);
	DDX_Control(pDX, IDC_Operator_M9, m_ctrlOperator[9]);
	DDX_Control(pDX, IDC_nDataSize_M9, m_ctrlDataSize[9]);
	DDX_Text(pDX, IDC_nDataSize_M9, m_strDataSize[9]);
	DDX_Text(pDX, IDC_PinName_M9, m_strMathProbeName[9]);
	DDX_Check(pDX, IDC_UseMathProbe_M9, m_bUseMathProbe[9]);
	DDX_Check(pDX, IDC_UseMathPinName_M9, m_bUseMathProbeName[9]);

	DDX_Control(pDX, IDC_Data1_M10, m_ctrlData1[10]);
	DDX_Control(pDX, IDC_Data2_M10, m_ctrlData2[10]);
	DDX_Control(pDX, IDC_Operator_M10, m_ctrlOperator[10]);
	DDX_Control(pDX, IDC_nDataSize_M10, m_ctrlDataSize[10]);
	DDX_Text(pDX, IDC_nDataSize_M10, m_strDataSize[10]);
	DDX_Text(pDX, IDC_PinName_M10, m_strMathProbeName[10]);
	DDX_Check(pDX, IDC_UseMathProbe_M10, m_bUseMathProbe[10]);
	DDX_Check(pDX, IDC_UseMathPinName_M10, m_bUseMathProbeName[10]);

	DDX_Control(pDX, IDC_Data1_M11, m_ctrlData1[11]);
	DDX_Control(pDX, IDC_Data2_M11, m_ctrlData2[11]);
	DDX_Control(pDX, IDC_Operator_M11, m_ctrlOperator[11]);
	DDX_Control(pDX, IDC_nDataSize_M11, m_ctrlDataSize[11]);
	DDX_Text(pDX, IDC_nDataSize_M11, m_strDataSize[11]);
	DDX_Text(pDX, IDC_PinName_M11, m_strMathProbeName[11]);
	DDX_Check(pDX, IDC_UseMathProbe_M11, m_bUseMathProbe[11]);
	DDX_Check(pDX, IDC_UseMathPinName_M11, m_bUseMathProbeName[11]);

	DDX_Control(pDX, IDC_Data1_M12, m_ctrlData1[12]);
	DDX_Control(pDX, IDC_Data2_M12, m_ctrlData2[12]);
	DDX_Control(pDX, IDC_Operator_M12, m_ctrlOperator[12]);
	DDX_Control(pDX, IDC_nDataSize_M12, m_ctrlDataSize[12]);
	DDX_Text(pDX, IDC_nDataSize_M12, m_strDataSize[12]);
	DDX_Text(pDX, IDC_PinName_M12, m_strMathProbeName[12]);
	DDX_Check(pDX, IDC_UseMathProbe_M12, m_bUseMathProbe[12]);
	DDX_Check(pDX, IDC_UseMathPinName_M12, m_bUseMathProbeName[12]);

	DDX_Control(pDX, IDC_Data1_M13, m_ctrlData1[13]);
	DDX_Control(pDX, IDC_Data2_M13, m_ctrlData2[13]);
	DDX_Control(pDX, IDC_Operator_M13, m_ctrlOperator[13]);
	DDX_Control(pDX, IDC_nDataSize_M13, m_ctrlDataSize[13]);
	DDX_Text(pDX, IDC_nDataSize_M13, m_strDataSize[13]);
	DDX_Text(pDX, IDC_PinName_M13, m_strMathProbeName[13]);
	DDX_Check(pDX, IDC_UseMathProbe_M13, m_bUseMathProbe[13]);
	DDX_Check(pDX, IDC_UseMathPinName_M13, m_bUseMathProbeName[13]);

	DDX_Control(pDX, IDC_Data1_M14, m_ctrlData1[14]);
	DDX_Control(pDX, IDC_Data2_M14, m_ctrlData2[14]);
	DDX_Control(pDX, IDC_Operator_M14, m_ctrlOperator[14]);
	DDX_Control(pDX, IDC_nDataSize_M14, m_ctrlDataSize[14]);
	DDX_Text(pDX, IDC_nDataSize_M14, m_strDataSize[14]);
	DDX_Text(pDX, IDC_PinName_M14, m_strMathProbeName[14]);
	DDX_Check(pDX, IDC_UseMathProbe_M14, m_bUseMathProbe[14]);
	DDX_Check(pDX, IDC_UseMathPinName_M14, m_bUseMathProbeName[14]);

	DDX_Control(pDX, IDC_Data1_M15, m_ctrlData1[15]);
	DDX_Control(pDX, IDC_Data2_M15, m_ctrlData2[15]);
	DDX_Control(pDX, IDC_Operator_M15, m_ctrlOperator[15]);
	DDX_Control(pDX, IDC_nDataSize_M15, m_ctrlDataSize[15]);
	DDX_Text(pDX, IDC_nDataSize_M15, m_strDataSize[15]);
	DDX_Text(pDX, IDC_PinName_M15, m_strMathProbeName[15]);
	DDX_Check(pDX, IDC_UseMathProbe_M15, m_bUseMathProbe[15]);
	DDX_Check(pDX, IDC_UseMathPinName_M15, m_bUseMathProbeName[15]);
}


BEGIN_MESSAGE_MAP(CMathProbeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMathProbeDlg::OnBnClickedOk)

	ON_CBN_SELCHANGE(IDC_Data1_M0, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M1, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M2, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M3, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M4, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M5, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M6, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data1_M7, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M0, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M1, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M2, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M3, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M4, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M5, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M6, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M7, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M0, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M1, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M2, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M3, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M4, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M5, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M6, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M7, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M0, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M1, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M2, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M3, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M4, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M5, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M6, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M7, &CMathProbeDlg::UpdateControl)

	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_EN_CHANGE(IDC_nDataSize_M0, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M1, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M2, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M3, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M4, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M5, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M6, &CMathProbeDlg::OnEnChangenDatasize)
	ON_EN_CHANGE(IDC_nDataSize_M7, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_ReLoadValue, &CMathProbeDlg::OnBnClickedReloadvalue)
	ON_BN_CLICKED(IDC_UseMathProbe_M0, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M1, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M2, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M3, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M4, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M5, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M6, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathProbe_M7, &CMathProbeDlg::UpdateControl)	
	ON_BN_CLICKED(IDC_UseMathPinName_M0, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M1, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M2, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M3, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M4, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M5, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M6, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M7, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M0, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M1, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M2, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M3, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M4, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M5, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M6, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M7, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_DefaultSetting, &CMathProbeDlg::OnBnClickedDefaultsetting)
	ON_BN_CLICKED(IDC_DefaultButton, &CMathProbeDlg::OnBnClickedDefaultbutton)
	ON_BN_CLICKED(IDC_SelectColorMath0, &CMathProbeDlg::OnBnClickedSelectcolormath0)
	ON_BN_CLICKED(IDC_SelectColorMath1, &CMathProbeDlg::OnBnClickedSelectcolormath1)
	ON_BN_CLICKED(IDC_SelectColorMath2, &CMathProbeDlg::OnBnClickedSelectcolormath2)
	ON_BN_CLICKED(IDC_SelectColorMath3, &CMathProbeDlg::OnBnClickedSelectcolormath3)
	ON_BN_CLICKED(IDC_SelectColorMath4, &CMathProbeDlg::OnBnClickedSelectcolormath4)
	ON_BN_CLICKED(IDC_SelectColorMath5, &CMathProbeDlg::OnBnClickedSelectcolormath5)
	ON_BN_CLICKED(IDC_SelectColorMath6, &CMathProbeDlg::OnBnClickedSelectcolormath6)
	ON_BN_CLICKED(IDC_SelectColorMath7, &CMathProbeDlg::OnBnClickedSelectcolormath7)

	ON_CBN_SELCHANGE(IDC_Data1_M8, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M8, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M8, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M8, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M8, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M8, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M8, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M8, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath8, &CMathProbeDlg::OnBnClickedSelectcolormath8)

	ON_CBN_SELCHANGE(IDC_Data1_M9, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M9, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M9, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M9, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M9, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M9, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M9, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M9, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath9, &CMathProbeDlg::OnBnClickedSelectcolormath9)

	ON_CBN_SELCHANGE(IDC_Data1_M10, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M10, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M10, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M10, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M10, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M10, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M10, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M10, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath10, &CMathProbeDlg::OnBnClickedSelectcolormath10)

	ON_CBN_SELCHANGE(IDC_Data1_M11, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M11, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M11, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M11, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M11, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M11, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M11, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M11, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath11, &CMathProbeDlg::OnBnClickedSelectcolormath11)

	ON_CBN_SELCHANGE(IDC_Data1_M12, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M12, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M12, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M12, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M12, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M12, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M12, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M12, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath12, &CMathProbeDlg::OnBnClickedSelectcolormath12)

	ON_CBN_SELCHANGE(IDC_Data1_M13, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M13, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M13, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M13, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M13, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M13, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M13, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M13, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath13, &CMathProbeDlg::OnBnClickedSelectcolormath13)

	ON_CBN_SELCHANGE(IDC_Data1_M14, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M14, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M14, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M14, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M14, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M14, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M14, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M14, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath14, &CMathProbeDlg::OnBnClickedSelectcolormath14)

	ON_CBN_SELCHANGE(IDC_Data1_M15, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Operator_M15, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_PinName_M15, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M15, &CMathProbeDlg::UpdateControl)
	ON_EN_CHANGE(IDC_nDataSize_M15, &CMathProbeDlg::OnEnChangenDatasize)
	ON_BN_CLICKED(IDC_UseMathProbe_M15, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_UseMathPinName_M15, &CMathProbeDlg::UpdateControl)
	ON_CBN_SELCHANGE(IDC_Data2_M15, &CMathProbeDlg::UpdateControl)
	ON_BN_CLICKED(IDC_SelectColorMath15, &CMathProbeDlg::OnBnClickedSelectcolormath15)
    ON_BN_CLICKED(IDC_PageUp, &CMathProbeDlg::OnBnClickedPageup)
    ON_BN_CLICKED(IDC_PageDn, &CMathProbeDlg::OnBnClickedPagedn)
		ON_BN_CLICKED(IDC_MathDefault, &CMathProbeDlg::OnBnClickedMathdefault)
		END_MESSAGE_MAP()


// CMathProbeDlg 메시지 처리기


void CMathProbeDlg::OnBnClickedOk() {
	UDT;
	UpdateCurrentConfig(m_iMathPage);
	UpdateMemoryData();
	pMain->UpdateMathDefine();	
	CDialogEx::OnOK();
}

void CMathProbeDlg::LoadControlData(void) {
	// Apply  Math Configure Data to GUI control
	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = iControl + m_iMathPage * MAX_MATHCONTROL_SIZE;
		m_ctrlData1[iControl].SetCurSel(m_MathData[iChannel].iDataID1);
		m_ctrlData2[iControl].SetCurSel(m_MathData[iChannel].iDataID2);
		m_ctrlOperator[iControl].SetCurSel(m_MathData[iChannel].iOperator);
		m_strDataSize[iControl].Format("%d", m_MathData[iChannel].nData);

		m_bUseMathProbe[iControl] = m_MathData[iChannel].bValid;
		if (m_MathData[iChannel].iOperator <= Operator_Exponential && m_MathData[iChannel].iDataID2 == pMain->m_iMaxDataSize + iChannel) {
			m_strDataSize[iControl].Format("%f", m_MathData[iChannel].fValue);
		}

		if (m_MathData[iChannel].iDataID1 == pMain->m_iMaxDataSize + iChannel) {
			m_strDataSize[iControl].Format("%f", m_MathData[iChannel].fValue);
		}
		m_strMathProbeName[iControl].Format("%s", (LPCTSTR)m_MathData[iChannel].strName);
		m_bUseMathProbeName[iControl] = m_MathData[iChannel].bUserName;
        CheckDlgButton(m_IDC_ResInt[iControl], m_MathData[iChannel].bResInt);
	}
	UDF;

	UpdateControl();
	DrawLine();
}

void CMathProbeDlg::LoadMemoryData() {
	for (int iChannel = 0; iChannel < pMain->m_iMaxMathSize; iChannel++) {
		CopyMathInfo(pMain->m_MathData[iChannel], &m_MathData[iChannel]);
	}
	UDF;
	LoadControlData();
}

void CMathProbeDlg::UpdateMemoryData() {
	for (int iChannel = 0; iChannel < pMain->m_iMaxMathSize; iChannel++) {
		CopyMathInfo(m_MathData[iChannel], &(pMain->m_MathData[iChannel]));
	}
	UDF;
}

void CMathProbeDlg::DrawLine() {
	UDT;
	CClientDC dc(this);
	CPen pen;
	LOGBRUSH lbr;

	lbr.lbStyle = BS_SOLID;	
	lbr.lbHatch = 0;

	int x0 = 610, l = 60, y0 = 90, dy = 55;
	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = m_iMathPage * MAX_MATHCONTROL_SIZE + iControl;
		int r1 = m_MathData[iChannel].iColor[0];
		int g1 = m_MathData[iChannel].iColor[1];
		int b1 = m_MathData[iChannel].iColor[2];
		int r = max(0, min(255, r1));
		int g = max(0, min(255, g1));
		int b = max(0, min(255, b1));
		lbr.lbColor = RGB(r, g, b);

		pen.CreatePen(PS_GEOMETRIC | m_MathData[iChannel].iPenStyle, m_MathData[iChannel].iPenThickness, &lbr, 0, 0);
		dc.SelectObject(&pen);
		if (m_MathData[iChannel].bUseMark) {
			int iSize = m_MathData[iChannel].iPenThickness / 2 + 1;
			if (iSize * 2 < m_MathData[iChannel].iPenThickness + 2) iSize++;
			dc.Rectangle(x0 + l / 2 - iSize, y0 + dy * iControl / 2 - iSize, x0 + l / 2 + iSize, y0 + dy * iControl / 2 + iSize);
		}
		dc.MoveTo(610, 90 + 55 * iControl / 2);
		dc.LineTo(670, 90 + 55 * iControl / 2);
	}
}

void CMathProbeDlg::UpdateControl() {
	UDT;   	  
	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = iControl + m_iMathPage * MAX_MATHCONTROL_SIZE;
        
		if (m_bUseMathProbe[iControl]) {
			m_ctrlData1[iControl].EnableWindow(TRUE);
			int iSelData1 = m_ctrlData1[iControl].GetCurSel();

			if (iSelData1 == iChannel + pMain->m_iMaxDataSize) {
				m_ctrlOperator[iControl].EnableWindow(FALSE);
				m_ctrlData2[iControl].EnableWindow(FALSE);
				m_ctrlDataSize[iControl].EnableWindow(TRUE);
				if (m_strDataSize[iControl] == "") m_strDataSize[iControl] = "0";
				m_ctrlData2[iControl].SetCurSel(-1);
				m_ctrlOperator[iControl].SetCurSel(-1);
			}
			else if (iSelData1 < 0) {
				m_ctrlOperator[iControl].EnableWindow(FALSE);
				m_ctrlData2[iControl].EnableWindow(FALSE);
				m_ctrlDataSize[iControl].EnableWindow(FALSE);
				m_strDataSize[iControl] = "";
			}
			else {
				m_ctrlOperator[iControl].EnableWindow(TRUE);
				int iOperator = m_ctrlOperator[iControl].GetCurSel();
				if (iOperator < 0) {
					m_ctrlData2[iControl].EnableWindow(FALSE);
					m_ctrlDataSize[iControl].EnableWindow(FALSE);
					m_strDataSize[iControl] = "";
				}				
				else if (iOperator < MAX_SIMPLE_OPERATOR) {   // Default : Need Data2
					int iSelData2 = m_ctrlData2[iControl].GetCurSel();
					m_ctrlData2[iControl].EnableWindow(TRUE);
					if (iSelData2 == pMain->m_iMaxDataSize + iChannel) { // Data2 : Constant Value
						m_ctrlDataSize[iControl].EnableWindow(TRUE);
						if (m_strDataSize[iControl] == "") {
							m_strDataSize[iControl].Format("%d", m_iDefaultSize[iOperator]);
						}
					}
					else {
						m_ctrlDataSize[iControl].EnableWindow(FALSE);  // Data2 : Channel data
						m_strDataSize[iControl] = "";
					}
				}
				else {
					m_ctrlData2[iControl].SetCurSel(-1);
					m_ctrlData2[iControl].EnableWindow(FALSE);

					if (m_strDataSize[iControl] == "") {
						m_strDataSize[iControl].Format("%d", m_iDefaultSize[iOperator]);
					}

					if (iOperator != MAX_OPERATOR_COUNT - 1)
						m_ctrlDataSize[iControl].EnableWindow(TRUE);
					else {
						m_strDataSize[iControl] = "";
						m_ctrlDataSize[iControl].EnableWindow(FALSE);
					}
				}
			}			
            (CButton *)GetDlgItem(m_IDC_ResInt[iControl])->EnableWindow(TRUE);
		}
		else {
			m_ctrlData1[iControl].SetCurSel(-1);
			m_ctrlData2[iControl].SetCurSel(-1);
			m_ctrlOperator[iControl].SetCurSel(-1);
			m_strDataSize[iControl] = "";

			m_ctrlData1[iControl].EnableWindow(FALSE);
			m_ctrlData2[iControl].EnableWindow(FALSE);
			m_ctrlOperator[iControl].EnableWindow(FALSE);
			m_ctrlDataSize[iControl].EnableWindow(FALSE);
			m_bUseMathProbeName[iControl] = FALSE;
            (CButton *)GetDlgItem(m_IDC_ResInt[iControl])->EnableWindow(FALSE);
		}
	}

	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = iControl + m_iMathPage * MAX_MATHCONTROL_SIZE;
		BOOL bValid = TRUE;
		BOOL bReqUDF = FALSE;

		CString str1 = "", str2 = "", str = "";
		int iSelData1 = m_ctrlData1[iControl].GetCurSel();
		int iSelData2 = m_ctrlData2[iControl].GetCurSel();
		int iOperator = m_ctrlOperator[iControl].GetCurSel();

		// ID1 Check
		if (iSelData1 < 0 || iSelData1 >= pMain->m_iMaxDataSize + pMain->m_iMaxMathSize - 1) bValid = FALSE;

		// Operation Check
		if (iSelData1 != iChannel + pMain->m_iMaxDataSize) {
			if (iOperator < 0 || iOperator >= MAX_OPERATOR_COUNT)	bValid = FALSE;

			if (bValid) {
				// Constanct Check

				if (iOperator < MAX_SIMPLE_OPERATOR) {
					// ID2 Check
					if (iSelData2 < 0) {
						bValid = FALSE;
					}
					else {
						if (iSelData1 < pMain->m_iMaxDataSize)
							str1.Format("D%d", iSelData1 + 1);
						else
							str1.Format("M%d", iSelData1 + 1 - pMain->m_iMaxDataSize);

						if (iSelData2 < pMain->m_iMaxDataSize)
							str2.Format("D%d", iSelData2 + 1);
						else {
							if (iSelData2 == pMain->m_iMaxDataSize + iChannel) {
								str2.Format("%s", (LPCTSTR)pMain->Double2Str(atof(m_strDataSize[iControl])));
							}
							else {
								str2.Format("M%d", iSelData2 + 1 - pMain->m_iMaxDataSize);
							}
						}

						if (!m_bUseMathProbeName[iControl] || m_strMathProbeName[iControl] == "") {
							switch (iOperator) {
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
							str.Format("[%s]", (LPCTSTR)m_strMathProbeName[iControl]);
						}
					}
				}
				else {
					// nSize
					int nSize = max(1, min(MAX_MATH_DATA, atoi(m_strDataSize[iControl])));
					if (iOperator != Operator_ABS) m_strDataSize[iControl].Format("%d", nSize);

					if (iSelData1 < pMain->m_iMaxDataSize)
						str1.Format("D%d", iSelData1 + 1);
					else
						str1.Format("M%d", iSelData1 + 1 - pMain->m_iMaxDataSize);

					switch (iOperator) {
					case Operator_Average: str.Format("[Avr %s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_RMS: str.Format("[RMS %s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_Stdev: str.Format("[Std %s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_Change: str.Format("[Δ%s, n%d]", (LPCTSTR)str1, nSize); break;
					case Operator_ABS: str.Format("[ abs(%s) ]", (LPCTSTR)str1); break;
					}
				}
			}
		}
		else {
			// Const Value
			str2.Format("%s", (LPCTSTR)pMain->Double2Str(atof(m_strDataSize[iControl])));

			if (!m_bUseMathProbeName[iControl] || m_strMathProbeName[iControl] == "") {
				str.Format("[Const. %s]", (LPCTSTR)str2);
			}
			else {
				str.Format("[%s]", (LPCTSTR)m_strMathProbeName[iControl]);
			}
		}

		if (bValid) {
			GetDlgItem(m_IDC_UsePinName[iControl])->EnableWindow(TRUE);
			if (m_bUseMathProbeName[iControl]) {
				GetDlgItem(m_IDC_PinName[iControl])->EnableWindow(TRUE);
			}
			else {
				GetDlgItem(m_IDC_PinName[iControl])->EnableWindow(FALSE);
				m_strMathProbeName[iControl].Format("%s", (LPCTSTR)str);
			}
		}
		else {
			GetDlgItem(m_IDC_UsePinName[iControl])->EnableWindow(FALSE);
			GetDlgItem(m_IDC_PinName[iControl])->EnableWindow(FALSE);
			m_bUseMathProbeName[iControl] = FALSE;
			m_strMathProbeName[iControl] = "";
		}
	}	
	UDF;
}

BOOL CMathProbeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ChangeMathConfigPage(-1,  0);
	LoadMemoryData();
    CString str;
    str.Format("%d / %d", m_iMathPage + 1, pMain->m_iMathPage);
    SetDlgItemText(IDC_PageNum, str);
	return TRUE; 
}


void CMathProbeDlg::OnPaint()
{
	CPaintDC dc(this); 
	DrawLine();
}


void CMathProbeDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_strMessage.Format("%d %d", point.x, point.y);
	UDF;
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CMathProbeDlg::OnEnChangenDatasize(){
	UDT;
	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = iControl + m_iMathPage * MAX_MATHCONTROL_SIZE;
		if (m_bUseMathProbe[iControl]) {
			if (m_ctrlOperator[iControl].GetCurSel() < MAX_SIMPLE_OPERATOR){
				if (m_ctrlData2[iControl].GetCurSel() == pMain->m_iMaxDataSize + iChannel) {
					double fValue = atof(m_strDataSize[iControl]);
					m_strDataSize[iControl].Format("%f", fValue);
				}
			}
			else {
				int iSize = max(1, min(MAX_MATH_DATA, atoi(m_strDataSize[iControl])));
				m_strDataSize[iControl].Format("%d", iSize);
			}
		}
	}
	UDF;
}

void CMathProbeDlg::OnBnClickedReloadvalue() {
	LoadMemoryData();
}

void CMathProbeDlg::OnBnClickedDefaultsetting() {
	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = iControl + m_iMathPage * MAX_MATHCONTROL_SIZE;
		m_ctrlData1[iControl].SetCurSel(-1);
		m_ctrlData2[iControl].SetCurSel(-1);
		m_ctrlOperator[iControl].SetCurSel(-1);
		m_strDataSize[iControl].Format("");
		m_bUseMathProbe[iControl] = FALSE;
		m_bUseMathProbeName[iControl] = FALSE;
		m_strMathProbeName[iControl] = "";
	}

	for (int i = 0; i < pMain->m_iMaxMathSize; i++) {
		m_MathData[i].bValid = FALSE;
		m_MathData[i].iColor[0] = pMain->m_iDefaultMathColor[i][0];
		m_MathData[i].iColor[1] = pMain->m_iDefaultMathColor[i][1];
		m_MathData[i].iColor[2] = pMain->m_iDefaultMathColor[i][2];
		m_MathData[i].iPenStyle = PS_SOLID;
		m_MathData[i].iPenThickness = 2;
		m_MathData[i].bUseMark = TRUE;
	}

	UDF;
	UpdateControl();
	Invalidate();
	DrawLine();
}

void CMathProbeDlg::OnBnClickedDefaultbutton() {

}

void CMathProbeDlg::OnBnClickedSelectcolormath0() { SelectColor(0); }
void CMathProbeDlg::OnBnClickedSelectcolormath1() { SelectColor(1); }
void CMathProbeDlg::OnBnClickedSelectcolormath2() { SelectColor(2); }
void CMathProbeDlg::OnBnClickedSelectcolormath3() { SelectColor(3); }
void CMathProbeDlg::OnBnClickedSelectcolormath4() { SelectColor(4); }
void CMathProbeDlg::OnBnClickedSelectcolormath5() { SelectColor(5); }
void CMathProbeDlg::OnBnClickedSelectcolormath6() { SelectColor(6); }
void CMathProbeDlg::OnBnClickedSelectcolormath7() { SelectColor(7); }

void CMathProbeDlg::OnBnClickedSelectcolormath8() { SelectColor(8); }
void CMathProbeDlg::OnBnClickedSelectcolormath9() { SelectColor(9); }
void CMathProbeDlg::OnBnClickedSelectcolormath10() { SelectColor(10); }
void CMathProbeDlg::OnBnClickedSelectcolormath11() { SelectColor(11); }
void CMathProbeDlg::OnBnClickedSelectcolormath12() { SelectColor(12); }
void CMathProbeDlg::OnBnClickedSelectcolormath13() { SelectColor(13); }
void CMathProbeDlg::OnBnClickedSelectcolormath14() { SelectColor(14); }
void CMathProbeDlg::OnBnClickedSelectcolormath15() { SelectColor(15); }


void CMathProbeDlg::SelectColor(int iControl) {
	int iChannel = iControl + m_iMathPage * MAX_MATHCONTROL_SIZE;
	CColorSelectDlg dlg;
	dlg.pAppMain = pAppMain;
	if (pAppMain->m_PropertyChange.bIsBusy) {
		AfxMessageBox("Property수정 모드 활성중으로 사용 불가");
		return;
	}

	pAppMain->m_PropertyChange.DefaultColor[0][0] = pMain->m_iDefaultMathColor[iChannel][0];
	pAppMain->m_PropertyChange.DefaultColor[1][0] = pMain->m_iDefaultMathColor[iChannel][1];
	pAppMain->m_PropertyChange.DefaultColor[2][0] = pMain->m_iDefaultMathColor[iChannel][2];

	pAppMain->m_PropertyChange.bIsBusy = TRUE;
	pAppMain->m_PropertyChange.bIsChanged = FALSE;
	pAppMain->m_PropertyChange.Color[0][0] = m_MathData[iChannel].iColor[0];
	pAppMain->m_PropertyChange.Color[1][0] = m_MathData[iChannel].iColor[1];
	pAppMain->m_PropertyChange.Color[2][0] = m_MathData[iChannel].iColor[2];
	pAppMain->m_PropertyChange.iLineType = m_MathData[iChannel].iPenStyle;
	pAppMain->m_PropertyChange.iThickness = m_MathData[iChannel].iPenThickness;
	pAppMain->m_PropertyChange.bUseMark = m_MathData[iChannel].bUseMark;
	dlg.DoModal();
	pAppMain->m_PropertyChange.bIsBusy = FALSE;
	if (pAppMain->m_PropertyChange.bIsChanged) {
        m_MathData[iChannel].iColor[0] = pAppMain->m_PropertyChange.Color[0][0];
        m_MathData[iChannel].iColor[1] = pAppMain->m_PropertyChange.Color[1][0];
        m_MathData[iChannel].iColor[2] = pAppMain->m_PropertyChange.Color[2][0];
        m_MathData[iChannel].iPenStyle = pAppMain->m_PropertyChange.iLineType;
        m_MathData[iChannel].iPenThickness = pAppMain->m_PropertyChange.iThickness;
        m_MathData[iChannel].bUseMark = pAppMain->m_PropertyChange.bUseMark;
		Invalidate();		
		DrawLine();
	}
}

void CMathProbeDlg::CopyMathInfo(const MathData_t& From, MathData_t* To) {
	To->iDataID1 = From.iDataID1;
	To->iDataID2 = From.iDataID2;
	To->iOperator = From.iOperator;
	To->nData = From.nData;
	To->iColor[0] = From.iColor[0];
	To->iColor[1] = From.iColor[1];
	To->iColor[2] = From.iColor[2];
	To->iPenStyle = From.iPenStyle;
	To->iPenThickness = From.iPenThickness;
	To->bValid = From.bValid;
	To->fValue = From.fValue;
	To->strName.Format("%s", (LPCTSTR)From.strName);
	To->bUserName = From.bUserName;
	To->bUseMark = From.bUseMark;
    To->bResInt = From.bResInt;
}

void CMathProbeDlg::UpdateCurrentConfig(int iMathPage) {
	for (int iControl = 0; iControl < MAX_MATHCONTROL_SIZE; iControl++) {
		int iChannel = iMathPage * MAX_MATHCONTROL_SIZE + iControl;

        m_MathData[iChannel].bValid = m_bUseMathProbe[iControl];
        m_MathData[iChannel].iDataID1 = m_ctrlData1[iControl].GetCurSel();
        m_MathData[iChannel].iDataID2 = m_ctrlData2[iControl].GetCurSel();
        m_MathData[iChannel].iOperator = m_ctrlOperator[iControl].GetCurSel();
        m_MathData[iChannel].strName.Format("%s", (LPCTSTR)m_strMathProbeName[iControl]);
        m_MathData[iChannel].nData = atoi(m_strDataSize[iControl]);
        m_MathData[iChannel].fValue = atof(m_strDataSize[iControl]);
        m_MathData[iChannel].bUserName = m_bUseMathProbeName[iControl];
        m_MathData[iChannel].bResInt = IsDlgButtonChecked(m_IDC_ResInt[iControl]);
	}
}

void CMathProbeDlg::ChangeMathConfigPage(int pre_page, int post_page) {
    static int iIDC[MAX_MATHCONTROL_SIZE] = { IDC_STATIC_M0, IDC_STATIC_M1, IDC_STATIC_M2, IDC_STATIC_M3, IDC_STATIC_M4, IDC_STATIC_M5, IDC_STATIC_M6, IDC_STATIC_M7, IDC_STATIC_M8, IDC_STATIC_M9, IDC_STATIC_M10, IDC_STATIC_M11, IDC_STATIC_M12, IDC_STATIC_M13, IDC_STATIC_M14, IDC_STATIC_M15, };
    CString str;

    if (pre_page != post_page) {
		if (pre_page == -1) pre_page = 0;
		int iOffset = post_page * MAX_MATHCONTROL_SIZE;
        str.Format("%d / %d", post_page + 1, pMain->m_iMathPage);
        SetDlgItemText(IDC_PageNum, str);

        UpdateCurrentConfig(pre_page);
        for (int i = 0; i < MAX_MATHCONTROL_SIZE; i++) {
            str.Format("Math%d", iOffset + i + 1);
            GetDlgItem(iIDC[i])->SetWindowTextA(str);
        }

        // Control List 재정의	
		for (int i = 0; i < MAX_MATHCONTROL_SIZE; i++) {
			m_ctrlData1[i].ResetContent();
			m_ctrlData2[i].ResetContent();

			// Data Channel추가
			for (int j = 0; j < pMain->m_iMaxDataSize; j++) {
				str.Format(" Data%d", j + 1);
				m_ctrlData1[i].AddString(str);
				m_ctrlData2[i].AddString(str);
			}

			// Math Channel 추가
			for (int j = 0; j < i + iOffset; j++) {
				str.Format(" M%d", j + 1);
				m_ctrlData1[i].AddString(str);
				m_ctrlData2[i].AddString(str);
			}

			// Value input 추가
			str = " Value";
			m_ctrlData1[i].AddString(str);
			m_ctrlData2[i].AddString(str);
		}
        LoadControlData();
    }
}


void CMathProbeDlg::OnBnClickedPageup()
{
    if (m_iMathPage + 1 < pMain->m_iMathPage) {
		int pre = m_iMathPage;
		ChangeMathConfigPage(pre, ++m_iMathPage);
    }
}


void CMathProbeDlg::OnBnClickedPagedn()
{
	if (m_iMathPage > 0) {
		int pre = m_iMathPage;
		ChangeMathConfigPage(pre, --m_iMathPage);
	}
}


void CMathProbeDlg::OnBnClickedMathdefault()
{
	if (AfxMessageBox("Apply Default Math Display Style for all", MB_YESNO) == IDYES)
	{
		for (int iChannel = 0; iChannel < MAXIMUM_MATH_SIZE; iChannel++) {
			m_MathData[iChannel].iColor[0] = pMain->m_iDefaultMathColor[iChannel][0];
			m_MathData[iChannel].iColor[1] = pMain->m_iDefaultMathColor[iChannel][1];
			m_MathData[iChannel].iColor[2] = pMain->m_iDefaultMathColor[iChannel][2];
			m_MathData[iChannel].iPenStyle = PS_SOLID;
			m_MathData[iChannel].iPenThickness = 2;
			m_MathData[iChannel].bUseMark = TRUE;
		}
		Invalidate();
		DrawLine();
	}
}
