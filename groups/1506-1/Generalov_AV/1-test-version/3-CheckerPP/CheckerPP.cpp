#include "../../include/resultStruct.h"
#include "../../include/checkerStruct.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
using namespace std;

// Используется для взаимодействия с тестирующей системой 
//////////////////////////////////////////////////////////////////////////////////////////// 
/* // Checker может устанавливать вот эти три вердикта: 
AC = Accepted = Решение выдаёт корректный результат на данном тесте 
WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте 
PE = Presentation Error = Ошибка формата выходных данных 

// Остальные вердикты checker не может устанавливать 
NO = No verdict = Вердикт отсутствует 
CE = Compilation Error = Ошибка компиляции 
ML = Memory Limit Exceeded = Превышено ограничение по памяти 
TL = Time Limit Exceeded = Превышено ограничение по времени работы 
RE = Runtime Error = Ошибка времени исполнения программы 
IL = Idle Limit Exceeded = Превышено время простоя (бездействия) программы 
DE = Deadly Error = Ошибка тестирующей системы */ 

//enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE }; 

class result {
private: 
	ofstream res;
public:
	void Open(string str) {
		if (str == "") {
			res.open("CheckRes111.dat", ios::binary, ios::trunc);
		}
		else
			res.open(str, ios::binary, ios::trunc);
	}

	void write_verdict(verdict v)	{ 
		res.write((char*)&v, sizeof(v));
	}

	void WriteMessage(string str) {
		size_t len = str.length() + 1;
		res.write((char*)&len, sizeof(len)); // Запись длины
		res.write((char*)str.c_str(), len);  // Запись данных
	}

	void Close() {
		res.close();
	}
} checker_result;



void TakeCheckRes(string CheckResName, ifstream& Seq, ifstream& Ref) {
	checker_result.Open(CheckResName);
	//while (1) {
		ResultOut DataRef;
		ResultOut DataSeq;
		DataRef.Read(Ref);
		DataSeq.Read(Seq);
		/*if (Seq.eof()) break;
		else {*/
			if (abs(DataSeq.result - DataRef.result)*abs(DataSeq.result - DataRef.result) < 1) {
				checker_result.WriteMessage("The solution is correct");
			}				
			else {
				checker_result.WriteMessage("The solution is uncorrect");
			}
				
		//}
	//}
	checker_result.Close();
}

int main(int argc, char* argv[])
{
	string CheckResName = "";
	ofstream CheckRes;		//Write
	ifstream SeqRes;		//Read
	ifstream RefRes;

	if (argc == 4) {
		CheckResName = argv[1];
		string SeqResName = argv[2];
		string RefResName = argv[3];

		SeqRes.open(SeqResName, ios::binary);
		RefRes.open(RefResName, ios::binary);
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		SeqRes.open("SeqRes.ans", ios::binary);
		RefRes.open("RefRes.ans", ios::binary);
	}

	if (!SeqRes.is_open() || !RefRes.is_open())
		cout << "Do not open file" << endl;
	else
		TakeCheckRes(CheckResName, SeqRes, RefRes);

	SeqRes.close();
	RefRes.close();
	return 0;
}

