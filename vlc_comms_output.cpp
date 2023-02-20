

#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_main.h"
#include "vlc_comms_output.h"


CCommsOutput* g_comms_output = NULL;

void CCommsOutput::Initialise()
{
	g_comms_output = new CCommsOutput();	
}

void CCommsOutput::Destroy()
{
	delete g_comms_output;
}

CCommsOutput::CCommsOutput()
{
	//m_file.open("comms_log.txt");
	m_packet_list.clear();
	CThreadTest* m_output_thread = (CThreadTest *)AfxBeginThread( RUNTIME_CLASS( CThreadTest), THREAD_PRIORITY_LOWEST, 0, 0 );
}

CCommsOutput::~CCommsOutput()
{	
}

void CCommsOutput::EnableLogging(bool p_enable)
{
	if(p_enable)
	{
		if(!m_file.is_open())
			m_file.open("comms_log.txt", ios::out|ios::app); //creates file if not present, appends to it if it does.
		ASSERT(m_file.is_open());
	}
	else
	{
		m_file.close();
		ASSERT(!m_file.is_open());
	}
}


void CCommsOutput::Output(CVLCPacket* p_packet)
{
	if(p_packet==NULL)
		return;
	CVLCPacket* packet = new CVLCPacket(*p_packet); //make a copy
	CSingleLock single_lock(&m_sync);
	single_lock.Lock();
	m_packet_list.push_back(packet); //add to list to be displayed
	single_lock.Unlock();
	//if(m_output_thread->
}

void CCommsOutput::DisplayList()
{
	//this runs on seperate thread - called by CThreadTest::Run()

	//Relinquish remainder of time slice if nothing to display
	//if(m_packet_list.empty())
	//	Sleep(0);
	if(!m_file.is_open())
		return;

	CSingleLock single_lock(&m_sync);
	while( !m_packet_list.empty() )
	{
		single_lock.Lock();
		CVLCPacket* packet = m_packet_list.front();
		single_lock.Unlock();

		if(packet==NULL)
			return;
		
		CVLCPacket::StringList str_list;
		packet->GetAsStringList(str_list,true,true,false);
		std::list<CString*>::reverse_iterator ritr = str_list.rbegin();
		while(ritr != str_list.rend())
		{
			m_file << *(*ritr) << "\n";
			ritr++;
		}
		m_file << "\n\n";

		single_lock.Lock();
		m_packet_list.pop_front();
		single_lock.Unlock();
	}
}