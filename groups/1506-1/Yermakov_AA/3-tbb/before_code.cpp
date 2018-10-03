#define _CRT_SECURE_NO_WARNINGS
#include "tbb\task_scheduler_init.h"
#include "tbb\parallel_for.h"
#include "tbb\blocked_range.h"
#include <cstdio>
#include "tbb\tbb.h"
#include <omp.h>
using namespace tbb;
using namespace std;



void ParallelSort(double *inp, int size, int nThreads);

int main(int argc, char * argv[])
{
	if (argc != 4) {
		puts("Error! You should input 'TbbSolver [num_threads] input.bin output.bin'");
		return 1;
	}

	int size, threads = atoi(argv[1]);
	double* arr;
	double fict_time;
	
	freopen(argv[2], "rb", stdin);

	fread(&fict_time, sizeof(fict_time), 1, stdin);
	fread(&size, sizeof(size), 1, stdin);

	arr = new double[size];

	fread(arr, sizeof(*arr), size, stdin);

	double time = omp_get_wtime();

	ParallelSort(arr, size, threads);

	time = omp_get_wtime() - time;

	printf("TBB time: %f\n", time);

	freopen(argv[3], "wb", stdout);
	fwrite(&time, sizeof(time), 1, stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(arr, sizeof(*arr), size, stdout);

	return 0;

}