#if !defined(AFX_CUSTOM_H__0B44C461_2880_11D7_9898_0010B543CAF9__INCLUDED_)
#define AFX_CUSTOM_H__0B44C461_2880_11D7_9898_0010B543CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Custom.h : header file
//
#include "list_ctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCustom dialog

class CCustom : public CDialog
{
// Construction
public:

	CCustom(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CCustom)
	enum { IDD = IDD_CUSTOM };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustom)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOM_H__0B44C461_2880_11D7_9898_0010B543CAF9__INCLUDED_)
