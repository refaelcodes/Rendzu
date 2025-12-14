// GameView.cpp : implementation of the CGameView class
//

#include "stdafx.h"
#include "Game.h"
#include "GameDoc.h"
#include "GameView.h"
#include "SettingDlg.h"
#include "Password.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameView

IMPLEMENT_DYNCREATE(CGameView, CScrollView)

BEGIN_MESSAGE_MAP(CGameView, CScrollView)
	//{{AFX_MSG_MAP(CGameView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BEGIN, OnBegin)
	ON_COMMAND(ID_END, OnEnd)
	ON_COMMAND(ID_MOVE_BACK, OnMoveBack)
	ON_COMMAND(ID_MOVE_UP, OnMoveUp)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_MACHINE, OnMachine)
	ON_COMMAND(ID_PAUSE, OnEditGame)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_LEVEL, OnLevel)
	ON_COMMAND(ID_HELP_BUTTON, OnHelpButton)
	ON_COMMAND(ID_HELP_CONTENS, OnHelpContens)
	ON_COMMAND(ID_HELP_ABOUTGAME, OnHelpAboutgame)
	ON_COMMAND(ID_VIEW, OnView)
	ON_MESSAGE(WM_USER_MACHINE_MOVE, &CGameView::OnMachineMove)
	ON_MESSAGE( WM_EXAMPLE, &CGameView::OnExample)
	ON_MESSAGE(WM_USER_EXAMPLE_OPEN, &CGameView::OnExampleOpen)
	ON_MESSAGE(WM_USER_REVIEW_DONE, &CGameView::OnReviewDone)
	//ON_MESSAGE( WM_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameView construction/destruction

CGameView::CGameView() 
{
	CShape::pView = this;
	IsExample = false;
	pApp = 0;
	CGameApp* tmp = (CGameApp*)AfxGetApp();
	pComponent = &tmp->Component;
	pComponent->pView = this;
}

CGameView::~CGameView()
{
}

LRESULT CGameView::OnMachineMove(WPARAM wParam, LPARAM lParam)
{
	CSize index(LOWORD(lParam), HIWORD(lParam));

	// вызываем UpdateNewMove уже в UI-потоке
	GameManager.UpdateNewMove(index, true, false);
	GameManager.pGame->IsMachineProcess = false;

	GameManager.pGame->ThreadActivated = false;

	GameManager.pGame->ThreadFinished = true;
	return 0;
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGameView drawing

void CGameView::OnDraw(CDC* pDC)
{
	GameManager.Redraw();
}

void CGameView::OnInitialUpdate()
{
	static bool first = true;
	
	if( first)
	{
		first = false;

		CScrollView::OnInitialUpdate();
		
		pApp = ( CGameApp*) AfxGetApp();
		
		pDoc = GetDocument();
				
		CSize sizeTotal = Board.DocSize;
		
		CClientDC dc( this);
		
		sizeTotal.cx = ::MulDiv( sizeTotal.cx, dc.GetDeviceCaps( LOGPIXELSX), 100);
		
		sizeTotal.cy = ::MulDiv( sizeTotal.cy, dc.GetDeviceCaps( LOGPIXELSY), 100);
				
		SetScrollSizes(MM_TEXT, sizeTotal);
		
		Board.SetDocSize( sizeTotal);
			
		GameManager.Init( this);			
		
		pDoc->GameData.pGameManager = &GameManager;
		
		OnFileNew();
	}

	ScrollToCurrent();
	
	Board.Clear();
	
	GameManager.Redraw();
}

void CGameView::PrepareClickedPoint(CPoint &point)
{
	CClientDC* pdc = new CClientDC( this);

	this->OnPrepareDC( pdc);
	
	pdc->DPtoLP( &point);

	delete pdc;
}

void CGameView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !pDoc->GameData.CheckLoadState()) return;

	if( GameManager.IsThreadActivated() || GameManager.IsMachineTurn()) return;

	if( !GameManager.CheckFirstClick()) return;

	PrepareClickedPoint( point);

	if( !Board.PointInBoard( point)) return ;

	if( pDoc->GameData.GetCurrent() != GameManager.FieldsPtrList->GetTailPosition()) return;// 04/04/04

	GameManager.UpdateNewMove( Board.GetIndex( point), false, false);

	if( GameManager.pDoc->GameData.AutoSave) OnFileSave(); 

	CScrollView::OnLButtonDown(nFlags, point);
}

void CGameView::ScrollToCurrent()
{
	int Xminp, Xmaxp, Yminp, Ymaxp;

	GetScrollRange( SB_HORZ, &Xminp, &Xmaxp);
	GetScrollRange( SB_VERT, &Yminp, &Ymaxp);

	if( !pDoc->GameData.GetCurrent())
	{
		//pDoc->GameData.xScroll = (Xminp + Ymaxp) /2 - 350;
		//pDoc->GameData.yScroll = (Yminp + Ymaxp) /2 - 190;

		pDoc->GameData.xScroll = (Xminp + Ymaxp) /2 - 450;
		pDoc->GameData.yScroll = (Yminp + Ymaxp) /2 - 300;
	}

	SetScrollPos( SB_HORZ, pDoc->GameData.xScroll, true);
	SetScrollPos( SB_VERT, pDoc->GameData.yScroll, true);
}

void CGameView::OnBegin() 
{
	if( !pDoc->GameData.CheckLoadState()) return;

	if( GameManager.IsThreadActivated()) return;

	GameManager.GoBegin();	
}

void CGameView::OnEnd() 
{
	if( !pDoc->GameData.CheckLoadState()) return;

	if( GameManager.IsThreadActivated()) return;

	GameManager.GoEnd();
}

void CGameView::OnMoveBack() 
{
	if( !pDoc->GameData.CheckLoadState()) return;

	if( GameManager.IsThreadActivated()) return;

	GameManager.GoBack();	
}

void CGameView::OnMoveUp() 
{
	if( !pDoc->GameData.CheckLoadState()) return;

	if( GameManager.IsThreadActivated()) return;

	GameManager.GoUp();	
}

void CGameView::OnFileNew() 
{
	if( GameManager.IsThreadActivated()) return;

	pDoc->OnFileNew();

	ScrollToCurrent();
	
	Board.Draw();
}

BOOL CGameView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	CPoint scroll = GetScrollPosition();

	pDoc->GameData.xScroll = scroll.x;

	pDoc->GameData.yScroll = scroll.y;
	
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}


/////////////////////////////////////////////////////////////////////////////
// CGameView diagnostics

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() // non-debug version is inline
{
	Assert(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CGameView message handlers


void CGameView::OnMachine() 
{
	GameManager.OnMachine();
}

void CGameView::OnEditGame() 
{
	if( !pDoc->GameData.CheckLoadState()) return;

	GameManager.OnEditGame(); 	
	GameManager.OnEditGame();

	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CScrollView::OnRButtonDown(nFlags, point);
}

void CGameView::OnFileSave() 
{
	pDoc->OnFileSave(); 		
}

void CGameView::OnFileOpen() 
{
	pDoc->OnFileOpen(); 		
}

void CGameView::OnAppAbout() 
{
	CAboutGame about( this);
	
	about.DoModal();
}

// Rename the original handler to OnPasswordImpl (or similar).

void CGameView::OnLevel() 
{
	if( GameManager.IsThreadActivated()) return;

	CLevelDlg  dlg( this);	

	dlg.DoModal();
}

void CGameView::OnHelpButton() 
{
	DHelpDlg dlg( this);

	dlg.DoModal();
}

void CGameView::OnHelpContens() 
{
	UINT length = 300;
	TCHAR Buffer[MAX_PATH];

	CGameApp* pApp = (CGameApp*)AfxGetApp();

	CString Home;

	Home = pApp->GetP();

	CString GoDir = Home;
	GoDir = Home + "\\Help";

	BOOL res = SetCurrentDirectory( GoDir.GetBuffer( 200) );

    DWORD rc = GetCurrentDirectory( length, Buffer);

	if( strstr( Buffer, "Desktop"))
	{
		MessageBox( "Run game from home dir");

		return;
	}
	
    CString Path = "start iexplore  " + GoDir +"\\Rendzu – Help.html";

	system( Path.GetBuffer( 250));	

	GoDir = Home + "\\Games";

	res = SetCurrentDirectory( GoDir.GetBuffer( 200) );

	DWORD wrc = GetCurrentDirectory( length, Buffer);
}

void CGameView::OnHelpAboutgame() 
{
	OnAppAbout();	
}

UINT ExampleThreadProc( LPVOID param)
{
	CGameApp* pApp = (CGameApp*)AfxGetApp();

	Sleep( 600);

	Assert(pApp->m_pDocManager != NULL);

	pApp->m_pDocManager->OnFileOpen();

	return 0;
}

void CGameView::ExampleThread()
{
	HWND hWnd = GetSafeHwnd();
		
	AfxBeginThread( ExampleThreadProc, hWnd, THREAD_PRIORITY_LOWEST);
}

LRESULT CGameView::OnExample(WPARAM wParam, LPARAM lParam)
{
	CGameApp* pApp = pDoc->GameData.pApp;

	UINT length = 300;
	TCHAR Buffer[MAX_PATH];

	GetCurrentDirectory(length, Buffer);

	CString Home = pApp->GetP();

	CString GoDir = Home + "\\Games";
	SetCurrentDirectory(GoDir.GetBuffer(200));

	GetCurrentDirectory(length, Buffer);

	IsExample = true;
	OnFileOpen();      // теперь это точно UI-поток
	IsExample = false;

	return 0;
}


LRESULT CGameView::OnExampleOpen(WPARAM wParam, LPARAM lParam)
{
	CGameApp* pApp = pDoc->GameData.pApp;

	UINT length = 300;
	TCHAR Buffer[MAX_PATH];

	DWORD rc = GetCurrentDirectory(length, Buffer);

	CString Home;

	Home = pApp->GetP();

	CString GoDir = Home;
	GoDir = Home + "\\Games";

	SetCurrentDirectory(GoDir.GetBuffer(200));

	rc = GetCurrentDirectory(length, Buffer);

	IsExample = true;

	pApp->m_pDocManager->OnFileOpen();

	IsExample = false;
	return 0;
}

LRESULT CGameView::OnReviewDone(WPARAM wParam, LPARAM lParam)
{
	CGameApp* tmp = (CGameApp*)AfxGetApp();
	CComponent* pComponent = &tmp->Component;

	pComponent->pManager->Review();

	return 0;
}

void CGameView::OnView() 
{
	if( GameManager.IsThreadActivated()) return;

	int lvl = AfxGetApp()->GetProfileInt( "Settings", "Load", 0);

	if( lvl == 0) 
	{
		Sleep( 200);

		GameManager.ReviewThread();	
	}	
	else
	{
		AfxGetApp()->WriteProfileInt( "Settings", "Load", 0);

		Sleep( 200);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLevel dialog


CLevel::CLevel(CWnd* pParent /*=NULL*/)
	: CDialog(CLevel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLevel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLevel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLevel, CDialog)
	//{{AFX_MSG_MAP(CLevel)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevel message handlers
