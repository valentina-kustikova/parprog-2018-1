#define _CRT_SECURE_NO_WARNINGS
#include"cstdio"
#include "iostream"
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

int n_tests[] = {10, 20, 50, 100, 200, 500, 1000, 5000, 10000, 50000};

int main (int argc, char* argv [])
{ 
	int size = 100;
	
	if (argc > 1)
	{
		size = n_tests[atoi(argv[1])];

		if (atoi(argv[1]) < 0)
		{
			cout << "Wrong num of test. " << endl;
			return 0;
		}
	}

	srand(time(NULL));
	int* mas = new int[size];
	for (int i = 0; i < size; i++)
	{
		mas[i] = rand();
	}


	freopen("arr.in", "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(mas, sizeof(*mas), size, stdout);
	fclose(stdout);

	delete[] mas;

	//ofstream fin("tests\\input.txt");
	//fin << size << endl;
	//for (int i = 0; i < size; i++)
	//	fin << mas[i] << " ";
	//fin.close();

	return 0;
}