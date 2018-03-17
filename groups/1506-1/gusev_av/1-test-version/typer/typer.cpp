#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "'please, enter typer [input.bin] [output.bin] '" << std::endl;
		return 1;
	}
	FILE *in, *out;
	in = fopen(argv[1], "rt");
	out = fopen(argv[2], "wb");
	char* num = new char[20];
	std::vector<int> vec;
	size_t size = 0;
	double time;

	if (in != nullptr)
	{
		while (fgets(num, 20, in) != nullptr)
			vec.push_back(atoi(num));
		size = vec.size();
		fwrite(&time, sizeof(double), 1, out);
			fwrite(&size, sizeof(int), 1, out);
		for (int i = 0; i < size; i++)
			fwrite(&vec[i], sizeof(int), 1, out);
	}
	fclose(in);
	fclose(out);
	delete[] num;
	return 0;
}
