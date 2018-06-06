#include <iostream>
#include <stack>
#include <deque>
#include <math.h>
using namespace std;

int prior(char a) {
	switch (a)
	{
	case '(': return 0; break;
	case ')': return 1; break;
	case '+': return 2; break;
	case '-': return 2; break;
	case '*': return 3; break;
	case '/': return 3; break;
	case '^': return 4; break;
	}
}

deque<string> OPZ(string s) {	//Реализация ОПЗ.
	std::string str = s;
	std::stack<char> StackOper;
	std::deque<std::string> Stack;
	int strLen = str.length();

	//Удаляем лишние пробелы
	for (int i = 0; i < strLen; i++) {
		if (str[strLen] == ' ') {
			for (int j = i; j < strLen - 1; j++)
				str[j] = str[j + 1];
			strLen--;
		}
	}

	for (int i = 0; i < strLen; i++) {
		if (i < strLen - 5) {
			if (str[i] == 's' && str[i + 1] == 'i' && str[i + 2] == 'n') {
				if (str[i + 3] == '(' && str[i + 5] == ')') {
					int k = i;
					std::string temp = "";
					while (k < i + 6) {
						temp += str[k];
						k++;
					}
					Stack.push_back(temp);
					i += 6;
				}
				else {
					cout << "Некорректный ввод синуса" << endl;
					break;
				}
			}
			else if (str[i] == 'c' && str[i + 1] == 'o' && str[i + 2] == 's') {
				if (str[i + 3] == '(' && str[i + 5] == ')') {
					int k = i;
					std::string temp = "";
					while (k < i + 6) {
						temp += str[k];
						k++;
					}
					Stack.push_back(temp);
					i += 6;
				}
				else {
					cout << "Некорректный ввод косинуса" << endl;
					break;
				}
			}
			else if (str[i] == 'l' && str[i + 1] == 'o' && str[i + 2] == 'g') {
				if (str[i + 3] == '(' && str[i + 5] == ')') {
					int k = i;
					std::string temp = "";
					while (k < i + 6) {
						temp += str[k];
						k++;
					}
					Stack.push_back(temp);
					i += 6;
				}
				else {
					cout << "Некорректный ввод логарифма" << endl;
					break;
				}
			}
			else if (str[i] == 't' && str[i + 1] == 'a' && str[i + 2] == 'n') {
				if (str[i + 3] == '(' && str[i + 5] == ')') {
					int k = i;
					std::string temp = "";
					while (k < i + 6) {
						temp += str[k];
						k++;
					}
					Stack.push_back(temp);
					i += 6;
				}
				else {
					cout << "Некорректный ввод тангенса" << endl;
					break;
				}
			}
			else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || isdigit(str[i])) {
				std::string temp = "";
				temp += str[i];
				Stack.push_back(temp);
			}
		}
		else  if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || isdigit(str[i])) {
			std::string temp = "";
			temp += str[i];
			Stack.push_back(temp);
		}

		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^') {
			if (StackOper.empty())
				StackOper.push(str[i]);
			else {
				if (prior(StackOper.top()) < prior(str[i]))
					StackOper.push(str[i]);
				else {
					while (!StackOper.empty() && prior(StackOper.top()) >= prior(str[i])) {
						std::string temp = "";
						temp += StackOper.top();
						Stack.push_back(temp);
						StackOper.pop();
					}
					StackOper.push(str[i]);
				}
			}
		}

		if (str[i] == '(')
			StackOper.push(str[i]);

		if (str[i] == ')') {
			if (StackOper.empty()) {
				cout << "Непарные скобки" << endl;
				break;
			}

			while (StackOper.top() != '(') {
				if (StackOper.empty()) {
					cout << "Непарные скобки" << endl;
					break;
				}
				std::string temp = "";
				temp += StackOper.top();
				Stack.push_back(temp);
				StackOper.pop();
			}
			StackOper.pop();
		}
	}

	while (!StackOper.empty()) {
		if (StackOper.top() == '(') {
			cout << "Непарные скобки" << endl;
			break;
		}
		std::string temp = "";
		temp += StackOper.top();
		Stack.push_back(temp);
		StackOper.pop();
	}

	return Stack;
}

double Result(std::deque<string> Stack, double lim) {	//Вычисление результата.
	std::stack<double> Value;
	float CheckVal = 0;

	while (!Stack.empty()) {
		std::string PartOfInteg = "";
		PartOfInteg += Stack.front();
		Stack.pop_front();

		if (PartOfInteg == "sin(x)")
			Value.push(sin(lim));
		else if (PartOfInteg == "cos(x)")
			Value.push(cos(lim));
		else if (PartOfInteg == "tan(x)")
			Value.push(tan(lim));
		else if (PartOfInteg == "log(x)")
			Value.push(log10(lim));
		else if (PartOfInteg == "+" || PartOfInteg == "-" || PartOfInteg == "*" || PartOfInteg == "/" || PartOfInteg == "^") {
			double op2 = Value.top();
			Value.pop();
			if (!Value.empty()) {
				double op1 = Value.top();
				Value.pop();
				switch (PartOfInteg[0])
				{
				case '*':  Value.push(op1*op2); break;
				case '/':  Value.push(op1 / op2); break;
				case '-':  Value.push(op1 - op2); break;
				case '+':  Value.push(op1 + op2); break;
				case '^':
					double temp = 1;
					for (int i = 0; i < op2; i++)
						temp = temp * op1;
					Value.push(temp); break;
				}
			}
		}
		else {
			char* p;
			if (strtod(PartOfInteg.c_str(), &p))
				Value.push(strtod(PartOfInteg.c_str(), &p));
			else
				Value.push(lim);
		}
	}

	return Value.top();
}

double fun(double x)
{
	return(sin(x) + cos(x));
}

double Rect_Meth_(double a, double b, double n, string f) {
	double h = (b - a) / n;
	double sum = 0.0;

	for (int i = 0; i < n; i++)
		sum += Result(OPZ(f), a + i * h);

	return round((h * sum) * 1000) / 1000;
}