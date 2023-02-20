#if !defined(AFX_EXCEPTIONS_H__9E6673C1_2A00_11D7_9899_0010B543CAF9__INCLUDED_)
#define AFX_EXCEPTIONS_H__9E6673C1_2A00_11D7_9899_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



#include "XListCtrl.h"
#include "notify.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgExceptions dialog

class CDlgExceptions : public CDialog, public INotifiable
{
// Construction
public:
	CDlgExceptions(CWnd* pParent = NULL);   // standard constructor

	void RemoveQueuedFront();
	void DisplayQueue();

// Dialog Data
	//{{AFX_DATA(CDlgExceptions)
	enum { IDD = IDD_EXCEPTIONS };
	CButton	m_send_queued;
	CXListCtrl	m_xlistctrl_queued;
	CXListCtrl	m_xlistctrl_exceptions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExceptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void UpdateQueuedException(WPARAM, LPARAM);

	// Generated message map functions
	//{{AFX_MSG(CDlgExceptions)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkXlistctrlExceptions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckSendPending();
	//}}AFX_MSG

	afx_msg LRESULT OnCheckBox(WPARAM, LPARAM);
	afx_msg LRESULT OnEditEnd(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()


protected:
	void ProcessNotify( int p_id );  // for INotifiable

};

extern CDlgExceptions* g_dlg_exceptions;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCEPTIONS_H__9E6673C1_2A00_11D7_9899_0010B543CAF9__INCLUDED_)
