#include "../../include/solution.h"
#include "../../include/dataStruct.h"
#include "../../include/resultStruct.h"

#include <iostream>
#include <string>
#include <istream>
#include <fstream>
using namespace std;

#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_reduce.h"
#include "tbb/tick_count.h"
using namespace tbb;

class integrTBB {
private:
	string func;
	double res;

public:
	explicit integrTBB(string _func) : func(_func), res(0) {}

	integrTBB(const integrTBB& _int, split) : func(_int.func), res(0) {}

	void operator()(blocked_range<double>& r) {
		double begin = r.begin(),
			end = r.end(),
			grain = r.grainsize();
		res += Rect_Meth_(begin, end, grain, func);
	}

	void join(const integrTBB& _int) {
		res += _int.res;
	}

	double getRes() {
		return res;
	}
};

void TakeRes(ifstream& in, ostream& out, int NumThreads) {
	Data d;
	ResultOut r;
	d.Read(in);
	string Func = d.func;
	double lowBound = d.lb;
	double uppBound = d.ub;
	double numSplit = d.nspl;

	integrTBB task(Func);
	tick_count start = tick_count::now();
	parallel_reduce(blocked_range<double>(lowBound, uppBound, numSplit / NumThreads), task);
	tick_count finish = tick_count::now();

	r.result = task.getRes();
	r.time = (finish - start).seconds();

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