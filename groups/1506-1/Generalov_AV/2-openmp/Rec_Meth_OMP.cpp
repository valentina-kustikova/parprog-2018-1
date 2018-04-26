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
	omp_set_num_threads(omp_get_max_threads());
	double start = omp_get_wtime();
#pragma omp parallel 
	{			
		int NumberOfThread = omp_get_num_threads();
		int tID = omp_get_thread_num();

		double step = (d.ub - d.lb) / NumberOfThread;

		double loc_lb = d.lb + step * tID;
		double loc_ub = loc_lb + step;

		double loc_res = 0;
		loc_res = Rect_Meth_(loc_lb, loc_ub, d.nspl / NumThreads, d.func);
#pragma omp critical
		{
			res += loc_res;
		}
#pragma omp barrier
	}
	double finishTime = omp_get_wtime();

	r.result = res;
	r.time = finishTime - start;
	r.Write(out);	
}

int main(int argc, char* argv[]) {
	ofstream Res;		//Write
	ifstream Test;		//Read
	if (argc == 4) {
		string ResName = argv[1];
		string TestName = argv[2];
		int NumThreads = 4;

		Res.open(ResName, ios::binary | ios::trunc);
		Test.open(TestName, ios::binary);

		if (!Res.is_open() || !Test.is_open())
			cout << "Do no open file" << endl;
		else
			TakeRes(Test, Res, NumThreads);
	}
	else {
		cout << "Invalid count of arguments" << endl;
		return 1;
	}

	Res.close();
	Test.close();	

	return 0;
}
