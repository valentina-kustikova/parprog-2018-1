#include <iostream>


// 2 матрица
void Deserialize(char* file_txt)
{
	FILE *file_in, *file_out;
	int N = 2 , Nz = 1;
	freopen_s(&file_in, "matr.in", "rb", stdin);

	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);

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

	for (int i = 0; i < 2; i++)
	{
		fread(values[i], sizeof(**values), size_nonzero, stdin);
		fread(collumns[i], sizeof(**collumns), size_nonzero, stdin);
		fread(index[i], sizeof(**index), N + 1, stdin);
	}
	fclose(file_in);


	fopen_s(&file_out, file_txt, "w");

	fprintf(file_out, "%d \n", N);
	fprintf(file_out, "%d \n", Nz);

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < size_nonzero; i++)
			fprintf(file_out, "%lf ", values[j][i]);
		fprintf(file_out, "\n");
		for (int i = 0; i < size_nonzero; i++)
			fprintf(file_out, "%d ", collumns[j][i]);
		fprintf(file_out, "\n");
		for (int i = 0; i < N + 1; i++)
			fprintf(file_out, "%d ", index[j][i]);
		fprintf(file_out, "\n");
	}
	fclose(file_out);
	for (int i = 0; i < 2; i++)
	{
		delete[] values[i];
		delete[] collumns[i];
		delete[] index[i];
	}
}

// 1 матрицы
void Deserialize(char* file_bin, char* file_txt)
{
	FILE *file_in, *file_out;
	int N = 2, Nz = 1;

	errno_t f;
	f = freopen_s(&file_in, file_bin, "rb", stdin);
	if (f != 0)
	{
		std::cout << "Could not open file \n" << std::endl;
		return;
	}
		

	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);

	// принимаем 2 разреженные матрицы 
	int size_nonzero = Nz;

	double* values = new double[size_nonzero];
	int* collumns = new int[size_nonzero];
	int* index = new int[N + 1];
	
	fread(values, sizeof(*values), size_nonzero, stdin);
	fread(collumns, sizeof(*collumns), size_nonzero, stdin);
	fread(index, sizeof(*index), N + 1, stdin);
	fclose(file_in);


	fopen_s(&file_out, file_txt, "w");
	fprintf(file_out, "N = ");
	fprintf(file_out, "%d \n", N);
	fprintf(file_out, "Nz = ");
	fprintf(file_out, "%d \n", Nz);
	fprintf(file_out, "Matrix С :  \n");

	for (int i = 0; i < size_nonzero; i++)
		fprintf(file_out, "%lf ", values[i]);
	fprintf(file_out, " \nCollumns: ");
	for (int i = 0; i < size_nonzero; i++)
		fprintf(file_out, "%d ", collumns[i]);
	fprintf(file_out, " \nRow index: ");
	for (int i = 0; i < N + 1; i++)
		fprintf(file_out, "%d ", index[i]);

	fclose(file_out);

	delete[] values;
	delete[] collumns;
	delete[] index;
	
}

int main(int argc, char* argv[])
{
	char* f1;
	char* f2;

	if (argc < 2 || argc > 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		std::cout << "The default values are used:\n  matr.in -> matr.txt" << std::endl;
		f1 = "matr.txt";
		f2 = "";
	}
	else
	{
		f1 = argv[1];
		f2 = argv[2];
	}


	if (f2 != "")
		Deserialize(f1, f2);
	else
		Deserialize(f1);

	return 0;
}


