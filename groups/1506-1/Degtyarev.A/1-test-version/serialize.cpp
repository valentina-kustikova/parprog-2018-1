#include <iostream>


void Serialize(char* file_bin)
{
	FILE *matr_in, *matr_out;
	int N = 1, Nz = 3;
	
	fopen_s(&matr_in,"matr.txt", "r");
	
	fscanf_s(matr_in,"%d",&N);
	fscanf_s(matr_in, "%d", &Nz);

	// принимаем 2 разреженные матрицы 
	int size_nonzero = N * Nz;
	double** values = new double*[2];
	int** collumns = new int*[2];
	int** index = new int*[2];


	for (int i = 0; i < 2; i++)
	{
		values[i] = new double[size_nonzero];
		collumns[i] = new int[size_nonzero];
		index[i] = new int[N + 1];
	}
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < size_nonzero; i++)
			fscanf_s(matr_in, "%lf", &values[j][i]);
		for (int i = 0; i < size_nonzero; i++)
			fscanf_s(matr_in, "%d ", &collumns[j][i]);
		for (int i = 0; i < N + 1; i++)
			fscanf_s(matr_in, "%d ", &index[j][i]);
	}
	
	fclose(matr_in);

	freopen_s(&matr_out, file_bin, "wb", stdout);
	
	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(&Nz, sizeof(Nz), 1, stdout);

	for (int i = 0; i < 2; i++)
	{
		fwrite(values[i], sizeof(**values), size_nonzero, stdout);
		fwrite(collumns[i], sizeof(**collumns), size_nonzero, stdout);
		fwrite(index[i], sizeof(**index), N + 1, stdout);
	}
	
	fclose(matr_out);

	for (int i = 0; i < 2; i++)
	{
		delete[] values[i];
		delete[] collumns[i];
		delete[] index[i];
	}
			
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		return 0;
	}

	char* f2 = argv[1];
	Serialize(f2);

	return 0;
}





