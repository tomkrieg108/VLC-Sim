
#ifndef __VLC_TRANSMIT_H
#define __VLC_TRANSMIT_H

#include "mscommctrl.h"
#include "timers.h"
#include "vlc_packet.h"

class CVlcTransmit : public ITimer
{
public:

	static void Initialise();
	static void Destroy();

	void OpenPort( CMSCommCtrl* p_comms);
	void ClosePort(void);

	void Send(CVLCPacket* p_packet);
	void ProcessEvent(void);
	CVLCPacket* GetLastPacket(){return m_last_packet_sent;}

protected:
	void ProcessTimeout( int p_id ); // from ITimer

private:

	CVlcTransmit();
	~CVlcTransmit();	

	CMSCommCtrl* m_comms;
	CVLCPacket* m_packet_being_sent;	//the (unmodified) packet currently being sent (0 if nothing being sent)
	CVLCPacket* m_last_packet_sent;
	
	int m_timer_data;
	int m_timer_rts;


	// this is for splitting up the packet into seperate chunks,
	// with delays between sending each chunk 

	void SplitBuffer();
	void MoveToBuffer();
	
	enum
	{
		MAX_CHUNKS = 5
	};

	CByteArray m_buffer; 
	CByteArray m_output[MAX_CHUNKS];
	int m_chunk_count;
	int m_chunk_index;
	int m_chunk_delay;
};


extern CVlcTransmit  *g_vlc_transmit_manager;


#endif