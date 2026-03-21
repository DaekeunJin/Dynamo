// SerialCom.h : main header file for the SERIALCOM application
//

#if !defined(AFX_SERIALCOM_H__F00EE35B_F230_48D1_9549_4321D4060AA2__INCLUDED_)
#define AFX_SERIALCOM_H__F00EE35B_F230_48D1_9549_4321D4060AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "UserSetting.h"
#include "stdafx.h"
#include <memory>

/////////////////////////////////////////////////////////////////////////////
// CSerialComApp:
// See SerialCom.cpp for the implementation of this class
//

class CSerialComApp : public CWinApp
{
public:
	CSerialComApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialComApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	BOOL ProcessKill();
	BOOL GetProcessModule(DWORD dwPID, CString strProcessName);
	CString GetCurrentProcessModule(void);

	struct LineProperty {
		int Color[3][MAX_COLOR_ITEM];
		int iLineType;
		int iThickness;
		BOOL bIsChanged;
		BOOL bIsBusy;
		BOOL bUseMark;
		int DefaultColor[3][MAX_COLOR_ITEM];
		//int Color2[3];			// for Another Mode
		//int DefaultColor2[3];	// for Another Mode
	};

	LineProperty m_PropertyChange;

	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialComApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOM_H__F00EE35B_F230_48D1_9549_4321D4060AA2__INCLUDED_)
