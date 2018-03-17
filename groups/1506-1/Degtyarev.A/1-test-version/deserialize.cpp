#include <iostream>

void Deserialize(char* file_bin, char* file_txt)
{
	FILE *file_in, *file_out;
	int N = 1 , Nz = 1;
	double time;

	freopen_s(&file_in, file_bin, "rb", stdin);

	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);
	int size_nonzero = Nz;

	double* values = new double[size_nonzero];
	int* collumns = new int[size_nonzero];
	int* index = new int[size_nonzero];
	

	fread(values, sizeof(*values), size_nonzero, stdin);
	fread(collumns, sizeof(*collumns), size_nonzero, stdin);
	fread(index, sizeof(*index), N + 1, stdin);
	fread(&time, sizeof(time), 1, stdin);

	fclose(file_in);

	fopen_s(&file_out, file_txt, "w");

	fprintf(file_out, "Matrix C :  \n");
	for (int i = 0; i < size_nonzero; i++)
		fprintf(file_out, "%lf ", values[i]);
	fprintf(file_out, " \nCollumns: ");
	for (int i = 0; i < size_nonzero; i++)
		fprintf(file_out, "%d ", collumns[i]);
	fprintf(file_out, " \nRow index: ");
	for (int i = 0; i < N + 1; i++)
		fprintf(file_out, "%d ", index[i]);

	delete[] values;
	delete[] collumns;
	delete[] index;	
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		return 0;
	}
	char* f1 = argv[1];
	char* f2 = argv[2];
	Deserialize(f1,f2);

	return 0;
}
