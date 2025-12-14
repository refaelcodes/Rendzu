// Shell.h: interface for the CShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELL_H__EC9BAF27_7ED5_11D9_9B3C_DB089894394B__INCLUDED_)
#define AFX_SHELL_H__EC9BAF27_7ED5_11D9_9B3C_DB089894394B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseTypes.h"



/////////////////////////////////////////////////////////////////////////////
// class CShellPower

class CShellPower
{
public:
	inline CShellPower();
	CShellPower(CShellPower& fp);
	inline CShellPower& operator=( CShellPower& fp);
//	inline CShellPower operator=( CShellPower& fp);
	
	eField Status;
	int Number;
	int Max;
	int InEmpty;
	int OutEmpty;
	ePower Power;
	CSize NearestPoint;
	CSize RemotePoint;
	int Distance;
};




/////////////////////////////////////////////////////////////////////////////
// class CShellBlock

class CShellBlock
{
public:
	inline CShellBlock();
	inline CShellBlock( CShellBlock& block);

	inline void Reset();

	inline void CalcWeight();
	CShellBlock& operator=(CShellBlock& block);

	CShellPower wFP[4];
	CShellPower bFP[4];
	UINT wCount[5];
	UINT bCount[5];

	UINT wTotalCount;
	UINT bTotalCount;

	UINT wOpenedCount[5];
	UINT bOpenedCount[5];

	UINT wHalfCount[5];
	UINT bHalfCount[5];

	UINT wWeight;
	UINT bWeight;
};



/////////////////////////////////////////////////////////////////////////////
// class CShellPoint


class CGameData;

class CShellPoint
{
public:

	inline CShellPoint();
	inline CShellPoint( CGameData* p);
	inline CShellPoint( CShellPoint& f);
	inline CShellPoint( CGameData* p, CSize& index);
	inline void Init( CGameData* p);

	inline CShellPoint operator=( CShellPoint& f);
	
	inline void Reset();
    inline void Build();
	inline void BuildFieldBlock();

//inline
	 inline bool IsBlackDoubleTwo();
	 inline bool IsWhiteDoubleTwo();
	 inline bool IsBlackTwo();
	 inline bool IsWhiteTwo();
	 inline bool IsBlackDoublePreCritical();
	 inline bool IsWhiteDoublePreCritical();
	 inline bool IsHard();
	 inline bool IsBlackHard();
	 inline bool IsWhiteHard();
	 inline bool IsDoubleHard();
	 inline bool IsDoubleBlackHard();
	 inline bool IsDoubleWhiteHard();
	 inline bool IsBlackPreCritical();
	 inline bool IsWhitePreCritical();
	 inline bool IsBlackCritical();
	 inline bool IsWhiteCritical();
	 inline bool IsWhitePreHard();
	 inline bool IsBlackPreHard();
	 inline bool IsPreHard();
// inline
	void WriteToFile();

protected:
	
	inline ePower PowerCheck(UINT x, UINT y, eExtDirect eDir);
	inline ePower PowerCheck( CSize &from, CShellPower& fp, eExtDirect eDir);
	inline ePower PowerCheck(UINT fx, UINT fy, UINT tx, UINT ty, eExtDirect eDir);
	inline ePower PowerCheck( CSize& from, CSize& to, eExtDirect eDir);
	inline ePower GetPower(CSize &index, eDirect Dir);
	inline ePower GetPower( CSize& index, eExtDirect eDir);
	inline void SetExtDir( eDirect Dir, eExtDirect& eDir1, eExtDirect& eDir2);
	inline void SetDir( eDirect& Dir, eExtDirect eDir);
	inline eDirect GetDirect( UINT intDir);
	int Find( ePower power, eField Status, int Number);

	inline CShellPower OnSameStatus( CShellPower& fpower1, CShellPower& fpower2, eExtDirect eDir);
	inline void SetIndex( CSize& index);
	inline CShellPower GetMap( CSize& index, eExtDirect eDir);
	inline void GetMap( CSize& index, eDirect Dir, CShellPower& wFP, CShellPower& bFP );
	inline int GetDistance( CSize& p1, CSize& p2);
	inline void WriteToFile( CShellPower* FP, FILE* fp);
	
	inline void GetRoundShellPoints( TMovesList& rcp, CSize& index);
	inline void GetRoundShellPoints( TMovesList& rcp, CSize& index, eExtDirect eDir);

	inline void Calculate();
	inline void Calculate( eExtDirect eDir);

public:

// DATA

	CSize Index;

	int BlackL;
	int BlackR;
	int BlackT;
	int BlackB;
	int BlackLT;
	int BlackRB;
	int BlackRT;
	int BlackLB;

	int WhiteL;
	int WhiteR;
	int WhiteT;
	int WhiteB;
	int WhiteLT;
	int WhiteRB;
	int WhiteRT;
	int WhiteLB;

	int WhiteSum;
	int BlackSum;

	int WhiteMax;
	int BlackMax;

	int wTotalOpened;
	int wTotalHalf;

	int bTotalOpened;
	int bTotalHalf;

	CField**   F;
	CGameData* pGame;
	int Dimension;

	CShellBlock FieldBlock;
};

class CShell: public CObject
{
public:
	
	class CShellPtr
	{
	public:
		POSITION pos;

		CShellPtr();
	};

	CShell();
	DECLARE_SERIAL( CShell)
	~CShell();
	void Init( CGameData* p);
	void RemoveAll();

	inline void BuildSubjectLists( CShellPoint &cp);
	inline void RemoveSubjectLists();
	inline CSize GetWeight();
	inline void Calculate();
	inline void UpdatePointSquare( CSize& index, bool included);
	inline void CalculateOnDirect( CSize& index, eExtDirect eDir);
	inline bool IsIsolated(CSize index, CSize except);
	inline bool IsIsolated( CSize index);

	void     OnInsertMove( CSize index);
	void     OnDeleteMove( CSize index, eField PrevStatus);
	inline void ShellListAppend( int x, int y, CSize& index);
	inline void ShellListRemove( int x, int y);

	inline bool ShellListRemoveAt( POSITION pos);
	inline bool ShellListRemoveAt( CSize& ind);
	void ShellListRemoveAll();
	inline bool ShellListAddTail( CShellPoint& p);



	void WriteToFile();

// DATA

	CShellPtr ShellArray[DIMENSION][DIMENSION];

	TShellMovesList ShellMovesList;
	CGameData* pGame;
	CField** F;
	int Dimension;
	TShellList TotalTree;
	TShellList HardTree;
	TShellList DoubleHardTree;
	TShellList WhiteCriticalTree;
	TShellList BlackCriticalTree;
	TShellList WhiteDoublePreCriticalTree;
	TShellList BlackDoublePreCriticalTree;
	TShellList WhiteDoubleTwoTree;
	TShellList BlackDoubleTwoTree;
	TShellList WhitePreCriticalTree;
	TShellList BlackPreCriticalTree;
	TShellList WhiteTwoTree;
	TShellList BlackTwoTree;
	TShellList PreHardTree;
	TShellList WhitePreHardTree;
	TShellList BlackPreHardTree;

	CSize Weight;
};
 



#endif // !defined(AFX_SHELL_H__EC9BAF27_7ED5_11D9_9B3C_DB089894394B__INCLUDED_)
