// FftDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialComDlg.h"
#include "FftDlg.h"
#include "afxdialogex.h"

// CFftDlg 대화 상자

IMPLEMENT_DYNAMIC(CFftDlg, CDialogEx)

CFftDlg::CFftDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DftDlg, pParent)
	, m_strItemName(_T(""))
	, m_strDataPeriod(_T("20.00"))
	, m_fDataPeriod(20.)
	, m_iScaleModeX(0)
	, m_strMessage(_T(""))
	, m_bIsCalculated(FALSE)
	, m_bShowMark(TRUE)
	, m_bShowLine(TRUE)
	, m_iSelectIndex(-1)
	, m_bReqDraw(TRUE)
{
	m_fValueLimit[0] = 0., m_fValueLimit[1] = 10.;
	m_iDrawLimit[0][0] = 100, m_iDrawLimit[1][0] = 95, m_iDrawLimit[0][1] = 1230, m_iDrawLimit[1][1] = 440;
}

CFftDlg::~CFftDlg()
{
}

void CFftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ItemName, m_strItemName);
	DDX_Text(pDX, IDC_DataPeriod, m_strDataPeriod);
	DDX_Radio(pDX, IDC_ScaleModeX, m_iScaleModeX);
	DDX_Text(pDX, IDC_Message, m_strMessage);
	DDX_Text(pDX, IDC_MinY_FFT, m_strValuetLimit[0]);
	DDX_Text(pDX, IDC_MaxY_FFT, m_strValuetLimit[1]);
	DDX_Text(pDX, IDC_MidY_FFT, m_strValuetLimit[2]);
	DDX_Control(pDX, IDC_MaxY_FFT, m_ctrlMaxY);
	DDX_Control(pDX, IDC_MinY_FFT, m_ctrlMinY);
	DDX_Control(pDX, IDC_MidY_FFT, m_ctrlMidY);
	DDX_Check(pDX, IDC_ShowMark, m_bShowMark);
	DDX_Check(pDX, IDC_ShowLine, m_bShowLine);
}

BEGIN_MESSAGE_MAP(CFftDlg, CDialogEx)
	ON_EN_CHANGE(IDC_DataPeriod, &CFftDlg::ChangeControl)
	ON_BN_CLICKED(IDC_ReqDraw, &CFftDlg::OnBnClickedReqdraw)
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ScaleModeX, &CFftDlg::ChangeControl)
	ON_BN_CLICKED(IDC_ScaleModeX2, &CFftDlg::ChangeControl)
	ON_EN_CHANGE(IDC_MinY_FFT, &CFftDlg::ChangeControl)
	ON_EN_CHANGE(IDC_MaxY_FFT, &CFftDlg::ChangeControl)
	ON_BN_CLICKED(IDC_ShowMark, &CFftDlg::ChangeControl)
	ON_BN_CLICKED(IDC_ShowLine, &CFftDlg::ChangeControl)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFftDlg 메시지 처리기
BOOL CFftDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	m_iN_FFT = m_iEndIndex - m_iStartIndex + 1;
    CString strTemp = m_strItemName;
	m_strItemName.Format("%s       Data Index : %d ~ %d (N:%d)", (LPCTSTR)strTemp,  m_iStartIndex, m_iEndIndex, m_iN_FFT);
	UDF;
	OnBnClickedReqdraw();
	ShowWindow(SW_SHOWMAXIMIZED);
	SetTimer(100, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CFftDlg::ChangeControl() {
	UDT;
	m_fDataPeriod = min(max(atof(m_strDataPeriod), 0.01), 10000.);	
	
	if (m_fDataPeriod == (int)m_fDataPeriod) {
		m_strDataPeriod.Format("%.2f", m_fDataPeriod);
	}

	m_fValueLimit[1] = atof(m_strValuetLimit[1]);
	m_fValueLimit[0] = atof(m_strValuetLimit[0]);
	m_strValuetLimit[2].Format("%.1f", (m_fValueLimit[0] + m_fValueLimit[1]) / 2.);
	UDF;
	Invalidate();
	DrawFFT();
}


void CFftDlg::OnBnClickedReqdraw() {
	UDT;
	CalculateFFT();	
	DrawFFT();
}

void CFftDlg::CalculateFFT() {
	m_bIsCalculated = FALSE;
    int iHistoryIndex = pMain->m_iHistoryIndex;
	for (int i = 0; i < (1+m_iN_FFT)/2; i++) {
		m_FFT[i] = 0.;
		int index = m_iStartIndex;
		if (m_bIsMath) {
			for (int j = 0; j < m_iN_FFT; j++) {
				index++;
                if (index >= iHistoryIndex) index = max(0, index-1);
				m_FFT[i] += cos(M_PI2 * i * j / m_iN_FFT) * (pMain->m_HistoryData[index].fMathValue[m_iDataChannel]);
			}
		}
		else {
			for (int j = 0; j < m_iN_FFT; j++) {
				index++;
                if (index >= iHistoryIndex) index = max(0, index - 1);
				m_FFT[i] += cos(M_PI2 * i * j / m_iN_FFT) * (pMain->m_HistoryData[index].fDataValue[m_iDataChannel]);
			}
		}
		m_FFT[i] /= m_iN_FFT;
		if (m_FFT[i] < 0) m_FFT[i] = -m_FFT[i];
	}

	m_fValueLimit[0] = 	m_fValueLimit[1] = 0.;
	for (int i = 1; i < m_iN_FFT; i++) {
		if (m_fValueLimit[1] < m_FFT[i]) m_fValueLimit[1] = m_FFT[i];
	}

	m_fValueLimit[1] *= 1.1;
	m_strMessage.Format("Data Value : %.2f ~ %.2f", m_fValueLimit[0], m_fValueLimit[1]);
	m_strValuetLimit[0].Format("%.1f", m_fValueLimit[0]);
	m_strValuetLimit[1].Format("%.1f", m_fValueLimit[1]);
	m_strValuetLimit[2].Format("%.1f", (m_fValueLimit[0] + m_fValueLimit[1]) / 2.);
	UDF;
	m_bIsCalculated = TRUE;
}

void CFftDlg::OnRButtonDown(UINT nFlags, CPoint point) {
	m_strMessage.Format("X:%d Y:%d", point.x, point.y);
	UDF;

	CDialogEx::OnRButtonDown(nFlags, point);
}

CPoint CFftDlg::Cal_DrawaPosition(double x, double y) {
	CPoint point;
	point.x = (int)((x - m_fFreqLimit[0]) / (m_fFreqLimit[1] - m_fFreqLimit[0]) * (m_iDrawLimit[0][1] - m_iDrawLimit[0][0]) + m_iDrawLimit[0][0]);
	point.y = (int)((m_fValueLimit[0] - y) / (m_fValueLimit[1] - m_fValueLimit[0]) * (m_iDrawLimit[1][1] - m_iDrawLimit[1][0]) + m_iDrawLimit[1][1]);
	return point;
}

void CFftDlg::DrawFFT() {

	CClientDC dc(this);
	CBrush brush;
	double Freq_Sampling = 1000. / m_fDataPeriod / (m_iN_FFT - 1);
	int iMaxDrawCount = (m_iN_FFT + 1) / 2;
	if (ReqInvalidate) {
		ReqInvalidate = FALSE;
		Invalidate();
	}
	dc.Rectangle(m_iDrawLimit[0][0] - 3, m_iDrawLimit[1][0] - 3, m_iDrawLimit[0][1] + 3, m_iDrawLimit[1][1] + 3);
	dc.Rectangle(m_iDrawLimit[0][0], m_iDrawLimit[1][0], m_iDrawLimit[0][1], m_iDrawLimit[1][1]);

	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(20, 20, 20));
	dc.SelectObject(&pen);
	//Grid Horizontal
	int N_GRID_Y = 3;
	for (int i = 0; i < N_GRID_Y; i++) {
		int iPy = m_iDrawLimit[1][0] + (m_iDrawLimit[1][1] - m_iDrawLimit[1][0]) * (i + 1) / (N_GRID_Y + 1);
		dc.MoveTo(m_iDrawLimit[0][0], iPy);
		dc.LineTo(m_iDrawLimit[0][1], iPy);
	}

	// Grid Vertical
	if (m_iScaleModeX == 0) {
		// Linear 
		m_fFreqLimit[1] = Freq_Sampling * iMaxDrawCount;
		m_fFreqLimit[0] = 0;

		double fDev = m_fFreqLimit[1] - m_fFreqLimit[0];
		int iOrder = (int)log10(fDev);
		int iCoefficient = (int)(fDev / pow(10., iOrder));
		switch (iCoefficient) {
			case 1: iOrder--; iCoefficient = 1;  break;
			case 2: case 3: iOrder--; iCoefficient = 2;  break;
			case 4: case 5: case 6:  case 7: iOrder--; iCoefficient = 5;  break;
			default:iCoefficient = 1;  break;
		}

		double fDeviation = pow(10., iOrder) * iCoefficient;
		double fStart = ((int)(m_fFreqLimit[0] / fDeviation)) * fDeviation;
		for (double fFreq = fStart; fFreq < m_fFreqLimit[1]; fFreq += fDeviation) {
			CPoint pt = Cal_DrawaPosition(fFreq, m_fValueLimit[0]);
			if (pt.x > m_iDrawLimit[0][0] && pt.x < m_iDrawLimit[0][1]) {
				dc.MoveTo(pt.x, m_iDrawLimit[1][0]);
				dc.LineTo(pt.x, m_iDrawLimit[1][1]);

				if (m_bIsCalculated) {
					CString str;
					if (iOrder >= 0) str.Format("%.0fHz", fFreq);
					else if (iOrder == -1)
						str.Format("%.1fHz", fFreq);
					else if (iOrder == -2)
						str.Format("%.2fHz", fFreq);
					else
						str.Format("%.3fHz", fFreq);
					dc.TextOutA(pt.x - str.GetLength() * 4, m_iDrawLimit[1][1] + 5, str);
				}
			}
		}
	}
	else {
		// Logscale
		m_fFreqLimit[1] = log10f((float)Freq_Sampling * iMaxDrawCount);
		m_fFreqLimit[0] = log10f((float)Freq_Sampling / 1.3f);
		int iStart = (int)m_fFreqLimit[0];
		if (iStart <= 0) iStart--;
		int iEnd = (int)m_fFreqLimit[1];
		if (iEnd <= 0) iEnd--;
		for (int i = iStart - 1; i <= iEnd; i++) {
			for (int k = 1; k < 10; k++) {
				double fFreqLinear = pow(10., (double)i) * k;
				double fFreq = log10(fFreqLinear);
				CPoint pt = Cal_DrawaPosition(fFreq, m_fValueLimit[0]);
				if (pt.x > m_iDrawLimit[0][0] && pt.x < m_iDrawLimit[0][1]) {
					dc.MoveTo(pt.x, m_iDrawLimit[1][0]);
					dc.LineTo(pt.x, m_iDrawLimit[1][1]);

					if (m_bIsCalculated && (k == 1 || k==5)) {
						CString str;
						if (i >= 0) str.Format("%.0fHz", fFreqLinear);
						else if (i == -1)
							str.Format("%.1fHz", fFreqLinear);
						else if (i == -2)
							str.Format("%.2fHz", fFreqLinear);
						else 
							str.Format("%.3fHz", fFreqLinear);
						
						dc.TextOutA(pt.x - str.GetLength() * 4, m_iDrawLimit[1][1] + 5, str);
					}
				}
			}
		}
	}

	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen);
	// zero position
	CPoint pt = Cal_DrawaPosition(10, 0);
	if (pt.y > m_iDrawLimit[1][0] && pt.y < m_iDrawLimit[1][1]) {
		dc.MoveTo(m_iDrawLimit[0][0], pt.y);
		dc.LineTo(m_iDrawLimit[0][1], pt.y);
	}

	if (!m_bIsCalculated) return;
	int iSize = 4;

	// Draw Mark & Line
	brush.CreateSolidBrush(RGB(255, 0, 0));
	dc.SelectObject(&brush);

	if (m_iScaleModeX == 0) {
		m_fFreqLimit[0] = 0.;
		m_fFreqLimit[1] = Freq_Sampling * iMaxDrawCount;
		for (int i = 1; i < iMaxDrawCount; i++) {
			double fFreq = Freq_Sampling * i;
			CPoint pt = Cal_DrawaPosition(fFreq, m_FFT[i]);
			if (i == 1) dc.MoveTo(pt);
			if (m_bShowLine) dc.LineTo(pt);
			if (m_bShowMark) dc.Rectangle(pt.x - iSize, pt.y - iSize, pt.x + iSize, pt.y + iSize);
		}
	}
	else {
		m_fFreqLimit[0] = log10(Freq_Sampling / 1.3);
		m_fFreqLimit[1] = log10(Freq_Sampling * iMaxDrawCount);
		for (int i = 1; i < iMaxDrawCount; i++) {
			double fFreq = log10(Freq_Sampling * i);
			CPoint pt = Cal_DrawaPosition(fFreq, m_FFT[i]);
			if (i == 1) dc.MoveTo(pt);
			if (m_bShowLine) dc.LineTo(pt);
			if (m_bShowMark) dc.Rectangle(pt.x - iSize, pt.y - iSize, pt.x + iSize, pt.y + iSize);
		}
	}

	if (m_iSelectIndex != -1) {
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		dc.SelectObject(&pen);
		CPoint PtSelect;

		if (m_iScaleModeX == 0) {
			PtSelect = Cal_DrawaPosition(Freq_Sampling * m_iSelectIndex, m_FFT[m_iSelectIndex]);
		}
		else {
			PtSelect = Cal_DrawaPosition(log10(Freq_Sampling * m_iSelectIndex), m_FFT[m_iSelectIndex]);
		}
		dc.MoveTo(PtSelect.x, m_iDrawLimit[1][0]);
		dc.LineTo(PtSelect.x, m_iDrawLimit[1][1]);
		int iOrder = (int)log10(Freq_Sampling * m_iSelectIndex);
		CString strFreq, strMag;;
		if (iOrder >= 0) {
			m_strMessage.Format("[ Select item ]\r\n  Freq : %.1fHz   Magnitude : %.3f", Freq_Sampling * m_iSelectIndex, m_FFT[m_iSelectIndex]);
			strFreq.Format("Freq : %.1fHz", Freq_Sampling * m_iSelectIndex);
		}
		else if (iOrder >= -1) {
			m_strMessage.Format("[ Select item ]\r\n  Freq : %.2fHz   Magnitude : %.3f", Freq_Sampling * m_iSelectIndex, m_FFT[m_iSelectIndex]);
			strFreq.Format("Freq : %.2fHz", Freq_Sampling * m_iSelectIndex);
		}
		else if (iOrder >= -2) {
			m_strMessage.Format("[ Select item ]\r\n  Freq : %.3fHz   Magnitude : %.3f", Freq_Sampling * m_iSelectIndex, m_FFT[m_iSelectIndex]);
			strFreq.Format("Freq : %.3fHz", Freq_Sampling * m_iSelectIndex);
		}
		else {
			m_strMessage.Format("[ Select item ]\r\n  Freq : %.4fHz   Magnitude : %.3f", Freq_Sampling * m_iSelectIndex, m_FFT[m_iSelectIndex]);
			strFreq.Format("Freq : %.4fHz", Freq_Sampling * m_iSelectIndex);
		}
		strMag.Format("Magnitude : %.3f", m_FFT[m_iSelectIndex]);
		

		int iMsgPosY = 0, iMsgPosX = 0;
		int iWidth = max(strMag.GetLength(), strFreq.GetLength()) * 8;
		int iLocationX = 0, iLocationY = 0;

		if (PtSelect.x > m_iDrawLimit[0][1] - iWidth - 15) {
			iMsgPosX = max(PtSelect.x - iWidth - 15, m_iDrawLimit[0][0] + 4);
			iLocationX = 0; // Left
		}
		else {
			iMsgPosX = PtSelect.x + 15;
			iLocationX = 1; // Right
		}

		if (iLocationX == 1) { // Right
			if (m_FFT[m_iSelectIndex] > m_FFT[min(m_iSelectIndex + 1, iMaxDrawCount)]) {
				iLocationY = 1; // Up
			}
			else {
				iLocationY = 0;  // down
			}
		}
		else { // Left
			if (m_FFT[m_iSelectIndex] > m_FFT[max(m_iSelectIndex - 1, 0)]) {
				iLocationY = 1; // Up
			}
			else {
				iLocationY = 0; // down
			}
		}

		if (iLocationY == 1) {
			iMsgPosY = max(PtSelect.y - 50, m_iDrawLimit[1][0] + 5);
		}
		else {
			iMsgPosY = min(PtSelect.y + 20, m_iDrawLimit[1][1] - 35);
		}

		dc.TextOutA(iMsgPosX, iMsgPosY, strFreq);
		dc.TextOutA(iMsgPosX, iMsgPosY + 16, strMag);
		//switch (iLocationX + iLocationY * 2) {
		//case 0: m_strMessage.Format("Left Down (%d %d)", PtSelect.x, PtSelect.y); break;
		//case 1: m_strMessage.Format("Right Up (%d %d)", PtSelect.x, PtSelect.y); break;
		//case 2: m_strMessage.Format("Left Down (%d %d)", PtSelect.x, PtSelect.y); break;
		//case 3: m_strMessage.Format("Right Up (%d %d)", PtSelect.x, PtSelect.y); break;
		//}		

		UDF;
	}
	CString str;
	str.Format("DC (0 Hz) : %.1f", m_FFT[0]);
	dc.TextOutA(m_iDrawLimit[0][0] + 10 , m_iDrawLimit[1][0] + 5, str);
	m_bReqDraw = FALSE;
}

void CFftDlg::OnPaint() {
	CPaintDC dc(this); 
	DrawFFT();
}

void CFftDlg::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);
	m_iDrawLimit[0][1] = cx - 50, m_iDrawLimit[1][1] = cy - 50;
	//GetDlgItem(IDC_MaxY_FFT)->MoveWindow(30, 100, 60, 14, TRUE);
	
	m_ctrlMinY.MoveWindow(30, m_iDrawLimit[1][1] - 14, 60, 14, TRUE);
	m_ctrlMaxY.MoveWindow(30, m_iDrawLimit[1][0], 60, 14, TRUE);
	m_ctrlMidY.MoveWindow(30, (m_iDrawLimit[1][0] + m_iDrawLimit[1][1]) / 2 - 7, 60, 14, TRUE);
	
	ReqInvalidate = TRUE;
	DrawFFT(); 	
}

void CFftDlg::OnLButtonDown(UINT nFlags, CPoint point) {

	CDialogEx::OnLButtonDown(nFlags, point);

	if (m_bIsCalculated) {
		if (point.x >= m_iDrawLimit[0][0] && point.x <= m_iDrawLimit[0][1]) {
			if (point.y >= m_iDrawLimit[1][0] && point.y <= m_iDrawLimit[1][1]) {
				double Freq_Sampling = 1000. / m_fDataPeriod / (m_iN_FFT - 1);
				if (m_iScaleModeX == 0) {
					double fIndex = (double)point.x;
					double fFreq = ((m_fFreqLimit[1] - m_fFreqLimit[0]) * (point.x - m_iDrawLimit[0][0]) / (m_iDrawLimit[0][1] - m_iDrawLimit[0][0]) + m_fFreqLimit[0]) + Freq_Sampling / 2.;
					m_iSelectIndex = min(max(1, (int)(fFreq / Freq_Sampling)), (m_iN_FFT - 1) / 2);
					m_bReqDraw = TRUE;
				}
				else {
					double fLogFreq = ((m_fFreqLimit[1] - m_fFreqLimit[0]) * (point.x - m_iDrawLimit[0][0]) / (m_iDrawLimit[0][1] - m_iDrawLimit[0][0]) + m_fFreqLimit[0]);
					double fFreq = pow(10., fLogFreq);
					m_iSelectIndex = min(max(1, (int)(fFreq / Freq_Sampling + 0.5)), (m_iN_FFT - 1) / 2 );
					m_bReqDraw = TRUE;
				}
			}
		}
	}
}

void CFftDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent) {
	case 100:if (m_bReqDraw) DrawFFT();	break;
	}
}
