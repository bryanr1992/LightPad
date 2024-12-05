#ifndef TEXTVIEW_INTERNAL_INCLUDED
#define TEXTVIEW_INTERNAL_INCLUDED

#include <windows.h>  // For HWND, LRESULT
#include <utility>    // For std::move if needed in future

class TextView {
public:
    explicit TextView(HWND hwnd) noexcept : m_hWnd(hwnd) {}

    [[nodiscard]] LRESULT OnPaint() noexcept;

private:
    HWND m_hWnd;
    int x = 0;  // Initialized directly for better clarity
};

#endif // TEXTVIEW_INTERNAL_INCLUDED

