// ---- 버그 --------
// 충돌체크가 정확하지 못함 (마주보고 부딪친다해도 만약 충돌전 0칸 간격이였다면 다음 루프때 서로 위치가 엇갈려 지나가서 이를 충돌체크 못하게됨, 만역 한칸간격이었다면 충돌체크됨) 
//-------------------


#include <windows.h>
#include <tchar.h>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>

#define L4 15

//#define TEST_SPAWN_RANGE 5		//def = 40
//#define TEST_POP_TAIL_INTERVAL 3	//def = 30
//#define TEST_MOVE_PATTERN 4			//def = 6
//#define TEST_POP_TAIL_TOTAL 10	//def = 20
//#define TEST_TIMER_INTERVAL 20	//def = 40
#define START_POS 0		//def = 0


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 3-1";
void CALLBACK TimerProc1(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime);
void CALLBACK TimerProc2(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime);

typedef struct BLOCK
{
	RECT rt;
	int move;		//1 - 오른쪽   2 - 왼쪽   3 - 위    4 - 아래    5 - 네모 서클	6 - 가만히
	int move5_seq;
	BOOL b_moved;		//0 - 양의 방향  1 - 음의 방향
	BOOL follow_character;
	BOOL follow_food;
	int num_character;
	int num_fhead;
	int num_init_follow_food;
}BLOCK;

struct Obstacle
{
	RECT rt;
};

BLOCK block[21];

Obstacle obstacle[10];
int obstacle_indx = 0;
bool pause;
int eaten_count;

int tail = 610, a = 1, b = 1000, c = 1000, d = 5;	// tail-꼬리추가 텀 조절변수 , a-꼬리추가에 사용하는 인덱스변수 , b-timeproc1 시간조절변수 , c-timeproc2 시간조절변수 , d-주인공 속도조절 변수 
int t = 0;	//꼬리원 순서 변수
int jump = 0;	//점프를 위한 변수
int uturn = 0;	//유턴을 위한 변수

int FindLastTail(int backward)
{
	for (int i = 1; i < 21; i++) {
		if (block[i].num_fhead == backward) {
			return FindLastTail(i);
		}
	}

	return backward;
}

int FindNumInitFollowTail(int backward, int eaten_count)
{
	for (int i = 1; i < 21; i++) {
		if (block[i].num_fhead == backward) {
			return FindNumInitFollowTail(i, ++eaten_count);
		}
	}

	return eaten_count;
}

//오류 있음
//int FindHead(int forward) 
//{
//	int head;
//
//	for (int i = 1; i < 21; i++) {
//		if (i == forward) {
//			if (block[i].num_head != 0)
//				return FindHead(block[i].num_head);
//			else
//				head = i;
//		}
//	}
//
//	return head;
//}

int FindTail(int head)
{
	for (int i = 1; i < 21; i++) {
		if (block[i].num_fhead == head) {
			return i;
		}
	}

	return 0;
}

void MoveFoodTails(int tail)		// 마지막 꼬리부터 꺼꾸로 작동해야 됨 
{
	for (int head = 1; head < 21; head++) {
		if (block[tail].num_fhead == head) {
			if (block[tail].num_init_follow_food > 0) {
				block[tail].num_init_follow_food--;
			}
			else {
				block[tail].rt = block[head].rt;
			}

			MoveFoodTails(head);
		}
	}
}

//꼬리
void CALLBACK TimerProc1(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime)
{
	HDC hdc;
	hdc = GetDC(hwnd);

	int x;
	int y;

	if (!pause) {
		if (tail > 0)
			tail--;
		if (a <= 20 && tail % 30 == 0) {
			x = rand() % 40;
			y = rand() % 40;
			block[a].rt = { 15 + x * L4,15 + y * L4,15 + x * L4 + L4,15 + y * L4 + L4 };
			block[a].move = rand() % 6 + 1;
			if (block[a].move == 5) {
				block[a].move5_seq = 1;
			}
			block[a].b_moved = rand() % 2;
			block[a].follow_character = 0;
			block[a].follow_food = 0;
			block[a].num_fhead = 0;
			block[a].num_init_follow_food = 0;
			a++;
		}

		b--;
		if (b <= 0)
			b = 1000;

		for (int i = 1; i < 21; i++)
		{
			for (int j = 1; j < 21; j++)
				if (i != j && block[i].rt.left != block[i].rt.right && block[j].rt.left != block[j].rt.right 
					&& block[i].rt.left == block[j].rt.left && block[i].rt.top == block[j].rt.top
					&& block[i].follow_character != 1 && block[j].follow_character != 1) {
					
					if (block[j].follow_food != 1 && block[i].follow_food != 1) {				//둘다 처음 부딪쳐봄
						block[j].follow_food = 1; block[j].num_fhead = 0;	//head
						block[i].follow_food = 1; block[i].num_fhead = j; block[i].num_init_follow_food = 1;		//tail
					}

					else if (block[j].follow_food == 1 && block[j].num_fhead == 0 && block[i].follow_food != 1) {				//머리와 부딪쳤을때
						block[i].follow_food = 1; block[i].num_fhead = FindLastTail(j); block[i].num_init_follow_food = FindNumInitFollowTail(j, 0) + block[FindLastTail(j)].num_init_follow_food;	//tail
					}
					else if (block[i].follow_food == 1 && block[i].num_fhead == 0 && block[j].follow_food != 1) {
						block[j].follow_food = 1; block[j].num_fhead = FindLastTail(i); block[j].num_init_follow_food = FindNumInitFollowTail(i, 0) + block[FindLastTail(i)].num_init_follow_food;	//tail
					}

					else if (block[j].follow_food == 1 && block[j].num_fhead != 0 && block[i].follow_food != 1) {		//꼬리와 부딪쳤을때	
						block[i].follow_food = 1; block[i].num_fhead = FindLastTail(j); block[i].num_init_follow_food= FindNumInitFollowTail(j, 0) + block[FindLastTail(j)].num_init_follow_food;;	//tail
					}
					else if (block[i].follow_food == 1 && block[i].num_fhead != 0 && block[j].follow_food != 1) {
						block[j].follow_food = 1; block[j].num_fhead = FindLastTail(i); block[j].num_init_follow_food= FindNumInitFollowTail(i, 0) + block[FindLastTail(i)].num_init_follow_food;;	//tail
					}
				}
		}

		if (b % 4 == 0) {
			for (int i = 1; i < 21; i++) {
				if (block[i].follow_character != 1) {
					if (block[i].follow_food == 1 && block[i].num_fhead == 0) {
						MoveFoodTails(FindLastTail(i));
					}

					if((block[i].follow_food != 1) || (block[i].follow_food == 1 && block[i].num_fhead == 0)) {
						if ((block[i].rt.top == 600 && block[i].b_moved == 0 && block[i].move == 1) || (block[i].rt.top == 600 && block[i].b_moved == 0 && block[i].move == 2))
						{
							block[i].b_moved = 1;
						}
						else if ((block[i].rt.top == 15 && block[i].b_moved == 1 && block[i].move == 1) || (block[i].rt.top == 15 && block[i].b_moved == 1 && block[i].move == 2))
						{
							block[i].b_moved = 0;
						}
						else if ((block[i].rt.left == 600 && block[i].b_moved == 0 && block[i].move == 3) || (block[i].rt.left == 600 && block[i].b_moved == 0 && block[i].move == 4))
						{
							block[i].b_moved = 1;
						}
						else  if ((block[i].rt.left == 15 && block[i].b_moved == 1 && block[i].move == 3) || (block[i].rt.left == 15 && block[i].b_moved == 1 && block[i].move == 4))
						{
							block[i].b_moved = 0;
						}

						if (block[i].move == 1)
						{
							block[i].rt.left += 15; block[i].rt.right += 15;

							if (block[i].rt.left >= 615) {
								if (block[i].b_moved == 0)
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
								if (block[i].b_moved == 0)
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
								if (block[i].b_moved == 0)
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

							if (block[i].rt.top >= 615) {
								if (block[i].b_moved == 0)
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
							if (block[i].move5_seq == 1 && block[i].rt.left <= 585)
							{
								block[i].rt.left += 15; block[i].rt.right += 15;
								block[i].move5_seq++;
							}
							else if (block[i].move5_seq == 2 && block[i].rt.top >= 30)
							{
								block[i].rt.top -= 15; block[i].rt.bottom -= 15;
								block[i].move5_seq++;
							}
							else if (block[i].move5_seq == 3 && block[i].rt.left >= 30)
							{
								block[i].rt.left -= 15; block[i].rt.right -= 15;
								block[i].move5_seq++;
							}
							else if (block[i].move5_seq == 4 && block[i].rt.top <= 585)
							{
								block[i].rt.top += 15; block[i].rt.bottom += 15;
								block[i].move5_seq = 1;
							}
						}
						else if (block[i].move == 6) {
							continue;
						}
					}
				}
			}
		}
	}

	InvalidateRgn(hwnd, NULL, 1);
	ReleaseDC(hwnd, hdc);
}

//주인공원
void CALLBACK TimerProc2(HWND hwnd, UINT iMsg, UINT idEvent, DWORD dwTime)
{
	HDC hdc;
	hdc = GetDC(hwnd);

	int dx, dy;
	int tail, h_num;

	if (!pause) {
		c--;
		if (c <= 0)
			c = 1000;

		for (int i = 1; i < 21; i++)
		{
			if (block[0].rt.left == block[i].rt.left && block[0].rt.top == block[i].rt.top && block[i].follow_character != 1) {
				if (block[i].follow_food == 1) {
					block[i].follow_character = 1;
					block[i].follow_food = 0;
					block[i].num_character = t;
					block[i].num_fhead = 0;
					t++;

					h_num = i;
					while (1) {
						tail = FindTail(h_num);

						if (tail == 0) {
							break;
						}
						else {
							block[tail].follow_character = 1;
							block[tail].follow_food = 0;
							block[tail].num_character = t;
							block[tail].num_fhead = 0;
							t++;
							h_num = tail;
						}
					}
				}

				else {
					block[i].follow_character = 1;
					block[i].num_character = t;
					t++;
				}
			}
		}


		if (c % d == 0) {
			eaten_count = 0;

			for (int n = t - 1; n >= 0; n--) {		//꼬리부터 그리기(지렁이 만드는 법)

				for (int i = 0; i < 21; i++) {
					if (block[i].num_character == n && block[i].follow_character == 1 && i == 0) {	//머리

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

						if ((block[i].rt.top == 600 && block[i].b_moved == 0 && block[i].move == 1) || (block[i].rt.top == 600 && block[i].b_moved == 0 && block[i].move == 2))
						{
							block[i].b_moved = 1;
						}
						else if ((block[i].rt.top == 15 && block[i].b_moved == 1 && block[i].move == 1) || (block[i].rt.top == 15 && block[i].b_moved == 1 && block[i].move == 2))
						{
							block[i].b_moved = 0;
						}
						else if ((block[i].rt.left == 600 && block[i].b_moved == 0 && block[i].move == 3) || (block[i].rt.left == 600 && block[i].b_moved == 0 && block[i].move == 4))
						{
							block[i].b_moved = 1;
						}
						else  if ((block[i].rt.left == 15 && block[i].b_moved == 1 && block[i].move == 3) || (block[i].rt.left == 15 && block[i].b_moved == 1 && block[i].move == 4))
						{
							block[i].b_moved = 0;
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
								if (block[i].b_moved == 0)
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
								if (block[i].b_moved == 0)
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
								if (block[i].b_moved == 0)
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
								if (block[i].b_moved == 0)
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
					
					else if (block[i].num_character == n && block[i].follow_character == 1)	//꼬리부분들 그리는 법
					{
						for (int j = 0; j < 21; j++)
						{
							if (block[j].num_character == n - 1 && block[j].follow_character == 1)
							{
								block[i].rt = block[j].rt;
								eaten_count++;
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
	WndClass.lpszMenuName = NULL;
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

	static bool start;

	static int mx, my;

	static int boost;

	HPEN hpen, oldpen;
	HBRUSH hbrush, oldbrush;

	static POINT point[3];

	static TCHAR lpOut[1024];

	switch (iMsg)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		start = 0;
		pause = 0;
		boost = 10;
		block[0].rt = { 15 + START_POS,15 + START_POS,30 + START_POS,30 + START_POS }; block[0].move = 1; block[0].b_moved = 0, block[0].num_character = t, t++, block[0].follow_character = 1; block[0].follow_food = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, 670, 0, L"<<명령어 목록>>", 10);
		TextOut(hdc, 670, 17, L"s/S:움직임 시작", 10);
		TextOut(hdc, 670, 34, L"방향키:주인공원 움직이기", 13);
		TextOut(hdc, 670, 51, L"+:주인공원 속도증가", 11);
		TextOut(hdc, 670, 68, L"-:주인공원 속도감소", 11);
		TextOut(hdc, 670, 85, L"j/J::주인공원 점프", 12);
		TextOut(hdc, 670, 102, L"T/t:유턴", 6);
		TextOut(hdc, 670, 119, L"p/P:정지", 6);
		TextOut(hdc, 670, 136, L"Q/q:종료", 6);

		wsprintf(lpOut, L"boost:%d, food pop time:%d, poped food count:%d", boost, tail, a - 1);
		TextOut(hdc, 0, 650, lpOut, lstrlen(lpOut));

		wsprintf(lpOut, L"tail count:%d", eaten_count);
		TextOut(hdc, 0, 670, lpOut, lstrlen(lpOut));

		Printcell(hdc);

		if (start == 1) {
			SetTimer(hwnd, 1, 40, (TIMERPROC) TimerProc1);		//꼬리 원
			SetTimer(hwnd, 2, 40, (TIMERPROC) TimerProc2);		//주인공 원
		}

		for (int i = 20; i >= 0; i--)
		{

			if (i == 0) {
				hbrush = CreateSolidBrush(RGB(255, 255, 0));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			}
			else
			{
				hbrush = CreateSolidBrush(RGB(255, 0, 0));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			}


			if (block[i].move != 0)
				Ellipse(hdc, block[i].rt.left, block[i].rt.top, block[i].rt.right, block[i].rt.bottom);

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
		else if(block[0].move == 1 && mx < block[0].rt.left)
			block[0].move = 2;
		else if (block[0].move == 2 && mx > block[0].rt.right)
			block[0].move = 1;
		else if (block[0].move == 3 && my > block[0].rt.bottom)
			block[0].move = 4;
		else if (block[0].move == 4 && my < block[0].rt.top)
			block[0].move = 3;
		else if (InCircle(block[0].rt.left + 7.5, block[0].rt.top + 7.5, mx, my))	//주인공원 내부 클릭
		{
			KillTimer(hwnd, 2);
			SetTimer(hwnd, 3, 40, NULL);
		}

		for (int i = 1; i < 21; i++)	//따라오고있는 꼬리원 클릭 
		{
			if (InCircle(block[i].rt.left + 7.5, block[i].rt.top + 7.5, mx, my) && block[i].follow_character == 1)
			{
				t = block[i].num_character;
				break;
			}
		}

		for (int i = 1; i < 21; i++)
		{
			if (block[i].num_character >= t)
			{
				block[i].num_character = 0;
				block[i].follow_character = 0;
				block[i].move = rand() % 6 + 1;
				if (block[i].move == 5) {
					block[i].move5_seq = 1;
				}
				block[i].b_moved = rand() % 2;
				block[i].follow_food = 0;
				block[i].num_fhead = 0;
				block[i].num_init_follow_food = 0;
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

	case WM_KEYDOWN:		//대소문자 구분X (+ 대신 'A'와 같이 대문자만 확인 가능)
		switch (wParam)
		{
		case 'S':
			start = 1;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'P':
			pause = !pause;
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
			if(uturn == 0)
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

			if ((block[0].rt.top == 600 && block[0].b_moved == 0 && block[0].move == 1) || (block[0].rt.top == 600 && block[0].b_moved == 0 && block[0].move == 2))
			{
				block[0].b_moved = 1;
			}
			else if ((block[0].rt.top == 15 && block[0].b_moved == 1 && block[0].move == 1) || (block[0].rt.top == 15 && block[0].b_moved == 1 && block[0].move == 2))
			{
				block[0].b_moved = 0;
			}
			else if ((block[0].rt.left == 600 && block[0].b_moved == 0 && block[0].move == 3) || (block[0].rt.left == 600 && block[0].b_moved == 0 && block[0].move == 4))
			{
				block[0].b_moved = 1;
			}
			else  if ((block[0].rt.left == 15 && block[0].b_moved == 1 && block[0].move == 3) || (block[0].rt.left == 15 && block[0].b_moved == 1 && block[0].move == 4))
			{
				block[0].b_moved = 0;
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
					if (block[0].b_moved == 0)
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
					if (block[0].b_moved == 0)
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
					if (block[0].b_moved == 0)
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
					if (block[0].b_moved == 0)
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
