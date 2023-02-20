
#ifndef __Vlc_RECEIVE_H
#define __Vlc_RECEIVE_H

#include "mscommctrl.h"
//#include "xseries_packet.h"
#include "vlc_packet.h"
//#include "sa_comms_crc.h"

class CVlcReceive
{
public:

	static void Initialise();
	static void Destroy();
	void ProcessEvent(CMSCommCtrl&	p_comms);
	CVLCPacket* GetLastPacket(void) { return m_last_packet; }

private:

	CVlcReceive();
	~CVlcReceive();	

	CByteArray m_packet_data;
	bool m_receiving;
	int m_index;

	int m_count; // for debug purposes only

	CVLCPacket		*m_last_packet;
};

extern CVlcReceive  *g_receive_manager;

#endif