
#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "vlc_exceptions.h"

CExceptionManager* g_exception_manager = 0;

void CExceptionManager::Initialise(void)
{
	g_exception_manager = new CExceptionManager();
}
void CExceptionManager::Destroy(void)
{
	if( g_exception_manager )
		delete g_exception_manager;
}

CExceptionManager::CExceptionManager(void)
{
}

CExceptionManager::~CExceptionManager(void)
{
	


}


const CString& CExceptionManager::GetDescription( ushort p_id )
{
	if( p_id <= EX_SUBSTANTIAL_CASHOUT ) 
		return m_exception_text[p_id][1];
	else
		return m_exception_text[EX_INVALID][1];
}	


const CString& CExceptionManager::GetDescription(CString& p_code, bool& p_found)
{
	for(int i=0; i<EX_MAX; i++)
	{
		if(p_code == m_exception_text[i][0])
		{
			p_found=true;
			return m_exception_text[i][1];
		}
	}
	p_found=false;
	return m_exception_text[EX_INVALID][1];
}

	
const CString& CExceptionManager::GetCodeText( ushort p_id )
{
	if( p_id <= EX_SUBSTANTIAL_CASHOUT ) 
		return m_exception_text[p_id][0];
	else
		return m_exception_text[EX_INVALID][0];
}	





ushort CExceptionManager::GetExceptionCode(CString& p_code_str)
{
	if(p_code_str.GetLength() != 2)
		return 0x0EF0; //Invalid

	ushort code=0x0000;
	code = CharToHex(p_code_str[0])<<4;
	code |= CharToHex(p_code_str[1]);
	code <<= 4;

	return code;
}







const CString CExceptionManager::m_exception_text[EX_MAX][2] =
{
	{"00","No Change"							},
	{"02","Memory Signature Complete"			},
	{"03","Entered Playable State"				},
	{"04","Entered Non-playable State (GM)"		},
	{"05","Entered Non-playable State (GMMS)"	},
	{"0B","Status Response Buffer Full"			},
	{"0C","Events Log Full"						},
	{"0D","Daily Poll Timeout"					},
	{"10","All Special Conditions Cleared"		},
	{"11","Coin Acceptor Tilt"					},
	{"13","Bill Acceptor Tilt"					},
	{"15","Printer Paper Low"					},
	{"16","Printer Paper Out"					},
	{"19","Hopper Empty"						},
	{"1B","Hopper Jam"							},
	{"1D","Reel Tilt"							},
	{"1E","Coin Diverter Error"					},
	{"1F","Hopper Excess Coin"					},
	{"20","All Doors Closed AK Off"				},
	{"21","Main Door Open AK Off"				},
	{"22","Cash Door Open AK Off"				},
	{"23","Secondary Door Open AK Off"			},
	{"24","Logic Door Open AK Off"				},
	{"27","AK Off"								},
	{"28","All Doors closed AK On"				},		
	{"29","Main Door Open AK On"				},
	{"2A","Cash Door Open AK On"				},
	{"2B","Secondary Door Open AK On"			},
	{"2C","Logic Door Open AK On"				},
	{"2F","AK On"								},
	{"30","Return to No Fault Condition"		},		
	{"34","EPROM Checksum Failure"				},
	{"35","Fatal RAM Failure"					},
	{"36","Battery Low"							},
	{"37","Main Logic Failure"					},
	{"38","Power Supply Failure"				},
	{"39","Coin Acceptor Failure"				},
	{"3B","Hopper Failure or Runnaway"			},
	{"3D","Printer Failure"						},
	{"3E","Bill Acceptor Failure"				},
	{"3F","Touchscreen Failure"					},
	{"40","Over Temperature"					},
	{"41","Hard Meters Failure"					},
	{"45","Misc Firmware Failure"				},	
	{"46","Misc Electronics Failure"			},
	{"47","Misc Mechanical Failure"				},
	{"48","Request Answered"					},
	{"49","Beverage Request"					},
	{"4A","Change Request"						},
	{"4B","Mechanic Request"					},
	{"51","Substantial Win"						},
	{"52","Substatial Cashout Performed"		},
	{"FF","Invalid Exception"					},

};


/*
const byte CExceptionManager::m_exception_code[EX_MAX] =
{
	{0x00},
	{0x02},
	{0x03},
	{0x04},
	{0x05},
	{0x0B},
	{0x0C},
	{0x0D},
	{0x10},
	{0x11},
	{0x13},
	{0x15},
	{0x16},
	{0x19},
	{0x1B},
	{0x1D},
	{0x1E},
	{0x1F},

	{0x20},
	{0x21},
	{0x22},
	{0x23},
	{0x24},
	{0x27},
	{0x28},	
	{0x29},
	{0x2A},
	{0x2B},
	{0x2C},
	{0x2F},

	{0x30},
	{0x34},
	{0x35},
	{0x36},
	{0x37},
	{0x38},
	{0x39},
	{0x3B},
	{0x3D},
	{0x3E},
	{0x3F},
	{0x40},
	{0x41},
	{0x45},	
	{0x46},
	{0x47},
	{0x48},
	{0x49},
	{0x4A},
	{0x4B},
	{0x51},
	{0x52},
	{0x53} //invalid
};
*/

