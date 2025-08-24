#include <stdio.h>
#pragma warning(disable:4996)

struct Client {
	char name[5];
	int num;
	int bir[2];
	int attend[6];
	int attend_sum;
};

void Input(struct Client* cl, char name[5], int num, int bir_m, int bir_d, int attend_0, int attend_1, int attend_2, int attend_3, int attend_4, int attend_5)
{
	strcpy(cl->name, name);
	cl->num = num;
	cl->bir[0] = bir_m;
	cl->bir[1] = bir_d;
	cl->attend[0] = attend_0;
	cl->attend[1] = attend_1;
	cl->attend[2] = attend_2;
	cl->attend[3] = attend_3;
	cl->attend[4] = attend_4;
	cl->attend[5] = attend_5;
}

void Copy(struct Client* cl, struct Client _cl)
{
	strcpy(cl->name, _cl.name);
	cl->num = _cl.num;
	cl->bir[0] = _cl.bir[0];
	cl->bir[1] = _cl.bir[1];
	cl->attend[0] = _cl.attend[0];
	cl->attend[1] = _cl.attend[1];
	cl->attend[2] = _cl.attend[2];
	cl->attend[3] = _cl.attend[3];
	cl->attend[4] = _cl.attend[4];
	cl->attend[5] = _cl.attend[5];
}

void Print(struct Client cl)
{
	printf("이름: %s, 회원번호: %d, 생일: %02d/%02d 1월: %d, 2월: %d, 3월: %d, 4월: %d, 5월: %d, 6월: %d\n"
		, cl.name, cl.num, cl.bir[0], cl.bir[1], cl.attend[0], cl.attend[1], cl.attend[2], cl.attend[3], cl.attend[4], cl.attend[5]);
}

void SortByName(struct Client cl[15])
{
	struct Client temp = { 0, };

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15 - i - 1; j++) {
			if (strcmp(cl[j].name, cl[j + 1].name) > 0 && strcmp(cl[j].name, "") != 0 && strcmp(cl[j + 1].name, "") != 0) {
				Copy(&temp, cl[j]);
				Copy(&cl[j], cl[j + 1]);
				Copy(&cl[j + 1], temp);
			}
		}
	}
}

void SortByNum(struct Client cl[15]) 
{
	struct Client temp = { 0, };

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15 - i - 1; j++) {
			if (cl[j].num > cl[j + 1].num && strcmp(cl[j].name, "") != 0 && strcmp(cl[j + 1].name, "") != 0) {
				/*Copy(&temp, cl[j]);
				Copy(&cl[j], cl[j + 1]);
				Copy(&cl[j + 1], temp);*/
				
				temp = cl[j];
				cl[j] = cl[j + 1];
				cl[j + 1] = temp;
			}
		}
	}
}

void SortByBirth(struct Client cl[15])
{
	struct Client temp = { 0, };

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15 - i - 1; j++) {
			if (strcmp(cl[j].name, "") != 0 && strcmp(cl[j + 1].name, "") != 0) {
				if (cl[j].bir[0] > cl[j + 1].bir[0]) {
					Copy(&temp, cl[j]);
					Copy(&cl[j], cl[j + 1]);
					Copy(&cl[j + 1], temp);
				}
				else if (cl[j].bir[0] == cl[j + 1].bir[0] && cl[j].bir[1] > cl[j + 1].bir[1]) {
					Copy(&temp, cl[j]);
					Copy(&cl[j], cl[j + 1]);
					Copy(&cl[j + 1], temp);
				}
			}
		}
	}
}


int main()
{
	struct Client cl[15] = { 0, };

	char command;

	char t_name[50] = "";
	int t_num[9] = { 0, };

	_Bool flag = 0;

	int cl_count = 3;

	int max, min;
	int temp = 0;

	Input(&cl[0], "Lee", 323, 1, 22, 5, 9, 15, 20, 25, 30);
	Input(&cl[1], "Song", 123, 3, 2, 10, 10, 15, 5, 25, 20);
	Input(&cl[2], "Park", 325, 9, 9, 1, 2, 3, 4, 5, 6);
	
	printf("미리 입력된 회원 정보들\n");
	Print(cl[0]);
	Print(cl[1]);
	Print(cl[2]);

	while (1) {
		printf("\n명령어를 입력해주세요: ");
		scanf("%c", &command);
		while (getchar() != '\n');

		switch (command) {
		case 'a':
		case 'A':
			printf("새로 입력할 회원 데이터를 적어주세요.\n");

			printf("이름(영문 최대 5글자): ");
			scanf("%s", t_name);
			while (getchar() != '\n');
			if (strlen(t_name) > 5) {
				printf("에러: 이름은 최대 5글자까지 입력가능합니다. 다시 입력해주세요.\n");
				continue;
			}

			printf("회원번호(최대 3자리 자연수): ");
			scanf("%d", &t_num[0]);
			while (getchar() != '\n');
			if (t_num[0] > 999 || t_num[0] < 0) {
				printf("에러: 회원번호는 3자리 이하 자연수이어야 합니다. 다시 입력해주세요.\n");
				continue;
			}

			printf("생일(**/**형식): ");
			scanf("%d/%d", &t_num[1], &t_num[2]);
			while (getchar() != '\n');
			if (t_num[1] > 12 || t_num[1] < 0 || t_num[2] > 31 || t_num[2] < 0) {
				flag = 1;
			}
			else if (t_num[1] == 4 && t_num[2] > 30 || t_num[1] == 6 && t_num[2] > 30 || t_num[1] == 9 && t_num[2] > 30 || t_num[1] == 11 && t_num[2] > 30) {
				flag = 1;
			}
			else if (t_num[1] == 2 && t_num[2] > 28) {
				flag = 1;
			}

			if (flag) {
				printf("에러: 생일 형식이 맞지 않습니다. 다시 입력해주세요.\n");
				flag = 0;
				continue;
			}

			printf("1~6월 출석횟수(*/*/*/*/*/*형식): ");
			scanf("%d/%d/%d/%d/%d/%d", &t_num[3], &t_num[4], &t_num[5], &t_num[6], &t_num[7], &t_num[8]);
			while (getchar() != '\n');
			for (int n = 3; n < 9; n++) {
				if (t_num[n] > 31 || t_num[n] < 0) {
					flag = 1;
				}
				else if (t_num[4] > 28) {
					flag = 1;
				}
				else if (t_num[6] > 30 || t_num[8] > 30) {
					flag = 1;
				}
			}

			if (flag) {
				printf("에러: 출석 횟수의 형식이 맞지 않습니다. 다시 입력해주세요.\n");
				flag = 0;
				continue;
			}

			Input(&cl[cl_count], t_name, t_num[0], t_num[1], t_num[2], t_num[3], t_num[4], t_num[5], t_num[6], t_num[7], t_num[8]);
			cl_count++;
			break;

		case 'p':
		case 'P':
			for (int n = 0; n < cl_count; n++) {
				Print(cl[n]);
			}
			break;

		case 'm':
		case 'M':
			for (int n = 0; n < cl_count; n++) {
				cl[n].attend_sum = 0;
				for (int m = 0; m < 6; m++) {
					cl[n].attend_sum += cl[n].attend[m];
				}
			}
			max = cl[0].attend_sum;

			for (int n = 0; n < cl_count; n++) {
				if (max < cl[n].attend_sum) {
					max = cl[n].attend_sum;
					temp = n;
				}
			}

			printf("출석수가 가장 많은 모범회원의 정보\n");
			Print(cl[temp]);
			break;

		case 'n':
		case 'N':
			for (int n = 0; n < cl_count; n++) {
				cl[n].attend_sum = 0;
				for (int m = 0; m < 6; m++) {
					cl[n].attend_sum += cl[n].attend[m];
				}
			}
			min = cl[0].attend_sum;

			for (int n = 0; n < cl_count; n++) {
				if (min > cl[n].attend_sum) {
					min = cl[n].attend_sum;
					temp = n;
				}
			}

			printf("출석수가 가장 적은 불량회원의 정보\n");
			Print(cl[temp]);
			break;

		case '1':
			max = cl[0].attend[0];

			for (int n = 0; n < cl_count; n++) {
				if (max < cl[n].attend[0]) {
					max = cl[n].attend[0];
					temp = n;
				}
			}

			printf("1월 출석수가 가장 많은 회원의 정보\n");
			Print(cl[temp]);
			break;

		case '2':
			max = cl[0].attend[1];

			for (int n = 0; n < cl_count; n++) {
				if (max < cl[n].attend[1]) {
					max = cl[n].attend[1];
					temp = n;
				}
			}

			printf("2월 출석수가 가장 많은 회원의 정보\n");
			Print(cl[temp]);
			break;

		case '3':
			max = cl[0].attend[2];

			for (int n = 0; n < cl_count; n++) {
				if (max < cl[n].attend[2]) {
					max = cl[n].attend[2];
					temp = n;
				}
			}

			printf("3월 출석수가 가장 많은 회원의 정보\n");
			Print(cl[temp]);
			break;

		case 'q':
		case 'Q':
			exit(0);
			break;

		case 'i':
		case 'I':
			SortByName(cl);
			printf("회원 이름에 따라 오름차순 정렬 완료\n");
			break;

		case 'j':
		case 'J':
			SortByNum(cl);
			printf("회원 번호에 따라 오름차순 정렬 완료\n");
			break;

		case 'k':
		case 'K':
			SortByBirth(cl);
			printf("회원 생일에 따라 오름차순 정렬 완료\n");
			break;

		default:
			printf("에러: 알 수 없는 명령어\n");
			break;
		}

	}

}