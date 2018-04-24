#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <chrono>
#include <ctime>
#include <iostream>

#define fmin -10
#define fmax 10

#define b_fmin -10000
#define b_fmax 10000

using namespace std;

int n_tests[] = { 1, 10, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900 }; // при больших не заливается на гит

int main(int argc, char* argv[])
{
	int n = 5;                                                       // default value of n
	if (argc > 2) {
		n = n_tests[atoi(argv[1])];
		freopen(argv[2], "wb", stdout);
	}
	else if (argc > 1) {
		n = n_tests[atoi(argv[1])];
		freopen("array.in", "wb", stdout);
	}
	else freopen("array.in", "wb", stdout);

	double **a = new double*[n];
	for (int i = 0; i < n; i++)
		a[i] = new double[n];

	srand((double)time(NULL));

	double f;
	for (int i = 0; i < n; i++)                                      // a
		for (int j = 0; j < n; j++){
			f = (double)rand() / RAND_MAX;
			a[i][j] = fmin + f * (fmax - fmin);
		}

	double *addition = new double[n];                                // transform a-matrix into diagonally dominant matrix 
	for (int i = 0; i < n; i++){
		addition[i] = 0;
		for (int l = 0; l < n; l++)
			addition[i] += abs(a[i][l]);
	}

	for (int i = 0; i < n; i++)
		if (a[i][i] >= 0) a[i][i] += addition[i];
		else  a[i][i] -= addition[i];

		double **at = new double*[n];                               // at- transpose matrix a
		for (int i = 0; i < n; i++){
			at[i] = new double[n];
			for (int l = 0; l < n; l++)
				at[i][l] = a[l][i];
		}

		double **A = new double*[n];                                // A - symmetric positive definite matrix, A= a'*a = at*a
		for (int i = 0; i < n; i++)
			A[i] = new double[n];

		for (int k = 0; k < n; k++)
			for (int l = 0; l < n; l++){
				A[k][l] = 0;
				for (int i = 0; i < n; i++)
					A[k][l] += at[k][i] * a[i][l];
			}



		double *b = new double[n];                                  // b
		for (int i = 0; i < n; i++){
			f = (double)rand() / RAND_MAX;
			b[i] = b_fmin + f * (b_fmax - b_fmin);
		}


		// write to binary file
		fwrite(&n, sizeof(n), 1, stdout);            // size
		for (int i = 0; i < n; i++)                  // A
			fwrite(A[i], sizeof(*A[i]), n, stdout);
		fwrite(b, sizeof(*b), n, stdout);            // b

		return 0;
}


