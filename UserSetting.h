#pragma once
#include "GraphLogItem.h"
#include "ColorSelectDlg.h"
#include <string.h>
#include <windows.h>

#define CONFIGURATION_VERSION           "1.00"
#define DEFAULT_REGISTRY_SETTING_PATH	"MY_TOOL\\DynamoController\\"
#define TITLE							"Motor Dynamo Ver1.00"

#define UDT                             UpdateData(TRUE)
#define UDF                             UpdateData(FALSE)

#define MAX_DATACONTROL_SIZE		    16
#define MAX_MATHCONTROL_SIZE            16
#define MAX_BIT_CONFIG_SIZE		        16

#define N_SHOW_DATA_CONTROL				6

#define N_MAX_DATA_PAGE					4
#define N_MAX_MATH_PAGE					4


#define MAXIMUM_DATA_SIZE				(MAX_DATACONTROL_SIZE * N_MAX_DATA_PAGE)
#define MAXIMUM_MATH_SIZE				(MAX_MATHCONTROL_SIZE * N_MAX_MATH_PAGE)

#define MAX_TESTDATA_SIZE	            (2000000)
#define MIN_TESTDATA_SIZE	            (100000)
#define DEFAULT_TESTDATA_SIZE	        (500000)

#define MAX_COM_PORT_SIZE	            100

#define MAX_FILTER		                12
#define MAX_BT_COMMAND	                20

#define MAX_GRAPH_COUNT 	            8

#define GRAPH_GAP_Y	                    4	    //30
#define GRAPH_GAP_X	                    6
#define GRAPH_MIN_HEIGHT                30

#define GRAPH_BOTTOM_MARGIN	            35		//35

#define GRAPH_RANGE_Y	                110		// Mouse Control start Point
#define GRAPH_START_Y	                (GRAPH_RANGE_Y+20)	

#define GRAPH_TIMELINE_Y0               (GRAPH_RANGE_Y + GRAPH_GAP_Y)
#define GRAPH_TIMELINE_Y1               (GRAPH_START_Y -GRAPH_GAP_Y)

#define CHANGE_FRAME_MIN_LEGEND_SIZE	200
#define CHANGE_FRAME_MIN_GRAPH_SIZE		200

#define DATA_ACQUISITION_MODE			1
#define DATA_ANALYZE_MODE				0

#define TIMER_GRAPH_UPDATE				1
#define TIMER_LOAD_DEFALUT_DISPLAY_SIZE	2
#define TIMER_AUTOCLOSE_MESSAGEBOX		3
#define TIMER_CHECK_STATISTICS_DLG		4

#define TIMER_CONFIG_UDF				200
#define TimerGameTime                   201
#define TIMER_REQ_CHANGE_DATASIZE		301
#define TIMER_GRAPHLOG_UPDATE			302
#define TIMER_WHEEL_ACC_CHECK			303
#define TIMER_XINPUT_CHECK				400
#define TIMER_DLG_INIT					401
#define TIME_SEND_BRICK					100
#define TIMER_UPDATE_LOG				402

#define GAME_DROP_TIME_0                1200
#define GAME_DROP_TIME_1                1000
#define GAME_DROP_TIME_2                800
#define GAME_DROP_TIME_3                550

#define MAX_OPERATOR_COUNT		        Operator_Last  // 
#define MAX_SIMPLE_OPERATOR		        Operator_Average
#define MAX_MATH_DATA			        10000

#define H_LABEL_TEXT	                12
#define W_LABEL_TEXT	                6

#define MAX_HEADER_LOG_COUNT		    500

#define MIN_RANGE_LOGFILTER_H			421
#define MAX_RANGE_LOGFILTER_H			1920
#define MIN_RANGE_GRAPH_H				502

#define DRAW_CONTROL_START_Y			(GRAPH_RANGE_Y + 5)		// 50  -> 150
#define DRAW_CONTROL_DY					18
#define DRAW_DATA_CONTROL_Y0			(34 + DRAW_CONTROL_START_Y)

#define DRAW_CONTROL_GAP_Y				4

#define SOCKET_PORT						25031

#define N_MAX_Control	                10
#define N_MAX_Preset	                10
#define MAX_COM_PORT_SIZE	            100

#define MIN_DISPLAY_SIZE	            4
#define N_MAX_GRAPHLOG_ITEM		        200

#define KEYCODE			                0
#define GLOG_TOOL_VERSION	            115

// Font size setting
#define MAX_FONT_SIZE                   2

//RANDOM_INDEX
#define N_RAND_INTERVAL				1000


//FFT
#define M_PI                            3.141592654
#define M_PI2                           6.283185308

#define N_MAX_FFT                       7000

#define COLOR_WHITE		                RGB(255, 255, 255)
#define COLOR_BLACK		                RGB(0, 0, 0)
#define COLOR_YELLOW	                RGB(255, 255, 0)
#define COLOR_ORANGE	                RGB(255, 192, 0)
#define COLOR_GRAY		                RGB(128, 128, 128)
#define COLOR_BLUE		                RGB(0, 0, 255)

#define BRUSH_WHITE		                (m_Brush[0])
#define BRUSH_BLACK		                (m_Brush[1])
#define BRUSH_YELLOW	                (m_Brush[2])
#define BRUSH_ORANGE	                (m_Brush[3])
#define BRUSH_GRAY		                (m_Brush[4])
#define BRUSH_BLUE		                (m_Brush[5])

#define MAX_CURSOR		                10
#define N_CONVERT_REG					(16)

#define MIN_LOG_UPDATE_PERIOD           500
#define MAX_LOG_UPDATE_PERIOD           10000


enum {
    Operator_Add,
    Operator_Substract,
    Operator_Multiply,
    Operator_Divide,
    Operator_Exponential,
    Operator_EqualTo,
    Operator_GreaterThan,
    Operator_LessThan,
    Operator_Remainder,
    Operator_Average,
    Operator_RMS,
    Operator_Stdev,
    Operator_Change,
    Operator_ABS,
    Operator_Last,
};

enum {
    eLogCondition_Blank = 0,
    eLogCondition_MatchMask = 1,
    eLogCondition_ShowValue = 2,
    eLogCondition_MaskedValue = 3,
	eLogCondition_Greater = 4,
	eLogCondition_GreaterEqual = 5,
	eLogCondition_Less = 6,
	eLogCondition_LessEqual = 7,
    eLogCondition_Last = 8
};

typedef struct {
    BOOL bDisplayGraph[MAX_GRAPH_COUNT]{ 0 };
	BOOL bSelectGraph[MAX_GRAPH_COUNT]{ 0 };
	BOOL bValid{ 0 };  // bDraw = bUse && ValidSetting 
	BOOL bUserName{ 0 };
	BOOL bUseMark{ 0 };
    BOOL bResInt{ 0 };
	int iDataID1{ 0 };
	int iDataID2{ 0 };
	int iOperator{ 0 };
	double fValue{ 0 };
	CString strName{ "" };
	double  iValue{ 0. };
	int nData{ 0 };
	int iPenStyle{ 0 };
	int iPenThickness{ 0 };
	int iColor[3]{ 0 };

	int index{ 0 };
	double fSaveData[MAX_MATH_DATA]{ 0. };
	double fSum{ 0. };
} MathData_t;

typedef struct {
    int iComPort{ 0 };
    CString strPortID{ "" };
    CString strDescription{ "" };
} ComPortList_t;

typedef struct {
    BYTE cHeader{ 0 };
    CString strRemark{ "" };
    int ItemCount{ 0 };
	BOOL bUse[MAX_HEADER_LOG_COUNT]{ 0 };
	short iCondition[MAX_HEADER_LOG_COUNT]{ 0 };
	short iDataType[MAX_HEADER_LOG_COUNT]{ 0 };
	short iID[MAX_HEADER_LOG_COUNT]{ 0 };
	unsigned int iMaskValue[MAX_HEADER_LOG_COUNT]{ 0 };
	unsigned int iRefValue[MAX_HEADER_LOG_COUNT]{ 0 };
	BOOL bShow1[MAX_HEADER_LOG_COUNT]{ 0 };
	CString strComment1[MAX_HEADER_LOG_COUNT]{ "" };
	BOOL bShow2[MAX_HEADER_LOG_COUNT]{ 0 };
	CString strComment2[MAX_HEADER_LOG_COUNT]{ "" };
} HeaderLogData_t;


typedef struct {
    BYTE Header{ 0 };
    int nData{ 0 };
    double DataValue[10000][MAXIMUM_DATA_SIZE]{ 0 };
    double MathValue[10000][MAXIMUM_MATH_SIZE]{ 0 };
    BOOL bUseMath[MAXIMUM_MATH_SIZE]{FALSE };
    CString strDataName[MAXIMUM_DATA_SIZE]{ "" };
    CString strMathName[MAXIMUM_MATH_SIZE]{ "" };
    CString strRemark{ "" };
    CString strTime{ "" };
} RefData_t;

#define N_MAX_INDICATOR 30
struct CIndicator {
    CPoint pt{ 0 };
    CString str{ "" };
    COLORREF text{ 0 };
    COLORREF bk{ 0 };
    int margin{ 0 };
    int sumMargin{ 0 };
    double val{ 0. };

	CIndicator() {
		pt = { 0,0 };
		str = "";
		text = bk = 0;
		sumMargin = margin = 0;
		val={ 0. };		
	}
	CIndicator(const CIndicator& other) :pt{ other.pt }, str{ other.str }, text{ other.text }, bk{ other.bk }, margin{ other.margin }, sumMargin{ other.sumMargin }, val{ other.val } {}
	CIndicator& operator= (const CIndicator& other) {
		if (this != &other) {
			pt = other.pt;
			str = other.str;
			text = other.text;
			bk = other.bk;
			margin = other.margin;
			sumMargin = other.sumMargin;
			val = other.val;
		}
		return *this;
	}
};

typedef struct{
    int iSendType{0};
    BOOL bUseHeader{FALSE};
    int nHeader{0};
    BYTE header[8]{0};
    
    BOOL bUseEnableKey{FALSE};
    int iEnableKeyMode{0};
	BOOL bUseLB_EnableKey{ FALSE };
	BOOL bUseRB_EnableKey{ FALSE };
    int iLT_Threshold_EnableKey{ 0 };
    int iRT_Threshold_EnableKey{ 0 };
	
	//Send Data	for Send string
    BOOL bUseSendData[8]{ FALSE };
    CString strSendData[8]{ "" };
    CString strSendData_negative[4]{ "" };
    int iSendThreshold[4]{ 0 };

	//Send Data	for Send Packet
    int iUpdatePeriod{ 0 };
    BOOL bUseSendButton{ FALSE };
    BOOL bUseChecksum{ FALSE };
    BOOL bShowUartSend{ FALSE };
} XINPUT_SETTING_T;

typedef struct {
	double fTorque;
	double fPower;
	int iRpm;
} TORQUE_DATA_T;


typedef struct {
	DWORD tick_send;
	int id;
	bool req_ack;
	int status;
	double volt[2];     // 0: output  1: setting
	double current[2];  // 0: output  1: setting
	char send_buf[20];
	int send_size;
	bool bIsSetting{ false };
} LOAD_PACKET_T;

typedef enum {
	PS_REQ_LOCK = 0,
	PS_REQ_CUR_SET = 1,
	PS_GET_CUR_SET = 2,
	PS_REQ_VOLT_SET = 3,
	PS_GET_VOLT_SET = 4,
	PS_GET_CUR_OUT = 5,
	PS_GET_VOLT_OUT = 6,
	PS_REQ_ONOFF = 7,
	PS_REQ_SET_OVER_CUR = 8,
	PS_REQ_SET_OVER_VOLT = 9,
	PS_REQ_SET_OV_ENABLE = 10,
	PS_REQ_SET_OC_ENABLE = 11,
	PS_REQ_SAVESETTING = 12,
	PS_REQ_RESTORE = 13,
	PS_GET_INFO = 14,
	PS_REQ_SET_BEEP = 15,
	PS_REQ_SET_MODE  =16,
	PS_GET_STATUS = 17,
} POWERSUPLY_CMD_e;


typedef struct {	
	int p_gain{30};
	double i_gain{ 0.3f };
	int iMinSpeed_control { 10};
	bool useOverCurProtection{false};
	double overCurrentLevel{ 9.9f };
	bool useOverVoltProtection{ true };
	int overVoltageLevel{ 1000 };
	bool useBeep;
	bool useMinSpeed{ TRUE };
} Load_Setting_t;