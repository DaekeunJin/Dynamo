// PortListDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "PortListDlg.h"
#include "afxdialogex.h"


// CPortListDlg 대화 상자

IMPLEMENT_DYNAMIC(CPortListDlg, CDialogEx)

CPortListDlg::CPortListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ComPortList, pParent)
	,m_iSelectItem(-1)
{

}

CPortListDlg::~CPortListDlg()
{
}

void CPortListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PortList, m_ctrlComList);
}


BEGIN_MESSAGE_MAP(CPortListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_AddPort, &CPortListDlg::OnBnClickedAddport)
	ON_EN_CHANGE(IDC_ComNo, &CPortListDlg::OnEnChangeComno)
	ON_NOTIFY(NM_CLICK, IDC_PortList, &CPortListDlg::OnClickPortlist)
	ON_BN_CLICKED(IDC_ModifyList, &CPortListDlg::OnBnClickedModifylist)
	ON_BN_CLICKED(IDC_DeleteList, &CPortListDlg::OnBnClickedDeletelist)
	ON_BN_CLICKED(IDC_SavePort, &CPortListDlg::OnBnClickedSaveport)
	ON_BN_CLICKED(IDC_ConnectList, &CPortListDlg::OnBnClickedConnectlist)
	ON_NOTIFY(NM_DBLCLK, IDC_PortList, &CPortListDlg::OnDblclkPortlist)
END_MESSAGE_MAP()


// CPortListDlg 메시지 처리기


BOOL CPortListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init_ComPortList();

	return TRUE; 
}

#define N_ITEM_COM_LIST 6
void CPortListDlg::Init_ComPortList() {
	LV_COLUMN lvcolumn;
	
	for (int i = 0; i < N_ITEM_COM_LIST; i++) {
		m_ctrlComList.DeleteColumn(0);
	}

	char *listColumn[N_ITEM_COM_LIST] = { " No. ", "  Com Port  ",  "  Device ID  ", "       Descritpion      ", "Status", ""};
	int width[N_ITEM_COM_LIST] = { 1, 1, 1, 1 };


	for (int i = 0; i < N_ITEM_COM_LIST; i++)
	{
		if (i == 1)
			lvcolumn.fmt = LVCFMT_LEFT;
		else
			lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.pszText = listColumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		m_ctrlComList.InsertColumn(i, &lvcolumn);
	}	

	LV_ITEM lvitem;
	CString str;

	m_ctrlComList.DeleteAllItems();

	//m_cSerialPort.AddString(strCurrentAvailPort);

	for (int i = 0; i < pMain->m_iComPortCount; i++) {
		int nCount = m_ctrlComList.GetItemCount();
		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = 0;

		lvitem.iItem = nCount;
		str.Format("%d", i+1);
		lvitem.pszText = (LPSTR)(LPCSTR)str;
		m_ctrlComList.InsertItem(&lvitem);

		str.Format("COM%d", pMain->m_ComPortList[i].iComPort);
		m_ctrlComList.SetItemText(nCount, 1, str);

		str.Format("%s", (LPCTSTR)pMain->m_ComPortList[i].strPortID);
		m_ctrlComList.SetItemText(nCount, 2, str);

		str.Format("%s", (LPCTSTR)pMain->m_ComPortList[i].strDescription);
		m_ctrlComList.SetItemText(nCount, 3, str);
		BOOL bFind = FALSE;
		for (int j = 0; j < pMain->m_iConnectedPortCount; j++) {
			if (pMain->m_ComPortList[i].iComPort == pMain->m_iConnectComPort[j]) bFind = TRUE;
		}
		if (bFind) {
			m_ctrlComList.SetItemText(nCount, 4, "Available");
		}
		else {
			m_ctrlComList.SetItemText(nCount, 4, "-");
		}
	}
	   
	for (int i = 0; i < m_ctrlComList.GetHeaderCtrl()->GetItemCount(); i++) {
		VERIFY(m_ctrlComList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
	}

	m_ctrlComList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CPortListDlg::OnBnClickedAddport() {
	if (pMain->m_iComPortCount >= MAX_COM_PORT_SIZE) {
		AfxMessageBox("저장할수있는 List 갯수를 초과하였습니다.");
		return;
	}
	CString str1, str2;
	int iPort = max(1, min(100, GetDlgItemInt(IDC_ComNo)));
	str1.Format("%d", iPort);
	GetDlgItemText(IDC_ComNo, str2);

	if (str1 != str2) SetDlgItemText(IDC_ComNo, str1);

	CString strID, strDescription;
	GetDlgItemText(IDC_DeviceName, strID);
	GetDlgItemText(IDC_Description, strDescription);

	// ComPort Duplication Check
	for (int i = 0; i < pMain->m_iComPortCount; i++) {
		if (pMain->m_ComPortList[i].iComPort == iPort) {
			CString str;
			str.Format("이미 COM%d 설정이 있습니다. 삭제후 추가하시기 바랍니다.", iPort);
			AfxMessageBox(str);
			return;
		}
	}

	pMain->m_ComPortList[pMain->m_iComPortCount].iComPort = iPort;
	pMain->m_ComPortList[pMain->m_iComPortCount].strPortID.Format("%s", (LPCTSTR)strID);
	pMain->m_ComPortList[pMain->m_iComPortCount].strDescription.Format("%s", (LPCTSTR)strDescription);
	pMain->m_iComPortCount++;

	Init_ComPortList();
}


void CPortListDlg::OnEnChangeComno() {
	CString str1, str2;
	str1.Format("%d", max(1, min(100, GetDlgItemInt(IDC_ComNo))));
	GetDlgItemText(IDC_ComNo, str2);

	if (str1 != str2) SetDlgItemText(IDC_ComNo, str1);
}


void CPortListDlg::OnClickPortlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_iSelectItem = pNMItemActivate->iItem;
	if (m_iSelectItem < 0) return;
	
	SetDlgItemInt(IDC_ComNo, pMain->Str2Num(m_ctrlComList.GetItemText(m_iSelectItem, 1)));	
	SetDlgItemText(IDC_DeviceName, m_ctrlComList.GetItemText(m_iSelectItem, 2));
	SetDlgItemText(IDC_Description, m_ctrlComList.GetItemText(m_iSelectItem, 3));

	*pResult = 0;
}

void CPortListDlg::OnBnClickedModifylist() {
	if (m_iSelectItem < 0) {
		AfxMessageBox("수정하기위해 선택된 항목이 없습니다.");
		return;
	}

	int iPort = GetDlgItemInt(IDC_ComNo);
	CString strID, strDescription, str;
	GetDlgItemText(IDC_DeviceName, strID);
	GetDlgItemText(IDC_Description, strDescription);

	// ComPort Duplication Check
	for (int i = 0; i < pMain->m_iComPortCount; i++) {
		if (i == m_iSelectItem) continue;
		if (pMain->m_ComPortList[i].iComPort == iPort) {
			CString str;
			str.Format("이미 COM%d 설정이 있어 수정을 취소합니다.", iPort);
			AfxMessageBox(str);
			return;
		}
	}

	pMain->m_ComPortList[m_iSelectItem].iComPort = iPort;
	pMain->m_ComPortList[m_iSelectItem].strPortID.Format("%s", (LPCTSTR)strID);
	pMain->m_ComPortList[m_iSelectItem].strDescription.Format("%s", (LPCTSTR)strDescription);

	m_iSelectItem = -1;
	Init_ComPortList();

	str = "";
	SetDlgItemText(IDC_ComNo, str);
	SetDlgItemText(IDC_DeviceName, str);
	SetDlgItemText(IDC_Description, str);
}


void CPortListDlg::OnBnClickedDeletelist() {
	if (m_iSelectItem < 0) {
		AfxMessageBox("삭제할 항목이 선택되지 않았습니다.");
		return;
	}

	int iPort = GetDlgItemInt(IDC_ComNo);
	CString strID, strDescription, str;
	GetDlgItemText(IDC_DeviceName, strID);
	GetDlgItemText(IDC_Description, strDescription);

	// ComPort Duplication Check
	for (int i = m_iSelectItem + 1; i < pMain->m_iComPortCount; i++) {
		pMain->m_ComPortList[i - 1].iComPort = pMain->m_ComPortList[i].iComPort;
		pMain->m_ComPortList[i - 1].strPortID = pMain->m_ComPortList[i].strPortID;
		pMain->m_ComPortList[i - 1].strDescription = pMain->m_ComPortList[i].strDescription;
	}	

	pMain->m_iComPortCount--;

	m_iSelectItem = -1;
	SetDlgItemText(IDD_HeaderLogDlg, "");
	Init_ComPortList();

	str = "";
	SetDlgItemText(IDC_ComNo, str);
	SetDlgItemText(IDC_DeviceName, str);
	SetDlgItemText(IDC_Description, str);
}


void CPortListDlg::OnBnClickedSaveport() {
	pMain->SaveComPortList();
}


void CPortListDlg::OnBnClickedConnectlist() {
	if (m_iSelectItem < 0) {
		AfxMessageBox("삭제할 항목이 선택되지 않았습니다.");
		return;
	}

	if (m_ctrlComList.GetItemText(m_iSelectItem, 4) == "-") {
		AfxMessageBox("선택된 Port가 활성화 되지 않았습니다.  OTL");
		return;
	}

	for (int i = 0; i < pMain->m_iConnectedPortCount; i++) {
		if (pMain->m_ComPortList[m_iSelectItem].iComPort == pMain->m_iConnectComPort[i]) {
			if (pMain->m_ComuPort.m_bConnected != TRUE) {
				pMain->m_cSerialPort.SetCurSel(i);
				pMain->OnPortOpen1();
				OnOK();
			}
			else {
				AfxMessageBox("전 Port가 이미 사용중입니다..  OTL");
				return;
			}
		}
	}
}


void CPortListDlg::OnDblclkPortlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	m_iSelectItem = pNMItemActivate->iItem;
	if (m_iSelectItem < 0) return;
	OnBnClickedConnectlist();
}
