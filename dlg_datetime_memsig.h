#if !defined(AFX_DATETIME_DLG_H__ED06F140_A736_11DC_BB32_000BCD4A42B3__INCLUDED_)
#define AFX_DATETIME_DLG_H__ED06F140_A736_11DC_BB32_000BCD4A42B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "vlc_packet.h"
#include "notify.h"
#include "dlg_packet_interface.h"
#include "XListCtrl.h"
//#include <list>

//using namespace std;

/////////////////////////////////////////////////////////////////////////////


class CDlgDatetimeMemsig : public CDialog, public INotifiable, public IDlgPacket
{
// Construction
public:
	CDlgDatetimeMemsig(CWnd* pParent = NULL);   // standard constructor
	
	//For IDlgPacket
	void DisplayPacket(const CVLCPacket& p_packet );
	void SetPacket(CVLCPacket& p_packet );
	CVlcData* GetVlcData();	
	void GetPacketName(CString& p_packet_name);

// Dialog Data
	//{{AFX_DATA(CDlgDatetimeMemsig)
	enum { IDD = IDD_DATETIME_MEMSIG };
	CButton	m_use_current_datetime;
	CXListCtrl	m_xlistctrl_datetime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDatetimeMemsig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDatetimeMemsig)
	virtual BOOL OnInitDialog();
	//afx_msg void OnBtnSet();
	//afx_msg void OnBtnView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ProcessNotify( int p_id ); // for INotifiable
};

extern CDlgDatetimeMemsig* g_dlg_datetime_memsig;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATETIME_DLG_H__ED06F140_A736_11DC_BB32_000BCD4A42B3__INCLUDED_)
