#include "../../include/solution.h"
#include "../../include/dataStruct.h"
#include "../../include/resultStruct.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <omp.h>
using namespace std;

void TakeRes(ifstream& in, ofstream& out) {
	Data d;
	ResultOut r;
	double res = 0.0;
	while (1) {
		d.Read(in);
		d.Show();
		if (in.eof()) break;
		double start = omp_get_wtime();
		res = Rect_Meth_(d.lb, d.ub, d.nspl, d.func);
		double finishTime = omp_get_wtime();
		r.result = res;
		r.time = finishTime - start;
		r.Write(out);
	}
}

int main(int argc, char * argv[])
{
	ofstream Res;		//Write
	ifstream Test;		//Read
	if (argc == 3) {
		string ResName = argv[1];
		string TestName = argv[2];
		Res.open(ResName, ios::binary | ios::trunc);
		Test.open(TestName, ios::binary);		
	}
	else {
		cout << "Invalid count of arguments" << endl;
		Res.open("res.ans", ios::binary, ios::trunc);
		Test.open("data.dat", ios::binary);
	}

	if (!Res.is_open() || !Test.is_open())
		cout << "Do no open file" << endl;
	else
		TakeRes(Test, Res);

	Res.close();
	Test.close();

	return 0;
}