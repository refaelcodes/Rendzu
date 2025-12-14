// GameDoc.h : interface of the CGameDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEDOC_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
#define AFX_GAMEDOC_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Game.h"	// Added by ClassView
#include "GameData.h"

//////////////////////////////////////////////////////////////////////////////


class CGameDoc : public CDocument
{
protected: // create from serialization only
	CGameDoc();
	DECLARE_DYNCREATE(CGameDoc)

	virtual ~CGameDoc();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CGameData GameData;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
//protected:
public:	
//	{{AFX_MSG(CGameDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
//	}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEDOC_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
