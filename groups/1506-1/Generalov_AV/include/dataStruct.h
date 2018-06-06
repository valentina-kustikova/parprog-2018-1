#include <string>
#include <iostream>
#include <fstream>
using namespace std;

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

		size_t func_length = func.length() + 1;
		os.write((char*)&func_length, sizeof(func_length)); // Запись длины
		os.write((char*)func.c_str(), func_length);  // Запись данных
		os.write((char*)&lb, sizeof(lb));
		os.write((char*)&ub, sizeof(ub));
		os.write((char*)&nspl, sizeof(nspl));
	}

	void Read(istream& in) {

		size_t len = 0;                         
		in.read((char*)&len, sizeof(len));  
		char * buf = new char[len];         
		in.read(buf, len);                   
		func = buf;
		in.read((char*)&lb, sizeof(lb));
		in.read((char*)&ub, sizeof(ub));
		in.read((char*)&nspl, sizeof(nspl));
		delete[] buf;
	}
};