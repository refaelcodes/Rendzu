
#include "stdafx.h"
#include "Game.h"
#include "GameDoc.h"
#include "GameView.h"
#include "Pattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//  class CTablesDriver

CTablesDriver::CTablesDriver()	
{
}

CTablesDriver::~CTablesDriver()
{
	FreeWinnerList();

	FreeFieldsPtrList();
}

void CTablesDriver::InitTablesDriver()
{
	CGameApp* tmp = (CGameApp*)AfxGetApp();

	CComponent* pComponent = &tmp->Component;
}

void CTablesDriver::BuildWinnerArray(CSize from, CSize to)
{
	CSize ind;

	UINT x, y, deltaX, deltaY;

	SetFinished( true);

	if( from.cx < to.cx) deltaX = 1;

	else if( from.cx > to.cx) deltaX = -1;

	else deltaX = 0;

	if( from.cy < to.cy) deltaY = 1;

	else if( from.cy > to.cy) deltaY = -1;

	else deltaY = 0;
		
	bool cont = true;

	eField Status = F[from.cx][from.cy].Status;

	for( x = from.cx, y = from.cy; cont; x += deltaX, y += deltaY)
	{
		ind.cx = x;
		ind.cy = y;

		CField field( ind, Status);
		
		WinnerList.AddTail( field);
		
		if( ( int)x == to.cx && ( int)y == to.cy) cont = false;
	}
}

void CTablesDriver::UpdateUnDoMove(CField& move)
{
	CSize cur = move.Index;

	int num, crt = 0, clb = 0, i = cur.cx, j = cur.cy, dim = Dimension - 1; 
	int delta;

	eField Status = F[cur.cx][cur.cy].Status;

	Assert( Status == move.Status);

	CPattern* pt = ( Status == WhiteStatus ? P[WHITE] : P[BLACK]);

	pt->UpdateUnDo( cur);


//                Update eL part ( = Update R part +  Update L part)								
	

// Update R part	
	
	if( cur.cx + 1 <= dim)
	{
		delta = F[cur.cx+1][cur.cy].l;
		
		for( num = 1; num <= F[cur.cx][cur.cy].r; num++) 
		{		
			F[cur.cx+num][cur.cy].l -= delta;			
		}

		Assert( F[cur.cx+1][cur.cy].l == 0);
	}


// Update L part

	if( cur.cx - 1 >= 0)
	{
		delta = F[cur.cx-1][cur.cy].r;
		
		for( num = 1; num <= F[cur.cx][cur.cy].l; num++) 
		{		
			F[cur.cx-num][cur.cy].r -= delta;
		}

		Assert( F[cur.cx-1][cur.cy].r == 0);
	}

//                Update eT part ( = Update B part +  Update T part)

// Update B part
	
	if( cur.cy + 1 <= dim)
	{
		delta = F[cur.cx][cur.cy+1].t;
	
		for( num = 1; num <= F[cur.cx][cur.cy].b; num++) 
		{		
			F[cur.cx][cur.cy+num].t -= delta;
		}
		
		Assert( F[cur.cx][cur.cy+1].t == 0);
	}

// Update T part
	
	if( cur.cy - 1 >= 0)
	{
		delta = F[cur.cx][cur.cy-1].b;
		
		for( num = 1; num <= F[cur.cx][cur.cy].t; num++) 
		{
			F[cur.cx][cur.cy-num].b -= delta;
		}
		
		Assert( F[cur.cx][cur.cy-1].b == 0);
	}

// Update RB part

	if( cur.cx + 1 <= dim && cur.cy + 1 <= dim)
	{
		delta = F[cur.cx+1][cur.cy+1].lt;

		for( num = 1; num <= F[cur.cx][cur.cy].rb; num++) 
		{			
			F[cur.cx+num][cur.cy+num].lt -= delta;
		}

		Assert( F[cur.cx+1][cur.cy+1].lt == 0);
	}

// Update LT part
	
	if( cur.cx - 1 >= 0 && cur.cy - 1 >= 0)
	{
		delta = F[cur.cx-1][cur.cy-1].rb;
		
		for( num = 1; num <= F[cur.cx][cur.cy].lt; num++) 
		{
			F[cur.cx-num][cur.cy-num].rb -= delta;
		}
		
		Assert( F[cur.cx-1][cur.cy-1].rb == 0);
	}

// Update RT part
	
	if( cur.cx + 1 <= dim && cur.cy - 1 >= 0)
	{
		delta = F[cur.cx+1][cur.cy-1].lb;
		
		for( num = 1; num <= F[cur.cx][cur.cy].rt; num++) 
		{
			F[cur.cx+num][cur.cy-num].lb -= delta;
		}
		
		Assert( F[cur.cx+1][cur.cy-1].lb == 0);
	}

// Update LB part
	
	if( cur.cx - 1 >= 0 && cur.cy + 1 <= dim)
	{
		delta = F[cur.cx-1][cur.cy+1].rt;
		
		for( num = 1; num <= F[cur.cx][cur.cy].lb; num++) 
		{		
			F[cur.cx-num][cur.cy+num].rt -= delta;
		}
		
		Assert( F[cur.cx-1][cur.cy+1].rt == 0);
	}

	F[cur.cx][cur.cy].Reset();
}

bool CTablesDriver::UpdateNewMove()
{
	bool status = false;

	CField f = *FieldsPtrList.GetTail();

	CSize index = f.Index;

	int i = f.Index.cx, j = f.Index.cy, dim = Dimension - 1; 

	int Length =  FIVE;	

	eField Status = F[f.Index.cx][f.Index.cy].Status;

	int sum, delta, x, y = 0;	

	CSize from, to;

	CSize beg, end;

	CPattern* pt = ( Status == WhiteStatus ? P[WHITE] : P[BLACK]);

	bool opened = false;
	bool closed = false;
	ePower power = eClosed;

	CMoveData id;

	int headX, headY;

// LtoR

	headX = i;

	int e = 0, r = 0;

	if( i+1 <= dim && F[i+1][j].Status == Status)
	{
		r = F[i+1][j].r + 1;

		id.Set( i+1, j, eL);
		
		pt->Delete( id, r-1, eL);
		
//pt->WriteToFile( Status);
	}

	if( i-1 >= 0 && F[i-1][j].Status == Status)
	{
		e = F[i-1][j].l + 1;

		headX = i-e;

		id.Set( headX, j, eL);
		
		pt->Delete( id, e-1, eL);

//pt->WriteToFile( Status);
	}

	for( x = i-e, delta = 0, sum = r + e; x <= i+r; x++)
	{
		F[x][j].r = sum--;
		F[x][j].l = delta++;
	}

	if( e + r + 1 >= Length)
	{
		from.cx = i - e;
		from.cy = to.cy = j;
		to.cx = i + r;

		status= true;
		BuildWinnerArray( from, to);
	}

		id.Set( headX, j, eL);
		
		beg.cx = i - e;
		beg.cy = j;
		end.cx = i+r;
		end.cy = j;
		pt->Insert( id, e + r, eL, GetPower( beg, end, eL));
			
 //pt->WriteToFile( Status);

// TtoB

	headY = j;

	int b = 0, t = 0;

	if( j+1 <= dim && F[i][j+1].Status == Status) 
	{
		b = F[i][j+1].b + 1;

		id.Set( i, j+1, eT);
		
		pt->Delete( id, b-1, eT);

//pt->WriteToFile( Status);
	}

	if( j-1 >= 0 && F[i][j-1].Status == Status) 
	{
		t = F[i][j-1].t + 1;

		headY = j-t;
		id.Set( i, headY, eT);
		
		pt->Delete( id, t-1, eT);

//pt->WriteToFile( Status);
	}
	
	for( y = j-t, delta = 0, sum = b + t; y <= j+b; y++)
	{
		F[i][y].b = sum--;
		F[i][y].t = delta++;
	}

	if( t + b + 1 >= Length)
	{
		from.cy = j - t;
		from.cx = to.cx = i;
		to.cy = j + b;

		status= true;
		BuildWinnerArray( from, to);
	}

	id.Set( i, headY, eT);

		beg.cx = i;
		beg.cy = j-t;
		end.cx = i;
		end.cy = j+b;
		pt->Insert( id, t + b, eT, GetPower( beg, end, eT));

//pt->WriteToFile( Status);

// LTtoRB

	headX = i;
	headY = j;

	int rb = 0, lt = 0;

	if( j+1 <= dim && i+1 <=dim && F[i+1][j+1].Status == Status) 
	{
		rb = F[i+1][j+1].rb + 1;

		id.Set( i+1, j+1, eLT);

		pt->Delete( id, rb-1, eLT);

 //pt->WriteToFile( Status);
	}

	if( j-1 >= 0 && i-1 >= 0 && F[i-1][j-1].Status == Status) 
	{
		lt = F[i-1][j-1].lt + 1;

		headX = i-lt;
		headY = j-lt;

		id.Set( headX, headY, eLT);

		pt->Delete( id, lt-1, eLT);

//pt->WriteToFile( Status);
	}

	for( x = i-lt, y = j-lt, sum = rb+lt, delta = 0; y <= j+rb, x <= i+rb; x++, y++)
	{
		F[x][y].rb = sum--; 

		F[x][y].lt = delta++;
	}

	if( rb + lt + 1 >= Length)
	{
		from.cx = i - lt;
		from.cy = j - lt;
		to.cx = i + rb;
		to.cy = j + rb;
		
		status= true;
		BuildWinnerArray( from, to);
	}

	id.Set( headX, headY, eLT);

		beg.cx = i - lt;
		beg.cy = j-lt;
		end.cx = i+rb;
		end.cy = j+rb;
		pt->Insert( id, lt+rb, eLT, GetPower( beg, end, eLT));

//pt->WriteToFile( Status);


// RTtoLB

	headX = i;
	headY = j;

	int lb = 0, rt = 0;

	if( i-1 >= 0 && j+1 <= dim && F[i-1][j+1].Status == Status)
	{
		lb = F[i-1][j+1].lb + 1;

		id.Set( i-1, j+1, eRT);

		pt->Delete( id, lb-1, eRT);

//pt->WriteToFile( Status);
	}

	if( i+1 <= dim && j-1 >=0 && F[i+1][j-1].Status == Status) 
	{
		rt = F[i+1][j-1].rt + 1;

		headX = i+rt;
		headY = j-rt;

		id.Set( headX, headY, eRT);

		pt->Delete( id, rt-1, eRT);

//pt->WriteToFile( Status);
	}

		
	for( x = i + rt, y = j - rt, sum = rt + lb, delta = 0; y <= j + lb, x >= i - lb; x--, y++)
	{
		F[x][y].lb = sum--;

		F[x][y].rt = delta++;
	}

	if( lb + rt + 1 >= Length)
	{
		from.cx = i + rt;
		from.cy = j - rt;
		to.cx = i - lb;
		to.cy = j + lb;
		
		status= true;
		BuildWinnerArray( from, to);
	}

	id.Set( headX ,headY, eRT);

		beg.cx = i+rt;
		beg.cy = j-rt;
		end.cx = i-lb;
		end.cy = j+lb;
		pt->Insert( id, rt+lb, eRT, GetPower( beg, end, eRT));

//pt->WriteToFile( Status);

	return status;
}

ePower CTablesDriver::GetPower(CSize from, CSize to, eDirect direct)
{
	CField f = *FieldsPtrList.GetTail();

	CSize index = f.Index;

	int dim = Dimension - 1; 

	int Length =  FIVE;	

	CSize first, second;

	bool opened = true, closed = false, half = false;

	ePower power = eOpened;

	bool beg = false, end = false;

	switch( direct)
	{
	case eL:
		if( from.cx-1>= 0) beg = ( F[from.cx-1][from.cy].Status == EmptyStatus);

		if( to.cx+1< dim) end = ( F[to.cx+1][to.cy].Status == EmptyStatus);

		break;

	case eT:
		if( from.cy-1>= 0) beg = ( F[from.cx][from.cy-1].Status == EmptyStatus);

		if( to.cy+1< dim) end = ( F[to.cx][to.cy+1].Status == EmptyStatus);

		break;
	
	case eLT:
		if( from.cx-1>= 0 && from.cy-1>= 0) beg = ( F[from.cx-1][from.cy-1].Status == EmptyStatus);

		if( to.cx+1< dim && to.cy+1< dim) end = ( F[to.cx+1][to.cy+1].Status == EmptyStatus);

		break;

	case eRT:
		if( from.cx+1< dim && from.cy-1>= 0) beg = ( F[from.cx+1][from.cy-1].Status == EmptyStatus);

		if( to.cx-1>= 0 && to.cy+1< dim) end = ( F[to.cx-1][to.cy+1].Status == EmptyStatus);

		break;

	default:
		break;
	};

	if( beg && end) power = eOpened;

	else if( !beg && !end) power = eClosed;

	else power = eHalfOpened;

	return power;
}










