// ColorSelectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "ColorSelectDlg.h"
#include "afxdialogex.h"


// CColorSelectDlg 대화 상자

IMPLEMENT_DYNAMIC(CColorSelectDlg, CDialogEx)

CColorSelectDlg::CColorSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ColorSelectDlg, pParent)
	, m_strMessage(_T(""))
	, m_iPenType(0)
	, m_iPenThickness(0)
	, m_bUseMark(FALSE)
	, m_iMode(0)
	, m_iSelectItem(0)
{

}

CColorSelectDlg::~CColorSelectDlg()
{
}

void CColorSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Message, m_strMessage);
	DDX_Text(pDX, IDC_PreProperty, m_strPreRGB[0]);
	DDX_Text(pDX, IDC_PreProperty1, m_strPreRGB[1]);
	DDX_Text(pDX, IDC_PreProperty2, m_strPreRGB[2]);
	DDX_Control(pDX, IDC_PenType, m_ctrlLineType);
	DDX_CBIndex(pDX, IDC_PenType, m_iPenType);
	DDX_CBIndex(pDX, IDC_ThicknessType, m_iPenThickness);
	DDX_Check(pDX, IDC_DrawMark, m_bUseMark);
	DDX_Text(pDX, IDC_CurrentProperty0, m_strColor[0][0]);
	DDX_Text(pDX, IDC_CurrentProperty1, m_strColor[1][0]);
	DDX_Text(pDX, IDC_CurrentProperty2, m_strColor[2][0]);
	DDX_Text(pDX, IDC_CurrentProperty10, m_strColor[0][1]);
	DDX_Text(pDX, IDC_CurrentProperty11, m_strColor[1][1]);
	DDX_Text(pDX, IDC_CurrentProperty12, m_strColor[2][1]);
	DDX_Text(pDX, IDC_CurrentProperty20, m_strColor[0][2]);
	DDX_Text(pDX, IDC_CurrentProperty21, m_strColor[1][2]);
	DDX_Text(pDX, IDC_CurrentProperty22, m_strColor[2][2]);
	DDX_Radio(pDX, IDC_SelectItem0, m_iSelectItem);
	DDX_Control(pDX, IDC_IncVal, m_ctrlInc);
}


BEGIN_MESSAGE_MAP(CColorSelectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SelectColor, &CColorSelectDlg::OnBnClickedSelectcolor)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CColorSelectDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_PenType, &CColorSelectDlg::OnCbnSelchangePentype)
	ON_CBN_SELCHANGE(IDC_ThicknessType, &CColorSelectDlg::OnCbnSelchangePentype)
	ON_BN_CLICKED(IDC_DrawMark, &CColorSelectDlg::OnBnClickedDrawmark)
	ON_EN_CHANGE(IDC_CurrentProperty0, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty1, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty2, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty10, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty11, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty12, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty20, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty21, &CColorSelectDlg::ChangeColorProperty)
	ON_EN_CHANGE(IDC_CurrentProperty22, &CColorSelectDlg::ChangeColorProperty)
	ON_BN_CLICKED(IDC_GetDefaultColor, &CColorSelectDlg::OnBnClickedGetdefaultcolor)
	ON_BN_CLICKED(IDC_GetDefaultColor1, &CColorSelectDlg::OnBnClickedGetdefaultcolor1)
	ON_BN_CLICKED(IDC_GetDefaultColor2, &CColorSelectDlg::OnBnClickedGetdefaultcolor2)
	ON_BN_CLICKED(IDC_SelectItem0, &CColorSelectDlg::OnBnClickedSelectitem0)
	ON_BN_CLICKED(IDC_SelectItem1, &CColorSelectDlg::OnBnClickedSelectitem0)
	ON_BN_CLICKED(IDC_SelectItem2, &CColorSelectDlg::OnBnClickedSelectitem0)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_R_UP, &CColorSelectDlg::OnBnClickedRUp)
	ON_BN_CLICKED(IDC_R_UP2, &CColorSelectDlg::OnBnClickedRUp2)
	ON_BN_CLICKED(IDC_R_UP3, &CColorSelectDlg::OnBnClickedRUp3)
	ON_BN_CLICKED(IDC_R_DN, &CColorSelectDlg::OnBnClickedRDn)
	ON_BN_CLICKED(IDC_R_DN2, &CColorSelectDlg::OnBnClickedRDn2)
	ON_BN_CLICKED(IDC_R_DN3, &CColorSelectDlg::OnBnClickedRDn3)
END_MESSAGE_MAP()


void CColorSelectDlg::DrawInit(){
	CClientDC dc(this);
	if (m_MemDC_Colormap == NULL) { DrawColorMap(); }
	
	// ColorMap
	dc.BitBlt(m_iLimitColorMap[0][0], m_iLimitColorMap[1][0], m_iLimitColorMap[0][1] - m_iLimitColorMap[0][0], m_iLimitColorMap[1][1] - m_iLimitColorMap[1][0], &m_MemDC_Colormap, m_iLimitColorMap[0][0], m_iLimitColorMap[1][0], SRCCOPY);
	
}

void CColorSelectDlg::DrawColorMap() {
	//CClientDC dc(this);
	CPen pen;
	CBrush brush;
	int iSize = COLOR_PIXEL_SIZE;
	int x0 = COLOR_MAP_START_X, y0 = COLOR_MAP_START_Y;
	int d = 1;
	int nGrid = COLOR_MAP_N_GRID;
	int nGridBlueY = (int)sqrt( nGrid);
	int nGridBlueX = nGrid / nGridBlueY;
	int nGridMultiply = 256 / nGrid;

	m_iLimitColorMap[0][0] = max(0, x0 - 2 * d);
	m_iLimitColorMap[1][0] = max(0, y0 - 2 * d);
	m_iLimitColorMap[0][1] = x0 + (2 * nGrid + 1) * iSize + d;
	m_iLimitColorMap[1][1] = y0 + 2 * nGrid * iSize + d;

	m_iLimitSubColorMap[0][0] = m_iLimitColorMap[0][1] + 5;
	m_iLimitSubColorMap[0][1] = m_iLimitSubColorMap[0][0] + 25;
	m_iLimitSubColorMap[1][0] = m_iLimitColorMap[1][0];
	m_iLimitSubColorMap[1][1] = m_iLimitColorMap[1][1];

	m_MemDC_Colormap.Rectangle(x0 - 2* d, y0 - 2 * d, x0 + (2 * nGrid + 1) * iSize + d, y0 + 2* nGrid * iSize + d);

	for (int r = 0; r < nGrid; r++) {
		for (int g = 0; g < nGrid; g++) {
			for (int b = 0; b < nGrid; b++) {
				int x, y;
				if (b / nGridBlueX == 0)
					x = (r + b / nGridBlueX * nGrid) * iSize;
				else 
					x = (nGrid - r + b / nGridBlueX * nGrid - 1) * iSize;
				y = (g * nGridBlueY + b % nGridBlueY) * iSize;

				int r1 = min(r * nGridMultiply, 255);
				int g1 = min(g * nGridMultiply, 255);
				int b1 = min(b * nGridMultiply, 255);

				pen.CreatePen(PS_SOLID, 1, RGB(r1, g1, b1));
				brush.CreateSolidBrush(RGB(r1, g1, b1));
				m_MemDC_Colormap.SelectObject(&pen);
				m_MemDC_Colormap.SelectObject(&brush);
				m_MemDC_Colormap.Rectangle(x0 + x, y0 + y, x0 + x + iSize-1, y0 + y + iSize-1);
			}
		}
	}
	
	for (int i = 0; i < 2 * nGrid; i++) {
		COLORREF color;
		switch (i) {
		case 0: color = RGB(255, 255, 255); break;
		case 1: color = RGB(192, 192, 192); break;
		case 2: color = RGB(128, 128, 128); break;
		case 3: color = RGB(64, 64, 64); break;
		case 4: color = RGB(0, 0, 0); break;
		case 5: color = RGB(255, 192, 255); break;
		case 6: color = RGB(255, 0, 255); break;
		case 7: color = RGB(255, 0, 0); break;
		case 8: color = RGB(128, 0, 0); break;
		case 9: color = RGB(192, 255, 255); break;
		case 10: color = RGB(0, 192, 192); break;
		case 11: color = RGB(255, 255, 128); break;
		case 12: color = RGB(192, 192, 64); break;
		case 13: color = RGB(0, 255, 0); break;
		case 14: color = RGB(0, 0, 255); break;
		case 15: color = RGB(0, 0, 128); break;
		}
		pen.CreatePen(PS_SOLID, 1, color);
		brush.CreateSolidBrush(color);
		m_MemDC_Colormap.SelectObject(&pen);
		m_MemDC_Colormap.SelectObject(&brush);
		int x = 2 * nGrid * iSize;
		int y = i * iSize;

		m_MemDC_Colormap.Rectangle(x0 + x, y0 + y, x0 + x + iSize-1 , y0 + y + iSize-1);
	}
}

BOOL CColorSelectDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	CClientDC dc(this);
	m_MemDC_Colormap.CreateCompatibleDC(&dc);
	m_Bitmap_Colormap.CreateCompatibleBitmap(&dc, 600, 600); 
	m_pOldBitmap_Colormap = m_MemDC_Colormap.SelectObject(&m_Bitmap_Colormap);
	pAppMain->m_PropertyChange.bIsChanged = FALSE;
		
	for (int i = 0; i < N_COLOR_DRAW; i++) {
		for (int j = 0; j < MAX_COLOR_ITEM; j++) {
			m_Color[i][0][j] = pAppMain->m_PropertyChange.Color[0][j];
			m_Color[i][1][j] = pAppMain->m_PropertyChange.Color[1][j];
			m_Color[i][2][j] = pAppMain->m_PropertyChange.Color[2][j];
		}
		m_bDrawSelectColor[i] = TRUE;
	}
	m_bUseMark = pAppMain->m_PropertyChange.bUseMark;
	m_iPenThickness = pAppMain->m_PropertyChange.iThickness-1;	
	for (int j = 0; j < MAX_COLOR_ITEM; j++) {
		m_strColor[0][j].Format("%d", m_Color[1][0][j]);
		m_strColor[1][j].Format("%d", m_Color[1][1][j]);
		m_strColor[2][j].Format("%d", m_Color[1][2][j]);
		m_strPreRGB[j].Format("R:%d, G:%d, B:%d", m_Color[0][0][j], m_Color[0][1][j], m_Color[0][2][j]);
	}
	
	m_iPenType = pAppMain->m_PropertyChange.iLineType;
	ShowControl();

	m_R = m_Color[0][0][0];
	m_G = m_Color[0][1][0];
	m_B = m_Color[0][2][0];
	DrawColorMap();	
	DrawSubColorMap();
	m_ctrlInc.SetCurSel(0);
	UDF;

	SetTimer(10, 30, NULL);
	return TRUE; 
}


void CColorSelectDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawInit();
	UpdateControl();
}

void CColorSelectDlg::OnRButtonDown(UINT nFlags, CPoint point)
{

	CDialogEx::OnRButtonDown(nFlags, point);
	m_strMessage.Format("X:%d, Y:%d", point.x, point.y);
	UDF;
}


void CColorSelectDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	CDialogEx::OnLButtonDown(nFlags, point);
	BOOL bIsColorMap = FALSE;
	BOOL bUpdateColorMap = FALSE;
	if (point.y >= m_iLimitColorMap[1][0] && point.y <= m_iLimitColorMap[1][1]) {
		if (point.x >= m_iLimitColorMap[0][0] && point.x <= m_iLimitColorMap[0][1]) {
			bIsColorMap = TRUE;
			bUpdateColorMap = TRUE;
		}
		if (point.x >= m_iLimitSubColorMap[0][0] && point.x <= m_iLimitSubColorMap[0][1]) {
			bIsColorMap = TRUE;
		}
	}

	if (bIsColorMap) {
		m_bSubColorMode = TRUE;
		if (m_iMode == 0) {
			COLORREF Color = GetPixel(m_MemDC_Colormap, point.x, point.y);
			m_R = m_Color[1][0][0] = GetRValue(Color);
			m_G = m_Color[1][1][0] = GetGValue(Color);
			m_B = m_Color[1][2][0] = GetBValue(Color);
			m_bDrawSelectColor[1] = TRUE;
			int iByte_Pixel = GetDeviceCaps(m_MemDC_Colormap, BITSPIXEL) / 8;
			int r = 0, g = 0, b = 0;
			if (iByte_Pixel == 2) {
				r = Color & 0x1F;
				g = (Color >> 6) & 0x1F;
				b = (Color >> 11) & 0x1F;
			}
			else if (iByte_Pixel == 4) {
				r = Color & 0xFF;
				g = (Color >> 8) & 0xFF;
				b = (Color >> 16) & 0xFF;
			}
			if (r == 255 && g == 255 && b == 255) {
				if (point.x <= m_iLimitColorMap[0][1] - COLOR_PIXEL_SIZE) return;
				if (point.x <= m_iLimitColorMap[0][1] && point.y > m_iLimitColorMap[1][0] + COLOR_PIXEL_SIZE) return;
			}
			m_strColor[0][0].Format("%d", m_Color[1][0][0]);
			m_strColor[1][0].Format("%d", m_Color[1][1][0]);
			m_strColor[2][0].Format("%d", m_Color[1][2][0]);
			UDF;
			m_bReqRedraw = TRUE;
			UpdateControl();
		}
		else if (m_iMode == 1) {
			COLORREF Color = GetPixel(m_MemDC_Colormap, point.x, point.y);
			m_R = m_Color[1][0][m_iSelectItem] = GetRValue(Color);
			m_G = m_Color[1][1][m_iSelectItem] = GetGValue(Color);
			m_B = m_Color[1][2][m_iSelectItem] = GetBValue(Color);
			m_bDrawSelectColor[1] = TRUE;

			int iByte_Pixel = GetDeviceCaps(m_MemDC_Colormap, BITSPIXEL) / 8;
			int r = 0, g = 0, b = 0;
			if (iByte_Pixel == 2) {
				r = Color & 0x1F;
				g = (Color >> 6) & 0x1F;
				b = (Color >> 11) & 0x1F;
			}
			else if (iByte_Pixel == 4) {
				r = Color & 0xFF;
				g = (Color >> 8) & 0xFF;
				b = (Color >> 16) & 0xFF;
			}
			if (r == 255 && g == 255 && b == 255) {
				if (point.x <= m_iLimitColorMap[0][1] - COLOR_PIXEL_SIZE) return;
				if (point.x <= m_iLimitColorMap[0][1] && point.y < m_iLimitColorMap[1][1] - COLOR_PIXEL_SIZE) return;
			}
			m_strColor[0][m_iSelectItem].Format("%d", m_Color[1][0][m_iSelectItem]);
			m_strColor[1][m_iSelectItem].Format("%d", m_Color[1][1][m_iSelectItem]);
			m_strColor[2][m_iSelectItem].Format("%d", m_Color[1][2][m_iSelectItem]);
			UDF;
			m_bReqRedraw = TRUE;
			UpdateControl();
		}
		else if (m_iMode == 2) {
			COLORREF Color = GetPixel(m_MemDC_Colormap, point.x, point.y);
			m_R = m_Color[1][0][0] = GetRValue(Color);
			m_G = m_Color[1][1][0] = GetGValue(Color);
			m_B = m_Color[1][2][0] = GetBValue(Color);
			m_bDrawSelectColor[1] = TRUE;

			int iByte_Pixel = GetDeviceCaps(m_MemDC_Colormap, BITSPIXEL) / 8;
			int r = 0, g = 0, b = 0;
			if (iByte_Pixel == 2) {
				r = Color & 0x1F;
				g = (Color >> 6) & 0x1F;
				b = (Color >> 11) & 0x1F;
			}
			else if (iByte_Pixel == 4) {
				r = Color & 0xFF;
				g = (Color >> 8) & 0xFF;
				b = (Color >> 16) & 0xFF;
			}
			if (r == 255 && g == 255 && b == 255) {
				if (point.x <= m_iLimitColorMap[0][1] - COLOR_PIXEL_SIZE) return;
				if (point.x <= m_iLimitColorMap[0][1] && point.y < m_iLimitColorMap[1][1] - COLOR_PIXEL_SIZE) return;
			}
			m_strColor[0][0].Format("%d", m_Color[1][0][0]);
			m_strColor[1][0].Format("%d", m_Color[1][1][0]);
			m_strColor[2][0].Format("%d", m_Color[1][2][0]);
			UDF;
			m_bReqRedraw = TRUE;
			UpdateControl();
		}

		if (bUpdateColorMap) {
			DrawSubColorMap();
			CopySubColorMap();
		}
		else {
			DrawSubColorMapChecked(point.y);
		}
	}	
}


void CColorSelectDlg::OnMouseMove(UINT nFlags, CPoint point) {
	CDialogEx::OnMouseMove(nFlags, point);

	if (m_bSubColorMode) {
		BOOL bIsColorMap = FALSE;
		BOOL bUpdateColorMap = FALSE;
		if (point.y >= m_iLimitColorMap[1][0] && point.y <= m_iLimitColorMap[1][1]) {
			if (point.x >= m_iLimitColorMap[0][0] && point.x <= m_iLimitColorMap[0][1]) {
				bIsColorMap = TRUE;
				bUpdateColorMap = TRUE;
			}
			if (point.x >= m_iLimitSubColorMap[0][0] && point.x <= m_iLimitSubColorMap[0][1]) {
				bIsColorMap = TRUE;
				static int PtY = -100;
				if (abs(point.y - PtY) < 3) return;
				PtY = point.y;
			}
		}

		if (bIsColorMap) {
			if (m_iMode == 0) {
				COLORREF Color = GetPixel(m_MemDC_Colormap, point.x, point.y);

				static int pc[3] = { -100, -100, -100 };
				int c[3];
				c[0] = GetRValue(Color);
				c[1] = GetGValue(Color);
				c[2] = GetBValue(Color);

				if (c[0] == 255 && c[1] == 255 && c[2] == 255) {
					if (point.x <= m_iLimitColorMap[0][1] - COLOR_PIXEL_SIZE) return;
					if (point.x <= m_iLimitColorMap[0][1] && point.y < m_iLimitColorMap[1][1] - COLOR_PIXEL_SIZE) return;
				}
				if (abs(pc[0] - c[0]) < 3 && abs(pc[1] - c[1]) < 3 && abs(pc[2] - c[2]) < 3) {
					return;
				}

				m_R = m_Color[1][0][0] = pc[0] = c[0];
				m_G = m_Color[1][1][0] = pc[1] = c[1];
				m_B = m_Color[1][2][0] = pc[2] = c[2];

				m_bDrawSelectColor[1] = TRUE;
				int iByte_Pixel = GetDeviceCaps(m_MemDC_Colormap, BITSPIXEL) / 8;
				int r = 0, g = 0, b = 0;
				if (iByte_Pixel == 2) {
					r = Color & 0x1F;
					g = (Color >> 6) & 0x1F;
					b = (Color >> 11) & 0x1F;
				}
				else if (iByte_Pixel == 4) {
					r = Color & 0xFF;
					g = (Color >> 8) & 0xFF;
					b = (Color >> 16) & 0xFF;
				}
				m_strColor[0][0].Format("%d", m_Color[1][0][0]);
				m_strColor[1][0].Format("%d", m_Color[1][1][0]);
				m_strColor[2][0].Format("%d", m_Color[1][2][0]);
				UDF;
				UpdateControl();
			}
			else if (m_iMode == 1) {
				COLORREF Color = GetPixel(m_MemDC_Colormap, point.x, point.y);
				static int pc[3] = { -100, -100, -100 };
				int c[3];
				c[0] = GetRValue(Color);
				c[1] = GetGValue(Color);
				c[2] = GetBValue(Color);

				if (c[0] == 255 && c[1] == 255 && c[2] == 255) {
					if (point.x <= m_iLimitColorMap[0][1] - COLOR_PIXEL_SIZE) return;
					if (point.x <= m_iLimitColorMap[0][1] && point.y < m_iLimitColorMap[1][1] - COLOR_PIXEL_SIZE) return;
				}
				if (abs(pc[0] - c[0]) < 3 && abs(pc[1] - c[1]) < 3 && abs(pc[2] - c[2]) < 3) {
					return;
				}

				m_R = m_Color[1][0][m_iSelectItem] = pc[0] = c[0];
				m_G = m_Color[1][1][m_iSelectItem] = pc[1] = c[1];
				m_B = m_Color[1][2][m_iSelectItem] = pc[2] = c[2];

				m_bDrawSelectColor[1] = TRUE;

				int iByte_Pixel = GetDeviceCaps(m_MemDC_Colormap, BITSPIXEL) / 8;
				int r = 0, g = 0, b = 0;
				if (iByte_Pixel == 2) {
					r = Color & 0x1F;
					g = (Color >> 6) & 0x1F;
					b = (Color >> 11) & 0x1F;
				}
				else if (iByte_Pixel == 4) {
					r = Color & 0xFF;
					g = (Color >> 8) & 0xFF;
					b = (Color >> 16) & 0xFF;
				}
				m_strColor[0][m_iSelectItem].Format("%d", m_Color[1][0][m_iSelectItem]);
				m_strColor[1][m_iSelectItem].Format("%d", m_Color[1][1][m_iSelectItem]);
				m_strColor[2][m_iSelectItem].Format("%d", m_Color[1][2][m_iSelectItem]);
				UDF;
				UpdateControl();
			}
			else if (m_iMode == 2) {
				COLORREF Color = GetPixel(m_MemDC_Colormap, point.x, point.y);
				static int pc[3] = { -100, -100, -100 };
				int c[3];
				c[0] = GetRValue(Color);
				c[1] = GetGValue(Color);
				c[2] = GetBValue(Color);

				if (c[0] == 255 && c[1] == 255 && c[2] == 255) {
					if (point.x <= m_iLimitColorMap[0][1] - COLOR_PIXEL_SIZE) return;
					if (point.x <= m_iLimitColorMap[0][1] && point.y < m_iLimitColorMap[1][1] - COLOR_PIXEL_SIZE) return;
				}
				if (abs(pc[0] - c[0]) < 3 && abs(pc[1] - c[1]) < 3 && abs(pc[2] - c[2]) < 3) {
					return;
				}

				m_R = m_Color[1][0][0] = pc[0] = c[0];
				m_G = m_Color[1][1][0] = pc[1] = c[1];
				m_B = m_Color[1][2][0] = pc[2] = c[2];

				m_bDrawSelectColor[1] = TRUE;

				int iByte_Pixel = GetDeviceCaps(m_MemDC_Colormap, BITSPIXEL) / 8;
				int r = 0, g = 0, b = 0;
				if (iByte_Pixel == 2) {
					r = Color & 0x1F;
					g = (Color >> 6) & 0x1F;
					b = (Color >> 11) & 0x1F;
				}
				else if (iByte_Pixel == 4) {
					r = Color & 0xFF;
					g = (Color >> 8) & 0xFF;
					b = (Color >> 16) & 0xFF;
				}
				m_strColor[0][0].Format("%d", m_Color[1][0][0]);
				m_strColor[1][0].Format("%d", m_Color[1][1][0]);
				m_strColor[2][0].Format("%d", m_Color[1][2][0]);
				UDF;
				UpdateControl();
			}

			if (bUpdateColorMap) {
				DrawSubColorMap();
				CopySubColorMap();
			}
			else {
				DrawSubColorMapChecked(point.y);
			}
		}
	}

}

void CColorSelectDlg::DrawSelectColor() {
	CClientDC dc(this);
	
	if (m_iMode == 0) {
		int iMode = 0;
		iMode += m_bDrawSelectColor[0];
		iMode += m_bDrawSelectColor[1];
		if (iMode == 0) return;
		CBrush brush;
		int x0 = 410, width = 150, gap = 80;
		int y0 = 100, height = 30;
		for (int i = 0; i < N_COLOR_DRAW; i++) {
			if (m_bDrawSelectColor[i]) {
				brush.CreateSolidBrush(RGB(m_Color[i][0][0], m_Color[i][1][0], m_Color[i][2][0]));
				dc.SelectObject(&brush);
				dc.Rectangle(x0 + i * (width + gap), y0, x0 + i * (width + gap) + width, y0 + height);
				m_bDrawSelectColor[i] = FALSE;
			}
		}
	}
	else if (m_iMode == 1) {
		CPen pen[3][N_COLOR_DRAW];
		CBrush brush[3][N_COLOR_DRAW];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < N_COLOR_DRAW; j++) {
				pen[i][j].CreatePen(PS_SOLID, 1, RGB(m_Color[j][0][i], m_Color[j][1][i], m_Color[j][2][i]));
				brush[i][j].CreateSolidBrush(RGB(m_Color[j][0][i], m_Color[j][1][i], m_Color[j][2][i]));
				dc.SelectObject(&pen[i][j]);
				dc.SelectObject(&brush[i][j]);
				if (i == 0) {
					dc.Rectangle(490 + 170 * j, 67, 560 + 170 * j, 70);

				}
				else {
					dc.Rectangle(490 + 170 * j, 55 + 65 * i, 560 + 170 * j, 75 + 65 * i);
				}
			}
		}

		for (int j = 0; j < N_COLOR_DRAW; j++) {
			dc.SelectObject(&pen[0][j]); dc.SelectObject(&brush[1][j]);
			dc.Rectangle(530 + 170 * j, 225, 570 + 170 * j, 265);
			dc.Rectangle(480 + 170 * j, 225, 520 + 170 * j, 265);

			dc.SelectObject(&pen[2][j]); dc.SelectObject(&brush[2][j]);
			dc.Rectangle(535 + 170 * j, 230, 565 + 170 * j, 260);
		}
	}
	else if (m_iMode == 2) {
		CBrush brush;
		int x0 = 410, width = 150, gap = 80;
		int y0 = 120, height = 130;
		for (int i = 0; i < N_COLOR_DRAW; i++) {
			if (m_bDrawSelectColor[i]) {
				brush.CreateSolidBrush(RGB(m_Color[i][0][0], m_Color[i][1][0], m_Color[i][2][0]));
				dc.SelectObject(&brush);
				dc.Rectangle(x0 + i * (width + gap), y0, x0 + i * (width + gap) + width, y0 + height);
			}
		}
	}

	CopySubColorMap();
}

void CColorSelectDlg::OnBnClickedSelectcolor(){
	UDT;
	pAppMain->m_PropertyChange.bIsChanged = TRUE;
	for (int i = 0; i < MAX_COLOR_ITEM; i++) {
		pAppMain->m_PropertyChange.Color[0][i] = m_Color[1][0][i];
		pAppMain->m_PropertyChange.Color[1][i] = m_Color[1][1][i];
		pAppMain->m_PropertyChange.Color[2][i] = m_Color[1][2][i];
	}
	pAppMain->m_PropertyChange.iLineType = m_iPenType;
	pAppMain->m_PropertyChange.iThickness = m_iPenThickness + 1;
	pAppMain->m_PropertyChange.bUseMark = m_bUseMark;
	OnOK();
}


void CColorSelectDlg::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
	case 10: DrawSelectColor(); break;
	}

	CDialogEx::OnTimer(nIDEvent);	
}

void CColorSelectDlg::OnBnClickedCancel() {	
	CDialogEx::OnCancel();
}

void CColorSelectDlg::UpdateControl() {
	CClientDC dc(this);

	UDT;
	if (m_bReqRedraw) {
		Invalidate();
		m_bReqRedraw = FALSE;
	}

	if (m_iMode == 0) {
		int iThick = max(1, min(10, m_iPenThickness + 1));
		m_iPenThickness = iThick - 1;

		LOGBRUSH lbr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = RGB(m_Color[1][0][0], m_Color[1][1][0], m_Color[1][2][0]);
		lbr.lbHatch = 0;
		CPen Pen(PS_GEOMETRIC | m_iPenType, iThick, &lbr, 0, 0);
		CPen PenMark;
		PenMark.CreatePen(PS_SOLID, iThick, RGB(m_Color[1][0][0], m_Color[1][1][0], m_Color[1][2][0]));
		int x0 = 670, l = 130, y0 = 215;
		if (m_bUseMark) {
			dc.SelectObject(&PenMark);
			int iSize = iThick / 2 + 1;
			if (iSize * 2 < iThick + 2) iSize++;
			dc.Rectangle(x0 + l / 2 - iSize, y0 - iSize + 1, x0 + l / 2 + iSize, y0 + iSize);
		}
		dc.SelectObject(Pen);
		dc.MoveTo(670, 215);
		dc.LineTo(800, 215);

		m_bDrawSelectColor[1] = TRUE;
		m_bDrawSelectColor[0] = TRUE;
		DrawSelectColor();
	}
}

void CColorSelectDlg::OnCbnSelchangePentype() {
	m_bReqRedraw = TRUE;
	UpdateControl();
}

void CColorSelectDlg::OnBnClickedDrawmark() {
	m_bReqRedraw = TRUE; 
	UpdateControl();
}


void CColorSelectDlg::ChangeColorProperty() {
	UDT;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < MAX_COLOR_ITEM; j++) {
			m_Color[1][i][j] = min(255, max(0, atoi(m_strColor[i][j])));
			m_strColor[i][j].Format("%d", m_Color[1][i][j]);
		}
	}
	UDF;
	m_bReqRedraw = TRUE;
	UpdateControl();

	int r = m_Color[1][0][m_iSelectItem];
	int g = m_Color[1][1][m_iSelectItem];
	int b = m_Color[1][2][m_iSelectItem];

	if (m_R != r || m_G != g || m_B != b) {
		m_R = r;
		m_G = g;
		m_B = b;
		DrawSubColorMap();
		CopySubColorMap();
	}
}


void CColorSelectDlg::OnBnClickedGetdefaultcolor() {
	SetDlgItemInt(IDC_CurrentProperty0, pAppMain->m_PropertyChange.DefaultColor[0][0]);
	SetDlgItemInt(IDC_CurrentProperty1, pAppMain->m_PropertyChange.DefaultColor[1][0]);
	SetDlgItemInt(IDC_CurrentProperty2, pAppMain->m_PropertyChange.DefaultColor[2][0]);
}


void CColorSelectDlg::ShowControl() {
	if (m_iMode == 0) {
		// default Setting
		GetDlgItem(IDC_SelectItem0)->ShowWindow(FALSE);
		GetDlgItem(IDC_SelectItem1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SelectItem2)->ShowWindow(FALSE);
	}
	else if (m_iMode == 1) {
		CString str;
		// Show
		GetDlgItem(IDC_STATIC0_Mode0)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC1_Mode0)->ShowWindow(FALSE);
		GetDlgItem(IDC_ThicknessType)->ShowWindow(FALSE);
		GetDlgItem(IDC_PenType)->ShowWindow(FALSE);
		GetDlgItem(IDC_DrawMark)->ShowWindow(FALSE);	
		GetDlgItem(IDC_SelectItem0)->ShowWindow(TRUE);
		GetDlgItem(IDC_SelectItem1)->ShowWindow(TRUE);
		GetDlgItem(IDC_SelectItem2)->ShowWindow(TRUE);
		GetDlgItem(IDC_Static_Des0)->ShowWindow(FALSE);
		GetDlgItem(IDC_Static_Des1)->ShowWindow(FALSE);				
		
		GetDlgItem(IDC_PreProperty1)->ShowWindow(TRUE);
		GetDlgItem(IDC_CurrentProperty10)->ShowWindow(TRUE);
		GetDlgItem(IDC_CurrentProperty11)->ShowWindow(TRUE);
		GetDlgItem(IDC_CurrentProperty12)->ShowWindow(TRUE);

		GetDlgItem(IDC_PreProperty2)->ShowWindow(TRUE);
		GetDlgItem(IDC_CurrentProperty20)->ShowWindow(TRUE);
		GetDlgItem(IDC_CurrentProperty21)->ShowWindow(TRUE);
		GetDlgItem(IDC_CurrentProperty22)->ShowWindow(TRUE);

		GetDlgItem(IDC_GetDefaultColor1)->ShowWindow(TRUE);
		GetDlgItem(IDC_GetDefaultColor2)->ShowWindow(TRUE);
		
		// Caption
		SetWindowText("< Select Control Style >");
		str.Format("[ As - Is ]                                                 [To - Be]");
		GetDlgItem(IDC_STATIC_0)->SetWindowTextA(str);


		// Location
		GetDlgItem(IDC_STATIC_0)->MoveWindow(480, 12, 280, 16, TRUE);
		GetDlgItem(IDC_PreProperty)->MoveWindow(480, 33, 140, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty0)->MoveWindow(646, 33, 30, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty1)->MoveWindow(680, 33, 30, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty2)->MoveWindow(714, 33, 30, 16, TRUE);		

		GetDlgItem(IDC_PreProperty1)->MoveWindow(480, 97, 140, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty10)->MoveWindow(646, 97, 30, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty11)->MoveWindow(680, 97, 30, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty12)->MoveWindow(714, 97, 30, 16, TRUE);

		GetDlgItem(IDC_PreProperty2)->MoveWindow(480, 161, 140, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty20)->MoveWindow(646, 161, 30, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty21)->MoveWindow(680, 161, 30, 16, TRUE);
		GetDlgItem(IDC_CurrentProperty22)->MoveWindow(714, 161, 30, 16, TRUE);

		GetDlgItem(IDC_GetDefaultColor1)->MoveWindow(769, 97, 45, 19, TRUE);
		GetDlgItem(IDC_GetDefaultColor2)->MoveWindow(769, 161, 45, 19, TRUE);


		GetDlgItem(IDC_SelectItem0)->MoveWindow(400, 33, 70, 16, TRUE);
		GetDlgItem(IDC_SelectItem1)->MoveWindow(400, 97, 70, 16, TRUE);
		GetDlgItem(IDC_SelectItem2)->MoveWindow(400, 161, 70, 16, TRUE);

		GetDlgItem(IDC_STATIC_1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_4)->ShowWindow(FALSE);

		GetDlgItem(IDC_R_UP)->ShowWindow(FALSE);
		GetDlgItem(IDC_R_DN)->ShowWindow(FALSE);
		GetDlgItem(IDC_R_UP2)->ShowWindow(FALSE);
		GetDlgItem(IDC_R_DN2)->ShowWindow(FALSE);
		GetDlgItem(IDC_R_UP3)->ShowWindow(FALSE);
		GetDlgItem(IDC_R_DN3)->ShowWindow(FALSE);
		GetDlgItem(IDC_IncVal)->ShowWindow(FALSE);
	}
	else if (m_iMode == 2) {
		CString str;
		// Show
		GetDlgItem(IDC_SelectItem0)->ShowWindow(FALSE);
		GetDlgItem(IDC_SelectItem1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SelectItem2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC0_Mode0)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC1_Mode0)->ShowWindow(FALSE);
		GetDlgItem(IDC_ThicknessType)->ShowWindow(FALSE);
		GetDlgItem(IDC_PenType)->ShowWindow(FALSE);
		GetDlgItem(IDC_DrawMark)->ShowWindow(FALSE);
		GetDlgItem(IDC_Static_Des0)->ShowWindow(FALSE);
		GetDlgItem(IDC_Static_Des1)->ShowWindow(FALSE);
		GetDlgItem(IDC_CurrentProperty10)->ShowWindow(FALSE);
		GetDlgItem(IDC_CurrentProperty11)->ShowWindow(FALSE);
		GetDlgItem(IDC_CurrentProperty12)->ShowWindow(FALSE);
		GetDlgItem(IDC_PreProperty1)->ShowWindow(FALSE);
		GetDlgItem(IDC_PreProperty2)->ShowWindow(FALSE);
		GetDlgItem(IDC_CurrentProperty20)->ShowWindow(FALSE);
		GetDlgItem(IDC_CurrentProperty21)->ShowWindow(FALSE);
		GetDlgItem(IDC_CurrentProperty22)->ShowWindow(FALSE);
		GetDlgItem(IDC_GetDefaultColor1)->ShowWindow(FALSE);
		GetDlgItem(IDC_GetDefaultColor2)->ShowWindow(FALSE);

		// Caption
		SetWindowText("< Select Control Style >");
		str.Format("[ As - Is ]                                                 [To - Be]");
		GetDlgItem(IDC_STATIC_0)->SetWindowTextA(str);
	}
}

void CColorSelectDlg::OnBnClickedGetdefaultcolor1() {
	SetDlgItemInt(IDC_CurrentProperty10, pAppMain->m_PropertyChange.DefaultColor[0][1]);
	SetDlgItemInt(IDC_CurrentProperty11, pAppMain->m_PropertyChange.DefaultColor[1][1]);
	SetDlgItemInt(IDC_CurrentProperty12, pAppMain->m_PropertyChange.DefaultColor[2][1]);
}


void CColorSelectDlg::OnBnClickedGetdefaultcolor2() {
	SetDlgItemInt(IDC_CurrentProperty20, pAppMain->m_PropertyChange.DefaultColor[0][2]);
	SetDlgItemInt(IDC_CurrentProperty21, pAppMain->m_PropertyChange.DefaultColor[1][2]);
	SetDlgItemInt(IDC_CurrentProperty22, pAppMain->m_PropertyChange.DefaultColor[2][2]);
}

void CColorSelectDlg::OnBnClickedSelectitem0() {
	UDT;
	ChangeColorProperty();
}

void CColorSelectDlg::CopySubColorMap() {
	try {
		CClientDC dc(this);
		// 디바이스 컨텍스트를 사용하는 코드
		if (m_applyBK) {
			dc.BitBlt(m_iLimitSubColorMap[0][0], m_iLimitSubColorMap[1][0], m_iLimitSubColorMap[0][1] - m_iLimitSubColorMap[0][0] + 5, m_iLimitSubColorMap[1][1] - m_iLimitSubColorMap[1][0], &m_MemDC_Colormap, m_iLimitSubColorMap[0][0], m_iLimitSubColorMap[1][0], SRCCOPY);
		}
		else {
			dc.BitBlt(m_iLimitSubColorMap[0][0], m_iLimitSubColorMap[1][0], m_iLimitSubColorMap[0][1] - m_iLimitSubColorMap[0][0], m_iLimitSubColorMap[1][1] - m_iLimitSubColorMap[1][0], &m_MemDC_Colormap, m_iLimitSubColorMap[0][0], m_iLimitSubColorMap[1][0], SRCCOPY);
			GetBkColor();
		}
	}
	catch (CResourceException* e) {		
		e->Delete();
	}
}


void CColorSelectDlg::DrawSubColorMap() {
	int R = m_R, G = m_G, B = m_B;
	int r, g, b;

	if (R == 0) R = 1;
	if (G == 0) G = 1;
	if (B == 0) B = 1;

	int iMax = max(max(R, G), B);
	int iMin = min(min(R, G), B);
	int spanY = m_iLimitSubColorMap[1][1] - m_iLimitSubColorMap[1][0];
	int deltaY = (int)((double(iMax) - iMin) * 255. / iMax + 5);
	double Gain = 256. / (double)(spanY - deltaY) / (double)iMax;
	double addValue = (255. - Gain * spanY * iMin) / deltaY;
	BOOL bSearchData = TRUE;
	int PtY = -1;
	double addMinColor = 0;
	for (int y = 0, drawY = m_iLimitColorMap[1][1]; y <= spanY; y++, drawY--) {
		int Vmax = (int)(iMax * Gain* y);
		if (bSearchData && Vmax >= iMax) {
			PtY = drawY;
			bSearchData = FALSE;
		}
		if (Vmax > 255) {
			r = max(0, min((int)(Gain * R * y + addMinColor), 255));
			g = max(0, min((int)(Gain * G * y + addMinColor), 255));
			b = max(0, min((int)(Gain * B * y + addMinColor), 255));
				addMinColor += addValue;
		}
		else {			
			r = max(0, min((int)(Gain * R * y), 255));
			g = max(0, min((int)(Gain * G * y), 255));
			b = max(0, min((int)(Gain * B * y), 255));
		}

		for (int x = m_iLimitSubColorMap[0][0]; x < m_iLimitSubColorMap[0][1]; x++) {
			SetPixel(m_MemDC_Colormap, x, drawY, RGB(r, g, b));
		}
	}

	PtY = max(PtY, m_iLimitSubColorMap[1][0] + 1);
	DrawSubColorMapChecked(PtY);
}

void CColorSelectDlg::DrawSubColorMapChecked(int y) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 5, m_BK);
	m_MemDC_Colormap.SelectObject(&pen);
	m_MemDC_Colormap.MoveTo(m_iLimitSubColorMap[0][1] + 2, m_iLimitSubColorMap[1][0]);
	m_MemDC_Colormap.LineTo(m_iLimitSubColorMap[0][1] + 2, m_iLimitSubColorMap[1][1]);
	
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_MemDC_Colormap.SelectObject(&pen);
	m_MemDC_Colormap.MoveTo(m_iLimitSubColorMap[0][1] + 0, y);
	m_MemDC_Colormap.LineTo(m_iLimitSubColorMap[0][1] + 5, y);
	m_MemDC_Colormap.MoveTo(m_iLimitSubColorMap[0][1] + 2, y+1);
	m_MemDC_Colormap.LineTo(m_iLimitSubColorMap[0][1] + 5, y+1);
	m_MemDC_Colormap.MoveTo(m_iLimitSubColorMap[0][1] + 2, y - 1);
	m_MemDC_Colormap.LineTo(m_iLimitSubColorMap[0][1] + 5, y - 1);
}

void CColorSelectDlg::GetBkColor() {
	CClientDC dc(this);
	COLORREF Color = GetPixel(dc, 337, 10);
	int iByte_Pixel = GetDeviceCaps(dc, BITSPIXEL) / 8;
	int r = 0, g = 0, b = 0;
	if (iByte_Pixel == 2) {
		r = Color & 0x1F;
		g = (Color >> 6) & 0x1F;
		b = (Color >> 11) & 0x1F;
	}
	else if (iByte_Pixel == 4) {
		r = Color & 0xFF;
		g = (Color >> 8) & 0xFF;
		b = (Color >> 16) & 0xFF;
	}

	m_BK = RGB(r, g, b);
	if (r + g + b > 0 && r + g + b <= 255 * 3) {
		m_applyBK = TRUE;
		DrawSubColorMap();
	}
}


void CColorSelectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{

	CDialogEx::OnLButtonUp(nFlags, point);
	m_bSubColorMode = FALSE;
}


void CColorSelectDlg::OnBnClickedRUp() {
	int val = GetDlgItemInt(IDC_CurrentProperty0);
	CString str;
	m_ctrlInc.GetLBText(m_ctrlInc.GetCurSel(), str);
	int inc = atoi(str);
	SetDlgItemInt(IDC_CurrentProperty0, min(255, val+inc));
}


void CColorSelectDlg::OnBnClickedRUp2() {
	int val = GetDlgItemInt(IDC_CurrentProperty1);
	CString str;
	m_ctrlInc.GetLBText(m_ctrlInc.GetCurSel(), str);
	int inc = atoi(str);
	SetDlgItemInt(IDC_CurrentProperty1, min(255, val + inc));
}


void CColorSelectDlg::OnBnClickedRUp3() {
	int val = GetDlgItemInt(IDC_CurrentProperty2);
	CString str;
	m_ctrlInc.GetLBText(m_ctrlInc.GetCurSel(), str);
	int inc = atoi(str);
	SetDlgItemInt(IDC_CurrentProperty2, min(255, val + inc));
}


void CColorSelectDlg::OnBnClickedRDn() {
	int val = GetDlgItemInt(IDC_CurrentProperty0);
	CString str;
	m_ctrlInc.GetLBText(m_ctrlInc.GetCurSel(), str);
	int inc = atoi(str);
	SetDlgItemInt(IDC_CurrentProperty0, max(0, val - inc));
}


void CColorSelectDlg::OnBnClickedRDn2() {
	int val = GetDlgItemInt(IDC_CurrentProperty1);
	CString str;
	m_ctrlInc.GetLBText(m_ctrlInc.GetCurSel(), str);
	int inc = atoi(str);
	SetDlgItemInt(IDC_CurrentProperty1, max(0, val - inc));
}


void CColorSelectDlg::OnBnClickedRDn3() {
	int val = GetDlgItemInt(IDC_CurrentProperty2);
	CString str;
	m_ctrlInc.GetLBText(m_ctrlInc.GetCurSel(), str);
	int inc = atoi(str);
	SetDlgItemInt(IDC_CurrentProperty2, max(0, val - inc));
}
