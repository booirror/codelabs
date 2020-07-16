#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>

using namespace std;

int key[] = { 1, 2, 9, 4, 5, 8, 7 };

int saveFile(const char* data, unsigned int len, const char *filename)
{
	FILE* pf = fopen(filename, "wb");
	if (pf == nullptr) {
		return -1;
	}

	fwrite(data, sizeof(char), len, pf);
	fclose(pf);
	return 0;
}

const char* loadFile(const char* filename, unsigned int *len)
{
	FILE *pfile = fopen(filename, "rb");
	if (pfile == nullptr) {
		return nullptr;
	}
	fseek(pfile, 0, SEEK_END);
	long size = ftell(pfile);
	rewind(pfile);

	char * buffer = nullptr;
	buffer = (char*)malloc(sizeof(char)*size);

	size_t result = fread(buffer, sizeof(char), size, pfile);
	if (result != size) {
		printf("read error %s", filename);
		return nullptr;
	}
	fclose(pfile);
	*len = size;
	return buffer;
}


void encryption(char* c, int len, int key[]) {
	for (int i = 0; i < len; i++) {
		c[i] = c[i] ^ key[i % 7];
	}
}
void decode(char* c, int len, int key[]) {
	for (int i = 0; i < len; i++) {
		c[i] = c[i] ^ key[i % 7];
	}
}

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		const char* filename = argv[2];
		char *output = (char*)malloc(strlen(filename) + 6);

		unsigned int filelen = 0;
		char* data = (char* )loadFile(filename, &filelen);

		if (strcmp(argv[1], "en")==0) {
			memcpy(output, filename, strlen(filename));
			memcpy(output + strlen(filename), ".en", 4);
			encryption(data, filelen, key);
		}
		else if (strcmp(argv[1], "de") == 0) {
			memcpy(output, filename, strlen(filename));
			memcpy(output + strlen(filename), ".zip", 5);
			decode(data, filelen, key);
		} else {
			printf("enter en/de\n");
		}
		if (data != NULL) {
			saveFile(data, filelen, output);
			free(data);
		}
		if (output != NULL) {
			free(output);
		}
		printf("ok\r\n");
	}
	else {
		printf("firecode en/de inputfile \r\n");
	}
	
	return 0;
}
