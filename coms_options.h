#if !defined(AFX_CCOMSOPTIONS_H__647354C9_01BB_47DF_B08B_61AB80FC9843__INCLUDED_)
#define AFX_CCOMSOPTIONS_H__647354C9_01BB_47DF_B08B_61AB80FC9843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// coms_options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComsOptions dialog

class CComsOptions : public CDialog
{
// Construction
public:
	CComsOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComsOptions)
	enum { IDD = IDD_COMMSOPTIONS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComsOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComsOptions)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCOMSOPTIONS_H__647354C9_01BB_47DF_B08B_61AB80FC9843__INCLUDED_)
