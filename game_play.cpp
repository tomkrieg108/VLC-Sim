
#include "stdafx.h"
#include "VLCSim.h"

#include "game_play_dlg.h"
#include "sdb_dlg.h"
#include "dlg_transactions.h"
#include "dlg_control.h"
#include "dlg_configuration.h"
#include "dlg_monitor.h"
#include "dlg_events.h"
#include "dlg_main.h"
#include "program_options_dlg.h"
#include "game_play.h"
#include "xseries_transmit_thread.h"
#include "Utilities.h"
#include "random.h"

// todo
// sequence counters - check all  datablocks
// checksums - check all updates


CGamePlayMgr* g_game_mgr = 0;

void CGamePlayMgr::Initialise()
{
	if( !g_game_mgr )
	{
		g_game_mgr = new CGamePlayMgr();
	}
}

void CGamePlayMgr::Destroy()
{
	if( g_game_mgr != 0 )
	{
		delete g_game_mgr;
		g_game_mgr = 0;
	}
}


CGamePlayMgr::CGamePlayMgr() :
	m_sdb_rate(1500),
	m_pmdb_rate(15000),
	m_fdb_rate(30000),
	m_pdb_rate(15000),
	m_mmdb_rate(15000),
	m_sdb_timer(0),
	m_pmdb_timer(0),
	m_fdb_timer(0),
	m_pdb_timer(0),
	m_mmdb_timer(0),
	m_game_play_timer(0),
	m_inter_game_timer(0),
	m_hopper_pay_timer(0),
	m_sap_level(0),
	m_lp_level(0),
	m_sap_hits_pending(0),
	m_cur_sap_win(0),
	m_state(IDLE),
	m_auto_play(false),
	m_lockup(false),
	m_power_up(false),
	m_hopper_pay_in_progress(false),
	m_cancel_cr_in_progress(false),
	m_first_power_up(true),
	m_send_fdb(false),
	m_send_pdb(false),
	m_sdb_changed(false),
	m_fdb_changed(false),
	m_pdb1_changed(false),
	m_pdb2_changed(false)
{
		//g_notify_mgr->Register( CNotify::CONNECT, this );
		//g_notify_mgr->Register( CNotify::DISCONNECT, this );
}

CGamePlayMgr::~CGamePlayMgr()
{
}


void CGamePlayMgr::ResetMeters()
{	
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::BeginTransmitting(void)
{

	// Initialise the xseries packet static data from the the dialogs - config, FDB, PDB etc.
	InitialiseDataBlocks();

	// Initialise the SAP values
	m_sap_cur[0] = float( m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_1 ));  
	m_sap_cur[1] = float( m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_2 ));  
	m_sap_cur[2] = float( m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_3 ));  
	m_sap_cur[3] = float( m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_4 ));  
	m_sap_inc[0] = float( m_pdb1.GetDBField( CPDB1Packet::INC_1 ) )/ 100000000.0;
	m_sap_inc[1] = float( m_pdb1.GetDBField( CPDB1Packet::INC_2 ) )/ 100000000.0;
	m_sap_inc[2] = float( m_pdb1.GetDBField( CPDB1Packet::INC_3 ) )/ 100000000.0;
	m_sap_inc[3] = float( m_pdb1.GetDBField( CPDB1Packet::INC_4 ) )/ 100000000.0;

//	m_send_fdb = (g_program_opts_dlg->GetLP() ) && (g_program_opts_dlg->GetPortSetting() == 1 );
//	m_send_pdb = (g_program_opts_dlg->GetSAP() ) && (g_program_opts_dlg->GetPortSetting() == 1 );

	PowerUp();

	g_game_play_dlg->SetStatus(1, "Idle");

}


//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::PowerUp(void)
{
	if( m_power_up )
		return;

	//send the datablocks as needed for a power up.

	//SDB
	m_sdb.ResetSeqCounter();
	m_sdb.SetDBField( CSDBPacket::GAMES_PLAYED_POWER_UP, 0);
	m_sdb.IncrementDBField( CSDBPacket::POWER_UP_COUNT, 1);
	m_sdb.SetFlag( CSDBPacket::_POWER_UP, true );
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
	
	m_sdb_timer = g_timers->AddTimer(m_sdb_rate,this);
	g_transmit_manager->Queue( &m_sdb, 80 );
	

	if(m_send_fdb) 
	{
		m_fdb.ResetSeqCounter();
		m_fdb.SetChecksum( m_fdb.CalculateChecksum() );
		m_fdb_timer = g_timers->AddTimer(m_fdb_rate,this);
		g_transmit_manager->Queue( &m_fdb, 40 );
	}

	if(m_send_pdb) 
	{
		m_pdb1.ResetSeqCounter();
		m_pdb2.ResetSeqCounter();
		m_pdb1.SetChecksum( m_pdb1.CalculateChecksum() );
		m_pdb2.SetChecksum( m_pdb2.CalculateChecksum() );
		m_pdb_timer = g_timers->AddTimer(m_pdb_rate,this);
		g_transmit_manager->Queue( &m_pdb1, 40 );
		g_transmit_manager->Queue( &m_pdb2, 40 );
	}

	m_pmdb.SetChecksum( m_pmdb.CalculateChecksum() );
	m_pmdb_timer = g_timers->AddTimer(m_pmdb_rate,this);
	g_transmit_manager->Queue( &m_pmdb, 40 );

	m_power_up = true;
	m_first_power_up = false;


	switch( m_state )
	{
		case IDLE:
		{
			if(m_auto_play)
			{
				m_inter_game_timer = g_timers->AddTimer( 500, this );	
			}
			break;
		}

		case PLAYING:
		{
			m_game_play_timer = g_timers->AddTimer( 2000, this );
			break;
		}

		case CHECKING_LP:
		case CHECKING_SAP:
		{
			DoProgressives();
			break;
		}

		default:
			break;
	};
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::PowerDown(void)
{
	if( !m_power_up )
		return;

	// stop periodic transmission of data blocks
	g_timers->RemoveTimer( m_sdb_timer );
	g_timers->RemoveTimer( m_fdb_timer );
	g_timers->RemoveTimer( m_pdb_timer );
	g_timers->RemoveTimer( m_pmdb_timer );
	g_timers->RemoveTimer( m_mmdb_timer );

	// stop the game timers if active
	g_timers->RemoveTimer( m_game_play_timer );
	g_timers->RemoveTimer( m_inter_game_timer );

	m_power_up = false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::InitialiseDataBlocks(void)
{	
	//SDB
//	g_sdb_dlg->SetStaticData( m_sdb );
	// todo - cjeck to see if this has changed

	m_sdb.SetFlag( CSDBPacket::_IDLE, true );
	m_sdb.SetFlag( CSDBPacket::_GAMCYCLE, false );	
	m_sdb.SetFlag( CSDBPacket::_POWER_UP, true );	
	m_sdb.SetFlag( CSDBPacket::_RESET, true );	
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );

	//PMDB
//	g_dlg_transactions->GetStaticData( m_pmdb );
	m_pmdb.SetFlag( CPMDBPacket::_PRINTER_VALID_TICKET_IN, true );
	m_pmdb.SetFlag( CPMDBPacket::_STACKING_COMPLETED, true );
	m_pmdb.SetChecksum( m_pmdb.CalculateChecksum() );

	//FDB
//	g_dlg_control->GetStaticData( m_fdb );
	m_fdb.SetChecksum( m_fdb.CalculateChecksum() );

	//PDB1
	//g_dlg_configuration->GetStaticData( m_pdb1 );
	//g_dlg_configuration->GetCurrentValues( m_pdb1 );
	m_pdb1.SetFlag( CPDB1Packet::_NORMAL, true );	
	m_pdb1.SetFlag( CPDB1Packet::_POWER_UP, true );	
	m_pdb1.SetFlag( CPDB1Packet::_RESET, true );	
	m_pdb1.SetChecksum( m_pdb1.CalculateChecksum() );

	//PDB2
//	g_dlg_monitor->GetStaticData( m_pdb2 );
	m_pdb2.SetChecksum( m_pdb2.CalculateChecksum() );

	//MMDB
	//g_dlg_events->GetStaticData( m_mmdb );
	m_mmdb.SetChecksum( m_mmdb.CalculateChecksum() );
}



//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::ProcessTimeout( int p_id )
{
	if( p_id == m_sdb_timer )
	{
		if( m_sdb_changed )
		{
			m_sdb.IncrementSeqCounter();
			m_sdb_changed = false;
		}
		m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
		g_transmit_manager->Queue( &m_sdb, 80 );
	}

	else if( p_id == m_fdb_timer )
	{
		if( m_send_fdb )
		{
			if( m_fdb_changed )
			{
				m_fdb.IncrementSeqCounter();
				m_fdb_changed = false;
			}
			m_fdb.SetChecksum( m_fdb.CalculateChecksum() );
			g_transmit_manager->Queue( &m_fdb, 40 );
		}
	}

	else if( p_id == m_pdb_timer )
	{
		if( m_send_pdb )
		{
			if( (m_pdb1_changed) || (m_pdb2_changed ))
			{
				m_pdb1.IncrementSeqCounter();
				m_pdb2.IncrementSeqCounter();
				m_pdb1_changed = false;
				m_pdb2_changed = false;
			}	
			m_pdb1.SetChecksum( m_pdb1.CalculateChecksum() );
			m_pdb2.SetChecksum( m_pdb2.CalculateChecksum() );
			g_transmit_manager->Queue( &m_pdb1, 40 );
			g_transmit_manager->Queue( &m_pdb2, 40 );
		}
	}


	else if( p_id == m_pmdb_timer )
	{
		m_pmdb.SetChecksum( m_pmdb.CalculateChecksum() );
		g_transmit_manager->Queue( &m_pmdb, 40 );
	}

	else if( p_id == m_game_play_timer )
		GameEnd();

	else if( p_id == m_inter_game_timer)
	{
		if( m_auto_play )
			PlayGameAuto();
	}

	else if( p_id == m_hopper_pay_timer )
	{
		CoinOut();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::ProcessNotify( int p_id )
{
//	if( p_id == CNotify::CONNECT )
//	{
//		if( m_first_power_up )
//			BeginTransmitting();
//		else
//			PowerUp();
//	}
//
//	else if( p_id == CNotify::DISCONNECT )
//	{
//		PowerDown();	
//	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::BankNoteIn(uint p_amount)	
{
	if(p_amount <= 0) return;
	if(p_amount >10000) return;

	//update MDB and send
	m_pmdb.IncrementDBField( CPMDBPacket::TOTAL_BILLS_COUNT,1 );
	m_pmdb.IncrementDBField( CPMDBPacket::TOTAL_BILLS_CENTS, p_amount );

	if( p_amount == 500 ) 
	{
		m_pmdb.IncrementDBField( CPMDBPacket::BILLS_5_COUNT,1 );
	}
	if( p_amount == 1000 ) 
	{
		m_pmdb.IncrementDBField( CPMDBPacket::BILLS_10_COUNT,1 );
	}
	if( p_amount == 2000 )
	{
		m_pmdb.IncrementDBField( CPMDBPacket::BILLS_20_COUNT,1 );
	}
	if( p_amount == 5000 ) 
	{
		m_pmdb.IncrementDBField( CPMDBPacket::BILLS_50_COUNT,1 );
	}
	if( p_amount == 10000 ) 
	{
		m_pmdb.IncrementDBField( CPMDBPacket::BILLS_100_COUNT,1 );
	}

	m_pmdb.SetChecksum( m_pmdb.CalculateChecksum() );
	
	g_transmit_manager->Queue( &m_pmdb, 80 );
	ResetTimer(m_pmdb_timer, m_pmdb_rate);  

	// Update SDB - don't send 
	m_sdb.IncrementDBField( CSDBPacket::CASH_IN, p_amount );
	m_sdb.IncrementDBField( CSDBPacket::CASHBOX, p_amount );
	m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, p_amount );
	m_sdb_changed = true; // seq counter updated when next sent.
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CoinInHop(void)
{
	m_sdb.IncrementDBField( CSDBPacket::CASH_IN, 100 );
	m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, 100 );
	m_sdb_changed = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CoinInCB(void)
{
	m_sdb.IncrementDBField( CSDBPacket::CASH_IN, 100 );
	m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, 100 );
	m_sdb.IncrementDBField( CSDBPacket::CASHBOX, 100 );
	m_sdb_changed = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::HopperPay(void)
{
	int current_credit = m_sdb.GetDBField( CSDBPacket::CREDIT_METER );

	if( (m_hopper_pay_in_progress) || (m_cancel_cr_in_progress) )
		return;

	if( current_credit < 100 )
		return;

	m_sdb.SetFlag( CSDBPacket::_COLLECT_CASH, true );
	//todo
	//this section is repeated a few times - could make a function
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer(m_sdb_timer, m_sdb_rate);				
	m_sdb_changed = false;
	m_hopper_pay_timer = g_timers->AddTimer( 200, this );
	m_hopper_pay_in_progress = true;
	g_game_play_dlg->EnableHopperOut(false);
	g_game_play_dlg->SetStatus(3, "Hopper pay in progress");
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CoinOut(void)
{
	if( !m_hopper_pay_in_progress )
	{
		g_timers->RemoveTimer( m_hopper_pay_timer );
		return;
	}

	int current_credit = m_sdb.GetDBField( CSDBPacket::CREDIT_METER );

	if( current_credit < 100 )
	{
		g_timers->RemoveTimer( m_hopper_pay_timer );
		return;
	}

	current_credit -= 100;

	m_sdb.SetDBField( CSDBPacket::CREDIT_METER, current_credit );
	m_sdb.IncrementDBField( CSDBPacket::CASH_OUT, 100 );
	m_sdb_changed = true;

	if( current_credit < 100 )
	{
		// hopper out finished
		g_timers->RemoveTimer( m_hopper_pay_timer );
		m_sdb.SetFlag( CSDBPacket::_COLLECT_CASH, false );
		m_sdb.IncrementSeqCounter();
		m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
		g_transmit_manager->Queue( &m_sdb, 80 );
		ResetTimer(m_sdb_timer, m_sdb_rate);				
		m_sdb_changed = false;
		m_hopper_pay_in_progress = false;
		g_game_play_dlg->EnableHopperOut(true);
		g_game_play_dlg->SetStatus(3, "");
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CancelCredit(void)
{
	int current_credit = m_sdb.GetDBField( CSDBPacket::CREDIT_METER );
	if( current_credit == 0 )
		return;

	if( (m_hopper_pay_in_progress) || (m_cancel_cr_in_progress) )
		return;

	m_sdb.SetFlag( CSDBPacket::_CANCEL_CREDIT, true );
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer(m_sdb_timer, m_sdb_rate);				
	m_sdb_changed = false;
	m_cancel_cr_in_progress = true;
	g_game_play_dlg->EnableCancelCr(false);
	g_game_play_dlg->SetStatus(3, "Cancel credit in progress");
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CancelCreditConfirm(bool p_confirm)
{
	if(!m_cancel_cr_in_progress)
		return;

	m_sdb.SetFlag( CSDBPacket::_CANCEL_CREDIT, false );

	if( p_confirm )
	{
		int current_credit = m_sdb.GetDBField( CSDBPacket::CREDIT_METER );
		m_sdb.SetDBField( CSDBPacket::CREDIT_METER, 0 );
		m_sdb.IncrementDBField( CSDBPacket::CANCEL_CR, current_credit );
	}
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer(m_sdb_timer, m_sdb_rate);				
	m_sdb_changed = false;
	m_cancel_cr_in_progress = false;
	g_game_play_dlg->EnableCancelCr(true);
	g_game_play_dlg->SetStatus(3, "");
}


void CGamePlayMgr::PlayGame(int p_bet, int p_win, int p_duration)
{
	

	if( p_bet < 0 ) return;
	if( p_duration < 1 ) return;
	if( m_state != IDLE ) return;

	//ToDo Make this a status message
	if(p_bet > CurrentCredits() )
	{
		AfxMessageBox("Add more credits");
		return;
	}

	g_game_play_dlg->SetStatus(1, "Game Cycle");
	g_game_play_dlg->SetStatus(3, "");

	m_state = PLAYING;

	m_win_current_game = p_win;

	// Update and send SDB
	// todo
	// might be a better way of setting all thes flags!
	// maybe just have a single flag in g_game_mgr saying whether to clear all the ccce flags here
	m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, -p_bet );
	m_sdb.IncrementDBField( CSDBPacket::TURNOVER, p_bet );
	m_sdb.IncrementDBField( CSDBPacket::GAMES_PLAYED, 1 );
	m_sdb.IncrementDBField( CSDBPacket::GAMES_PLAYED_POWER_UP, 1 );
	m_sdb.IncrementDBField( CSDBPacket::GAMES_PLAYED_DOOR, 1 );
	m_sdb.SetFlag( CSDBPacket::_IDLE, false );
	m_sdb.SetFlag( CSDBPacket::_GAMCYCLE, true );
	m_sdb.SetFlag( CSDBPacket::_POWER_UP, false );
	m_sdb.SetFlag( CSDBPacket::_RESET, false );
	m_sdb.SetFlag( CSDBPacket::_CCCE_COMPLETE, false );
	m_sdb.IncrementSeqCounter();
	
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
	g_transmit_manager->Queue( &m_sdb, 80 );
	m_sdb_rate = 15000;
	ResetTimer(m_sdb_timer, m_sdb_rate);				//SDB every 15 secs during a game
	m_sdb_changed = false;
	
	// Update PMDB - don't send
	m_pmdb.SetFlag( CPMDBPacket::_STD_WIN_PAY_COMPLETE , false );
	m_pmdb.SetFlag( CPMDBPacket::_MYST_WIN_PAY_COMPLETE , false );
	m_pmdb.SetFlag( CPMDBPacket::_CCCE_CENTS_TRANSFER_COMPLETE , false );
	m_pmdb.SetFlag( CPMDBPacket::_MYST_PROG_WIN , false );
//	if( !g_program_opts_dlg->GetCCCExt() )
	{
		// V2.01 of PMDB => increment stroke meter in PMDB
		m_pmdb.IncrementDBField( CPMDBPacket::GAMES_PLAYED, 1 );
	}

	//Update PDB1
	m_pdb1.SetFlag( CPDB1Packet::_POWER_UP, false );	
	m_pdb1.SetFlag( CPDB1Packet::_RESET, false );	

	for(int i=0; i<4; i++)
		m_sap_cur[i] += float(p_bet) * m_sap_inc[i]; 

	m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_1, Convert(m_sap_cur[0]) );
	m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_2, Convert(m_sap_cur[1]) );
	m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_3, Convert(m_sap_cur[2]) );
	m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_4, Convert(m_sap_cur[3]) );
	m_pdb1_changed = true;


	//Update PDB2
	m_pdb2.IncrementDBField( CPDB2Packet::TURNOVER_START, p_bet );
	m_pdb2.IncrementDBField( CPDB2Packet::TURNOVER_CFG, p_bet );
	m_pdb2_changed = true;

	m_game_play_timer = g_timers->AddTimer( p_duration*1000, this );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Called when the m_game_play_timer elapses

void CGamePlayMgr::GameEnd(void)
{
	g_timers->RemoveTimer( m_game_play_timer );
	if( m_state != PLAYING ) 
		return;
	m_state = CHECKING_LP;	
	m_lp_level = 1;
	m_sap_hits_pending = g_game_play_dlg->GetSAPHitCount();
	DoProgressives();
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::DoProgressives()
{
	switch (m_state)
	{
		case CHECKING_LP:
		{
			if( g_game_play_dlg->GetLPHit(m_lp_level) )
			{
				m_state = CLEARING_LP;

				// enable the reset button, update dialog display
				// make this a function
				CString level_str = Convert( m_lp_level );
				g_game_play_dlg->SetStatus(2, "Link Prog Hit - level " + level_str + " Waiting Reset..." );
				g_game_play_dlg->EnableReset("Reset LP " + level_str, true);

				if( m_fdb_rate != 1500 )
				{
					// this is the first LP hit - send the SDP with the progressive flag set
					SendSDBProg(true);
				}
				SendFDBProg( m_lp_level );
			}
			else
			{
				CheckNextLPLevel();
			}
			return;
		}

		case CLEARING_LP:
		{
			m_state = CHECKING_LP;

			// disable the reset button, update dialog display
			// make this a function
			g_game_play_dlg->SetStatus(2, "" );
			g_game_play_dlg->EnableReset("", false);

			CheckNextLPLevel();
			return;
		}

		case CHECKING_SAP:
		{
			if( g_game_play_dlg->GetSAPHit(m_sap_level) )
			{
				m_state = CLEARING_SAP;
				// enable the reset button, update dialog display
				// make this a function
				CString level_str = Convert( m_sap_level );
				g_game_play_dlg->SetStatus(2, "SAP Hit - level " + level_str + " Waiting Reset..." );
				g_game_play_dlg->EnableReset("Reset SAP " + level_str, true);

				SendPDBProg( m_sap_level );
			}
			else
			{
				CheckNextSAPLevel();
			}
			return;
		}

		case CLEARING_SAP:
		{
			m_state = CHECKING_SAP;

			// disable the reset button, update dialog display
			// make this a function
			g_game_play_dlg->SetStatus(2, "" );
			g_game_play_dlg->EnableReset("", false);

			// todo
			// update credit meter and win meter in SDB with the SAP win amount..
			m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, m_cur_sap_win );
			m_sdb.IncrementDBField( CSDBPacket::TOTAL_WINS, m_cur_sap_win );
			m_sdb_changed = true;


			//Update PDB1 - number of pending levels, current JP value, JP win level etc, status flag
			m_sap_hits_pending--;

			//Update PBB2 - Total Wins, Number of resets.
			int reset = 0;
			if( m_sap_level == 1 )
			{
				m_pdb2.IncrementDBField( CPDB2Packet::RESETS_1,1);
				m_pdb2.IncrementDBField( CPDB2Packet::TOTAL_1, m_cur_sap_win);
				reset = m_pdb2.GetDBField( CPDB2Packet::RESET_1 );
				m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_1,reset );
			}
			if( m_sap_level == 2 )
			{
				m_pdb2.IncrementDBField( CPDB2Packet::RESETS_2,1);
				m_pdb2.IncrementDBField( CPDB2Packet::TOTAL_2, m_cur_sap_win);
				reset = m_pdb2.GetDBField( CPDB2Packet::RESET_2 );
				m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_2,reset );
			}
			if( m_sap_level == 3 )
			{
				m_pdb2.IncrementDBField( CPDB2Packet::RESETS_3,1);
				m_pdb2.IncrementDBField( CPDB2Packet::TOTAL_3,m_cur_sap_win);
				reset = m_pdb2.GetDBField( CPDB2Packet::RESET_3 );
				m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_3,reset );
			}
			if( m_sap_level == 4 )
			{
				m_pdb2.IncrementDBField( CPDB2Packet::RESETS_4,1);
				m_pdb2.IncrementDBField( CPDB2Packet::TOTAL_4,m_cur_sap_win);
				reset = m_pdb2.GetDBField( CPDB2Packet::RESET_4 );
				m_pdb1.SetDBField( CPDB1Packet::CUR_VAL_4,reset );
			}
			m_pdb1.SetDBField( CPDB1Packet::CUR_WIN_LEVEL,Convert(0) );
			m_pdb1.SetDBField( CPDB1Packet::CUR_WIN_AMOUNT,Convert(0) );
			m_pdb1.SetDBField( CPDB1Packet::NUM_PENDING_RESET, Convert(m_sap_hits_pending) );

			m_pdb1_changed = true;
			m_pdb2_changed = true;

			CheckNextSAPLevel();
			return;
		}

	default:
		return;;
	};
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CheckNextLPLevel(void)
{
	if( m_lp_level == 4)
	{
		m_lp_level = 0;
		m_sap_level = 1;
		m_state = CHECKING_SAP;
		if( m_fdb_rate == 1500 )
		{
			// there was an LP hit - need to clear it in the SDB and FDB and go back to the slower FDB rate
			SendSDBProg(false);
			SendFDBProg(0);
		}
	}
	else
	{
		m_lp_level++;
	}
	DoProgressives();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::CheckNextSAPLevel(void)
{
	
	if( m_sap_level == 4 )
	{
		m_sap_level = 0;
		if( m_pdb_rate == 1500 ) // there was an SAP hit - need to clear it in the PDB and go back to the slower rate
			 SendPDBProg( 0 );
		// todo - maybe delay a second or so before calling this.
		GameFinalise();
	}
	else
	{
		m_sap_level++;
		DoProgressives();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::GameFinalise(void)
{
	//Update and send SDB
	m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, m_win_current_game );
	m_sdb.IncrementDBField( CSDBPacket::TOTAL_WINS, m_win_current_game );
	m_sdb.SetFlag( CSDBPacket::_IDLE, true );
	m_sdb.SetFlag( CSDBPacket::_GAMCYCLE, false );
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );
	g_transmit_manager->Queue( &m_sdb, 80 );
	m_sdb_rate = 1500;
	ResetTimer(m_sdb_timer, m_sdb_rate); //SDB every 1.5 secs in idle.
	m_sdb_changed = false;
	
	if( m_auto_play )
	{
		m_inter_game_timer = g_timers->AddTimer( 500, this );
	}

	m_state = IDLE;
	g_game_play_dlg->SetStatus(1, "Idle");
	g_game_play_dlg->SetStatus(2, "");
}

//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
// this gets called when reset is pressed

void CGamePlayMgr::ResetProg(void)
{
	DoProgressives();
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::SendFDBProg( int p_level )
{
	// need to check if on port 2-6 before sending
	//p_level = 0 => clear, > - => set

	if(!m_send_fdb)
		return;

	if(p_level < 0) return;
	if(p_level > 4) return;

	m_fdb.SetDBField( CFDBPacket::WIN_LEVEL, Convert(p_level) );

	if(p_level==1)
		m_fdb.IncrementDBField( CFDBPacket::COUNT_LEVEL_1, 1 );
	if(p_level==2)
		m_fdb.IncrementDBField( CFDBPacket::COUNT_LEVEL_2, 1 );
	if(p_level==3)
		m_fdb.IncrementDBField( CFDBPacket::COUNT_LEVEL_3, 1 );
	if(p_level==4)
		m_fdb.IncrementDBField( CFDBPacket::COUNT_LEVEL_4, 1 );

	m_fdb.IncrementSeqCounter();
	m_fdb.SetChecksum( m_fdb.CalculateChecksum() );
	m_fdb_changed = false;
	
	g_transmit_manager->Queue( &m_fdb, 40 );

	if( p_level >= 1)
		m_fdb_rate = 1500;
	else
		m_fdb_rate = 15000;

	ResetTimer( m_fdb_timer, m_fdb_rate);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::SendPDBProg( int p_level )
{
	if(!m_send_pdb)
		return;

	if(p_level < 0) return;
	if(p_level > 4) return;

	CString win = "0";
	if( p_level == 1) m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_1, win);
	if( p_level == 2) m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_2, win);
	if( p_level == 3) m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_3, win);
	if( p_level == 4) m_pdb1.GetDBField( CPDB1Packet::CUR_VAL_4, win);
	m_cur_sap_win = Convert(win);

	m_pdb1.SetDBField( CPDB1Packet::NUM_PENDING_RESET, Convert( m_sap_hits_pending) );
	m_pdb1.SetDBField( CPDB1Packet::CUR_WIN_LEVEL, Convert(p_level) );
	m_pdb1.SetDBField( CPDB1Packet::CUR_WIN_AMOUNT, win);

	if(p_level == 0)
		m_pdb1.SetFlag( CPDB1Packet::_JP_HIT, false);
	else
		m_pdb1.SetFlag( CPDB1Packet::_JP_HIT, true);


	m_pdb1.IncrementSeqCounter();
	m_pdb1.SetChecksum( m_pdb1.CalculateChecksum() );
	
	m_pdb2.IncrementSeqCounter();
	m_pdb2.SetChecksum( m_pdb2.CalculateChecksum() );

	g_transmit_manager->Queue( &m_pdb1, 40 );
	g_transmit_manager->Queue( &m_pdb2, 40 );

	if( p_level >= 1)
		m_pdb_rate = 1500;
	else
		m_pdb_rate = 15000;

	ResetTimer( m_pdb_timer, m_pdb_rate);
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::SendSDBProg( bool p_status )
{
	m_sdb.SetFlag( CSDBPacket::_PROGRESSIVE_WIN, p_status );
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

int CGamePlayMgr::CurrentCredits(void)
{
	CString val = "";
	m_sdb.GetDBField( CSDBPacket::CREDIT_METER, val);
	return (Convert(val));
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int f_win_weighting[6] = 
{
	50,
	60,
	70,
	80,
	90,
	100
};

int f_win[6] = 
{
	0,
	2,
	4,
	6,
	8,
	10
};

int f_duration_weighting[3] = 
{
	90,	
	97,
	100
};

int f_duration[3] = 
{
	3,	
	30,
	60
};

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::AutoPlayStart(int p_min_bet, int p_max_bet)
{

	m_min_bet = p_min_bet;
	m_max_bet = p_max_bet;

	m_auto_play = true;
	PlayGameAuto();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::AutoPlayStop(void)
{
	g_timers->RemoveTimer( m_inter_game_timer );
	m_auto_play = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CGamePlayMgr::PlayGameAuto(void)
{
	g_timers->RemoveTimer( m_inter_game_timer );

	if( !m_auto_play )
		return;

	if(m_max_bet > CurrentCredits() )
	{
		AutoPlayStop();
		return;
	}

	int bet;
	if( m_min_bet == m_max_bet )
		bet = m_min_bet;

	else
		bet = m_min_bet + g_random->Number( m_max_bet - m_min_bet );

	int win = 0;
	int win_rand = g_random->Number(100);
	for(int i=0; i< 6; i++)
	{
		if( win_rand < f_win_weighting[i] )
		{
			win = bet*f_win[i];
			break;
		}
	}
	
	int duration = 3;
	/*
	int duration_rand = g_random->Number(100);
	for(i=0; i< 6; i++)
	{
		if( duration_rand < f_duration_weighting[i] )
		{
			duration = f_duration[i];
			break;
		}
	}
	*/
	if( m_state == IDLE )
		PlayGame( bet, win, duration );
}

//////////////////////////////////////////////////////////////////////////////////////////////////

// todo
// make these a single function, specify the packet id as parameter.

// todo
// the timeout values should be reset only when the packet is actually sent - 
// not when queues of force sent (since a delay is specified for that )

// todo
// when timer is reset, should maybe also delete the packet from the queue is its on there.

// this should be a function of the timer class
void CGamePlayMgr::ResetTimer(int& p_id, int p_rate)
{
	g_timers->RemoveTimer( p_id );
	p_id = g_timers->AddTimer( p_rate, this );
}

void CGamePlayMgr::ResetSDBTimer()
{
	ResetTimer( m_sdb_timer, m_sdb_rate );
}

void CGamePlayMgr::ResetPMDBTimer()
{
	ResetTimer( m_pmdb_timer, m_pmdb_rate );
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::DoorMain( bool p_val )
{
	
	m_sdb.SetFlag( CSDBPacket::_MAIN_DOOR, p_val );

	if( p_val )
		m_sdb.SetDBField( CSDBPacket::GAMES_PLAYED_DOOR, 0);

	//todo - this could be a function - SendModifiedPacket()
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::DoorSecurity( bool p_val )
{
	m_sdb.SetFlag( CSDBPacket::_SECURITY_CAGE, p_val );

	if( p_val )
		m_sdb.SetDBField( CSDBPacket::GAMES_PLAYED_DOOR, 0);

	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::ErrorMemory( bool p_val )
{
	m_sdb.SetFlag( CSDBPacket::_MEMORY_ERROR, p_val );

	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;

	//todo - when this flag set, only SDB meant to be output

}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::ErrorSelfAudit( bool p_val )
{
	m_sdb.SetFlag( CSDBPacket::_SELF_AUDIT_ERROR, p_val );
	
	if( p_val )
		m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, 100 );
	else
		m_sdb.IncrementDBField( CSDBPacket::CREDIT_METER, -100 );

	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;

}
//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::ErrorDisplay( bool p_val )
{
	m_sdb.SetFlag( CSDBPacket::_DISPLAY_ERROR, p_val );
	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::PowerDown( bool p_val )
{
	if( p_val == true )
	{
		PowerDown();
	}
	else
	{
		PowerUp();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::Sef( bool p_val )
{
	m_sdb.SetFlag( CSDBPacket::_SUBSIDUARY_SUSP, p_val );

	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayMgr::AuditMode( bool p_val )
{
	m_sdb.SetFlag( CSDBPacket::_AUDIT_MODE, p_val );

	m_sdb.IncrementSeqCounter();
	m_sdb.SetChecksum( m_sdb.CalculateChecksum() );	
	g_transmit_manager->Queue( &m_sdb, 80 );
	ResetTimer( m_sdb_timer, m_sdb_rate);
	m_sdb_changed = false;

	m_lockup = p_val;
}





