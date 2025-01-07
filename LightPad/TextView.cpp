//
// MODULE:     TextView.cpp
//
// PURPOSE:    Implementation of the TextView control
//
// NOTES:      www.catch22.net
//

#include <windows.h>
#include <tchar.h>
#include <string>       // For std::string
#include <cwchar>  // For std::mbstowcs
#include "TextView.h"
#include "TextViewInternal.h"


//
//    Constructor for TextView class
//

TextView::TextView(HWND hwnd)
{
    m_hWnd = hwnd;
    m_nFontHeight = 0;
    m_pTextDoc = new TextDocument();
}

//
//    Destructor for TextView class
//
TextView::~TextView()
{
    m_nFontHeight = 0;
}

//
//    Drawing and Painting logic for out TextView centered around WM_PAINT
//

ULONG TextView::OnPaint()
{
    PAINTSTRUCT ps;

    BeginPaint(m_hWnd, &ps);

    ULONG first;
    ULONG last;
    ULONG i;

    //Find out which lines to draw
    first = ps.rcPaint.top / m_nFontHeight;
    last = ps.rcPaint.bottom / m_nFontHeight;

    for (i = first; i <= last; i++)
    {
        PaintLine(ps.hdc, i);
    }

    //paint
    EndPaint(m_hWnd, &ps);
    return 0;
}

LONG TextView::PaintLine(HDC hdc, ULONG nLineNo)
{
    TCHAR buf[LONGEST_LINE];
    ULONG len;

    RECT rect;
    GetClientRect(m_hWnd, &rect);

    //check where the line should be drawn
    rect.top = nLineNo * m_nFontHeight;
    rect.bottom = rect.top + m_nFontHeight;

    // get the data for this line of text
    len = m_pTextDoc->getline(nLineNo, buf, LONGEST_LINE);

    // draw text and erase the line background
    ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0);

    return 0;
}

//
// Painting procedure for TextView objects <TEMPLATE>
//
/*
LRESULT WINAPI TextView::OnPaint()
{
    PAINTSTRUCT ps;
    RECT rect;
    constexpr std::string_view text = "Hello World!";

    HDC hdc = BeginPaint(m_hWnd, &ps);

    HFONT hFont = static_cast<HFONT>(GetStockObject(ANSI_FIXED_FONT));
    HGDIOBJ hOldFont = SelectObject(hdc, hFont);

    GetClientRect(m_hWnd, &rect);

    ExtTextOutA(hdc, 10, 10, ETO_OPAQUE, &rect, text.data(), static_cast<UINT>(text.size()), nullptr);

    SelectObject(hdc, hOldFont);
    EndPaint(m_hWnd, &ps);

    return 0;
}
*/

//
// Win32 TextView window procedure.
//
LRESULT WINAPI TextViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto ptv = reinterpret_cast<TextView*>(GetWindowLongPtr(hwnd, 0));

    switch (msg)
    {
        case WM_NCCREATE:
        {
            auto* lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            ptv = new (std::nothrow) TextView(hwnd);
            if (!ptv) return FALSE;

            SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(ptv));
            return TRUE;
        }
        case WM_NCDESTROY:
            delete ptv;
            return 0;

        case WM_PAINT:
            return ptv->OnPaint();

        default:
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// Register the TextView window class
//
BOOL InitTextView() noexcept
{
    WNDCLASSEX wcx = {};

    wcx.cbSize = sizeof(wcx);
    wcx.style = 0;
    wcx.lpfnWndProc = TextViewWndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = sizeof(TextView*);
    wcx.hInstance = GetModuleHandle(nullptr);
    wcx.hCursor = LoadCursor(nullptr, IDC_IBEAM);
    wcx.hbrBackground = nullptr;  // Prevent flickering
    wcx.lpszClassName = TEXTVIEW_CLASS.data();

    return RegisterClassEx(&wcx) != 0;
}

//
// Create a TextView control
//
HWND CreateTextView(HWND hwndParent) noexcept
{
    return CreateWindowEx(WS_EX_CLIENTEDGE,
        TEXTVIEW_CLASS.data(), "",
        WS_VSCROLL | WS_HSCROLL | WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        hwndParent,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr);
}
