
#ifndef __GAMEPLAY_H
#define __GAMEPLAY_H

#include "xseries_packet.h"
#include "timers.h"
#include "notify.h"


typedef unsigned int uint;

class CGamePlayMgr : public ITimer, public INotifiable
{
//	friend class CTicketIn;
//	friend class CTicketOut;
//	friend class CCCCE;
	friend class CGamePlay;

public:

	static void Initialise(void);
	static void Destroy(void);

	void BeginTransmitting(void);
	void InitialiseDataBlocks(void);
	
	void BankNoteIn(uint p_amount);
	void HopperIn(void);
	void CashBoxIn(void);
	void CoinInHop(void);
	void CoinInCB(void);
	void HopperPay(void);
	void CancelCredit(void);
	void CancelCreditConfirm(bool p_confirm);
	
	void PlayGame(int p_bet, int p_win, int p_duration);
	bool AutoPlayStarted(void) { return m_auto_play; }
	void AutoPlayStart(int p_min_bet, int p_max_bet);
	void AutoPlayStop(void);
	
	void ResetMeters(void);
	void ResetProg(void);
	int CurrentCredits(void); 

	void DoorMain( bool p_val );
	void DoorSecurity( bool p_val );
	void ErrorMemory( bool p_val );
	void ErrorSelfAudit( bool p_val );
	void ErrorDisplay( bool p_val );
	void PowerDown( bool p_val );
	void Sef( bool p_val );
	void AuditMode( bool p_val );

	CSDBPacket& GetSDB(void)  {return m_sdb;}
	CPMDBPacket& GetPMDB(void)  {return m_pmdb;}
	CMMDBPacket& GetMMDB(void)  {return m_mmdb;}
	CFDBPacket& GetFDB(void)  {return m_fdb;}
	CPDB1Packet& GetPDB1(void)  {return m_pdb1;}
	CPDB2Packet& GetPDB2(void)  {return m_pdb2;}
	//const CSDBPacket& GetSDB(void) const {return m_sdb;}

	void ResetSDBTimer(void);
	void ResetPMDBTimer(void);

protected:
	void ProcessTimeout( int p_id ); // for ITimer
	void ProcessNotify( int p_id ); // for INotifiable

private:

	enum
	{
		IDLE = 0,
		PLAYING,
		CHECKING_LP,
		CLEARING_LP,
		CHECKING_SAP,
		CLEARING_SAP
	};

	CGamePlayMgr();
	~CGamePlayMgr();
	
	CSDBPacket		m_sdb;
	CPMDBPacket		m_pmdb;
	CFDBPacket		m_fdb;
	CPDB1Packet		m_pdb1;
	CPDB2Packet		m_pdb2;
	CMMDBPacket		m_mmdb;

	// indicates if DB has been changed - for updating sequence counters
	// these need to be set to true when a block has changed, 
	// when block due to be sent - if true increment counter then set back to false
	bool m_sdb_changed;
	bool m_fdb_changed;
	bool m_pdb1_changed;
	bool m_pdb2_changed;
	
	// send rates
	int	m_sdb_rate;
	int	m_pmdb_rate;
	int	m_fdb_rate;
	int	m_pdb_rate;
	int	m_mmdb_rate;
	
	// timers
	//data-blocks
	int	m_sdb_timer;
	int	m_pmdb_timer;
	int	m_fdb_timer;
	int	m_pdb_timer;
	int	m_mmdb_timer;
	//game play
	int	m_game_play_timer;
	int	m_inter_game_timer;
	int	m_hopper_pay_timer;
	
	//statuses
	bool	m_auto_play;
	bool  m_send_fdb;
	bool  m_send_pdb;
	bool  m_lockup;
	bool  m_power_up;
	bool	m_first_power_up;
	bool  m_hopper_pay_in_progress;
	bool	m_cancel_cr_in_progress;

	// game parameters
	int	m_win_current_game;
	int	m_max_bet;
	int	m_min_bet;
	int	m_sap_level;
	int	m_lp_level;
	int	m_sap_hits_pending;
	int	m_state;

	float m_sap_cur[4];	// current value of the 4 SAP levels.
	float m_sap_inc[4];  // increments for the 4 SAP levels
	int	m_cur_sap_win;

	//functions
	void GameEnd(void);
	void GameFinalise(void);
	void CoinOut(void);

	void ResetTimer(int& p_id, int p_rate);
	void PlayGameAuto(void);

	void DoProgressives(void);
	void CheckNextLPLevel(void);
	void CheckNextSAPLevel(void);
	void SendFDBProg( int p_level ); 
	void SendPDBProg( int p_level ); 
	void SendSDBProg( bool p_status );
	
	void PowerUp(void);
	void PowerDown(void);
};

extern CGamePlayMgr* g_game_mgr;

#endif
