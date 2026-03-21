// TextLogDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialComDlg.h"
#include "TextLogDlg.h"
#include "afxdialogex.h"

// CTextLogDlg 대화 상자

IMPLEMENT_DYNAMIC(CTextLogDlg, CDialogEx)

CTextLogDlg::CTextLogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TextLog, pParent)
{

}

CTextLogDlg::~CTextLogDlg()
{
}

void CTextLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTextLogDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Test, &CTextLogDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_Hide, &CTextLogDlg::OnBnClickedHide)
END_MESSAGE_MAP()


// CTextLogDlg 메시지 처리기


void CTextLogDlg::OnBnClickedTest()
{
}


void CTextLogDlg::OnCancel() {
	OnBnClickedHide();

}

void CTextLogDlg::OnBnClickedHide()
{
	pMain->CheckDlgButton(IDC_ShowTextLog, FALSE);
	pMain->OnBnClickedShowtextlog();
}
