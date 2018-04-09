#include <fstream>
#include <iostream>
#include <string>
using namespace std;


void show(double* a, int n){
	for (int i = 0; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}
// txt bin
void serializer(char* txt, char* bin) {

		int n;
		ifstream fin(txt);
		fin >> n;

		double* arr = new double[n];

		for (int i = 0; i < n; i++){
			fin >> arr[i];
		}

	//	show(arr, n);

		freopen(bin, "wb", stdout);
		fwrite(&n, sizeof(n), 1, stdout);
		fwrite(arr, sizeof(*arr), n, stdout);

		delete[] arr;
}
int main(int argc, char* argv[])
{
	if (argc < 3) {
		return 1;
	}

	char* txt = argv[1];
	char* bin = argv[2];


	serializer(txt, bin);

	
	return 0;
}