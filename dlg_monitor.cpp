
#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_monitor.h"
#include "dlg_main.h"
#include "vlc_id.h"
#include "vlc_id_list.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonitor dialog

CDlgMonitor* g_dlg_monitor = 0;

CDlgMonitor::CDlgMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMonitor::IDD, pParent),
	m_game_key_count(0)
{
		g_notify_mgr->Register(CNotify::ACTIVE_ID_CHANGED,this);
	//{{AFX_DATA_INIT(CDlgMonitor)
	//}}AFX_DATA_INIT
}
/////////////////////////////////////////////////////////////////////////////

void CDlgMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMonitor)
	DDX_Control(pDX, IDC_BUTTON_SNAPSHOT, m_ctrl_btn_snapshot);
	DDX_Control(pDX, IDC_STATIC_CREDITS, m_ctrl_static_current_credits);
	DDX_Control(pDX, IDC_EDIT_WIN, m_ctrl_edit_win);
	DDX_Control(pDX, IDC_EDIT_MANUAL_PAY, m_ctrl_edit_manual_pay);
	DDX_Control(pDX, IDC_EDIT_HOPPER_OUT, m_ctrl_edit_hopper_out);
	DDX_Control(pDX, IDC_EDIT_HOPPER_IN, m_ctrl_edit_hopper_in);
	DDX_Control(pDX, IDC_COMBO_GAME_LIST, m_ctrl_combo_game_list);
	DDX_Control(pDX, IDC_EDIT_COLLECT_DROP, m_ctrl_edit_collect_drop);
	DDX_Control(pDX, IDC_EDIT_BET, m_ctrl_edit_bet);
	DDX_Control(pDX, IDC_EDIT_DROP_BOX_IN, m_ctrl_edit_drop_box_in);
	DDX_Control(pDX, IDC_BUTTON_PLAY_GAME, m_ctrl_btn_play_game);
	DDX_Control(pDX, IDC_BUTTON_MANUAL_PAY, m_ctrl_btn_manual_pay);
	DDX_Control(pDX, IDC_BUTTON_HOPPER_OUT, m_ctrl_btn_hopper_out);
	DDX_Control(pDX, IDC_BUTTON_HOPPER_IN, m_ctrl_btn_hopper_in);
	DDX_Control(pDX, IDC_BUTTON_DROP_BOX_IN, m_ctrl_btn_drop_box_in);
	DDX_Control(pDX, IDC_BUTTON_COLLECT_DROP_BOX, m_ctrl_btn_collect_drop_box);
	DDX_Control(pDX, IDC_COMBO_CUR_DE, m_ctrl_combo_cur_de);
	DDX_Control(pDX, IDC_XLISTCTRL_MONITOR_PACKET, m_xlistctrl_monitor_packet);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgMonitor, CDialog)
	//{{AFX_MSG_MAP(CDlgMonitor)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_CUR_DE, OnSelchangeComboCurDe)
	ON_BN_CLICKED(IDC_BUTTON_COLLECT_DROP_BOX, OnButtonCollectDropBox)
	ON_BN_CLICKED(IDC_BUTTON_DROP_BOX_IN, OnButtonDropBoxIn)
	ON_BN_CLICKED(IDC_BUTTON_HOPPER_IN, OnButtonHopperIn)
	ON_BN_CLICKED(IDC_BUTTON_HOPPER_OUT, OnButtonHopperOut)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_PAY, OnButtonManualPay)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_GAME, OnButtonPlayGame)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, OnButtonSnapshot)
	ON_CBN_SELCHANGE(IDC_COMBO_GAME_LIST, OnSelchangeComboGameList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMonitor message handlers


BOOL CDlgMonitor::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

/////////////////////////////////////////////////////////////////////////////

void CDlgMonitor::AddMainSection(void)
{
	m_xlistctrl_monitor_packet.LockWindowUpdate();	

	m_xlistctrl_monitor_packet.InsertItem(FORMAT_ID, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(FORMAT_ID,1,"Format ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(FORMAT_ID,2,"05",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(FORMAT_ID,2, CXEdit::HEX,2);

	m_xlistctrl_monitor_packet.InsertItem(RESERVED2, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(RESERVED2,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(RESERVED2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(RESERVED2,2, CXEdit::HEX,2);

	m_xlistctrl_monitor_packet.InsertItem(SYS_MODE_FAILURE_CODE, "3-4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(SYS_MODE_FAILURE_CODE,1,"Sys mode/Failure code",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(SYS_MODE_FAILURE_CODE,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(SYS_MODE_FAILURE_CODE,2, CXEdit::HEX,4);

	m_xlistctrl_monitor_packet.InsertItem(ACTIVE_GAME_ID, "5",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(ACTIVE_GAME_ID,1,"Active game ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(ACTIVE_GAME_ID,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(ACTIVE_GAME_ID,2, CXEdit::HEX,2);

	m_xlistctrl_monitor_packet.InsertItem(BASE_PTR, "6-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(BASE_PTR,1,"BASE Ptr",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(BASE_PTR,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(BASE_PTR,2, CXEdit::DEC,6);

	m_xlistctrl_monitor_packet.InsertItem(STATUS_FLAGS, "9-10",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(STATUS_FLAGS,1,"Status Flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(STATUS_FLAGS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(STATUS_FLAGS,2, CXEdit::HEX,4);

	m_xlistctrl_monitor_packet.InsertItem(SUBASS_FAILURE_FLAGS, "11-12",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(SUBASS_FAILURE_FLAGS,1,"Subassembly Flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(SUBASS_FAILURE_FLAGS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(SUBASS_FAILURE_FLAGS,2, CXEdit::HEX,4);

	m_xlistctrl_monitor_packet.InsertItem(OPERATOR_FLAGS, "13-14",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(OPERATOR_FLAGS,1,"Operator Flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(OPERATOR_FLAGS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(OPERATOR_FLAGS,2, CXEdit::HEX,4);

	m_xlistctrl_monitor_packet.InsertItem(PAPER_LEVEL, "15-16",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(PAPER_LEVEL,1,"Paper level",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(PAPER_LEVEL,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(PAPER_LEVEL,2, CXEdit::DEC,6);

	m_xlistctrl_monitor_packet.InsertItem(SYS_FIRMWARE_VERS, "17",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(SYS_FIRMWARE_VERS,1,"System Firmware version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(SYS_FIRMWARE_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(SYS_FIRMWARE_VERS,2, CXEdit::DEC,3);

	m_xlistctrl_monitor_packet.InsertItem(GAME_FIRMWARE_VERS, "18",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(GAME_FIRMWARE_VERS,1,"Game Firmware version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(GAME_FIRMWARE_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(GAME_FIRMWARE_VERS,2, CXEdit::DEC,3);

	m_xlistctrl_monitor_packet.InsertItem(CONTROL_VERS, "19",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(CONTROL_VERS,1,"Control version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(CONTROL_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(CONTROL_VERS,2, CXEdit::DEC,3);

	m_xlistctrl_monitor_packet.InsertItem(CONFIG_VERS, "20",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(CONFIG_VERS,1,"Config version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(CONFIG_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(CONFIG_VERS,2, CXEdit::DEC,3);

	m_xlistctrl_monitor_packet.InsertItem(BANNER_VERS, "21",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(BANNER_VERS,1,"Banner version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(BANNER_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(BANNER_VERS,2, CXEdit::DEC,3);	

	m_xlistctrl_monitor_packet.InsertItem(ATL_BANNER_VERS, "22",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(ATL_BANNER_VERS,1,"Alt Banner version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(ATL_BANNER_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(ATL_BANNER_VERS,2, CXEdit::DEC,3);

	m_xlistctrl_monitor_packet.InsertItem(REPORT_VERS, "23",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(REPORT_VERS,1,"Report version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(REPORT_VERS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(REPORT_VERS,2, CXEdit::DEC,3);

	m_xlistctrl_monitor_packet.InsertItem(RESERVED24, "24-26",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(RESERVED24,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(RESERVED24,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(RESERVED24,2, CXEdit::HEX,6);

	m_xlistctrl_monitor_packet.InsertItem(MEM_SIG_VAL, "27-28",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(MEM_SIG_VAL,1,"Memory sig value",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(MEM_SIG_VAL,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(MEM_SIG_VAL,2, CXEdit::HEX,4);

	m_xlistctrl_monitor_packet.InsertItem(RESERVED29, "29-32",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(RESERVED29,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(RESERVED29,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(RESERVED29,2, CXEdit::HEX,8);

	m_xlistctrl_monitor_packet.InsertItem(DATE_ACC_DATA, "33-34",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(DATE_ACC_DATA,1,"Date accounting data",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(DATE_ACC_DATA,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(DATE_ACC_DATA,2, CXEdit::DEC,6);

	m_xlistctrl_monitor_packet.InsertItem(TIME_ACC_DATA, "35-36",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(TIME_ACC_DATA,1,"Time accounting data",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(TIME_ACC_DATA,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(TIME_ACC_DATA,2, CXEdit::DEC,6);

	m_xlistctrl_monitor_packet.InsertItem(MASTER_RESET_DATE, "37-38",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(MASTER_RESET_DATE,1,"Master reset date",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(MASTER_RESET_DATE,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(MASTER_RESET_DATE,2, CXEdit::DEC,6);

	m_xlistctrl_monitor_packet.InsertItem(MASTER_RESET_TIME, "39-40",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(MASTER_RESET_TIME,1,"Master reset time",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(MASTER_RESET_TIME,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(MASTER_RESET_TIME,2, CXEdit::DEC,6);

	m_xlistctrl_monitor_packet.InsertItem(BILLS_IN, "41-44",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(BILLS_IN,1,"Bills In - cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(BILLS_IN,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(BILLS_IN,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(COIN_IN_DROP, "45-48",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(COIN_IN_DROP,1,"Coins In drop box",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(COIN_IN_DROP,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(COIN_IN_DROP,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(COIN_IN_HOPPER, "49-52",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(COIN_IN_HOPPER,1,"Coins In hopper",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(COIN_IN_HOPPER,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(COIN_IN_HOPPER,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(PLAYER_CARD_IN, "53-56",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(PLAYER_CARD_IN,1,"Player card in",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(PLAYER_CARD_IN,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(PLAYER_CARD_IN,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(HOPPER_FILLS, "57-60",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(HOPPER_FILLS,1,"Hopper fills",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(HOPPER_FILLS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(HOPPER_FILLS,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(PRODUCT_IN, "61-64",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(PRODUCT_IN,1,"Product in",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(PRODUCT_IN,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(PRODUCT_IN,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(BILLS_COLLECTED, "65-68",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(BILLS_COLLECTED,1,"Bills collected",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(BILLS_COLLECTED,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(BILLS_COLLECTED,2, CXEdit::DEC,8);	

	m_xlistctrl_monitor_packet.InsertItem(DROP_BOX_COLLECTED, "69-72",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(DROP_BOX_COLLECTED,1,"Drop Box collected",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(DROP_BOX_COLLECTED,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(DROP_BOX_COLLECTED,2, CXEdit::DEC,8);
	
	m_xlistctrl_monitor_packet.InsertItem(COIN_OUT_HOPPER, "73-76",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(COIN_OUT_HOPPER,1,"Coin out hopper",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(COIN_OUT_HOPPER,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(COIN_OUT_HOPPER,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(PLAYER_CARD_OUT, "77-80",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(PLAYER_CARD_OUT,1,"Player card out",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(PLAYER_CARD_OUT,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(PLAYER_CARD_OUT,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(CASH_TICKETS_MAN_PAYS, "81-84",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(CASH_TICKETS_MAN_PAYS,1,"Cash tix / Man pays",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(CASH_TICKETS_MAN_PAYS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(CASH_TICKETS_MAN_PAYS,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(PRODUCT_OUT, "85-88",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(PRODUCT_OUT,1,"Product out",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(PRODUCT_OUT,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(PRODUCT_OUT,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(TOTAL_PLAYED, "89-92",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(TOTAL_PLAYED,1,"Total played",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(TOTAL_PLAYED,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(TOTAL_PLAYED,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(TOTAL_WON, "93-96",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(TOTAL_WON,1,"Total won",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(TOTAL_WON,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(TOTAL_WON,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.UnlockWindowUpdate();

	int rows = m_xlistctrl_monitor_packet.GetItemCount();
	ASSERT(rows == MAIN_NUM_FIELDS );
}

/////////////////////////////////////////////////////////////////////////////

void CDlgMonitor::AddGameKeySection(COLORREF p_colour)
{
	int start = MAIN_NUM_FIELDS + m_game_key_count*GK_NUM_FIELDS;

	m_xlistctrl_monitor_packet.LockWindowUpdate();	

	m_xlistctrl_monitor_packet.InsertItem(start+GK_GAME_ID, "1",p_colour,RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAME_ID,1,"Game ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAME_ID,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(start+GK_GAME_ID,2, CXEdit::DEC,3);
	
	m_xlistctrl_monitor_packet.InsertItem(start+GK_GAME_STATUS, "2",p_colour,RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAME_STATUS,1,"Game Status",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAME_STATUS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(start+GK_GAME_STATUS,2, CXEdit::HEX,2);

	m_xlistctrl_monitor_packet.InsertItem(start+GK_GAMES_PLAYED, "3-5",p_colour,RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAMES_PLAYED,1,"Games played",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAMES_PLAYED,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(start+GK_GAMES_PLAYED,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(start+GK_GAMES_WON, "6-8",p_colour,RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAMES_WON,1,"Games won",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(start+GK_GAMES_WON,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(start+GK_GAMES_WON,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(start+GK_CENTS_PLAYED, "9-12",p_colour,RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(start+GK_CENTS_PLAYED,1,"Cents played",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(start+GK_CENTS_PLAYED,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(start+GK_CENTS_PLAYED,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.InsertItem(start+GK_CENTS_WON, "13-16",p_colour,RGB(255,255,255));
	m_xlistctrl_monitor_packet.SetItemText(start+GK_CENTS_WON,1,"Cents won",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_monitor_packet.SetItemText(start+GK_CENTS_WON,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_monitor_packet.SetEdit(start+GK_CENTS_WON,2, CXEdit::DEC,8);

	m_xlistctrl_monitor_packet.UnlockWindowUpdate();

	m_game_key_count++;
	
	int rows = m_xlistctrl_monitor_packet.GetItemCount();
	ASSERT(rows == MAIN_NUM_FIELDS + m_game_key_count*GK_NUM_FIELDS);
}

/////////////////////////////////////////////////////////////////////////////
void CDlgMonitor::DeleteGameKeySection()
{
	if(m_game_key_count <= 0)
		return;
	int rows = m_xlistctrl_monitor_packet.GetItemCount();
	ASSERT(rows == MAIN_NUM_FIELDS + m_game_key_count*GK_NUM_FIELDS);	

	for(int item = rows-1; item >= rows-GK_NUM_FIELDS; item--)
		m_xlistctrl_monitor_packet.DeleteItem(item);

	m_game_key_count--;

	rows = m_xlistctrl_monitor_packet.GetItemCount();
	ASSERT(rows == MAIN_NUM_FIELDS + m_game_key_count*GK_NUM_FIELDS);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDlgMonitor::OnInitDialog()
{
	CDialog::OnInitDialog();	// CG: This was added by the ToolTips component.
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}

////////////////////////////////////////////////////////////////
// set up dlg for monitor packet data 
//////////////////////////////////////////////////////////////////
// XListCtrl must have LVS_EX_FULLROWSELECT if combo or edit boxes are used
	

	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_monitor_packet.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	CRect rect;
	m_xlistctrl_monitor_packet.GetWindowRect(&rect);
	int width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar


	// add columns
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "Bytes";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/6;
	m_xlistctrl_monitor_packet.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_monitor_packet.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_monitor_packet.InsertColumn(2, &lvcolumn);

	m_xlistctrl_monitor_packet.LockWindowUpdate();	
	m_xlistctrl_monitor_packet.DeleteAllItems();
	m_xlistctrl_monitor_packet.LockWindowUpdate();	

	AddMainSection();

	//These need to be in the same order as defined in CVlcMonitorData
	m_ctrl_combo_cur_de.AddString("Current");
	m_ctrl_combo_cur_de.AddString("Day End");
	m_ctrl_combo_cur_de.SetCurSel((int)(CVlcMonitorData::MONITOR_CURRENT));

	if(g_vlc_id_list->GetActiveId()==NULL) //should be
	{
		m_xlistctrl_monitor_packet.EnableWindow(false);
		m_ctrl_combo_cur_de.EnableWindow(false);
		EnablePlayGame(false);
		EnableCreditInOut(false);
		EnableSnapshot(false);
	}
	return TRUE;	// CG: This was added by the ToolTips component.
}

void CDlgMonitor::SetGamePlayControlEnable()
{
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)GetVlcData();
	if(monitor_data == NULL)
			return;
	if( m_ctrl_combo_cur_de.GetCurSel() == (int)(CVlcMonitorData::MONITOR_CURRENT) )
	{
		EnableCreditInOut(true);
		EnablePlayGame( monitor_data->GetGameCount() > 0);
		EnableSnapshot(false);
	}
	else
	{
		EnableCreditInOut(false);
		EnablePlayGame(false);
		EnableSnapshot(true);
	}
}

void CDlgMonitor::ProcessNotify( int p_id ) // for INotifiable
{
	if(p_id == CNotify::ACTIVE_ID_CHANGED)
	{
		m_xlistctrl_monitor_packet.EnableWindow(true);
		m_ctrl_combo_cur_de.EnableWindow(true);

		//Update the list of configured games in the combo
		CVlcMonitorData* monitor_data = (CVlcMonitorData*)GetVlcData();
		if(monitor_data == NULL)
			return;
		m_ctrl_combo_game_list.ResetContent(); 
		std::vector<uint> id_list;
		monitor_data->GetGameIdList(id_list);
		std::vector<uint>::iterator itr = id_list.begin();
		while(itr != id_list.end() )
		{
			m_ctrl_combo_game_list.AddString( Convert(*itr) );
			itr++;
		}
		SetGamePlayControlEnable();
	}
}

//enable disable game play controls.
void CDlgMonitor::EnablePlayGame(bool p_enable)
{
	m_ctrl_btn_play_game.EnableWindow(p_enable);
	m_ctrl_combo_game_list.EnableWindow(p_enable);
	m_ctrl_edit_bet.EnableWindow(p_enable);
	m_ctrl_edit_win.EnableWindow(p_enable);	
}
void CDlgMonitor::EnableCreditInOut(bool p_enable)
{
	m_ctrl_edit_manual_pay.EnableWindow(p_enable);	
	m_ctrl_edit_hopper_out.EnableWindow(p_enable);	
	m_ctrl_edit_hopper_in.EnableWindow(p_enable);	
	m_ctrl_edit_drop_box_in.EnableWindow(p_enable);	
	m_ctrl_edit_collect_drop.EnableWindow(p_enable);	
	m_ctrl_btn_manual_pay.EnableWindow(p_enable);	
	m_ctrl_btn_hopper_out.EnableWindow(p_enable);	
	m_ctrl_btn_hopper_in.EnableWindow(p_enable);	
	m_ctrl_btn_drop_box_in.EnableWindow(p_enable);	
	m_ctrl_btn_collect_drop_box.EnableWindow(p_enable);	
}
void CDlgMonitor::EnableSnapshot(bool p_enable)
{
	m_ctrl_btn_snapshot.EnableWindow(p_enable);	
}


//////////////////////////////////////////////////////////////////
//Implement IDlgPacket

inline void CDlgMonitor::DisplayField(const CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset)
{
	//p_offset is the offset to the start of the section.  e.g. system section offset is 8 if there is 1 game key etc
	CString value;
	p_packet.GetDataFieldFormatted(value, p_field_id, p_offset );
	m_xlistctrl_monitor_packet.SetItemText(p_field_pos,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
}
inline void CDlgMonitor::SetField(CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset)
{
	CString value;
	value = m_xlistctrl_monitor_packet.GetItemText(p_field_pos,2);
	p_packet.SetDataFieldFormatted(value, p_field_id, p_offset);
}

void CDlgMonitor::DisplayPacket(const CVLCPacket& p_packet )
{
	//check that its the right packet
	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_MONITOR)
		return;

	//Note: each game takes up 16 bytes.  No padding needed for monitor packets - always even multiple of 8

	int data_length = p_packet.GetDataLength();	

	//main section is 96 bytes
	//each game section is 16 bytes
	
	if(data_length < CVLCPacket::LENGTH_MONITOR_SYSTEM )
		return; // error - shouldn't happen

	int byte_of_game_data = data_length - CVLCPacket::LENGTH_MONITOR_SYSTEM;
	if(data_length > CVLCPacket::LENGTH_MONITOR_SYSTEM)
	{
		if( byte_of_game_data%CVLCPacket::LENGTH_MONITOR_GAME != 0)
			return; // error - shouldn't happen
	}

	int num_games = byte_of_game_data/CVLCPacket::LENGTH_MONITOR_GAME; 
	
	m_xlistctrl_monitor_packet.LockWindowUpdate();	

	DisplayField(p_packet, FORMAT_ID,				CVLCPacket::MON_FORMAT_ID);
	DisplayField(p_packet, RESERVED2,				CVLCPacket::MON_ERSERVED2);
	DisplayField(p_packet, SYS_MODE_FAILURE_CODE,	CVLCPacket::MON_SYS_MODE);
	DisplayField(p_packet, ACTIVE_GAME_ID,			CVLCPacket::MON_ACTIVE_GAME_ID);
	DisplayField(p_packet, BASE_PTR,				CVLCPacket::MON_BASE);
	DisplayField(p_packet, STATUS_FLAGS,			CVLCPacket::MON_STATUS_FLAGS);
	DisplayField(p_packet, SUBASS_FAILURE_FLAGS,	CVLCPacket::MON_SUBASSEMBLY_FAILURE_FLAGS);
	DisplayField(p_packet, OPERATOR_FLAGS,			CVLCPacket::MON_OPERATOR_FLAGS);
	DisplayField(p_packet, PAPER_LEVEL,				CVLCPacket::MON_PAPER_LEVEL);
	DisplayField(p_packet, SYS_FIRMWARE_VERS,		CVLCPacket::MON_SYSTEM_FIRMWARE_VERS);
	DisplayField(p_packet, GAME_FIRMWARE_VERS,		CVLCPacket::MON_GAME_FIRMWARE_VERS);
	DisplayField(p_packet, CONTROL_VERS,			CVLCPacket::MON_CONTROL_VERS);
	DisplayField(p_packet, CONFIG_VERS,				CVLCPacket::MON_CONFIG_VERS);
	DisplayField(p_packet, BANNER_VERS,				CVLCPacket::MON_BANNER_VERS);
	DisplayField(p_packet, ATL_BANNER_VERS,			CVLCPacket::MON_ALT_BANNER_VERS);
	DisplayField(p_packet, REPORT_VERS,				CVLCPacket::MON_REPORT_VERS);
	DisplayField(p_packet, RESERVED24,				CVLCPacket::MON_ERSERVED24);
	DisplayField(p_packet, MEM_SIG_VAL,				CVLCPacket::MON_MEM_SIG_VALUE);
	DisplayField(p_packet, RESERVED29,				CVLCPacket::MON_ERSERVED29);
	DisplayField(p_packet, DATE_ACC_DATA,			CVLCPacket::MON_DATE_FOR_ACCOUNTING_DATE);
	DisplayField(p_packet, TIME_ACC_DATA,			CVLCPacket::MON_TIME_FOR_ACCOUNTING_DATA);
	DisplayField(p_packet, MASTER_RESET_DATE,		CVLCPacket::MON_MASTER_RESET_DATE);
	DisplayField(p_packet, MASTER_RESET_TIME,		CVLCPacket::MON_MASTER_RESET_TIME);
	DisplayField(p_packet, BILLS_IN,				CVLCPacket::MON_BILLS_IN);
	DisplayField(p_packet, COIN_IN_DROP,			CVLCPacket::MON_COIN_IN_DROP_BOX);
	DisplayField(p_packet, COIN_IN_HOPPER,			CVLCPacket::MON_COIN_IN_HOPPER);
	DisplayField(p_packet, PLAYER_CARD_IN,			CVLCPacket::MON_PLAYER_DEBIT_CARD_IN);
	DisplayField(p_packet, HOPPER_FILLS,			CVLCPacket::MON_HOPPER_FILLS);
	DisplayField(p_packet, PRODUCT_IN,				CVLCPacket::MON_PRODUCT_IN);
	DisplayField(p_packet, BILLS_COLLECTED,			CVLCPacket::MON_BILLS_COLLECTED);
	DisplayField(p_packet, DROP_BOX_COLLECTED,		CVLCPacket::MON_DROP_BOX_COLLECTED);
	DisplayField(p_packet, COIN_OUT_HOPPER,			CVLCPacket::MON_COIN_OUT_HOPPER);
	DisplayField(p_packet, PLAYER_CARD_OUT,			CVLCPacket::MON_PLAYER_DEBIT_OUT);
	DisplayField(p_packet, CASH_TICKETS_MAN_PAYS,	CVLCPacket::MON_CASH_TICKETS_MANUAL_PAYS);
	DisplayField(p_packet, PRODUCT_OUT,				CVLCPacket::MON_PRODUCT_OUT);
	DisplayField(p_packet, TOTAL_PLAYED,			CVLCPacket::MON_TOTAL_PLAYED);
	DisplayField(p_packet, TOTAL_WON,				CVLCPacket::MON_TOTAL_WON);

	int rows = m_xlistctrl_monitor_packet.GetItemCount();

	while(m_game_key_count < num_games)
		AddGameKeySection(RGB(0,0,0)); //this increments m_game_key_count
	while(m_game_key_count > num_games )
		DeleteGameKeySection();	//this decrements m_game_key_count

	int pkt_offset = CVLCPacket::LENGTH_MONITOR_SYSTEM; 
	int field_offset = MAIN_NUM_FIELDS; 
	for(int game=0; game<num_games; game++)
	{
		DisplayField(p_packet, field_offset+GK_GAME_ID,			CVLCPacket::MON_GAME_ID,		pkt_offset);
		DisplayField(p_packet, field_offset+GK_GAME_STATUS,		CVLCPacket::MON_GAME_STATUS,	pkt_offset);
		DisplayField(p_packet, field_offset+GK_GAMES_PLAYED,	CVLCPacket::MON_GAME_GAMES_PLAYED,	pkt_offset);
		DisplayField(p_packet, field_offset+GK_GAMES_WON,		CVLCPacket::MON_GAME_GAMES_WON,	pkt_offset);
		DisplayField(p_packet, field_offset+GK_CENTS_PLAYED,	CVLCPacket::MON_GAME_CENTS_PLAYED,	pkt_offset);
		DisplayField(p_packet, field_offset+GK_CENTS_WON,		CVLCPacket::MON_GAME_CENTS_WON,	pkt_offset);
		
		pkt_offset += CVLCPacket::LENGTH_MONITOR_GAME;
		field_offset += GK_NUM_FIELDS;
	}

	m_xlistctrl_monitor_packet.UnlockWindowUpdate();	
}


////////////////////////////////////////////////////////////////////////////////////
/*
Set the packet in the dialog m_xlistctrl_datetime
*/

void CDlgMonitor::SetPacket(CVLCPacket& p_packet )
{
	CVlcData* monitor_data = GetVlcData();
	if(monitor_data==NULL)
		return;
	//check that its the right packet
	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_MONITOR)
		return;

	int required_data_length = CVLCPacket::LENGTH_MONITOR_SYSTEM + m_game_key_count*CVLCPacket::LENGTH_MONITOR_GAME;

	p_packet.SetDataLength(required_data_length);

	SetField(p_packet, FORMAT_ID,				CVLCPacket::MON_FORMAT_ID);
	SetField(p_packet, RESERVED2,				CVLCPacket::MON_ERSERVED2);
	SetField(p_packet, SYS_MODE_FAILURE_CODE,	CVLCPacket::MON_SYS_MODE);
	SetField(p_packet, ACTIVE_GAME_ID,			CVLCPacket::MON_ACTIVE_GAME_ID);
	SetField(p_packet, BASE_PTR,				CVLCPacket::MON_BASE);
	SetField(p_packet, STATUS_FLAGS,			CVLCPacket::MON_STATUS_FLAGS);
	SetField(p_packet, SUBASS_FAILURE_FLAGS,	CVLCPacket::MON_SUBASSEMBLY_FAILURE_FLAGS);
	SetField(p_packet, OPERATOR_FLAGS,			CVLCPacket::MON_OPERATOR_FLAGS);
	SetField(p_packet, PAPER_LEVEL,				CVLCPacket::MON_PAPER_LEVEL);
	SetField(p_packet, SYS_FIRMWARE_VERS,		CVLCPacket::MON_SYSTEM_FIRMWARE_VERS);
	SetField(p_packet, GAME_FIRMWARE_VERS,		CVLCPacket::MON_GAME_FIRMWARE_VERS);
	SetField(p_packet, CONTROL_VERS,			CVLCPacket::MON_CONTROL_VERS);
	SetField(p_packet, CONFIG_VERS,				CVLCPacket::MON_CONFIG_VERS);
	SetField(p_packet, BANNER_VERS,				CVLCPacket::MON_BANNER_VERS);
	SetField(p_packet, ATL_BANNER_VERS,			CVLCPacket::MON_ALT_BANNER_VERS);
	SetField(p_packet, REPORT_VERS,				CVLCPacket::MON_REPORT_VERS);
	SetField(p_packet, RESERVED24,				CVLCPacket::MON_ERSERVED24);
	SetField(p_packet, MEM_SIG_VAL,				CVLCPacket::MON_MEM_SIG_VALUE);
	SetField(p_packet, RESERVED29,				CVLCPacket::MON_ERSERVED29);
	SetField(p_packet, DATE_ACC_DATA,			CVLCPacket::MON_DATE_FOR_ACCOUNTING_DATE);
	SetField(p_packet, TIME_ACC_DATA,			CVLCPacket::MON_TIME_FOR_ACCOUNTING_DATA);
	SetField(p_packet, MASTER_RESET_DATE,		CVLCPacket::MON_MASTER_RESET_DATE);
	SetField(p_packet, MASTER_RESET_TIME,		CVLCPacket::MON_MASTER_RESET_TIME);
	SetField(p_packet, BILLS_IN,				CVLCPacket::MON_BILLS_IN);
	SetField(p_packet, COIN_IN_DROP,			CVLCPacket::MON_COIN_IN_DROP_BOX);
	SetField(p_packet, COIN_IN_HOPPER,			CVLCPacket::MON_COIN_IN_HOPPER);
	SetField(p_packet, PLAYER_CARD_IN,			CVLCPacket::MON_PLAYER_DEBIT_CARD_IN);
	SetField(p_packet, HOPPER_FILLS,			CVLCPacket::MON_HOPPER_FILLS);
	SetField(p_packet, PRODUCT_IN,				CVLCPacket::MON_PRODUCT_IN);
	SetField(p_packet, BILLS_COLLECTED,			CVLCPacket::MON_BILLS_COLLECTED);
	SetField(p_packet, DROP_BOX_COLLECTED,		CVLCPacket::MON_DROP_BOX_COLLECTED);
	SetField(p_packet, COIN_OUT_HOPPER,			CVLCPacket::MON_COIN_OUT_HOPPER);
	SetField(p_packet, PLAYER_CARD_OUT,			CVLCPacket::MON_PLAYER_DEBIT_OUT);
	SetField(p_packet, CASH_TICKETS_MAN_PAYS,	CVLCPacket::MON_CASH_TICKETS_MANUAL_PAYS);
	SetField(p_packet, PRODUCT_OUT,				CVLCPacket::MON_PRODUCT_OUT);
	SetField(p_packet, TOTAL_PLAYED,			CVLCPacket::MON_TOTAL_PLAYED);
	SetField(p_packet, TOTAL_WON,				CVLCPacket::MON_TOTAL_WON);

	int pkt_offset = CVLCPacket::LENGTH_MONITOR_SYSTEM; 
	int field_offset = MAIN_NUM_FIELDS; 
	for(int game=0; game < m_game_key_count; game++)
	{
		SetField(p_packet, field_offset+GK_GAME_ID,		CVLCPacket::MON_GAME_ID,	pkt_offset);
		SetField(p_packet, field_offset+GK_GAME_STATUS,	CVLCPacket::MON_GAME_STATUS,	pkt_offset);
		SetField(p_packet, field_offset+GK_GAMES_PLAYED,	CVLCPacket::MON_GAME_GAMES_PLAYED,	pkt_offset);
		SetField(p_packet, field_offset+GK_GAMES_WON,		CVLCPacket::MON_GAME_GAMES_WON,	pkt_offset);
		SetField(p_packet, field_offset+GK_CENTS_PLAYED,	CVLCPacket::MON_GAME_CENTS_PLAYED,	pkt_offset);
		SetField(p_packet, field_offset+GK_CENTS_WON,		CVLCPacket::MON_GAME_CENTS_WON,	pkt_offset);
		
		pkt_offset += CVLCPacket::LENGTH_MONITOR_GAME;
		field_offset += GK_NUM_FIELDS;
	}	

	monitor_data->UpdateDefault();
}

/////////////////////////////////////////////////////////////////////////////////////

CVlcData* CDlgMonitor::GetVlcData()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return NULL;
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type == CVlcMonitorData::MONITOR_CURRENT)
		return active_id->GetMonitorCurData();
	else
		return active_id->GetMonitorEndData();
}

void CDlgMonitor::GetPacketName(CString& p_packet_name)
{
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type == CVlcMonitorData::MONITOR_CURRENT)
		p_packet_name = "Current Monitor";
	else
		p_packet_name = "Day End Monitor";
}

void CDlgMonitor::AddGameId(uint p_game_id)
{
	//Adds an ID to the ids displayed in the combo box if not alreadt there
	if( m_ctrl_combo_game_list.FindString(0,Convert(p_game_id)) == CB_ERR ) 
		m_ctrl_combo_game_list.AddString( Convert(p_game_id) );
	m_ctrl_combo_game_list.SetCurSel(0);
	SetGamePlayControlEnable();
}

int CDlgMonitor::serialize(CArchive *pArchive)
{
    int nStatus = 0;
	
    // Serialize the object ...
    ASSERT (pArchive != NULL);
    try
    {
		if (pArchive->IsStoring()) 
		{
		}
		else 
		{
		}
    }
    catch (CException* pException)
    {
		nStatus = 1;
    }
    return (nStatus);
}

/////////////////////////////////////////////////////////////////////////////


void CDlgMonitor::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
	{
		UpdateData(false);
	}
	else
	{
		UpdateData(true);	
	}
}

void CDlgMonitor::OnSelchangeComboCurDe() 
{
	SetGamePlayControlEnable();
	g_dlg_main->DisplayNextToSend();
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;	
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
	m_ctrl_edit_collect_drop.SetWindowText(Convert(monitor_data->GetDropBox()));
}

void CDlgMonitor::OnButtonCollectDropBox() 
{
	CString amount; m_ctrl_edit_collect_drop.GetWindowText(amount);
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_CURRENT) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;
	monitor_data->CollectDropBox(Convert(amount));
	g_dlg_main->DisplayDefault();
}

void CDlgMonitor::OnButtonDropBoxIn() 
{
	CString amount; m_ctrl_edit_drop_box_in.GetWindowText(amount);
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_CURRENT) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;
	monitor_data->InCoinDropBox(Convert(amount));
	g_dlg_main->DisplayDefault();
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
	m_ctrl_edit_collect_drop.SetWindowText(Convert(monitor_data->GetDropBox()));
}

void CDlgMonitor::OnButtonHopperIn() 
{
	CString amount; m_ctrl_edit_hopper_in.GetWindowText(amount);
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_CURRENT) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;
	monitor_data->InCoinHopper(Convert(amount));
	g_dlg_main->DisplayDefault();
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
}

void CDlgMonitor::OnButtonHopperOut() 
{
	CString amount; m_ctrl_edit_hopper_out.GetWindowText(amount);
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_CURRENT) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;
	monitor_data->OutCoinHopper(Convert(amount));
	g_dlg_main->DisplayDefault();
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
}

void CDlgMonitor::OnButtonManualPay() 
{
	CString amount; m_ctrl_edit_manual_pay.GetWindowText(amount);
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_CURRENT) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;
	monitor_data->OutManualPay(Convert(amount));
	g_dlg_main->DisplayDefault();
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
}

void CDlgMonitor::OnButtonPlayGame() 
{
	CString bet; m_ctrl_edit_bet.GetWindowText(bet);
	CString win; m_ctrl_edit_win.GetWindowText(win);
	CString game_id; m_ctrl_combo_game_list.GetWindowText(game_id);
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_CURRENT) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;
	monitor_data->PlayGame(Convert(game_id),Convert(bet),Convert(win));
	g_dlg_main->DisplayDefault();
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
}

void CDlgMonitor::OnButtonSnapshot() 
{
	CVlcMonitorData::EMonitorType type = (CVlcMonitorData::EMonitorType)(m_ctrl_combo_cur_de.GetCurSel());
	if(type != CVlcMonitorData::MONITOR_END) //should be!
		return;
	CVlcMonitorData* monitor_data = (CVlcMonitorData*)(GetVlcData());
	if(monitor_data==NULL) 
		return;	
	monitor_data->DoSnapshot();
	g_dlg_main->DisplayDefault();
	CString credit_str = CString("Credit balance: ");
	credit_str += Convert(monitor_data->GetCurrentCredits());
	m_ctrl_static_current_credits.SetWindowText(credit_str);
	m_ctrl_edit_collect_drop.SetWindowText(Convert(monitor_data->GetDropBox()));
}

void CDlgMonitor::OnSelchangeComboGameList() 
{
	CString game_id;
	m_ctrl_combo_game_list.GetWindowText(game_id);
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return;
	CVlcMonitorData* cur_monitor = (CVlcMonitorData*)(active_id->GetMonitorCurData());
	cur_monitor->SetAciveGameId(Convert(game_id));	
}
