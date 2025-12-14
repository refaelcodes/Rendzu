// Shape.h: interface for the CShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAPE_H__7DED68C2_7E4E_11D9_9B3C_8FFFE6873E4B__INCLUDED_)
#define AFX_SHAPE_H__7DED68C2_7E4E_11D9_9B3C_8FFFE6873E4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseTypes.h"

/////////////////////////////////////////////////////////////////////


class CGameView;

class CShape: public CRect  
{
public:
	CShape();
	CShape( CMyPoint topLeft, CMyPoint bottomRight);
	virtual ~CShape();
	virtual void Draw()=0;
	virtual void Clear()=0;
	CLR DrawColor;
	CLR ClearColor;
	static CGameView* pView;
};

class CRectangle: public CShape 
{
public:
	BOOL PointInRect( CPoint& point);
	CRectangle( CMyPoint topLeft, CMyPoint bottomRight,
	            CLR drawColor, CLR clearColor);
	CRectangle( CLR drawColor, CLR clearColor);
	CRectangle();
	~CRectangle();
	void Draw();
	void Clear(); 
};

class CLine: public CShape  
{
public:
	CLine( CMyPoint from, CMyPoint to,
		   CLR drawColor, CLR clearColor);
	CLine( CLR drawColor, CLR clearColor);
	CLine();
	~CLine();
	void Draw();
	void Clear(); 
};


class CLinesSet: public CShape   
{
public:
	CLinesSet();
	CLinesSet( CMyPoint topLeft, CMyPoint bottomRight, UINT number,
		       CLR drawColor, CLR clearColor);
	CLinesSet( CMyPoint topLeft, UINT step, UINT number,
		       CLR drawColor, CLR clearColor);
	virtual ~CLinesSet();

	void Draw();
	void Clear();

	UINT Dimension;	
	UINT Step;
};


class CIcon: public CShape  
{
public:
	CIcon();
	CIcon( CPoint where, UINT id, CLR clearColor);
	virtual ~CIcon();
	void Draw();
	void Clear();

	UINT    IDResource;
	CMyPoint Where;
};

class CBoard: public CRectangle, public CLinesSet  
{
public:
	CBoard( UINT number = DIMENSION, UINT step = FIELD_SIZE,
		    CLR rdraw = DrawColorRect, CLR sdraw = DrawColorLine);
	virtual ~CBoard();

	void Draw();   // draw board
	void Clear();  // clear ...

	void ClearIcon(CSize index);
	void DrawIcon( CSize index, eIcon icon); // by given type
	void DrawIcon(CSize index, UINT idRes);  // by resource ID

	void SetTopLeft( CMyPoint tl);  // place the board in client rect 
	                         // according to new top left point
	                           
	CMyPoint GetFieldCenter( CSize index);
	CSize GetIndex(CPoint& point);
	BOOL PointInBoard( CPoint& point);
	CRect GetBoard(); 

	void InitIcons();	// initialize the icons by resource id
	
	CSize GetDocSize(){ return DocSize; }
	void SetDocSize( CSize sz);

	CLR RectDrawColor;  // Board color
	CLR LineDrawColor;  // Lines color

	CMyPoint TopLeft;   // Game board's TopLeft 
	CMyPoint BottomRight;  // and BottomRight

//protected:
	CSize DocSize;

	CIcon iX, iO, iHX, iHO, iWX, iWO, iClear;
};

#endif // !defined(AFX_SHAPE_H__7DED68C2_7E4E_11D9_9B3C_8FFFE6873E4B__INCLUDED_)
