#if !defined(AFX_EVENTS_H__DDB0CC92_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_EVENTS_H__DDB0CC92_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#pragma warning(disable:4101)
#pragma warning (disable:4786) // identifier was truncated to '255' characters in the debug 


#include "edit_hex.h"
#include "XListCtrl.h"
#include "dlg_packet_interface.h"
#include "notify.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgEvents dialog

class CDlgEvents : public CDialog, public IDlgPacket, public INotifiable
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDlgEvents(CWnd* pParent = NULL);   // standard constructor

	//For IDlgPacket
	void DisplayPacket(const CVLCPacket& p_packet );
	void SetPacket(CVLCPacket& p_packet );
	CVlcData* GetVlcData();	
	void GetPacketName(CString& p_packet_name);

	

// Dialog Data
	//{{AFX_DATA(CDlgEvents)
	enum { IDD = IDD_EVENTS };
	CButton	m_ctrl_btn_set_default;
	CButton	m_ctrl_btn_add_event;
	CComboBox	m_ctrl_combo_apply_errors_to;
	CStatic	m_ctrl_static_packet_count;
	CStatic	m_ctrl_static_record_count;
	CStatic	m_ctrl_static_start_read;
	CStatic	m_ctrl_static_end_read;
	CButton	m_ctrl_chk_use_current_time;
	CXListCtrl	m_xlistctrl_event_log;
	CXListCtrl	m_xlistctrl_event_data;
	CComboBox	m_ctrl_combo_event_type;
	//}}AFX_DATA

	uint GetCurSelErrorPacket() {return m_ctrl_combo_apply_errors_to.GetCurSel();}
	void SetCurSelErrorPacket(uint p_sel) 
	{	ASSERT(p_sel < m_ctrl_combo_apply_errors_to.GetCount());
		m_ctrl_combo_apply_errors_to.SetCurSel(p_sel);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEvents)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CDlgEvents)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeComboEventTypes();
	afx_msg void OnBtnAdd();
	afx_msg void OnDblclkListctrlEventLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListctrlEventLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkListctrlEventLog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnSetDefault();
	afx_msg void OnSelchangeComboApplyErrorsTo();
	//}}AFX_MSG

	void EnableControls(bool p_enable);
	void SetupColumns();
	void SetupRows(CVlcEventLogData::EEventType p_event_type = CVlcEventLogData::EVENT_RECORD_NO);

	void SetupRowsCashout();
	void SetupRowsCashoutCtn();
	void SetupRowsGameWin();
	void SetupRowsGameWinCtn();
	void SetupRowsException();
	void SetupRowsCreditSnapshot();
	void SetupRowsMsgToHost();
	void SetupRowsStatusPollTimeout();
	void SetupRowsFwRevChange();
	void SetupRowsEncryptionKeyChange();
	void SetupRowsTimeDate();
	void SetupRowsRecordNo();

	void DisplayField(const CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset=0);
	void SetField(CVLCPacket& p_packet, int p_field_pos, int p_field_id, int p_offset=0 );

	void DisplayRecord(CVlcEventLogData::EventRecord* p_record);
	void DisplayRecordCashout(const CVLCPacket& p_packet);
	void DisplayRecordCashoutCtn(const CVLCPacket& p_packet);
	void DisplayRecordGameWin(const CVLCPacket& p_packet);
	void DisplayRecordGameWinCtn(const CVLCPacket& p_packet);
	void DisplayRecordException(const CVLCPacket& p_packet);
	void DisplayRecordCreditSnapshot(const CVLCPacket& p_packet);
	void DisplayRecordMsgToHost(const CVLCPacket& p_packet);
	void DisplayRecordStatusPollTimeout(const CVLCPacket& p_packet);
	void DisplayRecordFwRevChange(const CVLCPacket& p_packet);
	void DisplayRecordEncryptionKeyChange(const CVLCPacket& p_packet);
	void DisplayRecordTimeDate(const CVLCPacket& p_packet);
	void DisplayRecordRecordNo(const CVLCPacket& p_packet);

	void SetRecord(CVLCPacket& p_packet);
	void SetRecordCashout(CVLCPacket& p_packet);
	void SetRecordCashoutCtn(CVLCPacket& p_packet);
	void SetRecordGameWin(CVLCPacket& p_packet);
	void SetRecordGameWinCtn(CVLCPacket& p_packet);
	void SetRecordException(CVLCPacket& p_packet);
	void SetRecordCreditSnapshot(CVLCPacket& p_packet);
	void SetRecordMsgToHost(CVLCPacket& p_packet);
	void SetRecordStatusPollTimeout(CVLCPacket& p_packet);
	void SetRecordFwRevChange(CVLCPacket& p_packet);
	void SetRecordEncryptionKeyChange(CVLCPacket& p_packet);
	void SetRecordTimeDate(CVLCPacket& p_packet);
	void SetRecordRecordNo(CVLCPacket& p_packet);

	enum
	{
		SET_DATE=0,
		SET_TIME
	};

	void SetDateTime(CVLCPacket& p_packet, int p_pos, int p_field, int p_which_one);
	void GetEventDescription(CString& p_string, CVlcEventLogData::EventRecord& p_record);
	void AddRecordToDisplayedList(CVlcEventLogData::EventRecord* p_record);	
	void ShowStartEndRecords(CVlcEventLogData* p_event_log);

protected:
	void ProcessNotify( int p_id );  // for INotifiable
	int m_read_record_start;
	int m_read_record_end;

	DECLARE_MESSAGE_MAP()
};

extern CDlgEvents* g_dlg_events;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDB_H__DDB0CC92_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
