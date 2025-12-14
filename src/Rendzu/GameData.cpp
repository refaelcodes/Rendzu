// GameData.cpp: implementation of the GameData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



////////////////////////////////////////////////////////////////////////////////
// class CGameData

IMPLEMENT_SERIAL( CGameData, CObject, 1)

CGameData::CGameData()
{
	pGameManager = 0;
	pApp = (CGameApp*)AfxGetApp();
	pComponent = &pApp->Component;
	pComponent->pGame = this;

	Div = MAX_PRICE;

	Loading = false;

	Tbl.InitTablesDriver();

	FieldSize = FIELD_SIZE;

	Current = 0;
	CurrentMode = EditGame;
	CurrentStatus = WhiteStatus;
	IsStarted = false;	

	xScroll = yScroll = 0;

	ChooseIt = true;

	Depth = 7;
	Level = GetLevel();
	if( Level == 0) Level = 1;

	SetDepth( Level);

	LevelMin = 1;
	UINT LevelMax = 10;

	ViewDetails = false;
	ViewPrice = true;
	ViewMove = true;
	ViewCount = true;
	AutoSave = false;

	Square = Tbl.Dimension * Tbl.Dimension;

	ThreadFinished = true;  
	ThreadActivated = false;  

	IsSerialize = false;

	bWriteToFile = false;

	sLevel = "LVL";

	sSettings = "Settings";

	Shell.Init( this);
}

CGameData::~CGameData()
{
	if( !WeightList.IsEmpty()) WeightList.RemoveAll();
}

void CGameData::Serialize( CArchive& ar)
{
	pComponent->pManager->BreakThread();

	UINT size; 

	IsSerialize = true;

	if( ar.IsStoring())  
	{
		UINT count = MovesList.GetCount();
		ar << xScroll << yScroll << Tbl.FieldsPtrList.GetCount();

		MovesList.Serialize( ar);
	}
	else               
	{
		ar >> xScroll >> yScroll >> size;	

		if( !MovesList.IsEmpty()) MovesList.RemoveAll();

		MovesList.Serialize( ar);

		UINT count = MovesList.GetCount();
		
		IsStarted = true;	

		UpdateAllByMovesList( true);

		pGameManager->GoBegin();

		pGameManager->ReviewThread();
	}

	IsSerialize = false;
}

bool CGameData::GameIsStarted()
{
	return IsStarted;
}

void CGameData::UpdateAllByMovesList( bool IsLoad)
{
	Tbl.CleareMatrix();

	CurrentStatus = WhiteStatus;

	Tbl.FreeFieldsPtrList(); 

	Tbl.FreeWinnerList();

	Tbl.FreePatternsTables();

	if( !WeightList.IsEmpty()) WeightList.RemoveAll(); 

	Tbl.CreatePatternTables();

	Shell.RemoveAll();

	SetFinished( false);

	IsStarted = true;

	SetCurrent( 0);

	TMovesList temp;

	POSITION pos = MovesList.GetHeadPosition();

	while( pos)
	{
		CSize index = MovesList.GetAt( pos);		

		temp.AddTail( index);	
		
		MovesList.GetNext( pos);
	}

	if( !MovesList.IsEmpty()) MovesList.RemoveAll();

	CurrentMode = EditGame;

	pComponent->pView->Board.Draw();

	pos = temp.GetHeadPosition();

	while( pos)
	{
		CSize index = temp.GetAt( pos);		

		pGameManager->UpdateNewMove( index, false, true);

		temp.GetNext( pos);
	}

	if( !temp.IsEmpty()) temp.RemoveAll();

	CurrentMode = EditGame;

	Loading = false;	
}

void CGameData::SetDepth(int level)
{
	int count = GetCount();

	switch( level)
	{
	case 1:
		Depth = 2;
		if( count > 5) Depth = 1;
		break;

	case 2:
		Depth = 2;
		if( count > 5) Depth = 1;
		break;

	case 3:
		Depth = 2;
		break;

	case 4:
		Depth = 2;
		break;

	case 5:
		Depth = 4;
		break;

	case 6:
		Depth = 10;
		break;

	case 7:
		Depth = 12;
		break;

	default:
		Depth = 16;
	};
}

int CGameData::GetLevel()
{
	int lvl = AfxGetApp()->GetProfileInt( "GameSection", "LVL", 0);

	if( lvl == 0) SetLevel( 1);

	return lvl;
}

void CGameData::SetLevel( int l)
{
	AfxGetApp()->WriteProfileInt( "GameSection", "LVL", l);

	Level = l;

	SetDepth( l);
}
				
UINT CGameData::GetDiv()
{ 
	CTime tm;
	
	tm = CTime::GetCurrentTime();
	
	UINT bf = tm.GetTime(); 
	
//	Div = bf%MAX_PRICE + 5;

	Div = 70;
	
	return Div; 
}

CSize CGameData::CalcPrice()
{	
	CSize price( Tbl.P[WHITE]->GetWeight(), Tbl.P[BLACK]->GetWeight());

	utility::WeightFormat( price, GetDiv());

	WeightList.AddTail( price); 

	return price;
}

void CGameData::UpdateCandidate(CSize index)
{
	IsMachineProcess = true;

	AddNewChecker( index);

	Tbl.UpdateNewMove();

	Shell.OnInsertMove( index);

	UpdateWeight();
}

void CGameData::AddNewChecker(CSize index)
{
	CField field( index, NextColorStatus());

	Tbl.F[index.cx][index.cy].Status = field.Status;

	Tbl.F[index.cx][index.cy].Index = field.Index;

	Tbl.FieldsPtrList.AddTail( &Tbl.F[index.cx][index.cy]);

	MovesList.AddTail( index);

	SetCurrent( Tbl.FieldsPtrList.GetTailPosition());
}

eField CGameData::NextColorStatus()
{
	if( Tbl.FieldsPtrList.GetCount() % 2 == 0) return WhiteStatus;

	else return BlackStatus;
}

bool CGameData::UpdateUnDo()
{		
	POSITION pos = Tbl.FieldsPtrList.GetTailPosition();
	
	if( !pos) return false;

	if( !CurrentIsTail()) Tbl.FreeWinnerList();

	SetFinished( false);
	
	while( ( pos = Tbl.FieldsPtrList.GetTailPosition()) != GetCurrent())
	{		
		if( !pos) return false;		
	
		CField field = *Tbl.FieldsPtrList.GetAt( pos);

		Tbl.FieldsPtrList.RemoveTail();

		MovesList.RemoveTail();

		WeightList.RemoveTail();		
	
		Tbl.UpdateUnDoMove( field);

		Shell.OnDeleteMove( field.Index, field.Status);

		Tbl.UpdateNeighbor( field.Status, field.Index, ON_DELETE);		
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GameData::GameData()
{

}

GameData::~GameData()
{

}
