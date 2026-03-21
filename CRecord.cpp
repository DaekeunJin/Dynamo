// CRecord.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "CRecord.h"
#include "afxdialogex.h"
#include "Game.h"

// CRecord 대화 상자

#define N_ITEM_LIST 11

IMPLEMENT_DYNAMIC(CRecord, CDialogEx)

CRecord::CRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Record, pParent)
{

}

CRecord::~CRecord()
{
}

void CRecord::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_Record, m_Best);
}


BEGIN_MESSAGE_MAP(CRecord, CDialogEx)
    ON_BN_CLICKED(IDC_DeleteRecord, &CRecord::OnBnClickedDeleterecord)
END_MESSAGE_MAP()


// CRecord 메시지 처리기





BOOL CRecord::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    AddColumn();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRecord::OnBnClickedDeleterecord()
{
    if (AfxMessageBox("이전 기록이 모두 삭제됩니다.  계속 진행할까요?", MB_YESNO) == IDYES) {

        RECORD temp;
        memset(&temp, 0, sizeof(RECORD));

        for (int i = 0; i < N_RECORD; i++)
            m_pGame->ModifyRecord(i, temp);

        for (int i = 0; i < N_ITEM_LIST; i++)
            m_Best.DeleteColumn(0);
        m_Best.DeleteAllItems();

        AddColumn();
    }
}


void CRecord::AddColumn() {

    LV_COLUMN lvcolumn;
    LV_ITEM lvitem;
    CString str;

    char *listColumn[N_ITEM_LIST] = { " Rank ", "    Score    ",  "  Lines  ", " Combo ", "T_spin", "Perfect", "APM",  "GameTime", "  Date  - Time  ", " Level ", "" };
    int width[N_ITEM_LIST] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,};


    for (int i = 0; i < N_ITEM_LIST; i++)
    {        
        lvcolumn.fmt = LVCFMT_CENTER;
        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.pszText = listColumn[i];
        lvcolumn.iSubItem = i;
        lvcolumn.cx = width[i];
        m_Best.InsertColumn(i, &lvcolumn);
    }

    m_Best.DeleteAllItems();

    for (int i = 0; i < N_RECORD; i++) {
        // score
        int nCount = m_Best.GetItemCount();
        lvitem.mask = LVIF_TEXT;
        lvitem.iSubItem = 0;

        lvitem.iItem = nCount;
        str.Format("  %2d", i + 1);
        lvitem.pszText = (LPSTR)(LPCSTR)str;
        m_Best.InsertItem(&lvitem);

        str.Format("%s", (LPCTSTR)m_pGame->AddComma(m_pGame->m_Record[i].score));
        if (str == "0") str = "-";
        m_Best.SetItemText(nCount, 1, str);

        str.Format("%d", m_pGame->m_Record[i].line);  
        if (str == "0") str = "-";
        m_Best.SetItemText(nCount, 2, str);

        str.Format("%d", m_pGame->m_Record[i].combo);
        if (str == "0") str = "-";
        m_Best.SetItemText(nCount, 3, str);

        str.Format("%d", m_pGame->m_Record[i].t_spin);
        if (str == "0") str = "-";
        m_Best.SetItemText(nCount, 4, str);

        str.Format("%d", m_pGame->m_Record[i].perfect);
        if (str == "0") str = "-";
        m_Best.SetItemText(nCount, 5, str);

        str.Format("%d", m_pGame->m_Record[i].apm);
        if (str == "0") str = "-";
        m_Best.SetItemText(nCount, 6, str);

        str.Format("%2d : %02d", m_pGame->m_Record[i].gameTime / 60, m_pGame->m_Record[i].gameTime % 60);
        if (m_pGame->m_Record[i].gameTime == 0) str = "-";
        m_Best.SetItemText(nCount, 7, str);

        int yy = m_pGame->m_Record[i].date / 10000;
        int mm = ( m_pGame->m_Record[i].date / 100 ) % 100;
        int dd = m_pGame->m_Record[i].date % 100;

        int h = m_pGame->m_Record[i].time / 10000;
        int m = (m_pGame->m_Record[i].time / 100) % 100;
        int s = m_pGame->m_Record[i].time % 100;
        str.Format("%02d.%02d.%02d   %02d:%02d:%02d", yy, mm, dd, h, m, s);    
        if (m_pGame->m_Record[i].date == 0) str = "-";
        m_Best.SetItemText(nCount, 8, str);
        
        if (m_Best.GetItemText(nCount, 1) == "-")
            str = "-";
        else 
            switch (m_pGame->m_Record[i].lvl) {
                case 0: str = "초급"; break;
                case 1: str = "중급"; break;
                case 2: str = "상급"; break;
                case 3: str = "Hell"; break;
                case 4: str = "Train"; break;
                default: str = "-";
            }
        m_Best.SetItemText(nCount, 9, str);
    }


    for (int i = 0; i < m_Best.GetHeaderCtrl()->GetItemCount(); i++) {
        VERIFY(m_Best.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
    }

    m_Best.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

}
