#include <iostream>
#include <ctime>

#include "mkl_spblas.h"




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

void SparseMKLMult(crsMatrix A, crsMatrix B, crsMatrix& C)
{
	int Size = A.Size;

	// Настроим параметры для вызова функции MKL
	// Переиндексируем матрицы A и B с единицы

	for (int i = 0; i < A.Size_Z; i++)
		A.Col[i]++;
	for (int i = 0; i < B.Size_Z; i++)
		B.Col[i]++;
	for (int j = 0; j <= Size; j++)
	{
		A.Row_Index[j]++;
		B.Row_Index[j]++;
	}

	// Используется функция, вычисляющая C = op(A) * B говорит о том, op(A) = A – не нужно транспонировать A
	char trans = 'N'; 

	// Если мы не знаем, сколько памяти необходимо
	// для хранения результата, необходимо:
	// 1) выделить память для массива индексов строк ic:
	// "Кол-во строк+1" элементов;
	// 2) вызвать функцию с параметром request = 1 –
	// в массиве ic будет заполнен последний элемент
	// 3) выделить память для массивов c и jc 
	// (кол-во элементов = ic[Кол-во строк]-1)
	// 4) вызвать функцию с параметром request = 2

	int request;
	// Еще один нетривиальный момент: есть возможность
	// настроить, нужно ли упорядочивать матрицы A, B и C.
	// У нас предполагается, что все матрицы упорядочены,
	// следовательно, выбираем вариант "No-No-Yes", который
	// соответствует любому значению, кроме целых чисел
	// от 1 до 7 включительно
	int sort = 8;

	// Количество ненулевых элементов.
	// Используется только если request = 0
	int nzmax = -1;

	// Служебная информация
	int info;

	// Выделим память для индекса в матрице C
	C.Row_Index = new int[Size + 1];
	// Сосчитаем количество ненулевых элементов в матрице C
	request = 1;
	C.Value = 0;
	C.Col = 0;

	mkl_dcsrmultcsr(&trans,&request,&sort,&Size, &Size, &Size,
		A.Value,A.Col,A.Row_Index, B.Value, B.Col, B.Row_Index, 
		C.Value, C.Col, C.Row_Index,&nzmax,&info);

	int nzc = C.Row_Index[Size] - 1;
	C.Value = new double[nzc];
	C.Col = new int[nzc];

	// Сосчитаем C = A * B
	request = 2;
	mkl_dcsrmultcsr(&trans, &request, &sort, &Size, &Size, &Size,
		A.Value, A.Col, A.Row_Index, B.Value, B.Col, B.Row_Index,
		C.Value, C.Col, C.Row_Index,&nzmax, &info);
	C.Size = Size;
	C.Size_Z = nzc;

  for (int i = 0; i < C.Size_Z; i++)
    C.Col[i]--;
  for (int j = 0; j <= Size; j++)
    C.Row_Index[j]--;
}




void SequenceSparseMult(char* file_in, char* file_out)
{
	FILE* matr_in, *matr_res;
	
	int N = 0, Nz = 0;
	crsMatrix A, B,C;


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

	SparseMKLMult(A,B,C);

	size_nonzero = C.Size_Z;

	freopen_s(&matr_res, file_out, "wb", stdout);
	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(&size_nonzero, sizeof(size_nonzero), 1, stdout);
	fwrite(C.Value, sizeof(*C.Value), size_nonzero, stdout);
	fwrite(C.Col, sizeof(*C.Col), size_nonzero, stdout);
	fwrite(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdout);

	fclose(matr_res);

	FreeMatr(A);
	FreeMatr(B);
	FreeMatr(C);
	
}

int main(int argc, char* argv[])
{
	double time;
	char* f1;
	char* f2;


	if (argc != 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		std::cout << "The default values are used:\n  matr.in -> matr.stand" << std::endl;

		f1 = "matr.in";
		f2 = "matr.stand";
	}
	else
	{
		f1 = argv[1];
		f2 = argv[2];
	}

	SequenceSparseMult(f1, f2);
	return 0;
}