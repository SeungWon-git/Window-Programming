#pragma warning(disable:4996)
#include <windows.h>
#include <atlimage.h>
#include <math.h>
#include <time.h>

#define WindowWidth 750
#define WindowHeight 750
#define nWidth 25
#define nHeight 25
#define BOARD_NUM 30
#define BULLET_NUM 12
#define M_PI       3.14159265358979323846 
#define ENEMY_NUM 50
#define SPECIAL_BULLET_NUM 20

typedef struct {
	int xPosTank, yPosTank;
	int dir;					//--- x+: 2,   x-: 0,   y+: 3,   y-: 1
	int moving;
	POINT gunLeft, gunRight;
	int shape;
	int bCnt;
	int fbAngle;
	int bReloadTime;
	int combo;
	int comboTimer;
	int sbCnt;
	int tankHp;
	bool immortal;
} Tank;

typedef struct {
	int xPosBullet, yPosBullet;
	int dir;
	bool isOn;
} Bullet;

typedef struct {
	int xPosEnemy, yPosEnemy;
	bool isOn;
	bool explode;
} Enemy;

typedef struct {
	bool block;
	bool alert;
	int itemNum;
	int fitemAngle;
} Board;


HINSTANCE g_hInst;
TCHAR lpszClass[]= TEXT("Inversus");

Board iBoard[BOARD_NUM][BOARD_NUM];		//-- 0: 빈 칸, 1: 장애물, 2: 위험(빈칸), 3: 위험(장애물)
int bulletCount = 0;
int bulletSpeed = 23;		//25 - 너무 빨라서 적군 넘어감
int enemySpawnTimer = 20;
int enemySpawnTime = 20;
int enemySpawnNum = 1;
int enemySpeed = 2;
int score = 0;
int comboResetTime = 60;
int specialBulletCount = 0;

Tank tankObject;
Bullet bulletList[BULLET_NUM];
Bullet specialBulletList[SPECIAL_BULLET_NUM];
Enemy enemyList[ENEMY_NUM];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void DrawBoard(HDC hdc);
void DrawBlock(HDC hdc);
void DrawTank(HDC hdc, int xP, int yP, int d);
void DrawBullet(HDC hdc, bool isTypeSpecial, int xP, int yP, int d);
double getRadian(int num);
void DrawEnemy(HDC hdc, int xP, int yP);
void CheckCollisionEnemyHit(Bullet bullet[], int bulletNum, RECT rect);
void CheckChainExplode(int num);
void Explode(int num);
void DrawComboTimer(HDC hdc);
void ResetComboTimer();
void ItemSpawn(int x, int y);
void DrawSpecialItem(HDC hdc);
void FireBullet(bool isTypeSpecial, int xBias, int yBias, RECT* rect, Bullet bullet[], int* cnt, int maxCnt);
void FireSpecialBullets(RECT* rect, Bullet bullet[], int* cnt, int maxCnt);
void BulletMove(RECT* rect, Bullet bullet[], int num);
void CheckCollisionPlayerHit();
void ResetGame();
void DrawTankHP(HDC hdc);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	if (!hPrevInstance) {
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//--- 윈도우 배경색 변경 가능
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = (WNDPROC)WndProc;
		WndClass.lpszClassName = lpszClass;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);
	}
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, WindowWidth + 20, WindowHeight + nHeight * 2 + 40, NULL, (HMENU)NULL, hInstance, NULL);		//--- 윈도우 크기 변경 가능
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oBrush;
	int i = 0, j = 0, tx = 0, ty = 0;
	RECT rect{ 0, };
	POINT pt;

	static CImage img;
	static int iWidth, iHeight, drawBoard;
	static HBITMAP hBitmap;
	static TCHAR scoreText[1024];

	switch (iMessage) {
	case WM_CREATE:
		for (i = 0; i < BOARD_NUM; i++)
			for (j = 0; j < BOARD_NUM; j++) {
				if (i >= (BOARD_NUM - 1) / 2 - 1 && i <= (BOARD_NUM - 1) / 2 + 1 &&
					j >= (BOARD_NUM - 1) / 2 - 1 && j <= (BOARD_NUM - 1) / 2 + 1)
					iBoard[i][j].block = 0;
				else
					iBoard[i][j].block = 1;

				iBoard[i][j].alert = 0;
				iBoard[i][j].itemNum = 0;
			}

		for (i = 0; i < ENEMY_NUM; i++)
			enemyList[i].isOn = 0;

		tankObject.dir = 2;							//--- x+: 2,   x-: 0,   y+: 3,   y-: 1
		tankObject.xPosTank = (BOARD_NUM - 1) / 2;
		tankObject.yPosTank = (BOARD_NUM - 1) / 2;
		tankObject.gunLeft.x = (tankObject.xPosTank + 1) * nWidth;
		tankObject.gunLeft.y = tankObject.yPosTank * nHeight + 15;
		tankObject.gunRight.x = (tankObject.xPosTank + 2) * nWidth - 10;
		tankObject.gunRight.y = (tankObject.yPosTank + 1) * nHeight - 15;
		tankObject.shape = 1;			
		tankObject.bCnt = 6;
		tankObject.fbAngle = 0;
		tankObject.bReloadTime = 8;
		tankObject.combo = 0;
		tankObject.comboTimer = 60;
		tankObject.sbCnt = 0;
		tankObject.tankHp = 2;
		tankObject.immortal = false;

		img.Load(TEXT("background.png"));
		iWidth = img.GetWidth();
		iHeight = img.GetHeight();
		drawBoard = 1;

		SetTimer(hWnd, 100, 100, NULL);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hBitmap = CreateCompatibleBitmap(hdc, WindowWidth + 1, WindowHeight + nHeight * 2 + 1);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBitmap);

		img.Draw(memdc, 0, 0, WindowWidth + 1, WindowHeight + nHeight * 2 + 1, 0, 0, iWidth, iHeight);

		if (drawBoard)
			DrawBoard(memdc);

		DrawBlock(memdc);

		if (tankObject.tankHp > 0)
			DrawTank(memdc, tankObject.xPosTank, tankObject.yPosTank, tankObject.dir);

		for (i = 0; i < ENEMY_NUM; i++)
			if (enemyList[i].isOn == true)
				DrawEnemy(memdc, enemyList[i].xPosEnemy, enemyList[i].yPosEnemy);

		for (i = 0; i < BULLET_NUM; i++)
			if (bulletList[i].isOn == true)
				DrawBullet(memdc, false, bulletList[i].xPosBullet, bulletList[i].yPosBullet, bulletList[i].dir);

		for (i = 0; i < SPECIAL_BULLET_NUM; i++)
			if (specialBulletList[i].isOn == true)
				DrawBullet(memdc, true, specialBulletList[i].xPosBullet, specialBulletList[i].yPosBullet, specialBulletList[i].dir);

		wsprintf(scoreText, L"※스코어: %d점     연속콤보: %d", score, tankObject.combo);
		TextOut(memdc, 30, WindowHeight + 5, (LPCWSTR)scoreText, lstrlen(scoreText));
		wsprintf(scoreText, L"콤보 지속:");
		TextOut(memdc, 30 + (25), WindowHeight + 5 + (20), (LPCWSTR)scoreText, lstrlen(scoreText));

		DrawComboTimer(memdc);

		DrawSpecialItem(memdc);

		DrawTankHP(memdc);

		BitBlt(hdc, 0, 0, WindowWidth + 1, WindowHeight + nHeight * 2 + 1, memdc, 0, 0, SRCCOPY);

		DeleteObject(hBitmap);
		DeleteDC(memdc);

		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 100:
			//(충돌체크) 
			//플레이어 - 적
			if (tankObject.immortal == false)
				CheckCollisionPlayerHit();

			//(키 입력)
			//이동키 입력
			if (GetAsyncKeyState('W') && tankObject.tankHp > 0) {			// y-
				if (tankObject.dir != 1)
					tankObject.dir = 1;
				else {
					if (tankObject.yPosTank > 1 && iBoard[tankObject.xPosTank][tankObject.yPosTank - 1].block != 1)
						tankObject.yPosTank--;
				}

				tankObject.gunLeft.x = tankObject.xPosTank * nWidth + 15;
				tankObject.gunLeft.y = (tankObject.yPosTank - 1) * nHeight + 10;
				tankObject.gunRight.x = (tankObject.xPosTank + 1) * nWidth - 15;
				tankObject.gunRight.y = tankObject.yPosTank * nHeight;
			}

			if (GetAsyncKeyState('S') && tankObject.tankHp > 0) {			// y+
				if (tankObject.dir != 3)
					tankObject.dir = 3;
				else {
					if (tankObject.yPosTank < BOARD_NUM - 2 && iBoard[tankObject.xPosTank][tankObject.yPosTank + 1].block != 1)
						tankObject.yPosTank++;
				}

				tankObject.gunLeft.x = tankObject.xPosTank * nWidth + 15;
				tankObject.gunLeft.y = (tankObject.yPosTank + 1) * nHeight;
				tankObject.gunRight.x = (tankObject.xPosTank + 1) * nWidth - 15;
				tankObject.gunRight.y = (tankObject.yPosTank + 2) * nHeight - 10;
			}

			if (GetAsyncKeyState('A') && tankObject.tankHp > 0) {			// x-
				if (tankObject.dir != 0)
					tankObject.dir = 0;
				else {
					if (tankObject.xPosTank > 1 && iBoard[tankObject.xPosTank - 1][tankObject.yPosTank].block != 1)
						tankObject.xPosTank--;
				}

				tankObject.gunLeft.x = (tankObject.xPosTank - 1) * nWidth + 10;
				tankObject.gunLeft.y = tankObject.yPosTank * nHeight + 15;
				tankObject.gunRight.x = tankObject.xPosTank * nWidth;
				tankObject.gunRight.y = (tankObject.yPosTank + 1) * nHeight - 15;
			}

			if (GetAsyncKeyState('D') && tankObject.tankHp > 0) {			// x+
				if (tankObject.dir != 2)
					tankObject.dir = 2;
				else {
					if (tankObject.xPosTank < BOARD_NUM - 2 && iBoard[tankObject.xPosTank + 1][tankObject.yPosTank].block != 1)
						tankObject.xPosTank++;
				}

				tankObject.gunLeft.x = (tankObject.xPosTank + 1) * nWidth;
				tankObject.gunLeft.y = tankObject.yPosTank * nHeight + 15;
				tankObject.gunRight.x = (tankObject.xPosTank + 2) * nWidth - 10;
				tankObject.gunRight.y = (tankObject.yPosTank + 1) * nHeight - 15;
			}

			//총알 발사
			if (GetAsyncKeyState(VK_RETURN) && tankObject.bCnt != 0 && tankObject.tankHp > 0) {
				FireBullet(false, 0, 0, &rect, bulletList, &bulletCount, BULLET_NUM);
			}

			//아이템 사용 (스페셜 총알 발사)
			if (GetAsyncKeyState(VK_SHIFT) && tankObject.sbCnt != 0 && tankObject.tankHp > 0) {
				FireSpecialBullets(&rect, specialBulletList, &specialBulletCount, SPECIAL_BULLET_NUM);
			}

			//이외 키 입력
			if (GetAsyncKeyState('P')) {
				drawBoard++;
				drawBoard %= 2;
			}

			if (GetAsyncKeyState('I')) {
				tankObject.immortal = !tankObject.immortal;
			}

			if (GetAsyncKeyState('Q')) {
				PostQuitMessage(0);
			}


			//아이템 획득
			for (int a = 0; a < BOARD_NUM; a++) {
				for (int b = 0; b < BOARD_NUM; b++)
					if (iBoard[a][b].itemNum != 0) {
						if (tankObject.xPosTank == a && tankObject.yPosTank == b) {
							int left = 3 - (iBoard[a][b].itemNum + tankObject.sbCnt);

							if (left >= 0) {
								tankObject.sbCnt += iBoard[a][b].itemNum;
								iBoard[a][b].itemNum = 0;
							}
							else {
								tankObject.sbCnt = 3;
								iBoard[a][b].itemNum = left * (-1);
							}

						}

						//아이템 회전
						iBoard[a][b].fitemAngle += 10;
					}
			}


			//경계선 초기화
			for (int a = 0; a < BOARD_NUM; a++) {
				for (int b = 0; b < BOARD_NUM; b++)
					if (iBoard[a][b].alert == 1)
						iBoard[a][b].alert = 0;
			}


			//탱크내 총알 회전
			tankObject.fbAngle += 15;

			//탱크내 총알 장전
			if (tankObject.bCnt != 6) {
				tankObject.bReloadTime--;

				if (tankObject.bReloadTime == 0) {
					tankObject.bReloadTime = 8;
					tankObject.bCnt++;
				}
			}


			//총알 이동
			for (i = 0; i < BULLET_NUM; i++) {
				if (bulletList[i].isOn == 1) {

					BulletMove(&rect, bulletList, i);


					//(충돌체크)
					//장애물과 충돌
					tx = bulletList[i].xPosBullet / nWidth;
					ty = bulletList[i].yPosBullet / nHeight;

					if (tx > 0 && ty > 0 && tx < BOARD_NUM - 1 && ty < BOARD_NUM - 1 && iBoard[tx][ty].block == 1) {	//--- Block
						iBoard[tx][ty].block = 0;
						score += 1;
					}


					//적과 충돌
					CheckCollisionEnemyHit(bulletList, i, rect);

				}
			}

			//스페셜 총알 이동
			for (i = 0; i < SPECIAL_BULLET_NUM; i++) {
				if (specialBulletList[i].isOn == 1) {

					BulletMove(&rect, specialBulletList, i);


					//(충돌체크)
					//장애물과 충돌
					tx = specialBulletList[i].xPosBullet / nWidth;
					ty = specialBulletList[i].yPosBullet / nHeight;

					if (tx > 0 && ty > 0 && tx < BOARD_NUM - 1 && ty < BOARD_NUM - 1 && iBoard[tx][ty].block == 1) {	//--- Block
						iBoard[tx][ty].block = 0;
						score += 1;
					}


					//적과 충돌
					CheckCollisionEnemyHit(specialBulletList, i, rect);

				}
			}


			//콤보 타이머
			tankObject.comboTimer--;
			if (tankObject.comboTimer <= 0) {
				tankObject.combo = 0;

				ResetComboTimer();
			}


			//적 생성
			if (enemySpawnTimer == 0 && tankObject.tankHp > 0) {
				enemySpawnTimer = enemySpawnTime;
				enemySpawnNum++;
				//스폰시간 감소
				if (enemySpawnNum % 2 == 0 && enemySpawnTime > 5) {
					enemySpawnTime--;
				}
				//적 속도 증가
				if (enemySpawnNum % 15 == 0) {
					enemySpeed++;
					if (enemySpeed > 10)
						enemySpeed = 10;
				}

				srand(time(NULL));

				for (i = 0; i < ENEMY_NUM; i++) {
					if (enemyList[i].isOn == false) {
						tx = rand() % BOARD_NUM;
						ty = rand() % BOARD_NUM;

						if (iBoard[tx][ty].block != 1) {
							--i;
							continue;
						}

						if (tx <= tankObject.xPosTank + 2 && tx >= tankObject.xPosTank - 2 && ty <= tankObject.xPosTank + 2 && ty >= tankObject.xPosTank - 2) {
							--i;
							continue;
						}

						enemyList[i].xPosEnemy = tx * nWidth + 1;
						enemyList[i].yPosEnemy = ty * nHeight + 1;
						enemyList[i].isOn = 1;
						enemyList[i].explode = 0;
						break;
					}
				}
			}
			else {
				enemySpawnTimer--;
			}

			//적 이동
			for (i = 0; i < ENEMY_NUM; i++) {
				if (enemyList[i].isOn == true && tankObject.tankHp > 0) {
					tx = enemyList[i].xPosEnemy - 1;
					ty = enemyList[i].yPosEnemy - 1;

					if (abs(tx - tankObject.xPosTank * nWidth) < enemySpeed)
						enemyList[i].xPosEnemy = tankObject.xPosTank * nWidth + 1;
					else if (tx < tankObject.xPosTank * nWidth)
						enemyList[i].xPosEnemy += enemySpeed;
					else if (tx > tankObject.xPosTank * nWidth)
						enemyList[i].xPosEnemy -= enemySpeed;

					if (abs(ty - tankObject.yPosTank * nHeight) < enemySpeed)
						enemyList[i].yPosEnemy = tankObject.yPosTank * nHeight + 1;
					else if (ty < tankObject.yPosTank * nHeight)
						enemyList[i].yPosEnemy += enemySpeed;
					else if (ty > tankObject.yPosTank * nHeight)
						enemyList[i].yPosEnemy -= enemySpeed;

					//주위 환경 변경
					tx = (tx + nWidth / 2) / nWidth;
					ty = (ty + nHeight / 2) / nHeight;
					if (iBoard[tx][ty].block != 1)
						iBoard[tx][ty].block = 1;
					for (int a = tx - 1; a <= tx + 1; a++)
						for (int b = ty - 1; b <= ty + 1; b++)
							if (a >= 0 && b >= 0 && a < BOARD_NUM && b < BOARD_NUM)
								iBoard[a][b].alert = 1;
				}
			}

			break;
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBoard(HDC hdc)
{
	int i;

	for (i = 0; i <= BOARD_NUM; i++) {
		MoveToEx(hdc, i * nWidth, 0, NULL);
		LineTo(hdc, i * nWidth, WindowHeight);

		MoveToEx(hdc, 0, i * nHeight, NULL);
		LineTo(hdc, WindowWidth, i * nHeight);
	}
}

void DrawBlock(HDC hdc)
{
	HBRUSH oldBrush, newBrush_inside, newBrush_boarder, newBrush_alert, newBrush_item;
	int i, j;
	int ix, iy;
	RECT rect;

	oldBrush = NULL;

	newBrush_inside = CreateSolidBrush(RGB(200, 200, 200));

	newBrush_boarder = CreateSolidBrush(RGB(100, 100, 100));

	newBrush_item = CreateSolidBrush(RGB(0, 0, 250));

	newBrush_alert = CreateSolidBrush(RGB(250, 0, 0));


	for (i = 0; i < BOARD_NUM; i++)
		for (j = 0; j < BOARD_NUM; j++) {
			if (iBoard[i][j].block == 1) {
				if (i == 0 || j == 0 || i == BOARD_NUM - 1 || j == BOARD_NUM - 1) {
					oldBrush = (HBRUSH)SelectObject(hdc, newBrush_boarder);

					Rectangle(hdc, i * nWidth, j * nHeight, i * nWidth + nWidth, j * nHeight + nHeight);

					SelectObject(hdc, oldBrush);
				}
				else {
					oldBrush = (HBRUSH)SelectObject(hdc, newBrush_inside);

					Rectangle(hdc, i * nWidth, j * nHeight, i * nWidth + nWidth, j * nHeight + nHeight);

					SelectObject(hdc, oldBrush);
				}
			}

			if (iBoard[i][j].alert == 1) {
				rect.left = i * nWidth + 1;
				rect.top = j * nHeight + 1;
				rect.right = i * nWidth + nWidth - 1;
				rect.bottom = j * nHeight + nHeight - 1;

				FrameRect(hdc, &rect, newBrush_alert);
			}

			if (iBoard[i][j].itemNum != 0) {
				oldBrush = (HBRUSH)SelectObject(hdc, newBrush_item);

				for (int n = 0; n < iBoard[i][j].itemNum; n++) {
					ix = i * nWidth + (nWidth / 2) - 3 + ((nWidth / 4) * cos(getRadian(iBoard[i][j].fitemAngle - n * 120)));
					iy = j * nHeight + (nHeight / 2) - 3 + ((nHeight / 4) * sin(getRadian(iBoard[i][j].fitemAngle - n * 120)));
					Ellipse(hdc, ix, iy, ix + 8, iy + 8);
				}

				SelectObject(hdc, oldBrush);
			}
		}

	SelectObject(hdc, oldBrush);
	DeleteObject(newBrush_inside);
	DeleteObject(newBrush_boarder);
	DeleteObject(newBrush_alert);
	DeleteObject(newBrush_item);
}

void DrawTank(HDC hdc, int xP, int yP, int d)
{
	HBRUSH hBrush, oBrush;

	//탱크 몸체
	hBrush = CreateSolidBrush(RGB(255, 0, 255));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//무적 상태 표시
	if (tankObject.immortal) {
		SelectObject(hdc, oBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(250, 250, 0));
		oBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}

	Rectangle(hdc, xP * nWidth + 2, yP * nHeight + 2, (xP + 1) * nWidth - 2, (yP + 1) * nHeight - 2);

	switch (d) {
	case 0:		// direction: left
		Rectangle(hdc, (xP - 1) * nWidth + 10, yP * nHeight + 15, xP * nWidth, (yP + 1) * nHeight - 15);
		break;
	case 1:		// direction: top
		Rectangle(hdc, xP * nWidth + 15, (yP - 1) * nHeight + 10, (xP + 1) * nWidth - 15, yP * nHeight);
		break;
	case 2:		// direction: right
		Rectangle(hdc, (xP + 1) * nWidth, yP * nHeight + 15, (xP + 2) * nWidth - 10, (yP + 1) * nHeight - 15);
		break;
	case 3:		// direction: bottom
		Rectangle(hdc, xP * nWidth + 15, (yP + 1) * nHeight, (xP + 1) * nWidth - 15, (yP + 2) * nHeight - 10);
		break;
	}

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);


	//탱크 내 총알 표시
	hBrush = CreateSolidBrush(RGB(0, 200, 200));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	int x, y;
	for (int n = 0; n < tankObject.bCnt; n++) {
		x = xP * nWidth + 1 + (nWidth / 2) + ((nWidth / 4) * cos(getRadian(tankObject.fbAngle - n * 60)));
		y = yP * nHeight + 1 + (nHeight / 2) + ((nHeight / 4) * sin(getRadian(tankObject.fbAngle - n * 60)));
		Ellipse(hdc, x - nWidth / 10, y - nHeight / 10, x + nWidth / 10, y + nHeight / 10);
	}

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
}

void DrawBullet(HDC hdc, bool isTypeSpecial, int xP, int yP, int d)
{
	HBRUSH hBrush_normal, oldBrush_normal = NULL;
	HBRUSH hBrush_special, oldBrush_special = NULL;

	hBrush_normal = CreateSolidBrush(RGB(0, 255, 255));
	hBrush_special = CreateSolidBrush(RGB(0, 0, 250));

	if (isTypeSpecial == false)
		oldBrush_normal = (HBRUSH)SelectObject(hdc, hBrush_normal);
	else 
		oldBrush_special = (HBRUSH)SelectObject(hdc, hBrush_special);

	switch (d) {				//--- x+: 2,   x-: 0,   y+: 3,   y-: 1
	case 0:
		Rectangle(hdc, xP, yP - 7, xP - 10, yP + 3);
		break;
	case 1:
		Rectangle(hdc, xP - 7, yP, xP + 3, yP - 10);
		break;
	case 2:
		Rectangle(hdc, xP, yP - 7, xP + 10, yP + 3);
		break;
	case 3:
		Rectangle(hdc, xP - 7, yP, xP + 3, yP + 10);
		break;
	}

	SelectObject(hdc, oldBrush_normal);
	DeleteObject(hBrush_normal);
	SelectObject(hdc, oldBrush_special);
	DeleteObject(hBrush_special);
}

double getRadian(int num)
{
	return num * (M_PI / 180);
}

void DrawEnemy(HDC hdc, int xP, int yP)
{
	HBRUSH hBrush, oBrush;

	hBrush = CreateSolidBrush(RGB(200, 0, 0));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);
	
	Rectangle(hdc, xP, yP, xP + nWidth - 2, yP + nHeight - 2);

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
}

void CheckCollisionEnemyHit(Bullet bullet[], int bulletNum, RECT rect)
{
	RECT temp, rect_;

	for (int j = 0; j < ENEMY_NUM; j++) {
		if (enemyList[j].isOn) {

			rect_.left = enemyList[j].xPosEnemy;
			rect_.top = enemyList[j].yPosEnemy;
			rect_.right = enemyList[j].xPosEnemy + nWidth - 2;
			rect_.bottom = enemyList[j].yPosEnemy + nHeight - 2;
			
			if (IntersectRect(&temp, &rect, &rect_)) {
				bullet[bulletNum].isOn = false;

				//폭발
				CheckChainExplode(j);

				break;
			}
		}
	}
}

void CheckChainExplode(int num)
{
	int sx, sy, tx, ty;

	Explode(num);

	sx = enemyList[num].xPosEnemy - 1;
	sy = enemyList[num].yPosEnemy - 1;
	sx = (sx + nWidth / 2) / nWidth;
	sy = (sy + nHeight / 2) / nHeight;

	for (int n = 0; n < ENEMY_NUM; n++) {
		if (enemyList[n].isOn == true && enemyList[n].explode == false) {
			tx = enemyList[n].xPosEnemy - 1;
			ty = enemyList[n].yPosEnemy - 1;
			tx = (tx + nWidth / 2) / nWidth;
			ty = (ty + nHeight / 2) / nHeight;

			for (int a = sx - 1; a <= sx + 1; a++)
				for (int b = sy - 1; b <= sy + 1; b++)
					if (a > 0 && b > 0 && a < BOARD_NUM - 1 && b < BOARD_NUM - 1)
						if (a == tx && b == ty) 
							CheckChainExplode(n);

			if (tx == sx + 2 && ty == sy) 
				CheckChainExplode(n);
			else if (tx == sx - 2 && ty == sy)
				CheckChainExplode(n);
			else if (tx == sx && ty == sy + 2)
				CheckChainExplode(n);
			else if (tx == sx && ty == sy - 2)
				CheckChainExplode(n);
		}
	}
}

void Explode(int num)
{
	int x, y;

	enemyList[num].isOn = false;
	enemyList[num].explode = true;

	x = enemyList[num].xPosEnemy - 1;
	y = enemyList[num].yPosEnemy - 1;
	x = (x + nWidth / 2) / nWidth;
	y = (y + nHeight / 2) / nHeight;

	ItemSpawn(x, y);

	for (int a = x - 1; a <= x + 1; a++)
		for (int b = y - 1; b <= y + 1; b++)
			if (a > 0 && b > 0 && a < BOARD_NUM - 1 && b < BOARD_NUM - 1)
				iBoard[a][b].block = 0;

	if (x + 2 < BOARD_NUM - 1 && y != 0 && y != BOARD_NUM - 1)
		iBoard[x + 2][y].block = 0;
	if (x - 2 > 0 && y != 0 && y != BOARD_NUM - 1)
		iBoard[x - 2][y].block = 0;
	if (y + 2 < BOARD_NUM - 1 && x != 0 && x != BOARD_NUM - 1)
		iBoard[x][y + 2].block = 0;
	if (y - 2 > 0 && x != 0 && x != BOARD_NUM - 1)
		iBoard[x][y - 2].block = 0;

	//콤보
	tankObject.combo++;

	int comboPlus = tankObject.combo / 5;
	if (comboPlus <= 0)
		comboPlus = 1;
	if (comboPlus >= 10)
		comboPlus = 10;

	score += 5 * comboPlus;
	
	ResetComboTimer();
}

void DrawComboTimer(HDC hdc)
{
	HBRUSH hBrush, oBrush;
	RECT rect;
	int xSize = 150;
	int ySize = 20;

	hBrush = CreateSolidBrush(RGB(250, 50, 50));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//조정 위치: ()
	rect.left = 30 + (105);
	rect.top = WindowHeight + 5 + (18);
	rect.right = 30 + (105) + xSize;
	rect.bottom = WindowHeight + 5 + (18) + ySize;

	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);


	hBrush = CreateSolidBrush(RGB(250, 250, 0));

	int gageSize = (int)((float)xSize * ((float)tankObject.comboTimer / (float)comboResetTime));

	rect.left += 2;
	rect.top += 2;
	rect.right = rect.left + (gageSize - 2);
	rect.bottom += (-2);

	FillRect(hdc, &rect, hBrush);

	DeleteObject(hBrush);
}

void ResetComboTimer()
{
	comboResetTime = 60 - (tankObject.combo / 5) * 5;
	if (comboResetTime < 15)
		comboResetTime = 15;
	tankObject.comboTimer = comboResetTime;
}

void ItemSpawn(int x, int y)
{
	int chance = rand() % 100;

	if (chance <= 30 && x > 0 && y > 0 && x < BOARD_NUM - 1 && y < BOARD_NUM - 1) {
		if (iBoard[x][y].itemNum == 0)
			iBoard[x][y].fitemAngle = 0;

		if (chance <= 5)
			iBoard[x][y].itemNum += 3;
		else if (chance <= 15)
			iBoard[x][y].itemNum += 2;
		else
			iBoard[x][y].itemNum += 1;

		if (iBoard[x][y].itemNum > 3)
			iBoard[x][y].itemNum = 3;
	}
}

void DrawSpecialItem(HDC hdc)
{
	HBRUSH hBrush, oBrush;
	int x, y;

	hBrush = CreateSolidBrush(RGB(0, 0, 250));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	for (int n = 0; n < tankObject.sbCnt; n++) {
		x = 27 * nWidth + ((nWidth / 2) * cos(getRadian(tankObject.fbAngle - n * 120)));
		y = WindowHeight + 15 + ((nHeight / 2) * sin(getRadian(tankObject.fbAngle - n * 120)));
		Ellipse(hdc, x, y, x + nWidth / 2, y + nHeight / 2);
	}

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
}

void FireBullet(bool isTypeSpecial, int xBias, int yBias, RECT* rect, Bullet bullet[], int* cnt, int maxCnt)
{
	int tx, ty;

	int xBiasValue, yBiasValue;

	if (xBias == 0)
		xBiasValue = 0;
	else if (xBias == 1)
		xBiasValue = nWidth;
	else if (xBias == -1)
		xBiasValue =  -nWidth;

	if (yBias == 0)
		yBiasValue = 0;
	else if (yBias == 1)
		yBiasValue = nHeight;
	else if (yBias == -1)
		yBiasValue = -nHeight;

	if (bullet[*cnt].isOn == 1)
		return;

	bullet[*cnt].isOn = 1;
	bullet[*cnt].dir = tankObject.dir;	//--- x+: 2,   x-: 0,   y+: 3,   y-: 1

	if (tankObject.dir == 0) {					//--- x-
		bullet[*cnt].xPosBullet = tankObject.gunLeft.x - 5;
		bullet[*cnt].yPosBullet = tankObject.gunLeft.y + yBiasValue;

		(*rect).left = bullet[*cnt].xPosBullet - 10;
		(*rect).top = bullet[*cnt].yPosBullet - 7;
		(*rect).right = bullet[*cnt].xPosBullet;
		(*rect).bottom = bullet[*cnt].yPosBullet + 3;
	}
	else if (tankObject.dir == 2) {			//--- x+
		bullet[*cnt].xPosBullet = tankObject.gunLeft.x + 5;
		bullet[*cnt].yPosBullet = tankObject.gunLeft.y + yBiasValue;

		(*rect).left = bullet[*cnt].xPosBullet;
		(*rect).top = bullet[*cnt].yPosBullet - 10;
		(*rect).right = bullet[*cnt].xPosBullet + 10;
		(*rect).bottom = bullet[*cnt].yPosBullet;
	}
	else if (tankObject.dir == 1) {			//--- y-
		bullet[*cnt].xPosBullet = tankObject.gunLeft.x + xBiasValue;
		bullet[*cnt].yPosBullet = tankObject.gunLeft.y - 5;

		(*rect).left = bullet[*cnt].xPosBullet;
		(*rect).top = bullet[*cnt].yPosBullet - 7;
		(*rect).right = bullet[*cnt].xPosBullet + 10;
		(*rect).bottom = bullet[*cnt].yPosBullet + 3;
	}
	else if (tankObject.dir == 3) {			//--- y+
		bullet[*cnt].xPosBullet = tankObject.gunLeft.x + xBiasValue;
		bullet[*cnt].yPosBullet = tankObject.gunLeft.y + 5;

		(*rect).left = bullet[*cnt].xPosBullet - 7;
		(*rect).top = bullet[*cnt].yPosBullet;
		(*rect).right = bullet[*cnt].xPosBullet + 3;
		(*rect).bottom = bullet[*cnt].yPosBullet + 10;
	}

	(*cnt)++;
	*cnt %= maxCnt;

	//(이동하기전 최초 발사시 충돌체크)
	//장애물과 충돌
	//tx = bullet[*cnt - 1].xPosBullet / nWidth;
	//ty = bullet[*cnt - 1].yPosBullet / nHeight;
	tx = tankObject.xPosTank;
	ty = tankObject.yPosTank;

	if (tankObject.dir == 0) {
		tx -= 1;
		ty += yBias;
	}
	else if (tankObject.dir == 2) {
		tx += 1;
		ty += yBias;
	}
	else if (tankObject.dir == 1) {
		tx += xBias;
		ty -= 1;
	}
	else if (tankObject.dir == 3) {
		tx += xBias;
		ty += 1;
	}


	if (tx > 0 && ty > 0 && tx < BOARD_NUM - 1 && ty < BOARD_NUM - 1 && iBoard[tx][ty].block == 1) {	//--- Block
		iBoard[tx][ty].block = 0;
	}

	//적과 충돌
	CheckCollisionEnemyHit(bullet, *cnt - 1, *rect);

	if (isTypeSpecial)
		tankObject.sbCnt--;
	else
		tankObject.bCnt--;
}

void FireSpecialBullets(RECT* rect, Bullet bullet[], int* cnt, int maxCnt)
{
	if (tankObject.sbCnt == 1) {
		FireBullet(true, 0, 0, rect, bullet, cnt, maxCnt);
	}
		
	else if (tankObject.sbCnt == 2) {
		//첫번째 총알
		FireBullet(true, 1, 1, rect, bullet, cnt, maxCnt);

		//두번째 총알
		FireBullet(true, -1, -1, rect, bullet, cnt, maxCnt);
	}

	else if (tankObject.sbCnt == 3) {
		//첫번째 총알
		FireBullet(true, 0, 0, rect, bullet, cnt, maxCnt);

		//두번째 총알
		FireBullet(true, 1, 1, rect, bullet, cnt, maxCnt);

		//세번째 총알
		FireBullet(true, -1, -1, rect, bullet, cnt, maxCnt);
	}
}

void BulletMove(RECT* rect, Bullet bullet[], int num)
{
	if (bullet[num].dir == 0) {						//--- 0: x-,   2: x+,   1: y-,  3: y+
		if (bullet[num].xPosBullet > 0)
			bullet[num].xPosBullet -= bulletSpeed;
		else
			bullet[num].isOn = false;

		(*rect).left = bullet[num].xPosBullet - 10;
		(*rect).top = bullet[num].yPosBullet - 7;
		(*rect).right = bullet[num].xPosBullet;
		(*rect).bottom = bullet[num].yPosBullet + 3;
	}
	else if (bullet[num].dir == 1) {
		if (bullet[num].yPosBullet > 0)
			bullet[num].yPosBullet -= bulletSpeed;
		else
			bullet[num].isOn = false;

		(*rect).left = bullet[num].xPosBullet - 7;
		(*rect).top = bullet[num].yPosBullet - 10;
		(*rect).right = bullet[num].xPosBullet + 3;
		(*rect).bottom = bullet[num].yPosBullet;
	}
	else if (bullet[num].dir == 2) {
		if (bullet[num].xPosBullet < WindowWidth)
			bullet[num].xPosBullet += bulletSpeed;
		else
			bullet[num].isOn = false;

		(*rect).left = bullet[num].xPosBullet;
		(*rect).top = bullet[num].yPosBullet - 7;
		(*rect).right = bullet[num].xPosBullet + 10;
		(*rect).bottom = bullet[num].yPosBullet + 3;
	}
	else if (bullet[num].dir == 3) {
		if (bullet[num].yPosBullet < WindowHeight)
			bullet[num].yPosBullet += bulletSpeed;
		else
			bullet[num].isOn = false;

		(*rect).left = bullet[num].xPosBullet - 7;
		(*rect).top = bullet[num].yPosBullet;
		(*rect).right = bullet[num].xPosBullet + 3;
		(*rect).bottom = bullet[num].yPosBullet + 10;
	}
}

void CheckCollisionPlayerHit()
{
	int x, y;

	for (int j = 0; j < ENEMY_NUM; j++) {
		if (enemyList[j].isOn) {
			x = enemyList[j].xPosEnemy - 1;
			y = enemyList[j].yPosEnemy - 1;
			x = (x + nWidth / 2) / nWidth;
			y = (y + nHeight / 2) / nHeight;

			if (tankObject.xPosTank == x && tankObject.yPosTank == y) {
				tankObject.tankHp--;
				
				if (tankObject.tankHp > 0)
					ResetGame();

				return;
			}

		}
	}
}

void ResetGame()
{
	enemySpawnTimer = 20;
	enemySpawnTime = 20;
	enemySpawnNum = 1;
	enemySpeed = 2;
	comboResetTime = 60;

	for (int i = 0; i < ENEMY_NUM; i++)
		enemyList[i].isOn = 0;

	tankObject.dir = 2;					//--- x+: 2,   x-: 0,   y+: 3,   y-: 1
	tankObject.xPosTank = rand() % (BOARD_NUM - 2) + 1;
	tankObject.yPosTank = rand() % (BOARD_NUM - 2) + 1;

	for (int x = tankObject.xPosTank - 1; x <= tankObject.xPosTank + 1; x++)
		for (int y = tankObject.yPosTank - 1; y <= tankObject.yPosTank + 1; y++)
			if (x > 0 && x < BOARD_NUM - 1 && y > 0 && y < BOARD_NUM - 1)
				iBoard[x][y].block = 0;

	tankObject.gunLeft.x = (tankObject.xPosTank + 1) * nWidth;
	tankObject.gunLeft.y = tankObject.yPosTank * nHeight + 15;
	tankObject.gunRight.x = (tankObject.xPosTank + 2) * nWidth - 10;
	tankObject.gunRight.y = (tankObject.yPosTank + 1) * nHeight - 15;
	tankObject.shape = 1;	
	tankObject.bCnt = 6;
	tankObject.fbAngle = 0;
	tankObject.bReloadTime = 8;
	tankObject.combo = 0;
	tankObject.comboTimer = 60;
	tankObject.sbCnt = 0;
}

void DrawTankHP(HDC hdc)
{
	HBRUSH hBrush, oBrush;

	hBrush = CreateSolidBrush(RGB(255, 0, 255));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	for (int n = 0; n < tankObject.tankHp; n++) {
		Rectangle(hdc, (14 + n) * nWidth + 2, WindowHeight + 10 + 2, (15 + n) * nWidth - 2, WindowHeight + 10 + nHeight - 2);
	}

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
}