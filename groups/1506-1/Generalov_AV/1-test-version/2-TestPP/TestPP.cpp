#include "../../include/dataStruct.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

vector<string> X{ "x", "x^2", "x^3", "x^4", "x^5", "x^6", "x^7", "x^8", "x^9", "x^15" };
vector<string> SIN{ "sin(x)", "sin(x)^2", "sin(x)^3", "sin(x)^4", "sin(x)^5",
"sin(x)^6", "sin(x)^7", "sin(x)^8", "sin(x)^9", "sin(x)^15" };
vector<string> COS{ "cos(x)", "cos(x)^2", "cos(x)^3", "cos(x)^4", "cos(x)^5",
"cos(x)^6", "cos(x)^7", "cos(x)^8", "cos(x)^9", "cos(x)^15" };
vector<string> TAN{ "tan(x)", "tan(x)^2", "tan(x)^3", "tan(x)^4", "tan(x)^5",
"tan(x)^6", "tan(x)^7", "tan(x)^8", "tan(x)^9", "tan(x)^15" };
vector<string> LOG{ "log(x)", "log(x)^2", "log(x)^3", "log(x)^4", "log(x)^5",
"log(x)^6", "log(x)^7", "log(x)^8", "log(x)^9", "log(x)^15" };

void GenerateFormula(string &s, int ComplexityTest) {
	int Fun = rand() % 4;
	int NFun = rand() % 9;

	switch (Fun)
	{
	case 0:
		if (ComplexityTest == 2 && NFun > 5)
			NFun = rand() % 4;
		s += X[NFun];
		break;
	case 1:
		s += SIN[NFun];
		break;
	case 2:
		s += COS[NFun];
		break;
	case 3:
		s += TAN[NFun];
		break;
	case 4:
		s += LOG[NFun];
		break;
	}
}

void GenerateOper(string &s) {
	int Oper = rand() % 2;

	switch (Oper) {
	case 0:
		s += "+";
		break;
	case 1:
		s += "-";
		break;
	case 2:
		s += "*";
		break;
	}
}

void GeneratorIntegral(ofstream &out, int ComplexityTest) {
	Data d;
	for (int i = 0; i < 3; i++) {
		GenerateFormula(d.func, ComplexityTest);
		if (i < 2)
			GenerateOper(d.func);
	}
	if (ComplexityTest == 2) {
		d.lb = 100 + rand() % 900;
		d.ub = (d.lb + 5) + rand() % 1000;
		d.nspl = 1000 + rand() % 10000;
	}
	else {
		d.lb = 1 + rand() % 90;
		d.ub = (d.lb + 5) + rand() % 100;
		d.nspl = 100 + rand() % 1000;
	}
	d.Write(out);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	ofstream f;

	if (argc == 3) {
		string NameF = argv[1];
		int ComplexityTest = atoi(argv[2]);
		Data d;
		f.open(NameF, ios::binary | ios::out | ios::trunc);
		if (!f.is_open())
			cout << "Do no open file" << endl;
		else 
			GeneratorIntegral(f, ComplexityTest);
	}
	else {
		cout << "Invalid number of arguments" << endl;
		return 1;
	}

	f.close();
	return 0;
}

