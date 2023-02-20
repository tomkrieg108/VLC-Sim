// CComsOptions.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "coms_options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComsOptions dialog


CComsOptions::CComsOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CComsOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComsOptions)
	//}}AFX_DATA_INIT
}


void CComsOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComsOptions)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComsOptions, CDialog)
	//{{AFX_MSG_MAP(CComsOptions)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComsOptions message handlers
