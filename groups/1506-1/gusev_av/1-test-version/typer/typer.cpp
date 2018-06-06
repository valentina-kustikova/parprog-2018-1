#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "'please, enter typer [input.txt] [output.bin] '" << std::endl;
		return 1;
	}

	freopen(argv[1], "rt", stdin);
	freopen(argv[2], "wb", stdout);
	double time;
	int size;
	std::ifstream fin(argv[1]);
	fin >> time;
	fin >> size; 
	double* arr = new double[size];
	for (int i = 0; i < size; i++)
		fin >> arr[i];
	fwrite(&time, sizeof(double), 1, stdout);
	fwrite(&size, sizeof(int), 1, stdout);
		fwrite(arr, sizeof(*arr), size, stdout);
	delete[] arr;
	fclose(stdin);
	fclose(stdout);
	return 0;
}