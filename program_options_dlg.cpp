// CProgramOptions.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "program_options_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgramOptions dialog

CProgramOptions* g_program_opts_dlg = 0;


CProgramOptions::CProgramOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgramOptions)
	//}}AFX_DATA_INIT
}


void CProgramOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgramOptions)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgramOptions, CDialog)
	//{{AFX_MSG_MAP(CProgramOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgramOptions message handlers

