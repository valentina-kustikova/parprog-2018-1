#include "../../include/solution.h"
#include "../../include/dataStruct.h"
#include "../../include/resultStruct.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <chrono>
using namespace std;

void TakeRes(ifstream& in, ofstream& out) {
	Data d;
	ResultOut r;
	double res = 0.0;
	while (1) {
		d.Read(in);
		if (in.eof()) break;
		auto begin = std::chrono::steady_clock::now(); // начальное время
		res = Rect_Meth_(d.lb, d.ub, d.nspl, d.func);
		auto end = std::chrono::steady_clock::now(); // конечное время
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin); // искомое время
		cout << "Result: " << res << endl;
		cout << "Time = " << (double)elapsed_ms.count() << " ms\n" << endl;
		r.result = res;
		r.time = (double)elapsed_ms.count();
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

		if (!Res.is_open() || !Test.is_open())
			cout << "Do no open file" << endl;
		else
			TakeRes(Test, Res);
	}
	else {
		cout << "Invalid count of arguments" << endl;
		return 1;
	}

	Res.close();
	Test.close();

	return 0;
}