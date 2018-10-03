#define _CRT_SECURE_NO_WARNINGS
#include "tbb\task_scheduler_init.h"
#include "tbb\parallel_for.h"
#include "tbb\blocked_range.h"
#include <string.h>
#include <string>
#include <cstdio>
#include "tbb\tbb.h"
#include <iostream>
#include <stdio.h>
#include <omp.h>
using namespace tbb;
using namespace std;

void QuickSort(double* arr, int size)
{
	int i = 0, j = size - 1;
	double pivot = arr[size / 2];

	do {
		while (arr[i] < pivot) i++;
		while (arr[j] > pivot) j--;

		if (i <= j) {
			double tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;

			i++;
			j--;
		}
	} while (i <= j);

	if (j > 0)
		QuickSort(arr, j + 1);
	if (i < size)
		QuickSort(&arr[i], size - i);
}


class Split :public task
{
private:
	double *mas1;
	double *mas2;
	double *tmp;
	int size1;
	int size2;
public:
	Split(double *_mas1, double *_mas2, double *_tmp, int _size1, int _size2)
		: mas1(_mas1), mas2(_mas2), tmp(_tmp), size1(_size1), size2(_size2)
	{
	}

	task* execute()
	{
		int a = 0;
		int b = 0;
		int i = 0;
		while ((a != size1) && (b != size2))
		{//слияние
			if (mas1[a] <= mas2[b])
			{
				tmp[i] = mas1[a];
				a++;
			}
			else
			{
				tmp[i] = mas2[b];
				b++;
			}
			i++;
		}
		if (a == size1)
		{
			int j = b;
			for (; j < size2; j++, i++)
				tmp[i] = mas2[j];
		}
		else
		{
			int j = a;
			for (; j < size1; j++, i++)
				tmp[i] = mas1[j];
		}
		return NULL;
	}
};


class TBBSort : public task { //выполняет сортировку либо части массива либо двух частей а после делает сплит
private:
	double* mas;
	double* tmp;
	int size;
	int blok;

	int threads;

	int BinSearch(double *mas, int l, int r, double x)
	{
		if (l == r)
			return l;
		if (l + 1 == r)
			if (x<mas[l])
				return l;
			else
				return r;
		int m = (l + r) / 2;
		if (x<mas[m])
			r = m;
		else
			if (x>mas[m])
				l = m;
			else
				return m;
		return BinSearch(mas, l, r, x);
	}

public:
	TBBSort(double *_mas, double *_tmp, int _size, int _blok, int _threads) : mas(_mas), tmp(_tmp), size(_size), blok(_blok), threads(_threads) {}

	task* execute()
	{
		if (size <= blok)
		{
			QuickSort(mas, size);
		}
		else
		{
			TBBSort &sorter1 = *new (allocate_child())	TBBSort(mas, tmp, size / 2, blok, threads / 2);
			TBBSort &sorter2 = *new (allocate_child())	TBBSort(mas + size / 2, tmp + size / 2, size - size / 2, blok, threads / 2);
			set_ref_count(3);
			spawn(sorter1);
			spawn_and_wait_for_all(sorter2);
			Split **sp = new Split*[threads - 1];
			int s = size / 2;
			s /= threads;
			int l = 0, r = s;
			int l2 = 0, r2;
			for (int i = 0; i<threads - 1; i++)
			{
				double x = mas[r];
				r2 = BinSearch(mas + size / 2, 0, size - size / 2, x);
				sp[i] = new (allocate_child())
					Split(mas + l, mas + size / 2 + l2,
						tmp + l + l2, r - l, r2 - l2);
				l += s;
				r += s;
				l2 = r2;
			}
			Split &spl = *new (allocate_child())
				Split(mas + l, mas + size / 2 + l2,
					tmp + l + l2, size / 2 - l,
					size - size / 2 - l2);
			set_ref_count(threads + 1);
			for (int i = 0; i<threads - 1; i++)
				spawn(*(sp[i]));
			spawn_and_wait_for_all(spl);
			for (int i = 0; i<size; i++)
				mas[i] = tmp[i];
			delete[] sp;
		}
		return NULL;
	}

};

void ParallelSort(double *inp, int size, int nThreads)
{// создаѐт корневой task, начиная с которого будет разворачиваться рекурсия
	double *out = new double[size];
	int blok = size / nThreads;
	if (size%nThreads != 0)
		blok++;
	TBBSort& sorter = *new (task::allocate_root())	TBBSort(inp, out, size, blok, nThreads);
	task::spawn_root_and_wait(sorter);
	delete[] out;
}