#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 3) {
		cout << "Error! You should input 'typer [input file] [output file]'" << endl;
		return 1;
	}

	FILE *in, *out;
	char* number = new char[15];
	int size = 0;
	vector<double> array_data;
	in = fopen(argv[1], "rt");
	out = fopen(argv[2], "wb");
	if (in != nullptr)
	{
		while (fgets(number, 15, in) != nullptr)
			array_data.push_back(atoi(number));

		size = array_data.size();

		fwrite(&size, sizeof(double), 1, out); 
		fwrite(&size, sizeof(size), 1, out);
		for (int i = 0; i < size; ++i)
			fwrite(&array_data[i], sizeof(double), 1, out);
	}
	fclose(in);
	fclose(out);
	delete[] number;
	return 0;
}