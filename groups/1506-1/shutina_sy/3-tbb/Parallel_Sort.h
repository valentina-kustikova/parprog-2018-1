#pragma once

#include "Merger.h"
#include "Ñomparison.h"

class Parallel_Sort :public task
{
private:
	double* arr;
	int n;
	int part;
public:
	Parallel_Sort(double* p_arr, int p_n, int p_part) : arr(p_arr), n(p_n), part(p_part) {}
	task* execute() {

		if (n <= part) {
			doubleRadixSort(arr, n);
		}
		else {
			int p = n / 2 + (n / 2) % 2;

			Parallel_Sort& sorter1 = *new(allocate_child()) Parallel_Sort(arr, p, part);
			Parallel_Sort& sorter2 = *new(allocate_child()) Parallel_Sort(arr + p, n - p, part);

			set_ref_count(3);

			spawn(sorter1);
			spawn_and_wait_for_all(sorter2);

			Merger& merger1 = *new(allocate_child()) Merger(EVEN, arr, p, n - p);
			Merger& merger2 = *new(allocate_child()) Merger(ODD, arr, p, n - p);

			set_ref_count(3);

			spawn(merger1);
			spawn_and_wait_for_all(merger2);

			parallel_for(blocked_range<int>(1, (n + 1) / 2), Ñomparison(arr));

		}
		return NULL;
	}

	void radixPass(int num_byte, long n, double *source, double *dest, int *count);
	void floatRadixLastPass(int num_byte, int n, double *source, double *dest, int *count);
	void createCounters(double *data, int *counters, int n);
	void doubleRadixSort(double* mas, int n);
};

