#include <windows.h>
#include <tchar.h>
#include "resource.h"

#pragma comment (lib, "msimg32.lib")

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 5-4";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

#define p_speed 10
#define pp_speed 3
#define twin_gap 50

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 520, 545, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC mem1dc, mem2dc;
	static HBITMAP hBit1, hBit2, hBit3, hBit4, hBit_bg;
	static HBITMAP oldBit1, oldBit2;
	PAINTSTRUCT ps;

	static int move_sprite;	//2~4 - 왼쪽 방향, 5~7 - 오른쪽 방향
	static bool dir;	//바라보는 방향 0 - 왼쪽, 1 - 오른쪽
	static int jump;
	static bool redc, ampl;
	static int size;
	static int twin;
	static bool pingpong;
	static bool pp_x[3], pp_y[3];

	static int new_animation;

	static int p_x[3], p_y[3];

	static int mx, my;

	static bool flag;

	static bool charNum[3];

	switch (iMsg)
	{
	case WM_CREATE:

		SetTimer(hwnd, 1, 50, NULL);

		hBit_bg = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));

		move_sprite = 5;
		dir = 1;
		jump = 0;
		redc = false;	ampl = false;
		size = 0;
		twin = 0;
		pingpong = false;

		for (int i = 0; i < 3; i++) {
			pp_x[i] = true;
			pp_y[i] = true;
		}
		new_animation = 0;

		p_x[0] = 180;
		p_y[0] = 160;

		mx = 0;
		my = 0;

		charNum[0] = true;
		charNum[1] = false;
		charNum[2] = false;

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

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			dir = 1;

			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					p_x[i] += p_speed;
					if (p_x[i] > 475)
						p_x[i] = -20;
				}
			}
			break;

		case VK_LEFT:
			dir = 0;

			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					p_x[i] -= p_speed;
					if (p_x[i] < -20)
						p_x[i] = 475;
				}
			}
			break;

		case VK_UP:
			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					p_y[i] -= p_speed;
					if (p_y[i] < -20)
						p_y[i] = 480;
				}
			}
			break;

		case VK_DOWN:
			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					p_y[i] += p_speed;
					if (p_y[i] > 480)
						p_y[i] = -20;
				}
			}
			break;

		case 'J':
			if (jump == 0)
				jump = 10;
			break;

		case 'E':
			if (size == 0) {
				ampl = true;
			}
			break;

		case 'S':
			if (size == 0) {
				redc = true;
			}
			break;

		case 'T':
			twin++;
			if (twin > 5)
				twin = 0;
			break;

		case 'A':
			pingpong = !pingpong;
			break;

		case 'Q':
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		flag = false;

		for (int i = 0; i < 3; i++) {
			if (charNum[i]) {
				for (int n = 0; n <= twin; n++) {
					int paintX = p_x[i] + twin_gap * n;

					if (paintX < -20)
						paintX = 475 - (paintX + 20);
					if (paintX > 475)
						paintX = -20 + (paintX - 475);

					if (mx > paintX && mx < paintX + 60 + size && my > p_y[i] && my < p_y[i] + 60 + size && new_animation == 0) {
						flag = true;
						new_animation = 20;
					}
				}
			}
		}

		if (!flag) {
			if (!charNum[1]) {
				p_x[1] = mx - 30;
				p_y[1] = my - 35;
				charNum[1] = true;
			}
			else if (!charNum[2]) {
				p_x[2] = mx - 30;
				p_y[2] = my - 35;
				charNum[2] = true;
			}
		}

		break;

	case WM_TIMER:
		//움직임 처리
		if (dir == 0) {
			++move_sprite;
			if (move_sprite >= 5)
				move_sprite = 2;
		}
		else if (dir == 1) {
			++move_sprite;
			if (move_sprite >= 8)
				move_sprite = 5;
		}

		//점프처리
		if (jump > 5) {	//상승
			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					p_y[i] -= p_speed;
					if (p_y[i] < -20)
						p_y[i] = 480;
				}
			}

			--jump;
		}
		else if (jump > 0) {	//하강
			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					p_y[i] += p_speed;
					if (p_y[i] > 480)
						p_y[i] = -20;
				}
			}

			--jump;
		}

		//크기변환 처리
		if (ampl == true) {
			if (size < 40)
				size += 4;
			else
				ampl = false;
		}
		else if (redc == true) {
			if (size > -40)
				size -= 4;
			else
				redc = false;
		}

		//크기 원상복구
		if (size > 0 && ampl == false && redc == false)
			size -= 4;
		else if (size < 0 && ampl == false && redc == false)
			size += 4;

		//팅기기 처리
		if (pingpong) {
			for (int i = 0; i < 3; i++) {
				if (charNum[i]) {
					if (pp_x[i])
						p_x[i] += pp_speed;
					else
						p_x[i] -= pp_speed;

					if (pp_y[i])
						p_y[i] += pp_speed;
					else
						p_y[i] -= pp_speed;

					if (p_x[i] < 0)
						pp_x[i] = !pp_x[i];
					if (p_x[i] > 460)
						pp_x[i] = !pp_x[i];

					if (p_y[i] < 0)
						pp_y[i] = !pp_y[i];
					if (p_y[i] > 470)
						pp_y[i] = !pp_y[i];
				}
			}
		}


		hdc = GetDC(hwnd);

		//걷기 스프라이트
		if (new_animation == 0) {
			if (move_sprite == 2) {
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));

				if (charNum[1])
					hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
				if (charNum[2])
					hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
			}
			else if (move_sprite == 3) {
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

				if (charNum[1])
					hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
				if (charNum[2])
					hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));
			}
			else if (move_sprite == 4) {
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));

				if (charNum[1])
					hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
				if (charNum[2])
					hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
			}
			else if (move_sprite == 5) {
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));

				if (charNum[1])
					hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
				if (charNum[2])
					hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
			}
			else if (move_sprite == 6) {
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));

				if (charNum[1])
					hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
				if (charNum[2])
					hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));
			}
			else if (move_sprite == 7) {
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));

				if (charNum[1])
					hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
				if (charNum[2])
					hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));
			}
		}
		//뛰기 스프라이트
		else {
			if (new_animation > 18) {
				if (dir == 0) {	//왼쪽
					hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));

					if (charNum[1])
						hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
					if (charNum[2])
						hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
				}
				else {     //오른쪽
					hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));

					if (charNum[1])
						hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
					if (charNum[2])
						hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
				}
			}
			else if (new_animation > 16) {
				if (dir == 0) {//왼쪽
					hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));

					if (charNum[1])
						hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
					if (charNum[2])
						hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
				}
				else {          //오른쪽
					hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));

					if (charNum[1])
						hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
					if (charNum[2])
						hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));
				}
			}
			else if (new_animation > 0) {
				if (dir == 0) {	//왼쪽
					if (new_animation % 2 == 1) {
						hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));

						if (charNum[1])
							hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
						if (charNum[2])
							hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
					}
					else {
						hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));

						if (charNum[1])
							hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
						if (charNum[2])
							hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));
					}
				}
				else {           //오른쪽
					if (new_animation % 2 == 1) {
						hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));

						if (charNum[1])
							hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
						if (charNum[2])
							hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));
					}
					else {
						hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));

						if (charNum[1])
							hBit3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
						if (charNum[2])
							hBit4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
					}
				}
			}

			if (new_animation != 0) {
				--new_animation;

				//뛰기 처리
				for (int i = 0; i < 3; i++) {
					if (charNum[i]) {
						if (new_animation < 16) {
							if (dir == 0) {
								p_x[i] -= p_speed * 2;
								if (p_x[i] < -20)
									p_x[i] = 475;
							}
							else {
								p_x[i] += p_speed * 2;
								if (p_x[i] > 475)
									p_x[i] = -20;
							}
						}
					}

				}

			}
		}

		//더블 버퍼링 처리
		if (hBit1 == NULL)
			hBit1 = CreateCompatibleBitmap(hdc, 505, 510);

		mem1dc = CreateCompatibleDC(hdc);

		mem2dc = CreateCompatibleDC(mem1dc);

		//배경
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_bg);

		BitBlt(mem1dc, 0, 0, 505, 510, mem2dc, 0, 0, SRCCOPY);

		//캐릭터
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);

		for (int n = 0; n <= twin; n++) {
			int paintX = p_x[0] + twin_gap * n;

			if (paintX < -20)
				paintX = 475 - (paintX + 20);
			if (paintX > 475)
				paintX = -20 + (paintX - 475);

			TransparentBlt(mem1dc, paintX, p_y[0], 60 + size, 60 + size, mem2dc, 0, 0, 47, 47, RGB(0, 84, 84));
		}

		if (charNum[1]) {
			oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit3);

			for (int n = 0; n <= twin; n++) {
				int paintX = p_x[1] + twin_gap * n;

				if (paintX < -20)
					paintX = 475 - (paintX + 20);
				if (paintX > 475)
					paintX = -20 + (paintX - 475);

				TransparentBlt(mem1dc, paintX, p_y[1], 60 + size, 60 + size, mem2dc, 0, 0, 47, 47, RGB(0, 84, 84));
			}
		}

		if (charNum[2]) {
			oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit4);

			for (int n = 0; n <= twin; n++) {
				int paintX = p_x[2] + twin_gap * n;

				if (paintX < -20)
					paintX = 475 - (paintX + 20);
				if (paintX > 475)
					paintX = -20 + (paintX - 475);

				TransparentBlt(mem1dc, paintX, p_y[2], 60 + size, 60 + size, mem2dc, 0, 0, 47, 47, RGB(0, 52, 52));
			}
		}

		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);

		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, NULL, false);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}