

#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_comms_errors.h"
#include "game_play.h"
#include "utilities.h"
#include  "dlg_main.h"
#include "program_options_dlg.h"
#include "vlc_id_list.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCommsErrors dialog

///////////////////////////////
//Golabal access to the dialog

CDlgCommsErrors* g_dlg_comms_errors = 0;
///////////////////////////////


CDlgCommsErrors::CDlgCommsErrors(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCommsErrors::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCommsErrors)
	//}}AFX_DATA_INIT
	g_dlg_comms_errors = this;
	g_notify_mgr->Register( CNotify::ACTIVE_ID_CHANGED, this );
}


void CDlgCommsErrors::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCommsErrors)
	DDX_Control(pDX, IDC_CHECK_N_BITS, m_ctrl_chk_corrupt_n_bits);
	DDX_Control(pDX, IDC_CHECK_ENABLE_TIMING_ER, m_ctrl_chk_enable_timing_err);
	DDX_Control(pDX, IDC_CHECK_ENABLE_SIZE_ER, m_ctrl_chk_enable_size_err);
	DDX_Control(pDX, IDC_CHECK_ENABLE_DATA_ER, m_ctrl_chk_enable_data_err);
	DDX_Control(pDX, IDC_COMBO_APPLY_PKT_TYPE, m_ctrl_combo_apply_pkt_type);
	DDX_Control(pDX, IDC_CHECK_NEXT_ACK_NACK, m_ctrl_chk_appply_next_ack_nack);
	DDX_Control(pDX, IDC_CHECK_NEXT_READ_RESPONSE, m_ctrl_chk_apply_next_read_reply);
	DDX_Control(pDX, IDC_BUTTON_SET_ERRORS, m_ctrl_btn_set_errors);
	DDX_Control(pDX, IDC_COMBO_FILL_TYPE, m_ctrl_combo_fill_type);
	DDX_Control(pDX, IDC_COMBO_DATA_SIZE_ERRORS, m_ctrl_combo_data_size_errors);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_VALIDATION_DATETIME, m_ctrl_chk_validation_datetime);
	DDX_Control(pDX, IDC_COMBO_NUMBER_OF_BREAKS, m_ctrl_combo_number_breaks);
	DDX_Control(pDX, IDC_EDIT_NUM_BYTES, m_ctrl_edit_num_bytes);
	DDX_Control(pDX, IDC_CHECK_KEEP_RTP_ENABLED, m_ctrl_chk_keep_rts_enabled);
	DDX_Control(pDX, IDC_CHECK_DONT_ENABLE_RTS, m_ctrl_chk_dont_enable_rts);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_VALIDATION_CODE, m_ctrl_chk_corrupt_validation_code);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_TRAILER, m_ctrl_chk_corrupt_trailer);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_TERMINAL_ID, m_ctrl_chk_corrupt_terminal_id);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_HEADER, m_ctrl_chk_corrupt_header);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_DATA, m_ctrl_chk_corrupt_data);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_CRC, m_ctrl_chk_corrupt_crc);
	DDX_Control(pDX, IDC_CHECK_CORRUPT_COMMAND, m_ctrl_chk_corrupt_command);
	DDX_Control(pDX, IDC_EDIT_DATA_READ_RESPONSE_TIME, m_ctrl_edit_data_read_response_time);
	DDX_Control(pDX, IDC_EDIT_INTER_CHAR_TIME, m_ctrl_edit_inter_char_time);
	DDX_Control(pDX, IDC_EDIT_RTS_DISABLE_TIME, m_ctrl_edit_rts_disable_time);
	DDX_Control(pDX, IDC_EDIT_POLL_RESPONSE_TIME, m_ctrl_edit_poll_response_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCommsErrors, CDialog)
	//{{AFX_MSG_MAP(CDlgCommsErrors)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_KEEP_RTP_ENABLED, OnCheckKeepRtpEnabled)
	ON_BN_CLICKED(IDC_BUTTON_SET_ERRORS, OnButtonSetErrors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCommsErrors message handlers


BOOL CDlgCommsErrors::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctrl_edit_data_read_response_time.SetLimitText(2);
	m_ctrl_edit_inter_char_time.SetLimitText(3);
	m_ctrl_edit_rts_disable_time.SetLimitText(2);
	m_ctrl_edit_poll_response_time.SetLimitText(3);	
	m_ctrl_edit_num_bytes.SetLimitText(2);	

	//Must be in the same order as defined in EAction in dlg_comms_errors.h
	m_ctrl_combo_data_size_errors.AddString("None");
	m_ctrl_combo_data_size_errors.AddString("Add data");
	m_ctrl_combo_data_size_errors.AddString("Remove data");
	m_ctrl_combo_data_size_errors.AddString("Append data");
	m_ctrl_combo_data_size_errors.SetCurSel(0);

	//Must be in the same order as defined in EFillType in dlg_comms_errors.h
	m_ctrl_combo_fill_type.AddString("Zeros");
	m_ctrl_combo_fill_type.AddString("Random");
	m_ctrl_combo_fill_type.SetCurSel(0);

	m_ctrl_combo_number_breaks.AddString("0");
	m_ctrl_combo_number_breaks.AddString("1");
	m_ctrl_combo_number_breaks.AddString("2");
	m_ctrl_combo_number_breaks.AddString("3");
	m_ctrl_combo_number_breaks.AddString("4");
	m_ctrl_combo_number_breaks.SetCurSel(0);

	m_ctrl_combo_apply_pkt_type.AddString("Datetime Memsig");
	m_ctrl_combo_apply_pkt_type.AddString("Control");
	m_ctrl_combo_apply_pkt_type.AddString("Configuration");
	m_ctrl_combo_apply_pkt_type.AddString("Current Monitor");
	m_ctrl_combo_apply_pkt_type.AddString("Day end Monitor");
	m_ctrl_combo_apply_pkt_type.AddString("Event Log");
	m_ctrl_combo_apply_pkt_type.AddString("Transaction Rq");
	m_ctrl_combo_apply_pkt_type.AddString("Exception");
	m_ctrl_combo_apply_pkt_type.SetCurSel(0);
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		m_tooltip.AddTool( (CWnd*)(&m_ctrl_edit_poll_response_time), "Time delay from the end of poll command to the beginning of the response (milliseconds)");
		m_tooltip.AddTool( (CWnd*)(&m_ctrl_edit_inter_char_time),"The maximum pause in the data during transmission (milliseconds)");
		m_tooltip.AddTool( (CWnd*)(&m_ctrl_edit_rts_disable_time), "The delay in disabling the RTS (i.e. transmit control line) on completion of transmission (milliseconds)");
		m_tooltip.AddTool( (CWnd*)(&m_ctrl_edit_data_read_response_time), "Response time to requests other than polls (milliseconds)");
	}

	DisplayErrorsDataDefault();
	DisplayErrorsTimingDefault();
	DisplayErrorsSizeDefault();

	if(g_vlc_id_list->GetActiveId()==NULL) //should be
		m_ctrl_btn_set_errors.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCommsErrors::ProcessNotify(int p_id)
{
	if(p_id==CNotify::ACTIVE_ID_CHANGED)
		m_ctrl_btn_set_errors.EnableWindow(true);
}

void CDlgCommsErrors::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
/*
	if( bShow )
	{
		UpdateData(false);
	}
*/
}


/*
void CDlgCommsErrors::ProcessNotify( int p_id )
{
}
*/

BOOL CDlgCommsErrors::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}


void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow)
{  
	//Get rectangles  
	CRect rctComboBox, rctDropDown;  
	pMyComboBox->GetClientRect(&rctComboBox); //Combo rect  
	pMyComboBox->GetDroppedControlRect(&rctDropDown); //DropDownList rect  
	int itemHeight = pMyComboBox->GetItemHeight(-1); //Get Item height  
	pMyComboBox->GetParent()->ScreenToClient(&rctDropDown); //Converts coordinates  
	rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight*itemsToShow; //Set height  
	pMyComboBox->MoveWindow(&rctDropDown); //enable changes
}

void CDlgCommsErrors::OnCheckKeepRtpEnabled() 
{
/*	
	if(m_ctrl_chk_keep_rts_enabled.GetCheck()==1)
		//g_dlg_main->m_Comms.SetDTREnable(true);
		g_dlg_main->m_Comms.SetRTSEnable(true);
	else
		//g_dlg_main->m_Comms.SetDTREnable(false);
		g_dlg_main->m_Comms.SetRTSEnable(true);
*/
}

void CDlgCommsErrors::DisplayErrorsTiming(const CommsErrorsTiming& p_errors)
{
	if(p_errors.m_keep_rts_enabled)
		m_ctrl_chk_keep_rts_enabled.SetCheck(1);
	else
		m_ctrl_chk_keep_rts_enabled.SetCheck(0);


	if(p_errors.m_dont_enable_rts)
		m_ctrl_chk_dont_enable_rts.SetCheck(1);
	else
		m_ctrl_chk_dont_enable_rts.SetCheck(0);


	m_ctrl_edit_data_read_response_time.SetWindowText( Convert(p_errors.m_data_read_response_time));
	m_ctrl_edit_inter_char_time.SetWindowText(Convert(p_errors.m_interchar_time));	
	m_ctrl_edit_rts_disable_time.SetWindowText(Convert(p_errors.m_rts_disable_time));
	m_ctrl_edit_poll_response_time.SetWindowText(Convert(p_errors.m_poll_response_time));

	if((p_errors.m_num_packet_breaks>=0)&&(p_errors.m_num_packet_breaks<=4))
		m_ctrl_combo_number_breaks.SetCurSel(p_errors.m_num_packet_breaks);
}


void CDlgCommsErrors::DisplayErrorsData(const CommsErrorsCorruptData& p_errors)
{

	if(p_errors.m_header)
		m_ctrl_chk_corrupt_header.SetCheck(1);
	else
		m_ctrl_chk_corrupt_header.SetCheck(0);


	if(p_errors.m_terminal_id)
		m_ctrl_chk_corrupt_terminal_id.SetCheck(1);
	else
		m_ctrl_chk_corrupt_terminal_id.SetCheck(0);

	if(p_errors.m_n_bits)
		m_ctrl_chk_corrupt_n_bits.SetCheck(1);
	else
		m_ctrl_chk_corrupt_n_bits.SetCheck(0);


	if(p_errors.m_command)
		m_ctrl_chk_corrupt_command.SetCheck(1);
	else
		m_ctrl_chk_corrupt_command.SetCheck(0);


	if(p_errors.m_data)
		m_ctrl_chk_corrupt_data.SetCheck(1);
	else
		m_ctrl_chk_corrupt_data.SetCheck(0);


	if(p_errors.m_validation_datetime)
		m_ctrl_chk_validation_datetime.SetCheck(1);
	else
		m_ctrl_chk_validation_datetime.SetCheck(0);


	if(p_errors.m_validation_code)
		m_ctrl_chk_corrupt_validation_code.SetCheck(1);
	else
		m_ctrl_chk_corrupt_validation_code.SetCheck(0);


	if(p_errors.m_crc)
		m_ctrl_chk_corrupt_crc.SetCheck(1);
	else
		m_ctrl_chk_corrupt_crc.SetCheck(0);


	if(p_errors.m_trailer)
		m_ctrl_chk_corrupt_trailer.SetCheck(1);
	else
		m_ctrl_chk_corrupt_trailer.SetCheck(0);
}


void CDlgCommsErrors::DisplayErrorsDataSize(const CommsErrorsDataSize& p_errors)
{
	m_ctrl_edit_num_bytes.SetWindowText( Convert(p_errors.m_num_bytes) );
	m_ctrl_combo_fill_type.SetCurSel(p_errors.m_fill_type);
	m_ctrl_combo_data_size_errors.SetCurSel(p_errors.m_action);
}

void CDlgCommsErrors::DisplayErrorsDataDefault()
{
	m_ctrl_chk_corrupt_header.SetCheck(0);
	m_ctrl_chk_corrupt_terminal_id.SetCheck(0);
	m_ctrl_chk_corrupt_command.SetCheck(0);
	m_ctrl_chk_corrupt_data.SetCheck(0);
	m_ctrl_chk_validation_datetime.SetCheck(0);
	m_ctrl_chk_corrupt_validation_code.SetCheck(0);
	m_ctrl_chk_corrupt_crc.SetCheck(0);
	m_ctrl_chk_corrupt_trailer.SetCheck(0);
}
void CDlgCommsErrors::DisplayErrorsTimingDefault()
{
	m_ctrl_chk_keep_rts_enabled.SetCheck(0);
	m_ctrl_chk_dont_enable_rts.SetCheck(0);
	m_ctrl_edit_data_read_response_time.SetWindowText(CString("0"));
	m_ctrl_edit_inter_char_time.SetWindowText(CString("0"));	
	m_ctrl_edit_rts_disable_time.SetWindowText(CString("0"));
	m_ctrl_edit_poll_response_time.SetWindowText(CString("0"));
	m_ctrl_combo_number_breaks.SetCurSel(0);
}

void CDlgCommsErrors::DisplayErrorsSizeDefault()
{
	m_ctrl_edit_num_bytes.SetWindowText(CString("0"));
	m_ctrl_combo_fill_type.SetCurSel(CommsErrorsDataSize::ZERO);
	m_ctrl_combo_data_size_errors.SetCurSel(CommsErrorsDataSize::NONE);
}


void CDlgCommsErrors::SetErrorsTiming( CommsErrorsTiming& p_errors)
{
	if(m_ctrl_chk_keep_rts_enabled.GetCheck()==1)
		p_errors.m_keep_rts_enabled = true;
	else
		p_errors.m_keep_rts_enabled = false;

	if(m_ctrl_chk_dont_enable_rts.GetCheck()==1)
		p_errors.m_dont_enable_rts = true;
	else
		p_errors.m_dont_enable_rts = false;

	CString str;
	m_ctrl_edit_data_read_response_time.GetWindowText(str);
	p_errors.m_data_read_response_time = Convert(str);

	m_ctrl_edit_inter_char_time.GetWindowText(str);
	p_errors.m_interchar_time = Convert(str);

	m_ctrl_edit_rts_disable_time.GetWindowText(str);
	p_errors.m_rts_disable_time = Convert(str);

	m_ctrl_edit_poll_response_time.GetWindowText(str);
	p_errors.m_poll_response_time = Convert(str);

	p_errors.m_num_packet_breaks = m_ctrl_combo_number_breaks.GetCurSel();
}


void CDlgCommsErrors::SetErrorsData( CommsErrorsCorruptData& p_errors)
{
	if(m_ctrl_chk_corrupt_header.GetCheck()==1)
		p_errors.m_header = true;
	else
		p_errors.m_header = false;


	if(m_ctrl_chk_corrupt_terminal_id.GetCheck()==1)
		p_errors.m_terminal_id = true;
	else
		p_errors.m_terminal_id = false;

	if(m_ctrl_chk_corrupt_n_bits.GetCheck())
		p_errors.m_n_bits = true;
	else
		p_errors.m_n_bits = false;

	if(m_ctrl_chk_corrupt_command.GetCheck()==1)
		p_errors.m_command = true;
	else
		p_errors.m_command = false;


	if(m_ctrl_chk_corrupt_data.GetCheck()==1)
		p_errors.m_data = true;
	else
		p_errors.m_data = false;


	if(m_ctrl_chk_validation_datetime.GetCheck()==1)
		p_errors.m_validation_datetime = true;
	else
		p_errors.m_validation_datetime = false;


	if(m_ctrl_chk_corrupt_validation_code.GetCheck()==1)
		p_errors.m_validation_code = true;
	else
		p_errors.m_validation_code = false;


	if(m_ctrl_chk_corrupt_crc.GetCheck()==1)
		p_errors.m_crc = true;
	else
		p_errors.m_crc = false;

	if(m_ctrl_chk_corrupt_trailer.GetCheck()==1)
		p_errors.m_trailer = true;
	else
		p_errors.m_trailer = false;
	
}

void CDlgCommsErrors::SetErrorsDataSize(CommsErrorsDataSize& p_errors)
{
	CString str;
	m_ctrl_edit_num_bytes.GetWindowText(str);
	p_errors.m_num_bytes = Convert(str);	

	p_errors.m_fill_type = (CommsErrorsDataSize::EFillType)m_ctrl_combo_fill_type.GetCurSel();
	p_errors.m_action = (CommsErrorsDataSize::EAction)m_ctrl_combo_data_size_errors.GetCurSel();
}

void CDlgCommsErrors::OnButtonSetErrors() 
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL) //shouldn't be
		return;
	CString id_str;
	active_id->GetId(id_str);

	CommsErrorsTiming* timing_errors = NULL;
	CommsErrorsCorruptData* data_errors = NULL;
	CommsErrorsDataSize* size_errors = NULL;
	
	CString set_clear_str = CString("Cleared ");
	if(m_ctrl_chk_enable_timing_err.GetCheck()==1)
	{
		timing_errors = new CommsErrorsTiming();
		SetErrorsTiming(*timing_errors);
		set_clear_str = CString("Set ");
	}
	if(m_ctrl_chk_enable_data_err.GetCheck()==1)
	{
		data_errors = new CommsErrorsCorruptData();
		SetErrorsData(*data_errors);
		set_clear_str = CString("Set ");
	}
	if(m_ctrl_chk_enable_size_err.GetCheck()==1)
	{
		size_errors = new CommsErrorsDataSize();
		SetErrorsDataSize(*size_errors);
		set_clear_str = CString("Set ");
	}

	CString pkt_type_str = CString("");
	uint apply_pkt_type = m_ctrl_combo_apply_pkt_type.GetCurSel();
	switch(apply_pkt_type)
	{
		case PKT_TYPE_DATETIME:		pkt_type_str=CString("Datetime Memsig"); break;
		case PKT_TYPE_CONTROL:		pkt_type_str=CString("Control"); break;
		case PKT_TYPE_CONFIGURATION:pkt_type_str=CString("Configuration"); break;
		case PKT_TYPE_CUR_MON:		pkt_type_str=CString("Current Monitor"); break;
		case PKT_TYPE_DAY_END_MON:	pkt_type_str=CString("Day End Monitor");break;
		case PKT_TYPE_EVENT_LOG:	pkt_type_str=CString("Event Log");break;
		case PKT_TYPE_TRANSACTION:	pkt_type_str=CString("Transaction");break;
		case PKT_TYPE_EXCEPTION:	pkt_type_str=CString("Status Response");break;
		default: return;
	};

	int apply_count=0;
	if(m_ctrl_chk_apply_next_read_reply.GetCheck()==1)
	{
		apply_count++;
		switch(apply_pkt_type)
		{
		case PKT_TYPE_DATETIME:		active_id->GetDatetimeMSData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_CONTROL:		active_id->GetControlData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_CONFIGURATION:active_id->GetConfigurationData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_CUR_MON:		active_id->GetMonitorCurData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_DAY_END_MON:	active_id->GetMonitorEndData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_EVENT_LOG:	active_id->GetEventLogData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_TRANSACTION:	active_id->GetTransactionData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		//case PKT_TYPE_EXCEPTION:	//pkt_type_str=Cstring("Datetime Memsig") active_id->GetDatetimeMSData()->SetErrorsReadReply(timing_errors,data_errors,size_errors);break;
		//default: return;
		};
	}
	if(m_ctrl_chk_appply_next_ack_nack.GetCheck()==1)
	{
		apply_count++;
		switch(apply_pkt_type)
		{
		case PKT_TYPE_DATETIME:		active_id->GetDatetimeMSData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_CONTROL:		active_id->GetControlData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_CONFIGURATION:active_id->GetConfigurationData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_CUR_MON:		active_id->GetMonitorCurData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_DAY_END_MON:	active_id->GetMonitorEndData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_EVENT_LOG:	active_id->GetEventLogData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		case PKT_TYPE_TRANSACTION:	active_id->GetTransactionData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		//case PKT_TYPE_EXCEPTION:	//active_id->GetDatetimeMSData()->SetErrorsAckNack(timing_errors,data_errors,size_errors);break;
		//default: return;
		};
	}

	if(apply_pkt_type == PKT_TYPE_EXCEPTION)
		active_id->SetErrorsStatusResponse(timing_errors, data_errors, size_errors);

	if((apply_count==0)&&(apply_pkt_type!=PKT_TYPE_EXCEPTION) )
	{
		MessageBox("Select Ack/Nack or Read Reply or both");
		return;
	}

	CString msg = CString("Errors ");
	msg += set_clear_str;
	msg += CString("for ID ");
	msg += id_str;
	msg += CString(": ");
	msg += pkt_type_str;

	if(apply_pkt_type!=PKT_TYPE_EXCEPTION)
	{
		msg += CString(" - ");
		if(m_ctrl_chk_apply_next_read_reply.GetCheck()==1)
			msg += CString("Next Read Reply");
		if(apply_count==2)
			msg += CString(", ");
		if(m_ctrl_chk_appply_next_ack_nack.GetCheck()==1)
			msg += CString("Next Ack Nack Response");	
	}
	MessageBox(msg);
}
