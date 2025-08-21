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

/*
* Scroll the viewport in the correct direction
*/
VOID TextView::Scroll(int dx, int dy)
{
	//Make sure we don't go past the edges of the doc

	//scroll up/down
	//up
	if (dy < 0)
	{
		dy = -(int)min((ULONG)-dy, m_nVScrollPos);
	}
	//down
	else if (dy > 0)
	{
		dy = min((ULONG)dy, m_nVScrollMax - m_nVScrollPos);
	}

	//scroll left/right
	if (dx < 0)
	{
		dx = -(int)min(-dx, m_nHScrollPos);
	}
	//right
	else if (dx > 0)
	{
		dx = min((unsigned)dx, (unsigned)m_nHScrollMax - m_nHScrollPos);
	}

	//adjust sBar Thumb Pos
	m_nHScrollPos += dx;
	m_nVScrollPos += dy;

	// Here is where we actually scroll

	if (dx != 0 || dy != 0)
	{
		ScrollWindowEx(
			m_hWnd,
			-dx * m_nFontWidth,
			-dy * m_nFontHeight,
			NULL,
			NULL,
			0, 0, SW_INVALIDATE
		);

		SetupScrollbars();
	}
	/*
	* NOTE TO SELF:
	* we have -dx -dy here because these represent the direction that the thumb is going to go
	* for example say we scroll down. In this case the viewport goes up which means our dy values is positive
	* however the scroll bar or thumb actually goes down, which means we should take the opposite of that dy.
	* Same principle applies to dx
	*/
}

LONG GetTrackPos32(HWND hwnd, int nBar)
{
	SCROLLINFO si = { sizeof(si), SIF_TRACKPOS };
	GetScrollInfo(hwnd, nBar, &si);
	return si.nTrackPos;
}
/*
* Handling V scrolling
*/

LONG TextView::OnVScroll(UINT nSBCode, UINT nPos)
{
	ULONG oldpos = m_nVScrollPos;

	switch (nSBCode)
	{
	case SB_TOP:
		m_nVScrollPos = 0;
		RefreshWindow();
		break;

	case SB_BOTTOM:
		m_nVScrollPos = m_nVScrollMax;
		RefreshWindow();
		break;

	case SB_LINEUP:
		Scroll(0, -1);//TODO: IMPLEMENT THIS METHOD
		break;

	case SB_LINEDOWN:
		Scroll(0, 1);
		break;

	case SB_PAGEDOWN:
		Scroll(0, m_nWindowLines);
		break;

	case SB_PAGEUP:
		Scroll(0, -m_nWindowLines);
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:

		m_nVScrollPos = GetTrackPos32(m_hWnd, SB_VERT);//TODO: Implement GetTrackPos32
		RefreshWindow();
		break;
	}

	if (oldpos != m_nVScrollPos)
	{
		SetupScrollbars();
	}

	return 0;
}