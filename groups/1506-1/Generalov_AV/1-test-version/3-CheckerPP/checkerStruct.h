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
		os.write((char*)&len, sizeof(len));
		os.write((char*)Res.c_str(), len);
	}

	void Read(istream& in) {
		size_t len = 0;
		char * buf = new char[len];
		in.read(buf, len);
		Res = buf;
		delete[] buf;
	}
};