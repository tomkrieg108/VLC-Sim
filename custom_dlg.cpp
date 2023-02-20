// Custom.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "custom_dlg.h"
#include "utilities.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustom dialog


CCustom::CCustom(CWnd* pParent /*=NULL*/)
	: CDialog(CCustom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustom)
	//}}AFX_DATA_INIT
}


void CCustom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustom)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustom, CDialog)
	//{{AFX_MSG_MAP(CCustom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustom message handlers

BOOL CCustom::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




