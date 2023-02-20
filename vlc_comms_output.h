#ifndef __VLC_COMMS_OUTPUT_H
#define __VLC_COMMS_OUTOUT_H

#include "stdtype.h"
#include "vlc_packet.h"
#include "ThreadTest.h"
#include <fstream.h>
#include <afxmt.h>

#pragma warning (disable:4786) // identifier was truncated to '255' characters in the debug 
#pragma warning (disable:4800) // forcing value to bool 'true' or 'false' (performance warning)

class CCommsOutput
{
	friend class CThreadTest;

public:
	static void Initialise();
	static void Destroy();
	void Output(CVLCPacket* p_packet);
	void EnableLogging(bool p_enable);
	bool IsLoggingEnabled() {return (bool)m_file.is_open();}
	
private:
	
	std::list<CVLCPacket*> m_packet_list; //list of packets to be displayed.
	CCommsOutput();
	virtual ~CCommsOutput();
	CThreadTest* m_output_thread;
	//CMutex m_sync; //CMutex is for synchronisation across processes
	CCriticalSection m_sync;
	ofstream m_file;
	void DisplayList();
};

extern CCommsOutput* g_comms_output;

#endif