// XEdit.h : header file
//

#ifndef XEDIT_H
#define XEDIT_H

extern UINT WM_XEDIT_KILL_FOCUS;
extern UINT WM_XEDIT_VK_ESCAPE;

/////////////////////////////////////////////////////////////////////////////
// CXEdit window

class CXEdit : public CEdit
{


public:
	enum Type
	{
		TEXT=0,
		HEX,
		DEC,
		BIN
	}; 


// Construction
public:
	CXEdit(CWnd *pParent, LPCTSTR lpszText, Type p_eType, int p_length=0);
	CXEdit();
	virtual ~CXEdit();

//OPS
	void SetLength(int p_length);
	void SetType(Type p_type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd *	m_pParent;
	CString	m_strText;
	BOOL	m_bMessageSent;
	Type	m_eType;
	int	m_nLength;

	void SendRegisteredMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	void ProcessChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Generated message map functions
protected:
	//{{AFX_MSG(CXEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};





#endif //XEDIT_H
