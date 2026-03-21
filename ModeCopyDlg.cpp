// ModeCopyDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialComDlg.h"
#include "ModeCopyDlg.h"
#include "afxdialogex.h"


// CModeCopyDlg 대화 상자

IMPLEMENT_DYNAMIC(CModeCopyDlg, CDialogEx)

CModeCopyDlg::CModeCopyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ModeCopyDlg, pParent)
{

}

CModeCopyDlg::~CModeCopyDlg()
{
}

void CModeCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SourceMode, m_ctrlSourceMode);
}


BEGIN_MESSAGE_MAP(CModeCopyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ReqCopy, &CModeCopyDlg::OnBnClickedReqcopy)
	ON_EN_CHANGE(IDC_SourceHeader, &CModeCopyDlg::OnEnChangeSourceheader)
	ON_BN_CLICKED(IDC_SelectAll, &CModeCopyDlg::OnBnClickedSelectall)
	ON_EN_CHANGE(IDC_TargetHeader, &CModeCopyDlg::OnEnChangeTargetheader)
	ON_BN_CLICKED(IDC_AllPresetCheck, &CModeCopyDlg::OnBnClickedAllpresetcheck)
END_MESSAGE_MAP()


// CModeCopyDlg 메시지 처리기


void CModeCopyDlg::OnBnClickedReqcopy()
{	
	int iCopyItem = 0;
	int iSourceMode = m_ctrlSourceMode.GetCurSel();
	int iTagetMode0 = GetDlgItemInt(IDC_TargetMode) - 1;
	int iTagetMode1 = GetDlgItemInt(IDC_TargetMode) - 1;

	if (iTagetMode0 > iTagetMode1) {
		int temp = iTagetMode1;
		iTagetMode1 = iTagetMode0;
		iTagetMode0 = temp;
	}

	if (IsDlgButtonChecked(IDC_CopyItem0)) {
		pMain->CopyDataProbeMode(m_cSourceHead, m_cTargetHead);
		iCopyItem++;
	}
	   
	if (IsDlgButtonChecked(IDC_CopyItem2)) {
		pMain->CopyDataGraphSetting(m_cSourceHead, iSourceMode, m_cTargetHead, iTagetMode0, iTagetMode1);
		iCopyItem++;
	}

	if (IsDlgButtonChecked(IDC_CopyItem1)) {
		pMain->CopyMathProbeMode(m_cSourceHead, iSourceMode, m_cTargetHead, iTagetMode0, iTagetMode1);
		iCopyItem++;
	}

	if (IsDlgButtonChecked(IDC_CopyItem3)) {
		pMain->CopyMathGraphSetting(m_cSourceHead, iSourceMode);
		iCopyItem++;
	}

	if (iCopyItem == 0) {
		AfxMessageBox("선택된 복사 항목이 없습니다..");
		return;
	}

	pMain->CheckMathProbeValid();
	pMain->OptimizeGraphCount();
	pMain->ShowDataMathControl();
	pMain->m_bReqGraphUpdate = TRUE;
	pMain->m_bReqGraphUpdate = TRUE;

	pMain->ShowDataMathControl(); 

	AfxMessageBox("설정을 복사하였습니다.  설정 저장을 눌러야 최종 저장이 됩니다.");
	OnOK();
}


BOOL CModeCopyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strTarget;
	strTarget.Format("%c", pMain->m_cPacketHead);
	SetDlgItemText(IDC_TargetHeader, strTarget);
	SetDlgItemText(IDC_SourceHeader, strTarget);

	strTarget.Format("%d", pMain->m_iPresetMode + 1);
	SetDlgItemText(IDC_TargetMode, strTarget);

	OnBnClickedAllpresetcheck();
	OnEnChangeSourceheader();
	return TRUE;
}

void CModeCopyDlg::OnBnClickedSelectall()
{
	CheckDlgButton(IDC_CopyItem0, TRUE);
	CheckDlgButton(IDC_CopyItem1, TRUE);
	CheckDlgButton(IDC_CopyItem2, TRUE);
	CheckDlgButton(IDC_CopyItem3, TRUE);
}

void CModeCopyDlg::OnEnChangeSourceheader()
{
	static BYTE pre = 'z';
	CString str, str1;
	BYTE aByte;
	GetDlgItemText(IDC_SourceHeader, str);
	aByte = str.GetAt(0);

	if ((aByte >= 'a' && aByte <= 'z') || (aByte >= 'A' && aByte <= 'Z')) {
		str1.Format("%c", aByte);
		if (str != str1) {
			SetDlgItemText(IDC_SourceHeader, str1);
		}
		pre = aByte;
	}
	else {		
		str.Format("%c", pre);
		SetDlgItemText(IDC_SourceHeader, str);
		aByte = pre;
	}
	m_cSourceHead = aByte;
	BOOL bActive = TRUE;
	if (m_cSourceHead == m_cTargetHead) {
		bActive = FALSE;
	}
	GetDlgItem(IDC_CopyItem0)->EnableWindow(bActive);
}


void CModeCopyDlg::OnEnChangeTargetheader()
{
	static BYTE pre = 'z';
	CString str, str1;
	BYTE aByte;
	GetDlgItemText(IDC_TargetHeader, str);
	aByte = str.GetAt(0);

	if ((aByte >= 'a' && aByte <= 'z') || (aByte >= 'A' && aByte <= 'Z')) {
		str1.Format("%c", aByte);
		if (str != str1) {
			SetDlgItemText(IDC_TargetHeader, str1);
		}
		pre = aByte;
	}
	else {
		str.Format("%c", pre);
		SetDlgItemText(IDC_TargetHeader, str);
		aByte = pre;
	}
	m_cTargetHead = aByte;
	BOOL bActive = TRUE;
	if (m_cSourceHead == m_cTargetHead) {
		bActive = FALSE;
	}
	GetDlgItem(IDC_CopyItem0)->EnableWindow(bActive);
}


void CModeCopyDlg::OnBnClickedAllpresetcheck()
{
	SetDlgItemInt(IDC_TargetMode, 1);

	SetDlgItemInt(IDC_TargetMode2, N_MAX_Preset);
}
