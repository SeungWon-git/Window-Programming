#include <stdio.h>
#pragma warning(disable:4996)

void Printcase1(int N)
{
	if (N % 2 == 1) {
		for (int j = 0; j < N / 2; ++j) {
			for (int i = 0; i < N - 1; i++)
				printf(" ");
			for (int i = 0; i < j; ++i)
				printf(" ");
			printf("*");
			for (int i = 0; i < N - 1 - 2 * j; i++)
				printf(" ");
			printf("*");
			printf("\n");
		}
		for (int i = 0; i < (N + 1) / 2 + N - 1; i++)
			printf(" ");
		printf("*\n");
		for (int j = 0; j < N / 2; ++j)
		{
			for (int i = 0; i < N - 1; i++)
				printf(" ");
			for (int i = N / 2 - j - 1; i > 0; --i)
				printf(" ");
			printf("*");
			for (int i = 2 * (j + 1); i > 0; --i)
				printf(" ");
			printf("*");
			printf("\n");
		}
	}
	else if (N % 2 == 0) {
		for (int j = 0; j < N / 2; ++j) {
			for (int i = 0; i < N; i++)
				printf(" ");
			for (int i = 0; i < j; ++i)
				printf(" ");
			printf("*");
			for (int i = 0; i < N - 2 * (j + 1); i++)
				printf(" ");
			printf("*");
			printf("\n");
		}
		for (int j = 0; j < N / 2; ++j)
		{
			for (int i = 0; i < N; i++)
				printf(" ");
			for (int i = N / 2 - j - 1; i > 0; --i)
				printf(" ");
			printf("*");
			for (int i = 2 * j; i > 0; --i)
				printf(" ");
			printf("*");
			printf("\n");
		}
	}
}

void Printcase2(int N)
{
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < i; ++j) {
			printf(" ");
		}
		for (int j = 0; j < N * 2 - i * 2; ++j) {
			printf("*");
		}
		printf("\n");
	}
}

void Printcase3(int N)
{
	if (N % 2 == 1) {
		for (int i = 0; i < N; ++i)
			printf(" ");
		printf("*\n");
		for (int j = 0; j < N / 2 - 1; ++j) {
			for (int i = 0; i < 2 * (N / 2 - 1 - j); ++i)
				printf(" ");
			for (int n = 0; n < 2 * N - 4 * (N / 2 - 1 - j); ++n)
				printf("*");
			printf("\n");
		}
		for (int i = 0; i < 2 * N; i++) {
			printf("*");
		}
		printf("\n");
		for (int j = 0; j < N / 2 - 1; ++j) {
			for (int i = 0; i < 2 * (j + 1); ++i)
				printf(" ");
			for (int n = 0; n < 2 * N - 4 * (j + 1); ++n)
				printf("*");
			printf("\n");
		}
		for (int i = 0; i < N; ++i)
			printf(" ");
		printf("*\n");
	}
	else
	{
		for (int i = 0; i < N; ++i)
			printf(" ");
		printf("*\n");
		for (int j = 0; j < N / 2 - 2; ++j) {
			for (int i = 0; i < 2 * (N / 2 - 2 - j); ++i)
				printf(" ");
			for (int n = 0; n < 2 * N - 4 * (N / 2 - 2 - j); ++n)
				printf("*");
			printf("\n");
		}
		for (int i = 0; i < 2 * N; i++) {
			printf("*");
		}
		printf("\n");
		for (int i = 0; i < 2 * N; i++) {
			printf("*");
		}
		printf("\n");
		for (int j = 0; j < N / 2 - 2; ++j) {
			for (int i = 0; i < 2 * (j + 1); ++i)
				printf(" ");
			for (int n = 0; n < 2 * N - 4 * (j + 1); ++n)
				printf("*");
			printf("\n");
		}
		for (int i = 0; i < N; ++i)
			printf(" ");
		printf("*\n");
	}
}

void Printcase4(int N)
{
	if (N % 2 == 1) {//홀수의 경우
		for (int i = 0; i < N / 2; ++i) {
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			for (int j = 0; j < N * 2 - 2 * (2 * i + 1); ++j) {
				printf(" ");
			}
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			printf("\n");
		}
		for (int i = 0; i < 2 * N; ++i) {
			printf("*");
		}
		printf("\n");
		for (int i = N / 2 - 1; i >= 0; --i) {
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			for (int j = 0; j < N * 2 - 2 * (2 * i + 1); ++j) {
				printf(" ");
			}
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			printf("\n");
		}
	}
	else  //짝수의 경우
	{
		for (int i = 0; i < N / 2; ++i) {
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			for (int j = 0; j < N * 2 - 2 * (2 * i + 1); ++j) {
				printf(" ");
			}
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			printf("\n");
		}
		for (int i = N / 2 - 1; i >= 0; --i) {
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			for (int j = 0; j < N * 2 - 2 * (2 * i + 1); ++j) {
				printf(" ");
			}
			for (int j = 0; j < 2 * i + 1; ++j) {
				printf("*");
			}
			printf("\n");
		}
	}
}

void Printcase5(int N)
{
	if (N % 2 == 1) { //홀수의 경우
		for (int i = N  / 2; i > 0; --i) {
			for (int j = 0; j < 2 * (N / 2 - i); ++j) {
				printf(" ");
			}
			for (int j = 0; j <= i * 2 * 2 + 1; ++j) {
				printf("*");
			}
			printf("\n");
		}
		for (int i = 0; i < N - 1; ++i) {
			printf(" ");
		}
		printf("**");
		printf("\n");
		for (int i = 0; i < N / 2; ++i) {
			for (int j = 0; j < 2 * (N / 2 - (i + 1)); ++j) {
				printf(" ");
			}
			for (int j = 0; j <= (i + 1) * 2 * 2 + 1; ++j) {
				printf("*");
			}
			printf("\n");
		}
	}
	else  //짝수의 경우
	{
		for (int i = N / 2; i > 0; --i) {
			for (int j = 0; j < 2 * (N / 2 - i); ++j) {
				printf(" ");
			}
			for (int j = 0; j < i * 2 * 2; ++j) {
				printf("*");
			}
			printf("\n");
		}
		for (int i = 0; i < N / 2; ++i) {
			for (int j = 0; j < 2 * (N / 2 - (i + 1)); ++j) {
				printf(" ");
			}
			for (int j = 0; j < (i + 1) * 2 * 2; ++j) {
				printf("*");
			}
			printf("\n");
		}
	}
}

void Printcase6(int N)
{
	for (int i = 0; i < N; ++i) {
		if (i == 0 || i == N - 1) {
			for (int j = 0; j < 2 * N; ++j)
				printf("*");
		}
		else {
			printf("*");
			for (int j = 0; j < 2 * N - 2; ++j)
				printf(" ");
			printf("*");
		}
		printf("\n");
	}
}

int main()		
{
	int a, b;

	while (1) {
		printf("<모양> 1- X, 2- 역삼각형, 3- 마름모, 4- 나비, 5-  모래시계, 6- 박스/ <너비는 무조건 짝수>\nInput command [모양 너비] : ");
		scanf("%d %d", &a, &b);
		while (getchar() != '\n');

		if (b % 2 == 1)
		{
			printf("잘못입력하였습니다. 너비는 짝수를 입력해주십시오.\n\n");
			continue;
		}
		break;
	}

	switch (a) {
	case 1:
		Printcase1(b / 2);
		break;
	case 2:
		Printcase2(b / 2);
		break;
	case 3:
		Printcase3(b / 2);
		break;
	case 4:
		Printcase4(b / 2);
		break;
	case 5:
		Printcase5(b / 2);
		break;
	case 6:
		Printcase6(b / 2);
		break;
	}

	printf("끝내기 위해 아무버튼: ");
	getchar();
}