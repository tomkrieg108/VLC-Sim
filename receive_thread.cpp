// receive_thread.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "receive_thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReceiveThread

IMPLEMENT_DYNCREATE(CReceiveThread, CWinThread)

CReceiveThread::CReceiveThread()
{
}

CReceiveThread::~CReceiveThread()
{
}

BOOL CReceiveThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CReceiveThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CReceiveThread, CWinThread)
	//{{AFX_MSG_MAP(CReceiveThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReceiveThread message handlers
