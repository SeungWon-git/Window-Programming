//미구현 내용
//'새게임'클릭 시 "'게임시작'을 다시 클릭해주세요."메세지 띄우기
//'멈춤'상태에서 움직이기 막기(테스트용으로 지금은 풀려있다.)
//'칸수'바꿀시 "'새게임'을 다시 클릭해주세요."메세지 띄우기
//게임 도중 '점수'클릭하여 현재 최고점수보다 낮은 점수를 목표점수로 바꿀시 "'현재 최고점수가 바꾸시려는 점수보다 높아 게임이 바로 끝나게 됩니다. 
//'확인'버튼을 누를시 게임이 종료되고, '취소'버튼을 누를시 다시 게임으로 돌아갑니다." 메세지 출력하고 '확인'버튼과 '취소'버튼 클릭 처리
//***합치기 애니메이션 만들기
// + 위험 알림 표시

// 참고로 연속 쓸어내리기 도중 합치기에서 정확한 중심(좌우합체시)은 따로 존재X (그냥 인덱스가 빠른 놈이 중심이다.)

#include <windows.h>
#include <tchar.h>
#include <random>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Numtiris";

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_int_distribution<> uid{ 1,100 };

const int win_width = 500;		//윈도우 너비
const int win_height = 800;		//윈도우 높이

const int s_block_size = 100;	//블록 원본 크기

HBITMAP Bit_2, Bit_4, Bit_8, Bit_16, Bit_32, Bit_64, Bit_128, Bit_256, Bit_512, Bit_1024, Bit_2048, Bit_4096, Bit_8192;



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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 10, win_width + 18, win_height + 50, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct Block {
	//좌표값
	int x, y;
	int num;	//점수
	bool deploy;	//내려가는 중
	bool slide;		//쓸어 내려가기
};

void ChangeImage(HDC hdc,HBITMAP& oldBit, int num)
{
	switch (num) {
	case 2:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_2);
		break;
	case 4:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_4);
		break;
	case 8:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_8);
		break;
	case 16:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_16);
		break;
	case 32:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_32);
		break;
	case 64:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_64);
		break;
	case 128:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_128);
		break;
	case 256:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_256);
		break;
	case 512:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_512);
		break;
	case 1024:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_1024);
		break;
	case 2048:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_2048);
		break;
	case 4096:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_4096);
		break;
	case 8192:
		oldBit = (HBITMAP)SelectObject(hdc, Bit_8192);
		break;
	}
}

void NextNumber(int& next_num)
{
	int a = uid(dre);

	//2,4,8 - 20% 확률
	if (a < 20)
		next_num = 2;
	else if (a < 40)
		next_num = 4;
	else if (a < 60)
		next_num = 8;

	//16,32 - 15% 확률
	else if (a < 75)
		next_num = 16;
	else if (a < 90)
		next_num = 32;

	//64,128 - 5% 확률
	else if (a < 95)
		next_num = 64;
	else
		next_num = 128;
}

int FindNextBlockIndex(Block b[])
{
	for (int i = 0; i < 65; ++i) {
		if (b[i].num == 0)
			return i;
	}
}

//[충돌검사]
bool CollisionCheck(Block block[], int indx, int block_size, int div_h)
{
	//바닥과 충돌
	if (block[indx].y + block_size >= 100 + block_size * div_h)
	{
		if (block[indx].slide == false){
			block[indx].y = 100 + block_size * div_h - block_size;
			block[indx].deploy = false;
		}
		else if (block[indx].slide) {
			block[indx].y = 100 + block_size * div_h - block_size;
			block[indx].slide = false;
		}

		return true;
	}

	//다른 블록과 충돌
	for (int i = 0; i < 65; ++i) {
		if (i != indx && block[i].num != 0 && block[indx].x == block[i].x && block[indx].y + block_size >= block[i].y && block[indx].y < block[i].y) 
		{
			if (block[indx].slide == false) {
				block[indx].y = block[i].y - block_size;
				block[indx].deploy = false;

				return true;
			}
			else if (block[indx].slide) {	//자기 자신 아래블록또한 자리를 잡아야지만 끝내기
				block[indx].y = block[i].y - block_size;
				if (CollisionCheck(block, i, block_size, div_h)) {
					block[indx].slide = false;

					return true;
				}
			}
		}
	}

	return false;
}

//[좌우하 합치기]
void InspectCombine(Block block[], int indx, int block_size) {
	int multiply = 1;

	//---좌 합치기
	for (int i = 0; i < 65; ++i) {
		if (indx != i && block[indx].num == block[i].num && block[indx].x - block_size == block[i].x && block[indx].y == block[i].y) {
			multiply *= 2;
			block[i].num = 0;

			//위에 있는 블록들 슬라이드
			for (int j = 0; j < 65; ++j) {
				if (i != j && block[j].num != 0 && block[i].x == block[j].x && block[i].y > block[j].y && block[j].slide == false) {
					block[j].slide = true;
				}
			}

			break;
		}
	}

	//---우 합치기
	for (int i = 0; i < 65; ++i) {
		if (indx != i && block[indx].num == block[i].num && block[indx].x + block_size == block[i].x && block[indx].y == block[i].y) {
			multiply *= 2;
			block[i].num = 0;

			//위에 있는 블록들 슬라이드
			for (int j = 0; j < 65; ++j) {
				if (i != j && block[j].num != 0 && block[i].x == block[j].x && block[i].y > block[j].y && block[j].slide == false) {
					block[j].slide = true;
				}
			}

			break;
		}
	}

	//---아래 합치기
	for (int i = 0; i < 65; ++i) {
		if (indx != i && block[indx].num == block[i].num && block[indx].x == block[i].x && block[indx].y + block_size == block[i].y) {
			multiply *= 2;
			block[i].num = 0;

			//위에 있는 블록들 슬라이드
			for (int j = 0; j < 65; ++j) {
				if (indx != j && block[j].num != 0 && block[indx].x == block[j].x && block[indx].y > block[j].y && block[j].slide == false) {
					block[j].slide = true;
				}
			}
	
			block[indx].slide = true;	//근데 여기 마지막 아래 검사에서 합쳐진다면 계속 슬라이드
			break;
		}
	}


	if (multiply != 1) {							//연속 검사
		block[indx].num *= multiply;
		InspectCombine(block, indx, block_size);	//재귀적
	}
}

//[다음 블록 생성 여부검사]
//모든 화면속 블록들이 모두 정지한 후 새로운 블록 생성
bool ChangeNextBlock(Block block[], int indx) {
	for (int i = 0; i < 65; ++i) {
		if (indx == i && block[i].deploy == true)
			return false;
		else if (block[i].num != 0 && block[i].slide == true)
			return false;
	}

	return true;
}

//[좌우 이동 가능 확인]
bool MoveCheck_Block(Block block[], int indx, int block_size, bool dir_right)
{
	//다른 블록과 충돌
	for (int i = 0; i < 65; ++i) {
		if (i != indx && block[i].num != 0 && block[indx].y + block_size >= block[i].y && block[indx].y + block_size <= block[i].y + block_size) {
			//우로 움직였을때 다른 도형에 막힘
			if (block[indx].x + block_size == block[i].x && dir_right == true)
				return false;
			//좌로 움직였을때 다른 도형에 막힘
			else if (block[indx].x - block_size == block[i].x && dir_right ==false)
				return false;
		}
	}

	return true;
}

bool MoveCheck_Boarder(int x, int block_size, bool dir_right) {
	if (x + block_size > 500 - block_size && dir_right == true)
		return false;

	if (x - block_size < -10 && dir_right == false)
		return false;

	return true;
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC mem1dc, mem2dc;
	static HBITMAP hBit_main, hBit_bg;
	static HBITMAP oldBit1, oldBit2;
	PAINTSTRUCT ps; 

	//5X6, 6X8, 7X9
	static int div_w, div_h;

	static int block_size;	//블록출력사이즈

	static bool start;

	static int start_xPos;

	static TCHAR start_word[] = L"\'메뉴\'에서 게임시작을 눌러주세요!";

	static bool pause;

	static Block block[65];
	static int m_indx;		//현재 내려가는 블록의 인덱스		

	static int goal_score;	//목표점수
	static int max_score;	//현재 최고 점수 갱신 변수

	static int quick_down;	//바로 내리기: 0 - 입력이 없는 상태, 1 - 바로 내리기키 클릭, 2 - 바로 내리기 시행중

	static int next_num;	//다음 숫자

	static bool gameover;	//게임 오버

	static bool changeblock;	//새로운 블록으로 바꿀때(생성시켜 움직이기 시작할때)

	static int speed;

	static int while_slide_move;	//쓸어내리기 도중에 움직이게 도와줌 -> 값이 1000이면 쓸어내리기 도중 움직이기를 안했다는 뜻

	switch (iMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		hBit_main = CreateCompatibleBitmap(hdc, win_width, win_height);

		ReleaseDC(hwnd, hdc);

		hBit_bg = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));

		Bit_2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		Bit_4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		Bit_8 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		Bit_16 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		Bit_32 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		Bit_64 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		Bit_128 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		Bit_256 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		Bit_512 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		Bit_1024 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		Bit_2048 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		Bit_4096 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
		Bit_8192 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));

		SetTimer(hwnd, 1, 50, NULL);

		NextNumber(next_num);

		div_w = 5;	div_h = 6;
		block_size = win_width / div_w;

		m_indx = 0;
		start = false;
		start_xPos = 200;
		pause = false;
		goal_score = 512;
		max_score = 0;
		quick_down = 0;
		gameover = false;
		changeblock = true;
		speed = 10;
		while_slide_move = 1000;

		block[0].x = block_size * 2; 	block[0].y = 0;

		for (int i = 0; i < 65; ++i) {
			block[i].num = 0;
			block[i].deploy = false;
		}

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);

		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit_main);

		BitBlt(hdc, 0, 0, win_width, win_height, mem1dc, 0, 0, SRCCOPY);

		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_START:
			start = true;
			break;

		case ID_NEW:
		{
			KillTimer(hwnd, 1);

			SetTimer(hwnd, 1, 50, NULL);

			NextNumber(next_num);

			m_indx = 0;
			start = false;
			start_xPos = 200;
			pause = false;
			max_score = 0;
			quick_down = 0;
			gameover = false;
			changeblock = true;
			speed = 10;
			while_slide_move = 1000;

			block[0].x = block_size * 2; 	block[0].y = 0;

			for (int i = 0; i < 65; ++i) {
				block[i].num = 0;
				block[i].deploy = false;
				block[i].slide = false;
			}
		}
			break;

		case ID_PAUSE:
			pause = !pause;
			break;

		case ID_QUIT:
			DestroyWindow(hwnd);
			break;



		case ID_GOAL_512:
			goal_score = 512;
			break;

		case ID_GOAL_1024:
			goal_score = 1024;
			break;

		case ID_GOAL_2048:
			goal_score = 2048;
			break;

		case ID_GOAL_4096:
			goal_score = 4096;
			break;

		case ID_GOAL_8192:
			goal_score = 8192;
			break;


		case ID_5X6:
		{
			KillTimer(hwnd, 1);

			SetTimer(hwnd, 1, 50, NULL);

			NextNumber(next_num);

			div_w = 5;	div_h = 6;
			block_size = win_width / div_w;

			m_indx = 0;
			start = false;
			start_xPos = 200;
			pause = false;
			goal_score = 512;
			max_score = 0;
			quick_down = 0;
			gameover = false;
			changeblock = true;
			speed = 10;
			while_slide_move = 1000;

			block[0].x = block_size * 2; 	block[0].y = 0;

			for (int i = 0; i < 65; ++i) {
				block[i].num = 0; 
				block[i].deploy = false;
				block[i].slide = false;
			}
		}
			break;

		case ID_6X8:
		{
			KillTimer(hwnd, 1);

			SetTimer(hwnd, 1, 50, NULL);

			NextNumber(next_num);

			div_w = 6;	div_h = 8;
			block_size = win_width / div_w;

			m_indx = 0;
			start = false;
			start_xPos = 200;
			pause = false;
			goal_score = 512;
			max_score = 0;
			quick_down = 0;
			gameover = false;
			changeblock = true;
			speed = 10;
			while_slide_move = 1000;

			block[0].x = block_size * 2; 	block[0].y = 0;

			for (int i = 0; i < 65; ++i) {
				block[i].num = 0;
				block[i].deploy = false;
				block[i].slide = false;
			}
		}
			break;

		case ID_7X9:
		{
			KillTimer(hwnd, 1);

			SetTimer(hwnd, 1, 50, NULL);

			NextNumber(next_num);

			div_w = 7;	div_h = 9;
			block_size = win_width / div_w;

			m_indx = 0;
			start = false;
			start_xPos = 200;
			pause = false;
			goal_score = 512;
			max_score = 0;
			quick_down = 0;
			gameover = false;
			changeblock = true;
			speed = 10;
			while_slide_move = 1000;

			block[0].x = block_size * 3; 	block[0].y = 0;

			for (int i = 0; i < 65; ++i) {
				block[i].num = 0;
				block[i].deploy = false;
				block[i].slide = false;
			}
		}
			break;


		case ID_SLOW:
			speed = 6;
			break;

		case ID_STANDARD:
			speed = 10;
			break;

		case ID_FAST:
			speed = 16;
			break;
		}

		break;

	case WM_CHAR:
		switch (wParam) {
		case '+':
			++speed;
			break;

		case '-':
			--speed;
			break;
		}

		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			if (start) {
				if (block[m_indx].deploy == true && quick_down == 0 && MoveCheck_Block(block, m_indx, block_size, true) && MoveCheck_Boarder(block[m_indx].x, block_size, true)) {
					block[m_indx].x += block_size;
				}
				else if (block[m_indx].deploy == false) {
					if (while_slide_move == 1000 && MoveCheck_Boarder(block[m_indx].x, block_size, true)) {
						while_slide_move = block[m_indx].x + block_size;
					}
					else if (while_slide_move != 1000 && MoveCheck_Boarder(while_slide_move, block_size, true)) {
						while_slide_move += block_size;
					}
				}
			}
			break;

		case VK_LEFT:
			if (start) {
				if (block[m_indx].deploy == true && quick_down == 0 && MoveCheck_Block(block, m_indx, block_size, false) && MoveCheck_Boarder(block[m_indx].x, block_size, false)) {
					block[m_indx].x -= block_size;
				}
				else if (block[m_indx].deploy == false) {
					if (while_slide_move == 1000 && MoveCheck_Boarder(block[m_indx].x, block_size, false)) {
						while_slide_move = block[m_indx].x - block_size;
					}
					else if (while_slide_move != 1000 && MoveCheck_Boarder(while_slide_move, block_size, false)) {
						while_slide_move -= block_size;
					}
				}
			}
			break;

		case VK_DOWN:
			if (start) {
				if (quick_down == 0)
					quick_down = 1;
			}
			break;


		case 'P':
			pause = !pause;
			break;

		case 'Q':
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
	
		break;

	case WM_TIMER:
		if (start == false) {
			//게임 시작을 누르지 않았다면
		}
		//게임오버
		else if (gameover) {
			KillTimer(hwnd, 1);
			TCHAR mesg[100];
			wsprintf(mesg, L"목표 점수: %d \n최고 점수: %d \n다시 시작하려면 \'메뉴\'에서 새게임을 클릭해주세요.", goal_score, max_score);
			MessageBox(NULL, mesg, L"게임오버", MB_ICONWARNING | MB_OK);
		}
		else if (max_score >= goal_score) {
			KillTimer(hwnd, 1);
			TCHAR mesg[100];
			wsprintf(mesg, L"승리하였습니다! \n다시 시작하려면 \'메뉴\'에서 새게임을 클릭해주세요.");
			MessageBox(NULL, mesg, L"승리", MB_ICONINFORMATION | MB_OK);
		}
		else if (pause) {
			//일시중지
		}
		else if (quick_down == 1) {
			//빨리 내리기
			KillTimer(hwnd, 1);
			quick_down = 2;
			SetTimer(hwnd, 1, 10, NULL);
		}
		//게임 루프
		else {
			//[객체 연산처리]
			//생성
			if (changeblock == true) {
				int t_x;
				t_x = block[m_indx].x;

				if (while_slide_move != 1000) {
					t_x = while_slide_move;
					while_slide_move = 1000;
				}

				m_indx = FindNextBlockIndex(block);

				block[m_indx].x = t_x;	block[m_indx].y = 0;
				block[m_indx].num = next_num;
				block[m_indx].deploy = true;
				block[m_indx].slide = false;

				NextNumber(next_num);

				changeblock = false;

				if (quick_down != 0) {
					KillTimer(hwnd, 1);
					quick_down = 0;
					SetTimer(hwnd, 1, 50, NULL);
				}
			}

			//내리기
			if (block[m_indx].deploy == true)
				block[m_indx].y += speed;

			//충돌처리 -- 멈추기, 합치기
			if (CollisionCheck(block, m_indx, block_size, div_h))
				InspectCombine(block, m_indx, block_size);

			//쓸어 내리기
			for (int i = 0; i < 65; ++i) {
				if (block[i].slide == true) {
					block[i].y += speed;

					if (CollisionCheck(block, i, block_size, div_h))
						InspectCombine(block, i, block_size);
				}
			}

			//새로운 블록 내리기 시작
			changeblock = ChangeNextBlock(block, m_indx);

			//게임오버 체크
			for (int i = 0; i < 65; ++i) {
				if (i != m_indx && block[i].num != 0 && block[i].y <= 100) {
					gameover = true;
					break;
				}
			}

			//최대점수 갱신
			for (int i = 0; i < 65; ++i) {
				max_score = block[i].num > max_score ? block[i].num : max_score;
			}
		}
			
		

		//[렌더링 처리]
		hdc = GetDC(hwnd);

		mem1dc = CreateCompatibleDC(hdc);

		mem2dc = CreateCompatibleDC(mem1dc);

		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit_main);

		//배경
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit_bg);

		StretchBlt(mem1dc, 0, 0, win_width, win_height, mem2dc, 0, 0, 48, 48, SRCCOPY);

		//구분선 그리기
		for (int i = 1; i <= div_w; ++i) {
			MoveToEx(mem1dc, block_size * i, 100, NULL);
			LineTo(mem1dc, block_size * i, 100 + div_h * block_size);
		}
		MoveToEx(mem1dc, 0, 100 + div_h * block_size, NULL);
		LineTo(mem1dc, win_width, 100 + div_h * block_size);
		
		//빨간 구분선
		if (start) {
			HPEN hpen, oldpen;

			hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			oldpen = (HPEN)SelectObject(mem1dc, hpen);

			if (while_slide_move != 1000)
				Rectangle(mem1dc, while_slide_move, 100, while_slide_move + block_size, 100 + block_size * div_h);
			else
				Rectangle(mem1dc, block[m_indx].x, 100, block[m_indx].x + block_size, 100 + block_size * div_h);

			SelectObject(mem1dc, oldpen);
			DeleteObject(hpen);
		}

		//블록 그리기
		for (int i = 0; i < 65; ++i) {
			if (block[i].num != 0) {
				ChangeImage(mem2dc, oldBit2, block[i].num);
				StretchBlt(mem1dc, block[i].x, block[i].y, block_size, block_size, mem2dc, 0, 0, s_block_size, s_block_size, SRCCOPY);
			}
		}

		//UI 그리기
		Rectangle(mem1dc, 1, 0, win_width, 100);

		TCHAR mesg[100];
		wsprintf(mesg, L"[다음 블록]");
		TextOut(mem1dc, 10, 3, mesg, lstrlen(mesg));

		wsprintf(mesg, L"[목표 점수] [색깔]");
		TextOut(mem1dc, 210, 40, mesg, lstrlen(mesg));
		wsprintf(mesg, L"%d", goal_score);
		TextOut(mem1dc, 235, 60, mesg, lstrlen(mesg));

		wsprintf(mesg, L"[속도]");
		TextOut(mem1dc, 423, 10, mesg, lstrlen(mesg));
		wsprintf(mesg, L"%d", speed);
		TextOut(mem1dc, 435, 30, mesg, lstrlen(mesg));

		//다음 블록
		ChangeImage(mem2dc, oldBit2, next_num);

		StretchBlt(mem1dc, 10, 20, 75, 75, mem2dc, 0, 0, s_block_size, s_block_size, SRCCOPY);

		if (start == false) {
			SetTextColor(mem1dc, RGB(0, 255, 0));
			SetBkMode(mem1dc, TRANSPARENT);
			TextOut(mem1dc, start_xPos, 400, start_word, lstrlen(start_word));

			start_xPos += 4;
			if (start_xPos > win_width)
				start_xPos = -250;
		}

		
		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);

		ReleaseDC(hwnd, hdc);

		InvalidateRgn(hwnd, NULL, false);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}