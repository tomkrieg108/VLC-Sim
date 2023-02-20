

#ifndef __VLC_EXCEPTIONS_H
#define __VLC_EXCEPTIONS_H

#include "stdtype.h"



struct ExceptionData
{
	ushort code;
	bool d_bit;

	ExceptionData(ushort p_code, bool p_d_bit):
		code(p_code),
		d_bit(p_d_bit)
	{
	}

	ExceptionData():
		code(0),
		d_bit(false)
	{
	}
};




class CExceptionManager
{
public:

//Todo
// this class could be made a static class

//These enums arn't currently used!
enum
{
	EX_NO_CHANGE = 0,							
	EX_MEM_SIG_COMPLETE	,					
	EX_PLAYABLE_STATE,						
	EX_NON_PLAYABLE_STATE_GM,				
	EX_NON_PLAYABLE_STATE_GMMS,				
	EX_STATUS_RESPONSE_BUFFER_FULL,			
	EX_EVENTS_LOG_FULL,						
	EX_DALY_POLL_TIMEOUT,					

	EX_ALL_SPECIAL_CONDITIONS_CLEARED,		
	EX_COIN_ACCEPTOR_TILT,					
	EX_BILL_ACCEPTOR_TILT,					
	EX_PRINTER_PAPER_LOW,					
	EX_PRINTER_PAPER_OUT,					
	EX_HOPPER_EMPTY,							
	EX_HOPPER_JAM,							
	EX_REEL_TILT,							
	EX_COIN_DIVERTER_ERROR,					
	EX_HOPPER_EXCESS_COIN,					

	EX_ALL_DOORS_CLOSED_AK_OFF,				
	EX_MAIN_DOOR_OPEN_AK_OFF,				
	EX_CASH_DOOR_OPEN_AK_OFF,				
	EX_SECONDARY_DOOR_OPEN_AK_OFF,			
	EX_LOGIC_DOOR_OPEN_AK_OFF,	
	EX_AK_OFF,

	EX_ALL_DOORS_CLOSED_AK_ON,					
	EX_MAIN_DOOR_OPEN_AK_ON,					
	EX_CASH_DOOR_OPEN_AK_ON,					
	EX_SECONDARY_DOOR_OPEN_AK_ON,			
	EX_LOGIC_DOOR_OPEN_AK_ON,				
	EX_AK_ON,								
									
	EX_RETURN_TO_NO_FAULT,					
	EX_EPROM_CHECKSUM_FAILURE,				
	EX_FATAL_RAM_ERROR,						
	EX_BATTERY_LOW,							
	EX_MAIN_LOGIC_FAILURE,					
	EX_POWER_SUPPLY_FAILURE,					
	EX_COIN_ACCEPTOR_FAILURE,				
	EX_HOPPER_FAILURE_RUNAWAY,				
	EX_PRINTER_FAILURE,						
	EX_BILL_ACCEPTOR_FAILURE,				
	EX_TOUCHSCREEN_FAILURE,					
	EX_OVER_TEMPERATURE,						
	EX_HARD_METERS_FAILURE,					
	EX_MISC_FIRMWARE_FAILURE,				
	EX_MISC_ELECTRONICS_FAILURE,				
	EX_MISN_MECHANICAL_FAILURE,				

	EX_REQUEST_ANSWERED,						
	EX_BEVO_REQUEST,							
	EX_CHANGE_REQUEST,						
	EX_MECHANIC_REQUEST,						

	EX_SUBSTANTIAL_WIN,						
	EX_SUBSTANTIAL_CASHOUT,	
	EX_INVALID,
	EX_MAX									
};


	static void Initialise(void);
	static void Destroy(void);

	const CString& GetDescription(ushort p_id);
	const CString& GetCodeText(ushort p_id);

	const CString& GetDescription(CString& p_code, bool& p_found);
	
	ushort GetExceptionCode(CString& p_code_str);

private:
	CExceptionManager(void);
	~CExceptionManager();

	static const CString m_exception_text[EX_MAX][2];

};

extern CExceptionManager* g_exception_manager;

#endif







