// EditHex.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "edit_hex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditHex

CEditHex::CEditHex()
{
	//m_max_chars = -1;
	//m_cur_chars = 0;
}

/*
CEditHex::CEditHex(int p_max_chars)
{
	m_max_chars = p_max_chars;
	m_cur_chars = 0;
}
*/

CEditHex::~CEditHex()
{
}

/*
void CEditHex::SetMaxChars(int p_max_chars)
{
	m_max_chars = p_max_chars;
}
*/

BEGIN_MESSAGE_MAP(CEditHex, CEdit)
	//{{AFX_MSG_MAP(CEditHex)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()






/////////////////////////////////////////////////////////////////////////////
// CEditHex message handlers

void CEditHex::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if(isdigit(nChar)   )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		//m_cur_chars++;
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
				CEdit::OnChar(nChar, nRepCnt, nFlags);

			}
		}
	}

	if( nChar == VK_BACK )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		
	}

}

CString CEditHex::Text()
{
	CString text;
	CEdit::GetWindowText( text );
	return text;
}
