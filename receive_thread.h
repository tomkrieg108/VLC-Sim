#if !defined(AFX_RECEIVE_THREAD_H__E5EF8977_D590_4AB2_B177_0120FBAF6A3A__INCLUDED_)
#define AFX_RECEIVE_THREAD_H__E5EF8977_D590_4AB2_B177_0120FBAF6A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// receive_thread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CReceiveThread thread

class CReceiveThread : public CWinThread
{
	DECLARE_DYNCREATE(CReceiveThread)
protected:
	CReceiveThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReceiveThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CReceiveThread();

	// Generated message map functions
	//{{AFX_MSG(CReceiveThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECEIVE_THREAD_H__E5EF8977_D590_4AB2_B177_0120FBAF6A3A__INCLUDED_)
