#pragma once


#define HEADER_LOG_COLUMN_SIZE	20

// CGraphLog 대화 상자
class CSerialComDlg;
class CGraphLog : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphLog)

public:
	CGraphLog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGraphLog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GraphLog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReqfiledlg();
	CSerialComDlg* pMain;
	CListCtrl m_ctrlList;
	void AddColumn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeItemtype();
	CComboBox m_ctrlItemType;
	afx_msg void OnBnClickedAdditem();
	CGraphLogSetting  m_Item;	
	CGraphLogSetting CheckData(CString *pstr);
	
	CComboBox m_ctrlCond;
	afx_msg void OnEnChangeTrueR();
	afx_msg void OnEnChangeTrueG();
	afx_msg void OnEnChangeTrueB();
	afx_msg void OnEnChangeFalseR();
	afx_msg void OnEnChangeFalseG();
	afx_msg void OnEnChangeFalseB();

	int m_iSelectItem{ -1 };
	int m_iGraphLogCount{ 0 };
	afx_msg void OnClickItemlist(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedModify();
	void UpdateListTbl_from_Data(int iPos);
	BOOL UpdateData_from_Control();
	void UpdateData_from_ListTable(int iPos);
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedMoveup();
	afx_msg void OnBnClickedMovedn();
	afx_msg void OnBnClickedSave();
	void LoadData();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeHeaderlogHeader();
	BYTE m_cGraphLogPacketHead{ ' ' };
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void DrawColor(int pos1);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedSavegraphlog();
	afx_msg void OnBnClickedLoadgraphlogsetting();
	void DrawBitControl(BOOL bShow);
    void UpdateBitControl();

	BOOL m_bMask[16]{ 0, };
	BOOL m_bValue[16]{ 0, };
	void UpdateMaskValue(int mask_val);
	afx_msg void OnBnClickedUsemaskvalue();
	CComboBox m_ctrlDigit;
	void InitMaskSetting();
	afx_msg void OnBnClickedDatachange();
	afx_msg void OnBnClickedSendregiondata();
	afx_msg void OnBnClickedGetregiondata();
	afx_msg void OnBnClickedSendcolordata1();
	afx_msg void OnBnClickedGetcolordata1();
	afx_msg void OnBnClickedSendcolordata2();
	afx_msg void OnBnClickedGetcolordata2();
	afx_msg void OnBnClickedLoadgraphlogsetting2();

	BOOL ParsingGraphLogConfig(const CString& pathName);
	afx_msg void OnEnChangeData2();
	afx_msg void OnEnChangeData1();
    CComboBox m_ctrlDiv;
    void ClearControl();
};
