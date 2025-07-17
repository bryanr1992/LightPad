#ifndef TEXTVIEW_INCLUDED
#define TEXTVIEW_INCLUDED

#include <string_view> // For std::string_view
#include <windows.h>   // For HWND, BOOL

// Use constexpr for compile-time constant
constexpr std::string_view TEXTVIEW_CLASS = "TextView32";
//#define TEXTVIEW_CLASS _T("TextView32")

// Use extern "C" for C-style linkage if compiling in C++
extern "C" {

	// Declare the API functions using modern C++ type conventions
	BOOL InitTextView() noexcept;
	HWND CreateTextView(HWND hwndParent) noexcept;

	/*
	* TextView Window Messages definitions go here
	*/
	#define TXM_BASE (WM_USER)
	#define TXM_OPENFILE (TXM_BASE + 0)
	#define TXM_CLEAR (TXM_BASE + 1)

	/*
	* TextView Macros Messages definitions go here
	*/
	#define TextView_OpenFile(hwndTV, szFile) SendMessage((hwndTV), TXM_OPENFILE, 0, (LPARAM)(TCHAR*)(szFile))
	#define TextView_Clear(hwndTV) SendMessage((hwndTV), TXM_CLEAR, 0, 0)

	/*
	*  TextView Macros definitions go here
	*/
	//#define TEXTVIEW_CLASS_T("TextView32")

	/*
	* TextView Colors
	*/
	#define HVC_BACKGROUND		0			// normal background colour
	#define HVC_FOREGROUND		1			// normal foreground colour

	#define HVC_MAX_COLOURS		2			// keep this updated!

} // extern "C"

#endif // TEXTVIEW_INCLUDED
