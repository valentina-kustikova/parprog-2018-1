#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "'please, enter viewer [input.bin] [output.txt] '" << std::endl;
		return 1;
	}
	double time;
	int size;
	freopen(argv[1], "rb", stdin);
	freopen(argv[2], "wt", stdout);
	fread(&time, sizeof(double), 1, stdin);
	fread(&size, sizeof(int), 1, stdin);
	double* arr = new double[size];
	fread(arr, sizeof(*arr), size, stdin);
	std::cout << time << std::endl;
	std::cout << size << std::endl;
	for (int i = 0; i < size; i++)
		std::cout << arr[i] << std::endl;
	delete arr;
	fclose(stdin);
	fclose(stdout);
	return 0;
}
