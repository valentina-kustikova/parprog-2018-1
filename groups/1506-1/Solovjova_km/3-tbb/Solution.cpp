#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <omp.h>
#include<conio.h>
#include<string>
#include <ctime>
#include <sstream>
#include <tbb\task.h>
#include <tbb\blocked_range.h>
#include <tbb\parallel_for.h>

using namespace std;
using namespace tbb;


double valueSum = 0;
class ApplyFoo {
	double* const my_a;
	//double valueSum1;
public:
	ApplyFoo(double* functionValues) : my_a(functionValues) {}
	void operator()(const blocked_range<size_t>& range) const {
		double* functionValues = my_a;
		//double valueSum = valueSum1;
		for (size_t i = range.begin(); i != range.end(); i++)
			valueSum += functionValues[i];
	}
};

void ParallelApplyFoo(double functionValues[], size_t n) {
	parallel_for(blocked_range<size_t>(0, n),
		ApplyFoo(functionValues), auto_partitioner());
}

double MonteCarloTBB(double *functionValues, int N, double leftBorderX, double leftBorderY, double rightBorderX, double rightBorderY)
{
	double average = 0;
	ParallelApplyFoo(functionValues, N);
	average = valueSum / N;
	auto square = (rightBorderX - leftBorderX)*(rightBorderY - leftBorderY);
	return average * square;
}

