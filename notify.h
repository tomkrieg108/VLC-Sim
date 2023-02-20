
#ifndef __NOTIFY_H
#define __NOTIFY_H

#include <list>
//using namespace std; //gives an error if this is un-commented


class INotifiable;

class CNotify
{

public:

	enum
	{
		ZERO = 0,

		SENT_VLC_PACKET,
		RECEIVE_VLC_PACKET,

		RECEIVE_VLC_POLL,
		RECEIVE_VLC_DATA_READ_REQUEST,
		RECEIVE_VLC_SITE_INFO_REQUEST,
		RECEIVE_VLC_WRITE_REQUEST,
		RECEIVE_VLC_ACK_NACK,
		RECEIVE_VLC_TRANSACTION_REPLY,
		RECEIVE_VLC_UNKNOWN,
		RECEIVE_VLC_ERROR,
	
		ACTIVE_ID_CHANGED,
		CONNECT,
		DISCONNECT,
		POWER_DOWN,
		POWER_UP,
		
		MAX
	};

	static void Initialise();
	static void Destroy();
	void Register(int p_id, INotifiable* p_notifiable);
	void DeRegister(int p_id, INotifiable* p_notifiable);
	void Notify( int p_id );

private:
	CNotify(void);
	~CNotify(void);
		
	std::list<INotifiable*>  m_notifies[MAX];
};



class INotifiable
{

friend class CNotify;

protected:

	INotifiable() {}
	virtual ~INotifiable() {}
	virtual void ProcessNotify( int p_id ) = 0;

};


extern CNotify *g_notify_mgr;



#endif