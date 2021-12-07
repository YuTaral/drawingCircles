// drawingCircles.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include "framework.h"
#include "drawingCircles.h"
#include <debugapi.h>
#include <Windowsx.h>


#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];         // the main window class name;
HINSTANCE childhInst;

BOOLEAN childWindowCreated = false;
int xPos = 0;
int yPos = 0;
int ak = 0;
int pointsCounter = 0;
Point points[8];
BOOLEAN drawCircles = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void drawCircle(HDC hdc, int xPos, int yPos);
void drawLastCircles(HDC hdc);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRAWINGCIRCLES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClass(childhInst);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWINGCIRCLES));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_DRAWINGCIRCLES));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DRAWINGCIRCLES);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
    {
        if (childWindowCreated == false) {

            childWindowCreated = true;
            RECT rect;
            GetClientRect(hWnd, &rect);

            int width = rect.right - rect.left;
            int heigth = rect.bottom - rect.top;

            HWND childhWnd = CreateWindow(szWindowClass, szTitle, WS_CHILD | WS_CAPTION,
                CW_USEDEFAULT, CW_USEDEFAULT, width * 0.5, heigth * 0.5, hWnd, nullptr, childhInst, nullptr);

            ShowWindow(childhWnd, SW_SHOW);
        }

        break;
    }

    case WM_LBUTTONDOWN:
    {
        InvalidateRect(hWnd, NULL, FALSE);
        ak++;
        xPos = GET_X_LPARAM(lParam) - 50;
        yPos = GET_Y_LPARAM(lParam) - 50;

        break;
    }
    case WM_RBUTTONDOWN:
    {
        /*PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);
        graphics.Clear(Color(255, 255, 255));*/

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_MBUTTONDOWN:
    {
        drawCircles = true;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        drawCircle(hdc, xPos, yPos);
        drawLastCircles(hdc);

        GdiplusShutdown(gdiplusToken);
        EndPaint(hWnd, &ps);

    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void drawCircle(HDC hdc, int xPos, int yPos) {
    if (ak == 1) {
        Graphics graphics(hdc);

        Rect ellipseRect(xPos, yPos, 100, 100);

        Pen redPen(Color(255, 255, 0, 0));
        graphics.DrawEllipse(&redPen, ellipseRect);

        points[pointsCounter].X = xPos;
        points[pointsCounter].Y = yPos;
        pointsCounter++;

        if (pointsCounter == 8) {
            pointsCounter = 0;
        }
    }

    ak = 0;
}

void drawLastCircles(HDC hdc) {

    if (drawCircles == true) {
        Graphics graphics(hdc);

        Pen redPen(Color(255, 255, 0, 0));

        for (int i = 0; i < 8; i++) {

            Rect ellipseRect(points[i].X, points[i].Y, 100, 100);
            graphics.DrawEllipse(&redPen, ellipseRect);

        }
        drawCircles = false;
    }
}