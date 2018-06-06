#include <iostream>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <omp.h>

using namespace std;

void LineMatrixMultiply(double* A, double* B, double* C, int n);

int main(int argc, char *argv[])
{
	setlocale(0, "RUS");
	FILE *stream;	// файл входных данных
	FILE *answer;	// файл выходных данных
	char* name;		// имя файла
	errno_t err;
	if (argc > 1) {
		name = argv[1];
		cout << name << endl;
		err = freopen_s(&stream, name, "rb", stdin);
		if (err != 0)
		{
			cout << "Файл не найден!" << endl;
			return 0;
		}
	}
	else {
		err = freopen_s(&stream, "matr.in", "rb", stdin);
		if (err != 0)
		{
			cout << "Файл не найден!" << endl;
			return 0;
		}
	}
	freopen_s(&answer, "matr.out", "wb", stdout);

	int N;
	double *A, *B, *C;
	// считываем размер матрицы
	fread(&N, sizeof(N), 1, stdin);
	A = new double[N*N];
	B = new double[N*N];
	C = new double[N*N];

	// считываем 1-ю матрицу
	fread(A, sizeof(*A), N*N, stdin);
	// считываем 2-ю матрицу
	fread(B, sizeof(*B), N*N, stdin);

	// Выполнение последовательной программы
	double time = omp_get_wtime();
	LineMatrixMultiply(A, B, C, N);
	time = omp_get_wtime() - time;

	// запись времени выполнения
	fwrite(&time, sizeof(time), 1, stdout);
	// запись размера матрицы
	fwrite(&N, sizeof(N), 1, stdout);
	// запись результирующей матрицы
	fwrite(C, sizeof(*C), N*N, stdout);

	fclose(stream);
	fclose(answer);
	delete[] A;
	delete[] B;
	delete[] C;

	return 0;
}