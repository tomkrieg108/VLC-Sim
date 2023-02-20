

#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_control.h"
#include "vlc_id.h"
#include "vlc_id_list.h"
#include "program_options_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgControl dialog

CDlgControl* g_dlg_control = 0;


CDlgControl::CDlgControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgControl)
	//}}AFX_DATA_INIT
}


void CDlgControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgControl)
	DDX_Control(pDX, IDC_XLISTCTRL_CONTROL_PKT, m_xlistctrl_control_packet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgControl, CDialog)
	//{{AFX_MSG_MAP(CDlgControl)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgControl message handlers

BOOL CDlgControl::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CDlgControl::OnInitDialog()
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


/////////////////////////////////////////////////////////////////
// set up dlg for control packet data 
//////////////////////////////////////////////////////////////////
// XListCtrl must have LVS_EX_FULLROWSELECT if combo or edit boxes are used
	

	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_control_packet.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);
	CRect rect;
	m_xlistctrl_control_packet.GetWindowRect(&rect);
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
	m_xlistctrl_control_packet.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*3/6;
	m_xlistctrl_control_packet.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Value";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*2/6;
	m_xlistctrl_control_packet.InsertColumn(2, &lvcolumn);

	m_xlistctrl_control_packet.LockWindowUpdate();	// ***** lock window updates while filling list *****
	m_xlistctrl_control_packet.DeleteAllItems();
	//Note: Using 0 instead of idx adds it to the start of the list

	
	m_xlistctrl_control_packet.InsertItem(FORMAT_ID, "1");
	m_xlistctrl_control_packet.SetItemText(FORMAT_ID,1,"Format ID",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(FORMAT_ID,2,"05",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(FORMAT_ID,2, CXEdit::HEX,2);

	m_xlistctrl_control_packet.InsertItem(RESERVED2, "2");
	m_xlistctrl_control_packet.SetItemText(RESERVED2,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(RESERVED2,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(RESERVED2,2, CXEdit::HEX,2);

	m_xlistctrl_control_packet.InsertItem(CTRL_VERS_NO, "3");
	m_xlistctrl_control_packet.SetItemText(CTRL_VERS_NO,1,"Ctrl Vers No.",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(CTRL_VERS_NO,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(CTRL_VERS_NO,2, CXEdit::HEX,2);

	m_xlistctrl_control_packet.InsertItem(RESERVED4, "4");
	m_xlistctrl_control_packet.SetItemText(RESERVED4,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(RESERVED4,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(RESERVED4,2, CXEdit::HEX,2);

	m_xlistctrl_control_packet.InsertItem(TERMINAL_MODE, "5");
	m_xlistctrl_control_packet.SetItemText(TERMINAL_MODE,1,"Terminal Mode",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(TERMINAL_MODE,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(TERMINAL_MODE,2, CXEdit::HEX,2);	

	m_xlistctrl_control_packet.InsertItem(RESERVED6, "6");
	m_xlistctrl_control_packet.SetItemText(RESERVED6,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(RESERVED6,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(RESERVED6,2, CXEdit::HEX,2);	

	m_xlistctrl_control_packet.InsertItem(DAILY_POLL_TIMEOUT, "7");
	m_xlistctrl_control_packet.SetItemText(DAILY_POLL_TIMEOUT,1,"Daly Poll Timeout",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(DAILY_POLL_TIMEOUT,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(DAILY_POLL_TIMEOUT,2, CXEdit::DEC,2);	

	m_xlistctrl_control_packet.InsertItem(HOLIDAY_FLAGS, "8");
	m_xlistctrl_control_packet.SetItemText(HOLIDAY_FLAGS,1,"Holiday Flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(HOLIDAY_FLAGS,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(HOLIDAY_FLAGS,2, CXEdit::DEC,2);	

	m_xlistctrl_control_packet.InsertItem(MEM_SIG_DIV, "9-10");
	m_xlistctrl_control_packet.SetItemText(MEM_SIG_DIV,1,"Mem Sig Divisor",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(MEM_SIG_DIV,2,"0000",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(MEM_SIG_DIV,2, CXEdit::HEX,4);
	
	m_xlistctrl_control_packet.InsertItem(RESERVED11, "11");
	m_xlistctrl_control_packet.SetItemText(RESERVED11,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(RESERVED11,2,"00",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(RESERVED11,2, CXEdit::HEX,2);	

	m_xlistctrl_control_packet.InsertItem(FROM, "12-14");
	m_xlistctrl_control_packet.SetItemText(FROM,1,"From Ptr",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(FROM,2,"000000",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(FROM,2, CXEdit::HEX,6);

	m_xlistctrl_control_packet.InsertItem(GAME_KEY_ENABLE_FLAGS, "15-22");
	m_xlistctrl_control_packet.SetItemText(GAME_KEY_ENABLE_FLAGS,1,"Game key enable flags",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(GAME_KEY_ENABLE_FLAGS,2,"000000",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(GAME_KEY_ENABLE_FLAGS,2, CXEdit::HEX,16);
	
	m_xlistctrl_control_packet.InsertItem(RESERVED23, "23-26");
	m_xlistctrl_control_packet.SetItemText(RESERVED23,1,"Reserved",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(RESERVED23,2,"000000",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(RESERVED23,2, CXEdit::HEX,8);

	m_xlistctrl_control_packet.InsertItem(VARIABLE_BANNER_DATA, "27-");
	m_xlistctrl_control_packet.SetItemText(VARIABLE_BANNER_DATA,1,"Variable Banner Data",RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_control_packet.SetItemText(VARIABLE_BANNER_DATA,2,"000000",RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_control_packet.SetEdit(VARIABLE_BANNER_DATA,2, CXEdit::HEX,256);

	m_xlistctrl_control_packet.UnlockWindowUpdate();	// ***** unlock window updates *****

	return TRUE;	// CG: This was added by the ToolTips component.

}

int CDlgControl::serialize(CArchive *pArchive)
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




void CDlgControl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
	{
		UpdateData(false);
	}
	else
	{
		UpdateData(false);
	}
}

//////////////////////////////////////////////////////////////////
//Implement IDlgPacket

void CDlgControl::DisplayPacket(const CVLCPacket& p_packet )
{
	CString value;

	m_xlistctrl_control_packet.LockWindowUpdate();

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CRTL_FORMAT_ID);
	m_xlistctrl_control_packet.SetItemText(FORMAT_ID,2,value,RGB(0,0,0), RGB(255,255,255)) ; 
	
	p_packet.GetDataFieldFormatted(value, CVLCPacket::CRTL_RESERVED2);
	m_xlistctrl_control_packet.SetItemText(RESERVED2,2,value,RGB(0,0,0), RGB(255,255,255)) ;	

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CRTL_VERS);
	m_xlistctrl_control_packet.SetItemText(CTRL_VERS_NO,2,value,RGB(0,0,0), RGB(255,255,255)) ;	

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CRTL_RESERVED4);
	m_xlistctrl_control_packet.SetItemText(RESERVED4,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_TERMINAL_MODE);
	m_xlistctrl_control_packet.SetItemText(TERMINAL_MODE,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CRTL_RESERVED6);
	m_xlistctrl_control_packet.SetItemText(RESERVED6,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_DALY_POLL_TIMEOUT);
	m_xlistctrl_control_packet.SetItemText(DAILY_POLL_TIMEOUT,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_HOLIDAY_FLAGS);
	m_xlistctrl_control_packet.SetItemText(HOLIDAY_FLAGS,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_MEM_SIG_DIVISOR);
	m_xlistctrl_control_packet.SetItemText(MEM_SIG_DIV,2,value,RGB(0,0,0), RGB(255,255,255)) ;
	
	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_RESERVED11);
	m_xlistctrl_control_packet.SetItemText(RESERVED11,2,value,RGB(0,0,0), RGB(255,255,255)) ;
	
	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_FROM);
	m_xlistctrl_control_packet.SetItemText(FROM,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_GAME_KEY_ENABLE_FLAGS);
	m_xlistctrl_control_packet.SetItemText(GAME_KEY_ENABLE_FLAGS,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_RESERVED23);
	m_xlistctrl_control_packet.SetItemText(RESERVED23,2,value,RGB(0,0,0), RGB(255,255,255)) ;

	//ToDo - This only if it exits
	//p_packet.GetDataFieldFormatted(value, CVLCPacket::CTRL_VARIABLE_BANNER_DATA);
	//m_xlistctrl_control_packet.SetItemText(VARIABLE_BANNER_DATA,2,value,RGB(0,0,0), RGB(255,255,255)) ;


	m_xlistctrl_control_packet.UnlockWindowUpdate();

}


////////////////////////////////////////////////////////////////////////////////////
/*
Set the packet in the dialog m_xlistctrl_datetime
*/

void CDlgControl::SetPacket(CVLCPacket& p_packet )
{

	//check that its the right packet
	if(p_packet.GetDataType() != CVLCPacket::DATA_TYPE_CONTROL)
		return;
	
	CString value;

	value = m_xlistctrl_control_packet.GetItemText(FORMAT_ID,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::DT_FORMAT_ID);

	value = m_xlistctrl_control_packet.GetItemText(RESERVED2,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CRTL_RESERVED2);

	value = m_xlistctrl_control_packet.GetItemText(CTRL_VERS_NO,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CRTL_VERS);

	value = m_xlistctrl_control_packet.GetItemText(RESERVED4,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CRTL_RESERVED4);

	value = m_xlistctrl_control_packet.GetItemText(TERMINAL_MODE,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_TERMINAL_MODE);

	value = m_xlistctrl_control_packet.GetItemText(RESERVED6,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CRTL_RESERVED6);

	value = m_xlistctrl_control_packet.GetItemText(DAILY_POLL_TIMEOUT,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_DALY_POLL_TIMEOUT);	

	value = m_xlistctrl_control_packet.GetItemText(HOLIDAY_FLAGS,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_HOLIDAY_FLAGS);
	
	value = m_xlistctrl_control_packet.GetItemText(MEM_SIG_DIV,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_MEM_SIG_DIVISOR);

	value = m_xlistctrl_control_packet.GetItemText(RESERVED11,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_RESERVED11);

	value = m_xlistctrl_control_packet.GetItemText(FROM,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_FROM);

	value = m_xlistctrl_control_packet.GetItemText(GAME_KEY_ENABLE_FLAGS,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_GAME_KEY_ENABLE_FLAGS);

	value = m_xlistctrl_control_packet.GetItemText(RESERVED23,2);
	p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_RESERVED23);

	//todo - need to update length
	//value = m_xlistctrl_control_packet.GetItemText(VARIABLE_BANNER_DATA,2);
	//p_packet.SetDataFieldFormatted(value, CVLCPacket::CTRL_VARIABLE_BANNER_DATA);
}




CVlcData* CDlgControl::GetVlcData()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id==NULL)
		return NULL;

	return active_id->GetControlData();
	
}

void CDlgControl::GetPacketName(CString& p_packet_name)
{
	p_packet_name = CString("Control");
}
