#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;

void main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Wrong quantity of arguments! Enter name of binary file then name of output file. " << endl;
		return;
	}

	int size;
	int* mas;
	char* inputname = argv[1];
	char* name = argv[2];
	ifstream fin(inputname);
	fin >> size;

	mas = new int[size];

	for (int i = 0; i < size; i++)
	{
		fin >> mas[i];
	}
	fin.close();

	freopen(("tests\\", name), "wb", stdout);

	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(mas, sizeof(*mas), size, stdout);
	
	delete[] mas;

}