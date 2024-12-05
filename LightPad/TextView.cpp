//
// MODULE:     TextView.cpp
//
// PURPOSE:    Implementation of the TextView control
//
// NOTES:      www.catch22.net
//

#include <windows.h>
#include <string>       // For std::string
#include <cwchar>  // For std::mbstowcs
#include "TextView.h"
#include "TextViewInternal.h"






//
// Painting procedure for TextView objects
//
LRESULT WINAPI TextView::OnPaint() noexcept
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
