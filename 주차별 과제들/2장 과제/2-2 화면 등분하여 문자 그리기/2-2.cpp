#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-2";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 400, 200, NULL, (HMENU)NULL, hInstance, NULL);

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
	TCHAR alp[200] = TEXT("abcdefghijklmnopqrstuvxyzabc\ndefghijklmnopqrstuvxyzabcdef\nghijklmnopqrstuvxyzabcdefghi\njklmnopqrstuvxyzabcdefghijklm\nnopqrstuvxyzabcdefghijklmnopqrstuvxyz");
	TCHAR ALP[200] = TEXT("ABCDEFGHIJKLMNOPQRSTUV\nXYZABCDEFGHIJKLMNOPQRS\nTUVXYZABCDEFGHIJKLMNOP\nQRSTUVXYZABCDEFGHIJKLM\nNOPQRSTUVXYZABCDEFGHIJKLMNOPQRSTUVXYZ");
	//TCHAR text[27];
	int R, G, B;
	int R_, G_, B_;

	srand(time(NULL));

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;
		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		rect.left = 0;
		rect.top = 0;
		rect.right = 200;
		rect.bottom = 100;

		SetTextColor(hdc, RGB(R, G, B));
		SetBkColor(hdc, RGB(R_, G_, B_));
		DrawText(hdc, alp, lstrlen(alp), &rect, DT_TOP | DT_LEFT);


		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;
		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		rect.left = 200;
		rect.top = 0;
		rect.right = 400;
		rect.bottom = 100;

		SetTextColor(hdc, RGB(R, G, B));
		SetBkColor(hdc, RGB(R_, G_, B_));
		DrawText(hdc, ALP, lstrlen(ALP), &rect, DT_TOP | DT_LEFT);


		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;
		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		rect.left = 0;
		rect.top = 80;
		rect.right = 200;
		rect.bottom = 200;

		SetTextColor(hdc, RGB(R, G, B));
		SetBkColor(hdc, RGB(R_, G_, B_));
		DrawText(hdc, ALP, lstrlen(ALP), &rect, DT_TOP | DT_LEFT);


		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;
		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		rect.left = 200;
		rect.top = 80;
		rect.right = 400;
		rect.bottom = 200;

		SetTextColor(hdc, RGB(R, G, B));
		SetBkColor(hdc, RGB(R_, G_, B_));
		DrawText(hdc, alp, lstrlen(alp), &rect, DT_TOP | DT_LEFT);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}