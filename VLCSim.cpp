// VLCSim.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VLCSim.h"
#include "dlg_main.h"
#include "notify.h"
#include "random.h"
#include "timers.h"
#include "date_time.h"
#include "game_play.h"
#include "vlc_id_list.h"
#include "vlc_exceptions.h"
#include "vlc_receive_thread.h"
#include "vlc_transmit.h"
#include "ThreadTest.h"
#include "vlc_comms_output.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVlcApp

BEGIN_MESSAGE_MAP(CVlcApp, CWinApp)
	//{{AFX_MSG_MAP(CVlcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVlcApp construction

CVlcApp::CVlcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVlcApp object

CVlcApp theApp;



// CVlcApp initialization

BOOL CVlcApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Initialise Global objects
	CTimeStamp::Initialise();
	CNotify::Initialise();
	CVlcReceive::Initialise();
	Rng::Initialise();
	CVlcTransmit::Initialise();
	CTimers::Initialise(); 
	CVlcIdList::Initialise();
	CExceptionManager::Initialise();
	CGamePlayMgr::Initialise();
	CCommsOutput::Initialise();
	
	CDlgMain dlg;
	m_pMainWnd = &dlg;
	g_dlg_main = &dlg;	//TK

	// todo
	// initialise all the global dialogs here
	//CThreadTest* thread_test = (CThreadTest *)AfxBeginThread( RUNTIME_CLASS( CThreadTest), THREAD_PRIORITY_LOWEST, 0, 0 );
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.


	// Delete global objetcs
	CNotify::Destroy();
	CVlcReceive::Destroy();
	CVlcTransmit::Destroy();
	Rng::Destroy();
	CTimers::Destroy();
	CVlcIdList::Destroy();
	CExceptionManager::Destroy();
	CGamePlayMgr::Destroy();
	CCommsOutput::Destroy();

	return TRUE;
}


// override Run for a laugh

int CVlcApp::Run()
{
	int a; 
	a=1;
	int b=a;
	DoMessageBox("xxx",0,0);
	
	return CWinApp::Run();
}



