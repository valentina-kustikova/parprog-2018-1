
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//bin txt

void deserializer(char* txt, char* bin, char* time) {
	


	freopen(bin, "rb", stdin);
	int n;
	// Считываем размерность 
	fread(&n, sizeof(n), 1, stdin);
	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);

	int m = (int)time;

	double ti;
	if (m != 0){
		fread(&ti, sizeof(ti), 1, stdin);
	}
	
	//FILE *file;
	//if ((file = fopen(txt, "w")) == NULL) {
	//	printf("Невозможно открыть файл\n");
	//	exit(1);
	//};

	//for (int i = 0; i<n; i++) {
	//	//fprintf(file, "%I32", arr[i]);
	//	fwrite(&arr[i], sizeof(int), 1, file);
	//};
	//fclose(file);

	ofstream out(txt, ios::app);
	for (int i = 0; i < n; i++){
		out << arr[i] <<"  ";
	}
	if (m != 0){
		out << ti;
	}
	out.close();
	delete[] arr;

}

int main(int argc, char* argv[])
{
	if (argc < 4) {
		return 1;
	}

	char* bin = argv[1];
	char* txt = argv[2];
	char* time = argv[3];



	deserializer(txt, bin, time);


	return 0;
}