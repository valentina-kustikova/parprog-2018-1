#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;

void main(int argc, char* argv[])
{
	int size;
	int* mas;
	char* inputname = "output.txt";
	char* name = "output.out";

	if (argc > 2)
	{
		//cout << "Wrong quantity of arguments! Enter name of binary file then name of output file. " << endl;
		//return;
		inputname = argv[1];
		name = argv[2];
	}

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