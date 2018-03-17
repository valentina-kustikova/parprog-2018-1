#include <iostream>
#include <string>
#include <istream>
#include <fstream>
using namespace std;

struct checkerOut {
	string Res;

public:
	void Show() {
		cout << Res << endl;
	}

	void Write(ostream& os) {
		size_t len = Res.length() + 1;
		os.write((char*)&len, sizeof(len)); // Запись длины
		os.write((char*)Res.c_str(), len);  // Запись данных
	}

	

	void Read(istream& in) {
		size_t len = 0;                         // Переменная для длины
		in.read((char*)&len, sizeof(len));  // Чтение длины записанной строки
											/*if (len == 0)
											return;*/
		char * buf = new char[len];         // Выделение буфера для чтения
		in.read(buf, len);                   // Чтение (с нулевым байтом)
		Res = buf;
	}
};