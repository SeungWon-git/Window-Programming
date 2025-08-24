#include <windows.h>
#include <tchar.h>
#include<time.h>
#include "resource.h"

#define D_3 170
#define D_4 128
#define D_5 102

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 5-2";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

typedef struct PUZZLE {
	RECT prt;	//���������� ��ġ����
	RECT lrt;	//�� ������ ��ġ
	int num;	//����
	BOOL empty;
}PUZZLE;

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 525, 565, NULL, (HMENU)NULL, hInstance, NULL);

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
	static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
	PAINTSTRUCT ps;

	static int sx, sy, ex, ey;

	static PUZZLE pz[25];

	static int device;	//3,4,5

	static BOOL start;

	static BOOL fullscreen;

	static BOOL reversal;

	static int d, empty, replace, move;	//���콺 Ŭ������ ����ϴ� ����(move�� ��ĭ�� [1-������,2-����,3-��,4-�Ʒ�] Ŭ���� �������� ���� ����)

	static BOOL mouseblock;	//��������̴� ���� ���콺�Է��� ������

	static RECT changert;	//������ �����϶� �ٲ� Ư��ȭ���� ��ġ�� �����ϴ� ����(������ ����)

	static int picnum;	//1-ù��° ���� 2-�ι�° ����

	//static TCHAR lpOut[1024];

	srand(time(NULL));


	switch (iMsg)
	{
	case WM_CREATE:

		SetTimer(hwnd, 3, 10, NULL);

		device = 3;
		start = 0;
		fullscreen = 0;
		reversal = 0;
		mouseblock = 0;
		picnum = 1;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//wsprintf(lpOut, L"X:%d Y:%d W:%d H:%d errormsg;%d space:%d mx:%d my:%d", X, Y, W, H, errormsg, space, mx, my);
		//TextOut(hdc, 10, 10, lpOut, lstrlen(lpOut));

		//MessageBox(NULL, L"�Է¹����� ������ϴ�.", L"����", MB_ICONWARNING | MB_OK);
	
		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		if (start == 0)
			BitBlt(hdc, 0, 0, 600, 600, mem1dc, 0, 0, SRCCOPY);
		else if (fullscreen == 1) {
			BitBlt(hdc, 0, 0, 600, 600, mem1dc, 0, 0, SRCCOPY);
			if (device == 3) {
				for (int i = 1; i < device; i++)
				{
					MoveToEx(hdc, i * D_3, 0, NULL); LineTo(hdc, i * D_3, 512);
				}
				for (int i = 1; i < device; i++)
				{
					MoveToEx(hdc, 0, i * D_3, NULL); LineTo(hdc, 512, i * D_3);
				}
			}
			else if (device == 4) {
				for (int i = 1; i < device; i++)
				{
					MoveToEx(hdc, i * D_4, 0, NULL); LineTo(hdc, i * D_4, 512);
				}
				for (int i = 1; i < device; i++)
				{
					MoveToEx(hdc, 0, i * D_4, NULL); LineTo(hdc, 512, i * D_4);
				}
			}
			else if (device == 5) {
				for (int i = 1; i < device; i++)
				{
					MoveToEx(hdc, i * D_5, 0, NULL); LineTo(hdc, i * D_5, 512);
				}
				for (int i = 1; i < device; i++)
				{
					MoveToEx(hdc, 0, i * D_5, NULL); LineTo(hdc, 512, i * D_5);
				}
			}
		}
		else
		{
			if (reversal == 1)
			{
				for (int i = 0; i < device * device; i++)
					if (pz[i].empty != 1)
						StretchBlt(hdc, pz[i].lrt.left, pz[i].lrt.top, pz[i].lrt.right - pz[i].lrt.left, pz[i].lrt.bottom - pz[i].lrt.top, mem1dc,
							pz[i].prt.left, pz[i].prt.top, pz[i].prt.right - pz[i].prt.left, pz[i].prt.bottom - pz[i].prt.top, NOTSRCCOPY);
			}
			else {
				for (int i = 0; i < device * device; i++)
					if (pz[i].empty != 1)
						StretchBlt(hdc, pz[i].lrt.left, pz[i].lrt.top, pz[i].lrt.right - pz[i].lrt.left, pz[i].lrt.bottom - pz[i].lrt.top, mem1dc,
							pz[i].prt.left, pz[i].prt.top, pz[i].prt.right - pz[i].prt.left, pz[i].prt.bottom - pz[i].prt.top, SRCCOPY);
			}
		}

		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
	{
		switch (wParam)
		{
		case '1':
			picnum = 1;
			break;
		case '2':
			picnum = 2;
			break;
		case '3':
			device = 3;
			KillTimer(hwnd, 2);
			mouseblock = 0;
			for (int i = 0; i < 9; i++)
			{
				pz[i].prt.left = D_3 * (i % 3); pz[i].prt.top = i / 3 * D_3;
				pz[i].prt.right = i % 3 * D_3 + D_3; pz[i].prt.bottom = i / 3 * D_3 + D_3;
				pz[i].num = i + 1;
				pz[i].empty = 0;
			}

			for (int i = 0; i < 50; i++)	//���� ����
			{
				int index1 = rand() % 9;
				int index2 = rand() % 9;
				int tempindex = pz[index1].num;
				pz[index1].num = pz[index2].num;
				pz[index2].num = tempindex;
			}

			for (int i = 0; i < 9; i++)
			{
				if (pz[i].num >= 1 && pz[i].num <= 3)
				{
					pz[i].lrt.left = D_3 * (pz[i].num - 1); pz[i].lrt.top = 0;
					pz[i].lrt.right = D_3 * (pz[i].num - 1) + D_3; pz[i].lrt.bottom = D_3;
				}
				else if (pz[i].num >= 4 && pz[i].num <= 6)
				{
					pz[i].lrt.left = D_3 * (pz[i].num - 4); pz[i].lrt.top = D_3;
					pz[i].lrt.right = D_3 * (pz[i].num - 4) + D_3; pz[i].lrt.bottom = D_3 * 2;
				}
				else if (pz[i].num >= 7 && pz[i].num <= 9)
				{
					pz[i].lrt.left = D_3 * (pz[i].num - 7); pz[i].lrt.top = D_3 * 2;
					pz[i].lrt.right = D_3 * (pz[i].num - 7) + D_3; pz[i].lrt.bottom = D_3 * 3;
				}
			}

			pz[rand() % 9].empty = 1;	//��ĭ ���ϱ�

			break;
		case '4':
			device = 4;
			KillTimer(hwnd, 2);
			mouseblock = 0;
			for (int i = 0; i < 16; i++)
			{
				pz[i].prt.left = D_4 * (i % 4); pz[i].prt.top = i / 4 * D_4;
				pz[i].prt.right = i % 4 * D_4 + D_4; pz[i].prt.bottom = i / 4 * D_4 + D_4;
				pz[i].num = i + 1;
				pz[i].empty = 0;
			}

			for (int i = 0; i < 90; i++)	//���� ����
			{
				int index1 = rand() % 16;
				int index2 = rand() % 16;
				int tempindex = pz[index1].num;
				pz[index1].num = pz[index2].num;
				pz[index2].num = tempindex;
			}

			for (int i = 0; i < 16; i++)
			{
				if (pz[i].num >= 1 && pz[i].num <= 4)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 1); pz[i].lrt.top = 0;
					pz[i].lrt.right = D_4 * (pz[i].num - 1) + D_4; pz[i].lrt.bottom = D_4;
				}
				else if (pz[i].num >= 5 && pz[i].num <= 8)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 5); pz[i].lrt.top = D_4;
					pz[i].lrt.right = D_4 * (pz[i].num - 5) + D_4; pz[i].lrt.bottom = D_4 * 2;
				}
				else if (pz[i].num >= 9 && pz[i].num <= 12)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 9); pz[i].lrt.top = D_4 * 2;
					pz[i].lrt.right = D_4 * (pz[i].num - 9) + D_4; pz[i].lrt.bottom = D_4 * 3;
				}
				else if (pz[i].num >= 13 && pz[i].num <= 16)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 13); pz[i].lrt.top = D_4 * 3;
					pz[i].lrt.right = D_4 * (pz[i].num - 13) + D_4; pz[i].lrt.bottom = D_4 * 4;
				}
			}

			pz[rand() % 16].empty = 1;	//��ĭ ���ϱ�

			break;
		case '5':
			device = 5;
			KillTimer(hwnd, 2);
			mouseblock = 0;
			for (int i = 0; i < 25; i++)
			{
				pz[i].prt.left = D_5 * (i % 5); pz[i].prt.top = i / 5 * D_5;
				pz[i].prt.right = i % 5 * D_5 + D_5; pz[i].prt.bottom = i / 5 * D_5 + D_5;
				pz[i].num = i + 1;
				pz[i].empty = 0;
			}

			for (int i = 0; i < 200; i++)	//���� ����
			{
				int index1 = rand() % 25;
				int index2 = rand() % 25;
				int tempindex = pz[index1].num;
				pz[index1].num = pz[index2].num;
				pz[index2].num = tempindex;
			}

			for (int i = 0; i < 25; i++)
			{
				if (pz[i].num >= 1 && pz[i].num <= 5)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 1); pz[i].lrt.top = 0;
					pz[i].lrt.right = D_5 * (pz[i].num - 1) + D_5; pz[i].lrt.bottom = D_5;
				}
				else if (pz[i].num >= 6 && pz[i].num <= 10)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 6); pz[i].lrt.top = D_5;
					pz[i].lrt.right = D_5 * (pz[i].num - 6) + D_5; pz[i].lrt.bottom = D_5 * 2;
				}
				else if (pz[i].num >= 11 && pz[i].num <= 15)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 11); pz[i].lrt.top = D_5 * 2;
					pz[i].lrt.right = D_5 * (pz[i].num - 11) + D_5; pz[i].lrt.bottom = D_5 * 3;
				}
				else if (pz[i].num >= 16 && pz[i].num <= 20)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 16); pz[i].lrt.top = D_5 * 3;
					pz[i].lrt.right = D_5 * (pz[i].num - 16) + D_5; pz[i].lrt.bottom = D_5 * 4;
				}
				else if (pz[i].num >= 21 && pz[i].num <= 25)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 21); pz[i].lrt.top = D_5 * 4;
					pz[i].lrt.right = D_5 * (pz[i].num - 21) + D_5; pz[i].lrt.bottom = D_5 * 5;
				}
			}

			pz[rand() % 25].empty = 1;	//��ĭ ���ϱ�

			break;
		}

		//InvalidateRect(hwnd, NULL, true);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'S':
			if (start == 0) {
				if (device == 3) {	//���� ���������� ���� ����
					for (int i = 0; i < 9; i++)
					{
						pz[i].prt.left = D_3 * (i % 3); pz[i].prt.top = i / 3 * D_3;					// i % 3 * D_3 ���� ���� �߿�! D_3 * i % 3 �ϸ� �տ� ���ϱ� ������ �����ϰ� ������ ������ �ϱ� ������ ���ϱ� ������ �Ǵ� ������ ������ �ѹ��� �Ҷ� ������ �� ��������Ѵ�. + ()���ؼ� �켱������ ����� �� ����
						pz[i].prt.right = i % 3 * D_3 + D_3; pz[i].prt.bottom = i / 3 * D_3 + D_3;
						pz[i].num = i + 1;
						pz[i].empty = 0;
					}

					for (int i = 0; i < 50; i++)	//���� ����
					{
						int index1 = rand() % 9;
						int index2 = rand() % 9;
						int tempindex = pz[index1].num;
						pz[index1].num = pz[index2].num;
						pz[index2].num = tempindex;
					}

					for (int i = 0; i < 9; i++)
					{
						if (pz[i].num >= 1 && pz[i].num <= 3)
						{
							pz[i].lrt.left = D_3 * (pz[i].num - 1); pz[i].lrt.top = 0;
							pz[i].lrt.right = D_3 * (pz[i].num - 1) + D_3; pz[i].lrt.bottom = D_3;
						}
						else if (pz[i].num >= 4 && pz[i].num <= 6)
						{
							pz[i].lrt.left = D_3 * (pz[i].num - 4); pz[i].lrt.top = D_3;
							pz[i].lrt.right = D_3 * (pz[i].num - 4) + D_3; pz[i].lrt.bottom = D_3 * 2;
						}
						else if (pz[i].num >= 7 && pz[i].num <= 9)
						{
							pz[i].lrt.left = D_3 * (pz[i].num - 7); pz[i].lrt.top = D_3 * 2;
							pz[i].lrt.right = D_3 * (pz[i].num - 7) + D_3; pz[i].lrt.bottom = D_3 * 3;
						}
					}

					pz[rand() % 9].empty = 1;	//��ĭ ���ϱ�
				}
				else if (device == 4) {	//���� ���������� ���� ����
					for (int i = 0; i < 16; i++)
					{
						pz[i].prt.left = D_4 * (i % 4); pz[i].prt.top = i / 4 * D_4;
						pz[i].prt.right = i % 4 * D_4 + D_4; pz[i].prt.bottom = i / 4 * D_4 + D_4;
						pz[i].num = i + 1;
						pz[i].empty = 0;
					}

					for (int i = 0; i < 90; i++)	//���� ����
					{
						int index1 = rand() % 16;
						int index2 = rand() % 16;
						int tempindex = pz[index1].num;
						pz[index1].num = pz[index2].num;
						pz[index2].num = tempindex;
					}

					for (int i = 0; i < 16; i++)
					{
						if (pz[i].num >= 1 && pz[i].num <= 4)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 1); pz[i].lrt.top = 0;
							pz[i].lrt.right = D_4 * (pz[i].num - 1) + D_4; pz[i].lrt.bottom = D_4;
						}
						else if (pz[i].num >= 5 && pz[i].num <= 8)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 5); pz[i].lrt.top = D_4;
							pz[i].lrt.right = D_4 * (pz[i].num - 5) + D_4; pz[i].lrt.bottom = D_4 * 2;
						}
						else if (pz[i].num >= 9 && pz[i].num <= 12)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 9); pz[i].lrt.top = D_4 * 2;
							pz[i].lrt.right = D_4 * (pz[i].num - 9) + D_4; pz[i].lrt.bottom = D_4 * 3;
						}
						else if (pz[i].num >= 13 && pz[i].num <= 16)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 13); pz[i].lrt.top = D_4 * 3;
							pz[i].lrt.right = D_4 * (pz[i].num - 13) + D_4; pz[i].lrt.bottom = D_4 * 4;
						}
					}

					pz[rand() % 16].empty = 1;	//��ĭ ���ϱ�
				}
				else if (device == 5) {	//���� ���������� ���� ����
					for (int i = 0; i < 25; i++)
					{
						pz[i].prt.left = D_5 * (i % 5); pz[i].prt.top = i / 5 * D_5;
						pz[i].prt.right = i % 5 * D_5 + D_5; pz[i].prt.bottom = i / 5 * D_5 + D_5;
						pz[i].num = i + 1;
						pz[i].empty = 0;
					}

					for (int i = 0; i < 200; i++)	//���� ����
					{
						int index1 = rand() % 25;
						int index2 = rand() % 25;
						int tempindex = pz[index1].num;
						pz[index1].num = pz[index2].num;
						pz[index2].num = tempindex;
					}

					for (int i = 0; i < 25; i++)
					{
						if (pz[i].num >= 1 && pz[i].num <= 5)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 1); pz[i].lrt.top = 0;
							pz[i].lrt.right = D_5 * (pz[i].num - 1) + D_5; pz[i].lrt.bottom = D_5;
						}
						else if (pz[i].num >= 6 && pz[i].num <= 10)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 6); pz[i].lrt.top = D_5;
							pz[i].lrt.right = D_5 * (pz[i].num - 6) + D_5; pz[i].lrt.bottom = D_5 * 2;
						}
						else if (pz[i].num >= 11 && pz[i].num <= 15)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 11); pz[i].lrt.top = D_5 * 2;
							pz[i].lrt.right = D_5 * (pz[i].num - 11) + D_5; pz[i].lrt.bottom = D_5 * 3;
						}
						else if (pz[i].num >= 16 && pz[i].num <= 20)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 16); pz[i].lrt.top = D_5 * 3;
							pz[i].lrt.right = D_5 * (pz[i].num - 16) + D_5; pz[i].lrt.bottom = D_5 * 4;
						}
						else if (pz[i].num >= 21 && pz[i].num <= 25)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 21); pz[i].lrt.top = D_5 * 4;
							pz[i].lrt.right = D_5 * (pz[i].num - 21) + D_5; pz[i].lrt.bottom = D_5 * 5;
						}
					}

					pz[rand() % 25].empty = 1;	//��ĭ ���ϱ�
				}
				start = 1;
			}
			break;
		case 'F':
			fullscreen = 1;
			SetTimer(hwnd, 1, 5000, NULL);
			break;
		case 'P':
			if (reversal == 0)
				reversal = 1;
			else
				reversal = 0;
			break;
		case 'Q':
			DestroyWindow(hwnd);
			break;
		}

		//InvalidateRect(hwnd, NULL, true);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_SCENE1:
			picnum = 1;
			break;
		case ID_SCENE2:
			picnum = 2;
			break;
		case ID_3X3:
			device = 3;
			KillTimer(hwnd, 2);
			mouseblock = 0;
			for (int i = 0; i < 9; i++)
			{
				pz[i].prt.left = D_3 * (i % 3); pz[i].prt.top = i / 3 * D_3;
				pz[i].prt.right = i % 3 * D_3 + D_3; pz[i].prt.bottom = i / 3 * D_3 + D_3;
				pz[i].num = i + 1;
				pz[i].empty = 0;
			}

			for (int i = 0; i < 50; i++)	//���� ����
			{
				int index1 = rand() % 9;
				int index2 = rand() % 9;
				int tempindex = pz[index1].num;
				pz[index1].num = pz[index2].num;
				pz[index2].num = tempindex;
			}

			for (int i = 0; i < 9; i++)
			{
				if (pz[i].num >= 1 && pz[i].num <= 3)
				{
					pz[i].lrt.left = D_3 * (pz[i].num - 1); pz[i].lrt.top = 0;
					pz[i].lrt.right = D_3 * (pz[i].num - 1) + D_3; pz[i].lrt.bottom = D_3;
				}
				else if (pz[i].num >= 4 && pz[i].num <= 6)
				{
					pz[i].lrt.left = D_3 * (pz[i].num - 4); pz[i].lrt.top = D_3;
					pz[i].lrt.right = D_3 * (pz[i].num - 4) + D_3; pz[i].lrt.bottom = D_3 * 2;
				}
				else if (pz[i].num >= 7 && pz[i].num <= 9)
				{
					pz[i].lrt.left = D_3 * (pz[i].num - 7); pz[i].lrt.top = D_3 * 2;
					pz[i].lrt.right = D_3 * (pz[i].num - 7) + D_3; pz[i].lrt.bottom = D_3 * 3;
				}
			}

			pz[rand() % 9].empty = 1;	//��ĭ ���ϱ�

			break;
		case ID_4X4:
			device = 4;
			KillTimer(hwnd, 2);
			mouseblock = 0;
			for (int i = 0; i < 16; i++)
			{
				pz[i].prt.left = D_4 * (i % 4); pz[i].prt.top = i / 4 * D_4;
				pz[i].prt.right = i % 4 * D_4 + D_4; pz[i].prt.bottom = i / 4 * D_4 + D_4;
				pz[i].num = i + 1;
				pz[i].empty = 0;
			}

			for (int i = 0; i < 90; i++)	//���� ����
			{
				int index1 = rand() % 16;
				int index2 = rand() % 16;
				int tempindex = pz[index1].num;
				pz[index1].num = pz[index2].num;
				pz[index2].num = tempindex;
			}

			for (int i = 0; i < 16; i++)
			{
				if (pz[i].num >= 1 && pz[i].num <= 4)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 1); pz[i].lrt.top = 0;
					pz[i].lrt.right = D_4 * (pz[i].num - 1) + D_4; pz[i].lrt.bottom = D_4;
				}
				else if (pz[i].num >= 5 && pz[i].num <= 8)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 5); pz[i].lrt.top = D_4;
					pz[i].lrt.right = D_4 * (pz[i].num - 5) + D_4; pz[i].lrt.bottom = D_4 * 2;
				}
				else if (pz[i].num >= 9 && pz[i].num <= 12)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 9); pz[i].lrt.top = D_4 * 2;
					pz[i].lrt.right = D_4 * (pz[i].num - 9) + D_4; pz[i].lrt.bottom = D_4 * 3;
				}
				else if (pz[i].num >= 13 && pz[i].num <= 16)
				{
					pz[i].lrt.left = D_4 * (pz[i].num - 13); pz[i].lrt.top = D_4 * 3;
					pz[i].lrt.right = D_4 * (pz[i].num - 13) + D_4; pz[i].lrt.bottom = D_4 * 4;
				}
			}

			pz[rand() % 16].empty = 1;	//��ĭ ���ϱ�

			break;
		case ID_5X5:
			device = 5;
			KillTimer(hwnd, 2);
			mouseblock = 0;
			for (int i = 0; i < 25; i++)
			{
				pz[i].prt.left = D_5 * (i % 5); pz[i].prt.top = i / 5 * D_5;
				pz[i].prt.right = i % 5 * D_5 + D_5; pz[i].prt.bottom = i / 5 * D_5 + D_5;
				pz[i].num = i + 1;
				pz[i].empty = 0;
			}

			for (int i = 0; i < 200; i++)	//���� ����
			{
				int index1 = rand() % 25;
				int index2 = rand() % 25;
				int tempindex = pz[index1].num;
				pz[index1].num = pz[index2].num;
				pz[index2].num = tempindex;
			}

			for (int i = 0; i < 25; i++)
			{
				if (pz[i].num >= 1 && pz[i].num <= 5)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 1); pz[i].lrt.top = 0;
					pz[i].lrt.right = D_5 * (pz[i].num - 1) + D_5; pz[i].lrt.bottom = D_5;
				}
				else if (pz[i].num >= 6 && pz[i].num <= 10)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 6); pz[i].lrt.top = D_5;
					pz[i].lrt.right = D_5 * (pz[i].num - 6) + D_5; pz[i].lrt.bottom = D_5 * 2;
				}
				else if (pz[i].num >= 11 && pz[i].num <= 15)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 11); pz[i].lrt.top = D_5 * 2;
					pz[i].lrt.right = D_5 * (pz[i].num - 11) + D_5; pz[i].lrt.bottom = D_5 * 3;
				}
				else if (pz[i].num >= 16 && pz[i].num <= 20)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 16); pz[i].lrt.top = D_5 * 3;
					pz[i].lrt.right = D_5 * (pz[i].num - 16) + D_5; pz[i].lrt.bottom = D_5 * 4;
				}
				else if (pz[i].num >= 21 && pz[i].num <= 25)
				{
					pz[i].lrt.left = D_5 * (pz[i].num - 21); pz[i].lrt.top = D_5 * 4;
					pz[i].lrt.right = D_5 * (pz[i].num - 21) + D_5; pz[i].lrt.bottom = D_5 * 5;
				}
			}

			pz[rand() % 25].empty = 1;	//��ĭ ���ϱ�

			break;
		case ID_START:
			if (start == 0) {
				if (device == 3) {	//���� ���������� ���� ����
					for (int i = 0; i < 9; i++)
					{
						pz[i].prt.left = D_3 * (i % 3); pz[i].prt.top = i / 3 * D_3;					// i % 3 * D_3 ���� ���� �߿�! D_3 * i % 3 �ϸ� �տ� ���ϱ� ������ �����ϰ� ������ ������ �ϱ� ������ ���ϱ� ������ �Ǵ� ������ ������ �ѹ��� �Ҷ� ������ �� ��������Ѵ�. + ()���ؼ� �켱������ ����� �� ����
						pz[i].prt.right = i % 3 * D_3 + D_3; pz[i].prt.bottom = i / 3 * D_3 + D_3;
						pz[i].num = i + 1;
						pz[i].empty = 0;
					}

					for (int i = 0; i < 50; i++)	//���� ����
					{
						int index1 = rand() % 9;
						int index2 = rand() % 9;
						int tempindex = pz[index1].num;
						pz[index1].num = pz[index2].num;
						pz[index2].num = tempindex;
					}

					for (int i = 0; i < 9; i++)
					{
						if (pz[i].num >= 1 && pz[i].num <= 3)
						{
							pz[i].lrt.left = D_3 * (pz[i].num - 1); pz[i].lrt.top = 0;
							pz[i].lrt.right = D_3 * (pz[i].num - 1) + D_3; pz[i].lrt.bottom = D_3;
						}
						else if (pz[i].num >= 4 && pz[i].num <= 6)
						{
							pz[i].lrt.left = D_3 * (pz[i].num - 4); pz[i].lrt.top = D_3;
							pz[i].lrt.right = D_3 * (pz[i].num - 4) + D_3; pz[i].lrt.bottom = D_3 * 2;
						}
						else if (pz[i].num >= 7 && pz[i].num <= 9)
						{
							pz[i].lrt.left = D_3 * (pz[i].num - 7); pz[i].lrt.top = D_3 * 2;
							pz[i].lrt.right = D_3 * (pz[i].num - 7) + D_3; pz[i].lrt.bottom = D_3 * 3;
						}
					}

					pz[rand() % 9].empty = 1;	//��ĭ ���ϱ�
				}
				else if (device == 4) {	//���� ���������� ���� ����
					for (int i = 0; i < 16; i++)
					{
						pz[i].prt.left = D_4 * (i % 4); pz[i].prt.top = i / 4 * D_4;
						pz[i].prt.right = i % 4 * D_4 + D_4; pz[i].prt.bottom = i / 4 * D_4 + D_4;
						pz[i].num = i + 1;
						pz[i].empty = 0;
					}

					for (int i = 0; i < 90; i++)	//���� ����
					{
						int index1 = rand() % 16;
						int index2 = rand() % 16;
						int tempindex = pz[index1].num;
						pz[index1].num = pz[index2].num;
						pz[index2].num = tempindex;
					}

					for (int i = 0; i < 16; i++)
					{
						if (pz[i].num >= 1 && pz[i].num <= 4)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 1); pz[i].lrt.top = 0;
							pz[i].lrt.right = D_4 * (pz[i].num - 1) + D_4; pz[i].lrt.bottom = D_4;
						}
						else if (pz[i].num >= 5 && pz[i].num <= 8)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 5); pz[i].lrt.top = D_4;
							pz[i].lrt.right = D_4 * (pz[i].num - 5) + D_4; pz[i].lrt.bottom = D_4 * 2;
						}
						else if (pz[i].num >= 9 && pz[i].num <= 12)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 9); pz[i].lrt.top = D_4 * 2;
							pz[i].lrt.right = D_4 * (pz[i].num - 9) + D_4; pz[i].lrt.bottom = D_4 * 3;
						}
						else if (pz[i].num >= 13 && pz[i].num <= 16)
						{
							pz[i].lrt.left = D_4 * (pz[i].num - 13); pz[i].lrt.top = D_4 * 3;
							pz[i].lrt.right = D_4 * (pz[i].num - 13) + D_4; pz[i].lrt.bottom = D_4 * 4;
						}
					}

					pz[rand() % 16].empty = 1;	//��ĭ ���ϱ�
				}
				else if (device == 5) {	//���� ���������� ���� ����
					for (int i = 0; i < 25; i++)
					{
						pz[i].prt.left = D_5 * (i % 5); pz[i].prt.top = i / 5 * D_5;
						pz[i].prt.right = i % 5 * D_5 + D_5; pz[i].prt.bottom = i / 5 * D_5 + D_5;
						pz[i].num = i + 1;
						pz[i].empty = 0;
					}

					for (int i = 0; i < 200; i++)	//���� ����
					{
						int index1 = rand() % 25;
						int index2 = rand() % 25;
						int tempindex = pz[index1].num;
						pz[index1].num = pz[index2].num;
						pz[index2].num = tempindex;
					}

					for (int i = 0; i < 25; i++)
					{
						if (pz[i].num >= 1 && pz[i].num <= 5)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 1); pz[i].lrt.top = 0;
							pz[i].lrt.right = D_5 * (pz[i].num - 1) + D_5; pz[i].lrt.bottom = D_5;
						}
						else if (pz[i].num >= 6 && pz[i].num <= 10)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 6); pz[i].lrt.top = D_5;
							pz[i].lrt.right = D_5 * (pz[i].num - 6) + D_5; pz[i].lrt.bottom = D_5 * 2;
						}
						else if (pz[i].num >= 11 && pz[i].num <= 15)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 11); pz[i].lrt.top = D_5 * 2;
							pz[i].lrt.right = D_5 * (pz[i].num - 11) + D_5; pz[i].lrt.bottom = D_5 * 3;
						}
						else if (pz[i].num >= 16 && pz[i].num <= 20)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 16); pz[i].lrt.top = D_5 * 3;
							pz[i].lrt.right = D_5 * (pz[i].num - 16) + D_5; pz[i].lrt.bottom = D_5 * 4;
						}
						else if (pz[i].num >= 21 && pz[i].num <= 25)
						{
							pz[i].lrt.left = D_5 * (pz[i].num - 21); pz[i].lrt.top = D_5 * 4;
							pz[i].lrt.right = D_5 * (pz[i].num - 21) + D_5; pz[i].lrt.bottom = D_5 * 5;
						}
					}

					pz[rand() % 25].empty = 1;	//��ĭ ���ϱ�
				}
				start = 1;
			}
			break;
		case ID_FULLSCREEN:
			fullscreen = 1;
			SetTimer(hwnd, 1, 5000, NULL);
			break;
		case ID_END:
			DestroyWindow(hwnd);
			break;
		}
		//InvalidateRect(hwnd, NULL, true);
		break;
	}
	case WM_LBUTTONDOWN:
		if (mouseblock != 1) {
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);

			empty;

			d = pz[0].lrt.right - pz[0].lrt.left;

			for (int i = 0; i < device * device; i++)
				if (pz[i].empty == 1)
					empty = i;

			//InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP:
		if (mouseblock != 1) {
			ex = LOWORD(lParam);
			ey = HIWORD(lParam);


			for (int i = 0; i < device * device; i++)
			{
				if (sx > pz[i].lrt.left && sx < pz[i].lrt.right && sy > pz[i].lrt.top && sy < pz[i].lrt.bottom) {

					if (pz[empty].lrt.left + d == pz[i].lrt.left && pz[empty].lrt.top == pz[i].lrt.top && sx > ex)// ��ĭ ���� Ŭ��
					{
						replace = i;
						move = 1;
						/*changert.left = pz[empty].lrt.left;
						changert.right = pz[replace].lrt.right;
						changert.top = pz[empty].lrt.top;
						changert.bottom = pz[empty].lrt.bottom;*/
						mouseblock = 1;
						SetTimer(hwnd, 2, 10, NULL);
					}
					else if (pz[empty].lrt.left - d == pz[i].lrt.left && pz[empty].lrt.top == pz[i].lrt.top && sx < ex)// ��ĭ ���� Ŭ��
					{
						replace = i;
						move = 2;
						/*changert.left = pz[replace].lrt.left;
						changert.right = pz[empty].lrt.right;
						changert.top = pz[empty].lrt.top;
						changert.bottom = pz[empty].lrt.bottom;*/
						mouseblock = 1;
						SetTimer(hwnd, 2, 10, NULL);
					}
					else if (pz[empty].lrt.left == pz[i].lrt.left && pz[empty].lrt.top - d == pz[i].lrt.top && sy < ey)// ��ĭ �� Ŭ��
					{
						replace = i;
						move = 3;
						/*changert.left = pz[empty].lrt.left;
						changert.right = pz[empty].lrt.right;
						changert.top = pz[replace].lrt.top;
						changert.bottom = pz[empty].lrt.bottom;*/
						mouseblock = 1;
						SetTimer(hwnd, 2, 10, NULL);
					}
					else if (pz[empty].lrt.left == pz[i].lrt.left && pz[empty].lrt.top + d == pz[i].lrt.top && sy > ey)// ��ĭ �Ʒ� Ŭ��
					{
						replace = i;
						move = 4;
						/*changert.left = pz[empty].lrt.left;
						changert.right = pz[empty].lrt.right;
						changert.top = pz[empty].lrt.top;
						changert.bottom = pz[replace].lrt.bottom;*/
						mouseblock = 1;
						SetTimer(hwnd, 2, 10, NULL);
					}

					break;
				}
			}

			//InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			fullscreen = 0;
			KillTimer(hwnd, 1);
			//InvalidateRect(hwnd, NULL, true);
			break;
		case 2:
			if (move == 1)	//��ĭ ������ Ŭ��
			{
				if (pz[replace].lrt.left > pz[empty].lrt.left) {
					pz[replace].lrt.left--; pz[replace].lrt.right--;
				}
				else {
					pz[empty].lrt.left += d; pz[empty].lrt.right += d;
					mouseblock = 0;
					KillTimer(hwnd, 2);
				}
			}
			else if (move == 2)	//��ĭ ������ Ŭ��
			{
				if (pz[replace].lrt.left < pz[empty].lrt.left) {
					pz[replace].lrt.left++; pz[replace].lrt.right++;
				}
				else {
					pz[empty].lrt.left -= d; pz[empty].lrt.right -= d;
					mouseblock = 0;
					KillTimer(hwnd, 2);
				}
			}
			else if (move == 3)	//��ĭ ���� Ŭ��
			{
				if (pz[replace].lrt.top < pz[empty].lrt.top) {
					pz[replace].lrt.top++; pz[replace].lrt.bottom++;
				}
				else {
					pz[empty].lrt.top -= d; pz[empty].lrt.bottom -= d;
					mouseblock = 0;
					KillTimer(hwnd, 2);
				}
			}
			else if (move == 4)	//��ĭ �Ʒ��� Ŭ��
			{
				if (pz[replace].lrt.top > pz[empty].lrt.top) {
					pz[replace].lrt.top--; pz[replace].lrt.bottom--;
				}
				else {
					pz[empty].lrt.top += d; pz[empty].lrt.bottom += d;
					mouseblock = 0;
					KillTimer(hwnd, 2);
				}
			}
			//InvalidateRect(hwnd, &changert, 1);	//change�� ��ġ�� �ٲ㼭 ������ ����
			break;
		case 3:
			hdc = GetDC(hwnd);

			mem1dc = CreateCompatibleDC(hdc);

			mem2dc = CreateCompatibleDC(mem1dc);

			hBit1 = CreateCompatibleBitmap(hdc, 600, 600);

			if (picnum == 1)
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			else if (picnum == 2)
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));

			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
			oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);

			BitBlt(mem1dc, 0, 0, 600, 600, mem2dc, 0, 0, SRCCOPY);;

			SelectObject(mem2dc, oldBit2);
			DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1);
			DeleteDC(mem1dc);

			ReleaseDC(hwnd, hdc);
			InvalidateRect(hwnd, NULL, true);
			break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}