
#include "stdafx.h"
#include "notify.h"



CNotify *g_notify_mgr = 0;


void CNotify::Initialise()
{
	if( g_notify_mgr == 0 )
	{
		g_notify_mgr = new CNotify();
	}
}

void CNotify::Destroy()
{
	if( g_notify_mgr != 0 )
	{
		delete g_notify_mgr;
		g_notify_mgr = 0;
	}
}

CNotify::CNotify()
{
	for(int i=0; i<MAX; i++)
	{
		m_notifies[i].clear();
	}
}

CNotify::~CNotify()
{
	for(int i=0; i<MAX; i++)
	{
		//itr = m_notifies[i].begin
		m_notifies[i].clear();
	}
}


void CNotify::Register(int p_id, INotifiable* p_notifiable)
{
	if( p_id <= 0)
		return;
	if( p_id >= MAX )
		return;

	std::list<INotifiable*>::iterator itr = m_notifies[p_id].begin();

	while( itr != m_notifies[p_id].end() )
	{
		if( (*itr) == p_notifiable ) //already registered
			return;
		itr++;
	}
	////////////////////////////////////////////////
	itr = m_notifies[p_id].begin();
	while( itr != m_notifies[p_id].end() )
	{
		if( (*itr) == NULL ) 
		{
			(*itr) = p_notifiable;
			return;
		}
		itr++;
	}	
	////////////////////////////////////////////////

	m_notifies[p_id].push_back( p_notifiable );
}

void CNotify::DeRegister(int p_id, INotifiable* p_notifiable)
{
	if( p_id <= 0)
		return;
	if( p_id >= MAX )
		return;

	//itr = m_notifies[p_id].begin();

	std::list<INotifiable*>::iterator itr = m_notifies[p_id].begin();
	while( itr != m_notifies[p_id].end() )
	{
		if( (*itr) == p_notifiable ) 
			//itr = m_notifies[p_id].erase( itr );	
			(*itr) = NULL; //ensures the destructor is not called
		else
			itr++;
	}
}

void CNotify::Notify(int p_id)
{
	if( p_id <= 0)
		return;
	if( p_id >= MAX )
		return;

	//itr = m_notifies[p_id].begin();

	std::list<INotifiable*>::iterator itr = m_notifies[p_id].begin();
	while( itr != m_notifies[p_id].end() )
	{
		if( (*itr) != NULL)
		(*itr)->ProcessNotify( p_id );
		itr++;
	}
}





