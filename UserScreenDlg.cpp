// UserScreenDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialComDlg.h"
#include "UserScreenDlg.h"
#include "afxdialogex.h"



// CUserScreenDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserScreenDlg, CDialogEx)

CUserScreenDlg::CUserScreenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UserScreen, pParent)
{
	m_brush_timeline.CreateSolidBrush(RGB(200, 255, 200));
	m_brush_current.CreateSolidBrush(RGB(0, 155, 0));
	m_brush_temp.CreateSolidBrush(RGB(255, 0, 255));
	m_pen_timeline.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
}

CUserScreenDlg::~CUserScreenDlg()
{
}

void CUserScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Mode, m_ctrlMode);
	DDX_Control(pDX, IDC_ReplaySpeed, m_ctrlSpeed);
	DDX_Control(pDX, IDC_ReplayStop, m_ctrlReplayStop);
}


BEGIN_MESSAGE_MAP(CUserScreenDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TestButton, &CUserScreenDlg::OnBnClickedTestbutton)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_Up_P0, &CUserScreenDlg::OnBnClickedUpP0)
	ON_BN_CLICKED(IDC_Up_P1, &CUserScreenDlg::OnBnClickedUpP1)
	ON_BN_CLICKED(IDC_Up_D1, &CUserScreenDlg::OnBnClickedUpD1)
	ON_BN_CLICKED(IDC_Up_C0, &CUserScreenDlg::OnBnClickedUpC0)
	ON_BN_CLICKED(IDC_Up_C1, &CUserScreenDlg::OnBnClickedUpC1)
	ON_BN_CLICKED(IDC_Up_C2, &CUserScreenDlg::OnBnClickedUpC2)
	ON_BN_CLICKED(IDC_Up_D0, &CUserScreenDlg::OnBnClickedUpD0)
	ON_BN_CLICKED(IDC_Dn_P0, &CUserScreenDlg::OnBnClickedDnP0)
	ON_BN_CLICKED(IDC_Dn_P1, &CUserScreenDlg::OnBnClickedDnP1)
	ON_BN_CLICKED(IDC_Dn_D0, &CUserScreenDlg::OnBnClickedDnD0)
	ON_BN_CLICKED(IDC_Dn_D1, &CUserScreenDlg::OnBnClickedDnD1)
	ON_BN_CLICKED(IDC_Dn_C0, &CUserScreenDlg::OnBnClickedDnC0)
	ON_BN_CLICKED(IDC_Dn_C1, &CUserScreenDlg::OnBnClickedDnC1)
	ON_BN_CLICKED(IDC_Dn_C2, &CUserScreenDlg::OnBnClickedDnC2)
	ON_EN_CHANGE(IDC_P0, &CUserScreenDlg::OnEnChangeP0)
	ON_EN_CHANGE(IDC_P1, &CUserScreenDlg::OnEnChangeP1)
	ON_EN_CHANGE(IDC_D0, &CUserScreenDlg::OnEnChangeD0)
	ON_EN_CHANGE(IDC_D1, &CUserScreenDlg::OnEnChangeD1)
	ON_BN_CLICKED(IDC_HideWindow, &CUserScreenDlg::OnBnClickedHidewindow)
	ON_WM_RBUTTONDOWN()
	ON_EN_CHANGE(IDC_Color0, &CUserScreenDlg::OnEnChangeColor0)
	ON_EN_CHANGE(IDC_Color1, &CUserScreenDlg::OnEnChangeColor1)
	ON_EN_CHANGE(IDC_Color2, &CUserScreenDlg::OnEnChangeColor2)
	ON_BN_CLICKED(IDC_Fill, &CUserScreenDlg::OnBnClickedFill)
	ON_BN_CLICKED(IDC_SendRange, &CUserScreenDlg::OnBnClickedSendrange)
	ON_BN_CLICKED(IDC_GetRange, &CUserScreenDlg::OnBnClickedGetrange)
	ON_BN_CLICKED(IDC_GetC1, &CUserScreenDlg::OnBnClickedGetc1)
	ON_BN_CLICKED(IDC_GetC2, &CUserScreenDlg::OnBnClickedGetc2)
	ON_BN_CLICKED(IDC_AutoMode, &CUserScreenDlg::OnBnClickedAutomode)
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_Mode, &CUserScreenDlg::OnCbnSelchangeMode)
	ON_EN_CHANGE(IDC_EventTime, &CUserScreenDlg::OnEnChangeEventtime)
	ON_EN_CHANGE(IDC_IndexPeriod, &CUserScreenDlg::OnEnChangeIndexperiod)
	ON_BN_CLICKED(IDC_replaybutton0, &CUserScreenDlg::OnBnClickedreplaybutton0)
	ON_BN_CLICKED(IDC_replaybutton4, &CUserScreenDlg::OnBnClickedreplaybutton4)
	ON_BN_CLICKED(IDC_replaybutton1, &CUserScreenDlg::OnBnClickedreplaybutton1)
	ON_BN_CLICKED(IDC_replaybutton2, &CUserScreenDlg::OnBnClickedreplaybutton2)
	ON_BN_CLICKED(IDC_replaybutton5, &CUserScreenDlg::OnBnClickedreplaybutton5)
	ON_CBN_SELCHANGE(IDC_ReplaySpeed, &CUserScreenDlg::OnCbnSelchangeReplayspeed)
END_MESSAGE_MAP()


// CUserScreenDlg 메시지 처리기
void CUserScreenDlg::OnBnClickedTestbutton()
{
	m_bReqUpdateDrawData = TRUE;
}

BOOL CUserScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	InitMemDC();
	SetDlgItemInt(IDC_Color0, 128);
	SetDlgItemInt(IDC_Color1, 128);
	SetDlgItemInt(IDC_Color2, 128);
	SetTimer(TIMER_CHECK_USERSCREEN, 30, NULL);
	SetTimer(TIMER_CHECK_FRAME_RATE, 2000, NULL);

	SetDlgItemInt(IDC_EventTime, 100);
	SetDlgItemInt(IDC_IndexPeriod, 1);
	m_ctrlSpeed.SetCurSel(0);
	m_ctrlMode.SetCurSel(0);
	m_ctrlReplayStop.SetCurSel(0);
	OnCbnSelchangeMode();
	   
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUserScreenDlg::DrawBackground() {
	CClientDC dc(this); // device context for painting

	CString jpg(pMain->m_strGraphLog_Jpg);
	int len = MultiByteToWideChar(CP_ACP, 0, jpg, -1, NULL, 0);
	WCHAR* wcharStr = new WCHAR[len];
	MultiByteToWideChar(CP_ACP, 0, jpg, -1, wcharStr, len);
	Gdiplus::Image image(wcharStr); // JPG 파일 경로	
	Gdiplus::Graphics graphics(m_MemDC_Img.m_hDC);

	m_iWidth = image.GetWidth();
	m_iHeight = image.GetHeight();
	if (m_iWidth == 0 || m_iHeight == 0) {
		m_iWidth = 900;
		m_iHeight = 400;
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));
		m_MemDC_Img.SelectObject(&brush);
		m_MemDC_Img.Rectangle(-10, -10, 1000, 1000);
		SetWindowPos(NULL, 0, 0, m_iWidth + US_PX * 2 + 15, m_iHeight + US_PY + 50, SWP_NOMOVE | SWP_NOZORDER);
	}
	else {
		SetWindowPos(NULL, 0, 0, m_iWidth + US_PX * 2 + 15, m_iHeight + US_PY + 50, SWP_NOMOVE | SWP_NOZORDER);
		graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
	}

	m_iTimeLineP[0] = 25;
	m_iTimeLineP[1] = m_iWidth - 25;
}

void CUserScreenDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case TIMER_CHECK_USERSCREEN:
		if (m_bBusyDrawData == FALSE) {
			UpdateDraw();
		}
		break;

	case TIMER_CHECK_FRAME_RATE:
		CheckFrameRate();
		break;


	case TIMER_REPLAY:
		SetNextIndex();
		break;


	default:
		CString str;
		str.Format("Unknown Timer ID : %d", nIDEvent);
		AfxMessageBox(str);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CUserScreenDlg::CheckFrameRate() {
	static int pre = -1;
	static int iHistoryIndex = pMain->m_iHistoryIndex;
	if (m_bAdjustDisplayDataSize == FALSE) {
		return;
	}

	if (pre != m_iLastDrawFrame) {
		pre = m_iLastDrawFrame;
	}
	else {
		// 분석 모드이거나 data 수집 모드인데, m_iHistoryIndex의 변화가 없다면 
		if (pMain->m_iProgramMode == DATA_ANALYZE_MODE) { 
			iHistoryIndex = pMain->m_iHistoryIndex; 
			return; 
		}
		if (pMain->m_iHistoryIndex > 0) {
			// Display Data Size를 10%씩 줄인다
			int display_size = pMain->m_iDisplaySize * 9 / 10;
		}
		iHistoryIndex = pMain->m_iHistoryIndex;
	}
}


void CUserScreenDlg::UpdateDraw() {	
	CString str;
	CPen pen;
	CBrush brush;
	static int frameCount = 0;

	m_bBusyDrawData = TRUE;
	CClientDC dc(this);
	if (m_bReqUpdateDrawData) {
		unsigned int startTick = GetTickCount();
		m_MemDC_US.BitBlt(US_PX, US_PY, m_iWidth, m_iHeight, &m_MemDC_Img, 0, 0, SRCCOPY);
		for (int i = 0; i < pMain->m_iGraphLogCount; i++) {
			if (m_DrawData[i].use == FALSE) {
				continue;
			}
			brush.DeleteObject();
			pen.DeleteObject();
			brush.CreateSolidBrush((COLORREF)m_DrawData[i].color1);
			pen.CreatePen(PS_SOLID, 1, (COLORREF)m_DrawData[i].color1);
			m_MemDC_US.SelectObject(&brush);
			m_MemDC_US.SelectObject(&pen);
			m_MemDC_US.Rectangle(m_DrawData[i].range[0], m_DrawData[i].range[1], m_DrawData[i].range[2], m_DrawData[i].range[3]);

			if (m_DrawData[i].type > 0 && m_DrawData[i].strText.GetLength() > 0) {
				m_MemDC_US.SetTextColor(m_DrawData[i].color2);
				m_MemDC_US.SetBkColor(m_DrawData[i].color1);
				m_MemDC_US.TextOutA(m_DrawData[i].ptText.x, m_DrawData[i].ptText.y, m_DrawData[i].strText);
			}
		}

		m_iLastDrawFrame = pMain->m_iCurrentIndex;
		DrawTimeLine();
		dc.BitBlt(US_PX, US_PY, m_iWidth, m_iHeight, &m_MemDC_US, US_PX, US_PY, SRCCOPY);
		if (m_EditMode) {
			m_bReqUpdateEditData = TRUE;
		}
		str.Format("Index: %d / %d      (#%d)     %dms", m_iIndex, pMain->m_iHistoryIndex, ++frameCount, GetTickCount() - startTick);
		SetDlgItemText(IDC_Message, str);
	}	

	if (m_bReqUpdateEditData) {
		if (m_bReqUpdateDrawData == FALSE) {
			dc.BitBlt(US_PX, US_PY, m_iWidth, m_iHeight, &m_MemDC_US, US_PX, US_PY, SRCCOPY);
		}
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 1, RGB(GetDlgItemInt(IDC_Color0), GetDlgItemInt(IDC_Color1), GetDlgItemInt(IDC_Color2)));
		dc.SelectObject(&pen);

		m_rect.SetRect(GetDlgItemInt(IDC_P0), GetDlgItemInt(IDC_P1), GetDlgItemInt(IDC_P0) + GetDlgItemInt(IDC_D0), GetDlgItemInt(IDC_P1) + GetDlgItemInt(IDC_D1));

		if (IsDlgButtonChecked(IDC_Fill)) {
			brush.DeleteObject();
			brush.CreateSolidBrush(RGB(GetDlgItemInt(IDC_Color0), GetDlgItemInt(IDC_Color1), GetDlgItemInt(IDC_Color2)));
			dc.SelectObject(&brush);
			dc.Rectangle(m_rect);
		}
		else {
			dc.MoveTo(m_rect.TopLeft());
			dc.LineTo(m_rect.right, m_rect.top);
			dc.LineTo(m_rect.BottomRight());
			dc.LineTo(m_rect.left, m_rect.bottom);
			dc.LineTo(m_rect.TopLeft());
		}
	}

	m_bReqUpdateDrawData = FALSE;
	m_bReqUpdateEditData = FALSE;
	m_bBusyDrawData = FALSE;
}

void CUserScreenDlg::InitMemDC() {

	CClientDC dc(this);
	m_MemDC_US.CreateCompatibleDC(&dc);
	m_Bitmap_US.CreateCompatibleBitmap(&dc, 2200, 2000); 
	m_pOldBitmap_US = m_MemDC_US.SelectObject(&m_Bitmap_US);

	m_MemDC_Img.CreateCompatibleDC(&dc);
	m_Bitmap_Img.CreateCompatibleBitmap(&dc, 2200, 2000);
	m_pOldBitmap_Img = m_MemDC_Img.SelectObject(&m_Bitmap_Img);

	m_MemDC_Edit.CreateCompatibleDC(&dc);
	m_Bitmap_Edit.CreateCompatibleBitmap(&dc, 2200, 2000);
	m_pOldBitmap_Edit = m_MemDC_Edit.SelectObject(&m_Bitmap_Edit);

	DrawBackground();
}

void CUserScreenDlg::OnPaint()
{
	CPaintDC dc(this); 
	dc.BitBlt(US_PX, US_PY, m_iWidth, m_iHeight, &m_MemDC_US, US_PX, US_PY, SRCCOPY);
}

void CUserScreenDlg::OnCancel() {
	OnBnClickedHidewindow();
}

void CUserScreenDlg::OnBnClickedUpP0()
{
	int val = GetDlgItemInt(IDC_P0) + 1;
	SetDlgItemInt(IDC_P0, val);
}


void CUserScreenDlg::OnBnClickedUpP1()
{
	int val = GetDlgItemInt(IDC_P1) + 1;
	SetDlgItemInt(IDC_P1, val);
}

void CUserScreenDlg::OnBnClickedDnP0()
{
	int val = max(0, GetDlgItemInt(IDC_P0) - 1);
	SetDlgItemInt(IDC_P0, val);
}


void CUserScreenDlg::OnBnClickedDnP1()
{
	int val = max(0, GetDlgItemInt(IDC_P1) - 1);
	SetDlgItemInt(IDC_P1, val);
}



void CUserScreenDlg::OnBnClickedUpD0()
{
	int val = GetDlgItemInt(IDC_D0) + 1;
	SetDlgItemInt(IDC_D0, val);
	
}


void CUserScreenDlg::OnBnClickedUpD1()
{
	int val = GetDlgItemInt(IDC_D1) + 1;
	SetDlgItemInt(IDC_D1, val);
}


void CUserScreenDlg::OnBnClickedDnD0()
{
	int val = max(0, GetDlgItemInt(IDC_D0) - 1);
	SetDlgItemInt(IDC_D0, val);
}


void CUserScreenDlg::OnBnClickedDnD1()
{
	int val = max(0, GetDlgItemInt(IDC_D1) - 1);
	SetDlgItemInt(IDC_D1, val);
}


void CUserScreenDlg::OnBnClickedUpC0()
{
	int val = max(0, min(255, GetDlgItemInt(IDC_Color0) + 10));
	SetDlgItemInt(IDC_Color0, val);
	
}


void CUserScreenDlg::OnBnClickedUpC1()
{
	int val = max(0, min(255, GetDlgItemInt(IDC_Color1) + 10));
	SetDlgItemInt(IDC_Color1, val);
}


void CUserScreenDlg::OnBnClickedUpC2()
{
	int val = max(0, min(255, GetDlgItemInt(IDC_Color2) + 10));
	SetDlgItemInt(IDC_Color2, val);
}


void CUserScreenDlg::OnBnClickedDnC0()
{
	int val = max(0, GetDlgItemInt(IDC_Color0) - 10);
	SetDlgItemInt(IDC_Color0, val);
}


void CUserScreenDlg::OnBnClickedDnC1()
{
	int val = max(0, GetDlgItemInt(IDC_Color1) - 10);
	SetDlgItemInt(IDC_Color1, val);
}


void CUserScreenDlg::OnBnClickedDnC2()
{
	int val = max(0, GetDlgItemInt(IDC_Color2) - 10);
	SetDlgItemInt(IDC_Color2, val);
}


void CUserScreenDlg::OnEnChangeP0()
{
	int val = GetDlgItemInt(IDC_P0);
	if (val < 0) {
		val = 0;
		SetDlgItemInt(IDC_P0, 0);
	}
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnEnChangeP1()
{
	int val = GetDlgItemInt(IDC_P1);
	if (val < 0) {
		val = 0;
		SetDlgItemInt(IDC_P1, 0);
	}
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnEnChangeD0()
{
	int val = GetDlgItemInt(IDC_D0);
	if (val < 1) {
		val = 1;
		SetDlgItemInt(IDC_D0, 1);
	}
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnEnChangeD1()
{
	int val = GetDlgItemInt(IDC_D1);
	if (val < 1) {
		val = 1;
		SetDlgItemInt(IDC_D1, 1);
	}
	m_bReqUpdateEditData = TRUE;
}


BOOL CUserScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CUserScreenDlg::OnBnClickedHidewindow() {
	m_ReplayMode = FALSE;
	m_iReplayDir = 0;
	KillTimer(TIMER_REPLAY);
	pMain->CheckDlgButton(IDC_ShowGraphLog, FALSE);
	pMain->OnBnClickedShowgraphlog();
}


void CUserScreenDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_EditMode && IsGraphArea(point)) {
		SetDlgItemInt(IDC_P0, point.x);
		SetDlgItemInt(IDC_P1, point.y);
		CDialogEx::OnRButtonDown(nFlags, point);
	}
}

BOOL CUserScreenDlg::IsGraphArea(CPoint pt) {
	if (US_PX <= pt.x && US_PY <= pt.y && pt.x <= m_iWidth + US_PX && pt.y <= m_iHeight + US_PY) {
		return TRUE;
	}
	return FALSE;
}

void CUserScreenDlg::OnEnChangeColor0()
{
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnEnChangeColor1()
{
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnEnChangeColor2()
{
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnBnClickedFill()
{
	m_bReqUpdateEditData = TRUE;
}


void CUserScreenDlg::OnBnClickedSendrange()
{
	pMain->m_DataConversion.range[0] = GetDlgItemInt(IDC_P0);
	pMain->m_DataConversion.range[1] = GetDlgItemInt(IDC_P1);
	pMain->m_DataConversion.range[2] = GetDlgItemInt(IDC_D0);
	pMain->m_DataConversion.range[3] = GetDlgItemInt(IDC_D1);
}


void CUserScreenDlg::OnBnClickedGetrange()
{
	SetDlgItemInt(IDC_P0, pMain->m_DataConversion.range[0]);
	SetDlgItemInt(IDC_P1, pMain->m_DataConversion.range[1]);
	SetDlgItemInt(IDC_D0, pMain->m_DataConversion.range[2]);
	SetDlgItemInt(IDC_D1, pMain->m_DataConversion.range[3]);
}

void CUserScreenDlg::OnBnClickedGetc1()
{
	COLORREF col = pMain->m_DataConversion.color1;
	SetDlgItemInt(IDC_Color0, GetRValue(col));
	SetDlgItemInt(IDC_Color1, GetGValue(col));
	SetDlgItemInt(IDC_Color2, GetBValue(col));
}

void CUserScreenDlg::OnBnClickedGetc2()
{
	COLORREF col = pMain->m_DataConversion.color1;
	SetDlgItemInt(IDC_Color0, GetRValue(col));
	SetDlgItemInt(IDC_Color1, GetGValue(col));
	SetDlgItemInt(IDC_Color2, GetBValue(col));
}


void CUserScreenDlg::OnBnClickedAutomode()
{
	m_bAdjustDisplayDataSize = IsDlgButtonChecked(IDC_AutoMode);
}


void CUserScreenDlg::DrawTimeLine() {
	m_MemDC_US.SelectObject(&m_brush_timeline);
	m_MemDC_US.SelectObject(&m_pen_timeline);
	m_MemDC_US.Rectangle(m_iTimeLineP[0], 55, m_iTimeLineP[1], 65);
	
	Cal_CurrentPos(m_iIndex);
	if (m_bReqChangeIndex) {
		m_MemDC_US.SelectObject(&m_brush_temp);	
	}
	else {
		m_MemDC_US.SelectObject(&m_brush_current);
	}
	m_MemDC_US.Rectangle(m_iCurrentPos - 20, 55 + 1, m_iCurrentPos + 20, 65 - 1);
	for (int i = 1; i < 10; i++) {
		m_MemDC_US.MoveTo(m_iCurrentPos + i * 4 - 20, 58);
		m_MemDC_US.LineTo(m_iCurrentPos + i * 4 - 20, 63);
	}
}


void CUserScreenDlg::Cal_CurrentPos(int idx) {
	int span = m_iTimeLineP[1] - m_iTimeLineP[0] - 48;
	if (pMain->m_iHistoryIndex > 0) {
		m_iCurrentPos = min(span, max(0, span * idx / pMain->m_iHistoryIndex)) + m_iTimeLineP[0] + 24;
	}
	else {
		m_iCurrentPos = (m_iTimeLineP[1] + m_iTimeLineP[0]) / 2;
	}
}

void CUserScreenDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);
	CString str;
	str.Format("%d %d", point.x, point.y);
	SetDlgItemText(IDC_Message, str);
	if (m_EditMode && IsGraphArea(point)) {
		m_StartPT = point;
	}
	else {
		m_StartPT = -1;
	}

	if (m_EditMode == FALSE) {
		if (point.x > m_iCurrentPos - 20 && point.x < m_iCurrentPos + 20 && point.y > 55 + 1 && point.y < 65 - 1 && pMain->m_iProgramMode != DATA_ACQUISITION_MODE) {
			m_bReqChangeIndex = TRUE;
			m_iCurrentIndexTemp = m_iLastDrawFrame;
		}
	}
	else {
		m_bReqChangeIndex = FALSE;
	}
}


void CUserScreenDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);

	if (m_EditMode && IsGraphArea(point) && m_StartPT.x != -1) {
		CPoint P1(min(m_StartPT.x, point.x), min(m_StartPT.y, point.y));
		CPoint P2(max(m_StartPT.x, point.x), max(m_StartPT.y, point.y));
		m_rect.SetRect(P1, P2);

		SetDlgItemInt(IDC_P0, m_rect.left);
		SetDlgItemInt(IDC_P1, m_rect.top);
		SetDlgItemInt(IDC_D0, m_rect.right - m_rect.left);
		SetDlgItemInt(IDC_D1, m_rect.bottom - m_rect.top);

		CString str;
		str.Format("(%d, %d) ~ (%d, %d)  dist(%d, %d)", P1.x, P1.y, P2.x, P2.y, m_rect.Width(), m_rect.Height());
		SetDlgItemText(IDC_Message, str);
	}

	if (m_EditMode == FALSE && m_bReqChangeIndex && pMain->m_iProgramMode != DATA_ACQUISITION_MODE && pMain->m_iHistoryIndex > 0) {

		int span = m_iTimeLineP[1] - m_iTimeLineP[0] - 48;
		int idx = max(0, min(pMain->m_iHistoryIndex - 1, (point.x - (m_iTimeLineP[0] + 24)) * pMain->m_iHistoryIndex / span));
		if (m_iLastDrawFrame != idx) {
			pMain->SetDlgItemInt(IDC_IndexNumber, idx);
		}
	}
	m_bReqChangeIndex = FALSE;
}

void CUserScreenDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	CDialogEx::OnMouseMove(nFlags, point);
	
	if (m_bReqChangeIndex) {
		int idx;
		if (m_EditMode == FALSE && pMain->m_iProgramMode != DATA_ACQUISITION_MODE && pMain->m_iHistoryIndex > 0) {
			int span = m_iTimeLineP[1] - m_iTimeLineP[0] - 48;
			idx = max(0, min(pMain->m_iHistoryIndex - 1, (point.x - (m_iTimeLineP[0] + 24)) * pMain->m_iHistoryIndex / span));
		}

		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
				m_iCurrentIndexTemp = idx;
		}
		else {
			m_bReqChangeIndex = FALSE;
			if (m_iLastDrawFrame != idx) {
				pMain->SetDlgItemInt(IDC_IndexNumber, idx);
			}
		}
	}
}


void CUserScreenDlg::OnCbnSelchangeMode()
{
	int iMode = m_ctrlMode.GetCurSel(); 
	if (iMode == -1) {
		iMode = 0;
		m_ctrlMode.SetCurSel(iMode);
	}
	int dy = 20;
	switch (iMode) {
	case 0: // Normal Mode
		m_bReplayOn = FALSE;
		m_EditMode = FALSE;
		m_ReplayMode = FALSE;
		dy = 17;
		GetDlgItem(IDC_Message)->MoveWindow(155, dy, m_iWidth - 50 - 190 - 10, 20, TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);

		GetDlgItem(IDC_EventTime)->ShowWindow(FALSE);
		GetDlgItem(IDC_IndexPeriod)->ShowWindow(FALSE);
		KillTimer(TIMER_REPLAY);
		GetDlgItem(IDC_AutoMode)->ShowWindow(TRUE);		
		break;

	case 1: // Edit Mode
		m_bReplayOn = FALSE;
		m_EditMode = TRUE;
		m_ReplayMode = FALSE;
		dy = 17;
		GetDlgItem(IDC_Message)->MoveWindow(638, dy, m_iWidth - 420 - 85 - 20 - 23 - 184, 20, TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);

		GetDlgItem(IDC_EventTime)->ShowWindow(FALSE);
		GetDlgItem(IDC_IndexPeriod)->ShowWindow(FALSE);
		KillTimer(TIMER_REPLAY);

		GetDlgItem(IDC_AutoMode)->ShowWindow(FALSE);
		break;

	case 2: // replay Mode
		m_bReplayOn = FALSE;
		m_EditMode = FALSE;
		m_ReplayMode = TRUE;
		dy = 13;
		GetDlgItem(IDC_Message2)->MoveWindow(550, dy,50, 20, TRUE);

		GetDlgItem(IDC_Message)->MoveWindow(605, dy, m_iWidth + 10 - 520 - 90 - 30 +2 - 414 + 363, 20, TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);
		
		GetDlgItem(IDC_EventTime)->ShowWindow(TRUE);
		GetDlgItem(IDC_IndexPeriod)->ShowWindow(TRUE);
		
		int time = min(10000, max(10, GetDlgItemInt(IDC_EventTime)));
		m_iReplayStatus = 0; 
		KillTimer(TIMER_REPLAY);
		m_iReplayDir = 0;
		UpdateReplayMessage();
		GetDlgItem(IDC_AutoMode)->ShowWindow(FALSE);
		break;
	}
	CheckDlgButton(IDC_AutoMode, FALSE);
	m_iReplayDir = 0;
	int Idc[26] = { IDC_P0 , IDC_P1, IDC_D0, IDC_D1, IDC_Color0, IDC_Color1, IDC_Color2, IDC_Up_P0, IDC_Up_P1, IDC_Dn_P0, IDC_Dn_P1, IDC_Up_D0, IDC_Up_D1, IDC_Dn_D0, IDC_Dn_D1, IDC_Up_C0, IDC_Up_C1, IDC_Up_C2,IDC_Dn_C0, IDC_Dn_C1, IDC_Dn_C2, IDC_Fill,IDC_SendRange,IDC_GetRange, IDC_GetC1, IDC_GetC2,	};

	for (int i = 0; i < 26; i++) {
		GetDlgItem(Idc[i])->ShowWindow(m_EditMode);
	}

	int idc_button[8] = { IDC_Message2, IDC_replaybutton0, IDC_replaybutton1, IDC_replaybutton2, IDC_replaybutton5, IDC_replaybutton4, IDC_ReplayStop, IDC_ReplaySpeed };
	for (int i = 0; i < 8; i++) {
		GetDlgItem(idc_button[i])->ShowWindow(iMode == 2);
	}
	
	GetDlgItem(IDC_TestButton)->MoveWindow(m_iWidth + 10 - 80 - 20 - 3, dy, 20, 20, TRUE);
	GetDlgItem(IDC_HideWindow)->MoveWindow(m_iWidth + 10 - 80, dy, 80, 20, TRUE);
}

void CUserScreenDlg::OnEnChangeEventtime()
{
	int time = min(10000, max(10, GetDlgItemInt(IDC_EventTime)));
	CString str1, str2;
	GetDlgItemText(IDC_EventTime, str1);
	str2.Format("%d", time);
	if (str1 != str2) {
		SetDlgItemText(IDC_EventTime, str2);
	}

	SetTimer(TIMER_REPLAY, time, NULL);

}


void CUserScreenDlg::OnEnChangeIndexperiod()
{
	int period = max(1, GetDlgItemInt(IDC_IndexPeriod));
	CString str1, str2;
	GetDlgItemText(IDC_IndexPeriod, str1);
	str2.Format("%d", period);
	if (str1 != str2) {
		SetDlgItemText(IDC_IndexPeriod, str2);
	}
}


void CUserScreenDlg::SetNextIndex() {
	if (m_bReplayOn == FALSE) { return;  }
	int period = GetDlgItemInt(IDC_IndexPeriod);
	if (period == 0) { return; }

	int nextIndex = m_iIndex + period * m_iReplaySpeed * m_iReplayDir;
	if (nextIndex < 0) {
		nextIndex = 0;
	}
	if (nextIndex >= pMain->m_iHistoryIndex) {
		nextIndex = max(0, pMain->m_iHistoryIndex - 1);
	}

	if (m_iIndex != nextIndex) {
		m_iReqReplayIndex = nextIndex;
	}
	else {
		int iMode = m_ctrlReplayStop.GetCurSel();
		if (iMode == 0) {
			// Stop End
			if (nextIndex == 0 || pMain->m_iHistoryIndex - 1 == nextIndex) {
				OnBnClickedreplaybutton1();
			}
		}
		else if (iMode == 1) {
			if (nextIndex == 0 || pMain->m_iHistoryIndex - 1 == nextIndex) {
				if (m_iReplayDir == 1) {
					m_iReplayDir = -1;
					UpdateReplayMessage();
				}
				else if (m_iReplayDir == -1) {
					m_iReplayDir = 1;
					UpdateReplayMessage();
				}
			}
		}
		else if (iMode == 2) {
			if (nextIndex == 0) {
				m_iIndex = max(0, pMain->m_iHistoryIndex - 1);
			}
			else if (nextIndex == pMain->m_iHistoryIndex - 1) {
				m_iIndex = 0;
			}
		}
	}
}

void CUserScreenDlg::OnBnClickedreplaybutton0()
{
	m_iIndex = 1;
}


void CUserScreenDlg::OnBnClickedreplaybutton4()
{
	m_iIndex = max(0, pMain->m_iHistoryIndex - 2);
}


void CUserScreenDlg::OnBnClickedreplaybutton1()
{
	// Pause Button
	m_bReplayOn = FALSE;
	m_iReplayStatus = 0;
	m_iReplayDir = 0;
	UpdateReplayMessage();
}


void CUserScreenDlg::OnBnClickedreplaybutton2()
{
	m_iReplayDir = 1;
	if (m_bReplayOn == FALSE) {
		OnEnChangeEventtime();
	}
	m_bReplayOn = TRUE;
	UpdateReplayMessage();
}

void CUserScreenDlg::UpdateReplayMessage() {
	CString str;
	if (m_iReplayDir > 0) {
		str.Format("Forwd");
	}
	else if (m_iReplayDir < 0) {
		str.Format("Backwd");
	}
	else { 
		str = "Stop";
	}

	SetDlgItemText(IDC_Message2, str);
}


void CUserScreenDlg::OnBnClickedreplaybutton5()
{
	// Backward
	m_iReplayDir = -1;
	if (m_bReplayOn == FALSE) {
		OnEnChangeEventtime();
	}
	m_bReplayOn = TRUE;
	UpdateReplayMessage();
}


void CUserScreenDlg::OnCbnSelchangeReplayspeed()
{
	static int accTbl[11] = { 1, 2, 3, 5, 10, 15, 20, 25, 30, 50, 100 };
	m_iReplaySpeed = accTbl[m_ctrlSpeed.GetCurSel()];

	if (m_bReplayOn == FALSE) {
		OnEnChangeEventtime();
		m_bReplayOn = TRUE;
	}
}
