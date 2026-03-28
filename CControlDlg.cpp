// CControlDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "CControlDlg.h"
#include "afxdialogex.h"


// CControlDlg 대화 상자

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ControlDlg, pParent)
	, m_iPreset(-1)
	, m_iControl(1)
	, m_bProhibtMessageUpdate(FALSE)
{
	int IDC_DecInput[N_MAX_Control] = { IDC_Parameter0, IDC_Parameter1, IDC_Parameter2, IDC_Parameter3, IDC_Parameter4, IDC_Parameter5, IDC_Parameter6, IDC_Parameter7, IDC_Parameter8, IDC_Parameter9 };
	int IDC_Label[N_MAX_Control] = { IDC_ParameterLabel0, IDC_ParameterLabel1, IDC_ParameterLabel2, IDC_ParameterLabel3, IDC_ParameterLabel4, IDC_ParameterLabel5, IDC_ParameterLabel6, IDC_ParameterLabel7, IDC_ParameterLabel8, IDC_ParameterLabel9 };
	int IDC_HexInput[N_MAX_Control] = { IDC_Parameter10, IDC_Parameter11, IDC_Parameter12, IDC_Parameter13, IDC_Parameter14, IDC_Parameter15, IDC_Parameter16, IDC_Parameter17, IDC_Parameter18, IDC_Parameter19 };
	int __IDC_DataType[N_MAX_Control] = { IDC_ParamType0, IDC_ParamType1, IDC_ParamType2, IDC_ParamType3, IDC_ParamType4, IDC_ParamType5, IDC_ParamType6, IDC_ParamType7, IDC_ParamType8, IDC_ParamType9 };
	int IDC_preset[N_MAX_Preset] = { IDC_ControlMessage0, IDC_ControlMessage1, IDC_ControlMessage2, IDC_ControlMessage3, IDC_ControlMessage4, IDC_ControlMessage5, IDC_ControlMessage6, IDC_ControlMessage7, IDC_ControlMessage8, IDC_ControlMessage9, IDC_ControlMessage10 };

	for (int i = 0; i < N_MAX_Control; i++) m_IDC_DecInput[i] = IDC_DecInput[i];
	for (int i = 0; i < N_MAX_Control; i++) m_IDC_Label[i] = IDC_Label[i];
	for (int i = 0; i < N_MAX_Control; i++) m_IDC_HexInput[i] = IDC_HexInput[i];
	for (int i = 0; i < N_MAX_Control; i++) m_IDC_DataType[i] = __IDC_DataType[i];
	for (int i = 0; i < N_MAX_Preset; i++) m_IDC_Preset[i] = IDC_preset[i];
}

CControlDlg::~CControlDlg() {
}

void CControlDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ControPreset0, m_iPreset);
	DDX_Radio(pDX, IDC_ByteOrder0, m_iByteOrder);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_IncreasControl, &CControlDlg::OnBnClickedIncreascontrol)
	ON_EN_CHANGE(IDC_nVariables, &CControlDlg::OnEnChangenvariables)
	ON_BN_CLICKED(IDC_DecreasControl2, &CControlDlg::OnBnClickedDecreascontrol2)
	ON_BN_CLICKED(IDC_ControPreset0, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset1, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset2, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset3, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset4, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset5, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset6, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset7, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset8, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset9, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_ControPreset10, &CControlDlg::OnBnClickedContropreset)
	ON_BN_CLICKED(IDC_SavePreset, &CControlDlg::OnBnClickedSavepreset)
	ON_EN_CHANGE(IDC_Header, &CControlDlg::OnEnChangeHeader)
	ON_BN_CLICKED(IDC_SendCommand, &CControlDlg::OnBnClickedSendcommand)
	ON_EN_CHANGE(IDC_Parameter0, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter1, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter2, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter3, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter4, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter5, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter6, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter7, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter8, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_Parameter9, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel0, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel1, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel2, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel3, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel4, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel5, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel6, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel7, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel8, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ParameterLabel9, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType0, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType1, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType2, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType3, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType4, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType5, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType6, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType7, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType8, &CControlDlg::OnEnChangeParameter)
	ON_CBN_SELCHANGE(IDC_ParamType9, &CControlDlg::OnEnChangeParameter)
	ON_EN_CHANGE(IDC_ControlMessage0, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage1, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage2, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage3, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage4, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage5, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage6, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage7, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage8, &CControlDlg::OnEnChangeControlmessage)
	ON_EN_CHANGE(IDC_ControlMessage9, &CControlDlg::OnEnChangeControlmessage)
	ON_BN_CLICKED(IDC_UseHeader, &CControlDlg::OnBnClickedUseheader)
	ON_EN_CHANGE(IDC_Convert_char, &CControlDlg::OnEnChangeConvertchar)
	ON_BN_CLICKED(IDC_FixDataType, &CControlDlg::OnBnClickedFixdatatype)
	ON_EN_CHANGE(IDC_Convert_Ascii4, &CControlDlg::OnEnChangeConvertAscii4)
	ON_EN_CHANGE(IDC_Convert_Ascii5, &CControlDlg::OnEnChangeConvertAscii5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ByteOrder0, &CControlDlg::OnBnClickedByteorder0)
	ON_BN_CLICKED(IDC_ByteOrder1, &CControlDlg::OnBnClickedByteorder1)
	ON_EN_CHANGE(IDC_float, &CControlDlg::OnEnChangefloat)
	ON_EN_CHANGE(IDC_Hex2, &CControlDlg::OnEnChangeHex2)
	ON_CBN_SELCHANGE(IDC_DataType, &CControlDlg::OnCbnSelchangeDatatype)
	ON_BN_CLICKED(IDC_RemoteCommand0, &CControlDlg::OnBnClickedRemotecommand0)
	ON_BN_CLICKED(IDC_RemoteCommand1, &CControlDlg::OnBnClickedRemotecommand1)
	ON_BN_CLICKED(IDC_RemoteCommand2, &CControlDlg::OnBnClickedRemotecommand2)
	ON_BN_CLICKED(IDC_RemoteCommand3, &CControlDlg::OnBnClickedRemotecommand3)
	ON_BN_CLICKED(IDC_RemoteCommand4, &CControlDlg::OnBnClickedRemotecommand4)
	ON_BN_CLICKED(IDC_RemoteCommand5, &CControlDlg::OnBnClickedRemotecommand5)
	ON_BN_CLICKED(IDC_RemoteCommand6, &CControlDlg::OnBnClickedRemotecommand6)
	ON_BN_CLICKED(IDC_RemoteCommand7, &CControlDlg::OnBnClickedRemotecommand7)
	ON_BN_CLICKED(IDC_RemoteCommand8, &CControlDlg::OnBnClickedRemotecommand8)
	ON_BN_CLICKED(IDC_RemoteCommand9, &CControlDlg::OnBnClickedRemotecommand9)
	ON_BN_CLICKED(IDC_RemoteCommand10, &CControlDlg::OnBnClickedRemotecommand10)
	ON_BN_CLICKED(IDC_RemoteCommand11, &CControlDlg::OnBnClickedRemotecommand11)
	ON_BN_CLICKED(IDC_RemoteCommand12, &CControlDlg::OnBnClickedRemotecommand12)
	ON_BN_CLICKED(IDC_RemoteCommand13, &CControlDlg::OnBnClickedRemotecommand13)
	ON_BN_CLICKED(IDC_RemoteCommand14, &CControlDlg::OnBnClickedRemotecommand14)
	ON_BN_CLICKED(IDC_RemoteCommand15, &CControlDlg::OnBnClickedRemotecommand15)
	ON_BN_CLICKED(IDC_RemoteCommand16, &CControlDlg::OnBnClickedRemotecommand16)
	ON_BN_CLICKED(IDC_RemoteCommand17, &CControlDlg::OnBnClickedRemotecommand17)
	ON_BN_CLICKED(IDC_RemoteCommand18, &CControlDlg::OnBnClickedRemotecommand18)
	ON_BN_CLICKED(IDC_RemoteCommand19, &CControlDlg::OnBnClickedRemotecommand19)
	ON_BN_CLICKED(IDC_DefaultButton2, &CControlDlg::OnBnClickedDefaultbutton2)
	ON_BN_CLICKED(IDC_ResetData, &CControlDlg::OnBnClickedResetdata)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ChangeSize, &CControlDlg::OnBnClickedChangesize)
	ON_BN_CLICKED(IDC_SendCommand2, &CControlDlg::OnBnClickedSendcommand2)
END_MESSAGE_MAP()


// CControlDlg 메시지 처리기
void CControlDlg::OnEnChangenvariables() {
	int iValue = GetDlgItemInt(IDC_nVariables);
	CString strValue = "";
	m_iControl = max(1, min(N_MAX_Control, iValue));
	if (iValue != m_iControl) SetDlgItemInt(IDC_nVariables, m_iControl);

	for (int i = 0; i < m_iControl; i++) {
		GetDlgItem(m_IDC_DecInput[i])->ShowWindow(TRUE);
		GetDlgItem(m_IDC_HexInput[i])->ShowWindow(TRUE);
		GetDlgItem(m_IDC_DataType[i])->ShowWindow(TRUE);
		GetDlgItem(m_IDC_Label[i])->ShowWindow(TRUE);
	}

	for (int i = m_iControl; i < N_MAX_Control; i++) {
		GetDlgItem(m_IDC_DecInput[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_Label[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_HexInput[i])->ShowWindow(FALSE);
		GetDlgItem(m_IDC_DataType[i])->ShowWindow(FALSE);
	}
	UpdateSendPacket();
}

void CControlDlg::OnBnClickedIncreascontrol() {
	int iValue = GetDlgItemInt(IDC_nVariables);
	iValue++;
	m_iControl = max(1, min(N_MAX_Control, iValue));
	SetDlgItemInt(IDC_nVariables, m_iControl);
	OnEnChangeParameter();
}

BOOL CControlDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SetDlgItemInt(IDC_nVariables, m_iControl);


	pMain = (CSerialComDlg*)GetParent();
	// Load PresetCmd
	CString str, strItem;
	for (int i = 0; i < N_MAX_Preset - 1; i++) {
		strItem.Format("PresetCmd_%d", i);
		str = pMain->GetReg_RegistryData("Common", strItem);
		SetDlgItemText(m_IDC_Preset[i], str);
		((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->SetCurSel(1);
	}
	str = pMain->GetReg_RegistryData("Common", "Cmd_Byte_Order");
	m_iByteOrder = atoi(str);

	if (m_iByteOrder == 0) {
		CheckDlgButton(IDC_ByteOrder1, FALSE);
		CheckDlgButton(IDC_ByteOrder0, TRUE);
	}
	else if (m_iByteOrder == 1) {
		CheckDlgButton(IDC_ByteOrder0, FALSE);
		CheckDlgButton(IDC_ByteOrder1, TRUE);
	}

	OnEnChangenvariables();
	CheckDlgButton(IDC_UseHeader, FALSE);

	((CComboBox *)GetDlgItem(IDC_DataType))->SetCurSel(0);
	GetDlgItem(IDC_DataType)->ShowWindow(FALSE);

	for (int i = 0; i < N_MAX_Control; i++) {
		SetDlgItemInt(m_IDC_DecInput[i], 0);
		SetDlgItemInt(m_IDC_HexInput[i], 0);
	}

	int idc_[MAX_BT_COMMAND] = { \
		IDC_RemoteCommand0, IDC_RemoteCommand1, IDC_RemoteCommand2, IDC_RemoteCommand3, IDC_RemoteCommand4, IDC_RemoteCommand5, IDC_RemoteCommand6, IDC_RemoteCommand7, IDC_RemoteCommand8, IDC_RemoteCommand9,\
		IDC_RemoteCommand10, IDC_RemoteCommand11, IDC_RemoteCommand12, IDC_RemoteCommand13, IDC_RemoteCommand14, IDC_RemoteCommand15, IDC_RemoteCommand16, IDC_RemoteCommand17, IDC_RemoteCommand18, IDC_RemoteCommand19,\
	};

	for (int i = 0; i < MAX_BT_COMMAND; i++) {
		if (pMain->m_BT_Command[i]) {
			str.Format("%c", pMain->m_BT_Command[i]);
			SetDlgItemText(idc_[i], str);
		}
		else {
			SetDlgItemText(idc_[i], " ");
		}
	}
	CheckDlgButton(IDC_SendPort1, true);
	UpdateChangeModeButtonTitle();
	return TRUE;  // return TRUE unless you set the focus to a contro
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CControlDlg::OnBnClickedSavepreset() {
	// Save PresetCmd
	CString str, strItem;
	for (int i = 0; i < N_MAX_Preset - 1; i++) {
		strItem.Format("PresetCmd_%d", i);
		GetDlgItemText(m_IDC_Preset[i], str);
		pMain->SetReg_RegistryData("Common", strItem, str);
	}

	str.Format("%d", m_iByteOrder);
	pMain->SetReg_RegistryData("Common", "Cmd_Byte_Order", str);
}

void CControlDlg::OnBnClickedDecreascontrol2() {
	int iValue = GetDlgItemInt(IDC_nVariables);
	iValue--;
	m_iControl = max(1, min(N_MAX_Control, iValue));

	SetDlgItemInt(IDC_nVariables, m_iControl);
	OnEnChangeParameter();
}


BOOL CControlDlg::UpdateDataType(int& dtype, char *arr, int sz) {
	if (strcmp(arr, "int32") == 0) { dtype = 0;  return TRUE; }
	else if (strcmp(arr, "int16") == 0) { dtype = 1; return TRUE; }
	else if (strcmp(arr, "int8") == 0) { dtype = 2; return TRUE; }
	else if (strcmp(arr, "uint32") == 0) { dtype = 3; return TRUE; }
	else if (strcmp(arr, "uint16") == 0) { dtype = 4; return TRUE; }
	else if (strcmp(arr, "uint8") == 0) { dtype = 5; return TRUE; }
	else if (strcmp(arr, "float") == 0) { dtype = 6; return TRUE; }
	else if (strcmp(arr, "4") == 0) { dtype = 0; strcpy_s(arr, 10, "int32"); return TRUE; }
	else if (strcmp(arr, "2") == 0) { dtype = 0; strcpy_s(arr, 10, "int16"); return TRUE; }
	else if (strcmp(arr, "1") == 0) { dtype = 0; strcpy_s(arr, 10, "int8"); return TRUE; }
	return FALSE;
}

void CControlDlg::OnBnClickedContropreset() {
	UDT;
	if (m_iPreset == -1) { return; }
	unsigned long uiPara[N_MAX_Control] = { 0, };
	signed long iPara[N_MAX_Control] = { 0, };
	float fPara[N_MAX_Control] = { 0, };

	m_bProhibtMessageUpdate = TRUE;
	CString str;
	GetDlgItemText(m_IDC_Preset[m_iPreset], str);
	int Length = str.GetLength();

	BOOL bUseHeader = FALSE;
	BYTE Header = 0;
	BOOL bHeaderCheck = TRUE;

	CString strLabel[N_MAX_Control] = { "", };
	int iStringIndex = 0;
	int iSequence = 0;
	int nPara = 0;
	BYTE aByte;

	CStringA strDec[N_MAX_Control];  // for string
	CStringA strHex[N_MAX_Control];  // for string

	int pre_dtype = ((CComboBox *)GetDlgItem(IDC_DataType))->GetCurSel();

	for (int para = nPara; para < N_MAX_Control; para++) {
		((CComboBox *)GetDlgItem(m_IDC_DataType[para]))->SetCurSel(pre_dtype);
	}

	BOOL bGetValue = FALSE;
	for (int i = 0; i < Length; i++) {
		aByte = str.GetAt(i);
		if (aByte == ' ') { continue; }
		else if (aByte == '{') {
			char arrType[10 + 1];
			int idx = 0;
			i++;
			while (i < Length) {
				char ch = str.GetAt(i++);
				if (ch >= 'a' && ch <= 'z') {
					arrType[idx++] = ch;
				}
				else if (ch >= 'A' && ch <= 'Z') {
					arrType[idx++] = ch - 'A' + 'a';
				}
				else if (ch >= '0' && ch <= '9') {
					arrType[idx++] = ch;
				}
				else if (ch == '}') {
					break;
				}
				if (idx > 9) {
					break;
				}
			}
			arrType[idx] = 0;
			if (strcmp(arrType, "uint32") == 0 || strcmp(arrType, "4") == 0) pre_dtype = 0;
			else if (strcmp(arrType, "uint16") == 0 || strcmp(arrType, "2") == 0) pre_dtype = 1;
			else if (strcmp(arrType, "uint8") == 0 || strcmp(arrType, "1") == 0) pre_dtype = 2;
			else if (strcmp(arrType, "int32") == 0) pre_dtype = 3;
			else if (strcmp(arrType, "int16") == 0) pre_dtype = 4;
			else if (strcmp(arrType, "int8") == 0) pre_dtype = 5;
			else if (strcmp(arrType, "float") == 0) pre_dtype = 6;
			else if (strcmp(arrType, "string") == 0) pre_dtype = 7;
			else { pre_dtype = 1; }
			for (int para = nPara; para < N_MAX_Control; para++) {
				((CComboBox *)GetDlgItem(m_IDC_DataType[para]))->SetCurSel(pre_dtype);
			}
		}
		else if (aByte == '[') {
			// Label
			if (nPara >= N_MAX_Control) { break; }
			char s[20];
			int idx = 0;
			while (++i < Length) {
				aByte = str.GetAt(i);
				if (aByte == ']') {
					break;
				}
				else {
					s[idx++] = aByte;
				}
			}
			s[idx] = 0;
			strLabel[nPara].Format("%s", s);
			bHeaderCheck = FALSE;
		}
		else if (('0' <= aByte && aByte <= '9') || aByte == '.' || aByte == '-' || aByte == '+') {
			// Value
			if (nPara >= N_MAX_Control) { break; }
			unsigned long val = 0;
			float order = 1;
			BOOL bPeriod = FALSE;
			BOOL bMinus = FALSE;
			BOOL bGetData = FALSE;

			if (aByte >= '0' && aByte <= '9') {
				bGetData = TRUE;
				val = val * 10 + aByte - '0';
			}
			else if (aByte == '.') { bPeriod = TRUE; }
			else if (aByte == '-') { bMinus = TRUE; }

			while (++i < Length) {
				if (nPara >= N_MAX_Control) { break; }
				aByte = str.GetAt(i);
				if (aByte >= '0' && aByte <= '9') {
					bGetData = TRUE;
					val = val * 10 + aByte - '0';
					if (bPeriod) { order *= 10; }
				}
				else if (aByte == '.') { bPeriod = TRUE; }
				else if (bGetData) {
					if (aByte != ' ') { --i; }
					break;
				}
			}
			switch (pre_dtype) {
			case 0: case 1: case 2:
				uiPara[nPara] = val; break;

			case 3: case 4: case 5:
				iPara[nPara] = val;
				if (bMinus) iPara[nPara] = -iPara[nPara];
				break;

			case 6:
				fPara[nPara] = (float)val;
				if (bMinus) fPara[nPara] = -fPara[nPara];
				fPara[nPara] /= order;
			}
			bHeaderCheck = FALSE;
			nPara++;
		}
		else if ('"' == aByte && pre_dtype == 7) {
			strDec[nPara] = "";
			strHex[nPara] = "";
			CStringA strTemp;
			while (++i < Length) {
				aByte = str.GetAt(i);
				if (aByte == '"') {
					break;
				}
				else {
					strDec[nPara] += aByte;
					strTemp.Format("%02X", aByte);
					strHex[nPara] += strTemp;
				}
			}
			bHeaderCheck = FALSE;
			nPara++;
		}
		else if (bHeaderCheck) {
			if (IsHead(aByte) != 0) {
				bUseHeader = TRUE;
				Header = aByte;
				bHeaderCheck = FALSE;
			}
		}
	}

	if (bGetValue && nPara < N_MAX_Control) {
		nPara++;
	}

	// Summary Data
	// Header 
	CheckDlgButton(IDC_UseHeader, bUseHeader);
	OnBnClickedUseheader();
	if (bUseHeader) {
		str.Format("%c", Header);
		SetDlgItemText(IDC_Header, str);
	}

	// nPara
	nPara = min(max(0, nPara), N_MAX_Control);
	SetDlgItemInt(IDC_nVariables, nPara);

	// Description
	for (int i = 0; i < nPara; i++) {
		GetDlgItemText(m_IDC_Label[i], str);
		if (str != strLabel[i]) {
			SetDlgItemText(m_IDC_Label[i], strLabel[i]);
		}
	}

	// Value
	CStringA strText;

	for (int i = 0; i < nPara; i++) {
		int dtype = ((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->GetCurSel();
		switch (dtype) {
		case 0:
		{
			unsigned int val = (unsigned int)uiPara[i];
			SetDlgItemInt(m_IDC_DecInput[i], val);
			str.Format("%lX", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 1:
		{
			unsigned short val = (unsigned short)uiPara[i];
			SetDlgItemInt(m_IDC_DecInput[i], val);
			str.Format("%X", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 2:
		{
			unsigned char val = (unsigned char)uiPara[i];
			SetDlgItemInt(m_IDC_DecInput[i], val);
			str.Format("%X", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 3:
		{
			int val = (int)iPara[i];
			SetDlgItemInt(m_IDC_DecInput[i], val);
			str.Format("%X", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 4:
		{
			signed short val = (signed short)iPara[i];
			SetDlgItemInt(m_IDC_DecInput[i], val);
			str.Format("%X", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 5:
		{
			signed char val = (signed char)iPara[i];
			SetDlgItemInt(m_IDC_DecInput[i], val);
			str.Format("%X", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 6:
		{
			CString strVal = pMain->Float2Str(fPara[i]);
			fPara[i] = (float)(atof(strVal));
			unsigned char* p = reinterpret_cast<unsigned char*>(&fPara[i]);
			SetDlgItemText(m_IDC_DecInput[i], strVal);
			long val = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
			str.Format("%lX", val);
			SetDlgItemText(m_IDC_HexInput[i], str);
		}
		break;

		case 7:
		{
			SetDlgItemText(m_IDC_DecInput[i], strDec[i]);
			SetDlgItemText(m_IDC_HexInput[i], strHex[i]);
		}
		break;
		}
	}
	m_bProhibtMessageUpdate = FALSE;
	OnEnChangeParameter();
}

void CControlDlg::OnEnChangeHeader() {
	CString str = "", str1;
	GetDlgItemText(IDC_Header, str);

	int iSize = str.GetLength();
	BOOL bFindHeader = FALSE;
	BYTE aByte;
	for (int i = 0; i < iSize; i++) {
		if (bFindHeader == FALSE) {
			aByte = IsHead(str.GetAt(i));
			if (aByte != 0) {
				str1.Format("%c", aByte);
				if (str != str1) SetDlgItemText(IDC_Header, str1);
				bFindHeader = TRUE;
			}
		}
	}
	if (bFindHeader) {
		if (m_iPreset >= 0 && m_bProhibtMessageUpdate == FALSE) {
			GetDlgItemText(IDC_Header, str);
			for (int i = 0; i < m_iControl; i++) {
				GetDlgItemText(m_IDC_Label[i], str1);
				CString strTemp = str;
				if (str1 == "")
					str.Format("%s %d", (LPCTSTR)strTemp, GetDlgItemInt(m_IDC_DecInput[i]));
				else
					str.Format("%s [%s]%d", (LPCTSTR)strTemp, (LPCTSTR)str1, GetDlgItemInt(m_IDC_DecInput[i]));
			}
			GetDlgItemText(m_IDC_Preset[m_iPreset], str1);
			if (str != str1) SetDlgItemText(m_IDC_Preset[m_iPreset], str);
		}
	}
	UpdateSendPacket();
}

int CControlDlg::ArrangeData(BOOL bCheckOnly, BYTE* pBuf) {
	CStringA strDec, strHex, str, str2, SendPacket = "";
	int iCount = 0;
	BYTE buf[500];
	if (pBuf == nullptr) {
		pBuf = buf;
	}

	// Header
	BOOL bHeaderError = FALSE;
	if (IsDlgButtonChecked(IDC_UseHeader)) {
		GetDlgItemText(IDC_Header, str);
		int iSize = str.GetLength();
		BYTE aByte, Header;
		BOOL bFindHeader = FALSE;
		for (int i = 0; i < iSize; i++) {
			aByte = IsHead(str.GetAt(i));
			if (aByte != 0) {
				Header = aByte;
				bFindHeader = TRUE;
				break;
			}
		}

		if (bFindHeader) {
			SendPacket.Format("%02X", Header);
			if (bCheckOnly == FALSE) { pBuf[iCount++] = Header; }
		}
		else {
			bHeaderError = TRUE;
		}
	}

	int dtype;
	BOOL bIsBlank = FALSE;
	for (int i = 0; i < m_iControl; i++) {
		dtype = ((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->GetCurSel();
		BOOL bNotBlank = FALSE;
		unsigned int ui_hex_val = 0;

		BOOL bMinus = FALSE;
		BOOL bPeriod = FALSE;
		GetDlgItemText(m_IDC_HexInput[i], strHex);
		GetDlgItemText(m_IDC_DecInput[i], strDec);

		unsigned int ui_dec_val = 0;
		float  fOrder = 1.;
		float fVal = 0;
		if (dtype != 7) {
			for (int d = 0; d < strDec.GetLength(); d++) {
				char ch = strDec.GetAt(d);
				if ((ch >= '0' && ch <= '9')) {
					bNotBlank = TRUE;
					ui_dec_val = ui_dec_val * 10 + ch - '0';
					if (bPeriod) { fOrder *= 10.; }
				}
				else if (ch == '-') {
					bMinus = TRUE;
				}
				else if (ch == '.') {
					bPeriod = TRUE;
				}
			}
			if (strDec.GetLength() == 0) { bIsBlank = TRUE; }
		}
		else if (dtype == 7) {
			strHex = "";
			for (int d = 0; d < strDec.GetLength(); d++) {
				char ch = strDec.GetAt(d);
				strHex.Format("%s%02X", strHex, ch);
			}
		}
		CString strDec2 = "";
		CString strTemp = SendPacket;
		if (dtype == 0) {
			strDec2.Format("%d", ui_dec_val);
			strHex.Format("%X", ui_dec_val);

			unsigned char* p = reinterpret_cast<unsigned char*>(&ui_dec_val);
			if (m_iByteOrder == 0) {
				pBuf[iCount++] = p[3];
				pBuf[iCount++] = p[2];
				pBuf[iCount++] = p[1];
				pBuf[iCount++] = p[0];
				SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, p[3], p[2], p[1], p[0]);
			}
			else {
				pBuf[iCount++] = p[0];
				pBuf[iCount++] = p[1];
				pBuf[iCount++] = p[2];
				pBuf[iCount++] = p[3];
				SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, p[0], p[1], p[2], p[3]);
			}

			if (strDec2 != strDec) {
				SetDlgItemText(m_IDC_DecInput[i], strDec2);
			}
			SetDlgItemText(m_IDC_HexInput[i], strHex);
		}
		else if (dtype == 1) {
			unsigned short val = (unsigned short)ui_dec_val;
			strDec2.Format("%d", val);
			strHex.Format("%X", val);
			unsigned char* p = reinterpret_cast<unsigned char*>(&ui_dec_val);
			if (m_iByteOrder == 0) {
				pBuf[iCount++] = p[1];
				pBuf[iCount++] = p[0];
				SendPacket.Format("%s %02X%02X", (LPCTSTR)strTemp, p[1], p[0]);
			}
			else {
				pBuf[iCount++] = p[0];
				pBuf[iCount++] = p[1];
				SendPacket.Format("%s %02X%02X", (LPCTSTR)strTemp, p[0], p[1]);
			}

			if (strcmp(strDec2, strDec) != 0) {
				SetDlgItemText(m_IDC_DecInput[i], strDec2);
			}
			SetDlgItemText(m_IDC_HexInput[i], strHex);
		}
		else if (dtype == 2) {
			unsigned char val = (unsigned char)ui_dec_val;
			strDec2.Format("%d", val);
			strHex.Format("%X", val);
			unsigned char* p = reinterpret_cast<unsigned char*>(&ui_dec_val);
			pBuf[iCount++] = p[0];
			SendPacket.Format("%s %02X", (LPCTSTR)strTemp, p[0]);
			if (strDec2 != strDec) {
				SetDlgItemText(m_IDC_DecInput[i], strDec2);
			}
			SetDlgItemText(m_IDC_HexInput[i], strHex);
		}
		else if (dtype <= 5) {
			int iVal = ui_dec_val;
			if (bMinus) {
				iVal = -iVal;
			}
			if (dtype == 3) {
				strDec2.Format("%d", iVal);
				strHex.Format("%X", iVal);
				unsigned char* p = reinterpret_cast<unsigned char*>(&iVal);
				if (m_iByteOrder == 0) {
					pBuf[iCount++] = p[3];
					pBuf[iCount++] = p[2];
					pBuf[iCount++] = p[1];
					pBuf[iCount++] = p[0];
					SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, p[3], p[2], p[1], p[0]);
				}
				else {
					pBuf[iCount++] = p[0];
					pBuf[iCount++] = p[1];
					pBuf[iCount++] = p[2];
					pBuf[iCount++] = p[3];
					SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, p[0], p[1], p[2], p[3]);
				}
			}
			else if (dtype == 4) {
				signed short val = (signed short)iVal;
				strDec2.Format("%d", val);
				strHex.Format("%X", val);
				unsigned char* p = reinterpret_cast<unsigned char*>(&val);
				if (m_iByteOrder == 0) {
					pBuf[iCount++] = p[1];
					pBuf[iCount++] = p[0];
					SendPacket.Format("%s %02X%02X", (LPCTSTR)strTemp, p[1], p[0]);
				}
				else {
					pBuf[iCount++] = p[0];
					pBuf[iCount++] = p[1];
					SendPacket.Format("%s %02X%02X", (LPCTSTR)strTemp, p[0], p[1]);
				}
			}
			else if (dtype == 5) {
				signed char val = (signed char)iVal;
				strDec2.Format("%d", val);
				strHex.Format("%X", val);
				unsigned char* p = reinterpret_cast<unsigned char*>(&iVal);
				pBuf[iCount++] = p[0];
				SendPacket.Format("%s %02X", (LPCTSTR)strTemp, p[0]);
			}

			if (strDec2 != strDec) {
				SetDlgItemText(m_IDC_DecInput[i], strDec2);
			}
			SetDlgItemText(m_IDC_HexInput[i], strHex);
		}
		else if (dtype == 6) {
			float fVal = (float)ui_dec_val;
			if (bMinus) {
				fVal = -fVal;
			}
			fVal /= fOrder;
			strDec2.Format("%s", (LPCTSTR)(pMain->Float2Str(fVal)));

			unsigned char* p = reinterpret_cast<unsigned char*>(&fVal);
			if (m_iByteOrder == 0) {
				pBuf[iCount++] = p[3];
				pBuf[iCount++] = p[2];
				pBuf[iCount++] = p[1];
				pBuf[iCount++] = p[0];
				SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, p[3], p[2], p[1], p[0]);
			}
			else {
				pBuf[iCount++] = p[0];
				pBuf[iCount++] = p[1];
				pBuf[iCount++] = p[2];
				pBuf[iCount++] = p[3];
				SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, p[0], p[1], p[2], p[3]);
			}

			if (abs(atof(strDec) - atof(strDec2)) > 0.000001) {
				SetDlgItemText(m_IDC_DecInput[i], strDec2);
			}
			unsigned int val = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
			strHex.Format("%X", val);
			SetDlgItemText(m_IDC_HexInput[i], strHex);
		}
		else if (dtype == 7) {
			if (i > 0) {
				SendPacket += " ";;
			}
			for (int d = 0; d < strDec.GetLength() && iCount <= 400; d++) {
				char ch = strDec.GetAt(d);
				pBuf[iCount++] = ch;
				strTemp.Format("%02X", ch);
				SendPacket += strTemp;
			}
			SetDlgItemText(m_IDC_HexInput[i], strHex);
			bNotBlank = TRUE;
		}

		if (bNotBlank == FALSE) { bIsBlank = TRUE; }
	}

	CString strTemp = SendPacket;
	SendPacket.Format("%s (%dBytes)", (LPCTSTR)strTemp, iCount);
	SetDlgItemText(IDC_SendPacket, SendPacket);
	if (bHeaderError) { return -2; }
	if (bIsBlank) { return -1; }
	return iCount;
}


void CControlDlg::OnBnClickedSendcommand() {
	static int idc[3] = { IDC_SendPort1 , IDC_SendPort2 , IDC_SendPort3 };
	CString str = "", SendPacket = "";
	BYTE pBuf[500];
	int iCount = ArrangeData(FALSE, pBuf);
	static int sendCount = 0;

	switch (iCount) {
	case -2:
		AfxMessageBox("Error : missing Header (a-z, A-Z)");
		return;

	case -1:
		AfxMessageBox("Error : 공백 항목 있음");
		return;
	}

	bool bSend = false;
	CString strSendDev = "";

	if (pMain->m_ComuPort.m_bConnected &&  IsDlgButtonChecked(idc[0])) {
		pMain->m_ComuPort.WriteComm(pBuf, iCount); bSend = true;
		strSendDev = "Motor ";
	}

	if (pMain->m_ComuPort2.m_bConnected &&  IsDlgButtonChecked(idc[1])) {
		pMain->m_ComuPort2.WriteComm(pBuf, iCount); bSend = true;
		strSendDev += "Braker ";
	}

	if (pMain->m_ComuPort3.m_bConnected &&  IsDlgButtonChecked(idc[2])) {
		pMain->m_ComuPort3.WriteComm(pBuf, iCount); bSend = true;
		strSendDev += "Torquemeter ";
	}

	if (bSend) {
		GetDlgItemText(IDC_SendPacket, str);
		CString strTemp = str;
		str.Format("SendCmd:%s -> %s(id:%d)", (LPCTSTR)strTemp, strSendDev, ++sendCount);
		pMain->SetDlgItemTextA(IDC_Temp, str);

		pMain->m_iLogIndex++;
		pMain->AddLogData(str);
		pMain->m_iLogHistoryIndexTable.emplace_back(pMain->m_iHistoryIndex);
		pMain->m_iLogFilterIndexTable.emplace_back(pMain->m_iFilterIndex);
		pMain->ScrollToLastItem();

	}
	else {
		AfxMessageBox("Error: Dev에 전송 실패! (선택된 가용한 통신포트 없음)");
	}
}


void CControlDlg::OnEnChangeParameter() {
	CString str = "", str1 = "", str2 = "";
	int ret = ArrangeData(TRUE);
	if (m_iPreset >= 0 && m_bProhibtMessageUpdate == FALSE && ret >= 0) {
		if (IsDlgButtonChecked(IDC_UseHeader)) {
			GetDlgItemText(IDC_Header, str);
		}

		char arrPreset[10] = "";
		char arrType[10];

		for (int i = 0; i < m_iControl; i++) {
			int dtype = ((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->GetCurSel();
			switch (dtype) {
			case 0: strcpy_s(arrType, 10, "uint32"); break;
			case 1: strcpy_s(arrType, 10, "uint16"); break;
			case 2: strcpy_s(arrType, 10, "uint8"); break;
			case 3: strcpy_s(arrType, 10, "int32"); break;
			case 4: strcpy_s(arrType, 10, "int16"); break;
			case 5: strcpy_s(arrType, 10, "int8"); break;
			case 6: strcpy_s(arrType, 10, "float"); break;
			case 7: strcpy_s(arrType, 10, "string"); break;
			default:
				strcpy_s(arrType, 10, "uint16");
				((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->SetCurSel(1);
				break;
			}

			if (strcmp(arrPreset, arrType) != 0) {
				strcpy_s(arrPreset, 10, arrType);
				CString strTemp = str;
				str.Format("%s {%s}", (LPCTSTR)strTemp, (LPCTSTR)arrType);
			}

			// Description
			GetDlgItemText(m_IDC_Label[i], str1);
			CString strTemp = str;
			if (str1 != "")
				str.Format("%s [%s]", (LPCTSTR)strTemp, (LPCTSTR)str1);

			// Value update            
			if (dtype < 6) {
				int val = GetDlgItemInt(m_IDC_DecInput[i]);
				CString strTemp = str;
				str.Format("%s %ld", (LPCTSTR)strTemp, val);
			}
			else if (dtype == 7) {
				strTemp = str;
				GetDlgItemText(m_IDC_DecInput[i], str1);
				str.Format("%s \"%s\"", (LPCTSTR)strTemp, (LPCTSTR)str1);
			}
			else if (dtype == 6) {
				strTemp = str;
				GetDlgItemText(m_IDC_DecInput[i], str1);
				str1 = pMain->Float2Str((float)atof(str1));
				str.Format("%s %s", (LPCTSTR)strTemp, (LPCTSTR)str1);
			}
		}
		GetDlgItemText(m_IDC_Preset[m_iPreset], str1);

		if (str1 != str) {
			SetDlgItemText(m_IDC_Preset[m_iPreset], str);
		}
	}
}

void CControlDlg::DelayChangeMessage() {
	if (m_iChangeMessageID == -1) { return; }

	int nID = m_iChangeMessageID;
	m_iChangeMessageID = -1;
	KillTimer(20);
	int iCursor = -1;
	for (int i = 0; i < N_MAX_Control; i++) {
		if (m_IDC_Preset[i] == nID) {
			CPoint pt = ((CEdit *)GetDlgItem(nID))->GetCaretPos();
			iCursor = pt.x;
		}
	}

	if (m_iPreset >= 0) {
		if (m_IDC_Preset[m_iPreset] == nID) {
			OnBnClickedContropreset();
		}
	}

	if (iCursor != -1) {
		((CEdit *)GetDlgItem(nID))->SetCaretPos(CPoint(iCursor, 0));
	}
}


void CControlDlg::OnEnChangeControlmessage() {
	int nID = GetFocus()->GetDlgCtrlID();
	m_iChangeMessageID = nID;
	SetTimer(20, 1500, NULL);
}


void CControlDlg::UpdateSendPacket() {
	CString str = "", SendPacket = "";
	BYTE Packet[N_MAX_Control * 4 + 3];
	int iCount = 0;
	int iSize = str.GetLength();
	BOOL bFindHeader = FALSE;
	BYTE aByte, Header;
	BOOL bUseHeader = IsDlgButtonChecked(IDC_UseHeader);
	int dtype;

	if (bUseHeader) {
		GetDlgItemText(IDC_Header, str);

		iSize = str.GetLength();
		for (int i = 0; i < iSize; i++) {
			if (bFindHeader == FALSE) {
				aByte = IsHead(str.GetAt(i));
				if (aByte != 0) {
					Header = aByte;
					bFindHeader = TRUE;
				}
			}
		}
	}
	else {
		bFindHeader = TRUE;
	}

	//UpdateVale();

	if (bFindHeader) {
		if (bUseHeader) {
			SendPacket.Format("%02X", Header);
			Packet[iCount++] = Header;
		}

		for (int i = 0; i < m_iControl; i++) {
			dtype = ((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->GetCurSel();
			CString strTemp = SendPacket;
			if (dtype < 0 || dtype > 6) {
				// Error
				SendPacket.Format("Datat Type Error : %d", i + 1);
				SetDlgItemText(IDC_SendPacket, SendPacket);
				break;
			}
			else if (dtype < 3) {
				unsigned char* p = reinterpret_cast<unsigned char*>(&m_uiPara[i]);
				if (dtype == 0) {
					if (m_iByteOrder == 0) {
						Packet[iCount++] = p[3];
						Packet[iCount++] = p[2];
						Packet[iCount++] = p[1];
						Packet[iCount++] = p[0];
					}
					else {
						Packet[iCount++] = p[0];
						Packet[iCount++] = p[1];
						Packet[iCount++] = p[2];
						Packet[iCount++] = p[3];
					}
					SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, Packet[iCount - 4], Packet[iCount - 3], Packet[iCount - 2], Packet[iCount - 1]);
				}
				else if (dtype == 1) {
					if (m_iByteOrder == 0) {
						Packet[iCount++] = p[1];
						Packet[iCount++] = p[0];
					}
					else {
						Packet[iCount++] = p[0];
						Packet[iCount++] = p[1];
					}

					SendPacket.Format("%s %02X%02X", (LPCTSTR)strTemp, Packet[iCount - 2], Packet[iCount - 1]);
				}
				else if (dtype == 2) {
					Packet[iCount++] = p[0];
					SendPacket.Format("%s %02X", (LPCTSTR)strTemp, Packet[iCount - 1]);
				}
			}
			else if (dtype < 6) {
				unsigned char* p = reinterpret_cast<unsigned char*>(&m_iPara[i]);
				if (dtype == 3) {
					if (m_iByteOrder == 0) {
						Packet[iCount++] = p[3];
						Packet[iCount++] = p[2];
						Packet[iCount++] = p[1];
						Packet[iCount++] = p[0];
					}
					else {
						Packet[iCount++] = p[0];
						Packet[iCount++] = p[1];
						Packet[iCount++] = p[2];
						Packet[iCount++] = p[3];
					}

					SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, Packet[iCount - 4], Packet[iCount - 3], Packet[iCount - 2], Packet[iCount - 1]);
				}
				else if (dtype == 4) {
					if (m_iByteOrder == 0) {
						Packet[iCount++] = p[1];
						Packet[iCount++] = p[0];
					}
					else {
						Packet[iCount++] = p[0];
						Packet[iCount++] = p[1];
					}

					SendPacket.Format("%s %02X%02X", (LPCTSTR)strTemp, Packet[iCount - 2], Packet[iCount - 1]);
				}
				else if (dtype == 5) {
					Packet[iCount++] = p[0];
					SendPacket.Format("%s %02X", (LPCTSTR)strTemp, Packet[iCount - 1]);
				}
			}
			else if (dtype == 6) {
				unsigned char* p = reinterpret_cast<unsigned char*>(&m_fPara[i]);
				if (m_iByteOrder == 0) {
					Packet[iCount++] = p[3];
					Packet[iCount++] = p[2];
					Packet[iCount++] = p[1];
					Packet[iCount++] = p[0];
				}
				else {
					Packet[iCount++] = p[0];
					Packet[iCount++] = p[1];
					Packet[iCount++] = p[2];
					Packet[iCount++] = p[3];
				}
				SendPacket.Format("%s %02X%02X%02X%02X", (LPCTSTR)strTemp, Packet[iCount - 4], Packet[iCount - 3], Packet[iCount - 2], Packet[iCount - 1]);
			}
		}

		SendPacket.Format("%s (%dBytes)", SendPacket.GetString(), iCount);
		SetDlgItemText(IDC_SendPacket, SendPacket);
	}
	else {
		SendPacket.Format("Wrong Transmit Data Format : Header Missing! ");
		SetDlgItemText(IDC_SendPacket, SendPacket);
	}
}

void CControlDlg::OnBnClickedUseheader() {
	if (IsDlgButtonChecked(IDC_UseHeader)) {
		GetDlgItem(IDC_Header)->ShowWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_Header)->ShowWindow(FALSE);
	}

	OnEnChangeParameter();
}


void CControlDlg::OnEnChangeConvertchar() {
	CString str = "", str2 = "";
	unsigned long long val = 0;

	GetDlgItemText(IDC_Convert_char, str);
	for (int i = 0; i < min(8, str.GetLength()); i++) {
		char ch = str.GetAt(i);
		CString strTemp = str2;
		str2.Format("%s%c", (LPCTSTR)strTemp, ch);
		val = (val << 8) + ch;
	}

	if (str != str2) {
		SetDlgItemText(IDC_Convert_char, str2);
	}

	str.Format("%llu", val);
	SetDlgItemText(IDC_Convert_Ascii, str);

	str.Format("%llX", val);
	SetDlgItemText(IDC_Convert_Ascii2, str);
}

void CControlDlg::OnEnChangeConvertAscii4() {
	CString str, str2;
	GetDlgItemText(IDC_Convert_Ascii4, str);  // DEC val
	unsigned long long val = 0;
	for (int i = 0; i < str.GetLength(); i++) {
		char ch = str.GetAt(i);
		if (ch >= '0' && ch <= '9') {
			val = val * 10 + ch - '0';
		}
	}

	str2.Format("%llu", val);
	if (str != str2) {
		SetDlgItemText(IDC_Convert_Ascii4, str2);
	}

	str.Format("%llX", val);
	SetDlgItemText(IDC_Convert_Ascii3, str);

	str2 = "";
	CString strT;
	while (val > 0) {
		strT.Format("%c", val & 0xFF);
		str2 = strT + str2;
		val >>= 8;
	}

	SetDlgItemText(IDC_Convert_char2, str2);
}

void CControlDlg::OnEnChangeConvertAscii5()
{
	CString str, str2;
	GetDlgItemText(IDC_Convert_Ascii5, str);  // DEC val
	unsigned long long val = 0;
	for (int i = 0; i < str.GetLength(); i++) {
		char ch = str.GetAt(i);
		if (ch >= '0' && ch <= '9') {
			val = (val << 4) + ch - '0';
		}
		else if (ch >= 'a' && ch <= 'f') {
			val = (val << 4) + ch - 'a' + 10;
		}
		else if (ch >= 'A' && ch <= 'F') {
			val = (val << 4) + ch - 'A' + 10;
		}
	}
	str2.Format("%llX", val);
	if (str != str2) {
		SetDlgItemText(IDC_Convert_Ascii5, str2);
	}

	str2.Format("%llu", val);
	if (str != str2) {
		SetDlgItemText(IDC_Convert_Ascii6, str2);
	}

	str2 = "";
	CString strT;
	while (val > 0) {
		strT.Format("%c", val & 0xFF);
		str2 = strT + str2;
		val >>= 8;
	}

	SetDlgItemText(IDC_Convert_char3, str2);
}

void CControlDlg::OnBnClickedCharinput() {
	OnEnChangenvariables();
}

void CControlDlg::OnBnClickedFixdatatype()
{
	BOOL bShow = TRUE;
	if (IsDlgButtonChecked(IDC_FixDataType)) {
		// Preset에서 혼용이면 자동으로 표시로 변경됨
		// 설정도 변경함
		// 추후 추가 필요함
		bShow = FALSE;
	}

	for (int i = 0; i < N_MAX_Control; i++) {
		GetDlgItem(m_IDC_DataType[i])->EnableWindow(bShow);
	}

	GetDlgItem(IDC_DataType)->ShowWindow(!bShow);
}

void CControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 20:
		DelayChangeMessage();
		break;
	default: break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CControlDlg::OnBnClickedByteorder0()
{
	m_iByteOrder = 0;
	OnEnChangeParameter();
}


void CControlDlg::OnBnClickedByteorder1()
{
	m_iByteOrder = 1;
	OnEnChangeParameter();
}


void CControlDlg::OnEnChangefloat()
{
	CString str1, str2;
	GetDlgItemText(IDC_float, str1);

	str2 = pMain->Float2Str((float)(atof(str1)));
	float val = (float)atof(str2);
	if (str1 != str2 && val != 0.) {
		SetDlgItemText(IDC_float, str2);
	}
	unsigned int *p = (unsigned int *)(&val);

	str1.Format("%X", *p);
	SetDlgItemText(IDC_Hex, str1);
}

void CControlDlg::OnCbnSelchangeDatatype()
{
	int dtype = ((CComboBox *)GetDlgItem(IDC_DataType))->GetCurSel();
	for (int i = 0; i < m_iControl; i++) {
		((CComboBox *)GetDlgItem(m_IDC_DataType[i]))->SetCurSel(dtype);
	}
	OnEnChangeParameter();
}


void CControlDlg::OnEnChangeHex2()
{
	CString str, str2;
	GetDlgItemText(IDC_Hex2, str);  // DEC val
	unsigned int val = 0;
	for (int i = 0; i < str.GetLength(); i++) {
		char ch = str.GetAt(i);
		if (ch >= '0' && ch <= '9') {
			val = (val << 4) + ch - '0';
		}
		else if (ch >= 'a' && ch <= 'f') {
			val = (val << 4) + ch - 'a' + 10;
		}
		else if (ch >= 'A' && ch <= 'F') {
			val = (val << 4) + ch - 'A' + 10;
		}
	}
	str2.Format("%X", val);
	if (str != str2) {
		SetDlgItemText(IDC_Hex2, str2);
	}
	float *pVal = (float *)(&val);

	GetDlgItemText(IDC_float2, str2);
	str2 = pMain->Float2Str(*pVal);
	if (str != str2) {
		SetDlgItemText(IDC_float2, str2);
	}
}

char CControlDlg::IsHead(char aByte) {
	if (aByte >= 'a' && aByte <= 'z') { return aByte; }
	if (aByte >= 'A' && aByte <= 'Z') { return aByte; }
	const char s[] = "~!@#$%^&*()_+`{}[]|:;\\\"',./<>?'";
	for (int i = 0; i < sizeof(s); i++) {
		if (aByte == s[i]) { return aByte; }
	}
	return 0;
}

void CControlDlg::OnBnClickedRemotecommand0() { pMain->ClickedUsercommand(0); }
void CControlDlg::OnBnClickedRemotecommand1() { pMain->ClickedUsercommand(1); }
void CControlDlg::OnBnClickedRemotecommand2() { pMain->ClickedUsercommand(2); }
void CControlDlg::OnBnClickedRemotecommand3() { pMain->ClickedUsercommand(3); }
void CControlDlg::OnBnClickedRemotecommand4() { pMain->ClickedUsercommand(4); }
void CControlDlg::OnBnClickedRemotecommand5() { pMain->ClickedUsercommand(5); }
void CControlDlg::OnBnClickedRemotecommand6() { pMain->ClickedUsercommand(6); }
void CControlDlg::OnBnClickedRemotecommand7() { pMain->ClickedUsercommand(7); }
void CControlDlg::OnBnClickedRemotecommand8() { pMain->ClickedUsercommand(8); }
void CControlDlg::OnBnClickedRemotecommand9() { pMain->ClickedUsercommand(9); }
void CControlDlg::OnBnClickedRemotecommand10() { pMain->ClickedUsercommand(10); }
void CControlDlg::OnBnClickedRemotecommand11() { pMain->ClickedUsercommand(11); }
void CControlDlg::OnBnClickedRemotecommand12() { pMain->ClickedUsercommand(12); }
void CControlDlg::OnBnClickedRemotecommand13() { pMain->ClickedUsercommand(13); }
void CControlDlg::OnBnClickedRemotecommand14() { pMain->ClickedUsercommand(14); }
void CControlDlg::OnBnClickedRemotecommand15() { pMain->ClickedUsercommand(15); }
void CControlDlg::OnBnClickedRemotecommand16() { pMain->ClickedUsercommand(16); }
void CControlDlg::OnBnClickedRemotecommand17() { pMain->ClickedUsercommand(17); }
void CControlDlg::OnBnClickedRemotecommand18() { pMain->ClickedUsercommand(18); }
void CControlDlg::OnBnClickedRemotecommand19() { pMain->ClickedUsercommand(19); }

void CControlDlg::OnBnClickedDefaultbutton2() {
	pMain->OnBnClickedDefaultbutton();
	UpdateChangeModeButtonTitle();
}

void CControlDlg::UpdateChangeModeButtonTitle() {
	CString str;
	pMain->GetDlgItemText(IDC_DefaultButton, str);
	SetDlgItemText(IDC_DefaultButton2, str);
}

void CControlDlg::OnBnClickedResetdata() {
	pMain->ReqResetData(TRUE);
}


void CControlDlg::OnBnClickedChangesize() {
	CRect rc;
	GetWindowRect(&rc);
	static int preSel = -1;
	if (m_WndSize == 0) {
		m_WndSize = 1;
		MoveWindow(rc.left, rc.top, 1020, 95 + 30);
		SetDlgItemText(IDC_ChangeSize, "Full size");
		preSel = m_iPreset;
		m_iPreset = 10;

		GetDlgItem(IDC_ControlMessage10)->ShowWindow(TRUE);
		GetDlgItem(IDC_SendCommand2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Full)->ShowWindow(FALSE);
		GetDlgItem(IDC_ControPreset0)->ShowWindow(FALSE);
		GetDlgItem(m_IDC_Preset[0])->ShowWindow(FALSE);


		if (preSel != -1) {
			CString str;
			GetDlgItemText(m_IDC_Preset[preSel], str);
			SetDlgItemText(m_IDC_Preset[m_iPreset], str);

		}
		else {
			SetDlgItemText(m_IDC_Preset[m_iPreset], "");
		}
	}
	else {
		m_WndSize = 0;
		MoveWindow(rc.left, rc.top, 1020, 650);
		SetDlgItemText(IDC_ChangeSize, "Mini size");
		m_iPreset = preSel;

		GetDlgItem(IDC_ControlMessage10)->ShowWindow(FALSE);
		GetDlgItem(IDC_SendCommand2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Full)->ShowWindow(TRUE);
		GetDlgItem(IDC_ControPreset0)->ShowWindow(TRUE);
		GetDlgItem(m_IDC_Preset[0])->ShowWindow(TRUE);
	}
	UDF;
	OnBnClickedContropreset();
}

void CControlDlg::OnCancel()
{
	DestroyWindow();
}

// ⭐ 윈도우가 파괴될 때
void CControlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	pMain->m_pCmdControl = nullptr;
}

void CControlDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

void CControlDlg::OnBnClickedSendcommand2()
{
	OnBnClickedContropreset();	
	OnBnClickedSendcommand();
}
