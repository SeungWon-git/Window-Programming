#include <windows.h>
#include <tchar.h>
#include "resource.h"

//#pragma comment (lib, "msimg32.lib")			//-> TransparentBlt 함수

HINSTANCE g_hInst;
LPCTSTR lpszWindowName = L"windows program 7-2";

int pwindowWidth = 950, pwindowHeight = 900;
int cwindowWidth = 680, cwindowHeight = 850;	//4:5 비율

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK childProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)wndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"Parent Window Class";
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = L"Child Window Class";
	WndClass.lpfnWndProc = childProc;

	RegisterClassEx(&WndClass);


	hWnd = CreateWindow(L"Parent Window Class", lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 50, 0, pwindowWidth, pwindowHeight, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK childProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;

	static HBITMAP oldBit;
	static HBITMAP hBit_bg0, hBit_bg1, hBit_bg2, hBit_bg3, hBit_bg4, hBit_bg5, hBit_bg6, hBit_bg7, hBit_bg8, hBit_bg9, hBit_bg10;

	switch (iMsg)
	{
	case WM_CREATE:
		hBit_bg0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP0));
		hBit_bg1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, L"Left Button", L"Mouse Test", MB_OK);
		break;

	case WM_PAINT:
		// 더블 버퍼링
		hdc = BeginPaint(hwnd, &ps);

		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit_bg0);

		StretchBlt(hdc, 0, 0, cwindowWidth, cwindowHeight, memdc, 0, 0, 820, 1025, SRCCOPY);

		SelectObject(memdc, oldBit);
		DeleteDC(memdc);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND child_hWnd;

	HDC hdc, memdc;
	PAINTSTRUCT ps;

	//static TCHAR lpOut[1024];

	switch (iMsg)
	{
	case WM_CREATE:
		child_hWnd = CreateWindow(L"Child Window Class", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_THICKFRAME, 0, 10, cwindowWidth, cwindowHeight, hwnd, (HMENU)NULL, g_hInst, NULL);

		break;

	case WM_PAINT:
		// 더블 버퍼링
		hdc = BeginPaint(hwnd, &ps);

		memdc = CreateCompatibleDC(hdc);

		BitBlt(hdc, 0, 0, pwindowWidth, pwindowHeight, memdc, 0, 0, SRCCOPY);

		DeleteDC(memdc);

		EndPaint(hwnd, &ps);

		break;

	case WM_CHAR:
		switch (wParam)
		{

		case 'q':
		case 'Q':
			DestroyWindow(hwnd);
			break;

		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:

		break;

	case WM_LBUTTONUP:

		break;

	case WM_RBUTTONDOWN:

		break;

	case WM_RBUTTONUP:
	
		break;

	case WM_MOUSEMOVE:
		
		break;

	case WM_TIMER:
		switch (wParam) {

		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}