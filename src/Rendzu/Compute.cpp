#include "stdafx.h"
#include "Game.h"
#include "Compute.h"
#include "Pattern.h"
#include "GameDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////////
// class CComputeResult

CComputeResult::CComputeResult(): Move( OUT_SIZE, OUT_SIZE), Price( 0, 0), Step( OUT_SIZE)
{
	Status = EmptyStatus;
}

CComputeResult::CComputeResult( CComputeResult& calc)
{
	Move = calc.Move;
	Price = calc.Price;
	Status = calc.Status;
	Step = calc.Step;
}

CComputeResult CComputeResult::operator=( CComputeResult& calc)
{
	if( this != &calc)
	{
		Move = calc.Move;
		Price = calc.Price;
		Status = calc.Status;
		Step = calc.Step;
	}

	return 	*this;
}

void CComputeResult::Set( CSize move, CSize price, eField status)
{
	Move = move;
	Price = price;
	Status = status;
}

///////////////////////////////////////////////////////////////////////////////////
// class CCandidate

CCandidate::CCandidate()
{
	if( !list.IsEmpty()) list.RemoveAll();
}

CCandidate::~CCandidate()
{
	if( !list.IsEmpty()) list.RemoveAll();
}

void CCandidate::Append( CComputeResult& cand)
{
	list.AddTail( cand);
}

void CCandidate::AddFirst( CComputeResult& cand)
{
	if( !list.IsEmpty()) list.RemoveAll();

	list.AddTail( cand);
}

int CCandidate::GetCount()
{
	return list.GetCount();
}

void CCandidate::Review()
{
	POSITION cur = list.GetHeadPosition();
	
	while( cur) CComputeResult cr = list.GetNext( cur);	
}

CComputeResult CCandidate::GetAt( int num)
{
	POSITION pos = list.GetHeadPosition();

	CComputeResult cr;

	while( num > -1)
	{
		cr = list.GetNext( pos);

		num--;
	}

	return cr;
}

////////////////////////////////////////////////////////////////////////
/////////////    class CTimer

CTimer::CTimer( int max_sec):MAX_TIME( max_sec) 
{
	hTime = CreateWaitableTimer( NULL, FALSE, NULL);

	nTimPerSec = 10000000;
	
	li.QuadPart = -( MAX_TIME * nTimPerSec);	
	
	IsOnState = false;
}

CTimer::~CTimer()
{
	CancelWaitableTimer( hTime);
}

void CTimer::TurnOn()
{
	SetWaitableTimer( hTime, &li, 0, NULL, NULL, FALSE);

	IsOnState = true;
}

void CTimer::TurnOff()
{
	CancelWaitableTimer( hTime);
}

bool CTimer::IsTurnOn()
{
	if( !IsOnState) return IsOnState;

	if( WaitForSingleObject( hTime, 0) == WAIT_OBJECT_0) IsOnState = false; 
	
	return IsOnState;
}

////////////////////////////////////////////////////////////////////////////////
// class CCompute

CCompute::CCompute():TimeLimit(120)
{	
}

CCompute::~CCompute()
{
}

bool CCompute::InitExtrimeVariantList( int depth)
{
	Assert( BuildGateList( depth, TOTAL));

	return true;
}

bool CCompute::InitVariantList( int depth)
{
	switch( pGame->Level)
	{
	case 1:	return InitAlgo1( depth);

	case 2:	return InitAlgo2( depth);

	case 3: return InitAlgo3( depth);

	case 4: return InitAlgo4( depth);

	case 5: return InitAlgo5( depth);

	default: return InitAlgo5( depth);
	};
}

CSize CCompute::CalcPrice()
{	
	TimeLimit.TurnOn();

	CSize index( pGame->Tbl.Dimension / 2, pGame->Tbl.Dimension / 2);

	if( pGame->GetCurrent())
	{
		CComputeResult result = CalcPrice( 0);

		CandList.Review(); 

		result = ChooseCand();

		Assert( result.Move.cx != OUT_SIZE);
	
		index = result.Move;		
	}

	pGame->ThreadFinished = true;

	if( TimeLimit.IsTurnOn()) TimeLimit.TurnOff();

	return index;
}

CComputeResult CCompute::CalcExtrimePrice( int depth)
{
	CComputeResult cand, result;			

	Assert( InitExtrimeVariantList( depth)) ;

	result.Status = pDepthColor[depth];
	
	POSITION pos = pVariantList[depth].GetHeadPosition();

	Assert( pos != 0);

	while( pGame->ThreadActivated && pos)
	{	
		CSize Move = IncrementCandidate( depth, pos);	
		
		cand = CalcSelfPrice( depth);

		Assert( cand.IsCorrect());
		
		UpdatePrice( result, cand, Move, pDepthColor[depth], ( depth == 0));
				
		DecrementCandidate();
		
		if( IsWinnerMove( result)) break;
	}

	pCalcResult[depth] = result;

	RemoveVariantList( depth);

	Assert( result.IsCorrect());

	return result;
}

CComputeResult CCompute::CalcPrice( int depth)
{
	CComputeResult cand, result;	

	if( !InitVariantList( depth))// return CalcExtrimePrice( depth);
	{
		Assert( InitExtrimeVariantList( depth));
	}
	
	result.Status = pDepthColor[depth];
	
	POSITION pos = pVariantList[depth].GetHeadPosition();

	Assert( pos != 0);

	while( pGame->ThreadActivated && pos)
	{	
		CSize Move = IncrementCandidate( depth, pos);	
				
		cand = CalcCandidatePrice( depth);
		
		UpdatePrice( result, cand, Move, pDepthColor[depth], ( depth == 0));
		
		DecrementCandidate();
		
		if( IsWinnerMove( result)) break;

		if( !TimeLimit.IsTurnOn()) break;
	}

	pCalcResult[depth] = result;

	RemoveVariantList( depth);

	Assert( result.IsCorrect());

	return result;
}

CSize CCompute::IncrementCandidate( int depth, POSITION& pos)
{
	CSize index = pVariantList[depth].GetNext( pos);	
	
	pGame->UpdateCandidate( index);

	return index;
}

void CCompute::DecrementCandidate()
{
	pGame->DecrementCurrent();
	
	pGame->UpdateUnDo();
}

bool CCompute::IsWinnerMove( CComputeResult& result)
{
	if( result.Status == WhiteStatus && result.Price.cx == 100) return true;

	return ( result.Status == BlackStatus && result.Price.cy == 100);
}

void CCompute::UpdatePrice( CComputeResult& result, CComputeResult& cand, CSize& index, eField Status, bool Rand)
{
	cand.Status = result.Status = Status;

	if( cand.Move.cx == OUT_SIZE && cand.Move.cy == OUT_SIZE)
	{
		Assert( result.Move.cx == OUT_SIZE && result.Move.cy == OUT_SIZE);
		
		return;
	}

	if( cand.Price.cx == 0 && cand.Price.cy == 0)
	{
		Assert( result.Price.cx == 0 && result.Price.cy == 0);
		
		return;
	}

	cand.Move = index;	

	if( Status == WhiteStatus)   // STATUS == WHITE 
	{		
		UpdateWhitePrice( result, cand, index, Rand);
	} 
	else                         // STATUS == BLACK
	{
		UpdateBlackPrice( result, cand, index, Rand);
	} 
}

void CCompute::UpdateWhitePrice( CComputeResult& result, CComputeResult& cand, CSize& index, bool Rand)
{
	if( result.Move.cx == OUT_SIZE)  
	{
		result = cand;	
		
		if( Rand) CandList.AddFirst( cand);
	}
	else if( cand.Price.cx > result.Price.cx)  
	{
		result = cand;;			

		if( Rand) CandList.AddFirst( cand);
	}
	else if( cand.Price.cx == result.Price.cx)
	{
		if( result.Price.cy == 100 && result.Step < cand.Step)
		{
			result = cand;		

			if( Rand) CandList.AddFirst( cand);
		}
		else if ( result.Price.cy != 100)
		{
			if( pGame->ChooseIt && ChooseIt())
			{
				result = cand;		

				if( Rand) CandList.Append( cand);
			}
		}
	}
}

void CCompute::UpdateBlackPrice( CComputeResult& result, CComputeResult& cand, CSize& index, bool Rand)
{
	if( result.Move.cy == OUT_SIZE)  
	{
		result = cand;	
		
		if( Rand) CandList.AddFirst( cand);
	}
	else if( cand.Price.cy > result.Price.cy)  
	{
		result = cand;
		
		if( Rand) CandList.AddFirst( cand);
	}		
	else if( cand.Price.cy == result.Price.cy)
	{
		if( result.Price.cx == 100 && result.Step < cand.Step)
		{
			result = cand;		

			if( Rand) CandList.AddFirst( cand);
		}
		if( result.Price.cx != 100)
		{
			if( pGame->ChooseIt && ChooseIt()) result = cand;;			

			if( Rand) CandList.Append( cand);
		}			
	}	
}

CComputeResult CCompute::CalcSelfPrice( int depth)
{
	CSize index = pGame->MovesList.GetTail();

	CComputeResult result;
	
	CSize price( 0, 0);
	
	if( pGame->Finished())
	{
		( pDepthColor[depth] == WhiteStatus) ? price.cx = 100 : price.cy = 100;		
	}
	else price = pGame->WeightList.GetTail();
	
	result.Set(  index, price, pDepthColor[depth]); 

	return result;
}

CComputeResult CCompute::CalcCandidatePrice( int depth)
{
	CComputeResult calc;

	if( depth == (int)pGame->Depth - 1 || pGame->Finished())
	{
		calc = CalcSelfPrice( depth);
	}		
	else 
	{
		calc = CalcPrice( depth + 1);

		Assert( calc.Price != CSize( 0, 0));
	}	

	calc.Step = depth;

	Assert( calc.IsCorrect());

	return calc;
}

bool CCompute::InitAlgo1( int depth)
{
	if( MyCritical( depth)) return true;

	if( HisCritical( depth)) return true;

	if( HisOpenedThree( depth)) return true;

	if( HisPreCritical( depth)) return true;

	//if( depth == 0)
	{
		Assert( BuildGateList( depth, TOTAL));
		return true;
	}
	
	return false;		
}

bool CCompute::InitAlgo2( int depth)
{
	if( MyCritical( depth)) return true;

	if( HisCritical( depth)) return true;

	bool b1 = HisPreCritical( depth);

	bool b2 = MyPreCritical( depth);

	if( b1 || b2) return true;

	int count = pGame->GetCount();

		if( depth == 0 && count < 12)
		{
			if( ( count%2) && HisPreHard( depth)) return true;

			if( ( count>6) && !( count%2) && HisPreHard( depth)) return true;
		}

//	if( depth == 0)
	{
		Assert( BuildGateList( depth, TOTAL));
	
		return true;
	}

	return false;
}

bool CCompute::InitAlgo3( int depth)
{	
	if( MyCritical( depth)) return true;
	
	if( HisCritical( depth)) return true;
	
	if( MyDoublePreCritical( depth)) return true;
	
	if( HisDoublePreCritical( depth)) return true;
	
	bool b1 = MyPreCritical( depth);
	
	bool b2 = HisPreCritical( depth);	
	
	if( b1 || b2) return true;
	
	if( MyDoubleTwo( depth)) return true;
	
	if( HisDoubleTwo( depth)) return true;

	int count = pGame->GetCount();

		if( depth == 0 && count < 12)
		{
			if( ( count%2) && HisPreHard( depth)) return true;

			if( ( count>6) && !( count%2) && HisPreHard( depth)) return true;
		}
	
	if( Hard( depth)) return true;
	
	if( HisOpenedTwo( depth)) return true;
	
	if( MyOpenedTwo( depth)) return true;		
	
//	if( depth == 0)
	{
		Assert( BuildGateList( depth, TOTAL));
	
		return true;
	}

	return false;
}

bool CCompute::InitAlgo4( int depth)
{	
	if( MyCritical( depth)) return true;
	
	if( HisCritical( depth)) return true;
	
	if( MyDoublePreCritical( depth)) return true;
	
	if( HisDoublePreCritical( depth)) return true;
	
	bool b1 = MyPreCritical( depth);
	
	bool b2 = HisPreCritical( depth);	
	
	if( b1 || b2) return true;
	
	if( MyDoubleTwo( depth)) return true;
	
	if( HisDoubleTwo( depth)) return true;

	int count = pGame->GetCount();

		if( depth == 0 && count < 12)
		{
			if( ( count%2) && HisPreHard( depth)) return true;

			if( ( count>6) && !( count%2) && HisPreHard( depth)) return true;
		}

	
	if( Hard( depth)) return true;
	
	if( HisOpenedTwo( depth)) return true;
	
	if( MyOpenedTwo( depth)) return true;		
	
//	if( depth == 0)
	{
		Assert( BuildGateList( depth, TOTAL));
	
		return true;
	}

	return false;
}

bool CCompute::InitAlgo5(int depth)
{
	bool b1, b2;
	
	if( MyCritical( depth)) return true;

	if( HisCritical( depth)) return true;

	if( MyDoublePreCritical( depth)) return true;

	if( HisDoublePreCritical( depth)) return true;
	
	if( depth < 4)
	{
		b1 = MyPreCritical( depth);
		
		b2 = HisPreCritical( depth);	
		
		if( b1 || b2) return true;

		if( MyDoubleTwo( depth)) return true;

		if( HisDoubleTwo( depth)) return true;
		
		int count = pGame->GetCount();

		if( depth == 0 && count < 12)
		{
			if( ( count%2) && HisPreHard( depth)) return true;

			if( ( count>6) && !( count%2) && HisPreHard( depth)) return true;
		}

		if( MyPreHard( depth)) return true;

		if( HisPreHard( depth)) return true;

		if( b1||b2) return true;

		if( Hard( depth)) return true;

		if( MyOpenedTwo( depth)) return true;		

		if( HisOpenedTwo( depth)) return true;	
	}

	if( depth < 5 && depth >= 4)
	{
		bool b1 = MyPreCritical( depth);
		
		bool b2 = HisPreCritical( depth);	
		
		if( b1 || b2) return true;

		if( MyDoubleTwo( depth)) return true;

		if( HisDoubleTwo( depth)) return true;		

		//if( pGame->GetCount() < 10 && HisOpenedTwo( depth))	return true;

		if( MyOpenedTwo( depth)) return true;	
	}

//	if( depth == 0) 
	{	
		if( MyNotClosedThree( depth)) return true;
		
//		if( depth == 0)
		{
			Assert( BuildGateList( depth, TOTAL));
			
			return true;
		}
	}
	
	return false;
}

void CCompute::WriteToFile( CComputeResult& calc, CString mode)
{
	CString File = "C:\\My Documents\\CrossZero\\Calculate.txt";

	FILE* fp = fopen( File,mode.GetBuffer( 10));

// MOVE to FILE

	CString MoveStrX = utility::IntToString( calc.Move.cx);
	CString MoveStrY = utility::IntToString( calc.Move.cy);
	CString MoveStr = "  Move( " + MoveStrX + ", " + MoveStrY + ";";

// PRICE to FILE

	CString PriceStrX = utility::IntToString( calc.Price.cx);
	CString PriceStrY = utility::IntToString( calc.Price.cy);
	CString PriceStr = "  Price( " + PriceStrX + ", " + PriceStrY + ";";

// DEPTH to FILE

	CString DepthStr = utility::IntToString( calc.Step);
	
    DepthStr = "  Depth( " + DepthStr + "; ";

	CString Append = MoveStr + "  " + PriceStr + "  " + DepthStr;

	fputs( "\n", fp);

	fputs( Append.GetBuffer( 100), fp);

	fclose( fp); 
}

void CCompute::WriteToFile( CString str, CString mode)
{
	CString File = "C:\\My Documents\\CrossZero\\Calculate.txt";

	FILE* fp = fopen( File,mode.GetBuffer( 10));

	fputs( "\n", fp);

	fputs( str.GetBuffer( 200), fp);

	fclose( fp); 
}

CComputeResult CCompute::ChooseCand()
{
	srand( (unsigned)time( NULL ) );

	int count = CandList.GetCount();

	Assert( count != 0);

	count = rand() % count;

	return CandList.GetAt( count);
}

//////////////////////////////////////////////////////////////////////
// CDepthData Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDepthData::CDepthData()
{
	CGameApp* tmp = (CGameApp*)AfxGetApp();
	CComponent* pComponent = &tmp->Component;

	pGame = pComponent->pGame;

	InitDepthData( pGame->Depth);
}

CDepthData::~CDepthData()
{	 
	for( int i = 0; i < Size; i++)
	{		
		if( !pVariantList[i].IsEmpty()) pVariantList[i].RemoveAll();
	}
	
	pVariantList->RemoveAll();

	delete [] pVariantList;

	delete[] pDepthColor;

	delete [] pCalcResult;
}

void CDepthData::InitDepthData(int size)
{	
	Size = size;

	pVariantList = new TVariantList[Size];

	Assert( pVariantList != 0);

	pDepthColor = new eField[Size];

	Assert( pDepthColor != 0);

	pCalcResult = new CComputeResult[Size];

	Assert( pCalcResult != 0);

	CGameApp* tmp = (CGameApp*)AfxGetApp();
	CComponent* pComponent = &tmp->Component;

	eField another = pComponent->pGame->Tbl.FieldsPtrList.GetTail()->Status;;

	if( another == WhiteStatus) OwnerColor = BlackStatus;

	else OwnerColor = WhiteStatus;
	
	bool flag = true;

	for( int i = 0; i < Size; i++)
	{
		if( flag) pDepthColor[i] = OwnerColor;

		else pDepthColor[i] = another;

		flag = !flag;
	}
}




//////////////////////////////////////////////////////////////////////
// CPatternAlgoInfo Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatternAlgoInfo::CPatternAlgoInfo()
{	
}

CPatternAlgoInfo::~CPatternAlgoInfo()
{

}


bool CPatternAlgoInfo::BuildCriticalList( int depth, int type)
{
	bool bCheck = false;

	int len = 5;

	int OwnColor = GetColor( depth);
		
	int AnotherColor = !OwnColor;

	POSITION find = 0;

	switch( type)
	{
	case SHORTEST_TYPE:
	{
		find = BuildCriticalList( depth, len - 2, OwnColor, eClosed, EXCEPT);
		
		if( type == SHORTEST_TYPE) return ( find != NULL);

		else if( find != NULL) bCheck = true;
	}		

	case SHORT_TYPE:
	{
		find = BuildCriticalList(  depth,len - 2, AnotherColor, eClosed, EXCEPT);
		
		if( type == SHORT_TYPE) return ( find != NULL);

		else if( find != NULL) bCheck = true;
	}

	case MIDDLE_TYPE: 
	{
		find = BuildCriticalList(  depth,len - 3, OwnColor, eClosed, EXCEPT);
		
		if( find != NULL) bCheck = true;

		else find = BuildCriticalList(  depth,len - 3, OwnColor, eClosed, EXCEPT);

		if( find != NULL) bCheck = true;

		if( type == MIDDLE_TYPE) return ( find != NULL);

		else if( find != NULL) bCheck = true;
	}

	case LONG_TYPE:
	{
		find = BuildCriticalList(  depth,len - 4, OwnColor, eClosed, EXCEPT);

		if( find != NULL) bCheck = true;
	
		find = BuildCriticalList(  depth,len - 4, AnotherColor, eClosed, EXCEPT);

		if( type == LONG_TYPE) return ( find != NULL);

		else if( find != NULL) bCheck = true;
	}

	case LONGEST_TYPE:	
	{
		find = BuildCriticalList(  depth,len - 5, OwnColor, eClosed, EXCEPT);

		if( find != NULL) bCheck = true;

		find = BuildCriticalList(  depth,len - 5, AnotherColor, eClosed, EXCEPT);

		if( type == LONGEST_TYPE) return ( find != NULL);

		else if( find != NULL) bCheck = true;

		break;
	}
		
	default: bCheck = false;
	};

	return bCheck;
}

POSITION CPatternAlgoInfo::BuildCriticalList( int depth, int NumList, int Color, ePower power, int type, bool check)
{
	if( !ExistsPattern( NumList, Color, power, type)) return NULL;

	CPattern* pOwnTable = pGame->Tbl.P[Color];

	POSITION find;

	if( type == INCLUDE) find = pOwnTable->Find( power, NumList, NULL);

	else find = pOwnTable->FindExcept( power, NumList, NULL);

	POSITION status = find;
	
	while( find)
	{
		CSize pre;

		if( pOwnTable->EntrancePattern( find, NumList, pre))
		{		
			AppendMoveToVariant( depth, pre);
		}
		
		CSize post;
		
		if( pOwnTable->ExitPattern( find, NumList, post))
		{
			AppendMoveToVariant( depth, post);
		}
		
		if( type == INCLUDE) find = pOwnTable->Find( power, NumList, find);

		else find = pOwnTable->FindExcept( power, NumList, find);
	}

	if( pVariantList[depth].GetHeadPosition() != 0) return status;

	else return 0;
}

bool CPatternAlgoInfo::ExistsPattern( int NumList, int Color, ePower power, int type)
{
	return pGame->Tbl.P[Color]->ExistsPattern( NumList, power, type);
}

//////////////////////////////////////////////////////////////////////
// CShellAlgoInfo Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellAlgoInfo::CShellAlgoInfo()
{

}

CShellAlgoInfo::~CShellAlgoInfo()
{

}

bool CShellAlgoInfo::BuildGateList(int depth, int type)
{
	bool status = false;

	TShellList* pList = 0;

	switch( type)
	{
	case WHITE_FOUR_ONLINE:
	
		pList = &pGame->Shell.WhiteCriticalTree;
		break;

	case BLACK_FOUR_ONLINE:
	
		pList = &pGame->Shell.BlackCriticalTree;
		break;

	case WHITE_DOUBLE_THREE_ONLINE:
	
		pList = &pGame->Shell.WhiteDoublePreCriticalTree;
		break;

	case BLACK_DOUBLE_THREE_ONLINE:
	
		pList = &pGame->Shell.BlackDoublePreCriticalTree;
		break;

	case WHITE_DOUBLE_TWO_ONLINE:
	
		pList = &pGame->Shell.WhiteDoubleTwoTree;
		break;

	case BLACK_DOUBLE_TWO_ONLINE:
	
		pList = &pGame->Shell.BlackDoubleTwoTree;
		break;

	case WHITE_THREE_ONLINE:
	
		pList = &pGame->Shell.WhitePreCriticalTree;
		break;

	case BLACK_THREE_ONLINE:
	
		pList = &pGame->Shell.BlackPreCriticalTree;
		break;

	case WHITE_TWO_ONLINE:
	
		pList = &pGame->Shell.WhiteTwoTree;
		break;

	case BLACK_TWO_ONLINE:
	
		pList = &pGame->Shell.BlackTwoTree;
		break;
		
	case HARD:
	
		pList = &pGame->Shell.HardTree;
		break;

	case WHITE_PRE_HARD:
	
		pList = &pGame->Shell.WhitePreHardTree;
		break;

	case BLACK_PRE_HARD:
	
		pList = &pGame->Shell.BlackPreHardTree;
		break;

	case PRE_HARD:
	
		pList = &pGame->Shell.PreHardTree;
		break;

	case DOUBLE_HARD:
	
		pList = &pGame->Shell.DoubleHardTree;
		break;

	case TOTAL:

		pList = &pGame->Shell.TotalTree;
		break;

	default:
		return false;
	};

	POSITION pos = pList->GetHeadPosition();

	if( !pos) return false;

	while( pos) AppendMoveToVariant( depth, pList->GetNext( pos));
	
	Assert( pVariantList[depth].GetHeadPosition() != 0);

	return true;
}


