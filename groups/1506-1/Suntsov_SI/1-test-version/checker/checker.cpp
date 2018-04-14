//checker.cpp: определяет точку входа для консольного приложения.

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath> 
#include <fstream>

using namespace std;

/*// Checker может устанавливать вот эти три вердикта:
AC = Accepted = Решение выдаёт корректный результат на данном тесте
WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте
PE = Presentation Error = Ошибка формата выходных данных
*/

enum verdict { NO = 1, AC, WA, PE };

class result
{
private:
	FILE * bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };

	result(bool read = false)
	{
		if (read)
		{
			fopen_s(&bur, "result.txt", "r");
		}
		else
		{
			fopen_s(&bur, "result.txt", "w");
		}
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t)
	{
		int t1;
		t1 = (int)t;
		//fwrite(&t, sizeof(t), 1, bur);
		fprintf(bur, " %d ", t1);
	}

	// Сообщить тестирующей системе, что решение получило один из вердиктов verdict   
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT);
		int v1 = (int)v;
		fprintf(bur, " %d ", v1);
		//fwrite(&v, sizeof(v), 1, bur);
	}

	// Написать сообщение от checker'a пользователю.    
	// Например, что решение верное, или неверное.    
	// Использовать только латинские буквы и знаки препинания   
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE);
		int l = str.size();
		//fwrite(&l, sizeof(l), 1, bur);
		fprintf(bur, " %d ", (int)l);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}

	// Сообщить тестирующей системе время работы программы участника,   
	// вычисленное с помощью before_code  
	// x имеет размерность 100 нс = 10 ^ (-7) сек 
	void write_time(long long x)
	{
		write_type(ext_cls::TIME);
		int x2 = (int)x;
		fprintf(bur, " %d ", x2);
		//fwrite(&x, sizeof(x), 1, bur);
	}

	// Сообщить тестирующей системе, память затребованную программой участника   
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY);
		int x1 = (int)x;
		fprintf(bur, " %d ", x1);
		//fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;



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
		checker_result.write_message("PE. Presentation Error.");
		checker_result.write_verdict(verdict::PE);
		return 0;
	}

	// считываем размерность матриц
	int N;
	fread(&N, sizeof(N), 1, bui);
	//int N = 8;

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
		checker_result.write_message("AC. Numbers are equal.");
		checker_result.write_verdict(verdict::AC);
		cout << "AC. Numbers are equal." << endl;
	}
	else
	{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
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