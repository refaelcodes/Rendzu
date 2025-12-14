#if !defined(AFX_SETTINGDLG_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
#define AFX_SETTINGDLG_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_

#include "GameDoc.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//
#include "BaseTypes.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutGame dialog

class CAboutGame : public CDialog
{
// Construction
public:
	CGameView* pView;
	friend class CGameView;
	friend class CGameDoc;

	CAboutGame(CGameView* p);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CAboutGame)
	enum { IDD = IDD_ABOUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutGame)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutGame)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CLevelDlg dialog

class CLevelDlg : public CDialog
{
// Construction
public:
	CGameView* pView;
	friend class CGameView;
	friend class CGameDoc;

//	CSettingDlg(CGameView* p);   
	CLevelDlg(CGameView* p);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLevelDlg)
	enum { IDD = IDD_LEVEL_DLG };
	int		m_level;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLevelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DHelpDlg dialog

class DHelpDlg : public CDialog
{
// Construction
public:
	CGameView* pView;
	DHelpDlg( CGameView* p);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DHelpDlg)
	enum { IDD = IDD_HELP_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DHelpDlg)
	afx_msg void OnContents();
	virtual void OnOK();
	afx_msg void OnAboutGame();
	afx_msg void OnExample();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__763BED04_BE32_11D7_8263_DFA2B6547B34__INCLUDED_)
