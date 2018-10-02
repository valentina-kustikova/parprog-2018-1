#include <omp.h>

double averageCalculation(double *functionValues, int N) {

	double average = 0.0;
	double valuesSum = 0.0;
#pragma omp  for default(shared)
	for (int i = 0; i < N; i++) {
		valuesSum += functionValues[i];
	}
	return valuesSum;
}

double MonteCarloOMP(double *functionValues, double leftBorderX, double leftBorderY, double rightBorderX, double rightBorderY, int N)
{
	double average = averageCalculation(functionValues, N);
	double square = (rightBorderX - leftBorderX)*(rightBorderY - leftBorderY) / N;;
	double result = average * square;
	return result;
}