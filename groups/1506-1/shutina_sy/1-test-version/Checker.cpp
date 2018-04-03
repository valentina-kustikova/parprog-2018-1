// Checker.cpp: определяет точку входа для консольного приложения.
//
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

class result
{
private:
	FILE *bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "r"); else bur = fopen("result.txt", "w");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) { fwrite(&t, sizeof(t), 1, bur); }
	// Сообщить тестирующей системе, что решение получило один из вердиктов verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fwrite(&v, sizeof(v), 1, bur);
	}
	// Написать сообщение от checker'a пользователю.
	// Например, что решение верное, или неверное.
	// Использовать только латинские буквы и знаки препинания
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	// Сообщить тестирующей системе время работы программы участника,	
	// x имеет размерность 100 нс = 10 ^ (-7) сек
	void write_time(long long x)
	{
		write_type(ext_cls::TIME); fwrite(&x, sizeof(x), 1, bur);
	}
	// Сообщить тестирующей системе, память затребованную программой участника
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;


int main(int argc, char* argv[])
{
	if (argc > 2) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "rb", stdout);
	}
	else {		
		// Открываем файл входных данных, файл выходных данных и ответ участника
		freopen("array.in", "rb", stdin);
		freopen("array.out", "rb", stdout);		
	}

	int n;
	// Считываем размерность 
	fread(&n, sizeof(n), 1, stdout);
	fread(&n, sizeof(n), 1, stdin);
	// Выделяем память для массива ответа жюри и ответа участника
	double * ans = new double[n], *res = new double[n];
	/*double ans_time, res_time;*/

	// Считываем время работы программы участника и массив участника

	long long res_time;
	
	fread(res, sizeof(*res), n, stdout);
	fread(&res_time, sizeof(res_time), 1, stdout);
	// Считываем  массив 
	fread(ans, sizeof(*ans), n, stdin);
	sort(ans, ans + n);
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

