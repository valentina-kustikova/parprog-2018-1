#include "../../include/dataStruct.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

void Serializer(ifstream& in, ofstream& out) {
	Data d;
	while (in.good()) {
		in >> d.func >> d.lb >> d.ub >> d.nspl;
		d.Write(out);
	}
}

int main(int argc, char* argv[])
{
	ofstream BIN;
	ifstream TXT;
	if (argc == 3) {
		string NameBIN = argv[1];
		string NameTXT = argv[2];
		BIN.open(NameBIN, ios::binary | ios::app);
		TXT.open(NameTXT);
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		BIN.open("data.bin", ios::binary);
		TXT.open("data.txt");
	}

	if (!BIN.is_open() || !TXT.is_open())
		cout << "Do not open file" << endl;
	else
		Serializer(TXT, BIN);

	BIN.close();
	TXT.close();

	return 0;
}