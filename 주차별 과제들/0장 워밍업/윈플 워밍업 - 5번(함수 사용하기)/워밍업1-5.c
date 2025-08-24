#include <stdio.h>
#pragma warning(disable:4996)

float Calculator(float a[], char c[], float r[], float result)
{
	for (int i = 0; i < 4; i++)
	{
		if (c[i] == '*')
		{
			r[i] = a[i] * a[i + 1];
			if (c[i - 1] == '-')
				result = result + a[i] - r[i];
			else if (c[i - 1] == '+')
				result = result - a[i] + r[i];
			else if (c[i - 1] == '*')
			{
				result = result - r[i - 1] + r[i - 1] * a[i + 1];
				r[i] = r[i - 1] * a[i - 1];
			}
			else if (c[i - 1] == '/') {
				result = result - r[i - 1] + r[i - 1] * a[i + 1];
				r[i] = r[i - 1] * a[i - 1];
			}
			else
				result = result - a[i] + r[i];
		}
		else if (c[i] == '/')
		{
			r[i] = a[i] / a[i + 1];
			if (c[i - 1] == '-')
				result = result + a[i] - r[i];
			else if (c[i - 1] == '+')
				result = result - a[i] + r[i];
			else if (c[i - 1] == '*') {
				result = result - r[i - 1] + r[i - 1] / a[i + 1];
				r[i] = r[i - 1] / a[i - 1];
			}
			else if (c[i - 1] == '/') {
				result = result - r[i - 1] + r[i - 1] / a[i + 1];
				r[i] = r[i - 1] / a[i - 1];
			}
			else
				result = result - a[i] + r[i];
		}
		else if (c[i] == '+')
		{
			result += a[i + 1];
		}
		else if (c[i] == '-')
		{
			result -= a[i + 1];
		}
		//printf("%f\n", result);
	}
	return result;
}

int main() 
{
	float n[5];

	char c[4];

	float r[4] = { 0 };

	float result = 0;

	printf("Input numbers: ");

	scanf("%f%c%f%c%f%c%f%c%f", &n[0], &c[0], &n[1], &c[1], &n[2], &c[2], &n[3], &c[3], &n[4]);
	while (getchar() != '\n');

	result += n[0];

	printf("Result: %.0f %c %.0f %c %.0f %c %.0f %c %.0f = %.0f \n", n[0], c[0], n[1], c[1], n[2], c[2], n[3], c[3], n[4], Calculator(n, c, r, result));

	printf("Press Enter to end. ");
	getchar();
}