#include <windows.h>
#include <tchar.h>
#include<cstdlib>
#include<time.h>

#define W 70								
//H가 W의 2배가 아닐경우 그림 뭉그러짐(수식 수정필요)
#define H 140

int Cr{ 100 }, Cg{ 100 }, Cb{ 100 };
int Cr_{ 0 }, Cg_{ 225 }, Cb_{200};

int Hr{ 225 }, Hg{ 0 }, Hb{ 0 };
int Hr_{ 225 }, Hg_{ 225 }, Hb_{ 0 };

int Ptr{ 5 }, Ptg{ 20 }, Ptb{200};
int Ptr_{ 255 }, Ptg_{ 0 }, Ptb_{ 200 };

int Pcr{ 50 }, Pcg{ 50 }, Pcb{ 150 };
int Pcr_{ 5 }, Pcg_{ 255 }, Pcb_{ 100 };

int Tr{ 200 }, Tg{ 200 }, Tb{ 50 };
int Tr_{ 100 }, Tg_{ 50 }, Tb_{ 200 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-8";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

struct Color {
	//펜색
	int r, g, b;
	//브러쉬 색
	int r_, g_, b_;
};

void ChangeColor(int& r, int& g, int& b, int& r_, int& g_, int& b_)
{
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;

	r_ = rand() % 256;
	g_ = rand() % 256;
	b_ = rand() % 256;
}

void CenterRectangle(HDC hdc)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	hPen = CreatePen(PS_SOLID, 2, RGB(Cr, Cg, Cb));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(Cr_, Cg_, Cb_));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, 280, 200, 450, 350);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
};

void Hourglass(HDC hdc, int x, int y, bool small_size)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	POINT point[4] = { {x + W,y + W},{x + W + H,y + W + H},{x + W,y + W + H},{x + W + H,y + W} };

	if (small_size == 1) {
		int adj = W;

		point[0] = { x + W / 2 + adj,y + W / 2 + adj };
		point[1] = { x + W / 2 + H / 2 + adj,y + W / 2 + H / 2 + adj };
		point[2] = { x + W / 2 + adj,y + W / 2 + H / 2 + adj };
		point[3] = { x + W / 2 + H / 2 + adj,y + W / 2 + adj };
	}

	hPen = CreatePen(PS_SOLID, 2, RGB(Hr, Hg, Hb));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(Hr_, Hg_, Hb_));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Polygon(hdc, point, 4);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
};

void Pentagon(HDC hdc, int x, int y, bool small_size)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	POINT point[5] = { {x + 125,y + 75},{x + 185,y + 130},{x + 165,y + 215},{x + 85,y + 215},{x + 65,y + 130} };

	if (small_size == 1) {
		int adj = W - 8;

		point[0] = { x + 125 / 2 + adj,y + 75 / 2 + adj };
		point[1] = { x + 185 / 2 + adj,y + 130 / 2 + adj };
		point[2] = { x + 165 / 2 + adj,y + 215 / 2 + adj };
		point[3] = { x + 85 / 2 + adj,y + 215 / 2 + adj };
		point[4] = {x + 65 / 2 + adj, y + 130 / 2 + adj };
	}

	hPen = CreatePen(PS_SOLID, 2, RGB(Ptr, Ptg, Ptb));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(Ptr_, Ptg_, Ptb_));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Polygon(hdc, point, 5);


	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
};

void Pacman(HDC hdc, int x, int y, bool small_size)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	hPen = CreatePen(PS_SOLID, 2, RGB(Pcr, Pcg, Pcb));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(Pcr_, Pcg_, Pcb_));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	if (small_size != 1)
		Pie(hdc, x + 50, y + 50, x + 200, y + 200, x + 190, y + 75, x + 190, y + 165);
	else {
		int adj = W - 10;

		Pie(hdc, x + 50 / 2 + adj, y + 50 / 2 + adj, x + 200 / 2 + adj, y + 200 / 2 + adj, x + 190 / 2 + adj, y + 75 / 2 + adj, x + 190 / 2 + adj, y + 165 / 2 + adj);
	}

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
};

void Triangle(HDC hdc, int x, int y, bool small_size)
{
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	POINT point[3] = { {x + H,y + W},{x + W + H,y + W + H},{x + W,y + W + H} };

	if (small_size == 1) {
		int adj = W;

		point[0] = { x + H / 2 + adj,y + W / 2 + adj };
		point[1] = { x + W / 2 + H / 2 + adj,y + W / 2 + H / 2 + adj };
		point[2] = { x + W / 2 + adj,y + W / 2 + H / 2 + adj };
	}

	hPen = CreatePen(PS_SOLID, 2, RGB(Tr, Tg, Tb));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(Tr_, Tg_, Tb_));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Polygon(hdc, point, 3);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
};

void Printscreen(HDC hdc, int temporary_color_change[], int spin, int select, bool small_size)
{
	HPEN hPen, oldPen;
	int R = 0, G = 0, B = 0;
	int R_ = 0, G_ = 0, B_ = 0;
	HBRUSH hBrush, oldBrush;
	
	int x1 = 225;
	int y1 = -50;
	int x2 = 450;
	int y2 = 130;
	int x3 = 225;
	int y3 = 325;
	int x4 = 0;
	int y4 = 130;

	int Hx = 0, Hy = 0;		//모래시계 좌표
	int Ptx = 0, Pty = 0;	//오각형 좌표
	int Px = 0, Py = 0;		//팩맨 좌표
	int Tx = 0, Ty = 0;		//삼각형 좌표

	int center_x{ 225 }, center_y{ 135 };

	switch (spin) // 12시방향 도형이 - 1:모래시계, 2:오각형, 3:팩맨, 4:삼각형
	{
	case 1:
		Hx = x1; Hy = y1;
		Ptx = x2; Pty = y2;
		Px = x3; Py = y3;
		Tx = x4; Ty = y4;
		break;
	case 2:
		Hx = x4; Hy = y4;
		Ptx = x1; Pty = y1;
		Px = x2; Py = y2;
		Tx = x3; Ty = y3;
		break;
	case 3:
		Hx = x3; Hy = y3;
		Ptx = x4; Pty = y4;
		Px = x1; Py = y1;
		Tx = x2; Ty = y2;
		break;
	case 4:
		Hx = x2; Hy = y2;
		Ptx = x3; Pty = y3;
		Px = x4; Py = y4;
		Tx = x1; Ty = y1;
		break;
	}

	hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(R_, G_, B_));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	srand(time(NULL));
	
	//중앙 사각형
	{
		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;

		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(R_, G_, B_));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}
	
	if (temporary_color_change[0] == 1)
		Rectangle(hdc, 280, 200, 450, 350);
	else
		CenterRectangle(hdc);

	//모래시계
	{
		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;

		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(R_, G_, B_));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}
	
	if (temporary_color_change[1] == 1) {
		POINT Hpoint[4] = { {Hx + W,Hy + W},{Hx + W + H,Hy + W + H},{Hx + W,Hy + W + H},{Hx + W + H,Hy + W} };
		Polygon(hdc, Hpoint, 4);

		if (select == 1) {
				POINT H_point[4] = { {center_x + W,center_y + W},{center_x + W + H,center_y + W + H},{center_x + W,center_y + W + H},{center_x + W + H,center_y + W} };
				Polygon(hdc, H_point, 4);
		}
	}
	else {
		Hourglass(hdc, Hx, Hy, 0);

		if (select == 1) {
				Hourglass(hdc, center_x, center_y, small_size);
		}
	}

	//오각형
	{
		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;

		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(R_, G_, B_));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}

	center_x += 15;
	center_y -= 5;

	if (temporary_color_change[2] == 1) {
		POINT Ptpoint[5] = { {Ptx + 125,Pty + 75},{Ptx + 185,Pty + 130},{Ptx + 165,Pty + 215},{Ptx + 85,Pty + 215},{Ptx + 65,Pty + 130} };
		Polygon(hdc, Ptpoint, 5);

		if (select == 2) {
			POINT P_point[5] = { {center_x + 125,center_y + 75},{center_x + 185,center_y + 130},{center_x + 165,center_y + 215},{center_x + 85,center_y + 215},{center_x + 65,center_y + 130} };
			Polygon(hdc, P_point, 5);
		}
	}
	else {
		Pentagon(hdc, Ptx, Pty, 0);

		if (select == 2)
			Pentagon(hdc, center_x, center_y, small_size);
	}

	//팩맨
	{
		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;

		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(R_, G_, B_));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}

	center_y += 20;
	
	if (temporary_color_change[3] == 1) {
		Pie(hdc, Px + 50, Py + 50, Px + 200, Py + 200, Px + 190, Py + 75, Px + 190, Py + 165);

		if (select == 3) 
			Pie(hdc, center_x + 50, center_y + 50, center_x + 200, center_y + 200, center_x + 190, center_y + 75, center_x + 190, center_y + 165);
	}
	else {
		Pacman(hdc, Px, Py, 0);

		if (select == 3)
			Pacman(hdc, center_x, center_y, small_size);
	}

	//삼각형
	{
		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;

		R_ = rand() % 256;
		G_ = rand() % 256;
		B_ = rand() % 256;

		hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(R_, G_, B_));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}

	center_x -= 15;
	center_y -= 15;

	if (temporary_color_change[4] == 1) {
		POINT Tpoint[3] = { {Tx + H,Ty + W},{Tx + W + H,Ty + W + H},{Tx + W,Ty + W + H} };
		Polygon(hdc, Tpoint, 3);

		if (select == 4) {
			POINT T_point[3] = { {center_x + H,center_y + W},{center_x + W + H,center_y + W + H},{center_x + W,center_y + W + H} };
			Polygon(hdc, T_point, 3);
		}
	}
	else {
		Triangle(hdc, Tx, Ty, 0);

		if (select == 4)
			Triangle(hdc, center_x, center_y, small_size);
	}

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
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

	static int spin{ 1 }; // 12시방향 도형이 - 1:모래시계, 2:오각형, 3:팩맨, 4:삼각형

	static int temporary_color_change[5] { 0 };//인덱스-[0]:가운데 사각형, [1]:모래시계, [2]:오각형, [3]:팩맨, [4]:삼각형

	static int select_obj{ 0 };	//선택하면 본래 색 자체 바뀜 - 1:모래시계, 2:오각형, 3:팩맨, 4:삼각형 

	static bool small_size{ 0 };

	srand(time(NULL));

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		Printscreen(hdc, temporary_color_change, spin, select_obj, small_size);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'R':
		case 'r':
			temporary_color_change[0] = 1;
			break;
		case 'S':
		case 's':
			select_obj = 1;
			small_size = 0;
			temporary_color_change[1] = 1;
			break;
		case 'P':
		case 'p':
			select_obj = 2;
			small_size = 0;
			temporary_color_change[2] = 1;
			break;
		case 'E':
		case 'e':
			select_obj = 3;
			small_size = 0;
			temporary_color_change[3] = 1;
			break;
		case 'T':
		case 't':
			select_obj = 4;
			small_size = 0;
			temporary_color_change[4] = 1;
			break;
		case 'C':
			spin--;
			if (spin == 0)
				spin = 4;
			ChangeColor(Cr, Cg, Cb, Cr_, Cg_, Cb_);
			ChangeColor(Hr, Hg, Hb, Hr_, Hg_, Hb_);
			ChangeColor(Ptr, Ptg, Ptb, Ptr_, Ptg_, Ptb_);
			ChangeColor(Pcr, Pcg, Pcb, Pcr_, Pcg_, Pcb_);
			ChangeColor(Tr, Tg, Tb, Tr_, Tg_, Tb_);
			break;
		case 'c':
			spin++;
			if (spin == 5)
				spin = 1;
			ChangeColor(Cr, Cg, Cb, Cr_, Cg_, Cb_);
			ChangeColor(Hr, Hg, Hb, Hr_, Hg_, Hb_);
			ChangeColor(Ptr, Ptg, Ptb, Ptr_, Ptg_, Ptb_);
			ChangeColor(Pcr, Pcg, Pcb, Pcr_, Pcg_, Pcb_);
			ChangeColor(Tr, Tg, Tb, Tr_, Tg_, Tb_);
			break;
		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case 'R':
		case 'r':
			temporary_color_change[0] = 0;
			break;
		case 'S':
		case 's':
			temporary_color_change[1] = 0;
			break;
		case 'P':
		case 'p':
			temporary_color_change[2] = 0;
			break;
		case 'E':
		case 'e':
			temporary_color_change[3] = 0;
			break;
		case 'T':
		case 't':
			temporary_color_change[4] = 0;
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			select_obj = 1;
			small_size = 1;
			ChangeColor(Hr, Hg, Hb, Hr_, Hg_, Hb_);
			break;
		case VK_RIGHT:
			select_obj = 2;
			small_size = 1;
			ChangeColor(Ptr, Ptg, Ptb, Ptr_, Ptg_, Ptb_);
			break;
		case VK_DOWN:
			select_obj = 3;
			small_size = 1;
			ChangeColor(Pcr, Pcg, Pcb, Pcr_, Pcg_, Pcb_);
			break;
		case VK_LEFT:
			select_obj = 4;
			small_size = 1;
			ChangeColor(Tr, Tg, Tb, Tr_, Tg_, Tb_);
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