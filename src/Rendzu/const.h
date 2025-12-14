// const.h: interface for the Cconst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONST_H__F2CB6C01_7EAD_11D9_9B3C_B8876C451B4B__INCLUDED_)
#define AFX_CONST_H__F2CB6C01_7EAD_11D9_9B3C_B8876C451B4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>


///////////////////////////    CONSTANT   ////////////////////////////


const int IS_RELEASE = 0;


const UINT MAX_PRICE = 30;//  FOR WEIGHT SPLIT

const  int OUT_SIZE = 1000;  // FOR MOVE AND PRICE DATA ARE OUTSIZE

const int FIVE = 5;

const UINT DIMENSION = 30;	// THE GAME BOARD DIMENSION
const UINT FIELD_SIZE = 47;//47;  // SIZE OF BOARD FIELDS 

const UINT MAX_DEPTH = 30;

/////////////    TO INSTALL DLG

const int TWO_WEEKS = 2;
const int THE_ALL_TIME = 1; 
const int WRONG_SALE_TYPE = 0;
const int CANCEL_TYPE = 4;
const int SALE = THE_ALL_TIME;//TWO_WEEKS; // TWO_WEEKS - two weeks demonstration
					// THE_ALL_TIME - saled	
const int WRONG_DIR = 5; 							

const int LEGAL = 1111;
const int UNLEGAL = 1000;

const int LIMITED = 1212;
const int UNLIMITED = 2121;

const int LIM_DAYS = 14;

const int LIM= 0;
const int UNLIM_RIGHT = 1;
const int UNLIM_WRONG = 2;

const UINT WHITE = 0;  
const UINT BLACK = 1;

const UINT ON_DELETE = 0;  // 
const UINT ON_INSERT = 1;

const UINT intL = 0;
const UINT intT = 1;
const UINT intLT = 2;
const UINT intRT = 3;


/////////////       FOR CALCULATE CLASSES

const int INCLUDE = 0;
const int EXCEPT = 1;

const int SHORTEST_TYPE = 0;
const int SHORT_TYPE = 1;
const int MIDDLE_TYPE = 2;
const int LONG_TYPE = 3;
const int LONGEST_TYPE = 4;

const int WHITE_FOUR_ONLINE = 0;
const int BLACK_FOUR_ONLINE = 1;
const int TOTAL_FOUR_ONLINE = 2;
const int WHITE_DOUBLE_THREE_ONLINE = 3;
const int BLACK_DOUBLE_THREE_ONLINE = 4;
const int WHITE_DOUBLE_TWO_ONLINE = 5;
const int BLACK_DOUBLE_TWO_ONLINE = 6;

const int WHITE_THREE_ONLINE = 7;
const int BLACK_THREE_ONLINE = 8;
const int WHITE_TWO_ONLINE = 9;
const int BLACK_TWO_ONLINE = 10;
const int TOTAL_THREE_ONLINE = 11;
const int HARD = 12;
const int DOUBLE_HARD = 13;
const int TOTAL = 14;
const int PRE_HARD = 15;
const int WHITE_PRE_HARD = 16;
const int BLACK_PRE_HARD = 17;
const int WHITE_TOTAL = 18;
const int BLACK_TOTAL = 19;


/////////////////   FOR BOARD CLASS

const COLORREF DrawColorRect = RGB( 255, 255, 255);
const COLORREF ClearColorRect = RGB( 255, 255, 255);

const COLORREF DrawColorLine = RGB( 0, 0, 128);
const COLORREF ClearColorLine = RGB( 255, 255, 255);



const UINT Xicon = IDI_ICON2;
const UINT Oicon = IDI_ICON3;

const UINT HXicon = IDI_ICON4;
const UINT HOicon = IDI_ICON5;

const UINT WXicon = IDI_ICON6;
const UINT WOicon = IDI_ICON7;


//////////////////////////   ENUM   //////////////////////////////

enum eIcon { eX, eO, eHX, eHO, eWX, eWO }; 

enum eIconState{ eSimple, eHighlighted, eWinner};

enum eExtDirect { T, B, L, R, LT, RB, RT, LB };

enum eAlgoType { eNothing, eUpdate, eUnDo };


enum eMode { MachineWhite, MachineBlack, TwoMachines, ViewMode, EditGame};

enum eDirect { eT, eL, eLT, eRT };

enum ePower { eOpened, eHalfOpened, eClosed };

enum eField { WhiteStatus, BlackStatus, EmptyStatus };



//////////////////////////////   TYPEDEF  ///////////////////////////

class CMoveData;
typedef CTypedPtrList<CPtrList, CMoveData*> TMoveDataList;

typedef CList<CSize, CSize&> TMovesList;
typedef TMovesList           TWeightList;

typedef CList<CSize, CSize&> TVariantList;

class CCompute;
class CComputeResult;
typedef CList<CComputeResult, CComputeResult&> TCandList;

class CField;
typedef CList<CField, CField&> TFieldsList;


typedef CTypedPtrList<CPtrList,CField*> TFieldsPtrList;

typedef TMovesList TShellList;

class CShellPoint;
typedef CList< CShellPoint, CShellPoint&> TShellMovesList;

typedef COLORREF CLR;


#endif // !defined(AFX_CONST_H__F2CB6C01_7EAD_11D9_9B3C_B8876C451B4B__INCLUDED_)
