#ifndef __VLC_PACKET_H
#define __VLC_PACKET_H

#include "VLCSim.h"
#include "stdtype.h"
#include "date_time.h"
#include "sa_comms_crc.h"
#include "dlg_comms_errors.h"


////////////////////////////////////////////////////////////////////
class CVLCPacket
{

friend class CVlcReceive;
friend class CVlcTransmit;

private:

	enum E_DATA_TYPE
	{
		BINARY=0,
		FLAGS,
		ASCII,
		CODE,
		VARIABLE,
		DATE_U,
		DATE_R,
		TIME_U,
		TIME_R,
		HEX,
		NUM_DATA_TYPES
	};

	struct VLCPAcketField
	{
		byte m_start;
		byte m_size;
		byte m_type;
	};

public:
		//here's all the fields for the different packet types
	enum
	{
		///////////////////////////
		// Control
		CRTL_FORMAT_ID=0,
		CRTL_RESERVED2,
		CRTL_VERS,
		CRTL_RESERVED4,
		CTRL_TERMINAL_MODE,
		CRTL_RESERVED6,
		CTRL_DALY_POLL_TIMEOUT,	
		CTRL_HOLIDAY_FLAGS,
		CTRL_MEM_SIG_DIVISOR,
		CTRL_RESERVED11,
		CTRL_FROM,
		CTRL_GAME_KEY_ENABLE_FLAGS,
		CTRL_RESERVED23,
		CTRL_VARIABLE_BANNER_DATA,
		///////////////////////////
		// Configuration overhead
		CFG_OH_FORMAT_ID,
		CFG_OH_RESERVED2,
		CFG_OH_VERS,
		CFG_OH_BYTES_SYSTEM,
		CFG_OH_RESERVED5,
		CFG_OH_COUNT_GAME,
		CFG_OH_BYTES_GAME_KEY_X,
		CFG_OH_RESERVEDX,
		///////////////////////////
		// Configuration system
		CFG_SYS_LOCATION_NAME,
		CFG_SYS_LOCATION_ADDRESS,
		CFG_SYS_PERMIT_NUMBER,
		CFG_SYS_RESERVED69,
		CFG_SYS_NON_HOL_TERM_ENABLE_TIME,
		CFG_SYS_NON_HOL_TERM_ENABLE_DURATION,
		CFG_SYS_HOL_TERM_ENABLE_TIME,
		CFG_SYS_HOL_TERM_ENABLE_DURATION,
		CFG_SYS_HOPPER_PAY_MAX,
		CFG_SYS_SUBSTANTIAL_CASHOUT,
		CFG_SYS_SUBSTANTIAL_WIN,
		CFG_SYS_DAY_END_SNAPSHOT_TIME,
		CFG_SYS_WEEK_END_SNAPSHOT_DAY,
		CFG_SYS_TIME_ZONE_CODE,
		CFG_SYS_DATE_FOR_DLS_CHANGE,
		CFG_SYS_TIME_FOR_DLS_CHANGE,
		CFG_SYS_SHARE_PERCENTAGE,
		CFG_SYS_TRANSACTION_ENABLE_FLAGS,
		CFG_SYS_BASE,	
		CFG_SYS_RESERVED103,
		CFG_SYS_STATUS_POLL_TIMEOUT,
		CFG_SYS_EXCEPTIONS_ENABLE,
		CFG_SYS_ENCRYPTION_CONTROL,
		CFG_SYS_ENCRYPTION_KEY_1,
		CFG_SYS_ENCRYPTION_KEY_2,
		CFG_SYS_ENCRYPTION_KEY_3,
		///////////////////////////
		// Configuration game	
		CFG_GAME_RESERVED1,
		CFG_GAME_TYPE,
		CFG_GAME_NAME,
		CFG_GAME_NON_HOL_GAME_DISABLE_TIME,
		CFG_GAME_NON_HOL_GAME_DISABLE_DURATION,
		CFG_GAME_HOL_GAME_DISABLE_TIME,
		CFG_GAME_HOL_GAME_DISABLE_DURATION,
		CFG_GAME_ID,
		CFG_GAME_ENABLE_CONTROL_FLAGS,
		CFG_GAME_CREDIT_SIZE,
		CFG_GAME_MAX_BET,
		CFG_GAME_MAX_AWARD,
		CFG_GAME_ADDITIONAL_PARAMMETERS,
		///////////////////////////
		// monitor
		MON_FORMAT_ID,
		MON_ERSERVED2,
		MON_SYS_MODE,
		MON_ACTIVE_GAME_ID,
		MON_BASE,
		MON_STATUS_FLAGS,
		MON_SUBASSEMBLY_FAILURE_FLAGS,
		MON_OPERATOR_FLAGS,
		MON_PAPER_LEVEL,
		MON_SYSTEM_FIRMWARE_VERS,
		MON_GAME_FIRMWARE_VERS,
		MON_CONTROL_VERS,
		MON_CONFIG_VERS,
		MON_BANNER_VERS,
		MON_ALT_BANNER_VERS,
		MON_REPORT_VERS,
		MON_ERSERVED24,
		MON_MEM_SIG_VALUE,
		MON_ERSERVED29,
		MON_DATE_FOR_ACCOUNTING_DATE,
		MON_TIME_FOR_ACCOUNTING_DATA,
		MON_MASTER_RESET_DATE,
		MON_MASTER_RESET_TIME,
		MON_BILLS_IN,
		MON_COIN_IN_DROP_BOX,
		MON_COIN_IN_HOPPER,
		MON_PLAYER_DEBIT_CARD_IN,
		MON_HOPPER_FILLS,
		MON_PRODUCT_IN,
		MON_BILLS_COLLECTED,
		MON_DROP_BOX_COLLECTED,
		MON_COIN_OUT_HOPPER,
		MON_PLAYER_DEBIT_OUT,
		MON_CASH_TICKETS_MANUAL_PAYS,
		MON_PRODUCT_OUT,
		MON_TOTAL_PLAYED,
		MON_TOTAL_WON,
		///////////////////////////
		// monitor game
		MON_GAME_ID,
		MON_GAME_STATUS,
		MON_GAME_GAMES_PLAYED,
		MON_GAME_GAMES_WON,
		MON_GAME_CENTS_PLAYED,
		MON_GAME_CENTS_WON,
		
		///////////////////////////
		// Date/Time Mem Sig
		DT_FORMAT_ID,
		DT_RESERVED2,
		DT_DATE,
		DT_TIME,
		DT_SIG_VALUE,
		DT_SIG_DIVISOR,

		///////////////////////////
		// event
		EVENT_FORMAT_ID,
		EVENT_RECORD,
		///////////////////////////
		EVENT_TYPE, //always used for each event type
		///////////////////////////
		//event cashout
		EV_CASHOUT_CODE,
		EV_CASHOUT_AMOUNT,
		EV_CASHOUT_TIME,
		///////////////////////////
		//event cashout ctn
		EV_CASHOUT_CTN_VENUE,
		EV_CASHOUT_CTN_NUMBER,
		EV_CASHOUT_CTN_SECURITY,
		///////////////////////////
		//event gamewin
		EV_GAMEWIN_AMOUNT,
		EV_GAMEWIN_GAME_ID,
		EV_GAMEWIN_CATEGORY,
		EV_GAMEWIN_TIME,
		///////////////////////////
		//event gamewin ctn
		EV_GAMEWIN_CTN_RESERVED2,
		EV_GAMEWIN_CTN_AMOUNT,
		///////////////////////////
		//event exception
		EV_EXCEPTION_CODE,
		EV_EXCEPTION_DATA,
		EV_EXCEPTION_TIME,
		///////////////////////////
		//event credit snapshot
		EV_CREDIT_SN_SUBTYPE,
		EV_CREDIT_SN_BALANCE,
		EV_CREDIT_SN_TIME,
		///////////////////////////
		//event message to host
		EV_MSG_HOST_SUBTYPE,
		EV_MSG_HOST_TEXT,
		///////////////////////////
		//event status poll timeout
		EV_SPT_SUBTYPE,
		EV_SPT_RESERVED3,
		EV_SPT_CHANGE_OF_STATE,
		EV_SPT_TIME,
		///////////////////////////
		//event FW rev change
		EV_FW_SUBTYPE,
		EV_FW_NEW_SYS,
		EV_FW_OLD_SYS,
		EV_FW_NEW_GAME,
		EV_FW_OLD_GAME,
		EV_FW_TIME,
		///////////////////////////
		//event encryption key change
		EV_ENCRYPT_SYBTYPE,
		EV_ENCRYPT_RESERVED3,
		EV_ENCRYPT_CAUSE,
		EV_ENCRYPT_RESERVED5,
		EV_ENCRYPT_TIME,
		///////////////////////////
		//event time date	
		EV_TD_REASON,
		EV_TD_OLD_TIME,
		EV_TD_NEW_DATE,
		EV_TD_NEW_TIME,
		///////////////////////////
		//event record number
		EV_RECORD_NUMBER,
		EV_RECORD_DATE,
		EV_RECORD_TIME,

		///////////////////////////
		//Transaction Request
		TRANS_RQ_FORMAT_ID,
		TRANS_RQ_RESERVED2,
		TRANS_RQ_TRANS_NUMBER,
		TRANS_RQ_CASHOUT_TYPE,
		TRANS_RQ_CASHOUT_AMOUNT,

		///////////////////////////
		//Transaction Reply
		TRANS_REPLY_FORMAT_ID,
		TRANS_REPLY_RESERVED2,
		TRANS_REPLY_TRANS_NUMBER,
		TRANS_REPLY_VENUE_ID,
		TRANS_REPLY_CASHOUT_AMOUNT,
		TRANS_REPLY_DATE_OF_ISSUE,
		TRANS_REPLY_TIME_OF_ISSUE,
		TRANS_REPLY_CASHOUT_NUMBER,
		TRANS_REPLY_SECURITY_NUMBER,
		TRANS_REPLY_RESERVED19,
	
		VLC_FIELDS_TOTAL
	};

	enum
	{
		HEADER	=	0x05,
		TRAILER	=	0x0D,
	};

	enum
	{
		START_DATA						= 5,
		LENGTH_VALIDATION				= 8,
		LENGTH_OVERHEAD					= 8,
		LENGTH_DATA_MAX					= 256,
		LENGTH_PACKET_MAX				= LENGTH_DATA_MAX + LENGTH_OVERHEAD + LENGTH_VALIDATION,

		LENGTH_DATETIME_DATA			= 16,
		LENGTH_CONTROL_DATA				= 26,		//excludes banner data
		LENGTH_CONFIGURATION_OH			= 6,		//no game key data
		LENGTH_CONFIGURATION_SYSTEM		= 108,	//no encryption data
		LENGTH_CONFIGURATION_GAME		= 38,		//no additional parameters
		LENGTH_MONITOR_SYSTEM			= 96,
		LENGTH_MONITOR_GAME				= 16,
		LENGTH_EVENT_RECORD				= 8,
		LENGTH_TRANS_REQUEST_DATA		= 8,
		LENGTH_TRANS_REPLY_DATA			= 24,

		LENGTH_DATETIME					= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_DATETIME_DATA,
		LENGTH_CONTROL					= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_CONTROL_DATA, 
		LENGTH_CONFIGURATION			= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_CONFIGURATION_OH + LENGTH_CONFIGURATION_SYSTEM, //no game key data
		LENGTH_MONITOR					= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_MONITOR_SYSTEM,    //no game data
		LENGTH_EVENT					= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_EVENT_RECORD,		//single record only	
		LENGTH_TRANS_REQUEST			= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_TRANS_REQUEST_DATA,	
		LENGTH_TRANS_REPLY				= LENGTH_OVERHEAD + LENGTH_VALIDATION + LENGTH_TRANS_REPLY_DATA
	};

	//command - bits ccc
	enum
	{
		CMD_POLL				= 0xC000,
		CMD_STATUS_RESPONSE		= 0xC000,
		CMD_READ_REQ			= 0xA000,
		CMD_WRITE_REQ			= 0x2000,
		CMD_READ_REPLY			= 0x2000,
		CMD_TRANSACTION_REQ		= 0x0000,
		CMD_TRANSACTION_REPLY	= 0x0000,
		CMD_SITE_INFO_REQ		= 0xE000,
		CMD_SITE_INFO_REPLY		= 0x6000,
		CMD_ACK_NACK			= 0x8000,

		BITS_CCC				= 0xE000
	};

	//bits kkk (same as bits ccc except in a different position 
	enum
	{
		CMD_ACK_WRITE_REQ			= 0x0020, 
		CMD_ACK_READ_REPLY			= 0x0020,
		CMD_ACK_TRANSACTION_REQ		= 0x0000,
		CMD_ACK_TRANSACTION_REPLY	= 0x0000,
		CMD_ACK_SITE_INFO_REPLY		= 0x0060,

		BITS_KKK					= 0x00E0
	};

	//command flags
	enum
	{
		CMD_FLG_V	=	0x1000,	
		CMD_FLG_D	=	0x0800,	
		CMD_FLG_F	=	0x0400,	
		CMD_FLG_P	=	0x0200,	
		CMD_FLG_I	=	0x0100,	
		CMD_FLG_B	=	0x0004,	
		CMD_FLG_Q	=	0x0004,	
		CMD_FLG_O	=	0x0010	
	};

	//source bits ss
	enum
	{
		CMD_SRC_EGM							=	0x0000,
		CMD_SRC_AUDIT_DEVICE				=	0x0001,
		CMD_SRC_GMMS_HOST					=	0x0002,
		CMD_SRC_GMMS_SITE_CONTROLLER		=	0x0003,

		BITS_SS								=	0x0003
	};

	//data read type - bits rrrr
	enum
	{
		CMD_READ_CONTROL		=	0x0000,
		CMD_READ_MONITER_END	=	0x0010,		
		CMD_READ_CONFIGURE		=	0x0020,
		CMD_READ_STATS_END		=	0x0030,
		CMD_READ_BANNER			=	0x0040,
		CMD_READ_MONITER_CUR	=	0x0050,
		CMD_READ_STATS_CUR		=	0x0070,
		CMD_READ_EVENT_LOG		=	0x0090,
		CMD_READ_DATETIME_MS	=	0x00C0,	
		CMD_READ_CONTINUATION	=	0x00F0,

		BITS_RRRR				=	0x00F0
	};

	//data write type - bits www
	enum
	{
		CMD_WRITE_CONTROL		=	0x0000,	
		CMD_WRITE_CONFIGURE		=	0x0020,	
		CMD_WRITE_BANNER		=	0x0040,
		CMD_WRITE_DATETIME_MS	=	0x00C0,
		CMD_WRITE_CONTINUATION	=	0x00E0,

		BITS_WWW				=	0x00E0
	};

	//ack-nack type - bits aaaaa
	enum
	{
		CMD_ACK						=	0x0000,	
		CMD_NACK_CRC				=	0x0200,
		CMD_NACK_TIMESTAMP			=	0x0300,
		CMD_NACK_COMMAND			=	0x1200,
		CMD_NACK_CODE				=	0x1400,
		CMD_NACK_TRANSACTION		=	0x1500,	
		CMD_NACK_CONFIGURATION		=	0x1600,
		CMD_NACK_NON_ZERO_BALANCE	=	0x1700,

		BITS_AAAAA					=	0x1F00
	};

	//Transactions
	enum
	{
		CMD_TRANS_CASHOUT			=	0x0000,
		BITS_TTTT					=	0x00F0
	};

	//N bits
	enum
	{
		BITS_NNNNN					=	0x1F00	
	};

	enum EDataType
	{
		DATA_TYPE_CONTROL = 0,	
		DATA_TYPE_CONFIGURATION,
		DATA_TYPE_MONITOR,
		DATA_TYPE_STATISTICS,
		DATA_TYPE_DATETIME_MS,
		DATA_TYPE_EVENT_LOG,
		DATA_TYPE_CONTINUATION,
		DATA_TYPE_TRANSACTION_REQUEST,
		DATA_TYPE_TRANSACTION_REPLY,
		DATA_TYPE_NONE
	};

	//comms error status - for packets received
	enum 
	{
		ERR_NONE					=	0x00,
		ERR_CRC						=	0x01,
		ERR_TIMESTAMP				=	0x02,	//out of window
		ERR_COMMAND					=	0x04,	//Unknown command 
		ERR_CODE					=	0x08,	
		ERR_LENGTH					=	0x10,	//n bits do not match length
		ERR_LENGTH_MAX				=	0x20,	//length exceeds max
		ERR_TRAILER					=	0x40,	//Trailer is not 0x0D
		ERR_HEADER					=  0x80  //Header no 0x05
	};
	
	//The designated response to data packets recieved or sent
	enum EResponse
	{
		RESP_AUTO = 0,	//Auto means that the sim code decides what the response will be
		RESP_ACK,			
		RESP_NACK_CRC,
		RESP_NACK_TIMESTAMP,
		RESP_NACK_COMMAND,
		RESP_NACK_CODE,
		RESP_NACK_TRANSACTION,
		RESP_NACK_CONFIGURATION,
		RESP_NACK_NON_ZERO_BALANCE,
		RESP_NONE
	};

	enum ETransmitDirection	//Whether the packet was sent or received
	{
		SEND,
		RECEIVE
	};

/////////////////////////////////////////////////////////////////
//Construction
public:
	CVLCPacket(int p_length);
	CVLCPacket(void);
	CVLCPacket(const CVLCPacket& p_copy);
	CVLCPacket& operator = (const CVLCPacket& p_copy);
	virtual ~CVLCPacket();
private:
	void CopyData(const CVLCPacket& p_copy);
	void InitialiseStatusInfo();

/////////////////////////////////////////////////////////////////
// Misc
public:
	const CByteArray& GetPacketData() const {return m_data;}
	int   GetLength() const  { return m_length; }
	void  SetLength(int p_length);
	void  SetDataLength(int p_length);
	void  PadZero();
	void  RemovePadZero();
	void  CopyPacketData(const CVLCPacket& p_copy);
	void  SetErrorStatus(byte p_status);
	byte  GetErrorStatus() const;	
	CString GetAsString() const;
	typedef std::list<CString*> StringList; 
	typedef std::list<CString*>::iterator StringListItr; 
	void  GetAsStringList(StringList& p_string_list, bool p_show_errors, bool p_show_descr, bool p_show_timestamp) const;
	static EResponse GetResponseFromAckCode(ushort p_ack_code);
	static ushort GetAckCodeFromResponse(EResponse p_response);
	static void GetResponseAsString(CString& p_str, EResponse p_response);

private:
	CString* GetErrorsAsString() const;
	CString* GetDescriptionAsString() const;
//////////////////////////////////////////////////////////////////
// Set and Get packet data - maybe should be in protected section
public:
	void  SetByte( byte p_val, int p_pos);
	byte  GetByte( int p_pos ) const ;

	void  SetShort(ushort p_val, int p_pos);
	ushort GetShort(int p_pos) const;

	void  SetInt(uint p_val, int p_pos);
	uint   GetInt(int p_pos) const;

	void  SetInt3(uint p_val, int p_pos);
	uint   GetInt3(int p_pos) const;

	void  SetIntX(uint p_val, int p_pos, int p_size);
	uint   GetIntX(int p_pos, int p_size) const;

	bool  SetData(CByteArray& p_data, int p_pos);
	bool  GetData(CByteArray& p_data, int p_pos, int p_length) const;

	bool InsertData(CByteArray& p_data, int p_pos);

	bool  SetDataRv(CByteArray& p_data, int p_pos, int p_length);
	bool  GetDataRv(CByteArray& p_data, int p_pos, int p_length) const;
////////////////////////////////////////////////////////////////////////
// Get set formatted data
public:
	bool SetDateString(CString& p_string, int p_pos);
	bool GetDateString(CString& p_string, int p_pos) const;

	bool SetTimeString(CString& p_string, int p_pos);
	bool GetTimeString(CString& p_string, int p_pos) const;

	bool SetDecString(CString& p_string, int p_pos, int p_length);
	bool GetDecString(CString& p_string, int p_pos, int p_length) const;

	bool SetHexString(CString& p_string, int p_pos);
   bool GetHexString(CString& p_string, int p_pos, int p_length) const;

	//bool SetHexRvString(CString& p_string, int p_start);
    //bool GetHexRvString(CString& p_string, int p_start, int p_length) const;
    
	bool SetAsciiString(CString& p_string, int p_pos);
	bool GetAsciiString(CString& p_string, int p_pos, int p_length) const;

////////////////////////////////////////////////////////////////////////
// Set and Get packet fields
// p_offset refers to the start of a section for packets that have variable number of sections - 
// i.e. config and monitor.  
protected:
	bool ValidateDataField(int p_field, int p_offset=0) const;
public:

	void GetDataField(CString& p_string, int p_field, int p_offset=0 )const;
	void SetDataField(CString& p_string, int p_field, int p_offset=0 );
	void GetDataFieldFormatted(CString& p_string, int p_field, int p_offset=0 )const ;
	void SetDataFieldFormatted(CString& p_string, int p_field, int p_offset=0 );
	
	uint GetDataFieldInt(int p_field, int p_offset=0 ) const;
	void SetDataFieldInt(uint p_val, int p_field, int p_offset=0 );
	void GetDataFieldHex(CByteArray& p_data, int p_field, int p_offset=0 ) const;
	void SetDataFieldHex(CByteArray& p_data, int p_field, int p_offset=0);	
////////////////////////////////////////////////////////////////////////
// Set and Get and check overhead data
public:
	//header
	void   SetHeader(byte p_header) { SetByte(p_header,0); }
	byte   GetHeader() { return GetByte(0); }

	//terminal id
	void   GetTerminalId(CString& p_out) const { GetHexString(p_out,1,2); }
	ushort GetTerminalId() const { return GetShort(1); }
	void   SetTerminalId(CString& p_id) { SetHexString(p_id,1); }
	void   SetTerminalId(ushort p_id) { SetShort(p_id,1); }

	//command
	ushort GetCommand() const { return GetShort(3); }
	void   SetCommand(ushort p_command) { SetShort(p_command,3); }
	ushort GetCommandType()   const{ return ( GetShort(3)&((ushort)(BITS_CCC)) ); }
	ushort GetDataReadType()  const{ return ( GetShort(3)&((ushort)(BITS_RRRR)) ); }
	ushort GetDataWriteType() const{ return ( GetShort(3)&((ushort)(BITS_WWW)) ); }
	ushort GetAckNackCode()   const{ return ( GetShort(3)&((ushort)(BITS_AAAAA)) ); }
	ushort GetAckNackCommandType()   const{ return ( GetShort(3)&((ushort)(BITS_KKK)) ); }
	ushort GetSourceType()    const{ return ( GetShort(3)&((ushort)(BITS_SS)) ); }
	ushort GetTransactionType() const { return ( GetShort(3)&((ushort)(BITS_TTTT)) ); }
	bool   IsCommandFlagSet(ushort p_flag) const;
	void   SetCommandFlag(ushort p_flag, bool p_status);
	bool   IsDataPacket() const;
	int    GetDataLength() const;
	EDataType   GetDataType() const ;

	ushort CalculateNBits() const;	// Calculate bits NNNNN for the command based on the length of the data.
	ushort GetNBits() const{ return ( GetShort(3)&((ushort)(BITS_NNNNN)) ); }
	void   SetNBits(ushort p_command) {ushort command = GetShort(3); command &= (~(ushort)(BITS_NNNNN));  command|=p_command; SetShort(command,3);  }
	void   SetNBits()                 {ushort command = GetShort(3); command &= (~(ushort)(BITS_NNNNN));  command|=CalculateNBits(); SetShort(command,3);  }

	//todo - these can be GetCommandBits(BITS), SetCommandBits(BITS, value)
	void   SetCommandType(ushort p_command)   {ushort command = GetShort(3); command &= (~(ushort)(BITS_CCC));  command|=p_command; SetShort(command,3);  }
	void   SetDataReadType(ushort p_command)  {ushort command = GetShort(3); command &= (~(ushort)(BITS_RRRR)); command|=p_command; SetShort(command,3);  }
	void   SetDataWriteType(ushort p_command) {ushort command = GetShort(3); command &= (~(ushort)(BITS_WWW));  command|=p_command; SetShort(command,3);  }
	void   SetAckNackType(ushort p_command)   {ushort command = GetShort(3); command &= (~(ushort)(BITS_AAAAA));command|=p_command; SetShort(command,3);  }
	void   SetAckNackCommandType(ushort p_command)   {ushort command = GetShort(3); command &= (~(ushort)(BITS_KKK));command|=p_command; SetShort(command,3);  }
	void   SetSourceType(ushort p_command)    {ushort command = GetShort(3); command &= (~(ushort)(BITS_SS));  command|=p_command; SetShort(command,3);  }
	void   SetTransactionType(uchar p_command) {ushort command = GetShort(3); command &= (~(ushort)(BITS_TTTT)); command|=p_command; SetShort(command,3);  }	

	//validation date
	void   SetValidationDate(ushort p_date) { SetShort(p_date, OffsetDate() ); }
	ushort GetValidationDate() const { return GetShort(OffsetDate() ); }

	//validation time
	void   SetValidationTime(ushort p_time) { SetShort(p_time, OffsetTime() ); }
	ushort GetValidationTime() const { return GetShort(OffsetTime() ); }

	//validation code
	void   SetValidationCodeVal(ushort p_val) { SetShort(p_val, OffsetCode() ); }
	ushort GetValidationCodeVal() const { return GetShort(OffsetCode() ); }

	//trailer
	void   SetTrailer(byte p_header) { SetByte(p_header,m_length-1); }
	byte   GetTrailer() { return GetByte(m_length-1); }

///////////////////////////////////////////////////////////////////////
//Set and Get status info
public:
	CommsErrorsCorruptData* GetDataErrors() const {return m_data_errors;}
	CommsErrorsTiming* GetTimingErrors() const {return m_timing_errors;}
	CommsErrorsDataSize* GetSizeErrors() const {return m_size_errors;}
	void SetDataErrors(CommsErrorsCorruptData* p_data_errors);
	void SetTimingErrors(CommsErrorsTiming* p_timing_errors);
	void SetSizeErrors(CommsErrorsDataSize* p_size_errors);
	void ClearAllErrors();

	EResponse GetResponse() const {return m_response;}
	void SetResponse(EResponse p_response) {m_response = p_response;}
	ETransmitDirection GetTransmitDirection() const {return m_transmit_direction;}
	void SetTransmitDirection(ETransmitDirection p_transmit_direction) {m_transmit_direction = p_transmit_direction;}
///////////////////////////////////////////////////////////////////////
//Validation / Verification
	ushort CalculateCrc();
	bool VerifyCrc();
	ushort GetCrc();
	void SetCrc(ushort p_val);
	void SetCrc();
	ushort CalculateValidationCode(ushort p_seed);
	bool CheckValidationCode();
	void SetValidationCode();
	void SetValidationCode(ushort p_seed);
	bool CheckDateTime();
	void SetValidationDateTime();
	void SetValidationDateTime( CVLCDateTime& p_date_time);
	bool ValidatePacket();
	int OffsetValidation() const {return (m_length-11); }
/////////////////////////////////////////////////////////////////////////

protected:

	int OffsetCrc() const {return (m_length-3); }
	int OffsetDate() const {return (m_length-11); }
	int OffsetTime() const {return (m_length-9); }
	int OffsetSeed() const {return (m_length-7); }
	int OffsetCode() const {return (m_length-5); }
	int OffsetID() const {return 1;}

//Data

	CByteArray m_data;
	int m_length;	
	int m_padding_length;
	byte m_error_status;			//Error status - for packets received

	CTimeStamp	m_timestamp_start;	//timstamp for start packet - send only
	CTimeStamp	m_timestamp_end;		//timestamp for end packet - send or receive

	//If this packet is for sending, this data specifies errors to introduce
	CommsErrorsCorruptData* m_data_errors;	
	CommsErrorsTiming* m_timing_errors;
	CommsErrorsDataSize* m_size_errors;

	//Specifies the required response if this is a received data packet
	//The response from the GMMS if this is a sent data packet
	EResponse m_response;

	//If its sent or received
	ETransmitDirection m_transmit_direction;
	
	static SaCrc m_crc;
	static const byte m_fields[VLC_FIELDS_TOTAL][3];
};
#endif