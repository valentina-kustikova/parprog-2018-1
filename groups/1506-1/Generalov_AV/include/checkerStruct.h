#include <iostream>
#include <string>
#include <istream>
#include <fstream>
using namespace std;

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

struct checkerOut {
	string Res;

public:
	void Show() {
		cout << Res << endl;
	}

	void Write(ostream& os) {
		os.write((char*)Res.c_str(), Res.length() + 1);
	}

	void Read(ifstream& in) {
		size_t len = 0;
		in.read((char*)&len, sizeof(len));
		char * buf = new char[len];
		in.read(buf, len);
		Res = buf;
		delete[] buf;
	}
};