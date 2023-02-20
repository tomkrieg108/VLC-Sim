#if !defined(AFX_CONFIGURATION_H__DDB0CC93_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_CONFIGURATION_H__DDB0CC93_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#pragma warning(disable:4101)

#include "dlg_packet_interface.h"
#include "XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConfiguration dialog

class CDlgConfiguration : public CDialog, public IDlgPacket
{
// Construction
public:

	//overhead data
	enum
	{
		OH_FORMAT_ID	= 0,
		OH_RESERVED2,
		OH_CONFIG_VERS,
		OH_BYTES_SYS,
		OH_RESERVED5,
		OH_COUNT_GAME_KEY,
		OH_BYTES_KEY_X,
		OH_RESERVED_X
	};

	//system data
	enum
	{
		SYS_LOCATION_NAME = 0,
		SYS_LOACTION_ADDRESS,
		SYS_PERMIT_NUMBER,
		SYS_RESERVED69,
		SYS_TERM_ENABLE_TIME_NH,
		SYS_TERM_ENABLE_DUR_NH,
		SYS_TERM_ENABLE_TIME_H,
		SYS_TERM_ENABLE_DUR_H,
		SYS_HOPPER_PAYOUT_MAX,
		SYS_SUBSTANTIAL_CASHOUT,
		SYS_SUBSTANTIAL_WIN,
		SYS_DAY_END_SNAPSHOT_TIME,
		SYS_WEEK_END_SNAPSHOT_DAY,
		SYS_TIMEZONE_CODE,
		SYS_DAYE_DAYLIGHT_SAVINGS,
		SYS_TIME_DAYLIGHT_SAVINGS,
		SYS_SHARE_PERCENTAGE,
		SYS_TRANSACTION_ENABLE_FLAGS,
		SYS_BEGINNING_EVENTS_LOG,
		SYS_RESERVED103,
		SYS_STATUS_POLL_TIMEOUT,
		SYS_EXCEPTION_ENABLE,
		SYS_NUM_FIELDS
		//wont include any encryption data for now
	};

	//game key data
	enum
	{
		GK_RESERVED1	=	0,
		GK_GAME_TYPE,
		GK_GAME_NAME,
		GK_GAME_DISABLE_TIME_NH,
		GK_GAME_DISABLE_DURATION_NH,
		GK_GAME_DISABLE_TIME_H,
		GK_GAME_DISABLE_DURATION_H,
		GK_GAME_ID,
		GK_GAME_ENABLE_CONTROL_FLAGS,
		GK_GAME_CR_SIZE_CENTS,
		GK_MAX_BET_PER_GAME_CENTS,
		GK_MAX_AWARD_PER_GAME_CENTS,
		GK_EXTRA_BYTES,
		GK_NUM_FIELDS
	};


	void AddOverheadSection(int p_count_game_key);
	void AddSystemSection(void);
	void AddGameKeySection(int p_num_system_fields, int p_num_game_keys, int p_game_key, int p_bytes_extra_parameters, COLORREF p_colour);

	//For IDlgPacket
	void DisplayPacket(const CVLCPacket& p_packet );
	void SetPacket(CVLCPacket& p_packet );
	CVlcData* GetVlcData();	
	void GetPacketName(CString& p_packet_name);

	int serialize(CArchive* pArchive);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	void OnStatus();
	CDlgConfiguration(CWnd* pParent = NULL);   // standard constructor

//	void GetStaticData( CPDB1Packet& p_packet );
//	void GetCurrentValues( CPDB1Packet& p_packet );

// Dialog Data
	//{{AFX_DATA(CDlgConfiguration)
	enum { IDD = IDD_CONFIGURATION };
	CButton	m_ctrl_chk_nack_2nd;
	CButton	m_ctrl_chk_nack_first;
	CButton	m_ctrl_chk_mess_with_o_bit;
	CXListCtrl	m_xlistctrl_config_packet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfiguration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CDlgConfiguration)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	int m_game_key_count;
	int* m_game_key_bytes;
	int m_sys_config_bytes;


	void DisplayField(const CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset=0);
	void SetField(CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset=0 );

	DECLARE_MESSAGE_MAP()
};

extern CDlgConfiguration* g_dlg_configuration;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATION_H__DDB0CC93_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
