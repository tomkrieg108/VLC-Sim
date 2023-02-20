
#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "notify.h"
#include "dlg_exceptions.h"
#include "vlc_exceptions.h"
#include "vlc_id_list.h"
#include "vlc_id.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////
// Global access to the ccce dlg
CDlgExceptions* g_dlg_exceptions = 0;
///////////////////////////////

CDlgExceptions::CDlgExceptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExceptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExceptions)
	//}}AFX_DATA_INIT
	g_notify_mgr->Register( CNotify::ACTIVE_ID_CHANGED, this );
}


void CDlgExceptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);  
	//{{AFX_DATA_MAP(CDlgExceptions)
	DDX_Control(pDX, IDC_CHECK_SEND_PENDING, m_send_queued);
	DDX_Control(pDX, IDC_LISTCTRL_PENDING, m_xlistctrl_queued);
	DDX_Control(pDX, IDC_XLISTCTRL_EXCEPTIONS, m_xlistctrl_exceptions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExceptions, CDialog)
	//{{AFX_MSG_MAP(CDlgExceptions)
	ON_NOTIFY(NM_DBLCLK, IDC_XLISTCTRL_EXCEPTIONS, OnDblclkXlistctrlExceptions)
	ON_BN_CLICKED(IDC_CHECK_SEND_PENDING, OnCheckSendPending)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnCheckBox)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExceptions message handlers

BOOL CDlgExceptions::OnInitDialog() 
{
	CDialog::OnInitDialog();

	/////////////////////////////////////////////////////////////////
	// Exception list
	//////////////////////////////////////////////////////////////////
		// XListCtrl must have LVS_EX_FULLROWSELECT if combo or edit boxes are used
	
	DWORD dwExStyle;
	dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
	//	WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;
	m_xlistctrl_exceptions.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	//m_List.EnableToolTips(TRUE);

	CRect rect;
	m_xlistctrl_exceptions.GetWindowRect(&rect);
	int width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar

	// add columns
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	
	//Exception Code
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = _T("Code");
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/6;
	m_xlistctrl_exceptions.InsertColumn(0, &lvcolumn);
	

	//Exception Text
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = _T("Description");
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width*5/6;
	m_xlistctrl_exceptions.InsertColumn(1, &lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	lvcolumn.fmt = LVCFMT_LEFT;
	m_xlistctrl_exceptions.SetColumn(0,&lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	lvcolumn.fmt = LVCFMT_LEFT;
	m_xlistctrl_exceptions.SetColumn(1,&lvcolumn);


	m_xlistctrl_exceptions.LockWindowUpdate();	// ***** lock window updates while filling list *****
	m_xlistctrl_exceptions.DeleteAllItems();

	for(int i=0; i<CExceptionManager::EX_MAX; i++)
	{	
		m_xlistctrl_exceptions.InsertItem(i, g_exception_manager->GetCodeText(i) );
		m_xlistctrl_exceptions.SetItemText(i,0, g_exception_manager->GetCodeText(i), RGB(0,0,0), RGB(255,255,255)); 
		m_xlistctrl_exceptions.SetItemText(i,1, g_exception_manager->GetDescription(i), RGB(0,0,0), RGB(255,255,255));
	}

	m_xlistctrl_exceptions.UnlockWindowUpdate();	// ***** unlock window updates *****
	


	/////////////////////////////////////////////////////////////////
	// Pending list
	//////////////////////////////////////////////////////////////////
	
	m_xlistctrl_queued.GetWindowRect(&rect);
	width = rect.Width() - 2;
	width -= ::GetSystemMetrics(SM_CXVSCROLL);	// subtract width of vertical scroll bar

	memset(&lvcolumn, 0, sizeof(lvcolumn));
	m_xlistctrl_queued.SetExtendedStyle(dwExStyle);

	//Exception Code
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.pszText = "D Bit";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = width/6;
	m_xlistctrl_queued.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = "Code";
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = width/6;
	m_xlistctrl_queued.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = "Description";
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = width*4/6;
	m_xlistctrl_queued.InsertColumn(2, &lvcolumn);
	

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	m_xlistctrl_queued.SetColumn(0,&lvcolumn);
	m_xlistctrl_queued.SetColumn(0,&lvcolumn);
	m_xlistctrl_queued.SetColumn(0,&lvcolumn);
	

/*
	RGB(210,221,244)); //pale blue
	RGB(255,230,162)); //pale orange
	RGB(194,209,166)); //Green	
	RGB(247,202,202));
	RGB(0,0,0) //black
	RGB(255,255,255) white
*/

	m_send_queued.SetCheck(0);
	m_send_queued.EnableWindow(false);
	m_xlistctrl_exceptions.EnableWindow(false);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////

void CDlgExceptions::OnDblclkXlistctrlExceptions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int item = m_xlistctrl_exceptions.GetCurSel();

	CString code = m_xlistctrl_exceptions.GetItemText(item,0);
	CString description = m_xlistctrl_exceptions.GetItemText(item,1);

	int idx = m_xlistctrl_queued.GetItemCount();

	m_xlistctrl_queued.LockWindowUpdate();
	//Note: Using 0 instead of idx adds it to the start of the list
	m_xlistctrl_queued.InsertItem(idx, "");
	m_xlistctrl_queued.SetItemText(idx,1,code,RGB(0,0,0), RGB(255,255,255) ); 
	m_xlistctrl_queued.SetItemText(idx,2,description,RGB(0,0,0), RGB(255,255,255)) ; 
	m_xlistctrl_queued.SetEdit(idx,1, CXEdit::HEX,2); //edit length is 2
	m_xlistctrl_queued.SetCheckbox(idx,0,0);
	m_xlistctrl_queued.UnlockWindowUpdate();

	ExceptionData exception_data;
	exception_data.code = g_exception_manager->GetExceptionCode(code);

	exception_data.d_bit = false;
	if(m_xlistctrl_queued.GetCheckbox(idx,0) == 1 )
		exception_data.d_bit = true;

	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id != NULL)
		active_id->QueueException(exception_data);

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
void CDlgExceptions::UpdateQueuedException(WPARAM nItem, LPARAM nSubItem)
{
	int check_state = m_xlistctrl_queued.GetCheckbox(nItem,0);
	CString code = m_xlistctrl_queued.GetItemText(nItem,1);	

	ExceptionData exception_data;
	exception_data.code = g_exception_manager->GetExceptionCode(code);

	exception_data.d_bit = false;
	if(check_state == 1 )
		exception_data.d_bit = true;

	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id != NULL)
		active_id->UpdateException(exception_data,nItem);	
}


/////////////////////////////////////////////////////////////////////////////

LRESULT CDlgExceptions::OnCheckBox(WPARAM nItem, LPARAM nSubItem)
{
	UpdateQueuedException(nItem,nSubItem);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////

LRESULT CDlgExceptions::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
	CString code = m_xlistctrl_queued.GetItemText(nItem,nSubItem);

	if(code.GetLength()==0)
		code="00";
	if(code.GetLength()==1)
		code.Insert(0,'0');

	code.MakeUpper();

	CString description="";

	bool found=false;
	description = g_exception_manager->GetDescription(code,found);

	COLORREF col;

	if(!found)
		col = RGB(255,0,0);
	else
		col = RGB(0,0,0);

	m_xlistctrl_queued.SetItemText(nItem,1,code, col, RGB(255,255,255));
	m_xlistctrl_queued.SetItemText(nItem,2,description, col, RGB(255,255,255));

	UpdateQueuedException(nItem,nSubItem);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
void CDlgExceptions::RemoveQueuedFront()
{
	m_xlistctrl_queued.DeleteItem(0);	
}

/////////////////////////////////////////////////////////////////////////////

void CDlgExceptions::DisplayQueue()
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id == NULL)
		return;

	m_xlistctrl_queued.LockWindowUpdate();
	
	m_xlistctrl_queued.DeleteAllItems();

	CVlcId::ExceptionList queued_exceptions = active_id->GetExceptionQueue();
	CVlcId::ExceptionList::iterator itr = queued_exceptions.begin();

	int idx=0;
	ExceptionData exception_data;
	while( itr != queued_exceptions.end() )
	{
		exception_data = *itr;
		ushort code = exception_data.code;
		byte upper = (byte)((code>>8)&(0x0F));
		byte lower = (byte)((code>>4)&(0x0F));

		CString& code_str = CString("");
		code_str += HexToChar(upper);
		code_str += HexToChar(lower);

		m_xlistctrl_queued.InsertItem(idx, "");
		m_xlistctrl_queued.SetItemText(idx,1,code_str,RGB(0,0,0), RGB(255,255,255) ); 
		m_xlistctrl_queued.SetEdit(idx,1, CXEdit::HEX);

		CString description="";
		bool found=false;
		description = g_exception_manager->GetDescription(code_str,found);
		COLORREF col;

		if(!found)
			col = RGB(255,0,0);
		else
			col = RGB(0,0,0);

		m_xlistctrl_queued.SetItemText(idx,2,description, col, RGB(255,255,255)) ; 
		
		if(exception_data.d_bit)
			m_xlistctrl_queued.SetCheckbox(idx,0,1);
		else
			m_xlistctrl_queued.SetCheckbox(idx,0,0);

		itr++;
		idx++;
	}	
	
	m_xlistctrl_exceptions.UnlockWindowUpdate();
}

void CDlgExceptions::ProcessNotify( int p_id )
{
	if(p_id == CNotify::ACTIVE_ID_CHANGED)
	{
		CVlcId* active_id = g_vlc_id_list->GetActiveId();
		if(active_id == NULL)
		{
			m_send_queued.EnableWindow(false);
			m_xlistctrl_exceptions.EnableWindow(false);
			return;
		}		
		m_send_queued.EnableWindow(true);
		m_xlistctrl_exceptions.EnableWindow(true);
		if(active_id->GetSendQueuedExceptions())
			m_send_queued.SetCheck(1);
		else
			m_send_queued.SetCheck(0);
		DisplayQueue();
		return;
	}
}

void CDlgExceptions::OnCheckSendPending() 
{
	CVlcId* active_id = g_vlc_id_list->GetActiveId();
	if(active_id == NULL)
		return;
	bool val = (m_send_queued.GetCheck()==1);
	active_id->SetSendQueuedExceptions(val);
}
