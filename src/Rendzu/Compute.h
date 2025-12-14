// Compute.h : interface of the CGameView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPUTE_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
#define AFX_COMPUTE_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_

#include "BaseTypes.h"

///////////////////////////////////////////////////////////////////////////////////
// class CComputeResult

class CComputeResult
{
public:
	inline CComputeResult();
	inline CComputeResult( CComputeResult& calc);

	inline CComputeResult operator=( CComputeResult& calc);
	inline void Set( CSize move, CSize price, eField status);
	
	inline bool IsCorrect() const
	{ 
		return ( Move.cx != OUT_SIZE && Move.cy != OUT_SIZE);
	}

	CSize Move;
	CSize   Price;
	eField  Status;
	int     Step;
};


///////////////////////////////////////////////////////////////////////////////////
// class CCandidate

class CCandidate
{
public:
	inline CCandidate();
	inline ~CCandidate();
	
	inline void Append( CComputeResult& cand);
	inline void AddFirst( CComputeResult& cand);
	
	inline int  GetCount();
	
	void Review();
	
	inline CComputeResult GetAt( int num);
private:

	 TCandList list;
};


////////////////////////////////////////////////////////////////////////
/////////////    class CTimer

class CTimer
{
public:
	CTimer( int max_sec = 10);
	~CTimer();
	void TurnOn();
	void TurnOff();
	inline bool IsTurnOn();

private:

	HANDLE hTime;
	const int MAX_TIME;
	int nTimPerSec;
	LARGE_INTEGER li;
	bool IsOnState;
};

//////////////////////////////////////////////////////////////////////////////////
// class CDepthData

class CDepthData  
{
public:
	CDepthData();

	virtual ~CDepthData();

	inline void RemoveVariantList(int depth)
	{
		if( !pVariantList[depth].IsEmpty()) pVariantList[depth].RemoveAll();
	}
	
	void InitDepthData( int size);

	inline int GetSize() const{ return Size; }

	inline int GetColor( int depth)
	{
		if( pDepthColor[depth] == WhiteStatus) return WHITE;

		else return BLACK;
	}

	inline void AppendMoveToVariant( int depth, CSize& move)
	{
		if( !pVariantList[depth].Find( move))
			pVariantList[depth].AddTail( move);
		
		Assert( !pVariantList[depth].IsEmpty());
	}

	CGameData*       pGame;
	TVariantList*    pVariantList;
	eField*          pDepthColor;
	CComputeResult*  pCalcResult;

private:
	int Size;
	eField OwnerColor;
};


//////////////////////////////////////////////////////////////////////////////////
// class CPatternAlgoInfo


class CPatternAlgoInfo: virtual public CDepthData  
{
public:		
	CPatternAlgoInfo();
	virtual ~CPatternAlgoInfo();
	
	inline bool BuildCriticalList( int depth, int type);
	
	POSITION BuildCriticalList( int depth, int NumList, int Color, ePower power, int type, bool check = false);

	inline bool ExistsPattern( int NumList, int Color, ePower power, int type);
	
	inline bool MyNotClosedFour( int depth)
	{
		bool status = BuildCriticalList( depth, 3, GetColor( depth), eClosed, EXCEPT) != NULL;
		
		return status; 
	}
	
	inline bool HisNotClosedFour( int depth)
	{
		bool status = BuildCriticalList( depth, 3, !GetColor( depth), eClosed, EXCEPT) != NULL;
		
		return status; 
	}
	
	inline bool MyOpenedThree( int depth)
	{
		bool status = BuildCriticalList( depth, 2, GetColor( depth), eOpened, INCLUDE) != NULL;
		
		return status; 
	}
	
	inline bool HisOpenedThree( int depth)
	{
		bool status = BuildCriticalList( depth, 2, !GetColor( depth), eOpened, INCLUDE) != NULL;
		
		return status; 
	}
	
	inline bool MyNotClosedThree( int depth)
	{
		bool status = BuildCriticalList( depth, 2, GetColor( depth), eClosed, EXCEPT) != NULL;
		
		return status; 
	}
	
	inline bool HisNotClosedThree( int depth)
	{
		bool status = BuildCriticalList( depth, 2, !GetColor( depth), eClosed, EXCEPT) != NULL;
		
		return status; 
	}	
};


//////////////////////////////////////////////////////////////////////////////////
// class CShellAlgoInfo


class CShellAlgoInfo : virtual public CDepthData  
{
public:
	
	CShellAlgoInfo();
	virtual ~CShellAlgoInfo();
	
	bool BuildGateList( int depth, int type);

	inline bool HisCritical( int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, BLACK_FOUR_ONLINE);
		
		else status = BuildGateList( depth, WHITE_FOUR_ONLINE);
		
		return status; 
	}
	
	inline bool MyPreCritical( int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus) status = BuildGateList( depth, WHITE_THREE_ONLINE);
		
		else status = BuildGateList( depth, BLACK_THREE_ONLINE);
		
		return status; 
	}
	
	inline bool HisPreCritical( int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, BLACK_THREE_ONLINE);
		
		else status = BuildGateList( depth, WHITE_THREE_ONLINE);
		
		return status; 
	}
	
	inline bool MyPreHard( int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, WHITE_PRE_HARD);
		
		else status = BuildGateList( depth, BLACK_PRE_HARD);
		
		return status; 
	}
	
	inline bool HisPreHard( int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus) status = BuildGateList( depth, BLACK_PRE_HARD);
		
		else status = BuildGateList( depth, WHITE_PRE_HARD);
		
		return status; 
	}
	
	inline bool DoubleHard( int depth)
	{
		return BuildGateList( depth, DOUBLE_HARD); 
	}
	
	inline bool PreHard(int depth)
	{
		return BuildGateList( depth, PRE_HARD); 
	}
	
	inline bool MyOpenedTwo(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, WHITE_TWO_ONLINE);
		
		else status = BuildGateList( depth, BLACK_TWO_ONLINE);
		
		return status; 
	}
	
	inline bool HisOpenedTwo(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, BLACK_TWO_ONLINE);
		
		else status = BuildGateList( depth, WHITE_TWO_ONLINE);
		
		return status; 
	}
	
	inline bool MyDoublePreCritical(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, WHITE_DOUBLE_THREE_ONLINE);
		
		else status = BuildGateList( depth, BLACK_DOUBLE_THREE_ONLINE);
		
		return status; 
	}
	
	inline bool HisDoublePreCritical(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, BLACK_DOUBLE_THREE_ONLINE);
		
		else status = BuildGateList( depth, WHITE_DOUBLE_THREE_ONLINE);
		
		return status; 
	}
	
	inline bool MyDoubleTwo(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, WHITE_DOUBLE_TWO_ONLINE);
		
		else status = BuildGateList( depth, BLACK_DOUBLE_TWO_ONLINE);
		
		return status; 
	}
	
	inline bool HisDoubleTwo(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, BLACK_DOUBLE_TWO_ONLINE);
		
		else status = BuildGateList( depth, WHITE_DOUBLE_TWO_ONLINE);
		
		return status; 
	}
	
	inline bool MyDoubleThree(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, WHITE_DOUBLE_THREE_ONLINE);
		
		else status = BuildGateList( depth, BLACK_DOUBLE_THREE_ONLINE);
		
		return status; 
	}
	
	inline bool HisDoubleThree(int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, BLACK_DOUBLE_THREE_ONLINE);
		
		else status = BuildGateList( depth, WHITE_DOUBLE_THREE_ONLINE);
		
		return status; 
	}
	
	inline bool MyCritical( int depth)
	{
		bool status;
		
		if( pDepthColor[depth] == WhiteStatus)	status = BuildGateList( depth, WHITE_FOUR_ONLINE);
		
		else status = BuildGateList( depth, BLACK_FOUR_ONLINE);
		
		return status; 
	}
	
	inline bool Hard( int depth)
	{
		return BuildGateList( depth, HARD); 
	}
};


//////////////////////////////////////////////////////////////////////////////////
// class CCompute

class CCompute: public CPatternAlgoInfo, public CShellAlgoInfo
{
public:
	CCompute();
	~CCompute();
	CSize CalcPrice();
private:	
		
	inline bool InitExtrimeVariantList( int depth);
	inline bool InitVariantList( int depth);
	
	inline bool InitAlgo1( int depth);
	inline bool InitAlgo2( int depth);
	inline bool InitAlgo3( int depth);
	inline bool InitAlgo4( int depth);
	inline bool InitAlgo5( int depth);


	inline CComputeResult CalcCandidatePrice( int depth);
	inline CComputeResult CalcSelfPrice( int depth);
	inline CComputeResult CalcExtrimePrice( int depth);
	CComputeResult CalcPrice( int depth);

	inline CSize IncrementCandidate( int depth, POSITION& pos);
	inline void DecrementCandidate();

	inline bool IsWinnerMove( CComputeResult& result);

	inline void UpdatePrice( CComputeResult& result, CComputeResult& cand, CSize& index, eField Status, bool Rand = false);
	inline void UpdateWhitePrice( CComputeResult& result, CComputeResult& cand, CSize& index, bool Rand = false);
	inline void UpdateBlackPrice( CComputeResult& result, CComputeResult& cand, CSize& index, bool Rand = false);

	CComputeResult ChooseCand();
		
	void WriteToFile( CComputeResult& calc, CString mode);
	void WriteToFile( CString str, CString mode);

	inline bool ChooseIt()
	{ 
		CTime tm;
		
		tm = CTime::GetCurrentTime();
		
		UINT bf = tm.GetTime();; 
		
		bool b = ( bf%2 == 0);
		
		return b; 
	}

	CCandidate CandList;

	CTimer     TimeLimit;
};



#endif // !defined(AFX_COMPUTE_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
