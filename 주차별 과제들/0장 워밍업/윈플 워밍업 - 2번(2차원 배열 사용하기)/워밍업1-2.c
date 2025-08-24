#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)

void Procession(int*);
void Print(int*);
void Dc(int*);
void As(int*);

void Procession(int a[])
{
	int r;
	int j = 0;

	for (int i = 0; i < 20; i++) {
		while (j < 20) {
			r = rand() % 50 + 1;
			//중복검사
			for (j = 0; j <= i; j++) {
				if (r == a[j]) break;
			}
			if (j < i) continue;
			else {
				a[i] = r;	//값 대입
				break;
			}
		}
	}
}

void Print(int a[])
{
	for (int c = 0; c < 20; c += 5) {
		for (int b = 0; b < 5; b++)
		{
			printf("%4d ", a[c + b]);
		}
		printf("\n");
	}
}

//1행5열 시작으로 열따라 내림차순
void Dc(int a[])
{
	int temp[20];
	int i = 0;

	for (int c = 3; c >= 0; --c) {
		for (int b = 0; b < 5; ++b) {
			temp[i] = a[c + 4 * b];
			++i;
		}
	}
	
	for (int idx = 0; idx < 20; ++idx) {
		a[idx] = temp[idx];
	}
}

//1행1열 시작으로 행따라 오름차순 정렬
void As(int a[]) //condition==1
{
	int temp;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20 - i - 1; j++) {
			if (a[j] > a[j + 1]) {
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}

int main()
{
	int matrix[20] = { 0 };
	int max, min;
	_Bool Q = 1;
	int itemp, idx1, idx2;

	srand(time(NULL));

	Procession(matrix);
	printf("행렬값: \n");
	Print(matrix);

	Q = 1;

	while (Q)
	{
		printf("\n\n명령어 입력해주세요:");
		char W = getchar();
		while (getchar() != '\n');

		switch (W) {
		case 'a':
			printf("명령어 입력 a:\n");
			As(matrix);
			Print(matrix);
			break;

		case 'd':
			printf("명령어 입력 d:\n");
			As(matrix);	//일단 오름차순 정리 후
			Dc(matrix);	//이를 이용해서 내림차순으로 정리
			Print(matrix);
			break;

		case 'e':
			printf("명령어 입력 e:\n");

			for (int c = 0; c < 20; c += 5) {
				for (int b = 0; b < 5; b++)
				{
					if (matrix[c + b] % 2 == 0)
						printf("%4d ", matrix[c + b]);
					else
						printf("   * ");
				}
				printf("\n");
			}
			break;

		case 'o':
			printf("명령어 입력 o:\n");

			for (int c = 0; c < 20; c += 5) {
				for (int b = 0; b < 5; b++)
				{
					if (matrix[c + b] % 2 != 0)
						printf("%4d ", matrix[c + b]);
					else
						printf("   * ");
				}
				printf("\n");
			}
			break;

		case 'm':
			printf("명령어 입력 m:\n");

			max = matrix[0];

			for (int i = 0; i < 20; i++)
			{
				max = matrix[i] > max ? matrix[i] : max;
			}

			printf("%d\n", max);
			break;

		case 'n':
			printf("명령어 입력 n:\n");

			min = matrix[0];

			for (int i = 0; i < 20; i++)
			{
				min = matrix[i] < min ? matrix[i] : min;
			}

			printf("%d\n", min);
			break;

		case 'p':
			for (int i = 0; i < 20; i++) {
				if (i % 5 == 0)
					matrix[i] = (matrix[i] + matrix[i + 1] + matrix[i + 2] + matrix[i + 3]);
			}
			Print(matrix);
			break;

		case 'r':
			printf("명령어 입력 r:\n");

			for (int i = 0; i < 100; i++)
			{
				idx1 = rand() % 20;
				idx2 = rand() % 20;
				itemp = matrix[idx1];
				matrix[idx1] = matrix[idx2];
				matrix[idx2] = itemp;
			}
			Print(matrix);
			break;

		case 's':
			printf("명령어 입력 s:\n");

			Procession(matrix);
			Print(matrix);
			break;

		case 'q':
			printf("명령어 입력 q:\n");
			Q = 0;
			break;

		}
	}

}