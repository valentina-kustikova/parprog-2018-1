#include <stdio.h>  
#include<cstdlib>
#include <ctime>
#include <chrono>
#include <random>
#include <iostream>
using namespace std;
double compareup(const double *a, const double *b)
{
	return *a - *b;
}
double comparedown(const double *a, const double *b)
{
	return *b - *a;
}
int main(int argc, char* argv[])
{
	FILE* fp;
	errno_t err;
	srand(time(0));
	err = fopen_s(&fp, "input", "wb");
	int k = atoi(argv[1]);
	fwrite(&k, sizeof(int), 1, fp);
	double * mas = new double[k];
	default_random_engine generator(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()));
	uniform_real_distribution <double> distribution(-10000, 10000);
	for (int i = 0; i < k; i++)
	{
		mas[i] = distribution(generator);
	}
	if (atoi(argv[2]) == 2)
		std::qsort(mas, k, sizeof(double), (int(*) (const void *, const void *))compareup);
	if (atoi(argv[2]) == 3)
		std::qsort(mas, k, sizeof(double), (int(*) (const void *, const void *))comparedown);
	fwrite(mas, sizeof(double), k, fp);
	delete mas;
	fclose(fp);
	return 0;
}