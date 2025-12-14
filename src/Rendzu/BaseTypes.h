// BaseTypes.h: interface for the BaseTypes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETYPES_H__EC9BAF21_7ED5_11D9_9B3C_DB089894394B__INCLUDED_)
#define AFX_BASETYPES_H__EC9BAF21_7ED5_11D9_9B3C_DB089894394B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "const.h"


//////////////////////////////////////////////////////////////////////////////
//    class CMyPoint

class CMyPoint: public CPoint
{
public:
	CMyPoint();
	CMyPoint( const CMyPoint& point);
	CMyPoint( const CPoint& point);
	CMyPoint( const CSize& size);
	CMyPoint( int xx, int yy);

	CMyPoint& operator=( const CMyPoint& point);
	CMyPoint& operator=( const CPoint& point);
	CMyPoint& operator=( const CSize& size);
	bool operator==( const CMyPoint& point) const;
	bool operator>( const CMyPoint& point) const;
	bool operator>=( const CMyPoint& point) const;
	bool operator<( const CMyPoint& point) const;
	bool operator<=( const CMyPoint& point) const;
	CMyPoint operator+( const CMyPoint& point) const;
	void Set( int xx, int yy);
};

//////////////////////////////////////////////////////////////////////////////
//    class CField

class CField : public CObject
{
public:
	CField();
	CField( CSize index, eField status);
	CField( const CField& field); // <-- FIXED: use const reference

	void Reset();
	void Serialize(CArchive& ar);

	CField& operator=(const CField& field); // <-- FIXED: return reference, use const

	bool operator==(const CField& field) const;
	bool operator!=(const CField& field) const;

	CSize  Index;
	eField Status;	
	BYTE   l;
	BYTE   r;
	BYTE   t;
	BYTE   b;
	BYTE   lb;
	BYTE   rt;
	BYTE   lt;
	BYTE   rb;
};


///////////////////////////////////////////////////////////////////////////////
// class CMoveData

class CMoveData
{
public:	
	CMoveData();
	CMoveData( const CMoveData& indata);


	void Set( UINT cx, UINT cy, eDirect direct);
	int& GetWeightByDirect( eDirect direct);

	CMoveData& operator=(const CMoveData& indata);
	bool operator==(const CMoveData& indata) const;
	bool operator!=(const CMoveData& indata) const;
	bool operator<(const CMoveData& indata) const;

	void SetPower( ePower power);
	ePower GetPower();

	CString DataToString();
	CSize   Index;
	eDirect Direct;
	int     NumList;

	static CGameManager* pManager;
	static CField**      F;

	CGameData*    pGame;
	
	int wT, wL, wLT, wRT;

private:
	ePower Power;
};


class C_Handle
{
public:
	static HANDLE hnd;

	C_Handle(){ 
		hnd = 0; }

	HANDLE& GetHandle(){ return hnd; };
};

//////////////////////////////////////////////////////////////////////////////
//    UTILITIES

namespace utility
{
	inline void Assert( BOOL exp)
	{	
		#ifndef _DEBUG  // debug version in GameView.cpp
		if( exp) return;
		ExitProcess( 0);     
		#endif 
		
		ASSERT( exp);
	};

 CString IntToString( int i);

 int Max( int n1, int n2);
 int Max( int n1, int n2, int n3, int n4);
 CSize WeightSplit( CSize first, CSize second);
 bool CheckSize( int m);
 bool CheckPoint( int x, int y);
 void WeightFormat( CSize& weight, UINT div); 

 void SetDelta( eExtDirect eDir, int& dX, int& dY);  

 CString GetCurTime();

 void ClearString( char* str);

 bool ThreadActive();

};

using namespace utility;


#endif // !defined(AFX_BASETYPES_H__EC9BAF21_7ED5_11D9_9B3C_DB089894394B__INCLUDED_)
