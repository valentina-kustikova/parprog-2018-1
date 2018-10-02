#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <random>
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <cmath> 
#include <cstdlib>
#include <fstream>
#include <windows.h> 
#include <ctime>
#include<conio.h>
#include <omp.h>


bool isDigit(char el)
{
	std::string digits = "123456789xy";
	int len = digits.length();
	for (int i = 0; i<len; i++)
	{
		if (el == digits[i])
			return true;
	}
	return false;
}

bool isSign(char el)
{
	std::string digits = "/+^*-()";
	int len = digits.length();
	for (int i = 0; i<len; i++)
	{
		if (el == digits[i])
			return true;
	}
	return false;
}

int priorityСheck(char one)
{
	int pr;
	if (one == '+' or one == '-')
		pr = 2;
	if (one == '*' or one == '/' or one == '^')
		pr = 3;
	if (one == '(' or one == ')')
		pr = 1;
	return pr;
}

//Перевод в польскую запись
std::vector<char> OPZ(std::string str)
{
	std::stack <char> stack1;
	std::vector<char> types;
	int pr1;
	//int pr2;
	//Создаем обратную польскую запись
	for (int i = 0; str[i] != 0; i++)
	{
		if (isDigit(str[i]))
		{
			types.push_back(str[i]);
			continue;
		}
		if (isSign(str[i]))
		{
			pr1 = priorityСheck(str[i]); //Приоритет текущего символа
										 //std::cout << "i = " << i << "pr = " << pr1;
			if (stack1.empty() or (priorityСheck(stack1.top()) < pr1))
			{
				stack1.push(str[i]);
				continue;
			}
			if (str[i] == ')')
			{
				while (stack1.top() != '(')
				{

					types.push_back(stack1.top());
					stack1.pop();
				}
				stack1.pop();
				continue;
			}
			else
			{
				while (priorityСheck(stack1.top()) >= pr1)
				{
					types.push_back(stack1.top());
					stack1.pop();
					if (stack1.empty())
						break;
				}
				stack1.push(str[i]);
			}
		}
		else
		{
			std::cout << "ERROR!";
			exit;
		}
	}
	while (!stack1.empty())
	{
		types.push_back(stack1.top());
		stack1.pop();
	}

	return types;
}

double computation(double el1, double el2, char el)
{
	if (el == '+')
		return el1 + el2;
	if (el == '-')
		return el1 - el2;
	if (el == '*')
		return el1 * el2;
	if (el == '/')
		return el1 / el2;
	if (el == '^')
		return pow(el1, el2);
	else
		return 0;
}

double conversion(char el, double x, double y)
{
	if (el == 'x')
		return x;
	if (el == 'y')
		return  y;
	else
	{
		return el - '0';
	}
}

//Парсинг строки
int ParsingStr(std::string str, double x, double y)
{
	std::vector<char> types;
	types = OPZ(str);
	//for (char n : types)
	//{
	//	std::cout << n;
	//}

	std::stack<double> stackDig;
	for (char n : types)
	{
		double el1, el2;
		if (isDigit(n))
		{
			double el = conversion(n, x, y);
			stackDig.push(el);
		}
		else
		{
			//el1 = conversion(stackDig.top(), x, y);
			el1 = stackDig.top();
			stackDig.pop();
			//el2 = conversion(stackDig.top(), x, y);
			el2 = stackDig.top();
			stackDig.pop();
			double r = computation(el1, el2, n);
			stackDig.push(r);
		}
	}
	double res = stackDig.top();
	//std::cout << res;
	return static_cast< int >(res);
}

double methodCell(double xa, double xb, int n, double ya, double yb, std::string str) {
	double xh((xb - xa) / n);
	double yh((yb - ya) / n);
	double s(0);
	for (double xi = xa + xh / 2; xi < xb; xi += xh) {
		for (double yi = ya + yh / 2; yi < yb; yi += yh) {
			s += ParsingStr(str, xi, yi);
		}
	}
	return xh * yh * s;
}

typedef std::pair<double, double> point;
//Генерируем число
double random(double min, double max)
{
	std::uniform_real_distribution<double>distribution(min, max);
	std::random_device rd;
	std::mt19937 generator(rd());
	return distribution(generator);
}
//генерация точки в заданном диапозоне
point CalculateRandomPoint(double leftBorderX, double leftBorderY, double rightBorderX, double rightBorderY)
{
	return std::make_pair(random(leftBorderX, rightBorderX), random(leftBorderY, rightBorderY));
}
//печать точки
//void printPoint(point pointToPrint) {
//	std::cout << "x = " << pointToPrint.first << " y = " << pointToPrint.second << "\n";
//}

//конвертация строки в число
void convertInDig(std::string n, int &s)
{
	std::istringstream ist(n);
	ist >> s;
}


int main(int argc, char * argv[])
{
	//argv[argc - 1] - функция
	//argv[argc - 2] - количество итераций
	//argv[argc - 3] - имя файла
	std::string str;
	std::string n;

	int sizeIter;
	std::cout << "Function.sizeIter.NameFile" << std::endl;
	char t[250];
	char *nameFile = new char[250];

	//Ввод данных 
	if (argc > 1)
	{
		str = argv[argc - 2];
		n = argv[argc - 1];
		//nameFile = argv[argc - 1];
		//std::cout << str << std::endl;
		//std::cout << n << std::endl;
		//std::cout << nameFile << std::endl;
	}
	else
	{
		str = "(x+y)*3+2*x+y";
		n = "1000";
		//std::cout << "Input str" << std::endl;
		//std::cin.getline(nameFile, 250);
	}

	OemToChar(nameFile, t);

	//конвертация в число
	convertInDig(n, sizeIter);
	//std::cout << "iter = " << sizeIter;

	//диапозон точки
	std::uniform_real_distribution<double>distribution(1, 10);
	std::random_device rd;
	std::mt19937 generator(rd());

	char index[10];
	//генерация Х
	double x1 = distribution(generator);
	double x2 = distribution(generator);
	if (x1 > x2) std::swap(x1, x2);
	//std::cout << "x1 = " << x1 << " x2= " << x2;

	//генерация Y
	double y1 = distribution(generator);
	double y2 = distribution(generator);
	if (y1 > y2) std::swap(y1, y2);
	//std::cout << " y1 = " << y1 << " y2= " << y2 << std::endl;

	//запись данных в файл
	freopen("test", "wb", stdout);
	fwrite(&sizeIter, sizeof(sizeIter), 1, stdout);
	fwrite(&x1, sizeof(x1), 1, stdout);
	fwrite(&x2, sizeof(x2), 1, stdout);
	fwrite(&y1, sizeof(y1), 1, stdout);
	fwrite(&y2, sizeof(y2), 1, stdout);

	double *functionValues = new double[sizeIter];
	double sum = 0.0;
	for (int i = 0; i < sizeIter; i++)
	{
		auto newPoint = CalculateRandomPoint(x1, y1, x2, y2);
		double funcValue = ParsingStr(str, newPoint.first, newPoint.second);
		functionValues[i] = funcValue;
		//std::cout << "res = " << functionValues[i];
		sum += funcValue;
	}
	fwrite(functionValues, sizeof(*functionValues), sizeIter, stdout);

	fclose(stdout);

	//Записываем результат, с которым будем сравнивать ответы
	//Результат функции 
	double start = omp_get_wtime();
	double res_ans = methodCell(x1, x2, 1000, y1, y2, str);
	double end = omp_get_wtime();
	double ans_time = end - start;
	//std::cout << res_ans;
	freopen("ans_res.ans", "wb", stdout);
	fwrite(&ans_time, sizeof(ans_time), 1, stdout);
	fwrite(&res_ans, sizeof(res_ans), 1, stdout);
	fclose(stdout);

	delete[]functionValues;
	delete[] nameFile;
	//std::cout << " res = " << funcValue;
	return 0;
}
