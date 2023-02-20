

#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "dlg_transactions.h"
#include "vlc_id.h"
#include "vlc_id_list.h"
#include "notify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgTransactions* g_dlg_transactions = 0;

/////////////////////////////////////////////////////////////////////////////
// CDlgTransactions dialog

CDlgTransactions::CDlgTransactions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTransactions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTransactions)
	//}}AFX_DATA_INIT
	g_notify_mgr->Register( CNotify::ACTIVE_ID_CHANGED, this );
}


void CDlgTransactions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTransactions)
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ctrl_btn_send);
	DDX_Control(pDX, IDC_LISTCTRL_TRANS_REQUEST, m_xlistctrl_transaction_request);
	DDX_Control(pDX, IDC_LISTCTRL_TRANS_REPLY, m_xlistctrl_transaction_reply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTransactions, CDialog)
	//{{AFX_MSG_MAP(CDlgTransactions)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BTN_SEND_ALL_IDS, OnBtnSendAllIds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDlgTransactions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetupColumns();
	SetupRows();
	SetControlEnableStatus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTransactions::SetupColumns()
{
	// add columns for transaction request
	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_transaction_request.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	CRect rect;
	m_xlistctrl_transaction_request.GetWindowRect(&rect);
	int width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar

	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "Bytes";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/6;
	m_xlistctrl_transaction_request.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_transaction_request.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_transaction_request.InsertColumn(2, &lvcolumn);

	m_xlistctrl_transaction_request.LockWindowUpdate();	
	m_xlistctrl_transaction_request.DeleteAllItems();
	m_xlistctrl_transaction_request.LockWindowUpdate();	


	// add columns for transaction reply
	m_xlistctrl_transaction_reply.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	m_xlistctrl_transaction_reply.GetWindowRect(&rect);
	width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar

	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "Bytes";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/6;
	m_xlistctrl_transaction_reply.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_transaction_reply.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_transaction_reply.InsertColumn(2, &lvcolumn);

	//lvcolumn.pszText = "Value";
	//lvcolumn.iSubItem = 2;
	//lvcolumn.cx = width*2/6;
	//m_xlistctrl_transaction_reply.InsertColumn(2, &lvcolumn);

	m_xlistctrl_transaction_reply.LockWindowUpdate();	
	m_xlistctrl_transaction_reply.DeleteAllItems();
	m_xlistctrl_transaction_reply.UnlockWindowUpdate();	
}


void CDlgTransactions::SetupRows()
{
	m_xlistctrl_transaction_reply.LockWindowUpdate();	
	//Request
	m_xlistctrl_transaction_request.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_request.SetItemText(0,1,"Format ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_request.SetItemText(0,2,"05",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_transaction_request.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_transaction_request.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_request.SetItemText(1,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_request.SetItemText(1,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_transaction_request.SetEdit    (1,2, CXEdit::HEX,2);

	m_xlistctrl_transaction_request.InsertItem (2, "3",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_request.SetItemText(2,1,"Transaction Number",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_request.SetItemText(2,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_transaction_request.SetEdit    (2,2, CXEdit::DEC,3);

	m_xlistctrl_transaction_request.InsertItem (3, "4",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_request.SetItemText(3,1,"Cashout Type",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_request.SetItemText(3,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_transaction_request.SetEdit    (3,2, CXEdit::HEX,2);

	m_xlistctrl_transaction_request.InsertItem (4, "5-8",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_request.SetItemText(4,1,"Amount (cents)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_request.SetItemText(4,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_transaction_request.SetEdit    (4,2, CXEdit::DEC,9);
	

	//Reply
	
	m_xlistctrl_transaction_reply.InsertItem (0, "1",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(0,1,"Format ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(0,2,"05",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (0,2, CXEdit::HEX,2);

	m_xlistctrl_transaction_reply.InsertItem (1, "2",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(1,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(1,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (1,2, CXEdit::HEX,2);

	m_xlistctrl_transaction_reply.InsertItem (2, "3",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(2,1,"Transaction Number",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(2,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (2,2, CXEdit::DEC,3);

	m_xlistctrl_transaction_reply.InsertItem (3, "4-6",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(3,1,"Venue ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(3,2,"0",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (3,2, CXEdit::DEC,8);	

	m_xlistctrl_transaction_reply.InsertItem (4, "7-10",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(4,1,"Amount (cents)",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(4,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (4,2, CXEdit::DEC,9);

	m_xlistctrl_transaction_reply.InsertItem (5, "11-12",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(5,1,"Date of Issue",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(5,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (5,2, CXEdit::DEC,9);

	m_xlistctrl_transaction_reply.InsertItem (6, "13-14",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(6,1,"Time of Issue",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(6,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (6,2, CXEdit::DEC,9);

	m_xlistctrl_transaction_reply.InsertItem (7, "15-16",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(7,1,"Cashout Number",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(7,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (7,2, CXEdit::HEX,2);

	m_xlistctrl_transaction_reply.InsertItem (8, "17-18",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(8,1,"Security Number",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(8,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (8,2, CXEdit::HEX,2);	

	m_xlistctrl_transaction_reply.InsertItem (9, "19-24",RGB(0,0,255),RGB(255,255,255));
	m_xlistctrl_transaction_reply.SetItemText(9,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_transaction_reply.SetItemText(9,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	//m_xlistctrl_transaction_reply.SetEdit    (10,2, CXEdit::HEX,2);
	m_xlistctrl_transaction_reply.UnlockWindowUpdate();	
}

void CDlgTransactions::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}
////////////////////////////////////////////////////
//Implement INotifiable
void CDlgTransactions::ProcessNotify(int p_id)  
{
	if(p_id==CNotify::ACTIVE_ID_CHANGED)
		SetControlEnableStatus();
}

//////////////////////////////////////////////////////////////////
//Implement IDlgPacket
/////////////////////////////////////////////////////////////////////////////
inline void CDlgTransactions::DisplayField(const CVLCPacket& p_packet, CXListCtrl& p_list, int p_field_pos, int p_field_id, int p_offset)
{
	//p_offset is the offset to the start of the section.  e.g. system section offset is 8 if there is 1 game key etc
	CString value;
	p_packet.GetDataFieldFormatted(value, p_field_id, p_offset );
	p_list.SetItemText(p_field_pos,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
}
inline void CDlgTransactions::SetField(CVLCPacket& p_packet, CXListCtrl& p_list, int p_field_pos, int p_field_id, int p_offset)
{
	CString value;
	value = p_list.GetItemText(p_field_pos,2);
	p_packet.SetDataFieldFormatted(value, p_field_id, p_offset);
}

void CDlgTransactions::DisplayPacket(const CVLCPacket& p_packet )
{
	CVLCPacket::EDataType packet_type = p_packet.GetDataType();

	if(packet_type == CVLCPacket::DATA_TYPE_TRANSACTION_REQUEST)
	{
		m_xlistctrl_transaction_request.LockWindowUpdate();	
		DisplayField(p_packet,m_xlistctrl_transaction_request,0, CVLCPacket::TRANS_RQ_FORMAT_ID);
		DisplayField(p_packet,m_xlistctrl_transaction_request,1, CVLCPacket::TRANS_RQ_RESERVED2);
		DisplayField(p_packet,m_xlistctrl_transaction_request,2, CVLCPacket::TRANS_RQ_TRANS_NUMBER);
		DisplayField(p_packet,m_xlistctrl_transaction_request,3, CVLCPacket::TRANS_RQ_CASHOUT_TYPE);
		DisplayField(p_packet,m_xlistctrl_transaction_request,4, CVLCPacket::TRANS_RQ_CASHOUT_AMOUNT);
		m_xlistctrl_transaction_request.UnlockWindowUpdate();	
	}

	if(packet_type == CVLCPacket::DATA_TYPE_TRANSACTION_REPLY)
	{
		m_xlistctrl_transaction_request.LockWindowUpdate();	
		DisplayField(p_packet,m_xlistctrl_transaction_reply,0, CVLCPacket::TRANS_REPLY_FORMAT_ID);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,1, CVLCPacket::TRANS_REPLY_RESERVED2);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,2, CVLCPacket::TRANS_REPLY_TRANS_NUMBER);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,3, CVLCPacket::TRANS_REPLY_TRANS_NUMBER);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,4, CVLCPacket::TRANS_REPLY_CASHOUT_AMOUNT);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,5, CVLCPacket::TRANS_REPLY_DATE_OF_ISSUE);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,6, CVLCPacket::TRANS_REPLY_TIME_OF_ISSUE);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,7, CVLCPacket::TRANS_REPLY_CASHOUT_NUMBER);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,8, CVLCPacket::TRANS_REPLY_SECURITY_NUMBER);
		DisplayField(p_packet,m_xlistctrl_transaction_reply,9, CVLCPacket::TRANS_REPLY_RESERVED19);
		m_xlistctrl_transaction_request.UnlockWindowUpdate();	
	}
}
////////////////////////////////////////////////////////////////////////////////////

void CDlgTransactions::SetPacket(CVLCPacket& p_packet )
{
	CVLCPacket::EDataType packet_type = p_packet.GetDataType();

	if(packet_type == CVLCPacket::DATA_TYPE_TRANSACTION_REQUEST)
	{
		m_xlistctrl_transaction_request.LockWindowUpdate();	
		SetField(p_packet,m_xlistctrl_transaction_request,0, CVLCPacket::TRANS_RQ_FORMAT_ID);
		SetField(p_packet,m_xlistctrl_transaction_request,1, CVLCPacket::TRANS_RQ_RESERVED2);
		SetField(p_packet,m_xlistctrl_transaction_request,2, CVLCPacket::TRANS_RQ_TRANS_NUMBER);
		SetField(p_packet,m_xlistctrl_transaction_request,3, CVLCPacket::TRANS_RQ_CASHOUT_TYPE);
		SetField(p_packet,m_xlistctrl_transaction_request,4, CVLCPacket::TRANS_RQ_CASHOUT_AMOUNT);
	}	
}

////////////////////////////////////////////////////////////////////////////////////////

CVlcData* CDlgTransactions::GetVlcData()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return NULL;
	return active_id->GetTransactionData();
}

void CDlgTransactions::GetPacketName(CString& p_packet_name)
{
	p_packet_name = CString("Transaction");
}
////////////////////////////////////////////////////////////////////////////////////////

void CDlgTransactions::OnButtonSend() 
{
	CVlcTransactionData* trans_data = (CVlcTransactionData*)(GetVlcData());
	if(trans_data != NULL)
		trans_data->SendTransactionRequest();
}

void CDlgTransactions::OnBtnSendAllIds() 
{
	CVlcIdList::Id_Iterator id_itr;
	g_vlc_id_list->GetIterator(id_itr);
	int count = 0;
	while(!g_vlc_id_list->AtEnd(id_itr) )
	{
		if(count>3)
			break;
		CVlcId* id = (*id_itr);
		CVlcTransactionData* trans_data = (CVlcTransactionData*)(id->GetTransactionData());
		trans_data->SendTransactionRequest();
		id_itr++;
		count++;
	}
}

void CDlgTransactions::SetControlEnableStatus()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
	{
		m_ctrl_btn_send.EnableWindow(false);
		m_xlistctrl_transaction_request.EnableWindow(false);
	}
	else
	{
		m_ctrl_btn_send.EnableWindow(true);
		m_xlistctrl_transaction_request.EnableWindow(true);
	}
}


