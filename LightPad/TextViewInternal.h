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
    ULONG OnPaint();
    LONG PaintLine(HDC hdc, ULONG nLineno);


private:
    //
    // ---- TextView State
    //
    HWND m_hWnd;
    //--------------------------------------------------


    int x = 0;  // Initialized directly for better clarity

    // Font related data
    int m_nFontHeight;

    //--------------------
    TextDocument* m_pTextDoc;
};

#endif // TEXTVIEW_INTERNAL_INCLUDED

