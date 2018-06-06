#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <random>
#include <ctime>
#include <chrono>
#include <iostream>
using namespace std;
int  n_tests[] = { 1, 5, 10, 10, 20, 50, 100, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000, 10000000};
int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "please, enter ' generator [number_test] [output.bin] ' " << std::endl;
		return 1;
	}
	int n = n_tests[atoi(argv[1])];
	freopen(argv[2], "wb", stdout);
	default_random_engine generator((chrono::system_clock::now().time_since_epoch().count()));
	uniform_real_distribution <double> distribution(-1e4, 1e4);
	fwrite(&n, sizeof(n), 1, stdout);
	double * cur = new double[n];
	for(int i = 0; i < n; i++) {
		cur[i] = distribution(generator);
	}
	fwrite(cur, sizeof(*cur), n, stdout);
	fclose(stdout);
	delete cur;
	return 0;
}

