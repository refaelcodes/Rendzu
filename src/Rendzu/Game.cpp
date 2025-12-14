// Game.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Game.h"

#include "MainFrm.h"
#include "GameDoc.h"
#include "GameView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CGameApp

BEGIN_MESSAGE_MAP(CGameApp, CWinApp)
	//{{AFX_MSG_MAP(CGameApp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameApp construction

CGameApp::CGameApp()
{
	Component.pApp = this;  
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGameApp object

CGameApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGameApp initialization

BOOL CGameApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

		// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGameDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGameView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	srand( (unsigned)time( NULL ) );

	return TRUE; 
}

CString CGameApp::GetP()
{
	CString Home;

	TCHAR exePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, exePath, MAX_PATH);

	// убираем имя exe → остаётся папка
	PathRemoveFileSpec(exePath);

	Home = exePath;
	return Home;
}
