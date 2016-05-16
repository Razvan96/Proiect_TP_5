// NOUUU.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include<windows.h>
#include<winresrc.h>
#include <commdlg.h>
#include<mmsystem.h>
#include<string>
//#include<stdio.h>

#include "NOUUU.h"
#include "Resource.h"
//#define VC_EXTRALEAN 
//#define WIN32_LEAN_AND_MEAN
//#define WIN32_EXTRA_LEAN
#define MAX_LOADSTRING 100
// Global Variables:
char nume_fis[100];
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Comp(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Decomp(HWND, UINT, WPARAM, LPARAM);
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
    LoadStringW(hInstance, IDC_NOUUU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOUUU));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOUUU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(217, 225, 226));//(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOUUU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
//  PURPOSE:  Processes messages for the main window.
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
		LPCTSTR WindowCaption = L"Windows Application";
		MessageBox(NULL, L"Bun-venit la aplicatia de compresie a datelor !", WindowCaption, MB_OK);
	}break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_COMPRESIE:
		{
			LPCTSTR WindowCaption = L"Compresia";
			MessageBox(NULL, L"Alege fisierul pentru compresia datelor...", WindowCaption, MB_OK);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX1), hWnd, Comp);
		}break;
		case ID_FILE_DECOMPRESIE:
		{
			LPCTSTR WindowCaption = L"Decompresia";
			MessageBox(NULL, L"Alege fisierul pentru decompresia datelor...", WindowCaption, MB_OK);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX2), hWnd, Decomp);
		}break;
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
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

INT_PTR CALLBACK Comp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTON1:
		{
			OPENFILENAMEW ofn;       // common dialog box structure
			char szFile[260];       // buffer for file name
			HWND hwnd;              // owner window
			HANDLE hf;              // file handle

									// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = (LPTSTR)szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = (WCHAR) '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = (LPCWSTR) "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 

			LPCTSTR WindowCaption = L"Ai ales fisierul cu numele...";
			if (GetOpenFileName(&ofn) == TRUE)
			{
				MessageBox(NULL, (LPCWSTR)szFile, WindowCaption, MB_ICONINFORMATION);
			}
			/*hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);*/
			/*LPCTSTR WindowCaption = L"Alegere";
			MessageBox(NULL, L"Alege fisierul pentru care va avea loc compresia...", WindowCaption, MB_ICONINFORMATION);*/
			/*OPENFILENAME ofn;       // common dialog box structure
			char szFile[260];       // buffer for file name
			HWND hwnd;              // owner window
			HANDLE hf;              // file handle

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile =(LPWSTR) szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter =(LPCWSTR) "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box.

			if (GetOpenFileName(&ofn))
			{
			MessageBox(NULL, (LPCWSTR)szFile, TEXT("You selected the file..."), MB_OK);
			//LPCTSTR WindowCaption = L"Ai ales fisierul...";
			//MessageBox(NULL,(LPCWSTR) szFileName, WindowCaption, MB_ICONINFORMATION);
			// Do something usefull with the filename stored in szFileName
			}
			*/
		}break;
		//Aici trebuie pus codul pentru compresie
		case IDOK1:
		{
			LPCTSTR WindowCaption = L"Compress...";
			MessageBox(NULL, L"Sa inceapa compresia!", WindowCaption, MB_ICONINFORMATION);
		}break;
		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}

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



INT_PTR CALLBACK Decomp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDC_BUTTON2:
			{
				OPENFILENAMEW ofn;       // common dialog box structure
				char szFile[260];       // buffer for file name
				HWND hwnd;              // owner window
				HANDLE hf;              // file handle

										// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hDlg;
				ofn.lpstrFile = (LPTSTR)szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = (WCHAR) '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = (LPCWSTR) "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 

				LPCTSTR WindowCaption = L"Ai ales fisierul cu numele...";
				if (GetOpenFileName(&ofn) == TRUE)
				{
					MessageBox(NULL, (LPCWSTR)szFile, WindowCaption, MB_ICONINFORMATION);
				}
			}break;
			//Aici trebuie pus codul pentru decompresie
			case IDOK2:
			{
				LPCTSTR WindowCaption = L"Decompress...";
				MessageBox(NULL, L"Sa inceapa decompresia!", WindowCaption, MB_ICONINFORMATION);
			}break;
		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}



