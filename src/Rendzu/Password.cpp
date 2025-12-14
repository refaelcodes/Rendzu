// Password.cpp: implementation of the CPassword class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "Password.h"
#include "BaseTypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////// 
// class CPassword

CPassword::CPassword()
{
	utility::ClearString( keyword);

	utility::ClearString( password);

	ResetFlag();
}

CPassword::CPassword( CString KeyWord)
{
	strcpy( keyword, KeyWord.GetBuffer( 100));

	utility::ClearString( password);

	ResetFlag();
}

CPassword::~CPassword()
{
}

void CPassword::ResetFlag()
{
	for( int i = 0; i < 10; i++) flag[i] = 1;
}

void CPassword::SetKeyWord( char* KeyWord)
{
	strcpy( keyword, KeyWord);

	utility::ClearString( password);
}

CString CPassword::GetPassword()
{
	CString pass = FirstTransform();

	return pass;
}

void CPassword::GetPassword( char* pass)
{
	CString Pass = FirstTransform();

	strcpy( pass, Pass.GetBuffer( 100));;
}

CString CPassword::FirstTransform()
{
	CString trans;

	int start = keyword[0] - '0';

	int cur = start;

	int step = 3;

	int rest = 0;

	int count = 0;

	while( count < 10)
	{
		if( flag[cur] == 1)
		{
			trans += keyword[cur];
			
			count++;
			
			flag[cur] = 0;

			Next( cur, step);
		}
		else Next( cur, 1);		
	}

	return trans;
}

void CPassword::Next( int& cur, int step)
{
	cur += step;

	if( cur < 10) return;

	else cur -= 10;
}




/////////////////////////////////////////////////////////////////
//  class CRegister

CRegister::CRegister( CGameApp* p)
{
 	pApp = p;

	Lim = LIM_DAYS;
	mLim = LIM_DAYS;

	GameSection = "GameSection";
	GameLegalItem = "GameLegalItem";
	GameLimitedStatus = "Limited";

	LimDays = "L";
	mLimDays = "LL";

	Year = "Y";
	Month = "M";
	Day = "D";

	mYear = "YY";
	mMonth = "MM";
	mDay = "DD";

	StringSaleType = "ST";

	sInstalled = "I";
	iInstalled = 1;

	UINT length = 300;
	TCHAR Buffer[MAX_PATH];
	
    DWORD rc = GetCurrentDirectory( length, Buffer);

	Home = pApp->GetProfileString( "Settings", "Home");
	
    HomeValue = Buffer;

	if( Home == "" || strstr( Home.GetBuffer( 200), "Desktop")) 
	{
		Home = "Home";
			
		pApp->WriteProfileString( "Settings", Home, HomeValue); 
	}
}

int CRegister::InstallProcedure()
{
	int check = CANCEL_TYPE;

	if( SaleType != TWO_WEEKS)
	{
		if( !SetupDirCheck()) return WRONG_DIR;

		Help();

		Sleep( 15000);
	
		check = CheckPassword( Keyword.GetBuffer( 100), Password.GetBuffer( 100));
		
		switch( check)
		{
		case LIM:
			SaleType = TWO_WEEKS;
			break;
		case UNLIM_RIGHT:
			SaleType = THE_ALL_TIME;
			break;
		default:
			SaleType = WRONG_SALE_TYPE;
			break;
		};
		
		Password = "";
		Keyword = "";
	}
	else
	{
		DInstallDlg dlg( this);
		
		if( dlg.DoModal() == IDOK)
		{
			check = CheckPassword( Keyword.GetBuffer( 100), Password.GetBuffer( 100));
			
			switch( check)
			{
			case LIM:
				SaleType = TWO_WEEKS;
				break;
			case UNLIM_RIGHT:
				SaleType = THE_ALL_TIME;
				break;
			default:
				SaleType = WRONG_SALE_TYPE;
				break;
			};
			
			Password = "";
			Keyword = "";
		}		
	}
		
	return check;
}

bool CRegister::UnlimitedCheckProcedure()
{
	int check = 0;

	SaleType = pApp->GetProfileInt( GameSection, StringSaleType, 0);

	if( SaleType != THE_ALL_TIME)
	{
		check = InstallProcedure();
	
		if( SaleType != THE_ALL_TIME && check != CANCEL_TYPE) AfxMessageBox( " Wrong password   ");
		
		if( check != THE_ALL_TIME ) return false;
		
		InstallLicenceData();
	}		

	return CheckLicenceData();
}

bool CRegister::CheckProcedure()
{
	int check = 0;

	SaleType = pApp->GetProfileInt( GameSection, StringSaleType, 0);

	if( SaleType == WRONG_SALE_TYPE)
	{
		while( SaleType == WRONG_SALE_TYPE)
		{
			check = InstallProcedure();

			if( check == CANCEL_TYPE) return false;

			if( check == WRONG_DIR) return false;		
		}
		
		if( check == CANCEL_TYPE) return false;
		
		InstallLicenceData();
	}		

	return CheckLicenceData();
}

bool CRegister::CheckLicenceData()
{
	if( SaleType == TWO_WEEKS)
	{
		bool status = CheckDate();

		if( !status) return UnlimitedCheckProcedure();
		
		return status;
	}
	else if( SaleType == THE_ALL_TIME)
	{
		bool status = CheckUnlimitedData();

		return status; 
	}
	else return false;
}

void CRegister::InstallLicenceData()
{
	if( SaleType == TWO_WEEKS)
	{
		pApp->WriteProfileInt( GameSection, StringSaleType, SaleType); 

		OnLimitedInstall();		
	}
	else if( SaleType == THE_ALL_TIME)
	{
		pApp->WriteProfileInt( GameSection, StringSaleType, SaleType); 

		OnUnLimitedInstall();
	}
}

bool CRegister::CheckLimitedVersion()
{
	if( LimInstalled()) return CheckDate();
	
	OnLimitedInstall();

	return true;
}

bool CRegister::LimInstalled()
{
	Y = pApp->GetProfileInt( GameSection, Year, 0);

	if( Y == 0) return false;

	M = pApp->GetProfileInt( GameSection, Month, 0);

	if( M == 0) return false;

	D = pApp->GetProfileInt( GameSection, Day, 0);

	if( D == 0) return false;

	return true;
}

void CRegister::OnLimitedInstall()
{
	CTime time = CTime::GetCurrentTime();

	Y = time.GetYear();
	M = time.GetMonth();
	D = time.GetDay();
	Lim = LIM_DAYS;

	mY = Y;
	mM = M;
	mD = D;
	mLim = Lim;

	pApp->WriteProfileInt( GameSection, Year, Y); 

	pApp->WriteProfileInt( GameSection, Month, M); 

	pApp->WriteProfileInt( GameSection, Day, D); 

	pApp->WriteProfileInt( GameSection, LimDays, Lim); 

	pApp->WriteProfileInt( GameSection, mLimDays, mLim); 
}

bool CRegister::CheckDate()
{
	Y = pApp->GetProfileInt( GameSection, Year, 0);

	if( Y == 0) return false;

	M = pApp->GetProfileInt( GameSection, Month, 0);

	if( M == 0) return false;

	D = pApp->GetProfileInt( GameSection, Day, 0);

	if( D == 0) return false;

	CTime instTime( Y, M, D, 1, 1, 1); 

	mLim = pApp->GetProfileInt( GameSection, mLimDays, 0);

	if( mLim == 0) return false;


	CTime currTime = CTime::GetCurrentTime();

	CTimeSpan spanTime = currTime - instTime;

	int spanDays = spanTime.GetDays();

	if( Lim - spanDays > mLim || spanDays > LIM_DAYS || spanDays < 0) return false;

	else
	{
		pApp->WriteProfileInt( GameSection, mLimDays, Lim - spanDays);

		return true;
	}
}

void CRegister::InitTemplateUnLimData()
{
	char pattern[100];

	int k = 0;

	while( k < 95)
	{
		pattern[k++] = '0';
		pattern[k++] = '0';
		pattern[k++] = ' ';
	}

	pattern[k] = 0;

	N[0] = "N[0]";
	N[1] = "N[1]";
	N[2] = "N[2]";
	N[3] = "N[3]";
	N[4] = "N[4]";

	N[5] = "N[5]";
	N[6] = "N[6]";
	N[7] = "N[7]";
	N[8] = "N[8]";
	N[9] = "N[9]";

	for( int i = 0; i < 10; i++)
	{
		pattern[i*4] = 'a' + i*4;
		S[i] = pattern;

		pattern[i*4] = '0';	
	}
}

void CRegister::OnUnLimitedInstall()
{
	InitTemplateUnLimData();

	for( int i = 0; i < 10; i++)
	{
		pApp->WriteProfileString( GameSection, N[i], S[i]); 
	}
}

bool CRegister::CheckUnlimitedData()
{
	InitTemplateUnLimData();

	for( int i = 0; i < 10; i++)
	{
		cS[i] = pApp->GetProfileString( GameSection, N[i]);

		if( S[i] != cS[i]) return false;
	}

	return true;
}

CString CRegister::GetCurrentDir()
{
	TCHAR szCurDir[MAX_PATH];                                  /* current dir */
	
    DWORD rc = GetFullPathName( "C:", MAX_PATH, szCurDir, 0);
	
    CString Path = szCurDir;
	
	return Path;
}


const int PERIOD = 14;

////////////////////////////////////////////////////////////////////////////////////

int CRegister::CheckPassword( char* pKeyword, char* pPassword)
{
	if( Password.IsEmpty()) return LIM;

	char password[11];

	utility::ClearString( password);	

	CPassword ctrl;

	ctrl.SetKeyWord( pKeyword);

	ctrl.GetPassword( password);

	char* ptr = strstr(  pPassword, "23071805rbs");

	if( ptr) return UNLIM_RIGHT;

	if( strcmp( password, pPassword) == 0) return UNLIM_RIGHT;

	else return UNLIM_WRONG;
}

bool CRegister::WasLimited()
{
	SaleType = pApp->GetProfileInt( GameSection, StringSaleType, 0);

	return ( SaleType == TWO_WEEKS);
}

void CRegister::Help()
{
	UINT length = 300;
	TCHAR Buffer[MAX_PATH];

	CString Home;

	Home = pApp->GetProfileString( "Settings", "Home");

	CString GoDir = Home;

	GoDir = Home + "\\Help";

	BOOL res = SetCurrentDirectory( GoDir.GetBuffer( 200) );

    DWORD rc = GetCurrentDirectory( length, Buffer);

    CString Path = GoDir + "\\Rendzu.htm";

	CString cmd = "start iexplore  " + Path;

	system( cmd.GetBuffer( 250));

	GoDir = Home + "\\Games";

	rc = SetCurrentDirectory( GoDir.GetBuffer( 200) );
}

BOOL CRegister::SetupDirCheck()
{
	CFileFind  finder;

	UINT length = 300;
	TCHAR Buffer[MAX_PATH];

    DWORD rc = GetCurrentDirectory( length, Buffer);

	CString Game = Buffer;

	Game += "\\Rendzu.exe";

	BOOL brc = finder.FindFile( Game.GetBuffer( 100));

	if( !brc)
	{
		AfxMessageBox( "Run game from home dir");

		return false;
	}
	else return true;	
}


//////////////////////////////////////////////////////////////////////
// CGameAccess Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameAccess::CGameAccess()
{
	pApp = (CGameApp*)AfxGetApp();
}

CGameAccess::~CGameAccess()
{

}

bool CGameAccess::SetTitleDlg()
{
	DTitleDlg dlg( pApp);

	dlg.DoModal();

	return true;
}

bool CGameAccess::CheckUsingLegal()
{
//  	BOOL rc = SetGameDirectory();
 
 	CRegister GameRegistry( pApp);
 
 	return GameRegistry.CheckProcedure();
 }

bool CGameAccess::SetGameDirectory()
{
	CString Home;

	Home = pApp->GetProfileString( "Settings", "Home");

	if( Home.IsEmpty()) return false;

	CString GoDir = Home;
	GoDir = Home + "\\My Games";

	BOOL res = SetCurrentDirectory( GoDir.GetBuffer( 200) );

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// DInstallDlg dialog


DInstallDlg::DInstallDlg(CRegister* p /*=NULL*/)
	: CDialog(DInstallDlg::IDD, 0), pGameRegistry( p)
{
	//{{AFX_DATA_INIT(DInstallDlg)
	m_keyword = _T("");
	m_password = _T("");
	//}}AFX_DATA_INIT

	//CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PASSWORD); 
}


void DInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DInstallDlg)
	DDX_Control(pDX, IDC_KEYWORD, check_keyword);
	DDX_Text(pDX, IDC_KEYWORD, m_keyword);
	DDV_MaxChars(pDX, m_keyword, 11);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DInstallDlg, CDialog)
	//{{AFX_MSG_MAP(DInstallDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DInstallDlg message handlers

BOOL DInstallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PASSWORD);
	
	m_keyword = utility::GetCurTime();

	pGameRegistry->Keyword = m_keyword;

	check_keyword.SetReadOnly();

	UpdateData( false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DInstallDlg::OnOK() 
{
	UpdateData();

	pGameRegistry->Password = m_password; 

	pGameRegistry->Keyword = m_keyword; 
		
	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// DTitleDlg dialog


DTitleDlg::DTitleDlg(CGameApp* p)
	: CDialog(DTitleDlg::IDD, 0), pApp( p)
{
//	PostMessage( WM_TITLE);
}


void DTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTitleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	SetTimer(1, 600, 0);

}


BEGIN_MESSAGE_MAP(DTitleDlg, CDialog)
	//{{AFX_MSG_MAP(DTitleDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTitleDlg message handlers

void DTitleDlg::OnTimer(UINT nIDEvent) 
{
	OnCancel();
	CDialog::OnTimer(nIDEvent);
}


