#include "dataStruct.h"
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

		if (!BIN.is_open() || !TXT.is_open())
			cout << "Do not open file" << endl;
		else
			Serializer(TXT, BIN);
	}
	else {
		cout << "Invalid count of arguments" << endl;
		return 1;
	}

	BIN.close();
	TXT.close();

	return 0;
}