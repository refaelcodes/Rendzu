// Pattern.h: interface for the CPattern class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERN_H__EC9BAF22_7ED5_11D9_9B3C_DB089894394B__INCLUDED_)
#define AFX_PATTERN_H__EC9BAF22_7ED5_11D9_9B3C_DB089894394B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseTypes.h"

///////////////////////////////////////////////////////////////////////////////
// class CPattern

class CPattern
{
public:	
	class CPowerNumber
	{
	public:
		CPowerNumber();
		
		int wOpened;
		int wHalfOpened;
		int wClosed;
	};

	CPattern();
	
	~CPattern();

	void CreatePatternTable( eField status);

	void FreePatternTable();

	bool ExistsPattern( int NumList, ePower power, int type);
	
	bool EntrancePattern( POSITION pos, int ListNum, CSize& point);

	bool ExitPattern( POSITION pos, int ListNum, CSize& point);

	POSITION Find( CMoveData& id, int ListNum);

	POSITION FindExcept( ePower power, int ListNum, POSITION from);
	
	POSITION Find( ePower power, int ListNum, POSITION from);
	
    void UpdateUnDo( CSize index);

	inline UINT GetWeight(){ return Weight;}
		
	void Delete( CMoveData& id, int ListNum, eDirect direct);
	
	void Insert( CMoveData& id, int ListNum, eDirect direct, ePower power = eOpened);
	
	inline UINT CalcWeight( CMoveData* pid, int ListNum, eDirect direct, ePower power);

	void WriteToFile( eField status);

public:	  // DATA

	CMoveData Id;
	ePower Power;	
	CPowerNumber*   pPowerNumber; 
	TMoveDataList*  pTable;
	UINT            Size;	
	eField          Status;	

	unsigned long   Weight;			
};

//////////////////////////////////////////////////////////////////////////////


class CPatternTables  
{
public:
	void UpdatePatterns( eField status, int flag , CSize index);
	POSITION UpdatePattern( eField status, int flag,
		                    CSize index, eExtDirect ExtDir);
	inline void UpdateNeighbor(eField status , CSize index, int flag)
	{		
		if( status == WhiteStatus)
			UpdatePatterns( BlackStatus, flag, index);
		else
		    UpdatePatterns( WhiteStatus, flag, index);
	}

	void FreePatternsTables();
	void CreatePatternTables();
	CPatternTables();
	virtual ~CPatternTables();

	CPattern*   P[2];
	bool IsEmpty;
};


class CFieldMatrix  
{
public:
	void RemoveMatrix();
	void CleareMatrix();
	void CreateMatrix();
	CFieldMatrix();
	virtual ~CFieldMatrix();

	static CField** F;
	int Dimension;
};

class CTablesDriver: public CPatternTables, public CFieldMatrix
{
public:
	void FreeFieldsPtrList();
	//bool Finished(){ return IsFinished; }
//	void SetFinished( bool state) { IsFinished = state; }
	//bool IsFinished;
	void FreeWinnerList();
	void InitTablesDriver();

	friend class CGameDoc;
	friend class CGameView;
	friend class CGameData;
	friend class CGameManager;

	CTablesDriver();
	~CTablesDriver();
	inline void SetFinished(bool state)
	{ 
		IsFinished = state; 
	}
	inline bool GetFinished(){ return IsFinished; }
	bool UpdateNewMove();
	TFieldsList		 WinnerList;
	TFieldsPtrList FieldsPtrList;
private:
	void BuildWinnerArray( CSize from, CSize to);
	ePower GetPower( CSize from, CSize to, eDirect direct);
	void UpdateUnDoMove( CField& move);
	bool IsFinished;
};






#endif // !defined(AFX_PATTERN_H__EC9BAF22_7ED5_11D9_9B3C_DB089894394B__INCLUDED_)
