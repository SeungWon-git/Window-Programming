#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

void spacebar(char A[]);

int main()
{
	char S[60 + 1];
	int R = 0;	//�ܾ� ����
	_Bool e = 0;	//error ����
	_Bool f_pushed = 0;	//f�� ���������� �ƴ���
	char temp;
	char command;

	printf("Input sentence: ");

	gets(S);

	spacebar(S);	//���� ���� ���ֱ�

	if (S[strlen(S) - 1] != 46) {
		printf("Error: The sentence is not ended.\n");
		exit(0);
	}

	for (int n = 0; n < 60; ++n) {	
		if (S[n] == 32) 	//����
			++R;
		else if (S[n] == 46) {	//��
			if (n == 0)	//ù���ڰ� ���϶� => �ܾ� 0�����
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
		printf("��ɾ�: ");
		scanf("%c", &command);
		while (getchar() != '\n');

		switch (command) {
		case 'e':
		case 'E':
			for (int n = 0; n < 60; ++n) {
				if (S[n] == 'e' || S[n] == 'E') {
					int j = n;
					int i = n;

					while (S[j] != 32 && S[j] != '@' && j >= 0)			// e,E�� ���ִ� �ܾ���� �ε��� ���ϱ�
					{
						j--;
					}
					while (S[i] != 32 && S[i] != '@' && S[i] != 46 && i <= 60)
					{
						i++;
					}

					for (int x = j + 1; x <= i - 1; ++x)  //�ش� �ε����� �ܾ� ��� �빮�ڷ� �ٲٱ�
					{
						if (S[x] == 46)		// ���� �����ϸ� ������
							break;
						else if (S[x] >= 97 && S[x] <= 122)
							S[x] -= 32;
					}
				}
				else if (S[n] == 46 || S[n] == NULL)
					break;
			}
			printf("E/e���� �ܾ� �빮�� ����: %s\n", S);
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
			printf("������ Ư�����ڷ� ����: %s\n", S);
			break;

		case 'l':
		case 'L':
			printf("�ܾ��� ����: %d��\n", R);
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
			printf("��/�� ����: %s\n", S);
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
			printf("���� �������� �� ĭ �̵�, �� ���� ���� �� �ڷ�: %s\n", S);
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
			printf("���� �������� �� ĭ �̵�, �� ���� ���� �� ������: %s\n", S);
			break;

		case 'q':
		case 'Q':
			printf(" <����>\n");
			exit(0);
			break;
		}
	}
	
}

void spacebar(char A[])
{
	int n = 0;

	if (A[0] == 32) {	//ó�� ���� ���� ���ֱ�
		for (int i = 0; i < 60; ++i)
		{
			A[i] = A[i + 1];
		}
	}

	while (A[n] != 46 && A[n] != NULL)
	{
		if (A[n] == 32 && A[n + 1] == 46) {//���� ���� ��ħǥ�� ������ ���� ���ֱ�
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