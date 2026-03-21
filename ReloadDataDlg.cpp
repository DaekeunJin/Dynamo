// ReloadDataDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "ReloadDataDlg.h"
#include "afxdialogex.h"
#include "SerialComDlg.h"


// CReloadDataDlg 대화 상자

IMPLEMENT_DYNAMIC(CReloadDataDlg, CDialogEx)

CReloadDataDlg::CReloadDataDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ReloadDlg, pParent)
{

}

CReloadDataDlg::~CReloadDataDlg()
{
}

void CReloadDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReloadDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Process, &CReloadDataDlg::OnBnClickedProcess)
	ON_EN_CHANGE(IDC_NewHeader, &CReloadDataDlg::OnEnChangeNewheader)
END_MESSAGE_MAP()


// CReloadDataDlg 메시지 처리기


BOOL CReloadDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CReloadDataDlg::UpdateDisplay() {	
	SetDlgItemText(IDC_LogCount, pMain->AddComma(pMain->m_iLogIndex));
	SetDlgItemText(IDC_DataCount, pMain->AddComma(pMain->m_iHistoryIndex));
	
	int HeaderSize = 0;
	int nHeader[10];
	char cHeader[10];

	for (int i = 0; i < pMain->m_iHistoryIndex; i++) {
		char header = pMain->m_HistoryData[i].Header;
		bool bFind = false;
		for (int j = 0; j < HeaderSize; j++) {
			if (cHeader[j] == header) {
				nHeader[j]++;
				bFind = true;
				break;
			}
		}
		if (bFind == false && HeaderSize < 10) {
			nHeader[HeaderSize] = 1;
			cHeader[HeaderSize] = header;
			HeaderSize++;			
		}
	}

	CString str = "";
	for (int i = 0; i < HeaderSize; i++) {
        CString strTemp = str;
		str.Format("%s%c (%s)    ", (LPCTSTR)strTemp, cHeader[i], (LPCTSTR)pMain->AddComma(nHeader[i]));
	}
	SetDlgItemText(IDC_HeaderInfo, str);
	
	str.Format("%c", pMain->m_cPacketHead);
	SetDlgItemText(IDC_NewHeader, str);

}

void CReloadDataDlg::OnEnChangeNewheader()
{
	static char preHeader = 0;
	CString str1, str2;
	GetDlgItemText(IDC_NewHeader, str1);
	int size = str1.GetLength();

	for (int i = 0; i < size; i++) {
		char ch = str1.GetAt(i);
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
			if (preHeader != ch) {
				preHeader = ch;
			}
			break;
		}
	}
	str2.Format("%c", preHeader);
	if (str1 != str2) {
		SetDlgItemText(IDC_NewHeader, str2);
	}
}


void CReloadDataDlg::OnBnClickedProcess()
{
	*m_pResult = 1;
	GetDlgItemText(IDC_NewHeader, pMain->m_strPacketHeader);	
	pMain->SetDlgItemText(IDC_PacketHead, pMain->m_strPacketHeader);
	pMain->OnEnChangePackethead();
	OnOK();
}