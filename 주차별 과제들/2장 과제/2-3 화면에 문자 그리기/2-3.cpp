#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-3";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_int_distribution<> uid;

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
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, 100, 50, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	TCHAR lpOut[10'000];

	int count = uid(dre) % 81 + 20;

	int t_r = uid(dre) % 256;
	int t_g = uid(dre) % 256;
	int t_b = uid(dre) % 256;

	int b_r = uid(dre) % 256;
	int b_g = uid(dre) % 256;
	int b_b = uid(dre) % 256;

	int num = uid(dre) % 10;

	switch (num)
	{
	case 0:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '0';
		lpOut[count] = '\0';
		break;
	case 1:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '1';
		lpOut[count] = '\0';
		break;
	case 2:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '2';
		lpOut[count] = '\0';
		break;
	case 3:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '3';
		lpOut[count] = '\0';
		break;
	case 4:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '4';
		lpOut[count] = '\0';
		break;
	case 5:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '5';
		lpOut[count] = '\0';
		break;
	case 6:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '6';
		lpOut[count] = '\0';
		break;
	case 7:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '7';
		lpOut[count] = '\0';
		break;
	case 8:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '8';
		lpOut[count] = '\0';
		break;
	case 9:
		for (int i = 0; i < count; ++i)
			lpOut[i] = '9';
		lpOut[count] = '\0';
		break;
	}

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		rect.left = uid(dre) % 701;
		rect.top = uid(dre) % 501;
		rect.right = rect.left + count * 10;
		rect.bottom = rect.top + 15;

		for (int i = 0; i < count; ++i) {
			SetTextColor(hdc, RGB(t_r, t_g, t_b));
			SetBkColor(hdc, RGB(b_r, b_g, b_b));
			DrawText(hdc, lpOut, _tcslen(lpOut), &rect, DT_SINGLELINE | DT_TOP | DT_LEFT);

			rect.top += 15;
			rect.bottom += 15;
		}
		
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}