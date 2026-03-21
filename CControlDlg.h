#pragma once
#include "SerialComDlg.h"

// CControlDlg 대화 상자
class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CControlDlg();
	CSerialComDlg* pMain;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ControlDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIncreascontrol();
	afx_msg void OnEnChangenvariables();

	int m_iControl;
	int m_IDC_DecInput[N_MAX_Control];
	int m_IDC_Label[N_MAX_Control];
	int m_IDC_HexInput[N_MAX_Control];
	int m_IDC_Preset[N_MAX_Preset];
	int m_IDC_DataType[N_MAX_Preset];
	virtual BOOL OnInitDialog();
	int m_iPreset;
	afx_msg void OnBnClickedDecreascontrol2();
	afx_msg void OnBnClickedContropreset();
	BOOL UpdateDataType(int&, char *, int);
	afx_msg void OnBnClickedSavepreset();
	afx_msg void OnEnChangeHeader();
	afx_msg void OnBnClickedSendcommand();
	afx_msg void OnEnChangeParameter();
	afx_msg void OnEnChangeControlmessage();
    void DelayChangeMessage();
    int m_iChangeMessageID;
	void UpdateSendPacket();
	BOOL m_bProhibtMessageUpdate;
	afx_msg void OnBnClickedUseheader();
	afx_msg void OnEnChangeConvertchar();
	afx_msg void OnBnClickedCharinput();

	int ArrangeData(BOOL bCheckOnly, BYTE *pBuf = nullptr);
	afx_msg void OnBnClickedFixdatatype();
	afx_msg void OnEnChangeConvertAscii4();
	afx_msg void OnEnChangeConvertAscii5();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_iByteOrder;
	afx_msg void OnBnClickedByteorder0();
	afx_msg void OnBnClickedByteorder1();
    afx_msg void OnEnChangefloat();
    afx_msg void OnEnChangeHex2();

    unsigned int m_uiPara[N_MAX_Control];
    int m_iPara[N_MAX_Control];
    float m_fPara[N_MAX_Control];
    afx_msg void OnCbnSelchangeDatatype();
    char IsHead(char aByte);
    afx_msg void OnBnClickedRemotecommand0();
    afx_msg void OnBnClickedRemotecommand1();
    afx_msg void OnBnClickedRemotecommand2();
    afx_msg void OnBnClickedRemotecommand3();
    afx_msg void OnBnClickedRemotecommand4();
    afx_msg void OnBnClickedRemotecommand5();
    afx_msg void OnBnClickedRemotecommand6();
    afx_msg void OnBnClickedRemotecommand7();
    afx_msg void OnBnClickedRemotecommand8();
    afx_msg void OnBnClickedRemotecommand9();
    afx_msg void OnBnClickedRemotecommand10();
    afx_msg void OnBnClickedRemotecommand11();
    afx_msg void OnBnClickedRemotecommand12();
    afx_msg void OnBnClickedRemotecommand13();
    afx_msg void OnBnClickedRemotecommand14();
    afx_msg void OnBnClickedRemotecommand15();
    afx_msg void OnBnClickedRemotecommand16();
    afx_msg void OnBnClickedRemotecommand17();
    afx_msg void OnBnClickedRemotecommand18();
    afx_msg void OnBnClickedRemotecommand19();
    afx_msg void OnBnClickedDefaultbutton2();
    void UpdateChangeModeButtonTitle();
    afx_msg void OnBnClickedResetdata();
    afx_msg void OnBnClickedChangesize();
    int m_WndSize{ 0 };
    afx_msg void OnCancel();
    afx_msg void OnDestroy();
};
