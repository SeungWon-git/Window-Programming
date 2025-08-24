#include <windows.h>
#include <tchar.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-4";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void Checknum(int* X, WPARAM wParam)
{
	if (wParam == '0')
		*X += 0;
	else if (wParam == '1')
		*X += 1;
	else if (wParam == '2')
		*X += 2;
	else if (wParam == '3')
		*X += 3;
	else if (wParam == '4')
		*X += 4;
	else if (wParam == '5')
		*X += 5;
	else if (wParam == '6')
		*X += 6;
	else if (wParam == '7')
		*X += 7;
	else if (wParam == '8')
		*X += 8;
	else if (wParam == '9')
		*X += 9;
}

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
	static TCHAR str[100];
	static int count;
	static SIZE size;
	static TCHAR lpOut[10'000];

	static int X, Y, N;

	static int X_, Y_, N_;

	static int X1, X2, X3, X4, Y1, Y2, Y3, Y4, N1, N2;

	static int space;

	static int errormsg;

	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5, 15);
		ShowCaret(hwnd);
		count = 0;
		space = 0;
		X = 10;
		X_ = 0;
		X1 = 0;
		X2 = 0;
		X3 = 0;
		X4 = 0;
		Y = 10;
		Y_ = 0;
		Y1 = 0;
		Y2 = 0;
		Y3 = 0;
		Y4 = 0;
		N = 0;
		N1 = 0;
		N2 = 0;
		N_ = 0;
		errormsg = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetTextExtentPoint32(hdc, str, lstrlen(str), &size);
		if (N != 0)
		{
			wsprintf(lpOut, L"1 * %d = %d   2 * %d = %d   3 * %d = %d   4 * %d = %d   5 * %d = %d   6 * %d = %d   7 * %d = %d   8 * %d = %d   9 * %d = %d ",
				N, 1 * N, N, 2 * N, N, 3 * N, N, 4 * N, N, 5 * N, N, 6 * N, N, 7 * N, N, 8 * N, N, 9 * N);
			for (int i = 0; i < 50; ++i)
				TextOut(hdc, X + i * 7, Y, &lpOut[i], 1);
			for (int i = 50; i < 100; ++i)
				TextOut(hdc, X + (i - 50) * 7, Y + 15, &lpOut[i], 1);
			for (int i = 100; i < _tcslen(lpOut); ++i)
				TextOut(hdc, X + (i - 100) * 7, Y + 30, &lpOut[i], 1);
		}
		if (errormsg == 1)
		{
			MessageBox(NULL, L"범위밖 숫자를 입력하였습니다.", L"에러", MB_OK);
			errormsg = 0;
		}
		TextOut(hdc, 0, 0, str, _tcslen(str));
		SetCaretPos(size.cx, 0);
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
	{
		if (wParam != VK_SPACE && wParam != VK_BACK && space < 2) {
			if (space == 0) {
				if (X1 == 0)
				{
					X1 = 1;
					Checknum(&X_, wParam);
				}
				else if (X2 == 0)
				{
					X2 = 1;
					X_ = X_ * 10;
					Checknum(&X_, wParam);
				}
				else if (X3 == 0)
				{
					X3 = 1;
					X_ = X_ * 10;
					Checknum(&X_, wParam);
				}
				else if (X4 == 0)
				{
					X4 = 1;
					X_ = X_ * 10;
					Checknum(&X_, wParam);
				}
				else if (X4 == 1)
				{
					errormsg = 1;
				}
			}
			else if (space == 1) {
				if (Y1 == 0)
				{
					Y1 = 1;
					Checknum(&Y_, wParam);
				}
				else if (Y2 == 0)
				{
					Y2 = 1;
					Y_ = Y_ * 10;
					Checknum(&Y_, wParam);
				}
				else if (Y3 == 0)
				{
					Y3 = 1;
					Y_ = Y_ * 10;
					Checknum(&Y_, wParam);
				}
				else if (Y4 == 0)
				{
					Y4 = 1;
					Y_ = Y_ * 10;
					Checknum(&Y_, wParam);
				}
				else if (Y4 == 1)
				{
					errormsg = 1;
				}
			}
		}
		else if (wParam != VK_RETURN && wParam != VK_BACK && space == 2)
		{
			if (N1 == 0)
			{
				N1 = 1;
				Checknum(&N_, wParam);
			}
			else if (N2 == 0)
			{
				N2 = 1;
				N_ = N_ * 10;
				Checknum(&N_, wParam);
			}
			else if (N2 == 1)
			{
				errormsg = 1;
			}
		}

		if (wParam == VK_BACK && count > 0)
		{
			if (space == 0) {
				if (X4 == 1)
					X4 = 0;
				else if (X3 == 1)
					X3 = 0;
				else if (X2 == 1)
					X2 = 0;
				else if (X1 == 1)
					X1 = 0;

				X_ /= 10;
			}
			else if (space == 1) {
				if (Y4 == 1)
					Y4 = 0;
				else if (Y3 == 1)
					Y3 = 0;
				else if (Y2 == 1)
					Y2 = 0;
				else if (Y1 == 1)
					Y1 = 0;
				else if (Y1 == 0)
					space--;
				

				Y_ /= 10;
			}
			else if (space == 2) {
				if (N2 == 1)
					N2 = 0;
				else if (N1 == 1)
					N1 = 0;
				else if (N1 == 0)
					space--;

				N_ /= 10;
			}

			count--;
		}
		else if (wParam == VK_RETURN)
		{
			if (X_ <= 0 || Y_ <= 0 || N_ <= 0)
			{
				DestroyWindow(hwnd);
			}
			else if (X_ > 705 || Y_ > 402 || N_ >= 20)
			{
				errormsg = 1;
			}
			else {
				X = X_;
				Y = Y_;
				N = N_;
			}

			count = 0;

			X_ = 0;
			X1 = 0;
			X2 = 0;
			X3 = 0;
			X4 = 0;
			Y_ = 0;
			Y1 = 0;
			Y2 = 0;
			Y3 = 0;
			Y4 = 0;
			N_ = 0;
			N1 = 0;
			N2 = 0;
			space = 0;
		}
		else if (wParam == VK_SPACE && space == 0)
		{
			str[count++] = wParam;
			space++;
		}
		else if (wParam == VK_SPACE && space == 1)
		{
			str[count++] = wParam;
			space++;
		}
		else if(errormsg != 1 && wParam != VK_BACK)
			str[count++] = wParam;


		str[count] = '\0';
		InvalidateRect(hwnd, NULL, true);
		break;
	}
	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}