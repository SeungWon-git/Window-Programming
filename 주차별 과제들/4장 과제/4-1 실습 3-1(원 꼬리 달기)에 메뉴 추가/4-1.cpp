#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<stdlib.h>
#include<ctime>
#include "resource.h"

#define L4 15

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 4-1";
void CALLBACK TimerProc1(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime);
void CALLBACK TimerProc2(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime);

typedef struct BLOCK
{
	RECT rt;
	int move;		//1 - ������   2 - ����   3 - ��    4 - �Ʒ�    5 - ��ó��ȸ
	BOOL moved;		//0 - ���� ����  1 - ���� ����
	int big;		//�浹�Ͽ� Ŀ�� ����
	BOOL follow;
	int num;
}BLOCK;

struct Obstacle
{
	RECT rt;
};

BLOCK block[31];

Obstacle obstacle[10];
int obstacle_indx = 0;

int tail = 610, a = 1, b = 1000, c = 1000, d = 5;	// tail-�����߰� �� �������� , a-�����߰��� ����ϴ� �ε������� , b-timeproc1 �ð��������� , c-timeproc2 �ð��������� , d-���ΰ� �ӵ����� ���� 
int t = 0;	//������ ���� ����
int jump = 0;	//������ ���� ����
int uturn = 0;	//������ ���� ����
int tail_num = 20;	//�ִ� ���� �� ��

//����
void CALLBACK TimerProc1(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime)
{
	HDC hdc;
	hdc = GetDC(hwnd);

	int x;
	int y;

	if (tail > 0)
		tail--;
	if (a <= tail_num && tail % 30 == 0) {
		x = rand() % 40;
		y = rand() % 40;
		block[a].rt = { 15 + x * L4,15 + y * L4,15 + x * L4 + L4,15 + y * L4 + L4 };
		block[a].move = rand() % 5 + 1;
		block[a].moved = rand() % 2;
		block[a].big = 0;
		block[a].follow = 0;
		a++;
	}

	b--;
	if (b <= 0)
		b = 1000;

	for (int i = 1; i < tail_num; i++)
	{
		for (int j = i + 1; j < tail_num + 1; j++)
			if (block[i].rt.left == block[j].rt.left && block[i].rt.top == block[j].rt.top && block[i].follow != 1 && block[j].follow != 1 && block[i].big <= 2) {
				block[i].rt.left -= 10; block[i].rt.right += 10; block[i].rt.top -= 10; block[i].rt.bottom += 10; block[i].big += 1;
				block[j].rt.left -= 10; block[j].rt.right += 10; block[j].rt.top -= 10; block[j].rt.bottom += 10; block[j].big += 1;
			}
	}

	if (b % 4 == 0)
		for (int i = 1; i < tail_num + 1; i++) {
			if (block[i].follow != 1) {
				if ((block[i].rt.top == 600 && block[i].moved == 0 && block[i].move == 1) || (block[i].rt.top == 600 && block[i].moved == 0 && block[i].move == 2))
				{
					block[i].moved = 1;
				}
				else if ((block[i].rt.top == 15 && block[i].moved == 1 && block[i].move == 1) || (block[i].rt.top == 15 && block[i].moved == 1 && block[i].move == 2))
				{
					block[i].moved = 0;
				}
				else if ((block[i].rt.left == 600 && block[i].moved == 0 && block[i].move == 3) || (block[i].rt.left == 600 && block[i].moved == 0 && block[i].move == 4))
				{
					block[i].moved = 1;
				}
				else  if ((block[i].rt.left == 15 && block[i].moved == 1 && block[i].move == 3) || (block[i].rt.left == 15 && block[i].moved == 1 && block[i].move == 4))
				{
					block[i].moved = 0;
				}

				if (block[i].move == 1)
				{
					block[i].rt.left += 15; block[i].rt.right += 15;
					if (block[i].big >= 1)
					{
						block[i].rt.left += 10; block[i].rt.right -= 10; block[i].rt.top += 10; block[i].rt.bottom -= 10; block[i].big -= 1;
					}

					if (block[i].rt.left >= 615) {
						if (block[i].moved == 0)
						{
							block[i].rt.top += 15; block[i].rt.bottom += 15;
							block[i].rt.left -= 15; block[i].rt.right -= 15;
						}
						else
						{
							block[i].rt.top -= 15; block[i].rt.bottom -= 15;
							block[i].rt.left -= 15; block[i].rt.right -= 15;
						}
						block[i].move = 2;
					}
				}
				else if (block[i].move == 2)
				{
					block[i].rt.left -= 15; block[i].rt.right -= 15;
					if (block[i].big >= 1)
					{
						block[i].rt.left += 10; block[i].rt.right -= 10; block[i].rt.top += 10; block[i].rt.bottom -= 10; block[i].big -= 1;
					}

					if (block[i].rt.left <= 0) {
						if (block[i].moved == 0)
						{
							block[i].rt.top += 15; block[i].rt.bottom += 15;
							block[i].rt.left += 15; block[i].rt.right += 15;
						}
						else
						{
							block[i].rt.top -= 15; block[i].rt.bottom -= 15;
							block[i].rt.left += 15; block[i].rt.right += 15;
						}
						block[i].move = 1;
					}
				}
				else if (block[i].move == 3)
				{
					block[i].rt.top -= 15; block[i].rt.bottom -= 15;
					if (block[i].big >= 1)
					{
						block[i].rt.left += 10; block[i].rt.right -= 10; block[i].rt.top += 10; block[i].rt.bottom -= 10; block[i].big -= 1;
					}

					if (block[i].rt.top <= 0) {
						if (block[i].moved == 0)
						{
							block[i].rt.right += 15; block[i].rt.left += 15;
							block[i].rt.top += 15; block[i].rt.bottom += 15;
						}
						else
						{
							block[i].rt.right -= 15; block[i].rt.left -= 15;
							block[i].rt.top += 15; block[i].rt.bottom += 15;
						}
						block[i].move = 4;
					}
				}
				else if (block[i].move == 4)
				{
					block[i].rt.top += 15; block[i].rt.bottom += 15;
					if (block[i].big >= 1)
					{
						block[i].rt.left += 10; block[i].rt.right -= 10; block[i].rt.top += 10; block[i].rt.bottom -= 10; block[i].big -= 1;
					}

					if (block[i].rt.top >= 615) {
						if (block[i].moved == 0)
						{
							block[i].rt.right += 15; block[i].rt.left += 15;
							block[i].rt.top -= 15; block[i].rt.bottom -= 15;
						}
						else
						{
							block[i].rt.right -= 15; block[i].rt.left -= 15;
							block[i].rt.top -= 15; block[i].rt.bottom -= 15;
						}
						block[i].move = 3;
					}
				}
				else if (block[i].move == 5)
				{
					int r = rand() % 4;

					if (block[i].big >= 1)
					{
						block[i].rt.left += 10; block[i].rt.right -= 10; block[i].rt.top += 10; block[i].rt.bottom -= 10; block[i].big -= 1;
					}

					if (r == 0 && block[i].rt.left <= 585)
					{
						block[i].rt.left += 15; block[i].rt.right += 15;
					}
					else if (r == 1 && block[i].rt.left >= 30)
					{
						block[i].rt.left -= 15; block[i].rt.right -= 15;
					}
					else if (r == 2 && block[i].rt.top >= 30)
					{
						block[i].rt.top -= 15; block[i].rt.bottom -= 15;
					}
					else if (r == 3 && block[i].rt.top <= 585)
					{
						block[i].rt.top += 15; block[i].rt.bottom += 15;
					}
				}
			}

		}

	InvalidateRgn(hwnd, NULL, 1);
	ReleaseDC(hwnd, hdc);
}

//���ΰ���
void CALLBACK TimerProc2(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime)
{
	HDC hdc;
	hdc = GetDC(hwnd);

	int dx, dy;

	c--;
	if (c <= 0)
		c = 1000;

	for (int i = 1; i < tail_num + 1; i++)
	{
		if (block[0].rt.left == block[i].rt.left && block[0].rt.top == block[i].rt.top && block[i].follow != 1) {
			block[i].follow = 1;
			block[i].num = t;
			t++;
		}
	}


	if (c % d == 0) {
		for (int n = t - 1; n >= 0; n--) {		//�������� �׸���(������ ����� ��)

			for (int i = 0; i < tail_num + 1; i++) {
				if (block[i].num == n && block[i].follow == 1 && i == 0) {	//�Ӹ�

					if (jump == 6)
					{
						block[i].rt.left -= 4; block[i].rt.right += 4; block[i].rt.top -= 4; block[i].rt.bottom += 4;
						jump--;
					}
					else if (jump == 5)
					{
						block[i].rt.left -= 4; block[i].rt.right += 4; block[i].rt.top -= 4; block[i].rt.bottom += 4;
						jump--;
					}
					else if (jump == 4)
					{
						block[i].rt.left -= 4; block[i].rt.right += 4; block[i].rt.top -= 4; block[i].rt.bottom += 4;
						jump--;
					}
					else if (jump == 3)
					{
						block[i].rt.left += 4; block[i].rt.right -= 4; block[i].rt.top += 4; block[i].rt.bottom -= 4;
						jump--;
					}
					else if (jump == 2)
					{
						block[i].rt.left += 4; block[i].rt.right -= 4; block[i].rt.top += 4; block[i].rt.bottom -= 4;
						jump--;
					}
					else if (jump == 1)
					{
						block[i].rt.left += 4; block[i].rt.right -= 4; block[i].rt.top += 4; block[i].rt.bottom -= 4;
						jump--;
					}

					if ((block[i].rt.top == 600 && block[i].moved == 0 && block[i].move == 1) || (block[i].rt.top == 600 && block[i].moved == 0 && block[i].move == 2))
					{
						block[i].moved = 1;
					}
					else if ((block[i].rt.top == 15 && block[i].moved == 1 && block[i].move == 1) || (block[i].rt.top == 15 && block[i].moved == 1 && block[i].move == 2))
					{
						block[i].moved = 0;
					}
					else if ((block[i].rt.left == 600 && block[i].moved == 0 && block[i].move == 3) || (block[i].rt.left == 600 && block[i].moved == 0 && block[i].move == 4))
					{
						block[i].moved = 1;
					}
					else  if ((block[i].rt.left == 15 && block[i].moved == 1 && block[i].move == 3) || (block[i].rt.left == 15 && block[i].moved == 1 && block[i].move == 4))
					{
						block[i].moved = 0;
					}

					if (uturn == 2) {
						switch (block[i].move) {
						case 1:
							if (block[i].rt.top < 600) {
								block[i].rt.top += 15; block[i].rt.bottom += 15;
							}
							break;
						case 2:
							if (block[i].rt.top > 15) {
								block[i].rt.top -= 15; block[i].rt.bottom -= 15;
							}
							break;

						case 3:
							if (block[i].rt.left < 600) {
								block[i].rt.left += 15; block[i].rt.right += 15;
							}
							break;
						case 4:
							if (block[i].rt.left > 15) {
								block[i].rt.left -= 15; block[i].rt.right -= 15;
							}
							break;
						}
						uturn--;
					}
					else if (uturn == 1) {
						switch (block[i].move) {
						case 1:
							block[i].move = 2;
							break;
						case 2:
							block[i].move = 1;
							break;
						case 3:
							block[i].move = 4;
							break;
						case 4:
							block[i].move = 3;
							break;
						}
						uturn--;
					}
					else if (block[i].move == 1)
					{
						block[i].rt.left += 15; block[i].rt.right += 15;

						if (block[i].rt.left >= 615) {
							if (block[i].moved == 0)
							{
								block[i].rt.top += 15; block[i].rt.bottom += 15;
								block[i].rt.left -= 15; block[i].rt.right -= 15;
							}
							else
							{
								block[i].rt.top -= 15; block[i].rt.bottom -= 15;
								block[i].rt.left -= 15; block[i].rt.right -= 15;
							}
							block[i].move = 2;
						}
					}
					else if (block[i].move == 2)
					{
						block[i].rt.left -= 15; block[i].rt.right -= 15;

						if (block[i].rt.left <= 0) {
							if (block[i].moved == 0)
							{
								block[i].rt.top += 15; block[i].rt.bottom += 15;
								block[i].rt.left += 15; block[i].rt.right += 15;
							}
							else
							{
								block[i].rt.top -= 15; block[i].rt.bottom -= 15;
								block[i].rt.left += 15; block[i].rt.right += 15;
							}
							block[i].move = 1;
						}
					}
					else if (block[i].move == 3)
					{
						block[i].rt.top -= 15; block[i].rt.bottom -= 15;

						if (block[i].rt.top <= 0) {
							if (block[i].moved == 0)
							{
								block[i].rt.right += 15; block[i].rt.left += 15;
								block[i].rt.top += 15; block[i].rt.bottom += 15;
							}
							else
							{
								block[i].rt.right -= 15; block[i].rt.left -= 15;
								block[i].rt.top += 15; block[i].rt.bottom += 15;;
							}
							block[i].move = 4;
						}
					}
					else if (block[i].move == 4)
					{
						block[i].rt.top += 15; block[i].rt.bottom += 15;

						if (block[i].rt.top >= 615) {
							if (block[i].moved == 0)
							{
								block[i].rt.right += 15; block[i].rt.left += 15;
								block[i].rt.top -= 15; block[i].rt.bottom -= 15;
							}
							else
							{
								block[i].rt.right -= 15; block[i].rt.left -= 15;
								block[i].rt.top -= 15; block[i].rt.bottom -= 15;
							}
							block[i].move = 3;
						}
					}
				}
				else if (block[i].num == n && block[i].follow == 1)	//�����κе� �׸��� ��
				{
					for (int c = 0; c < tail_num + 1; c++)
					{
						if (block[c].num == n - 1 && block[c].follow == 1)
						{
							block[i].rt = block[c].rt;
						}
					}
				}

			}

		}
	}

	for (int i = 0; i < 10; ++i) {
		if (block[0].rt.left == obstacle[i].rt.left && block[0].rt.top == obstacle[i].rt.top) {
			switch (block[0].move) {
			case 1:
				block[0].move = 2;
				block[0].rt.left -= 15; block[0].rt.right -= 15;
				break;
			case 2:
				block[0].move = 1;
				block[0].rt.left += 15; block[0].rt.right += 15;
				break;
			case 3:
				block[0].move = 4;
				block[0].rt.top += 15; block[0].rt.bottom += 15;
				break;
			case 4:
				block[0].move = 3;
				block[0].rt.top -= 15; block[0].rt.bottom -= 15;
				break;
			}
		}
	}

	InvalidateRgn(hwnd, NULL, 1);
	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

float LengthPts(float x1, float y1, int x2, int y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, int x2, int y2)
{
	if (LengthPts(x, y, x2, y2) < 7.5) return TRUE;
	else return FALSE;
}

void Printcell(HDC hdc)
{

	for (int i = 0; i <= 40; i++)
	{
		MoveToEx(hdc, 15 + L4 * i, 15, NULL);
		LineTo(hdc, 15 + L4 * i, 615);
		MoveToEx(hdc, 15, 15 + L4 * i, NULL);
		LineTo(hdc, 615, 15 + L4 * i);
	}

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 900, 750, NULL, (HMENU)NULL, hInstance, NULL);

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

	static int start;

	static int mx, my;

	static int boost;

	struct Color {
		int r;
		int g;
		int b;
	};
	static Color head_color;

	static bool head_shape_circle;

	HPEN hpen, oldpen;
	HBRUSH hbrush, oldbrush;

	static POINT point[3];

	//static TCHAR lpOut[1024];

	switch (iMsg)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		start = 0;
		boost = 10;
		block[0].rt = { 15,15,30,30 }; block[0].move = 1; block[0].moved = 0, block[0].num = t, t++, block[0].follow = 1;
		head_color.r = 255;
		head_color.g = 255;
		head_color.b = 0;
		head_shape_circle = 1;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, 670, 0, L"<<��ɾ� ���>>", 10);
		TextOut(hdc, 670, 17, L"s/S:������ ����", 10);
		TextOut(hdc, 670, 34, L"����Ű:���ΰ��� �����̱�", 13);
		TextOut(hdc, 670, 51, L"+:���ΰ��� �ӵ�����", 11);
		TextOut(hdc, 670, 68, L"-:���ΰ��� �ӵ�����", 11);
		TextOut(hdc, 670, 85, L"j/J::���ΰ��� ����", 12);
		TextOut(hdc, 670, 102, L"T/t:����", 6);
		TextOut(hdc, 670, 119, L"Q/q:����", 6);

		//wsprintf(lpOut, L"boost:%d tail:%d a:%d", boost, tail, a);
		//TextOut(hdc, 0, 650, lpOut, lstrlen(lpOut));

		Printcell(hdc);

		if (start == 1) {
			SetTimer(hwnd, 1, 40, (TIMERPROC)TimerProc1);		//���� ��
			SetTimer(hwnd, 2, 40, (TIMERPROC)TimerProc2);		//���ΰ� ��
		}

		for (int i = tail_num; i >= 0; i--)
		{

			if (i == 0) {
				hbrush = CreateSolidBrush(RGB(head_color.r, head_color.g, head_color.b));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			}
			else
			{
				hbrush = CreateSolidBrush(RGB(255, 0, 0));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			}


			if (block[i].move != 0 && i != 0)
				Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);

			if(i == 0 && head_shape_circle == 1)
				Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
			else if(i == 0 && head_shape_circle == 0)
				Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);

			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);

		}

		for (int i = 0; i < 10; ++i) {
			hbrush = CreateSolidBrush(RGB(0, 0, 255));
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

			Rectangle(hdc, obstacle[i].rt.left, obstacle[i].rt.top, obstacle[i].rt.right, obstacle[i].rt.bottom);

			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);
		}


		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			start = 1;
			break;
		case ID_RESET:
			start = 0;

			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			KillTimer(hwnd, 3);
			
			tail = 610, a = 1, b = 1000, c = 1000, d = 5;
			t = 0;
			jump = 0;
			uturn = 0;
			tail_num = 20;

			head_color.r = 255;
			head_color.g = 255;
			head_color.b = 0;
			head_shape_circle = 1;
			boost = 10;
			block[0].rt = { 15,15,30,30 }; block[0].move = 1; block[0].moved = 0, block[0].num = t, t++, block[0].follow = 1;
			for (int i = 1; i < 31; ++i) {
				block[i] = { 0, };
			}

			MessageBox(NULL, L"���µǾ����ϴ�.", L"����", MB_OK);
			break;
		case ID_QUIT:
			DestroyWindow(hwnd);
			break;

		case ID_FAST:
			d = 2;
			break;
		case ID_MEDIUM:
			d = 5;
			break;
		case ID_SLOW:
			d = 8;
			break;

		case ID_CYAN:
			head_color.r = 0;
			head_color.g = 255;
			head_color.b = 255;
			break;
		case ID_PURPLE:
			head_color.r = 255;
			head_color.g = 0;
			head_color.b = 255;
			break;
		case ID_YELLOW:
			head_color.r = 255;
			head_color.g = 255;
			head_color.b = 0;
			break;

		case ID_CIRCLE:
			head_shape_circle = 1;
			break;
		case ID_RECTANGLE:
			head_shape_circle = 0;
			break;

		case ID_TAIL_20:
			if (tail_num != 20)
				MessageBox(NULL, L"������ ������ �ٽ� ���̱� ���ؼ� ������ ���� �ؾ� �մϴ�.", L"����", MB_OK);
			else
				tail_num = 20;
			break;
		case ID_TAIL_25:
			if (tail_num == 30)
				MessageBox(NULL, L"������ ������ �ٽ� ���̱� ���ؼ� ������ ���� �ؾ� �մϴ�.", L"����", MB_OK);
			else
				tail_num = 25;
			break;
		case ID_TAIL_30:
			tail_num = 30;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if ((block[0].move == 1 && my > block[0].rt.bottom && my <= 615) || (block[0].move == 2 && my > block[0].rt.bottom && my <= 615)) {
			block[0].move = 4;
		}
		else if ((block[0].move == 1 && my < block[0].rt.top && my >= 15) || (block[0].move == 2 && my < block[0].rt.top && my >= 15)) {
			block[0].move = 3;
		}
		else if ((block[0].move == 3 && mx > block[0].rt.right && mx <= 615) || (block[0].move == 4 && mx > block[0].rt.right && mx <= 615)) {
			block[0].move = 1;
		}
		else if ((block[0].move == 3 && mx < block[0].rt.left && mx >= 15) || (block[0].move == 4 && mx < block[0].rt.left && mx >= 15)) {
			block[0].move = 2;
		}
		else if (block[0].move == 1 && mx < block[0].rt.left)
			block[0].move = 2;
		else if (block[0].move == 2 && mx > block[0].rt.right)
			block[0].move = 1;
		else if (block[0].move == 3 && my > block[0].rt.bottom)
			block[0].move = 4;
		else if (block[0].move == 4 && my < block[0].rt.top)
			block[0].move = 3;
		else if (InCircle(block[0].rt.left + 7.5, block[0].rt.top + 7.5, mx, my))	//���ΰ��� ���� Ŭ��
		{
			KillTimer(hwnd, 2);
			SetTimer(hwnd, 3, 40, NULL);
		}

		for (int i = 1; i < tail_num + 1; i++)	//��������ִ� ������ Ŭ�� 
		{
			if (InCircle(block[i].rt.left + 7.5, block[i].rt.top + 7.5, mx, my) && block[i].follow == 1)
			{
				t = block[i].num;
				break;
			}
		}

		for (int i = 1; i < tail_num + 1; i++)
		{
			if (block[i].num >= t)
			{
				block[i].num = 0;
				block[i].follow = 0;
				block[i].move = rand() % 5 + 1;
				block[i].moved = rand() % 2;
			}
		}

		break;

	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (mx >= 15 && mx <= 615 && my >= 15 && my <= 615) {
			int low, column;

			low = (int)((mx - 15) / L4);
			column = (int)((my - 15) / L4);

			obstacle[obstacle_indx].rt.right = low * L4 + 15 + L4;
			obstacle[obstacle_indx].rt.left = obstacle[obstacle_indx].rt.right - L4;
			obstacle[obstacle_indx].rt.bottom = column * L4 + 15 + L4;
			obstacle[obstacle_indx].rt.top = obstacle[obstacle_indx].rt.bottom - L4;
			++obstacle_indx;
			if (obstacle_indx == 10)
				obstacle_indx = 0;
		}
		break;

	case WM_KEYDOWN:		//��ҹ��� ����X (+ ��� 'A'�� ���� �빮�ڸ� Ȯ�� ����)
		switch (wParam)
		{
		case 'S':
			start = 1;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'Q':
			DestroyWindow(hwnd);
			break;
		case VK_RIGHT:
			block[0].move = 1;
			break;
		case VK_LEFT:
			block[0].move = 2;
			break;
		case VK_UP:
			block[0].move = 3;
			break;
		case VK_DOWN:
			block[0].move = 4;
			break;
		case 'J':
			if (jump == 0)
				jump = 6;
			break;
		case 'T':
			if (uturn == 0)
				uturn = 2;
			break;
		}
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case '+':
			d--;
			if (d <= 1) {
				d = 1;
			}
			break;
		case '-':
			d++;
			if (d >= 8) {
				d = 8;
			}
			break;
		}
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 3:

			if ((block[0].rt.top == 600 && block[0].moved == 0 && block[0].move == 1) || (block[0].rt.top == 600 && block[0].moved == 0 && block[0].move == 2))
			{
				block[0].moved = 1;
			}
			else if ((block[0].rt.top == 15 && block[0].moved == 1 && block[0].move == 1) || (block[0].rt.top == 15 && block[0].moved == 1 && block[0].move == 2))
			{
				block[0].moved = 0;
			}
			else if ((block[0].rt.left == 600 && block[0].moved == 0 && block[0].move == 3) || (block[0].rt.left == 600 && block[0].moved == 0 && block[0].move == 4))
			{
				block[0].moved = 1;
			}
			else  if ((block[0].rt.left == 15 && block[0].moved == 1 && block[0].move == 3) || (block[0].rt.left == 15 && block[0].moved == 1 && block[0].move == 4))
			{
				block[0].moved = 0;
			}

			if (block[0].move == 1)
			{
				block[0].rt.left += 15; block[0].rt.right += 15;
				boost--;
				if (boost <= 0)
				{
					boost = 10;
					KillTimer(hwnd, 3);
					SetTimer(hwnd, 2, 40, NULL);
				}

				if (block[0].rt.left >= 615) {
					if (block[0].moved == 0)
					{
						block[0].rt.top += 15; block[0].rt.bottom += 15;
						block[0].rt.left = 600; block[0].rt.right = 615;
					}
					else
					{
						block[0].rt.top -= 15; block[0].rt.bottom -= 15;
						block[0].rt.left = 600; block[0].rt.right = 615;
					}
					block[0].move = 2;
				}
			}
			else if (block[0].move == 2)
			{
				block[0].rt.left -= 15; block[0].rt.right -= 15;
				boost--;
				if (boost <= 0)
				{
					boost = 10;
					KillTimer(hwnd, 3);
					SetTimer(hwnd, 2, 40, NULL);
				}

				if (block[0].rt.left <= 0) {
					if (block[0].moved == 0)
					{
						block[0].rt.top += 15; block[0].rt.bottom += 15;
						block[0].rt.left = 15; block[0].rt.right = 30;
					}
					else
					{
						block[0].rt.top -= 15; block[0].rt.bottom -= 15;
						block[0].rt.left = 15; block[0].rt.right = 30;
					}
					block[0].move = 1;
				}
			}
			else if (block[0].move == 3)
			{
				block[0].rt.top -= 15; block[0].rt.bottom -= 15;
				boost--;
				if (boost <= 0)
				{
					boost = 10;
					KillTimer(hwnd, 3);
					SetTimer(hwnd, 2, 40, NULL);
				}

				if (block[0].rt.top <= 0) {
					if (block[0].moved == 0)
					{
						block[0].rt.right += 15; block[0].rt.left += 15;
						block[0].rt.top = 15; block[0].rt.bottom = 30;
					}
					else
					{
						block[0].rt.right -= 15; block[0].rt.left -= 15;
						block[0].rt.top = 15; block[0].rt.bottom = 30;
					}
					block[0].move = 4;
				}
			}
			else if (block[0].move == 4)
			{
				block[0].rt.top += 15; block[0].rt.bottom += 15;
				boost--;
				if (boost <= 0)
				{
					boost = 10;
					KillTimer(hwnd, 3);
					SetTimer(hwnd, 2, 40, NULL);
				}

				if (block[0].rt.top >= 615) {
					if (block[0].moved == 0)
					{
						block[0].rt.right += 15; block[0].rt.left += 15;
						block[0].rt.top = 600; block[0].rt.bottom = 615;
					}
					else
					{
						block[0].rt.right -= 15; block[0].rt.left -= 15;
						block[0].rt.top = 600; block[0].rt.bottom = 615;
					}
					block[0].move = 3;
				}
			}

			for (int i = 0; i < 10; ++i) {
				if (block[0].rt.left == obstacle[i].rt.left && block[0].rt.top == obstacle[i].rt.top) {
					switch (block[0].move) {
					case 1:
						block[0].move = 2;
						block[0].rt.left -= 15; block[0].rt.right -= 15;
						break;
					case 2:
						block[0].move = 1;
						block[0].rt.left += 15; block[0].rt.right += 15;
						break;
					case 3:
						block[0].move = 4;
						block[0].rt.top += 15; block[0].rt.bottom += 15;
						break;
					case 4:
						block[0].move = 3;
						block[0].rt.top -= 15; block[0].rt.bottom -= 15;
						break;
					}
				}
			}

			break;

		}
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		KillTimer(hwnd, 3);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
