#define _CRT_SECURE_NO_WARNINGS

#define fMin -100
#define fMax 100

#define maxIter 200      /// ?
#define Eps 0.0001

#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/tbb_allocator.h"
#include "tbb/tick_count.h"
using namespace tbb;

#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
using namespace std;

void copyVectors(double *a, const double *b, int n){ // a <- b
	for (int i = 0; i < n; i++)
		a[i] = b[i];
}

void minusVectors(double *m, double *a, double *b, int n){ // m = a - b	
	for (int i = 0; i < n; i++)
		m[i] = a[i] - b[i];
}

void plusVectors(double *m, double *a, double *b, int n){ // m = a + b
	for (int i = 0; i < n; i++)
		m[i] = a[i] + b[i];
}

double multiplVectors(const double *a, const double *b, int n){ /// m = a * b 
	double mult = 0;
	for (int i = 0; i < n; i++)
		mult += a[i] * b[i];
	return mult;
}


void numMultiplVector(double *m, double k, double *a, int n){ /// m = k * a 		
	for (int i = 0; i < n; i++)
		m[i] = k*a[i];
}

class VectorsMultiplicator
{
	const double **matrix, *vector;
	double *const resultVector;
	int const columns;
public:
	VectorsMultiplicator(double **tmatrix, double *tvector,
		double *tresultVector, int tcolumns) : matrix(tmatrix),
		vector(tvector), resultVector(tresultVector),
		columns(tcolumns) {}
	void operator()(const blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end(), n = columns;
		double *my_vector = new double[n];
		copyVectors(my_vector, vector, n);
		for (int i = begin; i != end; i++)
			resultVector[i] = multiplVectors(matrix[i], my_vector, n);
	}
};
//void TBBMatrixVectorMultiplication(double** matrix,
//	double* vector, double* resultVector, int rows, int columns,
//	int grainSize)
//{
//	parallel_for(blocked_range<int>(0, rows, grainSize), (const blocked_range<int> &r)
//	{
//
//		VectorsMultiplicator(matrix, vector, resultVector, columns);
//	},
//	simple_partitioner());
//	}
//};

void TBBMatrixVectorMultiplication(double** matrix, double* vector, double* resultVector, int rows, int columns)
{
	parallel_for(blocked_range<int>(0, rows),
		VectorsMultiplicator(matrix, vector, resultVector, columns),
		simple_partitioner());
}

void grad(double **A, double *b, double *_x, int n){
	double *g = new double[n]; /// previous gradient                 /// g
	double *_g = new double[n]; ///  actual gradient                 /// _g
	double *d = new double[n]; /// previous direction vector         /// d	
	double *_d = new double[n]; /// direction vector                 /// _d
	double *x = new double[n]; /// previous x                        /// x
	for (int i = 0; i < n; i++)
		x[i] = 0;                                                   /// x0
	double *Ad = new double[n];                                      /// Ad	

	TBBMatrixVectorMultiplication(A, x, g, n, n);                 /// g0
	//////////////////!!!!!!!!!!!!!!!!!!!!!!!! TBBMatrixVectorMultiplication(g, A, x, n, nThreads);                                 


	minusVectors(g, g, b, n);
	for (int i = 0; i < n; i++)
		d[i] = g[i];                                             /// d0 = g0
	double bNorm = sqrt(multiplVectors(b, b, n));
	double alf = 0;
	double betta = 0;
	bool flag;
	int iter = 0;

	do {

		/////////////!!!!!!!!!!!!!!!!!!!!!!!!multiplMatrixs(Ad, A, d, n, nThreads);
		TBBMatrixVectorMultiplication(A, d, Ad, n, n);	/// Ad= A * d

		alf = multiplVectors(g, g, n) / multiplVectors(Ad, d, n);

		numMultiplVector(_x, alf, d, n);  /// _x
		minusVectors(_x, x, _x, n);

		numMultiplVector(_g, alf, Ad, n);  /// _g
		minusVectors(_g, g, _g, n);

		betta = multiplVectors(_g, _g, n) / multiplVectors(g, g, n);

		numMultiplVector(_d, betta, d, n);   /// _d
		plusVectors(_d, _g, _d, n);

		flag = (sqrt(multiplVectors(_g, _g, n)) / bNorm)<Eps;
		copyVectors(d, _d, n);
		copyVectors(g, _g, n);
		copyVectors(x, _x, n);

		iter++;
	} while ((!flag) && (iter<maxIter));

	delete[] g;
	delete[] _g;
	delete[] d;
	delete[] _d;
	delete[] Ad;
	delete[] x;
}




int main(int argc, char* argv[]) {
	int nThreads = 1, n;

	if (argc > 3) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "wb", stdout);
		nThreads = atoi(argv[3]);
	}
	else {
		if (argc > 1) {
			nThreads = atoi(argv[1]);
			freopen("array.in", "rb", stdin);
			freopen("array.out", "wb", stdout);
		}
		else return 1;
	}

	task_scheduler_init init(nThreads);

	///////
	fread(&n, sizeof(n), 1, stdin);

	double **A = new double*[n];                                       /// A
	for (int i = 0; i < n; i++) A[i] = new double[n];
	for (int i = 0; i < n; i++)
		fread(A[i], sizeof(*A[i]), n, stdin);

	double *b = new double[n];                                       /// b
	fread(b, sizeof(*b), n, stdin);

	double *x = new double[n];                                       /// x
	fread(x, sizeof(*x), n, stdin);
	///////


	tick_count t0 = tick_count::now();
	//task_scheduler_init init(nThreads);	
	grad(A, b, x, n);
	tick_count t1 = tick_count::now();

	double time = (t1 - t0).seconds();

	///////
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(x, sizeof(*x), n, stdout);
	fwrite(&time, sizeof(time), 1, stdout);

	for (int i = 0; i < n; i++){
		delete[] A[i];
	}
	delete[] b;
	delete[] x;
	///////


	return 0;
}