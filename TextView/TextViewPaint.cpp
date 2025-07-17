/*
* Handles painting and displaying of the TextViewController
*/


#include <Windows.h>
#include <tchar.h>
#include "TextView.h"
#include "TextViewInternal.h"
#include <stdexcept>


/*
* Painting proc for TextView Object
*/

//LONG TextView::OnPaint()
//{
//	PAINTSTRUCT ps; // Part of WinUser.h
//
//	ULONG i;
//	ULONG first;
//	ULONG last;
//
//	BeginPaint(m_hWnd, &ps);
//
//	// select which font we will be using
//	SelectObject(ps.hdc, m_hFont);
//
//	// Check which lines to redraw.
//
//	/*
//	* For my own sanity. We divide the current size of the window by the height of the font and that
//	* tells us how many lines to redraw
//	*/
//	first = ps.rcPaint.top / m_nFontHeight;
//	last = ps.rcPaint.bottom / m_nFontHeight;
//
//	// Make sure we never wrap around the boundary
//	if (last < first) { last = -1; }
//
//	// draw the display line by line
//	for (i = first; i <= last; i++)
//	{
//		PaintLine(ps.hdc, i);
//	}
//
//	EndPaint(m_hWnd, &ps);
//
//	return 0;
//}
LONG TextView::OnPaint()
{
    //// Defensive check
    //if (!IsWindow(m_hWnd)) {
    //    OutputDebugStringA("OnPaint called with invalid window handle.\n");
    //    return 0;
    //}

    PAINTSTRUCT ps;
    ULONG i, first, last;

    HDC hdc = nullptr;

    __try  // Windows SEH (Structured Exception Handling) - MSVC only
    {
        hdc = BeginPaint(m_hWnd, &ps);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        OutputDebugStringA("BeginPaint threw a structured exception.\n");
        return 0;
    }

    if (!hdc)
    {
        OutputDebugStringA("BeginPaint returned NULL HDC.\n");
        return 0;
    }

    SelectObject(hdc, m_hFont);

    first = ps.rcPaint.top / m_nFontHeight;
    last = ps.rcPaint.bottom / m_nFontHeight;

    if (last < first) { last = -1; }

    for (i = first; i <= last; i++)
    {
		PaintLine(hdc, i);  
    }

    EndPaint(m_hWnd, &ps);
    return 0;
}



/*
* Emulates ExtTextOut, but we draw the text using TabbedTextOut
*/

void TextView::TabbedExtTextOut(HDC hdc, RECT* rect, TCHAR* buf, int len)
{
	int tab = 4 * m_nFontWidth;
	int width;
	RECT fill = *rect;

	// Draw line and expand tabs
	width = TabbedTextOut(hdc, rect->left, rect->top, buf, len, 1, &tab, rect->left);

	// Erase the rest of the line with the bg color
	fill.left += LOWORD(width);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &fill, 0, 0, 0);
}

/*
* Draw the specified line
*/

void TextView::PaintLine(HDC hdc, ULONG nLineNo)
{
	RECT rect;

	TCHAR buf[LONGEST_LINE];
	int len;


	// Get the area we want to update
	GetClientRect(m_hWnd, &rect); // From WINAPI

	// compute rect for entire length of line in the window
	rect.top = nLineNo * m_nFontHeight;
	rect.bottom = rect.top + m_nFontHeight;
	return;

	// Check that we have data/text to draw on this line

	if (nLineNo >= m_nLineCount)
	{
		SetBkColor(hdc, GetTextViewColor(HVC_BACKGROUND));//part of WINDGAPI. GetTextViewColor is ours
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
		return;
	}

	// Get the data for this line of text
	len = m_pTextDoc->getline(nLineNo, buf, LONGEST_LINE);

	// set the colors
	SetTextColor(hdc, GetTextViewColor(HVC_FOREGROUND));
	SetBkColor(hdc, GetTextViewColor(HVC_BACKGROUND));

	// Draw the text and fill line bg at the same time
	TabbedExtTextOut(hdc, &rect, buf, len);
}


COLORREF TextView::GetTextViewColor(UINT idx)
{
	switch (idx)
	{
	case HVC_BACKGROUND: return GetSysColor(COLOR_WINDOW);
	case HVC_FOREGROUND: return GetSysColor(COLOR_WINDOWTEXT);
	default:			 return 0;
	}
}