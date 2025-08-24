#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<stdlib.h>
#include"resource.h"

#define L3 20
#define L5 13
#define L7 10

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 4-2";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void Printcell(HDC hdc, int* boardnum, bool grid)
{
	HPEN hPen, oldPen;

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	if (grid == 0) {
		hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
	}

	if (*boardnum == 3) {
		for (int i = 0; i <= 30; i++)
		{
			MoveToEx(hdc, 15 + L3 * i, 15, NULL);
			LineTo(hdc, 15 + L3 * i, 615);
			MoveToEx(hdc, 15, 15 + L3 * i, NULL);
			LineTo(hdc, 615, 15 + L3 * i);
		}
	}
	else if (*boardnum == 5) {
		for (int i = 0; i <= 50; i++)
		{
			MoveToEx(hdc, 15 + L5 * i, 15, NULL);
			LineTo(hdc, 15 + L5 * i, 665);
			MoveToEx(hdc, 15, 15 + L5 * i, NULL);
			LineTo(hdc, 665, 15 + L5 * i);
		}
	}
	else if (*boardnum == 7) {
		for (int i = 0; i <= 70; i++)
		{
			MoveToEx(hdc, 15 + L7 * i, 15, NULL);
			LineTo(hdc, 15 + L7 * i, 715);
			MoveToEx(hdc, 15, 15 + L7 * i, NULL);
			LineTo(hdc, 715, 15 + L7 * i);
		}
	}

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}

typedef struct STANDARD
{
	int x1, y1, x2, y2, sx, sy;	//원과 직사각형을 위한 변수(sx,sy는 점을 찍을 당시 블록 위치를 기억하기 위함)
	bool round; // 0=원, 1=사각형
	int r, g, b;
	struct STANDARD* link;
	bool select;
	int m_x1, m_y1, m_x2, m_y2;	//이동시를 위해 기존 위치를 저장하는 변수
}Standard;

typedef struct FREE	//자유그리기 (하나씩 그려서 이어붙이기)
{
	int r, g, b;
	int x, y;	// 한 사각형의 좌표 저장
	struct FREE* link;
}Free;

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);

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

	static int boardnum;	//3=30X30, 5=50X50, 7=70X70 

	static bool grid, board_on;

	static int Red, Green, Blue;

	static int mx, my;

	static bool selection, active; //active는 자유그리기의 사용중을 확인하는 변수

	static int shape;	//1=원, 2=사각형, 3=자유그리기

	static Standard* p1_head, * _p1_head;

	static Standard* p1, * next1, * prev1;
	static Standard* _p1, * __p1, * _next1, * _prev1;	//겹치는 공간

	static Free* freedraw;

	static Free* p2, * next2, * prev2, * cp2;

	static RECT srect, _srect, frect, drect, _drect, mrect, _mrect, crect;

	static int _r, _g, _b;
	static int __r, __g, __b;

	switch (iMsg)
	{
	case WM_CREATE:
		boardnum = 3;
		grid = 1;
		board_on = 1;
		Red = 255; Green = 0; Blue = 0;
		mx = 0; my = 0;
		selection = 0; active = 0;
		shape = 2;
		p1_head = NULL; _p1_head = NULL;
		freedraw = NULL;
		p1 = NULL; p2 = NULL; next1 = NULL; next2 = NULL; prev1 = NULL; prev2 = NULL;
		_p1 = NULL; _prev1 = NULL; _next1 = NULL; __p1 = NULL;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, 0, 0, L"Shape:                          Color:", 38);

		if (shape == 1)
			TextOut(hdc, 48, 0, L"Ellipse", 7);
		else if (shape == 2)
			TextOut(hdc, 48, 0, L"Rectangle", 9);
		else if (shape == 3)
			TextOut(hdc, 48, 0, L"Freedraw", 8);

		if (Red == 255 && Green == 0 && Blue == 0)
			TextOut(hdc, 190, 0, L"Red", 3);
		else if (Red == 0 && Green == 255 && Blue == 0)
			TextOut(hdc, 190, 0, L"Green", 5);
		else if (Red == 0 && Green == 0 && Blue == 255)
			TextOut(hdc, 190, 0, L"Blue", 4);
		else if (Red == 255 && Green == 255 && Blue == 0)
			TextOut(hdc, 190, 0, L"Yellow(Red + Green)", 19);
		else if (Red == 255 && Green == 0 && Blue == 255)
			TextOut(hdc, 190, 0, L"Magenta(Red + Blue)", 19);
		else if (Red == 0 && Green == 255 && Blue == 255)
			TextOut(hdc, 190, 0, L"Cyan(Green + Blue)", 18);

		//충돌 검사
		p2 = freedraw;

		//그림그리기
		p1 = p1_head;
		while (p1 != NULL)
		{
			srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
			if (p1->round == 1)
			{
				hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0)); 
				oldPen = (HPEN)SelectObject(hdc, hPen);

				if (board_on == 0) {
					hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // 외각선을 없앰
					oldPen = (HPEN)SelectObject(hdc, hPen);
				}

				hBrush = CreateSolidBrush(RGB(p1->r, p1->g, p1->b));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, srect.left, srect.top, srect.right, srect.bottom);

				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);

				p1 = p1->link;
			}
		}

		//합친 색 그림 그리기
		_p1 = _p1_head;
		while (_p1 != NULL) {
			srect = { _p1->x1,_p1->y1 ,_p1->x2 ,_p1->y2 };

			hBrush = CreateSolidBrush(RGB(_p1->r, _p1->g, _p1->b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			
			FillRect(hdc, &srect, hBrush);

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			_p1 = _p1->link;
		}

		Printcell(hdc, &boardnum,grid);

		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (boardnum == 3)
		{
			if (mx >= 15 && my >= 15 && mx < 615 && my < 615) {
				selection = 1;
			}
		}
		else if (boardnum == 5)
		{
			if (mx >= 15 && my >= 15 && mx < 665 && my < 665) {
				selection = 1;
			}
		}
		else if (boardnum == 7)
		{
			if (mx >= 15 && my >= 15 && mx < 715 && my < 715) {
				selection = 1;
			}
		}

		if (selection == 1) {
			if (shape == 1 || shape == 2) {
				p1 = (Standard*)malloc(sizeof(Standard));

				p1->select = 0;

				p1->r = Red; p1->g = Green; p1->b = Blue;

				if (shape == 1)
					p1->round = 0;
				else if (shape == 2)
					p1->round = 1;
				p1->sx = mx;
				p1->sy = my;
				if (boardnum == 3) {
					for (int i = 0; i < 30; i++)
					{
						if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx) {
							p1->x1 = 15 + i * L3;
							p1->x2 = 15 + i * L3 + L3;
							break;
						}
					}
					for (int i = 0; i < 30; i++)
					{
						if (15 + i * L3 <= my && 15 + i * L3 + L3 > my) {
							p1->y1 = 15 + i * L3;
							p1->y2 = 15 + i * L3 + L3;
							break;
						}
					}
				}
				else if (boardnum == 5) {
					for (int i = 0; i < 50; i++)
					{
						if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx) {
							p1->x1 = 15 + i * L5;
							p1->x2 = 15 + i * L5 + L5;
							break;
						}
					}
					for (int i = 0; i < 50; i++)
					{
						if (15 + i * L5 <= my && 15 + i * L5 + L5 > my) {
							p1->y1 = 15 + i * L5;
							p1->y2 = 15 + i * L5 + L5;
							break;
						}
					}
				}
				else if (boardnum == 7) {
					for (int i = 0; i < 70; i++)
					{
						if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx) {
							p1->x1 = 15 + i * L7;
							p1->x2 = 15 + i * L7 + L7;
							break;
						}
					}
					for (int i = 0; i < 70; i++)
					{
						if (15 + i * L7 <= my && 15 + i * L7 + L7 > my) {
							p1->y1 = 15 + i * L7;
							p1->y2 = 15 + i * L7 + L7;
							break;
						}
					}
				}

				if (p1_head == NULL)
					p1_head = p1;
				else
					prev1->link = p1;
				p1->link = NULL;
				prev1 = p1;
			}
			else if (shape == 3) {
				active = 1;

				p2 = (Free*)malloc(sizeof(Free));

				p2->r = Red; p2->g = Green; p2->b = Blue;

				if (boardnum == 3) {
					for (int i = 0; i < 30; i++)
					{
						if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx) {
							p2->x = i;
							break;
						}
					}
					for (int i = 0; i < 30; i++)
					{
						if (15 + i * L3 <= my && 15 + i * L3 + L3 > my) {
							p2->y = i;
							break;
						}
					}
				}
				else if (boardnum == 5) {
					for (int i = 0; i < 50; i++)
					{
						if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx) {
							p2->x = i;
							break;
						}
					}
					for (int i = 0; i < 50; i++)
					{
						if (15 + i * L5 <= my && 15 + i * L5 + L5 > my) {
							p2->y = i;
							break;
						}
					}
				}
				else if (boardnum == 7) {
					for (int i = 0; i < 70; i++)
					{
						if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx) {
							p2->x = i;
							break;
						}
					}
					for (int i = 0; i < 70; i++)
					{
						if (15 + i * L7 <= my && 15 + i * L7 + L7 > my) {
							p2->y = i;
							break;
						}
					}
				}

				if (freedraw == NULL)
					freedraw = p2;
				else
					prev2->link = p2;
				p2->link = NULL;
				prev2 = p2;
			}
		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		if (selection == 1) {	//범위를 넘으면 그림그리기 중지
			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			if (boardnum == 3)
			{
				if (mx < 15 || my < 15 || mx >= 615 || my >= 615) {
					selection = 0; active = 0;
				}
			}
			else if (boardnum == 5)
			{
				if (mx < 15 || my < 15 || mx >= 665 || my >= 665) {
					selection = 0; active = 0;
				}
			}
			else if (boardnum == 7)
			{
				if (mx < 15 || my < 15 || mx >= 715 || my >= 715) {
					selection = 0; active = 0;
				}
			}
		}

		if (selection == 1) {
			if (shape == 1 || shape == 2)
			{
				p1 = p1_head;

				while (p1->link != NULL)
				{
					p1 = p1->link;
				}

				p1->r = Red; p1->g = Green; p1->b = Blue;

				if (boardnum == 3) {
					if (mx < p1->x1 && my < p1->y1)
					{
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sx && 15 + i * L3 + L3 > p1->sx)
								p1->x1 = 15 + i * L3 + L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sy && 15 + i * L3 + L3 > p1->sy)
								p1->y1 = 15 + i * L3 + L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx)
								p1->x2 = 15 + i * L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= my && 15 + i * L3 + L3 > my)
								p1->y2 = 15 + i * L3;
						}
					}
					else if (mx < p1->x1)
					{
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sx && 15 + i * L3 + L3 > p1->sx)
								p1->x1 = 15 + i * L3 + L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sy && 15 + i * L3 + L3 > p1->sy)
								p1->y1 = 15 + i * L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx)
								p1->x2 = 15 + i * L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= my && 15 + i * L3 + L3 > my)
								p1->y2 = 15 + i * L3 + L3;
						}
					}
					else if (my < p1->y1)
					{
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sx && 15 + i * L3 + L3 > p1->sx)
								p1->x1 = 15 + i * L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sy && 15 + i * L3 + L3 > p1->sy)
								p1->y1 = 15 + i * L3 + L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx)
								p1->x2 = 15 + i * L3 + L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= my && 15 + i * L3 + L3 > my)
								p1->y2 = 15 + i * L3;
						}
					}
					else {
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sx && 15 + i * L3 + L3 > p1->sx)
								p1->x1 = 15 + i * L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= p1->sy && 15 + i * L3 + L3 > p1->sy)
								p1->y1 = 15 + i * L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx)
								p1->x2 = 15 + i * L3 + L3;
						}
						for (int i = 0; i < 30; i++)
						{
							if (15 + i * L3 <= my && 15 + i * L3 + L3 > my)
								p1->y2 = 15 + i * L3 + L3;
						}
					}
				}
				else if (boardnum == 5) {
					if (mx < p1->x1 && my < p1->y1)
					{
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sx && 15 + i * L5 + L5 > p1->sx)
								p1->x1 = 15 + i * L5 + L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sy && 15 + i * L5 + L5 > p1->sy)
								p1->y1 = 15 + i * L5 + L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx)
								p1->x2 = 15 + i * L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= my && 15 + i * L5 + L5 > my)
								p1->y2 = 15 + i * L5;
						}
					}
					else if (mx < p1->x1)
					{
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sx && 15 + i * L5 + L5 > p1->sx)
								p1->x1 = 15 + i * L5 + L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sy && 15 + i * L5 + L5 > p1->sy)
								p1->y1 = 15 + i * L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx)
								p1->x2 = 15 + i * L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= my && 15 + i * L5 + L5 > my)
								p1->y2 = 15 + i * L5 + L5;
						}
					}
					else if (my < p1->y1)
					{
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sx && 15 + i * L5 + L5 > p1->sx)
								p1->x1 = 15 + i * L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sy && 15 + i * L5 + L5 > p1->sy)
								p1->y1 = 15 + i * L5 + L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx)
								p1->x2 = 15 + i * L5 + L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= my && 15 + i * L5 + L5 > my)
								p1->y2 = 15 + i * L5;
						}
					}
					else {
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sx && 15 + i * L5 + L5 > p1->sx)
								p1->x1 = 15 + i * L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= p1->sy && 15 + i * L5 + L5 > p1->sy)
								p1->y1 = 15 + i * L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx)
								p1->x2 = 15 + i * L5 + L5;
						}
						for (int i = 0; i < 50; i++)
						{
							if (15 + i * L5 <= my && 15 + i * L5 + L5 > my)
								p1->y2 = 15 + i * L5 + L5;
						}
					}
				}
				else if (boardnum == 7) {
					if (mx < p1->x1 && my < p1->y1)
					{
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sx && 15 + i * L7 + L7 > p1->sx)
								p1->x1 = 15 + i * L7 + L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sy && 15 + i * L7 + L7 > p1->sy)
								p1->y1 = 15 + i * L7 + L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx)
								p1->x2 = 15 + i * L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= my && 15 + i * L7 + L7 > my)
								p1->y2 = 15 + i * L7;
						}
					}
					else if (mx < p1->x1)
					{
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sx && 15 + i * L7 + L7 > p1->sx)
								p1->x1 = 15 + i * L7 + L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sy && 15 + i * L7 + L7 > p1->sy)
								p1->y1 = 15 + i * L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx)
								p1->x2 = 15 + i * L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= my && 15 + i * L7 + L7 > my)
								p1->y2 = 15 + i * L7 + L7;
						}
					}
					else if (my < p1->y1)
					{
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sx && 15 + i * L7 + L7 > p1->sx)
								p1->x1 = 15 + i * L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sy && 15 + i * L7 + L7 > p1->sy)
								p1->y1 = 15 + i * L7 + L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx)
								p1->x2 = 15 + i * L7 + L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= my && 15 + i * L7 + L7 > my)
								p1->y2 = 15 + i * L7;
						}
					}
					else {
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sx && 15 + i * L7 + L7 > p1->sx)
								p1->x1 = 15 + i * L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= p1->sy && 15 + i * L7 + L7 > p1->sy)
								p1->y1 = 15 + i * L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx)
								p1->x2 = 15 + i * L7 + L7;
						}
						for (int i = 0; i < 70; i++)
						{
							if (15 + i * L7 <= my && 15 + i * L7 + L7 > my)
								p1->y2 = 15 + i * L7 + L7;
						}
					}
				}
			}
			else if (shape == 3)
			{
				p2 = (Free*)malloc(sizeof(Free));

				p2->r = Red; p2->g = Green; p2->b = Blue;

				if (boardnum == 3) {
					for (int i = 0; i < 30; i++)
					{
						if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx) {
							p2->x = i;
							break;
						}
					}
					for (int i = 0; i < 30; i++)
					{
						if (15 + i * L3 <= my && 15 + i * L3 + L3 > my) {
							p2->y = i;
							break;
						}
					}
				}
				else if (boardnum == 5) {
					for (int i = 0; i < 50; i++)
					{
						if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx) {
							p2->x = i;
							break;
						}
					}
					for (int i = 0; i < 50; i++)
					{
						if (15 + i * L5 <= my && 15 + i * L5 + L5 > my) {
							p2->y = i;
							break;
						}
					}
				}
				else if (boardnum == 7) {
					for (int i = 0; i < 70; i++)
					{
						if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx) {
							p2->x = i;
							break;
						}
					}
					for (int i = 0; i < 70; i++)
					{
						if (15 + i * L7 <= my && 15 + i * L7 + L7 > my) {
							p2->y = i;
							break;
						}
					}
				}

				if (freedraw == NULL)
					freedraw = p2;
				else
					prev2->link = p2;
				p2->link = NULL;
				prev2 = p2;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}

		// 움직이기
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (boardnum == 3) {
			for (int i = 0; i < 30; i++)
			{
				if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx) {
					_mrect.left = 15 + i * L3;
					_mrect.right = 15 + i * L3 + L3;
					break;
				}
			}
			for (int i = 0; i < 30; i++)
			{
				if (15 + i * L3 <= my && 15 + i * L3 + L3 > my) {
					_mrect.top = 15 + i * L3;
					_mrect.bottom = 15 + i * L3 + L3;
					break;
				}
			}
		}

		int move_x, move_y;
		move_x = mrect.left - _mrect.left;
		move_y = mrect.top - _mrect.top;

		p1 = p1_head;
		while (p1 != NULL) {
			if (p1->select == 1) {
				p1->x1 = p1->m_x1 - move_x;
				p1->y1 = p1->m_y1 - move_y;
				p1->x2 = p1->m_x2 - move_x;
				p1->y2 = p1->m_y2 - move_y;

				InvalidateRgn(hwnd, NULL, TRUE);
			}
			p1 = p1->link;
		}

		break;

	case WM_LBUTTONUP:
		selection = 0; active = 0;

		//충돌검사 -> 색 합치기
		//현재 그린 그림 + 이전 그린 그림들 검사
		p1 = p1_head;
		while (p1->link != NULL)
		{
			p1 = p1->link;
		}

		if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)		//rect의 좌표가 순차적이지(rect.left<right && top<bottom)않으면 intersectrect함수가 제대로 작동되지않기 때문에
			frect = { p1->x1,p1->y1,p1->x2,p1->y2 };
		else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
			frect = { p1->x2,p1->y1,p1->x1,p1->y2 };
		else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
			frect = { p1->x1,p1->y2,p1->x2,p1->y1 };
		else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
			frect = { p1->x2,p1->y2,p1->x1,p1->y1 };

		_r = p1->r; _g = p1->g; _b = p1->b;
		__r = 0; __g = 0; __b = 0;

		if (boardnum == 3) {
			p1 = p1_head;

			while (p1->link != NULL) {
				if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)		//rect의 좌표가 순차적이지(rect.left<right && top<bottom)않으면 intersectrect함수가 제대로 작동되지않기 때문에
					srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
				else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x2,p1->y1,p1->x1,p1->y2 };
				else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x1,p1->y2,p1->x2,p1->y1 };
				else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x2,p1->y2,p1->x1,p1->y1 };

				if (IntersectRect(&drect, &srect, &frect))
				{
					__p1 = _p1_head;

					_drect.left = 0;
					_drect.right = 0;
					_drect.top = 0;
					_drect.bottom = 0;

					if (__p1 != NULL) {
						while (__p1->link != NULL) {
							if (__p1->x1 <= __p1->x2 && __p1->y1 <= __p1->y2)
								_srect = { __p1->x1,__p1->y1,__p1->x2,__p1->y2 };
							else if (__p1->x1 > __p1->x2 && __p1->y1 <= __p1->y2)
								_srect = { __p1->x2,__p1->y1,__p1->x1,__p1->y2 };
							else if (__p1->x1 <= __p1->x2 && __p1->y1 > __p1->y2)
								_srect = { __p1->x1,__p1->y2,__p1->x2,__p1->y1 };
							else if (__p1->x1 > __p1->x2 && __p1->y1 > __p1->y2)
								_srect = { __p1->x2,__p1->y2,__p1->x1,__p1->y1 };

							if (IntersectRect(&_drect, &_srect, &frect)) {
								__r = __p1->r;
								__g = __p1->g; 
								__b = __p1->b;

								break;
							}

							__p1 = __p1->link;
						}
					}

					_p1 = (Standard*)malloc(sizeof(Standard));

					_p1->r = p1->r + _r;
					_p1->g = p1->g + _g;
					_p1->b = p1->b + _b;

					if (_p1->r > 255) {
						_p1->r = 255;
					}
					if (_p1->g > 255) {
						_p1->g = 255;
					}
					if (_p1->b > 255) {
						_p1->b = 255;
					}

					_p1->round = 1;

					_p1->x1 = drect.left;	_p1->y1 = drect.top;	_p1->x2 = drect.right;	_p1->y2 = drect.bottom;

					if (_p1_head == NULL)
						_p1_head = _p1;
					else
						_prev1->link = _p1;

					_p1->link = NULL;
					_prev1 = _p1;

					if (_drect.right != _drect.left) {
						_p1 = (Standard*)malloc(sizeof(Standard));

						_p1->r = p1->r + _r + __r;
						_p1->g = p1->g + _g + __g;
						_p1->b = p1->b + _b + __b;

						if (_p1->r > 255) {
							_p1->r = 255;
						}
						if (_p1->g > 255) {
							_p1->g = 255;
						}
						if (_p1->b > 255) {
							_p1->b = 255;
						}

						_p1->round = 1;

						_p1->x1 = _drect.left;	_p1->y1 = _drect.top;	_p1->x2 = _drect.right;	_p1->y2 = _drect.bottom;

						if (_p1_head == NULL)
							_p1_head = _p1;
						else
							_prev1->link = _p1;

						_p1->link = NULL;
						_prev1 = _p1;
					}
				}

				p1 = p1->link;
			}
		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (boardnum == 3) {
			for (int i = 0; i < 30; i++)
			{
				if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx) {
					mrect.left = 15 + i * L3;
					mrect.right = 15 + i * L3 + L3;
					break;
				}
			}
			for (int i = 0; i < 30; i++)
			{
				if (15 + i * L3 <= my && 15 + i * L3 + L3 > my) {
					mrect.top = 15 + i * L3;
					mrect.bottom = 15 + i * L3 + L3;
					break;
				}
			}
		}

		p1 = p1_head;
		while (p1 != NULL) {
			if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)		//rect의 좌표가 순차적이지(rect.left<right && top<bottom)않으면 intersectrect함수가 제대로 작동되지않기 때문에
				srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
			else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
				srect = { p1->x2,p1->y1,p1->x1,p1->y2 };
			else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
				srect = { p1->x1,p1->y2,p1->x2,p1->y1 };
			else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
				srect = { p1->x2,p1->y2,p1->x1,p1->y1 };

			if (IntersectRect(&drect, &srect, &mrect)) {
				p1->select = 1;
				p1->m_x1 = p1->x1;	p1->m_y1 = p1->y1;	p1->m_x2 = p1->x2;	p1->m_y2 = p1->y2;
			}

			p1 = p1->link;
		}

		/*if (boardnum == 3) {
			for (int i = 0; i < 30; i++)
			{
				if (15 + i * L3 <= mx && 15 + i * L3 + L3 > mx) {
					mrect.left = 15 + i * L3;
					mrect.right = 15 + i * L3 + L3;
					break;
				}
			}
			for (int i = 0; i < 30; i++)
			{
				if (15 + i * L3 <= my && 15 + i * L3 + L3 > my) {
					mrect.top = 15 + i * L3;
					mrect.bottom = 15 + i * L3 + L3;
					break;
				}
			}
		}
		else if (boardnum == 5) {
			for (int i = 0; i < 50; i++)
			{
				if (15 + i * L5 <= mx && 15 + i * L5 + L5 > mx) {
					mrect.left = 15 + i * L5;
					mrect.right = 15 + i * L5 + L5;
					break;
				}
			}
			for (int i = 0; i < 50; i++)
			{
				if (15 + i * L5 <= my && 15 + i * L5 + L5 > my) {
					mrect.top = 15 + i * L5;
					mrect.bottom = 15 + i * L5 + L5;
					break;
				}
			}
		}
		else if (boardnum == 7) {
			for (int i = 0; i < 70; i++)
			{
				if (15 + i * L7 <= mx && 15 + i * L7 + L7 > mx) {
					mrect.left = 15 + i * L7;
					mrect.right = 15 + i * L7 + L7;
					break;
				}
			}
			for (int i = 0; i < 70; i++)
			{
				if (15 + i * L7 <= my && 15 + i * L7 + L7 > my) {
					mrect.top = 15 + i * L7;
					mrect.bottom = 15 + i * L7 + L7;
					break;
				}
			}
		}
		
		//충돌 검사
		if (boardnum == 3) {
			p1 = p1_head;
			while (p1 != NULL) {
				if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
				else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x2,p1->y1,p1->x1,p1->y2 };
				else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x1,p1->y2,p1->x2,p1->y1 };
				else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x2,p1->y2,p1->x1,p1->y1 };
				if (IntersectRect(&drect, &mrect, &srect))
				{
					p1->r = Red; p1->b = Blue; p1->g = Green;
				}
				p1 = p1->link;
			}

			cp2 = freedraw;
			while (cp2 != NULL) {
				crect = { 15 + L3 * cp2->x,15 + L3 * cp2->y,15 + L3 * cp2->x + L3,15 + L3 * cp2->y + L3 };
				if (IntersectRect(&drect, &crect, &mrect))
				{
					cp2->r = Red; cp2->b = Blue; cp2->g = Green;
				}
				cp2 = cp2->link;
			}
		}
		else if (boardnum == 5) {
			p1 = p1_head;
			while (p1 != NULL) {
				if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
				else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x2,p1->y1,p1->x1,p1->y2 };
				else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x1,p1->y2,p1->x2,p1->y1 };
				else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x2,p1->y2,p1->x1,p1->y1 };
				if (IntersectRect(&drect, &mrect, &srect))
				{
					p1->r = Red; p1->b = Blue; p1->g = Green;
				}
				p1 = p1->link;
			}

			cp2 = freedraw;
			while (cp2 != NULL) {
				crect = { 15 + L5 * cp2->x,15 + L5 * cp2->y,15 + L5 * cp2->x + L5,15 + L5 * cp2->y + L5 };
				if (IntersectRect(&drect, &crect, &mrect))
				{
					cp2->r = Red; cp2->b = Blue; cp2->g = Green;
				}
				cp2 = cp2->link;
			}
		}
		else if (boardnum == 7) {
			p1 = p1_head;
			while (p1 != NULL) {
				if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
				else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
					srect = { p1->x2,p1->y1,p1->x1,p1->y2 };
				else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x1,p1->y2,p1->x2,p1->y1 };
				else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
					srect = { p1->x2,p1->y2,p1->x1,p1->y1 };
				if (IntersectRect(&drect, &mrect, &srect))
				{
					p1->r = Red; p1->b = Blue; p1->g = Green;
				}
				p1 = p1->link;
			}

			cp2 = freedraw;
			while (cp2 != NULL) {
				crect = { 15 + L7 * cp2->x,15 + L7 * cp2->y,15 + L7 * cp2->x + L7,15 + L7 * cp2->y + L7 };
				if (IntersectRect(&drect, &crect, &mrect))
				{
					cp2->r = Red; cp2->b = Blue; cp2->g = Green;
				}
				cp2 = cp2->link;
			}

		}*/

		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_RBUTTONUP:
	{
		p1 = p1_head;
		while (p1 != NULL) {
			p1->select = 0;

			p1 = p1->link;
		}

		//기존 겹치는 부분 초기화
		_p1 = _p1_head;
		while (_p1 != NULL)
		{
			_next1 = _p1->link;
			free(_p1);
			_p1 = _next1;
		}

		//다시 겹치는 부분 검사 & 생성
		_p1 = NULL;
		_p1_head = NULL;
		_prev1 = NULL;
		_next1 = NULL;

		Standard* p1_ = p1_head;
		while (p1_ != NULL)
		{
			if (p1_->x1 <= p1_->x2 && p1_->y1 <= p1_->y2)		//rect의 좌표가 순차적이지(rect.left<right && top<bottom)않으면 intersectrect함수가 제대로 작동되지않기 때문에
				frect = { p1_->x1,p1_->y1,p1_->x2,p1_->y2 };
			else if (p1_->x1 > p1_->x2 && p1_->y1 <= p1_->y2)
				frect = { p1_->x2,p1_->y1,p1_->x1,p1_->y2 };
			else if (p1_->x1 <= p1_->x2 && p1_->y1 > p1_->y2)
				frect = { p1_->x1,p1_->y2,p1_->x2,p1_->y1 };
			else if (p1_->x1 > p1_->x2 && p1_->y1 > p1_->y2)
				frect = { p1_->x2,p1_->y2,p1_->x1,p1_->y1 };

			_r = p1_->r; _g = p1_->g; _b = p1_->b;

			if (boardnum == 3) {
				p1 = p1_head;

				while (p1 != NULL) {
					if (p1->x1 <= p1->x2 && p1->y1 <= p1->y2)		//rect의 좌표가 순차적이지(rect.left<right && top<bottom)않으면 intersectrect함수가 제대로 작동되지않기 때문에
						srect = { p1->x1,p1->y1,p1->x2,p1->y2 };
					else if (p1->x1 > p1->x2 && p1->y1 <= p1->y2)
						srect = { p1->x2,p1->y1,p1->x1,p1->y2 };
					else if (p1->x1 <= p1->x2 && p1->y1 > p1->y2)
						srect = { p1->x1,p1->y2,p1->x2,p1->y1 };
					else if (p1->x1 > p1->x2 && p1->y1 > p1->y2)
						srect = { p1->x2,p1->y2,p1->x1,p1->y1 };

					if (IntersectRect(&drect, &srect, &frect) && p1_ != p1)
					{
						__p1 = _p1_head;

						_drect.left = 0;
						_drect.right = 0;
						_drect.top = 0;
						_drect.bottom = 0;

						if (__p1 != NULL) {
							while (__p1->link != NULL) {
								if (__p1->x1 <= __p1->x2 && __p1->y1 <= __p1->y2)
									_srect = { __p1->x1,__p1->y1,__p1->x2,__p1->y2 };
								else if (__p1->x1 > __p1->x2 && __p1->y1 <= __p1->y2)
									_srect = { __p1->x2,__p1->y1,__p1->x1,__p1->y2 };
								else if (__p1->x1 <= __p1->x2 && __p1->y1 > __p1->y2)
									_srect = { __p1->x1,__p1->y2,__p1->x2,__p1->y1 };
								else if (__p1->x1 > __p1->x2 && __p1->y1 > __p1->y2)
									_srect = { __p1->x2,__p1->y2,__p1->x1,__p1->y1 };

								if (IntersectRect(&_drect, &_srect, &drect) && p1_ != __p1) {
									__r = __p1->r;
									__g = __p1->g;
									__b = __p1->b;

									break;
								}

								__p1 = __p1->link;
							}
						}

						_p1 = (Standard*)malloc(sizeof(Standard));

						_p1->r = p1->r + _r; 
						_p1->g = p1->g + _g; 
						_p1->b = p1->b + _b;

						if (_p1->r > 255) {
							_p1->r = 255;
						}
						if (_p1->g > 255) {
							_p1->g = 255;
						}
						if (_p1->b > 255) {
							_p1->b = 255;
						}

						_p1->round = 1;

						_p1->x1 = drect.left;	_p1->y1 = drect.top;	_p1->x2 = drect.right;	_p1->y2 = drect.bottom;

						if (_p1_head == NULL)
							_p1_head = _p1;
						else
							_prev1->link = _p1;

						_p1->link = NULL;
						_prev1 = _p1;

						if (_drect.right != _drect.left) {
							_p1 = (Standard*)malloc(sizeof(Standard));

							_p1->r = p1->r + _r + __r;
							_p1->g = p1->g + _g + __g;
							_p1->b = p1->b + _b + __b;

							if (_p1->r > 255) {
								_p1->r = 255;
							}
							if (_p1->g > 255) {
								_p1->g = 255;
							}
							if (_p1->b > 255) {
								_p1->b = 255;
							}

							_p1->round = 1;

							_p1->x1 = _drect.left;	_p1->y1 = _drect.top;	_p1->x2 = _drect.right;	_p1->y2 = _drect.bottom;

							if (_p1_head == NULL)
								_p1_head = _p1;
							else
								_prev1->link = _p1;

							_p1->link = NULL;
							_prev1 = _p1;
						}
					}

					p1 = p1->link;
				}
			}

			p1_ = p1_->link;
		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	}

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_GRID_LINE:
			grid = 1;
			break;
		case ID_GRID_POINT:
			grid = 0;
			break;

		case ID_COLOR_RED:
			Red = 255;
			Green = 0;
			Blue = 0;
			break;
		case ID_COLOR_GREEN:
			Red = 0;
			Green = 255;
			Blue = 0;
			break;
		case ID_COLOR_BLUE:
			Red = 0;
			Green = 0;
			Blue = 255;
			break;
		case ID_COLOR_YELLOW:
			Red = 255;
			Green = 255;
			Blue = 0;
			break;
		case ID_COLOR_CYAN:
			Red = 0;
			Green = 255;
			Blue = 255;
			break;
		case ID_COLOR_MAGENTA:
			Red = 255;
			Green = 0;
			Blue = 255;
			break;

		case ID_BORDER_ON:
			board_on = 1;
			break;
		case ID_BORDER_OFF:
			board_on = 0;
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		p1 = p1_head;
		while (p1 != NULL)
		{
			next1 = p1->link;
			free(p1);
			p1 = next1;
		}
		_p1 = _p1_head;
		while (_p1 != NULL)
		{
			_next1 = _p1->link;
			free(_p1);
			_p1 = _next1;
		}
		p2 = freedraw;
		while (p2 != NULL)
		{
			next2 = p2->link;
			free(p2);
			p2 = next2;
		}
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
