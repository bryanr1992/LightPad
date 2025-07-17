#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <string>     // For std::wstring
#include <array>      // For fixed-size arrays
#include <locale> // for wstr conversions
#include <codecvt> // for wstr conversions
#include "..\TextView\TextView.h"
#include "resource.h"


std::array<TCHAR, MAX_PATH> szFileName{};
std::array<TCHAR, MAX_PATH> szFileTitle{};

HWND hwndMain;
HWND hwndTextView;

//#pragma comment(linker, "/OPT:NOWIN98")

BOOL ShowOpenFileDlg(HWND hwnd, LPTSTR pstrFileName, LPTSTR pstrTitleName)
{
    constexpr auto szFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");

    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = hwnd;
    ofn.hInstance = GetModuleHandle(nullptr);
    ofn.lpstrFilter = szFilter;
    ofn.lpstrFile = pstrFileName;
    ofn.lpstrFileTitle = pstrTitleName;
    ofn.nFilterIndex = 1;
    ofn.nMaxFile = MAX_PATH;
    ofn.nMaxFileTitle = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;

    return GetOpenFileName(&ofn);
}

void ShowAboutDlg(HWND hwndParent)
{
    std::string message = APP_TITLE;
    message.append(" ");
    message.append(WEBSITE);
    MessageBox(hwndParent, message.c_str(), APP_TITLE, MB_OK | MB_ICONINFORMATION);
}

std::string LPC2str(LPCSTR str) {
    return std::string(str);
}


void SetWindowFileName(HWND hwnd, LPCTSTR szFileName)
{
    std::string title = LPC2str(szFileName);
    title.append(" - ");
    title.append(APP_TITLE);
   // std::wstring temp = std::wstring(newFlName) + L" - " + APP_TITLE;
    //std::string title(temp.begin(), temp.end());
    SetWindowText(hwnd, title.c_str());
}

LRESULT CALLBACK WndProc(_In_ HWND hwnd, 
                         _In_ UINT msg, 
                         _In_ WPARAM wParam, 
                         _In_ LPARAM lParam)
{
    static int width, height;

    switch (msg)
    {
    case WM_CREATE:
        hwndTextView = CreateTextView(hwnd);
        PostMessage(hwnd, WM_COMMAND, IDM_FILE_NEW, 0);  // Automatically create new document
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_FILE_NEW:
            SetWindowFileName(hwnd, _T("Untitled"));
            return 0;

        case IDM_FILE_OPEN:
            if (ShowOpenFileDlg(hwnd, szFileName.data(), szFileTitle.data()))
            {
                SetWindowFileName(hwnd, szFileTitle.data());
            }
            return 0;

        case IDM_HELP_ABOUT:
            ShowAboutDlg(hwnd);
            return 0;
        }
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        MoveWindow(hwndTextView, 0, 0, width, height, TRUE);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int InitMainWnd()
{
    WNDCLASSEX wcx = {};
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = 0;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = GetModuleHandle(nullptr);
    wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcx.hbrBackground = nullptr;
    wcx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    wcx.lpszClassName = APP_TITLE;
    wcx.hIcon = static_cast<HICON>(LoadImage(wcx.hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, LR_CREATEDIBSECTION));
    wcx.hIconSm = static_cast<HICON>(LoadImage(wcx.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_CREATEDIBSECTION));

    if (!RegisterClassEx(&wcx))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 0;
    }

    return 1;

    //RegisterClassEx(&wcx);
}

HWND CreateMainWnd()
{
    return CreateWindowEx(0, APP_TITLE, APP_TITLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 560, 320,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
}

int WINAPI WinMain(_In_ HINSTANCE hInst, 
                   _In_opt_ HINSTANCE hPrev, 
                   _In_ LPSTR lpCmdLine, 
                   _In_ int iShowCmd)
{
    MSG msg;
    HACCEL hAccel;

    InitMainWnd();
    InitTextView();

    hwndMain = CreateMainWnd();
    ShowWindow(hwndMain, iShowCmd);

    hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        if (!TranslateAccelerator(hwndMain, hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
