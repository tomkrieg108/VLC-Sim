// GamePlay.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "game_play_dlg.h"
#include "game_play.h"
#include "utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGamePlay dialog

CGamePlay* g_game_play_dlg = 0;


CGamePlay::CGamePlay(CWnd* pParent /*=NULL*/)
	: CDialog(CGamePlay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGamePlay)
	m_bet = _T("");
	m_win = _T("");
	m_duration = _T("3");
	m_max_bet = _T("");
	m_min_bet = _T("");
	m_lp_1 = FALSE;
	m_lp_2 = FALSE;
	m_lp_3 = FALSE;
	m_lp_4 = FALSE;
	m_sap_1 = FALSE;
	m_sap_2 = FALSE;
	m_sap_3 = FALSE;
	m_sap_4 = FALSE;
	m_main_door = FALSE;
	m_security_door = FALSE;
	m_error_memory = FALSE;
	m_error_self_audit = FALSE;
	m_power_down = FALSE;
	m_sef = FALSE;
	m_audit_mode = FALSE;
	m_error_display = FALSE;
	//}}AFX_DATA_INIT

//	g_notify_mgr->Register( CNotify::CONNECT, this );
//	g_notify_mgr->Register( CNotify::DISCONNECT, this );
}


void CGamePlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGamePlay)
	DDX_Control(pDX, IDC_STATUS_3, m_status_3);
	DDX_Control(pDX, IDC_STATUS_2, m_status_2);
	DDX_Control(pDX, IDC_STATUS_1, m_status_1);
	DDX_Text(pDX, IDC_BET, m_bet);
	DDV_MaxChars(pDX, m_bet, 5);
	DDX_Text(pDX, IDC_WIN, m_win);
	DDV_MaxChars(pDX, m_win, 8);
	DDX_Text(pDX, IDC_DURATION, m_duration);
	DDV_MaxChars(pDX, m_duration, 2);
	DDX_Text(pDX, IDC_MAX_BET, m_max_bet);
	DDV_MaxChars(pDX, m_max_bet, 6);
	DDX_Text(pDX, IDC_MIN_BET, m_min_bet);
	DDV_MaxChars(pDX, m_min_bet, 6);
	DDX_Check(pDX, IDC_LP_1, m_lp_1);
	DDX_Check(pDX, IDC_LP_2, m_lp_2);
	DDX_Check(pDX, IDC_LP_3, m_lp_3);
	DDX_Check(pDX, IDC_LP_4, m_lp_4);
	DDX_Check(pDX, IDC_SAP_1, m_sap_1);
	DDX_Check(pDX, IDC_SAP_2, m_sap_2);
	DDX_Check(pDX, IDC_SAP_3, m_sap_3);
	DDX_Check(pDX, IDC_SAP_4, m_sap_4);
	DDX_Check(pDX, IDC_DOOR_MAIN, m_main_door);
	DDX_Check(pDX, IDC_DOOR_SECURITY, m_security_door);
	DDX_Check(pDX, IDC_ERROR_MEMORY, m_error_memory);
	DDX_Check(pDX, IDC_ERROR_SELF_AUDIT, m_error_self_audit);
	DDX_Check(pDX, IDC_POWER_DOWN, m_power_down);
	DDX_Check(pDX, IDC_SEF, m_sef);
	DDX_Check(pDX, IDC_AUDIT_MODE, m_audit_mode);
	DDX_Check(pDX, IDC_DISPLAY_ERROR, m_error_display);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGamePlay, CDialog)
	//{{AFX_MSG_MAP(CGamePlay)
	ON_BN_CLICKED(IDC_ADD_100, OnAdd100)
	ON_BN_CLICKED(IDC_ADD_20, OnAdd20)
	ON_BN_CLICKED(IDC_ADD_10, OnAdd10)
	ON_BN_CLICKED(IDC_ADD_5, OnAdd5)
	ON_BN_CLICKED(IDC_ADD_50, OnAdd50)
	ON_BN_CLICKED(IDC_ADD_1_HOP, OnAdd1Hop)
	ON_BN_CLICKED(IDC_ADD_1_CB, OnAdd1Cb)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_PROG_RESET, OnProgReset)
	ON_BN_CLICKED(IDC_DOOR_MAIN, OnDoorMain)
	ON_BN_CLICKED(IDC_DOOR_SECURITY, OnDoorSecurity)
	ON_BN_CLICKED(IDC_ERROR_MEMORY, OnErrorMemory)
	ON_BN_CLICKED(IDC_ERROR_SELF_AUDIT, OnErrorSelfAudit)
	ON_BN_CLICKED(IDC_POWER_DOWN, OnPowerDown)
	ON_BN_CLICKED(IDC_SEF, OnSef)
	ON_BN_CLICKED(IDC_AUDIT_MODE, OnAuditMode)
	ON_BN_CLICKED(IDC_HOPPER_OUT, OnHopperOut)
	ON_BN_CLICKED(IDC_CANCEL_CR, OnCancelCr)
	ON_BN_CLICKED(IDC_CONFIRM, OnConfirm)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_DISPLAY_ERROR, OnDisplayError)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGamePlay message handlers

BOOL CGamePlay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*
	m_GameCycleCtrl.InsertItem(0, "Idle Bit Unset");
	m_GameCycleCtrl.InsertItem(1, "Game Cycle Status Bit Set");
	m_GameCycleCtrl.InsertItem(2, "Turnover Increment");
	m_GameCycleCtrl.InsertItem(3, "Credit Decrement (bet)");
	m_GameCycleCtrl.InsertItem(4, "Wait Game Duration");
	m_GameCycleCtrl.InsertItem(5, "Credit Increment (win)");
	m_GameCycleCtrl.InsertItem(6, "Game Cycle Status Bit Unset");
	m_GameCycleCtrl.InsertItem(7, "Idle Bit Set");
*/

	EnableControls(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGamePlay::OnAdd100() 
{
	g_game_mgr->BankNoteIn(10000);
}

void CGamePlay::OnAdd50() 
{
	g_game_mgr->BankNoteIn(5000);	
}

void CGamePlay::OnAdd20() 
{
	g_game_mgr->BankNoteIn(2000);
}

void CGamePlay::OnAdd10() 
{
	g_game_mgr->BankNoteIn(1000);
}

void CGamePlay::OnAdd5() 
{
	g_game_mgr->BankNoteIn(500);
	
}

void CGamePlay::OnAdd1Hop() 
{
	g_game_mgr->CoinInHop();
}

void CGamePlay::OnAdd1Cb() 
{
	g_game_mgr->CoinInCB();
}

void CGamePlay::OnPlay() 
{
	UpdateData(true);

	g_game_mgr->PlayGame( Convert(m_bet), Convert(m_win), Convert(m_duration) );
}



void CGamePlay::OnStart() 
{
	CButton* auto_play_btn = (CButton*)GetDlgItem( IDC_START );
	UpdateData(true);

	int min_bet = Convert( m_min_bet );
	int max_bet = Convert( m_max_bet );

	if( ! g_game_mgr->AutoPlayStarted() )
	{
		UpdateData(true);
		if( min_bet < 1 )
			return;
		//if ( max_bet > 1000 )
		//	return;
		if( m_min_bet > m_max_bet )
			return;
		auto_play_btn->SetWindowText("Stop");
		g_game_mgr->AutoPlayStart( min_bet, max_bet);
	}
	else
	{
		auto_play_btn->SetWindowText("Start");	
		g_game_mgr->AutoPlayStop();
	}
}


void CGamePlay::SetStatus(int p_num, const CString& p_val )
{
	if(p_num == 1)
		m_status_1.SetWindowText( p_val );
	if(p_num == 2)
		m_status_2.SetWindowText( p_val );
	if(p_num == 3)
		m_status_3.SetWindowText( p_val );
}

int CGamePlay::GetSAPHitCount(void)
{
	int count = 0;
	UpdateData(true);
	if(m_sap_1) count++;
	if(m_sap_2) count++;
	if(m_sap_3) count++;
	if(m_sap_4) count++;
	return count;
}

bool CGamePlay::GetSAPHit(int p_level)
{
	UpdateData(true);
	if(p_level==1) 
	{
		if(m_sap_1) return true; // this is needed because m_sap_1 etc is BOOL, not bool
		else return false;
	}
	if(p_level==2) 
	{
		if(m_sap_2) return true; 
		else return false;
	}
	if(p_level==3) 
	{
		if(m_sap_3) return true; 
		else return false;
	}
	if(p_level==4) 
	{
		if(m_sap_4) return true; 
		else return false;
	}
	
	return false;
}

bool CGamePlay::GetLPHit(int p_level)
{
	UpdateData(true);
	if(p_level==1) 
	{
		if(m_lp_1) return true; 
		else return false;
	}
	if(p_level==2) 
	{
		if(m_lp_2) return true; 
		else return false;
	}
	if(p_level==3) 
	{
		if(m_lp_3) return true; 
		else return false;
	}
	if(p_level==4) 
	{
		if(m_lp_4) return true; 
		else return false;
	}
	
	return false;
}


void CGamePlay::EnableReset( const CString& p_caption )
{
	CButton* reset = (CButton*)GetDlgItem(IDC_PROG_RESET);

	reset->ShowWindow( SW_SHOW );
	reset->EnableWindow( true );
	reset->SetWindowText( p_caption );

}

void CGamePlay::DisableReset( void )
{
	CButton* reset = (CButton*)GetDlgItem(IDC_PROG_RESET);
	reset->ShowWindow( SW_HIDE );
	reset->EnableWindow( false );
}

void CGamePlay::EnableReset( const CString& p_caption, bool p_enable )
{
	CButton* reset = (CButton*)GetDlgItem(IDC_PROG_RESET);

	if(p_enable)
	{
		reset->ShowWindow( SW_SHOW );
		reset->EnableWindow( true );
		reset->SetWindowText( p_caption );
	}
	else
	{
		reset->ShowWindow( SW_HIDE );
		reset->EnableWindow( false );
	}
}

void CGamePlay::OnProgReset() 
{
	g_game_mgr->ResetProg();
}

bool CGamePlay::GetSAPHit(void)
{
	for(int i=1; i<=4; i++)
	{
		if( GetSAPHit(i) )
			return true;
	}
	return false;
}

bool CGamePlay::GetLPHit(void)
{
	for(int i=1; i<=4; i++)
	{
		if( GetLPHit(i) )
			return true;
	}
	return false;
}

bool CGamePlay::GetProgHit(void)
{
	// todo
	// UpdateData() gets called 8 times for this!
	return ( GetLPHit() || GetSAPHit() );
}

void CGamePlay::OnDoorMain() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_DOOR_MAIN);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->DoorMain( flag );
	
}

void CGamePlay::OnDoorSecurity() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_DOOR_SECURITY);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->DoorSecurity( flag );
	
}

void CGamePlay::OnErrorMemory() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_ERROR_MEMORY);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->ErrorMemory( flag );
	
}

void CGamePlay::OnErrorSelfAudit() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_ERROR_SELF_AUDIT);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->ErrorSelfAudit( flag );
	
}

void CGamePlay::OnPowerDown() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_POWER_DOWN);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->PowerDown( flag );
}

void CGamePlay::OnDisplayError() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_DISPLAY_ERROR);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->ErrorDisplay( flag );
	
}



void CGamePlay::OnSef() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_SEF);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->Sef( flag );
	
}

void CGamePlay::OnAuditMode() 
{
	CButton* button = (CButton*)GetDlgItem(IDC_AUDIT_MODE);
	bool flag;
	if( button->GetCheck() )
		flag = true;
	else
		flag = false;
	
	g_game_mgr->AuditMode( flag );
}

void CGamePlay::EnableControls( bool p_enable )
{
	CWnd* wnd;

	wnd = (CWnd*)GetDlgItem(IDC_ADD_100);
	wnd->EnableWindow( p_enable );
	
	wnd = (CWnd*)GetDlgItem(IDC_ADD_50);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_ADD_20);
	wnd->EnableWindow( p_enable );
	
	wnd = (CWnd*)GetDlgItem(IDC_ADD_10);
	wnd->EnableWindow( p_enable );
	
	wnd = (CWnd*)GetDlgItem(IDC_ADD_5);
	wnd->EnableWindow( p_enable );
	
	wnd = (CWnd*)GetDlgItem(IDC_ADD_1_HOP);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_ADD_1_CB);
	wnd->EnableWindow( p_enable );

	
/*
	wnd = (CWnd*)GetDlgItem(IDC_BET);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_WIN);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_DURATION);
	wnd->EnableWindow( p_enable );
*/

	wnd = (CWnd*)GetDlgItem(IDC_PLAY);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_PROG_RESET);
	wnd->EnableWindow( p_enable );

	
	wnd = (CWnd*)GetDlgItem(IDC_SAP_1);
	wnd->EnableWindow( p_enable );
	
	wnd = (CWnd*)GetDlgItem(IDC_SAP_2);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_SAP_3);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_SAP_4);
	wnd->EnableWindow( p_enable );


	wnd = (CWnd*)GetDlgItem(IDC_LP_1);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_LP_2);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_LP_3);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_LP_4);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_MIN_BET);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_MAX_BET);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_START);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_DOOR_MAIN);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_DOOR_SECURITY);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_ERROR_SELF_AUDIT);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_ERROR_MEMORY);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_AUDIT_MODE);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_SEF);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_POWER_DOWN);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_DISPLAY_ERROR);
	wnd->EnableWindow( p_enable );
}

void CGamePlay::ProcessNotify( int p_id )
{
//	if( p_id == CNotify::CONNECT )
//		EnableControls(true);
//	if( p_id == CNotify::DISCONNECT )
//		EnableControls(false);
}


void CGamePlay::OnHopperOut() 
{
	g_game_mgr->HopperPay();
}

void CGamePlay::EnableHopperOut( bool p_enable )
{
	CWnd* wnd;
	wnd = (CWnd*)GetDlgItem(IDC_HOPPER_OUT);
	wnd->EnableWindow( p_enable );
}


void CGamePlay::OnCancelCr() 
{
	g_game_mgr->CancelCredit();
}

void CGamePlay::OnConfirm() 
{
	g_game_mgr->CancelCreditConfirm(true);
}

void CGamePlay::OnCancel() 
{
	g_game_mgr->CancelCreditConfirm(false);
}

void CGamePlay::EnableCancelCr( bool p_enable )
{
	CWnd* wnd;
	wnd = (CWnd*)GetDlgItem(IDC_CANCEL_CR);
	wnd->EnableWindow( p_enable );

	wnd = (CWnd*)GetDlgItem(IDC_CONFIRM);
	wnd->EnableWindow( !p_enable );

	if(!p_enable) wnd->ShowWindow( SW_SHOW );
	else wnd->ShowWindow( SW_HIDE );
	
	wnd = (CWnd*)GetDlgItem(IDC_CANCEL);
	wnd->EnableWindow( !p_enable );

	if(!p_enable) wnd->ShowWindow( SW_SHOW );
	else wnd->ShowWindow( SW_HIDE );
}


