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
	freopen("array.in", "wb", stdout);

	//	ofstream myarr("array.txt");

	int n;
	if (argc > 1) {
		n = n_tests[atoi(argv[1])];
	}
	//cout << n<<endl;
	fwrite(&n, sizeof(n), 1, stdout);

	int* mass = new int[n];

	srand((int)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (int)(rand() % 2000);
		//	myarr << mass[i];
	}

	fwrite(mass, sizeof(*mass), n, stdout);

	return 0;
}
