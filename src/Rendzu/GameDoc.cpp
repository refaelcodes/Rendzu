// GameDoc.cpp : implementation of the CGameDoc class
//

#include "stdafx.h"
#include "GameDoc.h"
#include "Gameview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGameDoc

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
	//{{AFX_MSG_MAP(CGameDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)	
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDoc construction/destruction

CGameDoc::CGameDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CGameDoc serialization

CGameDoc::~CGameDoc()
{
}

BOOL CGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;

	GameData.pDoc = this;

	return TRUE;
}

void CGameDoc::Serialize(CArchive& ar)
{
	GameData.Serialize( ar);
}

void CGameDoc::OnFileNew() 
{
	if( !GameData.CheckLoadState()) return;

	if( !GameData.MovesList.IsEmpty()) GameData.MovesList.RemoveAll();

	GameData.UpdateAllByMovesList();

	GameData.IsStarted = false;

	SetModifiedFlag( true);

	if( !GameData.IsThreadActivated()) SetTitle( "New Game");	

	GameData.SetFinished( false);

	GameData.GetDiv();
}

void CGameDoc::OnFileOpen() 
{
	if( !GameData.CheckLoadState()) return;

	if( GameData.MovesList.IsEmpty()) SetModifiedFlag( false);

	CGameApp* pApp = (CGameApp*)AfxGetApp();

	GameData.SetGamesDir();
	CGameView* pView = GameData.pGameManager->pView;
	pView->IsExample = true;

	Assert(pApp->m_pDocManager != NULL);
	::PostMessage(pView->GetSafeHwnd(), WM_USER_EXAMPLE_OPEN, 0, 0);

	//pApp->m_pDocManager->OnFileOpen();
}

void CGameDoc::OnFileSave() 
{
	if( !GameData.CheckLoadState()) return;

	CGameApp* pApp = (CGameApp*)AfxGetApp();
	CString Home = pApp->GetP();

	CString PathName = Home + "\\" + GetTitle();

	SetPathName( PathName.GetBuffer( 250), false);

	if( !GameData.IsThreadActivated()) DoSave( 0);		
}

/////////////////////////////////////////////////////////////////////////////
// CGameDoc diagnostics

#ifdef _DEBUG
void CGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGameDoc commands
