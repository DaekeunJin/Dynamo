// 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "StatisticsDlg.h"
#include "afxdialogex.h"
#include "SerialComDlg.h"

// PortSetting 대화 상자

IMPLEMENT_DYNAMIC(CStatisticsDlg, CDialogEx)

CStatisticsDlg::CStatisticsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_StatisticsDlg, pParent)
	, m_strRmsStartIndex(_T(""))
	, m_strStartTime(_T("0"))
	, m_strEndTime(_T("0"))
	, m_strMessage(_T(""))
	, m_iSelecIndexMode(0)
	, m_iResultGraph_MaxX(860)
	, m_iResultGraph_MaxY(733)
	, m_bClearVariable(TRUE)
	, m_bSaveResult(TRUE)
	, m_bSaveRawData(FALSE)
	, m_bMathResult(FALSE)
	, m_iSelectList(-1)
	, m_strSpanTime(_T(""))
{

}

CStatisticsDlg::~CStatisticsDlg()
{
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RMS_StartIndex, m_strRmsStartIndex);
	DDX_Text(pDX, IDC_RMS_EndIndex, m_strRmsEndIndex);
	DDX_Control(pDX, IDC_ResultList, m_ResultList);
	DDX_Text(pDX, IDC_StrartTime, m_strStartTime);
	DDX_Text(pDX, IDC_EndTime, m_strEndTime);
	DDX_Text(pDX, IDC_Message, m_strMessage);
	DDX_Radio(pDX, IDC_SelectIndexMode, m_iSelecIndexMode);
	DDX_Check(pDX, IDC_SaveResult, m_bSaveResult);
	DDX_Check(pDX, IDC_SaveRawData, m_bSaveRawData);
	DDX_Check(pDX, IDC_SaveMathResult, m_bMathResult);
	DDX_Text(pDX, IDC_SpanTime, m_strSpanTime);
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStatisticsDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ResultList, &CStatisticsDlg::OnColumnclickResultlist)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_MOUSEHWHEEL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SelectIndexMode, &CStatisticsDlg::OnBnClickedSelectindexmode)
	ON_BN_CLICKED(IDC_SelectIndexMode2, &CStatisticsDlg::OnBnClickedSelectindexmode)
	ON_BN_CLICKED(IDC_SelectIndexMode3, &CStatisticsDlg::OnBnClickedSelectindexmode)
	ON_BN_CLICKED(IDC_LoadDefaultIndex, &CStatisticsDlg::OnBnClickedLoaddefaultindex)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_SaveData, &CStatisticsDlg::OnBnClickedSavedata)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SaveResult, &CStatisticsDlg::UpdateControlData)
	ON_BN_CLICKED(IDC_SaveRawData, &CStatisticsDlg::UpdateControlData)
	ON_BN_CLICKED(IDC_SaveMathResult, &CStatisticsDlg::OnBnClickedSavemathresult)
	ON_NOTIFY(NM_CLICK, IDC_ResultList, &CStatisticsDlg::OnClickResultlist)
	ON_BN_CLICKED(IDC_DrawFFT, &CStatisticsDlg::OnBnClickedDrawfft)
	ON_BN_CLICKED(IDC_SaveData2, &CStatisticsDlg::OnBnClickedSavedata2)
	ON_BN_CLICKED(IDC_SaveData3, &CStatisticsDlg::OnBnClickedSavedata3)
END_MESSAGE_MAP()


// PortSetting 메시지 처리기


void CStatisticsDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CStatisticsDlg::OnColumnclickResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;	*/

}

void CStatisticsDlg::AddColumn() {
	LV_COLUMN lvcolumn;
	char *listColumn[8] = { "Data #", "Pin Name", "RMS", "Min Value", "Mean", "MaxValue", "Stdev", " " };
	int width[8] = { 1, 1, 1, 1, 1, 1, 1 , 1};
	int i;

	for (i = 0; i < 8; i++)
	{
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.pszText = listColumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		m_ResultList.InsertColumn(i, &lvcolumn);
	}

	m_ResultList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (i = 0; i < m_ResultList.GetHeaderCtrl()->GetItemCount(); i++)
		VERIFY(m_ResultList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));

	LV_ITEM lvitem;
	CString str, strAddress, strItem, strValue;

	m_ResultList.DeleteAllItems();
	for (int i = 0; i < 16; i++) {
		int nCount = m_ResultList.GetItemCount();
		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;

		lvitem.iItem = nCount;
		str.Format("Data%02d", i+1);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_ResultList.InsertItem(&lvitem);

		m_ResultList.SetItemText(nCount, 1, pMain->m_strPinName[i]);
		for (int i = 0; i < m_ResultList.GetHeaderCtrl()->GetItemCount(); i++)
			VERIFY(m_ResultList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}
}

CString CStatisticsDlg::Dbl2Str(double fValue) {
	return pMain->Double2Str(fValue);
}

BOOL CStatisticsDlg::OnBnClickedCalculation()
{
	UDT;

	CString str;
	// Index Check
	int iStart = min(pMain->m_iHistoryIndex-1, max(0, atoi(m_strRmsStartIndex)));
	int iEnd = min(pMain->m_iHistoryIndex - 1, max(0, atoi(m_strRmsEndIndex)));

	int iMax = m_iHistoryIndex;

	if (iStart > iEnd) {
		int iTemp;
		iTemp = iEnd;
		iEnd = iStart;
		iStart = iTemp;
	}

	if (iStart == iEnd) {
		m_strMessage.Format("Error : 시작과 끝 인덱스가 같아 처리할 수 없습니다(nData = 1)");
		UDF;
	//	return;
	}

	if (iStart < 1) {
		SetTimer(10, 100, NULL);
		pMain->m_iAdjustSelectPoint[0] = pMain->m_iDrawGraphRangeX[0];
		pMain->m_iAdjustSelectPoint[1] = pMain->m_iDrawGraphRangeX[1];
		return FALSE;
	}

	if (iEnd > iMax) {
	//	m_strMessage.Format("Error : 인덱스 값은 저장된 Data갯수(%d)보다 작아야합니다.", iMax);
	//	AfxMessageBox(m_strMessage);
	//	UDF;
		pMain->m_iAdjustSelectPoint[0] = pMain->m_iDrawGraphRangeX[0];
		pMain->m_iAdjustSelectPoint[1] = pMain->m_iDrawGraphRangeX[1];
		SetTimer(10, 100, NULL);
		return FALSE;
	}

	int nCount = -1;
	LV_ITEM lvitem;
	m_ResultList.DeleteAllItems();

	for (int iData = 0; iData < MAX_DATACONTROL_SIZE; iData++) {
		++nCount;

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;

		lvitem.iItem = nCount;
		str.Format("Data%02d", iData + 1);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_ResultList.InsertItem(&lvitem);
		m_ResultList.SetItemText(nCount, 1, pMain->m_strPinName[iData]);
			   		 	  
		double fSquareSum = 0.;
		double fSum = 0.;
		long iCount = 0;
		double fMaxValue = pMain->m_HistoryData[iStart].fDataValue[iData];
		double fMinValue = pMain->m_HistoryData[iStart].fDataValue[iData];

		for (int i = iStart; i <= iEnd; i++) {
			iCount++;
			double fValue = pMain->m_HistoryData[i].fDataValue[iData];
			fSum += fValue;
			fSquareSum += fValue * fValue;
			if (fMaxValue < fValue) fMaxValue = fValue;
			if (fMinValue > fValue) fMinValue = fValue;
		}

		double fAverage = fSum / iCount;
		double fSSE = 0.;
		for (int i = iStart; i <= iEnd; i++) {
			double fValue = pMain->m_HistoryData[i].fDataValue[iData];
			fSSE += (fAverage - fValue) * (fAverage - fValue);
		}

		double fRMS = sqrt(fSquareSum / iCount);
		double fStdev = sqrt(fSSE / iCount);

		str.Format(" %.1f ", fAverage);
		m_ResultList.SetItemText(nCount, 4, str);

		str.Format(" %s ", (LPCTSTR)Dbl2Str(fMinValue));
		m_ResultList.SetItemText(nCount, 3, str);
		str.Format(" %s ", (LPCTSTR)Dbl2Str(fMaxValue));
		m_ResultList.SetItemText(nCount, 5, str);

		str.Format(" %.1f ", fStdev);
		m_ResultList.SetItemText(nCount, 6, str);
		str.Format(" %.1f ", fRMS);
		m_ResultList.SetItemText(nCount, 2, str);

	}

	// Math data
	for (int iData = 0; iData < pMain->m_iMaxMathSize; iData++) {
		++nCount;

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;

		lvitem.iItem = nCount;
		str.Format("Math%02d", iData + 1);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_ResultList.InsertItem(&lvitem);
		m_ResultList.SetItemText(nCount, 1, pMain->m_MathData[iData].strName);

		if (pMain->m_MathData[iData].bValid) {
			double fSquareSum = 0.;
			double fSum = 0.;
			long iCount = 0;
			int iMaxValue = (int)pMain->m_HistoryData[iStart].fMathValue[iData];
			int iMinValue = (int)pMain->m_HistoryData[iStart].fMathValue[iData];

			for (int i = iStart; i <= iEnd; i++) {
				iCount++;
				long iValue = (long)pMain->m_HistoryData[i].fMathValue[iData];
				fSum += iValue;
				fSquareSum += iValue * iValue;
				if (iMaxValue < iValue) iMaxValue = iValue;
				if (iMinValue > iValue) iMinValue = iValue;
			}

			double fAverage = fSum / iCount;
			double fSSE = 0.;
			for (int i = iStart; i <= iEnd; i++) {
				long iValue = (long)pMain->m_HistoryData[i].fMathValue[iData];
				fSSE += (fAverage - iValue) * (fAverage - iValue);
			}

			double fRMS = sqrt(fSquareSum / iCount);
			double fStdev = sqrt(fSSE / iCount);

			str.Format(" %.1f ", fAverage);
			m_ResultList.SetItemText(nCount, 4, str);

			str.Format(" %d ", iMinValue);
			m_ResultList.SetItemText(nCount, 3, str);
			str.Format(" %d ", iMaxValue);
			m_ResultList.SetItemText(nCount, 5, str);

			str.Format(" %.1f ", fStdev);
			m_ResultList.SetItemText(nCount, 6, str);
			str.Format(" %.1f ", fRMS);
			m_ResultList.SetItemText(nCount, 2, str);
		}
	}

	// Delete Invalid Item
	for (int iChannel = pMain->m_iMaxMathSize -1; iChannel >= 0; iChannel--) {
		if (!pMain->m_MathData[iChannel].bValid)
			m_ResultList.DeleteItem(iChannel + MAX_DATACONTROL_SIZE);
	}
	for (int i = 0; i < m_ResultList.GetHeaderCtrl()->GetItemCount(); i++)
		VERIFY(m_ResultList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));

	UDF;
	return TRUE;
}


BOOL CStatisticsDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	m_iHistoryIndex = pMain->m_iHistoryIndex;
	AddColumn();
	OnBnClickedLoaddefaultindex();	
	SetTimer(1, 50, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CStatisticsDlg::OnEnChangeRmsStartindex() {
	UDT;

	CString str;
	// Index Check
	int iStart = atoi(m_strRmsStartIndex);
	if (m_strRmsStartIndex == "") iStart = -1;
	int iEnd = min(atoi(m_strRmsEndIndex), m_iHistoryIndex - 1);
	if (m_strRmsEndIndex == "") iEnd = -1;
	int iMax = m_iHistoryIndex;

	BOOL bErrorStartindex = FALSE;
	BOOL bErrorEndindex = FALSE;

	if (iStart < 1) bErrorStartindex = TRUE;
	if (iStart > iMax) bErrorStartindex = TRUE;
	if (iEnd < 1) bErrorEndindex = TRUE;
	if (iEnd > iMax) bErrorEndindex = TRUE;

	int iSH, iSM, iSS, iSMS, iEH, iEM, iES, iEMS;
	BOOL bErrorTime = FALSE;
	if (!bErrorStartindex) {
		int time = pMain->m_HistoryData[iStart].iRcvTime;
		iSH = time / 3600;   time %= 3600;
		iSM = time / 60;
		iSS = time % 60;
		iSMS = pMain->m_HistoryData[iStart].iRcvTime_ms;

		m_strStartTime.Format("(%02d:%02d:%02d.%03d)", iSH, iSM, iSS, iSMS);
	}
	else {
		m_strStartTime.Format("Index Error");
		bErrorTime = TRUE;
	}

	if (!bErrorEndindex) {
		int time = pMain->m_HistoryData[iEnd].iRcvTime;
		iEH = time / 3600;   time %= 3600;
		iEM = time / 60;
		iES = time % 60;
		iEMS = pMain->m_HistoryData[iEnd].iRcvTime_ms;

		m_strEndTime.Format("(%02d:%02d:%02d.%03d)      N:%d", iEH, iEM, iES, iEMS, m_iEndIndex - m_iStartIndex+1);
	}
	else {
		m_strEndTime.Format("Index Error");
		bErrorTime = TRUE;
	}
	if (!bErrorTime) {
		if (iEMS < iSMS) {iEMS += 1000; iES--;}
		if (iES < iSS) { iES += 60; iEM--; }
		if (iEM < iSM) { iEM += 60; iEH--; }
		if (iEH < iSH) { iEH += 24; }
		long TotalInterval = (iEH - iSH) * 3600000 + (iEM - iSM) * 60000 + (iES - iSS) * 1000 + iEMS - iSMS;
		long AvrInterval = (TotalInterval + (m_iEndIndex - m_iStartIndex + 1) /2) / (m_iEndIndex - m_iStartIndex + 1);
		int iAMS = AvrInterval % 1000;
		int iAS = AvrInterval / 1000;
		m_iSamplingTime = AvrInterval;
		m_strSpanTime.Format("Time interval  %02d:%02d:%02d.%03d     (Avr:%02d.%03d)", iEH - iSH, iEM - iSM, iES - iSS, iEMS - iSMS, iAS, iAMS);

	}
	UDF;

	return OnBnClickedCalculation();
}

void CStatisticsDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent) {
	case 1: OnBnClickedLoaddefaultindex(); break;
	case 10: KillTimer(10); OnBnClickedLoaddefaultindex(); break;
	}	
}


void CStatisticsDlg::OnBnClickedSelectindexmode()
{
	UDT;
}


void CStatisticsDlg::OnBnClickedLoaddefaultindex()
{
	// Load Default Graph Limit
	static int iStartIndex = -1;
	static int iEndIndex = -1;

	if (m_bClearVariable) {
		m_bClearVariable = FALSE;
		iStartIndex = -1;
		iEndIndex = -1;
	}
	
	m_iHistoryIndex = pMain->m_iHistoryIndex;
	int iStartIdx = m_iHistoryIndex - pMain->m_iDisplaySize - pMain->m_iHistoryMove + 1;
	int iEndIdx = max(0, min(iStartIdx + pMain->m_iDisplaySize - 1, m_iHistoryIndex));


	int iStartPt = pMain->m_iDrawGraphRangeX[0];
	int iSpanPt = max(pMain->m_iDrawGraphRangeX[1] - pMain->m_iDrawGraphRangeX[0], 1);
	
	long m_iDisplaySize = pMain->m_iDisplaySize;

	int iDistIndex = iSpanPt / m_iDisplaySize / 2;

	int iStart = max(1, min(m_iHistoryIndex, iStartIdx + m_iDisplaySize * (pMain->m_iAdjustSelectPoint[0] - iStartPt + iDistIndex) / iSpanPt));
	int iEnd = min(m_iHistoryIndex, iStartIdx + m_iDisplaySize * (pMain->m_iAdjustSelectPoint[1] - iStartPt + iDistIndex) / iSpanPt);


	if (iStartIndex != iStart || iEndIndex != iEnd) {
		iStartIndex = iStart;
		iEndIndex = iEnd;
		m_strRmsEndIndex.Format("%d", iEnd);
		m_strRmsStartIndex.Format("%d", iStart);
		UDF;
		m_iStartIndex = iStart;
		m_iEndIndex = iEnd;

		if (!OnEnChangeRmsStartindex()) {
			iStartIndex = -1;
			iEndIndex = -1;
		}
	}	
}

void CStatisticsDlg::OnBnClickedSavedata(){
	if (!m_bSaveRawData && !m_bSaveResult) {
		AfxMessageBox("저장 항목이 없습니다 :-(");
		return;
	}

	if (AfxMessageBox("정말 저장할까요?", MB_YESNO) == IDYES) {
		CFile SaveFile;
		CFileException ex;
		CString strFileName;
		SYSTEMTIME time;
		GetLocalTime(&time);
		CString str = "";

		strFileName.Format("SaveData_%02d%02d%02d_%02d%02d%02d.csv", time.wYear%100, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		SaveFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone, &ex);
		
		CString strTemp;
		if (m_bSaveResult) {
			int iListCount = m_ResultList.GetItemCount();

			if (m_bMathResult){
				str.Format("Item");
				for (int j = 0; j < iListCount; j++) {
					strTemp.Format("%s", (LPCTSTR)m_ResultList.GetItemText(j, 0));
					strTemp.Replace(",", "_");
                    CString str1 = str;
					str.Format("%s,%s", (LPCTSTR)str1, (LPCTSTR)strTemp);
				}
				
				str += "\r\n";
				str += "PinName";
				for (int j = 0; j < iListCount; j++) {
					strTemp.Format("%s", (LPCTSTR)m_ResultList.GetItemText(j, 1));
					strTemp.Replace(",", "_");
                    CString str1 = str;
					str.Format("%s,%s", (LPCTSTR)str1, (LPCTSTR)strTemp);
				}
				str += "\r\n";

				char *listColumn[5] = { "RMS", "Min Value", "Mean", "MaxValue", "Stdev" };
				for (int i = 0; i < 5; i++) {
                    CString str1 = str;
					str.Format("%s%s", (LPCTSTR)str1, (LPCTSTR)listColumn[i]);
					for (int j = 0; j < iListCount; j++) {
                        CString strTemp = str;
						str.Format("%s,%s", (LPCTSTR)strTemp, (LPCTSTR)m_ResultList.GetItemText(j, i + 2));
					}
					str += "\r\n";					
				}

				str += "\r\n\r\n";
				SaveFile.Write(str, str.GetLength());
				str = "";
			}
			else {
				str.Format("Item,Data1,Data2,Data3,Data4,Data5,Data6,Data7,Data8,Data9,Data10,Data11,Data12,Data13,Data14,Data15,Data16\r\n");
				str += "PinName";
				for (int j = 0; j < MAX_DATACONTROL_SIZE; j++) {
                    CString strTemp = str;
					str.Format("%s,%s", (LPCTSTR)strTemp, (LPCTSTR)pMain->m_strPinName[j]);
				}
				str += "\r\n";

				char *listColumn[5] = { "RMS", "Min Value", "Mean", "MaxValue", "Stdev" };
				for (int i = 0; i < 5; i++) {
                    CString str1 = str;
					str.Format("%s%s", (LPCTSTR)str1, (LPCTSTR)listColumn[i]);
					for (int j = 0; j < MAX_DATACONTROL_SIZE; j++) {
                        CString strTemp = str;
						str.Format("%s,%s", (LPCTSTR)strTemp, (LPCTSTR)m_ResultList.GetItemText(j, i + 2));
					}
					str += "\r\n";
				}

				str += "\r\n\r\n";
				SaveFile.Write(str, str.GetLength());
				str = "";
			}
		}

		if (m_bSaveRawData) {
			str.Format("n,Data1,Data2,Data3,Data4,Data5,Data6,Data7,Data8,Data9,Data10,Data11,Data12,Data13,Data14,Data15,Data16\r\n");

			for (int i = m_iStartIndex; i <= m_iEndIndex; i++) {
                CString str1 = str;
				str.Format("%s%d,%s", (LPCTSTR)str1, i, (LPCTSTR)Dbl2Str(pMain->m_HistoryData[i].fDataValue[0]));
				for (int j = 1; j < MAX_DATACONTROL_SIZE; j++) {
                    CString strTemp = str;
					str.Format("%s,%s", (LPCTSTR)strTemp, (LPCTSTR)Dbl2Str(pMain->m_HistoryData[i].fDataValue[j]));
				}
				str += "\r\n";

				if (str.GetLength() > 30000) {
					SaveFile.Write(str, str.GetLength());
					str = "";
				}
			}
		}
		if (str.GetLength() > 0) SaveFile.Write(str, str.GetLength());

		SaveFile.Close();
	}
}

void CStatisticsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (cy >= 110)
		m_ResultList.MoveWindow(3, 104, cx - 6, cy - 110, TRUE);

}

void CStatisticsDlg::OnBnClickedDeterminindexmode()
{

}

void CStatisticsDlg::OnClose()
{
	DestroyWindow();


	CDialogEx::OnClose();
}

void CStatisticsDlg::OnCancel()
{
	DestroyWindow();
}


void CStatisticsDlg::UpdateControlData() {
	UDT;
}


void CStatisticsDlg::OnBnClickedSavemathresult()
{
	UDT;
}

void CStatisticsDlg::OnClickResultlist(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_iSelectList = pNMItemActivate->iItem;
	*pResult = 0;
}


void CStatisticsDlg::OnBnClickedDrawfft(){
	if (m_iSelectList != -1) {
		int iStart = atoi(m_strRmsStartIndex);
		int iEnd = atoi(m_strRmsEndIndex);

		auto fft = std::make_unique< CFftDlg> ();
		fft->pMain = pMain;		
		fft->m_strDataPeriod.Format ("%ld", m_iSamplingTime);
		fft->m_fDataPeriod = (double)m_iSamplingTime;
		fft->m_strItemName.Format("%s", (LPCTSTR)m_ResultList.GetItemText(m_iSelectList, 1));
		if (m_iSelectList < MAX_DATACONTROL_SIZE) {
			fft->m_bIsMath = FALSE;
			fft->m_iDataChannel = m_iSelectList;
		}
		else {
			fft->m_bIsMath = TRUE;
			int iCount = 15;
			fft->m_iDataChannel = -1;
			for (int iData = 0; iData < pMain->m_iMaxMathSize; iData++) {
				if (pMain->m_MathData[iData].bValid) {
					if (++iCount == m_iSelectList) {
						fft->m_iDataChannel = iData;
					}
				}
			}
			if (fft->m_iDataChannel == -1) { AfxMessageBox("Item 선택 오류 발생");  return; }
		}
		if (iEnd - iStart + 1 > N_MAX_FFT) {
			CString str;
			str.Format("선택된 Data가 많아 FFT를 실행할 수 없습니다. (제한 : %d이하 / 선택:%d)", N_MAX_FFT, iEnd - iStart + 1);
			AfxMessageBox(str);  return;
		} 
		else if (iEnd == iStart) {
			AfxMessageBox("최소 2개 Data이상을 선택해주세요");  return;
		}

		fft->m_strItemName.Format("Item Info : %s   [ Ch:%d,  IsMath:%d ]", (LPCTSTR)fft->m_strItemName, fft->m_iDataChannel, fft->m_bIsMath);
		fft->m_iStartIndex = iStart;
		fft->m_iEndIndex = iEnd;

		fft->DoModal();
	}
	else {
		AfxMessageBox("아이템 선택후 사용 바랍니다.");
	}
}


void CStatisticsDlg::OnBnClickedSavedata2(){

	CString strClip = "";
	CString str = "";
	CString strTemp;
	
	int iListCount = m_ResultList.GetItemCount();

	if (m_bMathResult) {
		str.Format("Item");
		for (int j = 0; j < iListCount; j++) {
			strTemp.Format("%s", (LPCTSTR)m_ResultList.GetItemText(j, 0));
			strTemp.Replace(",", "_");
            CString str1 = str;
			str.Format("%s\t%s", (LPCTSTR)str1, (LPCTSTR)strTemp);
		}

		str += "\r\n";
		str += "PinName";
		for (int j = 0; j < iListCount; j++) {
			strTemp.Format("%s", (LPCTSTR)m_ResultList.GetItemText(j, 1));
			strTemp.Replace(",", "_");
            CString str1 = str;
			str.Format("%s\t%s", (LPCTSTR)str1, (LPCTSTR)strTemp);
		}
		str += "\r\n";

		char *listColumn[5] = { "RMS", "Min Value", "Mean", "MaxValue", "Stdev" };
		for (int i = 0; i < 5; i++) {
            CString strTemp = str;
			str.Format("%s%s", (LPCTSTR)strTemp, (LPCTSTR)listColumn[i]);
			for (int j = 0; j < iListCount; j++) {
                CString strTemp = str;
				str.Format("%s\t%s", (LPCTSTR)strTemp, (LPCTSTR)m_ResultList.GetItemText(j, i + 2));
			}
			str += "\r\n";
		}

		str += "\r\n\r\n";
		strClip += str;
		str = "";
	}
	else {
		str.Format("Item\tData1\tData2\tData3\tData4\tData5\tData6\tData7\tData8\tData9\tData10\tData11\tData12\tData13\tData14\tData15\tData16\r\n");
		str += "PinName";
		for (int j = 0; j < MAX_DATACONTROL_SIZE; j++) {
            CString strTemp = str;
			str.Format("%s\t%s", (LPCTSTR)strTemp, (LPCTSTR)pMain->m_strPinName[j]);
		}
		str += "\r\n";

		char *listColumn[5] = { "RMS", "Min Value", "Mean", "MaxValue", "Stdev" };
		for (int i = 0; i < 5; i++) {
            CString strTemp = str;
			str.Format("%s%s", (LPCTSTR)strTemp, (LPCTSTR)listColumn[i]);
			for (int j = 0; j < MAX_DATACONTROL_SIZE; j++) {
                CString strTemp = str;
				str.Format("%s\t%s", (LPCTSTR)strTemp, (LPCTSTR)m_ResultList.GetItemText(j, i + 2));
			}
			str += "\r\n";
		}

		str += "\r\n\r\n";
		strClip += str;
		str = "";
	}
	
	const char* convertMsg;
	CT2A ascii(strClip, CP_UTF8);
	convertMsg = ascii.m_psz;
	CopyTextToClipboard(convertMsg);	
}

int CStatisticsDlg::CopyTextToClipboard(const char *ap_string) {
	// 저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기) 
	int string_length = strlen(ap_string) + 1;

	// 클립보드로 문자열을 복사하기 위하여 메모리를 할당한다. 
	// 클립보드에는 핸들을 넣는 형식이라서 HeapAlloc 함수 사용이 블가능하다. 
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_length);

	// 할당된 메모리에 문자열을 복사하기 위해서 사용 가능한 주소를 얻는다. 
	char *p_data = (char *)::GlobalLock(h_data);

	if (NULL != p_data) {
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

BOOL CStatisticsDlg::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case 'C':
			if (GetKeyState(VK_CONTROL) < 0) {
				if (pMsg->hwnd == m_ResultList.GetSafeHwnd() && GetFocus()->GetSafeHwnd() == m_ResultList.GetSafeHwnd()) {
					if (m_iSelectList > -1){
						CString str;
						str.Format("Data#\tPinName\tRMS\tMinValue\tMean\tMaxValue\tStdev\r\n");
						for (int i = 0; i < 7; i++) {
                            CString strTemp = str;
							str.Format("%s%s\t", (LPCTSTR)strTemp, (LPCTSTR)m_ResultList.GetItemText(m_iSelectList, i));
						}

						const char* convertMsg;
						CT2A ascii(str, CP_UTF8);
						convertMsg = ascii.m_psz;
						CopyTextToClipboard(convertMsg);
					}
				}
			}
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}




void CStatisticsDlg::OnBnClickedSavedata3() {
	int iFrom = atoi(m_strRmsStartIndex);
	int iTo = atoi(m_strRmsEndIndex);

	int iFromLog = pMain->m_HistoryData[iFrom].iHistoryLogIndex;
	int iToLog = pMain->m_HistoryData[iTo].iHistoryLogIndex;

	pMain->ReqSaveData(FALSE, iFromLog, iToLog);
}
