#include <iostream>
#include <iomanip>
#include <cstdio> 
#include <random> 
#include <ctime> 
#include <chrono> 

using namespace std;

int n_tests[] = { 1, 2, 2, 10, 3, 4, 5, 40, 23, 8,
9, 10, 10, 10, 10, 100, 200, 300, 400, 500, 600, 700,
800, 900, 1000 };

void LineMatrixMultiply(double* A, double* B, double* C, int n)
{
	for (int i = 0; i < n;i++)
	{
		for (int j = 0; j < n;j++)
		{
			C[i*n + j] = 0.0;
			for (int k = 0; k < n; k++)
			{
				C[i*n + j] += A[i*n + k] * B[k*n + j];
			}
		}
	}
}


int main(int argc, char * argv[])
{
	// перенаправляем поток stdout в файл matr.in    
	FILE *stream;
	freopen_s(&stream, "matr.in", "wb", stdout);
	FILE *answer;
	
	// создаём генератор случайных чисел с seed равным количеству времени с начала эпохи   
	default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
	// создаём равномерное распределение случайной величины типа double в диапазоне    //   [-500, 500]     
	uniform_real_distribution <double> distribution(-500, 500);
	// задаём размер матриц   
	int n = 10;
	// если передали номер теста в аргументах командной строки, то берём размер из n_tests   
	if (argc > 1)
		n = n_tests[atoi(argv[1])];
	// записываем в бинарном виде размерность матриц   
	fwrite(&n, sizeof(n), 1, stdout);
	// создаём временный массив для строки матрицы  
	double *cur = new double[n];
	double *A = new double[n*n];
	double *B = new double[n*n];
	double *C = new double[n*n];
	double tempDis = 0.0;
	// генерируем первую матрицу   
	for (int i = 0; i < n; i++)
	{   // заполняем случайными числами из равномерного распределения очередную строку матрицы     
		for (int j = 0; j < n; j++) {
			cur[j] = distribution(generator);
			A[i*n + j] = cur[j];
		} 
	}
	// записываем матрицу в бинарном виде в файл 
	fwrite(A, sizeof(*A), n*n, stream);
	// аналогично генерируем вторую матрицу   
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			cur[j] = distribution(generator);
			B[i*n + j] = cur[j];
		}
	}
	fwrite(B, sizeof(*B), n*n, stream);
	LineMatrixMultiply(A, B, C, n);
	double time = 0.0;	// просто переменная, для корректной работы viewer'а, если захочется посмореть эталонный результат глазами.

	freopen_s(&answer, "answer.ans", "wb", stdout);
	fwrite(&time, sizeof(time), 1, answer);
	fwrite(&n, sizeof(n), 1, answer);
	fwrite(C, sizeof(*C), n*n, answer);

	fclose(answer);
	fclose(stream);

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] cur;
	return 0;
}