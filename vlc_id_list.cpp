
#include "stdafx.h"
#include "VLCSim.h"
#include "vlc_id_list.h"
#include "vlc_packet.h"
#include "vlc_receive_thread.h"
#include "dlg_main.h"
#include "notify.h"



CVlcIdList* g_vlc_id_list = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcIdList::Initialise()
{
	if( !g_vlc_id_list )
	{
		g_vlc_id_list = new CVlcIdList();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcIdList::Destroy()
{
	if( g_vlc_id_list != 0 )
	{
		delete g_vlc_id_list;
		g_vlc_id_list = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CVlcIdList::CVlcIdList() :
	m_active_id(0)
{
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_POLL, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_DATA_READ_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_SITE_INFO_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_WRITE_REQUEST, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_TRANSACTION_REPLY, this );
	g_notify_mgr->Register( CNotify::RECEIVE_VLC_ACK_NACK, this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcIdList::~CVlcIdList()
{
	//todo
	//Crashes!?
//	g_notify_mgr->DeRegister( CNotify::RECEIVE_VLC_POLL, this );
//	g_notify_mgr->DeRegister( CNotify::RECEIVE_VLC_DATA_READ_REQUEST, this );
//	g_notify_mgr->DeRegister( CNotify::RECEIVE_VLC_SITE_INFO_REQUEST, this );
//	g_notify_mgr->DeRegister( CNotify::RECEIVE_VLC_WRITE_REQUEST, this );
//	g_notify_mgr->DeRegister( CNotify::RECEIVE_VLC_ACK_NACK, this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CVlcIdList::ProcessNotify( int p_id )
{

	switch (p_id)
	{
	case CNotify::RECEIVE_VLC_POLL:
		HandlePoll();
		break;
	case CNotify::RECEIVE_VLC_DATA_READ_REQUEST:
	case CNotify::RECEIVE_VLC_SITE_INFO_REQUEST:
		HandleReadRequest();
		break;
	case CNotify::RECEIVE_VLC_WRITE_REQUEST:
		HandleWriteRequest();
		break;
	case CNotify::RECEIVE_VLC_TRANSACTION_REPLY:
		HandleTransactionReply();
		break;
	case CNotify::RECEIVE_VLC_ACK_NACK:
		HandleAckNack();
		break;
	default:
		break;
	};
}

int CVlcIdList::IdCount()
{
	return m_list_id.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Search through list of Id's and return the one matching p_command
	Return null if no match
 */

CVlcId* CVlcIdList::FindId(CString& p_id, int& p_pos)
{
	//find the indexed item in the list
	CVlcId* current=NULL;
	bool found=false;
	p_pos=0;
	std::list<CVlcId*>::iterator itr = m_list_id.begin();
	while( itr != m_list_id.end() )
	{
		current = (*itr);
		CString id_str;
		current->GetId(id_str);

		if( id_str == p_id)
		{	
			found=true;
			break;
		}
		else
		{
			p_pos++;
			itr++;
		}
	}	
	if(found)
		return current;
	else
		return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcIdList::HandlePoll()
{
	CString id_received; 
	const CVLCPacket *poll = g_receive_manager->GetLastPacket();
	poll->GetTerminalId(id_received);

	/*
	CDlgMain::PollData poll_data;
	poll_data.id = id_received;
	poll_data.v = poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_V);
	poll_data.d = poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_D);
	poll_data.f = poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_F);
	poll_data.p = poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_P);
	poll_data.i = poll->IsCommandFlagSet(CVLCPacket::CMD_FLG_I);
	*/

	int pos;
	CVlcId* id = FindId(id_received,pos);

	if(id==NULL)
	{	
		
		CVlcId* new_id = new CVlcId(id_received);
		AddId(new_id);
		new_id->HandlePoll(poll);
		//g_dlg_main->AddId(new_id);
		g_dlg_main->UpdateIdDisplay(new_id,poll);
		

		/*
		CVlcId* new_id = new CVlcId(poll);
		m_list_id.push_back(new_id);

		g_dlg_main->AddId(poll_data);
		//ushort command = poll->GetCommand();

		//If this is the first one in the list then make it the active ID
		if(m_list_id.size()==1)
		{
			m_active_id = new_id;
			new_id->m_is_active_id=true;
			g_notify_mgr->Notify( CNotify::ACTIVE_ID_CHANGED );
		}
		*/
	}
	else
	{
		id->HandlePoll(poll); 
		g_dlg_main->UpdateIdDisplay(id,poll);
		//g_dlg_main->UpdateId(poll_data,pos );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcIdList::HandleReadRequest()
{
	//Todo - might want to make this a function!
	CString id_received; 
	const CVLCPacket *read_request = g_receive_manager->GetLastPacket();
	read_request->GetTerminalId(id_received);

	int pos;
	CVlcId* id = FindId(id_received,pos);

	if(!id)
	{
		//This is an error condition - should not occur
		return;
	}

	id->HandleReadRequest();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcIdList::HandleWriteRequest()
{
	CString id_received; 
	const CVLCPacket *write_request = g_receive_manager->GetLastPacket();
	write_request->GetTerminalId(id_received);

	int pos;
	CVlcId* id = FindId(id_received,pos);

	if(!id)
	{
		//This is an error condition - should not occur
		return;
	}	
	id->HandleWriteRequest();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcIdList::HandleTransactionReply()
{
	CString id_received; 
	const CVLCPacket *trans_reply = g_receive_manager->GetLastPacket();
	trans_reply->GetTerminalId(id_received);

	int pos;
	CVlcId* id = FindId(id_received,pos);

	if(!id)
	{
		//This is an error condition - should not occur
		return;
	}	
	id->HandleTransactionReply();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVlcIdList::HandleAckNack()
{
	CString id_received; 
	const CVLCPacket *read_request = g_receive_manager->GetLastPacket();
	read_request->GetTerminalId(id_received);

	int pos;
	CVlcId* id = FindId(id_received,pos);

	if(!id)
	{
		//This is an error condition - should not occur
		return;
	}
	id->HandleAckNack();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CVlcIdList::SetActiveId(CString& p_id)
{
	int pos;
	CVlcId* id = FindId(p_id,pos);

	if(!id)
		return;
	
	CString id_str;
	id->GetId(id_str);

	if(id != m_active_id)
	{
		m_active_id->m_is_active_id=false;
		m_active_id = id;
		id->m_is_active_id=true;
		g_notify_mgr->Notify( CNotify::ACTIVE_ID_CHANGED );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVlcIdList::GetActiveId(CString& p_id)
{
	if(m_active_id != 0)
	{
		CString active_id_str;
		m_active_id->GetId(active_id_str);
		p_id = active_id_str;	
	}
	else
	{
		p_id="";	
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//todo
//i don't think this is called anywhere!?
void CVlcIdList::QueueException(ExceptionData& p_exception_data)
{
	if( m_active_id != NULL )
	{
		m_active_id->QueueException(p_exception_data);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CVlcId* CVlcIdList::GetActiveId()
{
	return m_active_id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVlcIdList::AddId(CString& p_id)
{
	int pos=0;
	CVlcId* id = FindId(p_id,pos);
	if(id != NULL)
	{
		MessageBox(0,"ID Already exists", "", MB_OK);
		return false;
	}
	AddId(new CVlcId(p_id));
	return true;
}

void CVlcIdList::AddId(CVlcId* p_id)
{
	if(p_id==NULL)
		return;
	m_list_id.push_back(p_id);
	g_dlg_main->AddId(p_id);
	//If this is the first one in the list then make it the active ID
	if(m_list_id.size()==1)
	{
		m_active_id = p_id;
		p_id->m_is_active_id=true;
		g_notify_mgr->Notify( CNotify::ACTIVE_ID_CHANGED );
	}
}

void CVlcIdList::DeleteId(CString& p_id)
{
	//todo - implement this
}










