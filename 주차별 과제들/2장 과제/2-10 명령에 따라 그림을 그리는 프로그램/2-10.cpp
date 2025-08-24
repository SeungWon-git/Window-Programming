#include <windows.h>
#include <tchar.h>
#include<time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 2-10";

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void Checknum(int* X, WPARAM wParam)
{
	if (wParam == '0')
		*X += 0;
	else if (wParam == '1')
		*X += 1;
	else if (wParam == '2')
		*X += 2;
	else if (wParam == '3')
		*X += 3;
	else if (wParam == '4')
		*X += 4;
	else if (wParam == '5')
		*X += 5;
	else if (wParam == '6')
		*X += 6;
	else if (wParam == '7')
		*X += 7;
	else if (wParam == '8')
		*X += 8;
	else if (wParam == '9')
		*X += 9;
}

void SizeChange(int& a, int b[4], bool increase)
{
	int high = 0;

	for (int i = 0; i < 4; ++i)
		if (a > b[i])
			++high;

	if (increase == 1)
		if (high >= 2)
			++a;
		else
			--a;
	else
		if (high >= 2)
			--a;
		else
			++a;
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

	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	static TCHAR str[100];
	static int count;
	static SIZE size;
	static POINT tpoint[3];
	static POINT spoint[4];
	static POINT ppoint[5];
	static POINT bpoint[4];
	static POINT rpoint[4];
	static POINT dpoint[7];
	static POINT randpoint[4];
	static TCHAR menu[100] = L"<형태>\n1.직선\n2.삼각형\n3.직사각형\n4.오각형\n5.원\n6.팩맨\n7.나비\n8.마름모\n9.화살표\n10.랜덤사각형";

	//static TCHAR lpOut[1024];

	static int N, x, y, X, Y, W;

	static int indx;
	static int current_indx;	// 현재 입력될 도형 인덱스 기록
	static int L_size;			// 입력된 도형 크기 기록(최대10)
	static int LN[10], Lx[10], Ly[10], LX[10], LY[10], LW[10];

	static int tx1, ty1, tx2, ty2, tx3, ty3; //삼각형
	static int px1, py1, px2, py2, px3, py3, px4, py4, px5, py5; //오각형
	static int pacx1, pacy1, pacx2, pacy2, pacx3, pacy3, pacx4, pacy4; //팩맨
	static int rhx1, rhy1, rhx2, rhy2, rhx3, rhy3, rhx4, rhy4; //마름모
	static int dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4, dx5, dy5, dx6, dy6, dx7, dy7; //화살표
	static int rx1, ry1, rx2, ry2, rx3, ry3, rx4, ry4; //랜덤사각형

	static int r, g, b, R, G, B;

	static int N_, x_, y_, X_, Y_, W_;

	static int N1, N2, N3, x1, x2, x3, x4, y1, y2, y3, y4, X1, X2, X3, X4, Y1, Y2, Y3, Y4, W1, W2, W3;

	static int space, enter;

	static int errormsg;

	RECT rt = { 0,520,300,535 };
	RECT rt_ = { 650,0,800,200 };

	srand(time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 3, 15);
		ShowCaret(hwnd);
		count = 0;
		space = 0; enter = 0;
		N = 0; 
		N_ = 0;
		N1 = 0;
		N2 = 0;
		N3 = 0;
		x = 0; 
		x_ = 0;
		x1 = 0;
		x2 = 0;
		x3 = 0;
		x4 = 0;
		y = 0; 
		y_ = 0;
		y1 = 0;
		y2 = 0;
		y3 = 0;
		y4 = 0;
		X = 0; 
		X_ = 0;
		X1 = 0;
		X2 = 0;
		X3 = 0;
		X4 = 0;
		Y = 0;
		Y_ = 0;
		Y1 = 0;
		Y2 = 0;
		Y3 = 0;
		Y4 = 0;
		W = 0; 
		W_ = 0;
		W1 = 0;
		W2 = 0;
		W3 = 0;
		r = 0; R = 255;
		g = 0; G = 255;
		b = 0; B = 255;
		indx = 0;
		current_indx = 0;
		L_size = 0;
		errormsg = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetTextExtentPoint(hdc, str, lstrlen(str), &size);

		if (errormsg == 1)
		{
			//wsprintf(lpOut, L"N:%d x:%d y:%d X:%d Y:%d W:%d", N, x, y, X, Y, W);
			//TextOut(hdc, 10, 10, lpOut, lstrlen(lpOut));
			MessageBox(NULL, L"출력범위를 넘었습니다.", L"에러", MB_OK);
			errormsg = 0;
		}
		else if (errormsg == 2)
		{
			MessageBox(NULL, L"도형을 먼저 그려주세요.", L"에러", MB_OK);
			errormsg = 0;
		}
		else if (errormsg == 3)
		{
			MessageBox(NULL, L"시작점의 좌표는 반드시 끝점의 좌표보다 작아야합니다.", L"에러", MB_OK);
			errormsg = 0;
		}
		else if (errormsg == 4)
		{
			MessageBox(NULL, L"최소 2개이상의 도형을 입력해야 합니다.", L"에러", MB_OK);
			errormsg = 0;
		}

		if (errormsg != 1 && errormsg != 3)
		{
			//wsprintf(lpOut, L"N:%d x:%d y:%d X:%d Y:%d W:%d", N, x, y, X, Y, W);
			//TextOut(hdc, 10, 10, lpOut, lstrlen(lpOut));

			hPen = CreatePen(PS_SOLID, W, RGB(r, g, b));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(R, G, B));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			if (N == 2)
			{
				tx1 = x + (X - x) / 2; ty1 = y;
				tx2 = X; ty2 = Y;
				tx3 = x; ty3 = Y;

				N = 12;
			}
			else if (N == 4)
			{
				px1 = x + (X - x) / 2; py1 = y;
				px2 = X; py2 = y + 2 * (Y - y) / 5;
				px3 = x + 4 * (X - x) / 5; py3 = Y;
				px4 = x + (X - x) / 5; py4 = Y;
				px5 = x; py5 = y + 2 * (Y - y) / 5;

				N = 14;
			}
			else if (N == 6)
			{
				pacx1 = x; pacy1 = y;
				pacx2 = X; pacy2 = Y;
				pacx3 = X; pacy3 = y + (Y - y) / 4;
				pacx4 = X; pacy4 = y + 3 * (Y - y) / 4;

				N = 16;
			}
			else if (N == 8)
			{
				rhx1 = x + (X - x) / 2; rhy1 = y;
				rhx2 = X; rhy2 = y + (Y - y) / 2;
				rhx3 = x + (X - x) / 2; rhy3 = Y;
				rhx4 = x; rhy4 = y + (Y - y) / 2;
				N = 18;
			}
			else if (N == 9)
			{
				dx1 = x + (X - x) / 2; dy1 = y;
				dx2 = X; dy2 = y + (Y - y) / 2;
				dx3 = x + (X - x) / 2; dy3 = Y;
				dx4 = x + (X - x) / 2; dy4 = y + 3 * (Y - y) / 4;
				dx5 = x; dy5 = y + 3 * (Y - y) / 4;
				dx6 = x; dy6 = y + (Y - y) / 4;
				dx7 = x + (X - x) / 2; dy7 = y + (Y - y) / 4;
				N = 19;
			}
			else if (N == 10)
			{
				rx1 = rand() % 766; ry1 = rand() % 541;
				rx2 = rand() % 766; ry2 = rand() % 541;
				rx3 = rand() % 766; ry3 = rand() % 541;
				rx4 = rand() % 766; ry4 = rand() % 541;
				N = 20;
			}

			switch (N)
			{
			case 1:
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, X, Y);
				break;
			case 12:
				tpoint[0] = { tx1,ty1 };
				tpoint[1] = { tx2,ty2 };
				tpoint[2] = { tx3,ty3 };
				Polygon(hdc, tpoint, 3);
				break;
			case 3:
				spoint[0] = { x,y };
				spoint[1] = { X,y };
				spoint[2] = { X,Y };
				spoint[3] = { x,Y };
				Polygon(hdc, spoint, 4);
				break;
			case 14:
				ppoint[0] = { px1,py1 };
				ppoint[1] = { px2,py2 };
				ppoint[2] = { px3,py3 };
				ppoint[3] = { px4,py4 };
				ppoint[4] = { px5,py5 };
				Polygon(hdc, ppoint, 5);
				break;
			case 5:
				Ellipse(hdc, x, y, X, Y);
				break;
			case 16:
				Pie(hdc, pacx1, pacy1, pacx2, pacy2, pacx3, pacy3, pacx4, pacy4);
				break;
			case 7:
				bpoint[0] = { x,y };
				bpoint[1] = { X, Y };
				bpoint[2] = { X,y };
				bpoint[3] = { x,Y };
				Polygon(hdc, bpoint, 4);
				break;
			case 18:
				rpoint[0] = { rhx1,rhy1 };
				rpoint[1] = { rhx2, rhy2 };
				rpoint[2] = { rhx3,rhy3 };
				rpoint[3] = { rhx4,rhy4 };
				Polygon(hdc, rpoint, 4);
				break;
			case 19:
				dpoint[0] = { dx1,dy1 };
				dpoint[1] = { dx2,dy2 };
				dpoint[2] = { dx3,dy3 };
				dpoint[3] = { dx4,dy4 };
				dpoint[4] = { dx5,dy5 };
				dpoint[5] = { dx6,dy6 };
				dpoint[6] = { dx7,dy7 };
				Polygon(hdc, dpoint, 7);
				break;
			case 20:
				randpoint[0] = { rx1,ry1 };
				randpoint[1] = { rx2,ry2 };
				randpoint[2] = { rx3,ry3 };
				randpoint[3] = { rx4,ry4 };
				Polygon(hdc, randpoint, 4);
				break;
			}
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		TextOut(hdc, 0, 503, L"순서 - 형태(1~10), 시작점 좌표<x(0~765),y(0~540)>, 끝점 좌표<X(0~765),Y(0~540)>, 두께(1~10)", lstrlen(L"순서 - 형태(1~10), 시작점 좌표<x(0~765),y(0~540)>, 끝점 좌표<X(0~765),Y(0~540)>, 두께(1~20)"));
		TextOut(hdc, 320, 520, L"[단, 시작점의 좌표는 반드시 끝점의 좌표보다 작아야한다.]", 33);
		DrawText(hdc, menu, lstrlen(menu), &rt_, DT_LEFT | DT_TOP); //메뉴출력
		DrawText(hdc, str, lstrlen(str), &rt, DT_SINGLELINE | DT_LEFT | DT_TOP);
		SetCaretPos(size.cx, 520);
		FrameRect(hdc, &rt, CreateSolidBrush(RGB(0, 0, 0)));
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
	{
		if (wParam >= '0' && wParam <= '9') {
			if (space == 0 && N1 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				N1 = 1;
				Checknum(&N_, wParam);
			}
			else if (space == 0 && N2 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				N1 = 1;
				N_ = N_ * 10;
				Checknum(&N_, wParam);
			}
			else if (space == 0 && N3 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				N1 = 1;
				N_ = N_ * 10;
				Checknum(&N_, wParam);
			}
			else if (space == 1 && x1 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				x1 = 1;
				Checknum(&x_, wParam);
			}
			else if (space == 1 && x2 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				x2 = 1;
				x_ = x_ * 10;
				Checknum(&x_, wParam);
			}
			else if (space == 1 && x3 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				x3 = 1;
				x_ = x_ * 10;
				Checknum(&x_, wParam);
			}
			else if (space == 1 && x4 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				x4 = 1;
				x_ = x_ * 10;
				Checknum(&x_, wParam);
			}
			else if (space == 2 && y1 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				y1 = 1;
				Checknum(&y_, wParam);
			}
			else if (space == 2 && y2 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				y2 = 1;
				y_ = y_ * 10;
				Checknum(&y_, wParam);
			}
			else if (space == 2 && y3 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				y3 = 1;
				y_ = y_ * 10;
				Checknum(&y_, wParam);
			}
			else if (space == 2 && y4 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				y4 = 1;
				y_ = y_ * 10;
				Checknum(&y_, wParam);
			}
			else if (space == 3 && X1 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				X1 = 1;
				Checknum(&X_, wParam);
			}
			else if (space == 3 && X2 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				X2 = 1;
				X_ = X_ * 10;
				Checknum(&X_, wParam);
			}
			else if (space == 3 && X3 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				X3 = 1;
				X_ = X_ * 10;
				Checknum(&X_, wParam);
			}
			else if (space == 3 && X4 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				X4 = 1;
				X_ = X_ * 10;
				Checknum(&X_, wParam);
			}
			else if (space == 4 && Y1 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				Y1 = 1;
				Checknum(&Y_, wParam);
			}
			else if (space == 4 && Y2 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				Y2 = 1;
				Y_ = Y_ * 10;
				Checknum(&Y_, wParam);
			}
			else if (space == 4 && Y3 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				Y3 = 1;
				Y_ = Y_ * 10;
				Checknum(&Y_, wParam);
			}
			else if (space == 4 && Y4 == 0 && wParam != VK_SPACE && wParam != VK_BACK)
			{
				Y4 = 1;
				Y_ = Y_ * 10;
				Checknum(&Y_, wParam);
			}
			else if (space == 5 && W1 == 0 && wParam != VK_SPACE && wParam != VK_BACK && wParam != VK_RETURN)
			{
				W1 = 1;
				Checknum(&W_, wParam);
			}
			else if (space == 5 && W2 == 0 && wParam != VK_SPACE && wParam != VK_BACK && wParam != VK_RETURN)
			{
				W2 = 1;
				W_ = W_ * 10;
				Checknum(&W_, wParam);
			}
			else if (space == 5 && W3 == 0 && wParam != VK_SPACE && wParam != VK_BACK && wParam != VK_RETURN)
			{
				W3 = 1;
				W_ = W_ * 10;
				Checknum(&W_, wParam);
			}
		}

		if (wParam == 'q' || wParam == 'Q')
		{
			DestroyWindow(hwnd);
		}
		else if (wParam == '+')
		{
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				if (W == 10)
				{
					if (N == 12 || N == 14 || N == 16 || N == 18 || N == 19) {
						N -= 10;
						--x; --y; ++X; ++Y;
					}
					else if (N == 20) {
						int rx[4] = { rx1,rx2,rx3,rx4 };
						int ry[4] = { ry1,ry2,ry3,ry4 };

						SizeChange(rx1, rx, 1); SizeChange(rx2, rx, 1); SizeChange(rx3, rx, 1); SizeChange(rx4, rx, 1);
						SizeChange(ry1, ry, 1); SizeChange(ry2, ry, 1); SizeChange(ry3, ry, 1); SizeChange(ry4, ry, 1);
					}
					else{
						--x; --y; ++X; ++Y;
					}
				}
				else
					W++;
			}
		}
		else if (wParam == '-')
		{
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				if (W == 1)
				{
					if (N == 12 || N == 14 || N == 16 || N == 18 || N == 19) {
						N -= 10;
						++x; ++y; --X; --Y;
					}
					else if (N == 20) {
						int rx[4] = { rx1,rx2,rx3,rx4 };
						int ry[4] = { ry1,ry2,ry3,ry4 };

						SizeChange(rx1, rx, 0); SizeChange(rx2, rx, 0); SizeChange(rx3, rx, 0); SizeChange(rx4, rx, 0);
						SizeChange(ry1, ry, 0); SizeChange(ry2, ry, 0); SizeChange(ry3, ry, 0); SizeChange(ry4, ry, 0);
					}
					else {
						++x; ++y; --X; --Y;
					}
				}
				else
					W--;
			}
		}
		else if (wParam == 'k' || wParam == 'K')
		{
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				r = rand() % 256; R = rand() % 256;
				g = rand() % 256; G = rand() % 256;
				b = rand() % 256; B = rand() % 256;
			}
		}
		else if (wParam == 'p' || wParam == 'P')
		{
			if (enter <= 1)
			{
				errormsg = 4;
			}
			else {
				// 이동후 정보 저장을 위해
				LN[indx] = N;
				Lx[indx] = x;
				Ly[indx] = y;
				LX[indx] = X;
				LY[indx] = Y;
				LW[indx] = W;

				--indx;
				if (indx < 0)
					indx = L_size - 1;

				N = LN[indx];
				x = Lx[indx];
				y = Ly[indx];
				X = LX[indx];
				Y = LY[indx];
				W = LW[indx];
			}
		} 
		else if (wParam == 'n' || wParam == 'N')
		{
			if (enter <= 1)
			{
				errormsg = 4;
			}
			else {
				// 이동후 정보 저장을 위해
				LN[indx] = N;
				Lx[indx] = x;
				Ly[indx] = y;
				LX[indx] = X;
				LY[indx] = Y;
				LW[indx] = W;

				if (indx == L_size - 1)
					indx = 0;
				else
					++indx;

				N = LN[indx];
				x = Lx[indx];
				y = Ly[indx];
				X = LX[indx];
				Y = LY[indx];
				W = LW[indx];
			}
		}
		else if (wParam == VK_BACK && count > 0)
			count--;		// 이미 입력한 숫자값을 변경하진 않는다.
		else if (wParam == VK_RETURN)
		{
			count = 0;

			if (x_ >= X_ || y_ >= Y_)
			{
				errormsg = 3;
			}
			else if (N_ >= 1 && N_ <= 10 && x_ >= 0 && x_ <= 765 && y_ >= 0 && y_ <= 540 && x_ >= 0 && X_ <= 765 && y_ >= 0 && Y_ <= 540 && W_ >= 1 && W_ <= 20)
			{
				// 이동후 정보 저장을 위해
				LN[indx] = N;
				Lx[indx] = x;
				Ly[indx] = y;
				LX[indx] = X;
				LY[indx] = Y;
				LW[indx] = W;

				N = N_;
				x = x_;
				y = y_;
				X = X_;
				Y = Y_;
				W = W_;

				++enter;

				if (enter > 0)
				{
					LN[current_indx] = N;
					Lx[current_indx] = x;
					Ly[current_indx] = y;
					LX[current_indx] = X;
					LY[current_indx] = Y;
					LW[current_indx] = W;

					if (current_indx == 9) {
						current_indx = 0;
						indx = 9;
					}
					else {
						indx = current_indx++;
					}

					if (L_size != 10)
						++L_size;
				}

			}
			else
			{
				errormsg = 1;
			}

			N_ = 0;
			N1 = 0;
			N2 = 0;
			N3 = 0;
			x_ = 0; X_ = 0;
			x1 = 0; X1 = 0;
			x2 = 0; X2 = 0;
			x3 = 0; X3 = 0;
			x4 = 0; X4 = 0;
			y_ = 0; Y_ = 0;
			y1 = 0; Y1 = 0;
			y2 = 0; Y2 = 0;
			y3 = 0; Y3 = 0;
			y4 = 0; Y4 = 0;
			W_ = 0;
			W1 = 0;
			W2 = 0;
			W3 = 0;

			space = 0;
		}
		else if (wParam == VK_SPACE && space == 0)
		{
			str[count++] = wParam;
			space++;
		}
		else if (wParam == VK_SPACE && space == 1)
		{
			str[count++] = wParam;
			space++;
		}
		else if (wParam == VK_SPACE && space == 2)
		{
			str[count++] = wParam;
			space++;
		}
		else if (wParam == VK_SPACE && space == 3)
		{
			str[count++] = wParam;
			space++;
		}
		else if (wParam == VK_SPACE && space == 4)
		{
			str[count++] = wParam;
			space++;
		}
		else if (wParam == VK_SPACE && space == 5)
		{
			str[count++] = wParam;
			space++;
		}
		else
			str[count++] = wParam;

		str[count] = '\0';
		InvalidateRect(hwnd, NULL, true);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				x--; X--;
				if (N == 12) {
					tx1--; tx2--; tx3--;
				}
				else if (N == 14) {
					px1--; px2--; px3--; px4--; px5--;
				}
				else if (N == 16) {
					pacx1--; pacx2--; pacx3--; pacx4--;
				}
				else if (N == 18) {
					rhx1--; rhx2--; rhx3--; rhx4--;
				}
				else if (N == 19) {
					dx1--; dx2--; dx3--; dx4--; dx5--; dx6--; dx7--;
				}
				else if (N == 20) {
					rx1--; rx2--; rx3--; rx4--;
				}
			}
			break;
		case VK_RIGHT:
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				x++; X++;
				if (N == 12) {
					tx1++; tx2++; tx3++;
				}
				else if (N == 14) {
					px1++; px2++; px3++; px4++; px5++;
				}
				else if (N == 16) {
					pacx1++; pacx2++; pacx3++; pacx4++;
				}
				else if (N == 18) {
					rhx1++; rhx2++; rhx3++; rhx4++;
				}
				else if (N == 19) {
					dx1++; dx2++; dx3++; dx4++; dx5++; dx6++; dx7++;
				}
				else if (N == 20) {
					rx1++; rx2++; rx3++; rx4++;
				}
			}
			break;
		case VK_UP:
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				y--; Y--;
				if (N == 12) {
					ty1--; ty2--; ty3--;
				}
				else if (N == 14) {
					py1--; py2--; py3--; py4--; py5--;
				}
				else if (N == 16) {
					pacy1--; pacy2--; pacy3--; pacy4--;
				}
				else if (N == 18) {
					rhy1--; rhy2--; rhy3--; rhy4--;
				}
				else if (N == 19) {
					dy1--; dy2--; dy3--; dy4--; dy5--; dy6--; dy7--;
				}
				else if (N == 20) {
					ry1--; ry2--; ry3--; ry4--;
				}
			}
			break;
		case VK_DOWN:
			if (N == 0)
			{
				errormsg = 2;
			}
			else {
				y++; Y++;
				if (N == 12) {
					ty1++; ty2++; ty3++;
				}
				else if (N == 14) {
					py1++; py2++; py3++; py4++; py5++;
				}
				else if (N == 16) {
					pacy1++; pacy2++; pacy3++; pacy4++;
				}
				else if (N == 18) {
					rhy1++; rhy2++; rhy3++; rhy4++;
				}
				else if (N == 19) {
					dy1++; dy2++; dy3++; dy4++; dy5++; dy6++; dy7++;
				}
				else if (N == 20) {
					ry1++; ry2++; ry3++; ry4++;
				}
			}
			break;
		}
		InvalidateRect(hwnd, NULL, true);
		break;
	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}