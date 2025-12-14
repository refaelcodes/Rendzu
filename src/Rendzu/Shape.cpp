// Shape.cpp: implementation of the CShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "Shape.h"
#include "GameView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameView* CShape::pView = 0;

CShape::CShape()
{

}

CShape::CShape( CMyPoint topLeft, CMyPoint bottomRight):
		CRect( topLeft, bottomRight)
{
}

CShape::~CShape()
{

}

//////////////////////////////////////////////////////////////////////
// CRectangle Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectangle::CRectangle( CMyPoint topLeft, CMyPoint bottomRight,
   					    CLR drawColor, CLR clearColor):
						CShape( topLeft, bottomRight) 
{
	DrawColor = drawColor;
	ClearColor = clearColor;	
}

CRectangle::CRectangle( CLR drawColor, CLR clearColor)							
{
	DrawColor = drawColor;
	ClearColor = clearColor;
	this->left = 0;
	this->right = 400;
	this->top = 0;
	this->bottom = 400;
}

CRectangle::CRectangle()
{
	DrawColor = DrawColorRect;
	ClearColor = ClearColorRect;
	this->left = 50;
	this->right = 400;
	this->top = 100;
	this->bottom = 400;
}

CRectangle::~CRectangle()
{

}

BOOL CRectangle::PointInRect(CPoint &point)
{
	return PtInRect( point);
}

void CRectangle::Draw()
{
	CClientDC* pdc = new CClientDC( pView);

	CBrush* pBrush = new CBrush( DrawColor);
	
	CBrush* oldBrush = pdc->SelectObject( pBrush);
	
	pView->OnPrepareDC( pdc);	

	pdc->FillRect( this, pBrush);

	pdc->SelectObject( oldBrush);

	delete pBrush;
	
	pBrush = 0;

	delete pdc;
}

void CRectangle::Clear()
{
	CClientDC* pdc = new CClientDC( pView);

	CBrush* pBrush = new CBrush( ClearColor);
	
	CBrush* oldBrush = pdc->SelectObject( pBrush);
	
	pView->OnPrepareDC( pdc);	

	pdc->FillRect( this, pBrush);

	pdc->SelectObject( oldBrush);

	delete pBrush;
	
	pBrush = 0;

	delete pdc;
}

//////////////////////////////////////////////////////////////////////
// CLine Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine( CMyPoint from, CMyPoint to,
			 CLR drawColor, CLR clearColor):
			CShape( from, to)
{
	DrawColor = drawColor;
	ClearColor = clearColor;
}

CLine::CLine( CLR drawColor, CLR clearColor):
			CShape(CMyPoint( 70, 100), CMyPoint( 140, 300))
{		
	DrawColor = drawColor;
	ClearColor = clearColor;
}

CLine::CLine(): CShape(CMyPoint( 70, 100), CMyPoint( 140, 300))
{
	DrawColor = DrawColorLine;
	ClearColor = ClearColorLine;
}

CLine::~CLine()
{

}

void CLine::Draw()
{	
	CClientDC* pdc = new CClientDC( pView);

	CPen* pPen = new CPen( PS_SOLID, 2, DrawColor);

	CPen* oldPen = pdc->SelectObject( pPen);

	pView->OnPrepareDC( pdc);	

	pdc->MoveTo( TopLeft());

	pdc->LineTo( BottomRight());

	pdc->SelectObject( oldPen);

	delete pdc;
	delete pPen;
	
	pPen = 0;
}

void CLine::Clear()
{

}


//////////////////////////////////////////////////////////////////////
// CLinesSet Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinesSet::CLinesSet():CShape(CMyPoint( 10, 10), CMyPoint( 1400, 1400)) 
{	
	Dimension = 30;
	DrawColor = DrawColorLine;
	ClearColor = ClearColorLine;
	Step = ( BottomRight().x - TopLeft().x) / Dimension;	
}

CLinesSet::CLinesSet( CMyPoint topLeft, CMyPoint bottomRight,
					 UINT number, CLR drawColor, CLR clearColor):
			CShape( topLeft, bottomRight)
{
	Dimension = number;
	DrawColor = drawColor;
	ClearColor = clearColor;
	Step = ( BottomRight().x - TopLeft().x) / Dimension;
}

CLinesSet::CLinesSet( CMyPoint topLeft, UINT step, UINT number,
					 CLR drawColor, CLR clearColor)
{
	TopLeft() = topLeft;
	BottomRight().x = TopLeft().x + number*step;
	BottomRight().y = TopLeft().y + number*step;
	Dimension = number;
	DrawColor = drawColor;
	ClearColor = clearColor;
	Step = step;
}

CLinesSet::~CLinesSet()
{

}

void CLinesSet::Draw()
{
	CLine line;

	line.DrawColor = DrawColor;
	line.ClearColor = ClearColor;

	line.TopLeft() = TopLeft();	
	line.BottomRight().x = BottomRight().x;
	line.BottomRight().y = TopLeft().y;

	for( UINT i = 0; i <= Dimension; i++)
	{
		line.Draw();

		line.TopLeft().y += Step;
		line.BottomRight().y += Step;
	}

	line.TopLeft() = TopLeft();
	line.BottomRight().x = TopLeft().x;
	line.BottomRight().y = BottomRight().y;

	for( int i = 0; i <= Dimension; i++)
	{
		line.Draw();

		line.TopLeft().x += Step;
		line.BottomRight().x += Step;
	}
}

void CLinesSet::Clear()
{

}


//////////////////////////////////////////////////////////////////////
// CIcon Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIcon::CIcon():
 CShape( CMyPoint(100, 100), CMyPoint( 147, 147))
{
 	IDResource = IDI_ICON1;

	ClearColor = DrawColorRect;

	CMyPoint half( 15, 15);

	Where = CenterPoint() - half;
}

CIcon::CIcon( CPoint where, UINT id, CLR clearColor)			               
{
	IDResource = id;

	ClearColor = clearColor;

	Where = where;
}
			  
void CIcon::Draw()
{
	CClientDC dc( pView);

	CClientDC* pdc = new CClientDC( pView);

	HICON irc = AfxGetApp()->LoadIcon( IDResource);

	BOOL brc = pdc->DrawIcon( Where.x, Where.y, irc);

	delete pdc;
}
			  
void CIcon::Clear()
{
	CPoint tl = (CPoint)Where - CPoint( 1, 1);
	CPoint br = (CPoint)Where + CPoint( 35, 35);

	CRect r( tl, br);

	CClientDC* pdc = new CClientDC( pView);

	CBrush* pBrush = new CBrush( ClearColor);
	
	CBrush* oldBrush = pdc->SelectObject( pBrush);
	
	pdc->FillRect( r, pBrush);

	pdc->SelectObject( oldBrush);

	delete pBrush;
	
	delete pdc;
}

CIcon::~CIcon()
{

}

//////////////////////////////////////////////////////////////////////
// CBoard Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoard::CBoard( UINT number, UINT step, CLR rdraw, CLR sdraw)
{
	Dimension = number;
	Step = step;
	RectDrawColor = CRectangle::DrawColor = rdraw;
	LineDrawColor = CLinesSet::DrawColor = sdraw;

	TopLeft.x = CRectangle::left = CLinesSet::left = 20;
	TopLeft.y = CRectangle::top = CLinesSet::top = 20;


	BottomRight.x = CRectangle::right = CLinesSet::right = 20+Dimension*Step;;
	BottomRight.y = CRectangle::bottom = CLinesSet::bottom = 20+Dimension*Step;

	DocSize.cx = DocSize.cy = Dimension*Step + 100; 

	InitIcons();
}

CBoard::~CBoard()
{

}

void CBoard::SetTopLeft( CMyPoint tl)
{
	TopLeft.x = CRectangle::left = CLinesSet::left = tl.x;
	TopLeft.y = CRectangle::top = CLinesSet::top = tl.y;


	BottomRight.x = CRectangle::right = CLinesSet::right = TopLeft.x+Dimension*Step;;
	BottomRight.y = CRectangle::bottom = CLinesSet::bottom = TopLeft.y+Dimension*Step;
}

void CBoard::Draw()
{
	CRectangle::Draw();

	CLinesSet::Draw();
}

void CBoard::Clear()
{
	CRectangle::Draw();

	CLinesSet::Draw();
}

BOOL CBoard::PointInBoard(CPoint &point)
{
	BOOL r =  CRectangle::PtInRect( point);

	return r;
}

CSize CBoard::GetIndex(CPoint &point)
{
	CSize index;

	index.cx = ( point.x - TopLeft.x) / Step;

	index.cy = ( point.y - TopLeft.y) / Step;

	return index;
}

CMyPoint CBoard::GetFieldCenter( CSize index)
{
	CMyPoint cnt;
	
	cnt.x = TopLeft.x + index.cx * Step + Step / 2;
	cnt.y = TopLeft.y + index.cy * Step + Step / 2;

	CClientDC* pdc = new CClientDC( pView);

	pView->OnPrepareDC( pdc);
		
	pdc->LPtoDP( &cnt);

	return cnt;
}

void CBoard::InitIcons()
{
	iX.IDResource = IDI_ICON2;
	iO.IDResource = IDI_ICON3;
	iHX.IDResource = IDI_ICON4;
	iHO.IDResource = IDI_ICON5;
	iWX.IDResource = IDI_ICON6;
	iWO.IDResource = IDI_ICON7;
	iClear.IDResource = IDI_ICON2;
}

void CBoard::DrawIcon(CSize index, eIcon icon)
{
	CIcon* ic;

	switch( icon)
	{
	case eX:	ic = &iX; break; 
	case eO:	ic = &iO; break;
	case eHX:   ic = &iHX; break;
	case eHO:   ic = &iHO; break;
	case eWX:   ic = &iWX; break;
	case eWO:   ic = &iWO; break;
	default:    ic = &iX; break;
	};

	ic->Where = GetFieldCenter( index) - CPoint( 15, 15);

	ic->Draw();
}

void CBoard::DrawIcon(CSize index, UINT idRes)
{
	CIcon* ic;

	switch( idRes)
	{
	case Xicon:	ic = &iX; break; 
	case Oicon:	ic = &iO; break;
	case HXicon:   ic = &iHX; break;
	case HOicon:   ic = &iHO; break;
	case WXicon:   ic = &iWX; break;
	case WOicon:   ic = &iWO; break;
	default:    ic = &iX; break;
	};

	ic->Where = GetFieldCenter( index) - CPoint( 15, 15);

	ic->Draw();
}

void CBoard::ClearIcon(CSize index)
{
	iClear.Where = GetFieldCenter( index) - CPoint( 15, 15);

	iClear.Clear();
}

CRect CBoard::GetBoard()
{
	CRect r( TopLeft, BottomRight);
	return r;
}

void CBoard::SetDocSize( CSize sz)
{
	
	DocSize = CSize( sz);

	UINT deltaX = BottomRight.x - TopLeft.x;
	UINT deltaY = BottomRight.y - TopLeft.y;

	UINT left = ( sz.cx-deltaX)/2;
	UINT top = ( sz.cy-deltaY)/2;

	SetTopLeft( CMyPoint( top, left));
}

