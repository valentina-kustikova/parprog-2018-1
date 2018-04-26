// generator.cpp: определяет точку входа для консольного приложения.
//


#include <iostream>
#include <iomanip>
#include <cstdio> 
#include <random> 
#include <ctime> 
#include <chrono> 

using namespace std;

int n_tests[] = { 5 , 4, 8, 10, 200, 1024, 15, 18, 400, 300 , 2000, 4096 };

void LineMatrixMultiply(double* A, double* B, double* C, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[i*n + j] = 0;
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
	//freopen("matr.in", "wb", stdout);  
	FILE *inp;
	freopen_s(&inp, "matr.in", "wb", stdout);
	FILE *answer;

	// создаём генератор случайных чисел с seed равным количеству времени с начала эпохи   
	default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
	// создаём равномерное распределение случайной величины типа double в диапазоне    //   [-10000, 10000]   
	//uniform_real_distribution <double> distribution(-1e4, 1e4);   
	uniform_real_distribution <double> distribution(-10, 10);
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
	// генерируем первую матрицу   
	for (int i = 0; i < n; i++)
	{   // заполняем случайными числами из равномерного распределения очередную строку матрицы     
		for (int j = 0; j < n; j++) {
			cur[j] = distribution(generator);
			A[i*n + j] = cur[j];
		}
		// записываем строку в бинарном виде в файл    
		fwrite(cur, sizeof(*cur), n, inp);
	}
	// аналогично генерируем вторую матрицу   
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			cur[j] = distribution(generator);
			B[i*n + j] = cur[j];
		}
		fwrite(cur, sizeof(*cur), n, inp);
	}

	LineMatrixMultiply(A, B, C, n);

	freopen_s(&answer, "answer.ans", "wb", stdout);
	//fwrite(&time, sizeof(time), 1, answer);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(C, sizeof(*C), n*n, stdout);


	fclose(answer);
	fclose(inp);
	return 0;
}

