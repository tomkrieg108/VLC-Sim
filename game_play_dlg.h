#if !defined(AFX_GAMEPLAY_H__20685A15_3398_4F2C_80FA_A3EB3D2B7EB8__INCLUDED_)
#define AFX_GAMEPLAY_H__20685A15_3398_4F2C_80FA_A3EB3D2B7EB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// game_play_dlg.h : header file
//

#include "notify.h"


/////////////////////////////////////////////////////////////////////////////
// CGamePlay dialog

class CGamePlay : public CDialog , public INotifiable
{

protected:
	void ProcessNotify( int p_id );	// for INotifiable

// Construction
public:
	CGamePlay(CWnd* pParent = NULL);   // standard constructor

	void SetStatus(int p_num, const CString& p_val );

	bool GetSAPHit(int p_level);
	bool GetLPHit(int p_level);
	bool GetSAPHit(void);
	bool GetLPHit(void);
	bool GetProgHit(void);
	int GetSAPHitCount(void);

	void EnableReset( const CString& p_caption );
	void DisableReset( void );
	void EnableReset( const CString& p_caption, bool p_enable );
	
	void EnableControls( bool p_enable );
	void EnableHopperOut( bool p_enable );
	void EnableCancelCr( bool p_enable );

	

// Dialog Data
	//{{AFX_DATA(CGamePlay)
	enum { IDD = IDD_GAMEPLAY };
	CStatic	m_status_3;
	CStatic	m_status_2;
	CStatic	m_status_1;
	CString	m_bet;
	CString	m_win;
	CString	m_duration;
	CString	m_max_bet;
	CString	m_min_bet;
	BOOL	m_lp_1;
	BOOL	m_lp_2;
	BOOL	m_lp_3;
	BOOL	m_lp_4;
	BOOL	m_sap_1;
	BOOL	m_sap_2;
	BOOL	m_sap_3;
	BOOL	m_sap_4;
	BOOL	m_main_door;
	BOOL	m_security_door;
	BOOL	m_error_memory;
	BOOL	m_error_self_audit;
	BOOL	m_power_down;
	BOOL	m_sef;
	BOOL	m_audit_mode;
	BOOL	m_error_display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGamePlay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGamePlay)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd100();
	afx_msg void OnAdd20();
	afx_msg void OnAdd10();
	afx_msg void OnAdd5();
	afx_msg void OnAdd50();
	afx_msg void OnAdd1Hop();
	afx_msg void OnAdd1Cb();
	afx_msg void OnPlay();
	afx_msg void OnStart();
	afx_msg void OnProgReset();
	afx_msg void OnDoorMain();
	afx_msg void OnDoorSecurity();
	afx_msg void OnErrorMemory();
	afx_msg void OnErrorSelfAudit();
	afx_msg void OnPowerDown();
	afx_msg void OnSef();
	afx_msg void OnAuditMode();
	afx_msg void OnHopperOut();
	afx_msg void OnCancelCr();
	afx_msg void OnConfirm();
	afx_msg void OnCancel();
	afx_msg void OnDisplayError();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CGamePlay* g_game_play_dlg;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEPLAY_H__20685A15_3398_4F2C_80FA_A3EB3D2B7EB8__INCLUDED_)
