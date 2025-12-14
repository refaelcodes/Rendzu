// Shell.cpp: implementation of the CShell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "Shell.h"
#include "GameDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


class CGameData;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// class CShellPower

CShellPower::CShellPower():NearestPoint( 1000, 1000), RemotePoint( 1000, 1000)
{
	Status = EmptyStatus;
	Number = 0;
	Power = eClosed;
	Max = 0;
	InEmpty = 0;
	OutEmpty = 0;
	Distance = 0;
}

CShellPower::CShellPower( CShellPower& fp)
{
	Status = fp.Status;
	Number = fp.Number;
	Power = fp.Power;
	Max = fp.Max;
	InEmpty = fp.InEmpty;
	OutEmpty = fp.OutEmpty;
	NearestPoint = fp.NearestPoint;
	RemotePoint = fp.RemotePoint;
	Distance = fp.Distance;
}
	
CShellPower& CShellPower::operator=( CShellPower& fp)
{
	if( this != &fp)
	{
		Status = fp.Status;
		Number = fp.Number;
		Power = fp.Power;
		Max = fp.Max;
		InEmpty = fp.InEmpty;
		OutEmpty = fp.OutEmpty;
		NearestPoint = fp.NearestPoint;
		RemotePoint = fp.RemotePoint;
		Distance = fp.Distance;
	}

	return *this;
}



/////////////////////////////////////////////////////////////////////////////
// class CShellBlock

CShellBlock::CShellBlock()
{
	for( int i = 0; i < 5; i++) 
	{
		wCount[i] = 0;
		bCount[i] = 0;
		wOpenedCount[i] = 0;
		wHalfCount[i] = 0;
		bOpenedCount[i] = 0;
		bHalfCount[i] = 0;		
	}

	wTotalCount = 0;
	bTotalCount = 0;

	wWeight = 0;
	bWeight = 0;
}

CShellBlock::CShellBlock( CShellBlock& block)
{
	for( int i = 0; i < 5; i++) 
	{
		wCount[i] = block.wCount[i];
		bCount[i] = block.bCount[i];
		wOpenedCount[i] = block.wOpenedCount[i];
		wHalfCount[i] = block.wHalfCount[i];
		bOpenedCount[i] = block.bOpenedCount[i];
		bHalfCount[i] = block.bHalfCount[i];		
	}

	wTotalCount = block.wTotalCount;
	bTotalCount = block.bTotalCount;

	wWeight = block.wWeight;
	bWeight = block.bWeight;
}

CShellBlock& CShellBlock::operator=( CShellBlock& block)
{
	if( this != &block)
	{
		for( int i = 0; i < 5; i++) 
		{
			wCount[i] = block.wCount[i];
			bCount[i] = block.bCount[i];
			wOpenedCount[i] = block.wOpenedCount[i];
			wHalfCount[i] = block.wHalfCount[i];
			bOpenedCount[i] = block.bOpenedCount[i];
			bHalfCount[i] = block.bHalfCount[i];		
		}
		
		wTotalCount = block.wTotalCount;
		bTotalCount = block.bTotalCount;
		
		wWeight = block.wWeight;
		bWeight = block.bWeight;
	}
	return *this;
}


void CShellBlock::Reset()
{
	CShellPower NullObj;

	int i;

	for( i = 0; i < 4; i++) 
	{
		wFP[i] = NullObj;
		bFP[i] = NullObj;
	}

	for( i = 0; i < 5; i++) 
	{
		wCount[i] = 0;
		bCount[i] = 0;
		wOpenedCount[i] = 0;
		bOpenedCount[i] = 0;
		wHalfCount[i] = 0;
		bHalfCount[i] = 0;
	}

	wTotalCount = 0;
	bTotalCount = 0;

	wWeight = 0;
	bWeight = 0;
}

void CShellBlock::CalcWeight()
{
	for( int i = 1; i < 5; i++)
	{
		switch( i)
		{
		case 1:
			wWeight += wOpenedCount[i]*2;
			bWeight += bOpenedCount[i]*2;

			wWeight += wHalfCount[i];
			bWeight += bHalfCount[i];

			break;

		case 2:
			wWeight += wOpenedCount[i]*4;
			bWeight += bOpenedCount[i]*4;

			wWeight += wHalfCount[i]*3;
			bWeight += bHalfCount[i]*3;

			break;

		case 3:
			wWeight += wOpenedCount[i]*9;
			bWeight += bOpenedCount[i]*9;

			wWeight += wHalfCount[i]*5;
			bWeight += bHalfCount[i]*5;

			break;

		case 4:
			wWeight += wOpenedCount[i]*20;
			bWeight += bOpenedCount[i]*20;

			wWeight += wHalfCount[i]*10;
			bWeight += bHalfCount[i]*10;

			break;
		};
	}
}



/////////////////////////////////////////////////////////////////////////////
// class CShellPoint
CShellPoint::CShellPoint()
{
	Reset();

	Index.cx = 0;
	Index.cy = 0;

	pGame = 0;

	F = 0;

	Dimension = DIMENSION; 
}

void CShellPoint::Init( CGameData* p)
{
	pGame = p;

	F = pGame->Tbl.F;
}

CShellPoint::CShellPoint( CGameData* p)
{
	Reset();

	Index.cx = 0;
	Index.cy = 0;

	pGame = p;

	F = pGame->Tbl.F;

	Dimension = (int) pGame->Tbl.Dimension; 
}

CShellPoint::CShellPoint( CShellPoint& f)
{
	Index = f.Index;

	BlackL = f.BlackL;
	BlackR = f.BlackR;
	BlackT = f.BlackT;
	BlackB = f.BlackB;
	BlackLT = f.BlackLT;
	BlackRB = f.BlackRB;
	BlackRT = f.BlackRT;
	BlackLB = f.BlackLB;

	WhiteL = f.WhiteL;
	WhiteR = f.WhiteR;
	WhiteT = f.WhiteT;
	WhiteB = f.WhiteB;
	WhiteLT = f.WhiteLT;
	WhiteRB = f.WhiteRB;
	WhiteRT = f.WhiteRT;
	WhiteLB = f.WhiteLB;

	WhiteSum = f.WhiteSum;
	BlackSum = f.BlackSum;

	WhiteMax = f.WhiteMax;
	BlackMax = f.BlackMax;

	wTotalOpened = f.wTotalOpened;
	wTotalHalf = f.wTotalHalf;

	bTotalOpened = f.bTotalOpened;
	bTotalHalf = f.bTotalHalf;

	FieldBlock = f.FieldBlock;

	pGame = f.pGame;

	F = pGame->Tbl.F;

	Dimension = (int) pGame->Tbl.Dimension; 
}

CShellPoint::CShellPoint( CGameData* p, CSize& index)
{
	Reset();

	Index = index;

	pGame = p;

	F = pGame->Tbl.F;

	Dimension = (int) pGame->Tbl.Dimension; 
}

CShellPoint CShellPoint::operator=( CShellPoint& f)
{
	if( this != &f)
	{
		Index = f.Index;
		
		BlackL = f.BlackL;
		BlackR = f.BlackR;
		BlackT = f.BlackT;
		BlackB = f.BlackB;
		BlackLT = f.BlackLT;
		BlackRB = f.BlackRB;
		BlackRT = f.BlackRT;
		BlackLB = f.BlackLB;
		
		WhiteL = f.WhiteL;
		WhiteR = f.WhiteR;
		WhiteT = f.WhiteT;
		WhiteB = f.WhiteB;
		WhiteLT = f.WhiteLT;
		WhiteRB = f.WhiteRB;
		WhiteRT = f.WhiteRT;
		WhiteLB = f.WhiteLB;

		WhiteSum = f.WhiteSum;
		BlackSum = f.BlackSum;

		WhiteMax = f.WhiteMax;
		BlackMax = f.BlackMax;

		wTotalOpened = f.wTotalOpened;
		wTotalHalf = f.wTotalHalf;

		bTotalOpened = f.bTotalOpened;
		bTotalHalf = f.bTotalHalf;
		pGame = f.pGame;
	}

	return *this;
}

void CShellPoint::Reset()
{
	BlackL = 0;
	BlackR = 0;
	BlackT = 0;
	BlackB = 0;
	BlackLT = 0;
	BlackRB = 0;
	BlackRT = 0;
	BlackLB = 0;

	WhiteL = 0;
	WhiteR = 0;
	WhiteT = 0;
	WhiteB = 0;
	WhiteLT = 0;
	WhiteRB = 0;
	WhiteRT = 0;
	WhiteLB = 0;

	WhiteSum = 0;
	BlackSum = 0;

	WhiteMax = 0;
	BlackMax = 0;

	wTotalOpened = 0;
	wTotalHalf = 0;

	bTotalOpened = 0;
	bTotalHalf = 0;
}

void CShellPoint::Build()
{
	if( Index.cx == 16 && Index.cy == 16)
	{
		bool stop = true;
	}

	Calculate();

	BuildFieldBlock();

	wTotalOpened = 0;
	wTotalHalf = 0;

	bTotalOpened = 0;
	bTotalHalf = 0;

	for( int i = 1; i < 5; i++)
	{
		wTotalOpened += FieldBlock.wOpenedCount[i]*i;

		bTotalOpened += FieldBlock.bOpenedCount[i]*i;

		wTotalHalf += FieldBlock.wHalfCount[i]*i;

		bTotalHalf += FieldBlock.bHalfCount[i]*i;
	}	
}

void CShellPoint::Calculate( eExtDirect eDir)
{
	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	int x = Index.cx + dX;
	int y = Index.cy + dY;

	if( !utility::CheckPoint( x, y)) return; 

	if( F[x][y].Status == EmptyStatus) return;

	eField Status = F[x][y].Status;

	int len = 0;

	switch( eDir)
	{
	case L:
		len = F[x][y].l + 1;
		( Status == WhiteStatus) ? WhiteL = len : BlackL = len;
		break;

	case R:
		len = F[x][y].r + 1;
		( Status == WhiteStatus) ? WhiteR = len : BlackR = len;
		break;

	case T:
		len = F[x][y].t + 1;
		( Status == WhiteStatus) ? WhiteT = len : BlackT = len;
		break;

	case B:
		len = F[x][y].b + 1;
		( Status == WhiteStatus) ? WhiteB = len : BlackB = len;
		break;

	case LT:
		len = F[x][y].lt + 1;
		( Status == WhiteStatus) ? WhiteLT = len : BlackLT = len;
		break;

	case RB:
		len = F[x][y].rb + 1;
		( Status == WhiteStatus) ? WhiteRB = len : BlackRB = len;
		break;

	case RT:
		len = F[x][y].rt + 1;
		( Status == WhiteStatus) ? WhiteRT = len : BlackRT = len;
		break;

	case LB:
		len = F[x][y].lb + 1;
		( Status == WhiteStatus) ? WhiteLB = len : BlackLB = len;
		break;

	default:
		len = 0;
	};
}

void CShellPoint::Calculate()
{
	Calculate( L);

	Calculate( R);

	Calculate( T);

	Calculate( B);

	Calculate( LT);

	Calculate( RB);

	Calculate( RT);

	Calculate( LB);

	WhiteSum = WhiteL + WhiteR + WhiteT + WhiteB + WhiteLT + WhiteRB + WhiteRT + WhiteLB;

	BlackSum = BlackL + BlackR + BlackT + BlackB + BlackLT + BlackRB + BlackRT + BlackLB;

	WhiteMax = utility::Max( WhiteL + WhiteR, WhiteT + WhiteB, WhiteLT + WhiteRB, WhiteRT + WhiteLB);

	BlackMax = utility::Max( BlackL + BlackR, BlackT + BlackB, BlackLT + BlackRB, BlackRT + BlackLB);
}


ePower CShellPoint::GetPower(CSize &index, eExtDirect eDir)
{
	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	int x = index.cx + dX;
	int y = index.cy + dY;
	
	if( x >= 0 && x < Dimension && y >= 0 && y < Dimension)
	{
		if( F[x][y].Status == EmptyStatus || F[x][y].Status == F[index.cx][index.cy].Status)
			return eOpened;
	}

	return eHalfOpened;
}

ePower CShellPoint::GetPower(CSize &index, eDirect Dir)
{
	eExtDirect eDir1, eDir2;

	SetExtDir( Dir, eDir1, eDir2);

	ePower one = GetPower( index, eDir1);

	ePower two = GetPower( index, eDir2);

	if( one == eOpened && two == eOpened) return eOpened;

	else if( one == eOpened || two == eOpened) return eHalfOpened;

	else return eClosed;
}

ePower CShellPoint::PowerCheck(UINT x, UINT y, eExtDirect eDir)
{
	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	if( !utility::CheckPoint( x+dX, y+dY)) return eHalfOpened;

	else if( F[x+dX][y+dY].Status != EmptyStatus && 
		     F[x+dX][y+dY].Status != F[x][y].Status) return eHalfOpened;

	else if( !utility::CheckPoint( x-dX, y-dY)) return eHalfOpened;

	else if( F[x-dX][y-dY].Status != EmptyStatus &&
		     F[x-dX][y-dY].Status != F[x][y].Status) return eHalfOpened;

	else return eOpened;
}

ePower CShellPoint::PowerCheck(UINT fx, UINT fy, UINT tx, UINT ty, eExtDirect eDir)
{
	ePower pow;

	ePower powFrom = PowerCheck( fx, fy, eDir);

	ePower powTo = PowerCheck( tx, ty, eDir);

	if( powFrom == powTo && powTo == eOpened) pow = eOpened;

	else if( powFrom != powTo) pow = eHalfOpened;

	else pow = eClosed;

	return pow;
}

ePower CShellPoint::PowerCheck( CSize &from, CSize &to , eExtDirect eDir)
{
	return PowerCheck( from.cx, from.cy, to.cx, to.cy, eDir);
}

ePower CShellPoint::PowerCheck( CSize &from, CShellPower& fp, eExtDirect eDir)
{
	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	int len = fp.Max;

	CSize to( from.cx + dX*len, from.cy + dY*len); 

	return PowerCheck( from, to, eDir);
}

int CShellPoint::Find( ePower power, eField Status, int Number)
{
	int count = 0, i = 0;

	UINT* pCount = 0;
	
	if(  Status == WhiteStatus) pCount = &FieldBlock.wOpenedCount[0];
	
	else pCount = &FieldBlock.bOpenedCount[0];

	for( i = Number; i < 5; i++) count += pCount[i];

	if( Status == WhiteStatus) pCount = &FieldBlock.wHalfCount[0];
	
	else pCount = &FieldBlock.bHalfCount[0];

	for( i = Number+1; i < 5; i++) count += pCount[i];
	
	return count;
}

void CShellPoint::SetDir( eDirect& Dir, eExtDirect eDir)
{
	switch( eDir)
	{
	case L:
		Dir = eL;
		break;

	case T:
		Dir = eT;
		break;

	case LT:
		Dir = eLT;
		break;

	case RT:
		Dir = eRT;
		break;

	default:
		Dir = eRT;
		break;
	};
}

void CShellPoint::SetExtDir( eDirect Dir, eExtDirect& eDir1, eExtDirect& eDir2)
{
	switch( Dir)
	{
	case eL:
		eDir1 = L;
		eDir2 = R;
		break;

	case eT:
		eDir1 = T;
		eDir2 = B;
		break;

	case eLT:
		eDir1 = LT;
		eDir2 = RB;
		break;

	case eRT:
		eDir1 = RT;
		eDir2 = LB;
		break;

	default:
		eDir1 = RT;
		eDir2 = LB;
		break;
	};
}

CShellPower CShellPoint::GetMap( CSize& index, eExtDirect eDir)
{
	CShellPower fpower;

	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	int x = index.cx;
	int y = index.cy;

//		             ON EMPTY FIRST FIELDS
	
	while( utility::CheckPoint( x+dX, y+dY) && F[x+dX][y+dY].Status == EmptyStatus)
	{
		x += dX;
		y += dY;

		fpower.Distance++;
		
		fpower.InEmpty++;
		
		fpower.Max++;		

		Assert( fpower.InEmpty + fpower.OutEmpty + fpower.Number == fpower.Max);
		
		if( fpower.InEmpty == 4)
		{
			fpower.Status = EmptyStatus;

			if( !utility::CheckPoint( x+dX, y+dY)) fpower.Power = eHalfOpened;

			else fpower.Power = eOpened;
			
			return fpower;			
		}		
	}	
	
	if( !utility::CheckPoint( x+dX, y+dY))
	{
		fpower.Status = EmptyStatus;

		fpower.Power = eHalfOpened;

		return fpower;
	}

//                     ON EMPTY/NOT EMPTY FIELDS

	fpower.Status = F[x+dX][y+dY].Status;

	fpower.NearestPoint = CSize( x+dX, y+dY);

	fpower.Distance++;
	
	while( fpower.Max < 4 && utility::CheckPoint( x+dX, y+dY))
	{
		x += dX;
		y += dY;

		if( F[x][y].Status == EmptyStatus) fpower.OutEmpty++;
					
		else if( F[x][y].Status == fpower.Status)
		{
			fpower.Number++;

			fpower.RemotePoint = CSize( x, y);
		}		
		else break;
	
		fpower.Max++;
	}

	x = fpower.RemotePoint.cx;
	y = fpower.RemotePoint.cy;

	if( !utility::CheckPoint( x+dX, y+dY))
	{
		fpower.Power = eHalfOpened;

		return fpower;
	}

	if( F[x][y].Status == F[x+dX][y+dY].Status || F[x+dX][y+dY].Status == EmptyStatus)
	{
		fpower.Power = eOpened;

		return fpower;
	}
	
	else fpower.Power = eHalfOpened;
	
	return fpower;
}

void CShellPoint::GetMap( CSize& index, eDirect Dir, CShellPower& wFP, CShellPower& bFP)
{
	CShellPower fpower1, fpower2, NullFP;

	eExtDirect eDir1, eDir2;

	SetExtDir( Dir, eDir1, eDir2);

	fpower1 = GetMap( index, eDir1);

	fpower2 = GetMap( index, eDir2);

	int Max = fpower1.Max + fpower2.Max + 1;

// THE BOTH ARE EMPTY

	if( fpower1.Status == EmptyStatus && fpower2.Status == EmptyStatus)
	{
		wFP = NullFP;
		bFP = NullFP;

		wFP.Max = Max;
		bFP.Max = Max;
	
		return;
	}

// ONE OF TWO IS EMPTY

	else if( fpower1.Status == EmptyStatus)
	{
		fpower2.Max = Max;
		fpower1.Max++;

		if( fpower2.Status == WhiteStatus)
		{
			wFP = fpower2;
			bFP = fpower1;
		}
		else
		{
			wFP = fpower1;
			bFP = fpower2;
		}
		
		return;
	}
	else if( fpower2.Status == EmptyStatus)
	{
		fpower1.Max = Max;
		fpower2.Max++;

		if( fpower1.Status == WhiteStatus)
		{
			wFP = fpower1;
			bFP = fpower2;
		}
		else
		{
			wFP = fpower2;
			bFP = fpower1;
		}		

		return;
	}

// THE BOTH ARE FULL 

	int len;
	int distance = 0;
	
	if( fpower1.Status == fpower2.Status)
	{	
		Max = fpower1.Max + fpower2.Max + 1;

		len = GetDistance( fpower1.RemotePoint, fpower2.RemotePoint)+1;

		if( fpower1.Distance < fpower2.Distance) distance = fpower1.Distance;

		else distance = fpower2.Distance;		

		if( fpower1.Status == WhiteStatus)
		{
			wFP.Max = Max;

			wFP = OnSameStatus( fpower1, fpower2, eDir2);
			
			wFP.Distance = distance;

			wFP.Max = Max;
		
			bFP = NullFP;
		}
		else
		{	
			bFP.Max = Max;

			bFP = OnSameStatus( fpower1, fpower2, eDir2);

			bFP.Distance = distance;

			bFP.Max = Max;
					
			wFP = NullFP;
		}
	}
	else
	{
		fpower1.Max = fpower1.Max + fpower2.InEmpty +1;
		fpower2.Max = fpower2.Max + fpower1.InEmpty +1;

		int InEmpty = fpower1.InEmpty + fpower2.InEmpty +1;

		fpower1.InEmpty = InEmpty;
		fpower2.InEmpty = InEmpty;
		
		if( fpower1.Status == WhiteStatus)
		{
			wFP = fpower1;
			bFP = fpower2;
		}
		else
		{
			wFP = fpower2;
			bFP = fpower1;
		}
	}
}

CShellPower CShellPoint::OnSameStatus( CShellPower& fpower1, CShellPower& fpower2, eExtDirect eDir)
{
	Assert( fpower1.Status == fpower2.Status);

	CShellPower fpower;	

	fpower.Max = fpower1.Max + fpower2.Max + 1;

	fpower.Status = fpower1.Status;

	int len = GetDistance( fpower1.RemotePoint, fpower2.RemotePoint)+1;

	if( fpower1.Distance < fpower2.Distance) fpower.Distance = fpower1.Distance;

	else fpower.Distance = fpower2.Distance;

	if( len <= 5)
	{
		fpower.Number = fpower1.Number + fpower2.Number;

		if( fpower.Max >= 5) 
		{
			fpower.Power = PowerCheck( fpower1.RemotePoint, fpower2.RemotePoint, eDir);
		}

		return fpower;
	}
	
	int lim = len - 4;

	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	for( int i = 0; i < len - 4; i++)
	{
		int count = 0;

		CSize f, t;

		bool first = true;

		for( int j = 0; j < 5; j++)
		{
			int x = fpower1.RemotePoint.cx + dX * ( i + j);
			int y = fpower1.RemotePoint.cy + dY * ( i + j);

			if( F[x][y].Status == fpower.Status)
			{
				count++;	
				
				t.cx = x;
				t.cy = y;

				if( first)
				{
					first = false;
					
					f = t;
				}				
			}
		}

		if( count > fpower.Number)
		{
			fpower.Number = count;

			fpower.Power = PowerCheck( f, t, eDir);
		}

		else if( count == fpower.Number)
		{
			ePower prev = fpower.Power;

			fpower.Power = PowerCheck( f, t, eDir);

			if( fpower.Power == eOpened) prev = fpower.Power;

			else if( fpower.Power == eHalfOpened && prev == eClosed) prev = fpower.Power;

			else fpower.Power = prev;
		}
	}

	return fpower;
}

eDirect CShellPoint::GetDirect( UINT intDir)
{
	if( intDir == intL) return eL;

	if( intDir == intT) return eT;

	if( intDir == intLT) return eLT;

	else return eRT;
}

void CShellPoint::BuildFieldBlock()
{
	for( int k = 0; k < 5; k++)
	{
		FieldBlock.wOpenedCount[k] = 0;
		FieldBlock.bOpenedCount[k] = 0;
		FieldBlock.wCount[k] = 0;
		FieldBlock.bCount[k] = 0;
		FieldBlock.wHalfCount[k] = 0;
		FieldBlock.bHalfCount[k] = 0;
	}

	for( UINT i = intL; i <= intRT; i++)
	{
		eDirect Dir = GetDirect( i);

		GetMap( Index, Dir, FieldBlock.wFP[i], FieldBlock.bFP[i]);

		if( FieldBlock.wFP[i].Max >= 5)
		{
			FieldBlock.wCount[FieldBlock.wFP[i].Number]++;

			FieldBlock.wTotalCount += FieldBlock.wFP[i].Number;

			if( FieldBlock.wFP[i].Power == eOpened)
			{
				FieldBlock.wOpenedCount[FieldBlock.wFP[i].Number]++;
			}
			else 
			{
				FieldBlock.wHalfCount[FieldBlock.wFP[i].Number]++;
			}
				
		}
		
		if( FieldBlock.bFP[i].Max >= 5)
		{
			FieldBlock.bCount[FieldBlock.bFP[i].Number]++;

			FieldBlock.bTotalCount += FieldBlock.bFP[i].Number;

			if( FieldBlock.bFP[i].Power == eOpened) 
			{
				FieldBlock.bOpenedCount[FieldBlock.bFP[i].Number]++;
			}
			else FieldBlock.bHalfCount[FieldBlock.bFP[i].Number]++;
		}
	}
}

void CShellPoint::WriteToFile( CShellPower* FP, FILE* fp)
{
	CString str, tmp1, tmp2;
	int i,j;

	fputs( "\n\n--------------------------------------------------------------", fp);
	
	if( FP == &FieldBlock.wFP[0]) fputs( "\n\t White:\n", fp);

	else fputs( "\n\t Black:\n", fp);

	fputs( "\n--------------------------------------------------------------", fp);

	if( FP == &FieldBlock.wFP[0]) str = utility::IntToString( FieldBlock.wTotalCount);

	else str = utility::IntToString( FieldBlock.bTotalCount);

	str = tmp1 + str;

	fputs( "\n\t Total Count = ", fp);

	fputs( str.GetBuffer( 100), fp);	
	
	for( i = 0; i < 4; i++)
	{		
		fputs( "\n\n\tdirect: ", fp);

		if( i == 0) fputs( "L", fp);

		else if( i == 1) fputs( "T", fp);

		else if( i == 2) fputs( "LT", fp);

		else if( i == 3) fputs( "RT", fp);

		fputs( ",\n\tMax = ", fp);

		str = utility::IntToString( FP[i].Max);

		fputs( str.GetBuffer( 100), fp);	
		
		fputs( ",\n\tNumber = ", fp);

		str = utility::IntToString( FP[i].Number);

		fputs( str.GetBuffer( 100), fp);

		switch( FP[i].Power)
		{
		case eOpened: str = "eOpened"; break;
			
		case eHalfOpened: str = "eHalfOpened"; break;
			
		case eClosed: str = "eClosed"; break;
		};

		fputs( ",\n\tPower = ", fp);

		fputs( str.GetBuffer( 100), fp);

		tmp1 = utility::IntToString( FP[i].NearestPoint.cx);
		tmp2 = utility::IntToString( FP[i].NearestPoint.cy);

		str = "\n\tNearestPoint( " + tmp1 + ", " + tmp2 + ")";

		fputs( str.GetBuffer( 100), fp);

		tmp1 = utility::IntToString( FP[i].RemotePoint.cx);
		tmp2 = utility::IntToString( FP[i].RemotePoint.cy);

		str = "\n\tRemotePoint( " + tmp1 + ", " + tmp2 + ")";

		fputs( str.GetBuffer( 100), fp);

		tmp1 = utility::IntToString( FP[i].Distance);

		str = "\n\tDistance = " + tmp1;

		fputs( str.GetBuffer( 100), fp);
	}

	fputs( "\n\n", fp);

	for( j = 0; j <= 4; j++)
	{
		fputs( "\n\tCount[", fp);
		
		str = utility::IntToString( j);
		
		fputs( str.GetBuffer( 100), fp);
		
		fputs( "] = ", fp);
		
		if( FP == &FieldBlock.wFP[0]) str = utility::IntToString( FieldBlock.wCount[j]);

		else str = utility::IntToString( FieldBlock.bCount[j]);
		
		fputs( str.GetBuffer( 100), fp);
	}
	
	for( j = 0; j <= 4; j++)
	{
		fputs( "\n\tOpenedCount[", fp);
		
		str = utility::IntToString( j);
		
		fputs( str.GetBuffer( 100), fp);
		
		fputs( "] = ", fp);
		
		if( FP == &FieldBlock.wFP[0]) str = utility::IntToString( FieldBlock.wOpenedCount[j]);
		
		else str = utility::IntToString( FieldBlock.bOpenedCount[j]);
		
		fputs( str.GetBuffer( 100), fp);
	}

	for( j = 0; j <= 4; j++)
	{
		fputs( "\n\tHalfCount[", fp);
		
		str = utility::IntToString( j);
		
		fputs( str.GetBuffer( 100), fp);
		
		fputs( "] = ", fp);
		
		if( FP == &FieldBlock.wFP[0]) str = utility::IntToString( FieldBlock.wHalfCount[j]);
		
		else str = utility::IntToString( FieldBlock.bHalfCount[j]);
		
		fputs( str.GetBuffer( 100), fp);
	}
}

void CShellPoint::WriteToFile()
{
	CString File = "C:\\My Documents\\CrossZero\\FieldBlock.txt";
	
	FILE* fp = fopen( File.GetBuffer( 100), "w");

	Assert( fp != 0);

	CString tmp1, tmp2, str;

	tmp1 = utility::IntToString( Index.cx);
	tmp2 = utility::IntToString( Index.cy);

	str = "\n\t\tIndex( " + tmp1 + ", " + tmp2 + ")";

	fputs( "\n\t\t", fp);

	fputs( str.GetBuffer( 100), fp);

	CShellPower* FP = &FieldBlock.wFP[0];

//	WriteToFile( &FieldBlock.wFP[0], fp);

//	WriteToFile( &FieldBlock.bFP[0], fp);

	fclose( fp);
}

int CShellPoint::GetDistance(CSize &p1, CSize &p2)
{
	int xd = abs( p1.cx - p2.cx);
	int yd = abs( p1.cy - p2.cy);

	return utility::Max( xd, yd);
}


bool CShellPoint::IsWhiteCritical()
{
	if( FieldBlock.wCount[4] > 0) return true;

	else return false;
}

bool CShellPoint::IsBlackCritical()
{
	if( FieldBlock.bCount[4] > 0) return true;

	else return false;
}

bool CShellPoint::IsWhitePreCritical()
{
	if( FieldBlock.wOpenedCount[3] > 0) return true;

	if( FieldBlock.wHalfCount[3] > 1) return true;

	if( FieldBlock.wOpenedCount[2] > 1) return true;

	if( FieldBlock.wOpenedCount[2] > 0 && FieldBlock.wHalfCount[3] > 0) return true;

	if( FieldBlock.wOpenedCount[1] > 3) return true;
	
	else return false;
}

bool CShellPoint::IsBlackPreCritical()
{
	if( FieldBlock.bOpenedCount[3] > 0) return true;

	if( FieldBlock.bHalfCount[3] > 1) return true;

	if( FieldBlock.bOpenedCount[2] > 1) return true;

	if( FieldBlock.bOpenedCount[2] > 0 && FieldBlock.bHalfCount[3] > 0) return true;

	if( FieldBlock.bOpenedCount[1] > 3) return true;
	
	else return false;
}

bool CShellPoint::IsWhitePreHard()
{
	if( FieldBlock.wHalfCount[3] > 0) return true; // 10/04/04

	if( wTotalOpened >= 2  && BlackSum == 0) return true;
	
	else if( IsWhitePreCritical() && BlackSum == 0 && WhiteSum >= 3) return true;

	else return false;
}

bool CShellPoint::IsBlackPreHard()
{
	if( FieldBlock.bHalfCount[3] > 0) return true; // 10/04/04

	if( bTotalOpened >= 2  && WhiteSum == 0) return true;
		
	else if( IsBlackPreCritical() && WhiteSum == 0 && BlackSum >= 3) return true;

	else return false;
}

bool CShellPoint::IsPreHard()
{
	return ( IsWhitePreHard() || IsBlackPreHard());
}

bool CShellPoint::IsWhiteHard()
{
	if( WhiteSum < 3) return false;

	if( wTotalOpened >= 3) return true;

	else if( wTotalOpened >= 2 && wTotalHalf >=3) return true;

	else if( wTotalOpened >= 1 && wTotalHalf >=5) return true;

	else return false;
}

bool CShellPoint::IsBlackHard()
{
	if( BlackSum < 3) return false;

	if( bTotalOpened >= 3) return true;

	else if( bTotalOpened >= 2 && bTotalHalf >=3) return true;

	else if( bTotalOpened >= 1 && bTotalHalf >=5) return true;

	else return false;
}

bool CShellPoint::IsHard()
{
	if ( IsWhiteHard() || IsBlackHard()) return true;

	return false;
}

bool CShellPoint::IsDoubleWhiteHard()
{
	if( WhiteSum < 4) return false;

	if( wTotalOpened >= 4) return true;

	else if( wTotalOpened >= 3 && wTotalHalf >=2) return true;

	else if( wTotalOpened >= 2 && wTotalHalf >=3) return true;

	else return false;
}

bool CShellPoint::IsDoubleBlackHard()
{
	if( BlackSum < 4) return false;

	if( bTotalOpened >= 4) return true;

	else if( bTotalOpened >= 3 && bTotalHalf >=2) return true;

	else if( bTotalOpened >= 2 && bTotalHalf >=3) return true;

	else return false;
}

bool CShellPoint::IsDoubleHard()
{
	return ( IsDoubleWhiteHard() || IsDoubleBlackHard());
}

bool CShellPoint::IsWhiteDoublePreCritical()
{
	if( FieldBlock.wOpenedCount[3] > 1) return true;

	else return false;
}

bool CShellPoint::IsBlackDoublePreCritical()
{
	if( FieldBlock.bOpenedCount[3] > 1) return true;

	else return false;
}

bool CShellPoint::IsWhiteDoubleTwo()
{
	return ( Find( eOpened, WhiteStatus, 2) > 1);
}

bool CShellPoint::IsBlackDoubleTwo()
{
	return ( Find( eOpened, BlackStatus, 2) > 1);
}

bool CShellPoint::IsWhiteTwo()
{
	return ( Find( eOpened, WhiteStatus, 2) > 0);
}

bool CShellPoint::IsBlackTwo()
{
	return ( Find( eOpened, BlackStatus, 2) > 0);
}

void CShellPoint::GetRoundShellPoints( TMovesList& rcp, CSize& index, eExtDirect eDir)
{
	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	CSize cand( index);

	for( int i = 1; i < 6; i++)
	{
		cand.cx += dX;
		cand.cy += dY;

		if( !utility::CheckPoint( cand.cx, cand.cy)) return;

		if( F[cand.cx][cand.cy].Status == EmptyStatus) rcp.AddTail( cand); 
	}
}

void CShellPoint::GetRoundShellPoints( TMovesList& rcp, CSize& index)
{
	if( !rcp.IsEmpty()) rcp.RemoveAll();

// L

	GetRoundShellPoints( rcp, index, L);

// R

	GetRoundShellPoints( rcp, index, R);

// T

	GetRoundShellPoints( rcp, index, T);

// B

	GetRoundShellPoints( rcp, index, B);

// LT

	GetRoundShellPoints( rcp, index, LT);

// RB

	GetRoundShellPoints( rcp, index, RB);

// RT

	GetRoundShellPoints( rcp, index, RT);

// LB

	GetRoundShellPoints( rcp, index, LB);
}



/////////////////////////////////////////////////////////////////////////////
// class CShell

CShell::CShellPtr::CShellPtr(){ pos = 0; }

IMPLEMENT_SERIAL( CShell, CObject, 1)

CShell::CShell()
{
	F = 0;
}

CShell::~CShell()
{
	RemoveAll();
}

void CShell::Init( CGameData* p)
{
	pGame = p;

	F = pGame->Tbl.F;

	Dimension = (int)pGame->Tbl.Dimension;
}

void CShell::RemoveAll()
{
	ShellListRemoveAll();

	if( !TotalTree.IsEmpty()) TotalTree.RemoveAll();

	if( !HardTree.IsEmpty()) HardTree.RemoveAll();

	if( !PreHardTree.IsEmpty()) PreHardTree.RemoveAll();

	if( !WhitePreHardTree.IsEmpty()) WhitePreHardTree.RemoveAll();

	if( !BlackPreHardTree.IsEmpty()) BlackPreHardTree.RemoveAll();

	if( !PreHardTree.IsEmpty()) PreHardTree.RemoveAll();

	if( !DoubleHardTree.IsEmpty()) DoubleHardTree.RemoveAll();

	if( !WhiteCriticalTree.IsEmpty()) WhiteCriticalTree.RemoveAll();

	if( !BlackCriticalTree.IsEmpty()) BlackCriticalTree.RemoveAll();

	if( !WhiteDoublePreCriticalTree.IsEmpty()) WhiteDoublePreCriticalTree.RemoveAll();

	if( !BlackDoublePreCriticalTree.IsEmpty()) BlackDoublePreCriticalTree.RemoveAll();

	if( !WhiteDoubleTwoTree.IsEmpty()) WhiteDoubleTwoTree.RemoveAll();

	if( !BlackDoubleTwoTree.IsEmpty()) BlackDoubleTwoTree.RemoveAll();

	if( !WhiteTwoTree.IsEmpty()) WhiteTwoTree.RemoveAll();

	if( !BlackTwoTree.IsEmpty()) BlackTwoTree.RemoveAll();

	if( !WhitePreCriticalTree.IsEmpty()) WhitePreCriticalTree.RemoveAll();

	if( !BlackPreCriticalTree.IsEmpty()) BlackPreCriticalTree.RemoveAll();
}

void CShell::ShellListAppend( int x, int y, CSize& index)
{
	if( !utility::CheckPoint( x, y)) return;

	if( ShellArray[x][y].pos == 0 && F[x][y].Status == EmptyStatus)
	{
		CSize cand( x, y);
	
		CShellPoint p( pGame, cand);
		
		ShellListAddTail( p);
	}
}

bool CShell::ShellListAddTail( CShellPoint& p)
{
	if( ShellArray[p.Index.cx][p.Index.cy].pos == 0)
	{
		ShellMovesList.AddTail( p);
		
		ShellArray[p.Index.cx][p.Index.cy].pos = ShellMovesList.GetTailPosition();
		
		return true;
	}
	
	return false;
}

bool CShell::ShellListRemoveAt( CSize& ind)
{
	POSITION pos = ShellArray[ind.cx][ind.cy].pos;

	if( !pos) return false;

	ShellArray[ind.cx][ind.cy].pos = 0;

	ShellMovesList.RemoveAt( pos);

	return true;
}

bool CShell::ShellListRemoveAt( POSITION pos)
{
	CShellPoint& p = ShellMovesList.GetAt( pos);

	p.Init( pGame);

	Assert( ShellArray[p.Index.cx][p.Index.cy].pos != 0);

	ShellArray[p.Index.cx][p.Index.cy].pos = 0;

	ShellMovesList.RemoveAt( pos);

	return true;
}

void CShell::ShellListRemoveAll()
{
	POSITION pos = ShellMovesList.GetHeadPosition();

	while( pos) 
	{
		CShellPoint p( ShellMovesList.GetNext( pos));

		ShellListRemoveAt( p.Index);
	}
}

void CShell::OnInsertMove( CSize index)
{
	ShellListRemoveAt( index);
	
	CSize cand;

// L 

	ShellListAppend( index.cx-1, index.cy, index);

// R

	ShellListAppend( index.cx+1, index.cy, index);

// T 

	ShellListAppend( index.cx, index.cy-1, index);

// B 
	ShellListAppend( index.cx, index.cy+1, index);
	
// LT 

	ShellListAppend( index.cx-1, index.cy-1, index);
	
// RB 

	ShellListAppend( index.cx+1, index.cy+1, index);

// RT 

	ShellListAppend( index.cx+1, index.cy-1, index);

// RB 

	ShellListAppend( index.cx-1, index.cy+1, index);
	
//////////////////////////////////////////

	UpdatePointSquare( index, false);
	Calculate();

//////////////////////////////////////////

//	WriteToFile();
}

void CShell::ShellListRemove( int x, int y)
{
	if( !utility::CheckPoint( x, y)) return;

	CSize cand( x, y);
	
	if( IsIsolated( cand) && F[x][y].Status == EmptyStatus)
	{
		Assert( ShellListRemoveAt( cand));
	}
}

void CShell::OnDeleteMove( CSize index, eField PrevStatus)
{
	bool included = false;

	if( !IsIsolated( index))
	{
		CShellPoint p( pGame, index);

		ShellListAddTail( p);

		included = true;

//		WriteToFile();
	}

// L 

	ShellListRemove( index.cx-1, index.cy);

// R

	ShellListRemove( index.cx+1, index.cy);

// T 

	ShellListRemove( index.cx, index.cy-1);

// B 

	ShellListRemove( index.cx, index.cy+1);
	
// LT 

	ShellListRemove( index.cx-1, index.cy-1);
	
// RB 

	ShellListRemove( index.cx+1, index.cy+1);

// RT 

	ShellListRemove( index.cx+1, index.cy-1);

// RB 

	ShellListRemove( index.cx-1, index.cy+1);

////////////////////////////////////

	UpdatePointSquare( index, included);
	Calculate();

////////////////////////////////////

//	WriteToFile();
}

void CShell::WriteToFile()
{
return;
	CString File = "C:\\My Documents\\CrossZero\\Shell.txt";

	FILE* fp = fopen( File.GetBuffer( 100), "w");

	Assert( fp != 0);

	POSITION find = ShellMovesList.GetHeadPosition();

	while( find)
	{
		CShellPoint& f = ShellMovesList.GetAt( find);

		f.Init( pGame);
		
		CSize ms = f.Index;

		CSize index = f.Index;

		CString IndexStr = " Index( " + utility::IntToString( index.cx) + "," + utility::IntToString( index.cy);

		CString WhiteT = "  T= " + utility::IntToString( f.WhiteT);
		CString WhiteB = "  B= " + utility::IntToString( f.WhiteB);
		CString WhiteL = "  L= " + utility::IntToString( f.WhiteL);
		CString WhiteR = "  R= " + utility::IntToString( f.WhiteR);
		CString WhiteLT = "  LT= " + utility::IntToString( f.WhiteLT);
		CString WhiteRB = "  RB= " + utility::IntToString( f.WhiteRB);
		CString WhiteRT = "  RT= " + utility::IntToString( f.WhiteRT);
		CString WhiteLB = "  LB= " + utility::IntToString( f.WhiteLB);

		CString BlackT = "  T= " + utility::IntToString( f.BlackT);
		CString BlackB = "  B= " + utility::IntToString( f.BlackB);
		CString BlackL = "  L= " + utility::IntToString( f.BlackL);
		CString BlackR = "  R= " + utility::IntToString( f.BlackR);
		CString BlackLT = "  LT= " + utility::IntToString( f.BlackLT);
		CString BlackRB = "  RB= " + utility::IntToString( f.BlackRB);
		CString BlackRT = "  RT= " + utility::IntToString( f.BlackRT);
		CString BlackLB = "  LB= " + utility::IntToString( f.BlackLB);

		CString Total = IndexStr + "-> White:";
		Total += WhiteT;
		Total += WhiteB;
		Total += WhiteL;
		Total += WhiteR;
		Total += WhiteLT;
		Total += WhiteRB;
		Total += WhiteRT;
		Total += WhiteLB;

		fputs( Total.GetBuffer( 500), fp);

		Total = IndexStr + "-> Black:";
		Total += BlackT;
		Total += BlackB;
		Total += BlackL;
		Total += BlackR;
		Total += BlackLT;
		Total += BlackRB;
		Total += BlackRT;
		Total += BlackLB;

		fputs( "\n", fp);

		fputs( Total.GetBuffer( 500), fp);

		fputs( "\n", fp);
		fputs( "\n", fp);

		ShellMovesList.GetNext( find);
	}
		
	fclose( fp);
}


///////////////////////////////////////////////////////////////////////////////////
// class CShell


bool CShell::IsIsolated(CSize index)
{
// L
	if( index.cx-1 >= 0)
	{
		if( F[index.cx-1][index.cy].Status != EmptyStatus)
			return false;
	}

// R
	if( index.cx+1 < Dimension)
	{
		if( F[index.cx+1][index.cy].Status != EmptyStatus)
			return false;
	}

// T
	if( index.cy-1 >= 0)
	{
		if( F[index.cx][index.cy-1].Status != EmptyStatus)
			return false;
	}

// B
	if( index.cy+1 < Dimension)
	{
		if( F[index.cx][index.cy+1].Status != EmptyStatus)
			return false;
	}

// LT
	if( index.cx-1 >= 0 && index.cy-1 >= 0)
	{
		if( F[index.cx-1][index.cy-1].Status != EmptyStatus)
			return false;
	}

// RB
	if( index.cx+1 < Dimension && index.cy+1 < Dimension)
	{
		if( F[index.cx+1][index.cy+1].Status != EmptyStatus)
			return false;
	}

// RT
	if( index.cx+1 < Dimension && index.cy-1 >= 0)
	{
		if( F[index.cx+1][index.cy-1].Status != EmptyStatus)
			return false;
	}

// LB
	if( index.cx-1 >= 0 && index.cy+1 < Dimension)
	{
		if( F[index.cx-1][index.cy+1].Status != EmptyStatus)
			return false;
	}

	return true;
}


bool CShell::IsIsolated(CSize index, CSize except)
{
	bool status = false;

// L
	if( index.cx-1 >= 0)
	{
		status = ( index.cx-1 == except.cx && index.cy == except.cy);

		if( F[index.cx-1][index.cy].Status != EmptyStatus && !status)
			return false;
	}

// R
	if( index.cx+1 < Dimension)
	{
		status = ( index.cx+1 == except.cx && index.cy == except.cy);

		if( F[index.cx+1][index.cy].Status != EmptyStatus && !status)
			return false;
	}

// T
	if( index.cy-1 >= 0)
	{
		status = ( index.cx == except.cx && index.cy-1 == except.cy);

		if( F[index.cx][index.cy-1].Status != EmptyStatus && !status)
			return false;
	}

// B
	if( index.cy+1 < Dimension)
	{
		status = ( index.cx == except.cx && index.cy+1 == except.cy);

		if( F[index.cx][index.cy+1].Status != EmptyStatus && !status)
			return false;
	}

// LT
	if( index.cx-1 >= 0 && index.cy-1 >= 0)
	{
		status = ( index.cx-1 == except.cx && index.cy-1 == except.cy);
		
		if( F[index.cx-1][index.cy-1].Status != EmptyStatus && !status)
			return false;
	}

// RB
	if( index.cx+1 < Dimension && index.cy+1 < Dimension)
	{
		status = ( index.cx+1 == except.cx && index.cy+1 == except.cy);
		
		if( F[index.cx+1][index.cy+1].Status != EmptyStatus && !status)
			return false;
	}

// RT
	if( index.cx+1 < Dimension && index.cy-1 >= 0)
	{
		status = ( index.cx+1 == except.cx && index.cy-1 == except.cy);

		if( F[index.cx+1][index.cy-1].Status != EmptyStatus && !status)
			return false;
	}

// LB
	if( index.cx-1 >= 0 && index.cy+1 < Dimension)
	{
		status = ( index.cx-1 == except.cx && index.cy+1 == except.cy);

		if( F[index.cx-1][index.cy+1].Status != EmptyStatus && !status)
			return false;
	}

	return true;
}

void CShell::CalculateOnDirect( CSize& index, eExtDirect eDir)
{
	int dX, dY;

	utility::SetDelta( eDir, dX, dY);

	for( int i = 1; i < 5; i++)
	{
		int x = index.cx + dX*i;
		int y = index.cy + dY*i;

		if( !utility::CheckPoint( x, y)) return;

		if( ShellArray[x][y].pos == 0 && F[x][y].Status == EmptyStatus) break;//return;

		if( ShellArray[x][y].pos != 0) 
		{
			CShellPoint& p = ShellMovesList.GetAt( ShellArray[x][y].pos);

			p.Init( pGame);

			p.Build();
		}
	}
}

void CShell::UpdatePointSquare( CSize& index, bool included)
{
	CalculateOnDirect( index, L);

	CalculateOnDirect( index, R);

	CalculateOnDirect( index, T);

	CalculateOnDirect( index, B);

	CalculateOnDirect( index, LT);

	CalculateOnDirect( index, RB);

	CalculateOnDirect( index, RT);

	CalculateOnDirect( index, LB);
	
	if( included)
	{
		CShellPoint& p = ShellMovesList.GetAt( ShellArray[index.cx][index.cy].pos);
		
		p.Init( pGame);
		
		p.Build();
	}
}

void CShell::Calculate()
{
	RemoveSubjectLists();

	Weight.cx = 0;
	Weight.cy = 0;

	POSITION pos = ShellMovesList.GetHeadPosition();

	while( pos)
	{
		CShellPoint cp( ShellMovesList.GetNext( pos));	
	
		Weight.cx += cp.WhiteSum;
		Weight.cy += cp.BlackSum;
	

		BuildSubjectLists( cp);

	}

	if( pGame->Tbl.FieldsPtrList.IsEmpty()) return;

	double sum = Weight.cx + Weight.cy;

	double coef;

	coef = 100 / sum;

	double wDouble = Weight.cx * coef;
	double bDouble = Weight.cy * coef;

	Weight.cx = (UINT)wDouble;
	Weight.cy = (UINT)bDouble;

	UINT sum2 = Weight.cx + Weight.cy;
}

CSize CShell::GetWeight()
{
	return Weight;
}

void CShell::RemoveSubjectLists()
{
	if( !TotalTree.IsEmpty()) TotalTree.RemoveAll();

	if( !HardTree.IsEmpty()) HardTree.RemoveAll();

	if( !PreHardTree.IsEmpty()) PreHardTree.RemoveAll();

	if( !WhitePreHardTree.IsEmpty()) WhitePreHardTree.RemoveAll();

	if( !BlackPreHardTree.IsEmpty()) BlackPreHardTree.RemoveAll();

	if( !DoubleHardTree.IsEmpty()) DoubleHardTree.RemoveAll();

	if( !WhiteCriticalTree.IsEmpty()) WhiteCriticalTree.RemoveAll();

	if( !BlackCriticalTree.IsEmpty()) BlackCriticalTree.RemoveAll();

	if( !WhitePreCriticalTree.IsEmpty()) WhitePreCriticalTree.RemoveAll();

	if( !BlackPreCriticalTree.IsEmpty()) BlackPreCriticalTree.RemoveAll();

	if( !WhiteDoublePreCriticalTree.IsEmpty()) WhiteDoublePreCriticalTree.RemoveAll();

	if( !BlackDoublePreCriticalTree.IsEmpty()) BlackDoublePreCriticalTree.RemoveAll();

	if( !WhiteDoubleTwoTree.IsEmpty()) WhiteDoubleTwoTree.RemoveAll();

	if( !BlackDoubleTwoTree.IsEmpty()) BlackDoubleTwoTree.RemoveAll();

	if( !WhiteTwoTree.IsEmpty()) WhiteTwoTree.RemoveAll();

	if( !BlackTwoTree.IsEmpty()) BlackTwoTree.RemoveAll();
}

void CShell::BuildSubjectLists( CShellPoint &cp)
{
	TotalTree.AddTail( cp.Index);

	if( cp.IsDoubleHard())
	{
		if( DoubleHardTree.GetCount() < 4) DoubleHardTree.AddTail( cp.Index);
	}
	
	if( cp.IsHard())
	{
		HardTree.AddTail( cp.Index);
	}

	if( cp.IsPreHard())
	{
		PreHardTree.AddTail( cp.Index);
	}

	if( cp.IsWhitePreHard())
	{
		WhitePreHardTree.AddTail( cp.Index);
	}

	if( cp.IsBlackPreHard())
	{
		BlackPreHardTree.AddTail( cp.Index);
	}
	
	if( cp.IsWhiteCritical())
	{
		WhiteCriticalTree.AddTail( cp.Index);
	}
	
	if( cp.IsBlackCritical())
	{
		BlackCriticalTree.AddTail( cp.Index);
	}
	
	if( cp.IsWhiteDoublePreCritical()) 
	{
		WhiteDoublePreCriticalTree.AddTail( cp.Index);
	}
	
	if( cp.IsBlackDoublePreCritical())
	{
		BlackDoublePreCriticalTree.AddTail( cp.Index);
	}
	
	if( cp.IsWhitePreCritical()) 
	{
		WhitePreCriticalTree.AddTail( cp.Index);
	}
	
	if( cp.IsBlackPreCritical())
	{
		BlackPreCriticalTree.AddTail( cp.Index);
	}
	
	if( cp.IsWhiteDoubleTwo()) 
	{
		WhiteDoubleTwoTree.AddTail( cp.Index);
	}
	
	if( cp.IsBlackDoubleTwo())
	{
		BlackDoubleTwoTree.AddTail( cp.Index);
	}
	
	if( cp.IsWhiteTwo()) 
	{
		if( WhiteTwoTree.GetCount() < 8) WhiteTwoTree.AddTail( cp.Index);
	}
	
	if( cp.IsBlackTwo())
	{
		if( BlackTwoTree.GetCount() < 8) BlackTwoTree.AddTail( cp.Index);
	}
}