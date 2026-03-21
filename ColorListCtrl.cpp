// ColorListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ColorListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl


CColorListCtrl::CColorListCtrl()
{
}

CColorListCtrl::~CColorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColorListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnBKDraw)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl message handlers
void CColorListCtrl::OnBKDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	int iRow;
	switch(lplvcd->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW; 
		break;
 
		case CDDS_ITEMPREPAINT:
			*pResult = CDRF_DODEFAULT;

			iRow = lplvcd->nmcd.dwItemSpec;	
				
			if ((iRow + 1) % 2)
				lplvcd->clrTextBk = RGB(220, 250, 250);
			else
				lplvcd->clrTextBk = RGB(245, 255, 255);
				
		break;

		default:
			*pResult = CDRF_DODEFAULT;
		break;
	 }
}

