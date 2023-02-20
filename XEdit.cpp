// XEdit.cpp : implementation file
//

#include "stdafx.h"
#include "XEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT WM_XEDIT_KILL_FOCUS = ::RegisterWindowMessage(_T("WM_XEDIT_KILL_FOCUS"));
UINT WM_XEDIT_VK_ESCAPE  = ::RegisterWindowMessage(_T("WM_XEDIT_VK_ESCAPE"));

///////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CXEdit, CEdit)
	//{{AFX_MSG_MAP(CXEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXEdit::CXEdit(CWnd *pParent, LPCTSTR lpszText, Type p_eType, int p_length) :
	m_pParent(pParent),
	m_strText(lpszText),
	m_bMessageSent(FALSE),
	m_eType(p_eType),
	m_nLength(p_length)
{
	if(p_eType<TEXT)
		m_eType = TEXT;
	if(p_eType>BIN)
		m_eType = TEXT;
	
//	XLISTCTRL_TRACE(_T("in CXEdit::CXEdit\n"));
}

CXEdit::CXEdit():
	m_pParent(NULL),
	m_strText(""),
	m_bMessageSent(FALSE),
	m_eType(TEXT),
	m_nLength(100)
{
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXEdit::~CXEdit()
{
//	XLISTCTRL_TRACE(_T("in CXEdit::~CXEdit\n"));
}
///////////////////////////////////////////////////////////////////////////////
void CXEdit::SetLength(int p_length)
{
	if(p_length<0)
		return;
	m_nLength = p_length;
	LimitText(m_nLength);
	SetLimitText(m_nLength);
}
void CXEdit::SetType(Type p_type)
{
	m_eType = p_type;
}

///////////////////////////////////////////////////////////////////////////////
// SendRegisteredMessage
void CXEdit::SendRegisteredMessage(UINT nMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	BOOL bMessageSent = m_bMessageSent;
	m_bMessageSent = TRUE;
	if (m_pParent && ::IsWindow(m_pParent->m_hWnd) && !bMessageSent)
		m_pParent->SendMessage(nMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// OnCreate
int CXEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//XLISTCTRL_TRACE(_T("in CXEdit::OnCreate\n"));

	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the proper font
	if (!m_pParent)
	{
		m_pParent = GetParent();
	}

	if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
	{
		CFont * pFont = m_pParent->GetFont();
		if (pFont)
			SetFont(pFont);
	}
	else
	{
		m_pParent = NULL;
	}

	if(m_nLength>0)
	{
		LimitText(m_nLength);
		SetLimitText(m_nLength);
	}

	SetWindowText(m_strText);
	SetSel(0, -1);
	SetFocus();
	SetCapture();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXEdit::OnPaint() 
{
	CRect rect;
	GetClientRect(&rect);
	rect.right += 2;

	CDC * pDC = GetDC();
	pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
	ReleaseDC(pDC);

	CEdit::OnPaint();		// let CEdit draw the text

	//rect.right -= 2;
	rect.left -= 1;
	rect.top -= 1;
	rect.bottom -=1;

	pDC = GetDC();

	// don't erase the text that CEdit has just drawn
	CBrush * pOldBrush = (CBrush *) pDC->SelectStockObject(NULL_BRUSH);
	//CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_INACTIVECAPTION));	// same as combobox
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&rect);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
}

///////////////////////////////////////////////////////////////////////////////
// PreTranslateMessage
BOOL CXEdit::PreTranslateMessage(MSG* pMsg)
{
	// handle WM_KEYDOWN in case the edit has focus - otherwise
	// it will be sent to parent, bypassing WM_CHAR
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			SendRegisteredMessage(WM_XEDIT_KILL_FOCUS);
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			SendRegisteredMessage(WM_XEDIT_VK_ESCAPE);
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
// OnChar
void CXEdit::ProcessChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// handle escape and return, in case edit does NOT have focus

	if (m_pParent)
	{
		if (nChar == VK_ESCAPE)
		{
			SendRegisteredMessage(WM_XEDIT_VK_ESCAPE);
			return;
		}
		else if (nChar == VK_RETURN)
		{
			SendRegisteredMessage(WM_XEDIT_KILL_FOCUS);
			return;
		}
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////////
// OnKillFocus
void CXEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	SendRegisteredMessage(WM_XEDIT_KILL_FOCUS);
}

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void CXEdit::OnDestroy() 
{
//	XLISTCTRL_TRACE(_T("in CXEdit::OnDestroy\n"));
	ReleaseCapture();
	CEdit::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// OnChar
void CXEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
		case VK_BACK:
		case VK_DELETE:
		case VK_ESCAPE:
		case VK_RETURN:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		case VK_END:
		case VK_HOME:
		case VK_TAB:
			{
				ProcessChar(nChar, nRepCnt, nFlags);
				return;
			}
	};

	if( m_eType == TEXT )
	{
		ProcessChar(nChar, nRepCnt, nFlags);
		return;
	}

	if( m_eType == HEX )
	{
		if(isdigit(nChar)   )
		{
			ProcessChar(nChar, nRepCnt, nFlags);
			return;
		}

		if( isalpha( nChar) )
		{
			switch (nChar)
			{
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				{
					ProcessChar(nChar, nRepCnt, nFlags);
					return;
				}
			}
		}
	}

	if( m_eType == DEC )
	{
		if(isdigit(nChar)   )
		{
			ProcessChar(nChar, nRepCnt, nFlags);
			return;
		}
	}

	if( m_eType == BIN )
	{
		if(isdigit(nChar)   )
		{
			switch(nChar)
			{
				case '0':
				case '1':
					ProcessChar(nChar, nRepCnt, nFlags);
					return;
			};
		}
	}
}


