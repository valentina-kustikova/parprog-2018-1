#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <omp.h>
#include <ctime>
#include <iostream>
void QuickSort(double* arr, int size);
int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "'please, enter before_code [input.bin] [output.bin] '" << std::endl;
		return 1;
	}
	int size;
	freopen(argv[1], "rb", stdin);
	freopen(argv[2], "wb", stdout);
	fread(&size, sizeof(size), 1, stdin);
	double *arr = new double[size];
	fread(arr, sizeof(*arr), size, stdin);
	double time = omp_get_wtime();
	QuickSort(arr, size);
	time = omp_get_wtime() - time;
	fwrite(&time, sizeof(time), 1, stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(arr, sizeof(*arr), size, stdout);
	fclose(stdin);
	fclose(stdout);
	delete arr;
	return 0;
}

