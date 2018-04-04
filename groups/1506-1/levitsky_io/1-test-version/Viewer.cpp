#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{

	if (argc != 3)
	{
		cout << "Wrong quantity of arguments! Enter name of input file. " << endl;
		return 0;
	}

	int size;
	double time;
	int* sortedmas;
	char* name = argv[1];
	char* outname = argv[2];
	freopen(("tests\\", name), "rb", stdin);
	//fread(&time, sizeof(time), 1, stdin);
	fread(&size, sizeof(size), 1, stdin);

	sortedmas = new int[size];

	fread(sortedmas, sizeof(*sortedmas), size, stdin);

	ofstream fout(("tests\\", outname));
	//fout << time << endl;
	fout << size << endl;
	for (int i = 0; i < size; i++)
		fout << sortedmas[i] << " ";
	fout.close();

	delete[] sortedmas;

	return 0;
}