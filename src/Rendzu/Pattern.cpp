// Pattern.cpp: implementation of the CPattern class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Game.h"
#include "Pattern.h"
#include "GameView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////
// class CPowerNumber

CPattern::CPowerNumber::CPowerNumber()
{
	wOpened = 0;
	
	wHalfOpened = 0;
	
	wClosed = 0; 
}

///////////////////////////////////////////////////////////////////////////
// class CPattern
/*
int Opn[] = {37, 80, 350, 2500, 8200};

int Hlf[] = {8, 50, 525, 2120, 5000};

int Cls[] = {1, 10, 25, 200, 730};
*/

int Opn[] = {37, 200, 350, 800,7200};

int Hlf[] = {8, 70, 125, 500, 4000};

int Cls[] = {1, 10, 25, 200, 730};

class CGameDoc;
class CGameManager;
class CGame;

//CGameDoc*  CPattern::pDoc=0;


CPattern::CPattern()
{
	pTable = 0;

	pPowerNumber = 0;

	Status = EmptyStatus;
}

CPattern::~CPattern()
{
	FreePatternTable();
}

void CPattern::CreatePatternTable( eField status)
{
	Assert( pTable == 0);

	Status = status;

	Size = FIVE;

	pTable = new TMoveDataList[Size];

	Assert( pTable != 0);

	pPowerNumber = new CPowerNumber[Size];

	Assert( pPowerNumber != 0);

	Weight = 0;
}

void CPattern::FreePatternTable()
{
	for( int i = 0; i < Size; i++)
	{
		POSITION pos = pTable[i].GetHeadPosition();
		
		while( pos)
		{
			CMoveData* p = pTable[i].GetAt( pos); 
			
			delete p;
			
			pTable[i].GetNext( pos);
		}
		
		if( !pTable[i].IsEmpty()) pTable[i].RemoveAll();			
	}
	
	pTable->RemoveAll();
	
	delete[] pTable;
	
	Size = 0;
	
	pTable = 0;
	
	if( pPowerNumber) delete [] pPowerNumber;
}

POSITION CPattern::Find( CMoveData& id, int ListNum)
{
	Assert( ListNum >= 0 && ListNum < (int)Size);

	POSITION find = pTable[ListNum].GetHeadPosition();

	while( find)
	{
		CMoveData* pid = pTable[ListNum].GetAt( find);

		if( *pid == id) break;

		pTable[ListNum].GetNext( find);
	}

	if( !find)
	{
		CSize index = id.Index;

		AfxMessageBox("find == NULL !!!");
	}

	return find;
}

void CPattern::Insert( CMoveData& id, int ListNum, eDirect direct, ePower power)
{
	Assert( ListNum >= 0);

	if( ListNum >= (int)Size) ListNum = Size - 1;

	CMoveData* pid = new CMoveData( id);

	pid->NumList = ListNum;

	pid->SetPower( power);

	Weight += CalcWeight( pid, ListNum, direct, power);

	pTable[ListNum].AddTail( pid);

	switch( power)
	{
	case eOpened : pPowerNumber[ListNum].wOpened++; break;

	case eHalfOpened : pPowerNumber[ListNum].wHalfOpened++; break;
	
	case eClosed : pPowerNumber[ListNum].wClosed++; break;

	default: break;
	};
}

void CPattern::Delete( CMoveData& id, int ListNum, eDirect direct)
{
	if( ListNum >= (int)Size) ListNum = Size - 1;

	Assert( ListNum >= 0 && ListNum < (int)Size);

	POSITION find = Find( id, ListNum);

	Assert( find != 0);

	CMoveData* pid = pTable[ListNum].GetAt( find);

	ePower power = pid->GetPower();
	
	switch( direct)
	{
	case eT:
		Weight -= pid->wT;
		break;
	case eL:
		Weight -= pid->wL;
		break;
	case eLT:
		Weight -= pid->wLT;
		break;
	case eRT:
		Weight -= pid->wRT;
		break;
	default:
		break;			
	};

	delete pid;

	pTable[ListNum].RemoveAt( find);

	switch( power)
	{
	case eOpened : pPowerNumber[ListNum].wOpened--; break;

	case eHalfOpened : pPowerNumber[ListNum].wHalfOpened--; break;
	
	case eClosed : pPowerNumber[ListNum].wClosed--; break;

	default: break;
	};
}

UINT CPattern::CalcWeight( CMoveData* pid, int ListNum, eDirect direct, ePower power)
{
	UINT weight = 0;

	int n = ListNum + 1;

	if( power == eOpened) weight = Opn[ListNum]; 

	else if( power == eHalfOpened) weight = Hlf[ListNum]; 

	else weight = Cls[ListNum]; 

	if( direct == eLT || direct == eRT) weight *= 2;

	switch( direct)
	{
	case eT:
		pid->wT = weight;
		break;
	case eL:	    
		pid->wL = weight;
		break;
	case eLT:
		weight = ( weight + 5) * 5/2; // 11/04/04
		pid->wLT = weight;		
		break;
	case eRT:	
		weight = ( weight + 5) * 5/2; // 11/04/04
		pid->wRT = weight;		
		break;
	default:
		break;
	};

	return weight;
}

void CPattern::WriteToFile( eField status)
{
return;
	CString File = "C:\\My Documents\\CrossZero\\WhiteTable.txt";

	if( status == BlackStatus) File = "C:\\My Documents\\CrossZero\\BlackTable.txt";

	char s[50];

	CString Title, b = "Table[";
	
	FILE* fp = fopen( File.GetBuffer( 100), "w");

	Assert( fp != 0);

	char opened[50], half[50], closed[50];

	for( UINT i = 0; i < Size; i++)
	{
		itoa( i+1, s, 10);

		Title = b + s;
		Title += "] :";

		fputs( "\n", fp);
		fputs( Title.GetBuffer( 50), fp);

		itoa( pPowerNumber[i].wOpened, opened, 10);
		itoa( pPowerNumber[i].wHalfOpened, half, 10);
		itoa( pPowerNumber[i].wClosed, closed, 10);

		CString app = "  O = ";
		app += opened;
		app += ",  H= ";
		app += half;
		app += ",  C = ";
		app += closed;

		fputs( app.GetBuffer( 150), fp);


		CString data;

		POSITION pos = pTable[i].GetHeadPosition();

		while( pos)
		{
			CMoveData* p = pTable[i].GetNext( pos);

			data = p->DataToString();

			fputs( "\n", fp);
			fputs( data.GetBuffer( 50), fp);
		}
	}
	
	fclose( fp);
}

POSITION CPattern::Find( ePower power, int ListNum, POSITION from)
{
	Assert( ListNum >= 0 && ListNum < (int)Size);

	POSITION find;
	
	if( from)
	{
		pTable[ListNum].GetNext( from);
		
		find = from; 
	}
	else find = pTable[ListNum].GetHeadPosition(); 

	while( find)
	{
		CMoveData* pid = pTable[ListNum].GetAt( find);

		if( pid->GetPower() == power) break;

		pTable[ListNum].GetNext( find);
	}	

	return find;
}

POSITION CPattern::FindExcept( ePower power, int ListNum, POSITION from)
{
	Assert( ListNum >= 0 && ListNum < (int)Size);

	POSITION find;
	
	if( from)
	{
		pTable[ListNum].GetNext( from);
		
		find = from; 
	}
	else find = pTable[ListNum].GetHeadPosition(); 

	while( find)
	{
		CMoveData* pid = pTable[ListNum].GetAt( find);

		if( pid->GetPower() != power) break;

		pTable[ListNum].GetNext( find);
	}	

	return find;
}

bool CPattern::EntrancePattern( POSITION pos, int ListNum, CSize& point)
{
	CSize pre( OUT_SIZE, OUT_SIZE) , no( OUT_SIZE, OUT_SIZE);

	int px, py;

	Assert( pos != 0);

	CMoveData* pid = pTable[ListNum].GetAt( pos);

	int Dimension = DIMENSION;

	eDirect dir = pid->Direct;
	CSize index = pid->Index;

	switch( dir)
	{
	case eL:
		if( index.cx-1 >= 0)
		{
			pre.cx = index.cx-1;
			pre.cy = index.cy;

			px = pre.cx - 1;
			py = pre.cy;
		}
		break;

	case eT:
		if( index.cy-1 >= 0)
		{
			pre.cx = index.cx;
			pre.cy = index.cy-1;

			px = pre.cx;
			py = pre.cy - 1;
		}
		break;

	case eLT:
		if( index.cx-1 >= 0 && index.cy-1 >= 0)
		{
			pre.cx = index.cx-1;
			pre.cy = index.cy-1;

			px = pre.cx - 1;
			py = pre.cy - 1;
		}
		break;

	case eRT:
		if( index.cx+1 < Dimension && index.cy-1 >= 0)
		{
			pre.cx = index.cx+1;
			pre.cy = index.cy-1;

			px = pre.cx + 1;
			py = pre.cy - 1;
		}
		break;
	
	default:
		break;
	};

	CSize pre2;

	if( px >= 0 && px < Dimension && py >= 0 && py < Dimension)
	{
		pre2.cx = px;
		pre2.cy = py;
	}
	
	point = pre;

	if( pre.cx == OUT_SIZE || pre.cx == OUT_SIZE) return false;

	if( CFieldMatrix::F[pre.cx][pre.cy].Status != EmptyStatus) return false;

	else if( px >= 0 && px < Dimension && py >= 0 && py < Dimension) return true;
	
	else return false;
}
	
bool CPattern::ExitPattern( POSITION pos, int ListNum, CSize& point)
{
	CSize post( OUT_SIZE, OUT_SIZE), no( OUT_SIZE, OUT_SIZE);

	int px, py;

	Assert( pos != 0);

	int Dimension = DIMENSION;

	CMoveData* pid = pTable[ListNum].GetAt( pos);

	eDirect dir = pid->Direct;
	CSize index = pid->Index;

	switch( dir)
	{
	case eT:
		if( index.cy+1 < Dimension)
		//if( index.cy + ListNum +1 < Dimension)
		{
			post.cx = index.cx;
			post.cy = index.cy + ListNum +1;

			px = post.cx;
			py = post.cy + 1;
		}
		break;

	case eL:
		if( index.cx-1 < Dimension)
		//if( index.cx + ListNum +1 < Dimension)
		{
			post.cx = index.cx + ListNum +1;
			post.cy = index.cy;

			px = post.cx + 1;
			py = post.cy;
		}
		break;

	case eLT:
		if( index.cx-1 < Dimension && index.cy-1 < Dimension)
		//if( index.cx + ListNum +1 < Dimension && index.cy + ListNum +1 < Dimension)
		{
			post.cx = index.cx + ListNum +1;
			post.cy = index.cy + ListNum +1;

			px = post.cx + 1;
			py = post.cy + 1;
		}
		break;

	case eRT:
		if( index.cx+1 >= 0 && index.cy-1 < Dimension)
		//if( index.cx - ListNum - 1 >= 0 && index.cy + ListNum +1 < Dimension)
		{
			post.cx = index.cx - ListNum - 1;
			post.cy = index.cy + ListNum +1;

			px = post.cx - 1;
			py = post.cy;
		}
		break;
	
	default:
		break;
	};

	point = post;

	if( px < 0 || px >= Dimension || py < 0 && py >= Dimension) return false;

	if( CFieldMatrix::F[post.cx][post.cy].Status != EmptyStatus) return false;

	else if( px >= 0 && px < Dimension && py >= 0 && py < Dimension) return true;
	
	else return false;
}

bool CPattern::ExistsPattern( int NumList, ePower power, int type)
{
	if( power == eOpened)
	{
		if( type == INCLUDE) return ( pPowerNumber->wOpened > 0);

		else return ( pPowerNumber->wHalfOpened || pPowerNumber->wClosed);
	}
	else if( power == eHalfOpened)
	{
		if( type == INCLUDE) return ( pPowerNumber->wHalfOpened > 0);

		else return ( pPowerNumber->wOpened || pPowerNumber->wClosed);
	}
	else 
	{
		if( type == INCLUDE) return ( pPowerNumber->wClosed > 0);

		else return ( pPowerNumber->wOpened || pPowerNumber->wHalfOpened);
	}
}


//////////////////////////////////////////////////////////////////////
// CPatternTables Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatternTables::CPatternTables()
{
	IsEmpty = true;

	CreatePatternTables();
}

CPatternTables::~CPatternTables()
{
	FreePatternsTables();
}

void CPatternTables::CreatePatternTables()
{
	if( !IsEmpty) return;

	P[WHITE] = new CPattern;
	Assert( P[WHITE] != 0);
	P[WHITE]->CreatePatternTable( WhiteStatus);

	P[BLACK] = new CPattern;
	Assert( P[BLACK] != 0);
	P[BLACK]->CreatePatternTable( BlackStatus);

	IsEmpty = false;
}

void CPatternTables::FreePatternsTables()
{
	if( IsEmpty) return;

	P[WHITE]->FreePatternTable();

	P[BLACK]->FreePatternTable();

	IsEmpty = true;
}


//////////////////////////////////////////////////////////////////////
// CFieldMatrix Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CField** CFieldMatrix::F;

CFieldMatrix::CFieldMatrix()
{
	F = 0;

	Dimension = DIMENSION;

	CreateMatrix();
}

CFieldMatrix::~CFieldMatrix()
{
	RemoveMatrix();
}

void CFieldMatrix::CreateMatrix()
{
	Assert( F == 0);

	F = new CField*[Dimension];
	
	Assert( F != 0);
	
	for( int i = 0; i < Dimension; i++)
	{
		F[i] = new CField[Dimension];
		
		Assert( F[i] != 0);
	}	
}


void CFieldMatrix::CleareMatrix()
{
	if( !F) return;

	for( int i = 0; i < Dimension; i++)
	{
		for( int j = 0; j < Dimension; j++) F[i][j].Reset();	
	}
}

void CFieldMatrix::RemoveMatrix()
{
	Assert( F != 0);

	for( int i = 0; i < Dimension; i++) if( F[i]) delete[] F[i];

	if( F) delete[] F;
		
	F = 0;
}



void CPatternTables::UpdatePatterns(eField status, int flag, CSize index)
{
	CPattern* pt = ( status == WhiteStatus ? P[WHITE] : P[BLACK]);

	CField** F = CFieldMatrix::F;

	int dim = DIMENSION;
	
	CSize cand = index;

// L

	if( index.cx-1 >= 0)
	{
		cand.cx = index.cx-1;
		cand.cy = index.cy;
		
		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag , cand, L);
	}

// R

	if( index.cx+1 < dim)
	{
		cand.cx = index.cx+1;
		cand.cy = index.cy;
		
		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag , cand, R);
	}

// 	T
	if( index.cy-1 >= 0)
	{
		cand.cx = index.cx;
		cand.cy = index.cy-1;

		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag ,cand, T);
	}

// B

	if( index.cy+1 < dim)
	{
		cand.cx = index.cx;
		cand.cy = index.cy+1;

		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag ,cand, B);
	}

// LT

	if( index.cx-1 >= 0 && index.cy-1 >= 0 )
	{
		cand.cx = index.cx-1;
		cand.cy = index.cy-1;
		
		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag , cand, LT);
	}

// RB

	if( index.cx+1 < dim && index.cy+1 < dim )
	{
		cand.cx = index.cx+1;
		cand.cy = index.cy+1;
		
		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag , cand, RB);
	}

// RT

	if( index.cx+1 < dim && index.cy-1 >= 0 )
	{
		cand.cx = index.cx+1;
		cand.cy = index.cy-1;
		
		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag , cand, RT);
	}

// LB

	if( index.cx-1 >= 0 && index.cy+1 < dim )
	{
		cand.cx = index.cx-1;
		cand.cy = index.cy+1;
		
		if( F[cand.cx][cand.cy].Status == status) UpdatePattern( status, flag , cand, LB);
	}
}


void CPattern::UpdateUnDo( CSize index)
{
	CMoveData id;

	CField** F = CFieldMatrix::F;

	int  x = index.cx, y = index.cy; 
		
	
	//                Update eL part
	
	if( F[x][y].r)
	{
		id.Set( x+1, y, eL);
		Insert( id, F[x][y].r-1, eL);
	}
	
	id.Set( x - F[x][y].l, y, eL);
	
	Delete( id, F[x][y].r+F[x][y].l, eL);
	
	if( F[x][y].l) Insert( id, F[x][y].l-1, eL);
	
//	WriteToFile( pManager->pGame->Tbl.F[x][y]. Status);
	
	
	//                Update eT part
	
	if( F[x][y].b)
	{
		id.Set( x, y+1, eT);
		Insert( id, F[x][y].b-1, eT);
	}
	
	id.Set( x, y - F[x][y].t, eT);
	
	Delete( id, F[x][y].b+F[x][y].t, eT);
	
	if( F[x][y].t) Insert( id, F[x][y].t-1, eT);		
	
//	WriteToFile( pManager->pGame->Tbl.F[x][y]. Status);
	
	
	//                Update eLT part
	
	if( F[x][y].rb)
	{
		id.Set( x+1, y+1, eLT);
		Insert( id, F[x][y].rb-1, eLT);
	}
	
	id.Set( x - F[x][y].lt, y - F[x][y].lt, eLT);
	
	Delete( id, F[x][y].rb+F[x][y].lt, eLT); 
	
	if( F[x][y].lt) Insert( id, F[x][y].lt-1, eLT);
	
//	WriteToFile( pManager->pGame->Tbl.F[x][y]. Status);
	
	//                Update eRT part
	
	if( F[x][y].lb)
	{
		id.Set( x-1, y+1, eRT);
		Insert( id, F[x][y].lb-1, eRT);
	}
	
	id.Set( x + F[x][y].rt, y - F[x][y].rt, eRT);
	
	Delete( id, F[x][y].lb+F[x][y].rt, eRT);
	
	if( F[x][y].rt) Insert( id, F[x][y].rt-1, eRT);
	
//	WriteToFile( pManager->pGame->Tbl.F[x][y]. Status);
}


POSITION CPatternTables::UpdatePattern(eField status, int flag,
									   CSize index, eExtDirect ExtDir)
{
	POSITION find = 0;

	CField** F = CFieldMatrix::F;
	CPattern* pt = ( status == WhiteStatus ? P[WHITE] : P[BLACK]);

	int NumList = 0;
	CSize PatternHead;
	eDirect Dir;

	switch( ExtDir)
	{
	case L:
		NumList = F[index.cx][index.cy].l;
		Dir = eL;
		PatternHead.cx = index.cx - NumList;
		PatternHead.cy = index.cy;
		break;
	
	case R:
		NumList = F[index.cx][index.cy].r;
		Dir = eL;
		PatternHead = index;
		break;

	case T:
		NumList = F[index.cx][index.cy].t;
		Dir = eT;
		PatternHead.cx = index.cx;
		PatternHead.cy = index.cy - NumList;
		break;
	
	case B:
		NumList = F[index.cx][index.cy].b;
		Dir = eT;
		PatternHead = index;		
		break;

	case LT:
		NumList = F[index.cx][index.cy].lt;
		Dir = eLT;
		PatternHead.cx = index.cx - NumList;
		PatternHead.cy = index.cy - NumList;
		break;
	
	case RB:
		NumList = F[index.cx][index.cy].rb;
		Dir = eLT;
		PatternHead = index;
		break;


	case RT:
		NumList = F[index.cx][index.cy].rt;
		Dir = eRT;
		PatternHead.cx = index.cx + NumList;
		PatternHead.cy = index.cy - NumList;
		break;
	
	case LB:
		NumList = F[index.cx][index.cy].lb;
		Dir = eRT;
		PatternHead = index;
		break;
	};

	if( F[PatternHead.cx][PatternHead.cy].Status == EmptyStatus) return 0;
	
	pt->Id.Set( PatternHead.cx, PatternHead.cy, Dir);
	
	find = pt->Find( pt->Id, NumList);
	
	Assert( find != 0);
	
	CMoveData* pid = pt->pTable[NumList].GetAt( find);
	
	if( flag == ON_DELETE)               // ON_DELETE
	{			
		if( pid->GetPower() == eHalfOpened) 
		{
			pid->SetPower( eOpened);
			pt->pPowerNumber[NumList].wHalfOpened--;
			pt->pPowerNumber[NumList].wOpened++;
		}
		
		else if( pid->GetPower() == eClosed)
		{
			pid->SetPower( eHalfOpened);
			pt->pPowerNumber[NumList].wClosed--;
			pt->pPowerNumber[NumList].wHalfOpened++;
		}
	}
	else                               // ON_INSERT 
	{		
		if( pid->GetPower() == eOpened) 
		{
			pid->SetPower( eHalfOpened);
			pt->pPowerNumber[NumList].wOpened--;
			pt->pPowerNumber[NumList].wHalfOpened++;
		}
		
		else if( pid->GetPower() == eHalfOpened)
		{
			pid->SetPower( eClosed);
			pt->pPowerNumber[NumList].wHalfOpened--;
			pt->pPowerNumber[NumList].wClosed++;
		}
	//  ?????????  was not here 27.02.06
	    pt->Weight -= pid->GetWeightByDirect( Dir);

		CPattern* pt2;

		if( status == WhiteStatus) pt2 = P[BLACK];

		else pt2 = P[WHITE];	
	
	    pt->Weight += pt2->CalcWeight( pid, NumList, Dir, pid->GetPower());
		
//		WriteToFile( F[PatternHead.cx][PatternHead.cy].Status);
	}  // ????????  was here  27.02.05

	return find;
}

void CTablesDriver::FreeWinnerList()
{
	if( !WinnerList.IsEmpty()) WinnerList.RemoveAll();
}

void CTablesDriver::FreeFieldsPtrList()
{
	if( !FieldsPtrList.IsEmpty()) FieldsPtrList.RemoveAll();
}
