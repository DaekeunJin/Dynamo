#pragma once

#include "Game.h"
#include "Colorlistctrl.h"
// CRecord 대화 상자

class CRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CRecord)

public:
	CRecord(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRecord();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Record };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CGame* m_pGame;
    CColorListCtrl  m_Best;
    afx_msg void OnBnClickedDeleterecord();
    
    virtual BOOL OnInitDialog();
    void  AddColumn(void);
};
