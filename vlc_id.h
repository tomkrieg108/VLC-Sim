

#ifndef __VLC_ID_H
#define __VLC_ID_H

#pragma warning (disable:4786) // identifier was truncated to '255' characters in the debug 


//#include "timers.h"
#include "notify.h"
#include "vlc_packet.h"
#include "vlc_exceptions.h"
#include "stdtype.h"
#include "afxtempl.h"
#include "date_time.h"
#include "Utilities.h"
#include "vlc_data.h"
#include "timers.h"
#include <list>
#include <vector>

class CVlcData;
class CVlcDatetimeMSData;
class CVlcControlData;




/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CVlcId
{
public:
	friend class CVlcIdList;
	friend class CVlcData;
	friend class CVlcDatetimeMSData;
	friend class CVlcControlData;
	friend class CVlcConfigurationData;
	friend class CVlcMonitorData;
	friend class CVlcTransactionData;
	friend class CVlcEventLogData;
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction and initialisation
public:
	CVlcId();
	CVlcId( CString& p_id ); 
	CVlcId( const CVLCPacket* p_poll );
	~CVlcId();
private:
	void InitialiseData();	

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// id
public:
	void GetId(CString& p_out) const {p_out = m_id;}
	ushort GetId() const {return (ushort)ConvertHexStringToInt(m_id);}
	bool IsActiveId() const {return m_is_active_id;}
	void SetNonResponsivePolls(bool p_val) {m_non_responsive_polls = p_val;}
	bool GetNonResponsivePolls() {return m_non_responsive_polls;}
	void SetNonResponsiveReadReq(bool p_val) {m_non_responsive_read_req = p_val;}
	bool GetNonResponsiveReadReq() {return m_non_responsive_read_req;}
	void SetNonResponsiveWriteReq(bool p_val) {m_non_responsive_write_req = p_val;}
	bool GetNonResponsiveWriteReq() {return m_non_responsive_write_req;}

private:
	bool m_is_active_id;
	CString m_id;					//This is the machine id - hexidecimal string.
	bool m_non_responsive_polls;	//If this is set then doesn't respond to any commands from host
	bool m_non_responsive_read_req;
	bool m_non_responsive_write_req;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//next to send packets
private:
	CVLCPacket* m_next_to_send_read_reply;		//Next read reply packet to send
	CVLCPacket* m_next_to_send_site_info_reply;	//Next Site info reply packet to send
	CVLCDateTime m_timestamp_last_read_request;
	CVLCDateTime m_timestamp_last_site_request;
	CVLCPacket* m_next_ack_nack;
	CVLCPacket* m_next_transaction_request;
	CVlcData*   m_next_read_reply_source;				
	CVlcData*   m_next_site_info_reply_source;
	CVlcData*   m_next_trans_request_source;
	CVlcData*	m_last_sent_source;
	
	bool CheckRequiredDelay(CVLCPacket* p_packet);
	void SetNextReadReply(CVLCPacket* p_packet, CVlcData* p_source);
	void SetNextSiteInfoReply(CVLCPacket* p_packet, CVlcData* p_source);
	void SetNextTransactionRequest(CVLCPacket* p_packet, CVlcData* p_source);
	void ClearTransactionRequest();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Polls, read requests and responses
public:
	void HandlePoll( const CVLCPacket* p_poll );
	
private:
	CVLCPacket* m_poll;					//Last poll command received.
	CVLCPacket* m_status_response;		//Next status response to send.
	CVLCPacket* m_status_response_no_change; //Status response with exception code 0x00 - no errors set for this
	bool m_previous_v_bit;				//false=>off, true=>on
	bool m_awaiting_v_ack;				//true if a status response has been sent, awaiting v bit to toggle in the next poll
	bool m_sent_queued;					//the previously sent response was from a queued exception rather than no-response
	bool m_disabled;						//The previously sent queued exception had d bit set 
	bool m_send_queued_exceptions;	//Flag as set in exceptions dialog saying whether to send the exceptions or not
	void SendStatusResponse(bool p_f_bit);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read Requests and responses
public:
	void HandleReadRequest();
	void HandleWriteRequest();
	void HandleTransactionReply();
	void HandleAckNack();	
private:
	 void SendDataPacket();
	 void SendDataPacket(CVLCPacket* p_packet);
	 void SendAckNack() ;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exceptions
public:
	typedef std::list<ExceptionData> ExceptionList;
	void QueueException(ExceptionData& p_exception_data);
	void UpdateException(ExceptionData& p_exception_data, int p_idx);
	void RemoveFrontException();
	void RemoveException(int p_idx);
	const ExceptionList& GetExceptionQueue() {return m_queued_exceptions;} 
	void SetSendQueuedExceptions(bool p_val) {m_send_queued_exceptions = p_val;}
	bool GetSendQueuedExceptions() { return m_send_queued_exceptions;}
	void SetErrorsStatusResponse(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors);
private:
	std::list<ExceptionData>  m_queued_exceptions;
///////////////////////////////////////////////////////////////////////////////////
private:
	CVlcDatetimeMSData*		m_datetime_ms_data;
	CVlcControlData*		m_control_data;
	CVlcConfigurationData*	m_configuration_data;
	CVlcMonitorData*		m_monitor_cur_data;
	CVlcMonitorData*		m_monitor_end_data;
	CVlcTransactionData*	m_transaction_data;
	CVlcEventLogData*		m_event_log_data;
public:
	CVlcData* GetDatetimeMSData()		{ return (CVlcData*)m_datetime_ms_data;}
	CVlcData* GetControlData()			{ return (CVlcData*)m_control_data;}
	CVlcData* GetConfigurationData()	{ return (CVlcData*)m_configuration_data;}
	CVlcData* GetMonitorCurData()		{ return (CVlcData*)m_monitor_cur_data;}
	CVlcData* GetMonitorEndData()		{ return (CVlcData*)m_monitor_end_data;}
	CVlcData* GetTransactionData()		{ return (CVlcData*)m_transaction_data;}
	CVlcData* GetEventLogData()			{ return (CVlcData*)m_event_log_data;}
	
public:
	uint GetCurrentCredits(); 
	bool IsFullyConfigured();
//	CVlcControlData::EMode GetMode();
	void DoMasterReset();

protected:
	//void ProcessTimeout( int p_id ); // for ITimer
	//void ProcessNotify( int p_id ); // for INotifiable
		
};

////////////////////////////////////////////////////////////////////////////////////

class CVlcData : public INotifiable
{
protected:
	//packet data
	CVLCPacket* m_default;		//default for next to send
	CVLCPacket* m_next_to_send;
	CVLCPacket* m_last_sent_read_reply;
	CVLCPacket* m_last_sent_site_info_reply;
	CVLCPacket* m_last_received;
	CVLCPacket* m_ack_nack;		//packet to sent in response to received packet

	//response to next received
	CVLCPacket::EResponse m_response_to_next_received; //As set from the dialog
	
	//terminal id that this data belongs to
	CVlcId* m_vlc_id;

	CVLCPacket* InitialiseReadReply(int p_size); 
	void InitialiseAckNack();
	virtual void Initialise()=0;
	virtual void ProcessNotify( int p_id );  // for INotifiable
	
	
public:
	CVlcData(CVlcId* p_vlc_id);
	virtual ~CVlcData();

	CVLCPacket* GetDefault()				const { return m_default; }
	CVLCPacket* GetNextToSend()				const {return m_next_to_send;}
	CVLCPacket* GetLastSentReadReply()		const {return m_last_sent_read_reply;}
	CVLCPacket* GetLastSentSiteReply()		const {return m_last_sent_site_info_reply;}
	CVLCPacket* GetLastReceived()			const {return m_last_received;}

	void SetNextToSend(const CVLCPacket& p_packet) {*m_next_to_send = p_packet;}
	CString GetId();

	void SetResponseToNextReceived(CVLCPacket::EResponse p_response) {m_response_to_next_received = p_response;}
	CVLCPacket::EResponse GetResponseToNextReceived() const {return m_response_to_next_received;}

	virtual void HandleReadRequest();
	virtual void HandleWriteRequest();
	virtual void HandleAckNack();
	virtual void UpdatePacketOnSend(); //This will get at the point when a packet is sent.
    
	virtual void SetErrorsReadReply(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors);
	virtual void SetErrorsAckNack  (CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors);

	//Some of the fields in the default packet to be updated when a packet is manually set via 'set next' button
	//thats what this does
	virtual void UpdateDefault(){} 
};

///////////////////////////////////////////////////////////////////////////////////

class CVlcDatetimeMSData : public CVlcData, public ITimer
{
protected:

	void Initialise();  //override for CVlcData
	bool m_use_current_date_time;
	int m_memsig_timer;

public:
	CVlcDatetimeMSData(CVlcId* p_vlc_id);

	void HandleReadRequest();	//override for CVlcData
	void HandleWriteRequest();  //override for CVlcData
	void UpdatePacketOnSend();		//override for CVlcData

	bool GetUseCurrentDateTime() {return m_use_current_date_time;}
	void SetUseCurrentDateTime(bool p_val) {m_use_current_date_time = p_val;}

	
	void UpdateDefault(); //override for CVlcData

	void CalcMemSig(uint p_divisor);
	void ProcessTimeout( int p_id ); // for ITimer
};


/////////////////////////////////////////////////////////////////////////////////////

class CVlcControlData : public CVlcData
{
public:
	enum EMode
	{
		MODE_NORMAL			= 0x00,
		MODE_FULL_CGF		= 0x20,
		MODE_DEMO			= 0x40,
		MODE_HOST_DISABLE	= 0x60
	};

protected:

	enum
	{
		MODE_BITS	= 0xE0,
		SPT_BIT		= 0x02,
		DPT_BIT		= 0x01
	};

	void Initialise();  //override for CVlcData
	EMode m_mode;
	uint m_game_key_enable_count;
	uint m_divisor;

public:
	CVlcControlData(CVlcId* p_vlc_id);

	void HandleReadRequest();	//override for CVlcData
	void HandleWriteRequest();  //override for CVlcData

	EMode GetMode() {return m_mode;}
	void GetModeAsString(CString& p_mode);
	uint GetGameKeyEnableCount() {return m_game_key_enable_count;}
};


/////////////////////////////////////////////////////////////////////////////////////

class CVlcConfigurationData : public CVlcData
{
protected:

	typedef std::list<CVLCPacket*>::iterator PacketListItr;

	struct GameCfgData
	{
		uint m_game_key_bytes;
		uint m_bytes_extra;
		uint m_pkt_offset;
		uint m_game_id;
		uint m_game_type;
		//CByteArray m_parameters_extra;

		//Note - in order to include CByteArray in the struct, the list
		//must be a list of pointers to this struct rather than a list of
		//structs - otherwise it wont compile

		GameCfgData():
			m_game_key_bytes(999),
			m_bytes_extra(999),
			m_game_id(999),
			m_game_type(999),
			m_pkt_offset(999)
		{
			//m_parameters_extra.SetSize(0);
		}
	};

	void Initialise();  //override for CVlcData
	bool IsConfigured(GameCfgData& p_game_cfg_data, int& p_idx_out);

	bool m_fully_configured;
	uint m_count_of_games_configured;
	uint m_total_bytes_game_data;
	bool m_system_data_configured;
	std::vector<GameCfgData> m_game_cfg_data_list;


	//stuff for handling continuation packets
	bool m_expecting_continuation;
	bool m_o_bit_expected;
	uint m_total_cfg_data_bytes_expected;
	uint m_cfg_data_bytes_received;
	uint m_game_key_count;
	uint m_bytes_sys_config;
	void ResetContinuationStatus();
	ushort ProcessReceivedPacket();
	void BuildSendList();
	//This is the total config packet just received (includes all continuation packets concatenated into 1 packet)
	//Its data size may be over 256. 
	CVLCPacket* m_cfg_pkt_last_write_req; 

	//This is the list of packets to send in a read response when continuation packets are needed.
	std::list<CVLCPacket*> m_send_list;
	
	PacketListItr m_current_pkt_read_req_itr;
	PacketListItr m_current_pkt_site_info_req_itr;
	bool m_expected_o_bit_read_reply;
	bool m_expected_o_bit_site_info_reply;

public:
	CVlcConfigurationData(CVlcId* p_vlc_id);

	void HandleReadRequest();	//override for CVlcData
	void HandleWriteRequest();  //override for CVlcData
	void HandleAckNack();		//override for CVlcData

	bool IsFullyConfigured(){ return m_fully_configured;}
};

/////////////////////////////////////////////////////////////////////////////////////

class CVlcMonitorData : public CVlcData, public ITimer
{
public:
	enum EMonitorType
	{
		MONITOR_CURRENT=0,
		MONITOR_END
	};
protected:

	struct GameInfo
	{
		CString m_game_name;
		uint m_id;
		uint m_offset_pkt;

		GameInfo():
			m_game_name(CString("")),
			m_id(0),
			m_offset_pkt(0)
		{
		}
	};

	std::vector<GameInfo*> m_game_info_list;

	void Initialise();  //override for CVlcData
	EMonitorType m_monitor_type;
	uint m_game_count;
	uint m_credits;
	uint m_drop_box_amount;	
	uint m_active_game_id;
	bool GameExists(uint p_id, int& p_idx);

	void UpdatePacketOnSend(); //override for CVlcData

	int m_memsig_timer;
	bool m_memsig_calc_previously_done;

	void UpdateCashoutAmount(uint m_amount);
	

public:
	CVlcMonitorData(CVlcId* p_vlc_id, EMonitorType p_monitor_type);

	void HandleReadRequest();	//override for CVlcData
	void HandleWriteRequest();  //override for CVlcData

	uint GetCurrentCredits(){ return m_credits;}
	uint GetDropBox(){return m_drop_box_amount;}
	void AddGame(uint p_game_id, CString& p_game_name);
	void ResetGame(uint p_game_id);
	void ResetAllGames();
	uint GetGameCount() {return m_game_count;}

	void InCoinHopper(uint p_val);
	void InCoinDropBox(uint p_val);
	void OutCoinHopper(uint p_val);
	void OutManualPay(uint p_val);
	void CollectDropBox(uint p_val);
	void FillHopper(uint p_val);
	void PlayGame(uint p_game_id, uint p_bet, uint p_win);
	void DoSnapshot();
	void GetGameIdList( std::vector<uint> &p_id_list);
	void SetMasterResetDateTime();
	void SetControlVersion(uint p_val);
	void SetConfigVersion(uint p_val);
	void SetSystemMode(CByteArray& p_val);
	void SetAciveGameId(uint p_active_game_id); 
	void SetBaseRecord(uint p_record);
	void CalcMemSig(uint p_divisor);

	void UpdateDefault(); //override for CVlcData

	void ProcessTimeout( int p_id ); // for ITimer
};


/////////////////////////////////////////////////////////////////////////////////////

class CVlcEventLogData : public CVlcData
{

public:

	enum EEventType
	{
		EVENT_CASHOUT			= 0,
		EVENT_CASHOUT_CTN,
		EVENT_GAMEWIN,
		EVENT_GAMEWIN_CTN,
		EVENT_EXCEPTION,
		EVENT_CREDIT_SNAPSHOT,
		EVENT_MSG_TO_HOST,
		EVENT_STATUS_POLL_TIMEOUT,
		EVENT_FW_REV_CHANGE,
		EVENT_ENCRYPTION_KEY_CHANGE,
		EVENT_TIME_DATE,
		EVENT_RECORD_NO,
		EVENT_INVALID
	};


	struct EventRecord
	{
		int m_record_number;
		EEventType m_type;
		CByteArray m_data;

		EventRecord()
		{
			m_data.SetSize(CVLCPacket::LENGTH_EVENT_RECORD);
		}
	};

	typedef std::list<EventRecord*> EventLog;
	typedef std::list<EventRecord*>::iterator EventLogItr;
	typedef std::list<CVLCPacket*> PacketList;
	typedef std::list<CVLCPacket*>::iterator PacketListItr;

public:
	
	void AddEvent(CVLCPacket& p_packet, EEventType p_type );
	void AddEvent(EventRecord* p_event_record);
	int RecordCount() {return m_event_log.size(); }
	EventRecord* GetEventRecord(CVLCPacket& p_packet,EEventType p_type=EVENT_RECORD_NO );
	static CVLCPacket*  GetEventPacket(EventRecord& p_record);

	EventLogItr GetStartItr() {return m_event_log.begin();}
	EventLogItr GetEndItr() {return m_event_log.end();}

	void SetReadRecordStart(EventRecord* p_start);
	void SetReadRecordEnd(EventRecord* p_end);
	EventRecord* GetReadRecordStart() {return m_read_start;}
	EventRecord* GetReadRecordEnd() {return m_read_end;}
	void SetDefaultRead();
	int NumRecordsNextRead();
	int NumPacketsNextRead() { return m_next_to_send_list.size();}

	//todo - remove these
	void SetErrors();	//For multipacket response, apply errors to pkts as set by the combo
	void ViewErrors();  //Display Combo according to current error setting 

private:
	EventLog m_event_log;
	PacketList m_next_to_send_list;

	//These are the pointers as defined in protocol doc
	EventRecord* m_start;	
	EventRecord* m_base;
	EventRecord* m_end_next;
	
	//These are the pointers for the next to send.  If sending default then 
	//these will be set to the corresponding pointers above.
	EventRecord* m_read_start;
	EventRecord* m_read_end;	
	
	int m_next_record_number; 

	//For multi-packet reads, specifies which packet the errors are to be applied to
	uint m_apply_errors_to; //Value of 0 means all packets	

	//For tracking of the o bit
	bool m_expected_o_bit_read_reply;
	bool m_expected_o_bit_site_info_reply;

	

	CVLCPacket* BuildPacket(EventRecord* p_start, EventRecord* p_end );
	void BuildList(PacketList& p_list, EventRecord* p_start, EventRecord* p_end);
	void AppendToList(PacketList& p_list, EventRecord& p_record);

	
	PacketListItr m_current_pkt_read_req_itr;
	PacketListItr m_current_pkt_site_info_req_itr;
	void HandleAckNack(bool& p_expected_o_bit, PacketListItr& p_list_itr, CVLCPacket* p_ack);

protected:

	void Initialise();  //override for CVlcData

public:
	CVlcEventLogData(CVlcId* p_vlc_id);

	void HandleReadRequest();	//override for CVlcData
	void HandleWriteRequest(); //override for CVlcData
	void HandleAckNack();		//override for CVlcData

	//override for CVlcData
	void SetErrorsReadReply(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors);
};

/////////////////////////////////////////////////////////////////////////////////////
// m_next_to_send is the next transaction request (not a read reply)
// m_last_received is the transaction reply (not write request)
// m_last_sent_read_reply is the last transaction request sent (not last read reply sent)


class CVlcTransactionData : public CVlcData
{
protected:

	void Initialise();  //override for CVlcData
	void ProcessNotify( int p_id );  //Override for CVlcData - for INotifiable

public:
	CVlcTransactionData(CVlcId* p_vlc_id);

	void HandleReadRequest();	 //override for CVlcData
	void HandleWriteRequest();  //override for CVlcData
	void HandleAckNack();		 //override for CVlcData
	void SendTransactionRequest();
	void HandleTransactionReply();

	void SetCashoutAmount(uint m_amount);
};

#endif