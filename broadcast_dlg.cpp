// broadcast_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "vlc_receive_thread.h"
#include "broadcast_dlg.h"
#include "utilities.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBroadcastDlg dialog


/////////////////////////

// global access to broadcast dialog

CBroadcastDlg *g_broadcast_dlg = 0;


/////////////////////////


CBroadcastDlg::CBroadcastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBroadcastDlg::IDD, pParent)
	
{
	//{{AFX_DATA_INIT(CBroadcastDlg)
	//}}AFX_DATA_INIT

	
}


void CBroadcastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBroadcastDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBroadcastDlg, CDialog)
	//{{AFX_MSG_MAP(CBroadcastDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBroadcastDlg message handlers


//////////////////////////////////////////////////////////////////////////////

void CBroadcastDlg::ProcessNotify( int p_id )
{
	
}



