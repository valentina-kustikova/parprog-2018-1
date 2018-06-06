#include "../include/solution.h"
#include "../include/resultStruct.h"

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

struct Data {
	string func;
	int lb;
	int ub;
	int nspl;

public:
	void Show() {
		cout << "Function = " << func << endl;
		cout << "Lower bound = " << lb << endl;
		cout << "Uper bound = " << ub << endl;
		cout << "Number of splitting = " << nspl << endl;
	}

	void Get() {
		cout << "Input function" << endl;
		cin >> func;
		cout << "Input Lower bound" << endl;
		cin >> lb;
		cout << "Input Uper bound" << endl;
		cin >> ub;
		cout << "Input Number of splitting" << endl;
		cin >> nspl;
	}

	void Write(ofstream& os) {

		size_t func_length = func.size();
		os.write(reinterpret_cast<char *>(&func_length), sizeof(int));
		os.write(func.c_str(), func_length);
		os.write((char*)&lb, sizeof(lb));
		os.write((char*)&ub, sizeof(ub));
		os.write((char*)&nspl, sizeof(nspl));
	}

	void Read(ifstream& in) {

		size_t len = 0;
		in.read(reinterpret_cast<char *>(&len), sizeof(int));
		char * buf = new char[len];
		in.read(buf, len);
		func = "";
		func.append(buf, len);
		in.read((char*)&lb, sizeof(lb));
		in.read((char*)&ub, sizeof(ub));
		in.read((char*)&nspl, sizeof(nspl));
		delete[] buf;
	}
};

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
	d.Show();
	string Func = d.func;
	double lowBound = d.lb;
	double uppBound = d.ub;
	double numSplit = d.nspl;

	integrTBB task(Func);
	tick_count start = tick_count::now();
	parallel_reduce(blocked_range<double>(lowBound, uppBound, numSplit / (double)NumThreads), task);
	tick_count finish = tick_count::now();

	r.result = task.getRes();
	r.time = (finish - start).seconds();
	r.Show();
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
		Test.open(TestName, ios_base::binary);
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		Res.open("res.ans", ios::binary, ios::trunc);
		Test.open("data.dat", ios_base::binary);
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