// viewer.cpp получает на вход имя файла и преобразует его содержимое из бинарного вида в текстовый

#include <cstdio> 
#include <omp.h> 
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char * argv[])
{
	setlocale(0, "RUS");
	int N;
	double *C;

	FILE *stream;
	char* name;
	if (argc > 1) {
		name = argv[1];
		cout << name << endl;
		errno_t err;
		err = freopen_s(&stream, name, "rb", stdin);
		if (err != 0)
		{
			cout << "Файл не найден!" << endl;
			return 0;
		}
	}
	else {
		freopen_s(&stream, "matr.out", "rb", stdin);
	}
	ofstream fout;	// файл для записи в него данных
	fout.open("matr.txt");
	// считываем время выполнения
	double time;
	fread(&time, sizeof(time), 1, stdin);
	cout << time << endl;
	// считываем размер матрицы
	fread(&N, sizeof(N), 1, stdin);
	cout << N << endl;
	cout << fixed << setprecision(4);
	C = new double[N * N];
	//считываем результирующую матрицу
	fread(C, sizeof(*C), N * N, stdin);
	if (N < 11) {
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				std::cout << C[i*N + j] << " ";
			}
			std::cout << endl;
		}
	}
	// записываем время выполнения в txt файл
	fout << time << endl;
	// записываем размер матрицы в txt файл
	fout << N << endl;
	// записываем матрицу в txt файл
	fout << fixed << setprecision(4);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fout << C[i*N + j] << " ";
		}
		fout << endl;
	}
	fout.close();
	delete[] C;
	return 0;
}