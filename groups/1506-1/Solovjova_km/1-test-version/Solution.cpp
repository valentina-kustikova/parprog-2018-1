#include <omp.h>
double Calculation(double *functionValues, int N) {

	double average = 0.0;
	double valuesSum = 0.0;
	for (int i = 0; i < N; i++) {
		valuesSum += functionValues[i];
	}
	average = valuesSum / N;
	return average;
}

double MonteCarlo(double *functionValues, double leftBorderX, double leftBorderY, double rightBorderX, double rightBorderY, int N)
{
	double average = Calculation(functionValues, N);
	double square = (rightBorderX - leftBorderX)*(rightBorderY - leftBorderY);
	double result = average * square;
	return result;
}


