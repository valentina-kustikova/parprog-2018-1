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
	double **arr = new double*[n];
	for (int i = 0; i < n; i++) {
		arr[i] = new double[n];		
		fread(arr[i], sizeof(*arr[i]), n, stdin);
	}
	// Открываем текстовый файл
	FILE *file;
	if ((file = fopen(txt, "w")) == NULL) {
		printf("Невозможно открыть файл\n");
		exit(1);
	}

	// Записываем матрицу А в текст. файл
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			fprintf(file, "%8.4f  ", arr[i][j]);
		fprintf(file, "\n");
	}

	// Записываем матрицу b в текст. файл
	double *barr = new double[n];
	fread(barr, sizeof(*barr), n, stdin);
	for (int i = 0; i < n; i++)
		fprintf(file, "%8.4f  ", barr[i]);
	fclose(file);
}

void outDeserializer(char* txt, char* bin) {
	freopen(bin, "rb", stdin);
	int n;
	// Считываем размерность
	fread(&n, sizeof(n), 1, stdin);
	double *arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);
	
	// Открываем текстовый файл
	FILE *file;
	if ((file = fopen(txt, "w")) == NULL) {
		printf("Невозможно открыть файл\n");
		exit(1);
	}

	// Записываем вектор х в текст. файл
	for (int i = 0; i < n; i++) {		
			fprintf(file, "%8.4f ", arr[i]);
	}

	// Записываем время в текст. файл
	double time;
	fread(&time, sizeof(time), 1, stdin);
	fprintf(file, "%8.4f  ", time);
	fclose(file);
}

int main(int argc, char* argv[])
//int main()
{	
	bool out = false; // txt file for .out

	if (argc < 3) {
	return 1;
	}

	char* bin = argv[1];
	char* txt = argv[2];

	if (argc > 3){
		out = true;
	}
	//char* bin = "array.in";
	//char* txt = "generation_rezult.txt";

	if (out) outDeserializer(txt, bin);
	else Deserializer(txt, bin);

	return 0;
}
