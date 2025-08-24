#include <windows.h>
#include <tchar.h>
#include "resource.h"


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 5-3";

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 50, 1180, 740, NULL, (HMENU)NULL, hInstance, NULL);

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
	HDC hdc;// , hmemdc;
	HDC mem1dc, mem2dc;
	//static HBITMAP hBitmap;
	static HBITMAP hBit1, hBit2;
	static HBITMAP oldBit1, oldBit2;
	PAINTSTRUCT ps;

	static BOOL drag;

	static BOOL fullscreenpaste;

	static BOOL paste;

	static BOOL bounce;

	static int sx, sy;

	static RECT rt, oldrt, copyframert, copyrt, stretchcopyrt, bounce_rt, bounce_frame_rt;

	static int picnum;

	static BOOL stretch1, stretch2;

	static int copystretch;

	static BOOL hreversal, vreversal;

	static int pastemove, pm_x;	// [pastemove]: 0- 정지,1-(x)오른쪽이동,2-(X)왼쪽이동

	static TCHAR lpOut[1024];

	static int bounce_dir;	//1-↘, 2-↗, 3-↖, 4-↙ 

	switch (iMsg)
	{
	case WM_CREATE:

		SetTimer(hwnd, 1, 50, NULL);
		
		hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

		drag = 0;
		fullscreenpaste = 0;
		paste = 0;
		bounce = 0;
		picnum = 1;
		stretch1 = 0, stretch2 = 0;
		copystretch = 0;
		hreversal = 0; vreversal = 0;
		pastemove = 0; pm_x = 0;
		bounce_dir = 1;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		BitBlt(hdc, 0, 0, 1600, 700, mem1dc, 0, 0, SRCCOPY);

		if (fullscreenpaste == 1)
		{
			StretchBlt(hdc, 0, 0, 1165, 686, mem1dc,
				copyrt.left, copyrt.top, copyrt.right - copyrt.left, copyrt.bottom - copyrt.top, SRCCOPY);
		}

		if (copystretch == 1)	//배율을 사용한 복사를 했을때 
		{
			StretchBlt(hdc, copyrt.left - (copyrt.right - copyrt.left) * 0.125, copyrt.top - (copyrt.bottom - copyrt.top) * 0.125, copyframert.right - copyframert.left, copyframert.bottom - copyframert.top, mem1dc,
				copyrt.left, copyrt.top, copyrt.right - copyrt.left, copyrt.bottom - copyrt.top, SRCCOPY);
			//바뀐 copyrt를 위하여 새로 Rect변수를 만들어줌(테두리를 위해)
			stretchcopyrt = { copyrt.left - (LONG)((copyrt.right - copyrt.left) * 0.125), copyrt.top - (LONG)((copyrt.bottom - copyrt.top) * 0.125), stretchcopyrt.left + copyframert.right - copyframert.left ,stretchcopyrt.top + copyframert.bottom - copyframert.top };
		}
		else if (copystretch == 2)
		{
			StretchBlt(hdc, copyrt.left - (copyrt.right - copyrt.left) * 0.333, copyrt.top - (copyrt.bottom - copyrt.top) * 0.333, copyframert.right - copyframert.left, copyframert.bottom - copyframert.top, mem1dc,
				copyrt.left, copyrt.top, copyrt.right - copyrt.left, copyrt.bottom - copyrt.top, SRCCOPY);
			stretchcopyrt = { copyrt.left - (LONG)((copyrt.right - copyrt.left) * 0.333), copyrt.top - (LONG)((copyrt.bottom - copyrt.top) * 0.333), stretchcopyrt.left + copyframert.right - copyframert.left ,stretchcopyrt.top + copyframert.bottom - copyframert.top };
		}
		else
		{
			StretchBlt(hdc, copyrt.left, copyrt.top, copyrt.right - copyrt.left, copyrt.bottom - copyrt.top, mem1dc,
				copyrt.left, copyrt.top, copyrt.right - copyrt.left, copyrt.bottom - copyrt.top, SRCCOPY);
		}

		if (stretch1 == 1) {	//1.2배 돋보기
			StretchBlt(hdc, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, mem1dc,
				rt.left + (rt.right - rt.left) * 0.1, rt.top + (rt.bottom - rt.top) * 0.1, (rt.right - rt.left) * 0.8, (rt.bottom - rt.top) * 0.8, SRCCOPY);
		}
		else if (stretch2 == 1) {	//1.4배 돋보기
			StretchBlt(hdc, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, mem1dc,
				rt.left + (rt.right - rt.left) * 0.2, rt.top + (rt.bottom - rt.top) * 0.2, (rt.right - rt.left) * 0.6, (rt.bottom - rt.top) * 0.6, SRCCOPY);
		}
		else              //1배 돋보기
		{
			StretchBlt(hdc, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, mem1dc,
				rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SRCCOPY);
		}

		FrameRect(hdc, &rt, CreateSolidBrush(RGB(100, 100, 100)));		//돋보기영역 테두리(안하면 1,2번 클릭시 예전 돋보기테두리가 사라진다...)

		if (copystretch == 0)	//배율 돋보기 사용시 copyrt의 크기가 줄어들기 때문..
			FrameRect(hdc, &copyrt, CreateSolidBrush(RGB(0, 0, 0)));		//복사영역 테두리(이전 복사본 테두리)
		else
			FrameRect(hdc, &stretchcopyrt, CreateSolidBrush(RGB(0, 0, 0)));		//복사영역 테두리(새로운 배율 조정된 복사본 테두리)

		if (paste == 1)		//붙여넣기
		{
			if (hreversal == 1 && vreversal == 1) {
				StretchBlt(hdc, 0, 0, copyframert.right - copyframert.left, copyframert.bottom - copyframert.top, mem1dc,
					copyrt.right, copyrt.bottom, copyrt.left - copyrt.right, copyrt.top - copyrt.bottom, SRCCOPY);
			}
			else if (hreversal == 1) {
				StretchBlt(hdc, 0, 0, copyframert.right - copyframert.left, copyframert.bottom - copyframert.top, mem1dc,
					copyrt.right, copyrt.top, copyrt.left - copyrt.right, copyrt.bottom - copyrt.top, SRCCOPY);
			}
			else if (vreversal == 1) {
				StretchBlt(hdc, 0, 0, copyframert.right - copyframert.left, copyframert.bottom - copyframert.top, mem1dc,
					copyrt.left, copyrt.bottom, copyrt.right - copyrt.left, copyrt.top - copyrt.bottom, SRCCOPY);
			}
			else {
				StretchBlt(hdc, 0, 0, copyframert.right - copyframert.left, copyframert.bottom - copyframert.top, mem1dc,
					copyrt.left, copyrt.top, copyrt.right - copyrt.left, copyrt.bottom - copyrt.top, SRCCOPY);
			}

			if (pastemove == 1)		// x 눌를시(오른쪽으로이동)
			{
				if (hreversal == 1 && vreversal == 1)
				{
					StretchBlt(hdc, pm_x, 0, copyframert.right - pm_x, copyframert.bottom - copyframert.top, hdc, 	//붙여넣기 오른쪽화면
						copyrt.right, copyrt.bottom, pm_x - copyframert.right, copyrt.top - copyrt.bottom, SRCCOPY);
					StretchBlt(hdc, 0, 0, pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 왼쪽 화면
						copyrt.left + pm_x, copyrt.bottom, -pm_x, copyrt.top - copyrt.bottom, SRCCOPY);
				}
				else if (hreversal == 1)
				{
					StretchBlt(hdc, 0, 0, pm_x, copyframert.bottom - copyframert.top, hdc, 								//붙여넣기 왼쪽화면
						copyrt.left + pm_x, copyrt.top, -pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
					StretchBlt(hdc, pm_x, 0, copyframert.right - pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 오른쪽 화면
						copyrt.right, copyrt.top, -copyframert.right + pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
				}
				else if (vreversal == 1)
				{
					StretchBlt(hdc, pm_x, 0, copyframert.right - pm_x, copyframert.bottom, hdc, 	//붙여넣기 오른쪽화면
						copyrt.left, copyrt.bottom, copyframert.right - pm_x, -copyframert.bottom, SRCCOPY);
					StretchBlt(hdc, 0, 0, pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 왼쪽 화면
						copyrt.right - pm_x, copyrt.bottom, pm_x, copyrt.top - copyrt.bottom, SRCCOPY);
				}
				else
				{
					StretchBlt(hdc, 0, 0, pm_x, copyframert.bottom - copyframert.top, hdc, 								//붙여넣기 왼쪽화면
						copyrt.right - pm_x, copyrt.top, pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
					StretchBlt(hdc, pm_x, 0, copyrt.right - copyrt.left - pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 오른쪽 화면
						copyrt.left, copyrt.top, copyrt.right - copyrt.left - pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
				}
			}
			else if (pastemove == 2)// X 눌를시(왼쪽으로이동)
			{
				if (hreversal == 1 && vreversal == 1)
				{
					StretchBlt(hdc, copyframert.right - pm_x, 0, pm_x, copyframert.bottom - copyframert.top, hdc, 	//붙여넣기 오른쪽화면
						copyrt.right, copyrt.bottom, -pm_x, copyrt.top - copyrt.bottom, SRCCOPY);
					StretchBlt(hdc, 0, 0, copyframert.right - pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 왼쪽 화면
						copyrt.right - pm_x, copyrt.bottom, -copyframert.right + pm_x, copyrt.top - copyrt.bottom, SRCCOPY);
				}
				else if (hreversal == 1)
				{
					StretchBlt(hdc, 0, 0, copyframert.right - pm_x, copyframert.bottom - copyframert.top, hdc, 								//붙여넣기 왼쪽화면
						copyrt.right - pm_x, copyrt.top, -copyframert.right + pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
					StretchBlt(hdc, copyframert.right - pm_x, 0, pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 오른쪽 화면
						copyrt.right, copyrt.top, -pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
				}
				else if (vreversal == 1)
				{
					StretchBlt(hdc, copyframert.right - pm_x, 0, pm_x, copyframert.bottom - copyframert.top, hdc, 	//붙여넣기 오른쪽화면
						copyrt.left, copyrt.bottom, pm_x, copyrt.top - copyrt.bottom, SRCCOPY);
					StretchBlt(hdc, 0, 0, copyframert.right - pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 왼쪽 화면
						copyrt.left + pm_x, copyrt.bottom, copyframert.right - pm_x, copyrt.top - copyrt.bottom, SRCCOPY);
				}
				else
				{
					StretchBlt(hdc, 0, 0, copyframert.right - pm_x, copyframert.bottom - copyframert.top, hdc, 								//붙여넣기 왼쪽화면
						copyrt.left + pm_x, copyrt.top, copyrt.right - copyrt.left - pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
					StretchBlt(hdc, copyframert.right - pm_x, 0, pm_x, copyframert.bottom - copyframert.top, hdc,	//붙여넣기 오른쪽 화면
						copyrt.left, copyrt.top, pm_x, copyrt.bottom - copyrt.top, SRCCOPY);
				}
			}

			FrameRect(hdc, &copyframert, CreateSolidBrush(RGB(0, 0, 0)));	//붙여넣기 테두리
		}

		if (bounce == 1) {
			StretchBlt(hdc, bounce_frame_rt.left, bounce_frame_rt.top, bounce_frame_rt.right - bounce_frame_rt.left, bounce_frame_rt.bottom - bounce_frame_rt.top, mem1dc,
				bounce_rt.left, bounce_rt.top, bounce_rt.right - bounce_rt.left, bounce_rt.bottom - bounce_rt.top, SRCCOPY);
		}

		/*wsprintf(lpOut, L"copystretch:%d h:%d v:%d ", copystretch,hreversal,vreversal);
		TextOut(hdc, 10, 10, lpOut, lstrlen(lpOut));*/

		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case '1':
			picnum = 1;
			oldrt = { 0,0,0,0 };	//안하면 화면에 남음
			break;
		case '2':
			picnum = 2;
			oldrt = { 0,0,0,0 };
			break;
		case '3':
			stretch1 = 1; stretch2 = 0;
			oldrt = { 0,0,0,0 };
			break;
		case '4':
			stretch2 = 1; stretch1 = 0;
			oldrt = { 0,0,0,0 };
			break;
		case '0':
			stretch1 = 0; stretch2 = 0;
			oldrt = { 0,0,0,0 };
			break;
		case 'c':
			if (stretch1 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.1; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.8; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.8;
				copystretch = 1;
			}
			else if (stretch2 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.2; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.6; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.6;
				copystretch = 2;
			}
			else {
				copyrt = rt;
				copystretch = 0;
			}

			copyframert.left = 0; copyframert.right = rt.right - rt.left; copyframert.top = 0; copyframert.bottom = rt.bottom - rt.top;

			pm_x = 0;
			oldrt = { 0,0,0,0 };
			break;
		case 'p':
			if (copyframert.right > 0 && paste == 0)	//copy를 누른후에만 붙여넣기 가능
				paste = 1;
			else if (paste == 1)
				paste = 0;
			oldrt = { 0,0,0,0 };
			break;
		case 'f':
			if (copyframert.right > 0 && fullscreenpaste == 0)
				fullscreenpaste = 1;
			else if (fullscreenpaste == 1)
				fullscreenpaste = 0;
			oldrt = { 0,0,0,0 };
			break;
		case 'm':
			copyrt.left++; copyrt.right--; copyrt.top++; copyrt.bottom--;
			copyframert.right -= 2; copyframert.bottom -= 2;
			oldrt = { 0,0,0,0 };
			break;
		case 'n':
			copyrt.left--; copyrt.right++; copyrt.top--; copyrt.bottom++;
			copyframert.right += 2; copyframert.bottom += 2;
			oldrt = { 0,0,0,0 };
			break;
		case 'h':
			if (paste == 1 && hreversal == 0)
				hreversal = 1;
			else if (hreversal == 1)
				hreversal = 0;
			oldrt = { 0,0,0,0 };
			break;
		case 'v':
			if (paste == 1 && vreversal == 0)
				vreversal = 1;
			else if (vreversal == 1)
				vreversal = 0;
			oldrt = { 0,0,0,0 };
			break;
		case 'x':
			if (pastemove != 1) {
				pastemove = 1;
				pm_x = 0;
				SetTimer(hwnd, 2, 100, NULL);
			}
			else {
				pastemove = 0;
				KillTimer(hwnd, 2);
			}
			oldrt = { 0,0,0,0 };
			break;
		case 'X':
			if (pastemove != 2) {
				pastemove = 2;
				pm_x = 0;
				SetTimer(hwnd, 2, 100, NULL);
			}
			else {
				pastemove = 0;
				KillTimer(hwnd, 2);
			}
			oldrt = { 0,0,0,0 };
			break;

		case 'y':
			copyrt.top++; copyrt.bottom--;
			copyframert.bottom -= 2;
			oldrt = { 0,0,0,0 };
			break;
		case 'Y':
			copyrt.top--; copyrt.bottom++;
			copyframert.bottom += 2;
			oldrt = { 0,0,0,0 };
			break;

		case 'A':
			bounce = 1;

			if (stretch1 == 1)
			{
				bounce_rt.left = rt.left + (rt.right - rt.left) * 0.1; bounce_rt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				bounce_rt.right = bounce_rt.left + (rt.right - rt.left) * 0.8; bounce_rt.bottom = bounce_rt.top + (rt.bottom - rt.top) * 0.8;
			}
			else if (stretch2 == 1)
			{
				bounce_rt.left = rt.left + (rt.right - rt.left) * 0.2; bounce_rt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				bounce_rt.right = bounce_rt.left + (rt.right - rt.left) * 0.6; bounce_rt.bottom = bounce_rt.top + (rt.bottom - rt.top) * 0.6;
			}
			else {
				bounce_rt = rt;
			}

			bounce_frame_rt = rt;

			SetTimer(hwnd, 3, 100, NULL);
			break;
		case 'a':
			bounce = 0;
			KillTimer(hwnd, 3);
			break;
		}

		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'R':
			drag = 0;
			fullscreenpaste = 0;
			paste = 0;
			bounce = 0;
			stretch1 = 0, stretch2 = 0;
			copystretch = 0;
			hreversal = 0; vreversal = 0;
			pastemove = 0;
			bounce_dir = 1;

			rt = { 0,0,0,0 };
			oldrt = { 0,0,0,0 };
			copyrt = { 0,0,0,0 };
			copyframert = { 0,0,0,0 };
			bounce_frame_rt = { 0,0,0,0 };
			bounce_rt = { 0,0,0,0 };
			
			KillTimer(hwnd, 2);
			KillTimer(hwnd, 3);
			break;
		case 'Q':
			DestroyWindow(hwnd);
			break;
		case VK_RIGHT:
			//방향키 누를시 현재 돋보기가 복사되어 붙여넣기 된다.
			paste = 1;

			if (stretch1 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.1; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.8; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.8;
				copystretch = 1;
			}
			else if (stretch2 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.2; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.6; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.6;
				copystretch = 2;
			}
			else {
				copyrt = rt;
				copystretch = 0;
			}

			copyframert.left = 0; copyframert.right = rt.right - rt.left; copyframert.top = 0; copyframert.bottom = rt.bottom - rt.top;

			rt.left++; rt.right++;
			copyrt.left++; copyrt.right++;
			oldrt = { 0,0,0,0 };
			break;
		case VK_LEFT:
			paste = 1;

			if (stretch1 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.1; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.8; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.8;
				copystretch = 1;
			}
			else if (stretch2 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.2; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.6; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.6;
				copystretch = 2;
			}
			else {
				copyrt = rt;
				copystretch = 0;
			}

			copyframert.left = 0; copyframert.right = rt.right - rt.left; copyframert.top = 0; copyframert.bottom = rt.bottom - rt.top;

			rt.left--; rt.right--;
			copyrt.left--; copyrt.right--;
			oldrt = { 0,0,0,0 };
			break;
		case VK_UP:
			paste = 1;

			if (stretch1 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.1; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.8; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.8;
				copystretch = 1;
			}
			else if (stretch2 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.2; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.6; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.6;
				copystretch = 2;
			}
			else {
				copyrt = rt;
				copystretch = 0;
			}

			copyframert.left = 0; copyframert.right = rt.right - rt.left; copyframert.top = 0; copyframert.bottom = rt.bottom - rt.top;

			rt.top--; rt.bottom--;
			copyrt.top--; copyrt.bottom--;
			oldrt = { 0,0,0,0 };
			break;
		case VK_DOWN:
			paste = 1;

			if (stretch1 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.1; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.8; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.8;
				copystretch = 1;
			}
			else if (stretch2 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.2; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.6; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.6;
				copystretch = 2;
			}
			else {
				copyrt = rt;
				copystretch = 0;
			}

			copyframert.left = 0; copyframert.right = rt.right - rt.left; copyframert.top = 0; copyframert.bottom = rt.bottom - rt.top;

			rt.top++; rt.bottom++;
			copyrt.top++; copyrt.bottom++;
			oldrt = { 0,0,0,0 };
			break;
		}
		break;
	

	case WM_LBUTTONDOWN:
		drag = 1;

		sx = LOWORD(lParam);
		sy = HIWORD(lParam);

		break;

	case WM_MOUSEMOVE:
		if (drag == 1) {
			hdc = GetDC(hwnd);

			SetROP2(hdc, R2_NOT);	//원래그림 반전
			SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));	//투명 브러쉬

			if (sx < LOWORD(lParam)) {
				rt.left = sx;
				rt.right = LOWORD(lParam);
			}
			else {
				rt.right = sx;
				rt.left = LOWORD(lParam);
			}

			if (rt.top < HIWORD(lParam)) {
				rt.top = sy;
				rt.bottom = HIWORD(lParam);
			}
			else {
				rt.bottom = sy;
				rt.top = HIWORD(lParam);
			}

			Rectangle(hdc, oldrt.left, oldrt.top, oldrt.right, oldrt.bottom);	//지우기
			Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);				//그리기

			oldrt = rt;

			ReleaseDC(hwnd, hdc);
		}
		break;

	case WM_LBUTTONUP:
		drag = 0;
		break;

	case WM_COMMAND:
		switch (wParam) {
		case ID_PIC1:
			picnum = 1;
			oldrt = { 0,0,0,0 };	//안하면 화면에 남음		
			break;
		case ID_PIC2:
			picnum = 2;
			oldrt = { 0,0,0,0 };
			break;
		case ID_EXPAND1:
			stretch1 = 1; stretch2 = 0;
			oldrt = { 0,0,0,0 };
			break;
		case ID_EXPAND2:
			stretch2 = 1; stretch1 = 0;
			oldrt = { 0,0,0,0 };
			break;
		case ID_NOEXPAND:
			stretch1 = 0; stretch2 = 0;
			oldrt = { 0,0,0,0 };
			break;
		case ID_COPY:
			if (stretch1 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.1; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.1;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.8; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.8;
				copystretch = 1;
			}
			else if (stretch2 == 1)
			{
				copyrt.left = rt.left + (rt.right - rt.left) * 0.2; copyrt.top = rt.top + (rt.bottom - rt.top) * 0.2;
				copyrt.right = copyrt.left + (rt.right - rt.left) * 0.6; copyrt.bottom = copyrt.top + (rt.bottom - rt.top) * 0.6;
				copystretch = 2;
			}
			else {
				copyrt = rt;
				copystretch = 0;
			}

			copyframert.left = 0; copyframert.right = rt.right - rt.left; copyframert.top = 0; copyframert.bottom = rt.bottom - rt.top;

			pm_x = 0;
			oldrt = { 0,0,0,0 };
			break;
		case ID_PASTE:
			if (copyframert.right > 0 && paste == 0)	//copy를 누른후에만 붙여넣기 가능
				paste = 1;
			else if (paste == 1)
				paste = 0;
			oldrt = { 0,0,0,0 };
			break;
		case ID_FULLSCREENPASTE:
			if (copyframert.right > 0 && fullscreenpaste == 0)
				fullscreenpaste = 1;
			else if (fullscreenpaste == 1)
				fullscreenpaste = 0;
			oldrt = { 0,0,0,0 };
			break;
		}

		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (picnum == 1)
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			else if (picnum == 2)
				hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));

			hdc = GetDC(hwnd);

			if (hBit1 == NULL)
				hBit1 = CreateCompatibleBitmap(hdc, 1165, 686);

			mem1dc = CreateCompatibleDC(hdc);

			mem2dc = CreateCompatibleDC(mem1dc);

			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
			oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);

			BitBlt(mem1dc, 0, 0, 1165, 686, mem2dc, 0, 0, SRCCOPY);

			SelectObject(mem2dc, oldBit2);
			DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1);
			DeleteDC(mem1dc);

			ReleaseDC(hwnd, hdc);
			InvalidateRect(hwnd, NULL, false);
			break;

		case 2:
			if (pastemove == 1)
			{
				pm_x++;

				if (pm_x == (copyrt.right - copyrt.left))
					pm_x = 0;

			}
			else if (pastemove == 2)
			{
				pm_x++;

				if (pm_x == (copyrt.right - copyrt.left))
					pm_x = 0;

			}

			break;

		case 3:
			if (bounce_dir == 1) {
				bounce_frame_rt.left += 5; bounce_frame_rt.right += 5;
				bounce_frame_rt.top += 5; bounce_frame_rt.bottom += 5;
			}
			else if (bounce_dir == 2) {
				bounce_frame_rt.left += 5; bounce_frame_rt.right += 5;
				bounce_frame_rt.top-= 5; bounce_frame_rt.bottom-= 5;
			}
			else if (bounce_dir == 3) {
				bounce_frame_rt.left-= 5; bounce_frame_rt.right-= 5;
				bounce_frame_rt.top -= 5; bounce_frame_rt.bottom -= 5;
			}
			else if (bounce_dir == 4) {
				bounce_frame_rt.left-= 5; bounce_frame_rt.right-= 5;
				bounce_frame_rt.top += 5; bounce_frame_rt.bottom += 5;
			}

			if (bounce_frame_rt.bottom >= 700) {
				if (bounce_dir == 1)
					bounce_dir = 2;
				else if (bounce_dir == 4)
					bounce_dir = 3;
			}
			else if (bounce_frame_rt.right >= 1180) {
				if (bounce_dir == 1)
					bounce_dir = 4;
				else if (bounce_dir == 2)
					bounce_dir = 3;
			}
			else if (bounce_frame_rt.top <= 0) {
				if (bounce_dir == 2)
					bounce_dir = 1;
				else if (bounce_dir == 3)
					bounce_dir = 4;
			}
			else if (bounce_frame_rt.left <= 0) {
				if (bounce_dir == 3)
					bounce_dir = 2;
				else if (bounce_dir == 4)
					bounce_dir = 1;
			}

			oldrt = { 0,0,0,0 };
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