#include "conio.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <omp.h>

#define ERROR_OPEN_FILE -3


#include <string>
using namespace std;



void show(double* a, int st, int n){
	for (int i = st; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}

void shellsort(double*a, int st, int n){

	int size_b = n - st;
	double* b = new double[size_b];

	for (int i = 0; i < size_b; i++){
		b[i] = a[i + st];
	}

	vector<int> seq;
	int k = 0;
	int i = 0;
	int lvl = 0;

	while (k == 0){
		int p = 0;
		int q = 0;
		int powq = 1;
		for (p = lvl; p >= 0; p--){
			i = pow(2, p)*powq;
			powq *= 3;
			if (i <= (n / 2)){ seq.push_back(i); }
			else{
				k++;
				break;
			}
		}
		lvl++;
	}
	int m = seq.size();
	reverse(seq.begin(), seq.end());

	//show(b, 0, size_b);

	double x;
	int gap;
	int j;
	for (k = 0; k < m; k++) { // seq
		gap = seq[k];
		for (int i = gap; i < size_b; ++i) {
			x = b[i];
			for (j = i - gap; (x < b[j]) && (j >= 0); j = j - gap)
				b[j + gap] = b[j];
			b[j + gap] = x;

		}
	}

	for (int i = 0; i < size_b; i++){
		a[i + st] = b[i];
	}

	delete[] b;
	//show(b, 0, size_b);

}


int main(int argc, char* argv[]){

	int n;
	double* a;
	FILE *in;
	// read from abinary file	
	if (argc > 2){
		in = fopen(argv[1], "rb"); // array.in
		if (!in) {
			return 0;
		}
		freopen(argv[2], "wb", stdout); // sorted_array.in

	}
	else{
		in = fopen("array.in", "rb"); // array.in
		if (!in) {
			return 0;
		}
		freopen("sorted_array.in", "wb", stdout); // sorted_array.in
	}

	//read length of array
	fread(&n, sizeof(size_t), 1, in);
	//initializing 
	a = new double[n];
	// read array
	
	fread(a, sizeof(double), n, in);
	fclose(in);
	
	double start = omp_get_wtime();

	shellsort(a,0, n);

	double end = omp_get_wtime();
	//show(a, n);
	double seconds = end - start;

	// time at the end of file 
	// file for a sorted array
	freopen("sorted_array.in", "wb", stdout);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(a, sizeof(*a), n, stdout);
	fwrite(&seconds, sizeof(seconds), 1, stdout);

	delete[] a;
	return 0;
}