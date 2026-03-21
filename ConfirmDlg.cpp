// ConfirmDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "ConfirmDlg.h"
#include "afxdialogex.h"


// CConfirmDlg 대화 상자

IMPLEMENT_DYNAMIC(CConfirmDlg, CDialogEx)

CConfirmDlg::CConfirmDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ConfimrDlg, pParent)
{

}

CConfirmDlg::~CConfirmDlg()
{
}

void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialogEx)
	ON_EN_CHANGE(IDC_Answer, &CConfirmDlg::OnEnChangeAnswer)
END_MESSAGE_MAP()


// CConfirmDlg 메시지 처리기


void CConfirmDlg::OnEnChangeAnswer() {
	CString str, str2;
	GetDlgItemText(IDC_Answer, str);
	int nLen = str.GetLength();

	if (nLen == 4) {
		GetDlgItemText(IDC_Answer, str);
		str2.Format("%04d", m_num);
		if (str == str2) {
			pSetting->m_bConfirmInit = TRUE;
			OnOK();
		}
		else {
			str = "";
			SetDlgItemText(IDC_Answer, str);
		}
	}
	else if (nLen > 4) {
		str = "";
		SetDlgItemText(IDC_Answer, str);
	}
}


BOOL CConfirmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	srand((unsigned)time(NULL));
	m_num = rand() % 10000;
	CString str;
	str.Format ("%s진행하려면 %04d를 입력하세요", (LPCTSTR)m_strJob, m_num);
	SetDlgItemText(IDC_ReqMessage, str);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
