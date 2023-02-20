// ThreadTest.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_main.h"
#include "utilities.h"
#include "ThreadTest.h"
#include "vlc_packet.h"
#include "vlc_comms_output.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CThreadTest

IMPLEMENT_DYNCREATE(CThreadTest, CWinThread)

CThreadTest::CThreadTest()
{
}

CThreadTest::~CThreadTest()
{
}

BOOL CThreadTest::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadTest::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}


int CThreadTest::Run()
{
	//int i=0;
	for(;;)
	{
		//i++;
		//if(g_dlg_main != NULL)
		//	g_dlg_main->m_receive_debug.SetWindowText(Convert(i));

		if(g_comms_output!=NULL)
			g_comms_output->DisplayList();
	}
	
	return 0;
}


BEGIN_MESSAGE_MAP(CThreadTest, CWinThread)
	//{{AFX_MSG_MAP(CThreadTest)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadTest message handlers



