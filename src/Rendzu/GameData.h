// GameData.h: interface for the GameData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEDATA_H__4DCC9461_90DF_11D9_9B3F_A9FE224CA54B__INCLUDED_)
#define AFX_GAMEDATA_H__4DCC9461_90DF_11D9_9B3F_A9FE224CA54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shell.h"
#include "Game.h"	// Added by ClassView
#include "Pattern.h"
#include "const.h"	// Added by ClassView

//////////////////////////////////////////////////////////////////////////////

class CMoveData;
class CPattern;
class CGameDoc;
class CGameManager;


//////////////////////////////////////////////////////////////////////////////
// class CGameData

class CGameData: public CObject
{
public:
	CGameData();

	DECLARE_SERIAL( CGameData)

	virtual ~CGameData();

	bool UpdateUnDo();

	eField NextColorStatus();
	
	void AddNewChecker(CSize index);
	
	void UpdateCandidate( CSize index);
	
	CSize CalcPrice();
	
	inline void UpdateWeight()
	{
		CField* tail = Tbl.FieldsPtrList.GetTail();
		
		Tbl.UpdateNeighbor( tail->Status, tail->Index, ON_INSERT);
		
		CalcPrice();
	}	

	UINT GetDiv();	
	
	inline int GetCount()
	{
		int count = Tbl.FieldsPtrList.GetCount();
		
		return count;
	}

	inline BOOL CurrentIsTail()
	{
		BOOL brc = false;
		
		brc = ( Tbl.FieldsPtrList.GetTailPosition() == GetCurrent());
		
		return brc;
	}
	
	bool GameIsStarted();

	void SetDepth( int level);
	
	virtual void Serialize( CArchive& ar);

	void UpdateAllByMovesList( bool IsLoad = false);



	inline POSITION IncrementCurrent()
	{if( Current) Tbl.FieldsPtrList.GetNext( Current);	return Current;}
	
	inline POSITION DecrementCurrent()
	{ if( Current) Tbl.FieldsPtrList.GetPrev( Current); return Current;}
			
	POSITION GetCurrent() const { return Current;}
	
	inline void SetCurrent(POSITION pos){ Current = pos;}

	inline CSize GetPrice() const { return Price; }

	inline void SetPrice( CSize price){ Price = price; }
	
	inline bool IsFullBoard()
	{
		return ( Square == (unsigned long)MovesList.GetCount());
	}

	inline bool Finished() { return Tbl.GetFinished(); }

	inline void SetFinished( bool finished)
	{
		Tbl.SetFinished( finished);
	}
	
	inline bool IsThreadActivated()
	{
		return !ThreadFinished; 
	}

	int  GetLevel();
	void SetLevel( int l);

	inline BOOL CheckLoadState()
	{
		if( pApp->GetProfileInt( "Settings", "Load", 0) == 1) return false;

		return true;
	}

	inline BOOL SetGamesDir()
	{
		CString Home = pApp->GetP();
		
		if( Home.IsEmpty()) return false;
		
		Home += "\\Games";
		
		BOOL res = SetCurrentDirectory( Home.GetBuffer( 200) );
		
		return true;
	}

// DATA
	CComponent*      pComponent;
	CGameManager*    pGameManager;
	CGameDoc*        pDoc;
	CGameApp*        pApp;

	eField           CurrentStatus;  // 'X' or 'O' is current move

	UINT			 Depth;  // the depth of calculation tree
	UINT		     FieldSize;
	
	TWeightList		 WeightList;

	CTablesDriver Tbl;

	TMovesList     MovesList; // the list of made moves ( go to serialize)
	CShell         Shell;  // the empty fields shell around made moves
	
	int            yScroll; //  current coordinates of
	int            xScroll;  //  scrool bars

	bool           IsStarted;  // the game state
	eMode          CurrentMode; // edit or machine mode
	unsigned long  Square;   // total fields number ( = 30*30 = 900)

	
	UINT Level;    // the game level ( betwen 1 to 5)
	UINT LevelMin; // = 1
	UINT LevelMax; // = 5
	
	CString    sLevel;  // string form of level ( to write in registers)
	CString    sSettings;  // 
	POSITION   saveCurrent;

	bool IsSerialize;  // the state of serialize
	bool ThreadFinished;  // the thread finished flag
	bool ThreadActivated;  // if=false =>the machine thread is finishing
	BOOL ChooseIt;  // define random move choosing
	bool Loading; // == teru if the programm load saved game
	bool AutoSave;   // if true - the all game is saved without request
	
	bool ViewDetails; //  for debug
	bool ViewPrice;  //  
	bool ViewMove;   //
	bool ViewCount;  //
	bool bWriteToFile;  // for debug 
	bool IsMachineProcess;
private:
	POSITION   Current; // pinter position in FieldsPtrList list
	CSize      Price;
	UINT Div;  // the current price size ( it is mutable )
};


class GameData  
{
public:
	GameData();
	virtual ~GameData();

};

#endif // !defined(AFX_GAMEDATA_H__4DCC9461_90DF_11D9_9B3F_A9FE224CA54B__INCLUDED_)
