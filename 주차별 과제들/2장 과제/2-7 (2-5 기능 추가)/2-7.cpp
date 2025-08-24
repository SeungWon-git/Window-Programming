#include <windows.h>
#include<tchar.h>

#define MAX_COLUMN 80
#define MAX_ROW 10

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
	static TCHAR str[MAX_ROW][MAX_COLUMN + 1];
	static TCHAR str_c[MAX_COLUMN + 1];	//캐럿위치를 위한 변수
	static int count, line;
	static SIZE size;
	static int X, Y;
	static bool caps_lock;
	static bool insert;

	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 3, 15);
		ShowCaret(hwnd);
		count = 0;
		line = 0;
		X = size.cx;
		caps_lock = 0;
		insert = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < MAX_ROW; i++)
		{
			TextOut(hdc, 0, i * 20, str[i], lstrlen(str[i]));
		}

		GetTextExtentPoint(hdc, str_c, lstrlen(str_c), &size);
		X = size.cx;
		Y = line * 20;
		SetCaretPos(X, Y);

		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		if (count >= MAX_COLUMN && wParam != VK_BACK && wParam != VK_RETURN)
		{
			str[line][count] = '\0';

			++line;
			count = 0;
			_tcscpy(str_c, L"");
		}

		if (wParam == VK_BACK)
		{
			--count;

			if (count < 0)	//맨 앞줄 삭제
			{
				--line;

				if (line < 0) {
					line = 9;

					count = lstrlen(str[9]);
					_tcscpy(str_c, str[9]);

					int blank = MAX_COLUMN - lstrlen(str[9]);	//편차

					TCHAR temp_str[MAX_COLUMN];
					_tcsncpy(temp_str, str[0], blank);
					if (lstrlen(str[9]) != 0)
						temp_str[blank] = '\0';
					_tcscat(str[9], temp_str);	// 이전줄에 남는 칸만큼 현재줄 내용 채우기

					int M_column = lstrlen(str[0]);
					for (int j = 0; j < M_column; ++j) {
						str[0][j] = str[0][j + blank];
					}

				}
				else {
					count = lstrlen(str[line]);
					_tcscpy(str_c, str[line]);

					int blank = MAX_COLUMN - lstrlen(str[line]);	//편차

					TCHAR temp_str[MAX_COLUMN];
					_tcsncpy(temp_str, str[line + 1], blank);
					if (lstrlen(str[line]) != 0)
						temp_str[blank] = '\0';
					_tcscat(str[line], temp_str);	// 이전줄에 남는 칸만큼 현재줄 내용 채우기

					int M_column = lstrlen(str[line + 1]);
					for (int j = 0; j < M_column; ++j) {
						str[line + 1][j] = str[line + 1][j + blank];
					}

				}
			}
			else
			{
				for (int i = count; i < lstrlen(str[line]); ++i) {
					str[line][i] = str[line][i + 1];
					str_c[i] = str_c[i + 1];
				}
			}
		}
		else if (wParam == VK_RETURN)
		{
			++line;
			count = 0;
			if (line >= MAX_ROW)
				line = 0;
			_tcscpy(str_c, L"");
		}
		else if (wParam == VK_TAB)
		{	//str[line][count] = '\t' 하면 이상한 문자 출력되므로
			for (int i = 0; i < 4; ++i) {
				if (count >= MAX_COLUMN)
				{
					++line;
					count = 0;
					_tcscpy(str_c, L"");

					if (line >= MAX_ROW)
						line = 0;
				}

				if (insert == 1) {
					int M_column = lstrlen(str[line]) + 1;
					for (int i = M_column; i >= count; --i) {
						str[line][i] = str[line][i - 1];
					}
				}

				str[line][count] = ' ';
				str_c[count] = ' ';
				++count;
			}
			str_c[count] = '\0';
		}
		else if (wParam == VK_ESCAPE)
		{
			for (int i = 0; i < MAX_ROW; ++i) {
				int M_column = lstrlen(str[i]);
				for (int j = 0; j < M_column; ++j) {
					str[i][j] = '\0';
				}
			}
			line = 0;
			count = 0;
			_tcscpy(str_c, L"");
			str_c[0] = '\0';
		}
		else
		{
			if (insert == 1) {
				int M_column = lstrlen(str[line]) + 1;
				for (int i = M_column; i >= count; --i) {
					str[line][i] = str[line][i - 1];
				}
			}

			str_c[count] = wParam;
			str_c[count + 1] = '\0';
			str[line][count] = wParam;

			if (caps_lock == 1) {
				switch (wParam) {
				case 'a':
					str_c[count] = 'A';
					str[line][count] = 'A';
					break;
				case 'b':
					str_c[count] = 'B';
					str[line][count] = 'B';
					break;
				case 'c':
					str_c[count] = 'C';
					str[line][count] = 'C';
					break;
				case 'd':
					str_c[count] = 'D';
					str[line][count] = 'D';
					break;
				case 'e':
					str_c[count] = 'E';
					str[line][count] = 'E';
					break;
				case 'f':
					str_c[count] = 'F';
					str[line][count] = 'F';
					break;
				case 'g':
					str_c[count] = 'G';
					str[line][count] = 'G';
					break;
				case 'h':
					str_c[count] = 'H';
					str[line][count] = 'H';
					break;
				case 'i':
					str_c[count] = 'I';
					str[line][count] = 'I';
					break;
				case 'j':
					str_c[count] = 'J';
					str[line][count] = 'J';
					break;
				case 'k':
					str_c[count] = 'K';
					str[line][count] = 'K';
					break;
				case 'l':
					str_c[count] = 'L';
					str[line][count] = 'L';
					break;
				case 'm':
					str_c[count] = 'M';
					str[line][count] = 'M';
					break;
				case 'n':
					str_c[count] = 'N';
					str[line][count] = 'N';
					break;
				case 'o':
					str_c[count] = 'O';
					str[line][count] = 'O';
					break;
				case 'p':
					str_c[count] = 'P';
					str[line][count] = 'P';
					break;
				case 'q':
					str_c[count] = 'Q';
					str[line][count] = 'Q';
					break;
				case 'r':
					str_c[count] = 'R';
					str[line][count] = 'R';
					break;
				case 's':
					str_c[count] = 'S';
					str[line][count] = 'S';
					break;
				case 't':
					str_c[count] = 'T';
					str[line][count] = 'T';
					break;
				case 'u':
					str_c[count] = 'U';
					str[line][count] = 'U';
					break;
				case 'v':
					str_c[count] = 'V';
					str[line][count] = 'V';
					break;
				case 'w':
					str_c[count] = 'W';
					str[line][count] = 'W';
					break;
				case 'x':
					str_c[count] = 'X';
					str[line][count] = 'X';
					break;
				case 'y':
					str_c[count] = 'Y';
					str[line][count] = 'Y';
					break;
				case 'z':
					str_c[count] = 'Z';
					str[line][count] = 'Z';
					break;
				}
			}
			++count;

			if (count == lstrlen(str[line]))
				str[line][count] = '\0';
		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:	//해당 키보드 입력해도 출력이 안되는 키입력들은 여기서
		switch (wParam) {

		case VK_F1:
			caps_lock = !(caps_lock);
			break;

		case VK_HOME:
			count = 0;
			_tcscpy(str_c, L"");
			break;

		case VK_END:
			count = lstrlen(str[line]);
			_tcscpy(str_c, str[line]);
			break;

		case VK_DELETE:
			while (str[line][count] != ' ' && str[line][count] != '\0') {	//만약 캐럿이 문자 중간에 있을 경우 count위치 해당 문자 맨 뒤로
				++count;
			}

			if (count > 0) {
				--count;	//띄어쓰기에 위치할 수 있으므로
			}

			while (str[line][count] != ' ') {	//해당 문자 다 지우기 
				for (int i = count; i < lstrlen(str[line]); ++i)
					str[line][i] = str[line][i + 1];
				
				if (count == 0) 
					break;
				else
					--count;
			}

			while (str[line][count] == ' ') {	//앞 문자 사이 띄어쓰기들 없애기
				for (int i = count; i < lstrlen(str[line]); ++i)
					str[line][i] = str[line][i + 1];
				
				if (count == 0)
					break;
				else
					--count;
			}

			if (count != 0) {
				_tcsncpy(str_c, str[line], count + 1);
				str_c[count + 1] = '\0';
				++count;
			}
			else if(lstrlen(str[line]) != 0){	// 예시) 1 (tab) 8 => 이때와 같이 있을때 8을 삭제하면 캐럿 위치 1 뒤에 있게 하기 위해 (안 하면 count=0이라서 아래 조건때문에 맨 앞에 위치하게 됨)
				_tcsncpy(str_c, str[line], count + 1);
				str_c[count + 1] = '\0';
				++count;
			}
			else {
				_tcscpy(str_c, L"");
			}
			break;

		case VK_INSERT:
			insert = !(insert);
			break;

		case VK_LEFT:
			--count;

			if (count < 0) {
				if (line != 0) {
					--line;
					count = lstrlen(str[line]);
				}
				else {
					line = 9;
					count = lstrlen(str[line]);
				}

				_tcscpy(str_c, str[line]);
			}
			else
				str_c[count] = '\0';
			break;

		case VK_RIGHT:
			if (count >= lstrlen(str[line])) {
				if (line != 9) {
					++line;
					count = 0;
				}
				else {
					line = 0;
					count = 0;
				}
				_tcscpy(str_c, L"");
			}
			else {
				str_c[count] = str[line][count];
				++count;
				str_c[count] = '\0';
			}
			break;

		case VK_UP:
			--line;

			if (line < 0) {
				line = 9;
			}

			if (lstrlen(str[line]) < count) { //올라가기 전 줄이 올라간 후 줄보다 긴 경우
				count = lstrlen(str[line]);
				_tcscpy(str_c, str[line]);
			}
			else
				_tcsncpy(str_c, str[line], count);
			break;

		case VK_DOWN:
			++line;

			if (line > 9) {
				line = 0;
			}

			if (lstrlen(str[line]) < count) { //내려가기 전 줄이 내려간 후 줄보다 긴 경우
				count = lstrlen(str[line]);
				_tcscpy(str_c, str[line]);
			}
			else
				_tcsncpy(str_c, str[line], count);
			break;

		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}