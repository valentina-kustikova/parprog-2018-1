#define _CRT_SECURE_NO_WARNINGS
#include"cstdio"
#include "iostream"
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	char* name = "SortedExArr.out";
	char* answ = "ExIdealAnsw.out";


	if (argc > 3)
	{
		name = argv[1];
		answ = argv[2];
	}

	int size;
	int size2;

	freopen(name, "rb", stdin);
	fread(&size, sizeof(size), 1, stdin);
	int* mas = new int[size];
	fread(mas, sizeof(mas), size, stdin);
	fclose(stdin);

	freopen(answ, "rb", stdin);
	fread(&size2, sizeof(size2), 1, stdin);
	int* sorted = new int[size];
	fread(sorted, sizeof(sorted), size2, stdin);
	fclose(stdin);

	if (size != size2)
	{
		cout << "Different sizes!" << endl;
		return 0;
	}

	sort(sorted, sorted + size);
	ofstream fout("Result.txt");
	for (int i = 0; i < size; i++)
	{
		if (mas[i] != sorted[i])
		{
			fout << "Bad sorting";
			break;
			fout.close();
		}
		else
		{
			fout << "All rigt!";
			fout.close();
		}
	}

	delete[] sorted;
	delete[] mas;
}