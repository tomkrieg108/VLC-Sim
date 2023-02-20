// broadcast_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "vlc_receive_thread.h"
#include "dlg_datetime_memsig.h"
#include "utilities.h"
#include "vlc_packet.h"
#include "vlc_id.h"
#include "vlc_id_list.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDatetimeMemsig dialog


/////////////////////////

// global access to dialog
CDlgDatetimeMemsig *g_dlg_datetime_memsig = 0;


/////////////////////////


CDlgDatetimeMemsig::CDlgDatetimeMemsig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDatetimeMemsig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDatetimeMemsig)
	//}}AFX_DATA_INIT
	g_notify_mgr->Register(CNotify::ACTIVE_ID_CHANGED,this);
}


void CDlgDatetimeMemsig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDatetimeMemsig)
	DDX_Control(pDX, IDC_CHK_USE_CURRENT_DATETIME, m_use_current_datetime);
	DDX_Control(pDX, IDC_XLISTCTRL_DATETIME_MEMSIG_PKT, m_xlistctrl_datetime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDatetimeMemsig, CDialog)
	//{{AFX_MSG_MAP(CDlgDatetimeMemsig)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDatetimeMemsig message handlers


//////////////////////////////////////////////////////////////////////////////

void CDlgDatetimeMemsig::ProcessNotify( int p_id )
{
	if(p_id == CNotify::ACTIVE_ID_CHANGED)
	{
		m_xlistctrl_datetime.EnableWindow(true);
		m_use_current_datetime.EnableWindow(true);
		//OnBtnView();			
	}
}

BOOL CDlgDatetimeMemsig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_use_current_datetime.SetCheck(1);
/////////////////////////////////////////////////////////////////
// set up dlg for date time packet data 
//////////////////////////////////////////////////////////////////
// XListCtrl must have LVS_EX_FULLROWSELECT if combo or edit boxes are used
	

	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_datetime.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	CRect rect;
	m_xlistctrl_datetime.GetWindowRect(&rect);
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
	m_xlistctrl_datetime.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_datetime.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_datetime.InsertColumn(2, &lvcolumn);
	

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	m_xlistctrl_datetime.SetColumn(0,&lvcolumn);

	m_xlistctrl_datetime.LockWindowUpdate();	// ***** lock window updates while filling list *****
	m_xlistctrl_datetime.DeleteAllItems();


	//Note: Using 0 instead of idx adds it to the start of the list

	//byte 1
	m_xlistctrl_datetime.InsertItem(0, "1");
	m_xlistctrl_datetime.SetItemText(0,1,"Format ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_datetime.SetItemText(0,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_datetime.SetEdit(0,2, CXEdit::HEX,2);

	//byte 2-8	
	m_xlistctrl_datetime.InsertItem(1, "2-8");
	m_xlistctrl_datetime.SetItemText(1,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_datetime.SetItemText(1,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_datetime.SetEdit(1,2, CXEdit::HEX,14);
	
	//byte 9-10	
	m_xlistctrl_datetime.InsertItem(2, "9-10");
	m_xlistctrl_datetime.SetItemText(2,1,"Date(U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_datetime.SetItemText(2,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_datetime.SetEdit(2,2, CXEdit::DEC,6);

	//byte 11-12	
	m_xlistctrl_datetime.InsertItem(3, "11-12");
	m_xlistctrl_datetime.SetItemText(3,1,"Time(U)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_datetime.SetItemText(3,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_datetime.SetEdit(3,2, CXEdit::DEC,6);
	
	//byte 13-14	
	m_xlistctrl_datetime.InsertItem(4, "13-14");
	m_xlistctrl_datetime.SetItemText(4,1,"Signature Value",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_datetime.SetItemText(4,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_datetime.SetEdit(4,2, CXEdit::HEX,4);

	//byte 15-16	
	m_xlistctrl_datetime.InsertItem(5, "15-16");
	m_xlistctrl_datetime.SetItemText(5,1,"Signature Divisor",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_datetime.SetItemText(5,2,"",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_datetime.SetEdit(5,2, CXEdit::HEX,4);
	
	m_xlistctrl_datetime.UnlockWindowUpdate();	// ***** unlock window updates *****


/////////////////////////////////////////////////////////////////
// date time packet list 
//////////////////////////////////////////////////////////////////

/*
	m_xlistctrl_datetime_list.GetWindowRect(&rect);
	width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar


	// add columns
	lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "ID";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/5;
	m_xlistctrl_datetime_list.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Send/Rcve";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width/5;
	m_xlistctrl_datetime_list.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Date";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width/5;
	m_xlistctrl_datetime_list.InsertColumn(2, &lvcolumn);

	lvcolumn.pszText = "Time";
	lvcolumn.iSubItem = 3;
	lvcolumn.cx = width/5;
	m_xlistctrl_datetime_list.InsertColumn(3, &lvcolumn);

	lvcolumn.pszText = "Ack/Nack";
	lvcolumn.iSubItem = 4;
	lvcolumn.cx = width/5;
	m_xlistctrl_datetime_list.InsertColumn(4, &lvcolumn);
	

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	m_xlistctrl_datetime_list.SetColumn(0,&lvcolumn);

	//m_xlistctrl_datetime_list.LockWindowUpdate();	// ***** lock window updates while filling list *****
	m_xlistctrl_datetime_list.DeleteAllItems();
*/

	m_xlistctrl_datetime.EnableWindow(false);
	m_use_current_datetime.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Implement IDlgPacket
////////////////////////////////////////////////////////////////////////////////////
/*
Display the packet in the dialog m_xlistctrl_datetime
*/

void CDlgDatetimeMemsig::DisplayPacket(const CVLCPacket& p_packet )
{
	//if(p_packet.GetLength() != CVLCPacket::LENGTH_DATETIME)
	//	return;
	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_DATETIME_MS)
		return;

	
	CString value;

	m_xlistctrl_datetime.LockWindowUpdate();

	p_packet.GetDataFieldFormatted(value, CVLCPacket::DT_FORMAT_ID);
	m_xlistctrl_datetime.SetItemText(0,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
	
	p_packet.GetDataFieldFormatted(value, CVLCPacket::DT_RESERVED2);
	m_xlistctrl_datetime.SetItemText(1,2,value,RGB(0,0,0), RGB(255,255,255)) ; 

	p_packet.GetDataFieldFormatted(value, CVLCPacket::DT_DATE);
	m_xlistctrl_datetime.SetItemText(2,2,value,RGB(0,0,0), RGB(255,255,255)) ; 

	p_packet.GetDataFieldFormatted(value, CVLCPacket::DT_TIME);
	m_xlistctrl_datetime.SetItemText(3,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
	
	p_packet.GetDataFieldFormatted(value, CVLCPacket::DT_SIG_VALUE);
	m_xlistctrl_datetime.SetItemText(4,2,value,RGB(0,0,0), RGB(255,255,255)) ; 

	p_packet.GetDataFieldFormatted(value, CVLCPacket::DT_SIG_DIVISOR);
	m_xlistctrl_datetime.SetItemText(5,2,value,RGB(0,0,0), RGB(255,255,255)) ; 

	m_xlistctrl_datetime.UnlockWindowUpdate();

	bool use_cur_datetime = false;
	CVlcDatetimeMSData* dtms_data = (CVlcDatetimeMSData*)(GetVlcData());
	if(dtms_data != NULL)
			use_cur_datetime = dtms_data->GetUseCurrentDateTime();
	
	if(use_cur_datetime)
		m_use_current_datetime.SetCheck(1);
	else
		m_use_current_datetime.SetCheck(0);

}


////////////////////////////////////////////////////////////////////////////////////
/*
Set the packet in the dialog m_xlistctrl_datetime
*/

void CDlgDatetimeMemsig::SetPacket(CVLCPacket& p_packet )
{
	//if(p_packet.GetLength() != CVLCPacket::LENGTH_DATETIME)
	//	return;

	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_DATETIME_MS)
		return;

	bool use_cur_datetime = (m_use_current_datetime.GetCheck()==1);
	CVlcDatetimeMSData* dtms_data = (CVlcDatetimeMSData*)(GetVlcData());
	if(dtms_data != NULL)
			dtms_data->SetUseCurrentDateTime(use_cur_datetime);
	else 
		return;

	CString value;

	value = m_xlistctrl_datetime.GetItemText(0,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_FORMAT_ID);

	value = m_xlistctrl_datetime.GetItemText(1,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_RESERVED2);

	if(use_cur_datetime)
	{
		p_packet.SetDataFieldInt(0,CVLCPacket::DT_DATE);
		p_packet.SetDataFieldInt(0,CVLCPacket::DT_TIME);
		/*
		CVLCDateTime date_time; //this is the universal date time
		uint date = (uint)date_time.GetJulianDate();
		uint time = (uint)date_time.GetTimeInSeconds();
		p_packet.SetDataFieldInt(date,CVLCPacket::DT_DATE);
		p_packet.SetDataFieldInt(time,CVLCPacket::DT_TIME);
		*/
	}
	else
	{
		value = m_xlistctrl_datetime.GetItemText(2,2);
		p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_DATE);

		value = m_xlistctrl_datetime.GetItemText(3,2);
		p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_TIME);
	}

	value = m_xlistctrl_datetime.GetItemText(4,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_SIG_VALUE);

	value = m_xlistctrl_datetime.GetItemText(5,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_SIG_DIVISOR);

	dtms_data->UpdateDefault();
}


CVlcData* CDlgDatetimeMemsig::GetVlcData()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return NULL;

	return active_id->GetDatetimeMSData();
}


void CDlgDatetimeMemsig::GetPacketName(CString& p_packet_name)
{
	p_packet_name = "Date Time / Memory Signature";
}
