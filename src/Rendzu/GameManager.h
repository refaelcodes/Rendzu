#if !defined(AFX_GAMEMANAGER_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
#include "GameDoc.h"	// Added by ClassView
#define AFX_GAMEMANAGER_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pattern.h"

// заголовок, например в GameManager.h
//static UINT AFX_CDECL ThreadProc(LPVOID pParam);

class CCriticalPoints;

///////////////////////////////////////////////////////////////////////////////
// class CGameManager

class CGameManager
{
public:	
	friend class CGameView;
	friend class CGameData;

	CGameManager( CGameView* view = 0);
	~CGameManager();
private:
	void DrawWinnerFields();

	void Init( CGameView* view);	

	void CutMovesUpdate();
	
	void Redraw();
	inline void DrawAllMoves();
	void DrawAllMoves( POSITION from);
	void DrawNewChecker();
	void DrawChecker( CField field);
	void DrawHighLightChecker( CField field);
	void DrawWinnerChecker( CField field);

	void GoUp();
	void GoBack();
	void GoEnd();
	void GoBegin();

	void StartGame();
			
		
	bool ClickIsNewMove();
	bool IsMachineTurn();

	bool  GameIsStarted() const;
public:
	static UINT AFX_CDECL ThreadProc(LPVOID pParam);
	bool UpdateUnDo();
	void BreakThread();
	HANDLE hMachine;
	void OnMachine();
	void Review();
	void ReviewThread();
	bool CheckFirstClick();
	void OnEditGame();
	void SetMode( eMode mode);
	void UpdateNewMove( CSize index, bool FromMachine, bool IsSerialize);
	void CallMachineThread();
	void CallMachine();

	inline bool IsThreadActivated()
	{
		return !pGame->ThreadFinished; 
	}

// DATA
	CComponent* pComponent;
	CGameView*       pView;
	CGameDoc*        pDoc;
	CGameData*       pGame;
	TFieldsPtrList*  FieldsPtrList;

	CSize            CurrentIndex;
	bool             IsSerialize;  // delete ??? check !!!
};


#endif // !defined(AFX_GAMEMANAGER_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
