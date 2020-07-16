#include <stdio.h>

void bubble_sort(int arr[], int n)
{
	int i, j, tmp = 0;
	for (i = 0; i < n - 1; i++)
	{
		for (j = i; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1]) {
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

void insertion_sort(int a[], int n)
{
	int i, j, tmp = 0;
	for (i = 1; i < n; i++)
	{
		tmp = a[i];
		for (j = i - 1; j >= 0 && a[j] > tmp; j--)
		{
			a[j + 1] = a[j];
		}
		a[j + 1] = tmp;
	}
}

void selection_sort(int a[], int n)
{
	int i, j, k;
	for (int i = 0; i < n - 1; i++)
	{
		k = i;
		for (j = i + 1; j < n; j++)
		{
			if (a[k] > a[j]) {
				k = j;
			}
		}
		if (k != i) {
			int tmp = a[k];
			a[k] = a[i];
			a[i] = tmp;
		}
	}
}

void printa(int a[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", a[i]);
	}
	printf("\n--------\n");
}

int c_main(int argc, char const* argv[])
{
	char ab[5] = "abc";
	printf("%d", sizeof(ab));

	printf("\n----------------\n");

	int a[] = { 22, 4, 14, 54, 23 };
	bubble_sort(a, 5);
	printa(a, 5);

	int b[] = { 22, 4, 14, 54, 23 };
	insertion_sort(b, 5);
	printa(b, 5);

	int c[] = { 22, 14, 23, 4, 54 };
	selection_sort(c, 5);
	printa(c, 5);

	return 0;
}