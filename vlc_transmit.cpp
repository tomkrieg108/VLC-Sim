
#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "random.h"
#include "vlc_transmit.h"
#include "timers.h"
#include "dlg_comms_errors.h"
#include "notify.h"


CVlcTransmit  *g_vlc_transmit_manager = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////
CVlcTransmit::CVlcTransmit() :
	m_chunk_count(0),
	m_chunk_index(0),
	m_chunk_delay(0),
	m_comms(0),
	m_packet_being_sent(0),
	m_last_packet_sent(0),
	m_timer_data(0),
	m_timer_rts(0)
{
	//QueryPerformanceCounter( &m_timestamp );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
CVlcTransmit::~CVlcTransmit()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcTransmit::Initialise()
{
	if( g_vlc_transmit_manager == 0 )
	{
		g_vlc_transmit_manager = new CVlcTransmit();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcTransmit::Destroy()
{
	if( g_vlc_transmit_manager != 0 )
	{
		delete(g_vlc_transmit_manager);
		g_vlc_transmit_manager = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Todo - notify that the port is open / closed
void CVlcTransmit::OpenPort( CMSCommCtrl* p_comms)
{
	m_comms = p_comms;
}
void CVlcTransmit::ClosePort(void)
{
	m_comms = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcTransmit::Send(CVLCPacket* p_packet)
{
	if(p_packet==NULL)
		return;

	//todo - status response packets not set to SEND
	//only send if this is meant to be sent...
	//if(p_packet->m_transmit_direction != CVLCPacket::SEND)
	//	return;

	//todo - occasionally the signal indicating that the packet has finished being sent is not triggered
	//therefore ProcessEvent() doesn't get called and m_packet_being_sent doesn't get reset to null
	//ASSERT(m_packet_being_sent==NULL); 
	if(m_packet_being_sent != NULL)
	{
		delete m_packet_being_sent;
		m_packet_being_sent = NULL;
	}

	//make a copy of the packet
	m_packet_being_sent= new CVLCPacket(*p_packet); 

	MoveToBuffer(); 
	SplitBuffer();
	
	int response_delay=0;
	if(p_packet->m_timing_errors != NULL)
		response_delay = p_packet->m_timing_errors->m_poll_response_time;

	
	if(response_delay==0)
	{
		(p_packet->m_timestamp_start).Stamp();
		//m_comms->SetRTSEnable(true);
		m_comms->SetOutput( (COleVariant)( m_output[m_chunk_index] ) );
	}
	else
	{
		m_timer_data = g_timers->AddTimer(response_delay,this);	
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcTransmit::ProcessTimeout( int p_id )
{
	if( m_comms == 0)
	{
		g_timers->RemoveTimer(p_id);
		return;
	}

	if(p_id == m_timer_data)
	{
		g_timers->RemoveTimer(m_timer_data);
		if(m_packet_being_sent)
		{
			if(m_chunk_index==0)
			{
				//start of packet => set timestamp
				(m_packet_being_sent->m_timestamp_start).Stamp();
				//m_comms->SetRTSEnable(true);
			}
			m_comms->SetOutput( (COleVariant)( m_output[m_chunk_index] ) );
		}
	}
	if(p_id == m_timer_rts)
	{
		g_timers->RemoveTimer(m_timer_rts);
		//m_comms->SetRTSEnable(false);	
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcTransmit::ProcessEvent(void)
{
	//A packet chunk has finished being sent
	if( m_comms == 0) return;
	if( m_comms->GetCommEvent() != 1 ) return;

	ASSERT(m_packet_being_sent != NULL);
	if(m_packet_being_sent==NULL) //should not be null
		return;

	m_chunk_index++;
	if(m_chunk_index < m_chunk_count)
	{
		//more chunks to send
		m_timer_data = g_timers->AddTimer(m_chunk_delay,this);	
	}
	else
	{
		//that was the last one
		(m_packet_being_sent->m_timestamp_end).Stamp();
			
		int rts_disable_time = 0;
		if(m_packet_being_sent->m_timing_errors != NULL)
			rts_disable_time = m_packet_being_sent->m_timing_errors->m_rts_disable_time;

		if(rts_disable_time==0)
			;
			//m_comms->SetRTSEnable(false);
		else
		{
			m_timer_rts	= g_timers->AddTimer(rts_disable_time,this);	
		}

		if(m_last_packet_sent!=NULL)	//save the packet that was sent
			delete m_last_packet_sent;
		m_last_packet_sent = m_packet_being_sent;
		m_packet_being_sent=NULL;
		g_notify_mgr->Notify(CNotify::SENT_VLC_PACKET);
		
		//todo
		//copy this packet and add it onto a list so that it can be processed by the logging thread.
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcTransmit::SplitBuffer()
{
	//Splits up the packet into seperate chunks (random size, max chunks is 5), copies into m_output[]
	//As specified by the dialog options in 'comms errors' dialog'
	//CList<int,int> - this doesn't have a sort function! - so use stl

	std::list<int> pos_list;
	std::list<int>::iterator itr;

	int num_splits=0;
	int packet_break_delay=0;
	if(m_packet_being_sent->m_timing_errors != NULL)
	{
		num_splits = m_packet_being_sent->m_timing_errors->m_num_packet_breaks;
		packet_break_delay = m_packet_being_sent->m_timing_errors->m_interchar_time;
	}
	if(num_splits+1>MAX_CHUNKS) 
		return;

	if( (packet_break_delay==0)||(num_splits==0) )
	{
		//no splits - copy all the packet data into m_output[0]
		m_output[0].SetSize(m_buffer.GetSize());
		m_output[0].Copy(m_buffer);
		m_chunk_count=1;
		m_chunk_index=0;
		return;
	}

	for(int i=0; i<num_splits; i++)
	{
		int pos;
		bool ok=false;

		while(!ok)
		{
			//pos = g_random->Number(p_packet->GetLength()-1);
			pos = g_random->Number(m_buffer.GetSize()-1);
		
			// check that this position hasn't already been picked
			ok=true;
			itr = pos_list.begin();
			while( itr != pos_list.end() )
			{
				if( (*itr)==pos ) //already in the list
				{
					ok=false;
					break;
				}
				itr++;
			}
		}
		pos_list.push_front(pos);
	}
	//add in the top position..
	pos_list.push_front(m_buffer.GetSize()-1);
	pos_list.sort();
	
	//////////////////////////////////////
	/*
	itr = pos_list.begin();
	int len = pos_list.size();
	int ar[5];
	int idx=0;
	while( itr != pos_list.end() )
	{
		ar[idx] = (*itr);
		itr++;
		idx++;
	}
	*/
	//////////////////////////////////////

	int buff_pos=0;
	int packet_pos=0;
	int buff_idx=0;

	itr = pos_list.begin();
	int packet_length = m_buffer.GetSize();
	int prev_break_pos=-1;
	m_output[buff_idx].SetSize( (*itr)-prev_break_pos );
	int chunk_size = (*itr)-prev_break_pos;

	for(packet_pos=0; packet_pos<packet_length; packet_pos++)
	{
		m_output[buff_idx].SetAt(buff_pos, m_buffer[packet_pos]);
		buff_pos++;
		if( packet_pos >= (*itr) )
		{
			buff_pos=0;
			buff_idx++;
			prev_break_pos = (*itr);	
			itr++;
			if( itr==pos_list.end() )
				break;
			chunk_size = (*itr)-prev_break_pos;
			m_output[buff_idx].SetSize( (*itr)-prev_break_pos );
		}	
	}

	m_chunk_count=num_splits+1; // the parameter specifies the number of splits in the data-number of chunks is 1 greater
	m_chunk_index=0;
	m_chunk_delay=packet_break_delay;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcTransmit::MoveToBuffer()
{
	//Move the packet data into m_buffer
	//Adds in or removes data as specified by the 'comms errors' dialog options.

	//make a copy of the packet - in case it needs to be modified - keep the original one the same
	//CVLCPacket* packet_copy = new CVLCPacket(*p_packet);

	//Introduce data errors as required by p_packet->m_data_errors;
	CommsErrorsCorruptData* data_errors = m_packet_being_sent->m_data_errors;
	if(data_errors!=NULL)
	{
		bool modified = false;
		if(data_errors->m_header)
		{
			byte header=CVLCPacket::HEADER;
			while(header==CVLCPacket::HEADER)
				header=g_random->Number(0x100);
			m_packet_being_sent->SetHeader(header);
			modified=true;
		}
		if(data_errors->m_terminal_id)
		{
			ushort id = m_packet_being_sent->GetTerminalId();
			while(id==m_packet_being_sent->GetTerminalId())
				id=g_random->Number(0x10000);
			m_packet_being_sent->SetTerminalId(id);
			modified=true;
		}
		if(data_errors->m_n_bits)
		{
			ushort n_bits = m_packet_being_sent->GetNBits();	
			n_bits>>=8;
			if(n_bits >= 1)
				n_bits--; 
			else
				n_bits++;
			n_bits<<=8;
			m_packet_being_sent->SetNBits(n_bits);
			modified = true;
		}
			
		if(data_errors->m_command)
		{
			ushort command = m_packet_being_sent->GetCommand();
			while(command==m_packet_being_sent->GetCommand())
				command=g_random->Number(0x10000);
			m_packet_being_sent->SetCommand(command);
			modified=true;
		}
		if( (data_errors->m_validation_datetime) && (m_packet_being_sent->IsDataPacket()) )
		{
			ushort date = m_packet_being_sent->GetValidationDate();
			while(date==m_packet_being_sent->GetValidationDate())
				date=g_random->Number(0x10000);
			ushort time = m_packet_being_sent->GetValidationTime();
			while(time==m_packet_being_sent->GetValidationTime())
				time=g_random->Number(0x10000);
			m_packet_being_sent->SetValidationDate(date);
			m_packet_being_sent->SetValidationTime(time);
			modified=true;
		}
		if(modified&&(m_packet_being_sent->IsDataPacket()))
			m_packet_being_sent->SetValidationCode(); //set the correct code based on the changes


		if( (data_errors->m_validation_code) && (m_packet_being_sent->IsDataPacket()) )
		{
			ushort code = m_packet_being_sent->GetValidationCodeVal();
			while(code==m_packet_being_sent->GetValidationCodeVal())
				code=g_random->Number(0x10000);
			m_packet_being_sent->SetValidationCodeVal(code);
			modified = true;
		}

		if(modified)
			m_packet_being_sent->SetCrc();	//set the correct crc based on the changes

		if(data_errors->m_crc)
		{
			ushort crc = m_packet_being_sent->GetCrc();
			while(crc==m_packet_being_sent->GetCrc())
				crc=g_random->Number(0x10000);
			m_packet_being_sent->SetCrc(crc);
		}
		
		if(data_errors->m_trailer)
		{
			byte trailer=CVLCPacket::TRAILER;
			while(trailer==CVLCPacket::TRAILER)
				trailer=g_random->Number(0x100);
			m_packet_being_sent->SetTrailer(trailer);
		}
	}

	m_packet_being_sent->ValidatePacket(); //set error statuses in the packet

	//Add or delete or append data as required by p_packet->m_size_errors
	CommsErrorsDataSize* size_errors = m_packet_being_sent->m_size_errors;
	int num_bytes=0;
	CommsErrorsDataSize::EAction action = CommsErrorsDataSize::NONE;
	CommsErrorsDataSize::EFillType fill_type = CommsErrorsDataSize::ZERO;
	if(size_errors != NULL)
	{
		num_bytes = size_errors->m_num_bytes;	
		action = size_errors->m_action;
		fill_type = size_errors->m_fill_type;
	}

	if((num_bytes==0)||(action==CommsErrorsDataSize::NONE))
	{
		m_buffer.SetSize(m_packet_being_sent->GetLength() );
		for(int i=0; i<m_buffer.GetSize(); i++)
		{
			byte b = m_packet_being_sent->GetByte(i);
			m_buffer[i]=m_packet_being_sent->GetByte(i);
		}
	}
	else
	{
		CByteArray extra_bytes;
		extra_bytes.SetSize(num_bytes);
		
		//Fill the extra bytes
		if(fill_type==CommsErrorsDataSize::RANDOM)
		{
			for(int i=0;i<num_bytes;i++)
				extra_bytes[i]=g_random->Number(0x100);
		}
		else
		{
			for(int i=0;i<num_bytes;i++)
				extra_bytes[i]=0;
		}
		
		//Pick the position where to insert or delete bytes from the packet
		int pos=0;
		while(pos==0)
			pos=g_random->Number(m_packet_being_sent->GetLength()-3); //exclude CRC and header.

		//build up m_buffer based on the options set.
		if(action == CommsErrorsDataSize::ADD )
		{
			m_buffer.SetSize(m_packet_being_sent->GetLength() + num_bytes );
			for(int i=0; i<pos; i++)
				m_buffer[i]=m_packet_being_sent->GetByte(i);
			int j=0;
			for(i=pos;i<pos+num_bytes;i++)
				m_buffer[i]=extra_bytes[j++];
			for(i=pos+num_bytes;i<m_packet_being_sent->GetLength()+num_bytes;i++)
				m_buffer[i]=m_packet_being_sent->GetByte(i-num_bytes);
		}
	
		else if(action == CommsErrorsDataSize::REMOVE)
		{
			if(num_bytes>(m_packet_being_sent->GetLength())/4)
				num_bytes=(m_packet_being_sent->GetLength())/4;
			//update pos when removing data to ensure that there are no data access problems
			pos=0;
			while(pos==0)
				pos=g_random->Number(m_packet_being_sent->GetLength()-3-num_bytes); //exclude CRC and header.
			m_buffer.SetSize(m_packet_being_sent->GetLength() - num_bytes );
			for(int i=0; i<pos; i++)
				m_buffer[i]=m_packet_being_sent->GetByte(i);
			for(i=pos; i<m_packet_being_sent->GetLength()-num_bytes; i++)
				m_buffer[i]=m_packet_being_sent->GetByte(i+num_bytes);
		}
		
		else if(action == CommsErrorsDataSize::APPEND)
		{
			m_buffer.SetSize(m_packet_being_sent->GetLength() + num_bytes );
			for(int i=0; i<m_packet_being_sent->GetLength(); i++)
				m_buffer[i]=m_packet_being_sent->GetByte(i);
			int j=0;
			for(i=m_packet_being_sent->GetLength();i<m_packet_being_sent->GetLength()+num_bytes;i++)
				m_buffer[i]=extra_bytes[j++];
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

