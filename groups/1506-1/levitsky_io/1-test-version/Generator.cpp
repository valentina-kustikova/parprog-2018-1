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
	if (argc != 3)
	{
		cout << "Wrong quantity of arguments! Enter num of test then name of output file. " << endl;
		return 0;
	}

	if (atoi(argv[1]) < 0)
	{
		cout << "Wrong num of test. " << endl;
		return 0;
	}

	srand(time(NULL));
	int size = n_tests[atoi(argv[1])];
	char* name = argv[2];
	int* mas = new int[size];

	for (int i = 0; i < size; i++)
	{
		mas[i] = rand();
	}


	freopen(("tests\\", name), "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(mas, sizeof(*mas), size, stdout);
	fclose(stdout);

	delete[] mas;

	//ofstream fin("tests\\input.txt");
	//fin << size << endl;
	//for (int i = 0; i < size; i++)
	//	fin << mas[i] << " ";
	//fin.close();

}