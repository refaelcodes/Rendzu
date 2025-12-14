#include "stdafx.h"
#include "Game.h"

#include "GameView.h"
#include "GameManager.h"
#include "Compute.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//  class CGameManager

CGameManager::CGameManager( CGameView* view): pView( view) 
{
	CMoveData::pManager = this;
	
	CGameApp* tmp = (CGameApp*)AfxGetApp();
	pComponent = &tmp->Component;
	pComponent->pManager = this;
	
	pDoc = pComponent->pDoc;

	pGame = pComponent->pGame;

	CMoveData::F = pGame->Tbl.F;
}

CGameManager::~CGameManager()
{
}

void CGameManager::Init(CGameView *view)
{
	pView = view;

	Assert( pView != 0);
	
	pDoc = pView->GetDocument();
	
	pGame = &pDoc->GameData;
		
	FieldsPtrList = &pGame->Tbl.FieldsPtrList;
}

bool CGameManager::ClickIsNewMove()
{
	if(!pGame->CurrentIsTail()&&(pGame->CurrentMode == EditGame))  // cut stay moves
	{
		pGame->Tbl.FreeWinnerList();

		UpdateUnDo();
	}
	else if(!pGame->CurrentIsTail()) return false;  // do nothing

	if( pGame->Finished() || !GameIsStarted() || IsMachineTurn()) return false;

	if( pGame->Tbl.F[CurrentIndex.cx][CurrentIndex.cy].Status == EmptyStatus) return true; // is new move
	
	return false;
}

bool CGameManager::GameIsStarted() const
{
	return pGame->IsStarted;
}

void CGameManager::StartGame()
{
	pGame->Tbl.CreatePatternTables();
	
	if( !pGame->IsStarted) pGame->IsStarted = true;

	if( !pGame->Finished() && IsMachineTurn())
	{
		CSize index( pGame->Tbl.Dimension / 2, pGame->Tbl.Dimension / 2);
		
		UpdateNewMove( index, false, false);

		Redraw();	
	}
}

void CGameManager::DrawChecker(CField field)
{
	UINT idRes = Xicon;

	if( field.Status == BlackStatus) idRes = Oicon;

	pView->Board.DrawIcon( field.Index, idRes);
}

void CGameManager::DrawHighLightChecker( CField field)
{
	UINT idRes = HXicon;

	if( field.Status == BlackStatus) idRes = HOicon;

	pView->Board.DrawIcon( field.Index, idRes);
}

void CGameManager::DrawWinnerChecker( CField field)
{
	UINT idRes = WXicon;

	if( field.Status == BlackStatus) idRes = WOicon;

	pView->Board.DrawIcon( field.Index, idRes);
}

void CGameManager::DrawAllMoves( POSITION from)
{
	if( pGame->GetCurrent() == 0) return;

	POSITION pos = from;

	POSITION flag;
	
	if( !IsThreadActivated()) flag = pGame->GetCurrent();

	else flag = pGame->saveCurrent;

	FieldsPtrList->GetNext( flag);

	FieldsPtrList->GetTailPosition();

	CField* field;

	while( pos && pos != flag )
	{
		field = FieldsPtrList->GetNext( pos);

		DrawChecker( *field);
	}

	if( !IsThreadActivated()) field = FieldsPtrList->GetAt( pGame->GetCurrent());

	else field = FieldsPtrList->GetAt( pGame->saveCurrent);

	DrawHighLightChecker( *field);
}

void CGameManager::DrawAllMoves()
{
	if( pGame->GetCurrent() == 0) return;

	DrawAllMoves( FieldsPtrList->GetHeadPosition());
}

void CGameManager::GoBegin()
{
	pGame->SetCurrent( 0);

	Redraw();
}

void CGameManager::GoEnd()
{
	if( pGame->GetCurrent() == FieldsPtrList->GetTailPosition()) return;

	POSITION prev = pGame->GetCurrent();
	
	if( prev)
	{
		CField* field = FieldsPtrList->GetAt( prev);

		pView->Board.ClearIcon( field->Index);
	}

	pGame->SetCurrent( FieldsPtrList->GetTailPosition());

	if( prev) DrawAllMoves( prev);

	else DrawAllMoves();

	DrawWinnerFields();
}

void CGameManager::GoBack()
{
	if( !pGame->GetCurrent()) return;

	POSITION prev = pGame->GetCurrent();

	pGame->DecrementCurrent();

	CField field = *FieldsPtrList->GetAt( prev);

	pView->Board.ClearIcon( field.Index);

	bool final = pGame->Finished();

	if( !pGame->GetCurrent()) return;

	else if( pGame->Finished() && !pGame->CurrentIsTail()) DrawAllMoves();
	
	else
	{
		field = *FieldsPtrList->GetAt( pGame->GetCurrent());
		
		DrawHighLightChecker( field);
	}
}

void CGameManager::GoUp()
{
	if( pGame->CurrentIsTail()) return;
	
	POSITION prev = pGame->GetCurrent();
	
	if( pGame->GetCurrent()) 
	{		
		pGame->IncrementCurrent();

		CField field = *FieldsPtrList->GetAt( prev);

		pView->Board.ClearIcon( field.Index);

		DrawChecker( field);
	}
	else pGame->SetCurrent( FieldsPtrList->GetHeadPosition());
	
	CField field = *FieldsPtrList->GetAt( pGame->GetCurrent());

	DrawHighLightChecker( field);

	DrawWinnerFields();
}

void CGameManager::DrawNewChecker()
{
	GoBack();

	GoUp();
}

bool CGameManager::IsMachineTurn()
{
	bool turn = false;

	switch( pGame->CurrentMode)
	{
	case EditGame: return false;

	case ViewMode: return true;
	
	case TwoMachines: return true;
	
	case MachineWhite: return ( pGame->NextColorStatus() == WhiteStatus);
	
	case MachineBlack: return ( pGame->NextColorStatus() == BlackStatus);
	
	default: return true;
	};
}

UINT AFX_CDECL CGameManager::ThreadProc(LPVOID pParam)
{
	// сюда мы передадим указатель на CGameManager
	CGameManager* pMgr = static_cast<CGameManager*>(pParam);
	ASSERT(pMgr != nullptr);

	CGameApp* tmp = (CGameApp*)AfxGetApp();
	CComponent* pComponent = &tmp->Component;

	// здесь можно вызывать pComponent->pManager->CallMachine();
	// или pMgr->CallMachineWorker(), если разделишь логику
	pComponent->pManager->CallMachine();

	return 0;
}

void CGameManager::DrawWinnerFields()
{
	if( pGame->IsMachineProcess || pGame->Loading || !pGame->CurrentIsTail()) return;

	POSITION pos = pGame->Tbl.WinnerList.GetHeadPosition();

	while( pos)
	{
		CField field = pGame->Tbl.WinnerList.GetNext( pos);

		DrawWinnerChecker( field);
	}
}

void CGameManager::Redraw()
{
	BOOL rc;

	int pr, pr2;

	if( pGame->ThreadActivated)
	{
		pr = GetThreadPriority( hMachine);

		rc = SetThreadPriority( hMachine, THREAD_PRIORITY_LOWEST); 

		pr2 = GetThreadPriority( hMachine);
	}

	pView->Board.Draw();

	DrawAllMoves();

	if( pGame->CurrentIsTail()) DrawWinnerFields();

	if( pGame->ThreadActivated)
	{
		pr2 = GetThreadPriority( hMachine);
		rc = SetThreadPriority( hMachine, pr); 
		pr2 = GetThreadPriority( hMachine);

	}
}

void CGameManager::CutMovesUpdate()
{
	if( !pGame->CurrentIsTail())
	{
		pGame->SetFinished( false);

		pGame->Tbl.FreeWinnerList();
	}

	UpdateUnDo();

	pGame->SetCurrent( pGame->Tbl.FieldsPtrList.GetTailPosition());

	return;
}

void CGameManager::UpdateNewMove(CSize index, bool FromMachine, bool IsSerialize)
{	
	if( pGame->IsFullBoard())
	{
		pGame->SetFinished( true);

		return;
	}

	pGame->IsMachineProcess = false;

	CurrentIndex = index;

	if( !IsMachineTurn() && !ClickIsNewMove()) return;

	if( FromMachine && pGame->GetCount() == 1) Sleep( 400);
	
	pGame->AddNewChecker( CurrentIndex);//, IsSerialize);

	if( !IsSerialize) DrawNewChecker();

	pGame->Tbl.UpdateNewMove();

	pGame->Shell.OnInsertMove( index);

	pGame->UpdateWeight();

	pDoc->SetModifiedFlag();

	if( pGame->Finished() && pGame->CurrentIsTail()) DrawWinnerFields();

	if( !pGame->Finished() && IsMachineTurn()) 	CallMachineThread();
}

void CGameManager::CallMachineThread()
{
	if (!pGame->GameIsStarted())
		return;

	if (pGame->Tbl.FieldsPtrList.IsEmpty())
		StartGame();
	else
	{
		pGame->ThreadActivated = true;

		// было: CreateThread(...)
		// стало (как мы обсуждали):
		CWinThread* pThread = AfxBeginThread(
			CGameManager::ThreadProc,
			this,
			THREAD_PRIORITY_HIGHEST);

		C_Handle::hnd = pThread->m_hThread;
	}
}




void CGameManager::CallMachine()
{	
	pGame->SetDepth( pGame->GetLevel());
	pGame->ThreadFinished = false;

	pGame->saveCurrent = pGame->GetCurrent();

	CCompute calc;

	pGame->IsMachineProcess = true;

	CurrentIndex = calc.CalcPrice();

	Assert( CurrentIndex.cx != OUT_SIZE);
	Assert( CurrentIndex.cy != OUT_SIZE);

	LPARAM lParam = MAKELPARAM(CurrentIndex.cx, CurrentIndex.cy);

	// посылаем сообщение главному окну / view
	::PostMessage(pView->GetSafeHwnd(), WM_USER_MACHINE_MOVE, 0, lParam);
	/*
	UpdateNewMove( CurrentIndex, true, false);

	pGame->IsMachineProcess = false;

	pGame->ThreadActivated = false;

	pGame->ThreadFinished = true;
	*/
}

void CGameManager::SetMode(eMode mode)
{
	if( IsThreadActivated() || pGame->Finished()) return;

	pGame->CurrentMode = mode;

	if( !GameIsStarted()) StartGame();

	if( IsMachineTurn()) CallMachineThread();	
}

void CGameManager::OnEditGame()
{
	BreakThread();

	BreakThread();

	SetMode( EditGame);

	if( !pGame->CurrentIsTail()) CutMovesUpdate();
}

bool CGameManager::CheckFirstClick()
{
	if( !GameIsStarted())
	{
		pGame->CurrentMode = MachineBlack;
		
		StartGame();

		return true;
	}
	else if( !IsMachineTurn()) return true;

	else return false;
}

UINT LoadThreadProc( LPVOID param)
{
	CGameApp* tmp = (CGameApp*)AfxGetApp();
	CComponent* pComponent = &tmp->Component;

	pComponent->pManager->Review();

	return 0;
}

void CGameManager::ReviewThread()
{
	HWND hWnd = pComponent->pView->GetSafeHwnd();
		
	//AfxBeginThread( LoadThreadProc, hWnd, THREAD_PRIORITY_LOWEST);
	::PostMessage(hWnd, WM_USER_REVIEW_DONE, 0, 0);
}

void CGameManager::Review()
{
	AfxGetApp()->WriteProfileInt( "Settings", "Load", 1);

	if( pGame->GetCurrent() == FieldsPtrList->GetTailPosition())
	{
		GoBegin();
	}

	while( pGame->GetCurrent() != FieldsPtrList->GetTailPosition())
	{
		GoUp();
	
		Sleep( 800);

		int lvl = AfxGetApp()->GetProfileInt( "Settings", "Load", 0);

		if( lvl == 0) return;
	}

	AfxGetApp()->WriteProfileInt( "Settings", "Load", 0);

	return;
}

void CGameManager::OnMachine()
{
	if( utility::ThreadActive()) return;
	
	if( !pGame->CheckLoadState()) return;

	if( IsThreadActivated()) return;
	
	if( !pGame->CurrentIsTail()) return;
	
	if( pGame->NextColorStatus() == BlackStatus) SetMode( MachineBlack);

	else SetMode( MachineWhite);		
}


void CGameManager::BreakThread()
{
	if( !pGame->ThreadFinished)
	{
		pGame->ThreadActivated = false;
	
		while( !pGame->ThreadFinished) Sleep( 100);		
	}

	pGame->ThreadFinished = true;
}

bool CGameManager::UpdateUnDo()
{
	pGame->UpdateUnDo();

	TMovesList& MovesList = pGame->MovesList;

	if( MovesList.IsEmpty()) Redraw();
	
	return true;
}