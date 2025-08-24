#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "resource.h"
#include "CSound.h"

#pragma comment (lib, "msimg32.lib")

#define WindowWidth 750
#define WindowHeight 750
#define M_PI       3.14159265358979323846 
#define gravity		3
#define max_gameStage 5

#define marioSize 25
#define marioHammerSize 28
#define marioJumpPower -13
#define marioClimbSpeed 2
#define mario_Climbing_Cnt 5

#define barrelCnt	60
#define barrelxSpeed 5
#define barrelSize 15

#define kongRollAnimateInterp 3
#define kongIdleAnimateInterp 4

#define mapTileCnt	75
#define mapTileSize 10

#define hammerRSize 13
#define hammerHSize 18
#define hammerSwingTime 240
#define hammerSwingInterp 4

#define boomCnt 10
#define boomSize 17
#define boomTime 12

#define stage1LadderCnt 11
#define stage2LadderCnt 10
#define stage3LadderCnt 18
#define stage4LadderCnt 12
#define stage5LadderCnt 18

#define birdCnt		5
#define birdySpeed 20
#define birdWidthSize 18
#define birdHeightSize 15

typedef struct {
	int xPos, yPos;
	int life;
	int xSpeed, ySpeed;
	bool dirRight;
	bool inAir;
	bool isMoving;
	int animation;
	int hammerCnt;
	bool immortal;
	bool isClimbable;
	bool isClimbing;
	bool death;
	int swingTime;
	int isClimbing_cnt;
} Mario;

typedef struct {
	int x, y;
	int xSpeed, ySpeed;
	bool active;
	bool inAir;
	int animation;
	bool isFalling;
	int climbLadder;
	int passLadderCnt;
	bool isBig;
	bool spawnReady;
} Barrel;

typedef struct {
	int x, y;
	int ySpeed;
	bool active;
	int animation;
} Bird;

typedef struct {
	int type;	//1 - 발판, 2 - 사다리, 3 - 미획득 아이템, 5 - 공주, 6 - 동키콩, 7 - 원통, 8 - 큰 원통, 9 - 도착지점
} Map;

typedef struct {
	int x, y;
	int animation;
} Boom;

typedef struct {
	int x, y;
	bool rollRight;
	bool big;
	int animation;	//0 - 일반, 1 - 굴리기1, 2 - 굴리기2, 3 - 굴리기3, 100 - (인트로)올라가기, 101 - (인트로)점프, 102 - (인트로)대기, 200 - (다음 레벨 이동)올라가기
	int animationCnt;
	int jump;			//1 - 오른발 점프, 2 - 왼발 착지, 3 - 왼발 점프, 4 - 오른발 착지
}Donkey;

typedef struct {
	int x, y;
	int animation;
	bool dirRight;
} Princess;

enum BGM {
	INTRO,
	START,
	INGAME,
	HAMMERTIME,
	DEATH,
	NEXTLEVEL,
	ENDING
};

HINSTANCE g_hInst;
TCHAR lpszClass[]= TEXT("Donkey Kong");

int gameStage;
int ladderCnt;
int barrelSpawnDir;	// 1 - 오른쪽으로 시작, (-1) - 왼쪽으로 시작
int barrel_spawnInterp;
int barrel_spawnTimer;
int bird_spawnInterp;
int bird_spawnTimer;
bool pause;
bool starting;
bool muteMusic, muteSfx;
bool clickToPosition;
bool intro;
int introAnimCnt;
bool nextlevel;
bool ending;
int text_xPos;

Mario mario;
Barrel barrel[barrelCnt];
Map map[mapTileCnt][mapTileCnt];
Bird bird[birdCnt];
Boom boom[boomCnt];
Donkey kong;
Princess peachy;

CSound* bgm_intro, * bgm_start, * bgm_ingame, * bgm_hammertime, * bgm_death, * bgm_nextlevel, * bgm_ending;
CSound* sfx_jump, * sfx_boom, * sfx_hammer, * sfx_hitted, * sfx_birdspawn, * sfx_walk, * sfx_climbingladder, * sfx_respawn;
int BGM_STATUS;

HBITMAP platform_L, platform_M, platform_R, platform_S, ladder_T, ladder_M, ladder_B, hammer_item, hammer_equip, boom0, boom1, boom2;
HBITMAP barrel_S, barrel1_F, barrel2_F, barrel1_R, barrel2_R, barrel3_R, barrel4_R;
HBITMAP s_barrel_S, s_barrel1_F, s_barrel2_F, s_barrel1_R, s_barrel2_R, s_barrel3_R, s_barrel4_R;
HBITMAP bird_V, bird1_H, bird2_H;
HBITMAP kong_stand0, kong_stand1, kong_stand2, kong_stand3, kong_roll_L, kong_roll_R, kong_climb1, kong_climb2, kong_move0, kong_move1_R, kong_move2_R, kong_move1_L, kong_move2_L, kong_fall1, kong_fall2, kong_fall3;
HBITMAP princess0_R, princess1_R, princess2_R, princess3_R, princess0_L, princess1_L, princess2_L, princess3_L, princess_heart, princess_help, princess_heart1, princess_heart2, princess_heart3;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void LoadIntro();
void LoadMap();
void DrawMap(HDC hdc, HDC memdc, HBITMAP oldBit);
void CheckCollisionPlatform();
void CheckCollisionLadder();
void DrawBarrel(HDC hdc, HDC memdc, HBITMAP oldBit);
void SpawnBarrel();
void ProcessBarrelPhysics();
void CheckCollisionHazard();
void ResetBarrels();
void RespawnMario();
bool CheckCollisionGoal();
void NextMap(HWND hwnd);
void ResetBirds();
void SpawnBird();
void DrawBird(HDC hdc, HDC memdc, HBITMAP oldBit);
void MoveBird();
void CheckCollisionItemHammer();
void CheckCollisionHammerSmash();
void ActivateBoom(int x, int y);
void DrawBoom(HDC hdc, HDC memdc, HBITMAP oldBit);
void ResetHammers();
void DrawKong(HDC hdc, HDC memdc, HBITMAP oldBit);
void DrawPrincess(HDC hdc, HDC memdc, HBITMAP oldBit);
void ResetBoom();
void ResetKong();
void ResetPrincess();
void DrawIntroKong(HDC hdc, HDC memdc, HBITMAP oldBit);
void EraseLadders();
void SetUpLadders();
void DrawNextLevelKong(HDC hdc, HDC memdc, HBITMAP oldBit);
void DrawEndingKong(HDC hdc, HDC memdc, HBITMAP oldBit);
void DrawEndingMap(HDC hdc, HDC memdc, HBITMAP oldBit, int x, int y);
void DrawPopUpMessage(HDC hdc);

void ChangeBGM(int bgm_change_to);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	if (!hPrevInstance) {
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = (WNDPROC)WndProc;
		WndClass.lpszClassName = lpszClass;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);
	}

	CSound::Init();

	bgm_intro = new CSound("sound/mp3/bgm_intro.mp3", false);
	bgm_start = new CSound("sound/mp3/bgm_start.mp3", false);
	bgm_ingame = new CSound("sound/mp3/bgm_ingame.mp3", true);
	bgm_hammertime = new CSound("sound/mp3/bgm_hammertime.mp3", false);
	bgm_death = new CSound("sound/mp3/bgm_death.mp3", false);
	bgm_nextlevel = new CSound("sound/mp3/bgm_nextlevel.mp3", false);
	bgm_ending = new CSound("sound/mp3/bgm_ending.mp3", false);

	sfx_jump = new CSound("sound/wav/sfx_jump.wav", false);
	sfx_boom = new CSound("sound/wav/sfx_boom.wav", false);
	sfx_hammer = new CSound("sound/wav/sfx_hammer.wav", false);
	sfx_hitted = new CSound("sound/wav/sfx_hitted.wav", false);
	sfx_birdspawn = new CSound("sound/wav/sfx_birdspawn.wav", false);
	sfx_walk = new CSound("sound/wav/sfx_walk.wav", true);
	sfx_climbingladder = new CSound("sound/wav/sfx_climbingladder.wav", true);
	sfx_respawn = new CSound("sound/wav/sfx_respawn.wav", false);
	
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, WindowWidth + 16, WindowHeight + 40, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);

		if (muteMusic) {
			bgm_intro->muteOn();
			bgm_start->muteOn();
			bgm_ingame->muteOn();
			bgm_hammertime->muteOn();
			bgm_death->muteOn();
			bgm_nextlevel->muteOn();
			bgm_ending->muteOn();
		}
		else {
			bgm_intro->muteOff();
			bgm_start->muteOff();
			bgm_ingame->muteOff();
			bgm_hammertime->muteOff();
			bgm_death->muteOff();
			bgm_nextlevel->muteOff();
			bgm_ending->muteOff();
		}

		//사운드 업데이트
		switch (BGM_STATUS) {
		case INTRO:
			bgm_intro->Update();
			break;

		case START:
			bgm_start->Update();

			if (bgm_start->status() == false) {
				ChangeBGM(INGAME);
				starting = false;
			}
			break;

		case INGAME:
			bgm_ingame->Update();
			break;

		case HAMMERTIME:
			bgm_hammertime->Update();

			if (bgm_hammertime->status() == false) {
				ChangeBGM(INGAME);
			}
			break;

		case DEATH:
			bgm_death->Update();

			if (bgm_death->status() == false) {
				ChangeBGM(INGAME);
			}
			break;

		case NEXTLEVEL:
			bgm_nextlevel->Update();

			if (bgm_nextlevel->status() == false) {
				nextlevel = false;
				NextMap(hWnd);
			}
			break;

		case ENDING:
			bgm_ending->Update();
			break;
		}

		if (muteSfx) {
			sfx_jump->muteOn();
			sfx_boom->muteOn();
			sfx_hammer->muteOn();
			sfx_hitted->muteOn();
			sfx_birdspawn->muteOn();
			sfx_walk->muteOn();
			sfx_climbingladder->muteOn();
			sfx_respawn->muteOn();
		}
		else {
			sfx_jump->muteOff();
			sfx_boom->muteOff();
			sfx_hammer->muteOff();
			sfx_hitted->muteOff();
			sfx_birdspawn->muteOff();
			sfx_walk->muteOff();
			sfx_climbingladder->muteOff();
			sfx_respawn->muteOff();
		}

		if (!pause && !nextlevel && !ending) {
			sfx_jump->Update();
			sfx_boom->Update();
			sfx_hammer->Update();
			sfx_hitted->Update();
			sfx_birdspawn->Update();
			sfx_walk->Update();
			sfx_climbingladder->Update();
			sfx_respawn->Update();
		}
		else {
			sfx_jump->pause();
			sfx_boom->pause();
			sfx_hammer->pause();
			sfx_hitted->pause();
			sfx_birdspawn->pause();
			sfx_walk->pause();
			sfx_climbingladder->pause();
			sfx_respawn->pause();
		}
	}

	delete bgm_intro;
	delete bgm_start;
	delete bgm_ingame;
	delete bgm_hammertime;
	delete bgm_death;
	delete bgm_nextlevel;
	delete bgm_ending;

	delete sfx_jump;
	delete sfx_boom;
	delete sfx_hammer;
	delete sfx_hitted;
	delete sfx_birdspawn;
	delete sfx_walk;
	delete sfx_climbingladder;
	delete sfx_respawn;

	CSound::Release();

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oBrush;

	static HBITMAP oldBit, hBit, screenBit;
	static HBITMAP background;
	static HBITMAP mario_walk0_R, mario_walk1_R, mario_walk2_R, mario_walk0_L, mario_walk1_L, mario_walk2_L, mario_jump0_R, mario_jump0_L,
		mario_life, mario_immotal, mario_death0, mario_death1, mario_death2, mario_death3, mario_death4,
		mario_climb0, mario_climb1, mario_climb2, mario_climb3, mario_climb4, mario_hammer0_L, mario_hammer1_L, mario_hammer0_R, mario_hammer1_R;

	switch (iMessage) {
	case WM_CREATE:
		sfx_walk->play();
		sfx_walk->pause();
		sfx_climbingladder->play();
		sfx_climbingladder->pause();

		srand(time(NULL));

		background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BACKGROUND));

		mario_walk0_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALK0_R));
		mario_walk1_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALK1_R));
		mario_walk2_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALK2_R));
		mario_walk0_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALK0_L));
		mario_walk1_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALK1_L));
		mario_walk2_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALK2_L));
		mario_jump0_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_JUMP0_R));
		mario_jump0_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_JUMP0_L));
		mario_life = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LIFE));
		mario_immotal = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_IMMOTAL));
		mario_death0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DEATH0));
		mario_death1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DEATH1));
		mario_death2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DEATH2));
		mario_death3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DEATH3));
		mario_death4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DEATH4));
		mario_climb0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIMB0));
		mario_climb1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIMB1));
		mario_climb2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIMB2));
		mario_climb3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIMB3));
		mario_climb4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIMB4));
		mario_hammer0_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HAMMER0_R));
		mario_hammer1_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HAMMER1_R));
		mario_hammer0_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HAMMER0_L));
		mario_hammer1_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HAMMER1_L));

		platform_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLATFORM_L));
		platform_M = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLATFORM_M));
		platform_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLATFORM_R));
		platform_S = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLATFORM_S));
		ladder_T = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LADDER_T));
		ladder_M = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LADDER_M));
		ladder_B = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LADDER_B));
		hammer_item = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HAMMER_ITEM));
		hammer_equip = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HAMMER_EQUIP));

		barrel_S = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL_S));
		barrel1_F = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL1_F));
		barrel2_F = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL2_F));
		barrel1_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL1_R));
		barrel2_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL2_R));
		barrel3_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL3_R));
		barrel4_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL4_R));

		s_barrel_S = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL_S));
		s_barrel1_F = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL1_F));
		s_barrel2_F = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL2_F));
		s_barrel1_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL1_R));
		s_barrel2_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL2_R));
		s_barrel3_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL3_R));
		s_barrel4_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_S_BARREL4_R));

		bird_V = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIRD_H));
		bird1_H = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIRD1_V));
		bird2_H = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIRD2_V));

		boom0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOOM0));
		boom1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOOM1));
		boom2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOOM2));

		kong_stand0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_S0));
		kong_stand1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_S1));
		kong_stand2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_S2));
		kong_stand3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_S3));
		kong_roll_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_RL));
		kong_roll_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_RR));
		kong_climb1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_C1));
		kong_climb2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_C2));
		kong_move0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_M0));
		kong_move1_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_ML1));
		kong_move2_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_ML2));
		kong_move1_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_MR1));
		kong_move2_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_MR2));
		kong_fall1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_F1));
		kong_fall2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_F2));
		kong_fall3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KONG_F3));

		princess0_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS0_R));
		princess1_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS1_R));
		princess2_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS2_R));
		princess3_R = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS3_R));
		princess0_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS0_L));
		princess1_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS1_L));
		princess2_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS2_L));
		princess3_L = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PRINCESS3_L));
		princess_heart = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HEART));
		princess_help = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HELP));
		princess_heart1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HEART1));
		princess_heart2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HEART2));
		princess_heart3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HEART3));

		pause = false;
		text_xPos = WindowWidth / 2;

		gameStage = 1;

		muteMusic = false;
		muteSfx = false;

		intro = true;
		introAnimCnt = 1;

		ChangeBGM(INTRO);

		kong.x = 325;
		kong.y = WindowHeight;
		kong.animation = 100;
		kong.jump = 1;

		clickToPosition = false;

		hBit = mario_walk0_R;

		ResetBarrels();
		ResetBirds();

		LoadIntro();

		mario.xPos = -100;

		nextlevel = false;
		ending = false;

		SetTimer(hWnd, 100, 50, NULL);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(mem1dc, screenBit);

		BitBlt(hdc, 0, 0, WindowWidth, WindowHeight, mem1dc, 0, 0, SRCCOPY);

		DeleteObject(screenBit);
		SelectObject(mem1dc, oldBit);
		DeleteDC(mem1dc);

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case 'P':
			if (intro || starting || nextlevel || mario.life == 0)
				break;

			pause = !pause;
			if (pause == true) {
				if (BGM_STATUS == INGAME) 
					bgm_ingame->pause();
				if(BGM_STATUS == HAMMERTIME) 
					bgm_hammertime->pause();
			}
			else {
				if (BGM_STATUS == INGAME)
					bgm_ingame->resume();
				if (BGM_STATUS == HAMMERTIME)
					bgm_hammertime->resume();
			}
			break;

		case 'I':
			mario.immortal = !mario.immortal;
			break;

		case 'R':
			if (intro || starting || nextlevel || ending)
				break;

			mario.life = 3;
			RespawnMario();
			ResetBarrels();
			ResetBirds();
			ResetHammers();
			ResetBoom();
			ResetKong();
			ResetPrincess();

			if (!muteSfx) 
				sfx_respawn->play();	
			break;

		case 'M':
			muteMusic = !muteMusic;
			break;

		case 'N':
			muteSfx = !muteSfx;
			break;

		case 'Q':
			PostQuitMessage(0);
			break;

		case VK_SHIFT:
			//아이템 사용 (해머 사용)
			if (mario.hammerCnt > 0 && !mario.death && mario.swingTime == 0 && !nextlevel) {
				mario.hammerCnt--;
				mario.swingTime = hammerSwingTime;

				ChangeBGM(HAMMERTIME);
			}
			break;

		case VK_RETURN:
			if (intro) {
				intro = false;

				LoadMap();
				ChangeBGM(START);

				mario.life = 3;
				mario.xSpeed = 5;
				mario.immortal = false;
				RespawnMario();

				ResetKong();
			}
			break;

		case VK_UP:
			bgm_intro->volumeUp();
			bgm_start->volumeUp();
			bgm_ingame->volumeUp();
			bgm_hammertime->volumeUp();
			bgm_death->volumeUp();
			bgm_nextlevel->volumeUp();
			bgm_ending->volumeUp();
			break;

		case VK_DOWN:
			bgm_intro->volumeDown();
			bgm_start->volumeDown();
			bgm_ingame->volumeDown();
			bgm_hammertime->volumeDown();
			bgm_death->volumeDown();
			bgm_nextlevel->volumeDown();
			bgm_ending->volumeDown();
			break;

		case VK_RIGHT:
			sfx_jump->volumeUp();
			sfx_boom->volumeUp();
			sfx_hammer->volumeUp();
			sfx_hitted->volumeUp();
			sfx_birdspawn->volumeUp();
			sfx_walk->volumeUp();
			sfx_climbingladder->volumeUp();
			sfx_respawn->volumeUp();
			break;

		case VK_LEFT:
			sfx_jump->volumeDown();
			sfx_boom->volumeDown();
			sfx_hammer->volumeDown();
			sfx_hitted->volumeDown();
			sfx_birdspawn->volumeDown();
			sfx_walk->volumeDown();
			sfx_climbingladder->volumeDown();
			sfx_respawn->volumeDown();
			break;
		}
		break;

	case WM_CHAR:
		switch (wParam) {
		case '+':
			if (!intro && !nextlevel && !ending && !mario.death)
				NextMap(hWnd);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		if ((mario.life != 0 || mario.yPos <= WindowHeight) && !intro && !nextlevel && !ending) {
			if (clickToPosition) {
				mario.xPos = LOWORD(lParam) - marioSize / 2;
				mario.yPos = HIWORD(lParam) - marioSize / 2;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		clickToPosition = !clickToPosition;
		break;

	case WM_TIMER:
		switch (wParam) {
		case 100:
			//<더블 버퍼링>
			//그리기
			hdc = GetDC(hWnd);

			mem1dc = CreateCompatibleDC(hdc);
			mem2dc = CreateCompatibleDC(mem1dc);

			screenBit = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);
			oldBit = (HBITMAP)SelectObject(mem1dc, screenBit);

			//배경
			oldBit = (HBITMAP)SelectObject(mem2dc, background);
			StretchBlt(mem1dc, 0, 0, WindowWidth + 5, WindowHeight + 5, mem2dc, 0, 0, 5, 5, SRCCOPY);

			//플랫폼
			DrawMap(mem1dc, mem2dc, oldBit);

			//동키콩
			DrawKong(mem1dc, mem2dc, oldBit);

			//공주
			DrawPrincess(mem1dc, mem2dc, oldBit);

			//원통
			DrawBarrel(mem1dc, mem2dc, oldBit);

			//새
			DrawBird(mem1dc, mem2dc, oldBit);

			//폭발 이펙트
			DrawBoom(mem1dc, mem2dc, oldBit);

			//무적상태 후광 이펙트
			if (mario.immortal && !ending) {
				oldBit = (HBITMAP)SelectObject(mem2dc, mario_immotal);
				TransparentBlt(mem1dc, mario.xPos, mario.yPos, marioSize, marioSize, mem2dc, 0, 0, 19, 19, RGB(0, 105, 105));
			}

			//마리오
			oldBit = (HBITMAP)SelectObject(mem2dc, hBit);
			if (ending && peachy.y == 95) {
				oldBit = (HBITMAP)SelectObject(mem2dc, mario_walk0_L);
				TransparentBlt(mem1dc, peachy.x + 35, peachy.y, marioSize, marioSize, mem2dc, 0, 0, 19, 19, RGB(0, 105, 105));
			}
			else if (mario.swingTime > 0 && mario.swingTime != hammerSwingTime && !mario.death) {
				int adj = 13;

				if (mario.dirRight)
					TransparentBlt(mem1dc, mario.xPos, mario.yPos - adj, marioSize + adj, marioSize + adj, mem2dc, 0, 0, marioHammerSize, marioHammerSize, RGB(0, 105, 105));
				else
					TransparentBlt(mem1dc, mario.xPos - adj, mario.yPos - adj, marioSize + adj, marioSize + adj, mem2dc, 0, 0, marioHammerSize, marioHammerSize, RGB(0, 105, 105));
			}
			else
				TransparentBlt(mem1dc, mario.xPos, mario.yPos, marioSize, marioSize, mem2dc, 0, 0, 19, 19, RGB(0, 105, 105));

			//라이프 표시
			oldBit = (HBITMAP)SelectObject(mem2dc, mario_life);
			for (int n = 0; n < mario.life; n++)
				TransparentBlt(mem1dc, 10 + 20 * n, 10, marioSize, marioSize, mem2dc, 0, 0, 19, 19, RGB(0, 105, 105));

			//해머 아이템 표시
			oldBit = (HBITMAP)SelectObject(mem2dc, hammer_equip);
			for (int n = 0; n < mario.hammerCnt; n++)
				TransparentBlt(mem1dc, 15 + 20 * n, 40, hammerRSize, hammerRSize, mem2dc, 0, 0, hammerRSize, hammerRSize, RGB(0, 105, 105));

			if (mario.hammerCnt != 0 && mario.swingTime == 0 && !ending)
				TransparentBlt(mem1dc, mario.xPos + hammerHSize / 4, mario.yPos - hammerHSize, hammerHSize, hammerHSize, mem2dc, 0, 0, hammerRSize, hammerRSize, RGB(0, 105, 105));

			DrawPopUpMessage(mem1dc);

			SelectObject(mem2dc, oldBit);
			DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit);
			DeleteDC(mem1dc);

			ReleaseDC(hWnd, hdc);

			//해당 상황이면 아래 작업 필요 X
			if (intro || nextlevel || ending || pause)
				break;

			//스테이지 시작시 사다리 오르기
			if (!intro && starting) {
				mario.yPos -= marioClimbSpeed;
				mario.animation++;
				if (mario.animation > 4)
					mario.animation = 0;
			}

			//<키 입력>
			//이동키 입력
			mario.isMoving = false;

			if (GetAsyncKeyState('W') && mario.isClimbable && !mario.death && !intro && !starting) {			//위
				mario.yPos -= marioClimbSpeed;
				mario.isClimbing = true;
				mario.isClimbing_cnt = mario_Climbing_Cnt;
				mario.inAir = true;
				mario.ySpeed = 0;
				mario.animation++;
				if (mario.animation > 4)
					mario.animation = 0;
			}

			if (GetAsyncKeyState('S') && mario.isClimbable && !mario.death && !intro && !starting) {			//아래
				mario.yPos += marioClimbSpeed;
				mario.isClimbing = true;
				mario.isClimbing_cnt = mario_Climbing_Cnt;
				mario.inAir = true;
				mario.ySpeed = 0;
				mario.animation++;
				if (mario.animation > 4)
					mario.animation = 0;
			}

			if (GetAsyncKeyState('A') && !mario.death && !intro && !starting) {			//왼쪽
				mario.xPos -= mario.xSpeed;
				mario.dirRight = false;
				mario.isMoving = true;
			}

			if (GetAsyncKeyState('D') && !mario.death && !intro && !starting) {			//오른쪽
				mario.xPos += mario.xSpeed;
				mario.dirRight = true;
				mario.isMoving = true;
			}

			//이동 사운드 이펙트
			if (mario.isMoving)
				sfx_walk->resume();
			else
				sfx_walk->pause();

			//사다리타기 사운드 이펙트
			if (mario.isClimbing && mario.isClimbing_cnt > 0) {
				mario.isClimbing_cnt--;
				sfx_climbingladder->resume();
			}
			else {
				mario.isClimbing_cnt = 0;
				sfx_climbingladder->pause();
			}

			//점프
			if ((GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_RETURN)) && mario.inAir == false && !mario.death && !intro && !starting) {
				mario.inAir = true;
				mario.ySpeed = marioJumpPower;

				sfx_jump->play();
			}

			//해머 스윙
			if (mario.swingTime > 0)
				mario.swingTime--;

			//공중 물리 처리 (중력)
			if (mario.inAir && !mario.isClimbing || mario.death) {
				mario.yPos += mario.ySpeed;
				mario.ySpeed += gravity;
				if (mario.ySpeed > gravity * 6)
					mario.ySpeed = gravity * 6;
			}

			//마리오 리스폰
			if (mario.death && mario.yPos > WindowHeight && BGM_STATUS == INGAME) {
				if (mario.life > 0) {
					RespawnMario();

					ResetBarrels();
					ResetBirds();
					ResetHammers();
				}
			}

			//<충돌체크>
			//해머와 장애물 충돌체크
			CheckCollisionHammerSmash();

			//맵 오브젝트와 충돌체크
			CheckCollisionPlatform();
			CheckCollisionLadder();
			CheckCollisionItemHammer();

			//장애물들과 충돌체크
			CheckCollisionHazard();

			//도착점 충돌체크
			if (CheckCollisionGoal() && nextlevel == false && !mario.death) {
				if (gameStage < max_gameStage) {
					nextlevel = true;
					ResetKong();
					ChangeBGM(NEXTLEVEL);
				}
				else if (!ending)
					NextMap(hWnd);
			}

			//<애니메이션>
			//마리오
			if (mario.death) {
				mario.animation++;

				int n = mario.animation % 8;

				if (n < 2)
					hBit = mario_death0;
				else if (n < 4)
					hBit = mario_death1;
				else if (n < 6)
					hBit = mario_death2;
				else if (n < 8)
					hBit = mario_death3;
			}
			else if (mario.swingTime != 0) {
				int n = mario.swingTime % hammerSwingInterp;

				if (mario.dirRight) {
					if (n > hammerSwingInterp / 2)
						hBit = mario_hammer0_R;
					else
						hBit = mario_hammer1_R;
				}
				else {
					if (n > hammerSwingInterp / 2)
						hBit = mario_hammer0_L;
					else
						hBit = mario_hammer1_L;
				}
			}
			else if (mario.isClimbing || starting) {
				if (mario.animation < 2)
					hBit = mario_climb0;
				else
					hBit = mario_climb1;
			}
			else {
				if (mario.isMoving) {
					mario.animation++;
					if (mario.animation >= MAXINT64)
						mario.animation = 0;

					int n = mario.animation % 2;

					if (mario.dirRight) {
						if (n == 0)
							hBit = mario_walk1_R;
						else
							hBit = mario_walk2_R;
					}
					else {
						if (n == 0)
							hBit = mario_walk1_L;
						else
							hBit = mario_walk2_L;
					}
				}
				else {
					mario.animation = 0;

					if (mario.dirRight)
						hBit = mario_walk0_R;
					else
						hBit = mario_walk0_L;
				}

				if (mario.inAir) {
					if (mario.dirRight)
						hBit = mario_jump0_R;
					else
						hBit = mario_jump0_L;
				}
			}


			//<스폰>
			if (!intro && !starting) {
				//원통 스폰
				SpawnBarrel();

				//새 스폰
				SpawnBird();
			}

			//<그 외 처리>
			//원통 애니메이션 + 물리처리
			ProcessBarrelPhysics();

			//새 이동
			MoveBird();

			//폭발 이펙트 애니메이션
			for (int n = 0; n < boomCnt; n++) {
				if (boom[n].animation > 0) {
					boom[n].animation--;
				}
			}

			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void LoadIntro()
{
	FILE* fp = NULL;
	char introDir[24];
	char introNum[7];

	itoa(introAnimCnt, introNum, 10);
	strcat(introNum, ").txt");
	strcpy(introDir, "stage_map/intro(");
	strcat(introDir, introNum);
	if ((fp = fopen(introDir, "rt")) == NULL)
		exit(1);

	for (int y = 0; y < mapTileCnt; y++) {
		for (int x = 0; x < mapTileCnt; x++) {
			map[x][y].type = fgetc(fp) - 48;
		}
		fgetc(fp);
	}

	fclose(fp);
}

void LoadMap()
{
	FILE* fp = NULL;
	char stageDir[22];
	char stageNum[6];

	if (gameStage == 1) {
		ladderCnt = stage1LadderCnt;

		barrel_spawnInterp = 30;
	}
	else if (gameStage == 2) {
		ladderCnt = stage2LadderCnt / 3;

		barrel_spawnInterp = 27;
	}
	else if (gameStage == 3) {
		ladderCnt = stage3LadderCnt / 2;

		barrel_spawnInterp = 25;

		bird_spawnInterp = 50;
	}
	else if (gameStage == 4) {
		ladderCnt = stage4LadderCnt / 2;

		barrel_spawnInterp = 25;
	}
	else if (gameStage == 5) {
		ladderCnt = stage5LadderCnt / 2;

		barrel_spawnInterp = 25;

		bird_spawnInterp = 60;
	}
	else {
		return;
	}

	itoa(gameStage, stageNum, 10);
	strcat(stageNum, ".txt");
	strcpy(stageDir, "stage_map/stage");
	strcat(stageDir, stageNum);
	if ((fp = fopen(stageDir, "rt")) == NULL)
		exit(1);

	for (int y = 0; y < mapTileCnt; y++) {
		for (int x = 0; x < mapTileCnt; x++) {
			map[x][y].type = fgetc(fp) - 48;
		}
		fgetc(fp);
	}

	fclose(fp);
}

void DrawMap(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	int extra;

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 1) {
				if (ending && y <= 5) {
					DrawEndingMap(hdc, memdc, oldBit, x, y);
				}
				else {
					extra = 0;

					if (x == 0)
						oldBit = (HBITMAP)SelectObject(memdc, platform_L);
					else if (x == mapTileCnt - 1)
						oldBit = (HBITMAP)SelectObject(memdc, platform_R);
					else if (map[x - 1][y].type != 1 && map[x + 1][y].type != 1) {
						oldBit = (HBITMAP)SelectObject(memdc, platform_S);
						extra = 1;
					}
					else if (map[x - 1][y].type != 1)
						oldBit = (HBITMAP)SelectObject(memdc, platform_L);
					else if (map[x + 1][y].type != 1)
						oldBit = (HBITMAP)SelectObject(memdc, platform_R);
					else
						oldBit = (HBITMAP)SelectObject(memdc, platform_M);

					TransparentBlt(hdc, x * mapTileSize, y * mapTileSize, mapTileSize, mapTileSize, memdc, 0, 0, mapTileSize + extra, mapTileSize, RGB(0, 105, 105));
				}
			}
		}
	}

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 2) {
				if (ending && y <= 10) {

				}
				else {
					oldBit = (HBITMAP)SelectObject(memdc, ladder_M);
					TransparentBlt(hdc, x * mapTileSize, y * mapTileSize, mapTileSize, mapTileSize, memdc, 0, 0, mapTileSize, mapTileSize, RGB(0, 105, 105));

					if (map[x][y - 1].type != 2) {
						oldBit = (HBITMAP)SelectObject(memdc, ladder_T);
						TransparentBlt(hdc, x * mapTileSize, (y - 1) * mapTileSize, mapTileSize, mapTileSize, memdc, 0, 0, mapTileSize, mapTileSize, RGB(0, 105, 105));
					}
					if (map[x][y + 1].type != 2) {
						oldBit = (HBITMAP)SelectObject(memdc, ladder_B);
						TransparentBlt(hdc, x * mapTileSize, (y + 1) * mapTileSize, mapTileSize, mapTileSize, memdc, 0, 0, mapTileSize, mapTileSize, RGB(0, 105, 105));
					}
				}
			}
		}
	}

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 3) {
				oldBit = (HBITMAP)SelectObject(memdc, hammer_item);
				TransparentBlt(hdc, x * mapTileSize, y * mapTileSize, hammerHSize, hammerHSize, memdc, 0, 0, hammerRSize, hammerRSize, RGB(0, 105, 105));
			}
		}
	}

	if (ending)
		return;

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 8) {
				oldBit = (HBITMAP)SelectObject(memdc, s_barrel_S);
				TransparentBlt(hdc, x * mapTileSize, y * mapTileSize, barrelSize * 2, barrelSize * 2 + barrelSize / 3 - 5, memdc, 0, 0, 12, 18, RGB(0, 105, 105));
			}
		}
	}

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 7) {
				oldBit = (HBITMAP)SelectObject(memdc, barrel_S);
				TransparentBlt(hdc, x * mapTileSize, y * mapTileSize, 13, 20, memdc, 0, 0, 12, 18, RGB(0, 105, 105));
			}
		}
	}
}

void CheckCollisionPlatform()
{
	RECT trect{ 0, }, mario_rect{ 0, }, platform_rect{ 0, };

	if (mario.xPos < 0)
		mario.xPos = 0;
	else if (mario.xPos + marioSize > WindowWidth)
		mario.xPos = WindowWidth - marioSize;

	if (mario.isClimbing || mario.death || starting)
		return;

	if (mario.yPos + marioSize > WindowHeight) {
		mario.yPos = WindowHeight - marioSize;
		mario.life--;
		mario.death = true;
		mario.animation = 0;
		mario.ySpeed = 0;

		sfx_hitted->play();
		ChangeBGM(DEATH);

		return;
	}

	if (mario.inAir == false) {
		mario.inAir = true;

		for (int x = 0; x < mapTileCnt; x++) {
			for (int y = 0; y < mapTileCnt; y++) {
				if (map[x][y].type == 1) {
					mario_rect.left = mario.xPos;
					mario_rect.right = mario.xPos + marioSize;
					mario_rect.top = mario.yPos;
					mario_rect.bottom = mario.yPos + marioSize;

					platform_rect.left = x * mapTileSize;
					platform_rect.right = (x + 1) * mapTileSize;
					platform_rect.top = y * mapTileSize;
					platform_rect.bottom = (y + 1) * mapTileSize;

					if (IntersectRect(&trect, &mario_rect, &platform_rect)) {
						//경사면 오르기
						if ((mario_rect.bottom - 1) - platform_rect.top < mapTileSize) {
							mario.yPos = platform_rect.top - marioSize;
							mario.inAir = false;
							mario.ySpeed = 0;
							return;
						}
						//부딪힘
						else {
							//if (mario.xPos < platform_rect.left)	//오른쪽으로 이동중에
							//	mario.xPos = platform_rect.left - marioSize;
							//else if (mario.xPos + marioSize > platform_rect.right)	//왼쪽으로 이동중에
							//	mario.xPos = platform_rect.right;
						}
					}

					mario_rect.top = mario.yPos + 1;
					mario_rect.bottom = mario.yPos + 1 + marioSize;

					if (IntersectRect(&trect, &mario_rect, &platform_rect)) {
						mario.inAir = false;
					}
				}
			}
		}
	}
	else {
		for (int x = 0; x < mapTileCnt; x++) {
			for (int y = 0; y < mapTileCnt; y++) {
				if (map[x][y].type == 1) {
					mario_rect.left = mario.xPos;
					mario_rect.right = mario.xPos + marioSize;
					mario_rect.top = mario.yPos;
					mario_rect.bottom = mario.yPos + marioSize;

					platform_rect.left = x * mapTileSize;
					platform_rect.right = (x + 1) * mapTileSize;
					platform_rect.top = y * mapTileSize;
					platform_rect.bottom = (y + 1) * mapTileSize;

					//플랫폼 부딪힘
					if (IntersectRect(&trect, &mario_rect, &platform_rect)) {
						//머리 부딪힘
						if (platform_rect.bottom - mario_rect.top < mapTileSize && mario_rect.bottom > platform_rect.bottom) {
							mario.yPos = platform_rect.bottom;
							if (mario.ySpeed < 0)
								mario.ySpeed = 0;
							return;
						}

						//발 걸림
						//if (mario.xPos + marioSize / 2 < platform_rect.left && mario.yPos + marioSize / 2 > platform_rect.top && mario.yPos + marioSize / 2 < platform_rect.bottom && mario.dirRight) { //오른쪽으로 이동중에
						//	mario.xPos = platform_rect.left - marioSize;
						//	x = 0; y = 0;
						//	continue;
						//}
						//if (mario.xPos + marioSize / 2 > platform_rect.right && mario.yPos + marioSize / 2 > platform_rect.top && mario.yPos + marioSize / 2 < platform_rect.bottom && !mario.dirRight) { //왼쪽으로 이동중에
						//	mario.xPos = platform_rect.right;
						//	x = 0; y = 0;
						//	continue;
						//}
						
						//발과 발판 사이 거리가 별 차이 없을때 바로 올라오게 만들기 (사다리를 타거나 파쿠르해서 올라갔을때)
						if (platform_rect.top > mario.yPos + marioSize / 5 * 4 && mario.ySpeed <= gravity) {
							mario.yPos = platform_rect.top - marioSize;

							mario.inAir = false;
							mario.ySpeed = 0;
						}
						//낙하중이 었을때 발판 밟음, 바로 올라오게 만들기
						else if (mario.ySpeed > gravity) {
							mario.yPos = platform_rect.top - marioSize;

							mario.inAir = false;
							mario.ySpeed = 0;
						}
						//발이 걸렸을때는 천천히 파쿠르해서 올라가기
						else {
							mario.ySpeed = -marioClimbSpeed * 2;
						}

						return;
					}
				}
			}
		}
	}
}

void CheckCollisionLadder()
{
	RECT trect{ 0, }, mario_rect{ 0, }, ladder_rect{ 0, };
	int padding = 5;

	mario_rect.left = mario.xPos + padding;
	mario_rect.right = mario.xPos + marioSize - padding;
	mario_rect.top = mario.yPos + padding * 2;
	mario_rect.bottom = mario.yPos + marioSize;

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 2) {
				ladder_rect.left = x * mapTileSize;
				ladder_rect.right = (x + 1) * mapTileSize;
				ladder_rect.top = y * mapTileSize - mapTileSize - 1;
				ladder_rect.bottom = (y + 1) * mapTileSize + mapTileSize + 1;

				if (IntersectRect(&trect, &mario_rect, &ladder_rect)) {
					mario.isClimbable = true;
					return;
				}
			}
		}
	}

	mario.isClimbable = false;
	mario.isClimbing = false;
}

void DrawBarrel(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	for (int n = 0; n < barrelCnt; n++) {
		if (barrel[n].active) {
			int a = barrel[n].animation % 12;

			if (barrel[n].isFalling) {
				if (barrel[n].isBig) {
					if (a < 6)
						oldBit = (HBITMAP)SelectObject(memdc, s_barrel1_F);
					else if (a < 12)
						oldBit = (HBITMAP)SelectObject(memdc, s_barrel2_F);

					TransparentBlt(hdc, barrel[n].x, barrel[n].y, barrelSize * 2 + barrelSize, barrelSize * 2, memdc, 0, 0, 18, 12, RGB(0, 105, 105));
				}
				else {
					if (a < 6)
						oldBit = (HBITMAP)SelectObject(memdc, barrel1_F);
					else if (a < 12)
						oldBit = (HBITMAP)SelectObject(memdc, barrel2_F);

					TransparentBlt(hdc, barrel[n].x, barrel[n].y, barrelSize + barrelSize / 2, barrelSize, memdc, 0, 0, 18, 12, RGB(0, 105, 105));
				}
			}
			else {
				if (barrel[n].isBig) {
					if (barrel[n].xSpeed > 0) {
						if (a < 3)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel1_R);
						else if (a < 6)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel2_R);
						else if (a < 9)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel3_R);
						else if (a < 12)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel4_R);
					}
					else {
						if (a < 3)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel4_R);
						else if (a < 6)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel3_R);
						else if (a < 9)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel2_R);
						else if (a < 12)
							oldBit = (HBITMAP)SelectObject(memdc, s_barrel1_R);
					}

					TransparentBlt(hdc, barrel[n].x, barrel[n].y, barrelSize * 2, barrelSize * 2, memdc, 0, 0, 14, 14, RGB(0, 105, 105));
				}
				else {
					if (barrel[n].xSpeed > 0) {
						if (a < 3)
							oldBit = (HBITMAP)SelectObject(memdc, barrel1_R);
						else if (a < 6)
							oldBit = (HBITMAP)SelectObject(memdc, barrel2_R);
						else if (a < 9)
							oldBit = (HBITMAP)SelectObject(memdc, barrel3_R);
						else if (a < 12)
							oldBit = (HBITMAP)SelectObject(memdc, barrel4_R);
					}
					else {
						if (a < 3)
							oldBit = (HBITMAP)SelectObject(memdc, barrel4_R);
						else if (a < 6)
							oldBit = (HBITMAP)SelectObject(memdc, barrel3_R);
						else if (a < 9)
							oldBit = (HBITMAP)SelectObject(memdc, barrel2_R);
						else if (a < 12)
							oldBit = (HBITMAP)SelectObject(memdc, barrel1_R);
					}

					TransparentBlt(hdc, barrel[n].x, barrel[n].y, barrelSize, barrelSize, memdc, 0, 0, 14, 14, RGB(0, 105, 105));
				}
			}
		}
	}
}

void SpawnBarrel()
{
	int spawnX = 10, spawnY = 10;
	bool big = false;
	int rightExtra = 0;
	int upExtra = 0;
	int leftExtra = 0;

	if (gameStage == 1) {
		spawnX = 30;
		spawnY = 120;

		barrelSpawnDir = 1;
	}
	else if (gameStage == 2) {
		spawnX = 335;
		spawnY = 105;

		if (rand() % 2 == 1) 
			barrelSpawnDir = 1;
		else
			barrelSpawnDir = -1;
	}
	else if (gameStage == 3) {
		spawnX = 335;
		spawnY = 130;

		if (rand() % 2 == 1) 
			barrelSpawnDir = 1;
		else
			barrelSpawnDir = -1;
	}
	else if (gameStage == 4) {
		spawnX = 555;
		spawnY = 90;

		if (rand() % 2 == 1) 
			barrelSpawnDir = 1;
		else
			barrelSpawnDir = -1;

		if (rand() % 4 == 0)
			big = true;
		else
			big = false;
	}
	else if (gameStage == 5) {
		spawnX = 335;
		spawnY = 100;

		if (rand() % 2 == 1)
			barrelSpawnDir = 1;
		else
			barrelSpawnDir = -1;

		if (rand() % 5 == 0)
			big = true;
		else
			big = false;
	}
	else {
		return;
	}

	if (barrelSpawnDir == 1) {
		rightExtra = 60;
	}

	if (big) {
		upExtra = -10;

		if (barrelSpawnDir == -1)
			leftExtra = -20;
	}

	barrel_spawnTimer++;
	if (barrel_spawnTimer % barrel_spawnInterp == 0) {
		barrel_spawnTimer = 0;
		for (int n = 0; n < barrelCnt; n++) {
			if (!barrel[n].active && !barrel[n].spawnReady) {
				//barrel[n].active = true;
				barrel[n].spawnReady = true;
				barrel[n].animation = 0;
				barrel[n].x = spawnX + rightExtra + leftExtra;
				barrel[n].y = spawnY + upExtra;
				barrel[n].xSpeed = barrelxSpeed * barrelSpawnDir;
				barrel[n].ySpeed = 0;
				barrel[n].isFalling = false;
				barrel[n].climbLadder = 0;
				barrel[n].passLadderCnt = rand() % (ladderCnt * 2);
				barrel[n].isBig = big;
				barrel[n].inAir = false;
				break;
			}
		}

		kong.animation = 1;
		kong.big = big;
		if (barrelSpawnDir == 1)
			kong.rollRight = true;
		else
			kong.rollRight = false;
		kong.animationCnt = kongRollAnimateInterp;
	}
}

void ProcessBarrelPhysics()
{
	RECT trect{ 0, }, barrel_rect{ 0, }, map_rect{ 0, };
	int size = 0;

	for (int n = 0; n < barrelCnt; n++) {
		if (barrel[n].active) {
			barrel[n].animation += 1;
			if (barrel[n].animation > MAXINT64)
				barrel[n].animation = 0;

			barrel[n].inAir = true;

			for (int x = 0; x < mapTileCnt; x++) {
				for (int y = 0; y < mapTileCnt; y++) {

					map_rect.left = x * mapTileSize;
					map_rect.right = (x + 1) * mapTileSize;
					map_rect.top = y * mapTileSize;
					map_rect.bottom = (y + 1) * mapTileSize;

					if (barrel[n].isBig)
						size = barrelSize * 2;
					else
						size = barrelSize;

					//굴러가기
					if (map[x][y].type == 1 && barrel[n].climbLadder == 0) {
						barrel_rect.left = barrel[n].x;
						barrel_rect.right = barrel[n].x + size;
						barrel_rect.top = barrel[n].y + size / 3 * 2;
						barrel_rect.bottom = barrel[n].y + size + size / 3 * 2;

						if (IntersectRect(&trect, &barrel_rect, &map_rect)) {
							barrel[n].y = map_rect.top - size;

							barrel[n].inAir = false;
							barrel[n].ySpeed = 0;

							//사다리를 타던 중 바닥만남
							if (barrel[n].isFalling) {
								barrel[n].isFalling = false;
								barrel[n].xSpeed = barrel[n].xSpeed * (-1);
							}
						}
					}
					//사다리 타기
					else if (map[x][y].type == 2 && !barrel[n].isFalling && !barrel[n].isBig) {
						barrel_rect.left = barrel[n].x;
						barrel_rect.right = barrel[n].x + size - size / 2;
						barrel_rect.top = barrel[n].y + size;
						barrel_rect.bottom = barrel[n].y + size + size;

						if (IntersectRect(&trect, &barrel_rect, &map_rect)) {
							barrel[n].passLadderCnt--;

							//현 위치 사다리 타기
							if (barrel[n].passLadderCnt < 0) {
								barrel[n].inAir = true;
								barrel[n].isFalling = true;
								barrel[n].climbLadder = mapTileSize;
								barrel[n].passLadderCnt = rand() % (ladderCnt * 2);
							}
						}
					}
				}
			}	//endof #for (int x = 0; x < mapTileCnt; x++)

			if (!barrel[n].isFalling) {
				barrel[n].x += barrel[n].xSpeed;
				if (barrel[n].x + size > WindowWidth || barrel[n].x < 0)
					barrel[n].xSpeed = barrel[n].xSpeed * (-1);
			}

			if (barrel[n].inAir)
				barrel[n].ySpeed += gravity;
			if (barrel[n].ySpeed > gravity * 5)
				barrel[n].ySpeed = gravity * 5;

			barrel[n].y += barrel[n].ySpeed;

			if (barrel[n].isFalling) {
				barrel[n].climbLadder -= barrel[n].ySpeed;

				if (barrel[n].climbLadder < 0)
					barrel[n].climbLadder = 0;
			}

			if (barrel[n].y > WindowHeight)
				barrel[n].active = false;
		}
	}

}

void CheckCollisionHazard()
{
	RECT trect{ 0, }, mario_rect{ 0, }, hazard_rect{ 0, };
	int size = 0;

	if (mario.immortal || mario.death || starting)
		return;

	mario_rect.left = mario.xPos;
	mario_rect.right = mario.xPos + marioSize;
	mario_rect.top = mario.yPos;
	mario_rect.bottom = mario.yPos + marioSize;

	LONG extra = 0;

	//원통과 충돌
	for (int n = 0; n < barrelCnt; n++) {
		if (barrel[n].active) {
			if (barrel[n].isBig)
				size = barrelSize * 2;
			else
				size = barrelSize;
				
			if (barrel[n].isFalling)
				extra = size / 2;
			else 
				extra = 0;

			hazard_rect.left = barrel[n].x;
			hazard_rect.right = barrel[n].x + size + extra;
			hazard_rect.top = barrel[n].y;
			hazard_rect.bottom = barrel[n].y + size;

			if(IntersectRect(&trect, &mario_rect, &hazard_rect)) {
				mario.life--;
				mario.death = true;
				mario.animation = 0;
				mario.ySpeed = 0;

				sfx_hitted->play();
				ChangeBGM(DEATH);

				return;
			}
		}
	}

	//새와 충돌
	for (int n = 0; n < birdCnt; n++) {
		if (bird[n].active) {
			hazard_rect.left = bird[n].x;
			hazard_rect.right = bird[n].x + birdWidthSize;
			hazard_rect.top = bird[n].y;
			hazard_rect.bottom = bird[n].y + birdHeightSize;

			if (IntersectRect(&trect, &mario_rect, &hazard_rect)) {
				mario.life--;
				mario.death = true;
				mario.animation = 0;
				mario.ySpeed = 0;

				sfx_hitted->play();
				ChangeBGM(DEATH);

				return;
			}
		}
	}
}

void ResetBarrels()
{
	barrel_spawnTimer = 0;

	for (int n = 0; n < barrelCnt; n++) {
		barrel[n].active = false;
		barrel[n].spawnReady = false;
		barrel[n].animation = 0;
		barrel[n].inAir = false;
		barrel[n].isBig = false;
		barrel[n].isFalling = false;
		barrel[n].climbLadder = 0;
	}
}

void RespawnMario()
{
	if (gameStage == 1) {
		mario.xPos = 103;
		mario.yPos = 700;

		if (starting) 
			mario.yPos = 810;
	}
	else if (gameStage == 2) {
		mario.xPos = 353;
		mario.yPos = 700;

		if (starting)
			mario.yPos = 810;
	}
	else if (gameStage == 3) {
		mario.xPos = 353;
		mario.yPos = 700;

		if (starting)
			mario.yPos = 810;
	}
	else if (gameStage == 4) {
		mario.xPos = 655;
		mario.yPos = 700;

		if (starting)
			mario.yPos = 810;
	}
	else if (gameStage == 5) {
		mario.xPos = 363;
		mario.yPos = 700;

		if (starting)
			mario.yPos = 810;
	}
	else {
		return;
	}

	mario.death = false;
	mario.dirRight = true;
	mario.ySpeed = 0;
	mario.animation = 0;
	mario.inAir = false;
	mario.isMoving = false;
	mario.hammerCnt = 0;
	mario.isClimbable = false;
	mario.isClimbing = false;
	mario.swingTime = 0;
	mario.isClimbing_cnt = 0;
}

bool CheckCollisionGoal()
{
	RECT trect{ 0, }, mario_rect{ 0, }, map_rect{ 0, };

	mario_rect.left = mario.xPos;
	mario_rect.right = mario.xPos + marioSize ;
	mario_rect.top = mario.yPos;
	mario_rect.bottom = mario.yPos + marioSize;

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 9) {
				map_rect.left = x * mapTileSize;
				map_rect.right = (x + 1) * mapTileSize;
				map_rect.top = y * mapTileSize - mapTileSize - 1;
				map_rect.bottom = (y + 1) * mapTileSize + mapTileSize + 1;

				if (IntersectRect(&trect, &mario_rect, &map_rect)) {
					return true;
				}
			}
		}
	}

	return false;
}

void NextMap(HWND hwnd)
{
	++gameStage;

	if (gameStage <= max_gameStage) {
		LoadMap();
		ChangeBGM(START);
		ResetBarrels();
		ResetBirds();
		ResetBoom();
		ResetKong();
		ResetPrincess();
		RespawnMario();
	}
	else {
		ending = true;
		ResetBarrels();
		ResetBirds();
		ResetBoom();
		ChangeBGM(ENDING);
		kong.animationCnt = 0;
	}
}

void ResetBirds()
{
	bird_spawnTimer = 0;

	for (int n = 0; n < birdCnt; n++) {
		bird[n].active = false;
		bird[n].animation = 0;
	}
}

void SpawnBird()
{
	int spawnX = 10, spawnY = 10;

	if (gameStage == 1) {
		return;
	}
	else if (gameStage == 2) {
		return;
	}
	else if (gameStage == 3) {
		spawnX = mario.xPos;
		spawnY = -birdHeightSize;
	}
	else if (gameStage == 4) {
		return;
	}
	else if (gameStage == 5) {
		spawnX = mario.xPos;
		spawnY = -birdHeightSize;
	}
	else {
		return;
	}

	bird_spawnTimer++;
	if (bird_spawnTimer % bird_spawnInterp == 0) {
		bird_spawnTimer = 0;
		for (int n = 0; n < birdCnt; n++) {
			if (!bird[n].active) {
				bird[n].active = true;
				bird[n].animation = 0;
				bird[n].x = spawnX;
				bird[n].y = spawnY;
				bird[n].ySpeed = birdySpeed;

				sfx_birdspawn->play();

				break;
			}
		}
	}
}

void DrawBird(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	for (int n = 0; n < birdCnt; n++) {
		if (bird[n].active) {
			oldBit = (HBITMAP)SelectObject(memdc, bird_V);

			TransparentBlt(hdc, bird[n].x, bird[n].y, birdWidthSize, birdHeightSize, memdc, 0, 0, birdWidthSize, birdHeightSize, RGB(0, 105, 105));
		}
	}
}

void MoveBird()
{
	for (int n = 0; n < birdCnt; n++) {
		if (bird[n].active) {
			bird[n].y += bird[n].ySpeed;

			if (bird[n].y > WindowHeight)
				bird[n].active = false;
		}
	}
}

void CheckCollisionItemHammer()
{
	RECT trect{ 0, }, mario_rect{ 0, }, hammer_rect{ 0, };

	if (mario.death || mario.hammerCnt >= 3)
		return;

	mario_rect.left = mario.xPos;
	mario_rect.right = mario.xPos + marioSize;
	mario_rect.top = mario.yPos;
	mario_rect.bottom = mario.yPos + marioSize;

	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 3) {
				hammer_rect.left = x * mapTileSize;
				hammer_rect.right = x * mapTileSize + hammerHSize;
				hammer_rect.top = y * mapTileSize;
				hammer_rect.bottom = y * mapTileSize + hammerHSize;

				if (IntersectRect(&trect, &mario_rect, &hammer_rect)) {
					mario.hammerCnt++;
					map[x][y].type = 4;

					sfx_hammer->play();

					return;
				}
			}
		}
	}
}

void CheckCollisionHammerSmash()
{
	RECT trect{ 0, }, hammer_rect{ 0, }, hazard_rect{ 0, };
	int size = 0;

	if (mario.swingTime == 0 || mario.death)
		return;

	int n = mario.swingTime % hammerSwingInterp;
	int adj = 13;
	int marioX_adj = 0;
	int marioY_adj = mario.yPos - adj;
	int hammerSize = marioSize + adj;

	if (mario.dirRight) {
		marioX_adj = mario.xPos;

		if (n > hammerSwingInterp / 2) {	//해머드는 애니메이션
			hammer_rect.left = marioX_adj;
			hammer_rect.right = marioX_adj + marioSize;
			hammer_rect.top = marioY_adj;
			hammer_rect.bottom = marioY_adj + hammerSize / 2;
		}
		else {		//해머 내리치는 애니메이션
			hammer_rect.left = marioX_adj + hammerSize / 2;
			hammer_rect.right = marioX_adj + hammerSize;
			hammer_rect.top = marioY_adj + hammerSize / 2;
			hammer_rect.bottom = marioY_adj + hammerSize;
		}
	}
	else {
		marioX_adj = mario.xPos - adj;

		if (n > hammerSwingInterp / 2) {	//해머드는 애니메이션
			hammer_rect.left = marioX_adj + hammerSize / 2;
			hammer_rect.right = marioX_adj + hammerSize;
			hammer_rect.top = marioY_adj;
			hammer_rect.bottom = marioY_adj + hammerSize / 2;
		}
		else {		//해머 내리치는 애니메이션
			hammer_rect.left = marioX_adj;
			hammer_rect.right = marioX_adj + hammerSize / 2;
			hammer_rect.top = marioY_adj + hammerSize / 2;
			hammer_rect.bottom = marioY_adj + hammerSize;
		}
	}

	LONG extra = 0;

	//원통과 충돌
	for (int n = 0; n < barrelCnt; n++) {
		if (barrel[n].active) {
			if (barrel[n].isBig)
				continue;
			//	size = barrelSize * 2;
			else
				size = barrelSize;

			if (barrel[n].isFalling)
				extra = size / 2;
			else
				extra = 0;

			hazard_rect.left = barrel[n].x;
			hazard_rect.right = barrel[n].x + size + extra;
			hazard_rect.top = barrel[n].y;
			hazard_rect.bottom = barrel[n].y + size;

			if (IntersectRect(&trect, &hammer_rect, &hazard_rect)) {
				barrel[n].active = false;
				
				ActivateBoom(barrel[n].x, barrel[n].y);
			}
		}
	}

	//새와 충돌
	for (int n = 0; n < birdCnt; n++) {
		if (bird[n].active) {
			hazard_rect.left = bird[n].x;
			hazard_rect.right = bird[n].x + birdWidthSize;
			hazard_rect.top = bird[n].y;
			hazard_rect.bottom = bird[n].y + birdHeightSize;

			if (IntersectRect(&trect, &hammer_rect, &hazard_rect)) {
				bird[n].active = false;

				ActivateBoom(bird[n].x, bird[n].y);
			}
		}
	}
}

void ActivateBoom(int x, int y)
{
	if(!ending)
		sfx_boom->play();

	for (int n = 0; n < boomCnt; n++) {
		if (boom[n].animation == 0) {
			boom[n].animation = boomTime;
			boom[n].x = x - boomSize / 2;
			boom[n].y = y - boomSize / 2;

			return;
		}
	}
}

void DrawBoom(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	for (int n = 0; n < boomCnt; n++) {
		if (boom[n].animation > 0) {
			int a = boom[n].animation % boomTime;

			if (a > boomTime / 3 * 2)
				oldBit = (HBITMAP)SelectObject(memdc, boom0);
			else if(a > boomTime / 3)
				oldBit = (HBITMAP)SelectObject(memdc, boom1);
			else
				oldBit = (HBITMAP)SelectObject(memdc, boom2);

			TransparentBlt(hdc, boom[n].x, boom[n].y, boomSize * 2, boomSize * 2, memdc, 0, 0, boomSize, boomSize, RGB(0, 105, 105));
		}
	}
}

void ResetHammers()
{
	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == 4)
				map[x][y].type = 3;
		}
	}
}

void ChangeBGM(int bgm_change_to)
{
	switch (BGM_STATUS) {
	case INTRO:
		bgm_intro->stop();
		break;

	case START:
		bgm_start->stop();
		break;

	case INGAME:
		bgm_ingame->stop();
		break;

	case HAMMERTIME:
		bgm_hammertime->stop();
		break;

	case DEATH:
		bgm_death->stop();
		break;

	case NEXTLEVEL:
		bgm_nextlevel->stop();
		break;

	case ENDING:
		bgm_ending->stop();
		break;
	}

	switch (bgm_change_to) {
	case INTRO:
		bgm_intro->play();
		break;

	case START:
		bgm_start->play();
		starting = true;
		break;

	case INGAME:
		bgm_ingame->play();
		break;

	case HAMMERTIME:
		bgm_hammertime->play();
		break;

	case DEATH:
		bgm_death->play();
		break;

	case NEXTLEVEL:
		bgm_nextlevel->play();
		break;

	case ENDING:
		bgm_ending->play();
		break;
	}

	BGM_STATUS = bgm_change_to;
}

void DrawKong(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	//인트로 애니메이션
	if (intro) {
		DrawIntroKong(hdc, memdc, oldBit);

		return;
	}

	//다음 스테이지 이동
	if (nextlevel) {
		DrawNextLevelKong(hdc, memdc, oldBit);

		return;
	}

	//엔딩
	if (ending) {
		DrawEndingKong(hdc, memdc, oldBit);

		return;
	}

	if (starting) {
		for (int x = 0; x < mapTileCnt; x++) {
			for (int y = 0; y < mapTileCnt; y++) {
				if (map[x][y].type == 6) {
					kong.x = x * mapTileSize;
					kong.y = y * mapTileSize;
				}
			}
		}
	}

	if (!pause)
		kong.animationCnt--;

	if (kong.animation == 0) {
		if (kong.animationCnt % (kongIdleAnimateInterp * 4) > kongIdleAnimateInterp * 3)
			oldBit = (HBITMAP)SelectObject(memdc, kong_stand1);
		else if (kong.animationCnt % (kongIdleAnimateInterp * 4) > kongIdleAnimateInterp * 2)
			oldBit = (HBITMAP)SelectObject(memdc, kong_stand2);
		else if (kong.animationCnt % (kongIdleAnimateInterp * 4) > kongIdleAnimateInterp)
			oldBit = (HBITMAP)SelectObject(memdc, kong_stand3);
		else 
			oldBit = (HBITMAP)SelectObject(memdc, kong_stand0);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));
	}
	else if (kong.animation == 1) {
		if (kong.rollRight)
			oldBit = (HBITMAP)SelectObject(memdc, kong_roll_L);
		else
			oldBit = (HBITMAP)SelectObject(memdc, kong_roll_R);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));

		if (!kong.big) {
			oldBit = (HBITMAP)SelectObject(memdc, barrel4_R);
			if (kong.rollRight)
				TransparentBlt(hdc, kong.x - 5, kong.y + 35, barrelSize, barrelSize, memdc, 0, 0, 14, 14, RGB(0, 105, 105));
			else
				TransparentBlt(hdc, kong.x + 55, kong.y + 35, barrelSize, barrelSize, memdc, 0, 0, 14, 14, RGB(0, 105, 105));
		}
		else {
			oldBit = (HBITMAP)SelectObject(memdc, s_barrel4_R);
			if (kong.rollRight)
				TransparentBlt(hdc, kong.x - 20, kong.y + 35 - barrelSize, barrelSize * 2, barrelSize * 2, memdc, 0, 0, 14, 14, RGB(0, 105, 105));
			else
				TransparentBlt(hdc, kong.x + 55, kong.y + 35 - barrelSize, barrelSize * 2, barrelSize * 2, memdc, 0, 0, 14, 14, RGB(0, 105, 105));
		}

		if (kong.animationCnt == 0) {
			kong.animation++;
			kong.animationCnt = kongRollAnimateInterp;
		}
	}
	else if (kong.animation == 2) {
		oldBit = (HBITMAP)SelectObject(memdc, kong_stand0);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));

		if (!kong.big) {
			oldBit = (HBITMAP)SelectObject(memdc, barrel1_F);
			TransparentBlt(hdc, kong.x + 20, kong.y + 32, barrelSize + barrelSize / 2, barrelSize, memdc, 0, 0, 18, 12, RGB(0, 105, 105));
		}
		else {
			oldBit = (HBITMAP)SelectObject(memdc, s_barrel1_F);
			TransparentBlt(hdc, kong.x + 16, kong.y + 35 - barrelSize, barrelSize * 2 + barrelSize / 3 - 5, barrelSize * 2, memdc, 0, 0, 18, 12, RGB(0, 105, 105));
		}

		if (kong.animationCnt == 0) {
			kong.animation++;
			kong.animationCnt = kongRollAnimateInterp;
		}
	}
	else if (kong.animation == 3) {
		if (kong.rollRight)
			oldBit = (HBITMAP)SelectObject(memdc, kong_roll_R);
		else
			oldBit = (HBITMAP)SelectObject(memdc, kong_roll_L);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));

		if (kong.animationCnt == kongRollAnimateInterp - 1) {
			for (int n = 0; n < barrelCnt; n++) {
				if (barrel[n].spawnReady) {
					barrel[n].spawnReady = false;
					barrel[n].active = true;
					break;
				}
			}
		}

		if (kong.animationCnt == 0) {
			kong.animation = 0;
			kong.animationCnt = kongIdleAnimateInterp * 4;
		}
	}
}

void DrawPrincess(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	if (intro && kong.animation != 102) {
		return;
	}

	if (nextlevel) {
		if (kong.animation == 0) {
			if (peachy.dirRight) 
				oldBit = (HBITMAP)SelectObject(memdc, princess2_R);
			else
				oldBit = (HBITMAP)SelectObject(memdc, princess2_L);
			TransparentBlt(hdc, peachy.x, peachy.y, 20, 27, memdc, 0, 0, 18, 25, RGB(0, 105, 105));

			int xAdj, yAdj;
			yAdj = -10;
			oldBit = (HBITMAP)SelectObject(memdc, princess_heart);
			if (peachy.dirRight)
				xAdj = 15;
			else
				xAdj = -15;
			TransparentBlt(hdc, peachy.x + xAdj, peachy.y + yAdj, 15, 15, memdc, 0, 0, 17, 17, RGB(0, 105, 105));
		}
		else {
			peachy.animation++;
			peachy.y += 5;

			if(peachy.animation % 8 < 2)	//0 1 
				oldBit = (HBITMAP)SelectObject(memdc, princess_heart);
			else if (peachy.animation % 8 < 4)	//2 3
				oldBit = (HBITMAP)SelectObject(memdc, princess_heart1);
			else if (peachy.animation % 8 < 6)	//4 5
				oldBit = (HBITMAP)SelectObject(memdc, princess_heart2);
			else //6 7
				oldBit = (HBITMAP)SelectObject(memdc, princess_heart3);

			TransparentBlt(hdc, peachy.x, peachy.y, 15, 15, memdc, 0, 0, 17, 17, RGB(0, 105, 105));
		}

		return;
	}

	if (ending) {
		peachy.y += 2;
		if (peachy.y > 95) {
			peachy.y = 95;

			if (boom[0].animation == 0) {
				oldBit = (HBITMAP)SelectObject(memdc, princess_heart);
				TransparentBlt(hdc, peachy.x + 20, peachy.y - 10, 15, 15, memdc, 0, 0, 17, 17, RGB(0, 105, 105));
			}
		}

		oldBit = (HBITMAP)SelectObject(memdc, princess0_R);
		TransparentBlt(hdc, peachy.x, peachy.y, 20, 27, memdc, 0, 0, 18, 25, RGB(0, 105, 105));

		return;
	}

	if (starting) {
		switch (gameStage) {
		case 1:
			peachy.dirRight = true;
			break;
		case 2:
			peachy.dirRight = true;
			break;
		case 3:
			peachy.dirRight = false;
			break;
		case 4:
			peachy.dirRight = false;
			break;
		case 5:
			peachy.dirRight = true;
			break;
		}

		for (int x = 0; x < mapTileCnt; x++) {
			for (int y = 0; y < mapTileCnt; y++) {
				if (map[x][y].type == 5) {
					peachy.x = x * mapTileSize;
					peachy.y = y * mapTileSize - 18;
				}
			}
		}
	}

	if (!pause)
		peachy.animation++;

	int animation;

	if (peachy.animation >= 100)
		peachy.animation = 0;

	if (peachy.animation < 55) {
		if (peachy.dirRight)
			oldBit = (HBITMAP)SelectObject(memdc, princess0_R);
		else
			oldBit = (HBITMAP)SelectObject(memdc, princess0_L);
		TransparentBlt(hdc, peachy.x, peachy.y, 20, 27, memdc, 0, 0, 18, 25, RGB(0, 105, 105));
	}
	else if (peachy.animation < 100) {
		animation = peachy.animation - 55;

		if (animation % 10 < 5) {
			if (peachy.dirRight)
				oldBit = (HBITMAP)SelectObject(memdc, princess1_R);
			else
				oldBit = (HBITMAP)SelectObject(memdc, princess1_L);
		}
		else {
			if (peachy.dirRight)
				oldBit = (HBITMAP)SelectObject(memdc, princess2_R);
			else
				oldBit = (HBITMAP)SelectObject(memdc, princess2_L);
		}

		TransparentBlt(hdc, peachy.x, peachy.y, 20, 27, memdc, 0, 0, 18, 25, RGB(0, 105, 105));

		if (peachy.dirRight) {
			oldBit = (HBITMAP)SelectObject(memdc, princess_help);
			TransparentBlt(hdc, peachy.x + 23, peachy.y, 26, 10, memdc, 0, 0, 26, 10, RGB(0, 105, 105));
		}
		else {
			oldBit = (HBITMAP)SelectObject(memdc, princess_help);
			TransparentBlt(hdc, peachy.x - 30, peachy.y, 26, 10, memdc, 0, 0, 26, 10, RGB(0, 105, 105));
		}
	}
}

void ResetBoom()
{
	for (int n = 0; n < boomCnt; n++) {
		boom[n].animation = 0;
		boom[n].x = 0;
		boom[n].y = 0;
	}
}

void ResetKong()
{
	kong.animation = 0;
	kong.animationCnt = 0;
}

void ResetPrincess()
{
	peachy.animation = 0;
}

void DrawIntroKong(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	int yloc = 90;
	int xloc = 40;

	int kongJumpInterp = 10;

	if (kong.y > yloc && kong.animation == 100) {			// kong.animation = 100
		kong.y -= marioClimbSpeed * 4;
		EraseLadders();
		
		if (kong.y <= yloc) {
			kong.animation = 101;
			kong.animationCnt = 1;
		}
	}
	else if (kong.x > xloc && kong.animation == 101) {	// kong.animation = 101
		kong.x -= 4;

		if (kong.jump == 1 || kong.jump == 3) kong.y -= 4;
		else kong.y += 4;
	}
	else if (kong.x < xloc) {
		kong.y = 90;
		kong.animation = 102;

		peachy.dirRight = true;
		peachy.x = 2 * mapTileSize;
		peachy.y = 7 * mapTileSize - 18;

		SetUpLadders();
	}

	if (kong.x < 225 && introAnimCnt == 1) {
		introAnimCnt = 2;
		LoadIntro();
	}
	else if (kong.x < 150 && introAnimCnt == 2) {
		introAnimCnt = 3;
		LoadIntro();
	}
	else if (kong.x < 85 && introAnimCnt == 3) {
		introAnimCnt = 4;
		LoadIntro();
	}

	if (kong.animation == 100) {
		kong.animationCnt++;

		if (kong.animationCnt % 8 < 4)
			oldBit = (HBITMAP)SelectObject(memdc, kong_climb1);
		else
			oldBit = (HBITMAP)SelectObject(memdc, kong_climb2);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));
	}
	else {

		if (kong.jump == 1 || kong.jump == 2)
			kong.animationCnt++;
		else
			kong.animationCnt--;

		if (kong.animationCnt > kongJumpInterp - kongJumpInterp / 5) {
			oldBit = (HBITMAP)SelectObject(memdc, kong_move2_R);
			if (kong.jump == 2 && kong.animationCnt == kongJumpInterp)
				kong.jump = 3;
		}
		else if (kong.animationCnt > kongJumpInterp - kongJumpInterp / 5 * 2)
			oldBit = (HBITMAP)SelectObject(memdc, kong_move1_R);
		else if (kong.animationCnt > kongJumpInterp - kongJumpInterp / 5 * 3) {
			oldBit = (HBITMAP)SelectObject(memdc, kong_move0);
			if (kong.jump == 1 && kong.animationCnt == kongJumpInterp / 2)
				kong.jump = 2;
			else if (kong.jump == 3 && kong.animationCnt == kongJumpInterp / 2)
				kong.jump = 4;
		}
		else if (kong.animationCnt > kongJumpInterp - kongJumpInterp / 5 * 4)
			oldBit = (HBITMAP)SelectObject(memdc, kong_move1_L);
		else {
			oldBit = (HBITMAP)SelectObject(memdc, kong_move2_L);
			if (kong.jump == 4 && kong.animationCnt == 1)
				kong.jump = 1;
		}

		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));
	}

}

void EraseLadders()
{
	for (int y = 0; y < mapTileCnt; y++) {
		if (kong.y + 40 < y * mapTileSize) {
			map[34][y].type = 0;
			map[36][y].type = 0;
			if (map[35][y].type == 1) {
				map[34][y].type = 1;
				map[36][y].type = 1;
			}
		}
	}
}

void SetUpLadders()
{
	for (int x = 0; x < mapTileCnt; x++) {
		for (int y = 0; y < mapTileCnt; y++) {
			if (map[x][y].type == (int)'*' - 48) {
				int x_ = x;
				int y_ = y;
				while (1) {
					if (y_ < 0)
						break;
					else if (map[x_][y_].type == 1 || map[x_][y_].type == 2) {
						map[x_][y_ + 1].type = 2;
						break;
					}
					y_--;
				}
			}

			if (map[x][y].type == (int)'#' - 48) {
				int x_ = x;
				int y_ = y;
				while (1) {
					if (y_ < 0)
						break;
					else if (map[x_][y_].type == 1 || map[x_][y_].type == 2) {
						map[x_][y_ - 1].type = 2;
						break;
					}
					y_++;
				}
			}
		}
	}
}

void DrawNextLevelKong(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	kong.animationCnt++;

	if (kong.animation == 0) {
		if (kong.animationCnt > 30) {
			kong.animation = 200;

			peachy.y -= 10;
			if (peachy.dirRight)
				peachy.x += 15;
			else
				peachy.x -= 15;
		}

		oldBit = (HBITMAP)SelectObject(memdc, kong_stand1);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));
	}
	else {
		kong.y -= marioClimbSpeed * 3;

		if (kong.animationCnt % 8 < 4)
			oldBit = (HBITMAP)SelectObject(memdc, kong_climb1);
		else
			oldBit = (HBITMAP)SelectObject(memdc, kong_climb2);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));
	}
}

void DrawEndingKong(HDC hdc, HDC memdc, HBITMAP oldBit)
{
	kong.animationCnt++;

	if (kong.animationCnt < 30) {
		oldBit = (HBITMAP)SelectObject(memdc, kong_stand1);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));
		kong.jump = 0;
	}
	else if (kong.y < WindowHeight + 50) {
		kong.y += kong.jump;	//그냥 (int)gravity 새로 선언해서 사용하기 싫어서 안 사용 중인 jump 사용 
		kong.jump += gravity;
		if (kong.jump > gravity * 6)
			kong.jump = gravity * 6;

		if (kong.animationCnt % 9 < 3) 
			oldBit = (HBITMAP)SelectObject(memdc, kong_fall1);
		else if (kong.animationCnt % 9 < 6) 
			oldBit = (HBITMAP)SelectObject(memdc, kong_fall2);
		else 
			oldBit = (HBITMAP)SelectObject(memdc, kong_fall3);
		TransparentBlt(hdc, kong.x, kong.y, 63, 50, memdc, 0, 0, 49, 38, RGB(0, 105, 105));

		if (boom[0].x == 0 && boom[0].y == 0) {
			ActivateBoom(325, 100);
			ActivateBoom(350, 100);
			ActivateBoom(375, 100);
			ActivateBoom(400, 100);
		}
		else {
			for (int n = 0; n < boomCnt; n++) {
				if (boom[n].animation > 0) {
					boom[n].animation--;
				}
			}
		}
	}
}

void DrawEndingMap(HDC hdc, HDC memdc, HBITMAP oldBit, int x, int y)
{
	int extra = 0;

	if (x == 0)
		oldBit = (HBITMAP)SelectObject(memdc, platform_L);
	else if (x == mapTileCnt - 1)
		oldBit = (HBITMAP)SelectObject(memdc, platform_R);
	else if (map[x - 1][y].type != 1 && map[x + 1][y].type != 1) {
		oldBit = (HBITMAP)SelectObject(memdc, platform_S);
		extra = 1;
	}
	else if (map[x - 1][y].type != 1)
		oldBit = (HBITMAP)SelectObject(memdc, platform_L);
	else if (map[x + 1][y].type != 1)
		oldBit = (HBITMAP)SelectObject(memdc, platform_R);
	else
		oldBit = (HBITMAP)SelectObject(memdc, platform_M);

	int yPos = y * mapTileSize + kong.animationCnt * 2;
	if (yPos > 120) 
		yPos = 120;

	TransparentBlt(hdc, x * mapTileSize, yPos, mapTileSize, mapTileSize, memdc, 0, 0, mapTileSize + extra, mapTileSize, RGB(0, 105, 105));
}

void DrawPopUpMessage(HDC hdc)
{
	TCHAR keyStr[100], desc1Str[100], desc2Str[100];

	SetBkMode(hdc, TRANSPARENT);

	if (intro && kong.x < 40) {
		SetTextColor(hdc, RGB(250, 250, 250));
		wsprintf(desc1Str, L"press");
		TextOut(hdc, 47, 680, desc1Str, lstrlen(desc1Str));

		SetTextColor(hdc, RGB(0, 250, 0));
		wsprintf(keyStr, L"\'ENTER\'");
		TextOut(hdc, 87, 680, keyStr, lstrlen(keyStr));

		SetTextColor(hdc, RGB(250, 250, 250));
		wsprintf(desc2Str, L"to start");
		TextOut(hdc, 145, 680, desc2Str, lstrlen(desc2Str));
	}

	if (pause) {
		int wordSize = 70;

		text_xPos -= 3;
		if(text_xPos + WindowWidth / 4 + wordSize < 0)
			text_xPos += (WindowWidth + wordSize);

		SetTextColor(hdc, RGB(255, 255, 0));
		wsprintf(keyStr, L" Paused ");

		if (text_xPos - WindowWidth / 4 + wordSize < 0)
			TextOut(hdc, text_xPos - WindowWidth / 4 + WindowWidth + wordSize, WindowHeight / 3, keyStr, lstrlen(keyStr));
		else
			TextOut(hdc, text_xPos - WindowWidth / 4, WindowHeight / 3, keyStr, lstrlen(keyStr));

		if (text_xPos + wordSize < 0)
			TextOut(hdc, text_xPos + WindowWidth + wordSize, WindowHeight / 2, keyStr, lstrlen(keyStr));
		else
			TextOut(hdc, text_xPos, WindowHeight / 2, keyStr, lstrlen(keyStr));

		TextOut(hdc, text_xPos + WindowWidth / 4, WindowHeight - WindowHeight / 3, keyStr, lstrlen(keyStr));
	}

	if (mario.life == 0 && mario.yPos > WindowHeight) {
		int strSize = 80;
		int fwordSize = 40;

		text_xPos -= 3;
		if (text_xPos + strSize < 0)
			text_xPos += (WindowWidth + strSize - fwordSize);

		SetTextColor(hdc, RGB(250, 250, 250));
		wsprintf(desc1Str, L"press");
		
		if (text_xPos < 0)
			TextOut(hdc, text_xPos + WindowWidth, WindowHeight / 2, desc1Str, lstrlen(desc1Str));
		else
			TextOut(hdc, text_xPos - fwordSize, WindowHeight / 2, desc1Str, lstrlen(desc1Str));

		SetTextColor(hdc, RGB(250, 0, 0));
		wsprintf(keyStr, L"\'R\'");
		if (text_xPos + fwordSize < 0)
			TextOut(hdc, text_xPos + WindowWidth + fwordSize, WindowHeight / 2, keyStr, lstrlen(keyStr));
		else
			TextOut(hdc, text_xPos, WindowHeight / 2, keyStr, lstrlen(keyStr));
		
		SetTextColor(hdc, RGB(250, 250, 250));
		wsprintf(desc2Str, L"to restart");
		TextOut(hdc, text_xPos + 23, WindowHeight / 2, desc2Str, lstrlen(desc2Str));
	}

	if (ending && bgm_ending->status() == false) {
		SetTextColor(hdc, RGB(250, 250, 250));
		wsprintf(desc1Str, L"press");
		TextOut(hdc, peachy.x + 20 - 40, peachy.y - 50, desc1Str, lstrlen(desc1Str));

		SetTextColor(hdc, RGB(0, 0, 250));
		wsprintf(keyStr, L"\'Q\'");
		TextOut(hdc, peachy.x + 20, peachy.y - 50, keyStr, lstrlen(keyStr));

		SetTextColor(hdc, RGB(250, 250, 250));
		wsprintf(desc2Str, L"to exit");
		TextOut(hdc, peachy.x + 20 + 23, peachy.y - 50, desc2Str, lstrlen(desc2Str));
	}
}