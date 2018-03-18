
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath> 
#include <fstream>

using namespace std;

int main(int argc, char * argv[])
{// в командной строке 1 аргумент - входные данные, 2 - выходные, 3 - эталонный результат
	errno_t input, input1, input2;
	FILE *bui;
	FILE *buo;
	FILE *bua;

	if (argc > 1) {
		char* matrin = new char[20];
		matrin = argv[1];
		char* matrout = new char[20];
		matrout = argv[2];
		char* answer = new char[20];
		answer = argv[3];
		cout << matrin << " " << matrout << " " << answer << endl;

		input = fopen_s(&bui, matrin, "rb");
		input1 = fopen_s(&buo, matrout, "rb");
		input2 = fopen_s(&bua, answer, "rb");
	}
	else {
		input = fopen_s(&bui, "matr.in", "rb");
		input1 = fopen_s(&buo, "matr.out", "rb");
		input2 = fopen_s(&bua, "answer.ans", "rb");
	}

	ofstream fout;	// файл для записи в него результата
	fout.open("result.txt");

	if (input != 0 || input1 != 0 || input2 != 0)
	{
		fout << "PE. File" << endl;
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
	//fread(&res_time, sizeof(res_time), 1, buo);
	int n;
	fread(&n, sizeof(n), 1, buo);
	fread(res, sizeof(*res), N*N, buo);
	//cout << res_time << endl;
	//cout << n << endl;

	// считываем время выполнения и результирующую матрицу из эталонного файла
	//fread(&ans_time, sizeof(res_time), 1, bua);
	fread(&n, sizeof(n), 1, bua);
	fread(ans, sizeof(*ans), N*N, bua);
	//cout << ans_time << endl;
	//cout << n << endl;
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

	// Вычисляем ошибку   
	double diff = 0.0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			double t = double(ans[i*N + j] - res[i*N + j]);
			diff += double(t * t);
		}
	}
	if (diff < 1e-5)
	{
		fout << "AC. Numbers are equal." << endl;
		cout << "AC. Numbers are equal." << endl;
	}
	else
	{
		fout << "WA. Output is not correct." << endl;
		cout << "WA. Output is not correct." << endl;
	}
	//fout << res_time*1e7 << endl;
	//fout << res_time << endl;

	fclose(buo);
	fclose(bui);
	fclose(bua);
	fout.close();
	return 0;
}