#define _CRT_SECURE_NO_WARNINGS
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <iostream>
#include <stdio.h>  
#include<cstdlib>
#include <ctime>
#include <chrono>
#include <random>
#include <omp.h>
using namespace tbb;

void QuickSort(double* arr, int size)
{
	int left = 0;
	int right = size - 1;
	double middle = arr[size / 2];
	do {
		while (arr[left] < middle)
			left++;
		while (arr[right] > middle)
			right--;

		if (left <= right) {
			double tmp = arr[left];
			arr[left] = arr[right];
			arr[right] = tmp;
			left++;
			right--;
		}
	} while (left <= right);
	if (right > 0)
		QuickSort(arr, right + 1);
	if (left < size)
		QuickSort(&arr[left], size - left);
}

void simple_merge(double* a, int left, int right, int mid)
{
	double *res = new double[right - left];
	double *res1 = new double[mid];
	double *res2 = new double[right - left - mid];
	for (int i = 0; i < mid; i++)
	{
		res1[i] = a[i + left];
	}
	for (int j = 0; j < right - left - mid; j++)
	{
		res2[j] = a[left + mid + j];
	}

	int i = 0, j = 0;
	while (i < mid && j < right - left - mid)
	{
		if (res1[i] <= res2[j])
		{
			res[i + j] = res1[i];
			i++;
		}
		else
		{
			res[i + j] = res2[j];
			j++;
		}
	}
	while (i < mid)
	{
		res[i + right - left - mid] = res1[i];
		i++;
	}
	while (j < right - left - mid)
	{
		res[j + mid] = res2[j];
		j++;
	}
	for (int i = 0; i < right - left; i++)
	{
		a[i + left] = res[i];
	}
	delete res;
	delete res1;
	delete res2;
}

void merge(double* a, int n, int parts, int thread_count1, int my_con)
{
	int glob_parts = parts / 2;
	int left, rigth, midl, midr;
	task_scheduler_init init(thread_count1);
	parallel_for(tbb::blocked_range<int>(0, thread_count1),
	[&parts, &left, &rigth, &thread_count1, &n, &a, &glob_parts, &my_con](const blocked_range<int> &r) {
		for (int i = 0; i < thread_count1; ++i)
		{
			left = i *((n - n%parts) / glob_parts);
			if (i == (thread_count1 - 1))
			{
				rigth = n;
			}
			else 
			{
				rigth = left + ((n - n%parts) / glob_parts);
			}
			simple_merge(a, left, rigth, my_con);
		}
	});
}

void tbb_QuickSort(double *arr, int size, int count_threads)
{
	task_scheduler_init init(count_threads);
	int part = size / count_threads; // 
	parallel_for(tbb::blocked_range<int>(0, count_threads),
		[&part, &count_threads, &size, &arr](const blocked_range<int> &r) {
		for (int i = 0; i < count_threads; i++) {
			if (i != count_threads - 1)
			{
				QuickSort(arr + i* part, part);			
			}
			else
				QuickSort(arr + i *part, size - part *i);
			}
	});
	//merge
	int parts = count_threads;
	int m = (int)(round((double)log(count_threads) / log(2.0))); 
	int mid= size / parts;
	for (int i = 0; i < m; i++) {
		count_threads /= 2;
		merge(arr, size, parts, count_threads, mid);
		mid *= 2;
		parts /= 2;
	}
}
int main(int argc, char ** argv)
{
	freopen(argv[2], "rb", stdin);
	int size;
	int count_divene = atoi(argv[1]);
	double time;
	fread(&size, sizeof(size), 1, stdin);
	double* arr = new double[size];
	fread(arr, sizeof(*arr), size, stdin);
	time = omp_get_wtime();
	tbb_QuickSort(arr, size, count_divene);
	time = omp_get_wtime() - time;
	printf("TBB time: %f\n", time);
	freopen(argv[3], "wb", stdout);
	fwrite(&time, sizeof(time), 1, stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(arr, sizeof(*arr), size, stdout);
	return 0;
}