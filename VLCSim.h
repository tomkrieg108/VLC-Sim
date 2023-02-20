// VLCSim.h : main header file for the VLCSim application
//

#if !defined(AFX_VLC_H__DDB0CC86_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
#define AFX_VLC_H__DDB0CC86_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#pragma warning (disable:4786) // identifier was truncated to '255' characters in the debug 
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVlcApp:
// See VLCSim.cpp for the implementation of this class
//

class CVlcApp : public CWinApp
{
public:
	CVlcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVlcApp)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVlcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//globals

extern CVlcApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VLC_H__DDB0CC86_7A1D_11D6_93DD_0010B543CAF9__INCLUDED_)
