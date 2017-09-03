/*Copyright 2016-2017 Siborgium
You're free to copy, replace, share, use, modify and do other actions with this program under rules of GNU GPL*/

#include "stdafx.h"
#include "TES Role Generator.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                 
WCHAR szTitle[MAX_LOADSTRING];          
WCHAR szWindowClass[MAX_LOADSTRING];           
HWND hWnd, genb, left, right;
PAINTSTRUCT ps;
HDC hdc;
RECT rectPlace;
HFONT hFont;
BOOL BOOLIsOnPress;

#include"Function.h"
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	hFont = CreateDefaultFont(-14, loadfontname("tech\\font").c_str());
	religion = new std::vector<std::string>(50);
	pattern = new std::vector<std::string>(20);
	weapon = new std::vector<std::string>(20);
	legacy = new std::vector<std::string>(50);
	interfacev = new std::vector<std::string>(10);
	database = new std::vector<hero>();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESROLEGENERATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow)) { return FALSE; }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESROLEGENERATOR));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))  { if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) { TranslateMessage(&msg); DispatchMessage(&msg); } }
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance){
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESROLEGENERATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESROLEGENERATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
   hInst = hInstance;
   char* TRG = "TES Role Generator";
      hWnd = CreateWindowW(szWindowClass, (LPCWSTR)TRG, WS_OVERLAPPED | WS_MINIMIZE | WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT,0, 580, 400, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd) { return FALSE;}
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message)    {
	case WM_CREATE: {
		database->reserve(50);
		initInterface();
		genb = CreateWindowA("BUTTON", loadStringFromFile("tech\\button").c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT, 355, 260, 150, 75, hWnd, HMENU(ID_BUTTON_GEN), NULL, NULL); SendMessage(genb, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
		right = CreateWindowW(L"BUTTON", L"←", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT, 325, 260, 30, 75, hWnd, HMENU(ID_BUTTON_RIGHT), NULL, NULL); SendMessage(right, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
		left = CreateWindowW(L"BUTTON", L"→", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT, 505, 260, 30, 75, hWnd, HMENU(ID_BUTTON_LEFT), NULL, NULL); SendMessage(left, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
	}
    case WM_COMMAND: {
			if ((HIWORD(wParam)==0)&&(LOWORD(wParam)==ID_BUTTON_GEN)) {
				database->push_back(hero());
				current_hero = database->size()-1;
				hdc = GetDC(hWnd);
				ResultOnScreen(hdc,hFont);
				ReleaseDC(hWnd, hdc);
			}
			if ((HIWORD(wParam) == 0) && (LOWORD(wParam) == ID_BUTTON_LEFT)) {
				if (current_hero >= 0 && current_hero < database->size()-1) {
					current_hero++;
					hdc = GetDC(hWnd);
					ResultOnScreen(hdc, hFont);
					ReleaseDC(hWnd, hdc);
				}
			}
			if ((HIWORD(wParam) == 0) && (LOWORD(wParam) == ID_BUTTON_RIGHT)) {
				if (current_hero > 0 && current_hero <= database->size()) {
					current_hero--;
					hdc = GetDC(hWnd);
					ResultOnScreen(hdc, hFont);
					ReleaseDC(hWnd, hdc);
				}
			}
			SetFocus(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
	case WM_PAINT: {
			hdc = BeginPaint(hWnd, &ps);
			InterfaceOnScreen(hFont, hdc);
			ResultOnScreen(hdc, hFont);
			EndPaint(hWnd, &ps);
		}
        break;
	case WM_KEYDOWN:  if (wParam == VK_ESCAPE) { DestroyWindow(hWnd); } break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: { return DefWindowProc(hWnd, message, wParam, lParam); }  }
    return 0;
}
