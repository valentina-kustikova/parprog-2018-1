#define _CRT_SECURE_NO_WARNINGS
#include"cstdio"
#include "iostream"
#include <ctime>
#include <algorithm>
#include <fstream>
#include "bitset"

using namespace std;

int n_tests[] = {10, 20, 50, 100, 200, 500, 1000, 5000, 10000, 50000};

int main (int argc, char* argv [])
{ 
	int size = 100;
	char* name = "ExArr.in";
	char* ideal = "ExIdealAnsw.out";
	
	if (argc > 3)
	{
		size = n_tests[atoi(argv[1])];
		name = argv[2];
		ideal = argv[3];

		if (atoi(argv[1]) < 0)
		{
			cout << "Wrong num of test. " << endl;
			return 0;
		}
	}

	srand(time(NULL));
	int* mas = new int[size];
	int* sorted = new int[size];

	for (int i = 0; i < size; i++)
	{
		mas[i] = (rand()-16000);
		sorted[i] = mas[i];
	}

	//int a = 13;
	//int b = -13;
	//cout << a << endl;
	//cout << b << endl;
	//bitset<32> bitset = a;
	//cout << bitset << endl;
	//bitset = b;
	//cout << bitset << endl;

	sort(sorted, sorted + size);

	freopen(ideal, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(sorted, sizeof(*sorted), size, stdout);
	fclose(stdout);

	freopen(name, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(mas, sizeof(*mas), size, stdout);
	fclose(stdout);

	delete[] mas;
	delete[] sorted;

	return 0;
}