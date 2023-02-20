


#include "stdafx.h"					
#include "VLCSim.h"	// need for "dlg_main.h"
#include "dlg_main.h"				// need for g_dlg_main->SetTimer() etc
#include "timers.h"

//todo - remove this debug stuff
//#include "game_play_dlg.h" // debug
//#include "utilities.h"		// these 2 only for displaying num timers used



void CTimers::Initialise()
{
	if( g_timers == 0 )
	{
		g_timers = new CTimers();
	}
}

void CTimers::Destroy()
{
	if( g_timers != 0 )
	{
		delete g_timers;
		g_timers = 0;
	}
}

CTimers::CTimers():
	m_count(0)
{
	for(int i=0; i<CAPACITY; i++)
	{
		m_timers[i] = 0; // indicating not used	
	}
}

CTimers::~CTimers()
{
}


void CTimers::RemoveTimer(int &p_id )
{
	if( p_id == 0 )
		return;

	if( p_id < CAPACITY )
	{
		m_timers[p_id] = 0;
		g_dlg_main->KillTimer( p_id );
		m_count--;
	//	g_game_play_dlg->SetStatus(1, Convert( m_count ));
		p_id = 0;
	}
}

int CTimers::AddTimer(int p_rate, ITimer *p_timer )
{
	int id;
	if(p_rate < 0) 
		return 0;
	if(p_timer == 0) 
		return 0;

	//find first unused element in m_timers (except 0)
	for( int i=1; i<CAPACITY; i++ )
	{
		if( m_timers[i] == 0 )
		{
			m_timers[i] = p_timer;
			g_dlg_main->SetTimer(i, p_rate, NULL);
			m_count++;
		//	g_game_play_dlg->SetStatus(1, Convert( m_count ));

			return i;
		}
	}
	return 0;
}

void CTimers::ResetTimer(int& p_id, int p_rate, ITimer* p_timer)
{
	RemoveTimer( p_id );
	p_id = AddTimer( p_rate, p_timer );
}

void CTimers::ProcessTimeout(int p_id)
{
	if( (p_id <= 0) || (p_id >= CAPACITY ) )
		return;

	ITimer* timer = m_timers[p_id];

	if( timer !=0 )
		timer->ProcessTimeout( p_id );
}



void ITimer::ProcessTimeout( int p_id )
{
	// default behaviour
	g_timers->RemoveTimer( p_id );
}


CTimers *g_timers = 0;


