#if !defined(AFX_PDB2_H__DDB0CC94_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_PDB2_H__DDB0CC94_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_monitor.h : header file
//
#pragma warning(disable:4101)

#include "dlg_packet_interface.h"
#include "notify.h"
#include "XListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonitor dialog

class CDlgMonitor : public CDialog, public IDlgPacket, public INotifiable
{
// Construction
public:

	enum
	{
		FORMAT_ID = 0,
		RESERVED2,
		SYS_MODE_FAILURE_CODE,
		ACTIVE_GAME_ID,
		BASE_PTR,
		STATUS_FLAGS,
		SUBASS_FAILURE_FLAGS,
		OPERATOR_FLAGS,
		PAPER_LEVEL,
		SYS_FIRMWARE_VERS,
		GAME_FIRMWARE_VERS,
		CONTROL_VERS,
		CONFIG_VERS,
		BANNER_VERS,
		ATL_BANNER_VERS,
		REPORT_VERS,
		RESERVED24,
		MEM_SIG_VAL,
		RESERVED29,
		DATE_ACC_DATA,
		TIME_ACC_DATA,
		MASTER_RESET_DATE,
		MASTER_RESET_TIME,
		BILLS_IN,
		COIN_IN_DROP,
		COIN_IN_HOPPER,
		PLAYER_CARD_IN,
		HOPPER_FILLS,
		PRODUCT_IN,
		BILLS_COLLECTED,
		DROP_BOX_COLLECTED,
		COIN_OUT_HOPPER,
		PLAYER_CARD_OUT,
		CASH_TICKETS_MAN_PAYS,
		PRODUCT_OUT,
		TOTAL_PLAYED,
		TOTAL_WON,
		MAIN_NUM_FIELDS
	};

	enum
	{
		GK_GAME_ID = 0,
		GK_GAME_STATUS,
		GK_GAMES_PLAYED,
		GK_GAMES_WON,
		GK_CENTS_PLAYED,
		GK_CENTS_WON,
		GK_NUM_FIELDS
	};

	void AddMainSection(void);
	void AddGameKeySection(COLORREF p_colour);
	void DeleteGameKeySection();

	void AddGameId(uint p_game_id);

	//For IDlgPacket
	void DisplayPacket(const CVLCPacket& p_packet );
	void SetPacket(CVLCPacket& p_packet );
	CVlcData* GetVlcData();	
	void GetPacketName(CString& p_packet_name);

	int serialize(CArchive* pArchive);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDlgMonitor(CWnd* pParent = NULL);   // standard constructor

	
//	void GetStaticData( CPDB2Packet& p_packet );

// Dialog Data
	//{{AFX_DATA(CDlgMonitor)
	enum { IDD = IDD_MONITOR };
	CButton	m_ctrl_btn_snapshot;
	CStatic	m_ctrl_static_current_credits;
	CEdit	m_ctrl_edit_win;
	CEdit	m_ctrl_edit_manual_pay;
	CEdit	m_ctrl_edit_hopper_out;
	CEdit	m_ctrl_edit_hopper_in;
	CComboBox	m_ctrl_combo_game_list;
	CEdit	m_ctrl_edit_collect_drop;
	CEdit	m_ctrl_edit_bet;
	CEdit	m_ctrl_edit_drop_box_in;
	CButton	m_ctrl_btn_play_game;
	CButton	m_ctrl_btn_manual_pay;
	CButton	m_ctrl_btn_hopper_out;
	CButton	m_ctrl_btn_hopper_in;
	CButton	m_ctrl_btn_drop_box_in;
	CButton	m_ctrl_btn_collect_drop_box;
	CComboBox	m_ctrl_combo_cur_de;
	CXListCtrl	m_xlistctrl_monitor_packet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int m_game_key_count; //count of games sections currently shown in the list

	CToolTipCtrl m_tooltip;
	virtual BOOL OnInitDialog();

	void DisplayField(const CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset=0);
	void SetField(CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset=0 );
	void ProcessNotify( int p_id ); // for INotifiable


	//enable disable game play controls.
	void EnablePlayGame(bool p_enable);
	void EnableCreditInOut(bool p_enable);
	void EnableSnapshot(bool p_enable);
	void SetGamePlayControlEnable();

	// Generated message map functions
	//{{AFX_MSG(CDlgMonitor)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeComboCurDe();
	afx_msg void OnButtonCollectDropBox();
	afx_msg void OnButtonDropBoxIn();
	afx_msg void OnButtonHopperIn();
	afx_msg void OnButtonHopperOut();
	afx_msg void OnButtonManualPay();
	afx_msg void OnButtonPlayGame();
	afx_msg void OnButtonSnapshot();
	afx_msg void OnSelchangeComboGameList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDlgMonitor* g_dlg_monitor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDB2_H__DDB0CC94_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
