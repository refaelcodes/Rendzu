// Game.h : main header file for the GAME application
//

#if !defined(AFX_GAME_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
#define AFX_GAME_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "afxtempl.h"
#include <afxcoll.h>
#include <afxmt.h>
#include <math.h>
#include "const.h"
#include <memory>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
using namespace std;
#define WM_TITLE WM_USER + 300

class DMsgToUser;
class DTitleDlg;


////////////////////////////////////////////////////////////////////////
//  Programm using check

#define HID_CONTENTS 0x01


/////////////////////////////////////////////////////////////////////////////
// CGameApp:
// See Game.cpp for the implementation of this class
//
class CMainFrame;
class CGameView;
class CGameDoc;
class CGameApp;
class CGameData;
class CGameManager;
class CGameAccess;


struct CComponent
{
	CGameApp* pApp;
	CGameView* pView;
	CMainFrame* pFrame;
	CGameDoc* pDoc;

	CGameData* pGame;
	CGameManager* pManager;

	CComponent(){ pApp = 0; pView = 0; pFrame = 0;
	              pDoc = 0; pGame = 0; pManager = 0; }
};

class CGameApp : public CWinApp
{
public:
	
	
	CComponent Component;
	CGameApp();
	CString GetP();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGameApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAME_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
