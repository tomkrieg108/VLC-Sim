#if !defined(AFX_THREADTEST_H__89300EBD_CB8B_42E6_B4B3_867AA7A1830D__INCLUDED_)
#define AFX_THREADTEST_H__89300EBD_CB8B_42E6_B4B3_867AA7A1830D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreadTest.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CThreadTest thread

class CThreadTest : public CWinThread
{
	DECLARE_DYNCREATE(CThreadTest)
protected:
	CThreadTest();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadTest)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CThreadTest();

	// Generated message map functions
	//{{AFX_MSG(CThreadTest)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_THREADTEST_H__89300EBD_CB8B_42E6_B4B3_867AA7A1830D__INCLUDED_)
