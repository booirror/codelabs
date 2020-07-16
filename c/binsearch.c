#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#define max 8
//#pragma warning(disable:4996)
static char* reservedWords[max] = {
	"else", "end", "if", 
	"read", "repeat", 
	"then", "until", "write"
};

int binary_search(char* words[], const char* word)
{
	int start = 0;
	int end = max - 1;
	int mid;
	while (start <= end) {
		mid = start + end;
		int val = strcmp(words[mid], word);
		if (val < 0) {
			start = mid + 1;
		}
		else if (val > 0) {
			end = mid - 1;
		}
		else {
			return mid;
		}
	}
	return -1;
}

int main()
{
	while (1) {
		char buff[128];

		scanf("%s", buff);
		if (strcmp(buff, "q") == 0) break;
		int v = binary_search(reservedWords, buff);
		if (v < 0) {
			printf("not found: %s\n", buff);
		}
		else {
			printf("words[%d]: %s\n", v, reservedWords[v]);
		}
	}
	return 0;
}