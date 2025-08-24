#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<time.h>

#define L 40								

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-9";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void Printcell(HDC hdc)
{
	Rectangle(hdc, 15, 15, 615, 615);

	for (int i = 1; i < 15; i++)
	{
		MoveToEx(hdc, 15 + L * i, 15, NULL);
		LineTo(hdc, 15 + L * i, 615);
		MoveToEx(hdc, 15, 15 + L * i, NULL);
		LineTo(hdc, 615, 15 + L * i);
	}
}

typedef struct Stone
{
	int x, y;
	int r, g, b;
	bool size_up;
	bool shape_change;
}stones;

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 870, 700, NULL, (HMENU)NULL, hInstance, NULL);

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

	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	static stones s[23];	//s[0]이 플레이어1,s[21]이 플레이어2,s[1~14]가 장애물,s[15]가 크기변환,s[16~20]이 색깔변화박스, s[22] 모양 변화박스

	static POINT point[4];

	static int p1, p2;

	srand(time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:
		//돌 초기 위치
		for (int i = 0; i < 23; i++)
		{
			s[i].x = rand() % 15;
			s[i].y = rand() % 15;
			for (int j = 0; j < i; j++)
			{
				while (s[i].x == s[j].x && s[i].y == s[j].y)
				{
					s[i].x = rand() % 15;
					s[i].y = rand() % 15;
				}
			}
		}

		//초기 색깔 넣기
		s[0].r = 255;	s[21].r = 0;
		s[0].g = 255;	s[21].g = 0;
		s[0].b = 255;   s[21].b = 0;

		for (int i = 1; i < 15; i++) {
			s[i].r = 255;
			s[i].g = 0;
			s[i].b = 0;
		}

		s[15].r = 150;
		s[15].g = 150;
		s[15].b = 150;

		for (int i = 16; i < 21; i++) {
			s[i].r = rand() % 256;
			s[i].g = rand() % 256;
			s[i].b = rand() % 256;
		}

		s[22].r = 250;
		s[22].g = 250;
		s[22].b = 0;

		//사이즈
		s[0].size_up = 0;
		s[21].size_up = 0;

		//모양
		s[0].shape_change = 0;
		s[21].shape_change = 0;

		//순서
		p1 = 0; p2 = 0;

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//TextOut(hdc, 500, 0, L"플레이어1 - 원   플레이어2 - 마름모", lstrlen(L"플레이어1 - 원   플레이어2 - 마름모"));

		if (p1 == p2)
			TextOut(hdc, 625, 50, L"플레이어<○> 차례 (wasd)", lstrlen(L"플레이어<○> 차례 (wasd)"));
		else
			TextOut(hdc, 625, 50, L"플레이어<◇> 차례 (↑←↓→)", lstrlen(L"플레이어<◇> 차례 (↑←↓→)"));

		Printcell(hdc);

		//빨간 블록
		for (int i = 1; i <= 14; i++) {
			hPen = CreatePen(PS_SOLID, 3, RGB(s[i].r, s[i].g, s[i].b));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(s[i].r, s[i].g, s[i].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, 15 + s[i].x * L, 15 + s[i].y * L, 55 + s[i].x * L, 55 + s[i].y * L);
		}

		//크기 변경 블록
		hPen = CreatePen(PS_DOT, 0.5, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(s[15].r, s[15].g, s[15].b));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, 15 + s[15].x * L, 15 + s[15].y * L, 55 + s[15].x * L, 55 + s[15].y * L);

		//모양 변경 블록
		hPen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(s[22].r, s[22].g, s[22].b));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, 15 + s[22].x * L, 15 + s[22].y * L, 55 + s[22].x * L, 55 + s[22].y * L);

		//색깔 블록
		for (int i = 16; i <= 20; i++) {
			hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(s[i].r, s[i].g, s[i].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, 15 + s[i].x * L, 15 + s[i].y * L, 55 + s[i].x * L, 55 + s[i].y * L);
		}

		//플레이어1
		hPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(s[0].r, s[0].g, s[0].b));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		if (s[0].size_up != 1) {
			if (s[0].shape_change != 1)
				Ellipse(hdc, 20 + s[0].x * L, 20 + s[0].y * L, 50 + s[0].x * L, 50 + s[0].y * L);
			else
				Pie(hdc, 20 + s[0].x * L, 20 + s[0].y * L, 50 + s[0].x * L, 50 + s[0].y * L, 
					((50)) + s[0].x * L, 20 + s[0].y * L + ((10)), 50 + s[0].x * L, 50 + s[0].y * L - ((10)));
		}
		else {
			if (s[0].shape_change != 1)
				Ellipse(hdc, 10 + s[0].x * L, 10 + s[0].y * L, 60 + s[0].x * L, 60 + s[0].y * L);
			else
				Pie(hdc, 10 + s[0].x * L, 10 + s[0].y * L, 60 + s[0].x * L, 60 + s[0].y * L,
					((60)) + s[0].x * L, 10 + s[0].y * L + ((16)), 60 + s[0].x * L, 60 + s[0].y * L - ((16)));
		}
		

		//플레이어2
		if (s[21].size_up != 1) {
			hPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(s[21].r, s[21].g, s[21].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			if (s[21].shape_change != 1) {
				point[0] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 5 };
				point[1] = { s[21].x * L + 30 + 15 + 5,s[21].y * L + 30 + 5 };
				point[2] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 30 + 5 };
				point[3] = { s[21].x * L + 15 + 5, s[21].y * L + 30 + 5 };

				Polygon(hdc, point, 4);
			}
			else {
				point[0] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 5 };
				point[1] = { s[21].x * L + 30 + 15 + 5,s[21].y * L + 30 + 5 + ((10)) };
				point[2] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 30 + 5 - ((10)) };
				point[3] = { s[21].x * L + 15 + 5, s[21].y * L + 30 + 5 + ((10)) };

				Polygon(hdc, point, 4);
			}
		}
		else {
			hPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(s[21].r, s[21].g, s[21].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			if (s[21].shape_change != 1) {
				point[0] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 5 - (10) };
				point[1] = { s[21].x * L + 30 + 15 + 5 + (10),s[21].y * L + 30 + 5 };
				point[2] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 30 + 5 + (10) };
				point[3] = { s[21].x * L + 15 + 5 - (10), s[21].y * L + 30 + 5 };

				Polygon(hdc, point, 4);
			}
			else {
				point[0] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 5 - (10) };
				point[1] = { s[21].x * L + 30 + 15 + 5 + (10),s[21].y * L + 30 + 5 + ((20))};
				point[2] = { s[21].x * L + 30 + 5,s[21].y * L + 15 + 30 + 5 + (10) - ((20))};
				point[3] = { s[21].x * L + 15 + 5 - (10), s[21].y * L + 30 + 5 + ((20))};

				Polygon(hdc, point, 4);
			}
		}

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
			//플레이어1 방향키
		case 'a':
		case 'A':
			if (p1 == p2) {
				s[0].x -= 1;
				p1++;

				if (s[0].x == -1)
					s[0].x = 14;

				//크기 변경
				if (s[0].x == s[15].x && s[0].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].size_up = !(s[0].size_up);
					}
				}

				//모양 변경
				if (s[0].x == s[22].x && s[0].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].shape_change = !(s[0].shape_change);
					}
				}

				//장애물 충돌 체크
				for (int i = 1; i < 15; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						if (s[0].x == 14)
							s[0].x = 0;
						else
							s[0].x += 1;

						p1--;
					}
				}

				//플레이어 충돌 체크
				if (s[0].x == s[21].x && s[0].y == s[21].y)
				{
					if (s[0].x == 14)
						s[0].x = 0;
					else
						s[0].x += 1;

					p1--;
				}

				//색 변경
				for (int i = 16; i < 21; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						s[0].r = s[i].r;
						s[0].g = s[i].g;
						s[0].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어◇의 차례입니다.", L"에러", MB_OK);
			break;
		case 'w':
		case 'W':
			if (p1 == p2) {
				s[0].y -= 1;
				p1++;

				if (s[0].y == -1)
					s[0].y = 14;

				//크기 변경
				if (s[0].x == s[15].x && s[0].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].size_up = !(s[0].size_up);
					}
				}

				//모양 변경
				if (s[0].x == s[22].x && s[0].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].shape_change = !(s[0].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						if (s[0].y == 14)
							s[0].y = 0;
						else
							s[0].y += 1;

						p1--;
					}
				}

				if (s[0].x == s[21].x && s[0].y == s[21].y)
				{
					if (s[0].y == 14)
						s[0].y = 0;
					else
						s[0].y += 1;

					p1--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						s[0].r = s[i].r;
						s[0].g = s[i].g;
						s[0].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어◇의 차례입니다.", L"에러", MB_OK);
			break;
		case 'd':
		case 'D':
			if (p1 == p2) {
				s[0].x += 1;
				p1++;

				if (s[0].x == 15)
					s[0].x = 0;

				//크기 변경
				if (s[0].x == s[15].x && s[0].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].size_up = !(s[0].size_up);
					}
				}

				//모양 변경
				if (s[0].x == s[22].x && s[0].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].shape_change = !(s[0].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						if (s[0].x == 0)
							s[0].x = 14;
						else
							s[0].x -= 1;

						p1--;
					}
				}
				
				if (s[0].x == s[21].x && s[0].y == s[21].y)
				{
					if (s[0].x == 0)
						s[0].x = 14;
					else
						s[0].x -= 1;

					p1--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						s[0].r = s[i].r;
						s[0].g = s[i].g;
						s[0].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어◇의 차례입니다.", L"에러", MB_OK);
			break;
		case 's':
		case 'S':
			if (p1 == p2) {
				s[0].y += 1;
				p1++;

				if (s[0].y == 15)
					s[0].y = 0;

				//크기 변경
				if (s[0].x == s[15].x && s[0].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변환
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].size_up = !(s[0].size_up);
					}
				}

				//모양 변경
				if (s[0].x == s[22].x && s[0].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변경
					if (s[0].x != s[21].x || s[0].y != s[21].y) {
						for (int i = 1; i < 15; i++) {
							if (s[0].x == s[i].x && s[0].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[0].shape_change = !(s[0].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						if (s[0].y == 0)
							s[0].y = 14;
						else
							s[0].y -= 1;

						p1--;
					}
				}
				
				if (s[0].x == s[21].x && s[0].y == s[21].y)
				{
					if (s[0].y == 0)
						s[0].y = 14;
					else
						s[0].y -= 1;

					p1--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[0].x == s[i].x && s[0].y == s[i].y)
					{
						s[0].r = s[i].r;
						s[0].g = s[i].g;
						s[0].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어◇의 차례입니다.", L"에러", MB_OK);
			break;

			//종료키
		case 'q':
		case 'Q':
			DestroyWindow(hwnd);
			break;

			//다시시작
		case 'r':
		case 'R':
			for (int i = 0; i < 23; i++)
			{
				s[i].x = rand() % 15;
				s[i].y = rand() % 15;
				for (int j = 0; j < i; j++)
				{
					while (s[i].x == s[j].x && s[i].y == s[j].y)
					{
						s[i].x = rand() % 15;
						s[i].y = rand() % 15;
					}
				}
			}

			//초기 색깔 넣기
			s[0].r = 255;	s[21].r = 0;
			s[0].g = 255;	s[21].g = 0;
			s[0].b = 255;   s[21].b = 0;

			for (int i = 1; i < 15; i++) {
				s[i].r = 255;
				s[i].g = 0;
				s[i].b = 0;
			}

			s[15].r = 150;
			s[15].g = 150;
			s[15].b = 150;

			for (int i = 16; i < 21; i++) {
				s[i].r = rand() % 256;
				s[i].g = rand() % 256;
				s[i].b = rand() % 256;
			}

			//사이즈
			s[0].size_up = 0;
			s[21].size_up = 0;

			//순서
			p1 = 0; p2 = 0;

			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
			//플레이어2 방향키
		case VK_LEFT:
			if (p1 > p2)
			{
				s[21].x -= 1;
				p2++;

				if (s[21].x == -1)
					s[21].x = 14;

				//크기 변경
				if (s[21].x == s[15].x && s[21].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].size_up = !(s[21].size_up);
					}
				}

				//모양 변경
				if (s[21].x == s[22].x && s[21].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].shape_change = !(s[21].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						if (s[21].x == 14)
							s[21].x = 0;
						else
							s[21].x += 1;

						p2--;
					}
				}
				
				if (s[21].x == s[0].x && s[21].y == s[0].y)
				{
					if (s[21].x == 14)
						s[21].x = 0;
					else
						s[21].x += 1;

					p2--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						s[21].r = s[i].r;
						s[21].g = s[i].g;
						s[21].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어○의 차례입니다.", L"에러", MB_OK);
			break;
		case VK_UP:
			if (p1 > p2)
			{
				s[21].y -= 1;
				p2++;

				if (s[21].y == -1)
					s[21].y = 14;

				//크기 변경
				if (s[21].x == s[15].x && s[21].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].size_up = !(s[21].size_up);
					}
				}

				//모양 변경
				if (s[21].x == s[22].x && s[21].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].shape_change = !(s[21].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						if (s[21].y == 14)
							s[21].y = 0;
						else
							s[21].y += 1;

						p2--;
					}
				}
				
				if (s[21].x == s[0].x && s[21].y == s[0].y)
				{
					if (s[21].y == 14)
						s[21].y = 0;
					else
						s[21].y += 1;

					p2--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						s[21].r = s[i].r;
						s[21].g = s[i].g;
						s[21].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어○의 차례입니다.", L"에러", MB_OK);
			break;
		case VK_RIGHT:
			if (p1 > p2)
			{
				s[21].x += 1;
				p2++;

				if (s[21].x == 15)
					s[21].x = 0;

				//크기 변경
				if (s[21].x == s[15].x && s[21].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].size_up = !(s[21].size_up);
					}
				}

				//모양 변경
				if (s[21].x == s[22].x && s[21].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].shape_change = !(s[21].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						if (s[21].x == 0)
							s[21].x = 14;
						else
							s[21].x -= 1;

						p2--;
					}
				}
				
				if (s[21].x == s[0].x && s[21].y == s[0].y)
				{
					if (s[21].x == 0)
						s[21].x = 14;
					else
						s[21].x -= 1;

					p2--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						s[21].r = s[i].r;
						s[21].g = s[i].g;
						s[21].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어○의 차례입니다.", L"에러", MB_OK);
			break;
		case VK_DOWN:
			if (p1 > p2)
			{
				s[21].y += 1;
				p2++;

				if (s[21].y == 15)
					s[21].y = 0;

				//크기 변경
				if (s[21].x == s[15].x && s[21].y == s[15].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 크기 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].size_up = !(s[21].size_up);
					}
				}

				//모양 변경
				if (s[21].x == s[22].x && s[21].y == s[22].y)
				{
					bool correct_move = 1;

					//움직일 수 있어야(즉 크기변환 칸을 빠져 나온 후에) 모양 변환
					if (s[21].x != s[0].x || s[21].y != s[0].y) {
						for (int i = 1; i < 15; i++) {
							if (s[21].x == s[i].x && s[21].y == s[i].y)
								correct_move = 0;
						}

						if (correct_move == 1)
							s[21].shape_change = !(s[21].shape_change);
					}
				}

				for (int i = 1; i < 15; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						if (s[21].y == 0)
							s[21].y = 14;
						else
							s[21].y -= 1;

						p2--;
					}
				}
				
				if (s[21].x == s[0].x && s[21].y == s[0].y)
				{
					if (s[21].y == 0)
						s[21].y = 14;
					else
						s[21].y -= 1;

					p2--;
				}

				for (int i = 16; i < 21; i++) {
					if (s[21].x == s[i].x && s[21].y == s[i].y)
					{
						s[21].r = s[i].r;
						s[21].g = s[i].g;
						s[21].b = s[i].b;
					}
				}
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else
				MessageBox(NULL, L"플레이어○의 차례입니다.", L"에러", MB_OK);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}