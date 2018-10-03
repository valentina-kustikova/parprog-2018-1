#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 3) {
		cout << "Error! You should input 'viewer file.bin file.txt'" << endl;
		return 1;
	}

	int size;
	double* array;
	double time;

	if (!freopen(argv[1], "rb", stdin) ||
		!freopen(argv[2], "wt", stdout))
		return 1;

	fread(&time, sizeof(time), 1, stdin);
	fread(&size, sizeof(size), 1, stdin);
	array = new double[size];
	
	fread(array, sizeof(*array), size, stdin);

	for (int i = 0; i < size; i++) {
		cout << array[i] << endl;
	}
	return 0;
}


