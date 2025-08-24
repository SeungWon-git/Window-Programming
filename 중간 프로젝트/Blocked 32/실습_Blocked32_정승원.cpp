#include <windows.h>
#include <tchar.h>
#include<time.h>
#include "resource.h"

#define D_6 100

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program \"Blocked 32\"";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

typedef struct Block {
	int x,y;	//hdc의 좌표
	int num;	//해당 숫자
	bool combine_check;
}BLOCK;

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 1181, 680, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC hdc, hMemDC;
	static HBITMAP hBitmap_chance1, hBitmap_chance2, hBitmap_chance3,hBitmap_obstacle, hBitmap_block2, hBitmap_block4, hBitmap_block8, hBitmap_block16, hBitmap_block32, hBitmap_block64, hBitmap_block128, hBitmap_block256, hBitmap_block512;
	PAINTSTRUCT ps;

	static BOOL flag;

	static int smx, smy, emx, emy;
	static BOOL blockmove;

	static Block block[36];	//숫자블록
	static Block obstacleblock[4];	//장애물블록

	static int stoptimer;	//타이머를 멈추는 변수
	static int failnum;		//칸이 모두 꽉차서 패배를 결정하기위한 변수
	static BOOL fail;

	static BOOL start;

	static int move;	//블록의 [1-오른쪽,2-왼쪽,3-위,4-아래] 움직임을 위한 변수

	static int chance, fullchance, chanceindx, usingchancelange;
	static bool chancechoice,chanceusing;

	static int c[3];

	static int obstacle, goal, tempobstacle, tempgoal;

	static BOOL mouseblock;	//퍼즐움직이는 동안 마우스입력을 막아줌

	static TCHAR lpOut[1024];

	static RECT rt_left = { 0,0,610,610 };	//왼쪽 칸(게임보트칸)만을 화면 변화시키기 위한 변수
	static RECT rt_chancenum = { 610,115,1000,134 };	//남은 찬스수만을 화면 변화시키기 위한 변수

	srand(time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:
		
		//	<<<중요!!!!!!!!!!!!!!!!!!!!!!!!>>>
		//	LoadBitmap은 메모리를 아주 많이 잡아 먹는 행위! 그래서 반드시! 한번만 생성하자!(WM_Paint문에서 쓸때마다 로드 하지 말자!)
		//	+ 이미지 로드하는 건 항상 메모리를 많이 잡으니 기억해두자!
		hBitmap_obstacle = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_OBSTACLE));

		hBitmap_block2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hBitmap_block4 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hBitmap_block8 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		hBitmap_block16 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
		hBitmap_block32 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		hBitmap_block64 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP64));
		hBitmap_block128 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP128));
		hBitmap_block256 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP256));
		hBitmap_block512 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP512));

		hBitmap_chance1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_1));
		hBitmap_chance2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_2));
		hBitmap_chance3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_3));

		start = 0;
		mouseblock = 1;
		chance = 0; chancechoice = 0; fullchance = 0;
		stoptimer = 0;
		obstacle = 2; tempobstacle = 2;
		goal = 32; tempgoal = 32;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);

		//장애물
		SelectObject(hMemDC, hBitmap_obstacle);

		if (start == 1 && chanceusing == 0) {
			for (int i = 0; i < obstacle; i++)
			{
				if (obstacleblock[i].num == 1)
					BitBlt(hdc, obstacleblock[i].x, obstacleblock[i].y, 100, 100, hMemDC, 0, 0, SRCCOPY);
			}
		}
		else if (start == 1 && chanceusing == 1)
		{
			for (int i = 0; i < obstacle; i++)
			{
				if (obstacleblock[i].num == 1)
					BitBlt(hdc, obstacleblock[i].x, obstacleblock[i].y, 100, 100, hMemDC, 0, 0, SRCCOPY);
				else if(i==chanceindx)
					StretchBlt(hdc, obstacleblock[i].x, obstacleblock[i].y, usingchancelange, usingchancelange, hMemDC, 0, 0,100,100, SRCCOPY);
			}
		}

		DeleteDC(hMemDC);

		//왼쪽테두리
		for (int i = 0; i < 7; i++)
		{
			MoveToEx(hdc, i * D_6, 0, NULL); LineTo(hdc, i * D_6, 600);
		}
		for (int i = 0; i < 7; i++)
		{
			MoveToEx(hdc, 0, i * D_6, NULL); LineTo(hdc, 600, i * D_6);
		}
		
		//블록
		if (start == 1) {
			for (int i = 0; i < 36; i++)
			{
				hMemDC = CreateCompatibleDC(hdc);

				if (block[i].num == 2)
				{
					SelectObject(hMemDC, hBitmap_block2);
				}
				else if (block[i].num == 4)
				{
					SelectObject(hMemDC, hBitmap_block4);
				}
				else if (block[i].num == 8)
				{
					SelectObject(hMemDC, hBitmap_block8);
				}
				else if (block[i].num == 16)
				{
					SelectObject(hMemDC, hBitmap_block16);
				}
				else if (block[i].num == 32)
				{
					SelectObject(hMemDC, hBitmap_block32);
				}
				else if (block[i].num == 64)
				{
					SelectObject(hMemDC, hBitmap_block64);
				}
				else if (block[i].num == 128)
				{
					SelectObject(hMemDC, hBitmap_block128);
				}
				else if (block[i].num == 256)
				{
					SelectObject(hMemDC, hBitmap_block256);
				}
				else if (block[i].num == 512)
				{
					SelectObject(hMemDC, hBitmap_block512);
				}

				if (block[i].num != 0)
					BitBlt(hdc, block[i].x, block[i].y, 100, 100, hMemDC, 0, 0, SRCCOPY);

				DeleteDC(hMemDC);
			}
		}

		//오른쪽 테두리
		for (int i = 0; i < 3; i++)
		{
			Rectangle(hdc, 750 + D_6 * i, 250, 750 + D_6 * i + D_6, 350);
		}

		//찬스
		hMemDC = CreateCompatibleDC(hdc);

		if (fullchance == 1)
		{
			SelectObject(hMemDC, hBitmap_chance1);

			if (c[0] == 1)
				BitBlt(hdc, 750, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
			else if (c[1] == 1)
				BitBlt(hdc, 850, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
			else if (c[2] == 1)
				BitBlt(hdc, 950, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
		}
		else if (fullchance == 2)
		{
			SelectObject(hMemDC, hBitmap_chance2);

			if (c[0] == 2)
				BitBlt(hdc, 750, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
			else if (c[1] == 2)
				BitBlt(hdc, 850, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
			else if (c[2] == 2)
				BitBlt(hdc, 950, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
		}
		else if (fullchance == 3)
		{
			SelectObject(hMemDC, hBitmap_chance3);

			if (c[0] == 3)
				BitBlt(hdc, 750, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
			else if (c[1] == 3)
				BitBlt(hdc, 850, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
			else if (c[2] == 3)
				BitBlt(hdc, 950, 250, 100, 100, hMemDC, 0, 0, SRCCOPY);
		}

		DeleteDC(hMemDC);

		if (chance == 0 && start == 1)
			TextOut(hdc, 750, 200, L"찬스를 선택해 주세요.", 12);

		wsprintf(lpOut, L"<<목표 점수 - %d>>", goal);
		TextOut(hdc, 700, 100, lpOut, lstrlen(lpOut));

		if (chancechoice != 0) {
			wsprintf(lpOut, L"  [남은 찬스 수-%d]", chance);
			TextOut(hdc, 700, 115, lpOut, lstrlen(lpOut));
		}

		if (start == 0) {
			wsprintf(lpOut, L"[시작/새게임]을 클릭하여 주세요.");
			TextOut(hdc, 750, 200, lpOut, lstrlen(lpOut));
		}

		for (int i = 0; i < 36; i++) {	//승리
			if (block[i].num == goal)
			{
				mouseblock = 1;
				start = 0;
				KillTimer(hwnd, 1);
				MessageBox(NULL, L"\"You Win!!\"\nIf you want to play one more time click the '새게임' button.", L"승리!", MB_ICONWARNING | MB_OK);
			}
		}

		if (fail == 1) {	//패배
			fail = 0;
			MessageBox(NULL, L"\"You Lose...\"\nIf you want to play one more time click the '새게임' button.", L"패배", MB_ICONWARNING | MB_OK);
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case ID_NEWGAME:

			//초기화
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			
			start = 1;
			mouseblock = 0;
			chancechoice = 0;
			chance = 0;
			fullchance = 0;
			chanceusing = 0;
			stoptimer = 0;
			blockmove = 0;
			obstacle = tempobstacle;
			goal = tempgoal;

			c[0] = 1; c[1] = 2; c[2] = 3;

			for (int i = 0; i < 30; i++)	//찬스블록숫자지정
			{
				int ind1 = rand() % 3;
				int ind2 = rand() % 3;
				int temp = c[ind1];

				c[ind1] = c[ind2];
				c[ind2] = temp;
			}

			for (int i = 0; i < 36; i++)
			{
				block[i].num = 0;
				block[i].combine_check = 0;
			}

			//초기 설정(스타트와 동일)
			block[0].x = rand() % 6 * D_6; block[1].x = rand() % 6 * D_6;
			block[0].y = rand() % 6 * D_6; block[1].y = rand() % 6 * D_6;
			block[0].num = 2; block[1].num = 2;
			while (block[1].x == block[0].x && block[1].y == block[0].y)	//중복체크
			{
				block[1].x = rand() % 6 * D_6;
				block[1].y = rand() % 6 * D_6;
			}

			for (int i = 0; i < obstacle; i++)
			{
				obstacleblock[i].x = rand() % 6 * D_6;
				obstacleblock[i].y = rand() % 6 * D_6;
				obstacleblock[i].num = 1; //배정 받았음을 뜻

				for (int j = 0; j < 2; j++)	//중복체크(완벽히 중복을 막지는 못함..)
				{
					while (obstacleblock[i].x == block[j].x && obstacleblock[i].y == block[j].y)
					{
						obstacleblock[i].x = rand() % 6 * D_6;
						obstacleblock[i].y = rand() % 6 * D_6;
					}
				}
				for (int j = i - 1; j >= 0; j--)	//중복체크(완벽히 중복을 막지는 못함..확률을 현저히 낮추기는 함)
				{
					while (obstacleblock[i].x == obstacleblock[j].x && obstacleblock[i].y == obstacleblock[j].y)
					{
						obstacleblock[i].x = rand() % 6 * D_6;
						obstacleblock[i].y = rand() % 6 * D_6;
					}
				}
			}

			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case ID_START:
			if (start == 0) {
				start = 1;
				obstacle = tempobstacle;
				goal = tempgoal;
				mouseblock = 0;
				blockmove = 0;

				c[0] = 1; c[1] = 2; c[2] = 3;

				for (int i = 0; i < 30; i++)	//찬스블록숫자지정
				{
					int ind1 = rand() % 3;
					int ind2 = rand() % 3;
					int temp = c[ind1];

					c[ind1] = c[ind2];
					c[ind2] = temp;
				}

				block[0].x = rand() % 6 * D_6; block[1].x = rand() % 6 * D_6;
				block[0].y = rand() % 6 * D_6; block[1].y = rand() % 6 * D_6;
				block[0].num = 2; block[1].num = 2;
				while (block[1].x == block[0].x && block[1].y == block[0].y)	//중복체크
				{
					block[1].x = rand() % 6 * D_6;
					block[1].y = rand() % 6 * D_6;
				}

				for (int i = 0; i < obstacle; i++)
				{
					obstacleblock[i].x = rand() % 6 * D_6;
					obstacleblock[i].y = rand() % 6 * D_6;
					obstacleblock[i].num = 1; //배정 받았음을 뜻

					for (int j = 0; j < 2; j++)	//중복체크(완벽히 중복을 막지는 못함..)
					{
						while (obstacleblock[i].x == block[j].x && obstacleblock[i].y == block[j].y)
						{
							obstacleblock[i].x = rand() % 6 * D_6;
							obstacleblock[i].y = rand() % 6 * D_6;
						}
					}
					for (int j = i - 1; j >= 0; j--)	//중복체크(완벽히 중복을 막지는 못함..확률을 현저히 낮추기는 함)
					{
						while (obstacleblock[i].x == obstacleblock[j].x && obstacleblock[i].y == obstacleblock[j].y)
						{
							obstacleblock[i].x = rand() % 6 * D_6;
							obstacleblock[i].y = rand() % 6 * D_6;
						}
					}
				}

				InvalidateRect(hwnd, NULL, true);
			}
			break;
		case ID_END:
			DestroyWindow(hwnd);
			break;
		case ID_NUM2:
			tempobstacle = 2;
			MessageBox(NULL, L"장애물 갯수를 수정하시려면 '새게임'을 클릭해주세요.", L"장애물 갯수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_NUM3:
			tempobstacle = 3;
			MessageBox(NULL, L"장애물 갯수를 수정하시려면 '새게임'을 클릭해주세요.", L"장애물 갯수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_NUM4:
			tempobstacle = 4;
			MessageBox(NULL, L"장애물 갯수를 수정하시려면 '새게임'을 클릭해주세요.", L"장애물 갯수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_GOAL512:
			tempgoal = 512;
			MessageBox(NULL, L"목표점수를 수정하시려면 '새게임'을 클릭해주세요.", L"목표점수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_GOAL256:
			tempgoal = 256;
			MessageBox(NULL, L"목표점수를 수정하시려면 '새게임'을 클릭해주세요.", L"목표점수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_GOAL128:
			tempgoal = 128;
			MessageBox(NULL, L"목표점수를 수정하시려면 '새게임'을 클릭해주세요.", L"목표점수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_GOAL64:
			tempgoal = 64;
			MessageBox(NULL, L"목표점수를 수정하시려면 '새게임'을 클릭해주세요.", L"목표점수 수정", MB_ICONWARNING | MB_OK);
			break;
		case ID_GOAL32:
			tempgoal = 32;
			MessageBox(NULL, L"목표점수를 수정하시려면 '새게임'을 클릭해주세요.", L"목표점수 수정", MB_ICONWARNING | MB_OK);
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
		if (mouseblock != 1) {
			smx = LOWORD(lParam);
			smy = HIWORD(lParam);

			//찬스 고르기
			if (smx > 750 && smx < 850 && smy>250 && smy < 350 && chancechoice == 0 && start == 1) {
				chancechoice = 1;
				chance = c[0];
				fullchance = c[0];
				InvalidateRect(hwnd, NULL, true);
			}
			else if (smx > 850 && smx < 950 && smy>250 && smy < 350 && chancechoice == 0 && start == 1) {
				chancechoice = 1;
				chance = c[1];
				fullchance = c[1];
				InvalidateRect(hwnd, NULL, true);
			}
			else if (smx > 950 && smx < 1050 && smy>250 && smy < 350 && chancechoice == 0 && start == 1) {
				chancechoice = 1;
				chance = c[2];
				fullchance = c[2];
				InvalidateRect(hwnd, NULL, true);
			}

			//장애물 클릭
			for (int i = 0; i < obstacle; i++)
			{
				if (obstacleblock[i].num != 0 && smx > obstacleblock[i].x && smx< obstacleblock[i].x + D_6 && smy > obstacleblock[i].y && smy < obstacleblock[i].y + D_6 && chance > 0)
				{
					chance--;
					obstacleblock[i].num = 0;
					chanceindx = i;
					mouseblock = 1;
					chanceusing = 1;
					usingchancelange = 100;

					SetTimer(hwnd, 2, 30, NULL);

					InvalidateRect(hwnd, &rt_chancenum, true);
				}
			}

			//블록 클릭	
			if (smx > 0 && smx < 600 && smy > 0 && smy < 600)
				blockmove = 1;


			InvalidateRect(hwnd, &rt_left, TRUE);
		}
		break;
	case WM_LBUTTONUP:
		if (mouseblock != 1 && blockmove == 1) {
			emx = LOWORD(lParam);
			emy = HIWORD(lParam);

			blockmove = 0;
			mouseblock = 1;

			SetTimer(hwnd, 1, 20, NULL);

			InvalidateRect(hwnd, &rt_left, TRUE);
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (abs(emx - smx) > abs(emy - smy) && emx > smx)//오른쪽으로 이동
			{
				stoptimer = 0;

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
					{
						block[i].x += 10;
						block[i].combine_check = 1;

						for (int j = 0; j < obstacle; j++)
						{
							if (block[i].x + D_6 >= obstacleblock[j].x && block[i].x <= obstacleblock[j].x && block[i].y == obstacleblock[j].y && obstacleblock[j].num != 0)
							{
								block[i].x = obstacleblock[j].x - D_6;
								block[i].combine_check = 0;
							}
						}

						for (int j = 0; j < 36; j++)
						{
							if (i != j && block[i].x + D_6 > block[j].x && block[i].x < block[j].x && block[i].y == block[j].y && block[i].num != block[j].num && block[j].num != 0)
							{
								block[i].x = block[j].x - D_6;
								block[i].combine_check = 0;
							}
						}

						if (block[i].x >= 500) {
							block[i].x = 500;
							block[i].combine_check = 0;
						}

					}

					if (block[i].combine_check == 0)
						stoptimer++;
				}

				//패배(더이상 2를 추가할 장소가 없을시)
				failnum = 36 - (obstacle - (fullchance - chance));

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
						failnum--;
				}

				if (failnum == 0)
				{
					mouseblock = 1;
					fail = 1;
					KillTimer(hwnd, 1);
				}

				if (stoptimer == 36 && failnum != 0)
				{
					stoptimer = 0;
					mouseblock = 0;

					for (int i = 0; i < 36; i++)	//2추가 
					{
						if (block[i].num == 0)
						{
							while (1)
							{
								block[i].x = rand() % 6 * D_6;
								block[i].y = rand() % 6 * D_6;
								flag = 1;

								for (int j = 0; j < obstacle; j++)
								{
									if (obstacleblock[j].num != 0 && block[i].x == obstacleblock[j].x && block[i].y == obstacleblock[j].y)
									{
										flag = 0;
										break;
									}
								}

								for (int j = 0; j < 36; j++)
								{
									if (i != j && block[j].num != 0 && block[i].x == block[j].x && block[i].y == block[j].y)
									{
										flag = 0;
										break;
									}
								}

								if (flag == 1)
									break;
							}
							block[i].num = 2;
							break;
						}

					}
					KillTimer(hwnd, 1);
				}
			}
			else if (abs(emx - smx) > abs(emy - smy) && emx < smx)//왼쪽으로 이동
			{
				stoptimer = 0;

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
					{
						block[i].x -= 10;
						block[i].combine_check = 1;

						for (int j = 0; j < obstacle; j++)
						{
							if (block[i].x <= obstacleblock[j].x + D_6 && block[i].x >= obstacleblock[j].x && block[i].y == obstacleblock[j].y && obstacleblock[j].num != 0)
							{
								block[i].x = obstacleblock[j].x + D_6;
								block[i].combine_check = 0;
							}
						}

						for (int j = 0; j < 36; j++)
						{
							if (i != j && block[i].x < block[j].x + D_6 && block[i].x > block[j].x && block[i].y == block[j].y && block[i].num != block[j].num && block[j].num != 0)
							{
								block[i].x = block[j].x + D_6;
								block[i].combine_check = 0;
							}
						}

						if (block[i].x <= 0) {
							block[i].x = 0;
							block[i].combine_check = 0;
						}

					}

					if (block[i].combine_check == 0)
						stoptimer++;
				}

				//패배(더이상 2를 추가할 장소가 없을시)
				failnum = 36 - (obstacle - (fullchance - chance));

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
						failnum--;
				}

				if (failnum == 0)
				{
					mouseblock = 1;
					fail = 1;
					KillTimer(hwnd, 1);
				}

				if (stoptimer == 36 && failnum != 0)
				{
					stoptimer = 0;
					mouseblock = 0;

					for (int i = 0; i < 36; i++)	//2추가 
					{
						if (block[i].num == 0)
						{
							while (1)
							{
								block[i].x = rand() % 6 * D_6;
								block[i].y = rand() % 6 * D_6;
								flag = 1;

								for (int j = 0; j < obstacle; j++)
								{
									if (obstacleblock[j].num != 0 && block[i].x == obstacleblock[j].x && block[i].y == obstacleblock[j].y)
									{
										flag = 0;
										break;
									}
								}

								for (int j = 0; j < 36; j++)
								{
									if (i != j && block[j].num != 0 && block[i].x == block[j].x && block[i].y == block[j].y)
									{
										flag = 0;
										break;
									}
								}

								if (flag == 1)
									break;
							}
							block[i].num = 2;
							break;
						}

					}
					KillTimer(hwnd, 1);
				}
			}
			else if (abs(emx - smx) < abs(emy - smy) && emy > smy)//아래로 이동
			{
				stoptimer = 0;

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
					{
						block[i].y += 10;
						block[i].combine_check = 1;

						for (int j = 0; j < obstacle; j++)
						{
							if (block[i].y + D_6 >= obstacleblock[j].y && block[i].y <= obstacleblock[j].y && block[i].x == obstacleblock[j].x && obstacleblock[j].num != 0)
							{
								block[i].y = obstacleblock[j].y - D_6;
								block[i].combine_check = 0;
							}
						}

						for (int j = 0; j < 36; j++)
						{
							if (i != j && block[i].y + D_6 > block[j].y && block[i].y < block[j].y && block[i].x == block[j].x && block[i].num != block[j].num && block[j].num != 0)
							{
								block[i].y = block[j].y - D_6;
								block[i].combine_check = 0;
							}
						}

						if (block[i].y >= 500) {
							block[i].y = 500;
							block[i].combine_check = 0;
						}

					}

					if (block[i].combine_check == 0)
						stoptimer++;
				}

				//패배(더이상 2를 추가할 장소가 없을시)
				failnum = 36 - (obstacle - (fullchance - chance));

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
						failnum--;
				}

				if (failnum == 0)
				{
					mouseblock = 1;
					fail = 1;
					KillTimer(hwnd, 1);
				}

				if (stoptimer == 36 && failnum != 0)
				{
					stoptimer = 0;
					mouseblock = 0;

					for (int i = 0; i < 36; i++)	//2추가 
					{
						if (block[i].num == 0)
						{
							while (1)
							{
								block[i].x = rand() % 6 * D_6;
								block[i].y = rand() % 6 * D_6;
								flag = 1;

								for (int j = 0; j < obstacle; j++)
								{
									if (obstacleblock[j].num != 0 && block[i].x == obstacleblock[j].x && block[i].y == obstacleblock[j].y)
									{
										flag = 0;
										break;
									}
								}

								for (int j = 0; j < 36; j++)
								{
									if (i != j && block[j].num != 0 && block[i].x == block[j].x && block[i].y == block[j].y)
									{
										flag = 0;
										break;
									}
								}

								if (flag == 1)
									break;
							}
							block[i].num = 2;
							break;
						}

					}
					KillTimer(hwnd, 1);
				}
			}
			else if (abs(emx - smx) < abs(emy - smy) && emy < smy)//위로 이동
			{
				stoptimer = 0;

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
					{
						block[i].y -= 10;
						block[i].combine_check = 1;

						for (int j = 0; j < obstacle; j++)
						{
							if (block[i].y <= obstacleblock[j].y + D_6 && block[i].y >= obstacleblock[j].y && block[i].x == obstacleblock[j].x && obstacleblock[j].num != 0)
							{
								block[i].y = obstacleblock[j].y + D_6;
								block[i].combine_check = 0;
							}
						}

						for (int j = 0; j < 36; j++)
						{
							if (i != j && block[i].y < block[j].y + D_6 && block[i].y > block[j].y && block[i].x == block[j].x && block[i].num != block[j].num && block[j].num != 0)
							{
								block[i].y = block[j].y + D_6;
								block[i].combine_check = 0;
							}
						}

						if (block[i].y <= 0) {
							block[i].y = 0;
							block[i].combine_check = 0;
						}

					}

					if (block[i].combine_check == 0)
						stoptimer++;
				}

				//패배(더이상 2를 추가할 장소가 없을시)
				failnum = 36 - (obstacle - (fullchance - chance));

				for (int i = 0; i < 36; i++)
				{
					if (block[i].num != 0)
						failnum--;
				}

				if (failnum == 0)
				{
					mouseblock = 1;
					fail = 1;
					KillTimer(hwnd, 1);
				}

				if (stoptimer == 36 && failnum != 0)
				{
					stoptimer = 0;
					mouseblock = 0;

					for (int i = 0; i < 36; i++)	//2추가 
					{
						if (block[i].num == 0)
						{
							while (1)
							{
								block[i].x = rand() % 6 * D_6;
								block[i].y = rand() % 6 * D_6;
								flag = 1;

								for (int j = 0; j < obstacle; j++)
								{
									if (obstacleblock[j].num != 0 && block[i].x == obstacleblock[j].x && block[i].y == obstacleblock[j].y)
									{
										flag = 0;
										break;
									}
								}

								for (int j = 0; j < 36; j++)
								{
									if (i != j && block[j].num != 0 && block[i].x == block[j].x && block[i].y == block[j].y)
									{
										flag = 0;
										break;
									}
								}
								block[i].num = 2;
								if (flag == 1)
									break;
							}

							break;
						}
						
					}
					KillTimer(hwnd, 1);
				}
			}
			else             //같은 자리 클릭시
			{
				mouseblock = 0;
				KillTimer(hwnd, 1);
			}
			
			for (int i = 0; i < 36; i++)	//같은 숫자 겹쳐질때
			{
				if (block[i].num != 0)
				{
					for (int j = i - 1; j >= 0; j--)
					{
						if (block[i].x == block[j].x && block[i].y == block[j].y && block[i].num == block[j].num)
						{
							block[i].num = 0;
							block[i].combine_check = 0;
							block[j].num *= 2;
						}
					}
				}

			}

			break;

			case 2:
				obstacleblock[chanceindx].x++; obstacleblock[chanceindx].y++;
				usingchancelange -= 2;

				if (usingchancelange == 0)
				{
					mouseblock = 0;
					chanceusing = 0;
					KillTimer(hwnd, 2);
				}
				break;

		}
		InvalidateRect(hwnd, &rt_left, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1); KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}