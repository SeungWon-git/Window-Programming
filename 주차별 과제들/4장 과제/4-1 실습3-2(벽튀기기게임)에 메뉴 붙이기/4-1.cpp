#include <windows.h>
#include <tchar.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 4-1";

typedef struct Block {
	int x, y;
	int life;
	int r, g, b;
}block;

void Ballmove(HWND hwnd, int* bx, int* by, int ballspeed, int* vx, int* vy, int* barx, block b[40], int* eb)
{
	//vx: 0-왼쪽으로 움직이는중 ,1-오른쪽으로 움직이는중
	//vy: 0-위쪽으로 움직이는중 ,1-아래쪽으로 움직이는중

	//벽을 만날때
	if (*bx - 15 <= 0)
		* vx = 1;
	else if (*bx + 15 >= 765)
		* vx = 0;
	if (*by - 15 <= 0)
		* vy = 1;
	else if (*by + 15 >= 540)
		* vy = 0;

	//바를 만날때
	if (*bx >= *barx && *bx <= *barx + 150 && *by + 15 >= 500) //바 위쪽 충돌
		* vy = 0;
	/*if (*bx - 15 <= *barx + 150 && *by >= 500) //바 오른쪽에 충돌
			* vx = 1;
		else if (*bx + 15 >= *barx && *by >= 500) //바 왼쪽에 충돌
			* vx = 0;*/

	//블록과 만날때
	for (int i = 0; i < 40; i++) {
		//블록 위쪽 충돌
		if (b[i].life != 0 && *bx >= b[i].x && *bx <= b[i].x + 70 && *by + 15 >= b[i].y && *by + 15 <= b[i].y + 30) {
			if (*vy == 0) {
				*by += 30;
				*vy = 1;
			}
			else
				*vy = 0;
			b[i].life--;
			if (b[i].life == 0)
				* eb += 1;
			b[i].y += 15;
			if (i >= 0 && i <= 9)
				for (int j = 0; j < 10; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
			else if (i >= 10 && i <= 19)
				for (int j = 10; j < 20; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
			else if (i >= 20 && i <= 29)
				for (int j = 20; j < 30; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
			else if (i >= 30 && i <= 39)
				for (int j = 30; j < 40; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
		}
		//블록 아래쪽 충돌
		else if (b[i].life != 0 && *bx >= b[i].x && *bx <= b[i].x + 70 && *by - 15 <= b[i].y + 30 && *by - 15 >= b[i].y) {
			if (*vy == 1)
				* vy = 0;
			else
			{
				*by += 30;
				*vy = 1;
			}
			b[i].life--;
			if (b[i].life == 0)
				* eb += 1;
			b[i].y += 15;
			if (i >= 0 && i <= 9)
				for (int j = 0; j < 10; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
			else if (i >= 10 && i <= 19)
				for (int j = 10; j < 20; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
			else if (i >= 20 && i <= 29)
				for (int j = 20; j < 30; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
			else if (i >= 30 && i <= 39)
				for (int j = 30; j < 40; j++) {
					b[j].y += 15;
					b[j].r -= 10;
					b[j].g -= 10;
					b[j].b += 10;
				}
		}

	}

	if (*vx == 0 && *vy == 0)
	{
		*bx -= ballspeed;
		*by -= ballspeed;
	}
	else if (*vx == 1 && *vy == 0)
	{
		*bx += ballspeed;
		*by -= ballspeed;
	}
	else if (*vx == 0 && *vy == 1)
	{
		*bx -= ballspeed;
		*by += ballspeed;
	}
	else if (*vx == 1 && *vy == 1)
	{
		*bx += ballspeed;
		*by += ballspeed;
	}


}

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

	HBRUSH hbrush, oldbrush; // HBRUSH ballbrush, balloldbrush;

	HBRUSH boardbrush, boardoldbrush;

	static int R, G, B;

	static TCHAR lpOut[1024];

	static block b[40];

	static int blockline;

	static int blockmove;

	static int eb;

	static bool start;	//움직이기 시작=1, 정지=0

	static bool selection;

	static bool reset;

	static bool gameover;

	static bool ellipse;

	static bool rectangle;

	static int mx, my;

	static int bx, by;

	static int ballx, bally;
	static int vx;	//0-왼쪽으로 움직이는중 ,1-오른쪽으로 움직이는중
	static int vy;	//0-위쪽으로 움직이는중 ,1-아래쪽으로 움직이는중

	static int ballspeed;

	switch (iMsg)
	{
	case WM_CREATE:

		for (int i = 0; i < 10; i++)
		{
			b[i].x = i * 70 + 30;
			b[i].y = 50;
			b[i].life = 2;
			b[i].r = 255;
			b[i].g = 255;
			b[i].b = 0;
		}
		for (int i = 10; i < 20; i++)
		{
			b[i].x = (i - 10) * 70 + 30;
			b[i].y = 80;
			b[i].life = 2;
			b[i].r = 255;
			b[i].g = 255;
			b[i].b = 0;
		}
		for (int i = 20; i < 30; i++)
		{
			b[i].x = (i - 20) * 70 + 30;
			b[i].y = 110;
			b[i].life = 2;
			b[i].r = 255;
			b[i].g = 255;
			b[i].b = 0;
		}
		for (int i = 30; i < 40; i++)
		{
			b[i].x = (i - 30) * 70 + 30;
			b[i].y = 140;
			b[i].life = 2;
			b[i].r = 255;
			b[i].g = 255;
			b[i].b = 0;
		}
		bx = 300; by = 500;
		blockmove = 1;
		eb = 0;
		ballx = 375; bally = 485; //원의 중심점
		vx = 0; vy = 0;
		ballspeed = 1;
		selection = 0;
		start = 0;
		R = 255; G = 255; B = 0;
		ellipse = 1; rectangle = 0;
		blockline = 2;
		for (int i = 20; i < 40; i++)
			b[i].life = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetTimer(hwnd, 1, 10, NULL);

		if (bally + 15 >= 540)
			gameover = 1;

		if (reset == 1)
		{
			reset = 0;
			MessageBox(hwnd, L"리셋되었습니다.", L"Reset", NULL);
		}

		if (gameover == 1)
		{
			gameover = 0;
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 30; i < 40; i++)
			{
				b[i].x = (i - 30) * 70 + 30;
				b[i].y = 140;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			R = 255; G = 255; B = 0;
			ellipse = 1; rectangle = 0;
			blockline = 2;
			for (int i = 20; i < 40; i++)
				b[i].life = 0;
			MessageBox(hwnd, L"패배", L"GameOver", NULL);
		}
		else if (eb == blockline*10)
		{
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 30; i < 40; i++)
			{
				b[i].x = (i - 30) * 70 + 30;
				b[i].y = 140;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			R = 255; G = 255; B = 0;
			ellipse = 1; rectangle = 0;
			blockline = 2;
			for (int i = 20; i < 40; i++)
				b[i].life = 0;
			MessageBox(hwnd, L"승리!", L"GameOver", NULL);
		}

		boardbrush = CreateSolidBrush(RGB(R, G, B));
		boardoldbrush = (HBRUSH)SelectObject(hdc, boardbrush);

		Rectangle(hdc, bx, by, bx + 150, by + 20);

		hbrush = CreateSolidBrush(RGB(b[0].r, b[0].g, b[0].b));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		for (int i = 0; i < 10; i++)
		{
			if (b[i].life != 0) {
				Rectangle(hdc, b[i].x, b[i].y, b[i].x + 70, b[i].y + 30);
			}
		}
		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);

		hbrush = CreateSolidBrush(RGB(b[10].r, b[10].g, b[10].b));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		for (int i = 10; i < 20; i++)
		{
			if (b[i].life != 0) {
				Rectangle(hdc, b[i].x, b[i].y, b[i].x + 70, b[i].y + 30);
			}
		}
		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);

		hbrush = CreateSolidBrush(RGB(b[20].r, b[20].g, b[20].b));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		for (int i = 20; i < 30; i++)
		{
			if (b[i].life != 0) {
				Rectangle(hdc, b[i].x, b[i].y, b[i].x + 70, b[i].y + 30);
			}
		}
		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);

		hbrush = CreateSolidBrush(RGB(b[30].r, b[30].g, b[30].b));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		for (int i = 30; i < 40; i++)
		{
			if (b[i].life != 0) {
				Rectangle(hdc, b[i].x, b[i].y, b[i].x + 70, b[i].y + 30);
			}
		}
		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);

		hbrush = CreateSolidBrush(RGB(0, 0, 255));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

		if (ellipse == 1)
			Ellipse(hdc, ballx - 15, bally - 15, ballx + 15, bally + 15);
		else
			Rectangle(hdc, ballx - 15, bally - 15, ballx + 15, bally + 15);

		wsprintf(lpOut, L"block left:%d", blockline * 10 - eb);
		TextOut(hdc, 10, 10, lpOut, lstrlen(lpOut));

		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);
		SelectObject(hdc, boardoldbrush);
		DeleteObject(boardbrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			start = 1;
			break;
		case ID_RESET:
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 30; i < 40; i++)
			{
				b[i].x = (i - 30) * 70 + 30;
				b[i].y = 140;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			R = 255; G = 255; B = 0;
			ellipse = 1; rectangle = 0;
			blockline = 2;
			for (int i = 20; i < 40; i++)
				b[i].life = 0;
			reset = 1;
			break;
		case ID_END:
			DestroyWindow(hwnd);
			break;
		case ID_FAST:
			ballspeed = 8;
			break;
		case ID_MEDIUM:
			ballspeed = 4;
			break;
		case ID_SLOW:
			ballspeed = 1;
			break;
		case ID_CYAN:
			R = 0; G = 255; B = 255;
			break;
		case ID_MAGENTA:
			R = 255; G = 0; B = 255;
			break;
		case ID_YELLOW:
			R = 255; G = 255; B = 0;
			break;
		case ID_ELLIPSE:
			ellipse = 1; rectangle = 0;
			break;
		case ID_RECTANGLE:
			ellipse = 0; rectangle = 1;
			break;

		case ID_SECOND:
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 30; i < 40; i++)
			{
				b[i].x = (i - 30) * 70 + 30;
				b[i].y = 140;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			R = 255; G = 255; B = 0;
			ellipse = 1; rectangle = 0;
			blockline = 2;
			for (int i = 20; i < 40; i++)
				b[i].life = 0;
			reset = 1;
			break;
		case ID_THIRD:
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 30; i < 40; i++)
			{
				b[i].x = (i - 30) * 70 + 30;
				b[i].y = 140;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			R = 255; G = 255; B = 0;
			ellipse = 1; rectangle = 0;
			blockline = 3;
			for (int i = 30; i < 40; i++)
				b[i].life = 0;
			reset = 1;
			break;
			
		case ID_FOURTH:
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			for (int i = 30; i < 40; i++)
			{
				b[i].x = (i - 30) * 70 + 30;
				b[i].y = 140;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			R = 255; G = 255; B = 0;
			ellipse = 1; rectangle = 0;
			blockline = 4;
			reset = 1;
			break;

		case ID_ON:
			blockmove = 1;
			break;
		case ID_OFF:
			blockmove = 0;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		if (start == 1 && mx >= bx && mx <= bx + 150 && my >= 500 && my <= 520)
			selection = 1;
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (selection == 1) {
			bx = mx - 75;
			if (bx >= 615)
				bx = 615;
			else if (bx <= 0)
				bx = 0;
		}
		break;

	case WM_LBUTTONUP:
		selection = 0;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:
			if (start == 1 && b[9].x + 70 >= 765 && blockmove == 1)
				blockmove = 2;
			else if (start == 1 && b[0].x <= 0 && blockmove == 2)
				blockmove = 1;

			if (start == 1) {
				for (int i = 0; i < 40; i++)
				{
					if (blockmove == 1)
						b[i].x += 1;
					else if (blockmove == 2)
						b[i].x -= 1;
				}
				Ballmove(hwnd, &ballx, &bally, ballspeed, &vx, &vy, &bx, b, &eb);
			}
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}