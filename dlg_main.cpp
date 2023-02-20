
#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_main.h"
#include "fstream.h"
#include "enumser.h"
#include "utilities.h"
#include "vlc_packet.h"
#include "date_time.h"
#include "vlc_receive_thread.h"
#include "vlc_transmit.h"
#include "vlc_id.h"
#include "vlc_id_list.h"
#include "dlg_datetime_memsig.h"
#include "dlg_control.h"
#include "dlg_comms_errors.h"
#include "game_play.h"
#include "random.h"
#include "notify.h"
#include "vlc_comms_output.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Global Access to main window Dlg - intitalised in "VLCSim.cpp"

CDlgMain *g_dlg_main = 0;

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMain::IDD, pParent),
	m_datime_timer(0),
	m_sef_status(false),
	m_active_sub_window(NULL),
	m_active_packet_window(NULL)
{
	//{{AFX_DATA_INIT(CDlgMain)
	m_Connect = FALSE;
	m_PowerGood = FALSE;
	m_sef = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		
	g_notify_mgr->Register( CNotify::SENT_VLC_PACKET, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_PACKET, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_POLL, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_DATA_READ_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_WRITE_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_SITE_INFO_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_WRITE_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_ACK_NACK, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_TRANSACTION_REPLY, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_ERROR, this );

	g_notify_mgr->Register( CNotify::RECEIVE_VLC_UNKNOWN, this );
	g_notify_mgr->Register( CNotify::ACTIVE_ID_CHANGED, this );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMain)
	DDX_Control(pDX, IDC_CHECK_NO_RESP_WRITE_REQ, m_ctrl_chk_no_resp_write_req);
	DDX_Control(pDX, IDC_CHECK_NON_RESPONDING_READ_REQ, m_ctrl_chk_no_resp_read_req);
	DDX_Control(pDX, IDC_CHECK_SET_NOT_RESPONDING, m_ctrl_chk_not_responding);
	DDX_Control(pDX, IDC_STATIC_TERMINAL_MODE, m_ctrl_static_terminal_mode);
	DDX_Control(pDX, IDC_BTN_ERRORS_VIEW, m_ctrl_btn_errors_view);
	DDX_Control(pDX, IDC_CHECK_SHOW_POLLS, m_ctrl_chk_no_polls);
	DDX_Control(pDX, IDC_BTN_DATA_VIEW_LAST_RECEIVED, m_ctrl_btn_data_view_last_received);
	DDX_Control(pDX, IDC_BTN_DATA_VIEW_LAST_SITE_REPLY, m_ctrl_btn_data_view_last_site_reply);
	DDX_Control(pDX, IDC_BTN_DATA_VIEW_LAST_READ_REPLY, m_ctrl_btn_data_view_last_read_reply);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_ctrl_static_status);
	DDX_Control(pDX, IDC_EDIT_ADD_ID, m_ctrl_xedit_add_id);
	DDX_Control(pDX, IDC_CHECK_PAUSE, m_ctrl_chk_watcher_pause);
	DDX_Control(pDX, IDC_CHECK_LOG, m_ctrl_chk_watcher_log);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_ERRORS, m_ctrl_chk_watcher_errors);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_DESCRIPTION, m_ctrl_chk_watcher_description);
	DDX_Control(pDX, IDC_CHECK_ACTIVE_ID_ONLY, m_ctrl_chk_watcher_active_id);
	DDX_Control(pDX, IDC_STATIC_TIME, m_ctrl_static_time_u);
	DDX_Control(pDX, IDC_STATIC_DATE, m_ctrl_static_date_u);
	DDX_Control(pDX, IDC_CHK_ERRORS_TIMING, m_ctrl_chk_errors_timing);
	DDX_Control(pDX, IDC_COMBO_NEXT_RESPONSE, m_ctrl_combo_next_response);
	DDX_Control(pDX, IDC_CHK_ERRORS_SIZE, m_ctrl_chk_errors_size);
	DDX_Control(pDX, IDC_CHK_ERRORS_DATA, m_ctrl_chk_errors_data);
	DDX_Control(pDX, IDC_BTN_NEXT_RESPONSE_SET, m_ctrl_btn_next_response_set);
	DDX_Control(pDX, IDC_BTN_ERRORS_SET, m_ctrl_btn_errors_set);
	DDX_Control(pDX, IDC_BTN_DATA_VIEW_NEXT, m_ctrl_btn_data_view_next);
	DDX_Control(pDX, IDC_BTN_DATA_VIEW_DEFAULT, m_ctrl_btn_data_view_default);
	DDX_Control(pDX, IDC_BTN_DATA_SET_NEXT, m_ctrl_btn_data_next_set);
	DDX_Control(pDX, IDC_XLISTCTRL_POLLING, m_xlistctrl_polling_list);
	DDX_Control(pDX, IDC_STATIC_ACTIVE_ID, m_ctrl_static_active_id);
	DDX_Control(pDX, IDC_COMMS_WATCHER, m_comms_watcher);
	DDX_Control(pDX, IDC_RECEIVE_DEBUG, m_receive_debug);
	DDX_Control(pDX, IDC_COMPORTLIST, m_ComPortList);
	DDX_Control(pDX, IDC_CHILDWINDOWTREE, m_ConfigTree);
	DDX_Check(pDX, IDC_CONNECT, m_Connect);
	DDX_Control(pDX, IDC_MSCOMMCTRL, m_Comms);
	DDX_Text(pDX, IDC_SEF, m_sef);
	DDX_Control(pDX, IDC_MSCOMM2, m_Comms2);
	//}}AFX_DATA_MAP
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
	//{{AFX_MSG_MAP(CDlgMain)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_CHILDWINDOWTREE, OnSelchangedConfigTree)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_COMPORTLIST, OnClickComPortList)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_XLISTCTRL_POLLING, OnDblclkXlistctrlPolling)
	ON_BN_CLICKED(IDC_BTN_DATA_SET_NEXT, OnBtnDataSetNext)
	ON_BN_CLICKED(IDC_BTN_DATA_VIEW_DEFAULT, OnBtnDataViewDefault)
	ON_BN_CLICKED(IDC_BTN_DATA_VIEW_NEXT, OnBtnDataViewNext)
	ON_BN_CLICKED(IDC_BTN_ERRORS_SET, OnBtnErrorsSet)
	ON_BN_CLICKED(IDC_BTN_NEXT_RESPONSE_SET, OnBtnNextResponseSet)
	ON_BN_CLICKED(IDC_BTN_ADD_ID, OnBtnAddId)
	ON_BN_CLICKED(IDC_BTN_DATA_VIEW_LAST_READ_REPLY, OnBtnDataViewLastReadReply)
	ON_BN_CLICKED(IDC_BTN_DATA_VIEW_LAST_RECEIVED, OnBtnDataViewLastReceived)
	ON_BN_CLICKED(IDC_BTN_DATA_VIEW_LAST_SITE_REPLY, OnBtnDataViewLastSiteReply)
	ON_BN_CLICKED(IDC_BTN_ERRORS_VIEW, OnBtnErrorsView)
	ON_BN_CLICKED(IDC_CHECK_LOG, OnCheckLog)
	ON_BN_CLICKED(IDC_CHECK_SET_NOT_RESPONDING, OnCheckSetNotResponding)
	ON_BN_CLICKED(IDC_CHECK_NON_RESPONDING_READ_REQ, OnCheckSetNotResponding)
	ON_BN_CLICKED(IDC_CHECK_NO_RESP_WRITE_REQ, OnCheckSetNotResponding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDlgMain message handlers

BOOL CDlgMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
/*

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
*/

	//List of responses to next packet received:
	//Must be in the same order as declared in 'enum EResponse' in vlc_packet.h
	m_ctrl_combo_next_response.AddString("Auto");
	m_ctrl_combo_next_response.AddString("ACK");
	m_ctrl_combo_next_response.AddString("NACK:Crc Error");
	m_ctrl_combo_next_response.AddString("NACK:Validation Time");
	m_ctrl_combo_next_response.AddString("NACK:Invalid Command");
	m_ctrl_combo_next_response.AddString("NACK:Validation Code");
	m_ctrl_combo_next_response.AddString("NACK:Transaction Error");
	m_ctrl_combo_next_response.AddString("NACK:Configuration Error");
	m_ctrl_combo_next_response.AddString("NACK:Non zero credit");
	m_ctrl_combo_next_response.AddString("None");
	m_ctrl_combo_next_response.SetCurSel(CVLCPacket::RESP_AUTO);


	///////////////////////////////////////////////////////////////////////////////////////
	//Polling ID list
	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_polling_list.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);

	CRect rect;
	m_xlistctrl_polling_list.GetWindowRect(&rect);
	int width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar

	// add columns
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	
	//Id
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = _T("Id");
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width*2/7;
	m_xlistctrl_polling_list.InsertColumn(0, &lvcolumn);
	
	//Poll flags
	TCHAR *	flags[] = { 
		_T("V"),
		_T("D"),
		_T("F"),
		_T("P"),
		_T("I")
	};
	
	for(int i=0; i<5; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = flags[i];
		lvcolumn.iSubItem = i+1;
		lvcolumn.cx = width*1/7;
		m_xlistctrl_polling_list.InsertColumn(i+1, &lvcolumn);
	}
	m_xlistctrl_polling_list.LockWindowUpdate();
	m_xlistctrl_polling_list.DeleteAllItems();
	m_xlistctrl_polling_list.UnlockWindowUpdate();
	/*
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	lvcolumn.fmt = LVCFMT_LEFT;
	m_xlistctrl_polling_list.SetColumn(0,&lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	lvcolumn.fmt = LVCFMT_LEFT;
	m_xlistctrl_polling_list.SetColumn(1,&lvcolumn);
	*/
	///////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////
	// Date / Time
	m_datime_timer = g_timers->AddTimer( 1000, this );
	DisplayDateTime();
	///////////////////////////////////////////////////////////////////////////////////////
	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	//htDataBlocks = m_ConfigTree.InsertItem("Data Blocks", TVI_ROOT, TVI_LAST);
	//htSimulatePlay = m_ConfigTree.InsertItem("Simulate Play", TVI_ROOT, TVI_LAST);

//	htSDB = m_ConfigTree.InsertItem("SDB", TVI_ROOT , TVI_LAST);
	ht_events = m_ConfigTree.InsertItem("Events", TVI_ROOT , TVI_LAST);
	ht_transactions = m_ConfigTree.InsertItem("Transactions", TVI_ROOT , TVI_LAST);
	ht_control = m_ConfigTree.InsertItem("Control", TVI_ROOT , TVI_LAST);
	ht_configuration = m_ConfigTree.InsertItem("Configuration", TVI_ROOT , TVI_LAST);
	ht_monitor = m_ConfigTree.InsertItem("Monitor", TVI_ROOT , TVI_LAST);
//	htCustom = m_ConfigTree.InsertItem("Custom", TVI_ROOT , TVI_LAST);
	ht_datetime_memsig = m_ConfigTree.InsertItem("Datetime Memsig", TVI_ROOT , TVI_LAST);
	ht_exceptions = m_ConfigTree.InsertItem("Exceptions", TVI_ROOT , TVI_LAST);
	ht_comms_errors = m_ConfigTree.InsertItem("Transmit Errors", TVI_ROOT , TVI_LAST);

//	htGamePlay = m_ConfigTree.InsertItem("Game Play", TVI_ROOT , TVI_LAST);
//	htProgramOptions = m_ConfigTree.InsertItem("Config", TVI_ROOT,  TVI_LAST); 


	m_comms_watcher.SetHorizontalExtent(3000);

	CUIntArray ports;
	EnumerateSerialPorts(ports);
	
	_tprintf(_T("The following serial ports are installed on this machine\n"));
	for (i=0; i<ports.GetSize(); i++)
	{
		CString port;
		port.Format("COM%d", ports.ElementAt(i));
		m_ComPortList.InsertItem(0, port);

	}

	m_ComPortList.SetItemState( 0, 1, 1);
	WriteSDBtoCaptureFile = false;

	//CRect rect;
    CWnd *wnd = GetDlgItem(IDC_PLACEHOLDER);
    ASSERT(wnd != NULL);
    ASSERT(IsWindow(wnd->m_hWnd) != FALSE);
    wnd->GetWindowRect(&rect);
    ScreenToClient(&rect);


	m_ctrl_dlg_control.Create(IDD_CONTROL, this);
	m_ctrl_dlg_control.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_control = &m_ctrl_dlg_control;

	m_ctrl_dlg_events.Create(IDD_EVENTS, this);
	m_ctrl_dlg_events.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_events = &m_ctrl_dlg_events;
		
	m_ctrl_dlg_transactions.Create(IDD_TRANSACTIONS, this);
	m_ctrl_dlg_transactions.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_transactions = &m_ctrl_dlg_transactions;
	
	m_ctrl_dlg_configuration.Create(IDD_CONFIGURATION, this);
	m_ctrl_dlg_configuration.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_configuration = &m_ctrl_dlg_configuration;
		
	m_ctrl_dlg_monitor.Create(IDD_MONITOR, this);
	m_ctrl_dlg_monitor.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_monitor = &m_ctrl_dlg_monitor;

	m_ctrl_dlg_exceptions.Create(IDD_EXCEPTIONS, this);
	m_ctrl_dlg_exceptions.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_exceptions = &m_ctrl_dlg_exceptions;
	
	m_ctrl_dlg_comms_errors.Create(IDD_COMMS_ERRORS, this);
	m_ctrl_dlg_comms_errors.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );	
	g_dlg_comms_errors = &m_ctrl_dlg_comms_errors;
	
	m_ctrl_dlg_datetime_memsig.Create(IDD_DATETIME_MEMSIG, this);
	m_ctrl_dlg_datetime_memsig.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	g_dlg_datetime_memsig = &m_ctrl_dlg_datetime_memsig;
		

	CString Title;
	DetermineOSVersion();

	m_ConfigTree.SelectItem(ht_events);
	m_ConfigTree.SelectItem(ht_transactions);
	m_ConfigTree.SelectItem(ht_control);
	m_ConfigTree.SelectItem(ht_configuration);
	m_ConfigTree.SelectItem(ht_monitor);
	m_ConfigTree.SelectItem(ht_exceptions);
	m_ConfigTree.SelectItem(ht_comms_errors);
	m_ConfigTree.SelectItem(ht_datetime_memsig);	//Datetime dialog is displayed initially
	m_active_sub_window = (CDialog*)(&m_ctrl_dlg_datetime_memsig);
	m_active_packet_window = &m_ctrl_dlg_datetime_memsig;
	SetControlEnableStatus();

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);
		m_tooltip.AddTool(GetDlgItem(IDOK), "Exit the Generator and close the COM port");
		m_tooltip.AddTool(GetDlgItem(IDC_CONNECT), "Toggle Connection to the specified COM port");
		m_tooltip.AddTool(GetDlgItem(IDC_COMPORTLIST), "Select the COM port to connect to");
	}

	m_ctrl_xedit_add_id.SetLength(4);
	m_ctrl_xedit_add_id.SetType(CXEdit::HEX);
	g_dlg_main->DisplayActiveIdStatus();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgMain::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawTitle();
		CDialog::OnPaint();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgMain::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::OnSelchangedConfigTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_ConfigTree.UpdateData(true);

	m_ctrl_dlg_events.ShowWindow(SW_HIDE);
	m_ctrl_dlg_transactions.ShowWindow(SW_HIDE);
	m_ctrl_dlg_control.ShowWindow(SW_HIDE);
	m_ctrl_dlg_configuration.ShowWindow(SW_HIDE);
	m_ctrl_dlg_monitor.ShowWindow(SW_HIDE);
	m_ctrl_dlg_exceptions.ShowWindow(SW_HIDE);
	m_ctrl_dlg_comms_errors.ShowWindow(SW_HIDE);
	m_ctrl_dlg_datetime_memsig.ShowWindow(SW_HIDE);

	if(m_ConfigTree.GetSelectedItem() == ht_control)
	{
		m_ctrl_dlg_control.EnableWindow(TRUE);
		m_ctrl_dlg_control.ShowWindow(SW_SHOW);
		m_ctrl_dlg_control.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_control);
		m_active_packet_window = &m_ctrl_dlg_control;
		SetControlEnableStatus();
	}
	
	if(m_ConfigTree.GetSelectedItem() == ht_events)
	{
		m_ctrl_dlg_events.EnableWindow(TRUE);
		m_ctrl_dlg_events.ShowWindow(SW_SHOW);
		m_ctrl_dlg_events.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_events);
		m_active_packet_window = &m_ctrl_dlg_events;
		SetControlEnableStatus();
	}
	
	if(m_ConfigTree.GetSelectedItem() == ht_transactions)
	{
		m_ctrl_dlg_transactions.EnableWindow(TRUE);
		m_ctrl_dlg_transactions.ShowWindow(SW_SHOW);
		m_ctrl_dlg_transactions.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_transactions);
		m_active_packet_window = &m_ctrl_dlg_transactions;
		SetControlEnableStatus();
	}
	
	if(m_ConfigTree.GetSelectedItem() == ht_configuration)
	{
		m_ctrl_dlg_configuration.EnableWindow(TRUE);
		m_ctrl_dlg_configuration.ShowWindow(SW_SHOW);
		m_ctrl_dlg_configuration.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_configuration);
		m_active_packet_window = &m_ctrl_dlg_configuration;
		SetControlEnableStatus();
	}
	
	if(m_ConfigTree.GetSelectedItem() == ht_monitor)
	{
		m_ctrl_dlg_monitor.EnableWindow(TRUE);
		m_ctrl_dlg_monitor.ShowWindow(SW_SHOW);
		m_ctrl_dlg_monitor.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_monitor);
		m_active_packet_window = &m_ctrl_dlg_monitor;
		SetControlEnableStatus();
	}
	
	if(m_ConfigTree.GetSelectedItem() == ht_exceptions)
	{
		m_ctrl_dlg_exceptions.EnableWindow(TRUE);
		m_ctrl_dlg_exceptions.ShowWindow(SW_SHOW);
		m_ctrl_dlg_exceptions.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_exceptions);
		m_active_packet_window = NULL;
		SetControlEnableStatus();
	}

	if(m_ConfigTree.GetSelectedItem() == ht_comms_errors)
	{
		m_ctrl_dlg_comms_errors.EnableWindow(TRUE);
		m_ctrl_dlg_comms_errors.ShowWindow(SW_SHOW);
		m_ctrl_dlg_comms_errors.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_comms_errors);
		m_active_packet_window = NULL;
		SetControlEnableStatus();
	}
	
	if(m_ConfigTree.GetSelectedItem() == ht_datetime_memsig)
	{
		m_ctrl_dlg_datetime_memsig.EnableWindow(TRUE);
		m_ctrl_dlg_datetime_memsig.ShowWindow(SW_SHOW);
		m_ctrl_dlg_datetime_memsig.InvalidateRect(NULL);
		m_active_sub_window = (CDialog*)(&m_ctrl_dlg_datetime_memsig);
		m_active_packet_window = &m_ctrl_dlg_datetime_memsig;
		SetControlEnableStatus();
	}	
	if(m_active_packet_window != NULL)
	{
		OnBtnDataViewNext(); //Vew packet data for next to send
		DisplayErrors(PKT_NEXT_TO_SEND, false); //View error settings (without detail) for next to send 
		DisplayResponseToNextReceived(); //view response setting for next recieved.
	}
	else
		DisplayStatus();

	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////
void CDlgMain::DisplayStatus()
{
	CString active_id;
	g_vlc_id_list->GetActiveId(active_id);
	
	if(active_id != "")
	{
		if(m_active_sub_window == (CDialog*)(&m_ctrl_dlg_comms_errors))
		{
			CString status = CString("Comms Errors For ID: " ) + active_id;
			m_ctrl_static_status.SetWindowText(status);
		}
		else if(m_active_sub_window == (CDialog*)(&m_ctrl_dlg_exceptions))
		{
			CString status = CString("Exceptions For ID: " ) + active_id;
			m_ctrl_static_status.SetWindowText(status);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::DetermineOSVersion()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	if (GetVersionEx(&osv))
	{
		switch(osv.dwPlatformId)
		{
		case VER_PLATFORM_WIN32s: //Win32s on Windows 3.1.
			m_OSVersion = "Microsoft® Windows 3.1";
			break;
			
		case VER_PLATFORM_WIN32_WINDOWS: //WIN32 on 95 or 98
			m_WinNT = false;
			break;
			
		case VER_PLATFORM_WIN32_NT: //Win32 on Windows NT.
			m_WinNT = true;
			break;
			
		default:
			AfxMessageBox("Failed to get correct Operating System.", MB_OK);  
		} //end switch
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
void CDlgMain::DrawTitle()
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void CDlgMain::OnConnect() 
{
	UpdateData(true);
	CButton* pConnection = (CButton*)GetDlgItem(IDC_CONNECT);
	CButton* pPort = (CButton*)GetDlgItem(IDC_COMPORTSETTING);

	if(m_Connect) 
	{
		UpdateData(TRUE);							// Get data from dialog
		
		//m_Comms.SetCommPort(atoi(SelectedComPort.Mid(3, 1)));
		int port_num=0;
		if(SelectedComPort.GetLength()==4)
			port_num = atoi( SelectedComPort.Mid(3, 1));
		else if(SelectedComPort.GetLength()==5)
			port_num = atoi( SelectedComPort.Mid(3, 2));
		else
			return;
		m_Comms.SetCommPort(port_num);

		m_Comms.SetSettings("9600,n,8,1");
		m_Comms.SetRThreshold(1);			// Min number of bytes before receive event fires
		m_Comms.SetSThreshold(1);			// send event fires when OP buffer empty
		m_Comms.SetInputLen(0);				// Can set this to 0 => gets all of the IP buffer
		m_Comms.SetInputMode(1);			// Set the input mode to receive binary number */
		m_Comms.SetPortOpen(TRUE);			// Open Comms Port 
		m_Comms.GetInput();					// Clear the IP buffer


		if( m_Comms.GetPortOpen() )
		{
			pConnection->SetWindowText("Dis&connect");	// Change button to Disconnect
			//m_Comms.SetDTREnable(true); // power good
			//m_Comms.SetRTSEnable(true);
			//todo
			// Note: the DSR line will not be phisically connected in the VLC system - so there's no point
			// in monitoring any other input lines other than the receive line.
			if( m_Comms.GetDSRHolding() ) // sef status 
			{
//				m_sef = "SEF Good";
//				m_sef_status = true;
			}
			else
			{
//				m_sef = "SEF Bad";
//				m_sef_status = false;
			}
			//todo - make these 2 objects respond to connect notification.
			g_vlc_transmit_manager->OpenPort( &m_Comms );
			//g_game_mgr->BeginTransmitting();
			g_notify_mgr->Notify( CNotify::CONNECT );
		}
		else
		{
			AfxMessageBox("Error opening Port");
			m_Connect = true;		
		}
	}
	else
	{
		//m_Comms.SetDTREnable(false); //power bad
		m_Comms.SetPortOpen(false);		
		g_vlc_transmit_manager->ClosePort();
		pConnection->SetWindowText("&Connect");	
		m_sef = "";		
		g_notify_mgr->Notify( CNotify::DISCONNECT );
	}
	
	UpdateData(false);
}

void CDlgMain::DisplayDateTime()
{
	CVLCDateTime date_time; //this is the universal date time
	m_ctrl_static_date_u.SetWindowText( date_time.GetDate(true) );
	m_ctrl_static_time_u.SetWindowText( date_time.GetTime(true) );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::ProcessTimeout( int p_id ) //for ITimer
{
	if( p_id = m_datime_timer )
	{
		if( m_Comms.GetPortOpen() )
		{
			/*
			if( m_Comms.GetDSRHolding() )
			{
				m_sef = "SEF Good";
				m_sef_status = true;
			}
			else
			{
				m_sef = "SEF Bad";
				m_sef_status = false;
			}
			*/
		}
		UpdateData(false);
		DisplayDateTime();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDlgMain::DisplayPacket(CVLCPacket::StringList& p_pkt_strs, COLORREF& p_colour)
{
	CVLCPacket::StringListItr itr = p_pkt_strs.begin();

	//int count = p_pkt_strs.size;
	while(itr != p_pkt_strs.end())
	{
		//CString* = *itr;

		m_comms_watcher.InsertString(0,*(*itr),p_colour);
		itr++;

		int count = m_comms_watcher.GetCount();
		while(count>100)
		{
			m_comms_watcher.DeleteString( count-1 );	
			count = m_comms_watcher.GetCount();
		}
	}
}

void CDlgMain::DisplayPacket(CVLCPacket* p_packet)
{
	if(p_packet==NULL)
		return;

	if(m_ctrl_chk_watcher_pause.GetCheck()==1)
		return;

	if(m_ctrl_chk_no_polls.GetCheck()==1)
	{
		if( (p_packet->GetTransmitDirection() == CVLCPacket::RECEIVE) &&
			(p_packet->GetCommandType() == CVLCPacket::CMD_POLL) )
			return;
	}

	if(m_ctrl_chk_watcher_active_id.GetCheck()==1)
	{
		CString active_id, packet_id;
		g_vlc_id_list->GetActiveId(active_id);
		p_packet->GetTerminalId(packet_id);
		if(active_id!=packet_id)
			return;
	}

	COLORREF colour;
	if(p_packet->GetTransmitDirection()==CVLCPacket::RECEIVE)
		colour = RGB(0,0,0); //black
	else
		colour = RGB(0,0,255); //blue
	if(p_packet->GetErrorStatus() != CVLCPacket::ERR_NONE)
		colour = RGB(255,0,0);	//red

	bool show_description = (m_ctrl_chk_watcher_description.GetCheck()==1);
	bool show_errors = (m_ctrl_chk_watcher_errors.GetCheck()==1);
	CVLCPacket::StringList str_list;
	p_packet->GetAsStringList(str_list,show_errors,show_description,false);
	CVLCPacket::StringListItr itr = str_list.begin();
	while(itr != str_list.end())
	{
		m_comms_watcher.InsertString(0,*(*itr),colour);
		itr++;
		int count = m_comms_watcher.GetCount();
		while(count>1000)
		{
			m_comms_watcher.DeleteString( count-1 );	
			count = m_comms_watcher.GetCount();
		}
	}
	//if(m_ctrl_chk_watcher_log.GetCheck()==1)
	//	g_comms_output->Output(p_packet);
	if(g_comms_output->IsLoggingEnabled())
		g_comms_output->Output(p_packet);	
	//if(show_description)
	//	m_comms_watcher.InsertString(0,"",RGB(0,0,0));
}

void CDlgMain::ProcessNotify( int p_id )  // for INotifiable
{
	CVLCPacket::StringList str_list;
	switch (p_id)
	{
		COLORREF colour;
		case CNotify::RECEIVE_VLC_PACKET:
		{
			DisplayPacket( g_receive_manager->GetLastPacket() );
			return;
		}

		case CNotify::SENT_VLC_PACKET:	
		{
			DisplayPacket(g_vlc_transmit_manager->GetLastPacket());
			return;
		}
			
		case CNotify::ACTIVE_ID_CHANGED:
		{
			//update display of active ID
			/*
			CString active_id_str;
			g_vlc_id_list->GetActiveId(active_id_str);
			CString str("Active ID: ");
			str+=active_id_str;
			m_ctrl_static_active_id.SetWindowText(str);
			*/
			DisplayActiveIdStatus();

			//Update display of the packet data (next to send)
			if(m_active_packet_window != NULL)
			{
				OnBtnDataViewNext(); //View packet data for next to send
				DisplayErrors(PKT_NEXT_TO_SEND, false); //View error settings (without detail) for next to send 
				DisplayResponseToNextReceived(); //view response setting for next recieved.
			}
			else
				DisplayStatus();
			break;
		}
		default:
			break;			
	};
}

void CDlgMain::DisplayActiveIdStatus()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	CString id_str = CString("Null");
	CString mode_str;
	CVlcControlData* control_data;
	if(active_id != NULL)
	{
		active_id->GetId(id_str);
		control_data = (CVlcControlData*)(active_id->GetControlData());
		if(control_data != NULL)
			control_data->GetModeAsString(mode_str);
	}
	CString str = CString("Active ID: ") + id_str;
	m_ctrl_static_active_id.SetWindowText(str);
	m_ctrl_static_terminal_mode.SetWindowText(mode_str);

	if(active_id != NULL)
	{
		if(active_id->GetNonResponsivePolls())
			m_ctrl_chk_not_responding.SetCheck(1);
		else
			m_ctrl_chk_not_responding.SetCheck(0);

		if(active_id->GetNonResponsiveReadReq())
			m_ctrl_chk_no_resp_read_req.SetCheck(1);
		else
			m_ctrl_chk_no_resp_read_req.SetCheck(0);

		if(active_id->GetNonResponsiveWriteReq())
			m_ctrl_chk_no_resp_write_req.SetCheck(1);
		else
			m_ctrl_chk_no_resp_write_req.SetCheck(0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::OnTimer(UINT nIDEvent) 
{
	g_timers->ProcessTimeout(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL CDlgMain::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CDlgMain, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgMain)
	ON_EVENT(CDlgMain, IDC_MSCOMMCTRL, 1 /* OnComm */, OnComms, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::OnComms() 
{
	if( m_Comms.GetCommEvent() == 2 ) //receive
	{
		g_receive_manager->ProcessEvent( m_Comms );
	}

	else if( m_Comms.GetCommEvent() == 1 ) //sent
	{
		g_vlc_transmit_manager->ProcessEvent();
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDlgMain::OnClickComPortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CPoint pt;

	::GetCursorPos((LPPOINT)&pt);
	ScreenToClient(&pt);
	
	POSITION selectedpos = m_ComPortList.GetFirstSelectedItemPosition();
	int selected = m_ComPortList.GetNextSelectedItem(selectedpos);

	SelectedComPort = m_ComPortList.GetItemText(selected, 0);

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HBRUSH CDlgMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int ctrl_id = pWnd->GetDlgCtrlID();
	if( ctrl_id == IDC_SEF )
	{
	}
	else
		pDC->SetTextColor( RGB(0,0,255) );

	return hbr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDlgMain::OnDblclkXlistctrlPolling(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int item = m_xlistctrl_polling_list.GetCurSel();

	CString id_str = m_xlistctrl_polling_list.GetItemText(item,COLUMN_ID);
	g_vlc_id_list->SetActiveId(id_str);

	*pResult = 0;
}



void CDlgMain::AddId(CVlcId* p_id)
{
	if(p_id==NULL)
		return;
	CString id_str;
	p_id->GetId(id_str);
	int idx = m_xlistctrl_polling_list.GetItemCount();
	m_xlistctrl_polling_list.LockWindowUpdate();
	m_xlistctrl_polling_list.InsertItem(idx, id_str, RGB(100,100,100),RGB(255,255,255)); //greyed out to indicate not being polled
	m_xlistctrl_polling_list.SetItemData(idx, (DWORD)(p_id));
	m_xlistctrl_polling_list.UnlockWindowUpdate();

	int list_count = g_vlc_id_list->IdCount();
	int display_count = m_xlistctrl_polling_list.GetItemCount();

	ASSERT(g_vlc_id_list->IdCount() == m_xlistctrl_polling_list.GetItemCount() );
	m_xlistctrl_polling_list.Invalidate(true);
	m_xlistctrl_polling_list.UnlockWindowUpdate();
}

void CDlgMain::UpdateIdDisplay(CVlcId* p_id, const CVLCPacket* p_poll)
{
	int count = m_xlistctrl_polling_list.GetItemCount();
	ASSERT(g_vlc_id_list->IdCount()==count);
	
	int idx=0;
	bool found=false;
	for(idx=0; idx<count; idx++)
	{
		if(m_xlistctrl_polling_list.GetItemData(idx) == (DWORD)(p_id))
		{
			found=true;
			break;
		}
	}
	if(!found)
		return;

	CString id1_str = m_xlistctrl_polling_list.GetItemText(idx,COLUMN_ID);
	CString id2_str;
	p_id->GetId(id2_str);
	ASSERT(id1_str==id2_str);
	
	if(p_poll==NULL)
		return;
	bool v = p_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_V);
	bool d = p_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_D);
	bool f = p_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_F);
	bool p = p_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_P);
	bool i = p_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_I);

	m_xlistctrl_polling_list.SetItemText(idx, COLUMN_ID, id1_str,RGB(0,0,255), RGB(255,255,255) ); 
	CString status;

	if(v) status = CString("*");
	else status = CString("");
	m_xlistctrl_polling_list.SetItemText(idx, COLUMN_V, status,RGB(0,0,255), RGB(255,255,255) ); 

	if(d) status = CString("*");
	else status = CString("");
	m_xlistctrl_polling_list.SetItemText(idx, COLUMN_D, status,RGB(0,0,255), RGB(255,255,255) ); 

	if(p) status = CString("*");
	else status = CString("");
	m_xlistctrl_polling_list.SetItemText(idx, COLUMN_P, status,RGB(0,0,255), RGB(255,255,255) ); 

	if(f) status = CString("*");
	else status = CString("");
	m_xlistctrl_polling_list.SetItemText(idx, COLUMN_F, status,RGB(0,0,255), RGB(255,255,255) ); 

	if(i) status = CString("*");
	else status = CString("");
	m_xlistctrl_polling_list.SetItemText(idx, COLUMN_I, status,RGB(0,0,255), RGB(255,255,255) ); 

	m_xlistctrl_polling_list.UnlockWindowUpdate();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CDialog* const CDlgMain::GetActiveSubWindow()
{
	return m_active_sub_window;
}

void CDlgMain::SetDataSendControlsEnabled(bool p_enable) //set next, view next, view default
{
	m_ctrl_btn_data_next_set.EnableWindow(p_enable);
	m_ctrl_btn_data_view_next.EnableWindow(p_enable);
	m_ctrl_btn_data_view_default.EnableWindow(p_enable);
	m_ctrl_btn_data_view_last_site_reply.EnableWindow(p_enable);
	m_ctrl_btn_data_view_last_read_reply.EnableWindow(p_enable);
}

void CDlgMain::SetDataReceiveControlsEnabled(bool p_enable)
{
	m_ctrl_combo_next_response.EnableWindow(p_enable);
	m_ctrl_btn_data_view_last_received.EnableWindow(p_enable);
	m_ctrl_btn_next_response_set.EnableWindow(p_enable);
}

void CDlgMain::SetErrorControlsEnabled(bool p_enable)
{
	m_ctrl_chk_errors_timing.EnableWindow(p_enable);
	m_ctrl_chk_errors_size.EnableWindow(p_enable);
	m_ctrl_chk_errors_data.EnableWindow(p_enable);
	m_ctrl_btn_errors_set.EnableWindow(p_enable);
	m_ctrl_btn_errors_view.EnableWindow(p_enable);
}


void CDlgMain::SetControlEnableStatus()
{	
	if( (m_active_sub_window==g_dlg_control) ||
		(m_active_sub_window==g_dlg_configuration ) ||
		(m_active_sub_window==g_dlg_transactions) ||
		(m_active_sub_window==g_dlg_datetime_memsig ))	
	{
		SetDataSendControlsEnabled(true);
		SetDataReceiveControlsEnabled(true);
		SetErrorControlsEnabled(true);
	}

	else if((m_active_sub_window==g_dlg_monitor) || 
		    (m_active_sub_window==g_dlg_events) ) 
	{
		SetDataSendControlsEnabled(true);
		SetDataReceiveControlsEnabled(false);
		SetErrorControlsEnabled(true);
	}

	else if(m_active_sub_window==g_dlg_exceptions)
	{
		SetDataSendControlsEnabled(false);
		SetDataReceiveControlsEnabled(false);
		SetErrorControlsEnabled(true);
	}

	else
	{
		SetDataSendControlsEnabled(false);
		SetDataReceiveControlsEnabled(false);
		SetErrorControlsEnabled(false);
	}
}


void CDlgMain::OnBtnDataSetNext() 
{
	CVLCPacket* next_to_send = GetPacket(PKT_NEXT_TO_SEND);
	if(next_to_send==NULL) return;
	m_active_packet_window->SetPacket(*next_to_send);
	DisplayPacketStatus(CString("Next To Send Set"));
}

void CDlgMain::OnBtnDataViewDefault() 
{
	CVLCPacket* default_pkt = GetPacket(PKT_DEFAULT);
	if(default_pkt==NULL) return;
	m_active_packet_window->DisplayPacket(*default_pkt);
	DisplayErrorSettings(*default_pkt); //note: should be no errors set for the default
	DisplayPacketStatus(CString("Default"));
}

void CDlgMain::OnBtnDataViewNext() 
{
	CVLCPacket* next_to_send = GetPacket(PKT_NEXT_TO_SEND);
	if(next_to_send==NULL) return;
	m_active_packet_window->DisplayPacket(*next_to_send);
	DisplayErrorSettings(*next_to_send,false);
	DisplayPacketStatus(CString("Next To Send"));
}

void CDlgMain::OnBtnDataViewLastReadReply() 
{
	CVLCPacket* last_read_reply = GetPacket(PKT_LAST_SENT_READ_REPLY);
	if(last_read_reply !=NULL )
	{
		m_active_packet_window->DisplayPacket(*last_read_reply);
		DisplayPacketStatus(CString("Last Read Reply"),true,last_read_reply);
		DisplayErrorSettings(*last_read_reply); 
	}
	else
		DisplayPacketStatus(CString("No Read Replies sent"));		
}



void CDlgMain::OnBtnDataViewLastSiteReply() 
{
	CVLCPacket* last_site_reply = GetPacket(PKT_LAST_SENT_SITE_REPLY);
	if(last_site_reply !=NULL )
	{
		m_active_packet_window->DisplayPacket(*last_site_reply);
		DisplayPacketStatus(CString("Last Site Info Reply"),true,last_site_reply);
		DisplayErrorSettings(*last_site_reply); 
	}
	else
		DisplayPacketStatus(CString("No Site Info Replies sent"));	
}

void CDlgMain::OnBtnDataViewLastReceived() 
{
	CVLCPacket* last_received = GetPacket(PKT_LAST_RECEIVED);
	if(last_received !=NULL )
	{
		m_active_packet_window->DisplayPacket(*last_received);
		DisplayPacketStatus(CString("Last Write Request"),true,last_received);
		DisplayErrorSettings(*last_received); 
	}
	else
		DisplayPacketStatus(CString("No Write Requests received"));	
}

CVlcData* CDlgMain::GetVlcData() //Return Vlc data - depending on current packet dialog dusplayed
{
	if(m_active_packet_window == NULL)
		return NULL;
	return m_active_packet_window->GetVlcData();
}

CVLCPacket* CDlgMain::GetPacket(CDlgMain::EPacketType p_packet_type)
{
	CVlcData* vlc_data = GetVlcData();
	if(vlc_data==NULL) return NULL;
	switch(p_packet_type)
	{
		case PKT_DEFAULT: return vlc_data->GetDefault();
		case PKT_NEXT_TO_SEND: return vlc_data->GetNextToSend();	
		case PKT_LAST_SENT_READ_REPLY: return vlc_data->GetLastSentReadReply();
		case PKT_LAST_SENT_SITE_REPLY: return vlc_data->GetLastSentSiteReply();
		case PKT_LAST_RECEIVED: return vlc_data->GetLastReceived();
		default: return NULL;		
	};
}

void CDlgMain::DisplayPacketStatus(CString& p_packet_type,bool p_include_resp,CVLCPacket* p_pkt)
{
	CVlcData* vlc_data = GetVlcData();
	if(vlc_data==NULL) return;
	CString pkt_name;
	m_active_packet_window->GetPacketName(pkt_name);
	CString status = pkt_name;
	status += CString(" : ");
	status += p_packet_type;
	status += CString(" For ID ");
	status += vlc_data->GetId();

	if(p_include_resp)
	{
		if(p_pkt==NULL)
			return;
		CVLCPacket::EResponse response = p_pkt->GetResponse();
		CString resp_str;
		CVLCPacket::GetResponseAsString(resp_str,response);
		status += ", Response:";
		status += resp_str;	
	}
	m_ctrl_static_status.SetWindowText(status);
}

//////////////////////////////////////////////

void CDlgMain::OnBtnErrorsSet() 
{
	//Set the Errors for the next packet to send
	CVLCPacket* next_to_send = GetPacket(PKT_NEXT_TO_SEND);
	if(next_to_send==NULL) return;
	SetErrorSettings(*next_to_send);
	//check if its the event log => special case for setting errors
	if(m_active_packet_window == g_dlg_events)
	{
		CVlcEventLogData* event_log_data = (CVlcEventLogData*)GetVlcData();		
		if(event_log_data==NULL)
			return;
		event_log_data->SetErrors();
	}
}

void CDlgMain::OnBtnErrorsView() 
{
	//View the Errors for the next packet to send
	CVLCPacket* next_to_send = GetPacket(PKT_NEXT_TO_SEND);
	if(next_to_send==NULL) return;
	DisplayErrorSettings(*next_to_send);
	//check if its the event log => special case for setting errors
	if(m_active_packet_window == g_dlg_events)
	{
		CVlcEventLogData* event_log_data = (CVlcEventLogData*)GetVlcData();		
		if(event_log_data==NULL)
			return;
		event_log_data->ViewErrors();
	}
}

void CDlgMain::DisplayErrors(EPacketType p_packet_type,bool p_include_detail)
{
	CVLCPacket* pkt = GetPacket(p_packet_type);
	if(pkt==NULL) return;
	DisplayErrorSettings(*pkt,p_include_detail);
}

void CDlgMain::OnBtnNextResponseSet() 
{	
	CVlcData* vlc_data = GetVlcData();
	if(vlc_data==NULL) return;
	CVLCPacket::EResponse response = (CVLCPacket::EResponse)(m_ctrl_combo_next_response.GetCurSel());
	vlc_data->SetResponseToNextReceived(response);	
}

void CDlgMain::DisplayResponseToNextReceived()
{
	CVlcData* vlc_data = GetVlcData();
	if(vlc_data==NULL) return;
	CVLCPacket::EResponse response = vlc_data->GetResponseToNextReceived();
	m_ctrl_combo_next_response.SetCurSel( (int)(response));
}

///////////////////////////////////////////////////////////



void CDlgMain::DisplayErrorSettings(const CVLCPacket& p_packet,bool p_update_details,CString& p_status_msg)
{
	if(p_packet.GetDataErrors() != NULL) 
		m_ctrl_chk_errors_data.SetCheck(1);
	else 
		m_ctrl_chk_errors_data.SetCheck(0);

	if(p_packet.GetTimingErrors() != NULL) 
		m_ctrl_chk_errors_timing.SetCheck(1);
	else 
		m_ctrl_chk_errors_timing.SetCheck(0);

	if(p_packet.GetSizeErrors() != NULL) 
		m_ctrl_chk_errors_size.SetCheck(1);
	else 
		m_ctrl_chk_errors_size.SetCheck(0);

	if(!p_update_details)
		return; //Don't update the display of the actual errors in the errors dlg

	if(p_packet.GetDataErrors() != NULL)
		g_dlg_comms_errors->DisplayErrorsData( *(p_packet.GetDataErrors()) );
	else
		g_dlg_comms_errors->DisplayErrorsDataDefault();

	if(p_packet.GetTimingErrors() != NULL)
		g_dlg_comms_errors->DisplayErrorsTiming( *(p_packet.GetTimingErrors()) );
	else
		g_dlg_comms_errors->DisplayErrorsTimingDefault();

	if(p_packet.GetSizeErrors() != NULL)
		g_dlg_comms_errors->DisplayErrorsDataSize( *(p_packet.GetSizeErrors()));
	else
		g_dlg_comms_errors->DisplayErrorsSizeDefault();
	
}


void CDlgMain::SetErrorSettings(CVLCPacket& p_packet)
{
	CommsErrorsCorruptData* data_errors = NULL;
	CommsErrorsTiming* timing_errors = NULL;
	CommsErrorsDataSize* size_errors = NULL;

	if(m_ctrl_chk_errors_data.GetCheck()==1)
	{
		data_errors = new CommsErrorsCorruptData();
		g_dlg_comms_errors->SetErrorsData(*data_errors);	
	}
	
	if(m_ctrl_chk_errors_timing.GetCheck()==1)
	{
		timing_errors = new CommsErrorsTiming();
		g_dlg_comms_errors->SetErrorsTiming(*timing_errors);
		
	}
	if(m_ctrl_chk_errors_size.GetCheck()==1)
	{
		size_errors = new CommsErrorsDataSize();
		g_dlg_comms_errors->SetErrorsDataSize(*size_errors);
	}
	p_packet.SetDataErrors(data_errors);
	p_packet.SetTimingErrors(timing_errors);
	p_packet.SetSizeErrors(size_errors);
}

void CDlgMain::DisplayResponseSetting(const CVLCPacket& p_packet)
{
	m_ctrl_combo_next_response.SetCurSel(p_packet.GetResponse());	
}

void CDlgMain::SetResponseSetting(CVLCPacket& p_packet)
{
	CVLCPacket::EResponse response = (CVLCPacket::EResponse)(m_ctrl_combo_next_response.GetCurSel());
	p_packet.SetResponse(response); 
}

void CDlgMain::OnBtnAddId() 
{
	CString id;
	m_ctrl_xedit_add_id.GetWindowText(id);
	//Add leading 0's
	while(id.GetLength() < 4)
		id = CString("0") + id;
	id.MakeUpper();
	if(id == "0000")
	{
		MessageBox("Invalid ID");
		m_ctrl_xedit_add_id.SetWindowText("");
		return;
	}
	m_ctrl_xedit_add_id.SetWindowText(id);
	g_vlc_id_list->AddId(id);
}

void CDlgMain::DisplayNextToSend()
{
	OnBtnDataViewNext();
}

void CDlgMain::DisplayDefault()
{
	OnBtnDataViewDefault();
}

void CDlgMain::DisplayLastReceived() //last write request
{
	OnBtnDataViewLastReceived();
}

void CDlgMain::DisplayLastReadReplyHost()
{
	OnBtnDataViewLastReadReply();
}

void CDlgMain::DisplayLastReadReplySite()
{
	OnBtnDataViewLastSiteReply();
}




void CDlgMain::OnCheckLog() 
{
	if(m_ctrl_chk_watcher_log.GetCheck()==1)
		g_comms_output->EnableLogging(true);
	else
		g_comms_output->EnableLogging(false);
}

void CDlgMain::OnCheckSetNotResponding() 
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return;
	bool non_responsive_polls = (m_ctrl_chk_not_responding.GetCheck()==1);
	bool non_responsive_read_req = (m_ctrl_chk_no_resp_read_req.GetCheck()==1);
	bool non_responsive_write_req = (m_ctrl_chk_no_resp_write_req.GetCheck()==1);
	active_id->SetNonResponsivePolls(non_responsive_polls);
	active_id->SetNonResponsiveReadReq(non_responsive_read_req);
	active_id->SetNonResponsiveWriteReq(non_responsive_write_req);
}

/*
void CDlgMain::OnCheckNonRespondingReadReq() 
{
	
	
}

void CDlgMain::OnCheckNoRespWriteReq() 
{
	
	
}
*/