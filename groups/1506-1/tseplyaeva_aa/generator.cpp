// Generator.cpp: определяет точку входа для консольного приложения.
//
#define _CRT_SECURE_NO_WARNINGS
//запустить generator.exe 2

#include <cstdio>
#include <chrono>
#include <ctime>
#include <fstream> 
#include <iostream>
#include <cstring>

using namespace std;

int n_tests[] = { 5, 10,50, 100, 500, 600, 900, 1000,1200, 1500,1700, 2000, 2500,3000,3200,3500,3700,4000,4200,4500,5000 };

int main(int argc, char* argv[])
{
	int n;
	if (argc > 2){
		n = n_tests[atoi(argv[1])];
		freopen(argv[2], "wb", stdout);
	}
	else {
		if (argc > 1){
			n = n_tests[atoi(argv[1])];
			freopen("array.in", "wb", stdout);
		}
	}
	//	ofstream myarr("array.txt");

	
	//cout << n<<endl;
	fwrite(&n, sizeof(n), 1, stdout);

	double* mass = new double[n];

	srand((double)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (double)((rand() % 200)-100)/10;
	//	cout << mass[i];
	}
	
	double m = 0;
	fwrite(mass, sizeof(*mass), n, stdout);
	fwrite(&m, sizeof(double), 1, stdout);

	delete[] mass;

	return 0;
}
