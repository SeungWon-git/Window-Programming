#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

void spacebar(char A[]);

int main()
{
	char S[60 + 1];
	int R = 0;	//단어 갯수
	_Bool e = 0;	//error 유무
	_Bool f_pushed = 0;	//f가 눌러졌는지 아닌지
	char temp;
	char command;

	printf("Input sentence: ");

	gets(S);

	spacebar(S);	//연속 띄어쓰기 없애기

	if (S[strlen(S) - 1] != 46) {
		printf("Error: The sentence is not ended.\n");
		exit(0);
	}

	for (int n = 0; n < 60; ++n) {	
		if (S[n] == 32) 	//띄어쓰기
			++R;
		else if (S[n] == 46) {	//끝
			if (n == 0)	//첫글자가 점일때 => 단어 0개출력
				break;
			else if (S[n - 1] >= 65 && S[n - 1] <= 90 || S[n - 1] >= 97 && S[n - 1] <= 122) {
				++R;
				break;
			}
		}
		else if (S[n] < 65 || S[n] > 90 && S[n] < 97 || S[n] > 122)
			e = 1;

		//printf("S[%d]: %c, R: %d\n", n, S[n], R);
	}

	if (e == 1) {
		printf("Error: Special character is included. Please use Alphabets only. \n");
		exit(0);
	}
	
	while (1) {
		printf("명령어: ");
		scanf("%c", &command);
		while (getchar() != '\n');

		switch (command) {
		case 'e':
		case 'E':
			for (int n = 0; n < 60; ++n) {
				if (S[n] == 'e' || S[n] == 'E') {
					int j = n;
					int i = n;

					while (S[j] != 32 && S[j] != '@' && j >= 0)			// e,E가 들어가있는 단어들의 인덱스 구하기
					{
						j--;
					}
					while (S[i] != 32 && S[i] != '@' && S[i] != 46 && i <= 60)
					{
						i++;
					}

					for (int x = j + 1; x <= i - 1; ++x)  //해당 인덱스의 단어 모두 대문자로 바꾸기
					{
						if (S[x] == 46)		// 점을 변경하면 오류남
							break;
						else if (S[x] >= 97 && S[x] <= 122)
							S[x] -= 32;
					}
				}
				else if (S[n] == 46 || S[n] == NULL)
					break;
			}
			printf("E/e포함 단어 대문자 변경: %s\n", S);
			break;

		case 'f':
		case 'F':
			f_pushed = !f_pushed;
			for (int n = 0; n < 59; ++n) {
				if (f_pushed && S[n] == 32)
					S[n] = '@';
				else if (!f_pushed && S[n] == '@')
					S[n] = 32;
			}
			printf("공백을 특수문자로 변경: %s\n", S);
			break;

		case 'l':
		case 'L':
			printf("단어의 개수: %d개\n", R);
			break;

		case 'c':
		case 'C':
			for (int n = 0; n < 60; ++n){
				if (S[n] >= 97 && S[n] <= 122) {
					S[n] -= 32;
				}
				else if (S[n] >= 65 && S[n] <= 90) {
					S[n] += 32;
				}
			}
			printf("대/소 변경: %s\n", S);
			break;

		case 'a':
			for (int n = 59; n >= 0; --n) {
				if (S[n] == 46) {
					temp = S[0];

					for (int _n = 0; _n < n; ++_n) {
						S[_n] = S[_n + 1];
					}

					S[n - 1] = temp;

					break;
				}
			}
			printf("문자 좌측으로 한 칸 이동, 맨 앞의 문자 맨 뒤로: %s\n", S);
			break;
		case 'A':
			for (int n = 59; n >= 0; --n) {
				if (S[n] == 46) {
					temp = S[n - 1];

					for (int _n = n - 2; _n >= 0; --_n) {
						S[_n + 1] = S[_n];
					}

					S[0] = temp;

					break;
				}
			}
			printf("문자 우측으로 한 칸 이동, 맨 뒤의 문자 맨 앞으로: %s\n", S);
			break;

		case 'q':
		case 'Q':
			printf(" <종료>\n");
			exit(0);
			break;
		}
	}
	
}

void spacebar(char A[])
{
	int n = 0;

	if (A[0] == 32) {	//처음 시작 공백 없애기
		for (int i = 0; i < 60; ++i)
		{
			A[i] = A[i + 1];
		}
	}

	while (A[n] != 46 && A[n] != NULL)
	{
		if (A[n] == 32 && A[n + 1] == 46) {//띄어쓰기 다음 마침표로 끝날시 띄어쓰기 없애기
			A[n] = 46;
			A[n + 1] = NULL;
		}
		if (A[n] == 32 && A[n + 1] == 32)
		{
			for (int i = n; i < 60; ++i)
			{
				A[i] = A[i + 1];
			}
			continue;
		}
		n++;
	}
}