#include "task_scheduler_init.h"
#include "task_group.h"
#include "parallel_for.h"
#include "blocked_range.h"
#include "tick_count.h"
#include <iostream>

using namespace tbb;
using namespace std;


class CannonTBB {
private:
	double *A;
	double *B;
	double *C;
	int row;
	int col;
	int size;
	int blockSize;
	int Shift;
public:
	CannonTBB(double *_A, double *_B, double *_C, int _row, int _col, int _size, int _blockSize, int _shift) :
		A(_A), B(_B), C(_C), row(_row), col(_col), size(_size), blockSize(_blockSize), Shift(_shift) {};
	
	void operator()() const {
		for (int shift = 0; shift < Shift; shift++) {
			for (int i = row * blockSize; i < (row + 1)*blockSize; i++) {
				for (int j = col * blockSize; j < (col + 1)*blockSize; j++) {
					for (int k = shift * blockSize; k < (shift + 1)*blockSize; k++)
					{
						C[i*size + j] += A[i*size + k] * B[k*size + j];
					}
				}
			}
		}
	}
};

int MultiCannon_TBB(double *A, double *B, double *C, int size, int Shift, task_group& tg) {
	int blockSize = size / Shift;
	for (int i = 0; i < Shift; i++) {
		for (int j = 0; j < Shift; j++) {
			tg.run(CannonTBB(A, B, C, i, j, size, blockSize, Shift));
		}
	}
	tg.wait();
	return 0;
}


int main(int argc, char** argv)
{
	setlocale(0, "RUS");
	FILE *stream;	// файл входных данных
	FILE *answer;	// файл выходных данных
	char* name;		// имя файла
	errno_t err;
	int NumThreads;	// число потоков
	if (argc > 2) {
		name = argv[1];
		cout << name << endl;
		err = freopen_s(&stream, name, "rb", stdin);
		if (err != 0)
		{
			cout << "Файл не найден!" << endl;
			return 0;
		}
		NumThreads = atoi(argv[2]);
	}
	else {
		err = freopen_s(&stream, "matr.in", "rb", stdin);
		if (err != 0)
		{
			cout << "Файл не найден!" << endl;
			return 0;
		}
		cout << "Не указано число потоков! Установлено значение по умолчанию = 1." << endl;
		NumThreads = 1;
	}
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

	int Shift = int(sqrt((int)NumThreads));
	if (Shift * Shift != NumThreads)
	{
		cout << "Число потоков должно быть полным квадратом." << endl;
		return 0;
	}
	if (N % Shift != 0)
	{
		cout << "Размер матрицы должен быть кратен корню из числа потоков" << endl;
		return 0;
	}

	task_scheduler_init init(NumThreads);
	task_group tg;
	tick_count start = tick_count::now();
	MultiCannon_TBB(A, B, C, N, Shift, tg);
	tick_count end = tick_count::now();
	double time = (end - start).seconds();
	cout << "time TBB version	: " << time << endl;	

	freopen_s(&answer, "matr.out", "wb", stdout);
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
