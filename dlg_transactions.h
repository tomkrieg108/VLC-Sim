#if !defined(AFX_TRANSACTIONS_H__97AA1881_3503_11D7_9899_0010B543CAF9__INCLUDED_)
#define AFX_TRANSACTIONS_H__97AA1881_3503_11D7_9899_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "edit_hex.h"
#include "XListCtrl.h"
#include "dlg_packet_interface.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTransactions dialog

class CDlgTransactions : public CDialog, public IDlgPacket, public INotifiable
{
// Construction
public:
	CDlgTransactions(CWnd* pParent = NULL);   // standard constructor
	//For IDlgPacket
	void DisplayPacket(const CVLCPacket& p_packet );
	void SetPacket(CVLCPacket& p_packet );
	CVlcData* GetVlcData();	
	void GetPacketName(CString& p_packet_name);

// Dialog Data
	//{{AFX_DATA(CDlgTransactions)
	enum { IDD = IDD_TRANSACTIONS };
	CButton	m_ctrl_btn_send;
	CXListCtrl	m_xlistctrl_transaction_request;
	CXListCtrl	m_xlistctrl_transaction_reply;
	CEditHex	m_test;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTransactions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTransactions)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonSend();
	afx_msg void OnBtnSendAllIds();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetupColumns();
	void SetupRows();
	void DisplayField(const CVLCPacket& p_packet, CXListCtrl& p_list, int p_field_pos, int p_field_id, int p_offset=0);
	void SetField(CVLCPacket& p_packet, CXListCtrl& p_list, int p_field_pos, int p_field_id, int p_offset=0 );

	//For INotifiable
	void ProcessNotify( int p_id );  // for INotifiable
	void SetControlEnableStatus();
};

extern CDlgTransactions* g_dlg_transactions;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMDB_H__97AA1881_3503_11D7_9899_0010B543CAF9__INCLUDED_)
