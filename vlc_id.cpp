
#include "stdafx.h"
#include "VLCSim.h"
#include "stdtype.h"
#include "random.h"
#include "Utilities.h"
#include "vlc_id.h"
#include "vlc_id_list.h"
#include "vlc_packet.h"
#include "vlc_transmit.h"
#include "vlc_receive_thread.h"
#include "vlc_exceptions.h"
#include "dlg_exceptions.h"
#include "dlg_datetime_memsig.h"
#include "dlg_control.h"
#include "dlg_main.h"
#include "dlg_configuration.h"
#include "dlg_monitor.h"
#include "dlg_transactions.h"
#include "dlg_events.h"
#include "dlg_comms_errors.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction and initialisation

CVlcId::CVlcId()
{
	InitialiseData();
}


CVlcId::CVlcId( CString& p_id )
{
	m_id = p_id;
	InitialiseData();
}

CVlcId::CVlcId( const CVLCPacket* p_poll )
{
	p_poll->GetTerminalId(m_id); 
	InitialiseData();
	HandlePoll(p_poll);	
}	


void CVlcId::InitialiseData()
{
	//status flags
	m_is_active_id		= false;
	m_previous_v_bit	= false;
	m_awaiting_v_ack	= false;
	m_sent_queued		= false;
	m_disabled			= true;
	m_send_queued_exceptions = true;
	m_non_responsive_polls = false;
	m_non_responsive_read_req = false;
	m_non_responsive_write_req = false;

	m_poll							= NULL;

	//next to send pointers
	m_next_to_send_read_reply			= NULL;
	m_next_to_send_site_info_reply	= NULL;
	m_next_read_reply_source			= NULL;
	m_next_site_info_reply_source		= NULL;
	m_next_trans_request_source		= NULL;
	m_last_sent_source					= NULL;

	m_next_ack_nack						= NULL;
	m_next_transaction_request			= NULL;

	//Initialise 'no change' status response
	m_status_response_no_change = new CVLCPacket();
	m_status_response_no_change->SetTransmitDirection(CVLCPacket::SEND);

	//initialise all send packets
	m_status_response = new CVLCPacket();
	m_status_response->SetTransmitDirection(CVLCPacket::SEND);

	

	m_datetime_ms_data = new CVlcDatetimeMSData(this);
	m_control_data = new CVlcControlData(this);
	m_configuration_data = new CVlcConfigurationData(this);
	m_monitor_cur_data = new CVlcMonitorData(this, CVlcMonitorData::MONITOR_CURRENT);
	m_monitor_end_data = new CVlcMonitorData(this, CVlcMonitorData::MONITOR_END);
	m_transaction_data = new CVlcTransactionData(this);
	m_event_log_data = new CVlcEventLogData(this);
}

CVlcId::~CVlcId()
{
//Todo - finish this
	if(m_poll)
	{
		delete m_poll;
		m_poll=0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Polls and responses

void CVlcId::SetErrorsStatusResponse(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors)
{
	m_status_response->ClearAllErrors();
	m_status_response->SetTimingErrors(p_timing_errors);
	m_status_response->SetDataErrors(p_data_errors);
	m_status_response->SetSizeErrors(p_size_errors);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcId::HandlePoll( const CVLCPacket* p_poll )
{
	if(m_non_responsive_polls)
		return;

	//Todo
	//There isn't really any need to make a copy of the poll packet received
	if( !m_poll )
		delete m_poll;	//delete the previous one
	m_poll = new CVLCPacket( *p_poll );

	//todo
	//Also need to check d bit and i bit
	bool f_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_F);
	bool v_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_V);
	bool d_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_D);
	bool i_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_I);
	bool p_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_P);

	if(f_bit)
		SendStatusResponse(true);
	else
	{
		if(p_bit)
		{
			//todo - data packets should be sent chronologically
			if(m_next_to_send_read_reply != NULL)
			{
				if(!CheckRequiredDelay(m_next_to_send_read_reply))
					return;
				m_next_read_reply_source->UpdatePacketOnSend();
				SendDataPacket(m_next_to_send_read_reply);
				m_next_to_send_read_reply = NULL;
			}
			if(m_next_to_send_site_info_reply != NULL)
			{
				if(!CheckRequiredDelay(m_next_to_send_site_info_reply))
					return;
				m_next_site_info_reply_source->UpdatePacketOnSend();
				SendDataPacket(m_next_to_send_site_info_reply);
				m_next_to_send_site_info_reply = NULL;
			}
			if(m_next_transaction_request != NULL)
			{
				m_next_trans_request_source->UpdatePacketOnSend();
				SendDataPacket(m_next_transaction_request);
				m_next_transaction_request = NULL;
			}

			else if(m_next_ack_nack != NULL)
				SendAckNack();
			else
				SendStatusResponse(false);
		}
		else
		{
			if(m_next_ack_nack != NULL)
				SendAckNack();
			else
				SendStatusResponse(false);
		}
	}
	m_previous_v_bit=v_bit;
}

bool CVlcId::CheckRequiredDelay(CVLCPacket* p_packet)
{
	//If the required delay as specified by timing_errors->m_data_read_response_time	
	//has ellapsed then return true,
	//otherwise false
	if(p_packet == NULL)
		return false;
	CommsErrorsTiming* timing_errors = NULL; 
	CVLCDateTime time_stamp;
	if(p_packet == m_next_to_send_read_reply)
	{
		timing_errors = m_next_to_send_read_reply->GetTimingErrors();
		time_stamp = m_timestamp_last_read_request; 
	}
	else if (p_packet == m_next_to_send_site_info_reply)
	{
		timing_errors = m_next_to_send_site_info_reply->GetTimingErrors();
		time_stamp = m_timestamp_last_site_request;
	}
	else
		return false;
	if(timing_errors == NULL)
		return true;

	int required_delay = timing_errors->m_data_read_response_time;	
	CVLCDateTime current_time;  //defaults to current universal time
	if(current_time.Difference(time_stamp) <= required_delay )
			return false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Sends a status response as  required:
	- either previously send response that was NACKed.
	- or the exception code on the queue
	- of 'no response' if f bit set

*/

void CVlcId::SendStatusResponse(bool p_f_bit)
{
	bool v_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_V);
	bool d_bit = m_poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_D);

	if(m_awaiting_v_ack)
	{
		//A response was sent on the previous poll
		//Check to see if it has been acked (v bit toggled)
		if(v_bit == m_previous_v_bit)
		{
			//Not acked - re-send previous exception.
			m_awaiting_v_ack=true;
			g_vlc_transmit_manager->Send(m_status_response);
			m_status_response->ClearAllErrors();
			return;
		}
		else
		{
			//Any previously sent status response has been acked
			m_awaiting_v_ack = false;
			if(m_sent_queued)
			{	
				m_sent_queued=false;
				m_queued_exceptions.pop_front();
				if(m_is_active_id)
					g_dlg_exceptions->RemoveQueuedFront();
				//todo
				//This could be made as a notify
			}
		}
	}
	
	ushort command=0x0000;
	bool response_to_send=false;
	//int chk = g_dlg_exceptions->m_send_queued.GetCheck();
	//int sz = m_queued_exceptions.size();

	if( m_send_queued_exceptions &&
	    (m_queued_exceptions.size() > 0) )
	{
		ExceptionData exception_data = m_queued_exceptions.front();		
		command |= exception_data.code;
		if( (exception_data.d_bit)|| d_bit) //d bit is set in the poll or has been manually set - set d in the response
		{
			command |= CVLCPacket::CMD_FLG_D;
			m_disabled=true;
		}
		else
			m_disabled=false;
		m_sent_queued=true;
		response_to_send=true;
	}
	else if(p_f_bit)
	{
		//no queued exceptions=> send 'no-change'
		command |= 0x0000; //exception code for no-change
		//if(m_disabled)
		if(d_bit) //d bit is set in the poll - set it in the response
			command |= CVLCPacket::CMD_FLG_D;
		m_sent_queued=false;
		response_to_send=true;
	}

	if(response_to_send)
	{
		//build the response
		m_status_response->SetHeader(0x05);
		m_status_response->SetTrailer(0x0D);
		ushort id = m_poll->GetTerminalId();
		m_status_response->SetTerminalId(id);
		command |= CVLCPacket::CMD_STATUS_RESPONSE;
		command |= CVLCPacket::CMD_SRC_EGM;	
		if( (m_next_to_send_read_reply != NULL) ||
			 (m_next_to_send_site_info_reply != NULL) ||
			 (m_next_transaction_request != NULL) )
		{
			//buffered data packet ready to send - set b bit
			command |= CVLCPacket::CMD_FLG_B;
		}
		m_status_response->SetCommand(command);
		ushort crc = m_status_response->CalculateCrc();
		m_status_response->SetCrc(crc);
		m_awaiting_v_ack = true;
		if(!m_sent_queued) //sending a'no-change' response - don't include any errors in the packet or clear set errors
		{
			m_status_response_no_change->CopyPacketData(*m_status_response); //Doesn't include errors
			g_vlc_transmit_manager->Send(m_status_response_no_change);
		}
		else
		{
			g_vlc_transmit_manager->Send(m_status_response);
			m_status_response->ClearAllErrors();
		}
	}
}

void CVlcId::SendAckNack()
{
	if(m_next_ack_nack==NULL)
		return;
	m_next_ack_nack->SetCrc();
	g_vlc_transmit_manager->Send(m_next_ack_nack);
	m_next_ack_nack=NULL;
}

// Read Requests, responses, transactions
void CVlcId::SetNextReadReply(CVLCPacket* p_packet, CVlcData* p_source)
{
	m_next_to_send_read_reply = p_packet;
	m_next_read_reply_source = p_source;
	m_timestamp_last_read_request.SetAsUniversalTime();
}

void CVlcId::SetNextSiteInfoReply(CVLCPacket* p_packet, CVlcData* p_source)
{
	m_next_to_send_site_info_reply = p_packet;
	m_next_site_info_reply_source = p_source;
	m_timestamp_last_site_request.SetAsUniversalTime();
}

void CVlcId::SetNextTransactionRequest(CVLCPacket* p_packet, CVlcData* p_source)
{
	m_next_transaction_request = p_packet;	
	m_next_trans_request_source = p_source;
}

void CVlcId::ClearTransactionRequest()
{
	m_next_transaction_request = NULL;
}

void CVlcId::SendDataPacket()
{
	//todo - this isn't used anymore - can delete
	//At the moment - give priority for host request over site request.
	if(m_next_to_send_read_reply != NULL)
	{
		//set current date time in validation section
		//m_next_to_send_site_info_reply->SetCommandType(CVLCPacket::CMD_READ_REPLY);
		m_next_to_send_read_reply->SetValidationDateTime();
		m_next_to_send_read_reply->SetValidationCode();
		m_next_to_send_read_reply->SetCrc();
		g_vlc_transmit_manager->Send(m_next_to_send_read_reply);
		m_next_to_send_read_reply = NULL;
	}
	else if(m_next_to_send_site_info_reply != NULL)
	{
		//m_next_to_send_site_info_reply->SetCommandType(CVLCPacket::CMD_SITE_INFO_REPLY);
		m_next_to_send_site_info_reply->SetValidationDateTime();
		m_next_to_send_site_info_reply->SetValidationCode();
		m_next_to_send_site_info_reply->SetCrc();
		g_vlc_transmit_manager->Send(m_next_to_send_site_info_reply);
		m_next_to_send_site_info_reply = NULL;
	}
	else if(m_next_transaction_request != NULL)
	{
		
	}
}

void CVlcId::SendDataPacket(CVLCPacket* p_packet)
{
	if(p_packet==NULL)
		return;
	if(!p_packet->IsDataPacket())
		return;
	p_packet->SetValidationDateTime();
	p_packet->SetValidationCode();
	p_packet->SetCrc();
	g_vlc_transmit_manager->Send(p_packet);
	p_packet->ClearAllErrors();
}	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcId::HandleReadRequest()
{
	if(m_non_responsive_read_req)
		return;

	const CVLCPacket *read_request = g_receive_manager->GetLastPacket();

	ushort data_read_type = read_request->GetDataReadType();

	if(data_read_type == CVLCPacket::CMD_READ_DATETIME_MS)
		m_datetime_ms_data->HandleReadRequest();

	if(data_read_type == CVLCPacket::CMD_READ_CONTROL)
		m_control_data->HandleReadRequest();

	if(data_read_type == CVLCPacket::CMD_READ_CONFIGURE)
		m_configuration_data->HandleReadRequest();

	if(data_read_type == CVLCPacket::CMD_READ_MONITER_CUR)
		m_monitor_cur_data->HandleReadRequest();
	
	if(data_read_type == CVLCPacket::CMD_READ_MONITER_END)
		m_monitor_end_data->HandleReadRequest();

	if(data_read_type == CVLCPacket::CMD_READ_EVENT_LOG)
		m_event_log_data->HandleReadRequest();
}

void CVlcId::HandleWriteRequest()
{
	if(m_non_responsive_write_req)
		return;

	const CVLCPacket *write_request = g_receive_manager->GetLastPacket();

	ushort data_write_type = write_request->GetDataWriteType();

	if(data_write_type == CVLCPacket::CMD_WRITE_DATETIME_MS)
		m_datetime_ms_data->HandleWriteRequest();

	if(data_write_type == CVLCPacket::CMD_WRITE_CONTROL)
		m_control_data->HandleWriteRequest();
	
	//If its a continuation packet it must be for the config packet
	if( (data_write_type == CVLCPacket::CMD_WRITE_CONFIGURE) ||
	    (data_write_type == CVLCPacket::CMD_WRITE_CONTINUATION) ) 
		m_configuration_data->HandleWriteRequest();
}

void CVlcId::HandleTransactionReply()
{
	m_transaction_data->HandleTransactionReply();
}

void CVlcId::HandleAckNack()
{
	const CVLCPacket *ack_nack = g_receive_manager->GetLastPacket();
	
	ushort ack_type = ack_nack->GetAckNackCommandType();
	if(ack_type==CVLCPacket::CMD_ACK_READ_REPLY)
	{
		if(m_next_read_reply_source != NULL)
			m_next_read_reply_source->HandleAckNack();
	}
	else if(ack_type==CVLCPacket::CMD_ACK_SITE_INFO_REPLY)
	{
		if(m_next_site_info_reply_source != NULL)
			m_next_site_info_reply_source->HandleAckNack();
	}
	else if(ack_type==CVLCPacket::CMD_ACK_TRANSACTION_REQ)
	{
		if(m_next_trans_request_source != NULL)
			m_next_trans_request_source->HandleAckNack();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//Exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcId::QueueException(ExceptionData& pexception_data)
{
	m_queued_exceptions.push_back(pexception_data);
	int size = m_queued_exceptions.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcId::UpdateException(ExceptionData& p_exception_data, int p_idx)
{
	if(p_idx >= m_queued_exceptions.size() )
		return;

	//find the indexed item in the list
	int count=0;
	std::list<ExceptionData>::iterator itr = m_queued_exceptions.begin();
	while( itr != m_queued_exceptions.end() )
	{
		if(count==p_idx) 
			break;
		else
		{
			itr++;
			count++;
		}
	}	
	if(count==p_idx) 
		*itr = p_exception_data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcId::RemoveFrontException()
{
	if(m_queued_exceptions.size() >0 )
		m_queued_exceptions.pop_front();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcId::RemoveException(int p_idx)
{
	if(p_idx >= m_queued_exceptions.size() )
		return;

	//find the indexed item in the list
	int count=0;
	std::list<ExceptionData>::iterator itr = m_queued_exceptions.begin();
	while( itr != m_queued_exceptions.end() )
	{
		if(count==p_idx) 
			break;
		else
		{
			itr++;
			count++;
		}
	}	
	if(count==p_idx)
		m_queued_exceptions.erase(itr);	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
uint CVlcId::GetCurrentCredits()
{
	return m_monitor_cur_data->GetCurrentCredits();
}


bool CVlcId::IsFullyConfigured()
{
	return m_configuration_data->IsFullyConfigured();
}

/*
CVlcControlData::EMode GetMode()
{
	return m_control_data->GetMode();
}
*/

void CVlcId::DoMasterReset()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcData::CVlcData(CVlcId* p_vlc_id):
	m_default(NULL),
	m_next_to_send(NULL),
	m_ack_nack(NULL),
	m_last_sent_read_reply(NULL),
	m_last_sent_site_info_reply(NULL),
	m_last_received(NULL),
	m_response_to_next_received(CVLCPacket::RESP_AUTO),
	m_vlc_id(p_vlc_id)
{
}

CVlcData::~CVlcData()
{
	if(m_default!=NULL)
	{
		delete m_default;
		m_default = NULL;
	}
	if(m_next_to_send!=NULL)
	{
		delete m_next_to_send;
		m_next_to_send = NULL;
	}

	if(m_ack_nack != NULL)
	{
		delete m_ack_nack;
		m_ack_nack = NULL;
	}
}

CVLCPacket* CVlcData::InitialiseReadReply(int p_size)
{
	//Return a pointer to an initialised Data Read reply Packet
	CVLCPacket* packet = new CVLCPacket(p_size);
	
	packet->SetHeader(CVLCPacket::HEADER);
	packet->SetTerminalId(m_vlc_id->m_id);
	ushort command = 0;
	command |= CVLCPacket::CMD_READ_REPLY;
	command |= CVLCPacket::CMD_FLG_Q;	//Q bit is set => no packet to follow
	command |= CVLCPacket::CMD_SRC_EGM;
	command |= packet->CalculateNBits();
	packet->SetCommand(command);
	packet->SetTrailer(CVLCPacket::TRAILER);
	packet->SetTransmitDirection(CVLCPacket::SEND);
	return packet;
}

void CVlcData::InitialiseAckNack()
{
	//Set Ack/Nack
	m_ack_nack = new CVLCPacket(CVLCPacket::LENGTH_OVERHEAD); //length is 8
	m_ack_nack->SetHeader(CVLCPacket::HEADER);
	m_ack_nack->SetTerminalId(m_vlc_id->m_id);
	ushort command = 0;
	command |= CVLCPacket::CMD_ACK_NACK; //this sets bits ccc in the response!
	command |= CVLCPacket::CMD_ACK; //this sets bits aaaaa in the response initialised to ACK (no error)
	command |= CVLCPacket::CMD_ACK_WRITE_REQ; //sets bits kkk
	command |= CVLCPacket::CMD_FLG_Q; //Q bit is set => no packet to follow
	command |= CVLCPacket::CMD_SRC_EGM;
	m_ack_nack->SetCommand(command);
	m_ack_nack->SetCrc();
	m_ack_nack->SetTrailer(CVLCPacket::TRAILER);
	m_ack_nack->SetTransmitDirection(CVLCPacket::SEND);
}

void CVlcData::HandleReadRequest()
{
	const CVLCPacket *read_request = g_receive_manager->GetLastPacket();
	if(read_request==NULL)
		return;

	//check if any errors in read request
	byte errors = read_request->GetErrorStatus();
	if( errors != CVLCPacket::ERR_NONE)
		return;

	//q bit - ignore
	bool q_bit = read_request->IsCommandFlagSet(CVLCPacket::CMD_FLG_Q);
	
	//Check if read request or site info request) and update pkt to send as required
	ushort command_type = read_request->GetCommandType();
	if(command_type == CVLCPacket::CMD_READ_REQ)
	{
		m_next_to_send->SetCommandType(CVLCPacket::CMD_READ_REPLY);
		m_vlc_id->SetNextReadReply(m_next_to_send, this);
		g_notify_mgr->Register(CNotify::SENT_VLC_PACKET,this); //be notified when this packet has finished being sent
	}
	if(command_type == CVLCPacket::CMD_SITE_INFO_REQ)
	{
		m_next_to_send->SetCommandType(CVLCPacket::CMD_SITE_INFO_REPLY);
		m_vlc_id->SetNextSiteInfoReply(m_next_to_send, this);
		g_notify_mgr->Register(CNotify::SENT_VLC_PACKET,this);
	}
}

void CVlcData::ProcessNotify( int p_id )  //Notification that a packet has finished being sent
{
//ToDo - will need to override this for transaction requests.
	if(p_id==CNotify::SENT_VLC_PACKET)
	{
		g_notify_mgr->DeRegister(p_id, this);	
		CVLCPacket* last_sent = g_vlc_transmit_manager->GetLastPacket();
		if(last_sent==NULL)
			return;
		ushort command_type = last_sent->GetCommandType();
		if(last_sent->GetDataType() == m_next_to_send->GetDataType()) //finished sending a read reply
		{	
			m_next_to_send->ClearAllErrors(); //Clear the error settings in Next to send
			
			if(command_type == CVLCPacket::CMD_READ_REPLY)
			{
				if(m_last_sent_read_reply != NULL)
					delete m_last_sent_read_reply;
				m_last_sent_read_reply = new CVLCPacket(*last_sent);
				m_last_sent_read_reply->SetResponse(CVLCPacket::RESP_NONE);	
			}
			if(command_type == CVLCPacket::CMD_SITE_INFO_REPLY)
			{
				if(m_last_sent_site_info_reply != NULL)
					delete m_last_sent_site_info_reply;
				m_last_sent_site_info_reply = new CVLCPacket(*last_sent);
				m_last_sent_site_info_reply->SetResponse(CVLCPacket::RESP_NONE);	
			}
			return;
		}
		if(command_type == CVLCPacket::CMD_ACK_NACK) //finished sending an ACK NACK
			m_ack_nack->ClearAllErrors(); //Clear the error settings for Ack Nack
	}	
}

void CVlcData::HandleWriteRequest()
{
	//Packet has arrived.
	const CVLCPacket *write_request = g_receive_manager->GetLastPacket();

	//To do - check q bit

	CVLCPacket::EResponse response = CVLCPacket::RESP_ACK;
	ushort ack_code = CVLCPacket::CMD_ACK;

	if( (m_response_to_next_received != CVLCPacket::RESP_AUTO) &&
		(m_response_to_next_received != CVLCPacket::RESP_NONE) )
	{
		ack_code = CVLCPacket::GetAckCodeFromResponse(m_response_to_next_received);
		response = m_response_to_next_received;
	}

	else if(m_response_to_next_received == CVLCPacket::RESP_AUTO)
	{
		byte errors = write_request->GetErrorStatus();	
		if(errors & CVLCPacket::ERR_CODE)
		{
			ack_code = CVLCPacket::CMD_NACK_CODE;
			response = CVLCPacket::RESP_NACK_CODE;
		}
		else if(errors & CVLCPacket::ERR_TIMESTAMP)
		{
			ack_code = CVLCPacket::CMD_NACK_TIMESTAMP;
			response = CVLCPacket::RESP_NACK_TIMESTAMP;
		}
		else if(errors & CVLCPacket::ERR_CRC)
		{
			ack_code = CVLCPacket::CMD_NACK_CRC;
			response = CVLCPacket::RESP_NACK_CRC;
		}
		else if( (errors & CVLCPacket::ERR_COMMAND)		||
				 (errors & CVLCPacket::ERR_LENGTH)		||
				 (errors & CVLCPacket::ERR_LENGTH)		||
				 (errors & CVLCPacket::ERR_LENGTH_MAX)  ||
				 (errors & CVLCPacket::ERR_TRAILER) )
		{
			ack_code = CVLCPacket::CMD_NACK_COMMAND;
			response = CVLCPacket::RESP_NACK_COMMAND;
		}
		else
		{
			ack_code = CVLCPacket::CMD_ACK;
			response = CVLCPacket::RESP_ACK;
		}
	}
	else if(m_response_to_next_received == CVLCPacket::RESP_NONE)
	{
		ack_code = CVLCPacket::CMD_ACK; //not used
		response = CVLCPacket::RESP_NONE;
	}

	//copy into m_last_received
	if(m_last_received != NULL)
		delete m_last_received;
	m_last_received = new CVLCPacket(*write_request);
	m_last_received->SetResponse(response);

	//Send the Ack/Nack
	m_ack_nack->SetAckNackType(ack_code);
	if(m_response_to_next_received != CVLCPacket::RESP_NONE)
	{
		m_vlc_id->m_next_ack_nack = m_ack_nack;
		g_notify_mgr->Register(CNotify::SENT_VLC_PACKET,this); //notify when finished sending it
	}
	else
		m_vlc_id->m_next_ack_nack = NULL;
}

void CVlcData::HandleAckNack()
{
	const CVLCPacket *ack_nack = g_receive_manager->GetLastPacket();
	if(ack_nack==NULL)
		return;
	if(ack_nack->GetCommandType() != CVLCPacket::CMD_ACK_NACK)
		return;
	ushort ack_type = ack_nack->GetAckNackCommandType();
	ushort ack_code = ack_nack->GetAckNackCode();
	CVLCPacket::EResponse response = CVLCPacket::GetResponseFromAckCode(ack_code);
	
	if(ack_type==CVLCPacket::CMD_ACK_READ_REPLY)
	{
		if(m_last_sent_read_reply != NULL)
			m_last_sent_read_reply->SetResponse(response);
	}
	else if(ack_type==CVLCPacket::CMD_ACK_SITE_INFO_REPLY)
	{
		if(m_last_sent_site_info_reply != NULL)
			m_last_sent_site_info_reply->SetResponse(response);
	}
}

void CVlcData::UpdatePacketOnSend()
{
	m_next_to_send->SetValidationDateTime();
	m_next_to_send->SetValidationCode();
	m_next_to_send->SetCrc();
}

CString CVlcData::GetId()
{
	CString id_str;
	m_vlc_id->GetId(id_str);
	return id_str;
}

void CVlcData::SetErrorsReadReply(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors)
{
	m_next_to_send->ClearAllErrors();
	m_next_to_send->SetTimingErrors(p_timing_errors);
	m_next_to_send->SetDataErrors(p_data_errors);
	m_next_to_send->SetSizeErrors(p_size_errors);	
}
void CVlcData::SetErrorsAckNack(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors)
{
	m_ack_nack->ClearAllErrors();
	m_ack_nack->SetTimingErrors(p_timing_errors);
	m_ack_nack->SetDataErrors(p_data_errors);
	m_ack_nack->SetSizeErrors(p_size_errors);	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcDatetimeMSData::CVlcDatetimeMSData(CVlcId* p_vlc_vlc_id):
	CVlcData(p_vlc_vlc_id),
	m_use_current_date_time(true),
	m_memsig_timer(0)
{
	Initialise();
}


void CVlcDatetimeMSData::Initialise()
{	
	if(m_vlc_id==NULL)
		return;

	m_default = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_DATETIME);
	m_default->SetDataReadType(CVLCPacket::CMD_READ_DATETIME_MS);

	//Initialise data fields
	m_default->SetDataFieldFormatted(CString("05"),CVLCPacket::DT_FORMAT_ID);
	CVLCDateTime date_time;
	uint date = (uint)date_time.GetJulianDate();
	uint time = (uint)date_time.GetTimeInSeconds();
	m_default->SetDataFieldInt(date,CVLCPacket::DT_DATE);
	m_default->SetDataFieldInt(time,CVLCPacket::DT_TIME);
	m_default->SetDataFieldFormatted(CString("0000"), CVLCPacket::DT_SIG_VALUE);
	m_default->SetDataFieldFormatted(CString("0000"), CVLCPacket::DT_SIG_DIVISOR);
	m_default->PadZero(); 

	//Set Next to send (initialised to be the same as the default
	m_next_to_send = new CVLCPacket(*m_default);

	CVlcData::InitialiseAckNack();

	g_dlg_datetime_memsig->DisplayPacket(*m_default);
}

void CVlcDatetimeMSData::UpdateDefault()
{
	//Update mem sig value 
	ushort sig_val = m_next_to_send->GetDataFieldInt(CVLCPacket::DT_SIG_VALUE);
	m_default->SetDataFieldInt(sig_val, CVLCPacket::DT_SIG_VALUE);
}


void CVlcDatetimeMSData::HandleReadRequest()
{
	//ToDo - if it is sending the default packet, update the daly poll timeout hours
	CVlcData::HandleReadRequest();

	
	uint divisor = m_default->GetDataFieldInt(CVLCPacket::DT_SIG_DIVISOR);
	if(divisor==0) //divisor has not yet been set in pkt to request a calc - send 0 for the value
		m_next_to_send->SetDataFieldInt(0,CVLCPacket::DT_SIG_VALUE);
}


void CVlcDatetimeMSData::HandleWriteRequest()
{
	CVlcData::HandleWriteRequest();

	//Update default packet
	const CVLCPacket *write_request = g_receive_manager->GetLastPacket();
	int ack_code = m_ack_nack->GetAckNackCode();
	if(ack_code == CVLCPacket::CMD_ACK)
	{
		ushort date = write_request->GetDataFieldInt(CVLCPacket::DT_DATE);
		ushort time = write_request->GetDataFieldInt(CVLCPacket::DT_TIME); 

		//Update the universal date/time with the values received.
		//Might want to send a notify for this - updating of universal date time.
		CVLCDateTime date_time;
		date_time.Set(date,time); 
		CVLCDateTime::SetUniversalDateTime(date_time); 	

		//If divisor is non-zero update m_default and m_next_to_send with the new value.
		ushort divisor = write_request->GetDataFieldInt(CVLCPacket::DT_SIG_DIVISOR);
		
		if(divisor != 0)
		{
			m_default->SetDataFieldInt(divisor,CVLCPacket::DT_SIG_DIVISOR);
			m_next_to_send->SetDataFieldInt(divisor,CVLCPacket::DT_SIG_DIVISOR);
			CalcMemSig(divisor);
		}
	}	
	//Display the packet received
	g_dlg_datetime_memsig->DisplayPacket(*write_request);
}

void CVlcDatetimeMSData::UpdatePacketOnSend()
{
	if(m_use_current_date_time)
	{
		CVLCDateTime date_time; //this is the universal date time
		uint date = (uint)date_time.GetJulianDate();
		uint time = (uint)date_time.GetTimeInSeconds();
		m_next_to_send->SetDataFieldInt(date,CVLCPacket::DT_DATE);
		m_next_to_send->SetDataFieldInt(time,CVLCPacket::DT_TIME);
	}
	CVlcData::UpdatePacketOnSend();
}


void CVlcDatetimeMSData::CalcMemSig(uint p_divisor)
{
	uint sig_val = 0xFFFF;
	m_next_to_send->SetDataFieldInt(sig_val,CVLCPacket::DT_SIG_VALUE);
	if(m_memsig_timer!=0)
		g_timers->RemoveTimer(m_memsig_timer); //kill that timer and restart
	m_memsig_timer = g_timers->AddTimer( 10000, this ); //wait 10 seconds
}

void CVlcDatetimeMSData::ProcessTimeout( int p_id )
{
	if(p_id == m_memsig_timer)
	{
		uint sig_val = m_default->GetDataFieldInt(CVLCPacket::DT_SIG_VALUE);
		m_next_to_send->SetDataFieldInt(sig_val,CVLCPacket::DT_SIG_VALUE);
		g_timers->RemoveTimer(p_id);
		m_memsig_timer=0;

		//queue memsig complete exception
		ushort code = g_exception_manager->GetExceptionCode(CString("02"));
		ExceptionData memsig_complete_ex(code,false);
		m_vlc_id->QueueException(memsig_complete_ex);
		g_dlg_exceptions->DisplayQueue();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcControlData::CVlcControlData(CVlcId* p_vlc_vlc_id):
	CVlcData(p_vlc_vlc_id),
	m_mode(MODE_DEMO),
	m_game_key_enable_count(0),
	m_divisor(0)
{
	Initialise();
}


void CVlcControlData::Initialise()
{	
	if(m_vlc_id==NULL)
		return;

	m_default = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_CONTROL);
	m_default->SetDataReadType(CVLCPacket::CMD_READ_CONTROL);

	//Initialise data fields
	m_default->SetDataFieldFormatted(CString("05"),CVLCPacket::CRTL_FORMAT_ID);
	m_default->SetDataFieldFormatted(CString("40"),CVLCPacket::CTRL_TERMINAL_MODE);
	m_default->PadZero();
	//Set Next to send (initialised to be the same as the default
	m_next_to_send = new CVLCPacket(*m_default);

	CVlcData::InitialiseAckNack(); 

	g_dlg_configuration->DisplayPacket(*m_default);
}

void CVlcControlData::HandleReadRequest()
{
	CVlcData::HandleReadRequest();
}



void CVlcControlData::HandleWriteRequest()
{
	CVlcData::HandleWriteRequest(); //sets up m_last_reseived, and saves the default response in that packet

	CVLCPacket::EResponse response = m_last_received->GetResponse();
	if(response != CVLCPacket::RESP_ACK) //packet already NACKed, so don't need to check data or act on data
		return;							 //RESP_NONE => same as NACKing data

	ushort new_ack_code = CVLCPacket::CMD_ACK;
	CVLCPacket::EResponse new_response = CVLCPacket::RESP_ACK;
	uint code_pkt = m_last_received->GetDataFieldInt(CVLCPacket::CTRL_TERMINAL_MODE);
	EMode mode_pkt = (EMode)(code_pkt&MODE_BITS);

	if(m_response_to_next_received == CVLCPacket::RESP_AUTO)	//if ACK was selected in dialog then assume data is correct - don't check
	{
		//check the data in the control packet - depend on the mode
		if(mode_pkt == MODE_DEMO)
		{
			if( m_mode != MODE_DEMO)
			{
				new_ack_code = CVLCPacket::CMD_NACK_CONFIGURATION;
				new_response = CVLCPacket::RESP_NACK_CONFIGURATION;
			}
		}

		if(mode_pkt == MODE_FULL_CGF)
		{
			if(m_vlc_id->GetCurrentCredits() !=0 )
			{
				new_ack_code = CVLCPacket::CMD_NACK_NON_ZERO_BALANCE;
				new_response = CVLCPacket::RESP_NACK_NON_ZERO_BALANCE;
			}	
		}
	
		if(mode_pkt == MODE_NORMAL)
		{
			if(!m_vlc_id->IsFullyConfigured() )
			{
				new_ack_code = CVLCPacket::CMD_NACK_CONFIGURATION;
				new_response = CVLCPacket::RESP_NACK_CONFIGURATION;
			}
		}
	}

	m_last_received->SetResponse(new_response);
	//Update Ack Nack
	m_ack_nack->SetAckNackType(new_ack_code);
	m_vlc_id->m_next_ack_nack = m_ack_nack;

	if(new_response != CVLCPacket::RESP_ACK)
		return; //don't act on the packet received

	
	//set master reset date time in monitor
	//To do 
	//Implement mode changes / config changes with notifications.
	if( (m_mode == MODE_DEMO)&&(mode_pkt != MODE_DEMO))
	{
		//m_vlc_id->DoMasterReset(); //exit out of demo => master reset
		m_vlc_id->m_monitor_cur_data->SetMasterResetDateTime();
	}

	//update mode
	m_mode = mode_pkt;
	if(m_vlc_id->IsActiveId())
		g_dlg_main->DisplayActiveIdStatus();
		
	//Update m_default and next to send
	m_default->CopyPacketData(*m_last_received);
	m_next_to_send->CopyPacketData(*m_default);

	//count game keys enabled and update
	CByteArray game_key_flags;
	m_last_received->GetDataFieldHex(game_key_flags, CVLCPacket::CTRL_GAME_KEY_ENABLE_FLAGS);
	ASSERT(game_key_flags.GetSize() == 8);
	int count=0;
	for(int i=0; i<8; i++)
	{
		byte mask = 0x80;
		byte cur_byte = game_key_flags[i];
		while(mask !=0x00)
		{
			if(cur_byte & mask)
				count++;
			mask >>= 1;
		} 
	}
	m_game_key_enable_count = count;
	

	//update event read record in monitor packet
	uint record = m_last_received->GetDataFieldInt(CVLCPacket::CTRL_FROM);
	m_vlc_id->m_monitor_cur_data->SetBaseRecord(record);

	//update control version in monitor packet
	uint control_version = m_last_received->GetDataFieldInt(CVLCPacket::CRTL_VERS);
	m_vlc_id->m_monitor_cur_data->SetControlVersion(control_version);

	//update system mode in monitor packet
	CByteArray sys_mode;
	sys_mode.SetSize(2);
	sys_mode[0] = (uint)mode_pkt >> 5;
	sys_mode[1] = 0;
	m_vlc_id->m_monitor_cur_data->SetSystemMode(sys_mode);

	//See if divisor has changed.
	uint divisor_pkt = m_last_received->GetDataFieldInt(CVLCPacket::CTRL_MEM_SIG_DIVISOR);
	if( (divisor_pkt!=0) && ( divisor_pkt != m_divisor ) )
	{
		m_vlc_id->m_monitor_cur_data->CalcMemSig(divisor_pkt);
		m_divisor = divisor_pkt;
	}
	//Display the packet received
	g_dlg_control->DisplayPacket(*m_last_received);
}

void CVlcControlData::GetModeAsString(CString& p_mode)
{
	switch(m_mode)
	{
		case MODE_NORMAL:		p_mode = CString("Mode: Normal");		return;
		case MODE_FULL_CGF:		p_mode = CString("Mode: Full Cfg");		return;
		case MODE_DEMO:			p_mode = CString("Mode: Demo");			return;
		case MODE_HOST_DISABLE: p_mode = CString("Mode: Host Disable"); return;
	};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcConfigurationData::CVlcConfigurationData(CVlcId* p_vlc_vlc_id):
	CVlcData(p_vlc_vlc_id),
	m_fully_configured(false),
	m_count_of_games_configured(0),
	m_total_bytes_game_data(0),
	m_system_data_configured(false),
	m_expecting_continuation(false),
	m_o_bit_expected(false), //o bit should be 0 in the first packet received
	m_total_cfg_data_bytes_expected(0),
	m_cfg_data_bytes_received(0),
	m_game_key_count(0),
	m_bytes_sys_config(0),
	m_cfg_pkt_last_write_req(NULL),
	m_expected_o_bit_read_reply(false),
	m_expected_o_bit_site_info_reply(false)
{
	Initialise();
	m_game_cfg_data_list.reserve(3);
}


void CVlcConfigurationData::Initialise()
{	
	if(m_vlc_id==NULL)
		return;

	m_default = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_CONFIGURATION);
	m_default->SetDataReadType(CVLCPacket::CMD_READ_CONFIGURE);

	//Initialise data fields
	m_default->SetDataFieldFormatted(CString("05"),CVLCPacket::CRTL_FORMAT_ID);
	m_default->SetDataFieldInt(108, CVLCPacket::CFG_OH_BYTES_SYSTEM);
	m_default->PadZero();

	//Set Next to send (initialised to be the same as the default
	m_next_to_send = new CVLCPacket(*m_default);

	CVlcData::InitialiseAckNack();
	BuildSendList();
	g_dlg_configuration->DisplayPacket(*m_default);
}

void CVlcConfigurationData::HandleReadRequest()
{
	//CVlcData::HandleReadRequest();

	const CVLCPacket *read_request = g_receive_manager->GetLastPacket();
	if(read_request==NULL)
		return;

	//check if any errors in read request
	byte errors = read_request->GetErrorStatus();
	if( errors != CVLCPacket::ERR_NONE)
		return;

	if(m_send_list.size()==0)
		return;

	CVLCPacket* pkt = m_send_list.front();
	
	ushort command_type = read_request->GetCommandType();
	if(command_type == CVLCPacket::CMD_READ_REQ)
	{
		pkt->SetCommandType(CVLCPacket::CMD_READ_REPLY);
		m_vlc_id->SetNextReadReply(pkt,this);
		m_current_pkt_read_req_itr = m_send_list.begin();
		m_expected_o_bit_read_reply=false;
	}
	if(command_type == CVLCPacket::CMD_SITE_INFO_REQ)
	{	
		pkt->SetCommandType(CVLCPacket::CMD_SITE_INFO_REPLY);
		m_vlc_id->SetNextSiteInfoReply(pkt, this);
		m_current_pkt_site_info_req_itr = m_send_list.begin();
		m_expected_o_bit_site_info_reply=false;
	}
}

void CVlcConfigurationData::HandleAckNack()
{
	const CVLCPacket *ack = g_receive_manager->GetLastPacket();
	if(ack==NULL)
		return;

	ushort ack_type = ack->GetAckNackCommandType();	//read reply of site info reply.
	if( (ack_type!=CVLCPacket::CMD_ACK_READ_REPLY)&&(ack_type!=CVLCPacket::CMD_ACK_SITE_INFO_REPLY))
		return;
	
	ushort ack_code = ack->GetAckNackCode();

	//todo - set the ack code for the last packet sent - save last packet sent in history log etc

	bool o_bit = ack->IsCommandFlagSet(CVLCPacket::CMD_FLG_O);

	if(ack_type==CVLCPacket::CMD_ACK_READ_REPLY)
	{
		if(ack_code!=CVLCPacket::CMD_ACK)
		{
			m_expected_o_bit_read_reply=false;
			return;
		}
		if(o_bit != m_expected_o_bit_read_reply)
		{
			//resend current packet.
			CVLCPacket* pkt = *m_current_pkt_read_req_itr;
			m_vlc_id->SetNextReadReply(pkt,this);
		}
		else
		{
			m_current_pkt_read_req_itr++;
			if(m_current_pkt_read_req_itr==m_send_list.end())
			{
				m_expected_o_bit_read_reply=false;
				return;
			}
			else
			{
				CVLCPacket* pkt = *m_current_pkt_read_req_itr;
				pkt->SetCommandType(CVLCPacket::CMD_READ_REPLY);
				m_vlc_id->SetNextReadReply(pkt,this);
				m_expected_o_bit_read_reply = !m_expected_o_bit_read_reply;
			}
		}
	}
	if(ack_type==CVLCPacket::CMD_ACK_SITE_INFO_REPLY)
	{
		if(ack_code!=CVLCPacket::CMD_ACK)
		{
			m_expected_o_bit_site_info_reply=false;
			return;
		}
		if(o_bit != m_expected_o_bit_site_info_reply)
		{
			//resend current packet.
			CVLCPacket* pkt = *m_current_pkt_site_info_req_itr;
			m_vlc_id->SetNextSiteInfoReply(pkt,this);
		}
		else
		{
			m_current_pkt_site_info_req_itr++;
			if(m_current_pkt_site_info_req_itr==m_send_list.end())
			{
				m_expected_o_bit_site_info_reply=false;
				return;
			}
			else
			{
				CVLCPacket* pkt = *m_current_pkt_site_info_req_itr;
				pkt->SetCommandType(CVLCPacket::CMD_SITE_INFO_REPLY);
				m_vlc_id->SetNextSiteInfoReply(pkt,this);
				m_expected_o_bit_site_info_reply = !m_expected_o_bit_site_info_reply;
			}
		}	
	}	
}

void CVlcConfigurationData::BuildSendList()
{
	//This function builds m_send_list based on the data in m_next_to_send.
	//i.e. if m_next_to_send has > 256 bytes, then a list of packets to send is build

	//first clear the list.
	std::list<CVLCPacket*>::iterator itr = m_send_list.begin();
	while(itr != m_send_list.end() )
	{
		if(*itr != NULL)
			delete (*itr);
		itr++;
	}
	m_send_list.clear();
	
	int tot_data_length = m_next_to_send->GetDataLength();
	if(tot_data_length == 0)
		return;
	int packets_required = tot_data_length / CVLCPacket::LENGTH_DATA_MAX;
	if(tot_data_length % CVLCPacket::LENGTH_DATA_MAX != 0) 
		packets_required++; 

	CVLCPacket* pkt;
	if(packets_required==1)
	{
		pkt = new CVLCPacket(*m_next_to_send);
		m_send_list.push_back(pkt);
		return;
	}

	//multiple packets required...
	int offset = CVLCPacket::START_DATA;
	CByteArray packet_data;

	for(int pkt_num=1; pkt_num<=packets_required; pkt_num++)
	{
		int data_length;
		bool last_pkt = (pkt_num == packets_required);
		if(!last_pkt)
			data_length = CVLCPacket::LENGTH_DATA_MAX;
		else 
		{
			data_length = tot_data_length - offset;
			ASSERT(data_length > 0);
			ASSERT(data_length <= CVLCPacket::LENGTH_DATA_MAX);
			if(data_length <= 0) return;
			if(data_length > CVLCPacket::LENGTH_DATA_MAX) return;
		}
		pkt = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_OVERHEAD + CVLCPacket::LENGTH_VALIDATION + data_length);
		if(pkt_num==1)
			pkt->SetDataReadType(CVLCPacket::CMD_READ_CONFIGURE);
		else
			pkt->SetDataReadType(CVLCPacket::CMD_READ_CONTINUATION);
		if(!last_pkt)	
			pkt->SetCommandFlag( CVLCPacket::CMD_FLG_Q, false); 
		else
		{
			pkt->SetCommandFlag( CVLCPacket::CMD_FLG_Q, true);
			pkt->PadZero(); //should already be an even multiple of 8 (since m_next_to_send is)
		}
		
		m_next_to_send->GetData(packet_data, offset, data_length);
		bool res = pkt->SetData(packet_data, CVLCPacket::START_DATA);
		ASSERT(res==true);
		if(!res) return;
		m_send_list.push_back(pkt);
		offset += data_length;
	}
	ASSERT(m_send_list.size() == packets_required);

	//Set errors - just set in the last packet if there are any.
	//the error settings are stored in m_next_to_send
	CommsErrorsTiming* timing_errors  = NULL;
	CommsErrorsCorruptData* data_errors = NULL;
	CommsErrorsDataSize* size_errors = NULL;

	if(m_next_to_send->GetTimingErrors() != NULL)
	{
		timing_errors = new(CommsErrorsTiming);
		*timing_errors = *(m_next_to_send->GetTimingErrors());
	}
	if(m_next_to_send->GetDataErrors() != NULL)
	{
		data_errors = new(CommsErrorsCorruptData);
		*data_errors = *(m_next_to_send->GetDataErrors());
	}
	if(m_next_to_send->GetSizeErrors() != NULL)
	{
		size_errors = new(CommsErrorsDataSize);
		*size_errors = *(m_next_to_send->GetSizeErrors());	
	}
	
	pkt->SetTimingErrors(timing_errors);
	pkt->SetDataErrors(data_errors);
	pkt->SetSizeErrors(size_errors);
}

ushort CVlcConfigurationData::ProcessReceivedPacket()
{
	//Returns the Ack/Nack command code as approproate

	bool o_bit = (m_last_received->IsCommandFlagSet(CVLCPacket::CMD_FLG_O) );
	bool q_bit = (m_last_received->IsCommandFlagSet(CVLCPacket::CMD_FLG_Q) );

	if(o_bit != m_o_bit_expected) 
		return CVLCPacket::CMD_NACK_COMMAND; 

	if(m_last_received->GetDataWriteType() == CVLCPacket::CMD_WRITE_CONFIGURE)
	{
		if(m_expecting_continuation)
			return CVLCPacket::CMD_NACK_COMMAND; ; //error

		ResetContinuationStatus();

		//Initialise m_cfg_pkt_last_write_req to the packet just received
		m_cfg_pkt_last_write_req = new CVLCPacket(*m_last_received);

		//Calculate the total bytes of packet data expected
		int m_game_key_count = m_last_received->GetDataFieldInt(CVLCPacket::CFG_OH_COUNT_GAME);
		int m_bytes_sys_config = m_last_received->GetDataFieldInt(CVLCPacket::CFG_OH_BYTES_SYSTEM);
		m_total_cfg_data_bytes_expected =  CVLCPacket::LENGTH_CONFIGURATION_OH; 
		m_total_cfg_data_bytes_expected += m_game_key_count*2;
		m_total_cfg_data_bytes_expected += m_bytes_sys_config;
		for(int key=0; key<m_game_key_count; key++)
			m_total_cfg_data_bytes_expected += m_last_received->GetDataFieldInt(CVLCPacket::CFG_OH_BYTES_GAME_KEY_X, key*2);

		//see if this pkt holds all the config data or if continuation pkts needed
		m_cfg_data_bytes_received = m_last_received->GetDataLength();
		m_expecting_continuation = (m_cfg_data_bytes_received < m_total_cfg_data_bytes_expected);

		//force error
		if(m_expecting_continuation && (g_dlg_configuration->m_ctrl_chk_nack_first.GetCheck()==1))
		{
			g_dlg_configuration->m_ctrl_chk_nack_first.SetCheck(false);
			return CVLCPacket::CMD_NACK_CRC;
		}
	}

	else if( m_last_received->GetDataWriteType() == CVLCPacket::CMD_WRITE_CONTINUATION )
	{
		if(!m_expecting_continuation)	
			return CVLCPacket::CMD_NACK_COMMAND;

		int pkt_data_length = m_last_received->GetDataLength();
		m_cfg_data_bytes_received += pkt_data_length;
		m_expecting_continuation = (m_cfg_data_bytes_received < m_total_cfg_data_bytes_expected);

		//force error
		if(m_expecting_continuation && (g_dlg_configuration->m_ctrl_chk_nack_2nd.GetCheck()==1))
		{
			g_dlg_configuration->m_ctrl_chk_nack_2nd.SetCheck(false);
			return CVLCPacket::CMD_NACK_CRC;
		}

		//force error
		if(!m_expecting_continuation && (g_dlg_configuration->m_ctrl_chk_mess_with_o_bit.GetCheck()==1))
		{
			g_dlg_configuration->m_ctrl_chk_mess_with_o_bit.SetCheck(false);
			//this is for the last packet.
			//don't concatenate the packet and dont toggle the o bit in the ACK - expect this packet to be resent 
			m_expecting_continuation = true;
			m_cfg_data_bytes_received -= pkt_data_length;
			bool o_bit = !(m_last_received->IsCommandFlagSet(CVLCPacket::CMD_FLG_O) );	//opposite to what it should be
			m_ack_nack->SetCommandFlag(CVLCPacket::CMD_FLG_O, o_bit); 
			if(!q_bit) return CVLCPacket::CMD_NACK_COMMAND; //same as 'return q_bit'!
			else return CVLCPacket::CMD_ACK;
		}

		//concatenate the packet 
		CByteArray game_data;
		m_last_received->GetData(game_data, CVLCPacket::START_DATA,pkt_data_length);
		ASSERT(m_cfg_pkt_last_write_req != NULL);
		if(m_cfg_pkt_last_write_req == NULL)
			return CVLCPacket::CMD_NACK_COMMAND; ; //error
		int pos = m_cfg_pkt_last_write_req->OffsetValidation(); //the continuation pkt data is going to start here since there should be no padding.
			m_cfg_pkt_last_write_req->InsertData(game_data, pos);
	}

	else
		return CVLCPacket::CMD_NACK_COMMAND; ; //error

	//make sure q bit is not set if more packets expected.
	//update expected o bit
	if(m_expecting_continuation)
	{
		if(q_bit) //q bit should be 0 if more packets to come 
			return CVLCPacket::CMD_NACK_COMMAND;   //error
		m_o_bit_expected = !m_o_bit_expected;
	}
	else
	{
		if(!q_bit) //q bit should be 1 if this is the last packet
			return CVLCPacket::CMD_NACK_COMMAND;  //error
		m_o_bit_expected = false;
	}
	return CVLCPacket::CMD_ACK;
}

void CVlcConfigurationData::ResetContinuationStatus()
{
	m_expecting_continuation = false;
	m_o_bit_expected = false;
	m_total_cfg_data_bytes_expected = 0;
	m_cfg_data_bytes_received = 0;
	m_game_key_count = 0;
	m_bytes_sys_config = 0;
	if(m_cfg_pkt_last_write_req != NULL)
		delete m_cfg_pkt_last_write_req;
	m_cfg_pkt_last_write_req = NULL;
}

void CVlcConfigurationData::HandleWriteRequest()
{
	//Todo
	//This function is starting to look like a dogs breakfast

	CVlcData::HandleWriteRequest();

	CVLCPacket::EResponse response = m_last_received->GetResponse();
	if(response != CVLCPacket::RESP_ACK) //packet already NACKed, so don't need to check data or act on data
	{
		ResetContinuationStatus();
		return;	 //If response set in dlg was no response then do not check anything (same as nack)
	}

	ushort new_ack_code = CVLCPacket::CMD_ACK;
	CVLCPacket::EResponse new_response = CVLCPacket::RESP_ACK;

	//o bit in ack should echo o bit in write request received
	// unless 'mess with o bit' in cfg dialog is checked
	bool o_bit = (m_last_received->IsCommandFlagSet(CVLCPacket::CMD_FLG_O) );	
	m_ack_nack->SetCommandFlag(CVLCPacket::CMD_FLG_O, o_bit); 

	new_ack_code = ProcessReceivedPacket(); 
	if(new_ack_code != CVLCPacket::CMD_ACK )
	{
		ResetContinuationStatus();
		if(new_ack_code == CVLCPacket::CMD_NACK_COMMAND)
		{
			m_last_received->SetResponse(CVLCPacket::RESP_NACK_COMMAND);
			m_ack_nack->SetAckNackType(CVLCPacket::CMD_NACK_COMMAND);
		}
		if(new_ack_code == CVLCPacket::CMD_NACK_CRC)
		{
			m_last_received->SetResponse(CVLCPacket::RESP_NACK_CRC);
			m_ack_nack->SetAckNackType(CVLCPacket::CMD_NACK_CRC);
		}
		m_vlc_id->m_next_ack_nack = m_ack_nack;	
		return;
	}

	if(m_expecting_continuation)
		return;

	//All config data has been received and is contained in m_cfg_pkt_last_write_req

	const int game_key_count = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_OH_COUNT_GAME);
	const int num_bytes_sys_config = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_OH_BYTES_SYSTEM);
	const bool pkt_has_game_data = game_key_count>0;
	const CVlcControlData::EMode mode = ((CVlcControlData*)m_vlc_id->GetControlData())->GetMode();
	const bool fully_configured = m_vlc_id->IsFullyConfigured();
	const bool in_full_cfg_mode = (mode==CVlcControlData::MODE_FULL_CGF);

	CVlcMonitorData* monitor_cur_data = (CVlcMonitorData*)(m_vlc_id->GetMonitorCurData() );
	CVlcMonitorData* monitor_end_data = (CVlcMonitorData*)(m_vlc_id->GetMonitorEndData() );

	if(m_response_to_next_received == CVLCPacket::RESP_AUTO)	//if ACK was selected in dialog then assume data is correct - don't check
	{
	/*
		//currently no restriction on any of this - just accept anything
		If ( InFullConfigMode()  && FullyConfigured()   && PacketContainsGameData() ) 
		If ( !InFullConfigMode() && FullyConfigured()   && PacketContainsGameData() ) 
		If ( InFullConfigMode()  && !FullyConfigured()  && PacketContainsGameData() ) 
		If ( InFullConfigMode()  && FullyConfigured()   && !PacketContainsGameData() ) 	
	    if( !InFullConfigMode()  &&  !FullyConfigured() &&  PacketContainsGameData() ) 
		if( !InFullConfigMode()  &&  FullyConfigured()  &&  !PacketContainsGameData() ) 
		if( InFullConfigMode()   &&  !FullyConfigured() &&  !PacketContainsGameData() ) 
	    if( !InFullConfigMode()  &&  !FullyConfigured() &&  !PacketContainsGameData() ) 
	*/	
	}

	GameCfgData* cfg_data; 
	int* cfg_idx;
	if(game_key_count > 0)
	{
		cfg_data = new GameCfgData[game_key_count]; 
		cfg_idx = new int[game_key_count];
		memset(cfg_idx,0,sizeof(int)*game_key_count);
	}

	 //At the moment, only check that the amount of game data in the received packet = current amount for a configured game
	bool data_error = false; 
	if( (num_bytes_sys_config !=0 ) && (num_bytes_sys_config != CVLCPacket::LENGTH_CONFIGURATION_SYSTEM) )
		data_error = true;

	//calc offset to game key sections
	int offset = CVLCPacket::LENGTH_CONFIGURATION_OH + game_key_count*2;
	if(num_bytes_sys_config==CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
		offset += num_bytes_sys_config;
		
	if(m_cfg_pkt_last_write_req->GetDataLength() < offset) 
		data_error = true;

	//Save in game cfg data
	bool new_game_included = false;
	
	for(int key=0; key<game_key_count; key++)
	{
		if(data_error)
			break;

		int bytes_cur_game = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_OH_BYTES_GAME_KEY_X, key*2);
		if( (bytes_cur_game>0)&&(bytes_cur_game<CVLCPacket::LENGTH_CONFIGURATION_GAME) )
			data_error = true;
		if(m_cfg_pkt_last_write_req->GetDataLength() < (offset+bytes_cur_game)) 
			data_error = true;

		if(data_error)
			break;

		cfg_data[key].m_game_key_bytes = bytes_cur_game;
		if(bytes_cur_game >= CVLCPacket::LENGTH_CONFIGURATION_GAME)
		{
			int bytes_extra = bytes_cur_game - CVLCPacket::LENGTH_CONFIGURATION_GAME;
			cfg_data[key].m_bytes_extra = bytes_extra;
			cfg_data[key].m_game_id = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_GAME_ID,offset);
			cfg_data[key].m_game_type = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_GAME_TYPE,offset); 
			cfg_data[key].m_pkt_offset = offset;
			//m_last_received->GetData(cfg_data[key].m_parameters_extra, CVLCPacket::START_DATA + offset + CVLCPacket::LENGTH_CONFIGURATION_GAME, bytes_extra);
		}
		int idx=0;
		if( !IsConfigured( cfg_data[key],idx) && (bytes_cur_game != 0) )
			new_game_included = true;
		else if(bytes_cur_game > 0)
		{	//existing game - check that the size of game data match
			if(bytes_cur_game != m_game_cfg_data_list[idx].m_game_key_bytes)
				data_error = true;
		}
			
		cfg_idx[key] = idx;
		offset += bytes_cur_game;
	}

	//check that the packet is valid
	//currently all it checks is:
	//1. If new a game is included it must be in full config mode.
	//2. If an existing game is included the size of the extra parameters matches existing size
	if(m_response_to_next_received == CVLCPacket::RESP_AUTO)	//if ACK was selected in dialog then assume data is correct - don't check
	{
		if( (new_game_included && !in_full_cfg_mode) || data_error)
		{
			new_ack_code = CVLCPacket::CMD_NACK_CONFIGURATION;
			new_response = CVLCPacket::RESP_NACK_CONFIGURATION;
		}
	}
	m_last_received->SetResponse(new_response);
	//Update Ack Nack
	m_ack_nack->SetAckNackType(new_ack_code);
	m_vlc_id->m_next_ack_nack = m_ack_nack;

	if(new_response != CVLCPacket::RESP_ACK)
	{
		ResetContinuationStatus();
		return; //don't act on the packet received
	}


	//Remove any existing padding if present
	m_default->RemovePadZero();

	//update the game data in m_default and m_game_cfg_data_list
	for(key=0; key<game_key_count; key++)
	{
		if(cfg_data[key].m_game_key_bytes == 0)
			continue;

		//get the game data out of the received pkt
		CByteArray game_data;
		m_cfg_pkt_last_write_req->GetData(game_data, CVLCPacket::START_DATA + cfg_data[key].m_pkt_offset , cfg_data[key].m_game_key_bytes);
		
		if(cfg_idx[key] == -1) //new game - add to list
		{
			//update over-head data in m_default.
			int gk_count = m_default->GetDataFieldInt(CVLCPacket::CFG_OH_COUNT_GAME);
			int pos = CVLCPacket::START_DATA + CVLCPacket::LENGTH_CONFIGURATION_OH + gk_count*2; //position to insert the extra game key info in the OH section
			ASSERT(gk_count == m_count_of_games_configured);
			m_default->SetDataFieldInt(gk_count+1, CVLCPacket::CFG_OH_COUNT_GAME);
			CByteArray extra_game;
			extra_game.SetSize(2);
			extra_game[0] = cfg_data[key].m_game_key_bytes;
			extra_game[1] = 0;
			m_default->InsertData(extra_game,pos);

			//Append new game data
			pos = m_default->OffsetValidation(); //the new game data is going to start here since padding removed.
			m_default->InsertData(game_data, pos);
		
			//update list of configured games
			//Since a new game has been added, the offset for all existing games in m_default is incremented 
			//by 2 due to the extra 2 bytes in the overhead data...
			std::vector<GameCfgData>::iterator itr = m_game_cfg_data_list.begin();
			while(itr != m_game_cfg_data_list.end() )
			{
				(*itr).m_pkt_offset+=2;
				itr++;
			}

			//Add the new game to the list of configured games
			GameCfgData cfg = cfg_data[key]; 
			cfg.m_pkt_offset = CVLCPacket::LENGTH_CONFIGURATION_OH + (m_count_of_games_configured+1)*2 + CVLCPacket::LENGTH_CONFIGURATION_SYSTEM +  m_total_bytes_game_data; 
			ASSERT(cfg.m_pkt_offset = pos - CVLCPacket::START_DATA);
			m_game_cfg_data_list.push_back(cfg);
			
			//update counts / sizes
			m_count_of_games_configured++;
			m_total_bytes_game_data += cfg.m_game_key_bytes;

			//Add the new game to the monitor pkt
			CString game_name;
			m_cfg_pkt_last_write_req->GetDataFieldFormatted(game_name, CVLCPacket::CFG_GAME_NAME, cfg_data[key].m_pkt_offset);
			monitor_cur_data->AddGame(cfg_data[key].m_game_id,game_name);
			monitor_end_data->AddGame(cfg_data[key].m_game_id,game_name);
		}
		else //existing game - update
		{
			//Todo - should be getting it out of m_game_cfg_data_list here - not cfg_data
			int offset_cfg = m_game_cfg_data_list[key].m_pkt_offset;
			int offset_pkt = cfg_data[key].m_pkt_offset;
			//m_default->SetData(game_data, CVLCPacket::START_DATA + cfg_data[key].m_pkt_offset);
			int idx = cfg_idx[key];
			m_default->SetData(game_data, CVLCPacket::START_DATA + m_game_cfg_data_list[idx].m_pkt_offset);
		}
	}

	//Update the rest of the overhead data (Config version only)
	uint config_version = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_OH_VERS);
	m_default->SetDataFieldInt(config_version, CVLCPacket::CFG_OH_VERS);

	//copy in the system data if it is there
	if(num_bytes_sys_config > 0)
	{
		ASSERT(num_bytes_sys_config == CVLCPacket::LENGTH_CONFIGURATION_SYSTEM);
		if(num_bytes_sys_config != CVLCPacket::LENGTH_CONFIGURATION_SYSTEM)
			return;
		const int offset_sys_received_pkt = CVLCPacket::LENGTH_CONFIGURATION_OH + game_key_count*2;
		const int offset_sys_default_pkt  = CVLCPacket::LENGTH_CONFIGURATION_OH + m_count_of_games_configured*2;
		CByteArray system_data;
		m_cfg_pkt_last_write_req->GetData(system_data, CVLCPacket::START_DATA + offset_sys_received_pkt, num_bytes_sys_config);
		m_default->SetData(system_data, CVLCPacket::START_DATA + offset_sys_default_pkt);

		if(in_full_cfg_mode)
		{
			uint base_record = m_cfg_pkt_last_write_req->GetDataFieldInt(CVLCPacket::CFG_SYS_BASE,offset_sys_received_pkt);
			monitor_cur_data->SetBaseRecord(base_record);
		}

		m_system_data_configured = true;
	}

	if(in_full_cfg_mode)
	{
		monitor_cur_data->ResetAllGames();	
		monitor_end_data->DoSnapshot();
	}

	m_default->PadZero();
	m_default->SetNBits();
	int len3 = m_default->GetDataLength();

	//update next to send 
	m_next_to_send->CopyPacketData(*m_default);
	m_next_to_send->SetNBits();

	//update config version in monitor packet
	m_vlc_id->m_monitor_cur_data->SetConfigVersion(config_version);

	m_fully_configured = (m_count_of_games_configured>0) && m_system_data_configured;

	//check some stuff
	int len = m_cfg_pkt_last_write_req->GetDataLength();
	int len2 = m_next_to_send->GetDataLength();
	if(m_last_received != NULL)
		delete m_last_received;
	m_last_received = new CVLCPacket(*m_cfg_pkt_last_write_req);
	m_last_received->SetResponse(new_response);

	//Display the packet received
	g_dlg_configuration->DisplayPacket(*m_last_received);
	ResetContinuationStatus();
	BuildSendList();
}
//Check the list of currently configured games 
//Return true if the supplied game is present 
//Also output the index of the game data.  -1 if not configured
//Check both game type and game ID
bool CVlcConfigurationData::IsConfigured(GameCfgData& p_game_cfg_data, int& p_idx_out)
{
	bool result =false;
	p_idx_out = 0;
	std::vector<GameCfgData>::iterator itr = m_game_cfg_data_list.begin();
	while(itr != m_game_cfg_data_list.end() )
	{
		if( ( (*itr).m_game_type == p_game_cfg_data.m_game_type) &&
			 ( (*itr).m_game_id == p_game_cfg_data.m_game_id) )
		{
			result = true;
			break;
		}
		itr++;
		p_idx_out++;
	}
	if(!result)
		p_idx_out = -1;
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcMonitorData::CVlcMonitorData(CVlcId* p_vlc_vlc_id, EMonitorType p_monitor_type):
	CVlcData(p_vlc_vlc_id),
	m_monitor_type(p_monitor_type),
	m_credits(0),
	m_game_count(0),
	m_drop_box_amount(0),
	m_active_game_id(0),
	m_memsig_timer(0),
	m_memsig_calc_previously_done(false)
{
	Initialise();
	m_game_info_list.clear();
	m_game_info_list.reserve(3);
}	


void CVlcMonitorData::Initialise()
{	
	if(m_vlc_id==NULL)
		return;

	m_default = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_MONITOR);
	if(m_monitor_type==MONITOR_CURRENT)
		m_default->SetDataReadType(CVLCPacket::CMD_READ_MONITER_CUR);
	else
		m_default->SetDataReadType(CVLCPacket::CMD_READ_MONITER_END);
	//Initialise data fields 
	m_default->SetDataFieldFormatted(CString("05"),CVLCPacket::CRTL_FORMAT_ID);
	m_default->SetDataFieldFormatted(CString("0200"),CVLCPacket::MON_SYS_MODE);
	m_default->PadZero();

	//Set Next to send (initialised to be the same as the default
	m_next_to_send = new CVLCPacket(*m_default);

	CVlcData::InitialiseAckNack();

	if(m_monitor_type==MONITOR_CURRENT)
		g_dlg_monitor->DisplayPacket(*m_default);
}

void CVlcMonitorData::UpdateDefault()
{
	ushort sys_fw_vers = m_next_to_send->GetDataFieldInt(CVLCPacket::MON_SYSTEM_FIRMWARE_VERS);
	m_default->SetDataFieldInt(sys_fw_vers,CVLCPacket::MON_SYSTEM_FIRMWARE_VERS);
	
	ushort game_fw_vers = m_next_to_send->GetDataFieldInt(CVLCPacket::MON_GAME_FIRMWARE_VERS);
	m_default->SetDataFieldInt(game_fw_vers,CVLCPacket::MON_GAME_FIRMWARE_VERS);

	ushort sig_val = m_next_to_send->GetDataFieldInt(CVLCPacket::MON_MEM_SIG_VALUE);
	m_default->SetDataFieldInt(sig_val, CVLCPacket::MON_MEM_SIG_VALUE);
}

void CVlcMonitorData::HandleReadRequest()
{
	CVlcData::HandleReadRequest();

	//Don't set the sig result in the packet if no request to calculate.
	if(!m_memsig_calc_previously_done)
		m_next_to_send->SetDataFieldInt(0,CVLCPacket::MON_MEM_SIG_VALUE);
}

void CVlcMonitorData::HandleWriteRequest()
{
	//Monitor packet is read only - do nothing
}

//Return true if the game has been added to the m_default pkt
//If true sets p_idx to index of game (-1 if doesn't exits)
bool CVlcMonitorData::GameExists(uint p_id, int& p_idx)
{
	p_idx = 0;
	std::vector<GameInfo*>::iterator itr = m_game_info_list.begin();
	while(itr != m_game_info_list.end())
	{
		if( (*itr)->m_id == p_id )
			return true;	//already exists
		itr++;
		p_idx++;
	}
	p_idx = -1;
	return false;
}


void CVlcMonitorData::AddGame(uint p_game_id, CString& p_game_name)
{
	uint data_length = m_default->GetDataLength();
	ASSERT(data_length == CVLCPacket::LENGTH_MONITOR_SYSTEM + m_game_count*CVLCPacket::LENGTH_MONITOR_GAME);
	
	//check that this id hasn't already been added
	int idx=0;
	if(GameExists(p_game_id,idx))
		return;

	//add a new game
	GameInfo* game_info = new GameInfo();
	game_info->m_game_name = p_game_name;
	game_info->m_id = p_game_id;
	game_info->m_offset_pkt = data_length;
	m_game_info_list.push_back(game_info);

	CByteArray game_data;
	game_data.SetSize(CVLCPacket::LENGTH_MONITOR_GAME);
	for(int i=0; i<game_data.GetSize(); i++)
		game_data[i]=0;
	int pos = m_default->OffsetValidation(); //insertion pos for new game
	m_default->InsertData(game_data,pos);
	m_default->SetDataFieldInt(p_game_id,CVLCPacket::MON_GAME_ID,game_info->m_offset_pkt);  
	m_default->PadZero();
	m_default->SetNBits();
	m_game_count++;
	ASSERT(m_default->GetDataLength() == CVLCPacket::LENGTH_MONITOR_SYSTEM + m_game_count*CVLCPacket::LENGTH_MONITOR_GAME);
	//Update m_next_to_send too
	m_next_to_send->CopyPacketData(*m_default);
	//set active id if in normal m
	SetAciveGameId(p_game_id);
	g_dlg_monitor->AddGameId(p_game_id);
}

void CVlcMonitorData::ResetGame(uint p_game_id)
{
	if(m_monitor_type != MONITOR_CURRENT) return;
	int idx=0;
	if(!GameExists(p_game_id,idx))
		return;
	uint offset = (m_game_info_list[idx])->m_offset_pkt;
	m_default->SetDataFieldInt(0, CVLCPacket::MON_GAME_GAMES_PLAYED,offset);
	m_default->SetDataFieldInt(0, CVLCPacket::MON_GAME_GAMES_WON,offset);
	m_default->SetDataFieldInt(0, CVLCPacket::MON_GAME_CENTS_PLAYED,offset);
	m_default->SetDataFieldInt(0, CVLCPacket::MON_GAME_CENTS_WON,offset);
}

void CVlcMonitorData::UpdateCashoutAmount(uint m_amount)
{
	CVlcTransactionData* trans_data = (CVlcTransactionData*)(m_vlc_id->GetTransactionData());
	trans_data->SetCashoutAmount(m_credits);
}

void CVlcMonitorData::ResetAllGames()
{
	for(int i=0; i<m_game_count; i++)
		ResetGame((m_game_info_list[i])->m_id);
}

void CVlcMonitorData::InCoinHopper(uint p_val)
{
	if(m_monitor_type != MONITOR_CURRENT) return;
	uint cur_val = m_default->GetDataFieldInt(CVLCPacket::MON_COIN_IN_HOPPER);
	m_default->SetDataFieldInt(cur_val+p_val, CVLCPacket::MON_COIN_IN_HOPPER);
	m_next_to_send->CopyPacketData(*m_default);
	m_credits += p_val;
	UpdateCashoutAmount(m_credits);
}
void CVlcMonitorData::InCoinDropBox(uint p_val)
{
	if(m_monitor_type != MONITOR_CURRENT) return;
	uint cur_val = m_default->GetDataFieldInt(CVLCPacket::MON_COIN_IN_DROP_BOX);
	m_default->SetDataFieldInt(cur_val+p_val, CVLCPacket::MON_COIN_IN_DROP_BOX);
	m_next_to_send->CopyPacketData(*m_default);
	m_credits += p_val;
	m_drop_box_amount += p_val;
	UpdateCashoutAmount(m_credits);
}
void CVlcMonitorData::OutCoinHopper(uint p_val)
{
	if(m_monitor_type != MONITOR_CURRENT) return;
	int amount = p_val;
	if(p_val > m_credits)
		amount = m_credits;
	uint cur_val = m_default->GetDataFieldInt(CVLCPacket::MON_COIN_OUT_HOPPER);
	m_default->SetDataFieldInt(cur_val+amount, CVLCPacket::MON_COIN_OUT_HOPPER);
	m_next_to_send->CopyPacketData(*m_default);
	m_credits -= amount;
	UpdateCashoutAmount(m_credits);
}
void CVlcMonitorData::OutManualPay(uint p_val)
{
	if(m_monitor_type != MONITOR_CURRENT) return;
	int amount = p_val;
	if(p_val > m_credits)
	amount = m_credits;	
	uint cur_val = m_default->GetDataFieldInt(CVLCPacket::MON_CASH_TICKETS_MANUAL_PAYS);
	m_default->SetDataFieldInt(cur_val+amount, CVLCPacket::MON_CASH_TICKETS_MANUAL_PAYS);
	m_next_to_send->CopyPacketData(*m_default);
	m_credits -= amount;
	UpdateCashoutAmount(m_credits);
}
void CVlcMonitorData::CollectDropBox(uint p_val)
{
	if(m_monitor_type != MONITOR_CURRENT) return;
	int amount = p_val;
	if(p_val > m_drop_box_amount)
		amount = m_drop_box_amount;
	uint cur_val = m_default->GetDataFieldInt(CVLCPacket::MON_DROP_BOX_COLLECTED);
	m_default->SetDataFieldInt(cur_val+amount, CVLCPacket::MON_DROP_BOX_COLLECTED);
	m_next_to_send->CopyPacketData(*m_default);
	m_drop_box_amount -= amount;
}

void CVlcMonitorData::DoSnapshot()
{
	if(m_monitor_type != MONITOR_END) return;
	CVlcMonitorData* cur_monitor_data = (CVlcMonitorData*)(m_vlc_id->GetMonitorCurData());
	*m_default = *(cur_monitor_data->GetDefault());
	m_default->SetDataReadType(CVLCPacket::CMD_READ_MONITER_END);
	m_next_to_send->CopyPacketData(*m_default);
	m_credits = cur_monitor_data->GetCurrentCredits();
	m_drop_box_amount = cur_monitor_data->GetDropBox();
}

void CVlcMonitorData::PlayGame(uint p_game_id, uint p_bet, uint p_win)
{
	GameInfo* game_info = NULL;
	std::vector<GameInfo*>::iterator itr = m_game_info_list.begin();
	while(itr != m_game_info_list.end() )
	{
		if( (*itr)->m_id == p_game_id)
		{
			game_info = (*itr);
			break;
		}
		itr++;
	}
	if(game_info==NULL)
		return;

	int bet = p_bet;
	if(p_bet > m_credits)
		bet = m_credits;
	
	uint cur_master_turnover = m_default->GetDataFieldInt(CVLCPacket::MON_TOTAL_PLAYED);
	uint cur_master_win = m_default->GetDataFieldInt(CVLCPacket::MON_TOTAL_WON);

	uint cur_game_turnover = m_default->GetDataFieldInt(CVLCPacket::MON_GAME_CENTS_PLAYED, game_info->m_offset_pkt);
	uint cur_game_win = m_default->GetDataFieldInt(CVLCPacket::MON_GAME_CENTS_WON, game_info->m_offset_pkt);
	uint cur_game_games_played = m_default->GetDataFieldInt(CVLCPacket::MON_GAME_GAMES_PLAYED, game_info->m_offset_pkt);
	uint cur_game_games_won = m_default->GetDataFieldInt(CVLCPacket::MON_GAME_GAMES_WON, game_info->m_offset_pkt);

	cur_master_turnover += bet;
	cur_master_win += p_win;

	cur_game_turnover += bet;
	cur_game_win += p_win;
	
	cur_game_games_played++;
	if(p_win>0)
		cur_game_games_won++;

	m_default->SetDataFieldInt(cur_master_turnover,CVLCPacket::MON_TOTAL_PLAYED);
	m_default->SetDataFieldInt(cur_master_win, CVLCPacket::MON_TOTAL_WON);
	m_default->SetDataFieldInt(cur_game_turnover, CVLCPacket::MON_GAME_CENTS_PLAYED, game_info->m_offset_pkt);
	m_default->SetDataFieldInt(cur_game_win, CVLCPacket::MON_GAME_CENTS_WON, game_info->m_offset_pkt);
	m_default->SetDataFieldInt(cur_game_games_played, CVLCPacket::MON_GAME_GAMES_PLAYED, game_info->m_offset_pkt);
	m_default->SetDataFieldInt(cur_game_games_won, CVLCPacket::MON_GAME_GAMES_WON, game_info->m_offset_pkt);

	m_next_to_send->CopyPacketData(*m_default);

	m_credits -= bet;
	m_credits += p_win;
	UpdateCashoutAmount(m_credits);
}


void CVlcMonitorData::GetGameIdList(std::vector<uint> &p_id_list)
{
	p_id_list.clear();
	std::vector<GameInfo*>::iterator itr = m_game_info_list.begin();
	while(itr != m_game_info_list.end() )
	{
		p_id_list.push_back( (*itr)->m_id );
		itr++;
	}
}

void CVlcMonitorData::UpdatePacketOnSend()
{
	if(m_monitor_type == MONITOR_CURRENT)
	{
		CVLCDateTime date_time; //this is the universal date time
		uint date = (uint)date_time.GetJulianDate();
		uint time = (uint)date_time.GetTimeInSeconds();
		m_next_to_send->SetDataFieldInt(date,CVLCPacket::MON_DATE_FOR_ACCOUNTING_DATE);
		m_next_to_send->SetDataFieldInt(time,CVLCPacket::MON_TIME_FOR_ACCOUNTING_DATA);
	}
	CVlcData::UpdatePacketOnSend();
}

void CVlcMonitorData::SetMasterResetDateTime()
{
	CVLCDateTime date_time; //the universal date time.
	m_default->SetDataFieldInt(date_time.GetJulianDate(), CVLCPacket::MON_MASTER_RESET_DATE);
	m_default->SetDataFieldInt(date_time.GetTimeInSeconds(), CVLCPacket::MON_MASTER_RESET_TIME);
	m_next_to_send->SetDataFieldInt(date_time.GetJulianDate(), CVLCPacket::MON_MASTER_RESET_DATE);
	m_next_to_send->SetDataFieldInt(date_time.GetTimeInSeconds(), CVLCPacket::MON_MASTER_RESET_TIME);
}

void CVlcMonitorData::SetControlVersion(uint p_val)
{
	m_default->SetDataFieldInt(p_val, CVLCPacket::MON_CONTROL_VERS);
	m_next_to_send->SetDataFieldInt(p_val, CVLCPacket::MON_CONTROL_VERS);
}
void CVlcMonitorData::SetConfigVersion(uint p_val)
{
	m_default->SetDataFieldInt(p_val, CVLCPacket::MON_CONFIG_VERS);
	m_next_to_send->SetDataFieldInt(p_val, CVLCPacket::MON_CONFIG_VERS);
}

void CVlcMonitorData::SetSystemMode(CByteArray& p_val)
{
	m_default->SetDataFieldHex(p_val, CVLCPacket::MON_SYS_MODE);
	m_next_to_send->SetDataFieldHex(p_val, CVLCPacket::MON_SYS_MODE);
	SetAciveGameId(m_active_game_id);
}

void CVlcMonitorData::SetAciveGameId(uint p_active_game_id)
{
	m_active_game_id = p_active_game_id;
	uint id = 0;
	CVlcControlData* control_data = (CVlcControlData*)m_vlc_id->GetControlData();	
	if(control_data->GetMode() == CVlcControlData::MODE_NORMAL )
		id = p_active_game_id;
	m_default->SetDataFieldInt(id,CVLCPacket::MON_ACTIVE_GAME_ID);
	m_next_to_send->SetDataFieldInt(id,CVLCPacket::MON_ACTIVE_GAME_ID);
}

void CVlcMonitorData::SetBaseRecord(uint p_record)
{
	m_default->SetDataFieldInt(p_record,CVLCPacket::MON_BASE);
	m_next_to_send->SetDataFieldInt(p_record,CVLCPacket::MON_BASE);
}

void CVlcMonitorData::CalcMemSig(uint p_divisor)
{
	uint sig_val = 0xFFFF;
	m_next_to_send->SetDataFieldInt(sig_val,CVLCPacket::MON_MEM_SIG_VALUE);
	if(m_memsig_timer!=0)
		g_timers->RemoveTimer(m_memsig_timer); //kill that timer and restart
	m_memsig_timer = g_timers->AddTimer( 10000, this ); //wait 10 seconds
	m_memsig_calc_previously_done=true;
}

void CVlcMonitorData::ProcessTimeout( int p_id )
{
	if(p_id == m_memsig_timer)
	{
		uint sig_val = m_default->GetDataFieldInt(CVLCPacket::MON_MEM_SIG_VALUE);
		m_next_to_send->SetDataFieldInt(sig_val,CVLCPacket::MON_MEM_SIG_VALUE);
		g_timers->RemoveTimer(p_id);
		m_memsig_timer=0;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcEventLogData::CVlcEventLogData(CVlcId* p_vlc_vlc_id):
	CVlcData(p_vlc_vlc_id),
	m_next_record_number(0),
	m_start(NULL),
	m_base(NULL),
	m_end_next(NULL),
	m_read_start(NULL),
	m_read_end(NULL),
	m_apply_errors_to(0)
{
	Initialise();
}


void CVlcEventLogData::Initialise()
{	
	if(m_vlc_id==NULL)
		return;

	m_default = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_EVENT);	//Initially it contains only the 8 byte header (byte 1 is 05, 7 bytes reserved) 
	m_default->SetDataReadType(CVLCPacket::CMD_READ_EVENT_LOG);
	m_default->PadZero();

	//Initialise data fields - everything 0 except format id
	m_default->SetDataFieldFormatted(CString("05"),CVLCPacket::EVENT_FORMAT_ID);

	//Set Next to send (initialised to be the same as the default
	m_next_to_send = new CVLCPacket(*m_default);

	CVlcData::InitialiseAckNack(); //wont be used for event log

	m_event_log.clear();
	m_next_to_send_list.clear();
	BuildList(m_next_to_send_list,NULL,NULL);
}

void CVlcEventLogData::HandleReadRequest()
{
	const CVLCPacket *read_request = g_receive_manager->GetLastPacket();
	if(read_request==NULL)
		return;

	//check if any errors in read request
	byte errors = read_request->GetErrorStatus();
	if( errors != CVLCPacket::ERR_NONE)
		return;

	if(m_next_to_send_list.size()==0)
		return;

	CVLCPacket* pkt = m_next_to_send_list.front();
	
	ushort command_type = read_request->GetCommandType();
	if(command_type == CVLCPacket::CMD_READ_REQ)
	{
		pkt->SetCommandType(CVLCPacket::CMD_READ_REPLY);
		m_vlc_id->SetNextReadReply(pkt,this);
		m_current_pkt_read_req_itr = m_next_to_send_list.begin();
		m_expected_o_bit_read_reply=false;
	}
	if(command_type == CVLCPacket::CMD_SITE_INFO_REQ)
	{	
		pkt->SetCommandType(CVLCPacket::CMD_SITE_INFO_REPLY);
		m_vlc_id->SetNextSiteInfoReply(pkt, this);
		m_current_pkt_site_info_req_itr = m_next_to_send_list.begin();
		m_expected_o_bit_site_info_reply=false;
	}
}

/*
void CVlcEventLogData::UpdatePacketOnSend()
{
	//toto - finish this
	//m_next_to_send->SetValidationDateTime();
	//m_next_to_send->SetValidationCode();
	//m_next_to_send->SetCrc();
}
*/

void CVlcEventLogData::HandleWriteRequest()
{
	//event log is read only - do nothing
}

void CVlcEventLogData::HandleAckNack()
{
	const CVLCPacket *ack = g_receive_manager->GetLastPacket();
	if(ack==NULL)
		return;

	ushort ack_type = ack->GetAckNackCommandType();	//read reply of site info reply.
	if( (ack_type!=CVLCPacket::CMD_ACK_READ_REPLY)&&(ack_type!=CVLCPacket::CMD_ACK_SITE_INFO_REPLY))
		return;
	
	ushort ack_code = ack->GetAckNackCode();

	//todo - set the ack code for the last packet sent - save last packet sent in history log etc

	bool o_bit = ack->IsCommandFlagSet(CVLCPacket::CMD_FLG_O);

	if(ack_type==CVLCPacket::CMD_ACK_READ_REPLY)
	{
		if(ack_code!=CVLCPacket::CMD_ACK)
		{
			m_expected_o_bit_read_reply=false;
			return;
		}
		if(o_bit != m_expected_o_bit_read_reply)
		{
			//resend current packet.
			CVLCPacket* pkt = *m_current_pkt_read_req_itr;
			m_vlc_id->SetNextReadReply(pkt,this);
		}
		else
		{
			m_current_pkt_read_req_itr++;
			if(m_current_pkt_read_req_itr==m_next_to_send_list.end())
			{
				m_expected_o_bit_read_reply=false;
				return;
			}
			else
			{
				CVLCPacket* pkt = *m_current_pkt_read_req_itr;
				pkt->SetCommandType(CVLCPacket::CMD_READ_REPLY);
				m_vlc_id->SetNextReadReply(pkt,this);
				m_expected_o_bit_read_reply = !m_expected_o_bit_read_reply;
			}
		}
	}
	if(ack_type==CVLCPacket::CMD_ACK_SITE_INFO_REPLY)
	{
		if(ack_code!=CVLCPacket::CMD_ACK)
		{
			m_expected_o_bit_site_info_reply=false;
			return;
		}
		if(o_bit != m_expected_o_bit_site_info_reply)
		{
			//resend current packet.
			CVLCPacket* pkt = *m_current_pkt_site_info_req_itr;
			m_vlc_id->SetNextSiteInfoReply(pkt,this);
		}
		else
		{
			m_current_pkt_site_info_req_itr++;
			if(m_current_pkt_site_info_req_itr==m_next_to_send_list.end())
			{
				m_expected_o_bit_site_info_reply=false;
				return;
			}
			else
			{
				CVLCPacket* pkt = *m_current_pkt_site_info_req_itr;
				pkt->SetCommandType(CVLCPacket::CMD_SITE_INFO_REPLY);
				m_vlc_id->SetNextSiteInfoReply(pkt,this);
				m_expected_o_bit_site_info_reply = !m_expected_o_bit_site_info_reply;
			}
		}	
	}	
}

void CVlcEventLogData::HandleAckNack(bool& p_expected_o_bit, PacketListItr& p_list_itr, CVLCPacket* p_ack)
{
}


CVlcEventLogData::EventRecord* CVlcEventLogData::GetEventRecord(CVLCPacket& p_packet,EEventType p_type )
{
	//gets the event data out of a CVLCPacket
	//Assumes that p_packet contains 1 event record. 
	if(p_packet.GetDataLength() != CVLCPacket::LENGTH_EVENT_RECORD)
		return NULL;

	CVlcEventLogData::EventRecord* event_record = new(CVlcEventLogData::EventRecord);

	bool success = p_packet.GetData(event_record->m_data, CVLCPacket::START_DATA, CVLCPacket::LENGTH_EVENT_RECORD);	

	if(!success)
	{
		delete event_record;
		return NULL;
	}
	event_record->m_record_number = m_next_record_number;
	event_record->m_type = p_type;
	return event_record;
}

CVLCPacket* CVlcEventLogData::GetEventPacket(CVlcEventLogData::EventRecord& p_record)
{
	CVLCPacket* packet = new CVLCPacket(CVLCPacket::LENGTH_EVENT);

	bool success = packet->SetData(p_record.m_data, CVLCPacket::START_DATA);
	if(!success)
	{
		delete packet;
		return NULL;
	}
	return packet;
}

//todo - is this used?
void CVlcEventLogData::AddEvent(CVLCPacket& p_packet, EEventType p_type)
{
	EventRecord* record = GetEventRecord(p_packet);
	if(record==NULL)
		return;
	record->m_type = p_type;
	AddEvent( GetEventRecord(p_packet) );
}

void CVlcEventLogData::AddEvent(CVlcEventLogData::EventRecord* p_event_record)
{
	if(p_event_record==NULL)
		return;
	m_event_log.push_back(p_event_record);
	m_next_record_number++;
	m_end_next = m_event_log.back();
	if(m_start==NULL)
		m_start = m_event_log.front();
	//Add the event to the end default packet
	//todo - not sure that m_default is used for anything here!
	int pos = m_default->OffsetValidation();
	m_default->InsertData(p_event_record->m_data, pos);
}

void CVlcEventLogData::SetReadRecordStart(EventRecord* p_start)
{
	if(m_read_end==NULL)
		return;
	if(p_start==NULL)
		return;
	if(p_start->m_record_number <= m_read_end->m_record_number)
		m_read_start = p_start;

	BuildList(m_next_to_send_list,m_read_start,m_read_end);
}

void CVlcEventLogData::SetReadRecordEnd(EventRecord* p_end)
{
	if(m_read_start==NULL)
		return;
	if(p_end==NULL)
		return;
	if(p_end->m_record_number >= m_read_start->m_record_number)
		m_read_end = p_end;

	BuildList(m_next_to_send_list,m_read_start,m_read_end);
}

void CVlcEventLogData::SetDefaultRead()
{
	if( (m_read_start == m_start) && (m_read_end == m_end_next) )
		return; //already set to default

	m_read_start = m_start;	
	m_read_end	 = m_end_next;
	
	BuildList(m_next_to_send_list,m_read_start,m_read_end);
}

int CVlcEventLogData::NumRecordsNextRead()
{
	if(m_read_start==NULL)
		return 0;
	if(m_read_end==NULL)
		return 0;

	int count = m_read_end->m_record_number - m_read_start->m_record_number;
	if(count < 0)
		return 0;
	return count+1;
}

void CVlcEventLogData::SetErrors()
{
	uint pkt_num_sel = g_dlg_events->GetCurSelErrorPacket(); //0 means apply to all
	ASSERT(pkt_num_sel <= NumPacketsNextRead() );
	m_apply_errors_to = pkt_num_sel;

	CommsErrorsTiming* timing_errors  = NULL;
	CommsErrorsCorruptData* data_errors = NULL;
	CommsErrorsDataSize* size_errors = NULL;

	//the error settings are stored in m_next_to_send
	//(m_next_to_send isn't used for anything else other than this for CVlcEventLogData)
	uint pkt_num_cur=1;
	PacketListItr itr = m_next_to_send_list.begin();
	while(itr != m_next_to_send_list.end() )
	{
		CVLCPacket* cur_pkt = (*itr);
		cur_pkt->ClearAllErrors();

		if(m_next_to_send->GetTimingErrors() != NULL)
		{
			timing_errors = new(CommsErrorsTiming);
			*timing_errors = *(m_next_to_send->GetTimingErrors());
		}
		if(m_next_to_send->GetDataErrors() != NULL)
		{
			data_errors = new(CommsErrorsCorruptData);
			*data_errors = *(m_next_to_send->GetDataErrors());
		}
		if(m_next_to_send->GetSizeErrors() != NULL)
		{
			size_errors = new(CommsErrorsDataSize);
			*size_errors = *(m_next_to_send->GetSizeErrors());	
		}
		if((pkt_num_sel==pkt_num_cur)||(pkt_num_sel==0))
		{
			cur_pkt->SetTimingErrors(timing_errors);
			cur_pkt->SetDataErrors(data_errors);
			cur_pkt->SetSizeErrors(size_errors);
		}
		
		itr++;
		pkt_num_cur++;
	}	
}

void CVlcEventLogData::ViewErrors()
{
	ASSERT(m_apply_errors_to <= NumPacketsNextRead());
	g_dlg_events->SetCurSelErrorPacket(m_apply_errors_to);
}

void CVlcEventLogData::SetErrorsReadReply(CommsErrorsTiming* p_timing_errors, CommsErrorsCorruptData* p_data_errors, CommsErrorsDataSize* p_size_errors)
{
	CVlcData::SetErrorsReadReply(p_timing_errors, p_data_errors, p_size_errors);	//save the error settings in m_next_to_send
	SetErrors();
}


//Todo - don't think this is used
CVLCPacket* CVlcEventLogData::BuildPacket(CVlcEventLogData::EventRecord* p_start, CVlcEventLogData::EventRecord* p_end  )
{
//return a pointer to the constructed packet

	if(p_start==NULL)
		return NULL;
	if(p_end==NULL)
		return NULL;
	
	EventLogItr itr = m_event_log.begin();
	while( (*itr) != p_start)  
	{
		itr++;
		if(itr == m_event_log.end())
			return NULL;
	}

	//itr now points to m_read_start
	CVLCPacket* packet = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_EVENT);	//Initially it contains only the 8 byte header (byte 1 is 05, 7 bytes reserved) 
	packet->SetDataReadType(CVLCPacket::CMD_READ_EVENT_LOG);
	packet->SetDataFieldFormatted(CString("05"),CVLCPacket::EVENT_FORMAT_ID);
	
	while( itr != m_event_log.end() )
	{
		//Append the record
		int pos = packet->OffsetValidation();
		packet->InsertData( (*itr)->m_data, pos);
		if( (*itr) == p_end )
			break;
		itr++;
	}
	return packet;
}


void CVlcEventLogData::BuildList(PacketList& p_list, CVlcEventLogData::EventRecord* p_start, CVlcEventLogData::EventRecord* p_end)
{
	p_list.clear(); //should also call destructor for each packet in the list  
	CVLCPacket* packet = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_EVENT);	//initially theq bit is set	
	packet->SetDataReadType(CVLCPacket::CMD_READ_EVENT_LOG);
	packet->SetTransmitDirection(CVLCPacket::SEND);
	packet->SetDataFieldFormatted(CString("05"),CVLCPacket::EVENT_FORMAT_ID);
	p_list.push_back(packet);

	if(p_start==NULL)
		return;
	if(p_end==NULL)
		return;
	
	EventLogItr itr = m_event_log.begin();
	while( (*itr) != p_start)  
	{
		itr++;
		if(itr == m_event_log.end())
			return;
	}
	
	//No need to worry about padding because allways will be an even multiple of 8
	while( itr != m_event_log.end() )
	{
		if(packet->GetDataLength() == CVLCPacket::LENGTH_DATA_MAX)
		{
			//clear q bit to indicate more packets to follow
			packet->SetCommandFlag(CVLCPacket::CMD_FLG_Q,false);

			//start a new continuation packet - add to list.
			packet = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_OVERHEAD+CVLCPacket::LENGTH_VALIDATION); //continuation packet initially has no data in it	
			packet->SetDataReadType(CVLCPacket::CMD_READ_CONTINUATION);
			packet->SetTransmitDirection(CVLCPacket::SEND);
			p_list.push_back(packet);
		}

		int pos = packet->OffsetValidation();
		packet->InsertData( (*itr)->m_data, pos);

		if( (*itr) == p_end )
			break;
		itr++;
	}
}


void CVlcEventLogData::AppendToList(PacketList& p_list, EventRecord& p_record)
{
	if(p_list.empty()) //should not be empty ever
	{
		CVLCPacket* packet = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_EVENT);	
		packet->SetDataReadType(CVLCPacket::CMD_READ_EVENT_LOG);
		packet->SetDataFieldFormatted(CString("05"),CVLCPacket::EVENT_FORMAT_ID);
		p_list.push_back(packet);		
	}
	
	CVLCPacket* packet = p_list.back();

	if(packet->GetDataLength() == CVLCPacket::LENGTH_DATA_MAX)
	{
		//clear q bit to indicate more packets to follow
		packet->SetCommandFlag(CVLCPacket::CMD_FLG_Q,false);
		
		//start a new continuation packet - add to list.
		packet = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_OVERHEAD+CVLCPacket::LENGTH_VALIDATION); //continuation packet initially has no data in it	
		packet->SetDataReadType(CVLCPacket::CMD_READ_CONTINUATION);
		p_list.push_back(packet);
	}
	
	int pos = packet->OffsetValidation();
	packet->InsertData(p_record.m_data, pos);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcTransactionData::CVlcTransactionData(CVlcId* p_vlc_vlc_id):
	CVlcData(p_vlc_vlc_id)
{
	Initialise();
}

void CVlcTransactionData::Initialise()
{	
	if(m_vlc_id==NULL)
		return;

	m_default = CVlcData::InitialiseReadReply(CVLCPacket::LENGTH_TRANS_REQUEST);
	m_default->SetCommandType(CVLCPacket::CMD_TRANSACTION_REQ);
	m_default->SetTransactionType(CVLCPacket::CMD_TRANS_CASHOUT);
	
	//Initialise data fields - everything 0 except format id
	m_default->SetDataFieldFormatted(CString("05"),CVLCPacket::CRTL_FORMAT_ID);

	//Set Next to send (initialised to be the same as the default
	m_next_to_send = new CVLCPacket(*m_default);

	CVlcData::InitialiseAckNack();
	m_ack_nack->SetAckNackCommandType(CVLCPacket::CMD_ACK_TRANSACTION_REPLY); //defaults to write request so update

	g_dlg_transactions->DisplayPacket(*m_default);
}

void CVlcTransactionData::HandleReadRequest()
{ //Do nothing
}

void CVlcTransactionData::HandleWriteRequest()
{ //Do Nothing
}

void CVlcTransactionData::HandleAckNack() //received an Ack nack
{
	const CVLCPacket *ack_nack = g_receive_manager->GetLastPacket();
	if(ack_nack==NULL)
		return;
	if(ack_nack->GetCommandType() != CVLCPacket::CMD_ACK_NACK)
		return;
	ushort ack_type = ack_nack->GetAckNackCommandType();
	ushort ack_code = ack_nack->GetAckNackCode();
	CVLCPacket::EResponse response = CVLCPacket::GetResponseFromAckCode(ack_code);
	
	if(ack_type==CVLCPacket::CMD_ACK_TRANSACTION_REQ)
	{
		if(m_last_sent_read_reply != NULL)
			m_last_sent_read_reply->SetResponse(response); 
	}
}

void CVlcTransactionData::SendTransactionRequest()
{
	g_dlg_transactions->SetPacket(*m_next_to_send);
	m_vlc_id->SetNextTransactionRequest(m_next_to_send,this);
	g_notify_mgr->Register(CNotify::SENT_VLC_PACKET,this);
}

void CVlcTransactionData::ProcessNotify( int p_id ) //a packet has finished being sent
{
	if(p_id==CNotify::SENT_VLC_PACKET)
	{
		g_notify_mgr->DeRegister(p_id, this);	
		CVLCPacket* last_sent = g_vlc_transmit_manager->GetLastPacket();
		if(last_sent==NULL)
			return;
		ushort command_type = last_sent->GetCommandType();
		if(last_sent->GetDataType() == m_next_to_send->GetDataType()) //finished sending a transaction request
		{	
			m_next_to_send->ClearAllErrors(); //Clear the error settings in Next to send (the transaction request)
			
			if(m_last_sent_read_reply != NULL)
				delete m_last_sent_read_reply;
			m_last_sent_read_reply = new CVLCPacket(*last_sent);
			m_last_sent_read_reply->SetResponse(CVLCPacket::RESP_NONE);	
		}
		if(command_type == CVLCPacket::CMD_ACK_NACK) //finished sending an ACK NACK
			m_ack_nack->ClearAllErrors(); //Clear the error settings for Ack Nack
	}	
}

void CVlcTransactionData::HandleTransactionReply()
{
	//This  sets m_ack_nack according to the data packet received.
	//(In this case the received packet is not a write request but a transaction reply)
	CVlcData::HandleWriteRequest(); 
	
	//Update default packet
	const CVLCPacket *trans_reply = g_receive_manager->GetLastPacket();
	int ack_code = m_ack_nack->GetAckNackCode();
	if(ack_code == CVLCPacket::CMD_ACK)
	{
		// update the credit amount in the monitor packet
		uint amount = trans_reply->GetDataFieldInt(CVLCPacket::TRANS_REPLY_CASHOUT_AMOUNT);
		CVlcMonitorData* cur_mon_data = (CVlcMonitorData*)(m_vlc_id->GetMonitorCurData());
		cur_mon_data->OutManualPay(amount);	
	}

	//Display the packet received
	g_dlg_transactions->DisplayPacket(*trans_reply);
}

void CVlcTransactionData::SetCashoutAmount(uint m_amount)
{
	m_default->SetDataFieldInt(m_amount,CVLCPacket::TRANS_RQ_CASHOUT_AMOUNT);
	m_next_to_send->SetDataFieldInt(m_amount,CVLCPacket::TRANS_RQ_CASHOUT_AMOUNT);
	g_dlg_transactions->DisplayPacket(*m_next_to_send);
}

