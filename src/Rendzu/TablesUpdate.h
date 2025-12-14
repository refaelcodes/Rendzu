// TablesUpdate.h: interface for the CTablesDriver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLESUPDATE_H__2B4E2D61_82DC_11D9_9B3D_CC533C49154B__INCLUDED_)
#define AFX_TABLESUPDATE_H__2B4E2D61_82DC_11D9_9B3D_CC533C49154B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTablesDriver
{
public:
	friend class CGameDoc;
	friend class CGameView;
	friend class CGameData;
	friend class CGameManager;

	CTablesDriver();

public:
	ePower GetPower( CSize from, CSize to, eDirect direct);
	bool UpdateNewMove();
	bool UpdateUnDoMove();
	void UpdateUnDoMove( CField& move);
	void BuildResultArray( CSize from, CSize to);

	void SetLen( int i, int j, int& len, eExtDirect dir, eDirect& CurDir);

	CGameManager* pManager;
	CGameDoc* pDoc;
	CGameData* pGame;
};

#endif // !defined(AFX_TABLESUPDATE_H__2B4E2D61_82DC_11D9_9B3D_CC533C49154B__INCLUDED_)
