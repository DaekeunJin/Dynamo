#if !defined(AFX_COLORLISTCTRL_H__5DBF3183_0A52_4BBD_BE9A_24E183F654C9__INCLUDED_)
#define AFX_COLORLISTCTRL_H__5DBF3183_0A52_4BBD_BE9A_24E183F654C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorListCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl window
//#include "user.h"

class CColorListCtrl : public CListCtrl
{
// Construction
public:
	CColorListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListCtrl)
	virtual	afx_msg void OnBKDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorListCtrl();
	//BOOL bGroup[MAX_ROW_COLOR];
	// Generated message map functions
protected:
	//{{AFX_MSG(CColorListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTCTRL_H__5DBF3183_0A52_4BBD_BE9A_24E183F654C9__INCLUDED_)
