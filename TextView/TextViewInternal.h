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
    LONG OnSize(UINT nFlags, int width, int height);

    LONG OnVScroll(UINT nSBCode, UINT nPos);

    LONG OpenFile(TCHAR* szFileName);
    LONG ClearFile();


private:
    //
    // ---- TextView State
    //

    void PaintLine(HDC hdc, ULONG line);
    void RefreshWindow();
    void TabbedExtTextOut(HDC hdc, RECT* rect, TCHAR* buf, int len);

    COLORREF GetTextViewColor(UINT idx);

    void Scroll(int dx, int dy);
    VOID SetupScrollbars();
    bool PinToBottomCorner();

    HWND m_hWnd;
    //--------------------------------------------------


    //int x = 0;  // Initialized directly for better clarity

    // Font related data
    HFONT m_hFont;
    int m_nFontWidth;
    int m_nFontHeight;

    // Scrollbar related data
    // 
    //H/V scroll bar thumb pos
    ULONG m_nVScrollPos;
    int m_nHScrollPos;

    //H/V max scroll bar thumb position (Not the maximum number of lines in the file)
    ULONG m_nVScrollMax;
    int m_nHScrollMax;

    //Maximum horizontal scrolling extend. Also together with m_nLinecount we can represent the width and height of the client ares
    int m_nLongestLine;

    //Width and height of client area
    int m_nWindowLines;
    int m_nWindowColumns;

    //------------------------------
    // File structure related data
    ULONG m_nLineCount;// holds the amounts of lines drawn or that our doc currently has

    TextDocument* m_pTextDoc;
};

#endif // TEXTVIEW_INTERNAL_INCLUDED

