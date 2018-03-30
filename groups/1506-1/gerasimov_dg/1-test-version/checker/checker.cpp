#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath> 
#include <fstream>

using namespace std;

int main(int argc, char * argv[])
{// в командной строке 1 аргумент - входные данные, 2 - выходные, 3 - эталонный результат
	errno_t err, err1, err2;
	FILE *bui;
	FILE *buo;
	FILE *bua;

	if (argc > 1) {
		char* matrin = argv[1];
		char* matrout = argv[2];	
		char* answer = argv[3];

		cout << matrin << " " << matrout << " " << answer << endl;

		err = fopen_s(&bui, matrin, "rb");
		err1 = fopen_s(&buo, matrout, "rb");
		err2 = fopen_s(&bua, answer, "rb");
	}
	else {
		err = fopen_s(&bui, "matr.in", "rb");
		err1 = fopen_s(&buo, "matr.out", "rb");
		err2 = fopen_s(&bua, "answer.ans", "rb");
	}

	ofstream fout;	// файл для записи в него результата
	fout.open("result.txt");

	if (err != 0 || err1 != 0|| err2 != 0)
	{
		cout << "There are no all input files." << endl;
		fout << "PE. Presentation Error" << endl;
		fout << 9 << endl;
		return 0;
	}

	// считываем размерность матриц
	int N;
	fread(&N, sizeof(N), 1, bui);

	//Выделяем память для матриц
	double *ans = new double[N*N];
	double *res = new double[N*N];
	double ans_time = 0;
	double res_time = 0;

	// считываем время выполнения программы и результирующую матрицу
	fread(&res_time, sizeof(res_time), 1, buo);
	int n;
	fread(&n, sizeof(n), 1, buo);
	fread(res, sizeof(*res), N*N, buo);
	cout << res_time << endl;
	cout << n << endl;

	// считываем время выполнения и результирующую матрицу из эталонного файла
	fread(&ans_time, sizeof(res_time), 1, bua);
	fread(&n, sizeof(n), 1, bua);
	fread(ans, sizeof(*ans), N*N, bua);
	cout << ans_time << endl;
	cout << n << endl;
	cout << fixed << setprecision(4);
	if (N < 11) {
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << res[i*N + j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << ans[i*N + j] << " ";
			}
			cout << endl;
		}
	}
	// Вычисляем ошибку, как квадрат нормы разности решений   
	double diff = 0.0;
	for (int i = 0; i < N*N; i++)
	{
		double t = double(ans[i] - res[i]);
		diff += double(t * t);
	}
	if (diff < 1e-5)
	{
		fout << "AC. Numbers are equal." << endl;
		fout << 2 << endl;
		cout << "AC. Numbers are equal." << endl;
	}
	else
	{
		fout << "WA. Output is not correct." << endl;
		fout << 3 << endl;
		cout << "WA. Output is not correct." << endl;
	}
	// записываем время
	//fout << res_time*1e7 << endl;
	fout << res_time << endl;

	fclose(buo);
	fclose(bui);
	fclose(bua);
	fout.close();

	delete[] res;
	delete[] ans;
	return 0;
}