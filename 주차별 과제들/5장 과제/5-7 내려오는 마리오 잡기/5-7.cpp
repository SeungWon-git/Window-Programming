#include <windows.h>
#include <tchar.h>
#include <random>
#include "resource.h"

#pragma comment (lib, "msimg32.lib")

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 5-7";

#define MARIO_CNT 30

typedef struct Block {
	int x, y;
	bool in_box;
	int speed;
	int sprite;
}block;

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 520, 550, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;

	static HBITMAP hBit_bg, hBit1, oldBit1, oldBit2;

	HBRUSH hBrush;

	static HBITMAP hBit_F1, hBit_F2, hBit_F3, hBit_F4, hBit_S1, hBit_S2, hBit_S3, hBit_S4, hBit_S5, hBit_S6, hBit_S7, hBit_S8;

	static block mario[MARIO_CNT];

	static bool start;	//움직이기 시작=1, 정지=0

	static bool drag;

	static bool move;

	static int mx, my;
	static int startX, startY, endX, endY, oldX, oldY;
	static int maxX, maxY, minX, minY;
	static int move_beginX, move_beginY, move_startX, move_startY, move_endX, move_endY;

	static int move_X, move_Y;

	static int overall_speed;

	static int ani_time;

	//static TCHAR lpOut[1024];

	switch (iMsg)
	{
	case WM_CREATE:
		// 객체 - x위치, 색깔, 하강속도, 시작 애니메이션 랜덤 생성
		for (int i = 0; i < MARIO_CNT; ++i) {
			mario[i].x = rand() % 500;
			mario[i].y = 0;
			mario[i].in_box = 0;
			mario[i].speed = rand() % 5 + 1;
			mario[i].sprite = rand() % 8 + 1;
		}

		start = 0;
		drag = 0;
		move = 0;
		mx = 0; my = 0;

		overall_speed = 2;

		ani_time = 0;

		hBit_bg = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));

		hBit_F1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		hBit_F2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
		hBit_F3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		hBit_F4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));

		hBit_S1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit_S2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit_S3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hBit_S4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBit_S5 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		hBit_S6 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		hBit_S7 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7)); 
		hBit_S8 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));

		SetTimer(hwnd, 1, 20, NULL);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		BitBlt(hdc, 0, 0, 505, 510, mem1dc, 0, 0, SRCCOPY);
		
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'p':
		case 'P':
			start = !start;
			break;

		case 'd':
		case 'D':
			for (int i = 0; i < MARIO_CNT; ++i) {
				mario[i].in_box = 0;
			}

			startX = 0; startY = 0;
			endX = 0; endY = 0;
			oldX = 0; oldY = 0;

			minX = 0; minY = 0;
			maxX = 0; maxY = 0;

			break;

		case 'q':
		case 'Q':
			DestroyWindow(hwnd);
			break;

		case '+':
			++overall_speed;
			if (overall_speed >= 5)
				overall_speed = 5;
			break;

		case '-':
			--overall_speed;
			if (overall_speed <= 0)
				overall_speed = 1;
			break;

		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (start == 1 && mx >= 0 && mx <= 500 && my >= 0 && my <= 500) {
			oldX = startX = mx;
			oldY = startY = my;

			drag = 1;
		}

		break;

	case WM_LBUTTONUP:
		if (drag == 1) {
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			oldX = endX;
			oldY = endY;
		}

		drag = 0;

		//후에 떨어지는 원들과 비교를 위해
		minX = endX < startX ? endX : startX;
		minY = endY < startY ? endY : startY;

		maxX = endX > startX ? endX : startX;
		maxY = endY > startY ? endY : startY;

		break;

	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (mx >= minX && mx <= maxX && my >= minY && my <= maxY) {
			move_beginX = mx;
			move_beginY = my;

			move = 1;

			move_startX = startX;
			move_startY = startY;

			move_X = 0; move_Y = 0;
		}
		break;

	case WM_RBUTTONUP:
		if (move == 1) {
			move = 0;

			startX = startX + move_X;
			startY = startY + move_Y;
			endX = endX + move_X; 
			endY = endY + move_Y;

			//후에 떨어지는 원들과 비교를 위해
			maxX += move_X;
			maxY += move_Y;

			minX += move_X;
			minY += move_Y;

			for (int i = 0; i < MARIO_CNT; ++i) {
				mario[i].in_box = 0;
				mario[i].sprite = 1;
			}
		}
		break;

	case WM_MOUSEMOVE:
		if (drag == 1) {
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			oldX = endX; 
			oldY = endY;
		}

		if (move == 1) {
			move_endX = LOWORD(lParam);
			move_endY = HIWORD(lParam);

			move_X = move_endX - move_beginX;
			move_Y = move_endY - move_beginY;

			move_startX = startX + move_X;
			move_startY = startY + move_Y;
			move_endX = endX + move_X;
			move_endY = endY + move_Y;
			
			oldX = move_endX; 
			oldY = move_endY;
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			
			if (start == 1) {
				ani_time++;


				// 충돌체크
				for (int i = 0; i < MARIO_CNT; ++i) {
					mario[i].y += overall_speed + mario[i].speed;

					if (drag == 0 && move != 1 && mario[i].in_box == 0 && mario[i].y >= minY && mario[i].y <= maxY && mario[i].x >= minX && mario[i].x <= maxX) {
						mario[i].in_box = 1;
						mario[i].sprite = 1;
					}
					else if (drag == 0 && mario[i].in_box == 1 && mario[i].y >= maxY && mario[i].x >= minX && mario[i].x <= maxX) {
						mario[i].y = maxY;
					}
					else if (drag == 1 && mario[i].in_box == 1) {
						mario[i].in_box = 0;
					}
					else if (move == 1 && mario[i].in_box == 1) {
						mario[i].in_box = 0;
					}

					if (mario[i].y >= 600) {
						mario[i].y = 0;
						mario[i].speed = rand() % 5 + 1;
					}
				}


				// 애니메이션 & 스프라이트 지정
				if (ani_time % 5 == 0) {
					for (int i = 0; i < MARIO_CNT; ++i) {
						if (mario[i].in_box != 1) {
							mario[i].sprite++;
							if (mario[i].sprite > 4)
								mario[i].sprite = 1;
						}
						else {
							mario[i].sprite++;
							if (mario[i].sprite > 8)
								mario[i].sprite = 1;
						}
					}
				}

			}

			// 더블 버퍼링
			hdc = GetDC(hwnd);

			mem1dc = CreateCompatibleDC(hdc);

			mem2dc = CreateCompatibleDC(mem1dc);

			hBit1 = CreateCompatibleBitmap(hdc, 505, 510);

			// 배경
			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
			oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_bg);

			BitBlt(mem1dc, 0, 0, 505, 510, mem2dc, 0, 0, SRCCOPY);

			// 캐릭터
			for (int i = 0; i < MARIO_CNT; ++i) {
				if (mario[i].in_box != 1) {
					switch (mario[i].sprite) {
					case 1:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_F1);
						break;
					case 2:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_F2);
						break;
					case 3:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_F3);
						break;
					case 4:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_F4);
						break;
					}
				}
				else {
					switch (mario[i].sprite) {
					case 1:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S1);
						break;
					case 2:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S2);
						break;
					case 3:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S3);
						break;
					case 4:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S4);
						break;
					case 5:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S5);
						break;
					case 6:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S6);
						break;
					case 7:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S7);
						break;
					case 8:
						oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_S8);
						break;
					}
				}

				TransparentBlt(mem1dc, mario[i].x - 23, mario[i].y - 23, 47, 47, mem2dc, 0, 0, 47, 47, RGB(0, 84, 84));
			}

			// 사각형
			if (move == 0) {
				RECT rt;
				if (startX < oldX) {
					rt.left = startX;
					rt.right = oldX;
				}
				else {
					rt.left = oldX;
					rt.right = startX;
				}
				if (startY < oldY) {
					rt.top = startY;
					rt.bottom = oldY;
				}
				else {
					rt.top = oldY;
					rt.bottom = startY;
				}

				hBrush = CreateSolidBrush(RGB(0, 0, 255));

				FrameRect(mem1dc, &rt, hBrush);

				DeleteObject(hBrush);
			}
			else {
				RECT rt;
				if (move_startX < oldX) {
					rt.left = move_startX;
					rt.right = oldX;
				}
				else {
					rt.left = oldX;
					rt.right = move_startX;
				}
				if (move_startY < oldY) {
					rt.top = move_startY;
					rt.bottom = oldY;
				}
				else {
					rt.top = oldY;
					rt.bottom = move_startY;
				}

				hBrush = CreateSolidBrush(RGB(255, 0, 0));

				FrameRect(mem1dc, &rt, hBrush);

				DeleteObject(hBrush);
			}

			SelectObject(mem2dc, oldBit2);
			DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1);
			DeleteDC(mem1dc);

			ReleaseDC(hwnd, hdc);

			InvalidateRect(hwnd, NULL, FALSE);
			break;

		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}