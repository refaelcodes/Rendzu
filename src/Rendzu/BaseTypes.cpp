// BaseTypes.cpp: implementation of the BaseTypes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "BaseTypes.h"
#include "GameView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////
// class CMyPoint

CMyPoint::CMyPoint() : CPoint()
{
    x = y = 0;
}

CMyPoint::CMyPoint(const CMyPoint& point) : CPoint(static_cast<const CPoint&>(point))
{
}

CMyPoint::CMyPoint(const CPoint& point) : CPoint(point)
{
}

CMyPoint::CMyPoint(const CSize& size)
{
    x = size.cx;
    y = size.cy;
}

CMyPoint::CMyPoint(int xx, int yy)
{
    x = xx;
    y = yy;
}

void CMyPoint::Set(int xx, int yy)
{
    x = xx;
    y = yy;
}

CMyPoint& CMyPoint::operator=(const CMyPoint& point)
{
    if (this != &point)
    {
        x = point.x;
        y = point.y;
    }
    return *this;
}

CMyPoint& CMyPoint::operator=(const CPoint& point)
{
    x = point.x;
    y = point.y;
    return *this;
}

CMyPoint& CMyPoint::operator=(const CSize& size)
{
    x = size.cx;
    y = size.cy;
    return *this;
}

bool CMyPoint::operator==(const CMyPoint& point) const
{
    return (x == point.x && y == point.y);
}

bool CMyPoint::operator>(const CMyPoint& point) const
{
    return (x > point.x) || (x == point.x && y > point.y);
}

bool CMyPoint::operator>=(const CMyPoint& point) const
{
    return (*this > point) || (*this == point);
}

bool CMyPoint::operator<(const CMyPoint& point) const
{
    return (x < point.x) || (x == point.x && y < point.y);
}

bool CMyPoint::operator<=(const CMyPoint& point) const
{
    return (*this < point) || (*this == point);
}

CMyPoint CMyPoint::operator+(const CMyPoint& point) const
{
    return CMyPoint(x + point.x, y + point.y);
}

//////////////////////////////////////////////////////////////////////////////
// CField

CField::CField()
{
    Status = EmptyStatus;

    Index.cx = Index.cy = 0;

    b = 0;
    t = 0;
    l = 0;
    r = 0;
    rb = 0;
    rt = 0;
    lb = 0;
    lt = 0;
}

CField::CField(CSize index, eField status) : Status(status)
{
    Index.cx = index.cx;
    Index.cy = index.cy;

    b = 0;
    t = 0;
    l = 0;
    r = 0;
    rb = 0;
    rt = 0;
    lb = 0;
    lt = 0;
}

CField::CField(const CField& field)
{
    if (this != &field)
    {
        Index.cx = field.Index.cx;
        Index.cy = field.Index.cy;
        Status = field.Status;
        b = field.b;
        t = field.t;
        l = field.l;
        r = field.r;
        rb = field.rb;
        rt = field.rt;
        lb = field.lb;
        lt = field.lt;
    }
}

CField& CField::operator=(const CField& field)
{
    if (this != &field)
    {
        Index.cx = field.Index.cx;
        Index.cy = field.Index.cy;
        Status = field.Status;
        b = field.b;
        t = field.t;
        l = field.l;
        r = field.r;
        rb = field.rb;
        rt = field.rt;
        lb = field.lb;
        lt = field.lt;
    }
    return *this;
}

bool CField::operator==(const CField& field) const
{
    if (this == &field) return true;

    if (Index.cx != field.Index.cx) return false;
    if (Index.cy != field.Index.cy) return false;
    if (Status != field.Status) return false;
    if (b != field.b) return false;
    if (t != field.t) return false;
    if (l != field.l) return false;
    if (r != field.r) return false;
    if (rb != field.rb) return false;
    if (rt != field.rt) return false;
    if (lb != field.lb) return false;
    if (lt != field.lt) return false;

    return true;
}

bool CField::operator!=(const CField& field) const
{
    return !(*this == field);
}

void CField::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << Index << (int)Status << l << r << t << b << lt << lb << rt << rb;
    }
    else
    {
        int s;
        ar >> Index >> s >> l >> r >> t >> b >> lt >> lb >> rt >> rb;

        Status = (eField)s;
    }
}

void CField::Reset()
{
    Index.cx = Index.cy = 0;

    Status = EmptyStatus;

    b = 0;
    t = 0;
    l = 0;
    r = 0;
    rb = 0;
    rt = 0;
    lb = 0;
    lt = 0;
}

///////////////////////////////////////////////////////////////////////////////
// CMoveData

CGameManager* CMoveData::pManager;
CField** CMoveData::F;

CMoveData::CMoveData() : Index(0, 0)
{
    Direct = eT;

    NumList = 0;

    wT = wL = wLT = wRT = 0;
}

CMoveData::CMoveData(const CMoveData& indata)
{
    if (this != &indata)
    {
        pGame = indata.pGame;

        F = indata.F;

        Direct = indata.Direct;

        NumList = indata.NumList;

        Index = indata.Index;

        Power = eOpened;

        wT = indata.wT;
        wL = indata.wL;
        wLT = indata.wLT;
        wRT = indata.wRT;
    }
}

void CMoveData::Set(UINT cx, UINT cy, eDirect direct)
{
    Index.cx = cx;
    Index.cy = cy;
    Direct = direct;
}

ePower CMoveData::GetPower() { return Power; }

void CMoveData::SetPower(ePower power) { Power = power; }

CMoveData& CMoveData::operator=(const CMoveData& indata)
{
    if (this != &indata)
    {
        pGame = indata.pGame;

        F = indata.F;

        Direct = indata.Direct;

        NumList = indata.NumList;

        Index = indata.Index;
    }
    return *this;
}

bool CMoveData::operator==(const CMoveData& indata) const
{
    return (Index == indata.Index && Direct == indata.Direct);
}

bool CMoveData::operator!=(const CMoveData& indata) const
{
    return (Index != indata.Index || Direct != indata.Direct);
}

bool CMoveData::operator<(const CMoveData& indata) const
{
    if (Index.cx < indata.Index.cx) return true;

    else if (Index.cx == indata.Index.cx) return (Index.cy < indata.Index.cy);

    else if (Index.cx == indata.Index.cx && Index.cy == indata.Index.cy) return (Direct < indata.Direct);

    else return false;
}

CString CMoveData::DataToString()
{
    CField& Field = F[Index.cx][Index.cy];
    CString str = " I(";
    char temp[20];
    _itoa(Index.cx, temp, 10);
    str += temp;
    str += ", ";
    _itoa(Index.cy, temp, 10);
    str += temp;
    str += ")   D = ";

    int len;

    switch (Direct)
    {
    case eT:
        str += "eT  ";
        len = Field.b;
        break;
    case eL:
        str += "eL  ";
        len = Field.r;
        break;
    case eLT:
        str += "eLT ";
        len = Field.rb;
        break;
    case eRT:
        str += "eRT ";
        len = Field.lb;
        break;
    default:
        str += "e_  ";
        len = 0;
        break;
    }

    switch (Power)
    {
    case eOpened:
        str += "eOpened  ";
        break;
    case eClosed:
        str += "eClosed  ";
        break;
    case eHalfOpened:
        str += "eHalfOpened  ";
        break;
    default:
        str += "ePower???  ";
        break;
    };


    _itoa(len + 1, temp, 10);
    str += " L = ";
    str += temp;

    return str;
}

int& CMoveData::GetWeightByDirect(eDirect direct)
{
    switch (direct)
    {
    case eT: return wT;

    case eL: return wL;

    case eLT: return wLT;

    case eRT: return wRT;

    default: return wT;
    };
}

//////////////////////////////////////////////////////////////////////////////
//    UTILITIES

CString utility::IntToString(int i)
{
    char temp[100];

    _itoa(i, temp, 10);

    CString IntStr = temp;

    return IntStr;
}

int utility::Max(int n1, int n2)
{
    if (n1 > n2) return n1;

    else return n2;
}

int utility::Max(int n1, int n2, int n3, int n4)
{
    int k1 = Max(n1, n2);

    int k2 = Max(n3, n4);

    int k = Max(k1, k2);

    return k;
}

CSize utility::WeightSplit(CSize first, CSize second)
{
    double n1 = first.cx + second.cx;
    double n2 = first.cy + second.cy;

    double coef = (n1 + n2) / 100;

    n1 = n1 / coef;
    n2 = n2 / coef;

    CSize result((UINT)n1, (UINT)n2);

    return result;
}

void utility::SetDelta(eExtDirect eDir, int& dX, int& dY)
{
    switch (eDir)
    {
    case L:
        dX = -1;
        dY = 0;
        break;

    case R:
        dX = 1;
        dY = 0;
        break;

    case T:
        dX = 0;
        dY = -1;
        break;

    case B:
        dX = 0;
        dY = 1;
        break;

    case LT:
        dX = -1;
        dY = -1;
        break;

    case RB:
        dX = 1;
        dY = 1;
        break;

    case RT:
        dX = 1;
        dY = -1;
        break;

    case LB:
        dX = -1;
        dY = 1;
        break;

    default:
        dX = 0;
        dY = 0;
    };
}

bool utility::CheckSize(int m)
{
    return (m >= 0 && m < DIMENSION);
}

bool utility::CheckPoint(int x, int y)
{
    return (x >= 0 && x < DIMENSION && y >= 0 && y < DIMENSION);
}

void utility::WeightFormat(CSize& weight, UINT div)
{
    double w = weight.cx;
    double b = weight.cy;

    double coef = (w + b) / div;

    Assert(coef != 0);

    w = (UINT)(w / coef);
    b = (UINT)(b / coef);

    coef = (w + b) / 100;

    Assert(coef != 0);

    w /= coef;
    b /= coef;

    weight.cx = (UINT)w;
    weight.cy = (UINT)b;
}

CString utility::GetCurTime()
{
    CTime CurTime = CTime::GetCurrentTime();

    __time64_t cur = CurTime.GetTime();

    CString sTime = "";

    for (int i = 0; i < 8; i++)
    {
        int x = cur % 10;
        cur = cur / 10;
        char c = '0' + x;
        sTime += c;
    }

    int sec = CurTime.GetSecond();

    for (int i = 0; i < 2; i++)
    {
        int x = cur % 10;
        cur = cur / 10;
        char c = '0' + x;
        sTime += c;
    }

    return sTime;
}

void utility::ClearString(char* str)
{
    int len = strlen(str);

    for (int i = 0; i < len; i++) str[i] = 0;
}

HANDLE C_Handle::hnd = 0;

bool utility::ThreadActive()
{
    DWORD dw = WaitForSingleObject(C_Handle::hnd, 0);

    switch (dw)
    {
    case  WAIT_FAILED:
        {
            bool fail = true;

        }
    case WAIT_OBJECT_0:
        return false;

    case WAIT_TIMEOUT:
    default:
        return true;
    };
}