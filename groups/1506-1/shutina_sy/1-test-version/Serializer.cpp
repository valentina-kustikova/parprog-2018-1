// Serializer.cpp: определяет точку входа для консольного приложения.
//
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void serializer(char* txt, char* bin) {

	int n;
	ifstream fin(txt);
	fin >> n;

	int* arr = new int[n];

	for (int i = 0; i < n; i++) {
		fin >> arr[i];
	}
	
	freopen(bin, "wb", stdout);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(arr, sizeof(*arr), n, stdout);

	fclose(stdout);

	delete[] arr;
}
int main(int argc, char* argv[])
{
	if (argc < 3) {
		return 1;
	}

	char* txt = argv[1];
	char* bin = argv[2];

	serializer(txt, bin);

	return 0;
}
