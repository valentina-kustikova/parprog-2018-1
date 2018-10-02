
//before_code.cpp – чтение данных и точка входа в программу
//Требования
//Программа на языке C++ должна реализовывать функцию со следующим заголовком :
//(double *functionValues, double x1, double y1, double x2, double y2, int N);
//Формат входных данных
//Функция получает в аргументах следующие переменные :
// double *functionValues - массив значений функции
// double x1 - левая граница Х
// double x2 - правая граница Х
// double y2 - правая граница Y
// double x2 - правая граница Х
//Формат выходных данных
//Вычисленный результат - это число типа double

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <omp.h>
#include <random>
#include<iostream>
#include <fstream>
#include<conio.h>
#include<string>
#include<cmath>
#include <ctime>
#include <sstream>
#include <chrono>
using namespace std;


double MonteCarloOMP(double *functionValues, double x1, double y1, double x2, double y2, int N);




//конвертация строки в число
void convertInDig(std::string n, int &s)
{
	std::istringstream ist(n);
	ist >> s;
}

int main(int argc, char * argv[])
{
	//Первый элемент - имя 2 файла, второй элемент - имя первого файла
	char t[250];
	char *nameFile = new char[250];
	char *nameFile2 = new char[250];
	int num_threads;
	string num_threads_char;
	if (argc > 1)
	{
		
		//nameFile = argv[argc - 3];
		//std::cout << nameFile << std::endl;
		//nameFile2 = argv[argc - 2];
		//std::cout << nameFile2 << std::endl;
		num_threads_char = argv[argc - 1];
		//std::cout << nameFile2 << std::endl;
	}
	else
	{
		//std::cout << "Input nameFile" << std::endl;
		//std::cin.getline(nameFile, 250);
		//std::cout << "Input nameFile2" << std::endl;
		//std::cin.getline(nameFile2, 250);
		num_threads_char = "1";
	}
	convertInDig(num_threads_char, num_threads);
	//Входные данные

	double x1;
	double y1;
	double x2;
	double y2;
	int N;
	double* functionValues;

	//открываем файл генерации тестов
	freopen("test", "rb", stdin);

	//Читаем данные из файла
	fread(&N, sizeof(N), 1, stdin);
	fread(&x1, sizeof(x1), 1, stdin);
	fread(&x2, sizeof(x2), 1, stdin);
	fread(&y1, sizeof(y1), 1, stdin);
	fread(&y2, sizeof(y2), 1, stdin);

	//массив значений
	functionValues = new double[N];
	fread(functionValues, sizeof(*functionValues), N, stdin);
	fclose(stdin);
	//omp_set_num_threads(1);

	omp_set_num_threads(num_threads);
	//считатем время
	auto begin = chrono::high_resolution_clock::now();
	double res_OpenMP = MonteCarloOMP(functionValues, x1, y1, x2, y2, N);
	//double end = omp_get_wtime();
	auto end = chrono::high_resolution_clock::now();
	double time_OpenMP = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

	//записываем данные в файл
	freopen("omp.ans", "wb", stdout);
	fwrite(&time_OpenMP, sizeof(time_OpenMP), 1, stdout);
	fwrite(&res_OpenMP, sizeof(res_OpenMP), 1, stdout);
	//std::cout << "time = " << time_ser;
	//std::cout << "res = " << res_ser;
	fclose(stdout);

	//std::cout << "x1 =" << x1 << " x2= " << x2 << " y1= " << y1 << " y2= " << y2;
	//std::cout << "time = " << time;
	//std::cout << "res = " << time;
	delete[]functionValues;
	delete[]nameFile;
	delete[]nameFile2;

	return 0;
}