// Deserializer.cpp: определяет точку входа для консольного приложения.
//
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void Deserializer(char* txt, char* bin) {
	freopen(bin, "rb", stdin);
	int n;
	// Считываем размерность 
	fread(&n, sizeof(n), 1, stdin);
	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);
	FILE *file;
	if ((file = fopen(txt, "w")) == NULL) {
		printf("Невозможно открыть файл\n");
		exit(1);
	};
	for (int i = 0; i<n; i++) {
		fprintf(file, "%f\n", arr[i]);
	};
	fclose(file);
}
int main(int argc, char* argv[])
{
	if (argc < 3) {
		return 1;
	}
	char* txt = argv[1];
	char* bin = argv[2];
	Deserializer(txt, bin);
	return 0;
}

