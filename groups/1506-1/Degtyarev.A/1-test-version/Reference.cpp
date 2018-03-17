#include <iostream>
#include <ctime>

// умножение плотных матриц

struct crsMatrix
{
	int Size; // Размер матрицы
	int Size_Z; // Кол-во ненулевых

	double* Value; // Массив значений
	int* Col; // Массив номеров столбцов
	int* Row_Index;	// Массив индексов строк
};

void InitMatr(int N, int NZ, crsMatrix &matr)
{
	matr.Size = N;
	matr.Size_Z = NZ;
	matr.Value = new double[NZ];
	matr.Col = new int[NZ];
	matr.Row_Index = new int[N + 1];
}

void FreeMatr(crsMatrix &matr)
{
	delete[] matr.Col;
	delete[] matr.Value;
	delete[] matr.Row_Index;
}

double* MultiplicationDenseMatrix(double* M_1, double* M_2, int Size)
{
	double* M_Rez = new double[Size * Size];

	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
		{
			M_Rez[i * Size + j] = 0;
			for (int k = 0; k < Size; k++)
				M_Rez[i*Size + j] += M_1[i*Size + k] * M_2[k * Size + j];
		}

	return M_Rez;
}

// перевод из разреженной в плотную
void Copying_dense(double* matr, crsMatrix A, int size)
{
	int i1, i2;
	for (int i = 0; i < size; i++)
	{
		i1 = A.Row_Index[i];
		i2 = A.Row_Index[i + 1] - 1;
		for (int j = i1; j <= i2; j++)
			matr[i*size + A.Col[j]] = A.Value[j];
	}
}

void SequenceDenseMatrix(char* file_in, char* file_out)
{
	FILE* matr_in, *matr_res;
	int N = 0, Nz = 0;
	crsMatrix A, B;

	clock_t start = clock();

	freopen_s(&matr_in, file_in, "rb", stdin);
	
	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);

	int size_nonzero = N * Nz;


	InitMatr(N, size_nonzero, A);
	InitMatr(N, size_nonzero, B);

	fread(A.Value, sizeof(*A.Value), size_nonzero, stdin);
	fread(A.Col, sizeof(*A.Col), size_nonzero, stdin);
	fread(A.Row_Index, sizeof(*A.Row_Index), N + 1, stdin);

	fread(B.Value, sizeof(*B.Value), size_nonzero, stdin);
	fread(B.Col, sizeof(*B.Col), size_nonzero, stdin);
	fread(B.Row_Index, sizeof(*B.Row_Index), N + 1, stdin);

	fclose(matr_in);

	double* res;
	double* M_1 = new double[N*N];
	double* M_2 = new double[N*N];

	for (int i = 0; i < N*N; i++)
		M_1[i] = M_2[i] = 0;

	Copying_dense(M_1,A,N);
	Copying_dense(M_2,B,N);

	res = MultiplicationDenseMatrix(M_1,M_2,N);


	freopen_s(&matr_res, file_out, "wb", stdout);
	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(res, sizeof(*res), N*N, stdout);
	fclose(matr_res);

	FreeMatr(A);
	FreeMatr(B);

	delete[] res;
	delete[] M_1;
	delete[] M_2;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		return 0;
	}

	SequenceDenseMatrix(argv[1], argv[2]);
	return 0;
}