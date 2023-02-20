// dlg_main.h : header file
//
//{{AFX_INCLUDES()
#include "mscommctrl.h"
//}}AFX_INCLUDES

#if !defined(AFX_XSERIESGENERATORDLG_H__DDB0CC88_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_XSERIESGENERATORDLG_H__DDB0CC88_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "sdb_dlg.h"
#include "dlg_events.h"
#include "dlg_transactions.h"
#include "dlg_control.h"
#include "dlg_configuration.h"
#include "dlg_monitor.h"
//#include "custom_dlg.h"
#include "dlg_exceptions.h"
#include "dlg_comms_errors.h"
#include "dlg_datetime_memsig.h"
#include "dlg_packet_interface.h"
//#include "coms_options.h"
//#include "program_options_dlg.h"
//#include "cputicker.h"
//#include "game_play_dlg.h"
#include "notify.h"
#include "timers.h"
#include "colour_list_box.h"
#include "vlc_packet.h"
#include "XEdit.h"

#pragma warning(disable:4101)

/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog
class CDlgMain : public CDialog , public INotifiable, public ITimer
{

public:

	//column refs for the id list
	enum
	{
		COLUMN_ID	=	0,
		COLUMN_V,
		COLUMN_D,
		COLUMN_F,
		COLUMN_P,
		COLUMN_I,
		NUM_COLUMNS
	};

	//For Adding / Updating columns in the poll ID list
	//todo - this struct not used anymore
	struct PollData
	{
		CString id;
		bool v;
		bool d;
		bool f;
		bool p;
		bool i;
	};

protected:
	enum EPacketType
	{
		PKT_DEFAULT = 0,
		PKT_NEXT_TO_SEND,
		PKT_LAST_SENT_READ_REPLY,
		PKT_LAST_SENT_SITE_REPLY,
		PKT_LAST_RECEIVED,
	};

protected:
	void ProcessNotify( int p_id );  // for INotifiable
	void ProcessTimeout( int p_id ); // for ITimer

public:

	CDlgMain(CWnd* pParent = NULL);	// standard constructor

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DrawTitle();
	void DetermineOSVersion();
	CString CaptureFilename;
	bool WriteSDBtoCaptureFile;
	CFile* CapFile;
	CString SelectedComPort;

	//List of Id's
	//void AddId(PollData& p_poll_data); //todo - not used
	//void UpdateId(PollData& p_poll_data, int p_idx); //todo - not used
	void AddId(CVlcId* p_id);
	void UpdateIdDisplay(CVlcId* p_id, const CVLCPacket* p_poll);

	const CDialog* const GetActiveSubWindow(); 
	void SetControlEnableStatus();
	void DisplayNextToSend();	
	void DisplayDefault();
	void DisplayLastReceived(); //last write request
	void DisplayLastReadReplyHost();
	void DisplayLastReadReplySite();
	void DisplayErrors(EPacketType p_packet_type, bool p_include_detail);
	void DisplayResponseToNextReceived();
	void DisplayActiveIdStatus();

	int m_datime_timer;


// Dialog Data
	//{{AFX_DATA(CDlgMain)
	enum { IDD = IDD_MAIN };
	CButton	m_ctrl_chk_no_resp_write_req;
	CButton	m_ctrl_chk_no_resp_read_req;
	CButton	m_ctrl_chk_not_responding;
	CStatic	m_ctrl_static_terminal_mode;
	CButton	m_ctrl_btn_errors_view;
	CButton	m_ctrl_chk_no_polls;
	CButton	m_ctrl_btn_data_view_last_received;
	CButton	m_ctrl_btn_data_view_last_site_reply;
	CButton	m_ctrl_btn_data_view_last_read_reply;
	CStatic	m_ctrl_static_status;
	CXEdit	m_ctrl_xedit_add_id;
	CButton	m_ctrl_chk_watcher_pause;
	CButton	m_ctrl_chk_watcher_log;
	CButton	m_ctrl_chk_watcher_errors;
	CButton	m_ctrl_chk_watcher_description;
	CButton	m_ctrl_chk_watcher_active_id;
	CStatic	m_ctrl_static_time_u;
	CStatic	m_ctrl_static_date_u;
	CButton	m_ctrl_chk_errors_timing;
	CComboBox	m_ctrl_combo_next_response;
	CButton	m_ctrl_chk_errors_size;
	CButton	m_ctrl_chk_errors_data;
	CButton	m_ctrl_btn_next_response_set;
	CButton	m_ctrl_btn_errors_set;
	CButton	m_ctrl_btn_data_view_next;
	CButton	m_ctrl_btn_data_view_default;
	CButton	m_ctrl_btn_data_next_set;
	CXListCtrl	m_xlistctrl_polling_list;
	CStatic	m_ctrl_static_active_id;
	CColorListBox	m_comms_watcher;
	CStatic	m_receive_debug;
	CListCtrl	m_ComPortList;
	CTreeCtrl	m_ConfigTree;
	HTREEITEM ht_datetime_memsig; 
	HTREEITEM ht_events; 
	HTREEITEM ht_transactions; 
	HTREEITEM ht_exceptions; 
	HTREEITEM ht_comms_errors; 
	HTREEITEM ht_control; 
	HTREEITEM ht_configuration; 
	HTREEITEM ht_monitor; 
	HTREEITEM htConfig; 
	BOOL	m_Connect;
	CMSCommCtrl	m_Comms;
	BOOL	m_PowerGood;
	CString	m_sef;
	CMSCommCtrl	m_Comms2;
	//}}AFX_DATA
;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	HICON m_hIcon;
	CDialog* m_active_sub_window; //The dialog currently shown in the placeholder position.
	IDlgPacket* m_active_packet_window; //The packet dialog currently shown in the placeholder position

	//CSDB m_SDBCtrl;
	//CGamePlay m_GamePlayCtrl;
	CDlgDatetimeMemsig m_ctrl_dlg_datetime_memsig;
	CDlgEvents m_ctrl_dlg_events;
	CDlgTransactions m_ctrl_dlg_transactions;
	CDlgExceptions m_ctrl_dlg_exceptions;
	CDlgCommsErrors m_ctrl_dlg_comms_errors;
	CDlgControl m_ctrl_dlg_control;
	CDlgConfiguration m_ctrl_dlg_configuration;
	CDlgMonitor m_ctrl_dlg_monitor;
	//CCustom m_CustomCtrl;
	//CComsOptions m_ComsOptionsCtrl;
	//CProgramOptions m_ProgramOptionsCtrl;
	bool m_sef_status;

	void SetDataSendControlsEnabled(bool p_enable); //set next, view next, view default
	void SetDataReceiveControlsEnabled(bool p_enable);
	void SetErrorControlsEnabled(bool p_enable);
	

	void DisplayErrorSettings(const CVLCPacket& p_packet, bool p_update_details=true, CString& p_status_msg=CString("")); 
	void SetErrorSettings(CVLCPacket& p_packet);
	void DisplayResponseSetting(const CVLCPacket& p_packet); 
	void SetResponseSetting(CVLCPacket& p_packet);
	void DisplayPacket(CVLCPacket::StringList& p_pkt_strs, COLORREF& p_colour); //For display in comms watcher
	void DisplayPacket(CVLCPacket* p_packet);	//For display in comms watcher
	

	void DisplayDateTime();
	void DisplayStatus();

	bool m_WinNT;
	CString m_OSVersion;
	CDC m_dc;
	CBitmap *m_pbitmapOldTitle;
    CBitmap m_bitmapTitle;

	// Generated message map functions
	//{{AFX_MSG(CDlgMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangedConfigTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnConnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnComms();
	afx_msg void OnClickComPortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDblclkXlistctrlPolling(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnDataSetNext();
	afx_msg void OnBtnDataViewDefault();
	afx_msg void OnBtnDataViewNext();
	afx_msg void OnBtnErrorsSet();
	afx_msg void OnBtnNextResponseSet();
	afx_msg void OnBtnAddId();
	afx_msg void OnBtnDataViewLastReadReply();
	afx_msg void OnBtnDataViewLastReceived();
	afx_msg void OnBtnDataViewLastSiteReply();
	afx_msg void OnBtnErrorsView();
	afx_msg void OnCheckLog();
	afx_msg void OnCheckSetNotResponding();
	//afx_msg void OnCheckNonRespondingReadReq();
	//afx_msg void OnCheckNoRespWriteReq();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CVlcData* GetVlcData(); //Return Vlc data - depending on current packet dialog dusplayed
	CVLCPacket* GetPacket(CDlgMain::EPacketType p_packet_type);
	void DisplayPacketStatus(CString& p_packet_type,bool p_include_resp=false,CVLCPacket* p_pkt=NULL ); //Displays the status of the packet in the strip about the packet display.
};

extern CDlgMain *g_dlg_main;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSERIESGENERATORDLG_H__DDB0CC88_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
