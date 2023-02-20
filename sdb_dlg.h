#if !defined(AFX_SDB_H__DDB0CC90_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_SDB_H__DDB0CC90_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sdb_dlg.h : header file
//
#pragma warning(disable:4101)

//#include "xseries_packet.h"
#include "notify.h"
#include "edit_hex.h"

/////////////////////////////////////////////////////////////////////////////
// CSDB dialog

class CSDB : public CDialog, public INotifiable
{
// Construction
public:
	int serialize (CArchive* pArchive);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSDB(CWnd* pParent = NULL);   // standard constructor

//	void SetStaticData(CSDBPacket& p_packet);
	void GetMeters(void);

// Dialog Data
	//{{AFX_DATA(CSDB)
	enum { IDD = IDD_SDB };
	CEditHex	m_sfs;
	CEdit	m_BSCCtrl;
	CEdit	m_ChkSum;
	BOOL	m_ChkGood;
	CString	m_Checksum;
	BOOL	m_BSCCorrect;
	CString	m_CancelledCredit;
	CString	m_CashBox;
	CString	m_CashIn;
	CString	m_CashOut;
	CString	m_Credit;
	CString	m_MiscellaneousAccrual;
	CString	m_MoneyIn;
	CString	m_MoneyOut;
	BOOL	m_P1SEF;
	BOOL	m_P2SEF;
	BOOL	m_P3SEF;
	BOOL	m_P4SEF;
	BOOL	m_P5SEF;
	BOOL	m_P6SEF;
	CString	m_ProgramID1;
	CString	m_ProgramID2;
	CString	m_ProgramID3;
	CString	m_ProgramID4;
	CString	m_TotalWins;
	CString	m_Turnover;
	BOOL	m_Status1_1;
	BOOL	m_Status1_2;
	BOOL	m_Status2_1;
	BOOL	m_Status1_3;
	BOOL	m_Status1_4;
	BOOL	m_Status1_5;
	BOOL	m_Status2_2;
	BOOL	m_Status2_3;
	BOOL	m_Status2_4;
	BOOL	m_Status2_5;
	BOOL	m_Status3_1;
	BOOL	m_Status3_2;
	BOOL	m_Status3_3;
	BOOL	m_Status3_4;
	BOOL	m_Status3_5;
	BOOL	m_Status4_1;
	BOOL	m_Status4_2;
	BOOL	m_Status4_3;
	BOOL	m_Status4_4;
	BOOL	m_Status4_5;
	BOOL	m_Status5_1;
	BOOL	m_Status5_2;
	BOOL	m_Status5_3;
	BOOL	m_Status5_4;
	BOOL	m_Status5_5;
	BOOL	m_Status6_1;
	BOOL	m_Status6_2;
	BOOL	m_Status6_3;
	BOOL	m_Status6_4;
	BOOL	m_Status6_5;
	BOOL	m_Status7_1;
	BOOL	m_Status7_2;
	BOOL	m_Status7_3;
	BOOL	m_Status7_4;
	BOOL	m_Status7_5;
	BOOL	m_Status8_1;
	BOOL	m_Status8_2;
	BOOL	m_Status8_3;
	BOOL	m_Status8_4;
	BOOL	m_Status8_5;
	CString	m_StatusDisplayString1;
	CString	m_StatusDisplayString2;
	CString	m_StatusDisplayString3;
	CString	m_StatusDisplayString4;
	CString	m_StatusDisplayString5;
	CString	m_StatusDisplayString6;
	CEdit	m_StatusDisplayCtrl6;
	CEdit	m_StatusDisplayCtrl5;
	CEdit	m_StatusDisplayCtrl4;
	CEdit	m_StatusDisplayCtrl3;
	CEdit	m_StatusDisplayCtrl2;
	CEdit	m_StatusDisplayCtrl1;
	byte m_bStatus1;
	byte m_bStatus2;
	byte m_bStatus3;
	byte m_bStatus4;
	byte m_bStatus5;
	byte m_bStatusSEF;
	CString	m_gmid;
	CString	m_sdb_vers;
	CString	m_rtp;
	CString	m_bcv;
	CString	m_mgmid;
	CString	m_games_door;
	CString	m_games_power;
	CString	m_games_played;
	CString	m_power_up;
	CString	m_seq_ctr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ProcessNotify( int p_id );  // for INotifiable

	CToolTipCtrl m_tooltip;
	virtual BOOL OnInitDialog();
	CDC m_dc;
	CBitmap *m_pbitmapOldTitle;
   CBitmap m_bitmapTitle;
	bool m_WinNT;

	// Generated message map functions
	//{{AFX_MSG(CSDB)
	afx_msg void OnStatus();
	afx_msg void OnSEF();
	afx_msg void OnIncreaseTOTW10000();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CSDB* g_sdb_dlg;  



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDB_H__DDB0CC90_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
