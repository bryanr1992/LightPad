//
// MODULE:     TextView.cpp
//
// PURPOSE:    Implementation of the TextView control
//
// NOTES:      www.catch22.net
//

#include <windows.h>
#include <tchar.h>
//#include <string>       // For std::string
//#include <cwchar>  // For std::mbstowcs
#include "TextView.h"
#include "TextViewInternal.h"


//
//    Constructor for TextView class
//

TextView::TextView(HWND hwnd)
{
    m_hWnd = hwnd;
    
    // Set the default font val
    OnSetFont((HFONT)GetStockObject(ANSI_FIXED_FONT));//GetStockObject is part of WINAPI

    // File structure related data
    m_nLineCount = 0;
    m_pTextDoc = new TextDocument();
}

//
//    Destructor for TextView class
//
TextView::~TextView()
{
    if (m_pTextDoc) { delete m_pTextDoc; }
}

// Set the default font
LONG TextView::OnSetFont(HFONT hFont)
{
    /*
    * SelectObject, GetDc and ReleaseDC are part of the WINAPI
    */
    HDC hdc;
    TEXTMETRIC tm;
    HANDLE hOld;

    m_hFont = hFont;

    hdc = GetDC(m_hWnd);
    hOld = SelectObject(hdc, hFont);

    GetTextMetrics(hdc, &tm);

    m_nFontHeight = tm.tmHeight;
    m_nFontWidth = tm.tmAveCharWidth;

    SelectObject(hdc, hOld);
    ReleaseDC(m_hWnd, hdc);

    return 0;

}
//
// Win32 TextView window procedure. (Moved for clarity when following the code. At lease mine)
//
LRESULT WINAPI TextViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    /*auto ptv = reinterpret_cast<TextView*>(GetWindowLongPtr(hwnd, 0));

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

    return DefWindowProc(hwnd, msg, wParam, lParam);*/
    TextView* ptv = (TextView*)GetWindowLong(hwnd, 0);

    switch (msg)
    {
        // First message received by any window - make a new TextView object
        // and store pointer to it in our extra-window-bytes
    case WM_NCCREATE:

        if ((ptv = new TextView(hwnd)) == 0)
            return FALSE;

        SetWindowLong(hwnd, 0, (LONG)ptv);
        return TRUE;

        // Last message received by any window - delete the TextView object
    case WM_NCDESTROY:

        delete ptv;
        return 0;

        // Draw contents of TextView whenever window needs updating
    case WM_PAINT:
        return ptv->OnPaint();

        // Set a new font 
    case WM_SETFONT:
        return ptv->OnSetFont((HFONT)wParam);

        //
    case TXM_OPENFILE:
        return ptv->OpenFile((TCHAR*)lParam);

    case TXM_CLEAR:
        return ptv->ClearFile();

    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// Register the TextView window class. (Moved for clarity when following the code. At lease mine) 
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
    wcx.hIcon = 0;
    wcx.hCursor = LoadCursor(nullptr, IDC_IBEAM);
    wcx.hbrBackground = nullptr;  // Prevent flickering. wcx.hbrBackground	= (HBRUSH)0;
    wcx.lpszMenuName = 0;
    wcx.lpszClassName = TEXTVIEW_CLASS.data();
    wcx.hIconSm = 0;

    return RegisterClassEx(&wcx) != 0;
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
