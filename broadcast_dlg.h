#if !defined(AFX_BROADCAST_DLG_H__ED06F140_A736_11DC_BB32_000BCD4A42B3__INCLUDED_)
#define AFX_BROADCAST_DLG_H__ED06F140_A736_11DC_BB32_000BCD4A42B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// broadcast_dlg.h : header file
//


//#include "xseries_packet.h"
//#include "timers.h"
#include "notify.h"
//#include <list>

//using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CBroadcastDlg dialog

class CBroadcastDlg : public CDialog, INotifiable
{
// Construction
public:
	CBroadcastDlg(CWnd* pParent = NULL);   // standard constructor



// Dialog Data
	//{{AFX_DATA(CBroadcastDlg)
	enum { IDD = IDD_BROADCASTS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBroadcastDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBroadcastDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



	void ProcessNotify( int p_id ); // for INotifiable
};

extern CBroadcastDlg* g_broadcast_dlg;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROADCAST_DLG_H__ED06F140_A736_11DC_BB32_000BCD4A42B3__INCLUDED_)
