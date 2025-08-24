#include<stdio.h>
#pragma warning(disable:4996)

int LeapYear(int);
void Calendar(int, int);
void ThirteenFriday(int, int, int);

int computeDays(int year)//서기 1년1월1일부터 year년도 1월 1일까지 몇일이 지났는가
{
	int days = 0;
	for (int i = 1; i < year; i++)
		if (LeapYear(i))
			days += 2;
		else
			days += 1;

	return days;
}

int main(void)
{
	Calendar(2021, 3);

	char char_year[4+1] = {0};
	int year = 0;
	int month = 0;
	int day = 0;
	int yoil = 1;

	int a;

	while (1)
	{
		yoil = 1;

		printf("날짜 입력(13일의 금요일 F/f) : ");
		scanf("%c%c", &char_year[0], &char_year[1]);
		if (char_year[0] != 'f' && char_year[1] != ' ' && char_year[0] != 'F' && char_year[1] != ' ') {
			scanf("%c%c/%d/%d", &char_year[2], &char_year[3], &month, &day);
			while (getchar() != '\n');
		}

		if (char_year[0] == 'f' || char_year[0] == 'F') {
			//printf("2020-03-13 2020-11-13 2021-08-13 2022-05-13 2023-01-13 2023-10-13\n2024-09-13 2024-12-13 2025-06-13 2026-02-13 2026-03-13 2026-11-13\n2027-08-13 2028-10-13 2029-04-13 2029-07-13 2030-09-13 2030-12-13\n\n");
			for (int i = 2020; i <= 2030; ++i) {
				for (int j = 1; j <= 12; ++j) {
					ThirteenFriday(i, j, 13);
				}
				printf("\n");
			}
		}
		else {
			//문자열 -> 숫자
			year = ChartoNum(char_year[0]) * 1000 + ChartoNum(char_year[1]) * 100 + ChartoNum(char_year[2]) * 10 + ChartoNum(char_year[3]);

			yoil = (yoil + computeDays(year)) % 7;

			switch (month)
			{
			case 1:
				break;
			case 2:
				yoil = (yoil + 31) % 7; break;
			case 3:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7; break;
				}
				else {
					yoil = (yoil + 28) % 7; break;
				}
			case 4:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7; break;
			case 5:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7; break;
			case 6:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7; break;
			case 7:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7; break;
			case 8:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7; break;
			case 9:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 31) % 7; break;
			case 10:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7; break;
			case 11:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7; break;
			case 12:
				yoil = (yoil + 31) % 7;
				if (LeapYear(year)) {
					yoil = (yoil + 29) % 7;
				}
				else {
					yoil = (yoil + 28) % 7;
				}
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7;
				yoil = (yoil + 31) % 7;
				yoil = (yoil + 30) % 7; break;
			}

			yoil = (yoil + day - 1) % 7;

			switch (yoil){
			case 0:
				printf("%d/%d/%d: 일요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			case 1:
				printf("%d/%d/%d: 월요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			case 2:
				printf("%d/%d/%d: 화요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			case 3:
				printf("%d/%d/%d: 수요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			case 4:
				printf("%d/%d/%d: 목요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			case 5:
				printf("%d/%d/%d: 금요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			case 6:
				printf("%d/%d/%d: 토요일\n\n", year, month, day);
				Calendar(year, month);
				break;
			}
		}
	}

	return 0;
}

/*윤년계산*/
int LeapYear(int a)
{
	if ((a % 4 == 0 && 0 != a % 100) || 0 == a % 400)
	{
		return 1;
	}
	return 0;
}

void ThirteenFriday(int year,int month,int day) {
	int yoil = 1;

	yoil = (yoil + computeDays(year)) % 7;

	switch (month)
	{
	case 1:
		break;
	case 2:
		yoil = (yoil + 31) % 7; break;
	case 3:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7; break;
		}
		else {
			yoil = (yoil + 28) % 7; break;
		}
	case 4:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7; break;
	case 5:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7; break;
	case 6:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7; break;
	case 7:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7; break;
	case 8:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7; break;
	case 9:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 31) % 7; break;
	case 10:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7; break;
	case 11:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7; break;
	case 12:
		yoil = (yoil + 31) % 7;
		if (LeapYear(year)) {
			yoil = (yoil + 29) % 7;
		}
		else {
			yoil = (yoil + 28) % 7;
		}
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7;
		yoil = (yoil + 31) % 7;
		yoil = (yoil + 30) % 7; break;
	}

	yoil = (yoil + day - 1) % 7;

	if(yoil == 5)
		printf("%d-%d-%d ", year, month, day);
}

void Calendar(int year, int month)
{
	int leapYearCnt = 0;
	int start_month;	//
	int last_day;

	for (int i = 1; i < year; i++)
	{
		leapYearCnt += LeapYear(i);
	}

	switch (month) {
	case 1:
		start_month = (year * 365 + leapYearCnt) % 7; break;
	case 2:
		start_month = (year * 365 + leapYearCnt + 31) % 7; break;
	case 3:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year))) % 7; break;
	case 4:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31) % 7; break;
	case 5:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30) % 7; break;
	case 6:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31) % 7; break;
	case 7:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31 + 30) % 7; break;
	case 8:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31 + 30 + 31) % 7; break;
	case 9:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31 + 30 + 31 + 31) % 7; break;
	case 10:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31 + 30 + 31 + 31 + 30) % 7; break;
	case 11:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) % 7; break;
	case 12:
		start_month = (year * 365 + leapYearCnt + 31 + (28 + LeapYear(year)) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) % 7; break;
	}

	switch (month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		last_day = 31;  break;
	case 4:
	case 6:
	case 9:
	case 11:
		last_day = 30;  break;
	case 2:
		if (LeapYear(year) == 1)
			last_day = 29;
		else
			last_day = 28;
		break;
	}

	printf("        %d월        \n\n", month);
	printf("일 월 화 수 목 금 토\n");
	for (int i = start_month; i > 0; --i)//달력 공백
	{
		printf("   ");
	}

	for (int t = 1; t <= 7 - start_month; t++) {//공백줄 날짜(공백으로 바뀌는 띄어 쓰기 때문에 따로 출력)
		printf("%2d ", t);
	}
	printf("\n");

	for (int j = 0; j < 5; j++) {
		for (int t = 0; t < 7; t++) {
			if (7 - start_month + t + 1 + j * 7 <= last_day)
				printf("%2d ", 7 - start_month + t + 1 + j * 7);
		}
		printf("\n");
	}

}

int ChartoNum(char ch)
{
	int num;

	if (ch == '1') {
		num = 1;
		return num;
	}
	else if (ch == '2') {
		num = 2;
		return num;
	}
	else if (ch == '3') {
		num = 3;
		return num;
	}
	else if (ch == '4') {
		num = 4;
		return num;
	}
	else if (ch == '5') {
		num = 5;
		return num;
	}
	else if (ch == '6') {
		num = 6;
		return num;
	}
	else if (ch == '7') {
		num = 7;
		return num;
	}
	else if (ch == '8') {
		num = 8;
		return num;
	}
	else if (ch == '9') {
		num = 9;
		return num;
	}
	else if (ch == '0') {
		num = 0;
		return num;
	}
}