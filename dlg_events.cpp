

#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "dlg_events.h"
#include "vlc_id.h"
#include "vlc_id_list.h"
#include "program_options_dlg.h"
#include "math.h"
#include "date_time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEvents dialog

CDlgEvents* g_dlg_events = 0;

CDlgEvents::CDlgEvents(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEvents::IDD, pParent),
	m_read_record_start(0),
	m_read_record_end(0)
{
	//{{AFX_DATA_INIT(CDlgEvents)
	//}}AFX_DATA_INIT
	g_notify_mgr->Register( CNotify::ACTIVE_ID_CHANGED, this );
}


void CDlgEvents::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEvents)
	DDX_Control(pDX, IDC_BTN_SET_DEFAULT, m_ctrl_btn_set_default);
	DDX_Control(pDX, IDC_BTN_ADD, m_ctrl_btn_add_event);
	DDX_Control(pDX, IDC_COMBO_APPLY_ERRORS_TO, m_ctrl_combo_apply_errors_to);
	DDX_Control(pDX, IDC_STATIC_PACKETS, m_ctrl_static_packet_count);
	DDX_Control(pDX, IDC_STATIC_RECORDS, m_ctrl_static_record_count);
	DDX_Control(pDX, IDC_STATIC_START_READ, m_ctrl_static_start_read);
	DDX_Control(pDX, IDC_STATIC_END_READ, m_ctrl_static_end_read);
	DDX_Control(pDX, IDC_CHK_USE_CURRENT_TIME, m_ctrl_chk_use_current_time);
	DDX_Control(pDX, IDC_LISTCTRL_EVENT_LOG, m_xlistctrl_event_log);
	DDX_Control(pDX, IDC_LISTCTRL_EVENT_DATA, m_xlistctrl_event_data);
	DDX_Control(pDX, IDC_COMBO_EVENT_TYPES, m_ctrl_combo_event_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEvents, CDialog)
	//{{AFX_MSG_MAP(CDlgEvents)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_EVENT_TYPES, OnSelchangeComboEventTypes)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_EVENT_LOG, OnDblclkListctrlEventLog)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCTRL_EVENT_LOG, OnRclickListctrlEventLog)
	ON_NOTIFY(NM_RDBLCLK, IDC_LISTCTRL_EVENT_LOG, OnRdblclkListctrlEventLog)
	ON_BN_CLICKED(IDC_BTN_SET_DEFAULT, OnBtnSetDefault)
	ON_CBN_SELCHANGE(IDC_COMBO_APPLY_ERRORS_TO, OnSelchangeComboApplyErrorsTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEvents message handlers

BOOL CDlgEvents::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
/////////////////////////////////////////////////////////////////////////////
BOOL CDlgEvents::OnInitDialog()
{
	CDialog::OnInitDialog();	// CG: This was added by the ToolTips component.
	// CG: The following block was added by the ToolTips component.

	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);
	}

	SetupColumns();

	//These need to be in the same order as defined in CVlcEventLogData
	m_ctrl_combo_event_type.AddString("Cashout");
	m_ctrl_combo_event_type.AddString("Cashout Ctn");
	m_ctrl_combo_event_type.AddString("Game Win Data");
	m_ctrl_combo_event_type.AddString("Game Win Data Ctn");
	m_ctrl_combo_event_type.AddString("Notable: Exception");
	m_ctrl_combo_event_type.AddString("Notable: Cr at snapshot");
	m_ctrl_combo_event_type.AddString("Notable: Msg to Host");
	m_ctrl_combo_event_type.AddString("Notable: Status poll timeout");
	m_ctrl_combo_event_type.AddString("Notable: FW Rev Change");
	m_ctrl_combo_event_type.AddString("Notable: Encryption Key change");
	m_ctrl_combo_event_type.AddString("Time and Date");
	m_ctrl_combo_event_type.AddString("record No. Date Marker");
	m_ctrl_combo_event_type.SetCurSel(CVlcEventLogData::EVENT_RECORD_NO);
	OnSelchangeComboEventTypes();
	ShowStartEndRecords(NULL);
	m_ctrl_chk_use_current_time.SetCheck(1);
	m_ctrl_combo_apply_errors_to.AddString("1");
	m_ctrl_combo_apply_errors_to.SetCurSel(0);
	
	if(g_vlc_id_list->GetActiveId()==NULL) //should be
		EnableControls(false);
	else
		EnableControls(true);

	return TRUE;	// CG: This was added by the ToolTips component.
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::EnableControls(bool p_enable)
{
	m_xlistctrl_event_data.EnableWindow(p_enable);
	m_xlistctrl_event_log.EnableWindow(p_enable);
	m_ctrl_chk_use_current_time.EnableWindow(p_enable);
	m_ctrl_btn_set_default.EnableWindow(p_enable);
	m_ctrl_btn_add_event.EnableWindow(p_enable);
	m_ctrl_combo_apply_errors_to.EnableWindow(p_enable);	
}

void CDlgEvents::SetupColumns()
{
	// add columns for data list
	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_event_data.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	CRect rect;
	m_xlistctrl_event_data.GetWindowRect(&rect);
	int width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar


	
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "Bytes";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/6;
	m_xlistctrl_event_data.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_event_data.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_event_data.InsertColumn(2, &lvcolumn);

	m_xlistctrl_event_data.LockWindowUpdate();	
	m_xlistctrl_event_data.DeleteAllItems();
	m_xlistctrl_event_data.LockWindowUpdate();	


	// add columns for event list
	m_xlistctrl_event_log.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	m_xlistctrl_event_log.GetWindowRect(&rect);
	width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar


	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "Record No.";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_event_log.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*4/6;
	m_xlistctrl_event_log.InsertColumn(1, &lvcolumn);

	//lvcolumn.pszText = "Value";
	//lvcolumn.iSubItem = 2;
	//lvcolumn.cx = width*2/6;
	//m_xlistctrl_event_log.InsertColumn(2, &lvcolumn);

	m_xlistctrl_event_log.LockWindowUpdate();	
	m_xlistctrl_event_log.DeleteAllItems();
	m_xlistctrl_event_log.UnlockWindowUpdate();	
}
/////////////////////////////////////////////////////////////////////////////

void CDlgEvents::SetupRows(CVlcEventLogData::EEventType p_event_type)
{
	m_xlistctrl_event_data.LockWindowUpdate();
	
	m_xlistctrl_event_data.DeleteAllItems();

	switch(p_event_type)
	{
		case CVlcEventLogData::EVENT_CASHOUT:				SetupRowsCashout();				break;
		case CVlcEventLogData::EVENT_CASHOUT_CTN:			SetupRowsCashoutCtn();			break;
		case CVlcEventLogData::EVENT_GAMEWIN:				SetupRowsGameWin();				break;
		case CVlcEventLogData::EVENT_GAMEWIN_CTN:			SetupRowsGameWinCtn();			break;
		case CVlcEventLogData::EVENT_EXCEPTION:				SetupRowsException();			break;
		case CVlcEventLogData::EVENT_CREDIT_SNAPSHOT:		SetupRowsCreditSnapshot();		break;
		case CVlcEventLogData::EVENT_MSG_TO_HOST:			SetupRowsMsgToHost();			break;
		case CVlcEventLogData::EVENT_STATUS_POLL_TIMEOUT:	SetupRowsStatusPollTimeout();	break;
		case CVlcEventLogData::EVENT_FW_REV_CHANGE:			SetupRowsFwRevChange();			break;
		case CVlcEventLogData::EVENT_ENCRYPTION_KEY_CHANGE: SetupRowsEncryptionKeyChange();	break;
		case CVlcEventLogData::EVENT_TIME_DATE:				SetupRowsTimeDate();			break;	
		case CVlcEventLogData::EVENT_RECORD_NO:				SetupRowsRecordNo();			break;	

		default:
			break;
	};


	m_xlistctrl_event_log.UnlockWindowUpdate();
}
/////////////////////////////////////////////////////////////////////////////

void CDlgEvents::SetupRowsCashout()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Cashout Code",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (2, "3-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Amount cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::DEC,8);

	m_xlistctrl_event_data.InsertItem (3, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////

void CDlgEvents::SetupRowsCashoutCtn()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FB",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2-4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Venue ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::DEC,7);

	m_xlistctrl_event_data.InsertItem (2, "5-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Cashout No.",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::DEC,5);

	m_xlistctrl_event_data.InsertItem (3, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Security No",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,5);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsGameWin()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Game Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"01",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2-4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Win Amount cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::DEC,7);

	m_xlistctrl_event_data.InsertItem (2, "5",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Game ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::DEC,3);

	m_xlistctrl_event_data.InsertItem (3, "6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Win Category",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,3);
		
	m_xlistctrl_event_data.InsertItem (4, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(4,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(4,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (4,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsGameWinCtn()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FB",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2-4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,6);

	m_xlistctrl_event_data.InsertItem (2, "5-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Win Amount cents",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::DEC,9);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsException()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FD",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Exception Code",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Event Data",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::HEX,8);	

	m_xlistctrl_event_data.InsertItem (3, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,6);

}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsCreditSnapshot()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FD",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Sub-Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"80",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Credit Balance",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::HEX,9);	

	m_xlistctrl_event_data.InsertItem (3, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsMsgToHost()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FD",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Sub-Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"C0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Text",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::TEXT,6);	
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsStatusPollTimeout()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FD",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Sub-Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"F0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3-5",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::HEX,6);
	
	m_xlistctrl_event_data.InsertItem (3, "6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Change of State",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::HEX,2);
	
	m_xlistctrl_event_data.InsertItem (4, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(4,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(4,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (4,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsFwRevChange()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FD",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Sub-Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"F1",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"New Sys FW",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::TEXT,1);

	m_xlistctrl_event_data.InsertItem (3, "4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Old Sys FW",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::TEXT,1);

	m_xlistctrl_event_data.InsertItem (4, "5",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(4,1,"New Game FW",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(4,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (4,2, CXEdit::TEXT,1);

	m_xlistctrl_event_data.InsertItem (5, "6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(5,1,"Old Game FW",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(5,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (5,2, CXEdit::TEXT,1);
	
	m_xlistctrl_event_data.InsertItem (6, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(6,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(6,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (6,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsEncryptionKeyChange()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FD",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Sub-Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"F2",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (3, "4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Cause of Change",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (4, "5-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(4,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(4,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (4,2, CXEdit::HEX,4);

	m_xlistctrl_event_data.InsertItem (5, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(5,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(5,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (5,2, CXEdit::DEC,6);

}	
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsTimeDate()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FE",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"reason",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::HEX,2);	

	m_xlistctrl_event_data.InsertItem (2, "3-4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Old Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::DEC,6);

	m_xlistctrl_event_data.InsertItem (3, "5-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"New Date (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,6);

	m_xlistctrl_event_data.InsertItem (4, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(4,1,"New Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(4,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (4,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetupRowsRecordNo()
{
	m_xlistctrl_event_data.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(0,1,"Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(0,2,"FF",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_event_data.InsertItem (1, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(1,1,"Record Number",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(1,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (1,2, CXEdit::DEC,7);

	m_xlistctrl_event_data.InsertItem (2, "5-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(2,1,"Date (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (2,2, CXEdit::DEC,6);

	m_xlistctrl_event_data.InsertItem (3, "7-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_event_data.SetItemText(3,1,"Time (U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_data.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_event_data.SetEdit    (3,2, CXEdit::DEC,6);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::OnSelchangeComboEventTypes() 
{
	CVlcEventLogData::EEventType event_type = (CVlcEventLogData::EEventType)m_ctrl_combo_event_type.GetCurSel();
	SetupRows(event_type);
}


/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::AddRecordToDisplayedList(CVlcEventLogData::EventRecord* p_record)
{
	CString record_num_str = Convert(p_record->m_record_number);
	CString description;
	GetEventDescription(description, *p_record);

	int idx = m_xlistctrl_event_log.GetItemCount();
	m_xlistctrl_event_log.LockWindowUpdate();	
	m_xlistctrl_event_log.InsertItem(idx,"");
	m_xlistctrl_event_log.SetItemText(idx,0,record_num_str, RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_event_log.SetItemText(idx,1,description, RGB(0,0,0), RGB(255,255,255) );  
	m_xlistctrl_event_log.SetItemData(idx,(DWORD)(p_record));
	m_xlistctrl_event_log.UnlockWindowUpdate();
}

/////////////////////////////////////////////////////////////////////////////
inline void CDlgEvents::DisplayField(const CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset)
{
	//p_offset is the offset to the start of the section.  e.g. system section offset is 8 if there is 1 game key etc
	CString value;
	p_packet.GetDataFieldFormatted(value, p_field_id, p_offset );
	m_xlistctrl_event_data.SetItemText(p_field_pos,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
}
inline void CDlgEvents::SetField(CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset)
{
	CString value;
	value = m_xlistctrl_event_data.GetItemText(p_field_pos,2);
	p_packet.SetDataFieldFormatted(value, p_field_id, p_offset);
}

/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecord(CVlcEventLogData::EventRecord* p_record)
{
	CVLCPacket* packet = CVlcEventLogData::GetEventPacket(*p_record);
	m_xlistctrl_event_data.LockWindowUpdate();	
	CVlcEventLogData::EEventType event_type = p_record->m_type;	
	SetupRows(event_type);
	switch(event_type)
	{
		case CVlcEventLogData::EVENT_CASHOUT:					DisplayRecordCashout(*packet);					break;
		case CVlcEventLogData::EVENT_CASHOUT_CTN:				DisplayRecordCashoutCtn(*packet);				break;
		case CVlcEventLogData::EVENT_GAMEWIN:					DisplayRecordGameWin(*packet);					break;
		case CVlcEventLogData::EVENT_GAMEWIN_CTN:				DisplayRecordGameWinCtn(*packet);				break;
		case CVlcEventLogData::EVENT_EXCEPTION:					DisplayRecordException(*packet);				break;
		case CVlcEventLogData::EVENT_CREDIT_SNAPSHOT:			DisplayRecordCreditSnapshot(*packet);			break;
		case CVlcEventLogData::EVENT_MSG_TO_HOST:				DisplayRecordMsgToHost(*packet);				break;
		case CVlcEventLogData::EVENT_STATUS_POLL_TIMEOUT:		DisplayRecordStatusPollTimeout(*packet);		break;
		case CVlcEventLogData::EVENT_FW_REV_CHANGE:				DisplayRecordFwRevChange(*packet);				break;
		case CVlcEventLogData::EVENT_ENCRYPTION_KEY_CHANGE:		DisplayRecordEncryptionKeyChange(*packet);	break;
		case CVlcEventLogData::EVENT_TIME_DATE:					DisplayRecordTimeDate(*packet);					break;	
		case CVlcEventLogData::EVENT_RECORD_NO:					DisplayRecordRecordNo(*packet);					break;	
		default:
			break;
	};
	m_xlistctrl_event_data.UnlockWindowUpdate();	
	m_ctrl_combo_event_type.SetCurSel( (int)event_type);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordCashout(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_CASHOUT_CODE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_CASHOUT_AMOUNT);
	DisplayField(p_packet, 3,		CVLCPacket::EV_CASHOUT_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordCashoutCtn(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_CASHOUT_CTN_VENUE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_CASHOUT_CTN_NUMBER);
	DisplayField(p_packet, 3,		CVLCPacket::EV_CASHOUT_CTN_SECURITY);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordGameWin(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_GAMEWIN_AMOUNT);
	DisplayField(p_packet, 2,		CVLCPacket::EV_GAMEWIN_GAME_ID);
	DisplayField(p_packet, 3,		CVLCPacket::EV_GAMEWIN_CATEGORY);
	DisplayField(p_packet, 4,		CVLCPacket::EV_GAMEWIN_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordGameWinCtn(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_GAMEWIN_CTN_RESERVED2);
	DisplayField(p_packet, 2,		CVLCPacket::EV_GAMEWIN_CTN_AMOUNT);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordException(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_EXCEPTION_CODE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_EXCEPTION_DATA);
	DisplayField(p_packet, 3,		CVLCPacket::EV_EXCEPTION_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordCreditSnapshot(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_CREDIT_SN_SUBTYPE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_CREDIT_SN_BALANCE);
	DisplayField(p_packet, 3,		CVLCPacket::EV_CREDIT_SN_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordMsgToHost(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_MSG_HOST_SUBTYPE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_MSG_HOST_TEXT);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordStatusPollTimeout(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_SPT_SUBTYPE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_SPT_RESERVED3);
	DisplayField(p_packet, 3,		CVLCPacket::EV_SPT_CHANGE_OF_STATE);
	DisplayField(p_packet, 4,		CVLCPacket::EV_SPT_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordFwRevChange(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_FW_SUBTYPE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_FW_NEW_SYS);
	DisplayField(p_packet, 3,		CVLCPacket::EV_FW_OLD_SYS);
	DisplayField(p_packet, 4,		CVLCPacket::EV_FW_NEW_GAME);
	DisplayField(p_packet, 5,		CVLCPacket::EV_FW_OLD_GAME);
	DisplayField(p_packet, 6,		CVLCPacket::EV_FW_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordEncryptionKeyChange(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_ENCRYPT_SYBTYPE);
	DisplayField(p_packet, 2,		CVLCPacket::EV_ENCRYPT_RESERVED3);
	DisplayField(p_packet, 3,		CVLCPacket::EV_ENCRYPT_CAUSE);
	DisplayField(p_packet, 4,		CVLCPacket::EV_ENCRYPT_RESERVED5);
	DisplayField(p_packet, 5,		CVLCPacket::EV_ENCRYPT_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordTimeDate(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_TD_REASON);
	DisplayField(p_packet, 2,		CVLCPacket::EV_TD_OLD_TIME);
	DisplayField(p_packet, 3,		CVLCPacket::EV_TD_NEW_DATE);
	DisplayField(p_packet, 4,		CVLCPacket::EV_TD_NEW_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::DisplayRecordRecordNo(const CVLCPacket& p_packet)
{
	DisplayField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	DisplayField(p_packet, 1,		CVLCPacket::EV_RECORD_NUMBER);
	DisplayField(p_packet, 2,		CVLCPacket::EV_RECORD_DATE);
	DisplayField(p_packet, 3,		CVLCPacket::EV_RECORD_TIME);
}
//////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecord(CVLCPacket& p_packet)
{
	CVlcEventLogData::EEventType event_type = (CVlcEventLogData::EEventType)m_ctrl_combo_event_type.GetCurSel();
	switch(event_type)
	{
		case CVlcEventLogData::EVENT_CASHOUT:					SetRecordCashout(p_packet);					break;
		case CVlcEventLogData::EVENT_CASHOUT_CTN:				SetRecordCashoutCtn(p_packet);				break;
		case CVlcEventLogData::EVENT_GAMEWIN:					SetRecordGameWin(p_packet);					break;
		case CVlcEventLogData::EVENT_GAMEWIN_CTN:				SetRecordGameWinCtn(p_packet);				break;
		case CVlcEventLogData::EVENT_EXCEPTION:					SetRecordException(p_packet);				break;
		case CVlcEventLogData::EVENT_CREDIT_SNAPSHOT:			SetRecordCreditSnapshot(p_packet);			break;
		case CVlcEventLogData::EVENT_MSG_TO_HOST:				SetRecordMsgToHost(p_packet);				break;
		case CVlcEventLogData::EVENT_STATUS_POLL_TIMEOUT:		SetRecordStatusPollTimeout(p_packet);		break;
		case CVlcEventLogData::EVENT_FW_REV_CHANGE:				SetRecordFwRevChange(p_packet);				break;
		case CVlcEventLogData::EVENT_ENCRYPTION_KEY_CHANGE:		SetRecordEncryptionKeyChange(p_packet);	break;
		case CVlcEventLogData::EVENT_TIME_DATE:					SetRecordTimeDate(p_packet);					break;	
		case CVlcEventLogData::EVENT_RECORD_NO:					SetRecordRecordNo(p_packet);					break;	
		default:
			break;
	};
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetDateTime(CVLCPacket& p_packet, int p_pos, int p_field, int p_which_one)
{
	if(m_ctrl_chk_use_current_time.GetCheck()==1)
	{
		CVLCDateTime date_time; //universal time
		ushort time_u = date_time.GetTimeInSeconds();
		ushort date_u = date_time.GetJulianDate();
		if(p_which_one==SET_TIME)
			p_packet.SetDataFieldInt(time_u,p_field);
		if(p_which_one==SET_DATE)
			p_packet.SetDataFieldInt(date_u,p_field);
	}
	else
		SetField(p_packet, p_pos, p_field);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordCashout(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_CASHOUT_CODE);
	SetField(p_packet, 2,		CVLCPacket::EV_CASHOUT_AMOUNT);
	SetDateTime(p_packet, 3,	CVLCPacket::EV_CASHOUT_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordCashoutCtn(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_CASHOUT_CTN_VENUE);
	SetField(p_packet, 2,		CVLCPacket::EV_CASHOUT_CTN_NUMBER);
	SetField(p_packet, 3,		CVLCPacket::EV_CASHOUT_CTN_SECURITY);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordGameWin(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_GAMEWIN_AMOUNT);
	SetField(p_packet, 2,		CVLCPacket::EV_GAMEWIN_GAME_ID);
	SetField(p_packet, 3,		CVLCPacket::EV_GAMEWIN_CATEGORY);
	SetDateTime(p_packet, 4,	CVLCPacket::EV_GAMEWIN_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordGameWinCtn(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_GAMEWIN_CTN_RESERVED2);
	SetField(p_packet, 2,		CVLCPacket::EV_GAMEWIN_CTN_AMOUNT);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordException(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_EXCEPTION_CODE);
	SetField(p_packet, 2,		CVLCPacket::EV_EXCEPTION_DATA);
	SetDateTime(p_packet, 3,	CVLCPacket::EV_EXCEPTION_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordCreditSnapshot(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_CREDIT_SN_SUBTYPE);
	SetField(p_packet, 2,		CVLCPacket::EV_CREDIT_SN_BALANCE);
	SetDateTime(p_packet, 3,	CVLCPacket::EV_CREDIT_SN_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordMsgToHost(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_MSG_HOST_SUBTYPE);
	SetField(p_packet, 2,		CVLCPacket::EV_MSG_HOST_TEXT);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordStatusPollTimeout(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_SPT_SUBTYPE);
	SetField(p_packet, 2,		CVLCPacket::EV_SPT_RESERVED3);
	SetField(p_packet, 3,		CVLCPacket::EV_SPT_CHANGE_OF_STATE);
	SetDateTime(p_packet, 4,	CVLCPacket::EV_SPT_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordFwRevChange(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_FW_SUBTYPE);
	SetField(p_packet, 2,		CVLCPacket::EV_FW_NEW_SYS);
	SetField(p_packet, 3,		CVLCPacket::EV_FW_OLD_SYS);
	SetField(p_packet, 4,		CVLCPacket::EV_FW_NEW_GAME);
	SetField(p_packet, 5,		CVLCPacket::EV_FW_OLD_GAME);
	SetDateTime(p_packet, 6,	CVLCPacket::EV_FW_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordEncryptionKeyChange(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_ENCRYPT_SYBTYPE);
	SetField(p_packet, 2,		CVLCPacket::EV_ENCRYPT_RESERVED3);
	SetField(p_packet, 3,		CVLCPacket::EV_ENCRYPT_CAUSE);
	SetField(p_packet, 4,		CVLCPacket::EV_ENCRYPT_RESERVED5);
	SetDateTime(p_packet, 5,	CVLCPacket::EV_ENCRYPT_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordTimeDate(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_TD_REASON);
	SetField(p_packet, 2,		CVLCPacket::EV_TD_OLD_TIME);
	SetDateTime(p_packet, 3,	CVLCPacket::EV_TD_NEW_DATE, SET_DATE);
	SetDateTime(p_packet, 4,	CVLCPacket::EV_TD_NEW_TIME, SET_TIME);
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::SetRecordRecordNo(CVLCPacket& p_packet)
{
	SetField(p_packet, 0,		CVLCPacket::EVENT_TYPE);
	SetField(p_packet, 1,		CVLCPacket::EV_RECORD_NUMBER);
	SetDateTime(p_packet, 2,	CVLCPacket::EV_RECORD_DATE, SET_DATE);
	SetDateTime(p_packet, 3,	CVLCPacket::EV_RECORD_TIME, SET_TIME);
}

/////////////////////////////////////////////////////////////////////////////

void CDlgEvents::OnShowWindow(BOOL bShow, UINT nStatus) 
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

//////////////////////////////////////////////////////////////////
//Implement IDlgPacket


void CDlgEvents::DisplayPacket(const CVLCPacket& p_packet )
{

}


////////////////////////////////////////////////////////////////////////////////////
/*
Set the packet in the dialog m_xlistctrl_datetime
*/

void CDlgEvents::SetPacket(CVLCPacket& p_packet )
{

}
/////////////////////////////////////////////////////////////////////////////

CVlcData* CDlgEvents::GetVlcData()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return NULL;

	return active_id->GetEventLogData();
}

void CDlgEvents::GetPacketName(CString& p_packet_name)
{
	p_packet_name = CString("Event Log");	
}


/////////////////////////////////////////////////////////////////////////////
//todo
//maybe these next 2 should go in class CVlcEventLogData??
void CDlgEvents::GetEventDescription(CString& p_string, CVlcEventLogData::EventRecord& p_record)
{
	p_string = CString("");

	CVlcEventLogData::EEventType event_type = p_record.m_type;
	switch(event_type)
	{
		case CVlcEventLogData::EVENT_CASHOUT:	
			p_string = CString("Cashout");
			break;
		case CVlcEventLogData::EVENT_CASHOUT_CTN:	
			p_string = CString("Cashout Continuation");
			break;
		case CVlcEventLogData::EVENT_GAMEWIN:	
			p_string = CString("Game Win Data");
			break;
		case CVlcEventLogData::EVENT_GAMEWIN_CTN:
			p_string = CString("Game Win Data Continuation");
			break;
		case CVlcEventLogData::EVENT_EXCEPTION:	
			p_string = CString("Exception");
			break;
		case CVlcEventLogData::EVENT_CREDIT_SNAPSHOT:	
			p_string = CString("Credit At Snapshot");
			break;
		case CVlcEventLogData::EVENT_MSG_TO_HOST:	
			p_string = CString("Message To Host");
			break;
		case CVlcEventLogData::EVENT_STATUS_POLL_TIMEOUT:	
			p_string = CString("Status Poll Timeout");
			break;
		case CVlcEventLogData::EVENT_FW_REV_CHANGE:		
			p_string = CString("Firmware Revision Change");
			break;
		case CVlcEventLogData::EVENT_ENCRYPTION_KEY_CHANGE:	
			p_string = CString("Encryption Key Change");
			break;
		case CVlcEventLogData::EVENT_TIME_DATE:		
			p_string = CString("Date Time");
			break;
		case CVlcEventLogData::EVENT_RECORD_NO:		
			p_string = CString("Record Number / Date Marker");
			break;
		default:
			p_string = CString("Error");
			break;
	};
}

/////////////////////////////////////////////////////////////////////////////

void CDlgEvents::OnDblclkListctrlEventLog(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//Displays the record
	int item = m_xlistctrl_event_log.GetCurSel();
	CVlcEventLogData::EventRecord* record = NULL;
	record = (CVlcEventLogData::EventRecord*)(m_xlistctrl_event_log.GetItemData(item)); 
	DisplayRecord(record);
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::OnRclickListctrlEventLog(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//This sets the start read record	
	int item = m_xlistctrl_event_log.GetCurSel();
	CVlcEventLogData::EventRecord* record = NULL;
	record = (CVlcEventLogData::EventRecord*)(m_xlistctrl_event_log.GetItemData(item)); 
	if(record==NULL)
		return;
	CVlcEventLogData* event_log = (CVlcEventLogData*)(GetVlcData() );
	if(event_log==NULL) 
		return;

	event_log->SetReadRecordStart(record); //rebuild list using start record set
	ShowStartEndRecords(event_log);
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::OnRdblclkListctrlEventLog(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//This sets the end read record
	int item = m_xlistctrl_event_log.GetCurSel();
	CVlcEventLogData::EventRecord* record = NULL;
	record = (CVlcEventLogData::EventRecord*)(m_xlistctrl_event_log.GetItemData(item)); 
	if(record==NULL)
		return;
	CVlcEventLogData* event_log = (CVlcEventLogData*)(GetVlcData() );
	if(event_log==NULL) 
		return;
	
	event_log->SetReadRecordEnd(record); //rebuild list using end record set
	ShowStartEndRecords(event_log);
	
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::OnBtnSetDefault() 
{
	CVlcEventLogData* event_log = (CVlcEventLogData*)(GetVlcData() );
	if(event_log==NULL) 
		return;
	event_log->SetDefaultRead(); //rebuild list using default start end records
	ShowStartEndRecords(event_log);
}

/////////////////////////////////////////////////////////////////////////////
void CDlgEvents::ProcessNotify( int p_id )
{
	if(p_id != CNotify::ACTIVE_ID_CHANGED)
		return;
	
	CVlcEventLogData* event_log = (CVlcEventLogData*)(GetVlcData() );
	if(event_log==NULL)
		return;

	m_xlistctrl_event_log.DeleteAllItems();
	CVlcEventLogData::EventLogItr itr = event_log->GetStartItr();
	
	while(itr != event_log->GetEndItr() )
	{
		AddRecordToDisplayedList(*itr);
		itr++;
	}
	ASSERT(event_log->RecordCount() == m_xlistctrl_event_log.GetItemCount() );
	if(event_log->RecordCount() > 0)
		DisplayRecord(*(event_log->GetStartItr()));	
	else
	{
		m_ctrl_combo_event_type.SetCurSel(CVlcEventLogData::EVENT_RECORD_NO);
		OnSelchangeComboEventTypes();
	}
	ShowStartEndRecords(event_log);
	EnableControls(true);
}

/////////////////////////////////////////////////////////////////////////////
//Updates the display of the start and end read records currently set
//Also updates list of packets that the errors settings can be applied to
void CDlgEvents::ShowStartEndRecords(CVlcEventLogData* p_event_log)
{
	CVlcEventLogData::EventRecord* record_start = NULL;
	CVlcEventLogData::EventRecord* record_end = NULL;
	int record_count=0;
	int pkt_count=0;
	if(p_event_log != NULL)
	{
		record_start = p_event_log->GetReadRecordStart();
		record_end = p_event_log->GetReadRecordEnd();
		record_count = p_event_log->NumRecordsNextRead();
		pkt_count = p_event_log->NumPacketsNextRead();
	}
	
	CString str = CString("Start: ");
	if(record_start != NULL)
		str += Convert(record_start->m_record_number);
	else
		str += CString("NULL");
	m_ctrl_static_start_read.SetWindowText(str);

	str = CString("End: ");
	if(record_end != NULL)
		str += Convert(record_end->m_record_number);
	else
		str += ("NULL");
	m_ctrl_static_end_read.SetWindowText(str);

	str = CString("Records: ");
	str += Convert(record_count);
	m_ctrl_static_record_count.SetWindowText(str);

	str = CString("Packets: ");
	str += Convert(pkt_count);
	m_ctrl_static_packet_count.SetWindowText(str);

	//update the combo used to pick which packets have the errors.
	if(pkt_count==0)
		return; 
	m_ctrl_combo_apply_errors_to.ResetContent();
	if(pkt_count==1)
		m_ctrl_combo_apply_errors_to.AddString("1");
	else
	{
		m_ctrl_combo_apply_errors_to.AddString("All");
		for(int i=1; i<=pkt_count; i++)
			m_ctrl_combo_apply_errors_to.AddString(Convert(i));
	}
	
	//update error setting
	m_ctrl_combo_apply_errors_to.SetCurSel(0);
	CVlcEventLogData* event_log_data = (CVlcEventLogData*)GetVlcData();		
	if(event_log_data==NULL)
		return;
	event_log_data->SetErrors();
}

void CDlgEvents::OnSelchangeComboApplyErrorsTo() 
{
	//update error setting
	CVlcEventLogData* event_log_data = (CVlcEventLogData*)GetVlcData();		
	if(event_log_data==NULL)
		return;
	event_log_data->SetErrors();
}


void CDlgEvents::OnBtnAdd() 
{
	CVlcEventLogData* event_log = (CVlcEventLogData*)(GetVlcData() );
	if(event_log==NULL)
		return;

	CVlcEventLogData::EEventType type = (CVlcEventLogData::EEventType)m_ctrl_combo_event_type.GetCurSel();

	CVLCPacket* event = new CVLCPacket(CVLCPacket::LENGTH_EVENT);
	SetRecord(*event);

	CVlcEventLogData::EventRecord* record = event_log->GetEventRecord(*event, type);
	if(record==NULL)
		return;
	event_log->AddEvent(record);
	AddRecordToDisplayedList(record);
	delete event;
	ASSERT(event_log->RecordCount() == m_xlistctrl_event_log.GetItemCount() );
	OnBtnSetDefault(); //rebuild list using default start end records
}