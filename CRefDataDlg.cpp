// CRefDataDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "CRefDataDlg.h"
#include "afxdialogex.h"
#include <memory>


// CRefDataDlg 대화 상자

IMPLEMENT_DYNAMIC(CRefDataDlg, CDialogEx)

CRefDataDlg::CRefDataDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RefData, pParent)
	, m_iSourceMode(FALSE)
	, m_iTargetMode(FALSE)
	, m_iSelectData(-1)
	, m_iSource(0)
	, m_iTarget(0)
{

}

CRefDataDlg::~CRefDataDlg()
{
}

void CRefDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ListRef, m_RefList);
	DDX_Radio(pDX, IDC_SourceMode0, m_iSourceMode);
	DDX_Control(pDX, IDC_ListRef, m_RefList);
	DDX_Radio(pDX, IDC_TargetMode0, m_iTargetMode);
	DDX_Radio(pDX, IDC_SourceType0, m_iSource);
	DDX_Radio(pDX, IDC_TargetType0, m_iTarget);
}


BEGIN_MESSAGE_MAP(CRefDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SourceMode0, &CRefDataDlg::ChangeSourceMode)
	ON_BN_CLICKED(IDC_SourceMode1, &CRefDataDlg::ChangeSourceMode)
	ON_BN_CLICKED(IDC_TargetMode0, &CRefDataDlg::ChangeTargetMode)
	ON_BN_CLICKED(IDC_TargetMode1, &CRefDataDlg::ChangeTargetMode)
	ON_BN_CLICKED(IDC_ReqSaveRefData, &CRefDataDlg::OnBnClickedReqsave)
	ON_NOTIFY(NM_CLICK, IDC_ListRef, &CRefDataDlg::SelectRefDataItem)
	ON_BN_CLICKED(IDC_Run, &CRefDataDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_Type1, &CRefDataDlg::OnBnClickedType1)
	ON_BN_CLICKED(IDC_Type0, &CRefDataDlg::OnBnClickedType0)
	ON_EN_CHANGE(IDC_SaveRange0, &CRefDataDlg::OnEnChangeSaverange0)
	ON_EN_CHANGE(IDC_SaveRange1, &CRefDataDlg::OnEnChangeSaverange1)
	ON_BN_CLICKED(IDC_TargetType0, &CRefDataDlg::OnBnClickedTargettype0)
	ON_BN_CLICKED(IDC_TargetType1, &CRefDataDlg::OnBnClickedTargettype1)
END_MESSAGE_MAP()


// CRefDataDlg 메시지 처리기
void CRefDataDlg::AddColumn() {
	LV_COLUMN lvcolumn;
	char *listColumn[6] = { "No.", "Save Time", "Header", "n_Data", "Description", " " };
	int width[6] = { 1, 1, 1, 1, 1, 1 };
	int i;

	for (i = 0; i < 6; i++) {
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.pszText = listColumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		m_RefList.InsertColumn(i, &lvcolumn);
	}

	m_RefList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	LV_ITEM lvitem;
	for (int i = 0; i < 20; i++) {
		CString str, strAddress, strItem, strValue;
		int nCount = m_RefList.GetItemCount();
		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;
		lvitem.iItem = nCount;
		str.Format(" %d", lvitem.iItem + 1);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_RefList.InsertItem(&lvitem);	
	}

	m_iReferenceDataCount = atoi(pMain->GetRegRoot_RegistryData("Common\\Ref_Data", "Count"));

	for (int i = 0; i < m_iReferenceDataCount; i++) {
		CString strAddress;
		strAddress.Format("Common\\Ref_Data\\Save%d", i);
		m_RefList.SetItemText(i, 1, pMain->GetRegRoot_RegistryData(strAddress, "SaveTime"));
		m_RefList.SetItemText(i, 1, pMain->GetRegRoot_RegistryData(strAddress, "Header"));
		m_RefList.SetItemText(i, 1, pMain->GetRegRoot_RegistryData(strAddress, "nData"));
		m_RefList.SetItemText(i, 1, pMain->GetRegRoot_RegistryData(strAddress, "Description"));
	}

	for (int i = m_iReferenceDataCount; i < 20; i++) {
		for (int j = 1; j <= 4; j++) {
			m_RefList.SetItemText(i, j, "  -  ");
		}
	}

	for (i = 0; i < m_RefList.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_RefList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}

BOOL CRefDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AddColumn();

	int iStart = max(pMain->m_iHistoryIndex - pMain->m_iDisplaySize + 1 - pMain->m_iHistoryMove, 0);
	int iEnd = min(iStart + pMain->m_iDisplaySize, pMain->m_iHistoryIndex);
	SetDlgItemInt(IDC_SaveRange0, iStart);
	SetDlgItemInt(IDC_SaveRange1, iEnd);

	GetDlgItem(IDC_SourceType0)->EnableWindow(FALSE);
	GetDlgItem(IDC_SourceType1)->EnableWindow(FALSE);
	GetDlgItem(IDC_SourceType2)->EnableWindow(FALSE);
	CheckDlgButton(IDC_SourceType0, TRUE);
	CheckMemory();
	UDT;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRefDataDlg::ChangeSourceMode() { 
	UDT;
	if (m_iSourceMode == 1) {
		GetDlgItem(IDC_TargetMode1)->EnableWindow(FALSE);
		CheckDlgButton(IDC_TargetMode0, TRUE);
		CheckDlgButton(IDC_TargetMode1, FALSE);
	}
	else {
		GetDlgItem(IDC_TargetMode1)->EnableWindow(TRUE);
	}
}

void CRefDataDlg::ChangeTargetMode(){
	UDT;
}


void CRefDataDlg::OnBnClickedReqsave() {
	SaveRefData();
}


void CRefDataDlg::SelectRefDataItem(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_iSelectData = pNMItemActivate->iItem;
	*pResult = 0;
}

void CRefDataDlg::SaveRefData(void) {
	CString str;
	if (m_iSelectData == -1) {
		AfxMessageBox("저장 공간을 선택하지 않았습니다.");
		return;
	}

	int iHistoryIndex = pMain->m_iHistoryIndex;
	int iHistoryMove = pMain->m_iHistoryMove;
	int iDisplaySize = pMain->m_iDisplaySize;
    auto sourceData = std::make_unique<RefData_t>();
	//RefData_t sourceData;

	if (m_iSourceMode == 0) {
		int iStart = iHistoryIndex - iHistoryMove - iDisplaySize + 1;
		int iEnd = min(iStart + iDisplaySize - 1, iHistoryIndex);
		iStart = min(iHistoryIndex, max(0, iStart));

		sourceData->Header = pMain->m_cPacketHead;

		sourceData->nData = iEnd - iStart + 1;
		if (sourceData->nData > 10000) {
			sourceData->nData = 10000;
			iEnd = min(iHistoryIndex, iStart + 10000 - 1);
		}

		for (int i = iStart; i <= iEnd; i++) {
			int index = i - iStart;
			for (int j = 0; j < pMain->m_iMaxDataSize; j++) {
				sourceData->DataValue[index][j] = pMain->m_HistoryData[i].fDataValue[j];
			}			
		}		
		for (int j = 0; j < pMain->m_iMaxDataSize; j++) {
			sourceData->strDataName[j].Format("[REF]%s", (LPCTSTR)pMain->m_strPinName[j]);
		}

		for (int j = 0; j < pMain->m_iMaxMathSize; j++) {
			sourceData->bUseMath[j] = FALSE;
			if (pMain->m_MathData[j].bValid) {
				sourceData->bUseMath[j] = TRUE;
				sourceData->strMathName[j].Format("[REF]%s", (LPCTSTR)pMain->m_MathData[j].strName);
				for (int i = iStart; i <= iEnd; i++) {
					int index = i - iStart;
					sourceData->MathValue[index][j] = pMain->m_HistoryData[i].fMathValue[j];
				}
			}
		}

		GetDlgItemText(IDC_RefDataDescript, sourceData->strRemark);
		SYSTEMTIME t; 	
		GetLocalTime(&t);
		sourceData->strTime.Format("%02d%02d%02d_%02d%02d%02d.%03d", t.wYear%100, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	}
	else {
		// 저장된 파일에서 Loading
	}
	
	// Registry에 저장 (공통)
	{

	}

	// Mode == 1일때만 파일로 저장
	if (m_iTargetMode == 1) {

	}
}

void CRefDataDlg::OnBnClickedRun()
{
	UDT;

	int iStart, iEnd, iSaved;
	CString strTemp = "";
	CString strItemText;
	CString str;
	
	int a = (int)GetDlgItemInt(IDC_SaveRange0);
	int b = (int)GetDlgItemInt(IDC_SaveRange1);

	if (a < b) {
		iEnd = b;
		iStart = a;
	}
	else {
		iStart = b;
		iEnd = a;
	}
	iSaved = iEnd;

	if (m_iTarget == 0) {
		// 선택된 항목의 인덱스를 사용하여 항목의 텍스트를 가져옵니다.
		

		for (int i = iStart; i < iEnd; i++)	{
			int iLogIdx = pMain->m_HistoryData[i].iHistoryLogIndex;
            CString strTemp2 = strTemp;
			strTemp.Format("%s\r\n%s", (LPCTSTR)strTemp2, (LPCTSTR)pMain->m_arrLogData[iLogIdx]);
		}

		const char* convertMsg;
		CT2A ascii(strTemp, CP_UTF8);
		convertMsg = ascii.m_psz;
		pMain->CopyTextToClipboard(convertMsg);
				
		str.Format("Data index %d ~ %d 가 복사되었습니다.(n:%d)", iStart, iEnd - 1, iEnd - iStart);		
		AfxMessageBox(str);
	}
	else {
		CFile SaveRcvData;
		CFileException ex;

		if (SaveRcvData.m_hFile == INVALID_HANDLE_VALUE) {
			CString strFileName;
			SYSTEMTIME time;
			GetLocalTime(&time);

			strFileName.Format("Data_%02d%02d%02d_%02d%02d%02d.csv", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			SaveRcvData.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone,&ex);
			for (int i = iStart; i <= iEnd; i++) {
				int iLogIdx = pMain->m_HistoryData[i].iHistoryLogIndex;
                CString strTemp1 = strTemp;
				strTemp.Format("%s%s\r\n", (LPCTSTR)strTemp1, (LPCTSTR)pMain->m_arrLogData[iLogIdx]);
				if (strTemp.GetLength() > 60000) {
					SaveRcvData.Write(strTemp, strTemp.GetLength());
					strTemp = "";
				}
			}
			if (strTemp.GetLength() > 0) {
				SaveRcvData.Write(strTemp, strTemp.GetLength());
				strTemp = "";
			}
			SaveRcvData.Close();
			str.Format("%s 를 저장합니다.", (LPCTSTR)strFileName);
			AfxMessageBox(str);
		}
	}
	OnOK();
}


void CRefDataDlg::OnBnClickedType1()
{
	int iStart = 0;
	int iEnd = pMain->m_iHistoryIndex - 1;
	SetDlgItemInt(IDC_SaveRange0, iStart);
	SetDlgItemInt(IDC_SaveRange1, iEnd);
	CheckDlgButton(IDC_SourceType0, FALSE);
	CheckDlgButton(IDC_SourceType1, TRUE);	
	CheckDlgButton(IDC_SourceType2, FALSE);
}


void CRefDataDlg::OnBnClickedType0()
{
	int iStart = max(pMain->m_iHistoryIndex - pMain->m_iDisplaySize + 1 - pMain->m_iHistoryMove, 0);
	int iEnd = min(iStart + pMain->m_iDisplaySize, pMain->m_iHistoryIndex - 1);
	SetDlgItemInt(IDC_SaveRange0, iStart);
	SetDlgItemInt(IDC_SaveRange1, iEnd);
	CheckDlgButton(IDC_SourceType0, TRUE);
	CheckDlgButton(IDC_SourceType1, FALSE);
	CheckDlgButton(IDC_SourceType2, FALSE);
}


void CRefDataDlg::OnEnChangeSaverange0()
{
	CheckDlgButton(IDC_SourceType0, FALSE);
	CheckDlgButton(IDC_SourceType1, FALSE);
	CheckDlgButton(IDC_SourceType2, TRUE);

	int val1 = GetDlgItemInt(IDC_SaveRange0);
	int val2 = max(0, min(val1, pMain->m_iHistoryIndex - 1));
	CString s1, s2;
	GetDlgItemText(IDC_SaveRange0, s1);
	s2.Format("%d", val2);
	if (s1 != s2) {
		SetDlgItemText(IDC_SaveRange0, s2);
	}
	CheckMemory();
}


void CRefDataDlg::OnEnChangeSaverange1() {
	CheckDlgButton(IDC_SourceType0, FALSE);
	CheckDlgButton(IDC_SourceType1, FALSE);
	CheckDlgButton(IDC_SourceType2, TRUE);

	int val1 = GetDlgItemInt(IDC_SaveRange1);
	int val2 = max(0, min(val1, pMain->m_iHistoryIndex - 1));
	CString s1, s2;
	GetDlgItemText(IDC_SaveRange1, s1);
	s2.Format("%d", val2);
	if (s1 != s2) {
		SetDlgItemText(IDC_SaveRange1, s2);
	}
	CheckMemory();
}

void CRefDataDlg::CheckMemory() {
	CString str = "";
	int iEnd, iStart, iSaved;
	int a = (int)GetDlgItemInt(IDC_SaveRange0);
	int b = (int)GetDlgItemInt(IDC_SaveRange1);

	if (a < b) {
		iEnd = b;
		iStart = a;
	}
	else {
		iStart = b;
		iEnd = a;
	}
	iSaved = iEnd;
	if (m_iTarget == 0) {
		CString strItemText, strTemp = "";
		for (int i = iStart; i < iEnd; i++)	{
			int iLogIdx = pMain->m_HistoryData[i].iHistoryLogIndex;
			strItemText = pMain->m_ctrlLogList.GetItemText(iLogIdx, 0);
			// 선택된 항목의 텍스트를 사용하여 필요한 작업을 수행합니다.
            CString strTemp2 = strTemp;
			strTemp.Format("%s\r\n%s", (LPCTSTR)strTemp2, (LPCTSTR)strItemText);
			if (strTemp.GetLength() > 60000) {
				iSaved = i;
				break;
			}
		}
		if (iEnd == iSaved) {
			str.Format("Data index %d ~ %d 가 복사 예정 (n:%d)", iStart, iEnd, iEnd - iStart + 1);
		}
		else {
			str.Format("Data %d ~ %d(n:%d) 중 %d ~ %d (n:%d)까지만 복사", iStart, iEnd, iEnd - iStart + 1, iStart, iSaved, iSaved - iStart + 1);
		}		
	}
	else {
		str.Format("Data index %d ~ %d 가 복사 예정 (n:%d)", iStart, iEnd, iEnd - iStart + 1);
	}
	SetDlgItemText(IDC_Message, str);
}


void CRefDataDlg::OnBnClickedTargettype0() {
	UDT;
	CheckMemory();
}


void CRefDataDlg::OnBnClickedTargettype1() {
	UDT;
	CheckMemory();
}
