#include "resultStruct.h"
#include "checkerStruct.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
using namespace std;

void WriteMessage(ostream& os, string str) {
	size_t len = str.length() + 1;
	os.write((char*)&len, sizeof(len)); // Запись длины
	os.write((char*)str.c_str(), len);  // Запись данных
}

void TakeCheckRes(ofstream& Check, ifstream& Seq, ifstream& Ref) {
	while (1) {
		ResultOut DataRef;
		ResultOut DataSeq;
		DataRef.Read(Ref);
		DataSeq.Read(Seq);
		if (Ref.eof()) break;
		else {
			if (abs(DataSeq.time - DataRef.time)*abs(DataSeq.time - DataRef.time) < 1)
				WriteMessage(Check, "The solution is correct");
			else 
				WriteMessage(Check, "The solution is uncorrect");
		}
	}
}

int main(int argc, char* argv[])
{
	ofstream CheckRes;		//Write
	ifstream SeqRes;		//Read
	ifstream RefRes;

	if (argc == 4) {
		string CheckResName = argv[1];
		string SeqResName = argv[2];
		string RefResName = argv[3];

		CheckRes.open(CheckResName, ios::binary, ios::trunc);
		SeqRes.open(SeqResName, ios::binary);
		RefRes.open(RefResName, ios::binary);
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		CheckRes.open("CheckRes.dat", ios::binary);
		SeqRes.open("SeqRes.ans", ios::binary);
		RefRes.open("RefRes.ans", ios::binary);
	}

	if (!CheckRes.is_open() || !SeqRes.is_open() || !RefRes.is_open())
		cout << "Do not open file" << endl;
	else
		TakeCheckRes(CheckRes, SeqRes, RefRes);

	CheckRes.close();
	SeqRes.close();
	RefRes.close();
	return 0;
}

