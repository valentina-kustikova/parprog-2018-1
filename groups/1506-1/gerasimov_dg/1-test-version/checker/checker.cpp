#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath> 
#include <fstream>
#include <string>

using namespace std;

// Используется для взаимодействия с тестирующей системой 
//////////////////////////////////////////////////////////////////////////////////////////// 
/*
// Checker может устанавливать вот эти три вердикта:
AC = Accepted = Решение выдаёт корректный результат на данном тесте
WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте
PE = Presentation Error = Ошибка формата выходных данных

// Остальные вердикты checker не может устанавливать
NO = No verdict = Вердикт отсутствует
CE = Compilation Error = Ошибка компиляции
ML = Memory Limit Exceeded = Превышено ограничение по памяти
TL = Time Limit Exceeded = Превышено ограничение по времени работы
RE = Runtime Error = Ошибка времени исполнения программы
IL = Idle Limit Exceeded = Превышено время простоя (бездействия) программы
DE = Deadly Error = Ошибка тестирующей системы */

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

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
		fwrite(&t, sizeof(t), 1, bur);
	}

	// Сообщить тестирующей системе, что решение получило один из вердиктов verdict   
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT);
		fwrite(&v, sizeof(v), 1, bur);
	}

	// Написать сообщение от checker'a пользователю.    
	// Например, что решение верное, или неверное.    
	// Использовать только латинские буквы и знаки препинания   
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE);
		int l = str.size();
		fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}

	// Сообщить тестирующей системе время работы программы участника,   
	// вычисленное с помощью before_code  
	// x имеет размерность 100 нс = 10 ^ (-7) сек 
	void write_time(long long x)
	{
		write_type(ext_cls::TIME);
		fwrite(&x, sizeof(x), 1, bur);
	}

	// Сообщить тестирующей системе, память затребованную программой участника   
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY);
		fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;
//////////////////////////////////////////////////////////////////////////////////////////// 

int main(int argc, char * argv[])
{
	// Открываем файл входных данных, файл выходных данных и ответ участника   
	errno_t err, err1, err2;
	FILE *bui;
	FILE *buo;
	FILE *bua;

	if (argc > 1) {
		char* matrin = argv[1];
		char* matrout = argv[2];
		char* answer = argv[3];

		err = fopen_s(&bui, matrin, "rb");
		err1 = fopen_s(&buo, matrout, "rb");
		err2 = fopen_s(&bua, answer, "rb");
	}
	else {
		err = fopen_s(&bui, "matr.in", "rb");
		err1 = fopen_s(&buo, "matr.out", "rb");
		err2 = fopen_s(&bua, "answer.ans", "rb");
	}
	if (err != 0 || err1 != 0 || err2 != 0)
	{
		checker_result.write_message("PE. Presentation Error.");
		checker_result.write_verdict(verdict::PE);
		return 0;
	}
	int n;
	// Считываем размерность матриц  
	fread(&n, sizeof(n), 1, bui);

	// Выделяем память для матрицы ответа жюри и ответа участника 
	double *ans = new double[n*n], *res = new double[n*n];
	double ans_time, res_time;

	// считываем время выполнения программы и результирующую матрицу
	int nn;	// размер матрицы из файла, чтобы правильно считался файл
	fread(&res_time, sizeof(res_time), 1, buo);
	fread(&nn, sizeof(nn), 1, buo);
	fread(res, sizeof(*res), n * n, buo);

	// Считываем время работы программы и матрицу жюри   
	fread(&ans_time, sizeof(ans_time), 1, bua);
	fread(&nn, sizeof(nn), 1, bua);
	fread(ans, sizeof(*ans), n * n, bua);

	// Вычисляем ошибку, как квадрат нормы разности решений  
	double diff = 0.0;
	for (int i = 0; i < n * n; i++)
	{
		double t = double(ans[i] - res[i]);
		diff += double(t * t);
	}
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
	fclose(bua);
	fclose(buo);
	fclose(bui);
	delete[] res;
	delete[] ans;

	return 0;
}