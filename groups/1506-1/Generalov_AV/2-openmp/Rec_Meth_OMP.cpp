#include "../include/solution.h"
#include "../include/dataStruct.h"
#include "../include/resultStruct.h"
#include <omp.h>
#include <iostream>
#include <istream>
#include <fstream>

using namespace std;

void TakeRes(ifstream& in, ostream& out, int NumThreads) {
	Data d;
	ResultOut r;
	double res = 0.0;
	d.Read(in);
	string Func = d.func;
	double lowBound = d.lb;
	double uppBound = d.ub;
	double numSplit = d.nspl;

	double start = omp_get_wtime();
#pragma omp parallel shared(res, NumThreads, Func, lowBound, uppBound, numSplit) num_threads(NumThreads)
	{
		int NumberOfThread = omp_get_num_threads();
		int tID = omp_get_thread_num();
		double step = (d.ub - d.lb) / (double)NumThreads;

		double loc_lb = d.lb + step * (double)tID;
		double loc_ub = loc_lb + step;

		res += Rect_Meth_(loc_lb, loc_ub, d.nspl / (double)NumThreads, d.func);
	}
	double finishTime = omp_get_wtime();

	r.result = res;
	r.time = finishTime - start;

	r.Write(out);	
}

int main(int argc, char* argv[]) {
	ofstream Res;		//Write
	ifstream Test;		//Read
	int NumThreads = 0;
	if (argc == 4) {
		string ResName = argv[1];
		string TestName = argv[2];
		NumThreads = atoi(argv[3]);

		Res.open(ResName, ios::binary | ios::trunc);
		Test.open(TestName, ios::binary);		
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		Res.open("res.ans", ios::binary, ios::trunc);
		Test.open("data.dat", ios::binary);
		NumThreads = 4;
	}

	if (!Res.is_open() || !Test.is_open())
		cout << "Do no open file" << endl;
	else
		TakeRes(Test, Res, NumThreads);

	Res.close();
	Test.close();	

	return 0;
}
