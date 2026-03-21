// XInputDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialComDlg.h"
#include "XInputDlg.h"
#include "afxdialogex.h"


// CXInputDlg 대화 상자
#define XPAD_POSI_X 129
#define XPAD_POSI_Y 357  
#define XPAD_SIZE_X 261
#define XPAD_SIZE_Y 181

#define XPAD_POSI_XS 201
#define XPAD_POSI_YS 434  
#define XPAD_SIZE_XS 190
#define XPAD_SIZE_YS 132


IMPLEMENT_DYNAMIC(CXInputDlg, CDialogEx)

CXInputDlg::CXInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XInput, pParent)
	, m_iEnableKeyMode(0)
{

}

CXInputDlg::~CXInputDlg()
{
}

void CXInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXInputDlg, CDialogEx)
    ON_BN_CLICKED(IDC_UseEnableKey, &CXInputDlg::OnBnClickedUseenablekey)
    ON_BN_CLICKED(IDC_Apply, &CXInputDlg::OnBnClickedApply)
    ON_EN_CHANGE(IDC_SendChar0, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar1, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar2, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar3, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar4, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar5, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar6, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendChar7, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharNeg0, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharNeg1, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharNeg2, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharNeg3, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_EnableKey_Threshold_LT, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_EnableKey_Threshold_RT, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharTh0, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharTh1, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharTh2, &CXInputDlg::UpdateControl)
    ON_EN_CHANGE(IDC_SendCharTh3, &CXInputDlg::UpdateControl)
    ON_BN_CLICKED(IDC_Exit, &CXInputDlg::OnBnClickedExit)
    ON_BN_CLICKED(IDC_Reload, &CXInputDlg::OnBnClickedReload)
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_UsePacketHeader, &CXInputDlg::OnBnClickedUsepacketheader)
    ON_EN_CHANGE(IDC_XInputHeader, &CXInputDlg::OnEnChangeXinputheader)
    ON_BN_CLICKED(IDC_SendType, &CXInputDlg::OnChangeSendtype)
    ON_BN_CLICKED(IDC_SendType2, &CXInputDlg::OnChangeSendtype)
    ON_EN_CHANGE(IDC_PacketSendPeriod, &CXInputDlg::OnEnChangePacketsendperiod)
END_MESSAGE_MAP()


// CXInputDlg 메시지 처리기
void CXInputDlg::OnBnClickedUseenablekey()
{
	BOOL bEnable = IsDlgButtonChecked(IDC_UseEnableKey);
	ChangeEnableKey(bEnable);
}

void CXInputDlg::ChangeEnableKey(BOOL bEnable)
{
	GetDlgItem(IDC_EnableKeyMode0)->EnableWindow(bEnable);
	GetDlgItem(IDC_EnableKeyMode1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EnableKeyUseLB)->EnableWindow(bEnable);
	GetDlgItem(IDC_EnableKeyUseRB)->EnableWindow(bEnable);
	GetDlgItem(IDC_EnableKey_Threshold_LT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EnableKey_Threshold_RT)->EnableWindow(bEnable);
}


BOOL CXInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();    
    SetTimer(100, 40, NULL); 
    SetTimer(101, 100, NULL);
    SetTimer(102, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CXInputDlg::Init_DC() {
    CClientDC dc(this);
    m_MemDC.CreateCompatibleDC(&dc);
    m_Bitmap.CreateCompatibleBitmap(&dc, XPAD_SIZE_X, XPAD_SIZE_Y);
    m_pOldBitmap = m_MemDC.SelectObject(&m_Bitmap);

    m_MemDC2.CreateCompatibleDC(&dc);
    m_Bitmap2.CreateCompatibleBitmap(&dc, XPAD_SIZE_X, XPAD_SIZE_Y);
    m_pOldBitmap2 = m_MemDC2.SelectObject(&m_Bitmap2);

    m_MemDC.BitBlt(0, 0, XPAD_SIZE_X, XPAD_SIZE_Y, &dc, XPAD_POSI_X, XPAD_POSI_Y, SRCCOPY);
    m_MemDC2.BitBlt(0, 0, XPAD_SIZE_X, XPAD_SIZE_Y, &dc, XPAD_POSI_X, XPAD_POSI_Y, SRCCOPY);
    GetDlgItem(IDC_STATIC_pad)->ShowWindow(FALSE);
}


void CXInputDlg::Get_XInputSetting_from_Main()
{
    XINPUT_SETTING_T* pData = &(pMain->m_XInputSetting);
    // Type
    if (pData->iSendType == 0) {
        CheckDlgButton(IDC_SendType, TRUE);
    }
    else {
        CheckDlgButton(IDC_SendType2, TRUE);
    }
    
    // Checksum
    CheckDlgButton(IDC_UseChecksum, pData->bUseChecksum);

    // Header
    CheckDlgButton(IDC_UsePacketHeader, pData->bUseHeader);
    CString str = "", strT = "";
    for (int i = 0; i < pData->nHeader; i++) {
        strT.Format("%0X", pData->header[i]);
        str += strT;
    }
    SetDlgItemText(IDC_XInputHeader, str);
    OnBnClickedUsepacketheader();

    CheckDlgButton(IDC_UseEnableKey, pData->bUseEnableKey);
    if (pData->iEnableKeyMode == 0)  {
        CheckDlgButton(IDC_EnableKeyMode0, TRUE);
        CheckDlgButton(IDC_EnableKeyMode1, FALSE);
    }
    else {
        CheckDlgButton(IDC_EnableKeyMode0, FALSE);
        CheckDlgButton(IDC_EnableKeyMode1, TRUE);        
    }

    CheckDlgButton(IDC_EnableKeyUseLB, pData->bUseLB_EnableKey);
    CheckDlgButton(IDC_EnableKeyUseRB, pData->bUseRB_EnableKey);
    SetDlgItemInt(IDC_EnableKey_Threshold_LT, pData->iLT_Threshold_EnableKey);
    SetDlgItemInt(IDC_EnableKey_Threshold_RT, pData->iRT_Threshold_EnableKey);

    for (int i = 0; i < 8; i++) {
        CheckDlgButton(m_idc_use[i], pData->bUseSendData[i]);
        SetDlgItemText(m_idc_ch_p[i], pData->strSendData[i]);
    }

    for (int i = 0; i < 4; i++) {
        SetDlgItemInt(m_idc_th[i], pData->iSendThreshold[i]);
        SetDlgItemText(m_idc_ch_n[i], pData->strSendData_negative[i]);
    }

	CheckDlgButton(IDC_UseSendButton, pData->bUseSendButton);
	CheckDlgButton(IDC_ShowUartSendData, pData->bShowUartSend);

    OnChangeSendtype();
    UpdateControl();
    SetDlgItemInt(IDC_PacketSendPeriod, pData->iUpdatePeriod);
}

void CXInputDlg::Set_XInputSetting_from_Control()
{    
    XINPUT_SETTING_T* pData = &(pMain->m_XInputSetting);
    CString strAddr, strValue;

    pData->iSendType = IsDlgButtonChecked(IDC_SendType2);
    strValue.Format("%d", pData->iSendType);
    strAddr = "Common\\Xinput";
    pMain->SetRegRoot_RegistryData(strAddr, "SendType", strValue);

    // Checksum
    pData->bUseChecksum = IsDlgButtonChecked(IDC_UseChecksum);
    strValue.Format("%d", pData->bUseChecksum);
    pMain->SetRegRoot_RegistryData(strAddr, "Use_Checksum", strValue);

    // Header
    pData->bUseHeader = IsDlgButtonChecked(IDC_UsePacketHeader);
    strAddr = "Common\\Xinput\\Header";
    strValue.Format("%d", pData->bUseHeader);
    pMain->SetRegRoot_RegistryData(strAddr, "Use_Header", strValue);

    GetDlgItemText(IDC_XInputHeader, strValue);
    pMain->SetRegRoot_RegistryData(strAddr, "Header", strValue);
    pMain->Update_XInput_Header(strValue, pData);

    // EnableKey
    pData->bUseEnableKey = IsDlgButtonChecked(IDC_UseEnableKey);
    strAddr = "Common\\Xinput\\EnableKey";
    strValue.Format("%d", pData->bUseEnableKey);
    pMain->SetRegRoot_RegistryData(strAddr, "EnbaleKey", strValue);

    if (IsDlgButtonChecked(IDC_EnableKeyMode0)) { pData->iEnableKeyMode = 0;    }
    else { pData->iEnableKeyMode = 1; }
    strValue.Format("%d", pData->iEnableKeyMode);
    pMain->SetRegRoot_RegistryData(strAddr, "EnbaleKeyMode", strValue);
    
    pData->bUseLB_EnableKey = IsDlgButtonChecked(IDC_EnableKeyUseLB);
    pData->bUseRB_EnableKey = IsDlgButtonChecked(IDC_EnableKeyUseRB);
    strValue.Format("%d", pData->bUseLB_EnableKey);
    pMain->SetRegRoot_RegistryData(strAddr, "UseLB_EnbaleKey", strValue);
    strValue.Format("%d", pData->bUseRB_EnableKey);
    pMain->SetRegRoot_RegistryData(strAddr, "UseRB_EnbaleKey", strValue);  

    pData->iLT_Threshold_EnableKey = GetDlgItemInt(IDC_EnableKey_Threshold_LT);
    pData->iRT_Threshold_EnableKey = GetDlgItemInt(IDC_EnableKey_Threshold_RT);
    strValue.Format("%d", pData->iLT_Threshold_EnableKey);
    pMain->SetRegRoot_RegistryData(strAddr, "LT_Threshold_EnbaleKey", strValue);
    strValue.Format("%d", pData->iRT_Threshold_EnableKey);
    pMain->SetRegRoot_RegistryData(strAddr, "RT_Threshold_EnbaleKey", strValue);

    // Send Data
    for (int i = 0; i < 8; i++) {
        strAddr.Format("Common\\Xinput\\SendData\\item%d", i);
        pData->bUseSendData[i] = IsDlgButtonChecked(m_idc_use[i]);
        strValue.Format("%d", pData->bUseSendData[i]);
        pMain->SetRegRoot_RegistryData(strAddr, "UseSendData", strValue);        
        
        GetDlgItemText(m_idc_ch_p[i], pData->strSendData[i]);
        strValue.Format("%s", (LPCTSTR)pData->strSendData[i]);
        pMain->SetRegRoot_RegistryData(strAddr, "SendData", strValue);
     
        if (i < 4) {
            pData->iSendThreshold[i] = GetDlgItemInt(m_idc_th[i]);
            strValue.Format("%d", pData->iSendThreshold[i]);
            pMain->SetRegRoot_RegistryData(strAddr, "Threshold", strValue);

            GetDlgItemText(m_idc_ch_n[i], pData->strSendData_negative[i]);            
            strValue.Format("%s", (LPCTSTR)pData->strSendData_negative[i]);
            pMain->SetRegRoot_RegistryData(strAddr, "SendData_Negative", strValue);
        }
    }  

	// Add Button Info in Packet Data
	strAddr.Format("Common\\Xinput\\SendData");
	pData->bUseSendButton = IsDlgButtonChecked(IDC_UseSendButton);
	strValue.Format("%d", pData->bUseSendButton);
	pMain->SetRegRoot_RegistryData(strAddr, "UseButtonData", strValue);

    // Update 주기설정
    int val = GetDlgItemInt(IDC_PacketSendPeriod);
    strValue.Format("%d", val);
    strAddr.Format("Common\\Xinput");
    pMain->SetRegRoot_RegistryData(strAddr, "UpdatePeriod", strValue);
    pData->iUpdatePeriod = val;
    pMain->SetTimer(TIMER_XINPUT_CHECK, val, NULL);

	// Show Uart Send Data in Staus window
	strAddr.Format("Common\\Xinput\\SendData");
	pData->bShowUartSend = IsDlgButtonChecked(IDC_ShowUartSendData);
	strValue.Format("%d", pData->bShowUartSend);
	pMain->SetRegRoot_RegistryData(strAddr, "Show_UartSend", strValue);
}

void CXInputDlg::OnBnClickedApply()
{
    Set_XInputSetting_from_Control();
    OnOK();
}

void CXInputDlg::UpdateControl()
{
    char arr[5+1];
    int sz;
    CString str = "", str2 = "";

    for (int i = 0; i <8; i++) {
        GetDlgItemText(m_idc_ch_p[i], str);
        int size = str.GetLength();
        sz = 0;
        for (int j = 0; j < size; j++) {
            char ch = str.GetAt(j);
            //if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
            if (ch >= 33 && ch <= 126) {
                arr[sz++] = ch;
                if (sz >= 5) { break; }
            }
        }
        arr[sz] = 0;
        str2.Format("%s", arr);
        if (str != str2) {
            SetDlgItemText(m_idc_ch_p[i], str2);
        }
        SetDlgItemInt(m_idc_SZ[i], str2.GetLength());
    }

    for (int i = 0; i < 4; i++) {
        GetDlgItemText(m_idc_ch_n[i], str);
        int size = str.GetLength();
        sz = 0;
        for (int j = 0; j < size; j++) {
            char ch = str.GetAt(j);
            if (ch >= 33 && ch <= 126) {
                arr[sz++] = ch;
                if (sz >= 5) { break; }
            }
        }
        arr[sz] = 0;
        str2.Format("%s", arr);
        if (str != str2) {
            SetDlgItemText(m_idc_ch_n[i], str2);
        }
        SetDlgItemInt(m_idc_SZ_n[i], str2.GetLength());
    }

    // LB RB  (0~255)
    int idc_thumb[2] = { IDC_EnableKey_Threshold_LT, IDC_EnableKey_Threshold_RT };
    int max_val = 255;
    for (int i = 0; i < 2; i++) {
        GetDlgItemText(idc_thumb[i], str);
        int val = GetDlgItemInt(idc_thumb[i]);
        
        if (val < 0 || (val == 0 && str != "0")) { SetDlgItemInt(idc_thumb[i], 0);  }
        if (val > max_val) { SetDlgItemInt(idc_thumb[i], max_val); }
    }

    int idc_threshold[4] = { IDC_SendCharTh0, IDC_SendCharTh1, IDC_SendCharTh2, IDC_SendCharTh3 };
    max_val = 32767;
    for (int i = 0; i < 4; i++) {
        GetDlgItemText(idc_threshold[i], str);
        int val = GetDlgItemInt(idc_threshold[i]);
        
        if (val < 0 || (val == 0 && str != "0")) { SetDlgItemInt(idc_threshold[i], 0); }
        if (val > max_val) { SetDlgItemInt(idc_threshold[i], max_val); }        
    }
}


void CXInputDlg::OnBnClickedExit()
{
	Set_XInputSetting_from_Control();
}


void CXInputDlg::OnBnClickedReload()
{
    Get_XInputSetting_from_Main();
}

void CXInputDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent) {
    case 100: 
        Update_XPad();
        break;

    case 101:
        KillTimer(101);
        Init_DC();
        Get_XInputSetting_from_Main();        
        break;

    case 102:
        Show_XPad();
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CXInputDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CString str;
    str.Format("%d, %d", point.x, point.y);
    SetDlgItemText(IDC_Temp, str);
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CXInputDlg::Show_XPad() {
    CClientDC dc(this);
    static BOOL bShow = FALSE;
    if (IsDlgButtonChecked(IDC_SendType2)) {
        if (bShow) {
            dc.BitBlt(XPAD_POSI_X, XPAD_POSI_Y, XPAD_SIZE_X, XPAD_SIZE_Y, &m_MemDC, 0, 0, SRCCOPY);
            bShow = FALSE;
        }
        else {
            dc.BitBlt(XPAD_POSI_X, XPAD_POSI_Y, XPAD_SIZE_X, XPAD_SIZE_Y, &m_MemDC2, 0, 0, SRCCOPY);
            bShow = TRUE;
        }
    }
    else {
        if (bShow) {
            dc.StretchBlt(XPAD_POSI_XS, XPAD_POSI_YS, XPAD_SIZE_XS, XPAD_SIZE_YS, &m_MemDC2, 0, 0, XPAD_SIZE_X, XPAD_SIZE_Y, SRCCOPY);
            bShow = FALSE;
        }
        else {
            dc.StretchBlt(XPAD_POSI_XS, XPAD_POSI_YS, XPAD_SIZE_XS, XPAD_SIZE_YS, &m_MemDC, 0, 0, XPAD_SIZE_X, XPAD_SIZE_Y, SRCCOPY);
            bShow = TRUE;
        }
    }
}

void CXInputDlg::Update_XPad() {
	static CString str = "";

	if (str != pMain->m_strXinput) {
		str = pMain->m_strXinput;
		SetDlgItemText(IDC_Temp, str);
	}
	m_MemDC2.BitBlt(0, 0, XPAD_SIZE_X, XPAD_SIZE_Y, &m_MemDC, 0, 0, SRCCOPY);

	CPen pen;

	if (pMain->m_bXInput_EnableKey) {
		pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	}
	else {
		pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	}
	m_MemDC2.SelectObject(&pen);

	static int px[8] = { 107, 94, 179, 166,  202, 221, 185, 203 };
	static int py[8] = { 121, 109, 121, 109,  100, 82, 82, 64 };
	static int npx[4] = { 82, 94, 154, 166 };
	static int npy[4] = { 121, 134, 121, 134 };
	static int r[8] = { 4, 4, 4, 4, 8, 8, 8, 8 };

	static int x[4] = { 58, 204, 56, 203 };  //LB, RB, LT, RT
	static int y[4] = { 22, 22, 8, 11 };
	static int rec[4] = { 6, 6, 6, 6 };

	if (pMain->m_bGetXInputData || m_bShowChecked) {
		XINPUT_GAMEPAD pad;
		memcpy(&pad, &(pMain->m_XInput_state.Gamepad), sizeof(pad));

		// LX
		if (pad.sThumbLX > 0 || m_bShowChecked) { m_MemDC2.Ellipse(px[0] - r[0], py[0] - r[0], px[0] + r[0], py[0] + r[0]); }
		if (pad.sThumbLX < 0 || m_bShowChecked) { m_MemDC2.Ellipse(npx[0] - r[0], npy[0] - r[0], npx[0] + r[0], npy[0] + r[0]); }

		if (pad.sThumbLY > 0 || m_bShowChecked) { m_MemDC2.Ellipse(px[1] - r[1], py[1] - r[1], px[1] + r[1], py[1] + r[1]); }
		if (pad.sThumbLY < 0 || m_bShowChecked) { m_MemDC2.Ellipse(npx[1] - r[1], npy[1] - r[1], npx[1] + r[1], npy[1] + r[1]); }

		if (pad.sThumbRX > 0 || m_bShowChecked) { m_MemDC2.Ellipse(px[2] - r[2], py[2] - r[2], px[2] + r[2], py[2] + r[2]); }
		if (pad.sThumbRX < 0 || m_bShowChecked) { m_MemDC2.Ellipse(npx[2] - r[2], npy[2] - r[2], npx[2] + r[2], npy[2] + r[2]); }

		if (pad.sThumbRY > 0 || m_bShowChecked) { m_MemDC2.Ellipse(px[3] - r[3], py[3] - r[3], px[3] + r[3], py[3] + r[3]); }
		if (pad.sThumbRY < 0 || m_bShowChecked) { m_MemDC2.Ellipse(npx[3] - r[3], npy[3] - r[3], npx[3] + r[3], npy[3] + r[3]); }

		if ((pad.wButtons & XINPUT_GAMEPAD_A) || m_bShowChecked) { m_MemDC2.Ellipse(px[4] - r[4], py[4] - r[4], px[4] + r[4], py[4] + r[4]); }
		if ((pad.wButtons & XINPUT_GAMEPAD_B) || m_bShowChecked) { m_MemDC2.Ellipse(px[5] - r[5], py[5] - r[5], px[5] + r[5], py[5] + r[5]); }
		if ((pad.wButtons & XINPUT_GAMEPAD_X) || m_bShowChecked) { m_MemDC2.Ellipse(px[6] - r[6], py[6] - r[6], px[6] + r[6], py[6] + r[6]); }
		if ((pad.wButtons & XINPUT_GAMEPAD_Y) || m_bShowChecked) { m_MemDC2.Ellipse(px[7] - r[7], py[7] - r[7], px[7] + r[7], py[7] + r[7]); }

		if ((pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) || m_bShowChecked) { m_MemDC2.Ellipse(x[0] - r[0], y[0] - r[0], x[0] + r[0], y[0] + r[0]); }
		if ((pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) || m_bShowChecked) { m_MemDC2.Ellipse(x[1] - r[1], y[1] - r[1], x[1] + r[1], y[1] + r[1]); }
		if ((pad.bLeftTrigger > 0) || m_bShowChecked) { m_MemDC2.Ellipse(x[2] - r[2], y[2] - r[2], x[2] + r[2], y[2] + r[2]); }
		if ((pad.bRightTrigger > 0) || m_bShowChecked) { m_MemDC2.Ellipse(x[3] - r[3], y[3] - r[3], x[3] + r[3], y[3] + r[3]); }

		int size = 0;
		BYTE* arr = pMain->Make_XInput_UartData_Data(pad, &size);
		
		int nHead = 0;
		if (pMain->m_XInputSetting.bUseHeader) {
			nHead = pMain->m_XInputSetting.nHeader;
		}
		if (pMain->m_XInputSetting.iSendType == 1 && pMain->m_XInputSetting.bUseChecksum) {
			nHead += 1;
		}

		if (size > nHead) {
			str.Format("Packet HEX (%dByte) : %s", size, pMain->XInput_SendData_hex(arr, size));
			SetDlgItemText(IDC_Packet, str);
			str.Format("Packet ASCII : %s", arr);
			SetDlgItemText(IDC_Packet2, str);
		}
		else {
			SetDlgItemText(IDC_Packet, "Packet HEX (%dByte) :");
			SetDlgItemText(IDC_Packet2, "Packet ASCII :");
		}
	}
	else {
			SetDlgItemText(IDC_Packet, "Packet HEX (%dByte) :");
			SetDlgItemText(IDC_Packet2, "Packet ASCII :");
	}
}


void CXInputDlg::OnBnClickedUsepacketheader()
{
    BOOL bUse = IsDlgButtonChecked(IDC_UsePacketHeader);
    GetDlgItem(IDC_XInputHeader)->EnableWindow(bUse);
}


void CXInputDlg::OnEnChangeXinputheader() {
    CString str = "", str1 = "", strT = "";
    XINPUT_SETTING_T Data;
    GetDlgItemText(IDC_XInputHeader, str);    
    pMain->Update_XInput_Header(str, &Data);

    for (int i = 0; i < Data.nHeader; i++) {
        if (i == 0) {
            strT.Format("%X", Data.header[i]);
        }
        else {
            strT.Format("%0X", Data.header[i]);
        }
        str1 += strT;
    }

    if (str1 != str) {
        SetDlgItemText(IDC_XInputHeader, str1);
    }
}


void CXInputDlg::OnChangeSendtype() {
    Invalidate(TRUE);

    BOOL bShow = TRUE;
    if(IsDlgButtonChecked(IDC_SendType2)) {
        bShow = FALSE;		
    }
    
    for (int i = 0; i < 8; i++) {
        GetDlgItem(m_idc_ch_p[i])->ShowWindow(bShow);
        GetDlgItem(m_idc_SZ[i])->ShowWindow(bShow);
    }
    for (int i = 0; i < 4; i++) {
        GetDlgItem(m_idc_ch_n[i])->ShowWindow(bShow);
        GetDlgItem(m_idc_th[i])->ShowWindow(bShow);
        GetDlgItem(m_idc_SZ_n[i])->ShowWindow(bShow);
    }
    for (int i = 0; i < 4; i++) {
        GetDlgItem(m_idc_static[i])->ShowWindow(bShow);
    }

	GetDlgItem(IDC_PressButton4)->ShowWindow(bShow);
	GetDlgItem(IDC_PressButton5)->ShowWindow(bShow);
	GetDlgItem(IDC_PressButton6)->ShowWindow(bShow);
	GetDlgItem(IDC_PressButton7)->ShowWindow(bShow);

    GetDlgItem(IDC_UseChecksum)->ShowWindow(!bShow);
	GetDlgItem(IDC_STATIC_UpdateTime)->ShowWindow(!bShow);
	GetDlgItem(IDC_PacketSendPeriod)->ShowWindow(!bShow);
	GetDlgItem(IDC_UseSendButton)->ShowWindow(!bShow);
}

void CXInputDlg::Circle(CClientDC* pDC, int x, int y, int r) {
    int N = 10;
    pDC->MoveTo(x + r, y);
    for (int i = 1; i <= N; i++) {
        double ang = M_PI2 / N * i;
        pDC->LineTo(x + (int)(r * cos(ang)), y + (int)(r * sin(ang)));
    }
}


void CXInputDlg::OnEnChangePacketsendperiod() {
    CString str, str2;
    int val = GetDlgItemInt(IDC_PacketSendPeriod);
    GetDlgItemText(IDC_PacketSendPeriod, str);
    str2.Format("%d", max(10, val));
    if (str != str2) {
        SetDlgItemText(IDC_PacketSendPeriod, str2);
    }
}
