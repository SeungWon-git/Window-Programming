#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<time.h>

#define N 3
#define W 8
#define H 13

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-6";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void Printcase1(HDC hdc, int X, int Y)	// X모양
{
	int x = X;
	int y = Y;

	for (int j = 0; j < N / 2; ++j) {
		for (int i = 0; i < N - 1; i++) 
			x += W;
		for (int i = 0; i < j; ++i) 
			x += W;
		x += W;
		TextOut(hdc, x, y, L"X", 1);
		x += W;
		for (int i = 0; i < N - 1 - 2 * j; i++) 
			x += W;
		x += W;
		TextOut(hdc, x, y, L"X", 1);
		y += H;
		x = X;
	}
	for (int i = 0; i < (N + 1) / 2 + N - 1; i++) 
		x += W;
	x += W;
	TextOut(hdc, x, y, L"X", 1);
	y += H;
	x = X;
	for (int j = 0; j < N / 2; ++j)
	{
		for (int i = 0; i < N - 1; i++) 
			x += W;
		for (int i = N / 2 - j - 1; i > 0; --i) 
			x += W;
		x += W;
		TextOut(hdc, x, y, L"X", 1);
		x += W;
		for (int i = 2 * (j + 1); i > 0; --i) 
			x += W;
		x += W;
		TextOut(hdc, x, y, L"X", 1);
		y += H;
		x = X;
	}
}

void Printcase2(HDC hdc, int X, int Y)	// 역삼각형
{
	int x = X;
	int y = Y;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < i; ++j)
			x += W;
		for (int j = 0; j < N * 2 - i * 2; ++j) {
			TextOut(hdc, x, y, L"T", 1);
			x += W;
		}
		y += H;
		x = X;
	}
}

void Printcase3(HDC hdc, int X, int Y)	// 마름모
{
	int x = X;
	int y = Y;

	int M = N + 1;

	for (int i = 0; i < M - 1; ++i)
		x += W;
	TextOut(hdc, x, y, L"DD", 2);
	y += H;
	x = X;
	for (int j = 0; j < M / 2 - 2; ++j) {
		for (int i = 0; i < 2 * (M / 2 - 2 - j); ++i)
			x += W;
		for (int n = 0; n < 2 * M - 4 * (M / 2 - 2 - j); ++n) {
			TextOut(hdc, x, y, L"D", 1);
			x += W;
		}
		y += H;
		x = X;
	}
	for (int i = 0; i < 2 * M; i++) {
		TextOut(hdc, x, y, L"D", 1);
		x += W;
	}
	y += H;
	x = X;
	for (int i = 0; i < 2 * M; i++) {
		TextOut(hdc, x, y, L"D", 1);
		x += W;
	}
	y += H;
	x = X;
	for (int j = 0; j < M / 2 - 2; ++j) {
		for (int i = 0; i < 2 * (j + 1); ++i)
			x += W;
		for (int n = 0; n < 2 * M - 4 * (j + 1); ++n) {
			TextOut(hdc, x, y, L"D", 1);
			x += W;
		}
		y += H;
		x = X;
	}
	for (int i = 0; i < M - 1; ++i)
		x += W;
	TextOut(hdc, x, y, L"DD", 2);
	y += H;
	x = X;
}

void Printcase4(HDC hdc, int X, int Y)	// 나비
{
	int x = X;
	int y = Y;

	int M = N + 1;

	for (int i = 0; i < M / 2; ++i) {
		for (int j = 0; j < 2 * i + 1; ++j) {
			TextOut(hdc, x, y, L"B", 1);
			x += W;
		}
		for (int j = 0; j < M * 2 - 2 * (2 * i + 1); ++j) 
			x += W;
		for (int j = 0; j < 2 * i + 1; ++j) {
			TextOut(hdc, x, y, L"B", 1);
			x += W;
		}
		y += H;
		x = X;
	}
	for (int i = M / 2 - 1; i >= 0; --i) {
		for (int j = 0; j < 2 * i + 1; ++j) {
			TextOut(hdc, x, y, L"B", 1);
			x += W;
		}
		for (int j = 0; j < M * 2 - 2 * (2 * i + 1); ++j) 
			x += W;
		for (int j = 0; j < 2 * i + 1; ++j) {
			TextOut(hdc, x, y, L"B", 1);
			x += W;
		}
		y += H;
		x = X;
	}

}

void Printcase5(HDC hdc, int X, int Y)	// 모래시계
{
	int x = X;
	int y = Y;

	int M = N + 1;

	for (int i = M / 2; i > 0; --i) {
		for (int j = 0; j < 2 * (M / 2 - i); ++j) 
			x += W;
		for (int j = 0; j < i * 2 * 2; ++j) {
			TextOut(hdc, x, y, L"Y", 1);
			x += W;
		}
		y += H;
		x = X;
	}
	for (int i = 0; i < M / 2; ++i) {
		for (int j = 0; j < 2 * (M / 2 - (i + 1)); ++j) 
			x += W;
		for (int j = 0; j < (i + 1) * 2 * 2; ++j) {
			TextOut(hdc, x, y, L"Y", 1);
			x += W;
		}
		y += H;
		x = X;
	}

}

void Printcase6(HDC hdc, int X, int Y)	// 사각형
{
	int x = X;
	int y = Y;

	int M = N + 1;

	for (int i = 0; i < M; ++i) {
		if (i == 0 || i == M - 1) {
			for (int j = 0; j < 2 * M; ++j) {
				TextOut(hdc, x, y, L"B", 1);
				x += W;
			}
		}
		else {
			TextOut(hdc, x, y, L"B", 1);
			for (int j = 0; j <= 2 * M - 2; ++j)
				x += W;
			TextOut(hdc, x, y, L"B", 1);
		}
		y += H;
		x = X;
	}
}

void PrintScreen(HDC hdc, int x, int y, int X, int Y)
{
	srand(time(NULL));

	int num;
	//X:1, 역삼각형:2, 마름모:3, 나비:4, 모래시계:5, 사각형:6

	int R, G, B;
	int R_, G_, B_;

	for (int i = 0; i < x; i++) {  //가로(열)개수
		for (int j = 0; j < y; j++)  //세로(행)개수
		{
			num = rand() % 6 + 1;

			R = rand() % 256;
			G = rand() % 256;
			B = rand() % 256;

			R_ = rand() % 256;
			G_ = rand() % 256;
			B_ = rand() % 256;

			SetBkColor(hdc, RGB(R, G, B));
			SetTextColor(hdc, RGB(R_, G_, B_));

			switch (num) {
			case 1:
				Printcase1(hdc, X * i, Y * j);
				break;
			case 2:
				Printcase2(hdc, X * i, Y * j);
				break;
			case 3:
				Printcase3(hdc, X * i, Y * j);
				break;
			case 4:
				Printcase4(hdc, X * i, Y * j);
				break;
			case 5:
				Printcase5(hdc, X * i, Y * j);
				break;
			case 6:
				Printcase6(hdc, X * i, Y * j);
				break;
			}
		}
	}
};


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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

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
	int x, y;
	int X, Y;

	srand(time(NULL));

	x = rand() % 9 + 2;
	y = rand() % 9 + 2;

	X = 750 / x;
	Y = 550 / y;

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		PrintScreen(hdc, x, y, X, Y);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}