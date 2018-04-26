#include <iostream>
#include <istream>
#include <fstream>
using namespace std;

struct ResultOut {
	double time;
	double result;

public:
	void Show() {
		cout << "Lead time: " << time << endl;
		cout << "Result: " << result << endl << endl;
	}

	void Write(ostream& os) {
		os.write((char*)&time, sizeof(time));
		os.write((char*)&result, sizeof(result));
	}

	void Read(istream& in) {
		in.read((char*)&time, sizeof(time));
		in.read((char*)&result, sizeof(result));
	}
};