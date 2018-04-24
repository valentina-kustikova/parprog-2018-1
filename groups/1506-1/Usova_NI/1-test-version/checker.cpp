
#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>
#include<string>
#include<algorithm>
#include <iostream>
using namespace std;

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

/*
// Checker может устанавливать вот эти три вердикта:
AC = Accepted = Решение выдаёт корректный результат на данном тесте
WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте
*/
void Gauss(double **a, double *y, double *x, int n)
{
	double max;
	int k, index;
	const double eps = 0.00001;  // точность	
	k = 0;
	while (k < n)
	{
		// Поиск строки с максимальным a[i][k]
		max = abs(a[k][k]);
		index = k;
		for (int i = k + 1; i < n; i++)
		{
			if (abs(a[i][k]) > max)
			{
				max = abs(a[i][k]);
				index = i;
			}
		}
		// Перестановка строк
		if (max < eps)
		{
			// нет ненулевых диагональных элементов
			/*cout << "Решение получить невозможно из-за нулевого столбца ";
			cout << index << " матрицы A" << endl;*/
			return;
		}
		for (int j = 0; j < n; j++)
		{
			double temp = a[k][j];
			a[k][j] = a[index][j];
			a[index][j] = temp;
		}
		double temp = y[k];
		y[k] = y[index];
		y[index] = temp;
		// Нормализация уравнений
		for (int i = k; i < n; i++)
		{
			double temp = a[i][k];
			if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
			for (int j = 0; j < n; j++)
				a[i][j] = a[i][j] / temp;
			y[i] = y[i] / temp;
			if (i == k)  continue; // уравнение не вычитать само из себя
			for (int j = 0; j < n; j++)
				a[i][j] = a[i][j] - a[k][j];
			y[i] = y[i] - y[k];
		}
		k++;
	}
	// обратная подстановка
	for (k = n - 1; k >= 0; k--)
	{
		x[k] = y[k];
		for (int i = 0; i < k; i++)
			y[i] = y[i] - a[i][k] * x[k];
	}
}

class result
{
private:
	FILE *bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "rb"); else bur = fopen("result.txt", "wb");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) { fprintf(bur, "%d ", t); }
	// Сообщить тестирующей системе, что решение получило один из вердиктов verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fprintf(bur, "%d ", v);
	}
	// Написать сообщение от checker'a пользователю.
	// Например, что решение верное, или неверное.
	// Использовать только латинские буквы и знаки препинания
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fprintf(bur, "%d ", l);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	// Сообщить тестирующей системе время работы программы участника,	
	// x имеет размерность 100 нс = 10 ^ (-7) сек
	void write_time(double x)
	{
		write_type(ext_cls::TIME); fprintf(bur, "%f ", x);
	}
	// Сообщить тестирующей системе, память затребованную программой участника
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fprintf(bur, "%f ", x);
	}
} checker_result;


int main(int argc, char* argv[])
{
	if (argc > 2) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "rb", stdout);
	}
	else {
		// Открываем файл входных данных и ответ участника
		freopen("array.in", "rb", stdin);
		freopen("array.out", "rb", stdout);
	}

	int n;
	// Считываем размерность 
	fread(&n, sizeof(n), 1, stdout);
	fread(&n, sizeof(n), 1, stdin);
	// Выделяем память для массива ответа жюри и ответа участника
	double **A = new double*[n];
	for (int i = 0; i < n; i++) {
		A[i] = new double[n];
		fread(A[i], sizeof(*A[i]), n, stdin);
	}
	double *b = new double[n];
	fread(b, sizeof(*b), n, stdin);
	double *ans = new double[n];
	Gauss(A, b, ans, n);

	// Считываем время работы программы участника и массив участника
	double res_time, *res = new double[n];
	fread(res, sizeof(*res), n, stdout);
	fread(&res_time, sizeof(res_time), 1, stdout);

	// Вычисляем ошибку, как квадрат нормы разности решений
	double diff = 0.0;
	for (int i = 0; i < n; i++)
		diff += (ans[i] - res[i]) * (ans[i] - res[i]);
	// Проверяем, что ошибка мала, тогда сообщаем, что решение корректно, иначе - некорректно.	
	if (diff < 1e-6)
	{
		checker_result.write_message("AC. Numbers are equal.");
		checker_result.write_verdict(verdict::AC);
	}
	else
	{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
	}
	// Записываем время в правильной размерности (интервалы по 100 нс = 10 ^ (-7) сек).

	checker_result.write_time(res_time * 1e7);

	delete[] ans;
	delete[] res;
	return 0;
}
