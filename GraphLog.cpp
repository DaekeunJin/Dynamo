// GraphLog.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialComDlg.h"
#include "GraphLog.h"
#include "afxdialogex.h"

// CGraphLog 대화 상자

IMPLEMENT_DYNAMIC(CGraphLog, CDialogEx)

CGraphLog::CGraphLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GraphLog, pParent)
{

	


}

CGraphLog::~CGraphLog()
{
}

void CGraphLog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ItemList, m_ctrlList);
    DDX_Control(pDX, IDC_ItemType, m_ctrlItemType);
    DDX_Control(pDX, IDC_Condition, m_ctrlCond);
    DDX_Control(pDX, IDC_Digit, m_ctrlDigit);
    DDX_Control(pDX, IDC_Divider, m_ctrlDiv);
}


BEGIN_MESSAGE_MAP(CGraphLog, CDialogEx)
	ON_BN_CLICKED(IDC_ReqFileDlg, &CGraphLog::OnBnClickedReqfiledlg)
	ON_CBN_SELCHANGE(IDC_ItemType, &CGraphLog::OnCbnSelchangeItemtype)
	ON_BN_CLICKED(IDC_AddItem, &CGraphLog::OnBnClickedAdditem)
	ON_EN_CHANGE(IDC_True_R, &CGraphLog::OnEnChangeTrueR)
	ON_EN_CHANGE(IDC_True_G, &CGraphLog::OnEnChangeTrueG)
	ON_EN_CHANGE(IDC_True_B, &CGraphLog::OnEnChangeTrueB)
	ON_EN_CHANGE(IDC_False_R, &CGraphLog::OnEnChangeFalseR)
	ON_EN_CHANGE(IDC_False_G, &CGraphLog::OnEnChangeFalseG)
	ON_EN_CHANGE(IDC_False_B, &CGraphLog::OnEnChangeFalseB)
	ON_NOTIFY(NM_CLICK, IDC_ItemList, &CGraphLog::OnClickItemlist)
	ON_BN_CLICKED(IDC_Modify, &CGraphLog::OnBnClickedModify)
	ON_BN_CLICKED(IDC_Delete, &CGraphLog::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_MoveUp, &CGraphLog::OnBnClickedMoveup)
	ON_BN_CLICKED(IDC_MoveDn, &CGraphLog::OnBnClickedMovedn)
	ON_BN_CLICKED(IDC_Save, &CGraphLog::OnBnClickedSave)
	ON_BN_CLICKED(IDC_Cancel, &CGraphLog::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_HeaderLog_Header, &CGraphLog::OnEnChangeHeaderlogHeader)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SaveGraphLog, &CGraphLog::OnBnClickedSavegraphlog)
	ON_BN_CLICKED(IDC_LoadGraphLogSetting, &CGraphLog::OnBnClickedLoadgraphlogsetting)
	ON_BN_CLICKED(IDC_UseMaskValue, &CGraphLog::OnBnClickedUsemaskvalue)
	ON_BN_CLICKED(IDC_DataChange, &CGraphLog::OnBnClickedDatachange)
	ON_BN_CLICKED(IDC_SendRegionData, &CGraphLog::OnBnClickedSendregiondata)
	ON_BN_CLICKED(IDC_GetRegionData, &CGraphLog::OnBnClickedGetregiondata)
	ON_BN_CLICKED(IDC_SendColorData1, &CGraphLog::OnBnClickedSendcolordata1)
	ON_BN_CLICKED(IDC_GetColorData1, &CGraphLog::OnBnClickedGetcolordata1)
	ON_BN_CLICKED(IDC_SendColorData2, &CGraphLog::OnBnClickedSendcolordata2)
	ON_BN_CLICKED(IDC_GetColorData2, &CGraphLog::OnBnClickedGetcolordata2)
	ON_BN_CLICKED(IDC_LoadGraphLogSetting2, &CGraphLog::OnBnClickedLoadgraphlogsetting2)
	ON_EN_CHANGE(IDC_Data2, &CGraphLog::OnEnChangeData2)
	ON_EN_CHANGE(IDC_Data1, &CGraphLog::OnEnChangeData1)
END_MESSAGE_MAP()


// CGraphLog 메시지 처리기


void CGraphLog::OnBnClickedReqfiledlg()
{
	CString dirdir;
	CString fileName;
	CString fileDir;

	CFileDialog        fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, "모든파일(*.jpg)||"); // 2

	if (IDOK == fileDlg.DoModal())
	{
		fileDir = fileDlg.GetPathName();       // 경로 받기
		SetDlgItemText(IDC_ImageFilename, fileDir);
	}
}

void CGraphLog::AddColumn() {
	LV_COLUMN lvcolumn;
	char *listColumn[HEADER_LOG_COLUMN_SIZE] = { "No.", "Use","Item TYPE", "Data1","Cond", "Data2", "Show Digit", "Divide", "Pre Text", "Post Text", "x", "y", "dx", "dy", "RGB1", "RGB2","Use Mask", "Masking Value" ,"MEMO" };
	int width[HEADER_LOG_COLUMN_SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };
	int i = 0;

	for (i = 0; i < HEADER_LOG_COLUMN_SIZE; i++) {
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.pszText = listColumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		m_ctrlList.InsertColumn(i, &lvcolumn);
	}

	m_ctrlList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (i = 0; i < m_ctrlList.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_ctrlList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}

BOOL CGraphLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AddColumn();
	m_ctrlItemType.SetCurSel(0);
    
	OnCbnSelchangeItemtype();
	m_ctrlCond.SetCurSel(0);
    m_ctrlDiv.SetCurSel(0);
	CString str;
	str.Format("%c", pMain->m_cPacketHead);
	SetDlgItemText(IDC_HeaderLog_Header, str);
	SetDlgItemText(IDC_ImageFilename, pMain->m_strGraphLog_Jpg);
	CheckDlgButton(IDC_UseItem, TRUE);
	LoadData();
    
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGraphLog::OnCbnSelchangeItemtype() {
	int iType = m_ctrlItemType.GetCurSel();
	CString strBlank = "";
	CString str;

	switch (iType) {
	case 0:  // Set Range for True or False
		GetDlgItem(IDC_ShowSysHint)->ShowWindow(FALSE);
		// static 
		((CButton*)GetDlgItem(IDC_STATIC1))->SetWindowTextA(" Data1     Cond     Data2");
		((CButton*)GetDlgItem(IDC_STATIC3))->SetWindowTextA("");
		((CButton*)GetDlgItem(IDC_STATIC4))->SetWindowTextA("True Color  Red  Green   Blue");
		((CButton*)GetDlgItem(IDC_STATIC5))->SetWindowTextA("False Color Red  Green   Blue");

		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		// control
		GetDlgItem(IDC_Data1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Condition)->ShowWindow(TRUE);
		GetDlgItem(IDC_Data2)->ShowWindow(TRUE);
        		
		GetDlgItem(IDC_Digit)->ShowWindow(FALSE);
        GetDlgItem(IDC_Divider)->ShowWindow(FALSE);
        m_ctrlDigit.SetCurSel(0);
        m_ctrlDiv.SetCurSel(0);

		GetDlgItem(IDC_AddPreText)->ShowWindow(FALSE);
		GetDlgItem(IDC_AddPostText)->ShowWindow(FALSE);
		SetDlgItemText(IDC_AddPreText, "");
		SetDlgItemText(IDC_AddPostText, "");
        GetDlgItem(IDC_UseMaskValue)->ShowWindow(TRUE);        
		break;

	case 1:  // Text for TRUE
		GetDlgItem(IDC_ShowSysHint)->ShowWindow(FALSE);
		// static 
		((CButton*)GetDlgItem(IDC_STATIC1))->SetWindowTextA(" Data1     Cond     Data2");
		((CButton*)GetDlgItem(IDC_STATIC3))->SetWindowTextA("Add Text");
		((CButton*)GetDlgItem(IDC_STATIC4))->SetWindowTextA("BK Color  Red  Green   Blue");
		((CButton*)GetDlgItem(IDC_STATIC5))->SetWindowTextA("TextColor Red  Green   Blue");

		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		// control
		GetDlgItem(IDC_Data1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Condition)->ShowWindow(TRUE);
		GetDlgItem(IDC_Data2)->ShowWindow(TRUE);

		m_ctrlDigit.SetCurSel(0);
		GetDlgItem(IDC_Digit)->ShowWindow(FALSE);
        GetDlgItem(IDC_Divider)->ShowWindow(FALSE);
        m_ctrlDigit.SetCurSel(0);
        m_ctrlDiv.SetCurSel(0);

		GetDlgItem(IDC_AddPreText)->ShowWindow(TRUE);
		GetDlgItem(IDC_AddPostText)->ShowWindow(FALSE);
		SetDlgItemText(IDC_AddPostText, "");
        GetDlgItem(IDC_UseMaskValue)->ShowWindow(TRUE);
		break;
		
	case 2:  // show value if True
		GetDlgItem(IDC_ShowSysHint)->ShowWindow(FALSE);
		// static 
		((CButton*)GetDlgItem(IDC_STATIC1))->SetWindowTextA(" Data1     Cond     Data2");
		((CButton*)GetDlgItem(IDC_STATIC2))->SetWindowTextA("Show Data Digit/Divider");
		((CButton*)GetDlgItem(IDC_STATIC3))->SetWindowTextA("   Pre / Post  Text");
		((CButton*)GetDlgItem(IDC_STATIC4))->SetWindowTextA("BK Color  Red  Green   Blue");
		((CButton*)GetDlgItem(IDC_STATIC5))->SetWindowTextA("TextColor Red  Green   Blue");

		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		// control
		GetDlgItem(IDC_Data1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Condition)->ShowWindow(TRUE);
		GetDlgItem(IDC_Data2)->ShowWindow(TRUE);
		GetDlgItem(IDC_Digit)->ShowWindow(TRUE);
        GetDlgItem(IDC_Divider)->ShowWindow(TRUE);

		GetDlgItem(IDC_AddPreText)->ShowWindow(TRUE);
		GetDlgItem(IDC_AddPostText)->ShowWindow(TRUE);
        GetDlgItem(IDC_UseMaskValue)->ShowWindow(TRUE);
		break;

	case 3: // Show Text
		GetDlgItem(IDC_ShowSysHint)->ShowWindow(FALSE);
		// static 
		((CButton*)GetDlgItem(IDC_STATIC1))->SetWindowTextA("");
		((CButton*)GetDlgItem(IDC_STATIC3))->SetWindowTextA("Add Text");
		((CButton*)GetDlgItem(IDC_STATIC4))->SetWindowTextA("BK Color  Red  Green   Blue");
		((CButton*)GetDlgItem(IDC_STATIC5))->SetWindowTextA("TextColor Red  Green   Blue");

		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		// control
		GetDlgItem(IDC_Data1)->ShowWindow(FALSE);
		GetDlgItem(IDC_Condition)->ShowWindow(FALSE);
		GetDlgItem(IDC_Data2)->ShowWindow(FALSE);
		SetDlgItemText(IDC_Data1, "");
		SetDlgItemText(IDC_Data2, "");
		m_ctrlCond.SetCurSel(0);
        		
		GetDlgItem(IDC_Digit)->ShowWindow(FALSE);
        GetDlgItem(IDC_Divider)->ShowWindow(FALSE);
        m_ctrlDigit.SetCurSel(0);
        m_ctrlDiv.SetCurSel(0);

		GetDlgItem(IDC_AddPreText)->ShowWindow(TRUE);
		GetDlgItem(IDC_AddPostText)->ShowWindow(FALSE);
		SetDlgItemText(IDC_AddPostText, "");
		CheckDlgButton(IDC_UseMaskValue, FALSE);
        GetDlgItem(IDC_UseMaskValue)->ShowWindow(FALSE);
		InitMaskSetting();
		break;

	case 4: //Show Value
		GetDlgItem(IDC_ShowSysHint)->ShowWindow(FALSE);
		// static 
		((CButton*)GetDlgItem(IDC_STATIC1))->SetWindowTextA("Data ID");
		((CButton*)GetDlgItem(IDC_STATIC2))->SetWindowTextA("Show Data Digit/Divider");
		((CButton*)GetDlgItem(IDC_STATIC3))->SetWindowTextA("   Pre / Post  Text");
		((CButton*)GetDlgItem(IDC_STATIC4))->SetWindowTextA("BK Color  Red  Green   Blue");
		((CButton*)GetDlgItem(IDC_STATIC5))->SetWindowTextA("TextColor Red  Green   Blue");

		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		// control
		GetDlgItem(IDC_Data1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Condition)->ShowWindow(FALSE);
		GetDlgItem(IDC_Data2)->ShowWindow(FALSE);
		GetDlgItem(IDC_Digit)->ShowWindow(TRUE);
        GetDlgItem(IDC_Divider)->ShowWindow(TRUE);
		SetDlgItemText(IDC_Data2, "");
		m_ctrlCond.SetCurSel(0);

		GetDlgItem(IDC_AddPreText)->ShowWindow(TRUE);
		GetDlgItem(IDC_AddPostText)->ShowWindow(TRUE);
		CheckDlgButton(IDC_UseMaskValue, FALSE);
        GetDlgItem(IDC_UseMaskValue)->ShowWindow(FALSE);
		InitMaskSetting();
		break;

	case 5: // Show System Value
	// static 
		((CButton*)GetDlgItem(IDC_STATIC1))->SetWindowTextA("");
		((CButton*)GetDlgItem(IDC_STATIC2))->SetWindowTextA("Sys.Item");
		((CButton*)GetDlgItem(IDC_STATIC3))->SetWindowTextA("   Pre / Post  Text");
		((CButton*)GetDlgItem(IDC_STATIC4))->SetWindowTextA("BK Color  Red  Green   Blue");
		((CButton*)GetDlgItem(IDC_STATIC5))->SetWindowTextA("TextColor Red  Green   Blue");

		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		// control
		GetDlgItem(IDC_Data1)->ShowWindow(FALSE);
		GetDlgItem(IDC_Condition)->ShowWindow(FALSE);
		GetDlgItem(IDC_Data2)->ShowWindow(FALSE);
		SetDlgItemText(IDC_Data1, "");
		SetDlgItemText(IDC_Data2, "");
		m_ctrlCond.SetCurSel(0);
        		
		GetDlgItem(IDC_Digit)->ShowWindow(TRUE);
        GetDlgItem(IDC_Divider)->ShowWindow(FALSE);        
        m_ctrlDiv.SetCurSel(0);

		GetDlgItem(IDC_AddPreText)->ShowWindow(TRUE);
		GetDlgItem(IDC_AddPostText)->ShowWindow(TRUE);
		CheckDlgButton(IDC_UseMaskValue, FALSE);
        GetDlgItem(IDC_UseMaskValue)->ShowWindow(FALSE);
		InitMaskSetting();

		str.Format("[System Item]\r\n 0: Recieved Time\r\n 1: Ellapsed Time \r\n 2: Index");
		SetDlgItemText(IDC_ShowSysHint, str);
		GetDlgItem(IDC_ShowSysHint)->ShowWindow(TRUE);
		break;
	}
    OnBnClickedUsemaskvalue();
}

CGraphLogSetting CGraphLog::CheckData(CString* pstr) {
	CStringA str;
	CString str2;
	str.Format("%s", (LPCTSTR)*pstr);

	CGraphLogSetting ret;
	int len = str.GetLength();
	int type = -1;
	double fVal = -1;
	int iVal = -1;
	int seq = 0;
	BOOL get_val = FALSE;
	BOOL bConti = TRUE;

	bool bPeriod = false;
	bool bMinus = false;
	double fOrder = 1.;

	for (int i = 0; i < len && bConti; i++) {
		BYTE aByte = str.GetAt(i);

		switch (seq) {
		case 0:
			if (aByte == 'D' || aByte == 'd') {
				type = 1;
				seq = 2;
				iVal = 0;
			}
			else if (aByte == 'M' || aByte == 'm') {
				type = 2;
				seq = 2;
				iVal = 0;
			}
			else if (aByte >= '0' && aByte <= '9') {
				type = 0;
				seq = 1;
				fVal = aByte - '0';
				get_val = TRUE;
			}
			else if (aByte == '+') {
				type = 0;
				seq = 1;
				fVal = 0.;
			}
			else if (aByte == '.') {
				type = 0;
				seq = 1;
				fVal = 0.;
				bPeriod = true;
			}
			else if (aByte == '-') {
				type = 0;
				seq = 1;
				fVal = 0.;
				bMinus = true;
			}
			break;

		case 1:
			if (aByte >= '0' && aByte <= '9') {
				get_val = TRUE;
				break;
			}
			else if (aByte == '.' && bPeriod == false) {
				bPeriod = true;
			}
			else  if (get_val) {
				bConti = FALSE;
			}
			break;

		case 2:
			if (aByte >= '0' && aByte <= '9') {
				get_val = TRUE;
				iVal = iVal * 10 + aByte - '0';
			}
			else if (get_val) {
				bConti = FALSE;
			}
			break;

		default:
			bConti = FALSE;
			break;
		}
	}

	if (get_val) {
		if (type == 1) {// && iVal >= 1 && iVal <= pMain->m_iMaxDataSize) {
			if (iVal < 1) { iVal = 1;  ret.bUpdateStr = true; }
			if (iVal > pMain->m_iMaxDataSize) { iVal = pMain->m_iMaxDataSize;  ret.bUpdateStr = true; }
			ret.bAvailable = TRUE;
		}
		else if (type == 2) { // && iVal >= 1 && iVal <= pMain->m_iMaxMathSize) {
			if (iVal < 1) { iVal = 1;  ret.bUpdateStr = true; }
			if (iVal > pMain->m_iMaxMathSize) { iVal = pMain->m_iMaxMathSize;  ret.bUpdateStr = true; }
			ret.bAvailable = TRUE;
		}
		else if (type == 0) {
			if (pMain->CheckNumStr(&str)) {
				ret.bUpdateStr = true;
			}
			ret.bAvailable = TRUE;
		}
		else {
			ret.bAvailable = FALSE;
		}
	}
	else {
		ret.bAvailable = FALSE;
	}

	if (ret.bAvailable) {
		ret.data1_type = type;
		switch (type) {
		case 1:
			ret.iData1_val = iVal;
			str2.Format("D%d", iVal);
			if (str2 != *pstr) {
				pstr->Format("D%d", iVal);
				ret.bUpdateStr = true;

			}
			break;

		case 2:
			ret.iData1_val = iVal;
			str2.Format("M%d", iVal);
			if (str2 != *pstr) {
				pstr->Format("M%d", iVal);
				ret.bUpdateStr = true;
			}
			break;

		case 0:
			if (pMain->CheckNumStr(&str)) {
				pstr->Format("%s", (LPCTSTR)str);
				ret.bUpdateStr = true;
			}
            ret.fData1_val = atof(str);
			break;
		}
	}
	return ret;
}


BOOL CGraphLog::UpdateData_from_Control() {
	CString str;
	int iType = m_ctrlItemType.GetCurSel();
	
	// Data1
	GetDlgItemText(IDC_Data1, str); 
	if (iType != 3 && iType != 5) {
		CGraphLogSetting temp = CheckData(&str);
		if (temp.bAvailable) {
			m_Item.data1_type = temp.data1_type;
			if (m_Item.data1_type == 0) {
				m_Item.fData1_val = temp.fData1_val;
			}
			else {
				m_Item.iData1_val = temp.iData1_val;
			}
            if (temp.bUpdateStr) {
                SetDlgItemText(IDC_Data1, str);
            }
		}
		else {
			return FALSE;
		}
	}
	else {
		m_Item.data1_type = 0;
		m_Item.iData1_val = 0;
		m_Item.fData1_val = 0.;
	}
	m_Item.use = IsDlgButtonChecked(IDC_UseItem);	
	m_Item.item_type = iType;

	//Data2
	if (iType < 3) {
		GetDlgItemText(IDC_Data2, str);
		CGraphLogSetting temp = CheckData(&str);

		if (temp.bAvailable) {
			m_Item.data2_type = temp.data1_type;
			if (m_Item.data2_type == 0) {
				m_Item.fData2_val = temp.fData1_val;
			}
			else {
				m_Item.iData2_val = temp.iData1_val;
			}
            if (temp.bUpdateStr) {
                SetDlgItemText(IDC_Data2, str);
            }
		}
		else {
			return FALSE;
		}
	}

	// Cond
	m_Item.cond = m_ctrlCond.GetCurSel();
	
	// Range
	m_Item.range[0] = GetDlgItemInt(IDC_X);
	m_Item.range[1] = GetDlgItemInt(IDC_Y);
	m_Item.range[2] = GetDlgItemInt(IDC_DX);
	m_Item.range[3] = GetDlgItemInt(IDC_DY);

	// Color
	m_Item.color1 = RGB(GetDlgItemInt(IDC_True_R), GetDlgItemInt(IDC_True_G), GetDlgItemInt(IDC_True_B));
	m_Item.color2 = RGB(GetDlgItemInt(IDC_False_R), GetDlgItemInt(IDC_False_G), GetDlgItemInt(IDC_False_B));

	// Mask
	if (IsDlgButtonChecked(IDC_UseMaskValue) && iType < 3) {
		int val = 0;
		for (int i = 0; i < 16; i++) {
			if (m_bMask[i]) {
				val += (0x01 << i);
			}
		}
		if (val == 0) {
			AfxMessageBox("설정된 Masking bit가 없습니다.");
			return FALSE;
		}
		m_Item.mask_val = val;
		m_Item.use_mask = IsDlgButtonChecked(IDC_UseMaskValue);		
	}
	else {
		m_Item.mask_val = 0;
		m_Item.use_mask = FALSE;
	}

	// digit
	m_Item.digit = m_ctrlDigit.GetCurSel();
    m_Item.div = m_ctrlDiv.GetCurSel();
	
	return TRUE;
}


void CGraphLog::UpdateData_from_ListTable(int iSelect) {
	CString str;
	CGraphLogSetting temp;

	// Use
	str = m_ctrlList.GetItemText(iSelect, 1);
	if (str == "Use") {
		m_Item.use = TRUE;
	}
	else {
		m_Item.use = FALSE;
	}
	CheckDlgButton(IDC_UseItem, m_Item.use);

	// Item TYPE
	str = m_ctrlList.GetItemText(iSelect, 2);
		
	if (str == "setRange for condition") {
		m_Item.item_type = 0;
	}
	else if (str == "SetText if True") {
		m_Item.item_type = 1;
	}
	else if (str == "Show Value if True") {
		m_Item.item_type = 2;
	}
	else if (str == "Show Text") {
		m_Item.item_type = 3;
	}
	else if (str == "Show Value") {
		m_Item.item_type = 4;
	}
	else if (str = "Show System") {
		m_Item.item_type = 5;
	}
	else {
		AfxMessageBox("Item type Error: " + str);
		return;
	}
	m_ctrlItemType.SetCurSel(m_Item.item_type);
	OnCbnSelchangeItemtype();

	// Data1
	str = m_ctrlList.GetItemText(iSelect, 3);
	SetDlgItemText(IDC_Data1, str);
	temp = CheckData(&str);
	m_Item.data1_type = temp.data1_type;
	if (m_Item.data1_type == 0)
		m_Item.fData1_val = temp.fData1_val;
	else 
		m_Item.iData1_val = temp.iData1_val;

	if (m_Item.item_type <= 2) {
		// Cond
		str = m_ctrlList.GetItemText(iSelect, 4);
		if (str == ">") {
			m_Item.cond = 0;
		}
		else if (str == ">=") {
			m_Item.cond = 1;
		}
		else if (str == "==") {
			m_Item.cond = 2;
		}
		else if (str == "!=") {
			m_Item.cond = 3;
		}
		else if (str == "<") {
			m_Item.cond = 4;
		}
		else if (str == "<=") {
			m_Item.cond = 5;
		}
		m_ctrlCond.SetCurSel(m_Item.cond);

		// Data2
		str = m_ctrlList.GetItemText(iSelect, 5);
		SetDlgItemText(IDC_Data2, str);
		temp = CheckData(&str);
		m_Item.data2_type = temp.data1_type;
		if (m_Item.data2_type == 0)
			m_Item.fData2_val = temp.fData1_val;
		else
			m_Item.iData2_val = temp.iData1_val;
	}

	// digit 
	str = m_ctrlList.GetItemText(iSelect, 6);
	m_Item.digit = atoi(str);
    m_ctrlDigit.SetCurSel(m_Item.digit);

    str = m_ctrlList.GetItemText(iSelect, 7);
    m_Item.div = atoi(str);
    m_ctrlDiv.SetCurSel(m_Item.div);

	// Add Text
	SetDlgItemText(IDC_AddPreText, m_ctrlList.GetItemText(iSelect, 8));
	SetDlgItemText(IDC_AddPostText, m_ctrlList.GetItemText(iSelect, 9));

	// Range
	str = m_ctrlList.GetItemText(iSelect, 10); m_Item.range[0] = atoi(str);  SetDlgItemText(IDC_X, str);
	str = m_ctrlList.GetItemText(iSelect, 11); m_Item.range[1] = atoi(str);  SetDlgItemText(IDC_Y, str);
	str = m_ctrlList.GetItemText(iSelect, 12); m_Item.range[2] = atoi(str);  SetDlgItemText(IDC_DX, str);
	str = m_ctrlList.GetItemText(iSelect, 13); m_Item.range[3] = atoi(str);  SetDlgItemText(IDC_DY, str);

	// Color1
	str = m_ctrlList.GetItemText(iSelect, 14);
	COLORREF color = (COLORREF)atoi(str);
	m_Item.color1 = color;
	SetDlgItemInt(IDC_True_R, GetRValue(color));
	SetDlgItemInt(IDC_True_G, GetGValue(color));
	SetDlgItemInt(IDC_True_B, GetBValue(color));

	// Color2
	str = m_ctrlList.GetItemText(iSelect, 15);
	color = (COLORREF)atoi(str);
	m_Item.color2 = color;
	SetDlgItemInt(IDC_False_R, GetRValue(color));
	SetDlgItemInt(IDC_False_G, GetGValue(color));
	SetDlgItemInt(IDC_False_B, GetBValue(color));
	
	// Use Mask
	str = m_ctrlList.GetItemText(iSelect, 16); m_Item.use_mask = atoi(str);  CheckDlgButton(IDC_UseMaskValue, atoi(str));
	
	// Mask value
	str = m_ctrlList.GetItemText(iSelect, 17); m_Item.mask_val = atoi(str); 
	UpdateMaskValue(m_Item.mask_val);	
	SetDlgItemText(IDC_Memo, m_ctrlList.GetItemText(iSelect, 18));

	for (int i = 0; i < m_ctrlList.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_ctrlList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}


void CGraphLog::UpdateMaskValue(int mask_val) {	
	for (int i = 0; i < 16; i++) {
		if (mask_val & (0x01 << i)) {
			m_bMask[i] = 1;
		}
		else {
			m_bMask[i] = 0;
		}
	}
}

void CGraphLog::UpdateListTbl_from_Data(int iPos) {	
	CString str;
	// use
	if (m_Item.use) {
		str = "Use";
	}
	else {
		str = "Not";
	}
	m_ctrlList.SetItemText(iPos, 1, str);

	// Item TYPE
	switch (m_Item.item_type) {
	case 0: str = "setRange for condition"; break;
	case 1: str = "SetText if True"; break;
	case 2: str = "Show Value if True"; break;
	case 3: str = "Show Text"; break;
	case 4: str = "Show Value"; break;
	case 5: str = "Show System"; break;
	default: str = "ERROR"; break;
	}
	m_ctrlList.SetItemText(iPos, 2, str);

	// Data1
	if (m_Item.item_type == 3 || m_Item.item_type == 5) {
		str = "-";
	}
	else {
		switch (m_Item.data1_type) {
		case 0:	str.Format("%s", (LPCTSTR)pMain->Double2Str(m_Item.fData1_val)); break;
		case 1: str.Format("D%d", m_Item.iData1_val); break;
		case 2: str.Format("M%d", m_Item.iData1_val); break;
		default: str = "ERROR"; break;
		}
	}
	m_ctrlList.SetItemText(iPos, 3, str);

	// Condition
	if (m_Item.item_type >= 3) {
		str = "-";
	}
	else {
		switch (m_Item.cond) {
		case 0: str = ">"; break;
		case 1: str = ">="; break;
		case 2: str = "=="; break;
		case 3: str = "!="; break;
		case 4: str = "<"; break;
		case 5: str = "<="; break;
		default: str = "ERROR"; break;
		}
	}
	m_ctrlList.SetItemText(iPos, 4, str);

	// Data2
	if (m_Item.item_type >= 3) {
		str = "-";
	}
	else {
		switch (m_Item.data2_type) {
		case 0:	str.Format("%s", (LPCTSTR)pMain->Double2Str(m_Item.fData2_val)); break;
		case 1: str.Format("D%d", m_Item.iData2_val); break;
		case 2: str.Format("M%d", m_Item.iData2_val); break;
		default: str = "ERROR"; break;
		}
	}
	m_ctrlList.SetItemText(iPos, 5, str);

	// Digit 
	if (m_Item.item_type == 2 || m_Item.item_type == 4 || m_Item.item_type == 5) {
		str.Format("%d", m_Item.digit);
		m_ctrlDigit.SetCurSel(m_Item.digit);
	}
	else {
		str = "-";
	}
	m_ctrlList.SetItemText(iPos, 6, str);

    // Divide
    if (m_Item.item_type == 2 || m_Item.item_type == 4) {
        str.Format("%d", m_Item.div);
        m_ctrlDigit.SetCurSel(m_Item.div);
    }
    else {
        str = "-";
    }
    m_ctrlList.SetItemText(iPos, 7, str);

	
	// Range
	for (int i = 0; i < 4; i++) {
		str.Format("%d", m_Item.range[i]);
		m_ctrlList.SetItemText(iPos, 10 + i, str);
	}

	// Color1
	str.Format("%d", m_Item.color1);
	m_ctrlList.SetItemText(iPos, 14, str);

	// Color2
	str.Format("%d", m_Item.color2);
	m_ctrlList.SetItemText(iPos, 15, str);

	// Use Mask
	str.Format("%d", m_Item.use_mask);
	m_ctrlList.SetItemText(iPos, 16, str);
	
	// Mask Value
	str.Format("%d", m_Item.mask_val);
	m_ctrlList.SetItemText(iPos, 17, str);

	for (int i = 0; i < m_ctrlList.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_ctrlList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}


void CGraphLog::OnBnClickedAdditem()
{
	if (UpdateData_from_Control()) {
		if (m_iGraphLogCount >= N_MAX_GRAPHLOG_ITEM) {
			AfxMessageBox("Graph Log Item의 갯수가 설정 한계에 도달하여 추가할 수 없습니다.");
			return;
		}
		
		LV_ITEM lvitem;
		CString str;

		int nCount = m_ctrlList.GetItemCount();
		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;
		lvitem.iItem = nCount;
		str.Format("%d", nCount);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_ctrlList.InsertItem(&lvitem);

		int add_pos = nCount;
		if (m_iSelectItem != -1 && IsDlgButtonChecked(IDC_Insert)) {
			// nCount -1 ~ iSelectItem
			for (int i = nCount - 1; i >= m_iSelectItem; i--) {
				for (int j = 1; j < HEADER_LOG_COLUMN_SIZE - 1; j++) {
					m_ctrlList.SetItemText(i+1, j, m_ctrlList.GetItemText(i, j));
				}
			}
			add_pos = m_iSelectItem;
		}
		GetDlgItemText(IDC_AddPreText, str);
		m_ctrlList.SetItemText(add_pos, 8, str);

		GetDlgItemText(IDC_AddPostText, str);
		m_ctrlList.SetItemText(add_pos, 9, str);

		GetDlgItemText(IDC_Memo, str);
		m_ctrlList.SetItemText(add_pos, 18, str);

		UpdateListTbl_from_Data(add_pos);

		m_iGraphLogCount++;
		m_iSelectItem = -1;
		SetDlgItemInt(IDC_SelectItem, m_iSelectItem);
		
		for (int i = 0; i < m_ctrlList.GetHeaderCtrl()->GetItemCount(); i++) {
			VERIFY(m_ctrlList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
		}
        ClearControl();
	}
	else {
		AfxMessageBox("입력값에 문제가 있습니다.");
	}
}


void CGraphLog::OnEnChangeTrueR() {
	int val = GetDlgItemInt(IDC_True_R);
	CString str1, str2;
	GetDlgItemText(IDC_True_R, str1);
	str2.Format("%d", max(0, min(255, val)));
	if (str1 != str2) {
		SetDlgItemText(IDC_True_R, str2);
	}
	DrawColor(0);
}


void CGraphLog::OnEnChangeTrueG() {
	int val = GetDlgItemInt(IDC_True_G);
	CString str1, str2;
	GetDlgItemText(IDC_True_G, str1);
	str2.Format("%d", max(0, min(255, val)));
	if (str1 != str2) {
		SetDlgItemText(IDC_True_G, str2);
	}
	DrawColor(0);
}



void CGraphLog::OnEnChangeTrueB() {
	int val = GetDlgItemInt(IDC_True_B);
	CString str1, str2;
	GetDlgItemText(IDC_True_B, str1);
	str2.Format("%d", max(0, min(255, val)));
	if (str1 != str2) {
		SetDlgItemText(IDC_True_B, str2);
	}
	DrawColor(0);
}


void CGraphLog::OnEnChangeFalseR() {
	int val = GetDlgItemInt(IDC_False_R);
	CString str1, str2;
	GetDlgItemText(IDC_False_R, str1);
	str2.Format("%d", max(0, min(255, val)));
	if (str1 != str2) {
		SetDlgItemText(IDC_False_R, str2);
	}
	DrawColor(1);
}

void CGraphLog::OnEnChangeFalseG() {
	int val = GetDlgItemInt(IDC_False_G);
	CString str1, str2;
	GetDlgItemText(IDC_False_G, str1);
	str2.Format("%d", max(0, min(255, val)));
	if (str1 != str2) {
		SetDlgItemText(IDC_False_G, str2);
	}
	DrawColor(1);
}

void CGraphLog::OnEnChangeFalseB() {
	int val = GetDlgItemInt(IDC_False_B);
	CString str1, str2;
	GetDlgItemText(IDC_False_B, str1);
	str2.Format("%d", max(0, min(255, val)));
	if (str1 != str2) {
		SetDlgItemText(IDC_False_B, str2);
	}
	DrawColor(1);
}

void CGraphLog::OnClickItemlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_iSelectItem = pNMItemActivate->iItem;
	SetDlgItemInt(IDC_SelectItem, m_iSelectItem);
	if (m_iSelectItem >= 0) {
		UpdateData_from_ListTable(m_iSelectItem);
	}

	*pResult = 0;
}

void CGraphLog::OnBnClickedModify()
{
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}
	if (UpdateData_from_Control()) {
		UpdateListTbl_from_Data(m_iSelectItem);
		CString str;

		GetDlgItemText(IDC_AddPreText, str);
		m_ctrlList.SetItemText(m_iSelectItem, 8, str);
		GetDlgItemText(IDC_AddPostText, str);
		m_ctrlList.SetItemText(m_iSelectItem, 9, str);

		GetDlgItemText(IDC_Memo, str);
		m_ctrlList.SetItemText(m_iSelectItem, 18, str);

		m_iSelectItem = -1;
		SetDlgItemInt(IDC_SelectItem, m_iSelectItem);
        ClearControl();
	}
}


void CGraphLog::OnBnClickedDelete()
{
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	m_ctrlList.DeleteItem(m_iSelectItem);

	m_iGraphLogCount--;

	for (int i = m_iSelectItem; i < m_iGraphLogCount; i++) {
		CString str;
		str.Format("%d", i);
		m_ctrlList.SetItemText(i, 0, str);
	}
	m_iSelectItem = -1;
	SetDlgItemInt(IDC_SelectItem, m_iSelectItem);
    ClearControl();
}


void CGraphLog::OnBnClickedMoveup()
{
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	if (m_iSelectItem == 0) return;

	CString str;
	for (int i = 1; i < HEADER_LOG_COLUMN_SIZE - 1; i++) {
		str = m_ctrlList.GetItemText(m_iSelectItem - 1, i);
		m_ctrlList.SetItemText(m_iSelectItem - 1, i, m_ctrlList.GetItemText(m_iSelectItem, i));
		m_ctrlList.SetItemText(m_iSelectItem, i, str);
	}

	m_iSelectItem --;
	SetDlgItemInt(IDC_SelectItem, m_iSelectItem);
}


void CGraphLog::OnBnClickedMovedn()
{
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	if (m_iSelectItem == m_iGraphLogCount - 1) return;
	CString str;
	for (int i = 1; i < HEADER_LOG_COLUMN_SIZE - 1; i++) {
		str = m_ctrlList.GetItemText(m_iSelectItem + 1, i);
		m_ctrlList.SetItemText(m_iSelectItem + 1, i, m_ctrlList.GetItemText(m_iSelectItem, i));
		m_ctrlList.SetItemText(m_iSelectItem, i, str);
	}

	m_iSelectItem++;
	SetDlgItemInt(IDC_SelectItem, m_iSelectItem);

}


void CGraphLog::OnBnClickedSave() {
	CString str;
	if (m_cGraphLogPacketHead == '_' ||(m_cGraphLogPacketHead >= 'a' && m_cGraphLogPacketHead <= 'z') || (m_cGraphLogPacketHead >= 'A' && m_cGraphLogPacketHead <= 'Z')) {
	}
	else {
		AfxMessageBox("Header의 범위는 '_',  a-z 또는 A-Z일때만 유효합니다.");
		return;
	}

	GetDlgItemText(IDC_ImageFilename, str);
	pMain->m_strGraphLog_Jpg.Format("%s", (LPCTSTR)str);
	if (str == "") {
		AfxMessageBox("배경 그림파일이 지정되지 않았습니다.");
	}

	pMain->m_iGraphLogCount = m_iGraphLogCount;
	for (int i = 0; i < m_iGraphLogCount; i++) {
		UpdateData_from_ListTable(i);
		pMain->m_GraphLog[i] = m_Item;
		pMain->m_GraphPreText[i].Format("%s", (LPCTSTR)m_ctrlList.GetItemText(i, 8));
		pMain->m_GraphPostText[i].Format("%s", (LPCTSTR)m_ctrlList.GetItemText(i, 9));
		pMain->m_GraphLogMemo[i].Format("%s", (LPCTSTR)m_ctrlList.GetItemText(i, 18));
	}
	pMain->SaveGraphLogData(m_cGraphLogPacketHead);
	
	pMain->GetGraphLogData();
	if (pMain->m_pUserScreen != nullptr) {
		if (pMain->m_iCurrentIndex > 0) {
			pMain->m_iCurrentIndex--;
		}
		else {
			pMain->m_iCurrentIndex++;
		}
	}
}


void CGraphLog::LoadData() {
	m_ctrlList.DeleteAllItems();
	m_iGraphLogCount = pMain->m_iGraphLogCount;
	for (int i = 0; i < m_iGraphLogCount; i++) {
		LV_ITEM lvitem;
		CString str;

		int nCount = m_ctrlList.GetItemCount();
		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;
		lvitem.iItem = nCount;
		str.Format("%d", nCount);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_ctrlList.InsertItem(&lvitem);
		m_Item = pMain->m_GraphLog[i];		
		UpdateListTbl_from_Data(i);
		m_ctrlList.SetItemText(i, 8, pMain->m_GraphPreText[i]);
		m_ctrlList.SetItemText(i, 9, pMain->m_GraphPostText[i]);
		m_ctrlList.SetItemText(i, 18, pMain->m_GraphLogMemo[i]);
	}
	for (int i = 0; i < m_ctrlList.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_ctrlList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}


void CGraphLog::OnBnClickedCancel()
{
	LoadData();
}

void CGraphLog::OnEnChangeHeaderlogHeader()
{
	CString str;
	GetDlgItemText(IDC_HeaderLog_Header, str);
	int iSize = str.GetLength();
	m_cGraphLogPacketHead = ' ';

	for (int i = 0; i < iSize; i++) {
		BYTE header = str.GetAt(i);
		if ((header >= 'a' && header <= 'z') || (header >= 'A' && header <= 'Z') || header == '_') {
			m_cGraphLogPacketHead = header;
			break;
		}
	}
}


BOOL CGraphLog::PreTranslateMessage(MSG* pMsg){
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGraphLog::DrawColor(int pos) {
	COLORREF color;
	CRect rect;
	if (pos == 0) {
		color = RGB(GetDlgItemInt(IDC_True_R), GetDlgItemInt(IDC_True_G), GetDlgItemInt(IDC_True_B));
		rect.SetRect(630, 360, 660, 379);
	}
	else {
		color = RGB(GetDlgItemInt(IDC_False_R), GetDlgItemInt(IDC_False_G), GetDlgItemInt(IDC_False_B));
		rect.SetRect(630, 427, 660, 446);
	}	

	CClientDC dc(this);
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 1, color);
	brush.CreateSolidBrush(color);
	dc.SelectObject(&pen);
	dc.SelectObject(&brush);
	dc.Rectangle(&rect);
}

void CGraphLog::OnPaint()
{    
	CPaintDC dc(this); 
	DrawColor(0);
	DrawColor(1);
    if (m_ctrlItemType.GetCurSel() < 3) {
        UpdateBitControl();
    }
}

void CGraphLog::OnBnClickedSavegraphlog() {
	if (m_iGraphLogCount == 0) {
		AfxMessageBox("저장가능한 항목이 없습니다.");
	}

	if (((m_cGraphLogPacketHead >= 'a' && m_cGraphLogPacketHead <= 'z') || (m_cGraphLogPacketHead >= 'A' && m_cGraphLogPacketHead <= 'Z')) == FALSE) {
		AfxMessageBox("Header 설정 오류 (a-z || A-Z)");
		return;
	}

	CFile GraphLogSettingFile;
	CFileException ex;

	CString strFileName;
	SYSTEMTIME time;
	GetLocalTime(&time);

	strFileName.Format("GraphLogSetting_%c_%02d%02d%02d_%02d%02d%02d.dat", m_cGraphLogPacketHead, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	CString str, strValue, strMemo, strPreText, strPostText;	
	CGraphLogSetting data, temp;

	GraphLogSettingFile.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary | CFile::shareDenyNone | CFile::typeBinary, &ex);
	
	// Log File Version
	str.Format("Version (%d)\r\nHeader(%d)\r\nGraphLog Count(%d)\r\n", GLOG_TOOL_VERSION, m_cGraphLogPacketHead, m_iGraphLogCount);
	GraphLogSettingFile.Write(str, str.GetLength());
	
	for (int iSelect = 0; iSelect < m_iGraphLogCount; iSelect++) {
		// use
		strValue = m_ctrlList.GetItemText(iSelect, 1);
		if (strValue == "Use") {
			data.use = 1;
		}
		else {
			data.use = 0;
		}

		// Item TYPE
		str = m_ctrlList.GetItemText(iSelect, 2);

		if (str == "setRange for condition") {
			data.item_type = 0;
		}
		else if (str == "SetText if True") {
			data.item_type = 1;
		}
		else if (str == "Show Value if True") {
			data.item_type = 2;
		}
		else if (str == "Show Text") {
			data.item_type = 3;
		}
		else if (str == "Show Value") {
			data.item_type = 4;
		}
		else {
			data.item_type = 0;
		}

		// Data1
		str = m_ctrlList.GetItemText(iSelect, 3);
		temp = CheckData(&str);
		data.data1_type = temp.data1_type;
		if (data.data1_type == 0)
			data.fData1_val = temp.fData1_val;
		else
			data.iData1_val = temp.iData1_val;

		str = m_ctrlList.GetItemText(iSelect, 4);
		if (str == ">") {
			data.cond = 0;
		}
		else if (str == ">=") {
			data.cond = 1;
		}
		else if (str == "==") {
			data.cond = 2;
		}
		else if (str == "!=") {
			data.cond = 3;
		}
		else if (str == "<") {
			data.cond = 4;
		}
		else if (str == "<=") {
			data.cond = 5;
		}
		else {
			data.cond = 0;
		}

		// Data2
		str = m_ctrlList.GetItemText(iSelect, 5);
		temp = CheckData(&str);
		data.data2_type = temp.data1_type;
		if (data.data2_type == 0)
			data.fData2_val = temp.fData1_val;
		else 
			data.iData2_val = temp.iData1_val;

		// Digit
		str = m_ctrlList.GetItemText(iSelect, 6);
		data.digit = atoi(str);

		// Test
		strPreText = m_ctrlList.GetItemText(iSelect, 7);
		strPostText = m_ctrlList.GetItemText(iSelect, 8);

		// Range
		str = m_ctrlList.GetItemText(iSelect, 9); data.range[0] = atoi(str);
		str = m_ctrlList.GetItemText(iSelect, 10); data.range[1] = atoi(str);
		str = m_ctrlList.GetItemText(iSelect, 11); data.range[2] = atoi(str);
		str = m_ctrlList.GetItemText(iSelect, 12); data.range[3] = atoi(str);

		// Color1
		str = m_ctrlList.GetItemText(iSelect, 13);
		COLORREF color = (COLORREF)atoi(str);
		data.color1 = color;

		// Color2
		str = m_ctrlList.GetItemText(iSelect, 14);
		color = (COLORREF)atoi(str);
		data.color2 = color;

		// Mask
		str = m_ctrlList.GetItemText(iSelect, 15);
		BOOL use_mask = atoi(str);

		str = m_ctrlList.GetItemText(iSelect, 16);
		int mask_val = atoi(str);

		//Memo
		strMemo = m_ctrlList.GetItemText(iSelect, 17);

		int type = (data.data1_type == 0) + 2 * (data.data2_type == 0);
		switch (type) {
		case 3:
			str.Format("N%d Use(%d) ItemType(%d) Data1(%d)(%s) Cond(%d) Data2(%d)(%s) Digit(%d) PreText{%s}  PostText{%s} Range(%d)(%d)(%d)(%d) Color1(%d) Color(%d) use_mask{%d} mask_val{%d}  Memo{%s}\r\n",
				iSelect, data.use, data.item_type, data.data1_type, (LPCTSTR)pMain->Double2Str(data.fData1_val), data.cond, data.data2_type, (LPCTSTR)pMain->Double2Str(data.fData2_val),
				data.digit, (LPCTSTR)strPreText, (LPCTSTR)strPostText,
				data.range[0], data.range[1], data.range[2], data.range[3], data.color1, data.color2, use_mask, mask_val, (LPCTSTR)strMemo);
			break;

		case 2:
			str.Format("N%d Use(%d) ItemType(%d) Data1(%d)(%d) Cond(%d) Data2(%d)(%s) Digit(%d) PreText{%s}  PostText{%s} Range(%d)(%d)(%d)(%d) Color1(%d) Color(%d) use_mask{%d} mask_val{%d}  Memo{%s}\r\n",
				iSelect, data.use, data.item_type, data.data1_type, data.iData1_val, data.cond, data.data2_type, (LPCTSTR)pMain->Double2Str(data.fData2_val),
				data.digit, (LPCTSTR)strPreText, (LPCTSTR)strPostText,
				data.range[0], data.range[1], data.range[2], data.range[3], data.color1, data.color2, use_mask, mask_val, (LPCTSTR)strMemo);
			break;

		case 1:
			str.Format("N%d Use(%d) ItemType(%d) Data1(%d)(%s) Cond(%d) Data2(%d)(%d) Digit(%d) PreText{%s}  PostText{%s} Range(%d)(%d)(%d)(%d) Color1(%d) Color(%d) use_mask{%d} mask_val{%d}  Memo{%s}\r\n",
				iSelect, data.use, data.item_type, data.data1_type, (LPCTSTR)pMain->Double2Str(data.fData1_val), data.cond, data.data2_type, data.iData2_val,
				data.digit, (LPCTSTR)strPreText, (LPCTSTR)strPostText,
				data.range[0], data.range[1], data.range[2], data.range[3], data.color1, data.color2, use_mask, mask_val, (LPCTSTR)strMemo);
			break;

		default:
			str.Format("N%d Use(%d) ItemType(%d) Data1(%d)(%d) Cond(%d) Data2(%d)(%d) Digit(%d) PreText{%s}  PostText{%s} Range(%d)(%d)(%d)(%d) Color1(%d) Color(%d) use_mask{%d} mask_val{%d}  Memo{%s}\r\n",
				iSelect, data.use, data.item_type, data.data1_type, data.iData1_val, data.cond, data.data2_type, data.iData2_val,
				data.digit, (LPCTSTR)strPreText, (LPCTSTR)strPostText,
				data.range[0], data.range[1], data.range[2], data.range[3], data.color1, data.color2, use_mask, mask_val, (LPCTSTR)strMemo);
			break;
		}
		GraphLogSettingFile.Write(str, str.GetLength());
	}
	str.Format ("End of File \r\n");
	GraphLogSettingFile.Write(str, str.GetLength());

	long size = (long)(GraphLogSettingFile.GetLength());
	BYTE* pBuffer = new BYTE[size];
	GraphLogSettingFile.SeekToBegin();
	GraphLogSettingFile.Read(pBuffer, (UINT)size);
	
	BYTE sum = 0;

	for (int i = 0; i < size; i++) {
		sum += pBuffer[i];
	}
	delete[] pBuffer;
	GraphLogSettingFile.SeekToEnd();	
	BYTE chksum = 0xFF - sum;
	GraphLogSettingFile.Write(&chksum, 1);
	int s = (int)GraphLogSettingFile.GetPosition();
	GraphLogSettingFile.Close();

	char buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	str.Format("%s\\%s 파일을 생성하였습니다. (checksum:%02X- %d byte)", buffer, (LPCTSTR)strFileName, chksum, s);
	AfxMessageBox(str);
}

void CGraphLog::OnBnClickedLoadgraphlogsetting2()
{
	CString pathName;
	GetDlgItemText(IDC_GraphLogConfigFilePath, pathName);
	while (pathName.Replace(" ", ""));

	ParsingGraphLogConfig(pathName);
}

void CGraphLog::OnBnClickedLoadgraphlogsetting() {
	static TCHAR BASED_CODE szFilter[] = _T("설정 파일(*.dat) | *.dat |모든파일(*.*)|*.*||");

	CString strSavedDirectory;
	char CurrentPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	CFileDialog dlg(TRUE, _T("*.dat"), _T("*.dat"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter); // 1
	// 마지막 Access한 폴더으로 시작
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	dlg.m_ofn.lpstrInitialDir = CurrentPath;
	dlg.m_ofn.lpstrTitle = _T("파일 열기");
	int result = dlg.DoModal();
	if (IDOK != result) {
		return;
	}
	CString pathName = dlg.GetPathName();
	ParsingGraphLogConfig(pathName);	
}

BOOL CGraphLog::ParsingGraphLogConfig(const CString&  pathName) {
	CFile OpenFile;
	BOOL bFinish = FALSE;	

	if (OpenFile.Open(pathName, CFile::modeRead | CFile::shareDenyNone)) {
		BYTE* pBuffer;
		// pathName에서 폴더 정보만 추출
		long size = (long)(OpenFile.GetLength());
		pBuffer = new BYTE[(UINT)size + 1];
		OpenFile.Read(pBuffer, (UINT)size);
		OpenFile.Close();
		pBuffer[size] = 0;
		CString str;
		CString strValue;
		CString strTemp[10];
		BOOL bConti = TRUE;
		int iVersion;
		BYTE chksum = 0;
		CString strMode = "", strAddr, strItem;

		// Buffer range초기화
		pMain->m_iBufferIndex = 0;
		pMain->m_iBufferSize = size;

		for (int i = 0; i < size; i++) {
			chksum += pBuffer[i];
			pBuffer[i] -= KEYCODE;
		}

		// FileVersion Check
		if (pMain->GetNextValue(pBuffer, &strValue) > 0) {
			iVersion = atoi(strValue);
			if (iVersion < 110) {  // Initial version : 100
				bConti = FALSE;
				AfxMessageBox("Setting File Version is Too Low");
			}
			else {
				if (iVersion > GLOG_TOOL_VERSION) {
					bConti = FALSE;
					AfxMessageBox("Setting File Version is latest than tool program.");
				}

				if (chksum != 0xFF) {
					if (AfxMessageBox("저장 파일에 오류가 있는것 같습니다.(Checksum Error)\r\n계속 진행할까요?", MB_YESNO) != IDYES) bConti = FALSE;
				}
			}
		}
		else {
			bConti = FALSE;
		}

		// Get Header
		int iMaxMode = 0;
		if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
			m_cGraphLogPacketHead = atoi(strValue);
			str.Format("%c", m_cGraphLogPacketHead);
			SetDlgItemText(IDC_HeaderLog_Header, str);
			pMain->Wait(500);
		}
		else {
			bConti = FALSE;
		}

		int iGraphLogCount = 0;
		int dataType;
		int dataID;
		COLORREF color;
		if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
			iGraphLogCount = atoi(strValue);
			if (iGraphLogCount <= 0 || iGraphLogCount > N_MAX_GRAPHLOG_ITEM) {
				bConti = FALSE;
			}
		}
		else {
			bConti = FALSE;
		}

		m_ctrlList.DeleteAllItems();
		m_iGraphLogCount = 0;
		for (int i = 0; i < iGraphLogCount && bConti; i++) {
			// use
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				CheckDlgButton(IDC_UseItem, atoi(strValue));
			}
			else {
				bConti = FALSE;
			}

			// ItemType
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				m_ctrlItemType.SetCurSel(atoi(strValue));
			}
			else {
				bConti = FALSE;
			}

			// DataType1
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				dataType = atoi(strValue);
			}
			else {
				bConti = FALSE;
			}

			//DataID1
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				dataID = atoi(strValue);
				switch (dataType) {
				case 1: str.Format("D%d", dataID); break;
				case 2: str.Format("M%d", dataID); break;
				case 0: str.Format("%d", dataID); break;
				}
				SetDlgItemText(IDC_Data1, str);
			}
			else {
				bConti = FALSE;
			}

			//Cond
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				m_ctrlCond.SetCurSel(atoi(strValue));
			}
			else {
				bConti = FALSE;
			}

			// DataType2
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				dataType = atoi(strValue);
			}
			else {
				bConti = FALSE;
			}

			//DataID2
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				dataID = atoi(strValue);
				switch (dataType) {
				case 1: str.Format("D%d", dataID); break;
				case 2: str.Format("M%d", dataID); break;
				case 0: str.Format("%d", dataID); break;
				}
				SetDlgItemText(IDC_Data2, str);
			}
			else {
				bConti = FALSE;
			}

			// Digit
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				m_ctrlDigit.SetCurSel(atoi(strValue));
			}
			else {
				bConti = FALSE;
			}
            
            if (iVersion >= 115) {
                if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
                    m_ctrlDiv.SetCurSel(atoi(strValue));
                }
                else {
                    bConti = FALSE;
                }
            }

			// Pre Text
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_AddPreText, strValue);
			}
			else {
				bConti = FALSE;
			}

			// Post Text
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_AddPostText, strValue);
			}
			else {
				bConti = FALSE;
			}

			// Range (4)
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_X, strValue);
			}
			else {
				bConti = FALSE;
			}

			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_Y, strValue);
			}
			else {
				bConti = FALSE;
			}

			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_DX, strValue);
			}
			else {
				bConti = FALSE;
			}

			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_DY, strValue);
			}
			else {
				bConti = FALSE;
			}

			// RGB1
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				color = (COLORREF)atoi(strValue);
				SetDlgItemInt(IDC_True_R, GetRValue(color));
				SetDlgItemInt(IDC_True_G, GetGValue(color));
				SetDlgItemInt(IDC_True_B, GetBValue(color));
			}
			else {
				bConti = FALSE;
			}

			// RGB2
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				color = (COLORREF)atoi(strValue);
				SetDlgItemInt(IDC_False_R, GetRValue(color));
				SetDlgItemInt(IDC_False_G, GetGValue(color));
				SetDlgItemInt(IDC_False_B, GetBValue(color));
			}
			else {
				bConti = FALSE;
			}

			//Use Mask
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				CheckDlgButton(IDC_UseMaskValue, atoi(strValue));
			}
			else {
				bConti = FALSE;
			}

			// Mask Value
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				int data = atoi(strValue);
				UpdateMaskValue(data);
			}
			else {
				bConti = FALSE;
			}

			//Memo
			if (bConti && pMain->GetNextValue(pBuffer, &strValue) > 0) {
				SetDlgItemText(IDC_Memo, strValue);
			}
			else {
				bConti = FALSE;
			}

			if (bConti) {
				pMain->Wait(100);
				OnBnClickedAdditem();
			}
		}

		if (bConti) {
			// 저장할까요?
			AfxMessageBox("설정 파일 불러오기가 완료되었습니다.");
		}
		else {
			AfxMessageBox("Error: 설정 파일 읽는 중 오류가 발생하였습니다.");
		}

		if (pBuffer != nullptr) {
			delete[]pBuffer;
		}
		return bConti;
	}
	else {
		CString str;
		str.Format("File Open: Error :  %s", (LPCTSTR)pathName);
		AfxMessageBox("Error: 설정 파일 읽는 중 오류가 발생하였습니다.");		
	}
	return FALSE;
}

void CGraphLog::UpdateBitControl() {
    BOOL bInit = IsDlgButtonChecked(IDC_UseMaskValue);

    CClientDC dc(this);
    int x0 = 907;
    int y0 = 385;
    int iLength = 12;
    int iDx = 5;
    dc.SelectObject(&(pMain->m_BrushSelectControl_Blank[1]));
    dc.SelectObject(&(pMain->m_PenSelectControl_Blank[1]));
    for (int i = 0; i < 16; i++) {
        int nBlock = i / 4;
        int x = x0 + i * (iLength + 1) + nBlock * iDx;
        int y = y0;
        dc.Rectangle(x, y, x + iLength, y + iLength);
    }
    dc.SelectObject(&(pMain->m_BrushSelectControl_Select[1]));
    dc.SelectObject(&(pMain->m_PenSelectControl_Select[1]));
    BOOL bCheck = FALSE;
    for (int i = 0; i < 16; i++) {
        if (m_bMask[15 - i]) {
            int nBlock = i / 4;
            int x = x0 + i * (iLength + 1) + nBlock * iDx + 2;
            int y = y0 + 2;
            dc.Rectangle(x, y, x + iLength - 4, y + iLength - 4);
            bCheck = TRUE;
        }
    }
    if (bInit != bCheck) {
        CheckDlgButton(IDC_UseMaskValue, bCheck);
    }
}

void CGraphLog::DrawBitControl(BOOL bShow) {	
	GetDlgItem(IDC_STATIC_bit0)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_bit1)->ShowWindow(bShow);
	
	if (bShow == TRUE) {		
        UpdateBitControl();
	}
	else {
		Invalidate(TRUE);
	}
}

void CGraphLog::InitMaskSetting() {
	for (int i = 0; i < 16; i++) {
		m_bMask[i] = 0;
	}
}

void CGraphLog::OnLButtonDown(UINT nFlags, CPoint point) {
    if (m_ctrlItemType.GetCurSel() < 3) {
        int x0 = 907;
        int y0 = 385;
        int iLength = 12;
        int iDx = 5;
        int iDy = 24;

        int x_min = x0;
        int x_max = x0 + 15 * (iLength + 1) + 3 * iDx + iLength;
        int y_min = y0;
        int y_max = y0 + iLength;
        if (x_min < point.x && x_max > point.x) {
            if (y_min < point.y && point.y < y_max) {
                for (int i = 0; i < 16; i++) {
                    int nBlock = i / 4;
                    int x = x0 + i * (iLength + 1) + nBlock * iDx;
                    if (point.x >= x && point.x <= x + iLength) {
                        m_bMask[15 - i] ^= 0x01;
                        DrawBitControl(TRUE);
                        break;
                    }
                }
            }
        }
    }

#if 0
    // Position Debug
    CString str;
    str.Format("X:%d   Y:%d", point.x, point.y);
	SetDlgItemText(IDC_ImageFilename, str);
#endif
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CGraphLog::OnBnClickedUsemaskvalue() {    
	if (m_ctrlItemType.GetCurSel() < 3) {
        if (IsDlgButtonChecked(IDC_UseMaskValue) == FALSE) {
            InitMaskSetting();
        }
		DrawBitControl(TRUE);
	}
	else {
		DrawBitControl(FALSE);
	}
}


void CGraphLog::OnBnClickedDatachange()
{
	int idc[] = { IDC_SendRegionData, IDC_GetRegionData, IDC_SendColorData1, IDC_GetColorData1,  IDC_SendColorData2, IDC_GetColorData2,	};
	BOOL bShow = IsDlgButtonChecked(IDC_DataChange);

	for (int i = 0; i < 6; i++) {
		GetDlgItem(idc[i])->ShowWindow(bShow);
	}
};


void CGraphLog::OnBnClickedSendregiondata()
{
	pMain->m_DataConversion.range[0] = GetDlgItemInt(IDC_X);
	pMain->m_DataConversion.range[1] = GetDlgItemInt(IDC_Y);
	pMain->m_DataConversion.range[2] = GetDlgItemInt(IDC_DX);
	pMain->m_DataConversion.range[3] = GetDlgItemInt(IDC_DY);
}


void CGraphLog::OnBnClickedGetregiondata()
{
	SetDlgItemInt(IDC_X, pMain->m_DataConversion.range[0]);
	SetDlgItemInt(IDC_Y, pMain->m_DataConversion.range[1]);
	SetDlgItemInt(IDC_DX, pMain->m_DataConversion.range[2]);
	SetDlgItemInt(IDC_DY, pMain->m_DataConversion.range[3]);
}


void CGraphLog::OnBnClickedSendcolordata1()
{
	pMain->m_DataConversion.color1 = RGB(GetDlgItemInt(IDC_True_R), GetDlgItemInt(IDC_True_G), GetDlgItemInt(IDC_True_B));
}


void CGraphLog::OnBnClickedGetcolordata1()
{
	COLORREF col = pMain->m_DataConversion.color1;
	SetDlgItemInt(IDC_True_R, GetRValue(col));
	SetDlgItemInt(IDC_True_G, GetGValue(col));
	SetDlgItemInt(IDC_True_B, GetBValue(col));
}


void CGraphLog::OnBnClickedSendcolordata2()
{
	pMain->m_DataConversion.color2 = RGB(GetDlgItemInt(IDC_False_R), GetDlgItemInt(IDC_False_G), GetDlgItemInt(IDC_False_B));
}


void CGraphLog::OnBnClickedGetcolordata2()
{
	COLORREF col = pMain->m_DataConversion.color2;
	SetDlgItemInt(IDC_False_R, GetRValue(col));
	SetDlgItemInt(IDC_False_G, GetGValue(col));
	SetDlgItemInt(IDC_False_B, GetBValue(col));
}

void CGraphLog::OnEnChangeData1() {
	CString str;
	GetDlgItemText(IDC_Data1, str); 
    
	CGraphLogSetting temp = CheckData(&str);

}


void CGraphLog::OnEnChangeData2()
{
}


void CGraphLog::ClearControl() {
    m_ctrlCond.SetCurSel(0);
    m_ctrlDigit.SetCurSel(0);
    m_ctrlDiv.SetCurSel(0);

    SetDlgItemText(IDC_Data2, "");
    SetDlgItemText(IDC_Data1, "");

    SetDlgItemText(IDC_True_R, "");
    SetDlgItemText(IDC_True_G, "");
    SetDlgItemText(IDC_True_B, "");

    SetDlgItemText(IDC_False_R, "");
    SetDlgItemText(IDC_False_G, "");
    SetDlgItemText(IDC_False_B, "");

    SetDlgItemText(IDC_X, "");
    SetDlgItemText(IDC_Y, "");
    SetDlgItemText(IDC_DX, "");
    SetDlgItemText(IDC_DY, "");
    
}
