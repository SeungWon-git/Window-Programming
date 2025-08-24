#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<stdlib.h>
#include<ctime>

#define L3 20
#define L4 15
#define L5 13

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-11";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

typedef struct BLOCK
{
	RECT rt;
	int shape;	//1-원 2-사각형 3-삼각형
	int r, g, b;
	bool size_up;
}BLOCK;

struct Color
{
	int r, g, b;
};

void Printcell(HDC hdc, int* boardnum)
{
	if (*boardnum == 3) {
		for (int i = 0; i <= 30; i++)
		{
			MoveToEx(hdc, 15 + L3 * i, 15, NULL);
			LineTo(hdc, 15 + L3 * i, 615);
			MoveToEx(hdc, 15, 15 + L3 * i, NULL);
			LineTo(hdc, 615, 15 + L3 * i);
		}
	}
	else if (*boardnum == 4) {
		for (int i = 0; i <= 40; i++)
		{
			MoveToEx(hdc, 15 + L4 * i, 15, NULL);
			LineTo(hdc, 15 + L4 * i, 615);
			MoveToEx(hdc, 15, 15 + L4 * i, NULL);
			LineTo(hdc, 615, 15 + L4 * i);
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 940, 750, NULL, (HMENU)NULL, hInstance, NULL);

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

	HPEN hpen, oldpen;
	HBRUSH hbrush, oldbrush;

	static int boardnum;	//3=30X30 ,4=40X40 ,5=50X50 

	static int shape;	//1=원, 2=사각형, 3=삼각형

	static POINT point[3];

	static BLOCK block[10];

	static int i, n;

	static bool selection;

	static int num;	//num은 선택모드에서 해당(몇번째) 인덱스인가를 지정하는 변수

	static int order; //order은 도형갯수가 10개를 넘어갈때 인덱스 순서를 바꿔주기위한 변수

	static int pause;

	static Color circ, rect, tria;	//'c' 명령어 - 랜덤색 정보
	static bool bool_input_c[3];	//'c' 명령어 - 랜덤색출력: 1 / 원색출력:0
									// [0] - 원, [1] - 삼각형, [2]-사각형

	static TCHAR lpOut[1024];

	switch (iMsg)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		boardnum = 4;
		shape = 1;
		block[0] = { 0, }; block[1] = { 0, }; block[2] = { 0, }; block[3] = { 0, }; block[4] = { 0, };
		block[5] = { 0, }; block[6] = { 0, }; block[7] = { 0, }; block[8] = { 0, }; block[9] = { 0, };
		i = 0;
		selection = 0;
		order = 0;
		pause = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, 670, 0, L"<<명령어 목록>>", 10);
		TextOut(hdc, 670, 15, L"s:30X30,m:40X40,l:50X50", 23);
		TextOut(hdc, 670, 30, L"E/e:원그리기", 8);
		TextOut(hdc, 670, 45, L"T/t:삼각형그리기", 10);
		TextOut(hdc, 670, 60, L"R/r:사각형그리기", 10);
		TextOut(hdc, 670, 75, L"숫자키보드(0~9):도형선택", 15);
		TextOut(hdc, 670, 90, L" / :도형선택취소", 10);
		TextOut(hdc, 670, 105, L"방향키:선택도형 움직이기", 13);
		TextOut(hdc, 670, 120, L"+/-:선택도형 크기 증가/감소", 17);
		TextOut(hdc, 670, 135, L"C/c:선택도형들 색변경/원상복구", 18);
		TextOut(hdc, 670, 150, L"D/d:선택도형 지우기", 12);
		TextOut(hdc, 670, 165, L"P/p:초기화", 7);
		TextOut(hdc, 670, 180, L"Q/q:종료", 6);

		//wsprintf(lpOut, L"i:%d num:%d order:%d selection:%d pause:%d", i, num, order, selection, pause);
		//TextOut(hdc, 0, 650, lpOut, lstrlen(lpOut));

		Printcell(hdc, &boardnum);

		for (int i = 0; i < 10; i++)
		{
			if (block[i].shape != 0)
			{
				if (bool_input_c[0] == 1 && block[i].shape == 1) {
					hbrush = CreateSolidBrush(RGB(circ.r, circ.g, circ.b));
					oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
				}
				else if (bool_input_c[1] == 1 && block[i].shape == 2) {
					hbrush = CreateSolidBrush(RGB(rect.r, rect.g, rect.b));
					oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
				}
				else if (bool_input_c[2] == 1 && block[i].shape == 3) {
					hbrush = CreateSolidBrush(RGB(tria.r, tria.g, tria.b));
					oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
				}
				else {
					hbrush = CreateSolidBrush(RGB(block[i].r, block[i].g, block[i].b));
					oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
				}

				if (block[i].shape == 1)
				{
					if (boardnum == 3)
						Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
					else if (boardnum == 4)
						Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
					else if (boardnum == 5)
						Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
				}
				else if (block[i].shape == 2)
				{
					if (boardnum == 3)
						Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
					else if (boardnum == 4)
						Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
					else if (boardnum == 5)
						Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
				}
				else if (block[i].shape == 3)
				{
					if (boardnum == 3)
					{
						point[0] = { (block[i].rt.left + block[i].rt.right) / 2,block[i].rt.top };
						point[1] = { block[i].rt.left,block[i].rt.bottom };
						point[2] = { block[i].rt.right, block[i].rt.bottom };
						Polygon(hdc, point, 3);
					}
					else if (boardnum == 4)
					{
						point[0] = { (block[i].rt.left + block[i].rt.right) / 2,block[i].rt.top };
						point[1] = { block[i].rt.left,block[i].rt.bottom };
						point[2] = { block[i].rt.right, block[i].rt.bottom };
						Polygon(hdc, point, 3);
					}
					else if (boardnum == 5)
					{
						point[0] = { (block[i].rt.left + block[i].rt.right) / 2,block[i].rt.top };
						point[1] = { block[i].rt.left,block[i].rt.bottom };
						point[2] = { block[i].rt.right, block[i].rt.bottom };
						Polygon(hdc, point, 3);
					}
				}

				SelectObject(hdc, oldbrush);
				DeleteObject(hbrush);
			}
		}

		if (selection == 1)		//선택 도형이 맨앞에 오게하기 위해서
		{
			for (int i = 0; i < 10; i++) {
				if (block[i].shape != 0 && num == i)
				{
					if (bool_input_c[0] == 1 && block[i].shape == 1) {
						hbrush = CreateSolidBrush(RGB(circ.r, circ.g, circ.b));
						oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
					}
					else if (bool_input_c[1] == 1 && block[i].shape == 2) {
						hbrush = CreateSolidBrush(RGB(rect.r, rect.g, rect.b));
						oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
					}
					else if (bool_input_c[2] == 1 && block[i].shape == 3) {
						hbrush = CreateSolidBrush(RGB(tria.r, tria.g, tria.b));
						oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
					}
					else {
						hbrush = CreateSolidBrush(RGB(block[i].r, block[i].g, block[i].b));
						oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
					}
					hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
					oldpen = (HPEN)SelectObject(hdc, hpen);

					if (block[i].shape == 1)
					{
						if (boardnum == 3)
							Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
						else if (boardnum == 4)
							Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
						else if (boardnum == 5)
							Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
					}
					else if (block[i].shape == 2)
					{
						if (boardnum == 3)
							Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
						else if (boardnum == 4)
							Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
						else if (boardnum == 5)
							Rectangle(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);
					}
					else if (block[i].shape == 3)
					{
						if (boardnum == 3)
						{
							point[0] = { (block[i].rt.left + block[i].rt.right) / 2,block[i].rt.top };
							point[1] = { block[i].rt.left,block[i].rt.bottom };
							point[2] = { block[i].rt.right, block[i].rt.bottom };
							Polygon(hdc, point, 3);
						}
						else if (boardnum == 4)
						{
							point[0] = { (block[i].rt.left + block[i].rt.right) / 2,block[i].rt.top };
							point[1] = { block[i].rt.left, block[i].rt.bottom };
							point[2] = { block[i].rt.right, block[i].rt.bottom };
							Polygon(hdc, point, 3);
						}
						else if (boardnum == 5)
						{
							point[0] = { (block[i].rt.left + block[i].rt.right) / 2,block[i].rt.top };
							point[1] = { block[i].rt.left,block[i].rt.bottom };
							point[2] = { block[i].rt.right, block[i].rt.bottom };
							Polygon(hdc, point, 3);
						}
					}

					SelectObject(hdc, oldpen);
					DeleteObject(hpen);
					SelectObject(hdc, oldbrush);
					DeleteObject(hbrush);
				}
			}
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:		//대소문자 구분X (+ 대신 'A'와 같이 대문자만 확인 가능)
		switch (wParam)
		{
		case 'S':
			boardnum = 3;
			for (int i = 0; i < 10; i++)
			{
				block[i].shape = 0;
			}
			i = 0;
			selection = 0;
			order = 0;
			pause = 0;
			break;

		case 'M':
			boardnum = 4;
			for (int i = 0; i < 10; i++)
			{
				block[i].shape = 0;
			}
			i = 0;
			selection = 0;
			order = 0;
			pause = 0;
			break;

		case 'L':
			boardnum = 5;
			for (int i = 0; i < 10; i++)
			{
				block[i].shape = 0;
			}
			i = 0;
			selection = 0;
			order = 0;
			pause = 0;
			break;

		case 'E':

			n = i % 10;
			if (pause >= 1)
			{
				pause--;
			}
			else if (i >= 10) {
				order++;
				num++;
				if (order == 10) {
					order = 0;
				}
				if (num == 10) {
					num = 0;
				}
			}
			if (boardnum == 3) {
				block[n].rt.left = rand() % 30 * L3 + 15;
				block[n].rt.top = rand() % 30 * L3 + 15;
				block[n].rt.right = block[n].rt.left + L3;
				block[n].rt.bottom = block[n].rt.top + L3;
			}
			else if (boardnum == 4) {
				block[n].rt.left = rand() % 40 * L4 + 15;
				block[n].rt.top = rand() % 40 * L4 + 15;
				block[n].rt.right = block[n].rt.left + L4;
				block[n].rt.bottom = block[n].rt.top + L4;
			}
			else if (boardnum == 5) {
				block[n].rt.left = rand() % 50 * L5 + 15;
				block[n].rt.top = rand() % 50 * L5 + 15;
				block[n].rt.right = block[n].rt.left + L5;
				block[n].rt.bottom = block[n].rt.top + L5;
			}
			block[n].shape = 1;
			block[n].r = rand() % 256; block[n].g = rand() % 256; block[n].b = rand() % 256;
			i++;
			break;

		case 'T':

			n = i % 10;
			if (pause >= 1)
			{
				pause--;
			}
			else if (i >= 10) {
				order++;
				num++;
				if (order == 10) {
					order = 0;
				}
				if (num == 10) {
					num = 0;
				}
			}
			if (boardnum == 3) {
				block[n].rt.left = rand() % 30 * L3 + 15;
				block[n].rt.top = rand() % 30 * L3 + 15;
				block[n].rt.right = block[n].rt.left + L3;
				block[n].rt.bottom = block[n].rt.top + L3;
			}
			else if (boardnum == 4) {
				block[n].rt.left = rand() % 40 * L4 + 15;
				block[n].rt.top = rand() % 40 * L4 + 15;
				block[n].rt.right = block[n].rt.left + L4;
				block[n].rt.bottom = block[n].rt.top + L4;
			}
			else if (boardnum == 5) {
				block[n].rt.left = rand() % 50 * L5 + 15;
				block[n].rt.top = rand() % 50 * L5 + 15;
				block[n].rt.right = block[n].rt.left + L5;
				block[n].rt.bottom = block[n].rt.top + L5;
			}
			block[n].shape = 3;
			block[n].r = rand() % 256; block[n].g = rand() % 256; block[n].b = rand() % 256;
			i++;
			break;

		case 'R':
			n = i % 10;
			if (pause >= 1)
			{
				pause--;
			}
			else if (i >= 10) {
				order++;
				num++;
				if (order == 10) {
					order = 0;
				}
				if (num == 10) {
					num = 0;
				}
			}
			if (boardnum == 3) {
				block[n].rt.left = rand() % 30 * L3 + 15;
				block[n].rt.top = rand() % 30 * L3 + 15;
				block[n].rt.right = block[n].rt.left + L3;
				block[n].rt.bottom = block[n].rt.top + L3;
			}
			else if (boardnum == 4) {
				block[n].rt.left = rand() % 40 * L4 + 15;
				block[n].rt.top = rand() % 40 * L4 + 15;
				block[n].rt.right = block[n].rt.left + L4;
				block[n].rt.bottom = block[n].rt.top + L4;
			}
			else if (boardnum == 5) {
				block[n].rt.left = rand() % 50 * L5 + 15;
				block[n].rt.top = rand() % 50 * L5 + 15;
				block[n].rt.right = block[n].rt.left + L5;
				block[n].rt.bottom = block[n].rt.top + L5;
			}
			block[n].shape = 2;
			block[n].r = rand() % 256; block[n].g = rand() % 256; block[n].b = rand() % 256;
			i++;
			break;

		case 'C':
			if (bool_input_c[block[num].shape - 1] == 0) {
				switch (block[num].shape) {
				case 1:
					circ.r = rand() % 256;
					circ.g = rand() % 256;
					circ.b = rand() % 256;
					break;

				case 2:
					rect.r = rand() % 256;
					rect.g = rand() % 256;
					rect.b = rand() % 256;
					break;

				case 3:
					tria.r = rand() % 256;
					tria.g = rand() % 256;
					tria.b = rand() % 256;
					break;
				}

				bool_input_c[block[num].shape - 1] = 1;
			}
			else
				bool_input_c[block[num].shape - 1] = 0;
			break;

		case 'D':
			if (selection == 1 && block[num].shape != 0)
			{
				if (order == 0)
				{
					if (i <= 10) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}

						block[9].shape = 0;
						i--;
						if (i == -1)
							i = 0;
					}
					else
					{
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}

						block[9].shape = 0;
						i = 9;
					}
				}
				else if (order == 1)
				{
					if (num != 0) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];

						block[0].shape = 0;
						i--;
						pause++;
					}
					else
					{
						block[0].shape = 0;
						i--;
						pause++;
					}
				}
				else if (order == 2)
				{
					if (num != 0 && num != 1) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];

						block[1].shape = 0;
						i--;
						pause++;
					}
					else
					{
						for (int i = num; i < 1; i++)
						{
							block[i] = block[i + 1];
						}

						block[1].shape = 0;
						i--;
						pause++;
					}
				}
				else if (order == 3)
				{
					if (num != 0 && num != 1 && num != 2) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];

						block[2].shape = 0;
						i--;
						pause++;
					}
					else
					{
						for (int i = num; i < 2; i++)
						{
							block[i] = block[i + 1];
						}

						block[2].shape = 0;
						i--;
						pause++;
					}
				}
				else if (order == 4)
				{
					if (num != 0 && num != 1 && num != 2 && num != 3) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];
						block[2] = block[3];

						block[3].shape = 0;
						i--;
						pause++;
					}
					else
					{
						for (int i = num; i < 3; i++)
						{
							block[i] = block[i + 1];
						}

						block[3].shape = 0;
						i--;
						pause++;
					}
				}
				else if (order == 5)
				{
					if (num != 0 && num != 1 && num != 2 && num != 3 && num != 4) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];
						block[2] = block[3];
						block[3] = block[4];
					}
					else
					{
						for (int i = num; i < 4; i++)
						{
							block[i] = block[i + 1];
						}
					}
					block[4].shape = 0;
					i--;
					pause++;
				}
				else if (order == 6)
				{
					if (num != 0 && num != 1 && num != 2 && num != 3 && num != 4 && num != 5) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];
						block[2] = block[3];
						block[3] = block[4];
						block[4] = block[5];
					}
					else
					{
						for (int i = num; i < 5; i++)
						{
							block[i] = block[i + 1];
						}
					}
					block[5].shape = 0;
					i--;
					pause++;
				}
				else if (order == 7)
				{
					if (num != 0 && num != 1 && num != 2 && num != 3 && num != 4 && num != 5 && num != 6) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];
						block[2] = block[3];
						block[3] = block[4];
						block[4] = block[5];
						block[5] = block[6];
					}
					else
					{
						for (int i = num; i < 6; i++)
						{
							block[i] = block[i + 1];
						}
					}
					block[6].shape = 0;
					i--;
					pause++;
				}
				else if (order == 8)
				{
					if (num != 0 && num != 1 && num != 2 && num != 3 && num != 4 && num != 5 && num != 6 && num != 7) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];
						block[2] = block[3];
						block[3] = block[4];
						block[4] = block[5];
						block[5] = block[6];
						block[6] = block[7];
					}
					else
					{
						for (int i = num; i < 7; i++)
						{
							block[i] = block[i + 1];
						}
					}
					block[7].shape = 0;
					i--;
					pause++;
				}
				else if (order == 9)
				{
					if (num != 0 && num != 1 && num != 2 && num != 3 && num != 4 && num != 5 && num != 6 && num != 7 && num != 8) {
						for (int i = num; i < 9; i++)
						{
							block[i] = block[i + 1];
						}
						block[9] = block[0];
						block[0] = block[1];
						block[1] = block[2];
						block[2] = block[3];
						block[3] = block[4];
						block[4] = block[5];
						block[5] = block[6];
						block[6] = block[7];
						block[7] = block[8];
					}
					else
					{
						for (int i = num; i < 8; i++)
						{
							block[i] = block[i + 1];
						}
					}
					block[8].shape = 0;
					i--;
					pause++;
				}
			}
			break;

		case 'P':
			for (int i = 0; i < 10; i++)
			{
				block[i].shape = 0;
			}
			i = 0;
			selection = 0;
			order = 0;
			pause = 0;
			break;

		case 'Q':
			DestroyWindow(hwnd);
			break;

		case VK_RIGHT:
			if (selection == 1 && block[num].shape != 0) {
				if (boardnum == 3)
				{
					block[num].rt.left += L3;
					block[num].rt.right += L3;

					if (block[num].rt.left == 615)
					{
						block[num].rt.right = 15 + block[num].rt.right - block[num].rt.left;
						block[num].rt.left = 15;
					}
				}
				else if (boardnum == 4)
				{
					block[num].rt.left += L4;
					block[num].rt.right += L4;

					if (block[num].rt.left == 615)
					{
						block[num].rt.right = 15 + block[num].rt.right - block[num].rt.left;
						block[num].rt.left = 15;
					}
				}
				else if (boardnum == 5)
				{
					block[num].rt.left += L5;
					block[num].rt.right += L5;

					if (block[num].rt.left == 665)
					{
						block[num].rt.right = 15 + block[num].rt.right - block[num].rt.left;
						block[num].rt.left = 15;
					}
				}
			}
			break;

		case VK_LEFT:
			if (selection == 1 && block[num].shape != 0) {
				if (boardnum == 3)
				{
					block[num].rt.left -= L3;
					block[num].rt.right -= L3;

					if (block[num].rt.left == 15 - L3)
					{
						block[num].rt.right = 615 - L3 + block[num].rt.right - block[num].rt.left;
						block[num].rt.left = 615 - L3;
					}
				}
				else if (boardnum == 4)
				{
					block[num].rt.left -= L4;
					block[num].rt.right -= L4;

					if (block[num].rt.left == 15 - L4)
					{
						block[num].rt.right = 615 - L4 + block[num].rt.right - block[num].rt.left;
						block[num].rt.left = 615 - L4;
					}
				}
				else if (boardnum == 5)
				{
					block[num].rt.left -= L5;
					block[num].rt.right -= L5;

					if (block[num].rt.left == 15 - L5)
					{
						block[num].rt.right = 665 - L5 + block[num].rt.right - block[num].rt.left;
						block[num].rt.left = 665 - L5;
					}
				}
			}
			break;

		case VK_UP:
			if (selection == 1 && block[num].shape != 0) {
				if (boardnum == 3)
				{
					block[num].rt.top -= L3;
					block[num].rt.bottom -= L3;

					if (block[num].rt.top == 15 - L3)
					{
						block[num].rt.bottom = 615 - L3 + block[num].rt.right - block[num].rt.left;
						block[num].rt.top = 615 - L3;
					}
				}
				else if (boardnum == 4)
				{
					block[num].rt.top -= L4;
					block[num].rt.bottom -= L4;

					if (block[num].rt.top == 15 - L4)
					{
						block[num].rt.bottom = 615 - L4 + block[num].rt.right - block[num].rt.left;
						block[num].rt.top = 615 - L4;
					}
				}
				else if (boardnum == 5)
				{
					block[num].rt.top -= L5;
					block[num].rt.bottom -= L5;

					if (block[num].rt.top == 15 - L5)
					{
						block[num].rt.bottom = 665 - L5 + block[num].rt.right - block[num].rt.left;
						block[num].rt.top = 665 - L5;
					}
				}
			}
			break;

		case VK_DOWN:
			if (selection == 1 && block[num].shape != 0) {
				if (boardnum == 3)
				{
					block[num].rt.top += L3;
					block[num].rt.bottom += L3;

					if (block[num].rt.top == 615)
					{
						block[num].rt.bottom = 15 + block[num].rt.right - block[num].rt.left;
						block[num].rt.top = 15;
					}
				}
				else if (boardnum == 4)
				{
					block[num].rt.top += L4;
					block[num].rt.bottom += L4;

					if (block[num].rt.top == 615)
					{
						block[num].rt.bottom = 15 + block[num].rt.right - block[num].rt.left;
						block[num].rt.top = 15;
					}
				}
				else if (boardnum == 5)
				{
					block[num].rt.top += L5;
					block[num].rt.bottom += L5;

					if (block[num].rt.top == 665)
					{
						block[num].rt.bottom = 15 + block[num].rt.right - block[num].rt.left;
						block[num].rt.top = 15;
					}
				}
			}
			break;

		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case '+':
			block[num].size_up = 1;

			switch (boardnum) {
			case 3:
				if (block[num].rt.right - block[num].rt.left <= L3 * 10) {
					block[num].rt.right += L3;
					block[num].rt.bottom += L3;
				}
				break;
			case 4:
				if (block[num].rt.right - block[num].rt.left <= L4 * 10) {
					block[num].rt.right += L4;
					block[num].rt.bottom += L4;
				}
				break;
			case 5:
				if (block[num].rt.right - block[num].rt.left <= L5 * 10) {
					block[num].rt.right += L5;
					block[num].rt.bottom += L5;
				}
				break;
			}
			break;

		case '-':
			block[num].size_up = 0;
			switch (boardnum) {
			case 3:
				if (block[num].rt.right - block[num].rt.left > L3) {
					block[num].rt.right -= L3;
					block[num].rt.bottom -= L3;
				}
				break;
			case 4:
				if (block[num].rt.right - block[num].rt.left > L4) {
					block[num].rt.right -= L4;
					block[num].rt.bottom -= L4;
				}
				break;
			case 5:
				if (block[num].rt.right - block[num].rt.left > L5) {
					block[num].rt.right -= L5;
					block[num].rt.bottom -= L5;
				}
				break;
			}
			break;

		case '/':

			selection = 0;

			break;

		case '0':

			selection = 1;
			num = order;
			break;

		case '1':

			selection = 1;
			num = order + 1;
			if (num > 9)
				num -= 10;

			break;

		case '2':

			selection = 1;
			num = order + 2;
			if (num > 9)
				num -= 10;

			break;

		case '3':

			selection = 1;
			num = order + 3;
			if (num > 9)
				num -= 10;

			break;

		case '4':

			selection = 1;
			num = order + 4;
			if (num > 9)
				num -= 10;

			break;

		case '5':

			selection = 1;
			num = order + 5;
			if (num > 9)
				num -= 10;

			break;

		case '6':

			selection = 1;
			num = order + 6;
			if (num > 9)
				num -= 10;

			break;

		case '7':

			selection = 1;
			num = order + 7;
			if (num > 9)
				num -= 10;

			break;

		case '8':

			selection = 1;
			num = order + 8;
			if (num > 9)
				num -= 10;

			break;

		case '9':

			selection = 1;
			num = order + 9;
			if (num > 9)
				num -= 10;

			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
