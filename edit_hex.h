#if !defined(AFX_EDITHEX_H__E2253084_89A2_4C90_B13E_1BAFE3F5603C__INCLUDED_)
#define AFX_EDITHEX_H__E2253084_89A2_4C90_B13E_1BAFE3F5603C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// edit_hex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditHex window

class CEditHex : public CEdit
{
// Construction
public:
	CEditHex();
	//CEditHex(int p_max_chars);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditHex)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditHex();
	CString Text();

	//void SetMaxChars(int p_max_chars);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditHex)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	//int m_max_chars;
	//int m_cur_chars;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITHEX_H__E2253084_89A2_4C90_B13E_1BAFE3F5603C__INCLUDED_)
