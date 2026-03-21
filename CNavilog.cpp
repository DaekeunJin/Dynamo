// CNavilog.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "CNavilog.h"
#include "afxdialogex.h"


// CNavilog 대화 상자

IMPLEMENT_DYNAMIC(CNavilog, CDialogEx)

CNavilog::CNavilog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NaviLog, pParent)
	, m_iTimeMode(0)
{

}

CNavilog::~CNavilog()
{
}

void CNavilog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TimeMode0, m_iTimeMode);
}


BEGIN_MESSAGE_MAP(CNavilog, CDialogEx)
	ON_BN_CLICKED(IDC_Test, &CNavilog::OnBnClickedTest)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_SavePhase, &CNavilog::OnBnClickedSavephase)
END_MESSAGE_MAP()


// CNavilog 메시지 처리기


void CNavilog::OnBnClickedTest() {	
	int m_countPharsing = 0;	
	BYTE m_PhasingMode[100][3];
    BYTE IgnorePhase[1000] = { 0 };
	int PosIgnorePhase = 0;
	int seq = 0;
	CString str;
	GetDlgItemText(IDC_NaviFormat, str);
	int n = str.GetLength();
	BOOL bIgnore = FALSE;
	for (int i = 0; i < n; i++) {
		char aByte = str.GetAt(i);
		
		switch (seq) {
		case 0:
			if (aByte == ' ') {

			}
			else if (aByte == '"') {
				seq = 10;
				m_PhasingMode[m_countPharsing][0] = '"';
				m_PhasingMode[m_countPharsing][1] = PosIgnorePhase;
				bIgnore = TRUE;				
			}
			else {
				m_PhasingMode[m_countPharsing++][0] = aByte;				
			}
			break;

		case 10:
			if (aByte == '"') {
				bIgnore = FALSE;
				m_PhasingMode[m_countPharsing][2] = PosIgnorePhase-1;
				if (m_PhasingMode[m_countPharsing][2] >= m_PhasingMode[m_countPharsing][1]) { 
					m_countPharsing++; 
				}
				seq = 0;
			}
			else {
				IgnorePhase[PosIgnorePhase++] = aByte;
			}
		}	
	}

	if (bIgnore) {
		m_PhasingMode[m_countPharsing][2] = PosIgnorePhase - 1;
		if (m_PhasingMode[m_countPharsing][2] >= m_PhasingMode[m_countPharsing][1]) {
			m_countPharsing++;
		}
	}

	str.Format("%d %d ", m_countPharsing, PosIgnorePhase);
	str = "";
	
	for (int i = 0; i < m_countPharsing; i++) {
		if (m_PhasingMode[i][0] == '"') {
			str += "Ignore:";
			for (int j = m_PhasingMode[i][1]; j <= m_PhasingMode[i][2]; j++) {
                CString strTemp = str;
				str.Format("%s%c", (LPCTSTR)strTemp, IgnorePhase[j]);
			}
			str += "\r\n";
		}
		else {
            CString strTemp = str;
			str.Format("%s%c\r\n", (LPCTSTR)strTemp, m_PhasingMode[i][0]);
		}
	}
	AfxMessageBox(str);

}

void CNavilog::OnBnClickedSavephase() {
	UDT;
	CString str;
	GetDlgItemText(IDC_NaviFormat, str);
	pMain->SetRegRoot_RegistryData("common", "NaviFormat", str);

	str.Format("%d", IsDlgButtonChecked(IDC_SaveFile));
	pMain->SetRegRoot_RegistryData("common", "NaviConvertSave", str);

	str.Format("%d", IsDlgButtonChecked(IDC_DataLoading));
	pMain->SetRegRoot_RegistryData("common", "NaviConvertDataLoading", str);

	str.Format("%d", IsDlgButtonChecked(IDC_AutoClose));
	pMain->SetRegRoot_RegistryData("common", "NaviConvertAutoClose", str);

	str.Format("%d", m_iTimeMode);
	pMain->SetRegRoot_RegistryData("common", "NaviConvertTimeMode", str);
}

BOOL CNavilog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	str = pMain->GetRegRoot_RegistryData("common", "NaviFormat");
	if (str == "") { str = "\"2023\" T :"; }
	SetDlgItemText(IDC_NaviFormat, str);

	str = pMain->GetRegRoot_RegistryData("common", "NaviConvertSave");
	if (str == "") { str = "1"; }
	CheckDlgButton(IDC_SaveFile, atoi(str));

	str = pMain->GetRegRoot_RegistryData("common", "NaviConvertDataLoading");
	if (str == "") { str = "1"; }
	CheckDlgButton(IDC_DataLoading, atoi(str));

	str = pMain->GetRegRoot_RegistryData("common", "NaviConvertAutoClose");
	if (str == "") { str = "1"; }
	CheckDlgButton(IDC_AutoClose, atoi(str));

	str = pMain->GetRegRoot_RegistryData("common", "NaviConvertTimeMode");
	m_iTimeMode = atoi(str);

	DragAcceptFiles(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNavilog::OnDropFiles(HDROP hDropInfo) {
	UpdateData(TRUE);
	char szPathName[MAX_PATH];
	CString strFullPath;
	CFile DataFile, SaveFile;

	int nFile = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH);

	DragQueryFile(hDropInfo, 0, szPathName, MAX_PATH);
	DragFinish(hDropInfo);
	CString str, strFilename = "", strSavefile;
	strFilename.Format("%s", szPathName);
	int iTimemode = m_iTimeMode;
	long Index = 0;	
	

	if (DataFile.Open(szPathName, CFile::modeRead | CFile::shareDenyNone)) {		
		int m_countPharsing = 0;
		BYTE m_PhasingMode[100][3];
        BYTE IgnorePhase[1000] = {0,};
		int PosIgnorePhase = 0;
		int seq = 0;

		CString str;
		GetDlgItemText(IDC_NaviFormat, str);
		int n = str.GetLength();
		BOOL bIgnore = FALSE;
		
		for (int i = 0; i < n; i++) {
			char aByte = str.GetAt(i);

			switch (seq) {
			case 0:
				if (aByte == ' ') {

				}
				else if (aByte == '"') {
					seq = 10;
					m_PhasingMode[m_countPharsing][0] = '"';
					m_PhasingMode[m_countPharsing][1] = PosIgnorePhase;
					bIgnore = TRUE;
				}
				else {
					if ((aByte >= 'a'  && aByte <= 'z') || (aByte >= 'A'  && aByte <= 'S') || (aByte >= 'U'  && aByte <= 'Z') || (aByte >= '0'  && aByte <= '9')) {

					}
					else {
						m_PhasingMode[m_countPharsing++][0] = aByte;
					}
				}
				break;

			case 10:
				if (aByte == '"') {
					bIgnore = FALSE;
					m_PhasingMode[m_countPharsing][2] = PosIgnorePhase - 1;
					if (m_PhasingMode[m_countPharsing][2] >= m_PhasingMode[m_countPharsing][1]) {
						m_countPharsing++;
					}
					seq = 0;
				}
				else {
					IgnorePhase[PosIgnorePhase++] = aByte;
				}
			}
		}

		if (bIgnore) {
			m_PhasingMode[m_countPharsing][2] = PosIgnorePhase - 1;
			if (m_PhasingMode[m_countPharsing][2] >= m_PhasingMode[m_countPharsing][1]) {
				m_countPharsing++;
			}
		}

		str.Format("%d %d ", m_countPharsing, PosIgnorePhase);
		str = "";

		for (int i = 0; i < m_countPharsing; i++) {
			if (m_PhasingMode[i][0] == '"') {
                CString strTemp = str;
				str.Format("%sIgnore:", (LPCTSTR)strTemp);
				for (int j = m_PhasingMode[i][1]; j <= m_PhasingMode[i][2]; j++) {
                    CString strTemp = str;
					str.Format("%s%c", (LPCTSTR)strTemp, IgnorePhase[j]);
				}
				str += "\r\n";
			}
			else {
                CString strTemp = str;
				str.Format("%s%c\r\n", (LPCTSTR)strTemp, m_PhasingMode[i][0]);
			}
		}

		if (m_countPharsing == 0) {
			AfxMessageBox("Error : Navi Log Phasing 규칙이 없습니다!");
			return;
		}

		BOOL bFindT = FALSE;
		for (int i = 0; i < m_countPharsing; i++) {
			if (m_PhasingMode[i][0] == 'T') {
				bFindT = TRUE;
			}
		}
		if (bFindT == FALSE) {
			if (AfxMessageBox("Error : 시간을 나타내는 부분이 없습니다! 계속할까요??", MB_YESNO) != IDYES) {
				return;
			}
		}

		int iCount = 0;
		int iIgnore = 0;
		int ignoreCount = 0;
		int ignore_st = m_PhasingMode[iCount][1];
		int ignore_en = m_PhasingMode[iCount][2];
		int mode = 0;
		int timeCount = 0;
		long timeValue = 0;
		BOOL bGetTime = FALSE;
		BOOL bGetData = FALSE;
		if (m_countPharsing == 0) {
			seq = 100;
		}
		else if (m_PhasingMode[iCount][0] == '"') {
			seq = 200;
		}
		else if (m_PhasingMode[iCount][0] == 'T') {
			seq = 300;
		}
		else {
			seq = 400;
		}

		BYTE save[1000];
		int idx = 0;
		int hh = 0;
		int mm = 0;
		int ss = 0;
		int ms = 0;
				
		long size = (long)(DataFile.GetLength());
		BYTE* pBuffer = new BYTE[(UINT)size];
		DataFile.Read(pBuffer, (UINT)size);
		DataFile.Close();

		
		BOOL bSaveFile = IsDlgButtonChecked(IDC_SaveFile);
		BOOL bDataLoad = IsDlgButtonChecked(IDC_DataLoading);
		if (bSaveFile) {
			strSavefile.Format("%s_Convert.txt", szPathName);
			SaveFile.Open(strSavefile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone);
		}		

		for (long i = 0; i < size; i++) {
			BYTE aByte = *(pBuffer + i);
			if (aByte == '\r' || aByte == '\n') {
				// 현재 내용 저장 & 변수 초기화				
				if (m_countPharsing == 0) {
					seq = 100;
				}
				else if (m_PhasingMode[iCount][0] == '"') {
					seq = 200;
					ignore_st = m_PhasingMode[iCount][1];
					ignore_en = m_PhasingMode[iCount][2];
					ignoreCount = 0;
				}
				else if (m_PhasingMode[iCount][0] == 'T') {
					seq = 300;
					timeCount = 0;
					timeValue = 0;
					hh = 0;
				}
				else {
					seq = 400; 
				}

				if (idx > 0) {
					if (bSaveFile) {
						save[idx++] = '\r';
						save[idx++] = '\n';
					
						if (m_countPharsing != 0) {
							str.Format("%02d:%02d:%02d.000] ", hh, mm, ss);
							SaveFile.Write(str, str.GetLength());
						}
						SaveFile.Write(save, idx);
					}	
					if (bDataLoad) {
						*(pBuffer + Index++) = '\r';
						*(pBuffer + Index++) = '\n';
					}
					idx = 0;
				}
				iCount = 0;
				continue;
			}

			switch (seq) {
			case 100: 
				save[idx++] = aByte;
				*(pBuffer + Index++) = aByte;
				iCount = 0;
				break;

			case 200:
				if (aByte == IgnorePhase[ignore_st + ignoreCount]) {
					if (ignore_st + ignoreCount >= ignore_en) {	
						iCount++;
						if (iCount >= m_countPharsing) {
							seq = 100;
							iCount = 0;
						}
						else {
							if (m_PhasingMode[iCount][0] == '"') {
								seq = 200;
								ignore_st = m_PhasingMode[iCount][1];
								ignore_en = m_PhasingMode[iCount][2];
								ignoreCount = 0;
							}
							else if (m_PhasingMode[iCount][0] == 'T') {
								seq = 300;
								timeCount = 0;
								timeValue = 0;
								hh = 0;
							}
							else {
								seq = 400;
							}
						}
						break;
					}		
					ignoreCount++;
				}
				else {
					ignoreCount = 0;
				}
				break;


			case 300: // TimeCount	
				switch (timeCount) {
				case 0: if (aByte >= '0' && aByte <= '9') { hh = aByte - '0'; timeCount++; } break;
				case 1: if (aByte >= '0' && aByte <= '9') { hh = hh * 10 + aByte - '0'; timeCount++; }
						else { timeCount = 0; } break;
				case 2: if (aByte >= ':') { timeCount++; }
						else { timeCount = 0; } break;
				case 3: if (aByte >= '0' && aByte <= '9') { mm = aByte - '0'; timeCount++; }
						else { timeCount = 0; } break;
				case 4: if (aByte >= '0' && aByte <= '9') { mm = mm * 10 + aByte - '0'; timeCount++; }
						else { timeCount = 0; } break;
				case 5: if (aByte >= ':') { timeCount++; }
						else { timeCount = 0; } break;
				case 6: if (aByte >= '0' && aByte <= '9') { ss = aByte - '0'; timeCount++; }
						else { timeCount = 0; } break;
				case 7:
					if (aByte >= '0' && aByte <= '9') {
						ss = ss * 10 + aByte - '0'; timeCount++;
						if (iTimemode == 0) {
							if (bDataLoad) {
								CString strTemp;
								strTemp.Format("%02d:%02d:%02d.000]", hh, mm, ss);
								strcpy_s((char *)(pBuffer + Index), strTemp.GetLength(), strTemp);
								//sprintf((char *)(pBuffer + Index), "%02d:%02d:%02d.000]", hh, mm, ss);
								Index += 13;
							}

							iCount++;
							if (iCount >= m_countPharsing) {
								seq = 100;
								iCount = 0;
							}
							else {
								if (m_PhasingMode[iCount][0] == '"') {
									seq = 200;
									ignore_st = m_PhasingMode[iCount][1];
									ignore_en = m_PhasingMode[iCount][2];
									ignoreCount = 0;
								}
								else if (m_PhasingMode[iCount][0] == 'T') {
									seq = 300;
									timeCount = 0;
									timeValue = 0;
									hh = 0;
								}
								else {
									seq = 400;
								}
							}
						}
					}
					else { timeCount = 0; } break;
				case 8:	if (aByte >= '.') { timeCount++; }
						else { timeCount = 0; } break;
				case 9: if (aByte >= '0' && aByte <= '9') { ms = aByte - '0'; timeCount++; }
						else { timeCount = 0; } break;
				case 10:if (aByte >= '0' && aByte <= '9') { ms = ms * 10 + aByte - '0'; timeCount++; }
						else { timeCount = 0; } break;
				case 11:
					if (aByte >= '0' && aByte <= '9') {
						ms = ms * 10 + aByte - '0'; timeCount++;

						if (bDataLoad) {
							CString strTemp;
							strTemp.Format("%02d:%02d:%02d.%03d]", hh, mm, ss, ms);
							strcpy_s((char *)(pBuffer + Index), strTemp.GetLength(), strTemp);
							Index += 13;
						}

						iCount++;
						if (iCount >= m_countPharsing) {
							seq = 100;
							iCount = 0;
						}
						else {
							if (m_PhasingMode[iCount][0] == '"') {
								seq = 200;
								ignore_st = m_PhasingMode[iCount][1];
								ignore_en = m_PhasingMode[iCount][2];
								ignoreCount = 0;
							}
							else if (m_PhasingMode[iCount][0] == 'T') {
								seq = 300;
								timeCount = 0;
								timeValue = 0;
								hh = 0;
							}
							else {
								seq = 400;
							}
						}

					}
					else { timeCount = 0; } break;
				}
				break;

			case 400:
				if (aByte == m_PhasingMode[iCount][0]) {
					iCount++;
					if (iCount >= m_countPharsing) {
						seq = 100;
						iCount = 0;
					}
					else {
						if (m_PhasingMode[iCount][0] == '"') {
							seq = 200;
							ignore_st = m_PhasingMode[iCount][1];
							ignore_en = m_PhasingMode[iCount][2];
							ignoreCount = 0;
						}
						else if (m_PhasingMode[iCount][0] == 'T') {
							seq = 300;
							timeCount = 0;
							timeValue = 0;
							hh = 0;
						}
						else {
							seq = 400;
						}
					}
				}
				break;
			}
		}
		
		if (bSaveFile) {
			SaveFile.Close();
		}
		if (bDataLoad) {
			pMain->GetSaveLog(pBuffer, Index);
		}

		delete [] pBuffer;
	}	
	if (IsDlgButtonChecked(IDC_AutoClose)) {
		OnOK();
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

