// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Game.h"
#include "GameDoc.h"
#include "GameView.h"
#include "SettingDlg.h"
#include "SettingDlg.h"
#include "Shape.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutGame dialog


CAboutGame::CAboutGame(CGameView* p)
	: CDialog(CAboutGame::IDD, NULL), pView( p)
{
	//{{AFX_DATA_INIT(CAboutGame)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutGame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutGame)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutGame, CDialog)
	//{{AFX_MSG_MAP(CAboutGame)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutGame message handlers
/////////////////////////////////////////////////////////////////////////////
// CLevelDlg dialog


CLevelDlg::CLevelDlg(CGameView* p)
	: CDialog(CLevelDlg::IDD, 0), pView( p)
{
	//{{AFX_DATA_INIT(CLevelDlg)
	m_level = -1;
	//}}AFX_DATA_INIT
}


void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLevelDlg)
	DDX_Radio(pDX, IDC_RADIO_L1, m_level);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CLevelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevelDlg message handlers

BOOL CLevelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_level = pView->GameManager.pGame->Level - 1;

	UpdateData( false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLevelDlg::OnOK() 
{
	UpdateData();

	pView->GameManager.pGame->SetLevel( m_level + 1);
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// DHelpDlg dialog


DHelpDlg::DHelpDlg( CGameView* p)
	: CDialog(DHelpDlg::IDD, 0), pView( p)
{
	//{{AFX_DATA_INIT(DHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DHelpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DHelpDlg, CDialog)
	//{{AFX_MSG_MAP(DHelpDlg)
	ON_BN_CLICKED(IDC_CONTENTS, OnContents)
	ON_BN_CLICKED(IDC_ABOUT_GAME, OnAboutGame)
	ON_BN_CLICKED(IDC_EXAMPLE, OnExample)
	ON_BN_CLICKED( IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DHelpDlg message handlers

void DHelpDlg::OnContents() 
{
	pView->OnHelpContens(); 
	
	OnOK();
}

void DHelpDlg::OnOK() 
{
	CDialog::OnOK();
}

void DHelpDlg::OnAboutGame() 
{
	pView->OnHelpAboutgame();

	CDialog::OnOK();
}

void DHelpDlg::OnExample() 
{
//	pView->PostMessage( WM_EXAMPLE);
	::PostMessage(pView->GetSafeHwnd(), WM_EXAMPLE, 0, 0);

	CDialog::OnOK();
}

void DHelpDlg::OnClose() 
{
	CDialog::OnOK();
}
