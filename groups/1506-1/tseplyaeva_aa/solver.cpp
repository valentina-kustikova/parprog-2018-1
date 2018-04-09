#include "conio.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define ERROR_OPEN_FILE -3


#include <string>
using namespace std;



void show(double* a, int n){
	for (int i = 0; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}

void shellsort(double*a, int n){
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

	double x;
	int gap;
	int j;
	for (k = 0; k < m; k++) { // seq
		gap = seq[k];
		for (int i = gap; i < n; ++i) {
			x = a[i];
			for (j = i - gap; (x < a[j]) && (j >= 0); j = j - gap)
				a[j + gap] = a[j];
			a[j + gap] = x;
			//show(a,n);
		}
	}

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
	//fseek(in, 0, SEEK_SET);
	fread(a, sizeof(double), n, in);
	fclose(in);
	////////////////////////////////////////////////////////
	//	for (int i = 0; i < n; i++){
	//		a[i] = rand() % 50;
	//	}


	//show(a, n);
	double start = clock();

	shellsort(a, n);

	double end = clock();
	//show(a, n);
	double seconds = (end - start) / CLOCKS_PER_SEC / 1000;

	// time at the end of file 
	// file for a sorted array
	freopen("sorted_array.in", "wb", stdout);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(a, sizeof(*a), n, stdout);
	fwrite(&seconds, sizeof(seconds), 1, stdout);

	delete[] a;
	return 0;
}