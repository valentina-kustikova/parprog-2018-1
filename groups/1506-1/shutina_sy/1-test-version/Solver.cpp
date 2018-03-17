// Shutina_14.cpp: определяет точку входа для консольного приложения.
//

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

void Auto_input(double* arr, int n) {
	srand((double)time(NULL));
	for (int i = 0; i < n; i++) {
		arr[i] = (double)(rand() % 20001) / 100;
	}
}

void Keyboard_input(double* arr, int n) {
	cout << "Enter of the array: " << endl;
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
}

void Sort(double* array_in, double* array_out, int byteNum, int n) {
	unsigned char* mas = (unsigned char*)array_in;
	int counter[256];
	int tem;
	memset(counter, 0, sizeof(int) * 256);
	for (int i = 0; i<n; i++)
		counter[mas[8 * i + byteNum]]++;
	int j = 0;
	for (; j<256; j++) {
		if (counter[j] != 0)
			break;
	}
	tem = counter[j];
	counter[j] = 0;
	j++;
	for (; j<256; j++) {
		int b = counter[j];
		counter[j] = tem;
		tem += b;
	}
	for (int i = 0; i<n; i++) {
		array_out[counter[mas[8 * i + byteNum]]] = array_in[i];
		counter[mas[8 * i + byteNum]]++;
	}
}
void Sort_Double(double* array_in, int n) {
	double* array_out = new double[n];
	Sort(array_in, array_out, 0, n);
	Sort(array_out, array_in, 1, n);
	Sort(array_in, array_out, 2, n);
	Sort(array_out, array_in, 3, n);
	Sort(array_in, array_out, 4, n);
	Sort(array_out, array_in, 5, n);
	Sort(array_in, array_out, 6, n);
	Sort(array_out, array_in, 7, n);
	delete[] array_out;
}

int main() {

	freopen("20", "rb", stdin);
	freopen("20.ans", "wb", stdout);

	int n;
	fread(&n, sizeof(n), 1, stdin);

	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);

	double start_time = clock();

	Sort_Double(arr, n);

	double end_time = clock();

	double time = end_time - start_time;
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(arr, sizeof(*arr), n, stdout);
	/*fwrite(&time, sizeof(time), 1, stdout);*/
	return 0;
}

