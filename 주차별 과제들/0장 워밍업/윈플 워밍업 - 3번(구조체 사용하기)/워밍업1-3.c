#include<stdio.h>
#include <windows.h>
#include <stdlib.h>
#include<conio.h>
#pragma warning(disable:4996)

int board[10][10] = { 0 };
enum { EMPTY, BLACK, WHITE, S_BLACK, S_WHITE };
char* arTile[] = { "  ","��","��", "��", "��" };
BOOL crash = FALSE;

struct Stone
{
	int shape;
	int x, y;
};

void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void Printscreen()
{
	//��ĭ ������ ��
	for (int i = 0; i < 21; i += 2)
	{
		for (int j = 0; j < 31; j++) {
			gotoxy(j, i);
			printf("-");
		}
		printf("\n");
	}
	//��ĭ ������ ��
	for (int i = 1; i < 21; i += 2)
	{
		for (int j = 0; j < 11; j++) {
			gotoxy(3 * j, i);
			printf("|");
		}
		printf("\n");
	}
	//�� �׸���
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j) {
			gotoxy(3 * i + 1, j * 2 + 1);
			puts(arTile[board[i][j]]);
		}
	}
}

void Movestone(_Bool order, int dr, struct Stone* s)	//dr: 1-��,2-�Ʒ�,3-��,4-��
{
	//�����¿� �����̱�
	if (dr == 1) {
		if (s->y == 0) {
			if (s->shape > 2)
				s->shape -= 2;
			else
				s->shape += 2;

			board[s->x][9] = s->shape;
			s->y = 9;
		}
		else {
			board[s->x][s->y - 1] = s->shape;
			s->y -= 1;
		}
	}
	else if (dr == 2) {
		if (s->y == 9) {
			if (s->shape > 2)
				s->shape -= 2;
			else
				s->shape += 2;

			board[s->x][0] = s->shape;
			s->y = 0;
		}
		else {
			board[s->x][s->y + 1] = s->shape;
			s->y += 1;
		}
	}
	else if (dr == 3) {
		if (s->x == 0) {
			if (s->shape > 2)
				s->shape -= 2;
			else
				s->shape += 2;

			board[9][s->y] = s->shape;
			s->x = 9;
		}
		else {
			board[s->x - 1][s->y] = s->shape;
			s->x -= 1;
		}
	}
	else if (dr == 4) {
		if (s->x == 9) {
			if (s->shape > 2)
				s->shape -= 2;
			else
				s->shape += 2;

			board[0][s->y] = s->shape;
			s->x = 0;
		}
		else {
			board[s->x + 1][s->y] = s->shape;
			s->x += 1;
		}
	}

}

void Crash(_Bool order, struct Stone* w, struct Stone* b)
{
	//�浹üũ
	if (w->x == b->x && w->y == b->y) {
		Beep(380, 1000);	//2�ʵ��� '��'�� ���

		if (order == 0) {//���� �浹�� ����������
			if (b->shape > 2) 
				b->shape -= 2;
			else
				b->shape += 2;
		}
		else {
			if (w->shape > 2) 
				w->shape -= 2;
			else
				w->shape += 2;
		}

		crash = TRUE;
	}
}

int main()
{
	struct Stone b;
	struct Stone w;
	_Bool order = 1;	//����: 0-��,1-��
	char input;

	srand(time(NULL));

	b.x = rand() % 10;
	b.y = rand() % 10;
	b.shape = 1;

	board[b.x][b.y] = b.shape;

	w.x = rand() % 10;
	w.y = rand() % 10;
	w.shape = 2;

	while (b.x == w.x && b.y == w.y) {
		w.x = rand() % 10;
		w.y = rand() % 10;
	}

	board[w.x][w.y] = w.shape;

	while (1) {
		//ȭ���ʱ�ȭ
		system("cls");

		//�ٵ���
		Printscreen();

		gotoxy(35, 19);
		printf("Reset: r");
		gotoxy(35, 20);
		printf("Quit: q");

		//��ġ����
		gotoxy(35, 0);
		printf("BLACK: (%d,%d)\n", b.x + 1, b.y + 1);
		gotoxy(35, 1);
		printf("WHITE: (%d,%d)\n", w.x + 1, w.y + 1);

		//�Է�â
		gotoxy(0, 21);
		if (order == 1)
			printf("([black]-wasd)Input command: ");
		else
			printf("([white]-ijkl)Input command: ");

		input = getch();

		// ����
		if (input == 'r') {
			printf(" <reset>\n");
			system("pause");


			board[b.x][b.y] = 0;

			b.x = rand() % 10;
			b.y = rand() % 10;
			b.shape = 1;

			board[b.x][b.y] = b.shape;

			board[w.x][w.y] = 0;

			w.x = rand() % 10;
			w.y = rand() % 10;
			w.shape = 2;

			while (b.x == w.x && b.y == w.y) {
				w.x = rand() % 10;
				w.y = rand() % 10;
			}

			board[w.x][w.y] = w.shape;
		}
		// ����
		else if (input == 'q') {
			printf(" <exit>\n");
			break;
		}
		else if (((order == 1) && (input == 'w' || input == 'a' || input == 's' || input == 'd')) || ((order == 0) && (input == 'i' || input == 'j' || input == 'k' || input == 'l'))) {
			//�����̱�
			switch (input) {
				//�浹
			case 'w':
				if (crash == FALSE)
					board[b.x][b.y] = 0;
				else {
					board[b.x][b.y] = w.shape;
					crash = FALSE;
				}
				Movestone(order, 1, &b);
				Crash(order, &w, &b);
				break;
			case 's':
				if (crash == FALSE)
					board[b.x][b.y] = 0;
				else {
					board[b.x][b.y] = w.shape;
					crash = FALSE;
				}
				Movestone(order, 2, &b);
				Crash(order, &w, &b);
				break;
			case 'a':
				if (crash == FALSE)
					board[b.x][b.y] = 0;
				else {
					board[b.x][b.y] = w.shape;
					crash = FALSE;
				}
				Movestone(order, 3, &b);
				Crash(order, &w, &b);
				break;
			case 'd':
				if (crash == FALSE)
					board[b.x][b.y] = 0;
				else {
					board[b.x][b.y] = w.shape;
					crash = FALSE;
				}
				Movestone(order, 4, &b);
				Crash(order, &w, &b);
				break;


				//�鵹
			case 'i':
				if (crash == FALSE)
					board[w.x][w.y] = 0;
				else {
					board[w.x][w.y] = b.shape;
					crash = FALSE;
				}
				Movestone(order, 1, &w);
				Crash(order, &w, &b);
				break;
			case 'k':
				if (crash == FALSE)
					board[w.x][w.y] = 0;
				else {
					board[w.x][w.y] = b.shape;
					crash = FALSE;
				}
				Movestone(order, 2, &w);
				Crash(order, &w, &b);
				break;
			case 'j':
				if (crash == FALSE)
					board[w.x][w.y] = 0;
				else {
					board[w.x][w.y] = b.shape;
					crash = FALSE;
				}
				Movestone(order, 3, &w);
				Crash(order, &w, &b);
				break;
			case 'l':
				if (crash == FALSE)
					board[w.x][w.y] = 0;
				else {
					board[w.x][w.y] = b.shape;
					crash = FALSE;
				}
				Movestone(order, 4, &w);
				Crash(order, &w, &b);
				break;
			}

			order = !order;	//���� �ٲٱ�
		}
		else
			continue;
	}
}