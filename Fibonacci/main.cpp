#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <cstdio>
#include <assert.h>
#include <gdiplus.h>

#include "resource.h"
#include "calcfib.h"

using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

LPTSTR long_string = "";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    INITCOMMONCONTROLSEX icc;

    HACCEL hAccelerators;
    HMENU hSysMenu;

    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = 0;// CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);                 /* menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Gerador de Fibonacci"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    hAccelerators = LoadAccelerators(hThisInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
    hSysMenu = GetSystemMenu(hwnd, FALSE);
    InsertMenu(hSysMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hSysMenu, 6, MF_BYPOSITION, ID_HELP_ABOUT, TEXT("Sobre"));

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    //UpdateWindow(hwnd);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    assert(st == Ok);
    if (st != Ok) return FALSE;

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        if (! TranslateAccelerator(messages.hwnd, hAccelerators, &messages))
        {
            /*std::cout << "looping...";
            std::cout << messages.message;
            std::cout << "|";*/
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }
    }

    GdiplusShutdown(gdiplusToken);

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}



INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
            //case IDCANCEL:

            EndDialog(hwndDlg, (INT_PTR) LOWORD(wParam));
            return (INT_PTR) TRUE;

        }
        break;
    }

    case WM_INITDIALOG:
        return (INT_PTR) TRUE;
    }

    return (INT_PTR) FALSE;
}

void Texto(HWND hWnd)
{
    RECT  rect;
    HDC hdc=GetWindowDC(hWnd);

    GetClientRect(hWnd, &rect);
    SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
    SetBkMode(hdc, TRANSPARENT);
    rect.left = 10;
    rect.top = 50;
    DrawText(hdc, "Hello World!", -1, &rect, DT_NOCLIP | DT_WORDBREAK | DT_CENTER);

    //HWND hedit=CreateWindow(TEXT("EDIT"),TEXT("Default window text"),WS_CHILD | WS_VISIBLE, rect.left,rect.top,rect.right,rect.bottom,hWnd,0,0,0);
    calcfib *cf = new calcfib();
    //cf=new calcfib();
    rect.top = 65;
    std::string my_str="";
    unsigned long c;
    for (int i=0; i<30; i++)
    {
        c=cf->calcula(i);
        my_str+=std::to_string(c)+" - ";
    }

    long_string = new TCHAR[my_str.size() + 1]; //define an array with size of my_str + 1
    strcpy(long_string, my_str.c_str());

    DrawText(hdc, long_string, -1, &rect, DT_NOCLIP | DT_WORDBREAK | DT_CENTER);
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance;

    switch (message)                  /* handle the messages */
    {

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_PAINT:
        {
            RECT  rect;
            HDC hdc=GetWindowDC(hwnd);

            GetClientRect(hwnd, &rect);
            SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
            SetBkMode(hdc, TRANSPARENT);
            rect.left = 10;
            rect.top = 65;
            DrawText(hdc, long_string, -1, &rect, DT_NOCLIP | DT_WORDBREAK | DT_CENTER);
            break;
        }

    case WM_SYSCOMMAND:
        switch (LOWORD(wParam))
        {
        case ID_HELP_ABOUT:
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hwnd, &AboutDialogProc);
            return 0;
        }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_HELP_ABOUT:
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hwnd, &AboutDialogProc);
            return 0;
        case ID_FILE_EXIT:
            DestroyWindow(hwnd);
            return 0;
        case IDM_GERAR1:
            Texto(hwnd);
        }
    }


    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


