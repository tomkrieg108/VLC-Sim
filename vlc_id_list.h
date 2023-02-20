

#ifndef __VLC_ID_LIST_H
#define __VLC_ID_LIST_H

#include "stdtype.h"
#include "timers.h"
#include "notify.h"
#include "afxtempl.h" // for CList
#include "vlc_id.h"
#include "vlc_exceptions.h"
//#include <list>

//////////////////////////////////////////////////////
class CVlcIdList : public INotifiable
{
public:
	typedef std::list<CVlcId*>::iterator Id_Iterator;

	static void Initialise();
	static void Destroy();

	void SetActiveId(CString& p_id);
	void GetActiveId(CString& p_id);
	bool AddId(CString& p_id);
	void AddId(CVlcId* p_id);
	void DeleteId(CString& p_id);
	CVlcId* GetActiveId();  
	int IdCount();
	void QueueException(ExceptionData& p_exception_data);	//Called when an exception is added-todo - not called?
	void GetIterator( Id_Iterator& p_itr) {p_itr = m_list_id.begin();}
	bool AtEnd(Id_Iterator& p_itr) { return p_itr == m_list_id.end(); }

protected:
	//void ProcessTimeout( int p_id ); // for ITimer
	void ProcessNotify( int p_id );  // for INotifiable
		
private:
	std::list<CVlcId*>  m_list_id;
	
	CVlcIdList(void);
	~CVlcIdList();
	
	CVlcId* FindId(CString& p_id, int& p_pos);	
	
	void HandlePoll();				//Called when a poll is received - called via notification
	void HandleReadRequest();
	void HandleWriteRequest();
	void HandleTransactionReply();
	void HandleAckNack();
	
	CVlcId* m_active_id;
};

extern CVlcIdList* g_vlc_id_list;

//////////////////////////////////////////////////////
#endif
