#if !defined(AFX_CPROGRAMOPTIONS_H__5A658510_76A6_4DCA_A5FF_E9A021490E5F__INCLUDED_)
#define AFX_CPROGRAMOPTIONS_H__5A658510_76A6_4DCA_A5FF_E9A021490E5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// program_options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgramOptions dialog

class CProgramOptions : public CDialog
{
// Construction
public:
	CProgramOptions(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CProgramOptions)
	enum { IDD = IDD_PROGRAMOPTIONS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgramOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgramOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


extern CProgramOptions* g_program_opts_dlg;

#endif // !defined(AFX_CPROGRAMOPTIONS_H__5A658510_76A6_4DCA_A5FF_E9A021490E5F__INCLUDED_)
