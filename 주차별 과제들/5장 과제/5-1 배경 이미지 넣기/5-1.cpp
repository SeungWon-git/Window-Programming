#include <windows.h>
#include <tchar.h>
#include <random>
#include "resource1.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 5-1";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_int_distribution<> uid{ 1, 4 };

void RandomRaster(DWORD& raster) {
	int num = uid(dre);

	if (num == 1) 
		raster = SRCCOPY;
	else if (num == 2)
		raster = NOTSRCCOPY;
	else if (num == 3)
		raster = BLACKNESS;
	else if (num == 4)
		raster = WHITENESS;
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC hdc, hMemDC;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;

	static int FW, FH, SW, SH;

	static auto raster = SRCCOPY;

	static int screen_div;

	static bool full_screen;

	static float slide_x;

	static int mx, my;

	static int idx_x, idx_y;

	static DWORD div_2_raster[2][2];
	static DWORD div_3_raster[3][3];

	static float stretch_rate;

	switch (iMsg)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

		FW = 800; 
		FH = 600;
		SW = 640;
		SH = 422;
		full_screen = false;
		screen_div = 1;
		slide_x = SW;
		stretch_rate = 0;

		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				RandomRaster(div_2_raster[i][j]);
			}
		}

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				RandomRaster(div_3_raster[i][j]);
			}
		}


		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC, hBitmap);

		if (screen_div == 1) {
			if (full_screen == false) {
				/*if (stretch_rate != 0 && slide_x == SW) {
					StretchBlt(hdc, 0, 0, SW, SH, hMemDC,
						SW * stretch_rate, SH * stretch_rate, SW * (1 - stretch_rate) - (SW * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), SRCCOPY);
				}
				else {
					BitBlt(hdc, SW - slide_x, 0, slide_x, SH, hMemDC, 0, 0, raster);
					BitBlt(hdc, 0, 0, SW - slide_x, SH, hMemDC, slide_x, 0, raster);
				}*/

				StretchBlt(hdc, SW - slide_x, 0, slide_x, SH, hMemDC,
					slide_x * stretch_rate, SH * stretch_rate, slide_x * (1 - stretch_rate) - (slide_x * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), raster);

				StretchBlt(hdc, 0, 0, SW - slide_x, SH, hMemDC,
					slide_x * (1 - stretch_rate), SH * stretch_rate, (SW - slide_x) * (1 - stretch_rate) + ((slide_x - SW) * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), raster);
			}
			else if (full_screen == true) {
				float slide_sx = slide_x * 0.8;

				/*StretchBlt(hdc, FW - slide_x, 0, slide_x, FH, hMemDC,
				0, 0, slide_sx, SH, raster);

				StretchBlt(hdc, 0, 0, FW - slide_x, FH, hMemDC,
				slide_sx, 0, SW - slide_sx, SH, raster);*/
				
				StretchBlt(hdc, FW - slide_x, 0, slide_x, FH, hMemDC,
					slide_sx * stretch_rate, SH * stretch_rate, slide_sx * (1 - stretch_rate) - (slide_sx * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), raster);

				StretchBlt(hdc, 0, 0, FW - slide_x, FH, hMemDC,
					slide_sx * (1 - stretch_rate), SH * stretch_rate, (SW - slide_sx) * (1 - stretch_rate) + ((slide_sx - SW) * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), raster);

			}
		}
		else if (screen_div == 2) {
			float slide_sx = slide_x * 0.8;

			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < 2; ++j) {
					if (idx_x == i + 1 && idx_y == j + 1) {
						/*StretchBlt(hdc, (FW - slide_x) / 2 + (FW / 2 * i), (FH / 2 * j), slide_x / 2, FH / 2, hMemDC,
							0, 0, slide_sx, SH, div_2_raster[i][j]);
						StretchBlt(hdc, (FW / 2 * i), (FH / 2 * j), (FW - slide_x) / 2, FH / 2, hMemDC,
							slide_sx, 0, SW - slide_sx, SH, div_2_raster[i][j]);*/

						StretchBlt(hdc, (FW - slide_x) / 2 + (FW / 2 * i), (FH / 2 * j), slide_x / 2, FH / 2, hMemDC,
							slide_sx * stretch_rate, SH * stretch_rate, slide_sx * (1 - stretch_rate) - (slide_sx * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), div_2_raster[i][j]);
						StretchBlt(hdc, (FW / 2 * i), (FH / 2 * j), (FW - slide_x) / 2, FH / 2, hMemDC,
							slide_sx * (1 - stretch_rate), SH * stretch_rate, (SW - slide_sx) * (1 - stretch_rate) + ((slide_sx - SW) * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), div_2_raster[i][j]);
					}
					else {
						StretchBlt(hdc, (FW / 2 * i), (FH / 2 * j), FW / 2, FH / 2, hMemDC, 0, 0, SW, SH, div_2_raster[i][j]);
					}
				}
			}
			
			RECT rt{ FW / 2 * (idx_x - 1),FH / 2 * (idx_y - 1),FW / 2 * (idx_x - 1) + FW / 2,FH / 2 * (idx_y - 1) + FH / 2 };
			FrameRect(hdc, &rt, (HBRUSH)CreateSolidBrush(RGB(255, 0, 0)));
		}
		else if (screen_div == 3) {
			float slide_sx = slide_x * 0.8;

			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (idx_x == i + 1 && idx_y == j + 1) {
						/*StretchBlt(hdc, (FW - slide_x) / 3 + (FW / 3 * i), (FH / 3 * j), slide_x / 3, FH / 3, hMemDC,
							0, 0, slide_sx, SH, div_3_raster[i][j]);
						StretchBlt(hdc, (FW / 3 * i), (FH / 3 * j), (FW - slide_x) / 3, FH / 3, hMemDC,
							slide_sx, 0, SW - slide_sx, SH, div_3_raster[i][j]);*/

						StretchBlt(hdc, (FW - slide_x) / 3 + (FW / 3 * i), (FH / 3 * j), slide_x / 3, FH / 3, hMemDC,
							slide_sx * stretch_rate, SH * stretch_rate, slide_sx * (1 - stretch_rate) - (slide_sx * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), div_3_raster[i][j]);
						StretchBlt(hdc, (FW / 3 * i), (FH / 3 * j), (FW - slide_x) / 3, FH / 3, hMemDC,
							slide_sx * (1 - stretch_rate), SH * stretch_rate, (SW - slide_sx) * (1 - stretch_rate) + ((slide_sx - SW) * stretch_rate), SH * (1 - stretch_rate) - (SH * stretch_rate), div_3_raster[i][j]);
					}
					else {
						StretchBlt(hdc, (FW / 3 * i), (FH / 3 * j), FW / 3, FH / 3, hMemDC, 0, 0, SW, SH, div_3_raster[i][j]);
					}
				}
			}

			RECT rt{ FW / 3 * (idx_x - 1),FH / 3 * (idx_y - 1),FW / 3 * (idx_x - 1) + FW / 3,FH / 3 * (idx_y - 1) + FH / 3 };
			FrameRect(hdc, &rt, (HBRUSH)CreateSolidBrush(RGB(255, 0, 0)));
		}

		DeleteDC(hMemDC);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam) 
		{
		case 'r':
		case 'R':
			if (raster == SRCCOPY)
				raster = NOTSRCCOPY;
			else
				raster = SRCCOPY;
			break;

		case 'a':
		case 'A':
			if (screen_div == 1) {
				if (full_screen == true) {
					full_screen = false;
					slide_x = slide_x * 0.8;	// -> W/FW 
				}
				else {
					full_screen = true;
					slide_x = slide_x * 1.25;	// -> FW/W
				}
			}
			break;

		case '1':
			screen_div = 1;
			break;

		case '2':
			idx_x = 0;
			idx_y = 0;
			if (full_screen == false) {
				full_screen = true;
				slide_x = slide_x * 1.25;
			}
			screen_div = 2;
			break;

		case '3':
			idx_x = 0;
			idx_y = 0;
			if (full_screen == false) {
				full_screen = true;
				slide_x = slide_x * 1.25;
			}
			screen_div = 3;
			break;

		case '+':
			if (stretch_rate < 0.40)
				stretch_rate += 0.05;
			else
				stretch_rate = 0.40;
			break;

		case '-':
			if (stretch_rate > -1.0)
				stretch_rate -= 0.05;
			else
				stretch_rate = -1.0;
			break;

		case 'q':
		case 'Q':
			DestroyWindow(hwnd);
			break;

		}

		InvalidateRect(hwnd, NULL, true);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) {
			slide_x -= 10;
			if (slide_x <= 0) {
				if (full_screen == true)
					slide_x = FW;
				else
					slide_x = SW;
			}
		}
		else if (wParam == VK_LEFT) {
			slide_x += 10;
			if (full_screen == true) {
				if (slide_x >= FW)
					slide_x = 0;
			}
			else {
				if (slide_x >= SW)
					slide_x = 0;
			}
		}

		InvalidateRect(hwnd, NULL, true);
		break;
	
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (screen_div == 2) {
			idx_x = mx / (FW / 2) + 1;
			idx_y = my / (FH / 2) + 1;
		}
		else if (screen_div == 3) {
			idx_x = mx / (FW / 3) + 1;
			idx_y = my / (FH / 3) + 1;
		}

		InvalidateRect(hwnd, NULL, true);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}