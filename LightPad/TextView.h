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

} // extern "C"

#endif // TEXTVIEW_INCLUDED
