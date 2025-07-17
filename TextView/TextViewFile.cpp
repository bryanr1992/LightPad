/*
* TextView file opening closing rutines
*/

#include <Windows.h>
#include <tchar.h>
#include "TextView.h"
#include "TextViewInternal.h"


LONG TextView::OpenFile(TCHAR* szFileName)
{
	ClearFile();

	m_pTextDoc->init(szFileName);
	m_nLineCount = m_pTextDoc->linecount();

	InvalidateRect(m_hWnd, NULL, FALSE);
	return TRUE;
}

LONG TextView::ClearFile()
{
	if (m_pTextDoc) { m_pTextDoc->clear(); }

	m_nLineCount = m_pTextDoc->linecount();

	InvalidateRect(m_hWnd, NULL, FALSE);
	return TRUE;
}