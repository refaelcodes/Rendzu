
// GameView.h : interface of the CGameView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEVIEW_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
#define AFX_GAMEVIEW_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_

#include "GameManager.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"

const UINT WM_USER_MACHINE_MOVE = WM_USER + 100;
const UINT WM_EXAMPLE = WM_USER + 110;
const UINT WM_USER_EXAMPLE_OPEN = WM_USER + 111;
const UINT WM_USER_REVIEW_DONE = WM_USER + 112;


class CBoard;

class CGameView : public CScrollView
{
protected: // create from serialization only
	CGameView();
	DECLARE_DYNCREATE(CGameView)

// Attributes
public:
	CGameDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	//void OnExample(); 
	protected:
	virtual void OnInitialUpdate(); // called first time after construct;
	//}}AFX_VIRTUAL

// Implementation
public:
	void   ScrollToCurrent();
	void   PrepareClickedPoint( CPoint& point);
	void   ExampleThread();	

	LRESULT OnMachineMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnExample(WPARAM wParam, LPARAM lParam);
	LRESULT OnExampleOpen(WPARAM wParam, LPARAM lParam);
	LRESULT OnReviewDone(WPARAM wParam, LPARAM lParam);
	//void OnPassword();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual ~CGameView();

// Generated message map functions
public:
	//{{AFX_MSG(CGameView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBegin();
	afx_msg void OnEnd();
	afx_msg void OnMoveBack();
	afx_msg void OnMoveUp();
	afx_msg void OnMachine();
	afx_msg void OnEditGame();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnAppAbout();

	afx_msg void OnLevel();
	afx_msg void OnHelpButton();
	afx_msg void OnHelpContens();
	afx_msg void OnHelpAboutgame();
	afx_msg void OnView();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CGameManager GameManager;
	CBoard       Board;
	CGameDoc*    pDoc;	
	CComponent*  pComponent;
	CGameApp*    pApp;
	bool         IsExample;
};

#ifndef _DEBUG  // debug version in GameView.cpp
inline CGameDoc* CGameView::GetDocument()
   { return (CGameDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CLevel dialog

class CLevel : public CDialog
{
// Construction
public:
	CLevel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLevel)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLevel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEVIEW_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
