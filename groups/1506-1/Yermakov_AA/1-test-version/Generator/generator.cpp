#define _CRT_SECURE_NO_WARNINGS
#include <cstdio> 
#include <random> 
#include <ctime> 
#include <chrono>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		cout << "Use: " << argv[0] << " <number> <filename>" << endl;
		return 1;
	}
	int n = atoi(argv[1]); //count of elements in vector/array

	freopen(argv[2], "wb", stdout);

	default_random_engine generator(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()));
	uniform_real_distribution <double> distribution(0, 1000);

	fwrite(&n, sizeof(double), 1, stdout);
	fwrite(&n, sizeof(n), 1, stdout);

	double* array = new double[n];

	for (int i = 0; i < n; i++) {
		array[i] = distribution(generator);
	}

	fwrite(array, sizeof(*array), n, stdout);
	delete[] array;
	return 0;
}


