#ifndef TEXTVIEW_INTERNAL_INCLUDED
#define TEXTVIEW_INTERNAL_INCLUDED

#include <windows.h>  // For HWND, LRESULT
#include <utility>    // For std::move if needed in future

#define LONGEST_LINE 0x100

#include "TextDocument.h"

class TextView {
public:
    
    //TextView(HWND hwnd) : m_hWnd(hwnd) {}
    TextView(HWND hwnd);
    ~TextView();
    //LRESULT OnPaint(); Template

    LONG OnPaint();
    LONG OnSetFont(HFONT hFont);

    LONG OpenFile(TCHAR* szFileName);
    LONG ClearFile();


private:
    //
    // ---- TextView State
    //

    void PaintLine(HDC hdc, ULONG line);
    void TabbedExtTextOut(HDC hdc, RECT* rect, TCHAR* buf, int len);

    COLORREF GetTextViewColor(UINT idx);

    HWND m_hWnd;
    //--------------------------------------------------


    int x = 0;  // Initialized directly for better clarity

    // Font related data
    HFONT m_hFont;
    int m_nFontWidth;
    int m_nFontHeight;

    //------------------------------
    // File structure related data
    ULONG m_nLineCount;

    TextDocument* m_pTextDoc;
};

#endif // TEXTVIEW_INTERNAL_INCLUDED

