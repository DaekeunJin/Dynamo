// HeaderLogDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "HeaderLogDlg.h"
#include "SerialComDlg.h"
#include "UserSetting.h"

// CHeaderLogDlg 대화 상자

IMPLEMENT_DYNAMIC(CHeaderLogDlg, CDialogEx)

CHeaderLogDlg::CHeaderLogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HeaderLogDlg, pParent)
	,m_iSelectItem(-1)
	,m_bIsUpdated(FALSE)
{
	for (int i = 0; i < 16; i++) { 
		m_bMask[i] = 0;
		m_bValue[i] = 0;
	}
}

CHeaderLogDlg::~CHeaderLogDlg()
{
}

void CHeaderLogDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ItemList, m_ctrlHeaderLog);
    DDX_Control(pDX, IDC_DataID, m_ctrlDataID);
    DDX_Control(pDX, IDC_Condition, m_ctrlCondition);
    DDX_Control(pDX, IDC_TextDigit, m_ctrlDP);
    DDX_Control(pDX, IDC_TextDivider, m_ctrlDivide);
}


BEGIN_MESSAGE_MAP(CHeaderLogDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Apply, &CHeaderLogDlg::OnBnClickedApply)
	ON_EN_CHANGE(IDC_HeaderLog_Header, &CHeaderLogDlg::OnEnChangeHeaderlogHeader)
	ON_NOTIFY(NM_CLICK, IDC_ItemList, &CHeaderLogDlg::OnClickItemlist)
	ON_BN_CLICKED(IDC_AddHeaderLogItem, &CHeaderLogDlg::OnBnClickedAddheaderlogitem)
	ON_BN_CLICKED(IDC_DeleteHeaderLogItem, &CHeaderLogDlg::OnBnClickedDeleteheaderlogitem)
	ON_CBN_SELCHANGE(IDC_Condition, &CHeaderLogDlg::OnSelchangeCondition)
	ON_EN_CHANGE(IDC_ValueHex, &CHeaderLogDlg::OnEnChangeValuehex)	
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_MaskValue, &CHeaderLogDlg::OnEnChangeMaskvalue)
	ON_EN_CHANGE(IDC_Value, &CHeaderLogDlg::OnEnChangeValue)
	ON_EN_CHANGE(IDC_ValueHex2, &CHeaderLogDlg::OnEnChangeValuehex2)
	ON_BN_CLICKED(IDC_ModifyHeaderLogItem, &CHeaderLogDlg::OnBnClickedModifyheaderlogitem)
	ON_BN_CLICKED(IDC_SaveHeaderLog, &CHeaderLogDlg::OnBnClickedSaveheaderlog)
	ON_BN_CLICKED(IDC_Export, &CHeaderLogDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_Import, &CHeaderLogDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_Default, &CHeaderLogDlg::OnBnClickedDefault)
	ON_BN_CLICKED(IDC_DeleteAll, &CHeaderLogDlg::OnBnClickedDeleteall)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MoveUp, &CHeaderLogDlg::OnBnClickedMoveup)
	ON_BN_CLICKED(IDC_MoveDn, &CHeaderLogDlg::OnBnClickedMovedn)
END_MESSAGE_MAP()


// CHeaderLogDlg 메시지 처리기
const char* const CHeaderLogDlg::m_strCond[] = {
	"Blank",
	"Masking & Match",
	"Show Value",
	"Masked Value",
	"Greater Than (>)",
	"Greater Equal (>=)",
	"Less Than (<)",
	"Less Equal (<=)",
	"Error"
};


void CHeaderLogDlg::OnBnClickedApply()
{
}


void CHeaderLogDlg::OnEnChangeHeaderlogHeader() {
	char cHeader = m_cHeader;
	CString strHeader;
	GetDlgItemText(IDC_HeaderLog_Header, strHeader);
	int iSize = strHeader.GetLength();

	if (iSize < 10) {
		char myString[10];
		strcpy_s(myString, (LPCTSTR)strHeader);
		for (int j = 0; j < iSize; j++) {
			if ((myString[j] >= 'a' && myString[j] <= 'z') || (myString[j] >= 'A' && myString[j] <= 'Z') || myString[j] == '_') {
				m_cHeader = myString[j];
				break;
			}
		}
	}

	m_strHeader.Format("%c", m_cHeader);
	if (m_strHeader != strHeader) {
		SetDlgItemText(IDC_HeaderLog_Header, m_strHeader);
	}

	if (cHeader != m_cHeader && cHeader) {
		if (m_bIsUpdated) {
			if (AfxMessageBox("저장되지 않은 수정항목이 있습니다. 계속진행할까요?", MB_YESNO) == IDNO) {
				m_strHeader.Format("%c", cHeader);
				m_cHeader = cHeader;
				SetDlgItemText(IDC_HeaderLog_Header, m_strHeader);
				return;
			}
		}
		m_iSelectItem = -1;
		SetDlgItemText(IDD_HeaderLogDlg, "");
		GetHeaderLog();
		m_bIsUpdated = FALSE;
	}
}


BOOL CHeaderLogDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	AddColumn();
	m_cHeader = (unsigned char)(pMain->m_cPacketHead);	
	m_strHeader.Format("%c", m_cHeader);
	SetDlgItemText(IDC_HeaderLog_Header, m_strHeader);
	m_ctrlCondition.SetCurSel(0);
	m_ctrlDataID.SetCurSel(0);
    m_ctrlDP.SetCurSel(0);
    m_ctrlDivide.SetCurSel(0);
	GetHeaderLog();
	ShowBit(FALSE);
	CheckDlgButton(IDC_UseItem, TRUE);
	ShowControl(0);
	return TRUE;
}


void CHeaderLogDlg::GetHeaderLog() {
	if ((m_cHeader >= 'a' && m_cHeader <= 'z') || (m_cHeader >= 'A' && m_cHeader <= 'Z') || m_cHeader == '_') {
		// Init Variables
		m_ctrlHeaderLog.DeleteAllItems();
		m_iTextLogCount = pMain->GetTextLogCount(m_cHeader);

        std::vector<CTextLogSetting> textLogs;
        textLogs.resize(m_iTextLogCount);

        for (int i = 0; i < m_iTextLogCount; i++) {
            pMain->GetHeaderLogFromRegistry(m_cHeader, i, &textLogs[i]);
            ShowHeaderLogData(i, &textLogs[i]);
        }

		for (int i = 0; i < m_ctrlHeaderLog.GetHeaderCtrl()->GetItemCount(); i++) {
			VERIFY(m_ctrlHeaderLog.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
		}
	}
}

void CHeaderLogDlg::AddColumn() {
#define HEADER_LOG_COLUMN_SIZE	13
	LV_COLUMN lvcolumn;
	char *listColumn[HEADER_LOG_COLUMN_SIZE] = { "No.", "Use","Condition", "Data_ID", "Masking", "Value", "Show1", "Comment 1", "Show2", "Comment 2", "Digit", "Divider","" };
	int width[HEADER_LOG_COLUMN_SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, };
	int i = 0;

	for (i = 0; i < HEADER_LOG_COLUMN_SIZE; i++) {
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;		
		lvcolumn.pszText = listColumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		m_ctrlHeaderLog.InsertColumn(i, &lvcolumn);
	}
		
	m_ctrlHeaderLog.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (i = 0; i < m_ctrlHeaderLog.GetHeaderCtrl()->GetItemCount(); i++)
		VERIFY(m_ctrlHeaderLog.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
}


void CHeaderLogDlg::OnBnClickedDeleteheaderlogitem() {
	UDF;
	if (m_iSelectItem != -1) {
		int nCount = m_ctrlHeaderLog.GetItemCount();
		for (int i = m_iSelectItem + 1; i < nCount; i++) {
			for (int j = 1; j < N_TEXTLOG_ITEM; j++) {
				m_ctrlHeaderLog.SetItemText(i - 1, j, m_ctrlHeaderLog.GetItemText(i, j));
			}
		}
		m_ctrlHeaderLog.DeleteItem(nCount - 1);
		m_iTextLogCount--;
		m_iSelectItem = -1;
		SetDlgItemText(IDD_HeaderLogDlg, "");
		m_bIsUpdated = TRUE;

	}
	else { 
		AfxMessageBox("No Item is Selected "); 
	}	
}


void CHeaderLogDlg::OnSelchangeCondition() {
	ShowControl(m_ctrlCondition.GetCurSel());
}

BOOL CHeaderLogDlg::IsValid_Input(int iCondition, CString* strError) {
	BOOL bIsValid = TRUE;
	int iMask, iValue;
	BOOL bCheck1, bCheck2;
	CString strMask, strValue, strComment1, strComment2;

	iMask = GetDlgItemInt(IDC_MaskValue);
	iValue = GetDlgItemInt(IDC_Value);
	GetDlgItemText(IDC_MaskValue, strMask);
	GetDlgItemText(IDC_Value, strValue);
	bCheck1 = IsDlgButtonChecked(IDC_ShowComment1);
	bCheck2 = IsDlgButtonChecked(IDC_ShowComment2);
	GetDlgItemText(IDC_Comment1, strComment1);
	GetDlgItemText(IDC_Comment2, strComment2);

	CString str;
	GetDlgItemText(IDC_DataID, str);
	if (iCondition != eLogCondition_Blank && IsAvailableDataID(&str) == FALSE) {
		return FALSE;
	}

	switch (iCondition) {
	case eLogCondition_Blank:
		bIsValid = TRUE;
		break;

	case eLogCondition_MatchMask:
		if (strMask == "") { bIsValid = FALSE;  strError->Format("Masking 항목이 비어있습니다."); }
		if (strValue == "") { bIsValid = FALSE;  strError->Format("Value 항목이 비어있습니다.");  }
		for (int i = 0; i < 16; i++) {
			if ((iValue & (0x01 << i))) {  // && ((iMask & (0x01 < i)) != (iValue & (0x01 << i)))) {
				if (!(iMask & (0x01 << i))) {
					bIsValid = FALSE;
					strError->Format("Bit%d의 Masking값(%d)과 Value값(%d)에 문제가 있습니다.", i, (iMask & (0x01 << i)) >> i, (iValue & (0x01 << i)) >> i);
				}
			}
		}
		if ((bCheck1 == FALSE) && (bCheck2 == FALSE)) { bIsValid = FALSE;  strError->Format("적어도 1개이상의 CheckBox가 선택되어야 합니다."); }
		if (bCheck1 == TRUE && strComment1 == "") { bIsValid = FALSE;  strError->Format("조건이 일치할때 표시될 설명이 비어있습니다."); }
		if (bCheck2 == TRUE && strComment2 == "") { bIsValid = FALSE;  strError->Format("조건이 불일치할때 표시될 설명이 비어있습니다."); }		
		break;

	case eLogCondition_ShowValue:
		if (strComment1 == "") { bIsValid = FALSE;  strError->Format("항목에 대한 설명이 비어있습니다."); }
		break;

	case eLogCondition_MaskedValue:
		if (strComment1 == "") { bIsValid = FALSE;  strError->Format("항목에 대한 설명이 비어있습니다."); }
		if (strValue == "") { bIsValid = FALSE;  strError->Format("배율 항목이 비어있습니다."); }		
		break;

	case eLogCondition_Greater:
	case eLogCondition_GreaterEqual:
	case eLogCondition_Less:
	case eLogCondition_LessEqual:
        if (strComment1 == "" &&  strComment2 == "") { bIsValid = FALSE;  strError->Format("항목에 대한 설명이 비어있습니다."); }
        if (strValue == "" || strValue == " ") { bIsValid = FALSE;  strError->Format("Ref value값이 비어 있습니다."); }        
		break;

	default:
		bIsValid = FALSE; 
		strError->Format("Log Condition이 선택되지 않았습니다.");		
		break;
	}

	if ((('a' <= m_cHeader && m_cHeader <= 'z') || ('A' <= m_cHeader && m_cHeader <= 'Z') || m_cHeader == '_') == FALSE) {
		bIsValid = FALSE;
		strError->Format("Header 설정에 문제가 있습니다.");
	}

	return bIsValid;
}

void CHeaderLogDlg::ShowControl(int iCondition) {
	CString str;
	switch(iCondition) {
	case eLogCondition_Blank:
		GetDlgItem(IDC_STATIC_DataID)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Mask)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Value)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_DataID)->ShowWindow(FALSE);
		GetDlgItem(IDC_MaskValue)->ShowWindow(FALSE);
		GetDlgItem(IDC_Value)->ShowWindow(FALSE);
		GetDlgItem(IDC_ShowComment1)->ShowWindow(FALSE);
		GetDlgItem(IDC_ShowComment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment2)->ShowWindow(FALSE);	
		SetDlgItemText(IDC_STATIC_Comment1, "  Comment");
        GetDlgItem(IDC_STATIC_TextDP)->ShowWindow(FALSE);
        GetDlgItem(IDC_TextDigit)->ShowWindow(FALSE);
        GetDlgItem(IDC_TextDivider)->ShowWindow(FALSE);

		break;

	case eLogCondition_MatchMask:
		GetDlgItem(IDC_STATIC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Mask)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Value)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment2)->ShowWindow(TRUE);
		GetDlgItem(IDC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_MaskValue)->ShowWindow(TRUE);
		GetDlgItem(IDC_Value)->ShowWindow(TRUE);
		GetDlgItem(IDC_ShowComment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_ShowComment2)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment2)->ShowWindow(TRUE);

		str.Format("일치할 때 표시 및 Comment");
		SetDlgItemText(IDC_STATIC_Comment1, str);
		ShowBit(TRUE);
		GetDlgItem(IDC_STATIC_Comment1)->MoveWindow(442, 345, 152, 14, TRUE);
		GetDlgItem(IDC_ShowComment1)->MoveWindow(442, 363, 15, 15, TRUE);
		SetDlgItemText(IDC_STATIC_Comment1, "일치할 때 표시 및 Comment");

		GetDlgItem(IDC_Value)->MoveWindow(388, 361, 45, 20);
		SetDlgItemText(IDC_STATIC_Value, "Value");
		GetDlgItem(IDC_STATIC_Value)->MoveWindow(388, 345, 45, 14);
        GetDlgItem(IDC_STATIC_TextDP)->ShowWindow(FALSE);
        GetDlgItem(IDC_TextDigit)->ShowWindow(FALSE);
        GetDlgItem(IDC_TextDivider)->ShowWindow(FALSE);
		break;

	case eLogCondition_ShowValue:
		GetDlgItem(IDC_STATIC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Mask)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Value)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_MaskValue)->ShowWindow(FALSE);
		GetDlgItem(IDC_Value)->ShowWindow(FALSE);
		GetDlgItem(IDC_ShowComment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_ShowComment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Comment1)->MoveWindow(333, 345, 192, 14, TRUE);
		GetDlgItem(IDC_ShowComment1)->MoveWindow(373, 363, 15, 15, TRUE);
		SetDlgItemText(IDC_STATIC_Comment1, "   값위치 전방               Comment     ");
        GetDlgItem(IDC_STATIC_TextDP)->ShowWindow(TRUE);
        GetDlgItem(IDC_TextDigit)->ShowWindow(TRUE);
        GetDlgItem(IDC_TextDivider)->ShowWindow(TRUE);
		break;

	case eLogCondition_MaskedValue:
		GetDlgItem(IDC_STATIC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Mask)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Value)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_MaskValue)->ShowWindow(TRUE);
		GetDlgItem(IDC_Value)->ShowWindow(TRUE);
		GetDlgItem(IDC_ShowComment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_ShowComment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Comment1)->MoveWindow(395, 345, 130, 14, TRUE);
		GetDlgItem(IDC_ShowComment1)->MoveWindow(415, 363, 15, 15, TRUE);
		SetDlgItemText(IDC_STATIC_Comment1, "값위치 전방         Comment");
		GetDlgItem(IDC_Value)->MoveWindow(603, 361, 70, 20);
		SetDlgItemText(IDC_STATIC_Value, "배율(미표시:1)");
		GetDlgItem(IDC_STATIC_Value)->MoveWindow(603, 345, 75, 14);
        GetDlgItem(IDC_STATIC_TextDP)->ShowWindow(FALSE);
        GetDlgItem(IDC_TextDigit)->ShowWindow(FALSE);
        GetDlgItem(IDC_TextDivider)->ShowWindow(FALSE);
		break;

	case eLogCondition_Greater:
	case eLogCondition_GreaterEqual:
	case eLogCondition_Less:
	case eLogCondition_LessEqual:
		GetDlgItem(IDC_STATIC_DataID)->ShowWindow(TRUE);		
		GetDlgItem(IDC_STATIC_Value)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Mask)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment2)->ShowWindow(FALSE);
		GetDlgItem(IDC_DataID)->ShowWindow(TRUE);
		GetDlgItem(IDC_MaskValue)->ShowWindow(FALSE);
		GetDlgItem(IDC_Value)->ShowWindow(TRUE);
		GetDlgItem(IDC_ShowComment1)->ShowWindow(FALSE);
		GetDlgItem(IDC_ShowComment2)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment1)->ShowWindow(TRUE);
		GetDlgItem(IDC_Comment2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Comment1)->MoveWindow(380, 345, 340, 14, TRUE);
		SetDlgItemText(IDC_STATIC_Comment1, "Ref. Value           Comment1                         value     Comment2");
		GetDlgItem(IDC_Value)->MoveWindow(405, 361, 45, 20);
        GetDlgItem(IDC_STATIC_TextDP)->ShowWindow(TRUE);
        GetDlgItem(IDC_TextDigit)->ShowWindow(TRUE);
        GetDlgItem(IDC_TextDivider)->ShowWindow(TRUE);
		break;
	}
	ShowBit(iCondition);
}

void CHeaderLogDlg::OnEnChangeValuehex() {	
	CString str1, str2;
	GetDlgItemText(IDC_ValueHex, str1);

	int iSize = str1.GetLength();

	int iDec = 0;
	if (iSize < 10) {
		char myString[10];
		strcpy_s(myString, (LPCTSTR)str1);
		for (int j = 0; j < iSize; j++) {
			if (myString[j] >= 'a' && myString[j] <= 'f'){
				iDec *= 16;
				iDec += myString[j] - 'a' + 10;
			}
			else if (myString[j] >= 'A' && myString[j] <= 'F') {
				iDec *= 16;
				iDec += myString[j] - 'A' + 10;
			}
			else if (myString[j] >= '0' && myString[j] <= '9') {
				iDec *= 16;
				iDec += myString[j] - '0';
			}
		}
	}

	iDec = min(65535, max(0, iDec));
	str2.Format("%X", iDec);
	if (str1 != str2) { SetDlgItemText(IDC_ValueHex, str2); }

	str2.Format("%d", iDec);
	GetDlgItemText(IDC_MaskValue, str1);
	if (str1 != str2) { SetDlgItemText(IDC_MaskValue, str2); }

	for (int i = 0; i < 16; i++) {
		m_bMask[i] = min(iDec & (0x01 << i), TRUE);
	}
	DrawBitControl();
}


void CHeaderLogDlg::OnBnClickedBin() {
	int iDec = 0;
	for (int i = 0; i < 16; i++) {
		if (m_bMask[i]) { iDec += (0x01 << i); }
	}

	CString str1, str2;
	str2.Format("%d", iDec);
	GetDlgItemText(IDC_MaskValue, str1);
	if (str1 != str2) { SetDlgItemText(IDC_MaskValue, str2); }

	GetDlgItemText(IDC_ValueHex, str1);
	str2.Format("%X", iDec);
	if (str1 != str2) { SetDlgItemText(IDC_ValueHex, str2); }


	iDec = 0;
	for (int i = 0; i < 16; i++) {
		if (m_bValue[i]) { iDec += (0x01 << i); }
	}

	str2.Format("%d", iDec);
	GetDlgItemText(IDC_Value, str1);
	if (str1 != str2) { SetDlgItemText(IDC_Value, str2); }

	GetDlgItemText(IDC_ValueHex2, str1);
	str2.Format("%X", iDec);
	if (str1 != str2) { SetDlgItemText(IDC_ValueHex2, str2); }

	DrawBitControl();
}


void CHeaderLogDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	CString str;
	str.Format("%d %d", point.x, point.y);
	SetDlgItemText(IDC_Message, str);

	int x0 = 123;
	int y0 = 413;
	int iLength = 12;
	int iDx = 5;
	int iDy = 24;
	
	int iCurrentCondition = m_ctrlCondition.GetCurSel();
	int x_min = x0; 
	int x_max = x0 + 15 * (iLength + 1) + 3 * iDx + iLength;
	int y_min = y0;
	int y_max = y0 + iLength;
	if (x_min < point.x && x_max > point.x) {
		if (y_min < point.y && point.y < y_max) {
			if (iCurrentCondition == eLogCondition_MatchMask || eLogCondition_MaskedValue == iCurrentCondition) {
				for (int i = 0; i < 16; i++) {
					int nBlock = i / 4;
					int x = x0 + i * (iLength + 1) + nBlock * iDx;
					if (point.x >= x && point.x <= x + iLength) {
						m_bMask[15-i] ^= 0x01;
						OnBnClickedBin();						
						break;
					}
				}
			}
		}

		y_min += iDy;
		y_max += iDy;
		if (y_min < point.y && point.y < y_max) {
			if (iCurrentCondition == eLogCondition_MatchMask) {
				for (int i = 0; i < 16; i++) {
					int nBlock = i / 4;
					int x = x0 + i * (iLength + 1) + nBlock * iDx;
					if (point.x >= x && point.x <= x + iLength) {
						m_bValue[15 -i] ^= 0x01;
						OnBnClickedBin();
						break;
					}
				}
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CHeaderLogDlg::OnEnChangeMaskvalue() {
	int iDec = min(65535, max(0, GetDlgItemInt(IDC_MaskValue)));
	CString str1, str2;
	GetDlgItemText(IDC_MaskValue, str1);
	str2.Format("%d", iDec);

	if (str1 != str2) { SetDlgItemText(IDC_MaskValue, str2); }
	
	str2.Format("%X", iDec);
	GetDlgItemText(IDC_ValueHex, str1);
	if (str1 != str2) { SetDlgItemText(IDC_ValueHex, str2); }

	for (int i = 0; i < 16; i++) {
		m_bMask[i] = min(iDec & (0x01 << i), TRUE);
	}
	DrawBitControl();
}


void CHeaderLogDlg::OnEnChangeValue() {
	CStringA str1, str2;
	GetDlgItemText(IDC_Value, str1);

	if (m_ctrlCondition.GetCurSel() < eLogCondition_Greater) {
		int iDec = min(65535, max(0, GetDlgItemInt(IDC_Value)));		
		str2.Format("%ld", iDec);
		if (str1 != str2) { SetDlgItemText(IDC_Value, str2); }

		str2.Format("%X", iDec);
		GetDlgItemText(IDC_ValueHex2, str1);
		if (str1 != str2) { SetDlgItemText(IDC_ValueHex2, str2); }

		for (int i = 0; i < 16; i++) {
			m_bValue[i] = min(iDec & (0x01 << i), TRUE);
		}
		DrawBitControl();
	}
	else if (pMain->CheckNumStr(&str1)) {
		SetDlgItemText(IDC_Value, str1);
	}
}


void CHeaderLogDlg::OnEnChangeValuehex2() {
	CString str1, str2;
	GetDlgItemText(IDC_ValueHex2, str1);

	int iSize = str1.GetLength();

	int iDec = 0;
	if (iSize < 10) {
		char myString[10];
		strcpy_s(myString, (LPCTSTR)str1);
		for (int j = 0; j < iSize; j++) {
			if (myString[j] >= 'a' && myString[j] <= 'f') {
				iDec *= 16;
				iDec += myString[j] - 'a' + 10;
			}
			else if (myString[j] >= 'A' && myString[j] <= 'F') {
				iDec *= 16;
				iDec += myString[j] - 'A' + 10;
			}
			else if (myString[j] >= '0' && myString[j] <= '9') {
				iDec *= 16;
				iDec += myString[j] - '0';
			}
		}
	}

	iDec = min(65535, max(0, iDec));
	str2.Format("%X", iDec);
	if (str1 != str2) { SetDlgItemText(IDC_ValueHex2, str2); }

	str2.Format("%d", iDec);
	GetDlgItemText(IDC_Value, str1);
	if (str1 != str2) { SetDlgItemText(IDC_Value, str2); }

	for (int i = 0; i < 16; i++) {
		m_bValue[i] = min(iDec & (0x01 << i), TRUE);
	}
	DrawBitControl();
}

void CHeaderLogDlg::OnBnClickedModifyheaderlogitem() {
	// 유효성 검증	
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	int iCondition = m_ctrlCondition.GetCurSel();
	CString strError = "";
	if (IsValid_Input(iCondition, &strError) == FALSE) {
		if (strError != "") {
			AfxMessageBox(strError);
		}
		return;
	}

	// m_iSelectItem에 현재 내용 추가
	GetCurrentControl(m_iSelectItem);
	m_bIsUpdated = TRUE;
}

void CHeaderLogDlg::OnBnClickedAddheaderlogitem() {
	// 유효성 검증	
	int iCondition = m_ctrlCondition.GetCurSel();
	CString strError = "";
	if (IsValid_Input(iCondition, &strError) == FALSE) {
		if (strError != "") {
			AfxMessageBox(strError);
		}
		return;
	}

	if (m_iTextLogCount + 1 >= MAX_HEADER_LOG_COUNT) {
		AfxMessageBox("Item을 추가할 수 없습니다.");
		return;
	}
	
	// 맨아래에 칸 추가
	LV_ITEM lvitem;
	CString str;

	int nCount = m_ctrlHeaderLog.GetItemCount();
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;
	lvitem.iItem = nCount;
	str.Format("%d", nCount);
	lvitem.pszText = (LPSTR)(LPCSTR)str;
	m_ctrlHeaderLog.InsertItem(&lvitem);


	// 추가 위치 결정
	CString strSelect;
	int iAddPos = m_iSelectItem + 1;
	if (m_iSelectItem == -1) iAddPos = nCount;

	// 이동 
	for (int i = nCount - 1; i >= iAddPos; i--) {
		for (int j = 1; j < N_TEXTLOG_ITEM; j++) {
			m_ctrlHeaderLog.SetItemText(i + 1, j, m_ctrlHeaderLog.GetItemText(i, j));
		}
	}

	// iAddPos에 현재 내용 추가
	GetCurrentControl(iAddPos);

	m_iTextLogCount++;
	m_iSelectItem = -1;
	SetDlgItemText(IDD_HeaderLogDlg, "");	
	m_bIsUpdated = TRUE;
	
	//m_ctrlHeaderLog.SetSelectionMark(iAddPos);
	//m_ctrlHeaderLog.SetItemState(iAddPos, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctrlHeaderLog.EnsureVisible(iAddPos, TRUE);
	//m_ctrlHeaderLog.SetFocus();
}

BOOL CHeaderLogDlg::IsAvailableDataID(CString* pstr) {
	int type, id;
	return IsAvailableDataID(pstr, &type, &id);
}

BOOL CHeaderLogDlg::IsAvailableDataID(CString* pstr, int *ptype, int * pid) {
	BOOL ret = TRUE;
	int n = pstr->GetLength();
	int seq = 0;
	BOOL get_data = FALSE;
	int val = 0;
	int type = -1;
	for (int i = 0; i < n; i++) {
		BYTE aByte = pstr->GetAt(i);
		switch (seq) {
		case 0:
			if (aByte == 'd' || aByte == 'D') {
				seq = 10;
				type = 0;
			}
			else if (aByte == 'm' || aByte == 'M') {
				seq = 10;
				type = 1;
			}
			break;

		case 10:
			if (aByte >= '0' && aByte <= '9') {
				val *= 10;
				val += aByte - '0';
			}
			break;
		}
	}

	if ((type == 0 && val >= 1 && val <= MAX_DATACONTROL_SIZE) || (type == 1 && val >= 1 && val <= pMain->m_iMaxMathSize)) {
		*ptype = type;
		*pid = val - 1;
		if (type == 0) {
			pstr->Format("D%d", val);
		}
		else {
			pstr->Format("M%d", val);
		}
		return TRUE;
	}

	AfxMessageBox("[Error] Data ID 입력범위는 D1~16, M1~32입니다.");
	return FALSE;
}

void CHeaderLogDlg::GetCurrentControl(int iSelectItem) {
	int iCondition = m_ctrlCondition.GetCurSel();
	CString s[N_TEXTLOG_ITEM];
	for (int i = 0; i < N_TEXTLOG_ITEM; i++) { s[i] = ""; }

	// Column 0 : Item Number
	// Column 1 : Use Item
	// Column 2 : Log Condition
	// Column 3 : Data ID
	s[0].Format("%d", iSelectItem);

	if (iCondition != eLogCondition_Blank) {
		GetDlgItemText(IDC_DataID, s[3]);
		if (IsAvailableDataID(&s[3]) == FALSE) {
			return;
		}
	}

	if (IsDlgButtonChecked(IDC_UseItem)) s[1] = "Use"; else s[1] = "Unused";

	s[2] = m_strCond[iCondition];

	switch (iCondition) {
		case eLogCondition_Blank:
			GetDlgItemText(IDC_Comment1, s[7]);
			break;

		case eLogCondition_MatchMask:
			GetDlgItemText(IDC_MaskValue, s[4]);
			GetDlgItemText(IDC_Value, s[5]);
			s[6].Format("%d", IsDlgButtonChecked(IDC_ShowComment1));
			GetDlgItemText(IDC_Comment1, s[7]);
			s[8].Format("%d", IsDlgButtonChecked(IDC_ShowComment2));
			GetDlgItemText(IDC_Comment2, s[9]);
			break;

		case eLogCondition_ShowValue:
			s[6].Format("%d", IsDlgButtonChecked(IDC_ShowComment1));
			GetDlgItemText(IDC_Comment1, s[7]);
            s[10].Format("%d", m_ctrlDP.GetCurSel());
            s[11].Format("%d", m_ctrlDivide.GetCurSel());
			break;

		case eLogCondition_MaskedValue:
			GetDlgItemText(IDC_MaskValue, s[4]);
			GetDlgItemText(IDC_Value, s[5]);
			if (atoi(s[5]) == 0) { s[5] = "1"; }
			s[6].Format("%d", IsDlgButtonChecked(IDC_ShowComment1));
			GetDlgItemText(IDC_Comment1, s[7]);
			break;
		
		case eLogCondition_Greater:
		case eLogCondition_Less:
		case eLogCondition_GreaterEqual:
		case eLogCondition_LessEqual:		
			s[5].Format("%s", (LPCTSTR)pMain->Double2Str(atof(s[5])));
			GetDlgItemText(IDC_Value, s[5]);
			GetDlgItemText(IDC_Comment1, s[7]);
			s[8].Format("%d", IsDlgButtonChecked(IDC_ShowComment2));
			GetDlgItemText(IDC_Comment2, s[9]);
            s[10].Format("%d", m_ctrlDP.GetCurSel());
            s[11].Format("%d", m_ctrlDivide.GetCurSel());
			break;
	}

	for (int j = 0; j < N_TEXTLOG_ITEM; j++) {
		m_ctrlHeaderLog.SetItemText(iSelectItem, j, s[j]);
	}
	for (int i = 0; i < m_ctrlHeaderLog.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_ctrlHeaderLog.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}


void CHeaderLogDlg::OnClickItemlist(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_iSelectItem = pNMItemActivate->iItem;

	if (m_iSelectItem != -1) {
		CString str;
		int iCondition = -1;
		SetDlgItemInt(IDC_SelectItemNo, m_iSelectItem);
		if (m_ctrlHeaderLog.GetItemText(m_iSelectItem, 1) == "Use") { CheckDlgButton(IDC_UseItem, TRUE); }
		else { CheckDlgButton(IDC_UseItem, FALSE); }
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, 2);

		for (int i = 0; i < eLogCondition_Last; i++) {
			if (str == m_strCond[i]) {
				iCondition = i;
				break;
			}
		}

		m_ctrlCondition.SetCurSel(iCondition);

		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, 3);
		SetDlgItemText(IDC_DataID, str);
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, 4);
		SetDlgItemText(IDC_MaskValue, str);
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, 5);
		SetDlgItemText(IDC_Value, str);

		if (atoi(m_ctrlHeaderLog.GetItemText(m_iSelectItem, 6)) == 1) { CheckDlgButton(IDC_ShowComment1, TRUE);	} else { CheckDlgButton(IDC_ShowComment1, FALSE); }
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, 7);
		SetDlgItemText(IDC_Comment1, str);

		if (atoi(m_ctrlHeaderLog.GetItemText(m_iSelectItem, 8)) == 1) { CheckDlgButton(IDC_ShowComment2, TRUE); }  else { CheckDlgButton(IDC_ShowComment2, FALSE); }
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, 9);
		SetDlgItemText(IDC_Comment2, str);

        m_ctrlDP.SetCurSel(atoi(m_ctrlHeaderLog.GetItemText(m_iSelectItem, 10)));
        m_ctrlDivide.SetCurSel(atoi(m_ctrlHeaderLog.GetItemText(m_iSelectItem, 11)));

		ShowControl(m_ctrlCondition.GetCurSel());		
	}

	*pResult = 0;
}


void CHeaderLogDlg::ShowBit(int iMode) {
	BOOL bShowMask, bShowValue;
	switch (iMode) {
	case 	eLogCondition_Blank:
		bShowMask = FALSE; bShowValue = FALSE;
		break;

	case 	eLogCondition_MatchMask:
		bShowMask = TRUE; bShowValue = TRUE;
		break;

	case 	eLogCondition_ShowValue:
		bShowMask = FALSE; bShowValue = FALSE;
		break;

	case	eLogCondition_MaskedValue:
		bShowMask = TRUE; bShowValue = FALSE;
		break;

	default:
		bShowMask = FALSE; bShowValue = FALSE;
		break;
	}
	GetDlgItem(IDC_STATIC_bit0)->ShowWindow(bShowMask);
	GetDlgItem(IDC_STATIC_bit1)->ShowWindow(bShowMask);
	GetDlgItem(IDC_STATIC_bit2)->ShowWindow(bShowValue);

	GetDlgItem(IDC_ValueHex)->ShowWindow(bShowMask);
	GetDlgItem(IDC_ValueHex2)->ShowWindow(bShowValue);

	DrawBitControl();
}


void CHeaderLogDlg::OnBnClickedSaveheaderlog() {
	// pMain->SetRegRoot_RegistryData(strAddress, strItem, strData);
	CString strAddress, strItem, strData;
	strAddress.Format("Log\\TextLog\\%c_0x%X", m_cHeader, m_cHeader);
	strItem.Format("nCount");
    m_iTextLogCount = max(0, m_ctrlHeaderLog.GetItemCount());
	if (MAX_HEADER_LOG_COUNT < m_iTextLogCount) { m_iTextLogCount = 0; }
	strData.Format("%d", m_iTextLogCount);
	pMain->SetRegRoot_RegistryData(strAddress, strItem, strData);

	for (int i = 0; i < m_iTextLogCount; i++) {
		strAddress.Format("Log\\TextLog\\%c_0x%X\\Item%d", m_cHeader, m_cHeader, i);

		strData = m_ctrlHeaderLog.GetItemText(i, 1);
		if (strData == "Use") {
			pMain->SetRegRoot_RegistryData(strAddress, "Use", "1");
		}
		else {
			pMain->SetRegRoot_RegistryData(strAddress, "Use", "0");
		}

		strData = m_ctrlHeaderLog.GetItemText(i, 2);
		for (int iCond = 0; iCond < eLogCondition_Last - 1; iCond++) {
			if (strData == m_strCond[iCond]) {
				CString strValue;
				strValue.Format("%d", iCond);
				pMain->SetRegRoot_RegistryData(strAddress, "Condition", strValue);
				break;
			}
		}

		if (strData == "Blank") {
			pMain->SetRegRoot_RegistryData(strAddress, "DataType", "0");
			pMain->SetRegRoot_RegistryData(strAddress, "DataID", "0");
		}
		else {
			int data_type, data_id;
			CString str = m_ctrlHeaderLog.GetItemText(i, 3);
			IsAvailableDataID(&str, &data_type, &data_id);
			str.Format("%d", data_type);
			pMain->SetRegRoot_RegistryData(strAddress, "DataType", str);

			str.Format("%d", data_id);
			pMain->SetRegRoot_RegistryData(strAddress, "DataID", str);
		}
			
		pMain->SetRegRoot_RegistryData(strAddress, "Masking", m_ctrlHeaderLog.GetItemText(i, 4));
		pMain->SetRegRoot_RegistryData(strAddress, "Value", m_ctrlHeaderLog.GetItemText(i, 5));
		pMain->SetRegRoot_RegistryData(strAddress, "Show1", m_ctrlHeaderLog.GetItemText(i, 6));
		pMain->SetRegRoot_RegistryData(strAddress, "Comment1", m_ctrlHeaderLog.GetItemText(i, 7));
		pMain->SetRegRoot_RegistryData(strAddress, "Show2", m_ctrlHeaderLog.GetItemText(i, 8));
		pMain->SetRegRoot_RegistryData(strAddress, "Comment2", m_ctrlHeaderLog.GetItemText(i, 9));

        pMain->SetRegRoot_RegistryData(strAddress, "Digit", m_ctrlHeaderLog.GetItemText(i, 10));
        pMain->SetRegRoot_RegistryData(strAddress, "Divide", m_ctrlHeaderLog.GetItemText(i, 11));
	}	
	OnOK();
}


void CHeaderLogDlg::ShowHeaderLogData(int num, CTextLogSetting* data) {
	LV_ITEM lvitem;
	CString str;

	int nCount = m_ctrlHeaderLog.GetItemCount();
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;
	lvitem.iItem = nCount;
	str.Format("%d", nCount);
	lvitem.pszText = (LPSTR)(LPCSTR)str;
	m_ctrlHeaderLog.InsertItem(&lvitem);

	CString s[N_TEXTLOG_ITEM];
	for (int i = 0; i < N_TEXTLOG_ITEM; i++) { s[i] = ""; }

	s[0].Format("%d", num);

	if (data->use) s[1] = "Use"; else s[1] = "Unused";

	s[2] = m_strCond[data->item_type];
	
	if (data->item_type != eLogCondition_Blank) {
		if (data->data_type == 0) {
			s[3].Format("D%d", data->data_id + 1);
		}
		else {
			s[3].Format("M%d", data->data_id + 1);
		}
	}

	s[7] = data->strComment1;

	switch (data->item_type) {
	case eLogCondition_Blank:		
			break;

	case eLogCondition_MatchMask:		
		s[4].Format("%d", data->mask_val);
		s[5].Format("%d", data->ref_iVal);
		if (data->show1) s[6] = "1"; else s[6] = "0";
		if (data->show2) s[8] = "1"; else s[8] = "0";
		s[9].Format("%s", (LPCTSTR)data->strComment2);
		break;

	case eLogCondition_ShowValue:	
		if (data->show1) s[6] = "1"; else s[6] = "0";
        s[10].Format("%d", data->dp);
        s[11].Format("%d", data->divide);
		break;

	case eLogCondition_MaskedValue:
		s[4].Format("%d", data->mask_val);
		s[5].Format("%d", data->ref_iVal);
		if (data->show1) s[6] = "1"; else s[6] = "0";
		break;

	default:
		s[5].Format("%s", (LPCTSTR)pMain->Double2Str(data->ref_fVal));
		if (data->show2) s[8] = "1"; else s[8] = "0";
		s[9].Format("%s", (LPCTSTR)data->strComment2);
        s[10].Format("%d", data->dp);
        s[11].Format("%d", data->divide);
	}

	for (int j = 0; j < N_TEXTLOG_ITEM; j++) {
		m_ctrlHeaderLog.SetItemText(num, j, s[j]);
	}
}

void CHeaderLogDlg::OnBnClickedExport() {
	CString strAddress, strItem, strData;
    // 현재의 Header정보가져오기
    BYTE cHeader = m_strHeader.GetAt(0);
    if (('a' <= cHeader && cHeader <= 'z') || ('A' <= cHeader && cHeader <= 'Z')) {
        CString strFileName;
        SYSTEMTIME time;
        GetLocalTime(&time);

        CFile SaveFile_Setting;
        CFileException ex;

        CString str = "";
        strFileName.Format("TextLog_Setting_%02d%02d%02d_%02d%02d_%c.dat", time.wYear % 100, time.wMonth, time.wDay, time.wHour, time.wMinute, cHeader);
        SaveFile_Setting.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);


        // Item Count
        strAddress.Format("Log\\TextLog\\%c_0x%X", cHeader, cHeader);        
        strItem = "nCount";
        strData = pMain->GetRegRoot_RegistryData(strAddress, strItem);
        int iItemCount = atoi(strData);        
        str.Format("Header {%c} nItem {%d}\r\n", cHeader, iItemCount);
        
        BOOL bUse, bShow1, bShow2;
        int iCondition = 0, iDataType, iDataID, iMaskValue, iRefValue;
        CString strComment1, strComment2;
        int dp, divide;

        for (int i = 0; i < iItemCount; i++) {
            strAddress.Format("Log\\TextLog\\%c_0x%X\\Item%d", cHeader, cHeader, i);

            strData = pMain->GetRegRoot_RegistryData(strAddress, "Use");
            if (strData == "Use" || strData == "1") bUse = TRUE;  else bUse = FALSE;

            strData = pMain->GetRegRoot_RegistryData(strAddress, "Condition");
            if (strData == "0") {
                iCondition = 0; //eLogCondition_Blank;
            }
            else if (strData == "1") {
                iCondition = 1; // eLogCondition_MatchMask;
            }
            else if (strData == "2") {
                iCondition = 2; // eLogCondition_ShowValue;
            }
            else if (strData == "3") {
                iCondition = 3; // eLogCondition_MaskedValue;
            }
            strData.Format("%s", (LPCTSTR)pMain->GetRegRoot_RegistryData(strAddress, "DataType"));
            iDataType = atoi(strData);
            strData.Format("%s", (LPCTSTR)pMain->GetRegRoot_RegistryData(strAddress, "DataID"));
            if (strData.Find("Data") > 0) { AfxMessageBox("ERRor txt Log save");  };
            iDataID = atoi(strData);
            iMaskValue = atoi(pMain->GetRegRoot_RegistryData(strAddress, "Masking"));
            iRefValue = atoi(pMain->GetRegRoot_RegistryData(strAddress, "Value"));
            bShow1 = atoi(pMain->GetRegRoot_RegistryData(strAddress, "Show1"));
            strComment1.Format("%s", (LPCTSTR)pMain->GetRegRoot_RegistryData(strAddress, "Comment1"));
            bShow2 = atoi(pMain->GetRegRoot_RegistryData(strAddress, "Show2"));
            strComment2.Format("%s", (LPCTSTR)pMain->GetRegRoot_RegistryData(strAddress, "Comment2"));
            dp = atoi(pMain->GetRegRoot_RegistryData(strAddress, "Digit"));
            divide = atoi(pMain->GetRegRoot_RegistryData(strAddress, "Divide"));
            CString strTemp = str;
            str.Format("%s%3d Use(%d) Condition(%d) DataType(%d) DataID(%d) Masking(%d) RefValue(%d) Show1(%d) Comment1 {%s} Show2(%d) Comment2 {%s} Digit {%d} Divide {%d}\r\n", (LPCTSTR)strTemp, i, bUse, iCondition, iDataType, iDataID, iMaskValue, iRefValue, bShow1, (LPCTSTR)strComment1, bShow2, (LPCTSTR)strComment2, dp, divide);
        
            if (str.GetLength() > 20000) {
                SaveFile_Setting.Write(str, str.GetLength());
                str = "";
            }
        }
        SaveFile_Setting.Write(str, str.GetLength());
        SaveFile_Setting.Close();
        AfxMessageBox("설정 파일을 생성하였습니다.");	
	}
	else {
	    AfxMessageBox("설정된 내용이 없어 파일을 만들지 못하였습니다.");
	}
}

void CHeaderLogDlg::OnBnClickedImport() {
    if (MessageBox("현재의 Log 분석 설정 저장 정보가 완전히 지워집니다. 계속 진행할까요?", "설정 파일 불러오기", MB_YESNO) == IDYES)
    {
		CString pathName;
		GetDlgItemText(IDC_ImportPath, pathName);
		while (pathName.Replace(" ", ""));

        CFile OpenFile;
        BOOL bFinish = FALSE;

        if (OpenFile.Open(pathName, CFile::modeRead | CFile::shareDenyNone))
        {
            long size = (long)(OpenFile.GetLength());
            BOOL bConti = TRUE;
            if (size > 0) {
                BYTE* pBuffer = new BYTE[(UINT)size + 1];
                OpenFile.Read(pBuffer, (UINT)size);
                OpenFile.Close();
                pBuffer[size] = 0;
                pMain->m_iBufferIndex = 0, pMain->m_iBufferSize = size;

                // Header Count 
                pMain->GetHeaderLogFromBuffer(pBuffer, &bConti);

                delete[] pBuffer;
            }
            if (bConti)
            {
                GetHeaderLog();
                AfxMessageBox("설정 파일 적용 완료");
            }
            else
            {
                AfxMessageBox("설정 파일 파싱 중 오류가 발생하였습니다.");
            }
        }
		else {
			AfxMessageBox("설정 파일 여는 중 오류가 발생하였습니다.");
		}
    }
}


void CHeaderLogDlg::OnBnClickedDefault() {
	pMain->LoadDefaultHeaderLog();
	GetHeaderLog();
	m_bIsUpdated = FALSE;
}


void CHeaderLogDlg::OnBnClickedDeleteall() {
	if (AfxMessageBox("현재 작성한 내용이 지워집니다. 계속할까요?", MB_YESNO) == IDYES){
		m_ctrlHeaderLog.DeleteAllItems();
		m_iTextLogCount = 0;
	}
}


void CHeaderLogDlg::OnPaint() {
	CPaintDC dc(this); 
	DrawBitControl();
}

void CHeaderLogDlg::DrawBitControl() {
	static int iPreCondition = -1;
	int iCurrentCondition = m_ctrlCondition.GetCurSel();
	if (iCurrentCondition != eLogCondition_MatchMask) {
		if (iPreCondition != iCurrentCondition) {			
			Invalidate();
			iPreCondition = iCurrentCondition;
		}
		if (m_ctrlCondition.GetCurSel() != eLogCondition_MaskedValue) {
			return;
		}
	}
	CClientDC dc(this);

	int x0 = 123; 
	int y0 = 413;
	int iLength = 12;
	int iDx = 5;
	int iDy = 24;

	dc.SelectObject(&(pMain->m_BrushSelectControl_Blank[1]));
	dc.SelectObject(&(pMain->m_PenSelectControl_Blank[1]));
	for (int i = 0; i < 16; i++) {
		int nBlock = i / 4;
		int x = x0 + i * (iLength+1) + nBlock * iDx;
		int y = y0;
		dc.Rectangle(x, y, x + iLength, y + iLength);
	}
	
	if (iCurrentCondition == eLogCondition_MatchMask) {
		for (int i = 0; i < 16; i++) {
			int nBlock = i / 4;
			int x = x0 + i * (iLength + 1) + nBlock * iDx;
			int y = y0 + iDy;
			dc.Rectangle(x, y, x + iLength, y + iLength);
		}
	}

	dc.SelectObject(&(pMain->m_BrushSelectControl_Select[1]));
	dc.SelectObject(&(pMain->m_PenSelectControl_Select[1]));
	for (int i = 0; i < 16; i++) {
		if (m_bMask[15-i]) {
			int nBlock = i / 4;
			int x = x0 + i * (iLength + 1) + nBlock * iDx + 2;
			int y = y0 + 2;
			dc.Rectangle(x, y, x + iLength - 4, y + iLength - 4);
		}
	}

	if (iCurrentCondition == eLogCondition_MatchMask) {
		for (int i = 0; i < 16; i++) {
			if (m_bValue[15-i]) {
				int nBlock = i / 4;
				int x = x0 + i * (iLength + 1) + nBlock * iDx + 2;
				int y = y0 + 2 + iDy;
				dc.Rectangle(x, y, x + iLength - 4, y + iLength - 4);
			}
		}
	}
}

void CHeaderLogDlg::OnBnClickedMoveup() {
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	if (m_iSelectItem == 0) {
		AfxMessageBox("현재 가장 위까지 이동 하였습니다..");
		return;
	}

	// 현재위치와 위쪽 위치를 Swap
	CString str;
	for (int i = 0; i < N_TEXTLOG_ITEM; i++) {
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, i);
		m_ctrlHeaderLog.SetItemText(m_iSelectItem, i, m_ctrlHeaderLog.GetItemText(m_iSelectItem-1, i));
		m_ctrlHeaderLog.SetItemText(m_iSelectItem - 1, i, str);
	}
	m_iSelectItem--;
	SetDlgItemInt(m_iSelectItem, m_iSelectItem);
}


void CHeaderLogDlg::OnBnClickedMovedn()
{
	if (m_iSelectItem == -1) {
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	if (m_iSelectItem == m_iTextLogCount - 1) {
		AfxMessageBox("현재 가장 아래까지 이동 하였습니다..");
		return;
	}

	// 현재위치와 아래쪽 위치를 Swap
	CString str;
	for (int i = 0; i < N_TEXTLOG_ITEM; i++) {
		str = m_ctrlHeaderLog.GetItemText(m_iSelectItem, i);
		m_ctrlHeaderLog.SetItemText(m_iSelectItem, i, m_ctrlHeaderLog.GetItemText(m_iSelectItem + 1, i));
		m_ctrlHeaderLog.SetItemText(m_iSelectItem + 1, i, str);
	}
	m_iSelectItem++;
	SetDlgItemInt(m_iSelectItem, m_iSelectItem);
}
