// LoadCtrlDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "LoadCtrlDlg.h"
#include "afxdialogex.h"


// CLoadCtrlDlg 대화 상자

IMPLEMENT_DYNAMIC(CLoadCtrlDlg, CDialogEx)

CLoadCtrlDlg::CLoadCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LoadControlDlg, pParent)
{

}

CLoadCtrlDlg::~CLoadCtrlDlg()
{
}

void CLoadCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoadCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SaveSetting, &CLoadCtrlDlg::OnBnClickedSavesetting)
	ON_BN_CLICKED(IDC_UseProtection_OverVoltge, &CLoadCtrlDlg::OnBnClickedUseprotectionOvervoltge)
END_MESSAGE_MAP()


// CLoadCtrlDlg 메시지 처리기


void CLoadCtrlDlg::OnBnClickedSavesetting()
{
	if (IsDlgButtonChecked(IDC_UseProtection_OverVoltge) == false) {
		SetDlgItemText(IDC_OVP, "3000");
	}

	if (IsDlgButtonChecked(IDC_UseProtection_OverCurrent) == false) {
		SetDlgItemText(IDC_OCP, "10.000");
	}


	pMain->m_LoadSetting.useOverVoltProtection = IsDlgButtonChecked(IDC_UseProtection_OverVoltge);
	pMain->m_LoadSetting.useOverCurProtection = IsDlgButtonChecked(IDC_UseProtection_OverCurrent);
	pMain->m_LoadSetting.useBeep = IsDlgButtonChecked(IDC_UseBeep);
	pMain->m_LoadSetting.useMinSpeed = IsDlgButtonChecked(IDC_UseMinSpeed);

	CString str;
	pMain->m_LoadSetting.iMinSpeed_control = GetDlgItemInt(IDC_MinCtrlRpm);

	GetDlgItemText(IDC_OVP, str);
	pMain->m_LoadSetting.overVoltageLevel = (int)(100.f * atof(str) + 0.1f);

	GetDlgItemText(IDC_OCP, str);
	pMain->m_LoadSetting.overCurrentLevel = atof(str) + 0.0001f;
	
	pMain->m_bLoadControlSetting = true;
	pMain->Wait(50);
	pMain->Send_LoadPacket(PS_REQ_SET_BEEP, pMain->m_LoadSetting.useBeep);
	pMain->m_bLoadControlSetting = false;


	OnOK();
}


BOOL CLoadCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CheckDlgButton(IDC_UseProtection_OverVoltge, pMain->m_LoadSetting.useOverVoltProtection);
	CheckDlgButton(IDC_UseProtection_OverCurrent, pMain->m_LoadSetting.useOverCurProtection);
	CheckDlgButton(IDC_UseBeep, pMain->m_LoadSetting.useBeep);
	CheckDlgButton(IDC_UseMinSpeed, pMain->m_LoadSetting.useMinSpeed);

	SetDlgItemInt(IDC_MinCtrlRpm, pMain->m_LoadSetting.iMinSpeed_control);

	CString str;
	str.Format("%.2f", pMain->m_LoadSetting.overVoltageLevel * .01f + 0.001f); 
	SetDlgItemText(IDC_OVP, str);

	str.Format("%.3f", pMain->m_LoadSetting.overCurrentLevel + 0.0001f);
	SetDlgItemText(IDC_OCP, str);	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLoadCtrlDlg::OnBnClickedUseprotectionOvervoltge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
