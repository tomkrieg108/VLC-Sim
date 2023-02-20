#if !defined(AFX_CONTROL_H__DDB0CC91_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_CONTROL_H__DDB0CC91_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#pragma warning(disable:4101)

#include "colour_list_box.h"
#include "dlg_packet_interface.h"
#include "XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgControl dialog

class CDlgControl : public CDialog, public IDlgPacket
{
// Construction
public:

	enum
	{
		FORMAT_ID = 0,
		RESERVED2,
		CTRL_VERS_NO,
		RESERVED4,
		TERMINAL_MODE,
		RESERVED6,
		DAILY_POLL_TIMEOUT,
		HOLIDAY_FLAGS,
		MEM_SIG_DIV,
		RESERVED11,
		FROM,
		GAME_KEY_ENABLE_FLAGS,
		RESERVED23,
		VARIABLE_BANNER_DATA
	};

	int serialize(CArchive* pArchive);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDlgControl(CWnd* pParent = NULL);   // standard constructor

	//For IDlgPacket
	void DisplayPacket(const CVLCPacket& p_packet );
	void SetPacket(CVLCPacket& p_packet );
	CVlcData* GetVlcData();	
	void GetPacketName(CString& p_packet_name);


// Dialog Data
	//{{AFX_DATA(CDlgControl)
	enum { IDD = IDD_CONTROL };
	CXListCtrl	m_xlistctrl_control_packet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CDlgControl)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDlgControl* g_dlg_control;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROL_H__DDB0CC91_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
