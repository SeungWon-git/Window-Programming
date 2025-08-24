#include <windows.h>
#include<tchar.h>

#define MAX_LENGTH 30
#define MAX_LINE 10

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = (HICON)LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"Window Class";
	wndClass.lpszMenuName = L"NULL";
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndClass);
	hwnd = CreateWindow(L"Window Class",
		L"메모장",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR str[MAX_LINE][100];
	static TCHAR str_c[100];	//캐럿위치를 위한 변수
	static int count, line;
	static SIZE size;
	static int X, Y;

	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 3, 15);
		ShowCaret(hwnd);
		count = 0;
		line = 0;
		X = size.cx;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetTextExtentPoint(hdc, str_c, lstrlen(str_c), &size);
		for (int i = 0; i < MAX_LINE; i++)
		{
			TextOut(hdc, 0, i * 20, str[i], lstrlen(str[i]));
		}
		X = size.cx;
		Y = line * 20;
		SetCaretPos(X, Y);
		
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		if (count >= MAX_LENGTH)
		{
			str[line][count] = '\0';

			line++;
			count = 0;
			for (int i = 0; i < _tcslen(str_c); ++i)
				str_c[i] = '\0';
		}
		else if (wParam == VK_BACK)
		{
			count--;
			
			if (count < 0)
			{
				line--;
				if (line < 0)
					line = 9;

				count = lstrlen(str[line]) - 1;
			}

			str[line][count] = '\0';
			_tcscpy(str_c, str[line]);
		}
		else if (wParam == VK_RETURN)
		{
			++line;
			count = 0;
			if (line >= MAX_LINE)
				line = 0;
			for (int i = 0; i < _tcslen(str_c); ++i)
				str_c[i] = '\0';
		}
		else if (wParam == VK_TAB)
		{	//str[line][count] = '\t' 하면 이상한 문자 출력되므로
			str[line][count] = ' ';
			str_c[count] = ' ';
			count++;
			str[line][count] = ' ';
			str_c[count] = ' ';
			count++;
			str[line][count] = ' ';
			str_c[count] = ' ';
			count++;
			str[line][count] = ' ';
			str_c[count] = ' ';
			count++;	//총 4칸 이동

			str_c[count] = '\0';

			if (count >= MAX_LENGTH)
			{
				line++;
				count = 0;
				for (int i = 0; i < _tcslen(str_c); ++i)
					str_c[i] = '\0';

				if (line >= MAX_LINE)
					line = 0;
			}
		}
		else
		{
			str_c[count] = wParam;
			str_c[count + 1] = '\0';
			str[line][count] = wParam;
			++count;
		}
		
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hwnd); 
		DestroyCaret(); 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}