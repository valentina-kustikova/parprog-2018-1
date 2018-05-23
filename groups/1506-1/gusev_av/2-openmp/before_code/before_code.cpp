#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <omp.h>
#include <ctime>
#include <iostream>
void QuickSort(double* arr, int size);
void Merge(double *arr1, double *arr2, int arr1_size, int arr2_size, double* res);
int main(int argc, char * argv[])
{
	if (argc != 4)
	{
		std::cout << "'please, enter before_code [num_threads] [input.bin] [output.bin] '" << std::endl;
		return 1;
	}
	int threads = atoi(argv[1]);
	int size;
	freopen(argv[2], "rb", stdin);

	fread(&size, sizeof(size), 1, stdin);
	double *arr = new double[size];
	fread(arr, sizeof(*arr), size, stdin);

	double time = omp_get_wtime();

	int iter;
	double ** res = new double*[threads];
	int *shift = new int[threads], *chunk = new int[threads];
#pragma omp parallel shared(arr, iter, shift, chunk, res) num_threads(threads)
	{
		int cur_thread = omp_get_thread_num();
		int index;
		if (cur_thread == threads - 1)
		{
			shift[cur_thread] = cur_thread*(size / threads);
			chunk[cur_thread] = size - cur_thread *(size / threads);
			res[cur_thread] = new double [chunk[cur_thread]];
			QuickSort(arr + shift[cur_thread], chunk[cur_thread]);
		}
		else
		{
			shift[cur_thread] = cur_thread*(size / threads);
			chunk[cur_thread] = size/ threads;
			res[cur_thread] = new double[chunk[cur_thread]];
			QuickSort(arr + shift[cur_thread], chunk[cur_thread]);
		}
#pragma omp barrier
		iter = 1;
		while (iter < threads)
		{
			index = (int)pow(2, iter - 1);
			for (int i = 0; i < chunk[cur_thread]; i++)
			{
				res[cur_thread][i] = (arr[shift[cur_thread] + i]);
			}
#pragma omp barrier
		if (cur_thread % (index * 2) == 0) {
			Merge(res[cur_thread], res[cur_thread + index], chunk[cur_thread], chunk[cur_thread +index], &arr[shift[cur_thread]]);
		}
#pragma omp single
		{
			iter *= 2;
		}
#pragma omp barrier
	}
}
		
		time = omp_get_wtime() - time;	
		freopen(argv[3], "wb", stdout);
		fwrite(&time, sizeof(time), 1, stdout);
		fwrite(&size, sizeof(size), 1, stdout);
		fwrite(arr, sizeof(*arr), size, stdout);
		fclose(stdin);
		fclose(stdout);
		delete arr;
		return 0;
	}