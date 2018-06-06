#include <omp.h> 
#include <iostream>
#include <ctime>
#include <random> 
#include <chrono>

#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
using namespace tbb;

struct crsMatrix
{
	int Size; // Размер матрицы
	int Size_Z; // Кол-во ненулевых

	double* Value; // Массив значений
	int* Col; // Массив номеров столбцов
	int* Row_Index;	// Массив индексов строк
};

class Multiplicat
{
	crsMatrix A;
	crsMatrix B;
	std::vector<int>* columns;
	std::vector<double>* values;
	int* rIndex;
public:
	Multiplicat(crsMatrix &_A, crsMatrix &_B, std::vector<int>* &_columns, 
		std::vector<double>* &_values, int* &_rIndex)
	{
		A = _A;
		B = _B;
		columns = _columns;
		values = _values;
		rIndex = _rIndex;
	}

	void operator() (const blocked_range<int>& range) const
	{
		int Size = A.Size;
		int NZ = 0;
		int *temp = new int[Size];


		for (int i = range.begin(); i != range.end(); i++)
		{
			for (int i = 0; i < Size; i++)
				temp[i] = -1;

			int ind1 = A.Row_Index[i], ind2 = A.Row_Index[i + 1];

			for (int j = ind1; j < ind2; j++)
			{
				int col = A.Col[j];
				temp[col] = j;
			}

			// Построен индекс строки i матрицы A
			// Теперь необходимо умножить ее на каждую из строк матрицы BT
			for (int j = 0; j < Size; j++)
			{
				double sum = 0;

				int ind3 = B.Row_Index[j], ind4 = B.Row_Index[j + 1];
				for (int k = ind3; k < ind4; k++)
				{
					int Bcol = B.Col[k];
					int Aind = temp[Bcol];
					if (Aind != -1)
						sum += A.Value[Aind] * B.Value[k];
				}
				if (fabs(sum) != 0)
				{
					columns[i].push_back(j);
					values[i].push_back(sum);
					rIndex[i]++;
				}
			}
		}
	}

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

crsMatrix Transpose(crsMatrix B)
{
	int Size = B.Size;
	int NZSize = B.Size_Z;
	crsMatrix B_T;
	InitMatr(Size, NZSize, B_T);

	for (int i = 0; i < Size + 1; i++)
		B_T.Row_Index[i] = 0;

	// RowIndex[i] = RowIndex[i - 1] + «Количество элементов в векторе i - 1».
	for (int i = 0; i < NZSize; i++)
		B_T.Row_Index[B.Col[i] + 1]++;

	int S = 0;
	int tmp;

	for (int i = 1; i <= Size; i++)
	{
		tmp = B_T.Row_Index[i];
		B_T.Row_Index[i] = S;
		S += tmp;
	}

	for (int i = 0; i < Size; i++)
	{
		int start = B.Row_Index[i];
		int end = B.Row_Index[i + 1];
		int Column = i;								// i-ая строка в B есть i-ый столбец в B_T
		double Value;
		int Rcolumn, Iindex;

		for (int j = start; j < end; j++)
		{
			Value = B.Value[j];
			Rcolumn = B.Col[j];					    // Столбец в B
			Iindex = B_T.Row_Index[Rcolumn + 1];	// индекс ячейки B_T, которая будет содержать Value
			B_T.Value[Iindex] = Value;
			B_T.Col[Iindex] = Column;
			B_T.Row_Index[Rcolumn + 1]++;			// В итоге B_T.rows[j] будет содержать индекс начала j-ой строки 
		}
	}

	return B_T;
}

crsMatrix Multiplication(crsMatrix A, crsMatrix B, double& time)
{
	if (A.Size != B.Size)
		return crsMatrix();
	int i;
	int Size = A.Size;

	crsMatrix C;
	clock_t start = clock();
	std::vector<int>* columns = new std::vector<int>[Size];
	std::vector<double>* values = new std::vector<double>[Size];


	int *rIndex = new int[Size + 1];
	memset(rIndex, 0, sizeof(int) * Size);

	parallel_for(blocked_range<int>(0, Size, 10),Multiplicat(A, B, columns, values, rIndex));

	int NZ = 0;
	for (i = 0; i < Size; i++)
	{
		int tmp = rIndex[i];
		rIndex[i] = NZ;
		NZ += tmp;
	}

	rIndex[Size] = NZ;
	InitMatr(Size, NZ, C);

	int count = 0;
	for (i = 0; i < Size; i++)
	{
		int N = columns[i].size();
		memcpy(&C.Col[count], &columns[i][0],
			N * sizeof(int));
		memcpy(&C.Value[count], &values[i][0],
			N * sizeof(double));
		count += N;
	}

	memcpy(C.Row_Index, &rIndex[0], (Size + 1) * sizeof(int));

	delete[] rIndex;
	delete[] columns;
	delete[] values;

	clock_t finish = clock();
	time = (double)(finish - start) / CLOCKS_PER_SEC;

	return C;
}

void Parallel(char* file_in, char* file_out)
{
	FILE* matr_in, *matr_res;
	crsMatrix A, B, BT, C;
	double time = -1;

	freopen_s(&matr_in, file_in, "rb", stdin);
	int N = 2, Nz = 1;

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

	BT = Transpose(B);
	C = Multiplication(A, BT, time);

	std::cout << "Time: " << time;
	int size_nonzero_C = C.Size_Z;

	// после умножения получилось другое кол во ненулевых
	freopen_s(&matr_res, file_out, "wb", stdout);

	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(&size_nonzero_C, sizeof(size_nonzero_C), 1, stdout);
	fwrite(C.Value, sizeof(*C.Value), size_nonzero_C, stdout);
	fwrite(C.Col, sizeof(*C.Col), size_nonzero_C, stdout);
	fwrite(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdout);	fwrite(&time, sizeof(time), 1, stdout);

	fclose(matr_res);

	FreeMatr(A);
	FreeMatr(B);
	FreeMatr(BT);
	FreeMatr(C);
}

int main(int argc, char *argv[])
{
	char* f1;
	char* f2;
	int num_th = 1;

	if (argc != 4 || atoi(argv[3]) > 10 || atoi(argv[3]) < 1)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		std::cout << "The default values are used:\n  matr.in -> matr.paral_tbb" << std::endl;

		f1 = "matr.in";
		f2 = "matr.paral_tbb";
		num_th = 4;
	}
	else
	{
		f1 = argv[1];
		f2 = argv[2];
		num_th = atoi(argv[3]);
	}
	task_scheduler_init init(task_scheduler_init::deferred);

	init.initialize(num_th);
	Parallel(f1, f2);
	init.terminate();
	return 0;
}




