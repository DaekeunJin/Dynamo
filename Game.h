
#pragma once
#include "SerialComDlg.h"

#define DEFAULT_REGISTRY_SETTING_GAME_PATH	"MY_TOOL\\Game\\Tetris\\"
#define GAME_TITLE                               "TETRIS v1.12  - 적당한 휴식이 필요할 때..."


#define TIMER_ID_SEND_BRICK					6

#define TimerGameTime                       201
#define TIME_SEND_BRICK						100

#define GAME_DROP_TIME_0                    1200
#define GAME_DROP_TIME_1                    1000
#define GAME_DROP_TIME_2                    800
#define GAME_DROP_TIME_3                    550

#define SOCKET_PORT							25031


#define GAME_ROW 20
#define GAME_COLUMN 9
#define GAME_VERSION  11

#define GAME_DRAW_START_X       15
#define GAME_DRAW_START_Y       40
#define PIXEL_SIZE_BLOCK		40

#define NEXT_BLOCK_STARTX       415

#define HOLD_BLOCK_STARTX       NEXT_BLOCK_STARTX

#define GAME_DRAW_START_X_PLAYER 561
#define GAME_DRAW_START_Y_PLAYER 130   
#define GAP_PLAYER_X			25
#define PIXEL_SIZE_BLOCK_PLAYER	16

#define PORT_SERVER_ACCEPT		27001
#define PORT_SERVER_READ_FRAME	27002

#define PORT_CLIENT_REQ_CONNECT	28001
#define PORT_CLIENT_READ_FRAME	28002

#define MAX_BLOCK_TYPE	7

#define GAME_PLAY_MODE_SINGLE   0
#define GAME_PLAY_MODE_MULTI    1

// CGame 대화 상자
#define MAX_CLIENT_COUNT 3

#define ROTATE_CW           0
#define ROTATE_CCW          1

#define N_RECORD 20


typedef struct {
    int score;
    int line;
    int combo;
    int apm;
    int t_spin;
    int perfect;
    int date;
    int time;
    int lvl;
    int gameTime;
} RECORD;


class CGame : public CDialogEx
{
	DECLARE_DYNAMIC(CGame)
public:
	typedef struct {
		BOOL is_available;
		SOCKET h_socket;
		CString ip_address;
		CString nickname;
		int iMode;
		char network_id;
		char Block[GAME_COLUMN][GAME_ROW];
	} ClientData;
	CSerialComDlg* pMain;

private:
	SOCKET mh_listen_socket;  // Client 접속을 받아주는 소켓
	int m_iClientCount;
	ClientData m_client[MAX_CLIENT_COUNT];
	SOCKET mh_ClientSocket;
	BOOL m_IsSurvival[MAX_CLIENT_COUNT];


public:
	CGame(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGame();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Game };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void Init_DrawGame();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Wait(DWORD dwMillisecond);

	void Init_Game();
    void Init_Setting();
    void Conver_PreSetting();
	void Load_NewBlock();
	void DrawBlock();	
    void DrawHoldBlock();
	void CheckLine();

	int m_iBlockType[4][4][3];
	char m_iBlock[GAME_COLUMN][GAME_ROW];

	CDC m_MemDC_Block[MAX_BLOCK_TYPE+2], m_MemDC, m_MemDC_Combo, m_MemDC_Blank, m_MemDC_NUM;
	CBitmap m_Bitmap_Block[MAX_BLOCK_TYPE + 2], m_Bitmap, m_Bitmap_Combo, m_Bitmap_Blank, m_Bitmap_NUM;
	CBitmap *m_pOldBitmap_Block[MAX_BLOCK_TYPE + 2], *m_pOldBitmap, *m_pOldBitmap_Combo, *m_pOldBitmap_Blank, *m_pOldBitmap_NUM;
	COLORREF m_ColorBlock[MAX_BLOCK_TYPE + 1];
	int m_iNextBlockCenterX[MAX_BLOCK_TYPE], m_iNextBlockCenterY[MAX_BLOCK_TYPE];
	CString m_strMessage;


	int m_iNextBlock_StartY;
    int m_iNextBlock_StartY2;
	int m_iGameMode;
    int m_iGameTime;
    int m_iTotalGameTime;
	enum {
		GAMEMODE_ON,
		GAMEMODE_PAUSE,
		GAMEMODE_END
	};
	DWORD m_dwStartTime;
	int m_iBlockPositionX, m_iBlockPositionY;
	BOOL m_bInitGame;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void CheckTimeout();
	afx_msg void OnBnClickedGamestart();

	void MoveBlockLeft();
	void MoveBlockRight();
	void MoveBlockDown();
    void RotateBlock(int mode);
	void DrawTime();
	void DropBlock();
	int m_iLines;
	int m_iBestRecordLines;
	int m_iDropTime;
	int m_iContineDropTime;
	CString GetRegGame_RegistryData(CString strAddress, CString strItem);
	void SetRegGame_RegistryData(CString strAddress, CString strItem, CString stData);

    CString GetRegGame_RegistryData_Pre(CString strAddress, CString strItem);

	CString m_strGameLevel;
	int m_iGameDifficulty;
	long m_iGameScore;
	long m_iBestRecordScore;
	
	CString AddComma(long iValue);
	CEdit m_ctrlGameScore;
	CFont m_FontScore;
	afx_msg void OnBnClickedGamemode0();
	afx_msg void OnBnClickedGamemode1();
	afx_msg void OnBnClickedGamemode2();
	int m_iCountBottomRowAdd;
	void AddBottomRow();

public:
	int m_iBlockShowType;
	BOOL m_bDrawGridLine;
	afx_msg void OnBnClickedShowgridline();
	int CheckMoveDown();
	int m_iTimeoutBottom;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	void AcceptProcess(SOCKET wParam_h_socket);
	void ClientCloseProcess(SOCKET wParam_h_socket, char param_force_flag);
	void ReceiveData(SOCKET param_h_socket, char *param_p_buffer, int param_size);
	void ServerFrameReadData(SOCKET h_socket);
	
	afx_msg void OnBnClickedClientconnectbtn();
	afx_msg void OnBnClickedServermakebtn();
	CListBox m_event_list;
	void AddEventString(CString param_str);
	int m_iCommMode;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSendMsg();
	void SendChatData_to_server(SOCKET param_h_socket, const CString& str, int parm_size);
	void SendChatData_to_clients(SOCKET param_h_socket, char sender_id, const CString& strParam, int parm_size);
	void ConnectProcess(LPARAM lParam);
	void DestroySocket();
	void ClientReadFrameData();
	void ReceiveData(char *param_p_buffer, int param_size);
	void GetIpAddress();
	void GetServerList();
	CString m_listIP[10];
	CString m_descriptIP[10];
	int m_countIP;
	int m_selectIP;
	int m_selectServerList;
	CString m_server_ip;
	CString m_server_ip_saved;
	BOOL m_req_save_server_ip;
	int m_iMyID;
	afx_msg void OnBnClickedCommmode();
	afx_msg void OnBnClickedCommmode2();
	int m_GamePlayerMode;
	afx_msg void OnBnClickedGameplayermode0();
	afx_msg void OnBnClickedGameplayermode1();
	
	void PlayerInit();
	void UpdatePlayer(int player);
	void SendCurrentBrick();
	void SendGameStart();
	void SendGameEnd();
	int m_iIncreaseRow;
	void Attack();
	void DrawGameStatus(int player, int status, CString strComment);
	int m_iTotalPlayer;
	int m_iGradePlayer;
	afx_msg void OnBnClickedMute();
	afx_msg void OnBnClickedChangedevice();
	void DrawGrade(int ID, int Grade);
    afx_msg void OnBnClickedGamemode3();
    
    int m_iDir;
    afx_msg void OnBnClickedDir0();
    afx_msg void OnBnClickedDir1();
    BOOL m_bPause;
    afx_msg void OnBnClickedGamemode4();
    afx_msg void OnEnChangeGamemode5();

    void HoldBlock();
    BOOL m_bIsHoldEmpty;
    void LoadBlockData(int pos, int type);
    int m_bChangeHoldBlock;
    int m_iComboCnt;
    int m_iT_SpinCnt;
    int m_iPerfectCnt;
    int m_iMaxComboCnt;
    BOOL m_bCheckCombo;

    void UpdateScore();
    void UpdateDropTime(int time);
    void UpdateDropTime();
    BOOL bBusyBlock;
    BOOL m_bShowGhost;

    afx_msg void OnBnClickedGhost();
    afx_msg void OnBnClickedDummybutton();

    void DisplayCombo();
    afx_msg void OnBnClickedRecord();
    RECORD m_Record[N_RECORD];
    void GetRecord();
    void ModifyRecord(int iRank, RECORD record);
    void CheckRecord(RECORD record);
    BOOL m_bTSpin;

    BOOL m_bReadyAPM;
    int m_iPreAPM_Key;
    int m_iAPM_count;
    int m_iAPM;

    void CheckVersion();
    afx_msg void OnCancel();
	int m_segment[7][7][2];
	int m_UseSegment[10][7];
	void InitSegment(int L, int H, int t);
};

