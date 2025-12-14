// Password.h: interface for the CPassword class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PASSWORD_H__96225581_81B0_11D9_9B3D_CA3066D9D14A__INCLUDED_)
#define AFX_PASSWORD_H__96225581_81B0_11D9_9B3D_CA3066D9D14A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/////////////////////////////////////////////////////////////////////////////
// CPassword

class CPassword
{
public:
	CPassword();
	CPassword( CString Keyword);
	~CPassword();
	void SetKeyWord( char* KeyWord);
	void GetPassword( char* pass);
private:
	void ResetFlag();
	CString GetPassword();
	CString FirstTransform();
	void Next( int& cur, int step);


// DATA

	char keyword[11];
	char password[11];
	int  flag[10];

};

class CGameApp;

class CRegister
{
public:
	CRegister( CGameApp* p);
	bool CheckProcedure();
	bool UnlimitedCheckProcedure();

	CString Keyword;
	CString Password;
	CString GameSection;
	CString StringSaleType;

private:
	BOOL SetupDirCheck();
	void Help();
	bool WasLimited();
	int  CheckPassword( char* pKeyword, char* pPassword);
	int  InstallProcedure();
	bool CheckLicenceData();
	void InstallLicenceData();
	bool CheckLimLicenceData();
	bool CheckUnLicenceData();

	bool CheckLimitedVersion();
	bool LimInstalled();
	void OnLimitedInstall();
	void OnUnLimitedInstall();
	bool CheckDate();

	void InitTemplateUnLimData(); 
	bool CheckUnlimitedData();

	CString GetCurrentDir();

	CGameApp* pApp;
	int IsLegal;

    CString GameLegalItem;  
	CString GameLimitedStatus;  

	CString Year;
	CString Month;
	CString Day;
	CString LimDays;

	CString mYear;
	CString mMonth;
	CString mDay;
	CString mLimDays;

	CString sInstalled;
	
	int SaleType;

	int Y;
	int M;
	int D;
	int Lim;

	int mY;
	int mM;
	int mD;
	int mLim;

	int iInstalled;

	CString S[10];
	CString N[10];

	CString cS[10];

	CString Home;
	CString HomeValue;

};


/////////////////////////////////////////////////////////////////////////////
// DInstallDlg dialog

//class CRegister;

class DInstallDlg : public CDialog
{
// Construction
public:
	CRegister* pGameRegistry;
	DInstallDlg(CRegister* p = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DInstallDlg)
	enum { IDD = IDD_INSTALL_DLG };
	CEdit	check_keyword;
	CString	m_keyword;
	CString	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DInstallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DInstallDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// DTitleDlg dialog

class DTitleDlg : public CDialog
{
// Construction
public:
	CGameApp* pApp;
	DTitleDlg( CGameApp* p);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DTitleDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTitleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DTitleDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



class DTitleDlg;


class CGameAccess  
{
public:
	bool SetTitleDlg();
	bool SetGameDirectory();

	CGameAccess();
	virtual ~CGameAccess();

	//bool SetTitleDlg();

private:
	CGameApp* pApp;	
};

#endif // !defined(AFX_PASSWORD_H__96225581_81B0_11D9_9B3D_CA3066D9D14A__INCLUDED_)
