#define _CRT_SECURE_NO_WARNINGS
#include <omp.h> 
#include <iostream>
#include <ctime>
#include "math.h"

#define fMin -100
#define fMax 100

#define maxIter 200      /// ?
#define Eps 0.0001

using namespace std;

void printMatrix(double **A, int m, int n){
	for (int i = 0; i < m; i++){
		for (int l = 0; l < n; l++)
			cout << A[i][l] << "  ";
		cout << endl;
	}
}

void copyVectors(double **A, double **B, int n){ // A <- B
	for (int i = 0; i < n; i++)
		A[i][0] = B[i][0];
}

void minusMatrixs(double **M, double **A, double **B, int m, int n){ // M = A - B
	double del;
	for (int i = 0; i < m; i++)
		for (int l = 0; l < n; l++){
			del = A[i][l] - B[i][l];
			M[i][l] = A[i][l] - B[i][l];
		}

}

void plusMatrixs(double **M, double **A, double **B, int m, int n){ // M = A + B
	for (int i = 0; i < m; i++)
		for (int l = 0; l < n; l++)
			M[i][l] = A[i][l] + B[i][l];

}

double multiplVectors(double **A, double **B, int n){ /// M = a * b 
	double mult = 0;
	for (int i = 0; i < n; i++)
		mult += A[i][0] * B[i][0];
	return mult;
}

void numMultiplVector(double **M, double k, double **A, int n){ /// M = k * A 	
	double del;
	for (int i = 0; i < n; i++){
		del = k*A[i][0];
		M[i][0] = k*A[i][0];
	}
}

void multiplMatrixs(double **M, double **A, double **b, int n, int nThreads){ // M = A * b
#pragma omp parallel for shared(A,M) firstprivate(b,n) num_threads(nThreads)		
	for (int k = 0; k < n; k++)	{
		M[k][0] = 0;
		for (int i = 0; i < n; i++)
			M[k][0] += A[k][i] * b[i][0];
	}
}


void grad(double **A, double **b, double **_x, int n, int nThreads){
	double **g = new double*[n]; /// previous gradient                 /// g
	for (int i = 0; i < n; i++)
		g[i] = new double[1];
	double **_g = new double*[n]; ///  actual gradient                 /// _g
	for (int i = 0; i < n; i++) _g[i] = new double[1];
	double **d = new double*[n]; /// previous direction vector         /// d
	for (int i = 0; i < n; i++)
		d[i] = new double[1];
	double **_d = new double*[n]; /// direction vector                 /// _d
	for (int i = 0; i < n; i++) _d[i] = new double[1];
	double **x = new double*[n]; /// previous x                        /// x
	for (int i = 0; i < n; i++) {
		x[i] = new double[1];
		x[i][0] = 0;                                                   /// x0
	}
	double **Ad = new double*[n];                                      /// Ad
	for (int i = 0; i < n; i++) Ad[i] = new double[1];
	multiplMatrixs(g, A, x, n, nThreads);                                  /// g0
	minusMatrixs(g, g, b, n, 1);
	for (int i = 0; i < n; i++)
		d[i][0] = g[i][0];                                             /// d0 = g0
	double bNorm = sqrt(multiplVectors(b, b, n));
	double alf = 0;
	double betta = 0;
	bool flag;
	int iter = 0;

	do {
		//#pragma omp parallel for shared(A,Ad) firstprivate(d,n) num_threads(nThreads)		/// Ad= A * d
		//		for (int k = 0; k < n; k++)	{
		//			Ad[k][0] = 0;
		//			for (int i = 0; i < n; i++)
		//				Ad[k][0] += A[k][i] * d[i][0];
		//		}
		multiplMatrixs(Ad, A, d, n, nThreads);	/// Ad= A * d
		alf = multiplVectors(g, g, n) / multiplVectors(Ad, d, n);

		numMultiplVector(_x, alf, d, n);  /// _x
		minusMatrixs(_x, x, _x, n, 1);

		numMultiplVector(_g, alf, Ad, n);  /// _g
		minusMatrixs(_g, g, _g, n, 1);

		betta = multiplVectors(_g, _g, n) / multiplVectors(g, g, n);

		numMultiplVector(_d, betta, d, n);   /// _d
		plusMatrixs(_d, _g, _d, n, 1);

		flag = (sqrt(multiplVectors(_g, _g, n)) / bNorm)<Eps;
		copyVectors(d, _d, n);
		copyVectors(g, _g, n);
		copyVectors(x, _x, n);

		iter++;
	} while ((!flag) && (iter<maxIter));
	for (int i = 0; i < n; i++){
		delete[] g[i];
		delete[] _g[i];
		delete[] d[i];
		delete[] _d[i];
		delete[] Ad[i];
		delete[] x[i];
	}
}

void main(int argc, char* argv[]) {
	int nThreads, n;

	if (argc > 3) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "wb", stdout);
		nThreads = atoi(argv[3]);
	}
	else if (argc > 1) {
		nThreads = atoi(argv[1]);
		freopen("array.in", "rb", stdin);
		freopen("array.out", "wb", stdout);
	}
	else return;

	fread(&n, sizeof(n), 1, stdin);

	double **A = new double*[n];                                       /// A
	for (int i = 0; i < n; i++) A[i] = new double[n];
	for (int i = 0; i < n; i++){
		fread(A[i], sizeof(*A[i]), n, stdin);
	}

	double **b = new double*[n];                                       /// b
	for (int i = 0; i < n; i++) {
		b[i] = new double[1];
		fread(b[i], sizeof(b[i][0]), 1, stdin);
	}

	double **x = new double*[n];                                       /// x
	for (int i = 0; i < n; i++)
		x[i] = new double[1];


	double start_time = omp_get_wtime();

	grad(A, b, x, n, nThreads);

	double time = omp_get_wtime() - start_time;

	fwrite(&n, sizeof(n), 1, stdout);
	for (int i = 0; i < n; i++)
		fwrite(x[i], sizeof(x[i][0]), 1, stdout);
	fwrite(&time, sizeof(time), 1, stdout);

	for (int i = 0; i < n; i++){
		delete[] A[i];
		delete[] b[i];
		delete[] x[i];
	}



	//#pragma omp parallel for shared(A,c) firstprivate(b,n) private(tid) num_threads(nThreads)		
	//		for (int k = 0; k < n; k++)	{
	//			c[k][0] = 0;
	//			for (int i = 0; i < n; i++)
	//				c[k][0] += A[k][i] * b[i][0];
	//			printf("c[%d][%d]= %f   from thread = %d\n", k, 0, c[k][0], omp_get_thread_num());
	//		}
	//	
	//
	//oldVersion(check, A, b, n, n, 1);
	//cout << endl<<"check:"<<endl;
	//printMatrix(check, n, 1);


}