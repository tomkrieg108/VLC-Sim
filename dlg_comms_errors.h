 #if !defined(AFX_COMMS_ERRORS_H__42D5451D_8CFF_4169_B839_FCA5F0D95B46__INCLUDED_)
#define AFX_COMMS_ERRORS_H__42D5451D_8CFF_4169_B839_FCA5F0D95B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "edit_hex.h"
#include "notify.h"


struct CommsErrorsTiming
{
	bool m_keep_rts_enabled;
	bool m_dont_enable_rts;
	int m_data_read_response_time;
	int m_interchar_time;
	int m_rts_disable_time;
	int m_poll_response_time;
	int m_num_packet_breaks;

	CommsErrorsTiming():
		m_keep_rts_enabled(false),
		m_dont_enable_rts(false),
		m_data_read_response_time(0),
		m_interchar_time(0),
		m_rts_disable_time(0),
		m_poll_response_time(0),
		m_num_packet_breaks(0)
	{
	}
};


struct CommsErrorsCorruptData
{
	bool m_header;
	bool m_terminal_id;
	bool m_n_bits;
	bool m_command;
	bool m_data;
	bool m_validation_datetime;
	bool m_validation_code;
	bool m_crc;
	bool m_trailer;

	CommsErrorsCorruptData():
		m_header(false),
		m_terminal_id(false),
		m_n_bits(false),
		m_command(false),
		m_data(false),
		m_validation_datetime(false),
		m_validation_code(false),
		m_crc(false),
		m_trailer(false)
	{
	}
};

struct CommsErrorsDataSize
{
	enum EAction
	{
		NONE=0,
		ADD,
		REMOVE,
		APPEND
	};

	enum EFillType
	{		
		ZERO=0,
		RANDOM
	};

	int m_num_bytes;
	EAction m_action;
	EFillType m_fill_type;

	
	CommsErrorsDataSize():
		m_num_bytes(0),
		m_action(APPEND),
		m_fill_type(RANDOM)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDlgCommsErrors dialog

class CDlgCommsErrors : public CDialog, public INotifiable 
{
protected:
	enum
	{
		PKT_TYPE_DATETIME = 0,
		PKT_TYPE_CONTROL,
		PKT_TYPE_CONFIGURATION,
		PKT_TYPE_CUR_MON,
		PKT_TYPE_DAY_END_MON,
		PKT_TYPE_EVENT_LOG,
		PKT_TYPE_TRANSACTION,
		PKT_TYPE_EXCEPTION
	};

protected:

	void ProcessNotify( int p_id );	// for INotifiable

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDlgCommsErrors(CWnd* pParent = NULL);   // standard constructor

	void DisplayErrorsTiming(const CommsErrorsTiming& p_errors);
	void DisplayErrorsData(const CommsErrorsCorruptData& p_errors);
	void DisplayErrorsDataSize(const CommsErrorsDataSize& p_errors);
	void SetErrorsTiming( CommsErrorsTiming& p_errors);
	void SetErrorsData( CommsErrorsCorruptData& p_errors);
	void SetErrorsDataSize(CommsErrorsDataSize& p_errors);
	void DisplayErrorsDataDefault();
	void DisplayErrorsTimingDefault();
	void DisplayErrorsSizeDefault();

// Dialog Data
	//{{AFX_DATA(CDlgCommsErrors)
	enum { IDD = IDD_COMMS_ERRORS };
	CButton	m_ctrl_chk_corrupt_n_bits;
	CButton	m_ctrl_chk_enable_timing_err;
	CButton	m_ctrl_chk_enable_size_err;
	CButton	m_ctrl_chk_enable_data_err;
	CComboBox	m_ctrl_combo_apply_pkt_type;
	CButton	m_ctrl_chk_appply_next_ack_nack;
	CButton	m_ctrl_chk_apply_next_read_reply;
	CButton	m_ctrl_btn_set_errors;
	CComboBox	m_ctrl_combo_fill_type;
	CComboBox	m_ctrl_combo_data_size_errors;
	CButton	m_ctrl_chk_validation_datetime;
	CComboBox	m_ctrl_combo_number_breaks;
	CEdit	m_ctrl_edit_num_bytes;
	CButton	m_ctrl_chk_keep_rts_enabled;
	CButton	m_ctrl_chk_dont_enable_rts;
	CButton	m_ctrl_chk_corrupt_validation_code;
	CButton	m_ctrl_chk_corrupt_trailer;
	CButton	m_ctrl_chk_corrupt_terminal_id;
	CButton	m_ctrl_chk_corrupt_header;
	CButton	m_ctrl_chk_corrupt_data;
	CButton	m_ctrl_chk_corrupt_crc;
	CButton	m_ctrl_chk_corrupt_command;
	CEdit	m_ctrl_edit_data_read_response_time;
	CEdit	m_ctrl_edit_inter_char_time;
	CEdit	m_ctrl_edit_rts_disable_time;
	CEdit	m_ctrl_edit_poll_response_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCommsErrors)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CDlgCommsErrors)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCheckKeepRtpEnabled();
	afx_msg void OnButtonSetErrors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetTOParameters(void);
	

};




extern CDlgCommsErrors* g_dlg_comms_errors;

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMS_ERRORS_H__42D5451D_8CFF_4169_B839_FCA5F0D95B46__INCLUDED_)
