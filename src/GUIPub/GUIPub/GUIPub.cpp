// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GUIPub.h"
#include "ShellAPI.h"
#include "DotTrainer.h"

#define MAX_LOADSTRING 100
#define GWL_HINSTANCE       (-6)

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWndMain;

// Forward declarations of functions included in this code module:
VOID				startup(LPCWSTR path);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND CreateButton(int x, int y, int width, int height, string buttonText, int ID, HWND, HWND);
int BCX_Circle(HWND Wnd, int X, int Y, int R, int color, int Fill, HDC DrawHDC);
bool OptiKeyActive = false;

#define ID_OPTIKEYBUTTON 0x8801
HWND optikeyButton;
HWND trainingButton;
#define ID_TRAININGBUTTON 0x8805
PubSubHandler *pubSubHandler;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	pubSubHandler = new PubSubHandler();
	pubSubHandler->StartThread();
	
	
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUIPUB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIPUB));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		OutputDebugStringW(L"CONTINUING GUI\n");
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIPUB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_GUIPUB);
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

	 hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWndMain)
	{
		return FALSE;
	}

	optikeyButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Start Optikey",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		50,         // x position 
		50,         // y position 
		100,        // Button width
		100,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID_OPTIKEYBUTTON,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	trainingButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Start Training",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		50,         // x position 
		200,         // y position 
		100,        // Button width
		100,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID_TRAININGBUTTON,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	 //CreateButton(50, 200, 100, 100, "Start Training", ID_TRAININGBUTTON, trainingButton, hWndMain);


	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	return TRUE;
}

DotTrainer* dt;

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
	TCHAR greeting[] = _T("Hello, Windows desktop!");
	LPWSTR outputMsg = (LPWSTR)greeting;

	switch (message)
	{
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
		case ID_OPTIKEYBUTTON:
			if (!OptiKeyActive) {
				startup(_T("C:\\Program Files (x86)\\OptiKey\\OptiKey.exe"));
			}
			OptiKeyActive = true;
			break;
		case ID_TRAININGBUTTON:
			dt = new DotTrainer(pubSubHandler);
			dt->StartThread();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc,
			5, 5,
			outputMsg, wcslen(outputMsg));
		// TODO: Add any drawing code that uses hdc here...
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

VOID startup(LPCWSTR path)
{
	std::cout << "in startup\n";
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWDEFAULT);
	std::cout << "after attempt\n";
}

void CreateButton(int x, int y, int width, int height, wchar_t buttonText, int ID, HWND &button ,HWND &hWndMain) {
	button = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		(LPCWSTR)buttonText,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		x,         // x position 
		y,         // y position 
		width,        // Button width
		height,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
}

const int DOTRADIUS = 20;
void AddDotToDisplay(int x, int y) {
	BCX_Circle(hWndMain, x, y, DOTRADIUS, RGB(255,0,0), true, NULL);
}

void ClearDot(int x, int y) {
	BCX_Circle(hWndMain, x, y, DOTRADIUS, RGB(255, 255, 255), true, NULL);
}

int BCX_Circle(HWND Wnd, int X, int Y, int R, int color, int Fill, HDC DrawHDC)
{
	int a, b = 0;
	if (!DrawHDC)
	{
		DrawHDC = GetDC(Wnd);
		b = 1;
	}
	HPEN   hNPen = CreatePen(PS_SOLID, 1, color);
	HPEN   hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
	HBRUSH hOldBrush;
	HBRUSH hNewBrush;
	if (Fill)
	{
		hNewBrush = CreateSolidBrush(color);
		hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
	}
	else
	{
		hNewBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
	}
	// Win API function
	a = Ellipse(DrawHDC, X - R, Y + R, X + R, Y - R);
	DeleteObject(SelectObject(DrawHDC, hOPen));
	DeleteObject(SelectObject(DrawHDC, hOldBrush));
	if (b) ReleaseDC(Wnd, DrawHDC);
	return a;
}
