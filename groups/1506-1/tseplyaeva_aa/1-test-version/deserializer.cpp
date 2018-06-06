
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//bin txt

void deserializer(char* txt, char* bin) {
	
	freopen(bin, "rb", stdin);
	int n;
	// Считываем размерность 
	fread(&n, sizeof(n), 1, stdin);
	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);

	double time;
	fread(&time, sizeof(time), 1, stdin);

	//double ti;
	//if (m != 0){
	//	fread(&ti, sizeof(ti), 1, stdin);
	//}
	
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
	out << time;
	

	out.close();
	delete[] arr;

}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		return 1;
	}

	char* bin = argv[1];
	char* txt = argv[2];
	
	deserializer(txt, bin);


	return 0;
}