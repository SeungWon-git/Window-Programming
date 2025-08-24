#include <windows.h>
#include <tchar.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 3-2";

typedef struct Block {
	int x, y;
	int life;
	int minus_size;
	int r, g, b;
}block;

void Ballmove(HWND hwnd, int* bx, int* by, int ballspeed, int* vx, int* vy, int* barx, block b[30], int* eb, int* eb_)
{
	//vx: 0-왼쪽으로 움직이는중 ,1-오른쪽으로 움직이는중
	//vy: 0-위쪽으로 움직이는중 ,1-아래쪽으로 움직이는중

	//벽을 만날때
	if (*bx - 15 <= 0)
		*vx = 1;
	else if (*bx + 15 >= 765)
		*vx = 0;
	if (*by - 15 <= 0)
		*vy = 1;
	else if (*by + 15 >= 540)
		*vy = 0;


	//바를 만날때
	if (*bx >= *barx && *bx <= *barx + 150 && *by + 15 >= 500) //바 위쪽 충돌
		*vy = 0;

	*eb = 0;
	*eb_ = 0;


	//블록과 만낫을때
	for (int i = 0; i < 30; i++) {
		//블록 위쪽 충돌
		if (b[i].life != 0 && *bx >= b[i].x && *bx <= b[i].x + 70 && *by + 15 >= b[i].y && *by + 15 <= b[i].y + 30) {
			if (*vy == 0) {
				*vy = 1;
			}
			else
				*vy = 0;

			b[i].minus_size -= 10;
			b[i].life--;
			b[i].r -= 150;
			b[i].g -= 150;
			b[i].b += 150;
		}
		//블록 아래쪽 충돌
		else if (b[i].life != 0 && *bx >= b[i].x && *bx <= b[i].x + 70 && *by - 15 <= b[i].y + 30 && *by - 15 >= b[i].y) {
			if (*vy == 1)
				*vy = 0;
			else
			{
				*vy = 1;
			}

			b[i].minus_size -= 10;
			b[i].life--;
			b[i].r -= 150;
			b[i].g -= 150;
			b[i].b += 150;
		}
		//블록 왼쪽 충돌
		else if (b[i].life != 0 && *bx + 15 >= b[i].x && *bx + 15 <= b[i].x + 70 && *by >= b[i].y && *by <= b[i].y + 30) {
			if (*vx == 0) {
				*vx = 1;
			}
			else
				*vx = 0;

			b[i].minus_size -= 10;
			b[i].life--;
			b[i].r -= 150;
			b[i].g -= 150;
			b[i].b += 150;
		}
		//블록 오른쪽 충돌
		else if (b[i].life != 0 && *bx - 15 >= b[i].x && *bx - 15 <= b[i].x + 70 && *by >= b[i].y && *by <= b[i].y + 30) {
			if (*vx == 0) {
				*vx = 1;
			}
			else
				*vx = 0;

			b[i].minus_size -= 10;
			b[i].life--;
			b[i].r -= 150;
			b[i].g -= 150;
			b[i].b += 150;
		}

		if (b[i].life == 0) {
			*eb += 1;
		}
		else if (b[i].life != 2) {
			*eb_ += 1;
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

	HBRUSH hbrush, oldbrush; // HBRUSH ballbrush, balloldbrush;

	static TCHAR lpOut[1024];

	static block b[30];

	static int blockmove;

	static int eb, eb_;

	static bool start;	//움직이기 시작=1, 정지=0

	static bool selection;

	static bool pause;

	static bool reset;

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
		bx = 300; by = 500;
		blockmove = 1;
		eb = 0;
		eb_ = 0;
		ballx = 375; bally = 485; //원의 중심점
		vx = 0; vy = 0;
		ballspeed = 1;
		selection = 0;
		start = 0;
		pause = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetTimer(hwnd, 1, 10, NULL);

		if (bally + 15 >= 540)
		{
			ballx = bx + 75;
			bally = 485;
			vx = 0; vy = 0;
		}

		if (reset == 1)
		{
			reset = 0;
			MessageBox(hwnd, L"리셋되었습니다.", L"Reset", NULL);
		}

		if (eb == 30)
		{
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
				b[i].minus_size = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
				b[i].minus_size = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
				b[i].minus_size = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			eb_ = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			pause = 0;
			MessageBox(hwnd, L"승리!", L"GameOver", NULL);
		}

		hbrush = CreateSolidBrush(RGB(255, 0, 0));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

		Rectangle(hdc, bx, by, bx + 150, by + 20);

		for (int i = 0; i < 30; i++)
		{
			if (b[i].life != 0) {
				hbrush = CreateSolidBrush(RGB(b[i].r, b[i].g, b[i].b));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

				Rectangle(hdc, b[i].x - b[i].minus_size, b[i].y - b[i].minus_size, b[i].x + 70 + b[i].minus_size, b[i].y + 30 + b[i].minus_size);

				SelectObject(hdc, oldbrush);
				DeleteObject(hbrush);
			}
		}
		
		hbrush = CreateSolidBrush(RGB(0, 0, 255));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

		Ellipse(hdc, ballx - 15, bally - 15, ballx + 15, bally + 15);

		wsprintf(lpOut, L"speed:%d, block left:%d", ballspeed, 30 - eb);
		TextOut(hdc, 10, 10, lpOut, lstrlen(lpOut));

		if (pause == 1) {
			wsprintf(lpOut, L"색이 변한 벽돌갯수:%d, 없앤 벽돌갯수:%d", eb_, eb);
			TextOut(hdc, 300, 200, lpOut, lstrlen(lpOut));
		}

		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 's':
			start = 1;
			break;
		case 'S':
			start = 1;
			break;
		case 'p':
			if (start == 1 && pause == 0)
			{
				pause = 1;
			}
			else
				pause = 0;
			break;
		case 'P':
			if (start == 1 && pause == 0)
			{
				pause = 1;
			}
			else
				pause = 0;
			break;
		case 'q':
			DestroyWindow(hwnd);
			break;
		case 'Q':
			DestroyWindow(hwnd);
			break;
		case 'n':
		case 'N':
			for (int i = 0; i < 10; i++)
			{
				b[i].x = i * 70 + 30;
				b[i].y = 50;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
				b[i].minus_size = 0;
			}
			for (int i = 10; i < 20; i++)
			{
				b[i].x = (i - 10) * 70 + 30;
				b[i].y = 80;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
				b[i].minus_size = 0;
			}
			for (int i = 20; i < 30; i++)
			{
				b[i].x = (i - 20) * 70 + 30;
				b[i].y = 110;
				b[i].life = 2;
				b[i].r = 255;
				b[i].g = 255;
				b[i].b = 0;
				b[i].minus_size = 0;
			}
			bx = 300; by = 500;
			blockmove = 1;
			eb = 0;
			eb_ = 0;
			ballx = 375; bally = 485; //원의 중심점
			vx = 0; vy = 0;
			ballspeed = 1;
			selection = 0;
			start = 0;
			pause = 0;
			reset = 1;
			break;
		case '+':
			ballspeed++;
			break;
		case '-':
			ballspeed--;
			if (ballspeed <= 1)
				ballspeed = 1;
			break;

		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		if (start == 1 && pause == 0 && mx >= bx && mx <= bx + 150 && my >= 500 && my <= 520)
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
			if (start == 1 && pause == 0 && b[9].x + 70 >= 765 && blockmove == 1)
				blockmove = 2;
			else if (start == 1 && pause == 0 && b[0].x <= 0 && blockmove == 2)
				blockmove = 1;

			if (start == 1 && pause == 0) {
				for (int i = 0; i < 30; i++)
				{
					if (blockmove == 1)
						b[i].x += 1;
					else if (blockmove == 2)
						b[i].x -= 1;
				}
				Ballmove(hwnd, &ballx, &bally, ballspeed, &vx, &vy, &bx, b, &eb, &eb_);
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