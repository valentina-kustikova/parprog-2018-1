#define _CRT_SECURE_NO_WARNINGS
#include"cstdio"
#include "iostream"
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{

	if (argc != 3)
	{
		cout << "Wrong quantity of arguments! Enter name of file with sorted mas then num of answer (same with num of test). " << endl;
		return 0;
	}

	int size;
	int size2;
	double time;
	char* name = argv[1];
	char* answ = argv[2];

	//char* str = new char[strlen(name) + 7];
	//char* str2 = new char[strlen(answ) + 7];
	//
	//str = strcat(way, name);
	//str2 = strcat(way, answ);

	freopen(name, "rb", stdin);
	fread(&size, sizeof(size), 1, stdin);

	int* sorted = new int[size];
	int* mas = new int[size];
	fread(sorted, sizeof(sorted), size, stdin);
	fclose(stdin);

	freopen(answ, "rb", stdin);
	fread(&size2, sizeof(size2), 1, stdin);
	//fread(&time, sizeof(time), 1, stdin);
	fread(mas, sizeof(mas), size, stdin);
	fclose(stdin);

	sort(sorted, sorted + size);
	ofstream fout("tests\\Result.txt");
	for (int i = 0; i < size; i++)
	{
		fout << sorted[i] << " ";
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
	/*freopen(answ, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(sorted, sizeof(sorted), size, stdout);
	fclose(stdout);*/
	//fwrite(mas, sizeof(*mas), size, stdout);

	delete[] sorted;
	delete[] mas;
}