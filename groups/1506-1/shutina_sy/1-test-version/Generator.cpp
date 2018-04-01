// Generator.cpp: определяет точку входа для консольного приложения.
//
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <chrono>
#include <ctime>

using namespace std;

int n_tests[] = { 1,10,256,500,600,900,1000,1500,2000,2500,3000,3500,40000,4500,5000,5500,6000,6500,7000,10000 };

int main(int argc, char* argv[])
{
	int n;
	int a, b;
	if (argc > 4) {
		a = atoi(argv[1]);
		b = atoi(argv[2]);
		n = n_tests[atoi(argv[3])];
		freopen(argv[4], "wb", stdout);
	}
	else {
		if (argc > 3) {
			a = atoi(argv[1]);
			b = atoi(argv[2]);
			n = n_tests[atoi(argv[3])];
			freopen("array.in", "wb", stdout);
		}
	}	

	fwrite(&n, sizeof(n), 1, stdout);

	double* mass = new double[n];

	srand((double)time(NULL));
	for (int i = 0; i < n; i++) {
		double r = (double) rand() / RAND_MAX;
		mass[i] = a + (b - a)*r;
	}
	fwrite(mass, sizeof(*mass), n, stdout);

	delete[] mass;

	return 0;
}

