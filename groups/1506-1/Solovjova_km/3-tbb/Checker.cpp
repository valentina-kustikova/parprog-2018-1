#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

#include <omp.h>
using namespace std;
//
////////////////////////////////////////////////////////////////////////////////////////////
/*
// Checker:
AC = Accepted =
WA = Wrong Answer =
PE = Presentation Error =
//
NO = No verdict =
CE = Compilation Error =
ML = Memory Limit Exceeded =
TL = Time Limit Exceeded =
RE = Runtime Error =
IL = Idle Limit Exceeded =
DE = Deadly Error =
*/
enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };
class result
{
private:
	FILE * bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "r");
		else bur = fopen("result.txt", "w");
	}
	~result()
	{
		fclose(bur);
	}
	void write_type(ext_cls t)
	{
		//fwrite(&t, sizeof(t), 1, bur);
	}
	//
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT);
		//fwrite(&v, sizeof(v), 1, bur);
	}
	//
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE);
		int l = str.size();
		//fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	//

	void write_time(long long x)
	{
		write_type(ext_cls::TIME);
		fwrite(&x, sizeof(x), 1, bur);
	}
	//
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY);
		fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;

int main(int argc, char *argv[]) {
	// Открываем файл входных данных, файл выходных данных и ответ участника  
	char t[250];
	char *nameFileData = new char[250];
	char *nameFileGenTest = new char[250];
	char* nameFileAnswer = new char[250];
	if (argc > 1)
	{
		//nameFileAnswer = argv[argc - 1];
		//std::cout << nameFileAnswer << std::endl;
		//nameFileGenTest = argv[argc - 2];
		//std::cout << nameFileGenTest << std::endl;
		//nameFileData = argv[argc - 3];
		//std::cout << nameFileData << std::endl;
	}
	else
	{
		//std::cout << "nameFileData" << std::endl;
		//std::cin.getline(nameFileData, 250);
		//std::cout << "nameFileGenTest" << std::endl;
		//std::cin.getline(nameFileGenTest, 250);
		std::cout << "nameFileAnswer" << std::endl;
		std::cin.getline(nameFileAnswer, 250);
	}
	


	int N;
	freopen("test", "rb", stdin);
	// Считываем размерность массива  
	fread(&N, sizeof(N), 1, stdin);
	fclose(stdin);

	// Выделяем память для результата жюри и ответа участника   
	double	res, ans;
	double ans_time, time;

	// Считываем время работы программы участника и матрицу участника   
	freopen(nameFileAnswer, "rb", stdin);
	fread(&time, sizeof(time), 1, stdin);
	fread(&res, sizeof(res), 1, stdin);
	fclose(stdin);

	// Считываем время работы программы и матрицу жюри   
	freopen("ans_res.ans", "rb", stdin);
	fread(&ans_time, sizeof(ans_time), 1, stdin);
	fread(&ans, sizeof(ans), 1, stdin);
	fclose(stdin);

	// Вычисляем ошибку, как квадрат нормы разности решений   
	double diff = 0.0;

	diff = abs(ans - res);
	///std::cout << diff;
	// Проверяем, что ошибка мала, тогда сообщаем, что решение корректно, иначе - некорректно.   
	if (diff < 5)
	{
		checker_result.write_message("AC. Numbers are equal. ");
		checker_result.write_verdict(verdict::AC);
	}
	else
	{
		checker_result.write_message("WA. Output is not correct. ");
		checker_result.write_verdict(verdict::WA);
	}
	 //Записываем время в правильной размерности (интервалы по 100 нс = 10 ^ (-7) сек).   
	//checker_result.write_time(time * 1e7);
	//std::cout << time * 1e7<<std::endl;
	//std::cout << "ans = " << ans << " res = " << res;
	string ss = to_string(time);
	checker_result.write_message(ss);

	std::cout << time;
	delete[] nameFileData;
	delete[] nameFileGenTest;
	//delete[] nameFileAnswer;
	return 0;
}
