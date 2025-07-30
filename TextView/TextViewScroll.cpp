#include <Windows.h>
#include <tchar.h>
#include "TextView.h"
#include "TextViewInternal.h"

/*
*  Set scrollbar positions and range
*/

VOID TextView::SetupScrollbars()
{
	SCROLLINFO si = { sizeof(si) };

	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_DISABLENOSCROLL;

	/*
	* Vertical scrollbar
	*/

	si.nPos = m_nVScrollPos;		// scrollbar thumb position
	si.nPage = m_nWindowLines;		// number of lines in a page
	si.nMin = 0;
	si.nMax = m_nLineCount - 1;		// total number of lines in a file

	SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);


	/*
	*  Horizontal scrollbar
	*/

	si.nPos = m_nHScrollPos;		// scrollbar thumb position
	si.nPage = m_nWindowColumns;	//number of cols in a page
	si.nMin = 0;
	si.nMax = m_nLongestLine - 1;	// longest line in a file

	SetScrollInfo(m_hWnd, SB_HORZ, &si, TRUE);

	/*
	*  adjust our interpretation of the max scrollar range to make
	*  range-checking easier. The scrollbars don't use these values, they
	*  are for our own use.
	*/

	m_nVScrollMax = m_nLineCount - m_nWindowLines;
	m_nHScrollMax = m_nLongestLine - m_nWindowColumns;

}

bool TextView::PinToBottomCorner()
{
	bool repos = false;

	if (m_nHScrollPos + m_nWindowColumns > m_nLongestLine)
	{
		m_nHScrollPos = m_nLongestLine - m_nWindowColumns;
		repos = true;
	}

	if (m_nVScrollPos + m_nWindowLines > m_nLineCount)
	{
		m_nVScrollPos = m_nLineCount - m_nWindowLines;
		repos = true;
	}

	return repos;
}

/*
*  The window has changed size - update scrollbars
*/

LONG TextView::OnSize(UINT nFlags, int width, int height)
{
	//TODO: THIS NEEDS REVISING HARD 
	__try
	{
		m_nWindowLines = min((unsigned)height / m_nFontHeight, m_nLineCount);
		m_nWindowColumns = min(width / m_nFontWidth, m_nLongestLine);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		OutputDebugStringA("OnSize threw a structured exception.\n");
		return 0;
	}

	if (PinToBottomCorner())
	{
		RefreshWindow();
	}

	SetupScrollbars();

	return 0;
}
