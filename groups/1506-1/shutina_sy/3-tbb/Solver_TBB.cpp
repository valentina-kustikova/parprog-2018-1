#define _CRT_SECURE_NO_WARNINGS

#include <omp.h>
#include"Parallel_Sort.h"
#include"iostream"
using namespace std;

void TBB_Parallel(double *arr, int n, int nThreads)
{
	int part = n / nThreads;
	if (n%nThreads != 0)
		part++;

	Parallel_Sort& sorter = *new(task::allocate_root())Parallel_Sort(arr, n, part);
	task::spawn_root_and_wait(sorter);
}

int main(int argc, char* argv[]) {
	int nThreads;

	if (argc > 3) {
		nThreads = atoi(argv[1]);
		freopen(argv[2], "rb", stdin);
		freopen(argv[3], "wb", stdout);
	}
	else {
		if (argc > 1) {
			nThreads = atoi(argv[1]);
			freopen("array.in", "rb", stdin);
			freopen("array.out", "wb", stdout);
		}
		else return 1;
	}

	int n;
	fread(&n, sizeof(n), 1, stdin);

	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);

	double start_time = omp_get_wtime();
	TBB_Parallel(arr, n, nThreads);
	double time = omp_get_wtime() - start_time;

	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(arr, sizeof(*arr), n, stdout);
	fwrite(&time, sizeof(time), 1, stdout);

	delete[] arr;

	return 0;
}

