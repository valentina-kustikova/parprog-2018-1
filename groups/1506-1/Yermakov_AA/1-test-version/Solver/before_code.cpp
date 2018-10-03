#define _CRT_SECURE_NO_WARNINGS
#include <cstdio> 
#include <random>
#include <omp.h>
#include <iostream>

void quicksort(double* arr, int size);

int main(int argc, char * argv[])
{
	if (argc != 3) {
		std::cout << "You input incorrect number of arguments. Input 'solver input.bin output.bin'" << std::endl;
		return 1;
	}

	int size;
	double* array;
	double _time;


	freopen(argv[1], "rb", stdin);
	freopen(argv[2], "wb", stdout);

	fread(&_time, sizeof(_time), 1, stdin); 
	fread(&size, sizeof(size), 1, stdin);

	array = new double[size];
	fread(array, sizeof(*array), size, stdin);
	
	double time = omp_get_wtime();
	quicksort(array, size);
	time = omp_get_wtime() - time;

	fwrite(&time, sizeof(time), 1, stdout);
	fwrite(&size, sizeof(size), 1, stdout);

	fwrite(array, sizeof(*array), size, stdout);

	std::cerr << "Time " << time << std::endl;
	delete[] array;
	return 0;
}