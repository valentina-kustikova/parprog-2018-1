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
	srand(time(NULL));
	int size = n_tests[atoi(argv[1])];
	int* mas = new int[size];
	int* sorted = new int[size];

	for (int i = 0; i < size; i++)
	{
		mas[i] = rand();
		sorted[i] = mas[i];
	}

	//sort(sorted, sorted + size);

	ofstream fin("tests\\input.txt");
	fin << size << endl;
	for (int i = 0; i < size; i++)
		fin << mas[i] << " ";
	fin.close();

	//ofstream fout("output.txt");
	//for (int i = 0; i < size; i++)
	//	fout << sorted[i] << " ";
	//fout.close();

	//freopen("input2.txt", "wb", stdout);
	//fwrite(&size, sizeof(size), 1, stdout);
	//fwrite(mas, sizeof(*mas), size, stdout);

	//freopen("output2.ans", "wb", stdout);
	//fwrite(sorted, sizeof(*sorted), size, stdout);
}