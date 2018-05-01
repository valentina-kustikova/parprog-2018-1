#include "../../include/checkerStruct.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

void Deserializer(ifstream& in, ofstream& out) {
	checkerOut d;
	while (1) {
		d.Read(in);
		if (in.eof()) break;
		else 
			d.Write(out);
	}
}

int main(int argc, char* argv[])
{
	ifstream BIN;
	ofstream TXT;
	if (argc == 3) {
		string NameBIN = argv[1];
		string NameTXT = argv[2];
		BIN.open(NameBIN, ios::binary);
		TXT.open(NameTXT, ios::trunc);
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		BIN.open("data.bin", ios::binary);
		TXT.open("data.txt");
	}

	if (!BIN.is_open() || !TXT.is_open())
		cout << "Do not open file" << endl;
	else
		Deserializer(BIN, TXT);

	BIN.close();
	TXT.close();

	return 0;
}
