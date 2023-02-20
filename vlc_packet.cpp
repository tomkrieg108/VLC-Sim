#include "stdafx.h"
#include "vlc_packet.h"
#include "utilities.h"
#include "random.h"
#include "date_time.h"
#include "sa_comms_crc.h"


SaCrc CVLCPacket::m_crc;

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////////////////////////
CVLCPacket::CVLCPacket(int p_length)
{
	InitialiseStatusInfo();
	if(p_length>8)
	{
		m_data.SetSize(p_length);
		m_length = m_data.GetSize();
	}
	else
	{
		m_data.SetSize(8);
		m_length = 8;
	}
	//m_data[0] = HEADER;
	m_data[m_length-1] = TRAILER;	
}


CVLCPacket::CVLCPacket(void)
{
	InitialiseStatusInfo();
	m_data.SetSize(8);
	m_length = 8;
	//m_data[0] = HEADER;
	m_data[m_length-1] = TRAILER;
}


CVLCPacket::CVLCPacket(const CVLCPacket& p_copy)
{
	InitialiseStatusInfo();
	CopyData(p_copy);	
}



CVLCPacket& CVLCPacket::operator = (const CVLCPacket& p_copy)
{
	InitialiseStatusInfo();
	CopyData(p_copy);		
	return *this;
}


CVLCPacket::~CVLCPacket()
{
	if(m_data_errors)
		delete m_data_errors;
	if(m_timing_errors)
		delete m_timing_errors;
	if(m_size_errors)
		delete m_size_errors;
}

void CVLCPacket::CopyData(const CVLCPacket& p_copy)
{
	m_data.SetSize( p_copy.m_data.GetSize() );
	m_data.Copy( p_copy.m_data );
	m_length = p_copy.m_length;
	m_error_status = p_copy.m_error_status;
	m_timestamp_start = p_copy.m_timestamp_start;
	m_timestamp_end = p_copy.m_timestamp_end;
	m_response = p_copy.m_response;
	m_transmit_direction = p_copy.m_transmit_direction;
	m_padding_length = p_copy.m_padding_length;

	if((m_data_errors != NULL)&&(p_copy.m_data_errors != NULL))
		*m_data_errors = *(p_copy.m_data_errors);
	else if((m_data_errors == NULL)&&(p_copy.m_data_errors != NULL))
	{
		m_data_errors = new CommsErrorsCorruptData();
		*m_data_errors = *(p_copy.m_data_errors);
	}
	else if((m_data_errors != NULL)&&(p_copy.m_data_errors == NULL))
	{
		delete m_data_errors;
		m_data_errors = NULL;
	}
	
	if((m_timing_errors != NULL)&&(p_copy.m_timing_errors != NULL))
		*m_timing_errors = *(p_copy.m_timing_errors);
	else if((m_timing_errors == NULL)&&(p_copy.m_timing_errors != NULL))
	{
		m_timing_errors = new CommsErrorsTiming();
		*m_timing_errors = *(p_copy.m_timing_errors);
	}
	else if((m_timing_errors != NULL)&&(p_copy.m_timing_errors == NULL))
	{
		delete m_timing_errors;
		m_timing_errors = NULL;
	}

	if((m_size_errors != NULL)&&(p_copy.m_size_errors != NULL))
		*m_size_errors = *(p_copy.m_size_errors);
	else if((m_size_errors == NULL)&&(p_copy.m_size_errors != NULL))
	{
		m_size_errors = new CommsErrorsDataSize();
		*m_size_errors = *(p_copy.m_size_errors);
	}
	else if((m_size_errors != NULL)&&(p_copy.m_size_errors == NULL))
	{
		delete m_size_errors;
		m_size_errors = NULL;
	}
}

void CVLCPacket::InitialiseStatusInfo()
{
	//Don't initialise timestamps
	//Leave that to receive / transmit mgrs
	m_error_status = ERR_NONE;
	m_data_errors = NULL;
	m_timing_errors = NULL;
	m_size_errors = NULL;
	m_response = RESP_NONE;
	m_transmit_direction = RECEIVE;
	m_padding_length=0;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//Misc

void CVLCPacket::SetLength(int p_length ) 
{
	if(p_length < 8)
		m_data.SetSize(8);
	else
		m_data.SetSize(p_length);

	m_length = m_data.GetSize();

	m_data[0] = HEADER; 
	m_data[m_length-1] = TRAILER;
}

void CVLCPacket::SetDataLength(int p_length ) 
{
//Set the data length of a data packet - pads with zeros as needed 
//to ensure even multiple of 8 bytes.
	if(p_length <= 0) 
		return;
	if( !IsDataPacket() )
		return;
	if(p_length == GetDataLength() )
		return;

	//save existing overhead and validation data in the packet
	//this is in case calling SetSize() mucks is up?
	CByteArray save_data;
	save_data.SetSize(16);

	int size = m_data.GetSize();
	for(int i=0; i<5; i++)
		save_data[i] = m_data[i];
	for(i=0; i<11; i++)
		save_data[15-i] = m_data[size-i-1];
	
	int length = p_length + p_length%8; //make sure its an even multiple of 8
	m_data.SetSize(length + 16);
	size = m_data.GetSize();
	//restore data
	for(i=0; i<5; i++)
		m_data[i] = save_data[i];
	for(i=0; i<11; i++)
		m_data[size-i-1] = save_data[15-i];

	m_length = m_data.GetSize();
	SetNBits();
}

void CVLCPacket::PadZero()
{
	if(!IsDataPacket() ) 
		return;
	int cur_len = GetDataLength();
	SetNBits();
	int bytes_to_pad = 8 - cur_len%8;
	if(bytes_to_pad==8)
	{
		m_padding_length=0;
		return; //already an even multiple of 8.
	}

	m_padding_length = bytes_to_pad;
	CByteArray padding;
	padding.SetSize(bytes_to_pad);
	for(int i=0; i<bytes_to_pad; i++)
		padding[i]=0;
	InsertData(padding, OffsetValidation()); //This updates the n bits in the command too
	
}

void  CVLCPacket::RemovePadZero()
{
	if(!IsDataPacket() ) 
		return;
	if(m_padding_length==0)
		return;
	ASSERT(m_padding_length<m_length);
	CByteArray data;
	data.SetSize(m_data.GetSize());
	data.Copy(m_data);
	int cur_len = m_data.GetSize(); 
	ASSERT(cur_len==m_length);
	m_data.SetSize(cur_len - m_padding_length);	
	for(int i=0; i<(OffsetValidation()-m_padding_length); i++)
		m_data[i]=data[i];
	for(i= OffsetValidation(); i<cur_len; i++)
		m_data[i-m_padding_length]=data[i];
	m_length = cur_len - m_padding_length;
	m_padding_length=0;
	ASSERT(m_length==m_data.GetSize());
}

void CVLCPacket::CopyPacketData(const CVLCPacket& p_copy)
{
	if(!IsDataPacket() ) 
	{
		if(!p_copy.IsDataPacket() )
			m_data.Copy( p_copy.m_data );	
		return;
	}
	//save existing overhead and validation data in the packet
	CByteArray save_data;
	save_data.SetSize(16);

	int size = m_data.GetSize();
	for(int i=0; i<5; i++)
		save_data[i] = m_data[i];
	for(i=0; i<11; i++)
		save_data[15-i] = m_data[size-i-1];
	//Copy the packet data only - don't touch any of the status info/
	//error settings etc.
	m_data.SetSize( p_copy.m_data.GetSize() );
	m_data.Copy( p_copy.m_data );
	m_length = p_copy.m_length;
	m_padding_length = p_copy.m_padding_length;
	size = m_data.GetSize();
	//restore data
	for(i=0; i<5; i++)
		m_data[i] = save_data[i];
	for(i=0; i<11; i++)
		m_data[size-i-1] = save_data[15-i];

	SetNBits(); //incase sizes are different.
}


void  CVLCPacket::SetErrorStatus(byte p_status)
{
	m_error_status |= p_status;
}

byte  CVLCPacket::GetErrorStatus() const
{
	return m_error_status;	
}

///////////////////////////////////////////////////////////////////////////////////////
// Put the whole packet into a string
// Todo...Want to be able the set the formatting of this:
// number of rows to display,
// Info about the packet - type/length/ID/status bits 
CString CVLCPacket::GetAsString(void) const
{
	CString val = "";
	GetHexString(val,0,m_length);
	return val;
}

CString* CVLCPacket::GetErrorsAsString() const
{
	CString* str_err = NULL;
	if(m_error_status != ERR_NONE)
	{
		str_err = new CString();
		*str_err = "Errors: ";
		if(m_error_status & ERR_CRC)		*str_err += "CRC ";
		if(m_error_status & ERR_TIMESTAMP)	*str_err += "Timestamp Out of Window ";
		if(m_error_status & ERR_COMMAND)	*str_err += "Invalid Command ";
		if(m_error_status & ERR_CODE)		*str_err += "Validation Code ";
		if(m_error_status & ERR_LENGTH)		*str_err += "Length ";
		if(m_error_status & ERR_LENGTH_MAX) *str_err += "Max Length ";
		if(m_error_status & ERR_TRAILER)	*str_err += "Trailer ";
	}
	return str_err;
}
CString* CVLCPacket::GetDescriptionAsString() const
{
	CString* str_descr = new CString();

	ushort command_type = GetCommandType();	
	//////////////////////////////////////////////////
	//bits CCC
	if(m_transmit_direction == RECEIVE)
	{
		*str_descr = "Receive ";
		switch(command_type)
		{
			case CMD_POLL:				*str_descr += "Poll, ";					break;
			case CMD_READ_REQ:			*str_descr += "Read Request-";			break;
			case CMD_WRITE_REQ:			*str_descr += "Write Request-";			break;
			case CMD_TRANSACTION_REPLY: *str_descr += "Transaction Reply-";		break;
			case CMD_SITE_INFO_REQ:		*str_descr += "Site Info Request-";		break;
			case CMD_ACK_NACK:													break;
			default:					*str_descr += "Unknown-";				break;
		}
	}
	else
	{
		*str_descr = "Send ";
		switch(command_type)
		{	
			case CMD_STATUS_RESPONSE:	*str_descr += "Status Response- ";		break;
			case CMD_READ_REPLY:		*str_descr += "Read Reply-";			break;
			case CMD_TRANSACTION_REQ:	*str_descr += "Transaction Request-";	break;
			case CMD_SITE_INFO_REPLY:	*str_descr += "Site Info Reply-";		break;
			case CMD_ACK_NACK:													break;
			default:					*str_descr += "Unknown-";				break;
		}
	}
	
	//////////////////////////////////////////////////
	//bits AAAAA
	if(command_type == CMD_ACK_NACK)
	{
		ushort ack_code = GetAckNackCode();
		switch(ack_code)
		{
			case CMD_ACK:					*str_descr += "ACK-";							break;	
			case CMD_NACK_CRC:				*str_descr += "NACK:CRC Error-";				break;	
			case CMD_NACK_TIMESTAMP:		*str_descr += "NACK:Timestamp Out Of Window-";	break;	
			case CMD_NACK_COMMAND:			*str_descr += "NACK:Invalid Command-";			break;	
			case CMD_NACK_CODE:				*str_descr += "NACK:Validation Code Error-";	break;	
			case CMD_NACK_TRANSACTION:		*str_descr += "NACK:Transaction Error-";		break;	
			case CMD_NACK_CONFIGURATION:	*str_descr += "NACK:Configuration Error-";		break;	
			case CMD_NACK_NON_ZERO_BALANCE:	*str_descr += "NACK:Non-Zero Credit Balance-";	break;	
			default:						*str_descr += "NACK:Unknown-";					break;	
		};

		//bits KKK
		ushort ack_command = GetAckNackCommandType();

		if(m_transmit_direction==SEND) //sending an ACK
		{
			switch(ack_command)
			{	
				case CMD_ACK_WRITE_REQ:			*str_descr += "Write Request, ";		break;
				case CMD_ACK_TRANSACTION_REPLY:	*str_descr += "Transaction Reply ";		break;	
			};
		}
		else	//receiving an ack
		{
			switch(ack_command)
			{		
				case CMD_ACK_READ_REPLY:		*str_descr += "Read Reply, ";			break;
				case CMD_ACK_TRANSACTION_REQ:	*str_descr += "Trans Request, ";		break;
				case CMD_ACK_SITE_INFO_REPLY:	*str_descr += "Site Info Reply, ";		break;
			}
		}

		//o flag
		*str_descr += "Flags: ";	
		if(IsCommandFlagSet(CMD_FLG_O)) *str_descr += "O ";
		*str_descr += ", ";
	}
	
	/////////////////////////////////////////////////////
	//flags
	if((m_transmit_direction == RECEIVE)&&(command_type==CMD_POLL))
	{
		*str_descr += "Flags: ";	
		if(IsCommandFlagSet(CMD_FLG_V)) *str_descr += "V ";
		if(IsCommandFlagSet(CMD_FLG_D)) *str_descr += "D ";
		if(IsCommandFlagSet(CMD_FLG_F)) *str_descr += "F ";
		if(IsCommandFlagSet(CMD_FLG_P)) *str_descr += "P ";
		if(IsCommandFlagSet(CMD_FLG_I)) *str_descr += "I ";
		*str_descr += ", ";
	}

	if((m_transmit_direction == SEND)&&(command_type==CMD_STATUS_RESPONSE))
	{
		*str_descr += "Flags: ";	
		if(IsCommandFlagSet(CMD_FLG_D)) *str_descr += "D ";
		if(IsCommandFlagSet(CMD_FLG_B)) *str_descr += "B ";
		*str_descr += ", ";
	}

	////////////////////////////////////////////////////////
	//bits RRR
	if( ((m_transmit_direction==RECEIVE)&&((command_type==CMD_READ_REQ)	 ||(command_type==CMD_SITE_INFO_REQ))) ||
		((m_transmit_direction==SEND)   &&((command_type==CMD_READ_REPLY)||(command_type==CMD_SITE_INFO_REPLY)))  )
	{
		ushort data_read_type = GetDataReadType();		
		switch(data_read_type)
		{
			case CMD_READ_CONTROL:		*str_descr += "Control, ";				break;
			case CMD_READ_MONITER_END:	*str_descr += "Monitor Day End, ";		break;
			case CMD_READ_CONFIGURE:	*str_descr += "Configuration, ";		break;
			case CMD_READ_STATS_END:	*str_descr += "Stats Week End, ";		break;
			case CMD_READ_BANNER:		*str_descr += "Banner, ";				break;
			case CMD_READ_MONITER_CUR:	*str_descr += "Monitor Current, ";		break;
			case CMD_READ_STATS_CUR:	*str_descr += "Stats Current, ";		break;
			case CMD_READ_EVENT_LOG:	*str_descr += "Event Log, ";			break;
			case CMD_READ_DATETIME_MS:	*str_descr += "DateTime Memory Sig, ";	break;
			case CMD_READ_CONTINUATION: *str_descr += "Continuation, ";			break;
			default:					*str_descr += "Unknown Data Type, ";	break;
		};
	}
	
	///////////////////////////////////////////////////////////
	//bits WWW
	bool o_flag=false;
	if((m_transmit_direction == RECEIVE)&&(command_type==CMD_WRITE_REQ))
	{
		ushort data_write_type = GetDataWriteType();		
		switch(data_write_type)
		{
			case CMD_WRITE_CONTROL:		*str_descr += "Control, ";				break; 
			case CMD_WRITE_CONFIGURE:	*str_descr += "Configuration, ";		break;  
			case CMD_WRITE_BANNER:		*str_descr += "Banner, ";				break;  
			case CMD_WRITE_DATETIME_MS:	*str_descr += "DateTime Memory Sig,  ";	break;  
			case CMD_WRITE_CONTINUATION:*str_descr += "Continuation, ";			break;  		
			default:					*str_descr += "Unknown Data Type, ";	break;
		};
		o_flag = IsCommandFlagSet(CMD_FLG_O);
	}
	
	/////////////////////////////////////////////////////////////
	//bits NNNNN, Q, O
	if(IsDataPacket())
	{
		*str_descr += "Data Length: ";
		*str_descr += Convert(GetDataLength());
		*str_descr += ", ";
		*str_descr += "Flags: ";
		if(IsCommandFlagSet(CMD_FLG_Q)) *str_descr += "Q ";
		if(o_flag)
			*str_descr += "O ";
		*str_descr += ", ";
	}

	/////////////////////////////////////////////////////////////
	//bits SS
	ushort source = GetSourceType();
	switch(source)
	{
		case CMD_SRC_EGM:					*str_descr += "Source: EGM ";			break;
		case CMD_SRC_AUDIT_DEVICE:			*str_descr += "Source: Audit Device ";	break;
		case CMD_SRC_GMMS_HOST:				*str_descr += "Source: GMMS Host ";		break;
		case CMD_SRC_GMMS_SITE_CONTROLLER:	*str_descr += "Source: Site Controller ";	break;
	}

	return str_descr;
}

void CVLCPacket::GetAsStringList(StringList& p_string_list, bool p_show_errors, bool p_show_descr, bool p_show_timestamp) const
{
	if(p_show_descr)
		p_string_list.push_front( GetDescriptionAsString() );

	if(p_show_errors)
	{
		CString* err_str = GetErrorsAsString();
		if(err_str != NULL)
			p_string_list.push_front(err_str);
	}	

	CString pkt_str = "";
	GetHexString(pkt_str,0,m_length);
	
	CString* cur_str = new CString();

	const int bytes_per_row = 40;
	const int bytes_per_interval = 10;
	int cur_bytes_in_row = 0;
	int cur_bytes_in_interval = 0;
	int idx=0;
	while(idx<pkt_str.GetLength())
	{
		*cur_str += pkt_str[idx++];
		if(idx>=pkt_str.GetLength())
			break;
		*cur_str += pkt_str[idx++];
		*cur_str += " ";
		cur_bytes_in_row++;
		cur_bytes_in_interval++;
		if(cur_bytes_in_interval==bytes_per_interval)
		{
			*cur_str += "   ";
			cur_bytes_in_interval=0;
		}
		if(cur_bytes_in_row==bytes_per_row)
		{
			p_string_list.push_front(cur_str);	
			cur_str = new CString();
			cur_bytes_in_row = 0;
		}
	}
	if(cur_str->GetLength()>0)
		p_string_list.push_front(cur_str);	
}

CVLCPacket::EResponse CVLCPacket::GetResponseFromAckCode(ushort p_ack_code)
{
	switch(p_ack_code)
	{
		case CMD_ACK:					return RESP_ACK;
		case CMD_NACK_CRC:				return RESP_NACK_CRC;
		case CMD_NACK_TIMESTAMP:		return RESP_NACK_TIMESTAMP;
		case CMD_NACK_COMMAND:			return RESP_NACK_COMMAND;
		case CMD_NACK_CODE:				return RESP_NACK_CODE;
		case CMD_NACK_TRANSACTION:		return RESP_NACK_TRANSACTION;
		case CMD_NACK_CONFIGURATION:	return RESP_NACK_CONFIGURATION;
		case CMD_NACK_NON_ZERO_BALANCE:	return RESP_NACK_NON_ZERO_BALANCE;
		default:						return RESP_NONE;
	};
}

ushort CVLCPacket::GetAckCodeFromResponse(CVLCPacket::EResponse p_response)
{
	switch(p_response)
	{
		case EResponse::RESP_ACK:					return CMD_ACK;
		case EResponse::RESP_NACK_CRC:				return CMD_NACK_CRC;
		case EResponse::RESP_NACK_TIMESTAMP:		return CMD_NACK_TIMESTAMP;
		case EResponse::RESP_NACK_COMMAND:			return CMD_NACK_COMMAND;
		case EResponse::RESP_NACK_CODE:				return CMD_NACK_CODE;
		case EResponse::RESP_NACK_TRANSACTION:		return CMD_NACK_TRANSACTION;
		case EResponse::RESP_NACK_CONFIGURATION:	return CMD_NACK_CONFIGURATION;
		case EResponse::RESP_NACK_NON_ZERO_BALANCE:	return CMD_NACK_NON_ZERO_BALANCE;
		default: return CMD_ACK;	
	};
}

void CVLCPacket::GetResponseAsString(CString &p_str, EResponse p_response)
{
	switch(p_response)
	{
		case EResponse::RESP_ACK:					p_str = CString("Ack"); return;
		case EResponse::RESP_NACK_CRC:				p_str = CString("Nack-Crc"); return;
		case EResponse::RESP_NACK_TIMESTAMP:		p_str = CString("Nack-Timestamp"); return;
		case EResponse::RESP_NACK_COMMAND:			p_str = CString("Nack-Command"); return;
		case EResponse::RESP_NACK_CODE:				p_str = CString("Nack-code"); return;
		case EResponse::RESP_NACK_TRANSACTION:		p_str = CString("Nack-Transaction"); return;
		case EResponse::RESP_NACK_CONFIGURATION:	p_str = CString("Nack-Configuration"); return;
		case EResponse::RESP_NACK_NON_ZERO_BALANCE:	p_str = CString("Nack-None zero credit"); return;
		case EResponse::RESP_NONE:					p_str = CString("None"); return;
		default: p_str = CString(""); return;	
	};
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Set and Get packet data


void CVLCPacket::SetByte( byte p_val, int p_pos)
{
	if( p_pos < m_length )
	{
		m_data[p_pos] = p_val;
	}
}

byte CVLCPacket::GetByte( int p_pos ) const
{
	byte val = 0;
	if( p_pos < m_length )
	{
		val = m_data[p_pos];
	}
	return val;
}

void CVLCPacket::SetShort(ushort p_val, int p_pos)
{
	if(p_pos<0) return;
	if(p_pos>m_length-2) return;
	m_data[p_pos] =   (uchar)(p_val>>8);
	m_data[p_pos+1] = (uchar)(p_val);	
}


ushort CVLCPacket::GetShort(int p_pos) const
{
	ushort val=0;

	if(p_pos<0) return val;
	if(p_pos>m_length-2) return val;
	val = (m_data[p_pos]<<8);
	val |=m_data[p_pos+1];
	return val;
}


void CVLCPacket::SetInt(uint p_val, int p_pos)
{
	if(p_pos<0) return;
	if(p_pos>m_length-4) return;
	m_data[p_pos] =  (uchar)(p_val>>24);
	m_data[p_pos+1] = (uchar)(p_val>>16);
	m_data[p_pos+2] = (uchar)(p_val>>8);
	m_data[p_pos+3] = (uchar)(p_val);
}

uint CVLCPacket::GetInt(int p_pos) const
{
	uint val=0;
	if(p_pos<0) return val;
	if(p_pos>m_length-4) return val;
	val =  (m_data[p_pos]<<24);
	val |= (m_data[p_pos+1]<<16);
	val |= (m_data[p_pos+2]<<8);
	val |= (m_data[p_pos+3]);
	return val;
}


void CVLCPacket::SetInt3(uint p_val, int p_pos)
{
	if(p_pos<0) return;
	if(p_pos>m_length-3) return;
	m_data[p_pos] =  (uchar)(p_val>>16);
	m_data[p_pos+1] = (uchar)(p_val>>8);
	m_data[p_pos+2] = (uchar)(p_val);
}

uint CVLCPacket::GetInt3(int p_pos) const
{
	uint val=0;
	if(p_pos<0) return val;
	if(p_pos>m_length-3) return val;
	val =  (m_data[p_pos]<<16);
	val |= (m_data[p_pos+1]<<8);
	val |= (m_data[p_pos+2]);
	return val;
}

void  CVLCPacket::SetIntX(uint p_val, int p_pos, int p_size)
{
	if(p_size==1) SetByte((byte)(p_val),p_pos);
	if(p_size==2) SetShort((ushort)(p_val),p_pos);
	if(p_size==3) SetInt3(p_val,p_pos);
	if(p_size==4) SetInt(p_val, p_pos);
}


uint CVLCPacket::GetIntX(int p_pos, int p_size) const
{
	if(p_size==1) return (uint)(GetByte(p_pos));
	if(p_size==2) return (uint)(GetShort(p_pos));
	if(p_size==3) return (uint)(GetInt3(p_pos));
	if(p_size==4) return (uint)(GetInt(p_pos));
	return 0;
}

bool CVLCPacket::SetData(CByteArray& p_data, int p_pos)
{
	int length = p_data.GetSize();
	if(length <= 0) 
		return false;
	if((p_pos < 0) || (p_pos+length > m_length )) 
		return false;
	for(int i=0; i<length; i++)
		m_data[p_pos+i] = p_data[i];
	return true;
}


bool CVLCPacket::GetData(CByteArray& p_data, int p_pos, int p_length) const
{
	if(p_length <= 0) 
		return false;
	if((p_pos < 0) || (p_pos+p_length > m_length )) 
		return false;
	p_data.SetSize(p_length);
	for(int i=0; i< p_length; i++)
		p_data[i] = m_data[p_pos+i];
	return true;
}

bool CVLCPacket::InsertData(CByteArray& p_data, int p_pos)
{
	if(!IsDataPacket())
		return false;
	int bytes_to_insert = p_data.GetSize();
	if(bytes_to_insert==0)
		return false;
	if((p_pos < START_DATA) || (p_pos > m_length )) 
		return false;

	int cur_data_length = m_data.GetSize();
	ASSERT(cur_data_length == m_length);
	int new_data_length = cur_data_length + bytes_to_insert;
	int bytes_to_shift = cur_data_length - p_pos;

	//save the data that needs to be shifted
	CByteArray save_data;
	save_data.SetSize(bytes_to_shift);
	ASSERT(save_data.GetSize()==bytes_to_shift);
	for(int i=0; i<bytes_to_shift; i++)
		save_data[i] = m_data[p_pos+i];

	m_data.SetSize(new_data_length);
	m_length = m_data.GetSize();
	ASSERT(new_data_length == m_length);

	//Restore the data than needs to be shifted
	for(i=0; i<bytes_to_shift; i++)
		m_data[p_pos+bytes_to_insert+i] = save_data[i];

	//Save the new data
	for(i=0; i<bytes_to_insert; i++)
		m_data[p_pos+i] = p_data[i];
	
	SetNBits();

	return true;
}


bool CVLCPacket::SetDataRv(CByteArray& p_data, int p_pos, int p_length)
{
	if(p_length <= 0) 
		return false;
	if((p_pos < 0) || (p_pos+p_length > m_length )) 
		return false;
	p_data.SetSize(p_length);
	for(int i=0; i< p_length; i++)
		p_data[p_length-(i+1)] = m_data[p_pos+i];
	return true;
}

bool  CVLCPacket::GetDataRv(CByteArray& p_data, int p_pos, int p_length) const
{
	if(p_length <= 0) 
		return false;
	if((p_pos < 0) || (p_pos+p_length > m_length )) 
		return false;
	p_data.SetSize(p_length);
	for(int i=0; i< p_length; i++)
		p_data[p_length-(i+1)] = m_data[p_pos+i];
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Formatted data

bool CVLCPacket::SetDateString(CString& p_string, int p_pos)
{
	CVLCDateTime date_time;
	if(!date_time.SetDate(p_string) )
		return false;
		
	ushort date = date_time.GetJulianDate();		
	SetShort(date,p_pos);
	return true;
}
bool CVLCPacket::GetDateString(CString& p_string, int p_pos) const
{
	ushort date = GetShort(p_pos);
	if(date==0)
		p_string = CString("000000");
	else
	{
		CVLCDateTime date_time;
		date_time.Set(date,0);
		p_string=date_time.GetDate();
	}
	return true;
}

bool CVLCPacket::SetTimeString(CString& p_string, int p_pos)
{
	CVLCDateTime date_time;
	if(!date_time.SetTime(p_string))
		return false;
	ushort time = date_time.GetTimeInSeconds();
	SetShort(time,p_pos);
	return true;
}

bool CVLCPacket::GetTimeString(CString& p_string, int p_pos) const
{
	ushort time = GetShort(p_pos);
	CVLCDateTime date_time;
	date_time.Set(0,time);
	p_string=date_time.GetTime();
	return true;
}

bool CVLCPacket::SetDecString(CString& p_string, int p_pos, int p_length)
{
	if(p_string.IsEmpty())
		return false;
	int value = Convert(p_string);
	SetIntX(value,p_pos,p_length);
	return true;
}

bool CVLCPacket::GetDecString(CString& p_string, int p_pos, int p_length) const
{
	uint value = GetIntX(p_pos,p_length);
	p_string = Convert(value);
	return true;
}


bool CVLCPacket::SetHexString(CString& p_string, int p_pos)
{
	int string_length = p_string.GetLength();
	if(string_length==0)
		return false;
	if(p_pos<0)
		return false;
	if(p_pos+string_length/2 > m_length )
		return false;
	byte upper=0;
	byte lower=0;
	//if even number of bytes, first byte is upper nibble, otherwise lower nibble
	bool is_upper=(string_length%2)==0; 
	int arr_idx=p_pos;
	for(int i=0; i<string_length; i++)
	{
		byte b = CharToHex(p_string[i]);
		if(is_upper)
			upper=b<<4; 
		else
		{
			lower=b;
			m_data[arr_idx] = upper|lower;
			arr_idx++;
		}
		is_upper = !is_upper;	
	}
	return true;
}

bool CVLCPacket::GetHexString(CString& p_string, int p_pos, int p_length) const
{
	byte value = 0;
	if(p_length <= 0) 
		return false;
	if((p_pos < 0) || (p_pos+p_length > m_length )) 
		return false;
	p_string.Empty();
	for(int i=0; i< p_length; i++)
	{
		value = m_data.GetAt( p_pos+i );
		char upper = HexToChar( value >> 4 );
		char lower = HexToChar( value & 0x0F );
		p_string = p_string + upper;
		p_string = p_string + lower;
	}
	return true;
}


bool CVLCPacket::SetAsciiString(CString& p_string, int p_pos)
{	
	int string_length = p_string.GetLength();
	if(string_length==0)
		return false;
	if(p_pos<0)
		return false;
	if(p_pos+string_length > m_length )
		return false;

	for(int i=0; i<string_length; i++)
		m_data[p_pos+i] = (byte)(p_string[i]);
	return true;
}

bool CVLCPacket::GetAsciiString(CString& p_string, int p_pos, int p_length) const
{
	byte value = 0;
	if(p_length <= 0) 
		return false;
	if((p_pos < 0) || (p_pos+p_length > m_length )) 
		return false;
	p_string.Empty();
	for(int i=0; i<p_length; i++)
		p_string += (char)(m_data[p_pos+i]);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//Data fields

bool CVLCPacket::ValidateDataField(int p_field, int p_offset) const
{
	if( !IsDataPacket() )
		return false;
	if( (p_field<0)||(p_field>=VLC_FIELDS_TOTAL)) 
		return false;
	int start = m_fields[p_field][0]-1+p_offset;
	int length = m_fields[p_field][1];
	if( GetDataLength() < start+length)
		return false;
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);
	if( (data_type<0)||(data_type>=NUM_DATA_TYPES) )
		return false;
	return true;
}

void CVLCPacket::GetDataField(CString& p_string, int p_field, int p_offset ) const
{
	p_string.Empty();
	if(!ValidateDataField(p_field, p_offset))
		return;
	int start = m_fields[p_field][0]-1+p_offset;
	int length = m_fields[p_field][1];
	GetHexString(p_string, start, length);
}


void CVLCPacket::SetDataField(CString& p_string, int p_field, int p_offset )
{
	if(!ValidateDataField(p_field,p_offset))
		return;
	int start = m_fields[p_field][0]-1+p_offset;
	SetHexString(p_string, start);
}

void CVLCPacket::GetDataFieldFormatted(CString& p_string, int p_field, int p_offset ) const
{
	p_string.Empty();
	if(!ValidateDataField(p_field,p_offset))
		return;
	int pos = m_fields[p_field][0]-1 + START_DATA + p_offset; 
	int length = m_fields[p_field][1];
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);

	switch(data_type)
	{
		case BINARY:
		{
			GetDecString(p_string,pos,length);
			return;
		}
		case TIME_U:
		case TIME_R:
		{
			if(length!=2) 
				return;
			GetTimeString(p_string,pos);
			return;	
		}

		case DATE_U:
		case DATE_R:
			if(length!=2)
				return;
			GetDateString(p_string,pos);
			return;

		case ASCII:
			GetAsciiString(p_string,pos,length);
			return;

		case HEX:
		case CODE:
		case FLAGS:
		case VARIABLE:
			GetHexString(p_string,pos,length);

		default:
			return;
	};
}


void CVLCPacket::SetDataFieldFormatted(CString& p_string, int p_field, int p_offset )
{
	if(!ValidateDataField(p_field, p_offset))
		return;
	int pos = m_fields[p_field][0]-1 + START_DATA + p_offset; 
	int length = m_fields[p_field][1];
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);

	switch(data_type)
	{
		case BINARY:
		{
			SetDecString(p_string,pos,length);
			return;
		}
		case TIME_U:
		case TIME_R:
		{
			if(length!=2) 
				return;
			SetTimeString(p_string,pos);
			return;	
		}

		case DATE_U:
		case DATE_R:
			if(length!=2)
				return;
			SetDateString(p_string,pos);
			return;

		case ASCII:
			SetAsciiString(p_string,pos);
			return;

		case HEX:
		case CODE:
		case FLAGS:
		case VARIABLE:
			SetHexString(p_string,pos);

		default:
			return;
	};
}


uint CVLCPacket::GetDataFieldInt(int p_field, int p_offset ) const
{
	if(!ValidateDataField(p_field, p_offset))
		return 0;
	int pos = m_fields[p_field][0]-1 + START_DATA + p_offset; 
	int length = m_fields[p_field][1];
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);

	if(length>4)
		return 0;

	return (uint)(GetIntX(pos,length));
}


void CVLCPacket::SetDataFieldInt(uint p_val, int p_field, int p_offset )
{
	if(!ValidateDataField(p_field, p_offset))
		return;
	int pos = m_fields[p_field][0]-1 + START_DATA + p_offset; 
	int length = m_fields[p_field][1];
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);

	if(length>4)
		return;

	SetIntX(p_val,pos,length);
}

void CVLCPacket::GetDataFieldHex(CByteArray& p_data, int p_field, int p_offset ) const
{
	if(!ValidateDataField(p_field, p_offset))
		return;
	int pos = m_fields[p_field][0]-1 + START_DATA + p_offset; 
	int length = m_fields[p_field][1];
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);

	p_data.SetSize(length);
	GetData(p_data, pos, length);
}


void CVLCPacket::SetDataFieldHex(CByteArray& p_data, int p_field, int p_offset )
{
	if(!ValidateDataField(p_field, p_offset))
		return;
	int pos = m_fields[p_field][0]-1 + START_DATA + p_offset; 
	int length = m_fields[p_field][1];
	E_DATA_TYPE data_type = (E_DATA_TYPE)(m_fields[p_field][2]);

	p_data.SetSize(length);
	SetData(p_data, pos);
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//Overhead data

bool CVLCPacket::IsDataPacket() const
{
	//return (! (m_data[3]&(0x80) ) );
	return m_length>=LENGTH_VALIDATION+LENGTH_OVERHEAD;
}

int CVLCPacket::GetDataLength() const
{
	//byte multiples_of_eight = m_data[3]&(0x1F);
	//return (int)( (multiples_of_eight+1)*8);
	int data_length = m_length - (LENGTH_VALIDATION+LENGTH_OVERHEAD);
	if(data_length>0)
		return data_length;
	else
		return 0;
}




bool CVLCPacket::IsCommandFlagSet(ushort p_flag) const
{
	return (GetCommand()&p_flag) != 0;
}


CVLCPacket::EDataType CVLCPacket::GetDataType() const
{
	if(!IsDataPacket())
		return DATA_TYPE_NONE;

	ushort command_type = GetCommandType();

	
	if(  (command_type == CVLCPacket::CMD_READ_REPLY)||
		 (command_type == CVLCPacket::CMD_SITE_INFO_REPLY ))
	{
		ushort r_bits = GetDataReadType();
		switch (r_bits)
		{
			case CVLCPacket::CMD_READ_CONFIGURE:	return DATA_TYPE_CONFIGURATION;
			case CVLCPacket::CMD_READ_CONTROL:		return DATA_TYPE_CONTROL;
			case CVLCPacket::CMD_READ_MONITER_CUR:	return DATA_TYPE_MONITOR;
			case CVLCPacket::CMD_READ_MONITER_END:	return DATA_TYPE_MONITOR;	
			case CVLCPacket::CMD_READ_STATS_CUR:	return DATA_TYPE_STATISTICS;
			case CVLCPacket::CMD_READ_STATS_END:	return DATA_TYPE_STATISTICS;
			case CVLCPacket::CMD_READ_EVENT_LOG:	return DATA_TYPE_EVENT_LOG;
			case CVLCPacket::CMD_READ_DATETIME_MS:	return DATA_TYPE_DATETIME_MS;
			case CVLCPacket::CMD_READ_CONTINUATION:	return DATA_TYPE_CONTINUATION;

			default: return DATA_TYPE_NONE;
		};
	}
	else if(command_type == CVLCPacket::CMD_WRITE_REQ)
	{
		ushort w_bits = GetDataWriteType();
	
		switch (w_bits)
		{
			case CVLCPacket::CMD_WRITE_CONFIGURE:	return DATA_TYPE_CONFIGURATION;
			case CVLCPacket::CMD_WRITE_CONTROL:		return DATA_TYPE_CONTROL;
			case CVLCPacket::CMD_WRITE_DATETIME_MS: return DATA_TYPE_DATETIME_MS;
			case CVLCPacket::CMD_WRITE_CONTINUATION:return DATA_TYPE_CONTINUATION;	

			default: return DATA_TYPE_NONE;
		};
	}
	else if( (command_type==CVLCPacket::CMD_TRANSACTION_REQ)&&(m_transmit_direction==SEND)  )	
		return DATA_TYPE_TRANSACTION_REQUEST;

	else if( (command_type==CVLCPacket::CMD_TRANSACTION_REPLY)&&(m_transmit_direction==RECEIVE)	)
		return DATA_TYPE_TRANSACTION_REPLY;

	else
		return DATA_TYPE_NONE;
}



void CVLCPacket::SetCommandFlag(ushort p_flag, bool p_status)
{
	ushort flags_compl = ~p_flag; //complement of the flag - all bits set except for the flag bit
	
	ushort command = GetCommand();
	command &= flags_compl;			//clears the flag bit

	if(p_status)
		command |= p_flag; //re-sets it if required

	SetCommand(command);
}

ushort CVLCPacket::CalculateNBits() const
{
	ushort n_bits = 0;

	if( !IsDataPacket() )
		return n_bits;
	int data_length = GetDataLength();
	//should be a multiple of 8, but just in case...
	data_length += (data_length%8);
	n_bits = (data_length/8) -1;
	if(n_bits<0)
		return 0; 
	n_bits<<=8;
	return n_bits;
}
//////////////////////////////////////////////////////////////////////////////////////
//Get and Set Status
void CVLCPacket::SetDataErrors(CommsErrorsCorruptData* p_data_errors)
{
	if(m_data_errors!=NULL)
		delete m_data_errors;
	m_data_errors=p_data_errors;
}
void CVLCPacket::SetTimingErrors(CommsErrorsTiming* p_timing_errors)
{
	if(m_timing_errors!=NULL)
		delete m_timing_errors;
	m_timing_errors=p_timing_errors;
}
void CVLCPacket::SetSizeErrors(CommsErrorsDataSize* p_size_errors)
{
	if(m_size_errors!=NULL)
		delete m_size_errors;
	m_size_errors=p_size_errors;
}
void CVLCPacket::ClearAllErrors()
{
	SetDataErrors(NULL);
	SetTimingErrors(NULL);
	SetSizeErrors(NULL);
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// Validation

ushort CVLCPacket::CalculateCrc()
{
	m_crc.Seed(0xFFFF);

	for(int i=0; i< OffsetCrc(); i++)
		m_crc.Byte(m_data[i]);

	ushort val = m_crc.Value();
	short pack_crc = GetShort( OffsetCrc() );
	return val;
}


bool CVLCPacket::VerifyCrc()
{
	bool res = ( CalculateCrc() == GetCrc() );
	if(!res)
		m_error_status |= ERR_CRC;
	return res;
}



ushort CVLCPacket::GetCrc()
{
	ushort val = GetShort( OffsetCrc() );
	return val;
}

void CVLCPacket::SetCrc(ushort p_val)
{
	SetShort( p_val, OffsetCrc() ); 
}

void CVLCPacket::SetCrc()
{
	SetShort( CalculateCrc(), OffsetCrc());	
}


ushort CVLCPacket::CalculateValidationCode(ushort p_seed)
{
	uint seed = (ushort)p_seed;
	int i=1; 
	while(i < m_length-12) // go thru to the end of the data section
	{
		seed<<=16;
		ushort tmp=0;
		tmp = m_data[i++]<<8;
		tmp |= m_data[i++];
		seed |= tmp;
		seed %=65521;
	}
	return (ushort)seed;
}


bool CVLCPacket::CheckValidationCode()
{
	ushort seed = GetShort( OffsetSeed() );
	ushort code = CalculateValidationCode(seed);
	ushort code_pkt = GetShort( OffsetCode() );
	bool res = (code == code_pkt);
	if(!res)
		m_error_status |= ERR_CODE;
	return res;
}


void CVLCPacket::SetValidationCode()
{
	ushort rand_seed = (ushort)(g_random->Number(65521));
	ushort code = CalculateValidationCode(rand_seed);
	SetShort(rand_seed, OffsetSeed() );
	SetShort(code, OffsetCode() );
}

void CVLCPacket::SetValidationCode(ushort p_seed)
{
	ushort code = CalculateValidationCode(p_seed);
	SetShort(p_seed, OffsetSeed() );
	SetShort(code, OffsetCode() );
}


bool CVLCPacket::CheckDateTime()
{
	ushort date_pkt = GetShort( OffsetDate() );
	ushort time_pkt = GetShort( OffsetTime() );

	CVLCDateTime current_date_time;	//is initialised to universal date/time
	CVLCDateTime packet_date_time;

	packet_date_time.Set(date_pkt, time_pkt);

	int diff = current_date_time.Difference(packet_date_time);

	bool res = ((diff>= -10)&&(diff <= 10));

	if(!res)
		m_error_status |= ERR_TIMESTAMP;

	return res;
}

void CVLCPacket::SetValidationDateTime()
{
	CVLCDateTime current_date_time; //universal
	SetShort( current_date_time.GetJulianDate(), OffsetDate() );
	SetShort( current_date_time.GetTimeInSeconds(), OffsetTime() );
}

void CVLCPacket::SetValidationDateTime( CVLCDateTime& p_date_time)
{
	SetShort( p_date_time.GetJulianDate(), OffsetDate() );
	SetShort( p_date_time.GetTimeInSeconds(), OffsetTime() );
}


bool CVLCPacket::ValidatePacket()
{
	//Note: error statuses ERR_COMMAND, ERR_LENGTH, ERR_TRAILER get set by g_receive_mgr
	//The remaining get checked here.

	bool result=true;

	// check length first (should have been checked in receive code - but double check!

	if(m_length < 8)
		return false;
	
	int expected_length=0;

	if( m_data[3]&(0x80) ) // MS Bit of command is 1 => no data
		expected_length = CVLCPacket::LENGTH_OVERHEAD;
	else
	{
		byte multiples_of_eight = m_data[3]&(0x1F);
		int data_length = (multiples_of_eight+1)*8;
		expected_length = data_length + CVLCPacket::LENGTH_OVERHEAD + CVLCPacket::LENGTH_VALIDATION; 

		if(data_length > CVLCPacket::LENGTH_DATA_MAX )
		{
			m_error_status |= CVLCPacket::ERR_LENGTH_MAX;
			result=false;
		}
	}

	
	if( m_length != expected_length)
	{
		m_error_status |= CVLCPacket::ERR_LENGTH;
		result=false;
	}

	// check header and footer.
	if( m_data[0] != HEADER)
	{
		m_error_status |= CVLCPacket::ERR_HEADER;
		result=false;
	}

	if( m_data[m_length-1] != TRAILER)
	{
		m_error_status |= CVLCPacket::ERR_TRAILER;
		result=false;
	}

	// check CRC
	if( !VerifyCrc() )
	{
		m_error_status |= CVLCPacket::ERR_CRC;
		result=false;
	}

	//check validation segment if data packet	
	if( (m_data[3]&(0x80)) ==0 )
	{
		if( !CheckValidationCode() )
		{
			m_error_status |= CVLCPacket::ERR_CODE;
			result=false;
		}

		if( !CheckDateTime() )
		{
			m_error_status |= CVLCPacket::ERR_TIMESTAMP;
			result=false;
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////



byte const CVLCPacket::m_fields[VLC_FIELDS_TOTAL][3] =
{
	// applies to data section - has to be defined in same order as in vlc_packet.h
	// start, length, format		// FIELD NAME	
	///////////////////////////////////////////////////////
	// Control
	{1,		1,		BINARY		},	// CRTL_FORMAT_ID
	{2,		1,		HEX			},	// CRTL_RESERVED2
	{3,		1,		BINARY		},	// CRTL_VERS
	{4,		1,		HEX			},	// CRTL_RESERVED4
	{5,		1,		CODE		},	// CTRL_TERMINAL_MODE
	{6,		1,		HEX			},	// CRTL_RESERVED6
	{7,		1,		BINARY		},	// CTRL_DALY_POLL_TIMEOUT
	{8,		1,		FLAGS		},	// CTRL_HOLIDAY_FLAGS
	{9,		2,		HEX			},	// CTRL_MEM_SIG_DIVISOR
	{11,	1,		HEX			},	// CRTL_RESERVED11
	{12,	3,		BINARY		},	// CTRL_FROM
	{15,	8,		FLAGS		},	// CTRL_GAME_KEY_ENABLE_FLAGS
	{23,	1,		HEX			},	// CRTL_RESERVED23
	{27,	0,		VARIABLE	},	// CTRL_VARIABLE_BANNER_DATA

	///////////////////////////////////////////////////////
	// Configuration overhead
	{1,		1,		BINARY		},	// CFG_OH_FORMAT_ID
	{2,		1,		HEX			},	// CFG_OH_RESERVED2,
	{3,		1,		BINARY		},	// CFG_OH_VERS
	{4,		1,		BINARY		},	// CFG_OH_BYTES_SYSTEM
	{5,		1,		HEX			},	// CFG_OH_RESERVED5
	{6,		1,		BINARY		},	// CFG_OH_COUNT_GAME
	{7,		1,		BINARY		},	// CFG_OH_BYTES_GAME_KEY_X
	{8,		1,		HEX			},	// CFG_OH_RESERVEDX

	///////////////////////////////////////////////////////
	// Configuration system
	{1,		20,		ASCII		},	// CFG_SYS_LOCATION_NAME
	{21,	40,		ASCII		},	// CFG_SYS_LOCATION_ADDRESS
	{61,	8,		ASCII		},	// CFG_SYS_PERMIT_NUMBER
	{69,	6,		HEX			},	// CFG_SYS_RESERVED69
	{75,	2,		TIME_R		},	// CFG_SYS_NON_HOL_TERM_ENABLE_TIME
	{77,	2,		BINARY		},	// CFG_SYS_NON_HOL_TERM_ENABLE_DURATION
	{79,	2,		TIME_R		},	// CFG_SYS_HOL_TERM_ENABLE_TIME
	{81,	2,		BINARY		},	// CFG_SYS_HOL_TERM_ENABLE_DURATION
	{83,	2,		BINARY		},	// CFG_SYS_HOPPER_PAY_MAX
	{85,	2,		BINARY		},	// CFG_SYS_SUBSTANTIAL_CASHOUT
	{87,	2,		BINARY		},	// CFG_SYS_SUBSTANTIAL_WIN
	{89,	2,		TIME_R		},	// CFG_SYS_DAY_END_SNAPSHOT_TIME
	{91,	1,		CODE		},	// CFG_SYS_WEEK_END_SNAPSHOT_DAY
	{92,	1,		CODE		},	// CFG_SYS_TIME_ZONE_CODE
	{93,	2,		DATE_U		},	// CFG_SYS_DATE_FOR_DLS_CHANGE
	{95,	2,		TIME_U		},	// CFG_SYS_TIME_FOR_DLS_CHANGE
	{97,	2,		BINARY		},	// CFG_SYS_SHARE_PERCENTAGE
	{99,	1,		FLAGS		},	// CFG_SYS_TRANSACTION_ENABLE_FLAGS
	{100,	3,		BINARY		},	// CFG_SYS_BASE
	{103,	1,		HEX			},	// CFG_SYS_RESERVED103	
	{104,	1,		BINARY		},	// CFG_SYS_STATUS_POLL_TIMEOUT
	{105,	4,		FLAGS		},	// CFG_SYS_EXCEPTIONS_ENABLE
	{109,	1,		FLAGS		},	// CFG_SYS_ENCRYPTION_CONTROL
	{117,	8,		HEX			},	// CFG_SYS_ENCRYPTION_KEY_1
	{125,	8,		HEX			},	// CFG_SYS_ENCRYPTION_KEY_2
	{133,	8,		HEX			},	// CFG_SYS_ENCRYPTION_KEY_3

	///////////////////////////
	// Configuration game	
	{1,		4,		HEX			},	// CFG_GAME_RESERVED1
	{5,		1,		BINARY		},	// CFG_GAME_TYPE
	{6,		15,		ASCII		},	// CFG_GAME_NAME
	{21,	2,		TIME_R		},	// CFG_GAME_NON_HOL_GAME_DISABLE_TIME
	{23,	2,		BINARY		},	// CFG_GAME_NON_HOL_GAME_DISABLE_DURATION
	{25,	2,		TIME_R		},	// CFG_GAME_HOL_GAME_DISABLE_TIME
	{27,	2,		BINARY		},	// CFG_GAME_HOL_GAME_DISABLE_DURATION
	{29,	1,		BINARY		},	// CFG_GAME_ID
	{30,	1,		FLAGS		},	// CFG_GAME_ENABLE_CONTROL_FLAGS
	{31,	2,		BINARY		},	// CFG_GAME_CREDIT_SIZE
	{33,	2,		BINARY		},	// CFG_GAME_MAX_BET
	{35,	4,		BINARY		},	// CFG_GAME_MAX_AWARD
	{39,	0,		VARIABLE	},	// CFG_GAME_ADDITIONAL_PARAMMETERS

	///////////////////////////
	// monitor
	{1,		1,		BINARY		},	//MON_FORMAT_ID,
	{2,		1,		HEX			},	//MON_ERSERVED2
	{3,		2,		CODE		},	//MON_SYS_MODE,
	{5,		1,		CODE		},	//MON_ACTIVE_GAME_ID,
	{6,		3,		BINARY		},	//MON_BASE,
	{9,		2,		FLAGS		},	//MON_STATUS_FLAGS,
	{11,	2,		FLAGS		},	//MON_SUBASSEMBLY_FAILURE_FLAGS,
	{13,	2,		FLAGS		},	//MON_OPERATOR_FLAGS,
	{15,	2,		BINARY		},	//MON_PAPER_LEVEL,
	{17,	1,		BINARY		},	//MON_SYSTEM_FIRMWARE_VERS,
	{18,	1,		BINARY		},	//MON_GAME_FIRMWARE_VERS,
	{19,	1,		BINARY		},	//MON_CONTROL_VERS,
	{20,	1,		BINARY		},	//MON_CONFIG_VERS,
	{21,	1,		BINARY		},	//MON_BANNER_VERS,
	{22,	1,		BINARY		},	//MON_ALT_BANNER_VERS,
	{23,	1,		BINARY		},	//MON_REPORT_VERS,
	{24,	3,		HEX			},	//MON_ERSERVED24
	{27,	2,		HEX			},	//MON_MEM_SIG_VALUE,
	{29,	4,		HEX			},	//MON_ERSERVED29	
	{33,	2,		DATE_U		},	//MON_DATE_FOR_ACCOUNTING_DATE,
	{35,	2,		TIME_U		},	//MON_TIME_FOR_ACCOUNTING_DATA,
	{37,	2,		DATE_U		},	//MON_MASTER_RESET_DATE,
	{39,	2,		TIME_U		},	//MON_MASTER_RESET_TIME,
	{41,	4,		BINARY		},	//MON_BILLS_IN,
	{45,	4,		BINARY		},	//MON_COIN_IN_DROP_BOX,
	{49,	4,		BINARY		},	//MON_COIN_IN_HOPPER,
	{53,	4,		BINARY		},	//MON_PLAYER_DEBIT_CARD_IN,
	{57,	4,		BINARY		},	//MON_HOPPER_FILLS,
	{61,	4,		BINARY		},	//MON_PRODUCT_IN,
	{65,	4,		BINARY		},	//MON_BILLS_COLLECTED,
	{69,	4,		BINARY		},	//MON_DROP_BOX_COLLECTED,
	{73,	4,		BINARY		},	//MON_COIN_OUT_HOPPER,
	{77,	4,		BINARY		},	//MON_PLAYER_DEBIT_OUT,
	{81,	4,		BINARY		},	//MON_CASH_TICKETS_MANUAL_PAYS,
	{85,	4,		BINARY		},	//MON_PRODUCT_OUT,
	{89,	4,		BINARY		},	//MON_TOTAL_PLAYED,
	{93,	4,		BINARY		},	//MON_TOTAL_WON,
	
	///////////////////////////
	// monitor game
	{1,		1,		BINARY		},	//MON_GAME_ID,
	{2,		1,		FLAGS		},	//MON_GAME_STATUS,
	{3,		3,		BINARY		},	//MON_GAME_GAMES_PLAYED,
	{6,		3,		BINARY		},	//MON_GAME_GAMES_WON,
	{9,		4,		BINARY		},	//MON_GAME_CENTS_PLAYED,
	{13,	4,		BINARY		},	//MON_GAME_CENTS_WON,

	/////////////////////////////////
	// Date/Time Mem Sig
	{1,		1,		BINARY		}, //DT_FORMAT_ID,
	{2,		7,		HEX			}, //DT_RESERVED2
	{9,		2,		DATE_U		}, //DT_DATE,
	{11,	2,		TIME_U		}, //DT_TIME,
	{13,	2,		HEX			}, //DT_SIG_VALUE,
	{15,	2,		HEX			}, //DT_SIG_DIVISOR,

	//////////////////////////////
	// event - first record
	{1,		1,		BINARY		},	//EVENT_FORMAT_ID
	{2,		7,		HEX			},	//EVENT_RECORD

	//////////////////////////////
	{1,		1,		HEX			},	//EVENT_TYPE

	//////////////////////////////
	// event - Cashout
	{2,		1,		HEX			},	//EV_CASHOUT_CODE
	{3,		4,		BINARY		},	//EV_CASHOUT_AMOUNT
	{7,		2,		TIME_U		},	//EV_CASHOUT_TIME

	///////////////////////////
	//event cashout ctn
	{2,		3,		BINARY		},	//EV_CASHOUT_CTN_VENUE,
	{5,		2,		BINARY		},	//EV_CASHOUT_CTN_NUMBER,
	{7,		2,		BINARY		},	//EV_CASHOUT_CTN_SECURITY,

	///////////////////////////
	//event gamewin
	{2,		3,		BINARY		},	//EV_GAMEWIN_AMOUNT,
	{5,		1,		BINARY		},	//EV_GAMEWIN_GAME_ID,
	{6,		1,		BINARY		},	//EV_GAMEWIN_CATEGORY,
	{7,		2,		TIME_U		},	//EV_GAMEWIN_TIME,

	///////////////////////////
	//event gamewin ctn
	{2,		3,		HEX			},	//EV_GAMEWIN_CTN_RESERVED2,
	{5,		4,		BINARY		},	//EV_GAMEWIN_CTN_AMOUNT,	

	///////////////////////////
	//event exception
	{2,		1,		HEX			},	//EV_EXCEPTION_CODE,
	{3,		4,		HEX			},	//EV_EXCEPTION_DATA,
	{7,		2,		TIME_U		},	//EV_EXCEPTION_TIME,
	
	///////////////////////////
	//event credit snapshot
	{2,		1,		HEX			},	//EV_CREDIT_SN_SUBTYPE,
	{3,		4,		BINARY		},	//EV_CREDIT_SN_BALANCE,
	{7,		2,		TIME_U		},	//EV_CREDIT_SN_TIME,

	///////////////////////////
	//event message to host
	{2,		1,		HEX			},	//EV_MSG_HOST_SUBTYPE
	{3,		6,		ASCII			},	//EV_MSG_HOST_TEXT,
	
	///////////////////////////
	//event status poll timeout
	{2,		1,		HEX			},	//EV_SPT_SUBTYPE,
	{3,		3,		HEX			},	//EV_SPT_RESERVED3,
	{6,		1,		HEX			},	//EV_SPT_CHANGE_OF_STATE,
	{7,		2,		TIME_U		},	//EV_SPT_TIME,

	///////////////////////////
	//event FW rev change
	{2,		1,		HEX			},	//EV_FW_SUBTYPE,
	{3,		1,		ASCII			},	//EV_FW_NEW_SYS,
	{4,		1,		ASCII			},	//EV_FW_OLD_SYS,
	{5,		1,		ASCII			},	//EV_FW_NEW_GAME,
	{6,		1,		ASCII			},	//EV_FW_OLD_GAME,
	{7,		2,		TIME_U		},	//EV_FW_TIME,

	///////////////////////////
	//event encryption key change
	{2,		1,		HEX			},	//EV_ENCRYPT_SYBTYPE,
	{3,		1,		HEX			},	//EV_ENCRYPT_RESERVED3,
	{4,		1,		HEX			},	//EV_ENCRYPT_CAUSE,
	{5,		2,		HEX			},	//EV_ENCRYPT_RESERVED5,
	{7,		2,		TIME_U		},	//EV_ENCRYPT_TIME,

	///////////////////////////
	//event time date	
	{2,		1,		HEX			},	//EV_TD_REASON,
	{3,		2,		TIME_U		},	//EV_TD_OLD_TIME,
	{5,		2,		DATE_U		},	//EV_TD_NEW_DATE,
	{7,		2,		TIME_U		},	//EV_TD_NEW_TIME,

	///////////////////////////
	//event record number
	{2,		3,		BINARY		},	//EV_RECORD_NUMBER,
	{5,		2,		DATE_U		},	//EV_RECORD_DATE,
	{7,		2,		TIME_U		},	//EV_RECORD_TIME,
	
	/////////////////////////////
	//Transaction Request
	{1,		1,		BINARY		}, //TRANS_RQ_FORMAT_ID,
	{2,		1,		HEX			},	//TRANS_RQ_RESERVED2,
	{3,		1,		BINARY		},	//TRANS_RQ_TRANS_NUMBER,
	{4,		1,		HEX,			},	//TRANS_RQ_CASHOUT_TYPE,
	{5,		4,		BINARY		},	//TRANS_RQ_CASHOUT_AMOUNT,

	/////////////////////////////
	//Transaction Reply
	{1,		1,		BINARY,			},	//TRANS_REPLY_FORMAT_ID,
	{2,		1,		HEX,				},	//TRANS_REPLY_RESERVED2,
	{3,		1,		BINARY,			},	//TRANS_REPLY_TRANS_NUMBER,
	{4,		3,		BINARY,			},	//TRANS_REPLY_VENUE_ID,
	{7,		4,		BINARY,			},	//TRANS_REPLY_CASHOUT_AMOUNT,
	{11,		2,		DATE_U,			},	//TRANS_REPLY_DATE_OF_ISSUE,
	{13,		2,		TIME_U,			},	//TRANS_REPLY_TIME_OF_ISSUE,
	{15,		2,		BINARY,			},	//TRANS_REPLY_CASHOUT_NUMBER,
	{17,		2,		BINARY,			},	//TRANS_REPLY_SECURITY_NUMBER,
	{19,		6,		HEX,				}	//TRANS_REPLY_RESERVED19,

};



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


