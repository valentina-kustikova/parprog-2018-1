#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <omp.h>
#include <ctime>
#include <iostream>
void QuickSort(int* arr, int size);
int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "'please, enter typer [input.bin] [output.bin] '" << std::endl;
		return 1;
	}
	int size;
	int *arr;	
	freopen(argv[1], "rb", stdin);
	freopen(argv[2], "wb", stdout);
	fread(&size, sizeof(size), 1, stdin);
	arr = new int[size];
	fread(arr, sizeof(*arr), size, stdin);
	double time = omp_get_wtime();
	QuickSort(arr, size);
	time = omp_get_wtime() - time;
	fwrite(&time, sizeof(time), 1, stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(arr, sizeof(*arr), size, stdout);

	return 0;
}

