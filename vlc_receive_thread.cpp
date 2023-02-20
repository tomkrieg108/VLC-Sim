
#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_main.h"
#include "dlg_comms_errors.h"
#include "vlc_packet.h"
#include "vlc_receive_thread.h"
#include "notify.h"
#include "utilities.h"



CVlcReceive::CVlcReceive():
	m_receiving(false),
	m_index(0),
	m_count(0)
{
	m_packet_data.SetSize(CVLCPacket::LENGTH_PACKET_MAX);
	m_last_packet = NULL;
}

CVlcReceive::~CVlcReceive()	
{
}

void CVlcReceive::Initialise()
{
	if( !g_receive_manager )
		g_receive_manager = new CVlcReceive();
}

void CVlcReceive::Destroy()	
{
	if( g_receive_manager )
	{
		delete g_receive_manager;
		g_receive_manager = 0;
	}
}


void CVlcReceive::ProcessEvent(CMSCommCtrl& p_comms)
{

	if( p_comms.GetCommEvent() != 2 ) // receive
		return;
	
//	If storing each individual part of the packet seperate thread) do this
//	QueryPerformanceCounter( &m_timestamp ); 

	int length = p_comms.GetInBufferCount();

	COleVariant cole_data;
	cole_data.Attach( p_comms.GetInput() );
	long length2;
	SafeArrayGetUBound(cole_data.parray, 1, &length2);
	ASSERT(length <= length2+1);

	//todo:
	//Note: this code assumes that all the data in the input buffer belongs to the same packet.
	//If for some reason this isn't the case then this code wont work!
	
	byte the_byte=0;	
	long i=0;
	byte buff[500];
	memset(buff,0,sizeof(buff));

	bool length_error = false;
	bool length_exceeds_max = false;

	if( !m_receiving ) //look for 0x05 for the start of the packet - should be first byte
	{
		bool found_start=false;
		for(i=0; i<length; i++)
		{
			SafeArrayGetElement(cole_data.parray, &i, &the_byte);
			if(the_byte == CVLCPacket::HEADER) //start of packet
			{
				found_start=true;
				break;
			}
		}
		if(!found_start) 
			return; 

		m_receiving = true;
		m_index = 0;
		for(;i<length;i++)
		{
			SafeArrayGetElement(cole_data.parray, &i, &the_byte);
			if(m_index < CVLCPacket::LENGTH_PACKET_MAX)
			{
				buff[m_index]=the_byte;
				m_packet_data[m_index++] = the_byte;
			}
			else
			{ 
				length_exceeds_max = true;
				break;
			}
		}
	}
	else //continuation of a packet - copy all into m_packet_data
	{
		for(i=0; i<length; i++)
		{
			SafeArrayGetElement(cole_data.parray, &i, &the_byte);
			if(m_index < CVLCPacket::LENGTH_PACKET_MAX)
			{
				buff[m_index]=the_byte;
				m_packet_data[m_index++] = the_byte;
			}
			else
			{
				length_exceeds_max = true;
				break;	
			}
		}
	}
	m_count++; 

	//check to see if all of the packet has been received
	if(m_index<4) 
		return; //havent received command bytes indicating length - still receiving

	int expected_length=0;
	int data_length=0;
	if( m_packet_data[3]&(0x80) ) // MS Bit of command is 1 => no data
		expected_length = CVLCPacket::LENGTH_OVERHEAD;
	else
	{	//packet has data section.
		byte multiples_of_eight = m_packet_data[3]&(0x1F);
		data_length = (multiples_of_eight+1)*8;
		expected_length = data_length + CVLCPacket::LENGTH_OVERHEAD + CVLCPacket::LENGTH_VALIDATION; 
	}

	if(m_index < expected_length)
		return; //still more of the packet to come.

	if(m_index > expected_length)
		length_error = true;

	//debug only
	g_dlg_main->m_receive_debug.SetWindowText( Convert(m_count) );
	m_count = 0;

	//Initialise new packet	
	if(m_last_packet != 0 )
		delete m_last_packet;
	m_last_packet = new CVLCPacket(expected_length);

	// copy packet data into packet object
	for( i=0; i<expected_length; i++)
		m_last_packet->SetByte(m_packet_data[i],i);

	//initialise status fields
	(m_last_packet->m_timestamp_end).Stamp();
	m_last_packet->m_data_errors = NULL;
	m_last_packet->m_timing_errors = NULL;
	m_last_packet->m_size_errors = NULL;
	m_last_packet->m_response = CVLCPacket::RESP_NONE;
	m_last_packet->m_transmit_direction = CVLCPacket::RECEIVE;
	
	//Set error status of received packet
	m_last_packet->m_error_status = CVLCPacket::ERR_NONE;
	if(length_exceeds_max)
		m_last_packet->m_error_status |= CVLCPacket::ERR_LENGTH_MAX;
	if(length_error)
		m_last_packet->m_error_status |= CVLCPacket::ERR_LENGTH;
	if(m_packet_data[m_index-1] != CVLCPacket::TRAILER)
		m_last_packet->m_error_status |= CVLCPacket::ERR_TRAILER;
	m_last_packet->ValidatePacket(); // Will set m_last_packet->m_error_status to an other errors as required

	// complete packet received - reset receiving flag and index
	m_receiving = false;
	m_index = 0;


	/*
		Todo	
		Might be better off not using notifications for this...
		Just call g_id_list->Process()
		Then after it returns call notify to do anything else required...
	*/

	ushort command_type = m_last_packet->GetCommandType();

	if(command_type == CVLCPacket::CMD_POLL)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_POLL );	

	else if(command_type == CVLCPacket::CMD_READ_REQ)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_DATA_READ_REQUEST);

	else if(command_type == CVLCPacket::CMD_SITE_INFO_REQ)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_SITE_INFO_REQUEST);

	else if(command_type == CVLCPacket::CMD_WRITE_REQ)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_WRITE_REQUEST);

	else if(command_type == CVLCPacket::CMD_ACK_NACK)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_ACK_NACK);	

	else if(command_type == CVLCPacket::CMD_TRANSACTION_REPLY)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_TRANSACTION_REPLY);	

	else
	{
		m_last_packet->m_error_status |= CVLCPacket::ERR_COMMAND;
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_UNKNOWN );	
	}
	
	if(m_last_packet->m_error_status != CVLCPacket::ERR_NONE)
		g_notify_mgr->Notify( CNotify::RECEIVE_VLC_ERROR );

	g_notify_mgr->Notify( CNotify::RECEIVE_VLC_PACKET );
}

CVlcReceive *g_receive_manager = 0;
