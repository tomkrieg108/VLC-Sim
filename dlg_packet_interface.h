
#ifndef __DLG_PACKET_INTERFACE_H
#define __DLG_PACKET_INTERFACE_H

#include "vlc_packet.h"
#include "vlc_id.h"

class IDlgPacket
{
public:
	IDlgPacket() {}
	virtual ~IDlgPacket() {}
	virtual void DisplayPacket(const CVLCPacket& p_packet )=0;
	virtual void SetPacket(CVLCPacket& p_packet )=0;
	virtual CVlcData* GetVlcData()=0;
	virtual void GetPacketName(CString& p_packet_name) {p_packet_name = "";}
};

#endif 