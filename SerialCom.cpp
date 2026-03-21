// SerialCom.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SerialCom.h"
#include "SerialComDlg.h"
#include "Game.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerialComApp

BEGIN_MESSAGE_MAP(CSerialComApp, CWinApp)
	//{{AFX_MSG_MAP(CSerialComApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialComApp construction

CSerialComApp::CSerialComApp() {
	// Place all significant initialization in InitInstance
	ProcessKill();  // 이중 실행 방지
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSerialComApp object

CSerialComApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSerialComApp initialization

BOOL CSerialComApp::InitInstance()
{
	//CSerialComDlg dlg;
    auto dlg = std::make_unique<CSerialComDlg>();
	m_pMainWnd = dlg.get();
	dlg->pAppMain = this;    
    dlg->m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 기존 도움말 경로 메모리 해제 및 새 CHM 파일 경로 설정
	// 실행 파일(exe)과 같은 폴더에 'Help.chm'이 있다고 가정합니다.
	// 1. 리소스 찾기 및 로드
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_CHM_HELP),"CHM_DATA");
	if (hRes)
	{
		HGLOBAL hData = LoadResource(NULL, hRes);
		DWORD dwSize = SizeofResource(NULL, hRes);
		void* pData = LockResource(hData);

		// 2. 임시 폴더 경로 구하기
		TCHAR szTempPath[MAX_PATH];
		GetTempPath(MAX_PATH, szTempPath);

		// 3. 임시 파일 경로 생성 (예: C:\Users\...\AppData\Local\Temp\SerialComHelp.chm)
		CString strHelpPath;
		strHelpPath.Format("%sTestDataAnalyzerTool_Help.chm", szTempPath);

		// 4. 파일로 저장 (이미 존재하면 덮어쓰기)
		CFile file;
		if (file.Open(strHelpPath, CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(pData, dwSize);
			file.Close();

			// 5. 앱의 도움말 경로를 이 임시 파일로 설정
			if (m_pszHelpFilePath != NULL)
				free((void*)m_pszHelpFilePath);
			m_pszHelpFilePath = _tcsdup(strHelpPath);
		}
	}
	if (m_pszHelpFilePath != NULL)
		free((void*)m_pszHelpFilePath);

	dlg->DoModal();
	
	return FALSE;
}


BOOL CSerialComApp::ProcessKill()
{
	CString strProcessName;
	HANDLE hProcessSnap = NULL;
	BOOL bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };

	strProcessName.Format("%s", (LPCTSTR)GetCurrentProcessModule());

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1) return false;
	pe32.dwSize = sizeof(PROCESSENTRY32); //프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다 
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL bCurrent = FALSE;
		MODULEENTRY32 me32 = { 0 };
        BOOL bForceDelete = FALSE;
		do
		{
			bCurrent = GetProcessModule(pe32.th32ProcessID, strProcessName);

			if (pe32.th32ProcessID == ::GetCurrentProcessId()) { continue; }
			if (bCurrent) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if (bForceDelete || (hProcess && AfxMessageBox("The application is already running.\r\nWould you like to close the existing instance?", MB_YESNO) == IDYES)) {
                    bForceDelete = TRUE;
					if (TerminateProcess(hProcess, 0)) {
						unsigned long nCode; //프로세스 종료 상태 
						GetExitCodeProcess(hProcess, &nCode);
					} 
					CloseHandle(hProcess);
				}
			}
		} //다음 프로세스의 정보를 구하여 있으면 루프를 돈다. 
		while (Process32Next(hProcessSnap, &pe32));
	} CloseHandle(hProcessSnap);
	return true;
}

CString CSerialComApp::GetCurrentProcessModule(void)
{
	CString strProcessName;
	HANDLE hProcessSnap = NULL;
	BOOL bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1) return "";
	pe32.dwSize = sizeof(PROCESSENTRY32); //프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다 

	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL bCurrent = FALSE;
		MODULEENTRY32 me32 = { 0 };
		do
		{
			if (pe32.th32ProcessID == ::GetCurrentProcessId()) { strProcessName.Format("%s", pe32.szExeFile); return strProcessName; }
		} while (Process32Next(hProcessSnap, &pe32));
	} CloseHandle(hProcessSnap);

	return "";
}

BOOL CSerialComApp::GetProcessModule(DWORD dwPID, CString sProcessName)
{
	HANDLE hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == (HANDLE)-1) return (FALSE);
	me32.dwSize = sizeof(MODULEENTRY32);

	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면 true를 리턴한다. 
	if (Module32First(hModuleSnap, &me32))
	{
		do
		{
			if (me32.szModule == sProcessName)
			{
				CloseHandle(hModuleSnap);
				return true;
			}
		} while (Module32Next(hModuleSnap, &me32));
	}
	CloseHandle(hModuleSnap);
	return false;
}

int CSerialComApp::ExitInstance()
{
	// 설정된 도움말 경로에 파일이 존재하면 삭제
	if (m_pszHelpFilePath != NULL)
	{
		DeleteFile(m_pszHelpFilePath);
	}

	return CWinApp::ExitInstance();
}