#define _CRT_SECURE_NO_WARNINGS
#include <cstdio> 
#include <random> 
#include <ctime> 
#include <chrono>

using namespace std;

int n_tests[] = { 10, 50, 100, 200, 300, 400, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 20000, 40000, 100000, 200000, 300000, 400000, 500000, 1000000 };

int main(int argc, char * argv[])
{
	freopen("matr.in", "wb", stdout);
	default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
	uniform_real_distribution <double> distribution(-1e3, 1e3);

	int n = 1000;
	if (argc > 1) 
		n = n_tests[atoi(argv[1])];
	fwrite(&n, sizeof(n), 1, stdout);

	int *cur = new int[2];
	for (int i = 0; i < 2*n; i++)
	{
		cur[0] = distribution(generator);
		cur[1] = distribution(generator);		
		fwrite(cur, sizeof(*cur), 2, stdout);
	}
	delete cur;
	return 0;
}