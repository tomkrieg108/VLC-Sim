// PDB1.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "dlg_configuration.h"
#include "vlc_id.h"
#include "vlc_id_list.h"
#include "program_options_dlg.h"
//#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfiguration dialog

CDlgConfiguration* g_dlg_configuration = 0;


CDlgConfiguration::CDlgConfiguration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfiguration::IDD, pParent),
	m_game_key_count(0),
	m_game_key_bytes(NULL),
	m_sys_config_bytes(108)
{
	//{{AFX_DATA_INIT(CDlgConfiguration)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////

void CDlgConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfiguration)
	DDX_Control(pDX, IDC_CHECK_NACK_2ND_PKT, m_ctrl_chk_nack_2nd);
	DDX_Control(pDX, IDC_CHECK_NACK_1ST_PKT, m_ctrl_chk_nack_first);
	DDX_Control(pDX, IDC_CHECK_O_BIT, m_ctrl_chk_mess_with_o_bit);
	DDX_Control(pDX, IDC_XLISTCTRL_CONFIG_PACKET, m_xlistctrl_config_packet);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgConfiguration, CDialog)
	//{{AFX_MSG_MAP(CDlgConfiguration)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////

BOOL CDlgConfiguration::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

/////////////////////////////////////////////////////////////////////////////


void CDlgConfiguration::AddOverheadSection(int p_count_game_key)
{
	m_xlistctrl_config_packet.LockWindowUpdate();	

	m_xlistctrl_config_packet.InsertItem(OH_FORMAT_ID, "1");
	m_xlistctrl_config_packet.SetItemText(OH_FORMAT_ID,1,"Format ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(OH_FORMAT_ID,2,"05",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(OH_FORMAT_ID,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(OH_RESERVED2, "2");
	m_xlistctrl_config_packet.SetItemText(OH_RESERVED2,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(OH_RESERVED2,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(OH_RESERVED2,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(OH_CONFIG_VERS, "3");
	m_xlistctrl_config_packet.SetItemText(OH_CONFIG_VERS,1,"Config Version",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(OH_CONFIG_VERS,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(OH_CONFIG_VERS,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(OH_BYTES_SYS, "4");
	m_xlistctrl_config_packet.SetItemText(OH_BYTES_SYS,1,"Bytes Sys Cfg",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(OH_BYTES_SYS,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(OH_BYTES_SYS,2, CXEdit::DEC,2);

	m_xlistctrl_config_packet.InsertItem(OH_RESERVED5, "5");
	m_xlistctrl_config_packet.SetItemText(OH_RESERVED5,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(OH_RESERVED5,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(OH_RESERVED5,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(OH_COUNT_GAME_KEY, "6");
	m_xlistctrl_config_packet.SetItemText(OH_COUNT_GAME_KEY,1,"Count - Game Keys",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(OH_COUNT_GAME_KEY,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(OH_COUNT_GAME_KEY,2, CXEdit::DEC,2);

	for(int key=0; key<p_count_game_key; key++)
	{
		int row_key = OH_BYTES_KEY_X + 2*key;
		int row_res = OH_RESERVED_X + 2*key;
		CString str_byte_key = Convert(row_key+1);					
		CString str_byte_res = Convert(row_res+1);					
		CString str_description = CString("Bytes for key ") + Convert(key+1);

		m_xlistctrl_config_packet.InsertItem(row_key, str_byte_key);
		m_xlistctrl_config_packet.SetItemText(row_key,1,str_description,RGB(0,0,0), RGB(255,255,255) ); 
		m_xlistctrl_config_packet.SetItemText(row_key,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
		m_xlistctrl_config_packet.SetEdit(row_key,2, CXEdit::DEC,2);

		m_xlistctrl_config_packet.InsertItem(row_res, str_byte_res);
		m_xlistctrl_config_packet.SetItemText(row_res,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
		m_xlistctrl_config_packet.SetItemText(row_res,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
		m_xlistctrl_config_packet.SetEdit(row_res,2, CXEdit::HEX,2);
		
	}	
	
	m_xlistctrl_config_packet.UnlockWindowUpdate();
}

/////////////////////////////////////////////////////////////////////////////


void CDlgConfiguration::AddSystemSection(void)
{
	int start = 6+m_game_key_count*2;

	m_xlistctrl_config_packet.LockWindowUpdate();	

	m_xlistctrl_config_packet.InsertItem(start+SYS_LOCATION_NAME, "1-20",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_LOCATION_NAME,1,"Location Name",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_LOCATION_NAME,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_LOCATION_NAME,2, CXEdit::TEXT,20);

	m_xlistctrl_config_packet.InsertItem(start+SYS_LOACTION_ADDRESS, "21-60",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_LOACTION_ADDRESS,1,"Location Addr",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_LOACTION_ADDRESS,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_LOACTION_ADDRESS,2, CXEdit::TEXT,40);

	m_xlistctrl_config_packet.InsertItem(start+SYS_PERMIT_NUMBER, "61-68",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_PERMIT_NUMBER,1,"Permit number",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_PERMIT_NUMBER,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_PERMIT_NUMBER,2, CXEdit::TEXT,8);
	
	m_xlistctrl_config_packet.InsertItem(start+SYS_RESERVED69, "69-74",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_RESERVED69,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_RESERVED69,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_RESERVED69,2, CXEdit::HEX,12);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TERM_ENABLE_TIME_NH, "75-76",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_TIME_NH,1,"Non-Hol term enab time",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_TIME_NH,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TERM_ENABLE_TIME_NH,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TERM_ENABLE_DUR_NH, "77-78",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_DUR_NH,1,"Non-Hol term enab dur",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_DUR_NH,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TERM_ENABLE_DUR_NH,2, CXEdit::DEC,5);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TERM_ENABLE_TIME_H, "79-80",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_TIME_H,1,"Non-Hol term enab time",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_TIME_H,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TERM_ENABLE_TIME_H,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TERM_ENABLE_DUR_H, "81-82",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_DUR_H,1,"Non-Hol term enab dur",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TERM_ENABLE_DUR_H,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TERM_ENABLE_DUR_H,2, CXEdit::DEC,5);

	m_xlistctrl_config_packet.InsertItem(start+SYS_HOPPER_PAYOUT_MAX, "83-84",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_HOPPER_PAYOUT_MAX,1,"Hopper payout max",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_HOPPER_PAYOUT_MAX,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_HOPPER_PAYOUT_MAX,2, CXEdit::DEC,4);

	m_xlistctrl_config_packet.InsertItem(start+SYS_SUBSTANTIAL_CASHOUT, "85-86",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_SUBSTANTIAL_CASHOUT,1,"Substantial cashout val",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_SUBSTANTIAL_CASHOUT,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_SUBSTANTIAL_CASHOUT,2, CXEdit::DEC,4);

	m_xlistctrl_config_packet.InsertItem(start+SYS_SUBSTANTIAL_WIN, "87-88",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_SUBSTANTIAL_WIN,1,"Substantial win amount",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_SUBSTANTIAL_WIN,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_SUBSTANTIAL_WIN,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_DAY_END_SNAPSHOT_TIME, "89-90",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_DAY_END_SNAPSHOT_TIME,1,"Day End snapshot time",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_DAY_END_SNAPSHOT_TIME,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_DAY_END_SNAPSHOT_TIME,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_WEEK_END_SNAPSHOT_DAY, "91",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_WEEK_END_SNAPSHOT_DAY,1,"Week End snapshot day",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_WEEK_END_SNAPSHOT_DAY,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_WEEK_END_SNAPSHOT_DAY,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TIMEZONE_CODE, "92",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TIMEZONE_CODE,1,"Time zone code",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TIMEZONE_CODE,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TIMEZONE_CODE,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(start+SYS_DAYE_DAYLIGHT_SAVINGS, "93-94",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_DAYE_DAYLIGHT_SAVINGS,1,"Date DLS change",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_DAYE_DAYLIGHT_SAVINGS,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_DAYE_DAYLIGHT_SAVINGS,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TIME_DAYLIGHT_SAVINGS, "95-96",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TIME_DAYLIGHT_SAVINGS,1,"Time DLS change",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TIME_DAYLIGHT_SAVINGS,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TIME_DAYLIGHT_SAVINGS,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_SHARE_PERCENTAGE, "97-98",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_SHARE_PERCENTAGE,1,"Share percentage",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_SHARE_PERCENTAGE,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_SHARE_PERCENTAGE,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+SYS_TRANSACTION_ENABLE_FLAGS, "99",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_TRANSACTION_ENABLE_FLAGS,1,"Transaction flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_TRANSACTION_ENABLE_FLAGS,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_TRANSACTION_ENABLE_FLAGS,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(start+SYS_BEGINNING_EVENTS_LOG, "100-102",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_BEGINNING_EVENTS_LOG,1,"Events start record",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_BEGINNING_EVENTS_LOG,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_BEGINNING_EVENTS_LOG,2, CXEdit::DEC,8);

	m_xlistctrl_config_packet.InsertItem(start+SYS_RESERVED103, "103",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_RESERVED103,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_RESERVED103,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_RESERVED103,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(start+SYS_STATUS_POLL_TIMEOUT, "104",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_STATUS_POLL_TIMEOUT,1,"Status poll timeout",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_STATUS_POLL_TIMEOUT,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_STATUS_POLL_TIMEOUT,2, CXEdit::DEC,3);

	m_xlistctrl_config_packet.InsertItem(start+SYS_EXCEPTION_ENABLE, "105-108",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+SYS_EXCEPTION_ENABLE,1,"Exceptions Enable Flgs",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+SYS_EXCEPTION_ENABLE,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+SYS_EXCEPTION_ENABLE,2, CXEdit::HEX,8);

	m_xlistctrl_config_packet.UnlockWindowUpdate();	
}

/////////////////////////////////////////////////////////////////////////////


void CDlgConfiguration::AddGameKeySection(int p_num_system_fields, int p_num_game_keys, int p_game_key, int p_bytes_extra_parameters, COLORREF p_colour)
{
	int start = 6+p_num_game_keys*2;
	start += p_num_system_fields;
	start += p_game_key*GK_NUM_FIELDS;

	m_xlistctrl_config_packet.LockWindowUpdate();
	
	m_xlistctrl_config_packet.InsertItem(start+GK_RESERVED1, "1-4",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_RESERVED1,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_RESERVED1,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_RESERVED1,2, CXEdit::HEX,8);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_TYPE, "5",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_TYPE,1,"Game type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_TYPE,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_TYPE,2, CXEdit::DEC,3);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_NAME, "6-20",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_NAME,1,"Game name",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_NAME,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_NAME,2, CXEdit::TEXT,15);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_DISABLE_TIME_NH, "21-22",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_TIME_NH,1,"N-hol game disable time",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_TIME_NH,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_DISABLE_TIME_NH,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_DISABLE_DURATION_NH, "23-24",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_DURATION_NH,1,"N-hol game disable duration",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_DURATION_NH,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_DISABLE_DURATION_NH,2, CXEdit::DEC,5);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_DISABLE_TIME_H, "25-26",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_TIME_H,1,"Hol game disable time",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_TIME_H,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_DISABLE_TIME_H,2, CXEdit::DEC,6);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_DISABLE_DURATION_H, "27-28",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_DURATION_H,1,"Hol game disable duration",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_DISABLE_DURATION_H,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_DISABLE_DURATION_H,2, CXEdit::DEC,5);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_ID, "29",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_ID,1,"Game ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_ID,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_ID,2, CXEdit::DEC,3);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_ENABLE_CONTROL_FLAGS, "30",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_ENABLE_CONTROL_FLAGS,1,"Game Enable Ctrl Flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_ENABLE_CONTROL_FLAGS,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_ENABLE_CONTROL_FLAGS,2, CXEdit::HEX,2);

	m_xlistctrl_config_packet.InsertItem(start+GK_GAME_CR_SIZE_CENTS, "31-32",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_CR_SIZE_CENTS,1,"Credit size cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_GAME_CR_SIZE_CENTS,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_GAME_CR_SIZE_CENTS,2, CXEdit::DEC,4);

	m_xlistctrl_config_packet.InsertItem(start+GK_MAX_BET_PER_GAME_CENTS, "33-34",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_MAX_BET_PER_GAME_CENTS,1,"Max bet cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_MAX_BET_PER_GAME_CENTS,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_MAX_BET_PER_GAME_CENTS,2, CXEdit::DEC,7);

	m_xlistctrl_config_packet.InsertItem(start+GK_MAX_AWARD_PER_GAME_CENTS, "35-38",p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_MAX_AWARD_PER_GAME_CENTS,1,"Max awards cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_MAX_AWARD_PER_GAME_CENTS,2,"",RGB(0,0,255), RGB(255,255,255)) ; 
	m_xlistctrl_config_packet.SetEdit(start+GK_MAX_AWARD_PER_GAME_CENTS,2, CXEdit::DEC,6);

	//game dependant parameters
	CString byte_range = CString("39-");
	CString description = CString("Game dependant data");
	if(p_bytes_extra_parameters>0)
		byte_range += Convert(p_bytes_extra_parameters+38);
	else
		description += " (none)";
		
	m_xlistctrl_config_packet.InsertItem(start+GK_EXTRA_BYTES, byte_range ,p_colour,RGB(255,255,255));
	m_xlistctrl_config_packet.SetItemText(start+GK_EXTRA_BYTES,1,description ,RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_config_packet.SetItemText(start+GK_EXTRA_BYTES,2,"",RGB(0,0,255), RGB(255,255,255)) ; 

	if(p_bytes_extra_parameters>0)
		m_xlistctrl_config_packet.SetEdit(start+GK_EXTRA_BYTES,2, CXEdit::HEX,p_bytes_extra_parameters*2);

	m_xlistctrl_config_packet.UnlockWindowUpdate();	
}

/////////////////////////////////////////////////////////////////////////////


BOOL CDlgConfiguration::OnInitDialog()
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
// set up dlg for config packet data 
//////////////////////////////////////////////////////////////////
// XListCtrl must have LVS_EX_FULLROWSELECT if combo or edit boxes are used
	

	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_config_packet.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	CRect rect;
	m_xlistctrl_config_packet.GetWindowRect(&rect);
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
	m_xlistctrl_config_packet.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_config_packet.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_config_packet.InsertColumn(2, &lvcolumn);

	m_xlistctrl_config_packet.LockWindowUpdate();	
	m_xlistctrl_config_packet.DeleteAllItems();
	m_xlistctrl_config_packet.LockWindowUpdate();	

	//m_game_key_count=4;
	AddOverheadSection(0);
	AddSystemSection();

	/*
	for(int i=0; i<m_game_key_count; i++)
	{
		int col = 100*(i%2);
		AddGameKeySection(i, 10, RGB(30,30+col,30+col) );
	}
	*/

	return TRUE;	// CG: This was added by the ToolTips component.
}




//////////////////////////////////////////////////////////////////
//Implement IDlgPacket

inline void CDlgConfiguration::DisplayField(const CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset)
{
	//p_offset is the offset to the start of the section.  e.g. system section offset is 8 if there is 1 game key etc
	CString value;
	p_packet.GetDataFieldFormatted(value, p_field_id, p_offset );
	m_xlistctrl_config_packet.SetItemText(p_field_pos,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
}
inline void CDlgConfiguration::SetField(CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset)
{
	CString value;
	value = m_xlistctrl_config_packet.GetItemText(p_field_pos,2);
	p_packet.SetDataFieldFormatted(value, p_field_id, p_offset);
}


void CDlgConfiguration::DisplayPacket(const CVLCPacket& p_packet )
{
	//Note in ProsimE2, if you send a config packet with system data as "none" it incorrectly includes the 
	//location name (i.e. bytes 1-20 of the system section) with the packet.
	//thats why it doesn't get displayed properly in this program'

	//check that its the right packet
	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_CONFIGURATION)
		return;

	int game_key_count = p_packet.GetDataFieldInt(CVLCPacket::CFG_OH_COUNT_GAME);
 	int num_bytes_sys_config = p_packet.GetDataFieldInt(CVLCPacket::CFG_OH_BYTES_SYSTEM);

	m_game_key_count = game_key_count;
	m_sys_config_bytes = num_bytes_sys_config;
	if(m_game_key_bytes != NULL)
	{
		delete m_game_key_bytes;
		m_game_key_bytes = NULL;
	}

	if(m_game_key_count > 0)
		m_game_key_bytes = new int[m_game_key_count];

	m_xlistctrl_config_packet.LockWindowUpdate();	
	m_xlistctrl_config_packet.DeleteAllItems();

	//Overhead section
	AddOverheadSection(game_key_count);

	DisplayField(p_packet, OH_FORMAT_ID,		CVLCPacket::DT_FORMAT_ID);
	DisplayField(p_packet, OH_RESERVED2,		CVLCPacket::CFG_OH_RESERVED2);
	DisplayField(p_packet, OH_CONFIG_VERS,		CVLCPacket::CFG_OH_VERS);
	DisplayField(p_packet, OH_BYTES_SYS,		CVLCPacket::CFG_OH_BYTES_SYSTEM);
	DisplayField(p_packet, OH_RESERVED5,		CVLCPacket::CFG_OH_RESERVED5);
	DisplayField(p_packet, OH_COUNT_GAME_KEY,	CVLCPacket::CFG_OH_COUNT_GAME);
	
	for(int key=0; key<game_key_count; key++)
	{
		int offset = 2*key;

		int pos_key = OH_BYTES_KEY_X + offset;
		int pos_res = OH_RESERVED_X + offset;

		DisplayField(p_packet, pos_key,		CVLCPacket::CFG_OH_BYTES_GAME_KEY_X, offset); 
		DisplayField(p_packet, pos_res,		CVLCPacket::CFG_OH_RESERVEDX,		 offset);
	}

	//System section
	if(num_bytes_sys_config == CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
	{
		AddSystemSection();

		int start = CVLCPacket::LENGTH_CONFIGURATION_OH + game_key_count*2;
		int offset = start;

		DisplayField(p_packet, start+SYS_LOCATION_NAME,				CVLCPacket::CFG_SYS_LOCATION_NAME, offset);
		DisplayField(p_packet, start+SYS_LOACTION_ADDRESS,			CVLCPacket::CFG_SYS_LOCATION_ADDRESS, offset);
		DisplayField(p_packet, start+SYS_PERMIT_NUMBER,				CVLCPacket::CFG_SYS_PERMIT_NUMBER, offset);
		DisplayField(p_packet, start+SYS_RESERVED69,				CVLCPacket::CFG_SYS_RESERVED69, offset);
		DisplayField(p_packet, start+SYS_TERM_ENABLE_TIME_NH,		CVLCPacket::CFG_SYS_NON_HOL_TERM_ENABLE_TIME, offset);
		DisplayField(p_packet, start+SYS_TERM_ENABLE_DUR_NH,		CVLCPacket::CFG_SYS_NON_HOL_TERM_ENABLE_DURATION, offset);
		DisplayField(p_packet, start+SYS_TERM_ENABLE_TIME_H,		CVLCPacket::CFG_SYS_HOL_TERM_ENABLE_TIME, offset);
		DisplayField(p_packet, start+SYS_TERM_ENABLE_DUR_H,			CVLCPacket::CFG_SYS_HOL_TERM_ENABLE_DURATION, offset);
		DisplayField(p_packet, start+SYS_HOPPER_PAYOUT_MAX,			CVLCPacket::CFG_SYS_HOPPER_PAY_MAX, offset);
		DisplayField(p_packet, start+SYS_SUBSTANTIAL_CASHOUT,		CVLCPacket::CFG_SYS_SUBSTANTIAL_CASHOUT, offset);
		DisplayField(p_packet, start+SYS_SUBSTANTIAL_WIN,			CVLCPacket::CFG_SYS_SUBSTANTIAL_WIN, offset);
		DisplayField(p_packet, start+SYS_DAY_END_SNAPSHOT_TIME,		CVLCPacket::CFG_SYS_DAY_END_SNAPSHOT_TIME, offset);
		DisplayField(p_packet, start+SYS_WEEK_END_SNAPSHOT_DAY,		CVLCPacket::CFG_SYS_WEEK_END_SNAPSHOT_DAY, offset);
		DisplayField(p_packet, start+SYS_TIMEZONE_CODE,				CVLCPacket::CFG_SYS_TIME_ZONE_CODE, offset);
		DisplayField(p_packet, start+SYS_DAYE_DAYLIGHT_SAVINGS,		CVLCPacket::CFG_SYS_DATE_FOR_DLS_CHANGE, offset);
		DisplayField(p_packet, start+SYS_TIME_DAYLIGHT_SAVINGS,		CVLCPacket::CFG_SYS_TIME_FOR_DLS_CHANGE, offset);
		DisplayField(p_packet, start+SYS_SHARE_PERCENTAGE,			CVLCPacket::CFG_SYS_SHARE_PERCENTAGE, offset);
		DisplayField(p_packet, start+SYS_TRANSACTION_ENABLE_FLAGS,	CVLCPacket::CFG_SYS_TRANSACTION_ENABLE_FLAGS, offset);
		DisplayField(p_packet, start+SYS_BEGINNING_EVENTS_LOG,		CVLCPacket::CFG_SYS_BASE, offset);
		DisplayField(p_packet, start+SYS_RESERVED103,				CVLCPacket::CFG_SYS_RESERVED103,offset);
		DisplayField(p_packet, start+SYS_STATUS_POLL_TIMEOUT,		CVLCPacket::CFG_SYS_STATUS_POLL_TIMEOUT, offset);
		DisplayField(p_packet, start+SYS_EXCEPTION_ENABLE,			CVLCPacket::CFG_SYS_EXCEPTIONS_ENABLE, offset);
	}

	//game key section
	int start = CVLCPacket::LENGTH_CONFIGURATION_OH + game_key_count*2;
	int offset = start;
	if(num_bytes_sys_config==CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
	{
		start += SYS_NUM_FIELDS;
		offset += num_bytes_sys_config;
	}

	for(key=0; key<game_key_count; key++)
	{
		int sys_fields =0;
		if(num_bytes_sys_config==CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
			sys_fields = SYS_NUM_FIELDS;
		int bytes_game_key = p_packet.GetDataFieldInt(CVLCPacket::CFG_OH_BYTES_GAME_KEY_X, key*2);
		int num_bytes_extra = bytes_game_key - CVLCPacket::LENGTH_CONFIGURATION_GAME;
		AddGameKeySection(sys_fields, game_key_count, key, num_bytes_extra, RGB(0,0,0) );

		DisplayField(p_packet, start+GK_RESERVED1,					CVLCPacket::CFG_GAME_RESERVED1, offset);
		DisplayField(p_packet, start+GK_GAME_TYPE,					CVLCPacket::CFG_GAME_TYPE, offset);
		DisplayField(p_packet, start+GK_GAME_NAME,					CVLCPacket::CFG_GAME_NAME, offset);
		DisplayField(p_packet, start+GK_GAME_DISABLE_TIME_NH,		CVLCPacket::CFG_GAME_NON_HOL_GAME_DISABLE_TIME, offset);
		DisplayField(p_packet, start+GK_GAME_DISABLE_DURATION_NH,	CVLCPacket::CFG_GAME_NON_HOL_GAME_DISABLE_DURATION, offset);
		DisplayField(p_packet, start+GK_GAME_DISABLE_TIME_H,		CVLCPacket::CFG_GAME_HOL_GAME_DISABLE_TIME, offset);
		DisplayField(p_packet, start+GK_GAME_DISABLE_DURATION_H,	CVLCPacket::CFG_GAME_HOL_GAME_DISABLE_DURATION, offset);
		DisplayField(p_packet, start+GK_GAME_ID,					CVLCPacket::CFG_GAME_ID, offset);
		DisplayField(p_packet, start+GK_GAME_ENABLE_CONTROL_FLAGS,	CVLCPacket::CFG_GAME_ENABLE_CONTROL_FLAGS, offset);
		DisplayField(p_packet, start+GK_GAME_CR_SIZE_CENTS,			CVLCPacket::CFG_GAME_CREDIT_SIZE, offset);
		DisplayField(p_packet, start+GK_MAX_BET_PER_GAME_CENTS,		CVLCPacket::CFG_GAME_MAX_BET, offset);
		DisplayField(p_packet, start+GK_MAX_AWARD_PER_GAME_CENTS,	CVLCPacket::CFG_GAME_MAX_AWARD, offset);

		//game-dependant data
		if(num_bytes_extra>0)
		{
			CString value;
			p_packet.GetHexString(value, CVLCPacket::START_DATA + offset + CVLCPacket::LENGTH_CONFIGURATION_GAME, num_bytes_extra);
			m_xlistctrl_config_packet.SetItemText(start+GK_EXTRA_BYTES ,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
		}
		
		offset += bytes_game_key;
		start += GK_NUM_FIELDS;

		m_game_key_bytes[key] = bytes_game_key;
	}

	m_xlistctrl_config_packet.UnlockWindowUpdate();
}





////////////////////////////////////////////////////////////////////////////////////
/*
Set the packet in the dialog m_xlistctrl_datetime
*/

void CDlgConfiguration::SetPacket(CVLCPacket& p_packet )
{
	//check that its the right packet
	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_CONFIGURATION)
		return;

	//set length of packet
	int required_data_size = CVLCPacket::LENGTH_CONFIGURATION_OH + 2*m_game_key_count + m_sys_config_bytes;
	for(int key=0; key<m_game_key_count; key++)
	{
		if(m_game_key_bytes==NULL)
			return; //somethings gone wrong if this happens
		required_data_size += m_game_key_bytes[key];
	}
	
	p_packet.SetDataLength(required_data_size); 

	//Overhead section
	SetField(p_packet, OH_FORMAT_ID,		CVLCPacket::DT_FORMAT_ID);
	SetField(p_packet, OH_RESERVED2,		CVLCPacket::CFG_OH_RESERVED2);
	SetField(p_packet, OH_CONFIG_VERS,		CVLCPacket::CFG_OH_VERS);
	SetField(p_packet, OH_BYTES_SYS,		CVLCPacket::CFG_OH_BYTES_SYSTEM);
	SetField(p_packet, OH_RESERVED5,		CVLCPacket::CFG_OH_RESERVED5);
	SetField(p_packet, OH_COUNT_GAME_KEY,	CVLCPacket::CFG_OH_COUNT_GAME);	

	for(key=0; key<m_game_key_count; key++)
	{
		int offset = 2*key;

		int pos_key = OH_BYTES_KEY_X + offset;
		int pos_res = OH_RESERVED_X + offset;

		SetField(p_packet, pos_key,		CVLCPacket::CFG_OH_BYTES_GAME_KEY_X, offset); 
		SetField(p_packet, pos_res,		CVLCPacket::CFG_OH_RESERVEDX,		 offset);
	}

	
	//System section
	if(m_sys_config_bytes == CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
	{
		int start = CVLCPacket::LENGTH_CONFIGURATION_OH + m_game_key_count*2;
		int offset = start;

		SetField(p_packet, start+SYS_LOCATION_NAME,				CVLCPacket::CFG_SYS_LOCATION_NAME, offset);
		SetField(p_packet, start+SYS_LOACTION_ADDRESS,			CVLCPacket::CFG_SYS_LOCATION_ADDRESS, offset);
		SetField(p_packet, start+SYS_PERMIT_NUMBER,				CVLCPacket::CFG_SYS_PERMIT_NUMBER, offset);
		SetField(p_packet, start+SYS_RESERVED69,				CVLCPacket::CFG_SYS_RESERVED69, offset);
		SetField(p_packet, start+SYS_TERM_ENABLE_TIME_NH,		CVLCPacket::CFG_SYS_NON_HOL_TERM_ENABLE_TIME, offset);
		SetField(p_packet, start+SYS_TERM_ENABLE_DUR_NH,		CVLCPacket::CFG_SYS_NON_HOL_TERM_ENABLE_DURATION, offset);
		SetField(p_packet, start+SYS_TERM_ENABLE_TIME_H,		CVLCPacket::CFG_SYS_HOL_TERM_ENABLE_TIME, offset);
		SetField(p_packet, start+SYS_TERM_ENABLE_DUR_H,			CVLCPacket::CFG_SYS_HOL_TERM_ENABLE_DURATION, offset);
		SetField(p_packet, start+SYS_HOPPER_PAYOUT_MAX,			CVLCPacket::CFG_SYS_HOPPER_PAY_MAX, offset);
		SetField(p_packet, start+SYS_SUBSTANTIAL_CASHOUT,		CVLCPacket::CFG_SYS_SUBSTANTIAL_CASHOUT, offset);
		SetField(p_packet, start+SYS_SUBSTANTIAL_WIN,			CVLCPacket::CFG_SYS_SUBSTANTIAL_WIN, offset);
		SetField(p_packet, start+SYS_DAY_END_SNAPSHOT_TIME,		CVLCPacket::CFG_SYS_DAY_END_SNAPSHOT_TIME, offset);
		SetField(p_packet, start+SYS_WEEK_END_SNAPSHOT_DAY,		CVLCPacket::CFG_SYS_WEEK_END_SNAPSHOT_DAY, offset);
		SetField(p_packet, start+SYS_TIMEZONE_CODE,				CVLCPacket::CFG_SYS_TIME_ZONE_CODE, offset);
		SetField(p_packet, start+SYS_DAYE_DAYLIGHT_SAVINGS,		CVLCPacket::CFG_SYS_DATE_FOR_DLS_CHANGE, offset);
		SetField(p_packet, start+SYS_TIME_DAYLIGHT_SAVINGS,		CVLCPacket::CFG_SYS_TIME_FOR_DLS_CHANGE, offset);
		SetField(p_packet, start+SYS_SHARE_PERCENTAGE,			CVLCPacket::CFG_SYS_SHARE_PERCENTAGE, offset);
		SetField(p_packet, start+SYS_TRANSACTION_ENABLE_FLAGS,	CVLCPacket::CFG_SYS_TRANSACTION_ENABLE_FLAGS, offset);
		SetField(p_packet, start+SYS_BEGINNING_EVENTS_LOG,		CVLCPacket::CFG_SYS_BASE, offset);
		SetField(p_packet, start+SYS_RESERVED103,				CVLCPacket::CFG_SYS_RESERVED103);
		SetField(p_packet, start+SYS_STATUS_POLL_TIMEOUT,		CVLCPacket::CFG_SYS_STATUS_POLL_TIMEOUT, offset);
		SetField(p_packet, start+SYS_EXCEPTION_ENABLE,			CVLCPacket::CFG_SYS_EXCEPTIONS_ENABLE, offset);
	}
	
	//game key section
	int start = CVLCPacket::LENGTH_CONFIGURATION_OH + m_game_key_count*2;
	int offset = start;
	if(m_sys_config_bytes==CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
	{
		start += SYS_NUM_FIELDS;
		offset += m_sys_config_bytes;
	}

	for(key=0; key<m_game_key_count; key++)
	{	
		int bytes_game_key = m_game_key_bytes[key];
		int num_bytes_extra = bytes_game_key - CVLCPacket::LENGTH_CONFIGURATION_GAME;
		
		SetField(p_packet, start+GK_RESERVED1,					CVLCPacket::CFG_GAME_RESERVED1, offset);
		SetField(p_packet, start+GK_GAME_TYPE,					CVLCPacket::CFG_GAME_TYPE, offset);
		SetField(p_packet, start+GK_GAME_NAME,					CVLCPacket::CFG_GAME_NAME, offset);
		SetField(p_packet, start+GK_GAME_DISABLE_TIME_NH,		CVLCPacket::CFG_GAME_NON_HOL_GAME_DISABLE_TIME, offset);
		SetField(p_packet, start+GK_GAME_DISABLE_DURATION_NH,	CVLCPacket::CFG_GAME_NON_HOL_GAME_DISABLE_DURATION, offset);
		SetField(p_packet, start+GK_GAME_DISABLE_TIME_H,		CVLCPacket::CFG_GAME_HOL_GAME_DISABLE_TIME, offset);
		SetField(p_packet, start+GK_GAME_DISABLE_DURATION_H,	CVLCPacket::CFG_GAME_HOL_GAME_DISABLE_DURATION, offset);
		SetField(p_packet, start+GK_GAME_ID,					CVLCPacket::CFG_GAME_ID, offset);
		SetField(p_packet, start+GK_GAME_ENABLE_CONTROL_FLAGS,	CVLCPacket::CFG_GAME_ENABLE_CONTROL_FLAGS, offset);
		SetField(p_packet, start+GK_GAME_CR_SIZE_CENTS,			CVLCPacket::CFG_GAME_CREDIT_SIZE, offset);
		SetField(p_packet, start+GK_MAX_BET_PER_GAME_CENTS,		CVLCPacket::CFG_GAME_MAX_BET, offset);
		SetField(p_packet, start+GK_MAX_AWARD_PER_GAME_CENTS,	CVLCPacket::CFG_GAME_MAX_AWARD, offset);
		

		//game-dependant data
		if(num_bytes_extra>0)
		{
			CString value;
			value = m_xlistctrl_config_packet.GetItemText(start+GK_EXTRA_BYTES,2);
			p_packet.SetHexString(value, CVLCPacket::START_DATA + offset + CVLCPacket::LENGTH_CONFIGURATION_GAME);
		}

		offset += bytes_game_key;	
		start += GK_NUM_FIELDS;
	}

}


CVlcData* CDlgConfiguration::GetVlcData()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return NULL;

	return active_id->GetConfigurationData();
	
}

void CDlgConfiguration::GetPacketName(CString& p_packet_name)
{
	p_packet_name = CString("Configuration");
}


/////////////////////////////////////////////////////////////////////////////


int CDlgConfiguration::serialize(CArchive* pArchive)
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



void CDlgConfiguration::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
	{
		if( g_program_opts_dlg )
		{
		}
		// display the variables in the controls.
		UpdateData(false);
	}
	else
	{
		UpdateData(true);
	}	
}