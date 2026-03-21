// CHistory.cpp: 구현 파일
//

#include "stdafx.h"
#include "SerialCom.h"
#include "CHistory.h"
#include "afxdialogex.h"


// CHistory 대화 상자

IMPLEMENT_DYNAMIC(CHistory, CDialogEx)

void CHistory::OnBnClickedAutoscroll() {

}

//  수정해야할것  검색어 : buglist 버그리스트 버그 리스트 
// Save File loading시 buff full 되었을때 처리 방안 모색
// Capacity MAX_DATA_SIZE, m_iMaxMathSize세팅 변경 처리방안
/*
vector 크기할당    reverse(sz)로 사용 (초기값 적용안하고 오직 영역만 할당함)    
                   assign(sz)  사용시는 초기값 넣어주는 동작을 수행하므로, 늦음
*/


CHistory::CHistory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_History, pParent)
	, m_strHistory(_T(""))
{
	CString strHistory;
    strHistory = "";
	strHistory += "\r\n[10.04]\r\n";
	strHistory += " 1. 엔트키를 통한 분석/수집 모드 전환 반응성 개선\r\n";


    strHistory += "\r\n[10.03]\r\n";
    strHistory += " 1. Log Click시 Graph연동 적용\r\n";
    strHistory += " 2. 통계 및 Log Copy시 Index오류 수정\r\n";
    strHistory += " 3. 자동저장시 로그 저장안되는 오류 수정\r\n";

    strHistory += "\r\n[10.02]\r\n";
    strHistory += " 1. No Header Packet 처리\r\n";
    strHistory += " 2. Save File Loading속도 개선\r\n";

    strHistory += "\r\n[10.01]\r\n";
    strHistory += " 1. 설정 저장위치 및 폴더 정리 Ver2 적용 (기존 설정 변환하여 사용가능)\r\n";
    strHistory += "    (설정 > 우측하단에서 설정 변환하여 파일 저장후 등록하여 사용)\r\n";

	strHistory += "\r\n[10.00]\r\n";
	strHistory += " 1. Data, Math 크기 변경 적용\r\n";
	strHistory += " 2. Data, Math 표시 범위 마우스로 조절기능 추가\r\n";
	
	strHistory += "\r\n[9.98]\r\n";
	strHistory += " 1. Graph Log 기능 추가\r\n";
	strHistory += " 2. Graph Log Replay 추가\r\n";
	strHistory += " 3. 렌더링 부하증가시 Freezing현상 개선\r\n";

	strHistory += "\r\n[9.97]\r\n";
	strHistory += " 1. Text Log 기능 Update\r\n";

	strHistory += "\r\n[9.96]\r\n";
	strHistory += " 1. 설정 복사기능추가 : Header, Mode선택  \r\n";
	strHistory += " 2. Screen Fit max size설정 추가 \r\n";

	strHistory += "\r\n[9.95]\r\n";
	strHistory += " 1. 좌측 메뉴 표시:  체크 박스로 변경 \r\n";
	strHistory += " 2. 모드:  라디오 버튼 --> 콤보 박스로 변경\r\n";

	strHistory += "\r\n[9.93.00]\r\n";
	strHistory += " 1. Math Probe 정보 복사 기능 추가\r\n";

	strHistory += "\r\n[9.92\r\n";
	strHistory += " 1. 전체 설정 파일로 저장 및 초기화 기능 추가(설정 메뉴에서)\r\n";
	strHistory += " 2. 자동 전체 Data보기 기능 추가\r\n";

	strHistory += "\r\n[9.91]\r\n";
	strHistory += " 1. Probe value 표시 옵션 추가 \r\n";
	strHistory += " 2. 로그 멀티 선택 불가 오류 개선 \r\n";

	strHistory += "\r\n[9.90]\r\n";
	strHistory += " 1. Header변경후 reLoading 기능 추가 \r\n";

	strHistory += "\r\n[9.89]\r\n";
	strHistory += " 1. Log 연동 bug fix\r\n";
	strHistory += " 2. 화면 표시 Data 변경 버튼 동작시 Timeline깨짐 개선\r\n";

	strHistory += "\r\n[9.88.001]\r\n";
	strHistory += " 1. Timeline check bar dbl-click시 전체 Data 화면 맞게 표시\r\n";
	strHistory += " 2. Graph창 수직 크기 변경시 옵션 추가(L버튼 UP하여 크기 적용시 shift 누르면 위쪽 기준,  Ctrl누르면 아래 기준 적용)\r\n";

	strHistory += "\r\n[9.88]\r\n";
	strHistory += " 1. Probe의 값표시 기능 강화\r\n";
	strHistory += " 2. 마우스 왼쪽 버튼 클릭후 Drag기능 변경: Drag시 Data변경,  ctrl키 누른채는 화면 Move됨\r\n";

	strHistory += "\r\n[9.87]\r\n";
	strHistory += " 1. Data일부 저장기능 추가( CopyData 버튼 )\r\n";
	strHistory += "    Data정보만 저장,  주석 및 설정 정보 저장안함\r\n";

	strHistory += "\r\n[9.86]\r\n";
	strHistory += " 1. 색상 미세선택 기능 개선\r\n";
	strHistory += " 2. 분석모드에서 Mouse L버튼 drag로 shift 기능 추가\r\n";

	strHistory += "\r\n[9.85]\r\n";
	strHistory += " 1. 배경색상 변경 기능 추가\r\n";
	strHistory += " 2. Testri Game 기능 추가\r\n";

	strHistory += "\r\n[9.84]\r\n";
	strHistory += " 1. Data수신시 Log cursor 이동 안되는 문제 개선\r\n";
	strHistory += " 2. Time tick 간격 설정 및 Display 개선\r\n";

	strHistory += "\r\n[9.83]\r\n";
	strHistory += " 1. multi selected log data copy & paste 기능 추가 \r\n";

	strHistory += "\r\n[9.82]\r\n";
	strHistory += " 1. 분석모드에서도 data 저장 기능 옵션 추가\r\n";

	strHistory += "\r\n[9.81]\r\n";
	strHistory += " 1. 일정 시간마다 자동 저장 기능추가\r\n";
	strHistory += " 2. 저장 data 폴더 지정\r\n";

	strHistory += "\r\n[9.80]\r\n";
	strHistory += " 1. Navi Log 변환 옵션 추가 및 안정화\r\n";
	
	strHistory += "\r\n[9.79]\r\n";
	strHistory += " 1. 영역 변경위치에서 커서 변경 및 설정 추가\r\n";
	strHistory += " 2. Navi Log 변환기능 추가\r\n";

	strHistory += "\r\n[9.78]\r\n";
	strHistory += " 1. Header변경시 마지막으로 사용한 Mode로 시작\r\n";

	strHistory += "\r\n[9.77]\r\n";
	strHistory += " 1. Graph선택 Control 색상 변경\r\n";

	strHistory += "\r\n[9.76]\r\n";
	strHistory += " 1. Navi저장 MCU분석 기능 추가\r\n";

	strHistory += "\r\n[9.75]\r\n";
	strHistory += " 1. Data Control Window 크기 조절에 따른 화면 배치변경\r\n";

	strHistory += "\r\n[9.74]\r\n";
	strHistory += " 1. Data Control Window 크기 조절 기능 추가\r\n";

	strHistory += "\r\n[9.73]\r\n";
	strHistory += " 1. Data Control View On/Off 기능 추가\r\n";

	strHistory += "\r\n[9.72]\r\n";
	strHistory += " 1. 수평 Log창 크기조정기능 추가\r\n";
	strHistory += " 2. Auto Header전환기능 개선\r\n";

	strHistory += "\r\n[9.71]\r\n";
	strHistory += " 1. Log 분석기능 제공\r\n";

	strHistory += "\r\n[9.70]\r\n";
	strHistory += " 1. Bluetooth 접속위한 대화창 추가\r\n";
	strHistory += " 2. 화면캡쳐 버튼 추가\r\n";

	strHistory += "\r\n[9.69]\r\n";
	strHistory += " 1. Header 자동 변환기능 추가\r\n";

	strHistory += "\r\n[9.68]\r\n";
	strHistory += " 1. Preset 저장갯수 5→10개로 증가\r\n";

	strHistory += "\r\n[9.67]\r\n";
	strHistory += " 1. 기본 입력 data Format double 형으로 변경 (소수점 정보 수신 가능)\r\n";

	strHistory += "\r\n[9.66]\r\n";
	strHistory += " 1. 같은 위치에 Cursor 추가 안됨\r\n";

	strHistory += "\r\n[9.65]\r\n";
	strHistory += " 1. Data저장시 설정정보 저장\r\n";

	strHistory += "\r\n[9.64]\r\n";
	strHistory += " 1. line 속성 : Mark 사용 유무 설정 추가\r\n";

	strHistory += "\r\n[9.63]\r\n";
	strHistory += " 1. Math Probe 갯수 증가 : 16 → 32\r\n";

	strHistory += "\r\n[9.62]\r\n";
	strHistory += " 1. Cursor간 시간 정보 표시\r\n";
	strHistory += " 2. Math  Constant 추가\r\n";
	strHistory += " 3. Math  결과 Data형 double로 변경\r\n";

	strHistory += "\r\n[9.61]\r\n";
	strHistory += " 1. Cursor추가 버튼 추가\r\n";

	strHistory += "\r\n[9.60]\r\n";
	strHistory += " 1. Cursor 기능추가 (nMax : 9)\r\n";
	strHistory += "    추가:Graph영역 좌측 클릭, 이동:Drag&+\r\n";

	strHistory += "\r\n[9.59]\r\n";
	strHistory += " 1. 선택된 data 값 표시 기능 추가\r\n";

	strHistory += "\r\n[9.58]\r\n";
	strHistory += " 1. Math 연산자 추가 : Divide (/) 추가 \r\n";

	strHistory += "\r\n[9.57]\r\n";
	strHistory += " 1. SaveFile Loading시 Freeze개선\r\n";

	strHistory += "\r\n[9.56]\r\n";
	strHistory += " 1. Math 연산자 : ABS 추가\r\n";
	strHistory += " 2. 통계 분석창 크기 변경\r\n";

	strHistory += "\r\n[9.55]\r\n";
	strHistory += " 1. 화면표시 갯수 변경 버튼 추가 \r\n";

	strHistory += "\r\n[9.53]\r\n";
	strHistory += " 1. Tetris Bug 수정\r\n";

	strHistory += "\r\n[9.52]\r\n";
	strHistory += " 1. Tetris BGM 추가\r\n";

	strHistory += "\r\n[9.51]\r\n";
	strHistory += " 1. Auto Save 적용여부 선택 추가\r\n";
	strHistory += " 2. Tetris Game 추가\r\n";

	strHistory += "\r\n[9.50]\r\n";
	strHistory += " 1. 통계 Data Clipboard저장 기능 추가\r\n";

	strHistory += "\r\n[9.49]\r\n";
	strHistory += " 1. 메뉴 On/Off 기능 추가\r\n";
	strHistory += " 2. Mode Change 저장 파일에 반영\r\n";

	strHistory += "\r\n[9.48]\r\n";
	strHistory += " 1. 윈도우 밖에서 Mouse Button Up처리 추가\r\n";
	strHistory += " 2. Port 선택시 Tool 이상 개선\r\n";

	strHistory += "\r\n[9.47]\r\n";
	strHistory += " 1. Graph영역 Double Click시 전체확대/축소 추가\r\n";

	strHistory += "\r\n[9.46]\r\n";
	strHistory += " 1. Timeline Control 기능 추가\r\n";

	strHistory += "\r\n[9.45]\r\n";
	strHistory += " 1. Motor 수신data Robot A/B 구분 표시\r\n";

	strHistory += "\r\n[9.44]\r\n";
	strHistory += " 1. 버그 Fix\r\n";

	strHistory += "\r\n[9.43]\r\n";
	strHistory += " 1. 모터 data최소 data 갯수 설정 추가\r\n";

	strHistory += "\r\n[9.42]\r\n";
	strHistory += " 1. 모터 분석기능 통합\r\n";

	strHistory += "\r\n[9.40~41]\r\n";
	strHistory += " 1. Header변경시 설정 data 누락 등 오류 개선\r\n";

	strHistory += "\r\n[9.39]\r\n";
	strHistory += " 1. 편의사항 추가 : Min-Max수정시 Auto 자동 off\r\n";
	strHistory += " 2. Graph갯수 조정 Combobox 삭제 \r\n";

	strHistory += "\r\n[9.38]\r\n";
	strHistory += " 1. 선택 항목 없는 그래프 제거\r\n";

	strHistory += "\r\n[9.37]\r\n";
	strHistory += " 1. 분석모드에서 Log창 또는 그래프 click후 '^ + C' 실행시 클립보드로 복사 기능 추가  \r\n";

	strHistory += "\r\n[9.36]\r\n";
	strHistory += " 1. 시간 표시 주기 균등하게 개선 \r\n";

	strHistory += "\r\n[9.35]\r\n";
	strHistory += " 1. FFT 결과 오류 수정 및 선택영역 화면 표시 기능 개선 \r\n";
	strHistory += " 2. 창 크기 자동 조절기능 (Frame doubleclick @분석모드)\r\n";

	strHistory += "\r\n[9.34]\r\n";
	strHistory += " 1. Grid 갯수 조정 및 저장 기능 추가(@세부설정 대화창)\r\n";
	strHistory += " 2. FFT 분석 기능 추가 (통계 > FFT) \r\n";


	strHistory += "\r\n[9.33]\r\n";
	strHistory += " 1. Line Property(색상,굵기,타입) 설정 변경 대화창 추가\r\n";
	strHistory += " 2. Math 갯수 8→16 증가 & 보기 갯수 변경 추가\r\n";
	strHistory += " 3. Log / Filter 창 크기 변경 추가\r\n";

	strHistory += "\r\n[9.32]\r\n";
	strHistory += " 1. Pen Style 및 굵기 조절 기능 추가(세부 설정 메뉴)\r\n";
	strHistory += " 2. Math Pin Name 설정 메뉴 추가\r\n";

	strHistory += "\r\n[9.31]\r\n";
	strHistory += " 1. Math 연산지 추가 : 변화량(delta)\r\n";
	strHistory += " 2. m-Mouse button으로 Position Graph 영역 선택후 확대 보기 추가\r\n";
	strHistory += " 3. Position Graph에 Auto scale 버튼 추가\r\n";

	strHistory += "\r\n[9.30]\r\n";
	strHistory += " 1. Mouse wheel 미세조정 기능 추가 : shift + Mouse wheel\r\n";
	strHistory += " 2. Math 나머지 연산지 추가 : % \r\n";

	strHistory += "\r\n[9.29]\r\n";
	strHistory += " 1. Positon 좌표 입력 변수 : Math data 추가\r\n";
	strHistory += " 2. Math 비교 연산자 추가 : =, >, < \r\n";

	strHistory += "\r\n[9.28]\r\n";
	strHistory += " 1. Positon 좌표 출력 기능 추가\r\n";
	strHistory += " 2. Frame 크기 저장 기능 추가\r\n";

	strHistory += "\r\n[9.26]\r\n";
	strHistory += " 1. Math Data 통계량 추가 & bug fix\r\n";

	strHistory += "\r\n[9.25]\r\n";
	strHistory += " 1. Math 계산 추가 & bug fix\r\n";

	strHistory += "\r\n[9.24]\r\n";
	strHistory += " 1. Data 통계값 계산 범위 선정 편의 증대\r\n";

	strHistory += "\r\n[9.21]\r\n";
	strHistory += " 1. Data 통계값 계산 기능 추가 \r\n";

	strHistory += "\r\n[9.20]\r\n";
	strHistory += " 1. Save data Loading시 16번째 data 누락 오류 개선 by JS Park \r\n";

	strHistory += "\r\n[9.19]\r\n";
	strHistory += " 1. Filter Log 선택시 Gragh 위치 오류 개선 \r\n";

	strHistory += "\r\n[9.18]\r\n";
	strHistory += " 1. 최대 지원 해상도 증가: 5120 x 3200 \r\n";

	strHistory += "\r\n[9.17]\r\n";
	strHistory += " 1. Model 추가 & Pin Name 복사기능 추가\r\n";
	strHistory += " 2. 화면 깜빡임 개선\r\n";

	strHistory += "\r\n[9.16]\r\n";
	strHistory += " 1. Filter 변경시 List Update기능 추가\r\n";
	strHistory += " 2. Filter 표시값 검색어 이후에서 탐색\r\n";

	strHistory += "\r\n[9.15]\r\n";
	strHistory += " 1. 가로 방향 Grid 추가\r\n";
	strHistory += " 2. Robot1/2 Graph 분리\r\n";

	strHistory += "\r\n[9.14]\r\n";
	strHistory += " 1. Wheel Scroll중 커서 이동\r\n";
	strHistory += " 2. ListBox Font 크기 설정 추가\r\n";
	strHistory += " 3. File Reading 진행상황 표시\r\n";

	strHistory += "\r\n[9.13]\r\n";
	strHistory += " 1. Time표시 오류 수정 및 라인 중앙에 Text배치\r\n";
	strHistory += " 2. History기능 추가\r\n";
	strHistory += " 3. Bit정보 Auto Close기능 추가\r\n";

	strHistory += "\r\n[9.12]\r\n";
	strHistory += " 1. Graph갯수 변경:1~8\r\n";
	strHistory += " 2. Index 정밀도 개선\r\n";
	m_strHistory.Format("%s", (LPCTSTR)strHistory);
}

CHistory::~CHistory()
{
}

void CHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_History, m_EditHistory);
	DDX_Text(pDX, IDC_History, m_strHistory);
}


BEGIN_MESSAGE_MAP(CHistory, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Autoscroll, &CHistory::OnBnClickedAutoscroll)
END_MESSAGE_MAP()


// CHistory 메시지 처리기


void CHistory::OnTimer(UINT_PTR nIDEvent)
{

	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent) {
	case 1:
		if (IsDlgButtonChecked(IDC_Autoscroll)) { m_EditHistory.LineScroll(1); }
		break;
	}
}


BOOL CHistory::OnInitDialog() {
	CDialogEx::OnInitDialog();
	UDF;
	CheckDlgButton(IDC_Autoscroll, TRUE);
	m_EditHistory.SetFocus();
	UDT;
	SetTimer(1, 1200, NULL);	
	return TRUE;
}

