#include <windows.h>
#include <tchar.h>
#include <time.h>
#include "resource.h"
#define _USE_MATH_DEFINES
#include <math.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 4-3";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

typedef struct ELLIPSE
{
	float x, y, r;	//x,y - 중심점좌표,r - 반지름
	int R, G, B;
	int mangle;		//위성원을 위한 각도 변수
	int angle1, angle2, angle3, angle4;		//원의 자전을 위한 변수
	BOOL turn;	//1 - 시계방향 / 0 - 반시계방향
}ELLIPSE;

double getRadian(int num)
{
	return num * (M_PI / 180);
}

bool IsPointInCircle(float cx, float cy, float cr, float px, float py)
{
	float deltaX = cx - px;
	float deltaY = cy - py;

	float length = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (length > cr)
		return FALSE;

	return TRUE;
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
	HDC hdc;
	PAINTSTRUCT ps;

	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	static BOOL stop;	//1 - stop / 0 - move

	static BOOL contrast; //1 - 반전 / 0 - 반전X

	static int num;	//원의 갯수를 저장하는 변수(10~20개)

	static BOOL crand[20];	//임의의 원 반전을 위한 변수

	static ELLIPSE one[20];

	static ELLIPSE moon[20];	// 위성원

	static float mx, my;

	bool flag;

	srand(time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 40, NULL);

		num = 10;//rand() % 11 + 10;

		for (int i = 0; i < 20; i++)
		{
			if (i < num) {
				crand[i] = 0;// 반전원을 위한 이진변수 초기화

				//원
				one[i].x = rand() % 600 + 50;
				one[i].y = rand() % 400 + 50;
				one[i].r = rand() % 50 + 40;

				one[i].R = rand() % 255;
				one[i].G = rand() % 255;
				one[i].B = rand() % 255;

				one[i].turn = rand() % 2;

				one[i].angle1 = 0;
				one[i].angle2 = 45;
				one[i].angle3 = 90;
				one[i].angle4 = 135;


				//위성원
				moon[i].mangle = rand() % 365;
				moon[i].x = one[i].x + (one[i].r + 10) * cos(getRadian(moon[i].mangle));
				moon[i].y = one[i].y + (one[i].r + 10) * sin(getRadian(moon[i].mangle));
				moon[i].turn = rand() % 2;
			}
			else {
				one[i].r = 0;
			}
		}

		stop = 0;
		contrast = 0;

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < 20; i++) {
			if (one[i].r != 0) {
				if (contrast == 1 && crand[i] == 1) {
					hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));	// 원의 테두리 펜색깔이 사라지는 것을 막기 위해
					oldpen = (HPEN)SelectObject(hdc, hpen);
					SetROP2(hdc, R2_NOTCOPYPEN);		//반전 시키기(흰색 배경과 비교)
				}
				else {
					SetROP2(hdc, R2_COPYPEN);	//원래 그대로 그리기
					hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldpen = (HPEN)SelectObject(hdc, hpen);
				}

				//원
				hbrush = CreateSolidBrush(RGB(one[i].R, one[i].G, one[i].B));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

				Ellipse(hdc, one[i].x - one[i].r, one[i].y - one[i].r, one[i].x + one[i].r, one[i].y + one[i].r);

				SelectObject(hdc, oldbrush);
				DeleteObject(hbrush);

				SetROP2(hdc, R2_COPYPEN);//원래 그대로 그리기
				hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldpen = (HPEN)SelectObject(hdc, hpen);

				//원 내부 톱니바퀴
				MoveToEx(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle1)), one[i].y + one[i].r * sin(getRadian(one[i].angle1)), NULL);
				LineTo(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle1 + 180)), one[i].y + one[i].r * sin(getRadian(one[i].angle1 + 180)));
				MoveToEx(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle2)), one[i].y + one[i].r * sin(getRadian(one[i].angle2)), NULL);
				LineTo(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle2 + 180)), one[i].y + one[i].r * sin(getRadian(one[i].angle2 + 180)));
				MoveToEx(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle3)), one[i].y + one[i].r * sin(getRadian(one[i].angle3)), NULL);
				LineTo(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle3 + 180)), one[i].y + one[i].r * sin(getRadian(one[i].angle3 + 180)));
				MoveToEx(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle4)), one[i].y + one[i].r * sin(getRadian(one[i].angle4)), NULL);
				LineTo(hdc, one[i].x + one[i].r * cos(getRadian(one[i].angle4 + 180)), one[i].y + one[i].r * sin(getRadian(one[i].angle4 + 180)));

				//위성원
				hbrush = CreateSolidBrush(RGB(255, 255, 0));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

				Ellipse(hdc, moon[i].x - 10, moon[i].y - 10, moon[i].x + 10, moon[i].y + 10);

				SelectObject(hdc, oldbrush);
				DeleteObject(hbrush);

				SelectObject(hdc, oldpen);
				DeleteObject(hpen);
			}
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		for (int i = 0; i < 20; i++) {
			if (one[i].r != 0) {
				if (one[i].turn == 1 && stop == 0) {
					one[i].angle1++; one[i].angle2++; one[i].angle3++; one[i].angle4++;

					if (one[i].angle1 > 45)
						one[i].angle1 = 0;
					if (one[i].angle2 > 90)
						one[i].angle2 = 45;
					if (one[i].angle3 > 135)
						one[i].angle3 = 90;
					if (one[i].angle4 > 180)
						one[i].angle4 = 135;
				}
				else if (one[i].turn == 0 && stop == 0) {
					one[i].angle1--; one[i].angle2--; one[i].angle3--; one[i].angle4--;

					if (one[i].angle1 < 0)
						one[i].angle1 = 45;
					if (one[i].angle2 < 45)
						one[i].angle2 = 90;
					if (one[i].angle3 < 90)
						one[i].angle3 = 135;
					if (one[i].angle4 < 135)
						one[i].angle4 = 180;
				}

				if (moon[i].turn == 1 && stop == 0) {
					moon[i].mangle++;
					moon[i].x = one[i].x + (one[i].r + 10) * cos(getRadian(moon[i].mangle));
					moon[i].y = one[i].y + (one[i].r + 10) * sin(getRadian(moon[i].mangle));


				}
				else if (moon[i].turn == 0 && stop == 0) {
					moon[i].mangle--;
					moon[i].x = one[i].x + (one[i].r + 10) * cos(getRadian(moon[i].mangle));
					moon[i].y = one[i].y + (one[i].r + 10) * sin(getRadian(moon[i].mangle));


				}
			}
		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_MOVE_1:
			for (int i = 0; i < num; i++)
			{
				if (one[i].turn == 0)
					one[i].turn = 1;
				else
					one[i].turn = 0;
			}
			break;

		case ID_MOVE_2:
			for (int i = 0; i < num; i++)
			{
				if (moon[i].turn == 0)
					moon[i].turn = 1;
				else
					moon[i].turn = 0;
			}
			break;

		case ID_MOVE_3:
			if (stop == 0)
				stop = 1;
			else
				stop = 0;
			break;


		case ID_CHANGE_C:
			if (contrast == 0) {
				contrast = 1;
				for (int i = 0; i < num; i++)
				{
					crand[i] = rand() % 2;
				}
			}
			else
				contrast = 0;
			break;

		case ID_CHANGE_RED:
			for (int i = 0; i < 20; i++) {
				if (one[i].r != 0) {
					one[i].R = 255;
					one[i].G = 0;
					one[i].B = 0;
				}
			}
			break;

		case ID_CHANGE_GREEN:
			for (int i = 0; i < 20; i++) {
				if (one[i].r != 0) {
					one[i].R = 0;
					one[i].G = 255;
					one[i].B = 0;
				}
			}
			break;

		case ID_CHANGE_BLUE:
			for (int i = 0; i < 20; i++) {
				if (one[i].r != 0) {
					one[i].R = 0;
					one[i].G = 0;
					one[i].B = 255;
				}
			}
			break;

		case ID_CHANGE_RANDOM:
			for (int i = 0; i < 20; i++) {
				if (one[i].r != 0) {
					one[i].R = rand() % 256;
					one[i].G = rand() % 256;
					one[i].B = rand() % 256;
				}
			}
			break;


		case ID_GAME_RESET:
			num = 10; //rand() % 11 + 10;

			for (int i = 0; i < 20; i++)
			{
				if (i < num) {
					crand[i] = 0;// 반전원을 위한 이진변수 초기화

					//원
					one[i].x = rand() % 600 + 50;
					one[i].y = rand() % 400 + 50;
					one[i].r = rand() % 50 + 40;

					one[i].R = rand() % 255;
					one[i].G = rand() % 255;
					one[i].B = rand() % 255;

					one[i].turn = rand() % 2;

					one[i].angle1 = 0;
					one[i].angle2 = 45;
					one[i].angle3 = 90;
					one[i].angle4 = 135;


					//위성원
					moon[i].mangle = rand() % 365;
					moon[i].x = one[i].x + (one[i].r + 10) * cos(getRadian(moon[i].mangle));
					moon[i].y = one[i].y + (one[i].r + 10) * sin(getRadian(moon[i].mangle));
					moon[i].turn = rand() % 2;
				}
				else {
					one[i].r = 0;
				}
			}

			stop = 0;
			contrast = 0;
			break;

		case ID_GAME_QUIT:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
	
		flag = false;
		
		for (int i = 0; i < 20; i++) {
			if (one[i].r != 0) {
				if (IsPointInCircle(one[i].x, one[i].y, one[i].r, mx, my)) {
					moon[i].turn = !moon[i].turn;

					flag = true;
				}
			}
		}

		if (!flag) {
			for (int i = 0; i < 20; i++) {
				if (one[i].r == 0) {

					crand[i] = 0;// 반전원을 위한 이진변수 초기화

					//원
					one[i].x = mx;
					one[i].y = my;
					one[i].r = rand() % 50 + 40;

					one[i].R = rand() % 255;
					one[i].G = rand() % 255;
					one[i].B = rand() % 255;

					one[i].turn = rand() % 2;

					one[i].angle1 = 0;
					one[i].angle2 = 45;
					one[i].angle3 = 90;
					one[i].angle4 = 135;


					//위성원
					moon[i].mangle = rand() % 365;
					moon[i].x = one[i].x + (one[i].r + 10) * cos(getRadian(moon[i].mangle));
					moon[i].y = one[i].y + (one[i].r + 10) * sin(getRadian(moon[i].mangle));
					moon[i].turn = rand() % 2;

					break;
				}
			}
		}

		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}