// Game.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "Game.h"
#include "afxdialogex.h"
#include "setupapi.h"
#include <Shlwapi.h>
#include <mmsystem.h>
#include <atlimage.h>
#include "CRecord.h"
#include <Ws2tcpip.h>

#pragma comment(lib, "winmm") 
#pragma comment (lib, "setupapi.lib")
#pragma comment(lib, "iphlpapi.lib")
// CGame 대화 상자

IMPLEMENT_DYNAMIC(CGame, CDialogEx)

CGame::CGame(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_Game, pParent)
    , m_iGameMode(GAMEMODE_END)
    , m_strMessage(_T(""))
    , m_iLines(0)
    , m_iDropTime(800)
    , m_iContineDropTime(0)
    , m_iGameDifficulty(2)
    , m_iGameScore(0)
    , m_iBestRecordLines(0)
    , m_iBestRecordScore(0)
    , m_strGameLevel(_T("상급"))
    , m_iBlockShowType(1)
    , m_iTimeoutBottom(1500)
    , m_iClientCount(0)
    , m_iCommMode(0)
    , m_iIncreaseRow(0)
    , m_iDir(0)
    , m_bPause(FALSE)
    , m_iTotalGameTime(0)
    , m_bReadyAPM(TRUE)
{
	m_ColorBlock[0] = RGB(128, 0, 0);
	m_ColorBlock[1] = RGB(128, 128, 0);
	m_ColorBlock[2] = RGB(0, 128, 0);
	m_ColorBlock[3] = RGB(128, 0, 128);
	m_ColorBlock[4] = RGB(0, 0, 128);
	m_ColorBlock[5] = RGB(0, 128, 128);
	m_ColorBlock[6] = RGB(128, 64, 255);
	m_ColorBlock[7] = RGB(255, 255, 255);
	
	int iBasicSize = PIXEL_SIZE_BLOCK / 2;
	int iNextBlockCenterX[MAX_BLOCK_TYPE] = { 0, -iBasicSize, iBasicSize, iBasicSize, 0, -iBasicSize, -iBasicSize };
	int iNextBlockCenterY[MAX_BLOCK_TYPE] = { 0, iBasicSize, iBasicSize, iBasicSize, PIXEL_SIZE_BLOCK, PIXEL_SIZE_BLOCK, iBasicSize };
	for (int i = 0; i < MAX_BLOCK_TYPE; i++) {
		m_iNextBlockCenterX[i] = iNextBlockCenterX[i];
		m_iNextBlockCenterY[i] = iNextBlockCenterY[i];
	}

	m_iNextBlock_StartY = 210; //  GAME_DRAW_START_Y + 8 * PIXEL_SIZE_BLOCK / 2
    m_iNextBlock_StartY2 = 575;

	for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
		m_client[i].is_available = FALSE;
		m_client[i].ip_address = "";
		for (int x = 0; x < GAME_COLUMN; x++) {
			for (int y = 0; y < GAME_ROW; y++) {
				m_client[i].Block[x][y] = 7;
			}
		}
	}

	mh_ClientSocket = INVALID_SOCKET;
	m_countIP = 0;
	m_selectIP = 0;
	m_selectServerList = 0;
	m_listIP[0].Format("0.0.0.0");
	m_descriptIP[0] = "";
}

CGame::~CGame() {
	PlaySound(NULL, AfxGetInstanceHandle(), NULL);
}

void CGame::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_GameMessage, m_strMessage);
    DDX_Radio(pDX, IDC_GameMode0, m_iGameDifficulty);
    DDX_Control(pDX, IDC_TotalScore, m_ctrlGameScore);
    DDX_Control(pDX, IDC_EventList, m_event_list);
    DDX_Radio(pDX, IDC_CommMode, m_iCommMode);
    DDX_Radio(pDX, IDC_GamePlayerMode_0, m_GamePlayerMode);
    DDX_Radio(pDX, IDC_Dir0, m_iDir);
}


BEGIN_MESSAGE_MAP(CGame, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_GameStart, &CGame::OnBnClickedGamestart)
	ON_BN_CLICKED(IDC_GameMode0, &CGame::OnBnClickedGamemode0)
	ON_BN_CLICKED(IDC_GameMode1, &CGame::OnBnClickedGamemode1)
	ON_BN_CLICKED(IDC_GameMode2, &CGame::OnBnClickedGamemode2)
	ON_BN_CLICKED(IDC_ShowGridLine, &CGame::OnBnClickedShowgridline)
	ON_BN_CLICKED(IDC_Connect, &CGame::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND_MSG, &CGame::OnBnClickedSendMsg)
	ON_BN_CLICKED(IDC_CommMode, &CGame::OnBnClickedCommmode)
	ON_BN_CLICKED(IDC_CommMode2, &CGame::OnBnClickedCommmode2)
	ON_BN_CLICKED(IDC_GamePlayerMode_0, &CGame::OnBnClickedGameplayermode0)
	ON_BN_CLICKED(IDC_GamePlayerMode_1, &CGame::OnBnClickedGameplayermode1)
	ON_BN_CLICKED(IDC_Mute, &CGame::OnBnClickedMute)
	ON_BN_CLICKED(IDC_ChangeDevice, &CGame::OnBnClickedChangedevice)
    ON_BN_CLICKED(IDC_GameMode3, &CGame::OnBnClickedGamemode3)
    ON_BN_CLICKED(IDC_Dir0, &CGame::OnBnClickedDir0)
    ON_BN_CLICKED(IDC_Dir1, &CGame::OnBnClickedDir1)
    ON_BN_CLICKED(IDC_GameMode4, &CGame::OnBnClickedGamemode4)
    ON_EN_CHANGE(IDC_GameMode5, &CGame::OnEnChangeGamemode5)
    ON_BN_CLICKED(IDC_Ghost, &CGame::OnBnClickedGhost)
    ON_BN_CLICKED(IDC_DummyButton, &CGame::OnBnClickedDummybutton)
    ON_BN_CLICKED(IDC_Record, &CGame::OnBnClickedRecord)
END_MESSAGE_MAP()

// CGame 메시지 처리기
void CGame::Init_DrawGame() {
	CClientDC dc(this);
	CPen BlackPen, GrayPen;
	BlackPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	GrayPen.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	dc.SelectObject(&BlackPen);
	dc.Rectangle(GAME_DRAW_START_X-1, GAME_DRAW_START_Y-1, GAME_DRAW_START_X + GAME_COLUMN * PIXEL_SIZE_BLOCK+2, GAME_DRAW_START_Y + GAME_ROW * PIXEL_SIZE_BLOCK+2);
	
	// Next HOLD Block AREA 
	dc.Rectangle(NEXT_BLOCK_STARTX - 1, m_iNextBlock_StartY - 1, NEXT_BLOCK_STARTX + 3 * PIXEL_SIZE_BLOCK + 2, m_iNextBlock_StartY + 4 * PIXEL_SIZE_BLOCK + 2);
    dc.Rectangle(HOLD_BLOCK_STARTX - 1, m_iNextBlock_StartY2 - 1, HOLD_BLOCK_STARTX + 3 * PIXEL_SIZE_BLOCK + 2, m_iNextBlock_StartY2 + 4 * PIXEL_SIZE_BLOCK + 2);

	for (int i = 0; i <= GAME_ROW; i++) {
		if (m_bDrawGridLine || i == 0 || i == GAME_ROW) {
			if (i == 0 || i == GAME_ROW)dc.SelectObject(&BlackPen); else dc.SelectObject(&GrayPen);
			dc.MoveTo(GAME_DRAW_START_X, GAME_DRAW_START_Y + i * PIXEL_SIZE_BLOCK);
			dc.LineTo(GAME_DRAW_START_X + GAME_COLUMN * PIXEL_SIZE_BLOCK, GAME_DRAW_START_Y + i * PIXEL_SIZE_BLOCK);
		}
	}

	for (int j = 0; j <= GAME_COLUMN; j++) {
		if (m_bDrawGridLine || j == 0 || j == GAME_COLUMN) {
			if (j == 0 || j == GAME_COLUMN)dc.SelectObject(&BlackPen); else dc.SelectObject(&GrayPen);
			dc.MoveTo(GAME_DRAW_START_X + j * PIXEL_SIZE_BLOCK, GAME_DRAW_START_Y);
			dc.LineTo(GAME_DRAW_START_X + j * PIXEL_SIZE_BLOCK, GAME_DRAW_START_Y + GAME_ROW * PIXEL_SIZE_BLOCK);
		}
	}
}

BOOL CGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CClientDC dc(this);
	
	CDC MemDC; MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;	bitmap.LoadBitmapA(IDB_BITMAP1);
	CBitmap *pOldBitmap = MemDC.SelectObject(&bitmap);

	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
	m_pOldBitmap = m_MemDC.SelectObject(&m_Bitmap);

	for (int i = 0; i <= MAX_BLOCK_TYPE; i++) {
		m_MemDC_Block[i].CreateCompatibleDC(&dc);
		m_Bitmap_Block[i].CreateCompatibleBitmap(&dc, 50, 50); 
		m_pOldBitmap_Block[i] = m_MemDC_Block[i].SelectObject(&m_Bitmap_Block[i]);
		if (i == MAX_BLOCK_TYPE || m_iBlockShowType == 0) {
			CBrush brush;
			brush.CreateSolidBrush(m_ColorBlock[i]);
			m_MemDC_Block[i].SelectObject(&brush);
			m_MemDC_Block[i].Rectangle(-10, -10, 210, 210);
		}
		else 
			m_MemDC_Block[i].BitBlt(0, 0, PIXEL_SIZE_BLOCK - 1, PIXEL_SIZE_BLOCK - 1, &MemDC, i*(PIXEL_SIZE_BLOCK - 1), 0, SRCCOPY);
	}	

    // Ghost
    {
        m_MemDC_Block[MAX_BLOCK_TYPE + 1].CreateCompatibleDC(&dc);
        m_Bitmap_Block[MAX_BLOCK_TYPE + 1].CreateCompatibleBitmap(&dc, 50, 50);
        m_pOldBitmap_Block[MAX_BLOCK_TYPE + 1] = m_MemDC_Block[MAX_BLOCK_TYPE + 1].SelectObject(&m_Bitmap_Block[MAX_BLOCK_TYPE + 1]);

        CBrush brush;
        brush.CreateSolidBrush(RGB(235, 235, 235));
        CPen pen;
        pen.CreatePen(PS_SOLID, 5, RGB(250, 250, 250));
        m_MemDC_Block[MAX_BLOCK_TYPE + 1].SelectObject(&pen);
        m_MemDC_Block[MAX_BLOCK_TYPE + 1].SelectObject(&brush);

        m_MemDC_Block[MAX_BLOCK_TYPE + 1].Rectangle(-30, -30, 100, 100);
        m_MemDC_Block[MAX_BLOCK_TYPE + 1].Rectangle(5, 5, 34, 34);
    }

    {
        m_MemDC_Combo.CreateCompatibleDC(&dc);
        m_Bitmap_Combo.LoadBitmapA(IDB_BITMAP3);
        m_pOldBitmap_Combo = m_MemDC_Combo.SelectObject(&m_Bitmap_Combo);

        m_MemDC_NUM.CreateCompatibleDC(&dc);
        m_Bitmap_NUM.LoadBitmapA(IDB_BITMAP4);
        m_pOldBitmap_NUM = m_MemDC_NUM.SelectObject(&m_Bitmap_NUM);

        m_MemDC_Blank.CreateCompatibleDC(&dc);
        m_Bitmap_Blank.CreateCompatibleBitmap(&dc, 500, 500);
        m_pOldBitmap_Blank = m_MemDC_Blank.SelectObject(&m_Bitmap_Blank);
    }

    CheckVersion();    
	Init_Game();
	SetTimer(1, 10, NULL);
	SetTimer(2, 100, NULL);	
    SetTimer(TimerGameTime, 1000, NULL);
    SetTimer(100, 350, NULL);

	InitSegment(24, 36, 4);

	CFont Font;
	Font.CreatePointFont(200, "바탕");
	m_ctrlGameScore.SetFont(&Font);

	GetIpAddress();
	AddEventString("현재 IP Address는 " + m_listIP[0] + "입니다");

	m_server_ip_saved.Format("%s", (LPCTSTR)GetRegGame_RegistryData("Setting", "PreSavedServer"));
	if (m_server_ip_saved == "") m_server_ip_saved.Format("%s", (LPCTSTR)m_listIP[m_selectIP]);
	SetDlgItemText(IDC_IP_ADDRESS, m_listIP[m_selectIP]);
	SetDlgItemText(IDC_DeviceInfo, m_descriptIP[m_selectIP]);
	
	for (int i = 0; i < MAX_CLIENT_COUNT - 1; i++) {
		UpdatePlayer(i);
	}

	UDF;
	SetTimer(TIMER_ID_SEND_BRICK, TIME_SEND_BRICK, NULL);
	GetDlgItem(IDC_ChangeDevice)->EnableWindow(TRUE);
	PlayerInit();
    
    m_iDir = atoi(GetRegGame_RegistryData("Setting", "Dir"));
    
    m_GamePlayerMode = atoi(GetRegGame_RegistryData("Setting", "G_MODE"));
    if (m_GamePlayerMode == GAME_PLAY_MODE_SINGLE)
        OnBnClickedGameplayermode0();
    else 
        OnBnClickedGameplayermode1();

    {
        CString str;
        str = GetRegGame_RegistryData("Setting", "TrainLevel");
        if (str == "") str = "1000";
        SetDlgItemText(IDC_GameMode5, str);

        str = GetRegGame_RegistryData("Setting", "Show_Ghost");
        if (str == "") str = "1";
        m_bShowGhost = atoi(str);
        CheckDlgButton(IDC_Ghost, m_bShowGhost);

        str = GetRegGame_RegistryData("Setting", "Mute");
        if (str == "") str = "0";
        CheckDlgButton(IDC_Mute, atoi(str));

        str = GetRegGame_RegistryData("Setting", "GridLine");
        if (str == "") str = "1";
        m_bDrawGridLine = atoi(str);
        CheckDlgButton(IDC_ShowGridLine, atoi(str));

        if (IsDlgButtonChecked(IDC_Mute) == FALSE) PlaySound((LPCSTR)MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP);
        else PlaySound(NULL, AfxGetInstanceHandle(), NULL);
    }  
        
    GetRecord();
    SetWindowText(GAME_TITLE);
    UDF;
    GetDlgItem(IDC_DummyButton)->SetFocus();
	return FALSE;  
}


void CGame::OnPaint() {
	CPaintDC dc(this); 
	Init_DrawGame();
	DrawBlock();
    DrawHoldBlock();
}

void CGame::OnLButtonDown(UINT nFlags, CPoint point) {
	CDialogEx::OnLButtonDown(nFlags, point);
	m_strMessage.Format("X:%d   Y:%d", point.x, point.y);
	UDF;
}

void CGame::Wait(DWORD dwMillisecond) {
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	if (dwMillisecond == 0) {
	    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
	        TranslateMessage(&msg);
	        DispatchMessage(&msg);
	    }
	}
	else {
		while (GetTickCount() - dwStart < dwMillisecond) {
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

void CGame::CheckVersion() {
    // Version Check    
    CString str;
    int iVersion = atoi(GetRegGame_RegistryData("Setting", "Version"));

    if (iVersion < GAME_VERSION) {
        // 11에서 registry위치 변경됨
        if (iVersion == 0) {
            // 0이면 읽지 못한 것이고 이전 Log인지 check 
			int iPreVersion = atoi(GetRegGame_RegistryData_Pre("Game\\BestRecord_Score", "version"));
            if ( iPreVersion >0){
                // 이전 로그가 있다면 가져 올지 확인 하자
                if (AfxMessageBox("이전 버전으로 저장된 설정이 있습니다. 가져 올까요??", MB_YESNO) == IDYES) {
                    // 설정 가져오기
                    Conver_PreSetting();
                }
                else {
                    // 전 설정 초기화
                    Init_Setting();
                }
            }
            else {
                // Pre Version : 설정 없음  --> 전 설정 초기화
                Init_Setting();
            }
        }
        else {
            // 현재보다 낮은 버전의 설정 어떻게 할까?
            //   ==> 10까지는  가만히 두자
            //   ==> 현재 버전만 Update
            if (iVersion < 10) {
                Init_Setting();
            }
        }
        str.Format("%d", GAME_VERSION);
        SetRegGame_RegistryData("Setting", "Version", str);
    }
    else if (iVersion > GAME_VERSION) {
        // 프로그램보다 높은 버전이 저장되어 있을때 어떻게 ??
    }
    
}


BOOL CGame::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			OnBnClickedSendMsg();
		}
		if (m_iGameMode == GAMEMODE_ON) {
            if (!m_bPause || pMsg->wParam == 'P') {
                if (m_bReadyAPM || pMsg->wParam != m_iPreAPM_Key) m_iAPM_count++;
                switch (pMsg->wParam) {
                case 32:  // spacebar                    
                    DropBlock();
                    break;
                case 37: // Left Arrow ==> Move Block to the Left
                    MoveBlockLeft();
                    break;
                case 38: // Up Arrow
                    if (m_iDir == 0) RotateBlock(ROTATE_CW);
                    else RotateBlock(ROTATE_CCW);
                    break;
                case 39: // Right Arrow
                    MoveBlockRight();
                    break;
                case 40: // Down Arrow
                    MoveBlockDown();
                    break;

                case 'C':
                    if (m_iDir == 1) RotateBlock(ROTATE_CW);
                    else RotateBlock(ROTATE_CCW);
                    break;

                case 'P':
                    m_bPause ^= 0x01;
                    break;

                case VK_CONTROL:
                    HoldBlock();
                    break;
                }
                m_iPreAPM_Key = pMsg->wParam;
                m_bReadyAPM = FALSE;
            }
		}
	}
    else if (pMsg->message == WM_KEYUP) {
        m_bReadyAPM = TRUE;
    }
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGame::Init_Setting() {
    char *level[4] = { "초급", "중급", "상급", "Hell" };

    for (int i = 0; i < 4; i++) {
        SetRegGame_RegistryData("BestRecord_Score", level[i], "0");
        SetRegGame_RegistryData("BestRecord_Lines", level[i], "0");
    }
}


void CGame::Conver_PreSetting() {
    char *level[4] = { "초급", "중급", "상급", "Hell" };
    for (int i = 0; i < 4; i++) {
        SetRegGame_RegistryData("BestRecord_Score", level[i], GetRegGame_RegistryData_Pre("Game\\BestRecord_Score", level[i]));
        SetRegGame_RegistryData("BestRecord_Lines", level[i], GetRegGame_RegistryData_Pre("Game\\BestRecord_Lines", level[i]));
    }

    CString strItem, strValue;
    for (int i = 0; i < N_RECORD; i++) {
        strItem.Format("Score_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("Line_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("Combo_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("T_spin%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("Perfect_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("APM%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("Date_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("time_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("GameTime_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
        strItem.Format("Level_%d", i);
        SetRegGame_RegistryData("BestRecord", strItem, GetRegGame_RegistryData_Pre("BestRecord", strItem));
    }


    // 기타 세팅
    strItem = "Dir";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    strItem = "G_MODE";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    strItem = "Multi_Mode";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    strItem = "Mute";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    strItem = "Show_Ghost";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    strItem = "Single_Lvl";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    strItem = "TrainLevel";
    SetRegGame_RegistryData("Setting", strItem, GetRegGame_RegistryData_Pre("Game", strItem));


    strValue = GetRegGame_RegistryData_Pre("Game", "ServerListIndex");
    SetRegGame_RegistryData("Server", "ServerListIndex", strValue);

    for (int i = 1; i <= atoi(strValue); i++) {
        strItem.Format("ServerList%d", i);
        SetRegGame_RegistryData("Server", strItem, GetRegGame_RegistryData_Pre("Game", strItem));
    }
}


void CGame::Init_Game() {
    m_bPause = FALSE;
    m_bIsHoldEmpty = TRUE;
    m_iT_SpinCnt = 0;
    m_iAPM_count = 0;
    m_iPerfectCnt = 0;
    m_iComboCnt = 0;
    m_iAPM = 0;
    m_iLines = 0;
    m_iGameScore = 0;
    m_iCountBottomRowAdd = 0;
    m_iMaxComboCnt = 0;
    m_iGameTime = 0;
    m_bTSpin = 0;

	Init_DrawGame();
	m_iDropTime = 1000;
	switch (m_iGameDifficulty) {
	case 0: // 
		m_iDropTime = GAME_DROP_TIME_0;
		break;

	case 1: 
		m_iDropTime = GAME_DROP_TIME_1;
		break;

	case 2:
		m_iDropTime = GAME_DROP_TIME_2;
		break;

    case 3:
        m_iDropTime = GAME_DROP_TIME_3;
        break;

    case 4:
        m_iDropTime = GetDlgItemInt(IDC_GameMode5);
        break;

	default: // 
		m_iDropTime = 1200;
		break;
	}

	m_iBestRecordScore = atoi(GetRegGame_RegistryData("BestRecord_Score", m_strGameLevel));
	m_iBestRecordLines = atoi(GetRegGame_RegistryData("BestRecord_Lines", m_strGameLevel));
    UpdateScore();

   	for (int i = 0; i < GAME_COLUMN; i++) {
		for (int j = 0; j < GAME_ROW; j++) {
			m_iBlock[i][j] = MAX_BLOCK_TYPE;
		}
	}
       
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 2; k++) {
				m_iBlockType[i][j][k] = 0;
			}
			m_iBlockType[i][j][2] = MAX_BLOCK_TYPE;
		}
	}
	m_bInitGame = TRUE;
	if (IsDlgButtonChecked(IDC_Mute) == FALSE) {
		PlaySound((LPCSTR)MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP);
	}
	Load_NewBlock();
}

void CGame::Load_NewBlock() {
    static int cnt = 0;
    m_bChangeHoldBlock = 0;
    m_bTSpin = FALSE;
    if (m_GamePlayerMode == GAME_PLAY_MODE_MULTI) {
        if (m_iDropTime > 100) {
            --m_iDropTime;
        }
    }
    else {
        if (m_iGameDifficulty == 4) {
            m_iDropTime = GetDlgItemInt(IDC_GameMode5);
        }
        else {
            if (m_iDropTime > 300) m_iDropTime -= m_iGameDifficulty;
            else if (m_iDropTime > 100) {
                --m_iDropTime;
            }
            else {
                if (++cnt > 5) {
                    --m_iDropTime;
                    cnt = 0;
                }
                if (m_iDropTime < 10) m_iDropTime = 10;
            }
        }
    }

    m_iGameScore += (2000 - m_iDropTime) / 100 + (m_iLines / 5 + (int)(pow(2, m_iComboCnt))) * (m_iGameDifficulty + 1);
    UpdateDropTime();
    UpdateScore();

    if (m_iGameDifficulty >= 3) {
		m_iCountBottomRowAdd++;
		if (m_iCountBottomRowAdd > max(3, max(40 - m_iLines / 2, 15 - m_iLines / 80))) {
			m_iCountBottomRowAdd = 0;
			AddBottomRow();
		}
	}
    else if (m_iGameDifficulty == 2) {
        m_iCountBottomRowAdd++;
        if (m_iCountBottomRowAdd > max(40 - m_iLines / 2, 15 )) {
            m_iCountBottomRowAdd = 0;
            AddBottomRow();
        }
    }
	else if (m_iGameDifficulty == 1) {
		m_iCountBottomRowAdd++;
		if (m_iCountBottomRowAdd > max(60 - m_iLines / 3, 20)) {
			m_iCountBottomRowAdd = 0;
			AddBottomRow();
		}
	}
	else if (m_iGameDifficulty == 0) {
		m_iCountBottomRowAdd++;
		if (m_iCountBottomRowAdd > max(90 - m_iLines / 4, 25)) {
			m_iCountBottomRowAdd = 0;
			AddBottomRow();
		}
	}

	if (!m_bInitGame) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 3; k++) {
				m_iBlockType[0][j][k] = m_iBlockType[1][j][k];
			}
		}
	}
	else {
		srand((unsigned)time(NULL));
	}

	// Generate Block Type 
	int iType = rand() % MAX_BLOCK_TYPE;
    LoadBlockData(1, iType);

	if (m_bInitGame) {
		m_bInitGame = FALSE;
		//Wait(10);
		Load_NewBlock();
	}
	else {
		m_dwStartTime = GetTickCount();
		m_iBlockPositionX = GAME_COLUMN / 2;
		m_iBlockPositionY = 1;

		BOOL bContinue = TRUE;
		for (int i = 0; i < 4; i++) {
            if (m_iBlockType[0][i][1] + m_iBlockPositionY >= 0)
			if (m_iBlock[m_iBlockType[0][i][0] + m_iBlockPositionX][m_iBlockType[0][i][1] + m_iBlockPositionY] != MAX_BLOCK_TYPE) {
				bContinue = FALSE;
			}
		}

		// Draw Block
		DrawBlock();

		if (!bContinue) {
			m_iGameMode = GAMEMODE_END;			
			if (IsDlgButtonChecked(IDC_Mute) == FALSE) PlaySound((LPCSTR)MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_ASYNC | SND_RESOURCE);

            if (m_GamePlayerMode != GAME_PLAY_MODE_MULTI) {   // Single Mode: 0  --> m_iGameMode = 2 우선은 2가지 모드만 있으므로 멀티가 아닌것으로 분기함
                if (m_iGameScore > m_Record[N_RECORD - 1].score) {
                    RECORD record;
                    record.score = m_iGameScore;
                    record.line = m_iLines;
                    record.combo = m_iMaxComboCnt;
                    record.t_spin = m_iT_SpinCnt;
                    record.perfect = m_iPerfectCnt;
                    record.apm = m_iAPM;
                    record.gameTime = m_iGameTime;
                    SYSTEMTIME t;
                    GetLocalTime(&t);
                    record.date = (t.wYear % 100) * 10000 + t.wMonth * 100 + t.wDay;
                    record.time = t.wHour * 10000 + t.wMinute * 100 + t.wSecond;
                    record.lvl = m_iGameDifficulty;
                    CheckRecord(record);
                }
            }

			if (m_GamePlayerMode == GAME_PLAY_MODE_SINGLE && m_iBestRecordScore <= m_iGameScore) {
				m_strMessage.Format("Congratualation ! New Record! \r\n Score : %s ", (LPCTSTR)AddComma(m_iGameScore));
				CString strValue;
				strValue.Format("%ld", m_iGameScore);
				SetRegGame_RegistryData("BestRecord_Score", m_strGameLevel, strValue);
				strValue.Format("%d", m_iLines);
				SetRegGame_RegistryData("BestRecord_Lines", m_strGameLevel, strValue);

				m_iBestRecordScore = m_iGameScore;
				m_iBestRecordLines = m_iLines;
			}
			else {
				m_strMessage.Format("Game End!! \r\n Score : %s", (LPCTSTR)AddComma(m_iGameScore));	
			}

			if (m_GamePlayerMode == GAME_PLAY_MODE_MULTI) {
				//최종 Block 상태 전달
				SendCurrentBrick();
				// Game 끝 서버에 통보
				SendGameEnd();
				AddEventString(m_strMessage);
			}
			else {
				AddEventString(m_strMessage);
				AfxMessageBox(m_strMessage);
				GetDlgItem(IDC_GameStart)->EnableWindow(TRUE);
                GetDlgItem(IDC_Record)->EnableWindow(TRUE);
			}		

			GetDlgItem(IDC_GameMode0)->EnableWindow(TRUE);
			GetDlgItem(IDC_GameMode1)->EnableWindow(TRUE);
			GetDlgItem(IDC_GameMode2)->EnableWindow(TRUE);
            GetDlgItem(IDC_GameMode3)->EnableWindow(TRUE);
            GetDlgItem(IDC_GameMode4)->EnableWindow(TRUE);
            GetDlgItem(IDC_GameMode5)->EnableWindow(TRUE);
            
            GetDlgItem(IDC_GamePlayerMode_1)->EnableWindow(TRUE);
            GetDlgItem(IDC_GamePlayerMode_0)->EnableWindow(TRUE);
			if (IsDlgButtonChecked(IDC_Mute) == FALSE) PlaySound((LPCSTR)MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_ASYNC | SND_RESOURCE);	
		}
	}
}

void CGame::DrawBlock() {
	CClientDC dc(this);  

	for (int i = 0; i < GAME_COLUMN; i++) {
		for (int j = 0; j < GAME_ROW; j++) {
			dc.BitBlt(GAME_DRAW_START_X + i * PIXEL_SIZE_BLOCK + 1, GAME_DRAW_START_Y + j * PIXEL_SIZE_BLOCK + 1, PIXEL_SIZE_BLOCK-1, PIXEL_SIZE_BLOCK-1, &m_MemDC_Block[m_iBlock[i][j]], 0, 0, SRCCOPY);
		}
	}

	dc.Rectangle(NEXT_BLOCK_STARTX - 1, m_iNextBlock_StartY - 1, NEXT_BLOCK_STARTX + 3 * PIXEL_SIZE_BLOCK + 2, m_iNextBlock_StartY + 4 * PIXEL_SIZE_BLOCK + 2);
	

    // Get Ghost Block MoveY 
    if (m_bShowGhost) {
        int maxMoveY = GAME_ROW;
        if (m_iGameMode == GAMEMODE_ON) {
            BOOL bConti = TRUE;

            for (int i = 0; i < 4; i++) {
                int moveY = 0;
                int x = m_iBlockType[0][i][0] + m_iBlockPositionX;
                for (;;) {
                    int y = m_iBlockType[0][i][1] + m_iBlockPositionY + moveY;
                    if (m_iBlock[x][y] != 7 || y == GAME_ROW) {
                        moveY--;
                        break;
                    }
                    moveY++;
                }
                if (maxMoveY > moveY) maxMoveY = moveY;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (m_iBlockType[0][i][1] + m_iBlockPositionY >= 0) {
                int GhostY = m_iBlockType[0][i][1] + m_iBlockPositionY + maxMoveY;
                if (GhostY < GAME_ROW && GhostY >= 0)
                    dc.BitBlt(GAME_DRAW_START_X + (m_iBlockType[0][i][0] + m_iBlockPositionX) * PIXEL_SIZE_BLOCK + 1, GAME_DRAW_START_Y + GhostY * PIXEL_SIZE_BLOCK + 1, PIXEL_SIZE_BLOCK - 1, PIXEL_SIZE_BLOCK - 1, &m_MemDC_Block[MAX_BLOCK_TYPE + 1], 0, 0, SRCCOPY);

                dc.BitBlt(GAME_DRAW_START_X + (m_iBlockType[0][i][0] + m_iBlockPositionX) * PIXEL_SIZE_BLOCK + 1, GAME_DRAW_START_Y + (m_iBlockType[0][i][1] + m_iBlockPositionY) * PIXEL_SIZE_BLOCK + 1, PIXEL_SIZE_BLOCK - 1, PIXEL_SIZE_BLOCK - 1, &m_MemDC_Block[m_iBlockType[0][i][2]], 0, 0, SRCCOPY);
            }
        }
    }

    // Draw Current Block
    for (int i = 0; i < 4; i++) {
        if (m_iBlockType[0][i][1] + m_iBlockPositionY >= 0) {
             dc.BitBlt(GAME_DRAW_START_X + (m_iBlockType[0][i][0] + m_iBlockPositionX) * PIXEL_SIZE_BLOCK + 1, GAME_DRAW_START_Y + (m_iBlockType[0][i][1] + m_iBlockPositionY) * PIXEL_SIZE_BLOCK + 1, PIXEL_SIZE_BLOCK - 1, PIXEL_SIZE_BLOCK - 1, &m_MemDC_Block[m_iBlockType[0][i][2]], 0, 0, SRCCOPY);
        }
    }

    for (int i = 0; i < 4; i++) {
        dc.BitBlt(NEXT_BLOCK_STARTX + (m_iBlockType[1][i][0] + 1) * PIXEL_SIZE_BLOCK + 1 + m_iNextBlockCenterX[m_iBlockType[1][i][2]], m_iNextBlock_StartY + (m_iBlockType[1][i][1] + 1) * PIXEL_SIZE_BLOCK + 1 + m_iNextBlockCenterY[m_iBlockType[1][i][2]], PIXEL_SIZE_BLOCK, PIXEL_SIZE_BLOCK, &m_MemDC_Block[m_iBlockType[1][i][2]], 0, 0, SRCCOPY);
    }
}


void CGame::OnTimer(UINT_PTR nIDEvent){

	CDialogEx::OnTimer(nIDEvent);
    switch (nIDEvent) {
    case 1:
        if (m_GamePlayerMode == GAME_PLAY_MODE_SINGLE && m_bPause) break;
        CheckTimeout(); break;
    case 2:
        if (m_iGameMode == GAMEMODE_ON) {
            m_iContineDropTime = min(++m_iContineDropTime, 2);
        }
        break;

    case TIMER_ID_SEND_BRICK:
        if (m_iGameMode == GAMEMODE_ON) {
            SendCurrentBrick();
        }
        break;

    case TimerGameTime:
        if (m_iGameMode == GAMEMODE_ON && m_bPause == 0) {
            m_iGameTime++;     
            m_iAPM = m_iAPM_count * 60 / m_iGameTime;
            m_strMessage.Format("C:%d (%d)  T:%d  P:%d  %02d:%02d", m_iComboCnt, m_iMaxComboCnt, m_iT_SpinCnt, m_iPerfectCnt, m_iGameTime / 60, m_iGameTime % 60);
            SetDlgItemText(IDC_GameMessage, m_strMessage);
			//DrawTime();
            if (++m_iTotalGameTime > 1800) {
                m_iTotalGameTime = 0;
				m_bPause = 1;
                AfxMessageBox("[자동 알림] \r\n프로그램 실행후 누적 게임시간 30분이 경과되었습니다. \r\n참고하세요...");
            }
        }

    case 100: 
        DisplayCombo();
        break;
    }
}

void CGame::DrawTime() {
	int px = 391;
	int py = 779;
	CClientDC dc(this);
	static int seed = -1;
	int seed_new = m_iGameTime % 10;
	if (seed == seed_new) return;
	seed = seed_new;
	dc.Rectangle(px - 5, py-5, px + 50, py+40);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 0, 0));
	dc.SelectObject(&pen);
	dc.SelectObject(&brush);

	for (int i = 0; i < 7; i++) {
		if (m_UseSegment[seed][i]) {
			dc.MoveTo(px + m_segment[i][0][0], py + m_segment[i][0][1]);
			for (int j = 1; j<7; j++) 
				dc.LineTo(px + m_segment[i][j][0], py + m_segment[i][j][1]);
		}
	}
}


void CGame::CheckTimeout() {
	if (m_iGameMode == GAMEMODE_ON) {    
        if (bBusyBlock) { Wait(0); }
        
		int iTimeoutBottom = CheckMoveDown();
		if (GetTickCount() - m_dwStartTime > (DWORD)(max(m_iDropTime, iTimeoutBottom))) {
            if (bBusyBlock) { Wait(0); }
            bBusyBlock = TRUE;
			// 이동 가능 check
			BOOL bIsMovable = TRUE;
			for (int i = 0; i < 4; i++) {
				int x = m_iBlockType[0][i][0] + m_iBlockPositionX;
				int y = m_iBlockType[0][i][1] + m_iBlockPositionY + 1;

				if (m_iBlock[x][y] != MAX_BLOCK_TYPE || y >= GAME_ROW) {
					bIsMovable = FALSE;
				}
			}

			if (bIsMovable) {
				m_iBlockPositionY++;
				m_dwStartTime = GetTickCount();	
                bIsMovable = TRUE;
                bBusyBlock = FALSE;
			}
			else {				
                bBusyBlock = FALSE;
				CheckLine();              
                if (m_bTSpin) {
                    Load_NewBlock();
                    m_bTSpin = TRUE;
                }
                else {
                    Load_NewBlock();
                }
			}
			DrawBlock();
		}
	}
}

void CGame::OnBnClickedGamestart() {	
    Invalidate(TRUE);
    UDF;
	if (m_iGameMode == GAMEMODE_ON) return;
	m_iGameMode = GAMEMODE_ON;
    m_iMaxComboCnt = 0;
    bBusyBlock = FALSE;

	if (m_GamePlayerMode == GAME_PLAY_MODE_MULTI) {
		// nplayer check
		int n_player = 0;
		for (int i = 0; i < MAX_CLIENT_COUNT - 1; i++) {
			if (m_client[i].is_available) { n_player++; }
		}

		if (m_iCommMode == 0 && n_player == 1) {
            int ret = AfxMessageBox("현재 접속된 사람이 없습니다. 계속할까요?", MB_YESNO);
			if ( ret != IDYES) { return;  }
		}
		Init_Game();
		Load_NewBlock();
		SendGameStart();
	}
	else {        
        CString str;
        str.Format("%d", m_iGameDifficulty);
        if (GetRegGame_RegistryData("Setting", "Single_Lvl") != str) {
            SetRegGame_RegistryData("Setting", "Single_Lvl", str);
        }
		Init_Game();
		Load_NewBlock();
		GetDlgItem(IDC_GameMode0)->EnableWindow(FALSE);
		GetDlgItem(IDC_GameMode1)->EnableWindow(FALSE);
		GetDlgItem(IDC_GameMode2)->EnableWindow(FALSE);
        GetDlgItem(IDC_GameMode3)->EnableWindow(FALSE);
        GetDlgItem(IDC_GameMode4)->EnableWindow(FALSE);
        GetDlgItem(IDC_GameMode5)->EnableWindow(FALSE);
        if (m_iGameDifficulty == 4) {            
            CString str;
            GetDlgItemText(IDC_GameMode5, str);
            if (str != GetRegGame_RegistryData("Setting", "TrainLevel"))
                SetRegGame_RegistryData("Setting", "TrainLevel", str);
        }
	}
    GetDlgItem(IDC_GamePlayerMode_1)->EnableWindow(FALSE);
    GetDlgItem(IDC_GamePlayerMode_0)->EnableWindow(FALSE);

	GetDlgItem(IDC_GameStart)->EnableWindow(FALSE);
    GetDlgItem(IDC_Record)->EnableWindow(FALSE);
	GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::MoveBlockLeft() {
	if (m_iGameMode == GAMEMODE_ON) {
        if (bBusyBlock) { Wait(0); }
        bBusyBlock = TRUE;
		BOOL bIsMovable = TRUE;
		for (int i = 0; i < 4; i++) {
			int x = m_iBlockType[0][i][0] + m_iBlockPositionX - 1;
			int y = m_iBlockType[0][i][1] + m_iBlockPositionY;

			if (m_iBlock[x][y] != MAX_BLOCK_TYPE || x < 0) {
				bIsMovable = FALSE;
			}
		}

		if (bIsMovable) {
			m_iBlockPositionX--;
            bBusyBlock = FALSE;
			DrawBlock();
		}
        else {
            bBusyBlock = FALSE;
        }
	}	
}

void CGame::MoveBlockRight() {
	if (m_iGameMode == GAMEMODE_ON) {
		BOOL bIsMovable = TRUE;
		for (int i = 0; i < 4; i++) {
			int x = m_iBlockType[0][i][0] + m_iBlockPositionX + 1;
			int y = m_iBlockType[0][i][1] + m_iBlockPositionY;

			if (m_iBlock[x][y] != MAX_BLOCK_TYPE || x < 0) {
				bIsMovable = FALSE;
			}
		}

		if (bIsMovable) {
			m_iBlockPositionX++;
			DrawBlock();
		}
	}
}

void CGame::RotateBlock(int mode) {
    static int rot[4][2][2] = {
                        {   {1, 0}, {0, 1},     }, // 0deg
                        {   {0, -1}, {1, 0}     }, // 90deg
                        {   {-1, 0}, {0, -1}    }, // 180deg
                        {   {0, 1}, {-1, 0}     }, // 270deg
    };

    int cur_idx = 1;
    int delta_idx = 2;
    if (mode != 1) cur_idx = 3;

    if (m_iGameMode == GAMEMODE_ON) {
        if (bBusyBlock) { Wait(0); }

        bBusyBlock = TRUE;

        BOOL bIsMovable = TRUE;

        if (m_iBlockType[0][0][2] == 5) return;

        int dx = 0, dy = 0;
        for (int m = 0; m < 2; m++) {
            dx = 0, dy = 0;
            int cont = 1;
            while (cont) {
                if (abs(dx) > 2 || abs(dy) > 1) {
                    bIsMovable = FALSE;
                    cont = 0;
                    break;
                }
                cur_idx = (cur_idx + delta_idx) % 4;
                int x, y;
                bIsMovable = TRUE;
             
                for (int i = 0; i < 4; i++) {
                    x = rot[cur_idx][0][0] * m_iBlockType[0][i][0] + rot[cur_idx][0][1] * m_iBlockType[0][i][1] + m_iBlockPositionX + dx;
                    y = rot[cur_idx][1][0] * m_iBlockType[0][i][0] + rot[cur_idx][1][1] * m_iBlockType[0][i][1] + m_iBlockPositionY + dy;
                    
                    if (x < 0) {
                        dx++;
                        cont = 1;
                        break;
                    }

                    if (y < 0) {
                        dy++;
                        cont = 1;
                        break;
                    }

                    if (x >= GAME_COLUMN) {
                        dx--;
                        cont = 1;
                        break;
                    }

                    if (y >= GAME_ROW) {
                        dy--;
                        cont = 1;
                        bIsMovable = FALSE;
                        break;
                    }
                    if (m_iBlock[x][y] != MAX_BLOCK_TYPE) {
                        bIsMovable = FALSE;
                    }
                    if (i == 3) cont = 0;
                }                
            }
            if (bIsMovable) break;
        }
        
        if (bIsMovable) {
            m_iBlockPositionX += dx;
            m_iBlockPositionY += dy;
            for (int i = 0; i < 4; i++) {
                int x = rot[cur_idx][0][0] * m_iBlockType[0][i][0] + rot[cur_idx][0][1] * m_iBlockType[0][i][1];
                int y = rot[cur_idx][1][0] * m_iBlockType[0][i][0] + rot[cur_idx][1][1] * m_iBlockType[0][i][1];

                m_iBlockType[0][i][0] = x;
                m_iBlockType[0][i][1] = y;
            }

            bBusyBlock = FALSE;
            DrawBlock();
            return;
        }        

        bBusyBlock = FALSE;
    }
}

void CGame::DropBlock() {
	if (m_iGameMode == GAMEMODE_ON) {
        if (bBusyBlock) { Wait(0); }        
        bBusyBlock = TRUE;
		BOOL bConti = TRUE;
        int maxMoveY = GAME_ROW;
        for (int i = 0; i < 4; i++) {
            int moveY = 0;
            int x = m_iBlockType[0][i][0] + m_iBlockPositionX;
            for (;;) {
                int y = m_iBlockType[0][i][1] + m_iBlockPositionY + moveY;
                if (m_iBlock[x][y] != 7 || y == GAME_ROW) {
                    moveY--;
                    break;
                }
                moveY++;
            }
            if (maxMoveY > moveY) maxMoveY = moveY;
        }
        m_iBlockPositionY += maxMoveY;
        m_iGameScore += 5 * maxMoveY; 
        		
		CheckLine();
		Load_NewBlock();
        bBusyBlock = FALSE;
		DrawBlock();

        GetDlgItem(IDC_DummyButton)->SetFocus();        
	}
}

void CGame::MoveBlockDown() {
	if (m_iGameMode == GAMEMODE_ON && m_iContineDropTime == 2) {
        if (bBusyBlock) { Wait(0); }
        bBusyBlock = TRUE;
		BOOL bConti = TRUE;
		m_iContineDropTime = 0;
		for (int j = 0; j < 4; j++) {
			if (m_iBlockType[0][j][1] + m_iBlockPositionY + 1 >= GAME_ROW) {
				bConti = FALSE;
			}
			if (bConti) {
				if (m_iBlock[m_iBlockType[0][j][0] + m_iBlockPositionX][m_iBlockType[0][j][1] + m_iBlockPositionY + 1] != MAX_BLOCK_TYPE) {
					bConti = FALSE;
				}
			}
		}
		m_iGameScore += 3;

		if (bConti) m_iBlockPositionY++;
        bBusyBlock = FALSE;
		DrawBlock();
	}
}


void CGame::CheckLine() {
	// Check Row Block Wipe out 
	int iDeleteRow = 0;
    int isT_Spin = 0;
	BOOL bUDF = FALSE;

    // T-Turn check
    if (m_iBlockType[0][0][2] == 4 && m_iBlockPositionX > 0 && m_iBlockPositionX < GAME_COLUMN - 1 && m_iBlockPositionX > 0 && m_iBlockPositionX < GAME_COLUMN - 1) {
        int cnt = 0;
        for (int j = 0; j < 4; j++) if (m_iBlockType[0][j][1] == 0) cnt++;
        if (cnt == 3) {
            if (m_iBlock[m_iBlockPositionX - 1][m_iBlockPositionY - 1] != MAX_BLOCK_TYPE && m_iBlock[m_iBlockPositionX - 1][m_iBlockPositionY + 1] != MAX_BLOCK_TYPE) isT_Spin = 1;
            if (m_iBlock[m_iBlockPositionX + 1][m_iBlockPositionY - 1] != MAX_BLOCK_TYPE && m_iBlock[m_iBlockPositionX + 1][m_iBlockPositionY + 1] != MAX_BLOCK_TYPE) isT_Spin = 1;
        }
    }

    for (int i = 0; i < 4; i++) {
        m_iBlock[m_iBlockType[0][i][0] + m_iBlockPositionX][m_iBlockType[0][i][1] + m_iBlockPositionY] = m_iBlockType[0][i][2];
    }    

	for (int i = GAME_ROW - 1; i >= 0; i--) {
		BOOL bDelete = TRUE;
		for (int j = 0; j < GAME_COLUMN; j++) {
			if (m_iBlock[j][i] == MAX_BLOCK_TYPE) bDelete = FALSE;
		}
		if (bDelete) {
            if (isT_Spin && i == m_iBlockPositionY) isT_Spin = 2;
			for (int j = i; j > 0; j--) {
				for (int k = 0; k < GAME_COLUMN; k++) {
					m_iBlock[k][j] = m_iBlock[k][j - 1];
				}
			}
			for (int j = 0; j < GAME_COLUMN; j++) {
				m_iBlock[j][0] = MAX_BLOCK_TYPE;
			}
			iDeleteRow++;
			i++;						
			++m_iLines;
			bUDF = TRUE;
		}
	}

	if (bUDF) {
        if (++m_iComboCnt >= 3)  iDeleteRow++;
        if (m_iMaxComboCnt < m_iComboCnt) m_iMaxComboCnt = m_iComboCnt;

		switch (iDeleteRow) {
		case 1: m_iGameScore += 80 * (m_iGameDifficulty + 1) ; break;
		case 2: m_iGameScore += 200 * (m_iGameDifficulty + 1); break;
		case 3: m_iGameScore += 450 * (m_iGameDifficulty + 1); break;
		case 4: m_iGameScore += 960 * (m_iGameDifficulty + 1); break;
		case 5: m_iGameScore += 1200 * (m_iGameDifficulty + 1); break;
        case 6: m_iGameScore += 1600 * (m_iGameDifficulty + 1); break;
		}        

        if (isT_Spin == 2) {
            m_iT_SpinCnt++;
            m_iGameScore += 1600 * (m_iGameDifficulty + 1);
            iDeleteRow = 2 * iDeleteRow + 1;
            m_bTSpin = TRUE;
        }

        // perfect check
        for (int j = 0; j < GAME_COLUMN; j++) {
            if (m_iBlock[j][GAME_ROW-1] != MAX_BLOCK_TYPE) break;
            if (j == GAME_COLUMN - 1) {
                //Perfect Game
                iDeleteRow++;
                m_iGameScore += 3000;
                m_iPerfectCnt++;
            }
        }
        
		// Block전달
        if (iDeleteRow >= 2) {
            if (mh_ClientSocket != INVALID_SOCKET) {
                char *p_send_data = new char[6];
                *p_send_data = 27;
                *(unsigned short int *)(p_send_data + 1) = 2;
                *(p_send_data + 3) = 10;
                *(p_send_data + 4) = 2;
                *(p_send_data + 5) = iDeleteRow;
                send(mh_ClientSocket, p_send_data, 6, 0);
                delete[] p_send_data;
            }
        }       

        UpdateScore(); 
	}
    else { m_iComboCnt = 0; }    
}

void CGame::SetRegGame_RegistryData(CString strAddress, CString strItem, CString stData) {
	HKEY hkey = NULL;
	DWORD dwDisp;

	CString strRegDirectory;
	strRegDirectory.Format("%s%s", DEFAULT_REGISTRY_SETTING_GAME_PATH, (LPCTSTR)strAddress);

	const unsigned char *str;
	unsigned char buff[1024];
	memcpy(buff, (unsigned char *)(LPCTSTR)stData, strlen(stData));
	str = buff;

	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	while (result != ERROR_SUCCESS) {
		// Key가 없다면
		::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
		result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	}

	::RegSetValueEx(hkey, strItem, 0, REG_SZ, str, strlen(stData));
	::RegCloseKey(hkey);
	::RegFlushKey(hkey);
}

CString CGame::GetRegGame_RegistryData(CString strAddress, CString strItem) {
	HKEY hkey = NULL;
	DWORD dwDisp;

	CString strRegDirectory;
	strRegDirectory.Format("%s%s", DEFAULT_REGISTRY_SETTING_GAME_PATH, (LPCTSTR)strAddress);
	LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);

	if (result != ERROR_SUCCESS) {
		// Key가 없다면
		::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
		result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
	}

	CString str;
	DWORD dwType = REG_SZ;
	DWORD dwCount = _MAX_PATH;

	::RegQueryValueEx(hkey, strItem, NULL, &dwType, (LPBYTE)str.GetBuffer(_MAX_PATH), &dwCount);
	::RegCloseKey(hkey);
	::RegFlushKey(hkey);
	return str;
}

CString CGame::GetRegGame_RegistryData_Pre(CString strAddress, CString strItem) { 
    HKEY hkey = NULL;
    DWORD dwDisp;

    CString strRegDirectory;
    strRegDirectory.Format("MY_TOOL\\Game\\%s", (LPCTSTR)strAddress);
    LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);

    if (result != ERROR_SUCCESS) {
        // Key가 없다면
        ::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
        result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    }

    CString str;
    DWORD dwType = REG_SZ;
    DWORD dwCount = _MAX_PATH;

    ::RegQueryValueEx(hkey, strItem, NULL, &dwType, (LPBYTE)str.GetBuffer(_MAX_PATH), &dwCount);
    ::RegCloseKey(hkey);
    ::RegFlushKey(hkey);
    return str;
}

void CGame::OnBnClickedGamemode0() {
	m_strGameLevel = "초급";
    m_iGameDifficulty = 0;
    UpdateDropTime(GAME_DROP_TIME_0);
    m_iLines = 0;
    m_iGameScore = 0;
    m_iCountBottomRowAdd = 0;

    m_iBestRecordScore = atoi(GetRegGame_RegistryData("BestRecord_Score", m_strGameLevel));
    m_iBestRecordLines = atoi(GetRegGame_RegistryData("BestRecord_Lines", m_strGameLevel));

    UpdateScore();
    GetDlgItem(IDC_GameMode5)->EnableWindow(FALSE);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::OnBnClickedGamemode1() {
	m_strGameLevel = "중급";
    m_iGameDifficulty = 1;
    UpdateDropTime(GAME_DROP_TIME_1);
    m_iLines = 0;
    m_iGameScore = 0;
    m_iCountBottomRowAdd = 0;

    m_iBestRecordScore = atoi(GetRegGame_RegistryData("BestRecord_Score", m_strGameLevel));
    m_iBestRecordLines = atoi(GetRegGame_RegistryData("BestRecord_Lines", m_strGameLevel));

    UpdateScore();
    GetDlgItem(IDC_GameMode5)->EnableWindow(FALSE);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::OnBnClickedGamemode2() {
	m_strGameLevel = "상급";
    m_iGameDifficulty = 2;
    UpdateDropTime(GAME_DROP_TIME_2);
    GetDlgItem(IDC_GameMode5)->EnableWindow(FALSE);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::OnBnClickedGamemode3() {
    m_strGameLevel = "Hell";
    m_iGameDifficulty = 3;
    UpdateDropTime(GAME_DROP_TIME_3);
    m_iLines = 0;
    m_iGameScore = 0;
    m_iCountBottomRowAdd = 0;

    m_iBestRecordScore = atoi(GetRegGame_RegistryData("BestRecord_Score", m_strGameLevel));
    m_iBestRecordLines = atoi(GetRegGame_RegistryData("BestRecord_Lines", m_strGameLevel));

    UpdateScore();

    GetDlgItem(IDC_GameMode5)->EnableWindow(FALSE);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::OnBnClickedGamemode4()
{
    m_strGameLevel = "Train";
    m_iGameDifficulty = 4;
    UpdateDropTime(GetDlgItemInt(IDC_GameMode5));
    m_iLines = 0;
    m_iGameScore = 0;
    m_iCountBottomRowAdd = 0;

    m_iBestRecordScore = atoi(GetRegGame_RegistryData("BestRecord_Score", m_strGameLevel));
    m_iBestRecordLines = atoi(GetRegGame_RegistryData("BestRecord_Lines", m_strGameLevel));

    UpdateScore();
    GetDlgItem(IDC_GameMode5)->EnableWindow(TRUE);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

CString CGame::AddComma(long iValue) {
	CString strReturn = "";
	BOOL bIsMinus = FALSE;
	if (iValue < 0) {
		bIsMinus = TRUE;
		iValue = -iValue;
	}

	while (iValue >= 1000) {
        if (strReturn != "") {
            CString strTemp = strReturn;
            strReturn.Format("%03d,%s", iValue % 1000, (LPCTSTR)strTemp);
        }
		else 
			strReturn.Format("%03d", iValue % 1000);
		iValue /= 1000;
	}

	if (strReturn == "")
		strReturn.Format("%d", iValue);
    else {
        CString strTemp = strReturn;
        strReturn.Format("%d,%s", iValue, (LPCTSTR)strTemp);
    }

	if (bIsMinus) {
        CString strTemp = strReturn;
		strReturn.Format("-%s", (LPCTSTR)strTemp);
	}
	return strReturn;
}

void CGame::AddBottomRow() {

    if (m_GamePlayerMode == GAME_PLAY_MODE_MULTI) {
        // No Add line for multi player mode
        return;
    }
    
	// Check Top Line
	BOOL bGameOver = FALSE;
	for (int i = 0; i < GAME_COLUMN; i++) {
		if (m_iBlock[i][0] != MAX_BLOCK_TYPE) bGameOver = TRUE;
	}

	for (int j = 1; j < GAME_ROW; j++) {
		for (int i = 0; i < GAME_COLUMN; i++) {
			m_iBlock[i][j - 1] = m_iBlock[i][j];
		}
	}

	if (bGameOver && m_iBlock[m_iBlockPositionX][m_iBlockPositionY] == MAX_BLOCK_TYPE) {
		m_iBlock[m_iBlockPositionX][m_iBlockPositionY] = 1;
	}

	//Add New Row
    int iMaxUpperRowVacancy = 0;
      
    for (int i = 0; i < GAME_COLUMN; i++) {
        if (m_iBlock[i][GAME_ROW - 2] == MAX_BLOCK_TYPE) iMaxUpperRowVacancy++;
        m_iBlock[i][GAME_ROW - 1] = rand() % MAX_BLOCK_TYPE;
    }
    
    int possible = rand() % 1000;  // possible 0~99
    int nVacancy;
    if (possible < 2) {  // 0.2%
        nVacancy = 4;
    }
    else if (possible < 10) {  // 1%
        nVacancy = 3;
    }
    else if (possible < 50 + m_iLines / 15) {  // 5%
        nVacancy = 2;
    }
    else {
        nVacancy = 1;
    }

    while (1) {
        int c = rand() % GAME_COLUMN;
        if (rand() % 1000 > 10 + m_iLines / 15) {            
            if (m_iBlock[c][GAME_ROW-2] == MAX_BLOCK_TYPE) {
                m_iBlock[c][GAME_ROW - 1] = MAX_BLOCK_TYPE;
                nVacancy--;
                break;
           }
        }
        else {
            m_iBlock[c][GAME_ROW - 1] = MAX_BLOCK_TYPE;
            nVacancy--;
            break;
        }
    }

    while (nVacancy) {
        int c = rand() % GAME_COLUMN;
        if (m_iBlock[c][GAME_ROW - 1] == MAX_BLOCK_TYPE) {
            m_iBlock[c][GAME_ROW - 1] = MAX_BLOCK_TYPE;
            nVacancy--;
        }
    }
}

void CGame::OnBnClickedShowgridline() {
    CString str = "0";
    if (IsDlgButtonChecked(IDC_ShowGridLine)) {
        str = "1";
        m_bDrawGridLine = TRUE;
    }
    else {
        m_bDrawGridLine = FALSE;
    }
    SetRegGame_RegistryData("Setting", "GridLine", str);

	Init_DrawGame();
	DrawBlock();
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

int CGame::CheckMoveDown() {
	for (int i = 0; i < 4; i++) {
		if (m_iBlockType[0][i][1] + m_iBlockPositionY + 1 >= GAME_ROW) {	
			return m_iTimeoutBottom;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (m_iBlock[m_iBlockType[0][i][0] + m_iBlockPositionX][m_iBlockType[0][i][1] + m_iBlockPositionY + 1] != MAX_BLOCK_TYPE) {
			return m_iTimeoutBottom;
		}
	}
    return 0;
}

void CGame::AcceptProcess(SOCKET wParam_h_socket) {
	// wParam : 메세지가 발생한 소켓 핸들  mh_listen_socket
	// lParam : 에러나 정보
	
	if (m_iClientCount < MAX_CLIENT_COUNT) {
		SOCKET h_socket = wParam_h_socket;   // mh_listen_socket의 핸들값과 동일
		sockaddr_in client_addr;
		client_addr.sin_family = AF_INET;
		int sockaddr_in_size = sizeof(client_addr);
			
		for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
			if (m_client[i].is_available == FALSE) {
				m_client[i].is_available = TRUE;				
				m_client[i].h_socket = accept(h_socket, (SOCKADDR *)&client_addr, &sockaddr_in_size);

                m_client[i].ip_address.Format(" %s", inet_ntoa(client_addr.sin_addr));
				WSAAsyncSelect(m_client[i].h_socket, m_hWnd, PORT_SERVER_READ_FRAME, FD_READ | FD_CLOSE);  // Data수신 또는 끊어졌을때 알림

				m_iClientCount++;
				if (i != 0) {
					AddEventString(m_client[i].ip_address + " 새로운 클라이언트가 접속하였습니다.");
				}
				int data_size = 1;
				// 기존 접속정보 전달
				char * p_send_data = new char[5];
				*p_send_data = 27;
				*(unsigned short int *)(p_send_data + 1) = 1;
				
				// 신규 접속자에게 자신의 ID송부
				*(p_send_data + 3) = 4;
				*(p_send_data + 4) = i;
				send(m_client[i].h_socket, p_send_data, 5, 0);

				// 기존 접속자 정보 전달
				*(p_send_data + 3) = 3;
				for (int user = 0; user < MAX_CLIENT_COUNT; user++) {
					if (m_client[user].is_available) {
						if (user != i) { 
							// 신규 접속자 정보 전달							
							*(p_send_data + 4) = i;  // user ID
							send(m_client[user].h_socket, p_send_data, 5, 0);

							*(p_send_data + 4) = user;  // user ID
							send(m_client[i].h_socket, p_send_data, 5, 0);
						}
					}		
				}		
				delete p_send_data;
				break;
			}
		}	
	}
	else {
		AfxMessageBox("최대 접속 회선을 초과하였습니다.");
	}
}

void CGame::ServerFrameReadData(SOCKET h_socket) {
	static int comm_error_count = 0;
	CString str;

	char key, network_message_id;
	recv(h_socket, &key, 1, 0);
	if (key == 27) {
		unsigned short int body_size;
		recv(h_socket, (char *)&body_size, 2, 0); // 2Byte
		recv(h_socket, &network_message_id, 1, 0);

		int sender_id = -1;
		for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
			if (m_client[i].is_available && h_socket == m_client[i].h_socket) {
				sender_id = i;
				break;
			}
		}

		if (body_size > 0) {
			char *p_body_data = new char[body_size];
			ReceiveData(h_socket, p_body_data, body_size);

			if (network_message_id == 1) {
				// Client 채팅정보 
				str.Format("%s", p_body_data);
				int size = str.GetLength();
				for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
					if (m_client[i].is_available) {
						SendChatData_to_clients(m_client[i].h_socket, (char)sender_id, str, size);
					}
				}
			}
			else if (network_message_id == 2) {
				// Send 
				BYTE * p_send_data = new BYTE[body_size + 5];
				*p_send_data = 27;
				*(unsigned short int *)(p_send_data + 1) = body_size + 1;
				*(p_send_data + 3) = 2;
				*(p_send_data + 4) = sender_id;
				memcpy(p_send_data + 5, p_body_data, body_size);
				for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
					if (m_client[i].is_available && i != sender_id) {
						send(m_client[i].h_socket, (char *)p_send_data, body_size + 5, 0);
					}
				}
				delete p_send_data;
			}
			else if (network_message_id == 10) {
				char *p_send_data = new char[10];
				*p_send_data = 27;
				*(unsigned short int *)(p_send_data + 1) = body_size;
				*(p_send_data + 3) = network_message_id;
				*(p_send_data + 4) = *p_body_data;

				if (*p_body_data == 1) {
					// Game Start
					// 추가 정보 Loading : Total Players
					m_iTotalPlayer = 0;
					for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
						if (m_client[i].is_available) {
							m_iTotalPlayer++;
							m_IsSurvival[i] = TRUE;
						}
						else {
							m_IsSurvival[i] = FALSE;
						}
					}
					*(unsigned short int *)(p_send_data + 1) = 2;
					*(p_send_data + 3) = network_message_id;
					*(p_send_data + 4) = *p_body_data;
					*(p_send_data + 5) = (char)m_iTotalPlayer;
					m_iGradePlayer = m_iTotalPlayer;

					for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
						if (m_client[i].is_available) {
							send(m_client[i].h_socket, p_send_data, 6, 0);
						}
					}
				}
				else if (*p_body_data == 2) {
					*(p_send_data + 5) = *(p_body_data + 1);
					for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
						if (m_client[i].is_available && i != sender_id) {
							send(m_client[i].h_socket, p_send_data, 6, 0);
						}
					}
				}
				else if (*p_body_data == 99) {		// Game End						
					char *p_send_data = new char[10];
					*p_send_data = 27;
					*(unsigned short int *)(p_send_data + 1) = 3;
					*(p_send_data + 3) = network_message_id;
					*(p_send_data + 4) = 99;
					*(p_send_data + 5) = sender_id;  // 종료 Player ID
					*(p_send_data + 6) = m_iGradePlayer;  // 순위
					m_IsSurvival[sender_id] = FALSE;
					for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
						if (m_client[i].is_available && i != sender_id) {
							send(m_client[i].h_socket, p_send_data, 7, 0);
						}
					}

					*(unsigned short int *)(p_send_data + 1) = 2;
					if (m_client[sender_id].is_available) {
						*(p_send_data + 4) = 98;
						*(p_send_data + 5) = (char)m_iGradePlayer;  // 최종 순위  2~N		 
						m_iGradePlayer--;
						send(m_client[sender_id].h_socket, p_send_data, 6, 0);
					}
					str.Format("%d is game over", sender_id);
					AddEventString(str);

					if (m_iGradePlayer == 1) {
						// 1위에게 Game 종료 알림
						int iWin = -1;
						for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
							if (m_IsSurvival[i] && m_client[i].is_available) {
								iWin = i;
							}
						}

						for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
							if (m_client[i].is_available) {
								*(p_send_data + 4) = 97;
								*(p_send_data + 5) = (char)iWin;
								send(m_client[i].h_socket, p_send_data, 6, 0);								
							}
						}

						// 재게임 가능 
						for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
							if (m_IsSurvival[i] && m_client[i].is_available) {
								*(p_send_data + 4) = 96;
								*(p_send_data + 5) = (char)m_iGradePlayer;
								send(m_client[i].h_socket, p_send_data, 6, 0);
								AddEventString(str);
							}
						}
						GetDlgItem(IDC_GamePlayerMode_0)->EnableWindow(TRUE);
                        GetDlgItem(IDC_GamePlayerMode_1)->EnableWindow(TRUE);
                        GetDlgItem(IDC_GameStart)->EnableWindow(TRUE);
                        GetDlgItem(IDC_Record)->EnableWindow(TRUE);
					}
					delete[] p_send_data;
				}
				delete[] p_send_data;
			}
			delete[] p_body_data;
		}
		WSAAsyncSelect(h_socket, m_hWnd, PORT_SERVER_READ_FRAME, FD_READ | FD_CLOSE);
		comm_error_count = 0;
	}
	else {	// FD_CLOSE						
		if (++comm_error_count > 5) {
			ClientCloseProcess(h_socket, 0);
			AddEventString("Client가 접속을 해제하였습니다.");
		}
		else {
			WSAAsyncSelect(h_socket, m_hWnd, PORT_SERVER_READ_FRAME, FD_READ | FD_CLOSE);
			ClientCloseProcess(h_socket, 0);
		}
	}
}

void CGame::ClientCloseProcess(SOCKET wParam_h_socket, char param_force_flag) {
	if (param_force_flag == 1) {  
		LINGER temp_linger = { TRUE, 0 };  // 데이터가 수신 여부와 상관없이 소켓을 닫는 옵션
		setsockopt(wParam_h_socket, SOL_SOCKET, SO_LINGER, (char*)&temp_linger, sizeof(LINGER));
	}

	closesocket(wParam_h_socket);

	int close_id = -1;
	for (int i = 1; i < MAX_CLIENT_COUNT; i++) {
		if (m_client[i].is_available) {
			if (m_client[i].h_socket == wParam_h_socket) {
				m_iClientCount--;
				m_client[i].is_available = FALSE;
				AddEventString(m_client[i].ip_address + "접속을 해제하였습니다.");
				close_id = i;
				break;
			}
		}
	}

	if (close_id == -1) return;
	char *p_send_data = new char[6];
	*p_send_data = 27;
	*(unsigned short int *)(p_send_data + 1) = 1;
	*(p_send_data + 3) = 10;
	*(p_send_data + 4) = 90;
	*(p_send_data + 5) = close_id;


	for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
		if (m_client[i].is_available && close_id != i) {
			send(m_client[i].h_socket, p_send_data, 6, 0);
		}
	}

	delete[] p_send_data;
}


void CGame::ReceiveData(SOCKET param_h_socket, char *param_p_buffer, int param_size) {
	int current_size, total_size = 0, retry_count = 0;
	while (total_size < param_size) {
		current_size = recv(param_h_socket, param_p_buffer + total_size, param_size - total_size, 0);

		if (current_size == SOCKET_ERROR) {
			Wait(0);
			if (retry_count++ > 500) break;   // 바로 끊으면 문제 발생 가능성 있음
		}
		else {
			retry_count = 0;				// 정상 수신 : 에러 카운트 초기화
			total_size += current_size;
		}
	}
}


LRESULT CGame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 변수 선언은 자제하고 함수로 처리하면 처리속도가 빨라진다고 함
	SOCKET h_socket;
	switch (message) {
	case PORT_SERVER_ACCEPT:  // Server Accept		
		AcceptProcess((SOCKET) wParam);
		break;

	case PORT_SERVER_READ_FRAME:  // Server Receive (FD_READ)  or   Disconnect (FD_CLOSE)
		h_socket = (SOCKET)wParam;
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WSAAsyncSelect(h_socket, m_hWnd, PORT_SERVER_READ_FRAME, FD_CLOSE);  // 끊어서 읽을때 호출되지 않도록 처리함
			ServerFrameReadData(h_socket);					
		}		
		else {
			ClientCloseProcess((SOCKET)wParam, 0);
		}
		break;


	case PORT_CLIENT_REQ_CONNECT:  // FD_CONNECT
		ConnectProcess(lParam);		
		break;

	case PORT_CLIENT_READ_FRAME:	// FD_READ | FD_CLOSE
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {  // FD_READ
			WSAAsyncSelect(mh_ClientSocket, m_hWnd, PORT_CLIENT_READ_FRAME, FD_CLOSE);
			ClientReadFrameData();

			if (mh_ClientSocket != INVALID_SOCKET) WSAAsyncSelect(mh_ClientSocket, m_hWnd, PORT_CLIENT_READ_FRAME, FD_READ | FD_CLOSE);
		}
		else {  // FD_CLOSE
			DestroySocket();
			AddEventString("서버에서 연결을 해제하였습니다.");			
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);	
}

void CGame::ClientReadFrameData() {
	// Client 
	static int comm_error_count = 0;
	char key, message_id;
	recv(mh_ClientSocket, &key, 1, 0);
	if (27 == key) {
		unsigned short int body_size;
		recv(mh_ClientSocket, (char *)&body_size, 2, 0);
		recv(mh_ClientSocket, &message_id, 1, 0);

		char* p_body_data;
		if (body_size > 0) {
			p_body_data = new char[body_size];
			ReceiveData(p_body_data, body_size);
		}
		if (message_id == 1) {
			CString str;
			p_body_data[body_size] = 0;
			str.Format("%s", p_body_data + 1);
			str.Format("%d->  %s", p_body_data[0], p_body_data + 1);
			AddEventString(str);
		}
		else if (message_id == 2) {
			int user_id = *p_body_data;
			memcpy(m_client[user_id].Block, p_body_data+1, body_size-1);
			UpdatePlayer(user_id);
		}
		else if (message_id == 3) {
			BOOL bFind = FALSE;
			int id = *p_body_data;			
			m_client[id].is_available = TRUE;
			CString str;
			str.Format("Add Player:%d ", id);
			AddEventString(str);
			UpdatePlayer(id);			
		}
		else if (message_id == 4) {
			BOOL bFind = FALSE;
			m_iMyID = *p_body_data;
			CString str;
			str.Format("서버로부터 할당받은 ID는 %d입니다", m_iMyID);
			AddEventString(str);			
		}
		else if (message_id == 10) {
			int id = *p_body_data;
			if (id == 1) {
				m_iTotalPlayer = *(p_body_data + 1);
				CString str;
				str.Format("Game을 시작합니다. (%d)", m_iTotalPlayer);
				AddEventString(str);
				if (m_iGameMode != GAMEMODE_ON) {					
					Init_Game();
					Load_NewBlock();
					m_iGameMode = GAMEMODE_ON;
					GetDlgItem(IDC_GameMode0)->EnableWindow(FALSE);
					GetDlgItem(IDC_GameMode1)->EnableWindow(FALSE);
					GetDlgItem(IDC_GameMode2)->EnableWindow(FALSE);
                    GetDlgItem(IDC_GameMode3)->EnableWindow(FALSE);
                    GetDlgItem(IDC_GameMode4)->EnableWindow(FALSE);
                    GetDlgItem(IDC_GameMode5)->EnableWindow(FALSE);
					GetDlgItem(IDC_GameStart)->EnableWindow(FALSE);
                    GetDlgItem(IDC_Record)->EnableWindow(FALSE);
					GetDlgItem(IDC_DummyButton)->SetFocus();
				}
			}
			else if (id == 2) {
				m_iIncreaseRow += *(p_body_data + 1);
				CString str;
				str.Format("Attack %d", m_iIncreaseRow);
				Attack();
				AddEventString(str);
			}
			else if (id == 90) {
				int user_id = *(p_body_data + 1);
				int player = -1;
				for (int i = 0; i < MAX_CLIENT_COUNT - 1; i++) {
					if (m_client[i].is_available) {
						m_client[i].is_available = FALSE;
						CString str;
						player = i;
						str.Format("Player %d : 접속해제 되었습니다.", player);
						AddEventString(str);
						UpdatePlayer(i);
						break;
					}
				}
			}
			else if (id == 99) {
				CString str;
				int user_id = *(p_body_data + 1);
				int grade = *(p_body_data + 2);
				int player = -1;
				for (int i = 0; i < MAX_CLIENT_COUNT - 1; i++) {
					if (m_client[i].is_available) {
						player = i;
						str.Format("Player %d : Game over!", player);
						AddEventString(str);	
						break;
					}
				}	
				DrawGrade(user_id, grade-1);
				str.Format(" %d / %d ", grade, m_iTotalPlayer);
				DrawGameStatus(player, 99, str);
			}
			else if (id == 98) {
				// 종료후 최종 순위
				int grade = *(p_body_data + 1);
				CString str;
				str.Format("당신의 최종 순위는 %d / %d입니다.", grade, m_iTotalPlayer);
				AddEventString(str);
				DrawGrade(m_iMyID, grade-1);
			}
			else if (id == 97) {
				// 최종 1위 
				int ID = *(p_body_data + 1);

				CString str;
				if (ID == m_iMyID) {
					str.Format("Congratualation !! You win the game.");
					AddEventString(str);
				}
				DrawGrade (ID, 0);
				str.Format("Game over !!!");
				AddEventString(str);
				m_iGameMode = GAMEMODE_END;
				GetDlgItem(IDC_GameStart)->EnableWindow(TRUE);
                GetDlgItem(IDC_Record)->EnableWindow(TRUE);
				
			}
			else if (id == 96) {
				AddEventString("Game이 종료되었습니다.");
				GetDlgItem(IDC_GameStart)->EnableWindow(TRUE);
                GetDlgItem(IDC_Record)->EnableWindow(TRUE);
			}
		}		
		else {
			CString str;
			str.Format("%d message Received", message_id);
			AddEventString(str);
		}

		delete[] p_body_data;
		comm_error_count = 0;		
		
	}	
	else {
		if (++comm_error_count > 10) {
			DestroySocket();
			AddEventString("잘못된 프로토콜 입니다.");
		}
	}
}


void CGame::ReceiveData(char *param_p_buffer, int param_size) {
	int current_size, total_size = 0, retry_count = 0;
	while (total_size < param_size) {
		current_size = recv(mh_ClientSocket, param_p_buffer + total_size, param_size - total_size, 0);  // 다 받을수 있을까?? ==> 에러 내용도 들어옴
		if (current_size == SOCKET_ERROR) {
			Sleep(1);
			if (++retry_count > 500) break;
		}
		else {
			retry_count = 0;
			total_size += current_size;
		}
	}
}


void CGame::ConnectProcess(LPARAM lParam) {
	if (WSAGETSELECTERROR(lParam) == 0) {
		//접속 성공
		WSAAsyncSelect(mh_ClientSocket, m_hWnd, PORT_CLIENT_READ_FRAME, FD_READ | FD_CLOSE);
		if (m_iCommMode != 0) {
			AddEventString("서버 접속에 성공하였습니다.");

			// 마지막에 저장된 IP 주소와 비교하여 다르면 추가함			
			CString strItem, strValue, strIP;
			GetDlgItemText(IDC_IP_ADDRESS, strIP);
			int index = atoi(GetRegGame_RegistryData("Server", "ServerListIndex"));
			strItem.Format("ServerList%d", index);
			strValue = GetRegGame_RegistryData("Server", strItem);

			if (strValue != strIP) {				
				if (++index >= 10) {
					index = 0;
				}
				strValue.Format("%d", index);
				strItem.Format("ServerList%d", index);
				SetRegGame_RegistryData("Server", strItem, strIP);
				SetRegGame_RegistryData("Server", "ServerListIndex", strValue);
			}
		}
        GetDlgItem(IDC_GameStart)->EnableWindow(TRUE);
        GetDlgItem(IDC_Record)->EnableWindow(TRUE);
	}
	else {
		DestroySocket();
		AddEventString("서버 접속에 실패하였습니다.");
        GetDlgItem(IDC_Connect)->EnableWindow(TRUE);        
	}
}

void CGame::DestroySocket() {
	LINGER temp_linger = { TRUE, 0 }; // 즉각적인 종료
	setsockopt(mh_ClientSocket, SOL_SOCKET, SO_LINGER, (char *)&temp_linger, sizeof(temp_linger));
	closesocket(mh_ClientSocket);  // client는 1개만 있음
	mh_ClientSocket = INVALID_SOCKET;
}


void CGame::OnBnClickedClientconnectbtn()
{
	CString Address;
	if (m_iCommMode == 0) { 
		Address = m_listIP[m_selectIP];
	}
	else {
		GetDlgItemText(IDC_IP_ADDRESS, Address);
		if (Address != "") {
			m_server_ip = Address;
		}
		else {
			AfxMessageBox("접속 IP Address가 비었습니다.");
			return;
		}
		m_req_save_server_ip = TRUE;
	}
	mh_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(Address);
	srv_addr.sin_port = htons(SOCKET_PORT);
	

	WSAAsyncSelect(mh_ClientSocket, m_hWnd, PORT_CLIENT_REQ_CONNECT, FD_CONNECT);
	connect(mh_ClientSocket, (SOCKADDR *)&srv_addr, sizeof(srv_addr));
}

void CGame::AddEventString(CString param_str) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	CString str;
	str.Format("%02d:%02d:%02d] ", t.wHour, t.wMinute, t.wSecond);

	int index = m_event_list.InsertString(-1, str+param_str);	
	m_event_list.SetCurSel(index);
}

void CGame::OnBnClickedServermakebtn()
{
	// 통신 소켓 만들기 : 서버용
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);		// 주소 체계 IP4:AF_INET,        
															// Type :   TCP(연결지향성) : SOCK_STREAM, UDP(비연결지향성) : SOCK_DGRAM
															// protocol : Auto설정 적용

	// 연결 설정하기
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;							// 주소 체계
	srv_addr.sin_addr.s_addr = inet_addr(m_listIP[m_selectIP]);				// ipconfig에서 확인된 주소
	srv_addr.sin_port = htons(SOCKET_PORT);					// 연결 포트       byte ordering : htons()

	// 연결 등록하기
	bind(mh_listen_socket, (SOCKADDR *)&srv_addr, sizeof(srv_addr));
	
	// Accept() 함수로 등록해야하나 접속까지 기다려야 함
	WSAAsyncSelect(mh_listen_socket, m_hWnd, PORT_SERVER_ACCEPT, FD_ACCEPT);  // 사용자 접속시(FD_ACCEPT)  Wnd handle에 해당 포트 Msg발생   (비동기)

	// Client 접속 허용
	listen(mh_listen_socket, 1);  // 동시 접속 1개 
}


void CGame::OnBnClickedConnect() {
	static BOOL is_connected = FALSE;
	UDT;
	GetDlgItem(IDC_Connect)->EnableWindow(FALSE);
	if (is_connected == FALSE) {
		if (m_iCommMode == 0) {
			// Server Mode
			OnBnClickedServermakebtn();
			AddEventString("서버모드로 접속대기중 입니다.");
			OnBnClickedClientconnectbtn();
		}
		else {
			// Client Mode			
			OnBnClickedClientconnectbtn();
		}

		is_connected = TRUE;
		GetDlgItem(IDC_CommMode)->EnableWindow(FALSE);
		GetDlgItem(IDC_CommMode2)->EnableWindow(FALSE);
	}
	else {
		is_connected = FALSE;
		GetDlgItem(IDC_CommMode)->EnableWindow(TRUE);
		GetDlgItem(IDC_CommMode2)->EnableWindow(TRUE);
	}
	//GetDlgItem(IDC_GameStart)->EnableWindow(TRUE);
}


void CGame::OnBnClickedSendMsg() {	
	if (mh_ClientSocket != INVALID_SOCKET) {
		CString str;
		GetDlgItemText(IDC_Chat_Data, str);
		if (str == "") return;
		SendChatData_to_server(mh_ClientSocket, str, str.GetLength());
		str = "";
		SetDlgItemText(IDC_Chat_Data, str);
		GotoDlgCtrl(GetDlgItem(IDC_Chat_Data));
	}
}

void CGame::SendChatData_to_server(SOCKET param_h_socket, const CString& strParam, int parm_size) {
	if (parm_size > 512) {
		AddEventString("전송 글자수 제한 Error : 최대512자");
		return;
	}
	CString str = "";
	char * p_send_data = new char[parm_size + 5];
	*p_send_data = 27;
	*(unsigned short int *)(p_send_data + 1) = parm_size + 1;
	*(p_send_data + 3) = 1;
	strcpy_s(p_send_data + 4, strParam.GetLength(), (LPCTSTR)strParam);
	*(p_send_data + 4 + parm_size) = 0;
	send(param_h_socket, p_send_data, parm_size + 5, 0);
	delete[] p_send_data;
}

void CGame::SendChatData_to_clients(SOCKET param_h_socket, char sender_id, const CString& strParam, int parm_size) {
	if (parm_size > 512) {
		AddEventString("전송 글자수 제한 Error : 최대512자");
		return;
	}
	char * p_send_data = new char[parm_size + 5];
	*p_send_data = 27;
	*(unsigned short int *)(p_send_data + 1) = parm_size + 1;
	*(p_send_data + 3) = 1;
	*(p_send_data + 4) = sender_id;
	strcpy_s(p_send_data + 5, strParam.GetLength(), (LPCTSTR)strParam);
	send(param_h_socket, p_send_data, parm_size + 5, 0);
	delete[] p_send_data;
}

void CGame::GetServerList() {
	CString strItem, strValue;
	int index = atoi(GetRegGame_RegistryData("Server", "ServerListIndex"));

	strItem.Format("ServerList%d", index + m_selectServerList);
	strValue = GetRegGame_RegistryData("Server", strItem);

	SetDlgItemText(IDC_IP_ADDRESS, strValue);
}

void CGame::GetIpAddress() {
	CString ip; // 여기에 lcoal ip가 저장됩니다.
	// 복수개의 랜카드를 사용할 때
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	CString str = "", strTemp;

	pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));

	if (NULL == pAdapterInfo) {
		return;
	}

	if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (NULL == pAdapterInfo) {
			return;
		}
	}

	dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);

	if (NO_ERROR == dwRetVal) {
		pAdapter = pAdapterInfo;
		int count = 0;
		while (pAdapter != NULL) {
			strTemp.Format("%s", pAdapter->IpAddressList.IpAddress.String);
			if (strTemp != "0.0.0.0") {
				m_listIP[m_countIP].Format("%s", (LPCTSTR)strTemp);
				m_descriptIP[m_countIP++].Format("%s", pAdapter->Description);
			}
			pAdapter = pAdapter->Next;
		}
	}

	if (pAdapterInfo != NULL) {
		free(pAdapterInfo);
	}
}

void CGame::OnBnClickedCommmode() {
	m_iCommMode = 0;
	GetDlgItem(IDC_IP_ADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_GameStart)->ShowWindow(TRUE);
    GetDlgItem(IDC_Record)->EnableWindow(TRUE);
	SetDlgItemText(IDC_IP_ADDRESS, m_listIP[m_selectIP]);
	SetDlgItemText(IDC_DeviceInfo, m_descriptIP[m_selectIP]);
	GetDlgItem(IDC_DeviceInfo)->ShowWindow(TRUE);

    if (GetRegGame_RegistryData("Setting", "Multi_Mode") != "0") {
        SetRegGame_RegistryData("Setting", "Multi_Mode", "0");
    }
}


void CGame::OnBnClickedCommmode2() {
	m_iCommMode = 1;
	GetDlgItem(IDC_IP_ADDRESS)->EnableWindow(TRUE);
	GetDlgItem(IDC_DeviceInfo)->ShowWindow(FALSE);
	m_selectServerList = 0;
	GetServerList();
    if (GetRegGame_RegistryData("Setting", "Multi_Mode") != "1") {
        SetRegGame_RegistryData("Setting", "Multi_Mode", "1");
    }
}


void CGame::OnBnClickedGameplayermode0() {
    if (GetRegGame_RegistryData("Setting", "G_MODE") != "0") {
        SetRegGame_RegistryData("Setting", "G_MODE", "0");
    }
    GetDlgItem(IDC_GameStart)->MoveWindow(793, 19, 875 - 793, 90 - 19, TRUE);


	m_GamePlayerMode = GAME_PLAY_MODE_SINGLE;
	BOOL show = TRUE;
	GetDlgItem(IDC_GameMode0)->ShowWindow(show);
	GetDlgItem(IDC_GameMode1)->ShowWindow(show);
	GetDlgItem(IDC_GameMode2)->ShowWindow(show);
    GetDlgItem(IDC_GameMode3)->ShowWindow(show);
    GetDlgItem(IDC_GameMode4)->ShowWindow(show);
    GetDlgItem(IDC_GameMode5)->ShowWindow(show);

    GetDlgItem(IDC_GameMode6)->ShowWindow(show);
	show = FALSE;

	GetDlgItem(IDC_CommMode)->ShowWindow(show);
	GetDlgItem(IDC_CommMode2)->ShowWindow(show);
	GetDlgItem(IDC_IP_ADDRESS)->ShowWindow(show);
	GetDlgItem(IDC_Connect)->ShowWindow(show);
	GetDlgItem(IDC_ChangeDevice)->ShowWindow(show);
	GetDlgItem(IDC_DeviceInfo)->ShowWindow(show);

	GetDlgItem(IDC_GameStart)->EnableWindow(TRUE); 
	GetDlgItem(IDC_GameStart)->ShowWindow(TRUE);

    CString str;
    str = GetRegGame_RegistryData("Setting", "Single_Lvl");
    if (str == "") m_iGameDifficulty = 3;
    else m_iGameDifficulty = atoi(str);
    switch (m_iGameDifficulty) {
        case 0: OnBnClickedGamemode0(); break;
        case 1: OnBnClickedGamemode1(); break;
        case 2: OnBnClickedGamemode2(); break;
        case 3: OnBnClickedGamemode3(); break;
    }
    UDF;

    GetDlgItem(IDC_DummyButton)->SetFocus();
}


void CGame::OnBnClickedGameplayermode1() {
    GetDlgItem(IDC_GameStart)->MoveWindow(794, 18, 875 - 794, 62- 18, TRUE);
    if (GetRegGame_RegistryData("Setting", "G_MODE") != "1") {
        SetRegGame_RegistryData("Setting", "G_MODE", "1");
    }

    if (GetRegGame_RegistryData("Setting", "Multi_Mode") != "1") {
        OnBnClickedCommmode();
    }
    else {
        OnBnClickedCommmode2();
    }

	m_GamePlayerMode = GAME_PLAY_MODE_MULTI;
	BOOL show = FALSE;
	GetDlgItem(IDC_GameMode0)->ShowWindow(show);
	GetDlgItem(IDC_GameMode1)->ShowWindow(show);
	GetDlgItem(IDC_GameMode2)->ShowWindow(show);
    GetDlgItem(IDC_GameMode3)->ShowWindow(show);
    GetDlgItem(IDC_GameMode4)->ShowWindow(show);
    GetDlgItem(IDC_GameMode5)->ShowWindow(show);

    GetDlgItem(IDC_GameMode6)->ShowWindow(show);

	show = TRUE;
	GetDlgItem(IDC_CommMode)->ShowWindow(show);
	GetDlgItem(IDC_CommMode2)->ShowWindow(show);
	GetDlgItem(IDC_IP_ADDRESS)->ShowWindow(show);
	GetDlgItem(IDC_Connect)->ShowWindow(show);

	GetDlgItem(IDC_ChangeDevice)->ShowWindow(show);
	GetDlgItem(IDC_DeviceInfo)->ShowWindow(show);

	if (m_iCommMode == 0) {
		GetDlgItem(IDC_IP_ADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_GameStart)->ShowWindow(TRUE);

	}
	else {
		GetDlgItem(IDC_IP_ADDRESS)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_GameStart)->ShowWindow(FALSE);
	}
	m_iGameDifficulty = 2;
	UDF;
	GetDlgItem(IDC_GameStart)->EnableWindow(FALSE);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::SendCurrentBrick() {	
	if (mh_ClientSocket != INVALID_SOCKET) {		
		char * p_send_data = new char[GAME_ROW*GAME_COLUMN + 4];
		*p_send_data = 27;
		*(unsigned short int *)(p_send_data + 1) = GAME_ROW * GAME_COLUMN;
		*(p_send_data + 3) = 2;
		memcpy(p_send_data + 4, m_iBlock, GAME_ROW*GAME_COLUMN);
		send(mh_ClientSocket, p_send_data, GAME_ROW*GAME_COLUMN + 4, 0);
		delete[] p_send_data;
	}	
}

void CGame::PlayerInit(void) {
	int idc[2] = { IDC_STATIC_P1 , IDC_STATIC_P2 };
	for (int player = 0; player < MAX_CLIENT_COUNT-1; player++) {
		int width = PIXEL_SIZE_BLOCK_PLAYER * GAME_COLUMN, height = PIXEL_SIZE_BLOCK_PLAYER * GAME_ROW;
		int x0 = GAME_DRAW_START_X_PLAYER + (width + GAP_PLAYER_X) * player;
		GetDlgItem(idc[player])->MoveWindow(x0, GAME_DRAW_START_Y_PLAYER, width, height);
	}
}

void CGame::UpdatePlayer(int ID) {
	int player = ID;
	if (m_iMyID < ID) {
		player--;
	}
	else if (m_iMyID == ID){
		return ;
	}


	int idc[2] = { IDC_STATIC_P1 , IDC_STATIC_P2 };
	if (player >= MAX_CLIENT_COUNT - 1 || player < 0) {
		AfxMessageBox("Player 입력 이상");
		return;
	}
	// GAME_COLUMN][GAME_ROW];;

	int width = PIXEL_SIZE_BLOCK_PLAYER * GAME_COLUMN, height = PIXEL_SIZE_BLOCK_PLAYER * GAME_ROW;
	int x0 = GAME_DRAW_START_X_PLAYER + (width + GAP_PLAYER_X) * player;

	if (m_client[ID].is_available) {
		GetDlgItem(idc[player])->ShowWindow(FALSE);
		GetDlgItem(idc[player])->MoveWindow(561, 122, 140, 230);

		// 그림을 그리자
		CClientDC dc(this);
		CBrush brush1, brush2;
		brush1.CreateSolidBrush(RGB(255, 255, 255));
		brush2.CreateSolidBrush(RGB(155, 155, 155));
		dc.SelectObject(&brush1);
		dc.Rectangle(x0 - 2, GAME_DRAW_START_Y_PLAYER - 2, x0 + width + 2, GAME_DRAW_START_Y_PLAYER + height + 3);
		dc.Rectangle(x0, GAME_DRAW_START_Y_PLAYER, x0 + width, GAME_DRAW_START_Y_PLAYER + height+1);

		// Guide line
		for (int i = 1; i < GAME_ROW; i++) {
			int y = i * 16 + GAME_DRAW_START_Y_PLAYER;
			dc.MoveTo(x0, y);
			dc.LineTo(x0 + width, y);
		}

		for (int i = 1; i < GAME_COLUMN; i++) {
			int x = i * 16 + x0;
			dc.MoveTo(x, GAME_DRAW_START_Y_PLAYER);
			dc.LineTo(x, GAME_DRAW_START_Y_PLAYER + height);
		}

		dc.SelectObject(&brush2);
		for (int i = 0; i < GAME_COLUMN; i++) {
			for (int j = 0; j < GAME_ROW; j++) {
				dc.BitBlt(x0 + i * 16 + 1, GAME_DRAW_START_Y_PLAYER + j * 16 + 1, 16 - 1, 16 - 1, &m_MemDC_Block[m_client[ID].Block[i][j]], 0, 0, SRCCOPY);
			}
		}
	}
	else {
		GetDlgItem(idc[player])->ShowWindow(TRUE);
		GetDlgItem(idc[player])->MoveWindow(x0, GAME_DRAW_START_Y_PLAYER, width, height);
	}
}


void CGame::DrawGameStatus(int player, int status, CString strComment) {
	if (player == -1) return;
	CClientDC dc(this);

	int width = 144, height = 288, dx = 25;
	int x0 = 561 + (width + dx) * player, y0 = 114;
}


void CGame::SendGameStart() {
	if (mh_ClientSocket == INVALID_SOCKET) return;
	char *p_send_data = new char[5];
	*p_send_data = 27;
	*(unsigned short int *)(p_send_data + 1) = 1;
	*(p_send_data + 3) = 10;
	*(p_send_data + 4) = 1;
	send(mh_ClientSocket, p_send_data, 5, 0);
	delete[] p_send_data;
}

void CGame::SendGameEnd() {
	if (mh_ClientSocket == INVALID_SOCKET) return;
	char *p_send_data = new char[5];
	*p_send_data = 27;
	*(unsigned short int *)(p_send_data + 1) = 1;
	*(p_send_data + 3) = 10;
	*(p_send_data + 4) = 99;
	send(mh_ClientSocket, p_send_data, 5, 0);
	delete[] p_send_data;
}


void CGame::DrawGrade(int ID, int iGrade) {
	CPoint Ribon[2][6];	
	CPoint CharacterGrade[3][13];	
	CPoint PtCenter;

	double fRibon[2][6][2] = {
		{ {-15.41, -19.04}, {-4.58, -20.96}, {-9.09, -46.56}, {-13.56, -40.19}, {-19.93, -44.65}, {-15.41, -19.04},  },
		{ { 3.58, -18.96}, {14.41, -17.04}, {18.93, -42.65}, {12.56, -38.19}, {8.10, -44.56}, { 3.58, -18.96},	}
	};

	int nGrade[3] = { 7, 13, 13 };
	double fCharacter[3][13][2] = {
		{ { 0.5, -9.6 }, { 0.5, 7.0 }, { -2.4, 7.0 }, { -2.4, 9.7 }, { 2.4, 9.7 }, { 2.4, -9.6 }, { 0.5, -9.6 }, },
		{ { -4.9, -9.6 }, { -4.9, 2.7 }, { 2.1, 2.7 }, { 2.1, 6.9 }, { -2.7, 6.9 }, { -2.7, 9.6 }, { 4.9, 9.6 }, { 4.9, 0.1 }, { -2.0, 0.1 }, { -2.0, -6.9 }, { 4.9, -6.9 }, { 4.9, -9.6 }, { -4.9, -9.6 }, },
		{ { -4.9, -9.6 }, { -4.9, -6.9 }, { 2.1, -6.9 }, { 2.1, 0.1 }, { -4.9, 0.1 }, { -4.9, 2.7 }, { 2.1, 2.7 }, { 2.1, 7.0 }, { -2.7, 7.0 }, { -2.7, 9.6 }, { 4.9, 9.6 }, { 4.9, -9.6 }, { -4.9, -9.6 }, },
	};

	COLORREF colorMedal[3][5] = {
		{  RGB(250, 253, 106), RGB(253, 192, 21),  RGB(255, 209, 4), RGB(249, 250, 99), RGB(255, 196, 0), },
		{  RGB(240, 240, 240), RGB(215, 215, 217), RGB(192, 192, 192), RGB(248, 248, 248), RGB(173, 173, 173),  },
		{  RGB(253, 214, 214), RGB(228, 179, 143), RGB(134, 91, 66), RGB(201, 154, 151), RGB(115, 57, 57), },
	};

	double round[4] = { 30., 28.49, 20.86, 16.92, };

	// Get Draw Center Point
	double fScale;
	if (ID == m_iMyID) {
		fScale = 3;
		PtCenter.x = PIXEL_SIZE_BLOCK * GAME_COLUMN / 2 + GAME_DRAW_START_X;
		PtCenter.y = PIXEL_SIZE_BLOCK * GAME_ROW / 2 + GAME_DRAW_START_Y - 30;
	}
	else {
		fScale = 1.9;
		if (m_iMyID < ID) { ID--; }
		PtCenter.x = GAME_DRAW_START_X_PLAYER + (PIXEL_SIZE_BLOCK_PLAYER * GAME_COLUMN) / 2 + ID * (GAP_PLAYER_X + PIXEL_SIZE_BLOCK_PLAYER * GAME_COLUMN);
		PtCenter.y = GAME_DRAW_START_Y_PLAYER + PIXEL_SIZE_BLOCK_PLAYER * GAME_ROW / 2;
	}
	   	 
	// Get Draw Point for Ribbon
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			Ribon[i][j].x = PtCenter.x + (int)(fRibon[i][j][0] * fScale);
			Ribon[i][j].y = PtCenter.y - (int)(fRibon[i][j][1] * fScale);
		}
	}

	// Get Draw Point for Grade Character
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < nGrade[i]; j++) {
			CharacterGrade[i][j].x = PtCenter.x + (int)(fCharacter[i][j][0] * fScale);
			CharacterGrade[i][j].y = PtCenter.y - (int)(fCharacter[i][j][1] * fScale);
		}
	}

	// Draw on Screen
	CClientDC dc(this);	

	// Draw Ribon
	CPen penRibon;
	CBrush brushRibon;
	penRibon.CreatePen(PS_SOLID, 2, RGB(205, 29, 29));
	brushRibon.CreateSolidBrush(RGB(255, 109, 109));	
	dc.SelectObject(&penRibon);
	dc.SelectObject(&brushRibon);
	dc.Polygon(Ribon[0], 6);
	dc.Polygon(Ribon[1], 6);

	// Draw Medal
	CPen penMedal;
	CBrush brushMedal;
	for (int i = 0; i < 4; i++) {
		penMedal.CreatePen(PS_SOLID, 1, colorMedal[iGrade][i]);	
		brushMedal.CreateSolidBrush(colorMedal[iGrade][i]);
		dc.SelectObject(&penMedal);
		dc.SelectObject(&brushMedal);
		int iRadius = (int)(round[i] * fScale);
		dc.Ellipse(PtCenter.x - iRadius, PtCenter.y - iRadius, PtCenter.x + iRadius, PtCenter.y + iRadius);
	}

	penMedal.CreatePen(PS_SOLID, 1, colorMedal[iGrade][4]);
	brushMedal.CreateSolidBrush(colorMedal[iGrade][4]);
	dc.SelectObject(&penMedal);
	dc.SelectObject(&brushMedal);
	dc.Polygon(CharacterGrade[iGrade], nGrade[iGrade]);
}

void CGame::Attack(){
	int vacancy[4];	
	int iColor = rand() % (MAX_BLOCK_TYPE - 1);

	int inc_row = m_iIncreaseRow;
	m_iIncreaseRow = 0;
	for (int i = 0; i < inc_row; i++) {
		vacancy[i] = rand() % GAME_COLUMN;
	}

	for (int x = 0; x < GAME_COLUMN; x++) {
		for (int y = 0; y < inc_row; y++) {
			m_iBlock[x][0] = min(m_iBlock[x][y], m_iBlock[x][y + inc_row]);
		}

		for (int y = inc_row+1; y < GAME_ROW; y++) {
			m_iBlock[x][y-inc_row] = m_iBlock[x][y];
		}
	}

	for (int dy = 0; dy < inc_row; dy++) {
		int y = GAME_ROW - 1 - dy;
		for (int x = 0; x < GAME_COLUMN; x++) {
			m_iBlock[x][y] = iColor;
		}
		m_iBlock[vacancy[dy]][y] = 7;
	}

	CString str;
	str.Format("%d %d %d %d", vacancy[0], vacancy[1], vacancy[2], vacancy[3]);
	AddEventString(str);
}

void CGame::OnBnClickedMute()
{
    if (IsDlgButtonChecked(IDC_Mute)) {
        PlaySound(NULL, AfxGetInstanceHandle(), NULL);
        SetRegGame_RegistryData("Setting", "Mute", "1");
    }
    else {
        PlaySound((LPCSTR)MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_ASYNC | SND_RESOURCE | SND_LOOP);
        SetRegGame_RegistryData("Setting", "Mute", "0");
    }
    GetDlgItem(IDC_DummyButton)->SetFocus();
}


void CGame::OnBnClickedChangedevice() {
	if (m_iCommMode == 0) {
		if (++m_selectIP >= m_countIP) {
			m_selectIP = 0;
		}
		SetDlgItemText(IDC_IP_ADDRESS, m_listIP[m_selectIP]);
		SetDlgItemText(IDC_DeviceInfo, m_descriptIP[m_selectIP]);
	}
	else {
		CString strValue, strItem, strIP;
		int index = atoi(GetRegGame_RegistryData("Server", "ServerListIndex"));
		m_selectServerList;
		for (int i = 0; i < 10; i++) {
			if (++m_selectServerList >= 10) {
				m_selectServerList = 0;
			}
			strItem.Format("ServerList%d", (index + m_selectServerList) % 10);
			strValue = GetRegGame_RegistryData("Server", strItem);
			if (strValue != "") {
				SetDlgItemText(IDC_IP_ADDRESS, strValue);
				break;
			}
		}
	}
}

void CGame::OnBnClickedDir0()
{
    m_iDir = 0;
    SetRegGame_RegistryData("Setting", "Dir", "0");
    GetDlgItem(IDC_DummyButton)->SetFocus();
}


void CGame::OnBnClickedDir1()
{
    m_iDir = 1;
    SetRegGame_RegistryData("Setting", "Dir", "1");
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::OnEnChangeGamemode5()
{
    
}

void CGame::LoadBlockData(int iPos, int iType) {

    for (int j = 0; j < 4; j++) m_iBlockType[iPos][j][2] = iType;

    // 공통 position
    m_iBlockType[iPos][0][0] = 0;
    m_iBlockType[iPos][0][1] = 0;
    m_iBlockType[iPos][1][0] = 0;
    m_iBlockType[iPos][1][1] = -1;

    switch (iType) {
    case 0:     // I
        m_iBlockType[iPos][2][0] = 0;
        m_iBlockType[iPos][2][1] = 1;
        m_iBlockType[iPos][3][0] = 0;
        m_iBlockType[iPos][3][1] = 2;
        break;

    case 1:     
        m_iBlockType[iPos][2][0] = 0;
        m_iBlockType[iPos][2][1] = 1;
        m_iBlockType[iPos][3][0] = 1;
        m_iBlockType[iPos][3][1] = -1;
        break;

    case 2:
        m_iBlockType[iPos][2][0] = 0;
        m_iBlockType[iPos][2][1] = 1;
        m_iBlockType[iPos][3][0] = -1;
        m_iBlockType[iPos][3][1] = -1;
        break;

    case 3:
        m_iBlockType[iPos][2][0] = -1;
        m_iBlockType[iPos][2][1] = 0;
        m_iBlockType[iPos][3][0] = -1;
        m_iBlockType[iPos][3][1] = 1;
        break;

    case 4:  // T
        m_iBlockType[iPos][2][0] = -1;
        m_iBlockType[iPos][2][1] = 0;
        m_iBlockType[iPos][3][0] = 1;
        m_iBlockType[iPos][3][1] = 0;
        break;

    case 5:
        m_iBlockType[iPos][2][0] = 1;
        m_iBlockType[iPos][2][1] = 0;
        m_iBlockType[iPos][3][0] = 1;
        m_iBlockType[iPos][3][1] = -1;
        break;

    case 6:
        m_iBlockType[iPos][2][0] = 1;
        m_iBlockType[iPos][2][1] = 0;
        m_iBlockType[iPos][3][0] = 1;
        m_iBlockType[iPos][3][1] = 1;
        break;
    }
   
}

void CGame::HoldBlock() {
    int iType = m_iBlockType[0][0][2];
    if (m_bIsHoldEmpty) {
        m_bIsHoldEmpty = FALSE;
        
        LoadBlockData(2, iType);
        DrawHoldBlock();
        Load_NewBlock();
    }
    else if (m_bChangeHoldBlock == 0) {        
        m_bChangeHoldBlock++;
        LoadBlockData(0, m_iBlockType[2][0][2]);
        LoadBlockData(2, iType);

        m_dwStartTime = GetTickCount();
        m_iBlockPositionX = GAME_COLUMN / 2;
        m_iBlockPositionY = 1;

        BOOL bContinue = TRUE;
        for (int i = 0; i < 4; i++) {
            if (m_iBlock[m_iBlockType[0][i][0] + m_iBlockPositionX][m_iBlockType[0][i][1] + m_iBlockPositionY] != MAX_BLOCK_TYPE) {
                bContinue = FALSE;
            }
        }

        // Draw Block
        DrawBlock();
        DrawHoldBlock();
    }
}

void CGame::DrawHoldBlock() {
    CClientDC dc(this);
    dc.Rectangle(HOLD_BLOCK_STARTX - 1, m_iNextBlock_StartY2 - 1, HOLD_BLOCK_STARTX + 3 * PIXEL_SIZE_BLOCK + 2, m_iNextBlock_StartY2 + 4 * PIXEL_SIZE_BLOCK + 2);
    if (!m_bIsHoldEmpty) {
        for (int i = 0; i < 4; i++) {
            dc.BitBlt(HOLD_BLOCK_STARTX + (m_iBlockType[2][i][0] + 1) * PIXEL_SIZE_BLOCK + 1 + m_iNextBlockCenterX[m_iBlockType[2][i][2]], m_iNextBlock_StartY2 + (m_iBlockType[2][i][1] + 1) * PIXEL_SIZE_BLOCK + 1 + m_iNextBlockCenterY[m_iBlockType[2][i][2]], PIXEL_SIZE_BLOCK, PIXEL_SIZE_BLOCK, &m_MemDC_Block[m_iBlockType[2][i][2]], 0, 0, SRCCOPY);
        }
    }
}

void CGame::UpdateScore() {
    CString str;
    if (m_iBestRecordScore < m_iGameScore) {
        m_iBestRecordLines = m_iLines;
        m_iBestRecordScore = m_iGameScore;
    }
    str.Format("Score: %s(%dLines) / Best: %s(%dLines)", (LPCTSTR)AddComma(m_iGameScore), m_iLines, (LPCTSTR)AddComma(m_iBestRecordScore), m_iBestRecordLines);
    SetDlgItemText(IDC_TotalScore, str);
}

void CGame::UpdateDropTime(int time) {
    CString str;
    str.Format("Drop:%dmsec   APM: -", time);
    SetDlgItemText(IDC_DropTime, str);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::UpdateDropTime() {
    CString str;
    str.Format("Drop:%dmsec   APM:%d", m_iDropTime, m_iAPM);
    SetDlgItemText(IDC_DropTime, str);
    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::OnBnClickedGhost() {
    if (IsDlgButtonChecked(IDC_Ghost)) {
        SetRegGame_RegistryData("Setting", "Show_Ghost", "1");
        m_bShowGhost = 1;
    }
    else {
        SetRegGame_RegistryData("Setting", "Show_Ghost", "0");
        m_bShowGhost = 0;
    }

    GetDlgItem(IDC_DummyButton)->SetFocus();
}

void CGame::DisplayCombo() {
  
    static int mode = 0, preMode = 0;
    static int showMode = 0;
    static int init = 1;
    static int comboNum[10][2] = { { 0,	42}, { 49,	29}, { 81,	38}, { 122,	37}, { 163,	40}, { 208,	38}, { 254,	35}, { 298,	37}, { 338,	37}, { 383,	35}, };

    int x0 = 400;
    int dx = 130;
    int y0 = 450;
    int dy = 72;      
    if (m_bTSpin) {
        CClientDC dc(this);

        if (init) {
            m_MemDC_Blank.BitBlt(0, 0, dx, dy, &dc, x0, y0, SRCCOPY);
            init = 0;
        }

        if (showMode == 0) {
            dc.BitBlt(x0, y0, dx, dy, &m_MemDC_Blank, 0, 0, SRCCOPY);
            showMode = 1;
        }
        else {
            dc.BitBlt(402, y0, 123, 39, &m_MemDC_Combo, 0, 31, SRCCOPY);
            showMode = 0;
        }
    }
    else {
        if (m_iComboCnt >= 3) mode = 1; else mode = 0;
        if (mode == 1 || preMode != mode) {
            CClientDC dc(this);

            if (init) {
                m_MemDC_Blank.BitBlt(0, 0, dx, dy, &dc, x0, y0, SRCCOPY);
                init = 0;
            }

            preMode = mode;
            if (mode == 0) {
                dc.BitBlt(x0, y0, dx, dy, &m_MemDC_Blank, 0, 0, SRCCOPY);
            }
            else {
                if (showMode == 0) {
                    dc.BitBlt(x0, y0, dx, dy, &m_MemDC_Blank, 0, 0, SRCCOPY);
                    showMode = 1;
                }
                else {
                    dc.BitBlt(402, y0, 123, 30, &m_MemDC_Combo, 0, 0, SRCCOPY);
                    int n = m_iComboCnt;
                    int w = 0;
                    while (n) {
                        w += comboNum[n % 10][1];
                        n /= 10;
                    }

                    int startx = x0 + dx - (dx - w) / 2;
                    n = m_iComboCnt;
                    while (n) {
                        int i = n % 10;
                        startx -= comboNum[i][1];
                        dc.BitBlt(startx, y0 + 32, comboNum[i][1], 38, &m_MemDC_NUM, comboNum[i][0], 0, SRCCOPY);
                        n /= 10;
                    }
                    showMode = 0;
                }
            }
        }
    }
}



void CGame::OnBnClickedRecord() { 
    CRecord dlg;
    dlg.m_pGame = this;
    dlg.DoModal();
}


void CGame::GetRecord() {
    CString strItem, strValue;
    for (int i = 0; i < N_RECORD; i++) {
        strItem.Format("Score_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].score = atoi(strValue);

        strItem.Format("Line_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].line = atoi(strValue);

        strItem.Format("Combo_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].combo = atoi(strValue);


        strItem.Format("T_spin%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].t_spin = atoi(strValue);

        strItem.Format("Perfect_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].perfect = atoi(strValue);


        strItem.Format("APM%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].apm = atoi(strValue);

        strItem.Format("Date_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].date = atoi(strValue);

        strItem.Format("time_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].time = atoi(strValue);

        strItem.Format("GameTime_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].gameTime = atoi(strValue);

        strItem.Format("Level_%d", i);
        strValue = GetRegGame_RegistryData("BestRecord", strItem);
        m_Record[i].lvl = atoi(strValue);
    }
}


void CGame::ModifyRecord(int iRank, RECORD record) {
    CString strItem, strValue;
    m_Record[iRank] = record;

    strItem.Format("Score_%d", iRank);
    strValue.Format("%d", m_Record[iRank].score);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("Line_%d", iRank);
    strValue.Format("%d", m_Record[iRank].line);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("Combo_%d", iRank);
    strValue.Format("%d", m_Record[iRank].combo);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("T_spin_%d", iRank);
    strValue.Format("%d", m_Record[iRank].t_spin);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("Perfect_%d", iRank);
    strValue.Format("%d", m_Record[iRank].perfect);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("APM%d", iRank);
    strValue.Format("%d", m_Record[iRank].apm);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);


    strItem.Format("Date_%d", iRank);
    strValue.Format("%d", m_Record[iRank].date);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("time_%d", iRank);
    strValue.Format("%d", m_Record[iRank].time);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("gameTime_%d", iRank);
    strValue.Format("%d", m_Record[iRank].gameTime);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);

    strItem.Format("Level_%d", iRank);
    strValue.Format("%d", m_Record[iRank].lvl);
    SetRegGame_RegistryData("BestRecord", strItem, strValue);
}


void CGame::CheckRecord(RECORD record) {
    for (int i = 0; i < N_RECORD; i++) {
        if (m_Record[i].score >= record.score) continue;

        for (int j = N_RECORD - 2; j >= i; j--) {
            ModifyRecord(j+1, m_Record[j]);
        }
        ModifyRecord(i, record);
        break;
    }
}

void CGame::OnCancel() {
    if (m_iGameMode == GAMEMODE_ON) {
        m_bPause = 1;
        if (AfxMessageBox("프로그램을 종료할까요 ?", MB_YESNO) == IDYES) {
            OnOK();
        }
    }
    else {
        OnOK();
    }
}


void CGame::OnBnClickedDummybutton() {
    // Default disabled
    // Only use for test
}

double m_segment[7][6][2];
void CGame::InitSegment(int Width, int Height, int Thick) {
	int Gap = 1;
	int t = Thick / 2 * 2;
	int L = Width - 2 * Gap - t;
	int hor[7][2] = { {0, t / 2}, {t / 2, 0}, {L - t / 2, 0}, {L, t / 2}, {L - t / 2, t}, {t / 2, t}, {0, t / 2} };
	int H = (Height - 4 - t) / 4 * 2;
	int ver[7][2] = { {0, t / 2}, {t / 2, 0}, {t, t / 2}, {t, H - t / 2}, {t / 2, H}, {0, H - t / 2}, {0, t / 2} };
	int offset[7][2] = { {t / 2 + 1, 0}, {2 + L, t / 2 + 1}, {2 + L, H + t / 2 + 3}, {t / 2 + 1, 2 * H + 4}, {0 , H + t / 2 + 3}, {0 , t / 2 + 1}, {t / 2 + 1, H + 2} };

	int type[7] = { 0, 1, 1, 0, 1, 1, 0 };

	for (int i=0; i<7; i++){
		if (type[i] == 0) {
			for (int j = 0; j < 7; j++) {
				for (int k = 0; k < 2; k++) {					
					m_segment[i][j][k] = offset[i][k] + hor[j][k];
				}
			}
		}
		else {
			for (int j = 0; j < 7; j++) {
				for (int k = 0; k < 2; k++) {
					m_segment[i][j][k] = offset[i][k] + ver[j][k];
				}
			}
		}
	};

	int UseSegment[10][7] = { 
		{1, 1, 1, 1, 1, 1, 0},    {0, 1, 1, 0, 0, 0, 0}, 
	    {1, 1, 0, 1, 1, 0, 1},    {1, 1, 1, 1, 0, 0, 1},
		{0, 1, 1, 0 ,0, 1, 1},    {1, 0, 1, 1, 0, 1, 1},
	    {1, 0, 1, 1, 1, 1, 1},    {1, 1, 1, 0, 0, 1, 0},    
	    {1, 1, 1, 1, 1, 1, 1},    {1, 1, 1, 0, 0, 1, 1}
	};

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 7; j++)
			m_UseSegment[i][j] = UseSegment[i][j];

}